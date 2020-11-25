#include "input.h"
#define CHAR_1 1 //valor de un caracter
#define MAX_MATCHES 1 //La cantidad maxima de similitudes en uns sting (para el regex)

/**
 * Función para leer una cadena de carácteres hasta el caracter deseado de un archivo
 * @param  fd       fd del archivo a leer.
 * @param  end_char carácter hasta el que se quiere leer
 * @return          retorna la cadena de carácteres leída.
 */
char * INPUT_getTEXT(int fd, char end_char){
    int str_l = 0;
    char * c;
    char * str;
    c = (char*) malloc (sizeof(char*));
    str = (char*) malloc (sizeof(char*));
    if (c != NULL && str != NULL) {
        do {
            //leemos el primer caracter
            read(fd, c, CHAR_1);
            //si es el final se le pone un \0 al final
            if (*c == end_char) {
                str[str_l] = '\0';
            }else{
                //si no es el final hacer un realloc para seguir leyendo
                str[str_l] = *c;
                str_l++;
                str = (char*) realloc (str, str_l+1);
                if (str == NULL) MSSG_errorRealloc();
            }

        } while(*c != end_char);
        free(c);
        return str;

    } else {
        return NULL;//ha devuelto NULL
    }
}

/**
 * Función para leer carácteres hasta un determinado carácter. Solo sirve para
 * leer caracteres que no se deseen.
 * @param fd       fd de donde leer.
 * @param end_char carácter hasta el que se quiere leer. 
 */
void INPUT_comeChars(int fd, char end_char) {
    char c;
    do {
        read(fd, &c, 1);
    }while (c != end_char);
}
