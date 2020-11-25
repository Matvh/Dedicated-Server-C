#ifndef FILE_H
#define FILE_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cuesmensajes.h"
#include "../../Aux/mssg.h"

typedef struct {
    char * tipo_archivo;
    char * fecha;
    unsigned long int tamanyo;
    char * datos;
} Archivo;

typedef struct {
    Archivo * archivos;
    unsigned long int num_archivos;
}Carpeta;

void FILE_escribirFichero(Archivo archivo, int kalkun);
void FILE_escribirFicheroDatosAstro(char * data);

Mensaje FILE_getDatosContelaciones(char * archivo, long tamano);

void FILE_initCarpeta(Carpeta * carpeta);
void FILE_anadeArchivo(Carpeta * carpeta, Archivo archivo);
void FILE_destruyeCarpeta(Carpeta * carpeta);
void FILE_pintaArchivo(Archivo a);


#endif
