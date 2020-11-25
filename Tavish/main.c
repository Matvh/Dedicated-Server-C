#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <pthread.h>
#include "../Aux/input.h"
#include "Aux/tavish.h"

char * nombre_cientifico;
char * ip;
int puerto_lionel;
int socket_fd;
int muerto;
Trama trama;

void reconfSIGINT() {
    if (!muerto) {
        CONEXION_enviaTrama(socket_fd, TIPO2,"[]", nombre_cientifico);
        trama = CONEXION_recibeTrama(socket_fd);
        if(strcmp(trama.header, "[CONOK]") == 0) {
            MSSG_disconnect("Disconnecting scientist ", nombre_cientifico);
            close(socket_fd);
            if (nombre_cientifico != NULL) free (nombre_cientifico);
            if (ip != NULL) free (ip);

            kill(getpid(), SIGKILL);
        }
    } else {
        close(socket_fd);
        if (nombre_cientifico != NULL) free (nombre_cientifico);
        if (ip != NULL) free (ip);
        kill(getpid(), SIGKILL);
    }
}


int main(int argc, char const *argv[]) {
    int fd_config;
    muerto = 0;
    nombre_cientifico = NULL;

    if (argc < 2) {
        MSSG_errorParam(NUM_ARGC);
        return -1;
    }
    signal(SIGINT, reconfSIGINT);
    fd_config = open(argv[1], O_RDONLY);
    if (fd_config > 0){

        nombre_cientifico = INPUT_getTEXT(fd_config, '\n');
        ip = INPUT_getTEXT(fd_config, '\n');
        puerto_lionel = TAVISH_getIntFromString(INPUT_getTEXT(fd_config, '\n'));
        close(fd_config);

        socket_fd = CONEXION_connectCliente(ip,puerto_lionel);
        if(socket_fd < 0) {
            MSSG_error(NO_CONEXION, "Lionel");
            return -1;
        }

        TAVISH_conectaLionel();
    }

    return 0;
}
