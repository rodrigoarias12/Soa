#include "SDL/SDL.h"
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
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mutex.h>


#define BUFFERSIZE 1000
#define MAXCONEXIONES 100

typedef struct{
  int x,y, height, width;
}t_coordenadas;

typedef struct{
  int vidas;
  t_coordenadas coordenadas;
}t_jugador;

/*Estructura a dibujar*/
typedef struct{
  int codigoPaquete;
  int nivel;
  int tiempo;
  t_jugador jugador1,jugador2;
  t_coordenadas ralph;
  t_coordenadas ladrillos[3];
  t_coordenadas gaviotas[3];
  t_coordenadas vidrios[40];
}t_paquete;

t_paquete miPaquete;

struct s_datosCliente {
	int id;
	int socket;
	char *ip;
};


void imprimirError(int codigo, const char *msg);
void terminarServer(int signal);
void conectarServidor(struct sockaddr_in *serv_address, int *sockFileDescriptor, int *portNumber);
void dibujarVidrios(int completo = 0);


SDL_mutex *mtx;
int comm_socket = 0;
int comm_socket2=0;

int conectados=0;
int flagTiempo=1;
int sockFileDescriptor; //Contiene los I/O Streams
struct s_datosCliente v_datosCliente[2];
int moverJugador1(void * n);
int moverJugador2(void * n);

int mov_paj1=random();
int mov_paj2=random();
int mov_paj3=random();

int tipoEdificio = 0;