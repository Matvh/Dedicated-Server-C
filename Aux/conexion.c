#include "conexion.h"

/**
 * Función para conectar un cliente con un servidor
 * @param  ip     dirección IP (en número o en letra)
 * @param  puerto puerto al que se desea conectar
 * @return        retorna el fd del socket.
 */
int CONEXION_connectCliente(char * ip, int puerto){

    int socket_fd;
    struct sockaddr_in s_addr;

    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd < 0){
        MSSG_errorSocket();
        return -1;
    }

    //limpiamos el s_addr
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(puerto);

    struct hostent *host = gethostbyname(ip);

    if (host == NULL){ //si es NULL no es direccion de host
        //Verificar si es direccion de ip
        if(inet_aton(ip, &s_addr.sin_addr) == 0 ){
            MSSG_errorIPValida(ip);
            return -1;
        }

    }else{
        bcopy (host->h_addr, &s_addr.sin_addr.s_addr, host->h_length);
    }


    if(connect(socket_fd, (void*) &s_addr, sizeof(s_addr)) < 0){
        MSSG_errorConnect();
        return -1;
    }
    return socket_fd;
}

/**
 * Función para conectar un servidor con un cliente
 * @param  puerto puerto desde el cual se va a estar escuchando peticiones
 * @return        retorna el fd del socket
 */
int CONEXION_conectarServer(int puerto){

    int socket_fd;
    socket_fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd < 0) {
        MSSG_errorSocket();
        return -1;
    }

    struct sockaddr_in s_addr;
    memset (&s_addr, 0, sizeof (s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons (puerto);
    s_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind (socket_fd, (void *) &s_addr, sizeof (s_addr)) < 0){
        MSSG_errorBind();
        raise(SIGINT);
        pause();
    }
    listen (socket_fd, NUM_CONEXIONES);
    return socket_fd;
}

/**
 * Función para aceptar conexiones una vez ya se hayan conectado previamente
 * @param  socket_fd socket del conectarServer o conectarCliente
 * @return           fd socket de la conexion
 */
int CONEXION_aceptarServer(int socket_fd){

    struct sockaddr_in s_addr;
    socklen_t len = sizeof (s_addr);
    int newsock;
    newsock = accept (socket_fd, (void *) &s_addr, &len);
    if (newsock < 0){
        MSSG_errorAccept();
        return -1;
    }

    return newsock;
}

/**
 * Función que lee el campo HEADER de la trama. Lee hasta que encuentra el
 * carácter ']'
 * @param  fd  fd del socket
 * @return    retorna todo el campo HEADER, incluidos los []
 */
char* CONEXION_leerBufferSocket(int fd){
    int str_l = 0;
    //char end_char = ']';
    char c;
    char * str = NULL;
    str = malloc(sizeof(char));
    if(str != NULL) {
        read(fd, &c, sizeof(char));
        while( c!= ']') {
            str[str_l] = c;
            str_l++;
            read(fd, &c, sizeof(char));
            str = realloc(str, sizeof(char) *(str_l + 1));

        }
        str[str_l] = ']';
        str_l++;
        str = realloc(str, sizeof(char) *(str_l + 1));

        str[str_l] = '\0';

    }

    return str;
}

/**
 * Función para limpiar el buffer del socket. Lee hasta que el read devuelve 0
 * @param socket_fd fd del socket.
 */
void limpiaBuffer(int socket_fd){
    char c;
    ssize_t size;
    do {
        size = read(socket_fd, &c, sizeof(char));
    }while(size != 0);
}

/**
 * Función para leer una trama recibida que no sea imagen. Lee todos los campos de una trama.
 * ¡No utilizar con imagenes!
 * @param  socket_fd fd del socket
 * @return           Devuelve la trama recibida ya tratada en una estructura.
 */
Trama CONEXION_recibeTrama(int socket_fd){
    Trama trama;
    memset(&trama, 0, sizeof(Trama));
    trama.header = NULL;
    read(socket_fd, &(trama.type), sizeof(char));
    trama.header = CONEXION_leerBufferSocket(socket_fd);
    read(socket_fd, &(trama.length), sizeof(uint16_t));
    trama.data = malloc (sizeof(char) * trama.length);

    if(trama.data != NULL){
        read(socket_fd, trama.data, sizeof(char) * trama.length);

    } else {
        trama.data = NULL;
        if (trama.header != NULL) free(trama.header);
        trama.type = TIPONULL;
    }

    return trama;
}

/**
 * Función para recibir una trama de tipo imagen. Lee todos los campos de una trama
 * de tipo imagen.
 * @param  socket_fd fd del socket
 * @return           Devuelve una Trama Imagen.
 */

TramaImagen CONEXION_recibeTramaImagen(int socket_fd){
    TramaImagen trama;
    memset(&trama, 0, sizeof(TramaImagen));
    read(socket_fd, &(trama.type), sizeof(char));
    trama.header = CONEXION_leerBufferSocket(socket_fd);
    read(socket_fd, &(trama.length), sizeof(uint16_t));
    read(socket_fd, &trama.data, sizeof(uint64_t));
    return trama;
}

/**
 * Función para enviar una trama, que no sea ni de tipo Texto ni de tipo Imagen
 * @param  socket_fd fd del socket
 * @param  type      campo TYPE de la trama
 * @param  header    campo HEADER de la trama
 * @param  data      campo DATA de la trama
 * @return           retorna >= 0 si se ha podido enviar correctamente la trama
 */
int CONEXION_enviaTrama(int socket_fd, char type, char * header, char * data){
    int recibe = 0;
    write(socket_fd, &type, sizeof(char));
    unsigned short header_length = strlen(header);
    write(socket_fd, header, header_length);
    uint16_t data_length = strlen(data);
    write(socket_fd, &data_length, sizeof(uint16_t));
    recibe = write(socket_fd, data, strlen(data));
    return recibe;

}

/**
 * Función para enviar una trama de tipo Texto
 * @param  socket_fd fd del socket
 * @param  type      campo TYPE de la trama
 * @param  header    campo HEADER de la trama
 * @param  data      campo DATA de la trama
 * @return           retorna >= 0 si se ha podido enviar correctamente la trama
 */
int CONEXION_enviaTramaTexto(int socket_fd, char type, char * header, char * data, uint16_t tamanyo){
    int recibe = 0;
    recibe = write(socket_fd, &type, sizeof(char));
    unsigned short header_length = strlen(header);
    recibe = write(socket_fd, header, header_length);
    recibe = write(socket_fd, &tamanyo, sizeof(uint16_t));
    recibe = write(socket_fd, data, tamanyo);
    return recibe;

}
/**
 * Función para enviar una trama de tipo Imagen
 * @param  socket_fd fd del socket
 * @param  type      campo TYPE de la trama
 * @param  header    campo HEADER de la trama
 * @param  data      campo DATA de la trama
 * @return           retorna >= 0 si se ha podido enviar correctamente la trama
 */
int CONEXION_enviaTramaImagen(int socket_fd, char type, char * header, unsigned char * data,
                                uint16_t bytes_a_enviar){
    int res = write(socket_fd, &type, sizeof(char));
    unsigned short header_length = strlen(header);
    res = write(socket_fd, header, header_length);
    res = write(socket_fd, &bytes_a_enviar, sizeof(uint16_t));
    res = write(socket_fd, data, sizeof(unsigned char)*bytes_a_enviar);
    return res;
}
