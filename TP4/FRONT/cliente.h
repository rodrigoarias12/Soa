#include "SDL/SDL.h"
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
  int x,y;
}t_coordenadas;

/*Estructura a dibujar*/
typedef struct{
  int codigoPaquete;
  t_coordenadas coordenadas;
}t_paquete;
/**/

/*Definición*/

int bRun = 1;
t_config_cli config;
SDL_Event event;
SDL_Rect pantallaPrincipal;
SDL_mutex *mtx;
int caller_socket =0;

/*Sección Crítica*/
t_paquete miPaquete;
/*Sección Crítica*/

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
const char SPRITES_PORTADA[] = "./sprites/portada.bmp";
// Imagenes ambiente
const char SPRITES_AMBIENTE[] = "./sprites/ambiente/";

// Imagenes ave
const char SPRITES_AVE[] = "./sprites/ave/";

// Imagenes edificio
const char SPRITES_EDIFICIO_CUERPO[] = "./sprites/edificio/cuerpo.bmp";
const char SPRITES_EDIFICIO_TECHO[] = "./sprites/edificio/techo.bmp";

// Imagenes felix
const char SPRITES_FELIX[] = "./sprites/felix/felix.bmp";

// Imagenes puerta
const char SPRITES_PUERTA_1[] = "./sprites/puerta/puerta1.bmp";
const char SPRITES_PUERTA_2[] = "./sprites/puerta/puerta2.bmp";
const char SPRITES_PUERTA_3[] = "./sprites/puerta/puerta3.bmp";
const char SPRITES_PUERTA_4[] = "./sprites/puerta/puerta4.bmp";

// Imagenes ralph
const char SPRITES_RALPH[] = "./sprites/ralph/";

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

// Declaramos todas las partes graficas
SDL_Surface *screen,
	    *portada,
            *jugadores[2],
            *edificio,
            *puertas[4],
            *ventanas[3],
            *ventanaGrande[2];


int cargarConfigCliente(t_config_cli *);
int extraerNumero(char *);
int validarNumero(char *);
int validarNumeroIP(char *);
char * extraerNumeroIP(char *);
int extraerTecla(char *);
int recibirDatos(void *);
int enviarDatos(void *);
void finalizar(void);
void imprimirError(int);
void dibujarSprite(SDL_Surface *, int , int );
void inicializarSprite(SDL_Surface *, const char *);
void inicializar(void);


/*Implementación*/
/*Me encargo de recibir los datos enviados desde el servidor de partida*/

int recibirDatos(void * n){
 
  while(true){
    printf("Estoy recibiendo datos!\n");
    if(recv(caller_socket,&t_paquete,sizeof(t_paquete),0)<0){
      imprimirError(4);
    }
    /*RECIBO LOS DATOS*/
    SDL_mutexP(mtx);
    switch(miPaquete.codigoPaquete){
      case 0: break;
      case 1: break;
      case 2: break;
      case 3: break;
      case 4: break;
    }
    SDL_mutexV(mtx);
    sleep(5);
  }
}

/**
 TODO
 Mensajes:
 cod: 0 - Comienzo de la partida.
 cod: 1 - Actualización Jugadores.
 cod: 2 - Reinicio de partida.
 cod: 3 - Fin de la partida.
 cod: 4 - Fin de torneo.
 cod: 5 - 
 */
int enviarDatos(void* n){	
  while(true){	
	printf("Estoy Escribiendo datos!\n");
	sleep(5);
  }
}

void imprimirError(int codigo){
  switch(codigo){
    case 0: 
       printf("No se pudo iniciar SDL: %s\n",SDL_GetError());
      break;
    case 1: 
       printf("Error en la carga de los parámetros\n");
      break;
    case 2: 
      printf("No se puede inicializar el modo gráfico: \n",SDL_GetError());
      break;
    case 3:
      printf("Error en al creación del semáforo mutex\n");
      break;
    case 4:
      printf("Se perdión la conexión\n");
      break;
  }
  exit(0);
}

int cargarConfigCliente(t_config_cli *conf){
	char aux[30];	
	int intAux;
	char auxChar[30];
	FILE *ARTXT;
	
	ARTXT = fopen("config_cliente","r+");
	if(!ARTXT)
		return 0;
	/*Leo la Ip del Servidor*/
		fscanf(ARTXT,"%s",aux);
		strcpy(auxChar, extraerNumeroIP(aux));
		if(auxChar != NULL)
		{		
			strcpy(conf->ip, auxChar); 
			printf("La IP es: %s\n",conf->ip);
		}
		else
			return 0;
	/*Leo el Puerto*/		
		fscanf(ARTXT,"%s",aux);
		intAux = extraerNumero(aux);
		if(intAux)
		{				
			conf->port = intAux; 
			printf("El Puerto es: %d\n",conf->port);
		}
		else
			return 0;

	/*Leo las teclas*/
	/*Leo KEYUP*/		
		fscanf(ARTXT,"%s",aux);
		intAux = extraerTecla(aux);		
		if(intAux != -1)
		{						
			conf->k_up = intAux; 
			printf("Tecla UP: %d\n",conf->k_up);
		}
		else
			return 0;
	/*Leo KEYDOWN*/		
		fscanf(ARTXT,"%s",aux);
		intAux = extraerTecla(aux);
		if(intAux != -1)
		{						
			conf->k_down = intAux; 
			printf("Tecla DOWN: %d\n",conf->k_down);
		}
		else
			return 0;
	/*Leo KEYLEFT*/		
		fscanf(ARTXT,"%s",aux);
		intAux = extraerTecla(aux);
		if(intAux != -1)
		{						
			conf->k_left = intAux; 
			printf("TECLA Left: %d\n",conf->k_left);
		}
		else
			return 0;
	/*Leo KEYRIGHT*/		
		fscanf(ARTXT,"%s",aux);
		intAux = extraerTecla(aux);
		if(intAux != -1)
		{						
			conf->k_right =intAux; 
			printf("TECLA Right: %d\n",conf->k_right);
		}
		else
			return 0;

	fclose(ARTXT);
	return 1;
	
}

