#ifndef CUESMENSAJES_H
#define CUESMENSAJES_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "semaphore_v2.h"
extern semaphore semaforo_cua;

// MENSAJES
typedef struct {
    // Buzones: 1-Imagenes 2-Texto 3-Datos Generales 4- Ultimo Archivo
    // 5-Respuesta de Paquita Datos Generales 6- Respuesta de Paquita Datos Ultimo Archivo
    long id_msg;
    //Campos para mandar info de la info de texto
    float suma_densitat;
    float max_valor_magnitud;
    float min_valor_magnitud;
    int num_constelacions;
    int num_constelacions_diferentes;
    //Campos para mandar info del tamaño de una imagen
    unsigned long tamanyo_imagen;
}Mensaje;

typedef struct{
    long id_msg; //en el 5 buzon 5 esperamos la respuesta
    //Respuesta de la opcion 1
    int img_recibidas;
    float mida_total;
    int txt_recibidos;
    int mitjana_const;
    //Respuesta de la opcion 2
    int constelacions;
    float mitjana_densitat;
    float max_magnitud;
    float min_magnitud;
} MensajeRespuesta;



// PRE: La cola de mensages tiene que estar abierta
// POST: Envia un Mensaje por la cola de mensajes
void CUESMENSAJES_enviaMensaje(int id_cua, int id_msg, Mensaje mensaje);

// PRE: La cola de mensages tiene que estar abierta
// POST: Envia un MensajeRespuesta para que Paquita devuelva la información
// por la cola de mensajes
void CUESMENSAJES_enviaMensajeRespuesta(int id_cua, int id_msg, MensajeRespuesta mensaje_respuesta);

#endif
