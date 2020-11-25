#include "mssg.h"

// MENSAJES PARA ESCRIBIR POR PANTALLA DE LA TERMINAL SEGUN EL CASO.
// CREEMOS QUE NO HACE FALTA COMENTAR FUNCIÓN A FUNCIÓN TODAS LAS FUNCIONES.

void MSSG_connection(char * mssg) {
    write(1, mssg, strlen(mssg));
    write(1, "\n\n", 1);
}
void MSSG_connectionTelescope(char * mssg, char * telescope){
    write(1, mssg, strlen(mssg));
    write(1, telescope, strlen(telescope));
    write(1, "\n", 1);

}
void MSSG_petition1Tavish() {
    write(1, "Pidiendo datos sobre las transmisiones recibidas...\n",
            strlen("Pidiendo datos sobre las transmisiones recibidas...\n"));

}
void MSSG_petition2Tavish() {
    write(1, "Pidiendo datos sobre el último fichero de constelaciones...\n",
            strlen("Pidiendo datos sobre el último fichero de constelaciones...\n"));

}
void MSSG_archivo(char * nombre_archivo, char * telescope){
    write(1, "Receiving data from ", strlen("Receiving data from "));
    write(1, telescope, strlen(telescope));
    write(1, "\n", strlen("\n"));
    write(1, "File ", strlen("File "));
    write(1, nombre_archivo, strlen(nombre_archivo));
    write(1, " received.\n", strlen(" received.\n"));
    write(1, "\n", strlen("\n"));

}

void MSSG_novaConexio(char * ip, u_int16_t port){
    write(1, "Nueva conexion de ", strlen("Nueva conexion de "));
    write(1, &ip, strlen(ip));
    write(1, ":", strlen(":"));
    write(1, &port, sizeof(port));
    write(1, "\n", 1);
}

void MSSG_nuevaConexionLionel(char * connection_name){
    write(1, "Connection Lionel-", strlen("Connection Lionel-"));
    write(1, connection_name, strlen(connection_name));
    write(1, " ready.\n", strlen(" ready.\n"));
}

void MSSG_disconnect(char * mssg, char * nombre_telescopio){
    write(1, mssg, strlen(mssg));
    write(1, nombre_telescopio, strlen(nombre_telescopio));
    write(1, "...\n", strlen("...\n"));
}

void MSSG_fileFound(char * mssg, char * nameFile){
    write(1, mssg, strlen(mssg));
    write(1, nameFile, strlen(nameFile));
    write(1, "\n", 1);
}

void MSSG_sendingFile(char * mssg, char * fileName){
    write(1, mssg, strlen(mssg));
    write(1, fileName, strlen(fileName));
    write(1, "\n", 1);
    write(1, "\n", 1);


}

void MSSG_mostrarProgressBar(long int actual, long int total){
    int i;
    float porcentaje;
    int progreso;
    char num[15];

    porcentaje = ((float)actual /(float)total) * 100;
    progreso = (int)porcentaje/10;

    write(1, "\rsending ", strlen("\rsending "));
    for (i = 0; i < 10; i++){
        if (i < progreso) {
            write(1, "#", 1);
        }else{
            write(1, " ", 1);
        }
    }

    sprintf(num, "[%.2f%%]", porcentaje);
    write(1, num, strlen(num));
    fflush(stdout);
}

void MSSG_mostrarProgressBar100(){
    write(1, "\rsending ##########[100.00%]\n", strlen("\rsending ##########[100.00%]\n") );
}


//ERRORES

void MSSG_error(char * mssg, char * nombre){
    write(1, mssg, strlen(mssg));
    write(1, nombre, strlen(nombre));
    write(1, "\n", 1);
}
void MSSG_error_2(char * mssg, int fd){
    char aux[50];
    sprintf(aux, "%s: %d\n", mssg, fd);
    write(1, aux, strlen(aux));


};

void MSSG_errorParam(char * mssg){
    write(1, mssg, strlen(mssg));
}

void MSSG_errorSocket(){
    write(1, "Error con el socket\n", strlen("Error con el socket\n"));
}

void MSSG_errorIPValida(char * ip) {
    write(1, "Error, ", strlen("Error "));
    write(1, &ip, strlen(ip));
    write(1, " no es una IP valida.\n", strlen(" no es una IP valida.\n"));
}

void MSSG_errorConnect(){
    write(1, "Error en la función connect\n", strlen("Error en la función connect\n"));

}

void MSSG_errorBind(){
    write(1, "Error en la función bind\n", strlen("Error en la función bind\n"));
}
void MSSG_ejemploTavish() {
    write(1, "-> Ejemplo: ./McTavish config1.dat\n\n", strlen("-> Ejemplo: ./McTavish config1.dat\n\n"));
}

void MSSG_ejemploLionel() {
    write(1, "-> Ejemplo: ./Lionel configLionel.dat\n\n", strlen("-> Ejemplo: ./Lionel configLionel.dat\n\n"));
}

void MSSG_ejemploMcGruder() {
    write(1, "-> Ejemplo: ./McGruder config1.dat files/\n\n", strlen("-> Ejemplo: ./McGruder config1.dat files/\n\n"));
}

void MSSG_errorAccept(){
    write(1, "Error, no se ha podido aceptar la nueva conexion\n", strlen("Error, no se ha podido aceptar la nueva conexion\n"));
}

void MSSG_errorLionel(){
    write(1, "\nError, Lionel ha caído... Cerrando McGruder...\n", strlen("\nError, Lionel ha caído... Cerrando McGruder...\n"));
}

void MSSG_errorRealloc(){
    write(1, "Error al hacer REALLOC\n", strlen("Error al hacer REALLOC\n"));
}

void MSSG_errorMalloc(){
    write(1, "Error al pedir memoria\n", strlen("Error al pedir memoria\n"));
}
void MSSG_errorPaquita(){
    write(1, "Error al crear Paquita\n", strlen("Error al crear Paquita\n"));
}
