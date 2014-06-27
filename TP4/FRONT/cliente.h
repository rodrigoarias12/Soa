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

typedef struct{
  t_coordenadas coordenadas;
}t_ladrillo;

typedef struct{
  t_coordenadas coordenadas;
}t_gaviota;

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
  t_ladrillo ladrillo[3];
  t_gaviota gaviota[3];
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

//despues lo sacamos por ahor qued asi 
SDL_Rect pajaroCoordenadas1,pajaroCoordenadas2,pajaroCoordenadas3;

/*Sección Crítica*/
t_paquete miPaquete;
/*Sección Crítica*/

t_ladrillo coordenadasLadrillos[2];
t_gaviota coordenadasGaviotas[2];

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
	    *rechazado,
	    *aceptado,
            *jugadores[2],
            *edificios[3],
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
	    *pajaros[3];
	    
SDL_Rect jugador1Coordenadas;
	    


int cargarConfigCliente(t_config_cli *);
int extraerNumero(char *);
int validarNumero(char *);
int validarNumeroIP(char *);
char * extraerNumeroIP(char *);
int extraerTecla(char *);
int recibirDatos(void *);
int dibujar(void *);
void finalizar(void);
void imprimirError(int);
void inicializar(SDL_Surface *);
void dibujarSprite(SDL_Surface *, int , int, SDL_Surface *);
SDL_Surface *inicializarSprite(const char *);
void mover_pajaros(void);
void dibujarVentanas(int completo = 0);
void dibujarVidrios(int completo = 0);
void incrementarNivel(void);
void cambiarVelocidadLadrillos(int);
void cambiarVelocidadGaviotas(int);
void inicializar(void);
void dibujarSiguienteNivel(SDL_Surface *);
void dibujarTecho(SDL_Surface *);


///movimiento del pajaro
 int mov_paj1=random();
 int mov_paj2=random();
 int mov_paj3=random();

/*Implementación*/
/*Me encargo de recibir los datos enviados desde el servidor de partida*/

