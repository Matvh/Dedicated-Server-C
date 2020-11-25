#ifndef FILE_H
#define FILE_H
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "../../Aux/mssg.h"
#include "../../Aux/conexion.h"

extern Trama trama;
extern int socket_fd;
extern char * nombre_cientifico;
extern int muerto;

int TAVISH_getIntFromString(char * num_c);
void TAVISH_conectaLionel();

#endif
