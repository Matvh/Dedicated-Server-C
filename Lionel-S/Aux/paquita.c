#include "paquita.h"

static int pid;
void reconfSIGTERM(){
    if(pid == 0){
        raise(SIGKILL);
    }
}
/**
 * Función que ejecuta Paquita y hace todos los calculos respectivos a los DATOS
 * que le llegan por la cola de mensajes.
 * Se dispone de 4 buzones en la cola de mensajes:
 *  - 1. Para notificar a Paquita de una nueva imagen recibida en Lionel.
 *  - 2. Para notificar a Paquita de un nuevo dato de constelaciones en Lionel.
 *  - 3. Para que Paquita devuelva los datos generales de los calculos que ha ido haciendo
 *  - 4. Para que Paquita devuelva los datos del último archivo recibido. 
 * @param id_cua [description]
 */
void PAQUITA_ejecutaPaquita(int id_cua){
    //datos generales
    pid = 0;
    int num_imgs, num_txt;
    num_imgs  = num_txt = 0;
    float total_kb_img = 0.0;
    float media_constelaciones = 0.0;
    int total_num_constelaciones = 0;

    //datos ultimo archivo
    int ultimo_contelaciones = 0;
    float ultimo_media_densidades = 0.0;
    float ultimo_max_magnitud, ultimo_min_magnitud = 0.0;

    signal(SIGTERM, reconfSIGTERM);
    Mensaje mensaje;
    MensajeRespuesta mensajeResp;
    while (1) {
        //El -4 es para que lea los buzones 1 a 4.
        //El 0 es para que sea lectura bloqueante.
        msgrcv(id_cua, &mensaje, sizeof(Mensaje)-sizeof(long),-4,0);

        switch (mensaje.id_msg) {
            //Imagenes
            case 1:
                num_imgs++;
                total_kb_img += (mensaje.tamanyo_imagen/(1024));
                break;
            //Texto
            case 2:
                num_txt++;
                //calcular cosas del mensaje
                total_num_constelaciones += mensaje.num_constelacions;
                media_constelaciones = total_num_constelaciones /  num_txt;
                //Calcular cosas ultimo recibido
                ultimo_contelaciones = mensaje.num_constelacions_diferentes;
                ultimo_media_densidades = mensaje.suma_densitat / mensaje.num_constelacions;
                ultimo_max_magnitud = mensaje.max_valor_magnitud;
                ultimo_min_magnitud = mensaje.min_valor_magnitud;
                break;
            //Datos generales
            case 3:
                mensajeResp.img_recibidas = num_imgs;
                mensajeResp.txt_recibidos = num_txt;
                mensajeResp.mida_total = total_kb_img;
                mensajeResp.mitjana_const = media_constelaciones;
                CUESMENSAJES_enviaMensajeRespuesta(id_cua,5, mensajeResp);
                break;
            //Datos ultimo archivo
            case 4:
                mensajeResp.constelacions = ultimo_contelaciones;
                mensajeResp.mitjana_densitat = ultimo_media_densidades;
                mensajeResp.max_magnitud = ultimo_max_magnitud;
                mensajeResp.min_magnitud = ultimo_min_magnitud;
                CUESMENSAJES_enviaMensajeRespuesta(id_cua,6, mensajeResp);
                break;
            default:
                break;
        }
    }
}
