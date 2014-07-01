#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mutex.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct{
	char ip[30];
	int port;
	int k_up;
	int k_down;
	int k_left;
	int k_right;
}t_config_cli;

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

/**/

/*Definición*/

int bRun = 1;
t_config_cli config;
SDL_Event event;
SDL_Rect pantallaPrincipal;
SDL_mutex *mtx;
int caller_socket =0;

int factorVelocidadLadrillo = 1;
int factorVelocidadGaviota = 1;

int velocidadGaviota = 1;
int velocidadLadrillo = 1;

int tiempoTorneo = 0;
int tiempoPartida = 0;
int enviar = 0;

//despues lo sacamos por ahor qued asi
// SDL_Rect pajaroCoordenadas1,
// 	 pajaroCoordenadas2,
// 	 pajaroCoordenadas3;

/*Sección Crítica*/
t_paquete miPaquete;
/*Sección Crítica*/

t_coordenadas coordenadasLadrillos[2];
t_coordenadas coordenadasGaviotas[2];

int fuiAceptado;

//Resolucion de pantalla
const int SCREEN_ANCHO = 640;
const int SCREEN_ALTO = 480;
const int SCREEN_BPP = 32;

// Directorios generales
const char SPRITES_AMBIENTE_DIR[] = "./sprites/ambiente/";
const char SPRITES_AVE_DIR[] = "./sprites/ave/";
const char SPRITES_EDIFICIO_DIR[] = "./sprites/edificio/";
const char SPRITES_FELIX_DIR[] = "./sprites/felix/";
const char SPRITES_PUERTA_DIR[] = "./sprites/puerta/";
const char SPRITES_RALPH_DIR[] = "./sprites/ralph/";
const char SPRITES_VENTANA_DIR[] = "./sprites/ventana/";

// Imagen icono
const char SPRITES_ICONO[] = "./sprites/icon.bmp";
const char SPRITES_PORTADA[] = "./sprites/portadas/portada.bmp";
const char SPRITES_ERROR_CONEXION[] = "./sprites/portadas/error_de_conexion.bmp";
const char SPRITES_ESPERANDO_OPONENTE[] = "./sprites/portadas/esperando_oponente.bmp";
const char SPRITES_GANADO[] = "./sprites/portadas/ganado.bmp";
const char SPRITES_NO_FUISTE_ACEPTADO[] = "./sprites/portadas/no_fuiste_aceptado.bmp";
const char SPRITES_PERDIDO[] = "./sprites/portadas/perdido.bmp";
const char SPRITES_LADRILLO[] = "./sprites/ladrillo/ladrillo.bmp";

// Imagenes ambiente
const char SPRITES_AMBIENTE[] = "./sprites/ambiente/";

// Imagenes ave
const char SPRITES_AVE[] = "./sprites/ave/swallow.bmp";

// Imagenes edificio
const char SPRITES_EDIFICIO_CUERPO_1[] = "./sprites/edificio/cuerpo1.bmp";
const char SPRITES_EDIFICIO_CUERPO_2[] = "./sprites/edificio/cuerpo2.bmp";
const char SPRITES_EDIFICIO_CUERPO_3[] = "./sprites/edificio/cuerpo3.bmp";

// Imagenes edificio terminado
const char SPRITES_EDIFICIO_TERMINADO_1[] = "./sprites/edificio/terminado1.bmp";
const char SPRITES_EDIFICIO_TERMINADO_2[] = "./sprites/edificio/terminado2.bmp";

// Imagenes felix
const char SPRITES_FELIX[] = "./sprites/felix/felix.bmp";

// Imagenes puerta
const char SPRITES_PUERTA_1[] = "./sprites/puerta/puerta1.bmp";
const char SPRITES_PUERTA_2[] = "./sprites/puerta/puerta2.bmp";
const char SPRITES_PUERTA_3[] = "./sprites/puerta/puerta3.bmp";
const char SPRITES_PUERTA_4[] = "./sprites/puerta/puerta4.bmp";

// Imagenes ralph
const char SPRITES_RALPH[] = "./sprites/ralph/ralph.bmp";

// Imagenes ventana
const char SPRITES_VENTANA_1[] = "./sprites/ventana/ventana1.bmp";
const char SPRITES_VENTANA_2[] = "./sprites/ventana/ventana2.bmp";
const char SPRITES_VENTANA_3[] = "./sprites/ventana/ventana3.bmp";
const char SPRITES_VENTANA_GRANDE_1[] = "./sprites/ventana/ventana_grande1.bmp";
const char SPRITES_VENTANA_GRANDE_2[] = "./sprites/ventana/ventana_grande2.bmp";
const char SPRITES_VIDRIO[] = "./sprites/ventana/vidrio5.bmp";
const char SPRITES_VIDRIO_ROTO_1[] = "./sprites/ventana/vidrio1.bmp";
const char SPRITES_VIDRIO_ROTO_2[] = "./sprites/ventana/vidrio2.bmp";
const char SPRITES_VIDRIO_ROTO_3[] = "./sprites/ventana/vidrio3.bmp";
const char SPRITES_VIDRIO_ROTO_4[] = "./sprites/ventana/vidrio4.bmp";

// Imagenes de informacion
const char SPRITES_INFO_BARRA_SUPERIOR[] = "./sprites/info/barra_superior.bmp";


// Declaramos todas las partes graficas
SDL_Surface *screen,
*portada,
*rechazado,
*aceptado,
*jugadores[2],
*edificios[5],
*puertas[4],
*ventanasTipo1[10],
*ventanasTipo2[10],
*ventanasTipo3[10],
*vidrios[40],
*vidriosRotos1[40],
*vidriosRotos2[40],
*vidriosRotos3[40],
*vidriosRotos4[40],
*ventanasGrandes[2],
*ladrillos[3],
*vidas[3],
*score,
*ralph,
*felix[2],
*gaviota,
*pajaros[3],
*barraSuperior;

SDL_Rect jugador1Coordenadas;
int codigoEdificio = 0;
TTF_Font *fuente2 = NULL;
SDL_Surface *texto2;
SDL_Rect contenedorTexto2;

int cargarConfigCliente(t_config_cli *);
int extraerNumero(char *);
int validarNumero(char *);
int validarNumeroIP(char *);
char * extraerNumeroIP(char *);
int extraerTecla(char *);
int recibirDatos(void *);
int dibujar(/*void **/);
void finalizar(void);
void imprimirError(int);
void inicializar(SDL_Surface *);
void dibujarSprite(SDL_Surface *, int , int, SDL_Surface *);
SDL_Surface *inicializarSprite(const char *);
void mover_pajaros(void);
void dibujarVentanas(int completo = 0);
void dibujarVidrios(SDL_Surface *);
void incrementarNivel(void);
void cambiarVelocidadLadrillos(int);
void cambiarVelocidadGaviotas(int);
void inicializar(void);
void dibujarSiguienteNivel();
void dibujarTecho(SDL_Surface *);
