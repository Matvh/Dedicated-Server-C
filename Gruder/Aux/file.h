#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../Aux/mssg.h"
#include "../../Aux/conexion.h"

extern char * carpeta;
extern int tiempo;
extern char * nombre_archivo;
extern int socket_fd;
extern struct dirent ** archivos; //directorio con los archivos

//PRE: fd del socket activo. El archivo tiene que existir.
//     Parametros inicializados correctamente.
//POST: --
void FILE_enviarFicheroImagen(long int tamanyo_archivo, char * date, int socket_fd,
                                char * nombre_archivo);

//PRE: fd del socket activo. El archivo tiene que existir.
//     Parametros inicializados correctamente.
//POST: Devuelve >= 0 si se ha enviado correctamente el fichero. < 0 en caso contrario.
int FILE_enviarFicheroTexto(long int tamanyo_archivo, char * date, int socket_fd,
                                char * nombre_archivo);

//PRE: num_c inicializado y con un valor entero en ASCII.
//POST: retorna el valor en int de la cadena de caracteres.
int FILE_getIntFromString(char * num_c);

void FILE_mirarArchivosPath();

char * FILE_getCarpeta(const char * nom_param);

#endif
