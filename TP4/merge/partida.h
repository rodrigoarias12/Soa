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
  int roto;
  t_coordenadas coordenadas;
}t_vidrio;

typedef struct{
  int vidas;
  int puntos;
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
  t_vidrio vidrios[40];
}t_paquete;


t_paquete miPaquete;

void imprimirError(int codigo, const char *msg);
void terminarServer(int signal);
void conectarServidor(struct sockaddr_in *serv_address, int *sockFileDescriptor, int *portNumber);
void dibujarVidrios(int completo = 0);
void colisicionPajaros();
void colisicionLadrillos();
void inicializar();
void movimientoPajarosLadrillosRalph(int nivel);

SDL_mutex *mtx;
int comm_socket = 0;
int comm_socket2=0;

int conectados=0;
int flagTiempo=1;
int sockFileDescriptor; //Contiene los I/O Streams
//struct s_datosCliente v_datosCliente[2];
int moverJugador1(void * n);
int moverJugador2(void * n);
int arregloVentana(int);
int colision(int ,int ,int,int ,int,int,int,int);

//movimiento de ralph en x
int vectorderalph[3]={120,250,400};
//mov pajaros
int mov_paj1=random();
int mov_paj2=random();
int mov_paj3=random();
//mov ladrillos
int mov_lad1=random();
int mov_lad2=random();
int mov_lad3=random();

///lo agrego jorge no entiendo por que 
int tipoEdificio = 0;
int partidaPrimeraVez = 1;
int partidaActiva = 1;
// tampoco entiendo
int jugadorUnoJugando = 1;
int jugadorDosJugando = 1;
//matriz de ventanas
int ventanasParesRotas[20][2];