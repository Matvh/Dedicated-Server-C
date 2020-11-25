#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Aux/file.h"
#include "../Aux/input.h"
#include "../Aux/mssg.h"
#include "../Aux/conexion.h"


DIR * dir;
char * carpeta;
char * nombre_telescopio; //nombre del telecospio
struct dirent ** archivos; //directorio con los archivos
char * nombre_archivo; //nombre del archivo que se lee actualmente
int socket_fd;
int tiempo;

void newSigAlrm(){
    alarm(tiempo);
}
void reconfSIGINT();

int main(int argc, char const *argv[]) {

    int fd_config;
    int puerto;
    char * ip;

    if(argc < 3){
        MSSG_errorParam(NUM_ARGC);
        MSSG_ejemploMcGruder();
        return -1;
    }

    //argv[1] --> nombre del archivo que me pasan
    fd_config = open(argv[1], O_RDONLY);
    if (fd_config > 0){
        nombre_telescopio = INPUT_getTEXT(fd_config,'\n'); //leemos el nombre del telecospio
        tiempo = FILE_getIntFromString(INPUT_getTEXT(fd_config,'\n'));
        ip = INPUT_getTEXT(fd_config,'\n');
        puerto = FILE_getIntFromString(INPUT_getTEXT(fd_config,'\n'));
        close(fd_config); //cerramos el archivo

        MSSG_fileFound(STARTING, nombre_telescopio);
        MSSG_connection(CONNECTING_LIONEL);

        socket_fd = CONEXION_connectCliente(ip,puerto);
        if (socket_fd < 0) {
            MSSG_error(NO_CONEXION, "Lionel");
            return -1;
        }
        CONEXION_enviaTrama(socket_fd,TIPO1,"[]",nombre_telescopio);

        Trama trama;
        trama = CONEXION_recibeTrama(socket_fd);
        if(strcmp(trama.header, "[CONOK]\0") == 0) {
            MSSG_connection(CONNECTION_READY);
            //Liberar memoria porque no lo volvemos a usar
            free (ip);
            if (trama.header != NULL) free(trama.header);
            if (trama.data != NULL) free(trama.data);

            //Redefinicion CTRL C --> //cuando nos cierren el programa
            signal(SIGINT, reconfSIGINT);
            signal(SIGALRM, newSigAlrm);
            //Abrimos el directiorio
            carpeta = FILE_getCarpeta(argv[2]); //argv[2] nombre de la carpeta para buscar los files
            dir = opendir(carpeta);
            if (dir != NULL){
                //Vamos viendo si hay archivos
                FILE_mirarArchivosPath();
            } else {
                MSSG_error(NO_DIR, carpeta);
                raise(SIGINT);
            }
        } else {
            if (trama.header != NULL) free(trama.header);
            if (trama.data != NULL) free(trama.data);
        }
    }else{
        MSSG_error(NO_ARCHIVO, (char *) argv[1]);
    }
    return 0;
}

void reconfSIGINT(){

    if(nombre_telescopio != NULL) {
        MSSG_disconnect(DISCONNECT, nombre_telescopio);
        CONEXION_enviaTrama(socket_fd,TIPO2,"[]",nombre_telescopio);

        Trama trama = CONEXION_recibeTrama(socket_fd);
        free(nombre_telescopio);
        if (strcmp(trama.header,"[CONOK]") == 0) {
            if (trama.header != NULL) free(trama.header);
            if (trama.data != NULL) free(trama.data);
        }
    }
    if (carpeta != NULL) free(carpeta);
    if(nombre_archivo != NULL) free(nombre_archivo);
    if(archivos != NULL) free(archivos);
    if(dir != NULL) closedir(dir); //cerrar el directorio
    close(socket_fd);
    raise(SIGKILL);
}
