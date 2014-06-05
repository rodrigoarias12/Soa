#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MINIMO_PARAM 7

/*Variables Globales*/
int cantidadTotalDeArchivos = 0;
int procesosTotales = 0;
int procesosActivos = 0;
int procesosEsperados = 0;

int *pids;

void help();
void imprimirError(int);
int validarDirectorio(char *);
int validarEntero(char *);
void calcularCantidadArchivos(char *);
char ** cargarArchivosEnVector(char *);
void avisarFinalizacion(int);