#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <dirent.h>
#include <pthread.h>
#include <sys/wait.h>

#include "../Aux/input.h"
#include "../Aux/mssg.h"
#include "../Aux/conexion.h"
#include "../Aux/checksum.h"
#include "Aux/lista.h"
#include "Aux/file.h"
#include "Aux/paquita.h"
#include "Aux/cuesmensajes.h"
#include "Aux/server.h"

#define UNUSED(x) (void)(x)

pthread_t thread_id_Gruder, thread_id_Tavish;
ThreadData td_Gruder, td_Tavish;

Lista l;

semaphore semaforo_cua;
Carpeta carpeta;

pthread_mutex_t mtxListaPuertos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtxCarpeta = PTHREAD_MUTEX_INITIALIZER;

//Datos cues
int id_cua;

//Pid del fork
int pid;
int status;

void reconfSIGINT();

int main(int argc, char const *argv[]) {

    if (argc < 2) {
        MSSG_errorParam(NUM_ARGC);
        //raise(SIGINT);
        MSSG_ejemploLionel();
        exit(-1);
        pause();
    }
    int fd_config;
    int paquita;
    int estado_threadGruder, estado_threadTavish;

    key_t clau;

    clau = ftok("main.c", 'M');
    if (clau == (key_t)-1) exit(-1);
    id_cua = msgget(clau, 0600 | IPC_CREAT);
    if (id_cua == -1) exit(-1);

    pid = fork();

    if (pid == -1){
        MSSG_errorPaquita();
        raise(SIGINT);
        pause();
    } else if(pid == 0){
        //hijo --> paquita
        PAQUITA_ejecutaPaquita(id_cua);
    }else{
        //Reconfiguramos signals
        signal(SIGINT, reconfSIGINT);
        //Verificar num de argumentos. Si son menos salimos

        //Crear lista de sockets del servidor dedicado de McGruder
        l = LISTA_crea();

        //Semforo y crea la estrucutra de datos para guardar los archivos
        SEM_constructor(&semaforo_cua);
        SEM_init(&semaforo_cua, 1);
        FILE_initCarpeta(&carpeta);

        //Abrir fichero de configuracion
        fd_config = open(argv[1], O_RDONLY);
        if (fd_config > 0){
            //leemos del archivo de configuracion
            //Leemos la ip
            INPUT_comeChars(fd_config, '\n');
            //Leemos los puertos
            td_Gruder.puerto = SERVER_getPuerto(INPUT_getTEXT(fd_config,'\n'));
            td_Tavish.puerto = SERVER_getPuerto(INPUT_getTEXT(fd_config,'\n'));
            close(fd_config);

            td_Gruder.id_cua = td_Tavish.id_cua = id_cua;
            MSSG_fileFound(STARTING, "Lionel");
            //creamos el thread para conexion de McGruders y McTavish comprobamos
            estado_threadGruder = pthread_create(&thread_id_Gruder,NULL,SERVER_McGruderDedicado,&td_Gruder);
            estado_threadTavish = pthread_create(&thread_id_Tavish,NULL,SERVER_McTavishDedicado,&td_Tavish);

            if (estado_threadGruder != 0 || estado_threadTavish != 0) {
                if (estado_threadGruder != 0) MSSG_error(THREAD_ERROR, "McGruder");
                else MSSG_error(THREAD_ERROR, "McTavish");
                raise(SIGINT);
                pause();
            }
            pthread_detach(thread_id_Gruder);
            pthread_detach(thread_id_Tavish);

            //Esperamos para mantener el servidor funcionando
            while(1) {
                pause();
            }
            wait(&paquita);
        }
    }

    return 0;
}



void reconfSIGINT(){

    if(pid != 0){//Verificar que el SIGINT le llegue a LIONEL
        unsigned int i;//Recorrer estrucutra y crear archivos
        for (i = 0; i < carpeta.num_archivos; i++) {
            if(strcmp(carpeta.archivos[i].tipo_archivo, "TXT") == 0) {
                FILE_escribirFichero(carpeta.archivos[i], 0);
            } else {
                FILE_escribirFichero(carpeta.archivos[i], 1);
            }
        }
        //Liberar recursos de los sockets
        LISTA_irInicio(&l);
        while (!LISTA_fin(l)){
            Element e = LISTA_consultar(l);
            close(e.socket_fd);
            LISTA_borrar(&l);
        }
        LISTA_destruye(&l);

        //Liberar recursos de los archivos que nos llegan
        FILE_destruyeCarpeta(&carpeta);

        pthread_mutex_destroy(&mtxListaPuertos);
        pthread_mutex_destroy(&mtxCarpeta);
        SEM_destructor(&semaforo_cua);

        //mandar señal de kill a Paquita
        kill(pid, SIGTERM);
        wait(&status);//esperamos al hijo
        msgctl(id_cua, IPC_RMID, NULL);//elimina cua

        raise(SIGKILL);
    }
}
