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
      /*Estoy en condiciones de enviar y recibir datos*/
      /*Fin datos de Conexión*/
      
      
      /*Ingreso la portada*/
	portada = inicializarSprite(SPRITES_PORTADA);
	dibujarSprite(portada,0,0,screen);
	SDL_Flip(screen);
	/*Envío pedido de conexión*/
      do{
	do{
	    while(SDL_PollEvent(&event));
	 }while(event.type != SDL_KEYDOWN);	 
      }while(event.key.keysym.sym !=SDLK_RETURN);
      /*Final de la portada*/
      
      
      
      
	 if(0/*recv(caller_socket,&fuiAceptado,sizeof(int),0)<0*/){
	  imprimirError(4);
	}else if(!fuiAceptado){
	  //FUI RECHAZADO POR EL SERVIDOR
	  //Imágen de Fui rechazado
	  rechazado = inicializarSprite(SPRITES_NO_FUISTE_ACEPTADO);
	  dibujarSprite(rechazado,0,0,screen);
	  SDL_Flip(screen);
	  
	}
	
	//Fui aceptado, quedo a la espera de un contrincante
	//Imágen de fui aceptado
	  aceptado = inicializarSprite(SPRITES_ESPERANDO_OPONENTE);
	  dibujarSprite(aceptado,0,0,screen);
	  SDL_Flip(screen);
	  /*Después se saca*/
	  do{
	      do{
		while(SDL_PollEvent(&event));
	      }while(event.type != SDL_KEYDOWN);	 
	  }while(event.key.keysym.sym !=SDLK_SPACE);
	  
	
	
	/*Receive de primer conexion*/
	if(0/*recv(caller_socket,&miPaquete,sizeof(t_paquete),0)<0*/){
	  //Hubo un error en la conexión
	  exit(0);
	}
	
      inicializar(screen);
      /*Se va a encargar de recibir los datos del paquete de datos, llama al método que dibujar los Sprites según el código y los dibuja a todos*/
      acciones[0] = SDL_CreateThread(recibirDatos,NULL);
      /*Se encarga sólamente de dibujar*/
      acciones[1] = SDL_CreateThread(dibujar,NULL);
      /*El thread Principal se encarga sólo de enviar las teclas presionadas luego de algunas validaciones*/
      while(bRun){		
		while(SDL_PollEvent(&event)){
		switch(event.type){
		    case SDL_KEYDOWN:
					if(event.key.keysym.sym == config.k_up){
						
						if((jugador1Coordenadas.y - 120) >= 0){
						SDL_mutexP(mtx);
						    jugador1Coordenadas.y = jugador1Coordenadas.y - 120;
						SDL_mutexV(mtx);						  
						}
					}
					if(event.key.keysym.sym == config.k_down){
						if((jugador1Coordenadas.y + 120) <= 450){
						  SDL_mutexP(mtx);
						   jugador1Coordenadas.y = jugador1Coordenadas.y + 120; 
						  SDL_mutexV(mtx);
						}
					}
						
					if(event.key.keysym.sym == config.k_left)
						if((jugador1Coordenadas.x - 85) >= 120 ){
						  SDL_mutexP(mtx);
						    jugador1Coordenadas.x = jugador1Coordenadas.x -85;
						  SDL_mutexV(mtx);
						}   
						
					if(event.key.keysym.sym == config.k_right){
						if((jugador1Coordenadas.x + 85) <= 420 ){
						  SDL_mutexP(mtx);  
						  jugador1Coordenadas.x = jugador1Coordenadas.x +85;
						  SDL_mutexV(mtx);
						}   
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