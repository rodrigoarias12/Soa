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
#include <sys/prctl.h>

#include "utils.semaforo.c"
#include "utils.validaciones.c"
#include "utils.colaDinamica.c"


struct s_datosCliente {
	int id;
	int socket;
	char *ip;
	char nombre[25];
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

struct msjDeCliente {
	int codigo;
	int nroJugador;
};


void sigint_handler(int signal);
void sighup_test(int signal);
void imprimirError(int codigo, const char *msg);
void *leeCliente(void *argumentos);
void *procesamientoMensajes();
void *verificaEstadoServer();
void enviaCliente(void* paqAux);




typedef struct{
  int x,y, height, width;
}t_coordenadas;

typedef struct{
  int roto;
  t_coordenadas coordenadas;
}t_vidrio;

typedef struct{
	int dibujar;
	t_coordenadas coordenadas;
}t_torta;

typedef struct{
  int vidas;
  int puntos;
  char nombre[25];
  t_coordenadas coordenadas;
}t_jugador;


typedef struct{
	int codigoPaquete;
	int nroJugador;
	int nivel;
	int tiempo;
	t_jugador jugadores[2];
	t_coordenadas ralph;
	t_coordenadas ladrillos[3];
	t_coordenadas gaviotas[3];
	t_vidrio vidrios[40];
	int marquesina[3];
	t_torta torta;
	int idPaquete;
}t_paquete; /*Estructura a dibujar*/

//Jugadores
int numeroJugador1 = 1;
int numeroJugador2 = 2;

//Marquesinas
int hilera, ventana, res;
t_coordenadas mUno, mDos, mTres;
t_coordenadas posicionesMarquesinas[2][5];
int marquesinas = 0;
//variables torta
int tortaComida = 0;
int primeraTorta = 1;
int ventanasArregladasParaTorta = 0;
int cantidadVidriosParaTorta = 5;

//movimiento de ralph en x
int vectorderalph[3]={120,250,400};
//mov pajaros
int mov_paj1;
int mov_paj2;
int mov_paj3;
//mov ladrillos
int mov_lad1;
int mov_lad2;
int mov_lad3;

int matrizladrillos[3][3]={{120,140,200},{220,260,300},{420,450,480}};
int movimiento;

///lo agrego jorge no entiendo por que
int tipoEdificio = 0;
int partidaPrimeraVez = 1;
//matriz de ventanas
int ventanasParesRotas[20][2];
int ventanasAReparar = 0;
int VentanasArregladas =0;


int chequearPosicion(t_jugador, int, int);
void moverJugador(int codigo, int numJugador);
int colision(int, int, int, int, int, int, int, int);
int arregloVentana(int);
void inicializar();
void generarMarquesinasRandom();
void inicializar_marquesinas();
void setearPosicionMarquesinasParaComparar();
void dibujarVidrios(int completo);
void movimientoPajarosLadrillosRalph(int nivel);
void colisicionPajaros();
void colisicionLadrillos();
//seba
void colisionTorta();
void setearCoordenadasTorta();
