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
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>

#include "utils.semaforo.h"
#include "utils.validaciones.c"
#include "utils.colaDinamica.c"


#define BUFFERSIZE 1000


struct s_datosCliente {
	int id;
	int socket;
	char *ip;
	int activo;
	int jugando;
};

struct s_datosPartida {
	int idCliente1;
	int socketCliente1;
	int idCliente2;
	int socketCliente2;
	int flag_partidaViva;
};



void imprimirError(int codigo, const char *msg);
void *leeCliente(void *argumentos);
void *enviaCliente();

