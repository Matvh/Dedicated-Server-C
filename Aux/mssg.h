#ifndef MSSG_H
#define MSSG_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#define NO_ARCHIVO "No se encontro el archivo de configuracion: "
#define NO_DIR "No se encontro el directorio: "

#define STARTING "Starting "
#define CONNECTING_LIONEL "Connecting to Lionel..."
#define CONNECTION_READY "Connection ready."
#define WAITING "Waiting..."
#define TESTING_FILES "Testing files..."
#define NO_FILES_FOUND "No files found."
#define FILE_SENT "File sent"
#define FILE "File: "
#define SENDING "Sending "
#define DISCONNECT "\nDisconnecting "
#define NUM_ARGC "Faltan parametros\n"

#define THREAD_ERROR "Error al crear el thread "
#define NO_CONEXION "No se pudo establecer la conexion con "

void MSSG_connection(char * mssg);
void MSSG_connectionTelescope(char * mssg, char * telescope);
void MSSG_novaConexio(char * ip, u_int16_t port);
void MSSG_nuevaConexionLionel(char * connection_name);
void MSSG_petition1Tavish();
void MSSG_petition2Tavish();
void MSSG_disconnect(char * mssg, char * nombre_telescopio);
void MSSG_fileFound(char * mssg, char * fileName);
void MSSG_sendingFile(char * mssg, char * fileName);
//actual -> cantidad de datos enviados/recibidos actuales
//total -> cantidad total de datos a enviar/recibir
void MSSG_mostrarProgressBar(long int actual, long int total);
void MSSG_mostrarProgressBar100();

//ERRORES

//Escribe el mensaje de error de archivo/directorio no encontrado
//mssg -> mensaje a escribir
//nombre -> nombre del archivo/directorio

void MSSG_error(char * mssg, char * nombre);
void MSSG_errorParam(char * mssg);
void MSSG_errorBind();
void MSSG_errorSocket();
void MSSG_archivo(char * nombre_archivo, char * telescope);
//int FILE_encuentraMaxMagnitud(char * datos);
void MSSG_errorIPValida(char * ip);
void MSSG_errorConnect();
void MSSG_errorAccept();
void MSSG_errorLionel();
void MSSG_ejemploTavish();
void MSSG_ejemploMcGruder();
void MSSG_ejemploLionel();

void MSSG_error_2(char * mssg,int fd);
void MSSG_errorRealloc();
void MSSG_errorMalloc();
void MSSG_errorPaquita();


#endif
