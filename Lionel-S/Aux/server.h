#ifndef SERVER_H
#define SERVER_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semaphore_v2.h"
#include "../../Aux/mssg.h"
#include "../../Aux/conexion.h"
#include "../../Aux/checksum.h"
#include "lista.h"
#include "file.h"
#include "cuesmensajes.h"

extern Lista l;
extern Carpeta carpeta;
extern pthread_mutex_t mtxListaPuertos;
extern pthread_mutex_t mtxCarpeta;

typedef struct{
    int puerto;
    int fd_socket;
    pthread_t thread_id_accept;

    int id_cua;
    int fd_socket_cliente;
}ThreadData;

//PRE: puerto_c inicializado y con un valor entero en ASCII.
//POST: retorna el valor en int de la cadena de caracteres.
int SERVER_getPuerto(char * puerto_c);

//PRE: Se tiene que haber hecho hasta el connect de la conexión mediante sockets.
//POST: --
void * SERVER_McGruderDedicado(void * arg);

//PRE: Se tiene que haber hecho hasta el connect de la conexión mediante sockets.
//POST:
void * SERVER_McTavishDedicado(void * arg);

#endif
