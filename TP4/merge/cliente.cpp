#include "cliente.h"
int cambioNivelTerminado ;

int main(int argc, char * argv[]){

  atexit(finalizar);
  mtx = SDL_CreateMutex();
  SDL_Color color = {0,255,0};
  texto2 = TTF_RenderText_Solid(fuente2,"Hola!",color);
  contenedorTexto2.x = 100;
  contenedorTexto2.y = 200;
  contenedorTexto2.w = 150;
  contenedorTexto2.h = 50;

  /*Proceso principal identifica el mensaje*/

  /*Acción 0, recibir datos del servidor*/
  /*Acción 1, dibujar*/


  t_config_cli config;
  SDL_Thread* acciones[2];
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
  SDL_WM_SetCaption("Wreck It Ralph - Cliente: ", NULL);
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
  /*Fin datos de conexión*/


  /*Ingreso la portada*/
  portada = inicializarSprite(SPRITES_PORTADA);
  dibujarSprite(portada,0,0,screen);
  SDL_mutexP(mtx);
  SDL_Flip(screen);
  SDL_mutexV(mtx);
  int test = 0;
  while(test ==0 && SDL_WaitEvent(&event)>=0){
    switch(event.type){
      case SDL_KEYDOWN:
      switch(event.key.keysym.sym){
        case SDLK_RETURN:
        test=1;
        break;
      }
      break;
    }
  }
  /*Envío pedido de conexión*/

  connect(caller_socket, (struct sockaddr*)&listen_address, sizeof(struct sockaddr));
  /*Estoy en condiciones de enviar y recibir datos*/
  /*Fin datos de Conexión*/



  /*Final de la portada*/
  if(recv(caller_socket,&fuiAceptado,sizeof(int),0)<0){
    imprimirError(4);
  }else if(!fuiAceptado){
    //FUI RECHAZADO POR EL SERVIDOR
    //Imágen de Fui rechazado
printf("fuiAceptado");
   /* rechazado = inicializarSprite(SPRITES_NO_FUISTE_ACEPTADO);
    dibujarSprite(rechazado,0,0,screen);

    SDL_mutexP(mtx);
    SDL_Flip(screen);
    SDL_mutexV(mtx);*/

  }

int njug=fuiAceptado;

  //Fui aceptado, quedo a la espera de un contrincante
  //Imágen de fui aceptado
  aceptado = inicializarSprite(SPRITES_ESPERANDO_OPONENTE);
  dibujarSprite(aceptado,0,0,screen);

  SDL_mutexP(mtx);
  SDL_Flip(screen);
  SDL_mutexV(mtx);
  usleep(10000);
  /*Después se saca*/
  /*Receive de primer conexion*/
  	if(recv(caller_socket,&miPaquete,sizeof(t_paquete),0)<0){
  	 printf("Hubo un error en la conexión");
   	//  exit(0);
   	}
         printf("Recibí paquete!!");
       fflush(NULL);
int move=0;
  inicializar(screen);
  /*Se va a encargar de recibir los datos del paquete de datos, llama al método que dibujar los Sprites según el código y los dibuja a todos*/
  acciones[0] = SDL_CreateThread(recibirDatos,NULL);
  /*Se encarga sólamente de dibujar*/
  //       acciones[1] = SDL_CreateThread(dibujar,NULL);
  /*El thread Principal se encarga sólo de enviar las teclas presionadas luego de algunas validaciones*/
  while(bRun){	enviar=0;
    while(SDL_WaitEvent(&event)){enviar=0;
      switch(event.type){
        case SDL_KEYDOWN:
                  if(event.key.keysym.sym == config.k_up){
                    enviar=10;//arriba
                    send(caller_socket, &enviar, sizeof(enviar), 0);
                  }
                if(event.key.keysym.sym == config.k_down){
                  enviar=20;//abajo
                  send(caller_socket, &enviar, sizeof(enviar), 0);
              }

              if(event.key.keysym.sym == config.k_left){
                enviar=30;//izq
                send(caller_socket, &enviar, sizeof(enviar), 0);
              }

            if(event.key.keysym.sym == config.k_right){
              enviar=40;//der
              send(caller_socket, &enviar, sizeof(enviar), 0);
            }
          if(event.key.keysym.sym == SDLK_ESCAPE){
            enviar=1000;//findetodo
            send(caller_socket, &enviar, sizeof(enviar), 0);
            bRun = 0;}
            if(event.key.keysym.sym == SDLK_SPACE){
         					if(njug==1) 
                                              { if(move==1)
                                                {   
  						  jugadores[0] = inicializarSprite(SPRITES_FELIX_MOVE);
						 
                                               move=0;
                                                }
                                                else 
                                                {
  					 	  jugadores[0] = inicializarSprite(SPRITES_FELIX);
                                                 move=1;
                                                }
					      }	
                                           if(njug==2) 
                                              { if(move==1)
                                                {   
  						 jugadores[1] = inicializarSprite(SPRITES_FELIX_MOVE);
                                               move=0;
                                                }
                                                else 
                                                {jugadores[1] = inicializarSprite(SPRITES_FELIX);
                                                 move=1;
                                                }
                                               }
                enviar=1;//space
              send(caller_socket, &enviar, sizeof(enviar), 0);            
            }

      break;
    case SDL_QUIT:   enviar=1000;//findetodo
        send(caller_socket, &enviar, sizeof(enviar), 0);
        bRun = 0;
      break;
    default:
      break;
  }
if(!bRun) break;
}
}

return 0;
}

