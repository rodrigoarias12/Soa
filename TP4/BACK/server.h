/*#####################################
#Trabajo Practico Nº4
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#Garcia Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

#include "utils.validaciones.c"

#define BUFFERSIZE 1000

void help();
void imprimirError(int codigo, const char *msg);
void armarEsqueletoDemonio();
void *manejadorCliente(void *argumentos);
void terminarServer(int signal);

//Socket servidor
int sockFileDescriptor; //Contiene los I/O Streams
