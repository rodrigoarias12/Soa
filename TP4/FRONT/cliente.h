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
  int x,y,vidas;
}t_jugador;

typedef struct{
  int x,y;
}t_ralph;

typedef struct{
  int x,y;
}t_ventana;

typedef struct{
  int x,y;
}t_vidrio;

/*Estructura a dibujar*/
typedef struct{
  int codigoPaquete;
  int nivel;
  t_jugador jug1, jug2;
  t_ralph ralph;
  t_ventana ventanas[20];
  t_vidrio vidrios[50];
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
            *jugador1,
	    *jugador2,
            *edificio,
            *puerta1,
            *puerta2,
            *puerta3,
            *puerta4,
            *ventana1,
            *ventana2,
            *ventana3,
            *ventanaGrande1,
            *ventanaGrande2;

SDL_Rect jugador1Coordenadas,
	 jugador2Coordenadas,
         edificioCoordenadas,
         ventana1Coordenadas,
         ventana2Coordenadas,
         ventana3Coordenadas,
         ventana4Coordenadas,
         puerta1Coordenadas,
         puerta2Coordenadas,
         puerta3Coordenadas,
         puerta4Coordenadas,
         ventanaGrande1Coordenadas,
         ventanaGrande2Coordenadas;


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


/*Implementación*/
/*Me encargo de recibir los datos enviados desde el servidor de partida*/

int recibirDatos(void * n){
 
  while(true){
    printf("Estoy recibiendo datos!\n");
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
    sleep(2);
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

void dibujar(SDL_Surface *screen){
  
}

void inicializar(){
   // Se cargan todos los sprites necesarios

      edificio = SDL_LoadBMP(SPRITES_EDIFICIO_CUERPO);
      ventana1 = SDL_LoadBMP(SPRITES_VENTANA_1);
      ventana2 = SDL_LoadBMP(SPRITES_VENTANA_2);
      ventana3 = SDL_LoadBMP(SPRITES_VENTANA_3);
      ventanaGrande1 = SDL_LoadBMP(SPRITES_VENTANA_GRANDE_1);
      ventanaGrande2 = SDL_LoadBMP(SPRITES_VENTANA_GRANDE_1);
      puerta1 = SDL_LoadBMP(SPRITES_PUERTA_1);
      puerta2 = SDL_LoadBMP(SPRITES_PUERTA_2);
      puerta3 = SDL_LoadBMP(SPRITES_PUERTA_3);
      puerta4 = SDL_LoadBMP(SPRITES_PUERTA_4);
      jugador1 = SDL_LoadBMP(SPRITES_FELIX);
      jugador2 = SDL_LoadBMP(SPRITES_FELIX);

      // Seteo de coordenadas para cada sprite

      jugador1Coordenadas.x = 125;
      jugador1Coordenadas.y = 365;
      jugador2Coordenadas.x = 430;
      jugador2Coordenadas.y = 365;
      edificioCoordenadas.x = 60;
      edificioCoordenadas.y = 0;
      puerta1Coordenadas.x = 270;
      puerta1Coordenadas.y = 350;
      ventanaGrande1Coordenadas.x = 270;
      ventanaGrande1Coordenadas.y = 270;
      ventana1Coordenadas.x = 130;
      ventana1Coordenadas.y = 365;
      ventana2Coordenadas.x = ventana1Coordenadas.x + 80;
      ventana2Coordenadas.y = 365;
      ventana3Coordenadas.x = ventana2Coordenadas.x + 150;
      ventana3Coordenadas.y = 365;
      ventana4Coordenadas.x = ventana3Coordenadas.x + 80;
      ventana4Coordenadas.y = 365;


      ventana1->format->Amask = 0xFF000000;
      ventana1->format->Ashift = 24;
      ventana2->format->Amask = 0xFF000000;
      ventana2->format->Ashift = 24;
      ventana3->format->Amask = 0xFF000000;
      ventana3->format->Ashift = 24;
      ventanaGrande1->format->Amask = 0xFF000000;
      ventanaGrande1->format->Ashift = 24;
      ventanaGrande2->format->Amask = 0xFF000000;
      ventanaGrande2->format->Ashift = 24;

      jugador1->format->Amask = 0xFF000000;
      jugador1->format->Ashift = 24;
      jugador2->format->Amask = 0xFF000000;
      jugador2->format->Ashift = 24;

      // Seteo de colores para cada objeto

      SDL_SetColorKey(ventana1, SDL_SRCCOLORKEY, SDL_MapRGB(ventana1->format, 255,0,255));
      SDL_SetColorKey(ventana2, SDL_SRCCOLORKEY, SDL_MapRGB(ventana2->format, 255,0,255));
      SDL_SetColorKey(ventana3, SDL_SRCCOLORKEY, SDL_MapRGB(ventana3->format, 255,0,255));
      SDL_SetColorKey(ventanaGrande1, SDL_SRCCOLORKEY, SDL_MapRGB(ventanaGrande1->format, 255,0,255));
      SDL_SetColorKey(ventanaGrande2, SDL_SRCCOLORKEY, SDL_MapRGB(ventanaGrande2->format, 255,0,255));
      SDL_SetColorKey(jugador1, SDL_SRCCOLORKEY, SDL_MapRGB(jugador1->format, 255,0,255));
      SDL_SetColorKey(jugador2, SDL_SRCCOLORKEY, SDL_MapRGB(jugador2->format, 255,0,255));
      
      SDL_FillRect(screen, NULL, 0x224487);
      SDL_BlitSurface(edificio, NULL, screen, &edificioCoordenadas);
      SDL_BlitSurface(ventanaGrande1, NULL, screen, &ventanaGrande1Coordenadas);
      SDL_BlitSurface(ventana3, NULL, screen, &ventana1Coordenadas);
      SDL_BlitSurface(ventana3, NULL, screen, &ventana2Coordenadas);
      SDL_BlitSurface(ventana3, NULL, screen, &ventana3Coordenadas);
      SDL_BlitSurface(ventana3, NULL, screen, &ventana4Coordenadas);
      SDL_BlitSurface(puerta1, NULL, screen, &puerta1Coordenadas);
      SDL_BlitSurface(jugador1, NULL, screen, &jugador1Coordenadas);
      SDL_BlitSurface(jugador2, NULL, screen, &jugador2Coordenadas);
     
}
