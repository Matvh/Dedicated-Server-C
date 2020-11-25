#ifndef INPUT_H
#define INPUT_H
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <regex.h>
#include <sys/types.h>
#include "mssg.h"

//fd --> file descriptor
//end_char --> char hasta cual leer el texto
//devuelve toda la cadena leida hasta end_char
//lee end_char pero no lo deja en la cadena
char * INPUT_getTEXT(int fd, char end_char);

//fd --> file descriptor
//end_char --> char hasta cual leer el texto
//se come todos los caracteres que se encuentre hasta end_char
void INPUT_comeChars(int fd, char end_char);

//Limpia el buffer del teclado
void INPUT_limpiaBufferTeclado();

#endif
