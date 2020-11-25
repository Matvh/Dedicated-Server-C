#include "lista.h"

/**
 * Función para crear una lista de conexiones, tanto de McGruders como de McTavish
 * @return una lista inicializada lista para usar.
 */
Lista LISTA_crea(){
	Lista l;
	l.pri = (Nodo *)malloc(sizeof(Nodo));
	if (l.pri != NULL){
		l.ant = l.pri;
		l.pri -> sig = NULL;
	}
	return l;
}

/**
 * Inserta un nuevo elemento en la lista.
 * @param l lista.
 * @param e elemento nuevo a insertar.
 */
void LISTA_inserir(Lista *l , Element e){
	Nodo * aux;
	aux = (Nodo *) malloc (sizeof(Nodo));
	if (aux != NULL){
		aux -> e = e;
		aux -> sig = l ->ant -> sig;
		l -> ant -> sig = aux;
		l -> ant = aux;
	}
}

/**
 * Borra la lista entera.
 * @param l Lista a borrar.
 */
void LISTA_borrar(Lista *l){
	Nodo * aux;
	if(l->ant->sig != NULL) {//!Lista_fin (*l)
		aux = l-> ant->sig;
		l ->ant->sig = l -> ant -> sig -> sig; //aux->
		free(aux -> e.nombre);
		free(aux);
	}
}

/**
 * Devuelve el último elemento sobre el que se ha quedado el puntero ant.
 * @param  l lista
 * @return   elemento a consultar
 */
Element LISTA_consultar(Lista l){
	Element e;
    e.socket_fd = -1;
	e.nombre = NULL;
    if(l.ant->sig != NULL){
        e = l.ant ->sig->e;
    }
    return e;
}

/**
 * Función para consultar si la lista está vacía o no
 * @param  l lista a consultar
 * @return   devuelve == 0 si la lista NO está vacía.
 */
int LISTA_vacia(Lista l) {
	return l.pri->sig == NULL; //si el siguiente del fantasma es null, esta vacia
}

/**
 * Función para avanzar el puntero a la siguiente posicion disponible
 * @param l lista
 */
void LISTA_avanzar (Lista * l){
	if( l -> ant -> sig !=NULL){
		l -> ant = l -> ant -> sig;
	}
}

/**
 * Función para consultar si estamos al final de la lista.
 * @param  l lista
 * @return   retorna == 0 si la lista no está al final
 */
int LISTA_fin (Lista l){
	return l.ant->sig == NULL;
}
/**
 * Función para mover el puntero de ant al principio de la lista.
 * @param l lista
 */
void LISTA_irInicio(Lista * l){
	l-> ant = l-> pri;
}
/**
 * Función para eliminar un elemento en concreto de la lista.
 * @param l lista
 * @param e elemento a eliminar.
 */
void LISTA_borrarElemento(Lista *l, Element e){
	Element aux;
	int encontrado = 0;
	LISTA_irInicio(l);
	while (!LISTA_fin(*l) && !encontrado){
		aux = LISTA_consultar(*l);
		if(aux.socket_fd == e.socket_fd &&
			strcmp(aux.nombre, e.nombre) == 0){
			encontrado = 1;
			LISTA_borrar(l);
		}else{
			LISTA_avanzar(l);
		}
	}
	//Dejar la lista al final para añadir el siguiente elemento al final
	if(encontrado){
		while (!LISTA_fin(*l)) {
			LISTA_avanzar(l);
		}
	}
}
/**
 * Función para liberar todos los recursos de la lista (aka destructor de la lista).
 * @param l lista a destruir.
 */
void LISTA_destruye(Lista * l){
	LISTA_irInicio(l);
	while (!LISTA_vacia(*l)){
		LISTA_borrar(l);
	}
	free(l->pri);
	l->pri = NULL;
	l-> ant = NULL;
}

void pintaElemento(Element e){
	char str[30];
	sprintf(str,"Socket: %d Nombre: ", e.socket_fd);
	write(1,str, strlen(str));
	write(1,e.nombre, strlen(e.nombre));
	write(1, "\n", 1);
}

void Lista_pintaLista(Lista l){
	Element e;
	LISTA_irInicio(&l);
	while (!LISTA_fin(l)){
		e = LISTA_consultar(l);
		pintaElemento(e);
		LISTA_avanzar(&l);
	}
}