/*Implementación*/
/*Me encargo de recibir los datos enviados desde el servidor de partida*/
/**
TODO
Mensajes:
cod: 0 - Comienzo de la partida.
cod: 1 - Actualización Jugadores.
cod: 2 - Proximo nivel
cod: 3 - Reinicio de partida.
cod: 4 - Fin de la partida.
cod: 5 -Fin de torneo.
*/
int recibirDatos(void * n){
  
  cambioNivelTerminado = 1;
  while(true){
    if(recv(caller_socket,&miPaquete,sizeof(t_paquete),0)<0){
      imprimirError(4);
    }else{
    /*RECIBO LOS DATOS*/
    // sleep(100000);
    SDL_mutexP(mtx);
    switch(miPaquete.codigoPaquete){
      case 0: break;
      case 1: dibujar();
      break;
      case 2:
      if(cambioNivelTerminado){cambioNivelTerminado=0;
      printf("Llego el cambio de nivel \n");
      dibujarSiguienteNivel();
      printf("Mando el cambio terminado. \n");
  }
      break;
      case 3:break;
      case 4:dibujarTecho(screen); break;
    }
   
  //  usleep(10000); esto petea todo 
    SDL_mutexV(mtx);
    }
  }//fin del while
exit(0);
}


int dibujar(){
  SDL_mutexP(mtx);
  SDL_FillRect(screen, NULL, 0x000000);

  dibujarSprite(edificios[codigoEdificio], 60, 0,screen);
  if(codigoEdificio >= 1){
    dibujarVentanas(1);
  }else{
    dibujarSprite(puertas[0],266,350,screen);
    dibujarSprite(ventanasGrandes[0],266,250,screen);
    dibujarVentanas(0);
  }
  dibujarVidrios(screen);
  dibujarSprite(jugadores[0], miPaquete.jugador1.coordenadas.x, miPaquete.jugador1.coordenadas.y,screen);
  dibujarSprite(jugadores[1], miPaquete.jugador2.coordenadas.x, miPaquete.jugador2.coordenadas.y,screen);
  dibujarSprite(ladrillos[0], miPaquete.ladrillos[0].x,miPaquete.ladrillos[0].y,screen);
  dibujarSprite(ladrillos[1], miPaquete.ladrillos[1].x,miPaquete.ladrillos[1].y,screen);
  dibujarSprite(ladrillos[2], miPaquete.ladrillos[2].x,miPaquete.ladrillos[2].y,screen);
  dibujarSprite(pajaros[0],miPaquete.gaviotas[0].x, miPaquete.gaviotas[0].y, screen);
  dibujarSprite(pajaros[1],miPaquete.gaviotas[1].x, miPaquete.gaviotas[1].y, screen);
  dibujarSprite(pajaros[2],miPaquete.gaviotas[2].x, miPaquete.gaviotas[2].y, screen);
  dibujarSprite(ralph, miPaquete.ralph.x, miPaquete.ralph.y, screen);
  SDL_BlitSurface(texto2,NULL,screen,&contenedorTexto2);
//mostrarVidas();
  SDL_Flip(screen);
  SDL_mutexV(mtx);
  //  		SDL_Delay(20);
  //   }
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

void finalizar(void){
  printf("Finalizando...\n");
  SDL_Quit();
  SDL_DestroyMutex(mtx);
}

void inicializar(SDL_Surface *destino){

  int i = 0;
  SDL_mutexP(mtx);
  SDL_FillRect(destino, NULL, 0x000000);
  // Se cargan todos los sprites necesarios
  edificios[0] = inicializarSprite(SPRITES_EDIFICIO_CUERPO_1);
  edificios[1] = inicializarSprite(SPRITES_EDIFICIO_CUERPO_2);
  edificios[2] = inicializarSprite(SPRITES_EDIFICIO_CUERPO_3);
  edificios[3] = inicializarSprite(SPRITES_EDIFICIO_TERMINADO_1);
  edificios[4] = inicializarSprite(SPRITES_EDIFICIO_TERMINADO_2);
  for(i=0;i<=10;i++){
    ventanasTipo1[i] = inicializarSprite(SPRITES_VENTANA_3);
    ventanasTipo2[i] = inicializarSprite(SPRITES_VENTANA_2);
    ventanasTipo3[i] = inicializarSprite(SPRITES_VENTANA_1);
  }
  for(i=0;i<=40;i++){
    vidrios[i] = inicializarSprite(SPRITES_VIDRIO);
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

  ralph = inicializarSprite(SPRITES_RALPH);
  SDL_mutexV(mtx);

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

void cambiarVelocidadLadrillos(int variacion){
  velocidadLadrillo = velocidadLadrillo + (variacion * factorVelocidadLadrillo);
}

void cambiarVelocidadGaviotas(int variacion){
  velocidadGaviota = velocidadGaviota + (variacion * factorVelocidadGaviota);
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

void dibujarVidrios(SDL_Surface *screen){
  int i;
  for(i = 0; i < 40; i++){
    if(miPaquete.vidrios[i].coordenadas.x != 0){
      if(miPaquete.vidrios[i].roto == 1){
        vidrios[i] = inicializarSprite(SPRITES_VIDRIO_ROTO_2);
      }else{
        vidrios[i] = inicializarSprite(SPRITES_VIDRIO);
      }
      dibujarSprite(vidrios[i], miPaquete.vidrios[i].coordenadas.x, miPaquete.vidrios[i].coordenadas.y,screen);
    }
  }
}

void dibujarSiguienteNivel(){
  int posicionYEdificioAnterior = 0;
  int posicionYEdificioActual = -480; 
      enviar = 500;
      codigoEdificio=1;//nivel 2
      send(caller_socket, &enviar, sizeof(enviar), 0);
  
  while(posicionYEdificioAnterior<480 && posicionYEdificioActual<480){
    SDL_mutexP(mtx);
    SDL_FillRect(screen, NULL, 0x000000);
    dibujarSprite(edificios[3],60, posicionYEdificioAnterior, screen);
    dibujarSprite(edificios[4],60, posicionYEdificioActual, screen);
    posicionYEdificioAnterior+=6;
    posicionYEdificioActual+=6;
    SDL_Flip(screen);
    SDL_Delay(1);
    SDL_mutexV(mtx);
  }
  
  
  dibujarSprite(edificios[1], 60, 0,screen);
  //deberiamos hacer que se dibuje las ventanas rotas del nivel 2 y 3
  //dibujarVentanas(1);
  //dibujarVidrios(screen);
 cambioNivelTerminado = 1;
}

void dibujarTecho(SDL_Surface *screen){
  int posicionYEdificioAnterior = 0;
  int posicionYEdificioActual = -480;
  while(posicionYEdificioAnterior<480 && posicionYEdificioActual<480){
    SDL_mutexP(mtx);
    SDL_FillRect(screen, NULL, 0x000000);
    dibujarSprite(edificios[4],60, posicionYEdificioAnterior, screen);
    dibujarSprite(edificios[2],60, posicionYEdificioActual, screen);
    posicionYEdificioAnterior++;
    posicionYEdificioActual++;
    SDL_Flip(screen);
    SDL_mutexV(mtx);
    SDL_Delay(1);
  }
   dibujarSprite(edificios[2], 60, 0,screen);
//fin de todo
   sleep(60);
finalizar();
}

