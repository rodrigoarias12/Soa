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


#define BUFFERSIZE 1000
#define MAXCONEXIONES 100


struct s_datosCliente {
	int id;
	int socket;
	char *ip;
	int activo;
	int jugando;
};


void imprimirError(int codigo, const char *msg);
void terminarServer(int signal);
void conectarServidor(struct sockaddr_in *serv_address, int *sockFileDescriptor, int *portNumber);
void *aceptaConexiones();
void *armaPartidas();
int sumatoriaPartidas(const int num);
void partidasRandom();
void inicializaVector(int **,int);
int sumatoriaPartidas(int);
void cierraClientes();