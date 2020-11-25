#include "tavish.h"

int TAVISH_getIntFromString(char * num_c){
    if(num_c != NULL){
        int num = atoi(num_c);
        free(num_c);
        return num;
    }
    return -1;
}



void TrataDatosTramaTransmision(unsigned char * data){
    if(data != NULL){
        char * aux = strtok((char*)trama.data, "&");
        write(1, "\nNumero de imagenes recibidas en Lionel: ",strlen("\nNumero de imagenes recibidas en Lionel: "));
        write(1, aux, strlen(aux));
        write(1, "\n", strlen("\n"));
        aux = strtok(NULL, "&");
        write(1, "Tamaño total (en KB) de imagenes recibidas en Lionel: ", strlen("Tamaño total (en KB) de imagenes recibidas en Lionel: "));
        write(1, aux, strlen(aux));
        write(1, "\n", strlen("\n"));
        aux = strtok(NULL, "&");
        write(1, "Numero de ficheros astronomicos recibidos en Lionel: ", strlen("Numero de ficheros astronomicos recibidos en Lionel: "));
        write(1, aux, strlen(aux));
        write(1, "\n", strlen("\n"));
        aux = strtok(NULL, "&");
        write(1, "Mediana de constelaciones recibidas en Lionel: ", strlen("Mediana de constelaciones recibidas en Lionel: "));
        write(1, aux, strlen(aux));
        write(1, "\n\n", strlen("\n\n"));
    } else {
        write(1, "Error con los datos recibidos\n\n", strlen("Error con los datos recibidos\n\n"));
    }

}

void TrataDatosConstelaciones(unsigned char * data) {
    if(data != NULL){
        char * aux = strtok((char*)trama.data, "&");
        write(1, "\nNumero de constelaciones existentes recibidas en Lionel: ",strlen("\nNumero de constelaciones existentes recibidas en Lionel: "));
        write(1, aux, strlen(aux));
        write(1, "\n", strlen("\n"));
        aux = strtok(NULL, "&");
        write(1, "Mediana de densidades recibidas en Lionel: ", strlen("Mediana de densidades recibidas en Lionel: "));
        write(1, aux, strlen(aux));
        write(1, "\n", strlen("\n"));
        aux = strtok(NULL, "&");
        write(1, "Valor maximo (magnitud) recibido en Lionel: ", strlen("Valor maximo (magnitud) recibido en Lionel: "));
        write(1, aux, strlen(aux));
        write(1, "\n", strlen("\n"));
        aux = strtok(NULL, "&");
        write(1, "Valor minimo (magnitud) recibido en Lionel: ", strlen("Valor minimo (magnitud) recibido en Lionel: "));
        write(1, aux, strlen(aux));
        write(1, "\n\n", strlen("\n\n"));
    } else {
        write(1, "Error con los datos recibidos\n\n", strlen("Error con los datos recibidos\n\n"));
    }
}

void pintaMenu(){
    write(1, nombre_cientifico, strlen(nombre_cientifico));
    write(1, "\n\n-------- MENU -------- \n", strlen("\n\n-------- MENU -------- \n"));
    write(1, "1. Dades sobre les transmissions rebudes.\n", strlen("1. Dades sobre les transmissions rebudes.\n"));
    write(1, "2. Dades sobre el darrer fitxer de constel·lacions.\n", strlen("2. Dades sobre el darrer fitxer de constel·lacions.\n"));
    write(1, "3. Finalitzar sessió.\n", strlen("3. Finalitzar sessió.\n"));

}

void TAVISH_conectaLionel(){

    char opcion, basura;
    int recibe = 1;
    CONEXION_enviaTrama(socket_fd, TIPO1,"[]", nombre_cientifico);
    trama = CONEXION_recibeTrama(socket_fd);
    if(strcmp(trama.header, "[CONOK]") == 0) {
        if (trama.header != NULL) free(trama.header);
        if (trama.data != NULL) free(trama.data);
        do {
            pintaMenu();
            read(0, &opcion, sizeof(char));
            do {
                read(0, &basura, sizeof(char));
            } while (basura != '\n');
            switch (opcion) {
                case '1':
                    MSSG_petition1Tavish();
                    recibe = CONEXION_enviaTrama(socket_fd, TIPO4,"[]", "");
                    if (recibe < 0) {
                        muerto = 1;
                        MSSG_errorLionel();
                        raise(SIGINT);
                    }
                    trama = CONEXION_recibeTrama(socket_fd);
                    TrataDatosTramaTransmision(trama.data);
                    if(trama.header != NULL) free(trama.header);
                    if(trama.data != NULL) free(trama.data);
                    break;
                case '2':
                    MSSG_petition2Tavish();
                    recibe = CONEXION_enviaTrama(socket_fd, TIPO5,"[]", "");
                    if(recibe < 0) {
                        muerto = 1;
                        MSSG_errorLionel();
                        raise(SIGINT);
                    }
                    trama = CONEXION_recibeTrama(socket_fd);
                    TrataDatosConstelaciones(trama.data);
                    if(trama.header != NULL) free(trama.header);
                    if(trama.data != NULL) free(trama.data);
                    break;
                case '3':
                    //desconexion
                    recibe = CONEXION_enviaTrama(socket_fd, TIPO2,"[]", nombre_cientifico);
                    if(recibe < 0) {
                        muerto = 1;
                        MSSG_errorLionel();
                        raise(SIGINT);
                    }
                    trama = CONEXION_recibeTrama(socket_fd);
                    if(strcmp(trama.header, "[CONOK]") == 0) {
                        MSSG_disconnect("Disconnecting scientist ", nombre_cientifico);
                        muerto = 1;
                        if(trama.header != NULL) free(trama.header);
                        if(trama.data != NULL) free(trama.data);

                        raise(SIGINT);

                    }
                    break;
                default:
                    write(1, "\n-> No has introducido bien la opción (1-3)\n\n", strlen("\n-> No has introducido bien la opción (1-3)\n\n"));
                    break;
            }
        } while(opcion != '3');
    } else {
        //no se ha podido conectar con Lionel
    }
}
