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
	            *logo,
	            *edificio,
	            *puerta,
	            *ventana;
	SDL_Rect logoCoordenadas,
	         edificioCoordenadas;

	logoCoordenadas.x = 100;
	logoCoordenadas.y = 120;
	edificioCoordenadas.x = 60;
	edificioCoordenadas.y = 0;

	atexit(SDL_Quit);

	if(SDL_Init(SDL_INIT_VIDEO) < 0) exit(1);
	SDL_WM_SetCaption("SDL Window", NULL);
	screen = SDL_SetVideoMode(640,480,32,SDL_HWSURFACE);
	edificio = SDL_LoadBMP(SPRITES_EDIFICIO_CUERPO);
	logo = SDL_LoadBMP(SPRITES_FELIX);
	SDL_SetColorKey(logo, SDL_SRCCOLORKEY, SDL_MapRGB(logo->format, 255,255,255));

	while(bRun){
		SDL_FillRect(screen, NULL, 0x224487);
		SDL_BlitSurface(edificio, NULL, screen, &edificioCoordenadas);
		SDL_BlitSurface(logo, NULL, screen, &logoCoordenadas);
		SDL_Flip(screen);
		SDL_Delay(20);
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_UP:
							logoCoordenadas.y = logoCoordenadas.y -10;
							break;
						case SDLK_DOWN:
							logoCoordenadas.y = logoCoordenadas.y +10;
							break;
						case SDLK_LEFT:
							logoCoordenadas.x = logoCoordenadas.x -10;
							break;
						case SDLK_RIGHT:
							logoCoordenadas.x = logoCoordenadas.x +10;
							break;
						case SDLK_ESCAPE:
							bRun = 0;
							break;
						default:
							break;
					}
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