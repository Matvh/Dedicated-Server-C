#include "file.h"

#define TAMANYO_TRAMITA 1000
#define WAIT_TIME 50000000L


/**
 * Funcion que recibe un numero en caracteres y devuelve su entero.
 * Además se libera la memoria de la cadena de caracteres recibida.
 * @param  num_c cadena de caracteres del puerto
 * @return          valor del numero (entero)
 */
int FILE_getIntFromString(char * num_c){
    if(num_c != NULL){
        int num = atoi(num_c);
        free(num_c);
        return num;
    }
    return -1;
}

//funcion para obtener el num. de bytes de un archivo
off_t FILE_fileSize(const char * nombre_archivo) {
    struct stat st;

    char * path = malloc ((strlen(nombre_archivo)+8)*sizeof(char));
    sprintf(path, "%s%s",  carpeta, nombre_archivo);
    if (stat(path, &st) == 0) {
        free(path);
        return st.st_size;
    }
    free(path);
    return -1;
}

char * FILE_getCarpeta(const char * nom_param){
    char * carpeta;
    if (nom_param[strlen(nom_param)] != '/') {
        carpeta = malloc (sizeof(char)*(strlen(nom_param)+1));
        strcpy(carpeta, (char*) nom_param);
        strcat(carpeta, "/");
        carpeta[strlen(carpeta)] = '/';
    } else {
        carpeta = (char*) nom_param;
    }
    return carpeta;
}

void FILE_mirarArchivosPath() {
    int num_archivos;
    char date[15];
    Trama trama;
    long int tamanyo_archivo;

    MSSG_connection(WAITING);
    alarm(tiempo);
    pause();
        while (1){
            num_archivos = scandir (carpeta,&archivos, NULL, NULL);
            MSSG_connection(TESTING_FILES);

            if (archivos != NULL){ //quiere decir que hay archivos

                if (num_archivos == 2){
                    free (archivos[0]);
                    free (archivos[1]);
                    free (archivos);
                    archivos = NULL;
                    MSSG_connection(NO_FILES_FOUND);
                }else{
                    while (num_archivos--){

                        if (strcmp(archivos[num_archivos]->d_name, ".") != 0 && strcmp(archivos[num_archivos]->d_name, "..") != 0 ){

                            tamanyo_archivo = FILE_fileSize(archivos[num_archivos]->d_name);

                            strcpy(date, (char*) archivos[num_archivos]->d_name);
                            int tamano = sizeof(carpeta) + strlen(archivos[num_archivos]->d_name);
                            nombre_archivo = malloc (sizeof(char*) * (tamano+1));
                            if (nombre_archivo != NULL) {

                                strcpy(nombre_archivo, carpeta);
                                strcat(nombre_archivo, archivos[num_archivos]->d_name);
                                nombre_archivo[strlen(nombre_archivo)] = '\0';

                                if (strstr(archivos[num_archivos]->d_name, ".txt") != NULL ||
                                        strstr(archivos[num_archivos]->d_name, ".TXT") != NULL) {
                                            MSSG_fileFound(FILE, archivos[num_archivos]->d_name);
                                    // es un dato astronomico y se envia tal cual

                                    int status = FILE_enviarFicheroTexto(tamanyo_archivo, date, socket_fd,nombre_archivo);
                                    if (status >= 0) {
                                        MSSG_sendingFile(SENDING, archivos[num_archivos]->d_name);
                                        trama = CONEXION_recibeTrama(socket_fd);
                                        if (strcmp((char*)trama.header, "[CHECKOK]\0") == 0) {
                                            MSSG_connection(FILE_SENT);
                                            unlink(nombre_archivo);

                                        }
                                        if (trama.header != NULL) free(trama.header);
                                        if(trama.data != NULL) free(trama.data);
                                        free(nombre_archivo);
                                        nombre_archivo = NULL;
                                    }

                                } else if (strstr(archivos[num_archivos]->d_name, ".jpg") != NULL ||
                                        strstr(archivos[num_archivos]->d_name, ".JPG") != NULL){
                                            MSSG_fileFound(FILE, archivos[num_archivos]->d_name);
                                    // es una imagen y hay que enviarlo partido

                                    FILE_enviarFicheroImagen(tamanyo_archivo, date, socket_fd, nombre_archivo);
                                    trama = CONEXION_recibeTrama(socket_fd);
                                    if (strcmp(trama.header, "[CHECKOK]") == 0) {
                                        MSSG_connection(FILE_SENT);
                                        unlink(nombre_archivo);
                                    }
                                    if (trama.header != NULL) free(trama.header);
                                    if(trama.data != NULL) free(trama.data);
                                    free(nombre_archivo);
                                    nombre_archivo = NULL;

                                } else {
                                    if (nombre_archivo != NULL) {
                                        free(nombre_archivo);
                                        nombre_archivo = NULL;
                                    }
                                }

                            }
                        }
                        free (archivos[num_archivos]);
                    }
                    free (archivos);
                    archivos = NULL;
                }
            } else {
                MSSG_connection(NO_FILES_FOUND);
            }
            MSSG_connection(WAITING);
            //esperamos el tiempo deseado para scanear otra vez
            pause();
        }
}

