#include "server.h"

/**
 * Funcion que recibe un puerto en caracteres y devuelve su entero.
 * Además se libera la memoria de la cadena de caracteres recibida.
 * @param  puerto_c cadena de caracteres del puerto
 * @return          valor del puerto (entero)
 */
int SERVER_getPuerto(char * puerto_c){
    if(puerto_c != NULL){
        int puerto = atoi(puerto_c);
        free(puerto_c);
        return puerto;
    }
    return -1;
}


/**
 * Función para añadir un archivo nuevo a la lista de archivos ya existente (carpeta).
 * Además se protege dicha operación con mutex.
 * @param archivo Archivo a añadir a la Carpeta.
 */
void SERVER_addArchivoCarpeta(Archivo archivo){
    pthread_mutex_lock(&mtxCarpeta);
    //if (s != 0) errExitEN (s, "pthread_mutex_lock");
    FILE_anadeArchivo(&carpeta, archivo);
    pthread_mutex_unlock(&mtxCarpeta);
    //if (s != 0) errExitEN (s, "pthread_mutex_unlock");
}

/**
 * Función para añadir una nueva conexión entrante en Lionel.
 * @param e elemento a añadir a la lista.
 */
void SERVER_addElementLista(Element e){

    pthread_mutex_lock(&mtxListaPuertos);
    //if (s != 0) errExitEN (s, "pthread_mutex_lock");
    LISTA_inserir(&l,e);
    pthread_mutex_unlock(&mtxListaPuertos);
    //if (s != 0) errExitEN (s, "pthread_mutex_unlock");

}

/**
 * Función para eliminar un elemento de la lista debido a su desconexión en el
 * servidor (Lionel)
 * @param e Elemento a eliminar.
 */
void SERVER_removeElementLista(Element e){

    pthread_mutex_lock(&mtxListaPuertos);
    //if (s != 0) errExitEN (s, "pthread_mutex_lock");
    LISTA_borrarElemento(&l,e);
    pthread_mutex_unlock(&mtxListaPuertos);
    //if (s != 0) errExitEN (s, "pthread_mutex_unlock");
}

/**
 * Thread dedicado para escuchar las peticiones de un McGruder en concreto.
 * Se dispone de 3 casos:
 *  - 1. Nueva conexión
 *  - 2. Desconexión
 *  - 3. Tratamiento de los archivos recibidos
 * @param  arg Información referente al socket del McGruder
 * @return     1 si todo ha ido bien, -1 en caso contrario.
 */