int recibirDatos(void * n){
 
  while(true){
//     printf("Estoy recibiendo datos!\n");
//    if(recv(caller_socket,&t_paquete,sizeof(t_paquete),0)<0){
//      imprimirError(4);
//    }
    /*RECIBO LOS DATOS*/
    sleep(100000);
    SDL_mutexP(mtx);
    switch(miPaquete.codigoPaquete){
      case 0: break;
      case 1: break;
      case 2: break;
      case 3: break;
      case 4: break;
    }
    SDL_mutexV(mtx);
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
int dibujar(void* n){	
  while(true){
		SDL_mutexP(mtx);
		SDL_FillRect(screen, NULL, 0x000000);
		dibujarSprite(edificios[0], 60, 0,screen);
		dibujarSprite(puertas[0], 266,350,screen);
		dibujarSprite(ventanasGrandes[0], 266, 270,screen);
		dibujarVentanas(0);
		dibujarVidrios(0);
//      Prueba para ver como se dibuja el siguiente nivel
//		dibujarSiguienteNivel(screen);
		dibujarSprite(jugadores[0], jugador1Coordenadas.x, jugador1Coordenadas.y,screen);
		dibujarSprite(jugadores[1], 430, 365,screen);
		dibujarSprite(ladrillos[0], 200,150,screen);
		dibujarSprite(ladrillos[1], 180,200,screen);
		dibujarSprite(ladrillos[2], 350,50,screen);
		
		if(mov_paj1<640)mov_paj1++;else mov_paj1=-random()%100;
                if(mov_paj2<640)mov_paj2++;else mov_paj2=-random()%100-100;
                if(mov_paj3<640)mov_paj3++;else mov_paj3=-random()%100-50;
		
                pajaroCoordenadas1.x=mov_paj1;     
                pajaroCoordenadas2.x=mov_paj2;     
                pajaroCoordenadas3.x=mov_paj3;
		
		dibujarSprite(pajaros[0],pajaroCoordenadas1.x, pajaroCoordenadas1.y, screen);	
                dibujarSprite(pajaros[1],pajaroCoordenadas2.x, pajaroCoordenadas2.y, screen);
		dibujarSprite(pajaros[2],pajaroCoordenadas3.x, pajaroCoordenadas3.y, screen);
		
//		SDL_BlitSurface(edificios[0], NULL, screen, &edificioCoordenadas);
		SDL_Flip(screen);
		SDL_mutexV(mtx);
		SDL_Delay(20);
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
      printf("Se perdió la conexión\n");
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

void inicializar(SDL_Surface *destino){

      int i = 0;

      SDL_FillRect(destino, NULL, 0x000000);
      // Se cargan todos los sprites necesarios
      edificios[0] = inicializarSprite(SPRITES_EDIFICIO_CUERPO_1);
      edificios[1] = inicializarSprite(SPRITES_EDIFICIO_CUERPO_2);
      edificios[2] = inicializarSprite(SPRITES_EDIFICIO_CUERPO_3);
      for(i=0;i<=10;i++){
        ventanasTipo1[i] = inicializarSprite(SPRITES_VENTANA_3);
        ventanasTipo2[i] = inicializarSprite(SPRITES_VENTANA_2);
        ventanasTipo3[i] = inicializarSprite(SPRITES_VENTANA_1);
      }
      for(i=0;i<=40;i++){
        vidrios[i] = inicializarSprite(SPRITES_VIDRIO);
        vidriosRotos1[i] = inicializarSprite(SPRITES_VIDRIO_ROTO_1);
        vidriosRotos2[i] = inicializarSprite(SPRITES_VIDRIO_ROTO_2);
        vidriosRotos3[i] = inicializarSprite(SPRITES_VIDRIO_ROTO_3);
        vidriosRotos4[i] = inicializarSprite(SPRITES_VIDRIO_ROTO_4);
      }
      ventanasGrandes[0] = inicializarSprite(SPRITES_VENTANA_GRANDE_1);
      ventanasGrandes[1] = inicializarSprite(SPRITES_VENTANA_GRANDE_2);
      puertas[0] = inicializarSprite(SPRITES_PUERTA_1);
      puertas[1] = inicializarSprite(SPRITES_PUERTA_2);
      puertas[2] = inicializarSprite(SPRITES_PUERTA_3);
      puertas[3] = inicializarSprite(SPRITES_PUERTA_4);
      ladrillos[0] = inicializarSprite(SPRITES_LADRILLO);
      ladrillos[1] = inicializarSprite(SPRITES_LADRILLO);
      ladrillos[2] = inicializarSprite(SPRITES_LADRILLO);
      jugadores[0] = inicializarSprite(SPRITES_FELIX);
      jugadores[1] = inicializarSprite(SPRITES_FELIX);
      pajaros[0] = inicializarSprite(SPRITES_AVE);
      pajaros[1] = inicializarSprite(SPRITES_AVE);
      pajaros[2] = inicializarSprite(SPRITES_AVE);

      //pajaros
      pajaroCoordenadas1.x=-400;
      pajaroCoordenadas1.y=316;
      pajaroCoordenadas2.x=-300;
      pajaroCoordenadas2.y=200;
      pajaroCoordenadas3.x=-300;
      pajaroCoordenadas3.y=100;
      //fin pajaros
      dibujarSprite(jugadores[0], 125, 365,destino);
      jugador1Coordenadas.x = 125;
      jugador1Coordenadas.y = 365;
}

void dibujarSprite(SDL_Surface *sprite, int x, int y, SDL_Surface *destino){
  SDL_Rect posicion;
  posicion.x = x;
  posicion.y = y;
  
  SDL_BlitSurface(sprite, NULL, destino, &posicion);
}

SDL_Surface *inicializarSprite(const char *path){
    SDL_Surface *sprite;
    sprite = SDL_LoadBMP(path);
    sprite->format->Amask = 0xFF000000;
    sprite->format->Ashift = 24;
    SDL_SetColorKey(sprite, SDL_SRCCOLORKEY, SDL_MapRGB(sprite->format, 255,0,255));

    return sprite;
}

void dibujarVentanas(int completo){

    int i,x,y,comienzoX,comienzoY,distanciaEntreVentanas;
    comienzoX = 127;
    comienzoY = 5;
    distanciaEntreVentanas = 78;
    y = comienzoY;
    x = comienzoX;
    for(i=0;i<10;i++){
        if(i==5){
            x = comienzoX;
            y+=120;
        }
        dibujarSprite(ventanasTipo3[i], x, y,screen);
        x+=distanciaEntreVentanas;
    }

    y+=110;
    x = comienzoX;
    for(i=0;i<10;i++){
        if(i==5){
            x = comienzoX;
            y+=135;
        }
        if(completo == 1 || (i!=2 && i!=7)){
            dibujarSprite(ventanasTipo1[i], x, y,screen);
        }
        x+=distanciaEntreVentanas;
    }
}

void dibujarVidrios(int completo){
    int i,x,y,comienzoX,comienzoY,distanciaEntreVidrios,totalVidrios;
    comienzoX = 140;
    comienzoY = 31;
    x = comienzoX;
    y = comienzoY;
    distanciaEntreVidrios = 78;
    totalVidrios = 36;
    if(completo == 1){
        totalVidrios = 38;
    }
    for(i=0;i<totalVidrios;i++){
        if((completo == 0 && (i==5 || i==10 || i==14 || i== 18)) ||
           (completo == 1 && (i==5 || i==10 || i==15 || i== 20))){
            x = comienzoX;
            switch(i){
                case 10: y+=110; break;
                case 14: if(completo == 0) y+=135; break;
                case 15: if(completo == 1) y+=135; break;
                default: y+=120; break;
            }
        }

        if(completo != 1 && (i == 12 || i == 16)){
            x+=distanciaEntreVidrios;
        }
        dibujarSprite(vidrios[i], x, y,screen);
        dibujarSprite(vidrios[i], x, y+30,screen);
        x+=distanciaEntreVidrios;
    }
}

void cambiarVelocidadLadrillos(int variacion){
  velocidadLadrillo = velocidadLadrillo + (variacion * factorVelocidadLadrillo);
}

void cambiarVelocidadGaviotas(int variacion){
  velocidadGaviota = velocidadGaviota + (variacion * factorVelocidadGaviota);
}



/*
    Relación "Codigo - Objeto" a dibujar.
    El código se compone de 4 dígitos, los cuales agrupan por tipo:

    1er Dígito - Grupo de objeto:
        Agrupamos los objetos por tipo:
            1 - Edificio
            2 - Ventanas
            3 - Elementos que se rompen y arreglan
            4 - Personajes
            5 - Objetos
            6 - Datos del juego (score, vidas, etc)
            7 - Niveles
    2do Dígito - Agregar o remover:
        0 - Agregar/Mueve
        1 - Remover

    3er y 4to Dígito - Identificador del objeto: lo utilizamos para identificar el
                 objeto a dibujar con un id único.

    Ejemplo: código 1010
        1: Tipo edificio
        0: Agrega/Mueve sprite
        10: ID del objeto

    Listado de códigos del servidor al cliente:

    1010 - Edificio parte 1
    1020 - Edificio parte 2
    1030 - Edificio parte 3
    2010 - Ventana chica 1
    2011 - Ventana chica 2
    2012 - Ventana chica 3
    3010 - Ventana grande 1
    3011 - Ventana grande 2
    3020 - Puerta normal
    3021 - Puerta rota 1
    3022 - Puerta rota 2
    3023 - Puerta rota 3
    3030 - Vidrio normal
    3031 - Vidrio roto 1
    3032 - Vidrio roto 2
    3033 - Vidrio roto 3
    3034 - Vidrio roto 4
    4010 - Felix
    4011 - Felix muerto
    4020 - Ralph
    4030 - Gaviota
    5010 - Ladrillo 1
    5011 - Ladrillo 2
    5012 - Ladrillo 3
    6010 - Score
    6020 - Vidas 1
    6021 - Vidas 2
    6022 - Vidas 3
    7020 - Nivel 2
    7030 - Nivel 3
 */

void dibujarCodigo(int codigo, int x, int y, SDL_Surface *screen){
    switch(codigo){
        case 0000: 
	  /*Dibujar Todo*/
	  SDL_mutexP(mtx);
	  
	  SDL_mutexV(mtx);
	  break;
        case 1010: SDL_mutexP(mtx); dibujarSprite(edificios[0],x,y,screen); SDL_mutexV(mtx); break;
        case 1020: SDL_mutexP(mtx); dibujarSprite(edificios[1],x,y,screen); SDL_mutexV(mtx);break;
        case 1030: SDL_mutexP(mtx); dibujarSprite(edificios[2],x,y,screen); SDL_mutexV(mtx);break;
        case 2010: SDL_mutexP(mtx); dibujarSprite(ventanasTipo1[0],x,y,screen); SDL_mutexV(mtx); break;
        case 2011: SDL_mutexP(mtx);dibujarSprite(ventanasTipo1[1],x,y,screen); SDL_mutexV(mtx);break;
        case 2012: SDL_mutexP(mtx); dibujarSprite(ventanasTipo1[2],x,y,screen); SDL_mutexV(mtx); break;
        case 3010: SDL_mutexP(mtx); dibujarSprite(ventanasGrandes[0],x,y,screen); SDL_mutexV(mtx);break;
        case 3011: SDL_mutexP(mtx); dibujarSprite(ventanasGrandes[1],x,y,screen); SDL_mutexV(mtx);break;
        case 3020: SDL_mutexP(mtx); dibujarSprite(puertas[0],x,y,screen); SDL_mutexV(mtx);break;
        case 3021: SDL_mutexP(mtx); dibujarSprite(puertas[1],x,y,screen); SDL_mutexV(mtx);break;
        case 3022: SDL_mutexP(mtx); dibujarSprite(puertas[2],x,y,screen); SDL_mutexV(mtx);break;
        case 3023: SDL_mutexP(mtx); dibujarSprite(puertas[3],x,y,screen); SDL_mutexV(mtx);break;
        case 3030: SDL_mutexP(mtx); dibujarSprite(vidrios[0],x,y,screen); SDL_mutexV(mtx);break;
        case 3031: SDL_mutexP(mtx); dibujarSprite(vidrios[1],x,y,screen); SDL_mutexV(mtx);break;
        case 3032: SDL_mutexP(mtx); dibujarSprite(vidrios[2],x,y,screen); SDL_mutexV(mtx);break;
        case 3033: SDL_mutexP(mtx); dibujarSprite(vidrios[3],x,y,screen); SDL_mutexV(mtx);break;
        case 3034: SDL_mutexP(mtx); dibujarSprite(vidrios[4],x,y,screen); SDL_mutexV(mtx);break;
        case 4010: SDL_mutexP(mtx); dibujarSprite(felix[0],x,y,screen); SDL_mutexV(mtx);break;
        case 4011: SDL_mutexP(mtx); dibujarSprite(felix[1],x,y,screen); SDL_mutexV(mtx);break;
        case 4020: SDL_mutexP(mtx); dibujarSprite(ralph,x,y,screen); SDL_mutexV(mtx);break;
        case 4030: SDL_mutexP(mtx); dibujarSprite(gaviota,x,y,screen); SDL_mutexV(mtx);break;
        case 5010: SDL_mutexP(mtx); dibujarSprite(ladrillos[0],x,y,screen); SDL_mutexV(mtx);break;
        case 5011: SDL_mutexP(mtx); dibujarSprite(ladrillos[1],x,y,screen); SDL_mutexV(mtx);break;
        case 5012: SDL_mutexP(mtx); dibujarSprite(ladrillos[2],x,y,screen); SDL_mutexV(mtx);break;
        case 6010: SDL_mutexP(mtx); dibujarSprite(score,x,y,screen); SDL_mutexV(mtx);break;
        case 6020: SDL_mutexP(mtx); dibujarSprite(vidas[0],x,y,screen); SDL_mutexV(mtx);break;
        case 6021: SDL_mutexP(mtx); dibujarSprite(vidas[1],x,y,screen); SDL_mutexV(mtx);break;
        case 6022: SDL_mutexP(mtx); dibujarSprite(vidas[2],x,y,screen); SDL_mutexV(mtx);break;
        case 7010: SDL_mutexP(mtx); dibujarSiguienteNivel(screen); SDL_mutexV(mtx);break;
        case 7020: SDL_mutexP(mtx); dibujarTecho(screen); SDL_mutexV(mtx);break;
        default: break;
    }
}

void dibujarSiguienteNivel(SDL_Surface *screen){
    dibujarSprite(edificios[1], 60, 0,screen);
    dibujarVentanas(1);
    dibujarVidrios(1);
    dibujarSprite(jugadores[0], jugador1Coordenadas.x, jugador1Coordenadas.y,screen);
    dibujarSprite(jugadores[1], 430, 365,screen);
}

void dibujarTecho(SDL_Surface *screen){
    dibujarSprite(edificios[2], 60, 0,screen);
}