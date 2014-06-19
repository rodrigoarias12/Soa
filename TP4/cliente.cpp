#include "cliente.h"


int main(int argc, char * argv[]){

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
	SDL_Surface *screen, *logo;
	SDL_Rect logoRect;
	logoRect.x = 100;
	logoRect.y = 120;
	atexit(SDL_Quit);

	if(SDL_Init(SDL_INIT_VIDEO) < 0) exit(1);
	SDL_WM_SetCaption("SDL Window", NULL);
	screen = SDL_SetVideoMode(640,480,32,SDL_HWSURFACE);
	logo = SDL_LoadBMP("./logo.bmp");
	SDL_SetColorKey(logo, SDL_SRCCOLORKEY, SDL_MapRGB(logo->format, 255,255,255));

	while(bRun){
		SDL_FillRect(screen, NULL, 0x224487);
		SDL_BlitSurface(logo, NULL, screen, &logoRect);
		SDL_Flip(screen);
		SDL_Delay(20);
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_UP:
							logoRect.y = logoRect.y -10;
							break;
						case SDLK_DOWN:
							logoRect.y = logoRect.y +10;
							break;
						case SDLK_LEFT:
							logoRect.x = logoRect.x -10;
							break;
						case SDLK_RIGHT:
							logoRect.x = logoRect.x +10;
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