void * conexionMcGruderLista(void * arg) {
    ThreadData * td;
    td = (ThreadData * ) arg;
    int socket_fd = td->fd_socket_cliente;
    int id_cua = td->id_cua;

    Trama trama = {TIPONULL, NULL, 0, NULL};
    int continua = 1;
    char * telescope = NULL;
    char * trama_total_imagen = NULL;
    Element e;
    char * nombre_archivo = NULL;
    long unsigned int tamanyo_actual_imagen = 0;
    while(continua) {
        MSSG_connection(WAITING);
        trama = CONEXION_recibeTrama(socket_fd);

        if(trama.type == TIPO1) {
            CONEXION_enviaTrama(socket_fd, TIPO1,"[CONOK]", "");
            telescope = malloc(sizeof(char)*(strlen((char*)trama.data)+1));
            if (telescope != NULL) {
                //copiar nombre telescopio
                strcpy(telescope, (char*)trama.data);
                MSSG_connectionTelescope("Connection from ", telescope);
                //crear elemento nuevo para guardar en la lista
                e.socket_fd = socket_fd;
                e.nombre = NULL;
                e.nombre = malloc (sizeof(char)*(strlen((char*)trama.data)+1));
                if (e.nombre != NULL) {
                    strcpy(e.nombre, (char *)trama.data);
                    SERVER_addElementLista(e);
                }
                //liberar memoria
                if(trama.data != NULL) free(trama.data);
                if(trama.header != NULL) free(trama.header);

            }

        } else if(trama.type == TIPO2) {
            MSSG_disconnect("Disconnecting ", (char*)trama.data);
            CONEXION_enviaTrama(socket_fd, TIPO2,"[CONOK]", "");
            //cerrar socket
            //elminar elemento de la lista
            Element e;
            e.socket_fd = socket_fd;
            e.nombre = malloc(sizeof(char)*(strlen((char*)trama.data)+1));
            if(e.nombre != NULL){
                strcpy(e.nombre,(char*)trama.data);
                SERVER_removeElementLista(e);
                free(e.nombre);
            }
            //liberar memoria
            if (trama.data != NULL)free(trama.data);
            if (trama.header != NULL) free(trama.header);
            if (telescope != NULL) free(telescope);
            continua = 0;

        } else if(trama.type == TIPO3 && (strcmp((char*)trama.header, "[METADATA]") == 0)){
            //write(1, "B", strlen("B"));
            tamanyo_actual_imagen = 0;
            Archivo archivo;
            //conseguir los datos necesarios del archivo
            char * aux = strtok((char*)trama.data, "&");
            char * str = malloc((strlen(aux)+1)*(sizeof(char)));
            if (str != NULL){
                strcpy(str, aux);
                archivo.tipo_archivo = str;
            }
            aux = strtok(NULL, "&");
            char *file_size = malloc( (strlen(aux)+1) * (sizeof(char) ));
            if(file_size != NULL){
                strcpy(file_size, aux);
                archivo.tamanyo = atoi(file_size);
                free(file_size);

            }
            aux = strtok(NULL, ".");
            char * fecha = malloc((strlen(aux)+1)*(sizeof(char)));
            if (fecha != NULL) {
                strcpy(fecha, aux);
                archivo.fecha = fecha;
            }
            if (trama.data != NULL) free(trama.data);
            if (trama.header != NULL) free(trama.header);

            /************* PROCESAR ARCHIVO *************/
            if(strcmp(archivo.tipo_archivo, "TXT\0")==0) {
                trama = CONEXION_recibeTrama(socket_fd);

                //copiamos datos del fichero de txt
                char * datos_texto = malloc(sizeof(char)*((trama.length)+1));
                if (datos_texto != NULL) strcpy(datos_texto, (char*)trama.data);

                //liberamos memoria
                if(trama.header != NULL) free(trama.header);
                if(trama.data != NULL) free(trama.data);

                trama = CONEXION_recibeTrama(socket_fd);
                if(trama.data != NULL) free(trama.header); //liberamos memoria ENDFILE

                if (trama.type == TIPONULL) {
                    continua = 0;
                    continue;
                }


                nombre_archivo = malloc(sizeof(char)*(strlen(archivo.fecha)+11));
                if (nombre_archivo != NULL) {
                    strcpy(nombre_archivo, "files/");
                    strcat(nombre_archivo, archivo.fecha);
                    strcat(nombre_archivo, ".txt");

                    int fd_fichero = open(nombre_archivo, O_CREAT | O_WRONLY,  0777);
                    write(fd_fichero, datos_texto, archivo.tamanyo*sizeof(char));
                    if(CHECKSUM_compruebaChecksum(nombre_archivo,trama.data)){
                        //liberamos checksum
                        if (trama.data != NULL) free(trama.data);

                        archivo.datos = datos_texto;
                        MSSG_archivo(nombre_archivo, telescope);
                        CONEXION_enviaTrama(socket_fd, TIPO3, "[CHECKOK]", "");

                        SERVER_addArchivoCarpeta(archivo);

                        Mensaje mensaje;
                        mensaje = FILE_getDatosContelaciones(archivo.datos, archivo.tamanyo);
                        CUESMENSAJES_enviaMensaje(id_cua,2,mensaje);
                        if(nombre_archivo != NULL) free(nombre_archivo);

                    } else {
                        CONEXION_enviaTrama(socket_fd, TIPO3, "[CHECKKO]", "");
                        //liberamos checksum
                        unlink(nombre_archivo);
                        if (trama.data != NULL) free(trama.data);
                        if (nombre_archivo != NULL) free(nombre_archivo);
                    }

                }

            } else {

                trama_total_imagen = malloc(sizeof(char) * archivo.tamanyo);
                if (trama_total_imagen != NULL) {
                    long unsigned int index_file = 0;
                    long unsigned int i_file;
                    //Leemos la trama de la imagen hasta que llegue el [ENDFILE]
                    while(1) {
                        trama = CONEXION_recibeTrama(socket_fd);
                        //Si la trama es ENDFILE se sale del bucle
                        if (strcmp((char*)trama.header, "[ENDFILE]") == 0) break;

                        for(i_file = 0; i_file < trama.length; i_file++){
                            trama_total_imagen[index_file] = trama.data[i_file];
                            index_file++;
                        }
                        tamanyo_actual_imagen += trama.length;

                        if (trama.header != NULL) free(trama.header);
                        if(trama.data != NULL) free(trama.data);

                    }

                    //liberando ENDFILE
                    if (trama.header != NULL) free(trama.header);

                    nombre_archivo = malloc(sizeof(char)*(strlen(archivo.fecha)+11));

                    if (nombre_archivo != NULL) {
                        strcpy(nombre_archivo, "files/");
                        strcat(nombre_archivo, archivo.fecha);
                        strcat(nombre_archivo, ".jpg");
                        //nombre_archivo[strlen(nombre_archivo)] = '\0';
                        int fd_fichero = open(nombre_archivo, O_CREAT | O_WRONLY,  0777);
                        write(fd_fichero, trama_total_imagen, archivo.tamanyo*sizeof(char));

                        if(CHECKSUM_compruebaChecksum(nombre_archivo, trama.data)) {

                            archivo.datos = trama_total_imagen;
                            MSSG_archivo(nombre_archivo, telescope);

                            CONEXION_enviaTrama(socket_fd, TIPO3, "[CHECKOK]", "");

                            SERVER_addArchivoCarpeta(archivo);

                            Mensaje mensaje;
                            mensaje.tamanyo_imagen = archivo.tamanyo;
                            CUESMENSAJES_enviaMensaje(id_cua,1, mensaje);

                            if(nombre_archivo != NULL) free(nombre_archivo);

                            //liberamos checksum
                            if(trama.data != NULL) free(trama.data);

                        }
                        else {
                            CONEXION_enviaTrama(socket_fd, TIPO3, "[CHECKKO]", "");
                            unlink(nombre_archivo);

                            //liberamos checksum
                            if (trama.data != NULL) free(trama.data);
                            if (nombre_archivo != NULL) free(nombre_archivo);

                        }


                    }

                }

            }
        }

    }
    return (void*) 1;
}

