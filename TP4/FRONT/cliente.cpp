#include "cliente.h"


int main(int argc, char * argv[]){
      
    atexit(SDL_Quit);
    sem_init(&mtxPantalla,0,1);
    /*Proceso principal identifica el mensaje*/
  
    /*Acción 0, recibir datos del servidor*/
    /*Acción 1, dibujar*/
    
    pthread_t acciones[2];

  
    // Directorios generales
    static const char SPRITES_AMBIENTE_DIR[] = "./sprites/ambiente/";
    static const char SPRITES_AVE_DIR[] = "./sprites/ave/";
    static const char SPRITES_EDIFICIO_DIR[] = "./sprites/edificio/";
    static const char SPRITES_FELIX_DIR[] = "./sprites/felix/";
    static const char SPRITES_PUERTA_DIR[] = "./sprites/puerta/";
    static const char SPRITES_RALPH_DIR[] = "./sprites/ralph/";
    static const char SPRITES_VENTANA_DIR[] = "./sprites/ventana/";

    // Imagenes ambiente
    static const char SPRITES_AMBIENTE[] = "./sprites/ambiente/";

    // Imagenes ave
    static const char SPRITES_AVE[] = "./sprites/ave/";

    // Imagenes edificio
    static const char SPRITES_EDIFICIO_CUERPO[] = "./sprites/edificio/cuerpo.bmp";
    static const char SPRITES_EDIFICIO_TECHO[] = "./sprites/edificio/techo.bmp";

    // Imagenes felix
    static const char SPRITES_FELIX[] = "./sprites/felix/felix.bmp";

    // Imagenes puerta
    static const char SPRITES_PUERTA_1[] = "./sprites/puerta/puerta1.bmp";
    static const char SPRITES_PUERTA_2[] = "./sprites/puerta/puerta2.bmp";
    static const char SPRITES_PUERTA_3[] = "./sprites/puerta/puerta3.bmp";
    static const char SPRITES_PUERTA_4[] = "./sprites/puerta/puerta4.bmp";

    // Imagenes ralph
    static const char SPRITES_RALPH[] = "./sprites/ralph/";

    // Imagenes ventana
    static const char SPRITES_VENTANA_1[] = "./sprites/ventana/ventana1.bmp";
    static const char SPRITES_VENTANA_2[] = "./sprites/ventana/ventana2.bmp";
    static const char SPRITES_VENTANA_3[] = "./sprites/ventana/ventana3.bmp";
    static const char SPRITES_VENTANA_GRANDE_1[] = "./sprites/ventana/ventana_grande1.bmp";
    static const char SPRITES_VENTANA_GRANDE_2[] = "./sprites/ventana/ventana_grande2.bmp";
    static const char SPRITES_VIDRIO[] = "./sprites/ventana/vidrio5.bmp";
    static const char SPRITES_VIDRIO_ROTO_1[] = "./sprites/ventana/vidrio1.bmp";
    static const char SPRITES_VIDRIO_ROTO_2[] = "./sprites/ventana/vidrio2.bmp";
    static const char SPRITES_VIDRIO_ROTO_3[] = "./sprites/ventana/vidrio3.bmp";
    static const char SPRITES_VIDRIO_ROTO_4[] = "./sprites/ventana/vidrio4.bmp";

	t_config_cli config;
	/*Cargo la configuración del cliente
	IP - TECLAS
	*/
	if(!cargarConfigCliente(&config)){
		printf("Error en la carga de los parámetros\n");
		exit(0);
	}
	/*Fin carga de archivo de configuración*/


	SDL_Event event;
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


	atexit(SDL_Quit);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("No se pudo iniciar SDL: %s\n",SDL_GetError());
		return 1;
	}
	
	SDL_WM_SetCaption("Wreck It Ralph - Cliente", NULL);
	SDL_WM_SetIcon(SDL_LoadBMP("./sprites/icon.bmp"),NULL);
	screen = SDL_SetVideoMode(640,480,32,SDL_HWSURFACE);
	
	if (screen == NULL) {
		printf("No se puede inicializar el modo gráfico: \n",SDL_GetError());
		return 1;
	}
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
	
		

// 	pthread_create(&acciones[0],NULL,recibirDatos,NULL);
	pthread_create(&acciones[1],NULL,enviarDatos,NULL);	
	 
	while(bRun){
	  sem_wait(&mtxPantalla);
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
		SDL_Flip(screen);
		sem_post(&mtxPantalla);
		SDL_Delay(20);
		
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
						if(event.key.keysym.sym == config.k_up){
						    	if((jugador1Coordenadas.y - 120) >= 0)
							      jugador1Coordenadas.y = jugador1Coordenadas.y - 120;
						}
						if(event.key.keysym.sym == config.k_down){
							if((jugador1Coordenadas.y + 120) <= 450)
							      jugador1Coordenadas.y = jugador1Coordenadas.y + 120;
						}
							
						if(event.key.keysym.sym == config.k_left)
							if((jugador1Coordenadas.x - 85) >= 120 )
							    jugador1Coordenadas.x = jugador1Coordenadas.x -85;   
							
						if(event.key.keysym.sym == config.k_right){
						 	if((jugador1Coordenadas.x + 85) <= 420 )
							    jugador1Coordenadas.x = jugador1Coordenadas.x +85;   
						  
						}

						if(event.key.keysym.sym == SDLK_ESCAPE)
							bRun = 0;
						if(event.key.keysym.sym == SDLK_SPACE)
							printf("Fixing Ventana!\n");
					
						break;
				case SDL_QUIT:
						bRun = 0;
						break;
				default:
						break;
			}
		}
	}

  return 0;
}

void* enviarDatos ( void* n){	
  while(true){	
	printf("Estoy corriendo!\n");
	sleep(2);
  }
}


