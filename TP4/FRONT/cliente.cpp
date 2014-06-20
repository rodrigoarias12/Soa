#include "cliente.h"


int main(int argc, char * argv[]){

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
    static const char SPRITES_PUERTA[] = "./sprites/puerta/";

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
	int bRun = 1;
	// Declaramos todas las partes graficas
	SDL_Surface *screen,
	            *jugador1,
		    *jugador2,
	            *edificio,
	            *puerta,
	            *ventana;
	SDL_Rect jug1Coord,
		 jug2Coord,
	         edificioCoordenadas;
	 
	jug1Coord.x = 120;
	jug1Coord.y = 350;	 
	jug2Coord.x = 420;
	jug2Coord.y = 350;
	edificioCoordenadas.x = 60;
	edificioCoordenadas.y = 0;

	atexit(SDL_Quit);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("No se pudo iniciar SDL: %s\n",SDL_GetError());
		return 1;
	}
	
	SDL_WM_SetCaption("Wreck It Ralph - Cliente: ", NULL);
	SDL_WM_SetIcon(SDL_LoadBMP("./sprites/icon.bmp"),NULL);
	screen = SDL_SetVideoMode(640,480,32,SDL_HWSURFACE);
	
	if (screen == NULL) {
		printf("No se puede inicializar el modo gráfico: \n",SDL_GetError());
		return 1;
	}
	
	edificio = SDL_LoadBMP(SPRITES_EDIFICIO_CUERPO);
	jugador1 = SDL_LoadBMP(SPRITES_FELIX);
	jugador2 = SDL_LoadBMP(SPRITES_FELIX);
	jugador1->format->Amask = 0xFF000000;
	jugador1->format->Ashift = 24;
	jugador2->format->Amask = 0xFF000000;
	jugador2->format->Ashift = 24;
	
	SDL_SetColorKey(jugador1, SDL_SRCCOLORKEY, SDL_MapRGB(jugador1->format, 255,0,255));
	SDL_SetColorKey(jugador2, SDL_SRCCOLORKEY, SDL_MapRGB(jugador2->format, 255,0,255));
	while(bRun){
		SDL_FillRect(screen, NULL, 0x224487);
		SDL_BlitSurface(edificio, NULL, screen, &edificioCoordenadas);
		SDL_BlitSurface(jugador1, NULL, screen, &jug1Coord);
		SDL_BlitSurface(jugador2, NULL, screen, &jug2Coord);
		SDL_Flip(screen);
		SDL_Delay(20);
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
						if(event.key.keysym.sym == config.k_up)
							if((jug1Coord.y - 100) >= 50)
							jug1Coord.y = jug1Coord.y - 100;
						if(event.key.keysym.sym == config.k_down){
						  if((jug1Coord.y + 100) <= 350)
							jug1Coord.y = jug1Coord.y + 100;
						}
							
						if(event.key.keysym.sym == config.k_left)
							if((jug1Coord.x - 100) >= 120 )
							    jug1Coord.x = jug1Coord.x -100;   
							
						if(event.key.keysym.sym == config.k_right){
						 	if((jug1Coord.x + 100) <= 420 )
							    jug1Coord.x = jug1Coord.x +100;   
						  
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