/**
 * Thread para aceptar conexiones de los distintos McGruders que se quieren conectar
 * @param  arg Información del socket de la conexión.
 * @return     1 si todo ha ido bien, -1 en caso contrario
 */
void * SERVER_McGruderDedicado(void * arg){
    ThreadData * td;
    td = (ThreadData *) arg;
    int fd_socket_cliente;
    int estado_threadAccept;
    td->fd_socket = CONEXION_conectarServer(td->puerto);
    if(td->fd_socket > 0){

        while(1) {
            //MSSG_connection(WAITING);
            fd_socket_cliente = CONEXION_aceptarServer(td->fd_socket);
            if (fd_socket_cliente > 0){
                td->fd_socket_cliente = fd_socket_cliente;
                estado_threadAccept = pthread_create(&(td->thread_id_accept),
                    NULL,conexionMcGruderLista, td);
                if (estado_threadAccept != 0) {
                    MSSG_error(THREAD_ERROR, "MCGruder");
                    return (void *) -1;
                }
                pthread_detach(td->thread_id_accept);
            }
        }
    }else{
       return (void * ) -1;
    }
    return (void * ) 1;
}

/**
 * Thread dedicado para escuchar las peticiones de un McTavish en concreto.
 * Además se efectua la comunicación con Paquita.
 * Se dispone de 4 casos:
 *  - 1. Nueva conexión
 *  - 2. Desconexión
 *  - 4. Tratamiento de la petición recibida en el caso de las estadísticas.
 *  - 5. Tratamiento de la petición recibida en el caso del último fichero recibido.
 * @param  arg Información del socket de la nueva conexión
 * @return     1 si todo ha ido bien, -1 en caso contrario.
 */
