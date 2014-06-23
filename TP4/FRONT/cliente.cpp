#include "cliente.h"


int main(int argc, char * argv[]){
      
      atexit(finalizar);
      mtx = SDL_CreateMutex();
    
      /*Proceso principal identifica el mensaje*/
  
      /*Acción 0, recibir datos del servidor*/
      /*Acción 1, dibujar*/
    
      SDL_Thread* acciones[2];

      t_config_cli config;
      /*Cargo la configuración del cliente
      IP - TECLAS
      */
      if(!cargarConfigCliente(&config)){
	      imprimirError(1);
      }
      /*Fin carga de archivo de configuración*/

      atexit(SDL_Quit);

      if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	      imprimirError(0);
      }
      
      SDL_WM_SetIcon(SDL_LoadBMP(SPRITES_ICONO),NULL);
      SDL_WM_SetCaption("Wreck It Ralph - Cliente", NULL);
      screen = SDL_SetVideoMode(SCREEN_ANCHO,SCREEN_ALTO,SCREEN_BPP,SDL_HWSURFACE);
      
      if (screen == NULL) {
	      imprimirError(2);
      }
      
      SDL_Flip(screen);
      
      
      /*Ingreso la portada*/
	portada = inicializarSprite(SPRITES_PORTADA);
	dibujarSprite(portada,0,0,screen);
      do{
	SDL_PollEvent(&event);
	printf("No fue un Enter\n");
      }while(event.type != SDL_KEYDOWN && event.key.keysym.sym !=SDLK_RETURN);
      /*Final de la portada*/
      
      
      /*Datos de Conexión*/
      unsigned short int listen_port=0;
      unsigned long int listen_ip_address=0;
      struct sockaddr_in listen_address;
      caller_socket=socket(AF_INET,SOCK_STREAM,0);
	  
      listen_address.sin_family=AF_INET;

      listen_port=htons(config.port);
      listen_address.sin_port=listen_port;

      listen_ip_address=inet_addr(config.ip);
      listen_address.sin_addr.s_addr=listen_ip_address;
      bzero(&(listen_address.sin_zero), 8);
      connect(caller_socket, (struct sockaddr*)&listen_address, sizeof(struct sockaddr));
      
      
      /*Fin datos de Conexión*/
      
      inicializar(screen);

      acciones[1] = SDL_CreateThread(enviarDatos,NULL);
//    acciones[0] = SDL_CreateThread(recibirDatos,NULL);
	while(bRun){
		SDL_FillRect(screen, NULL, 0x224487);
		dibujarSprite(edificios[0], 60, 0,screen);
		dibujarSprite(puertas[0], 270,350,screen);
		dibujarSprite(ventanasGrandes[0], 270, 270,screen);
		dibujarSprite(ventanas[0], 130, 365,screen);
		dibujarSprite(ventanas[1], 210, 365,screen);
		dibujarSprite(ventanas[2], 360, 365,screen);
		dibujarSprite(ventanas[1], 440, 365,screen);
		dibujarSprite(jugadores[0], 125, 365,screen);
		dibujarSprite(jugadores[1], 430, 365,screen);
//		SDL_BlitSurface(edificios[0], NULL, screen, &edificioCoordenadas);
		SDL_mutexP(mtx);
		SDL_Flip(screen);
		SDL_mutexV(mtx);
		SDL_Delay(20);
		

// 		while(SDL_PollEvent(&event)){
// 		switch(event.type){
// 		    case SDL_KEYDOWN:
// 					if(event.key.keysym.sym == config.k_up){
// 						
// 						if((jugador1Coordenadas.y - 120) >= 0){
// 						SDL_mutexP(mtx);
// 						    jugador1Coordenadas.y = jugador1Coordenadas.y - 120;
// 						SDL_mutexV(mtx);						  
// 						}
// 					}
// 					if(event.key.keysym.sym == config.k_down){
// 						if((jugador1Coordenadas.y + 120) <= 450){
// 						  SDL_mutexP(mtx);
// 						   jugador1Coordenadas.y = jugador1Coordenadas.y + 120; 
// 						  SDL_mutexV(mtx);
// 						}
// 					}
// 						
// 					if(event.key.keysym.sym == config.k_left)
// 						if((jugador1Coordenadas.x - 85) >= 120 ){
// 						  SDL_mutexP(mtx);
// 						    jugador1Coordenadas.x = jugador1Coordenadas.x -85;
// 						  SDL_mutexV(mtx);
// 						}   
// 						
// 					if(event.key.keysym.sym == config.k_right){
// 						if((jugador1Coordenadas.x + 85) <= 420 ){
// 						  SDL_mutexP(mtx);  
// 						  jugador1Coordenadas.x = jugador1Coordenadas.x +85;
// 						  SDL_mutexV(mtx);
// 						}   
// 					}
// 					if(event.key.keysym.sym == SDLK_ESCAPE)
// 						bRun = 0;
// 					if(event.key.keysym.sym == SDLK_SPACE)
// 						printf("Fixing Ventana!\n");
// 				
// 					break;
// 			case SDL_QUIT:
// 					bRun = 0;
// 					break;
// 			default:
// 					break;
// 		}
// 	}
	}

  return 0;
}