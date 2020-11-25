#include "file.h"

/**
 * Función que convierte un entero (int) a ASCII(char*)
 * @param  val  entero a convertir
 * @param  base base en la que está el entero
 * @return      Devuelve el valor en ASCII del entero
 */
char* itoa(int val, int base){

	static char buf[32] = {0};
	int i = 30;
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	return &buf[i+1];

}
/**
 * Función para escribir datos recibidos en Lionel.
 * @param archivo Información del archivo a escribir en el fichero.
 * @param kalkun  Si kalkun == 1 se escribe el fichero con los datos de las imagenes.
 *                Si kalkun == 0 se escribe el fichero de datos astronomicos.
 */
void FILE_escribirFichero(Archivo archivo, int kalkun) {
    char anyo[5], mes[3], dia[3], hora[3], min[3];
    int i, j;
    int fd_kalkun;

    j= 0;
    for (i = 0; i < 4; i++) {
        anyo[j] = archivo.fecha[i];
        j++;
    }
    anyo[4] = '\0';

    j = 0;
    for (i = 4;i < 6; i++){
        mes[j] = archivo.fecha[i];
        j++;
    }
    mes[2] = '\0';
    j = 0;
    for (i = 6; i < 8; i++) {
        dia[j] = archivo.fecha[i];
        j++;
    }
    dia[2] = '\0';

    j = 0;
    for (i = 9; i < 11; i++) {
        hora[j] = archivo.fecha[i];
        j++;
    }
    hora[2] = '\0';

    j = 0;
    for(i = 11; i < 13; i++){
        min[j] = archivo.fecha[i];
        j++;
    }
    min[2] = '\0';
    char * size = itoa(archivo.tamanyo, 10);

    if (kalkun) {
        fd_kalkun = open("Kalkun.txt", O_CREAT | O_WRONLY | O_APPEND,  0777);
    } else {
        fd_kalkun = open("AstroData.txt", O_CREAT | O_WRONLY | O_APPEND,  0777);
    }
    write(fd_kalkun, anyo, strlen(anyo));
    write(fd_kalkun, "-", strlen("-"));
    write(fd_kalkun, mes, strlen(mes));
    write(fd_kalkun, "-", strlen("-"));
    write(fd_kalkun, dia, strlen(dia));
    write(fd_kalkun, " ", strlen(" "));
    write(fd_kalkun, hora, strlen(hora));
    write(fd_kalkun, ":", strlen(":"));
    write(fd_kalkun, min, strlen(min));
    write(fd_kalkun, " ", strlen(" "));
    write(fd_kalkun, size, strlen(size));
    write(fd_kalkun, " bytes\n", strlen(" bytes\n"));
    close(fd_kalkun);
}

void FILE_escribirFicheroDatosAstro(char * data) {
    int fd_datos;
    if(strcmp(data, "") != 0){
        fd_datos = open("AstroData.txt", O_CREAT | O_WRONLY | O_APPEND,  0777);
        write(fd_datos, data, strlen(data));
        write(fd_datos, "\n", strlen("\n"));
    }
}

/**
 * Funcion que convierte un numero decimal ASCII en float. Además deja el puntero
 * actualizado de por donde se iba en esa cadena de carácteres.
 * @param  archivo   cadena de caracteres que contiene el numero en deciamal en ASCII
 * @param  i         puntero de la cadena de caracteres "archivo"
 * @param  delimiter caracter hasta el que se quiere convertir el número decimal
 * @return           valor en float del valor en ASCII
 */
float atof_contelaciones(char * archivo, long int * i, char delimiter){
    float num, decimal;
    num = decimal = 0;
    double div = 0;
    int negativo = 0;

    while(archivo[*i] != '.'){
        if (archivo[*i] == '-') negativo = 1;
        else num = num * 10 + archivo[*i] - '0';
        (*i)++;
    }
    (*i)++;
    while(archivo[*i] != delimiter){
        decimal = decimal * 10 + archivo[*i] - '0';
        (*i)++;
        div++;
    }
    num += decimal / pow(10.0, div);
    if(negativo) num *= -1;
    return num;
}

typedef char TipoConst[4];
/**
 * Función para obtener los datos de las constelaciones de los ficheros de texto recibididos
 * @param  archivo contiene todo el texto del archivo de constelaciones.
 * @param  tamano  tamaño del archivo.
 * @return         [description]
 */
