#ifndef CONEXION_H
#define CONEXION_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <dirent.h>
#include <regex.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "mssg.h"

#define NUM_CONEXIONES 5 //numero de conexiones en espera del servidor
#define TIPO1 0x01
#define TIPO2 0x02
#define TIPO3 0x03
#define TIPO4 0x04
#define TIPO5 0x05
#define TIPONULL 0x10


typedef struct{
    char type;
    char * header; // como minimo tiene "[]"
    uint16_t length; //tamaño del campo data
    unsigned char * data; //datos a enviar
}Trama;

typedef struct{
    char type;
    char * header; // como minimo tiene "[]"
    uint16_t length; //tamaño del campo data
    uint64_t data; //datos a enviar
}TramaImagen;



//Devuelve -1 si hay problemas o > 0 con el file descriptor del socket
int CONEXION_connectCliente(char * ip, int puerto);

//Devuelve -1 si hay problemas o > 0 con el file descriptor del socket
int CONEXION_conectarServer(int puerto);


//Devuelve -1 si hay problemas o > 0 con el file descriptor del cliente
int CONEXION_aceptarServer(int socket_fd);


//PRE: parametros correctamente inicializados y asignados
//POST: Devuelve >= 0 si se ha enviado correctamente la trama
int CONEXION_enviaTramaTexto(int socket_fd, char type, char * header, char * data, uint16_t tamanyo);

//Leer el header de una TRAMA.
//Retorna null si no se puede hacer el malloc
char * INPUT_leerBufferSocket(int fd);

//PRE: conexión con el servidor/cliente hecha.
//POST: Devuelve una TramaImagen con los campos asignados.
TramaImagen CONEXION_recibeTramaImagen(int socket_fd);

//PRE: parametros correctamente inicializados y asignados
//POST: Devuelve >= 0 si se ha enviado correctamente la trama
int CONEXION_enviaTramaImagen(int socket_fd, char type, char * header, unsigned char * data, uint16_t bytes_a_enviar);

//PRE: conexión con el servidor/cliente hecha.
//POST: Devuelve una trama con los campos asignados.
Trama CONEXION_recibeTrama(int socket_fd);

//PRE: parametros correctamente inicializados y asignados
//POST: Devuelve >= 0 si se ha enviado correctamente la trama
int CONEXION_enviaTrama(int socket_fd, char type, char * header,char * data);


#endif
