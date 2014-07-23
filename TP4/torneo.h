/*#####################################
#Trabajo Practico N 4
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
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <SDL/SDL_mutex.h>

#include "utils.semaforo.c"
#include "utils.validaciones.c"


#define BUFFERSIZE 1000
#define MAXCONEXIONES 100
#define EJECUTABLEPARTIDA "./partida.exe"
#define MAX_JUGADORES_PANTALLA 10
//Codigo de mensajes de envio al cliente
#define DISCONNECT 7
#define FIN_TORNEO 8


struct s_datosCliente {
	int id;
	int socket;
	char *ip;
	char nombre[25]; //TODO Debe recibir el nombre del jugador
	int activo;
	int jugando;
	int puntos;
};

struct s_datosPartida {
	int pidPartida;
	int idCliente1;
	int socketCliente1;
	int puntosCliente1;
	int idCliente2;
	int socketCliente2;
	int puntosCliente2;
	int flag_partidaViva;
};


void imprimirError(int codigo, const char *msg);
void sigchld_handler(int signal);
void exit_handler(int signal);
void terminarServer(int signal);

void conectarServidor(struct sockaddr_in *serv_address, int *sockFileDescriptor, int *portNumber);
void *aceptaConexiones();
void *armaPartidas();
void *verificaEstadoPartidas();
int sumatoriaPartidas(const int num);
void partidasRandom();
void inicializaVector(int **,int);
int sumatoriaPartidas(int);
void cierraClientes();
char** generaParametrosPartida(int, int, int, int, int, int, int, int, int, int, int);
void cargaVectorPartidas(int ,int);
void creaPartida(int,int);
void reLanzarPartida(int,int,int);
int cargarConfiguracion(int*, int*, int*);

void *dibujarTabla(void *);  //Encargada de dibujar la pantalla
void *dibujarTiempoTorneo(void *); //Se encarga de dibujar el tiempo
void *dibujarContenidoTabla(void *); //Se encarga de dibujar la partida
void inicializarPosicionesTorneo(void); //Inicializa las posiciones fijas de los conectados al torneo
void shellSort (int *);
void bubbleSort (int *, long);


typedef struct{
	int pidId;
	int sem1;
} t_semaforo;

typedef struct {
	int x,y,w,h;
} t_coordenadas;

typedef struct {
	t_coordenadas nombre;
	t_coordenadas puntos;
} t_filaTabla;

SDL_Surface *ventana, *texto, *titulo;
SDL_mutex *mtx;
t_filaTabla tablaJugadores[MAX_JUGADORES_PANTALLA];