Mensaje FILE_getDatosContelaciones(char * archivo, long tamano){
    Mensaje msg;

    TipoConst * constelaciones = NULL;
    TipoConst tipo_const = {"   "};
    int i_const = 0;
    int num_const = 0;
    float magnitud = 0.0;
    int j = 0;
    int existe = 0;

    msg.max_valor_magnitud = -1;
    msg.min_valor_magnitud = 99999999999;
    msg.num_constelacions = 0;
    msg.num_constelacions_diferentes = 0;;
    msg.suma_densitat = 0.0;

    constelaciones = malloc(sizeof(TipoConst));
    if (constelaciones != NULL){
        long i = 0;
        //int max = tamano;
        while (i < tamano) {
            //leemos el codigo de la constelacion
            while(archivo[i] != ' '){
                tipo_const[i_const] = archivo[i];
                i_const++;
                i++;
            }
            //i_const++;
            i++; //quedar en el 0
            tipo_const[3] = '\0';
            i_const = 0;

            //realloc de contelaciones para encontrar el numero de const diferentes
            if (num_const == 0){
                strcpy(constelaciones[num_const], tipo_const);
                num_const++;
            }
            else{
                existe = 0;
                for(j = 0; j < num_const; j++){
                    if (strcmp(constelaciones[j], tipo_const) == 0){
                        existe = 1;
                        break;
                    }
                }
                if(existe == 0){
                    constelaciones = realloc(constelaciones, sizeof(TipoConst)*(num_const+1));
                    if (constelaciones != NULL) {
                        strcpy(constelaciones[num_const], tipo_const);
                        num_const++;
                    }
                }
            }
            //leer la densitat
            msg.suma_densitat += atof_contelaciones(archivo, &i, ' ');
            i++;
            //leer la magnitud
            magnitud = atof_contelaciones(archivo, &i, '\n');

            if(magnitud < msg.min_valor_magnitud){
                msg.min_valor_magnitud = magnitud;
            }
            if(magnitud > msg.max_valor_magnitud){
                msg.max_valor_magnitud = magnitud;
            }
            msg.num_constelacions++;
            i++;
        }
        msg.num_constelacions_diferentes = num_const;
		free(constelaciones);
        return msg;
    }
    write(1, "No se pudo pedir memoria para las constelaciones\n", strlen("No se pudo pedir memoria para las constelaciones\n"));
    msg.num_constelacions = -1;
    return msg;
}

/**
 * Función para inicializar una estructura dinámica (lista) para guardar los
 * archivos que le llegan a Lionel
 * @param carpeta Sirve para inicializar la carpeta.
 */
void FILE_initCarpeta(Carpeta * carpeta){
    carpeta->archivos = NULL;
    carpeta->num_archivos = 0;
}

/**
 * Función que añade un archivo a la lista (carpeta).
 * @param carpeta Estructura con todos los archivos
 * @param archivo Archivo nuevo a añadir a la Carpeta
 */
void FILE_anadeArchivo(Carpeta * carpeta, Archivo archivo){
    if(carpeta->num_archivos != 0){
        int num_nuevo = carpeta -> num_archivos + 1;
        carpeta->archivos = realloc(carpeta->archivos, sizeof(Archivo) * num_nuevo);

        if(carpeta->archivos != NULL){
            carpeta->archivos[carpeta -> num_archivos] = archivo;
            carpeta -> num_archivos = num_nuevo;
        }else{
            MSSG_errorRealloc();
        }

    }else{
        carpeta -> archivos = malloc (sizeof(Archivo));
        if(carpeta -> archivos){
            carpeta->archivos[carpeta -> num_archivos] = archivo;
            carpeta -> num_archivos += 1;
        }else{
            MSSG_errorMalloc();
        }
    }
}
/**
 * "Destructor" de la lista Carpeta
 * @param carpeta Lista de archivos (carpeta)
 */
void FILE_destruyeCarpeta(Carpeta * carpeta){
    unsigned int i;
    if (carpeta->archivos != NULL){
        for (i = 0; i < carpeta->num_archivos; i++){
            free(carpeta->archivos[i].fecha);
            free(carpeta->archivos[i].tipo_archivo);
            free(carpeta->archivos[i].datos);
        }
        free(carpeta->archivos);
		carpeta->archivos = NULL;
    }
}

void FILE_pintaArchivo(Archivo a){
    char tamano[60];

    write(1, "\n--------  Archivo --------\n", strlen("\n--------  Archivo --------\n"));
    write(1, "Tipo: ", strlen("Tipo: "));
    write(1, a.tipo_archivo, 4);
    write(1, "\nFecha: ", strlen("\nFecha: "));
    write(1, a.fecha , strlen(a.fecha));
    sprintf(tamano,"\nTamano: %lu", a.tamanyo);
    write(1, tamano, strlen(tamano));
    write(1, "\nDatos: ", strlen("\nDatos: "));
    write(1, a.datos, 50);
    write(1, "\n\n", 2);
}