int extraerNumero(char *cad)
{
	char *aux;	
	aux = strchr(cad,':');	
	if(*aux)
	{
		aux++;		
		if(validarNumero(aux))
		{			
			return atoi(aux); //retorno el valor encontrado//	
		}
	}
return -1;
}

int validarNumero(char *num)
{
	
	while(*num)
	{
		if(*num < '0' || *num > '9')	
			return 0;		
		
		num++;
	}		
	return 1;
}

int validarNumeroIP(char *num)
{
	while(*num)
	{	
		if((*num < '0' || *num > '9') && *num != '.')	
			return 0;
		
		num++;
	}		
	return 1;
}

char * extraerNumeroIP(char *cad)
{
	char *aux;	
	aux = strchr(cad,':');	
	if(*aux)
	{
		aux++;		
		if(validarNumeroIP(aux))
		{			
			return aux; //retorno el valor encontrado//	
		}
	}
return NULL;
}

int extraerTecla(char *cad)
{
	char *aux;	
	aux = strchr(cad,':');	
	if(*aux){
		aux++;
		if(strlen(aux) > 1)
			{
				if(!strcmp(aux,"ARRIBA"))
					return SDLK_UP;
				if(!strcmp(aux,"ABAJO"))
					return SDLK_DOWN;
				if(!strcmp(aux,"IZQUIERDA"))
					return SDLK_LEFT;
				if(!strcmp(aux,"DERECHA"))
					return SDLK_RIGHT;
			}
		else{
			if(*aux >= 'A' && *aux <= 'Z')
				return *aux+32;
			
			return *aux;
		}
	}
    return -1;
}

void apply_surface( int x, int y, SDL_Surface* elemento, SDL_Surface* pantalla )
{
    SDL_Rect coordenadas;

    coordenadas.x = x;
    coordenadas.y = y;

    SDL_BlitSurface( elemento, NULL, pantalla, &coordenadas );
}

void finalizar(void){
  printf("Finalizando...\n");
  SDL_Quit();
  SDL_DestroyMutex(mtx);
}

void inicializar(){
   // Se cargan todos los sprites necesarios
      inicializarSprite(edificio, SPRITES_EDIFICIO_CUERPO);
      inicializarSprite(ventanas[0], SPRITES_VENTANA_1);
      inicializarSprite(ventanas[1], SPRITES_VENTANA_2);
      inicializarSprite(ventanas[2], SPRITES_VENTANA_3);
      inicializarSprite(ventanaGrande[0], SPRITES_VENTANA_GRANDE_1);
      inicializarSprite(ventanaGrande[1], SPRITES_VENTANA_GRANDE_2);
      inicializarSprite(puertas[0], SPRITES_PUERTA_1);
      inicializarSprite(puertas[1], SPRITES_PUERTA_1);
      inicializarSprite(puertas[2], SPRITES_PUERTA_1);
      inicializarSprite(puertas[3], SPRITES_PUERTA_1);
      inicializarSprite(jugadores[0], SPRITES_FELIX);
      inicializarSprite(jugadores[1], SPRITES_FELIX);
      
      dibujarSprite(jugadores[0], 125, 365);
      dibujarSprite(jugadores[1], 430, 365);
      dibujarSprite(edificio, 60, 0);
      dibujarSprite(puertas[0], 270,350);
      dibujarSprite(ventanaGrande[0], 270, 270);
      dibujarSprite(ventanas[0], 130, 365);
      dibujarSprite(ventanas[1], 210, 365);
      dibujarSprite(ventanas[2], 360, 365);
      dibujarSprite(ventanas[1], 440, 365);

      SDL_FillRect(screen, NULL, 0x224487); 
}

void dibujarSprite(SDL_Surface *sprite, int x, int y){
  SDL_Rect posicion;
  posicion.x = x;
  posicion.y = y;
  
  SDL_BlitSurface(sprite, NULL, screen, &posicion);
}

void inicializarSprite(SDL_Surface *sprite, const char *path){
  sprite = SDL_LoadBMP(path);
  sprite->format->Amask = 0xFF000000;
  sprite->format->Ashift = 24;
  
  SDL_SetColorKey(sprite, SDL_SRCCOLORKEY, SDL_MapRGB(sprite->format, 255,0,255));
}