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


#include "utils.semaforo.h"
#include "utils.validaciones.c"


#define BUFFERSIZE 1000
#define MAXCONEXIONES 100
#define EJECUTABLEPARTIDA "./partida.exe"
#define MAX_JUGADORES_PANTALLA 10


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

typedef struct{
	int x,y,w,h;
}t_coordenadas;

typedef struct{
	t_coordenadas nombre;
	t_coordenadas puntos;
}t_filaTabla;

void imprimirError(int codigo, const char *msg);
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
char ** generaParametrosPartida(int, int, int, int, int, int,int,int);
void cargaVectorPartidas(int ,int);
void creaPartida(int,int);
void reLanzarPartida(int,int,int);
void sigchld_handler(int signal);
int cargarConfiguracion(int*, int*, int*);
void exit_handler(int signal);
void *dibujarTabla(void *);  //Encargada de dibujar la pantalla
void *dibujarTiempoTorneo(void *); //Se encarga de dibujar el tiempo
void *dibujarContenidoTabla(void *); //Se encarga de dibujar la partida
void inicializarPosicionesTorneo(void); //Inicializa las posiciones fijas de los conectados al torneo
void ShellSort(int *, int ); //Ordena los jugadores
void bubbleSort(int *, long);


SDL_Surface *ventana, *texto, *titulo;
SDL_mutex *mtx;
t_filaTabla tablaJugadores[MAX_JUGADORES_PANTALLA];