void * conexionMcTavishLista(void * arg) {
    ThreadData * tdd;
    tdd = (ThreadData *) arg;
    int id_cua = tdd-> id_cua;
    int socket_fd = tdd->fd_socket_cliente;
    Trama trama;
    int continua = 1;
    //char *str_data;
    char str_data[50];
    Element e;
    while (continua) {
        MSSG_connection(WAITING);
        trama = CONEXION_recibeTrama(socket_fd);

        if (trama.type == TIPO1) {

            e.nombre = malloc (sizeof(char)*(strlen((char*)trama.data)+1));
            if (e.nombre != NULL) {
                CONEXION_enviaTrama(socket_fd, TIPO1, "[CONOK]", "");
                strcpy(e.nombre, (char*)trama.data);
                e.socket_fd = socket_fd;
                MSSG_connectionTelescope("Connection from ", e.nombre);
                SERVER_addElementLista(e);
                if (trama.header != NULL ) free(trama.header);
                if (trama.data != NULL ) free(trama.data);

            } else {
                CONEXION_enviaTrama(socket_fd, TIPO1, "[CONKO]", "");
                close(socket_fd);
                if (trama.data != NULL)free(trama.data);
                if (trama.header != NULL) free(trama.header);
                if (e.nombre != NULL) free(e.nombre);
                continua = 0;
            }

        } else if(trama.type == TIPO2) {
            CONEXION_enviaTrama(socket_fd, TIPO2,"[CONOK]", "");
            MSSG_disconnect("Disconnecting ", (char*)trama.data);
            //elminar elemento de la lista
            Element e;
            e.socket_fd = socket_fd;
            e.nombre = malloc(sizeof(char)*(strlen((char*)trama.data)+1));

            if(e.nombre != NULL){
                strcpy(e.nombre,(char*)trama.data);
                SERVER_removeElementLista(e);
            }
            //liberar memoria
            if (trama.data != NULL) free(trama.data);
            if (trama.header != NULL) free(trama.header);
            if (e.nombre != NULL) free(e.nombre);

            continua = 0;

        } else if(trama.type == TIPO4) {
            Mensaje mensaje;
            MensajeRespuesta mensajeResp;

            CUESMENSAJES_enviaMensaje(id_cua,3, mensaje);

            msgrcv(id_cua, &mensajeResp, sizeof(MensajeRespuesta)-sizeof(long),5,0);

            sprintf(str_data, "%d&%.2f&%d&%d", mensajeResp.img_recibidas, mensajeResp.mida_total,mensajeResp.txt_recibidos, mensajeResp.mitjana_const);

            CONEXION_enviaTrama(socket_fd, TIPO4,"[]", str_data);
            MSSG_petition1Tavish();
            if (trama.data != NULL) free(trama.data);
            if (trama.header != NULL) free(trama.header);

        } else if(trama.type == TIPO5) {
            Mensaje mensaje;
            MensajeRespuesta mensajeResp;
            CUESMENSAJES_enviaMensaje(id_cua,4, mensaje);

            msgrcv(id_cua, &mensajeResp, sizeof(MensajeRespuesta)-sizeof(long),6,0);

            sprintf(str_data, "%d&%.2f&%.2f&%.2f", mensajeResp.constelacions, mensajeResp.mitjana_densitat,mensajeResp.max_magnitud, mensajeResp.min_magnitud);

            CONEXION_enviaTrama(socket_fd, TIPO4,"[]", str_data);
            MSSG_petition2Tavish();

            if (trama.data != NULL) free(trama.data);
            if (trama.header != NULL) free(trama.header);
        }
    }

    return (void * ) 1;
}

/**
 * Thread para aceptar conexiones de los distintos McTavish que se quieren conectar
 * @param  arg Información del socket del McTavish que ha hecho el connect.
 * @return     1 si todo ha ido bien, -1 en caso contrario.
 */
void * SERVER_McTavishDedicado(void * arg){
    ThreadData * td;
    td = (ThreadData *) arg;

    int fd_socket_cliente;
    int estado_threadTavishAccept;
    td->fd_socket = CONEXION_conectarServer(td->puerto);
    if(td->fd_socket > 0){

        while(1) {

            fd_socket_cliente = CONEXION_aceptarServer(td->fd_socket);
            if (fd_socket_cliente > 0){
                td->fd_socket_cliente = fd_socket_cliente;
                estado_threadTavishAccept = pthread_create(&(td->thread_id_accept),
                    NULL,conexionMcTavishLista,td);
                if (estado_threadTavishAccept != 0) {
                    MSSG_error(THREAD_ERROR, "MCTavish");
                    return (void *) -1;
                }
                pthread_detach(td->thread_id_accept);
            }
        }
    }else{
        return (void * ) -1;
    }
    return (void * ) 1;
}
