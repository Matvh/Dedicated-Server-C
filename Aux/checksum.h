#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * [CHECKSUM_compruebaChecksum]
 *
 * PRE: Nombre archivo y Checksum_trama != NULL
 * POST: 1 si los checksums coinciden, 0 en caso contrario.
 */
int CHECKSUM_compruebaChecksum(char * nombre_archivo,unsigned char * checksum_trama);

#endif