/**
 * Función para enviar una imagen por el socket. Se envían TODAS las tramas
 * que hacen referencia a la imagen: METADATA, DATOS DE LAS IMAGENES Y CHECKSUM.
 * @param tamanyo_archivo Tamaño del archivo en bytes.
 * @param date            Fecha de creación de la imagen.
 * @param socket_fd       Fd del socket.
 * @param nombre_archivo  Nombre del archivo a enviar.
 */
void FILE_enviarFicheroImagen(long int tamanyo_archivo, char * date, int socket_fd,
                                char *nombre_archivo){
    char metadata[32];
    int fd_fichero;
    int status;
    long int bytes_leidos;
    uint16_t bytes_a_enviar;
    unsigned char tramita[TAMANYO_TRAMITA];
    sprintf(metadata, "%s&%ld&%s", "JPG",tamanyo_archivo ,date);
    status = CONEXION_enviaTrama(socket_fd, TIPO3, "[METADATA]\0", metadata);
    if (status < 0) {
        MSSG_errorLionel();
        raise(SIGINT);
    }
    fd_fichero = open(nombre_archivo,O_RDONLY);
    bytes_leidos = 0;
    char checksum[33];
    int conexion;

    while (bytes_leidos < tamanyo_archivo) {
        MSSG_mostrarProgressBar(bytes_leidos, tamanyo_archivo );
        nanosleep((const struct timespec[]){{0, WAIT_TIME}}, NULL);
        bytes_a_enviar = read(fd_fichero, tramita, sizeof(unsigned char)*TAMANYO_TRAMITA);
        conexion = CONEXION_enviaTramaImagen(socket_fd, TIPO3, "[]\0", tramita, bytes_a_enviar);
        if (conexion < 0) {
            MSSG_errorLionel();
            raise(SIGINT);
        }
        bytes_leidos += bytes_a_enviar;
    }

    MSSG_mostrarProgressBar100();

    unsigned char file [tamanyo_archivo];
    close(fd_fichero);
    fd_fichero = open(nombre_archivo,O_RDONLY);
    read(fd_fichero, file, sizeof(file));
    //printf("Nombre archivo: %s\n", nombre_archivo);
    //write(1, file, sizeof(char) * tamanyo_archivo );
    int fd[2];
    if(pipe(fd) == -1) exit(-1);
    switch (fork()) {
        case 0:
            //es el hijo y hace el execl
            dup2 (fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execl("/usr/bin/md5sum", "md5sum",(char *const) nombre_archivo, (char*const)NULL);
            exit(0);
        default:
            close(fd[1]);
            read(fd[0], checksum, sizeof(checksum));
            checksum[32] = '\0';
            wait(&status);
            //printf("Checksum: (%.*s)\n", num_bytes, checksum);
            CONEXION_enviaTrama(socket_fd, TIPO3,"[ENDFILE]", checksum);
            break;
    }
    //printf("Imagen: %hhx\n", file_data);

    close(fd_fichero);

}
/**
 * Función para enviar una archivo de texto por el socket. Se envían TODAS las tramas
 * que hacen referencia a la imagen: METADATA, DATO Y CHECKSUM.
 * @param tamanyo_archivo Tamaño del archivo en bytes.
 * @param date            Fecha de creación de la imagen.
 * @param socket_fd       Fd del socket.
 * @param nombre_archivo  Nombre del archivo a enviar.
 * @return                Devuelve >= 0 en el caso que se haya enviado correctamente
 *                        < 0 en caso contrario.
 */
int FILE_enviarFicheroTexto(long int tamanyo_archivo, char * date, int socket_fd,
                                char * nombre_archivo) {
    char metadata[32];
    char * file_data;
    int fd_fichero;
    int status = 0;
    char checksum[33];
    sprintf(metadata, "%s&%ld&%s", "TXT",tamanyo_archivo ,date);
    status = CONEXION_enviaTrama(socket_fd, TIPO3, "[METADATA]", metadata);
    if (status < 0) {
        MSSG_errorLionel();
        raise(SIGINT);
    }
    fd_fichero = open(nombre_archivo,O_RDONLY);
    if(tamanyo_archivo == 0) {

        file_data = malloc (sizeof(char));
        if (file_data != NULL){
            *file_data = ' ';
        }
    } else {
        file_data = malloc (tamanyo_archivo);
        if (file_data != NULL) {
            read(fd_fichero, file_data, tamanyo_archivo);
        }

    }

    int fd[2];
    if(pipe(fd) == -1) exit(-1);
    switch (fork()) {
        case 0:
            //es el hijo y hace el execl
            dup2 (fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execl("/usr/bin/md5sum", "md5sum",(char *const) nombre_archivo, (char*const)NULL);
            exit(0);
        default:
            close(fd[1]);
            read(fd[0], checksum, sizeof(checksum));
            checksum[32] = '\0';
            wait(&status);
            CONEXION_enviaTramaTexto(socket_fd, TIPO3, "[]", file_data, (uint16_t)tamanyo_archivo);
            MSSG_mostrarProgressBar100();
            CONEXION_enviaTrama(socket_fd, TIPO3,"[ENDFILE]", checksum);
            close(fd_fichero);
            free(file_data);
            break;
    }
    return status;
}
