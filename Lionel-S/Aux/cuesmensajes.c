#include "cuesmensajes.h"

/**
 * Función para enviar un mensaje de tipo Mensaje por la cola de mensajes.
 * @param id_cua  identificador de la cola
 * @param id_msg  identificador del mensaje
 * @param mensaje Mensaje de tipo Mensaje para enviar por la cola.
 */
void CUESMENSAJES_enviaMensaje(int id_cua, int id_msg, Mensaje mensaje){
    SEM_wait(&semaforo_cua);
    mensaje.id_msg = id_msg;
    msgsnd(id_cua,&mensaje, sizeof(Mensaje) - sizeof(long),IPC_NOWAIT);
    SEM_signal(&semaforo_cua);
}


/**
 * Función para enviar un mensaje de tipo MensajeRespuesta por la cola de mensajes.
 * Sirve para que Paquita sepa cuando enviar los datos a Lionel.
 * @param id_cua  identificador de la cola
 * @param id_msg  identificador del mensaje
 * @param mensaje_respuesta Mensaje de tipo MensajeRespuesta para enviar por la cola.
 */
void CUESMENSAJES_enviaMensajeRespuesta(int id_cua, int id_msg, MensajeRespuesta mensaje_respuesta){
    SEM_wait(&semaforo_cua);
    mensaje_respuesta.id_msg = id_msg;
    msgsnd(id_cua,&mensaje_respuesta, sizeof(MensajeRespuesta) - sizeof(long),IPC_NOWAIT);
    SEM_signal(&semaforo_cua);
}
