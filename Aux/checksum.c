#include "checksum.h"

/**
 * Función que comprueba si el checksum recibido en la trama es igual al checksum
 * que se genera del archivo recibido.
 * @param  nombre_archivo direccion de donde está el archivo recibido
 * @param  checksum_trama checksum recibido
 * @return                1 si los checksums son iguales, 0 en caso contrario.
 */

int CHECKSUM_compruebaChecksum(char * nombre_archivo, unsigned char * checksum_trama) {
    char checksum[33];
    int status;
    int return_value = 0;
    int fd[2];
    if(pipe(fd) == -1) exit(-1);
    switch (fork()) {
        case 0:
            //es el hijo y hace el execl
            dup2 (fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execl("/usr/bin/md5sum", "md5sum",(char *const) nombre_archivo, (char*const)NULL);
            exit(0);
        default:
            close(fd[1]);
            read(fd[0], checksum, sizeof(checksum));
            checksum[32] = '\0';

            wait(&status);

            if(strcmp((char*)checksum_trama, checksum) == 0) return_value = 1;
            else return_value = 0;

            break;
    }
    return return_value;
}
