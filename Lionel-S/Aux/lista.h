#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct{
    int socket_fd;
    char * nombre;
}Element;


typedef struct _nodo{
	Element e;
	struct _nodo * sig;
}Nodo;

typedef struct{
	Nodo * pri;
	Nodo * ant;
}Lista;

//Crea la lista
Lista LISTA_crea();
//Inisere un elemento a la lista
void LISTA_inserir(Lista *l , Element e);
//Borra el elemento en la posicion actual
void LISTA_borrar(Lista *l);
//Borra el elemento
void LISTA_borrarElemento(Lista *l, Element e);
//Consulta en la posicion actual.
//si no puede avanzar devuelve un elemento NUEVO socket_fd = -1 y nombre = NULL
Element LISTA_consultar(Lista l);
//Indica si la lista esta vacia
int LISTA_vacia(Lista l);
//Va al principio de la lista
void LISTA_irInicio(Lista * l);
//Avanza una posicion en la lista
void LISTA_avanzar(Lista * l);
//Indica si la lista llego al final
int LISTA_fin(Lista l);
//Destruye la lista entera
void LISTA_destruye(Lista * l);
//Printa todos los elementos de la lista
void Lista_pintaLista(Lista l);

#endif
