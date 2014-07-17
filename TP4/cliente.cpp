/*#####################################
#Trabajo Practico N 4
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#Garcia Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/

#include "cliente.h"


int cambioNivelTerminado = 1;
int njug;
int jugando=0;
int torneoVivo=1;
SDL_Rect pantallaNegra;
//dos variables agregadas negro
TTF_Font *fuente2;
char cadena[25];



int main(int argc, char * argv[]){
	atexit(finalizar);
	mtx = SDL_CreateMutex();
	t_config_cli config;
	SDL_Thread* acciones[2];
	/*
        Cargo la configuracion del cliente
	IP - TECLAS
	*/
	if(!cargarConfigCliente(&config)){
		imprimirError(1);
	}
	/*Fin carga de archivo de configuracion*/
	atexit(SDL_Quit);
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		imprimirError(0);
	}
	TTF_Init();
	fuente2 = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSerifItalic.ttf",25);
	SDL_WM_SetIcon(SDL_LoadBMP(SPRITES_ICONO),NULL);
	SDL_WM_SetCaption("Wreck It Ralph - Cliente: ", NULL);
	screen = SDL_SetVideoMode(SCREEN_ANCHO,SCREEN_ALTO,SCREEN_BPP,SDL_HWSURFACE);
	if (screen == NULL) {
		imprimirError(2);
	}
	/*Datos de Conexion*/
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
	/*Fin datos de conexion*/
	/*Ingreso la portada*/
	ingresar_usuario();
	portada = inicializarSprite(SPRITES_ESPERANDO_OPONENTE);
	dibujarSprite(portada,0,0,screen);
	SDL_mutexP(mtx);
	SDL_Flip(screen);
	SDL_mutexV(mtx);

	/*Envio pedido de conexion*/
	connect(caller_socket, (struct sockaddr*)&listen_address, sizeof(struct sockaddr));
	/*Estoy en condiciones de enviar y recibir datos*/
	/*Fin datos de Conexion*/
	/*Final de la portada*/

	/*Enviar el nombre del Jugador al cliente*/
	send(caller_socket, &cadena, strlen(cadena), 0);
	/*Fin envio de nombres al cliente*/

	while(torneoVivo) {
		njug = 0;
		int datosLeidos = recv(caller_socket, &njug, sizeof(int), 0);
		printf("num jugador %d\n", njug);
		if(datosLeidos < 0) {
			imprimirError(4);
		} else if(!datosLeidos || !njug) {
			printf("datosLeidos %d || numJug %d\n", datosLeidos, njug);
			//FUI RECHAZADO POR EL SERVIDOR
			//Imagen de Fui rechazadoquit
			rechazado = inicializarSprite(SPRITES_NO_FUISTE_ACEPTADO);
			dibujarSprite(rechazado,0,0,screen);
			SDL_mutexP(mtx);
			SDL_Flip(screen);
			SDL_mutexV(mtx);
		}
		//Fui aceptado, quedo a la espera de un contrincante
		//Imagen de fui aceptado
		jugando=1;
		printf("Torneo vivo\n");
		aceptado = inicializarSprite(SPRITES_ESPERANDO_OPONENTE);
		dibujarSprite(aceptado,0,0,screen);

		SDL_mutexP(mtx);
		SDL_Flip(screen);
		SDL_mutexV(mtx);
		/*Despues se saca*/
		/*Receive de primer conexion*/
		printf("esperando conexion\n");
		datosLeidos = recv(caller_socket,&miPaquete,sizeof(t_paquete),0);
		if(datosLeidos < 0){
			//Hubo un error en la conexion
			exit(0);
		}
		else if (!datosLeidos) {
			printf("el primer paquete llego vacio\n");
		}
		printf("Recibi primer paquete !!\n");
		fflush(NULL);
		int move=0;
		inicializar(screen);
		/*Se va a encargar de recibir los datos del paquete de datos, llama al metodo que dibujar los Sprites segun el codigo y los dibuja a todos*/
		acciones[0] = SDL_CreateThread(recibirDatos,NULL);
		fflush(NULL);
		/*Se encarga solamente de dibujar*/
		//       acciones[1] = SDL_CreateThread(dibujar,NULL);
		/*El thread Principal se encarga solo de enviar las teclas presionadas luego de algunas validaciones*/
		printf("jugando %d\n", jugando);

		while(jugando && SDL_WaitEvent(&event)){
			//if(njug==1){if(miPaquete.jugadores[0].vidas<0){puts("sali de la partida jug 1");bRun=0;}}
			//if(njug==2){if(miPaquete.jugadores[1].vidas<0){puts("sali de la partida jug 2");bRun=0;}}
			enviar=0;
			switch(event.type){
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == config.k_up){
						enviar=10;//arriba
					}
					if(event.key.keysym.sym == config.k_down){
						enviar=20;//abajo
					}
					if(event.key.keysym.sym == config.k_left){
						enviar=30;//izq
					}
					if(event.key.keysym.sym == config.k_right){
						enviar=40;//der
					}
					if(event.key.keysym.sym == SDLK_ESCAPE){
						enviar=1000;//findetodo
						jugando = 0;
						torneoVivo = 0;
					}
					if(event.key.keysym.sym == config.k_fix){
						/*if(njug==1)
						{ if(move==1)
						{
							jugadores[0] = inicializarSprite(SPRITES_FELIX_MOVE_IZQ);
							move=0;
						}
						else
						{
							jugadores[0] = inicializarSprite(SPRITES_FELIX_IZQ);
							move=1;
						}
						}
						if(njug==2)
						{ if(move==1)
						{
							jugadores[1] = inicializarSprite(SPRITES_FELIX_MOVE_DER);
							move=0;
						}
						else
						{jugadores[1] = inicializarSprite(SPRITES_FELIX_DER);
						move=1;
						}
						}*/
						enviar=1;//space
					}
					break;
				case SDL_QUIT:
					enviar=1000;//findetodo
					jugando = 0;
					torneoVivo = 0;
					break;
				default:
					break;
			}//fin switch
			if (enviar != 0 && miPaquete.jugadores[njug-1].vidas > 0) {
				send(caller_socket, &enviar, sizeof(enviar), 0);
			}
		}//fin del waitevent
		printf("sali de la partida \n");
		//sleep(60);
	}
	return 0;
}

/*Implementacion*/
/*Me encargo de recibir los datos enviados desde el servidor de partida*/
/**
TODO
Mensajes:
cod: 0 - Comienzo de la partida.
cod: 1 - Actualizacion Jugadores.
cod: 2 - Proximo nivel
cod: 3 - Reinicio de partida.
cod: 4 - Fin de la partida.
cod: 5 -Fin de torneo.
*/
int recibirDatos(void * n) {
	cambioNivelTerminado = 1;
	while(jugando) {
		int datosLeidos = recv(caller_socket, &miPaquete, sizeof(t_paquete), 0);
		if(datosLeidos < 0) {
			imprimirError(4);
		} else if (!datosLeidos) {
			printf("metodo recibir: Datos llego un paquete vacio\n");
			dibujarErrorConexion(screen);
		} else {
			/*RECIBO LOS DATOS*/
			// sleep(100000);
			SDL_mutexP(mtx);
			switch(miPaquete.codigoPaquete){
				case 0: break;
				case 1:
					//if(njug==1) {
					//	if(miPaquete.jugadores[0].vidas>0)
					//		dibujar();
					//}
					//if(njug==2) {
					//	if(miPaquete.jugadores[1].vidas>0)
					//		dibujar();
					//}
					dibujar();
					break;
				case 2:
					if(cambioNivelTerminado) {
						cambioNivelTerminado=0;
						enviar = 500;
						send(caller_socket, &enviar, sizeof(enviar), 0);
						dibujarSiguienteNivel();
						printf("Cambio de nivel terminado. \n");
					}
					break;
				case 4:
					printf("dibujoTecho\n");
					dibujarTecho(screen);
					jugando=0;
					enviar = 600;
					send(caller_socket, &enviar, sizeof(enviar), 0);
					break;
				case 7:
					dibujarErrorConexion(screen);
					jugando = 0;
					torneoVivo = 0;
					break;
			}
			// usleep(10000); esto petea todo
			SDL_mutexV(mtx);
		}
	}//fin del while
	return 0;
}

int dibujar(){
	int hilera = 0, ventana = 0;
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
	dibujarTorta(screen);
	if(miPaquete.jugadores[0].vidas > 0)
		dibujarSprite(jugadores[0], miPaquete.jugadores[0].coordenadas.x, miPaquete.jugadores[0].coordenadas.y,screen);
	if(miPaquete.jugadores[1].vidas > 0)
		dibujarSprite(jugadores[1], miPaquete.jugadores[1].coordenadas.x, miPaquete.jugadores[1].coordenadas.y,screen);
	dibujarSprite(ladrillos[0], miPaquete.ladrillos[0].x,miPaquete.ladrillos[0].y,screen);
	dibujarSprite(ladrillos[1], miPaquete.ladrillos[1].x,miPaquete.ladrillos[1].y,screen);
	dibujarSprite(ladrillos[2], miPaquete.ladrillos[2].x,miPaquete.ladrillos[2].y,screen);
	dibujarSprite(pajaros[0],miPaquete.gaviotas[0].x, miPaquete.gaviotas[0].y, screen);
	dibujarSprite(pajaros[1],miPaquete.gaviotas[1].x, miPaquete.gaviotas[1].y, screen);
	dibujarSprite(pajaros[2],miPaquete.gaviotas[2].x, miPaquete.gaviotas[2].y, screen);
	dibujarSprite(ralph, miPaquete.ralph.x, miPaquete.ralph.y, screen);
	//SDL_BlitSurface(texto2,NULL,screen,&contenedorTexto2);
	mostrarInformacion(screen);

	if(miPaquete.nivel == 1){

		//Dibujo la maquesina del nivel 2
		if(miPaquete.marquesina[0] < 5){
			hilera = 0;
			ventana = miPaquete.marquesina[0];
		}else if(miPaquete.marquesina[0] >= 5 && miPaquete.marquesina[0] < 10){
			hilera = 1;
			ventana = miPaquete.marquesina[0] - 5;
		}

		//printf("Hilera: %d, Ventana: %d\n", hilera, ventana);

		dibujarSprite(marquesina[0], posicionesMarquesinas[hilera][ventana].x, posicionesMarquesinas[hilera][ventana].y, screen);
	}else if(miPaquete.nivel == 2){
		//Dibujo las maquesinas del nivel 3
		if(miPaquete.marquesina[1] < 5){
			hilera = 0;
			ventana = miPaquete.marquesina[1];
		}else if(miPaquete.marquesina[1] >= 5 && miPaquete.marquesina[1] < 10){
			hilera = 1;
			ventana = miPaquete.marquesina[1] - 5;
		}
		//printf("Hilera: %d, Ventana: %d\n", hilera, ventana);

		dibujarSprite(marquesina[1], posicionesMarquesinas[hilera][ventana].x, posicionesMarquesinas[hilera][ventana].y, screen);
		if(miPaquete.marquesina[2] < 5){
			hilera = 0;
			ventana = miPaquete.marquesina[2];
		}else if(miPaquete.marquesina[2] >= 5 && miPaquete.marquesina[2] < 10){
			hilera = 1;
			ventana = miPaquete.marquesina[2] - 5;
		}

		//printf("Hilera: %d, Ventana: %d\n", hilera, ventana);
		dibujarSprite(marquesina[2], posicionesMarquesinas[hilera][ventana].x, posicionesMarquesinas[hilera][ventana].y, screen);
	}

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
		printf("Error en la carga de los parametros\n");
		break;
	case 2:
		printf("No se puede inicializar el modo grafico: \n",SDL_GetError());
		break;
	case 3:
		printf("Error en al creacion del semaforo mutex\n");
		break;
	case 4:
		printf("Se perdio la conexion\n");
		break;
	}
	exit(EXIT_FAILURE);
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
	/*Leo fix*/
	fscanf(ARTXT,"%s",aux);
	intAux = extraerTecla(aux);
	if(intAux != -1)
	{
		conf->k_fix =intAux;
		printf("TECLA FIX: %d\n",conf->k_fix);
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
			if(!strcmp(aux,"ESPACIO"))
				return SDLK_SPACE;
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
	inicializar_marquesinas();
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
	jugadores[0] = inicializarSprite(SPRITES_FELIX_IZQ);
	jugadores[1] = inicializarSprite(SPRITES_FELIX_DER);
	pajaros[0] = inicializarSprite(SPRITES_AVE);
	pajaros[1] = inicializarSprite(SPRITES_AVE);
	pajaros[2] = inicializarSprite(SPRITES_AVE);
	ralph = inicializarSprite(SPRITES_RALPH);
	torta[0] = inicializarSprite(SPRITES_TORTA_1);
	torta[1] = inicializarSprite(SPRITES_TORTA_2);

	//marquesinas
	marquesina[0] = inicializarSprite(SPRITES_MARQUESINA);
	marquesina[1] = inicializarSprite(SPRITES_MARQUESINA);
	marquesina[2] = inicializarSprite(SPRITES_MARQUESINA);

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
	codigoEdificio=1;//nivel 2
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
		posicionYEdificioAnterior+=5;
		posicionYEdificioActual+=5;
		SDL_Flip(screen);
		SDL_mutexV(mtx);
		SDL_Delay(1);
	}
	//dibujarSprite(edificios[2], 60, 0,screen);
	if(njug==1){ //Jugador 1
		printf("Puntos del jugador 1: %d\n", miPaquete.jugadores[0].puntos);
		if(miPaquete.jugadores[0].puntos >= miPaquete.jugadores[1].puntos){ //Gane
			resultado = inicializarSprite(SPRITES_GANADO);
			SDL_mutexP(mtx);
			dibujarSprite(resultado,0,0,screen);
			SDL_Flip(screen);
			SDL_mutexV(mtx);
		}else{//Perdi
			resultado = inicializarSprite(SPRITES_PERDIDO);
			SDL_mutexP(mtx);
			dibujarSprite(resultado,0,0,screen);
			SDL_Flip(screen);
			SDL_mutexV(mtx);
		}
	}else if(njug==2){ //Jugador 2
		printf("Puntos del jugador 2: %d\n", miPaquete.jugadores[1].puntos);
		if(miPaquete.jugadores[1].puntos > miPaquete.jugadores[0].puntos){ //Gane
			resultado = inicializarSprite(SPRITES_GANADO);
			SDL_mutexP(mtx);
			dibujarSprite(resultado,0,0,screen);
			SDL_Flip(screen);
			SDL_mutexV(mtx);
		}else{//Perdi
			resultado = inicializarSprite(SPRITES_PERDIDO);
			SDL_mutexP(mtx);
			dibujarSprite(resultado,0,0,screen);
			SDL_Flip(screen);
			SDL_mutexV(mtx);
		}
	}
	//fin de todo
	printf("Puntajes Jugador N 1: %d\n", miPaquete.jugadores[0].puntos);
	printf("Puntajes Jugador N 2: %d\n", miPaquete.jugadores[1].puntos);
	sleep(3);
	//finalizar();
}

void mostrarVidas() {
	SDL_Surface *texto2;
	char aux[20];
	char aux2[26]="Vidas1: ";
	SDL_Color color = {0,255,0};
	sprintf(aux,"%d",miPaquete.jugadores[0].vidas);
	strcat(aux2,aux);
	texto2 = TTF_RenderText_Solid(fuente2,aux2,color);
	SDL_Rect contenedorTexto2;
	contenedorTexto2.x = 0;
	contenedorTexto2.y = 20;
	contenedorTexto2.w = 640;
	contenedorTexto2.h = 50;
	SDL_BlitSurface(texto2,NULL,screen,&contenedorTexto2);
	// segundo jugador
	sprintf(aux,"%d",miPaquete.jugadores[1].vidas);
	strcpy(aux2,"Vidas2: ");
	strcat(aux2,aux);
	texto2 = TTF_RenderText_Solid(fuente2,aux2,color);
	contenedorTexto2.x = 500;
	contenedorTexto2.y = 20;
	contenedorTexto2.w = 640;
	contenedorTexto2.h = 50;
	SDL_BlitSurface(texto2,NULL,screen,&contenedorTexto2);
}

void mostrarPuntos() {
	char aux[200];
	char aux2[260]="Puntos1: ";
	SDL_Surface *texto2;
	SDL_Color color = {255,255,125};
	sprintf(aux,"%d",miPaquete.jugadores[0].puntos);
	strcat(aux2,aux);
	texto2 = TTF_RenderText_Solid(fuente2,aux2,color);
	SDL_Rect contenedorTexto2;
	contenedorTexto2.x = 150;
	contenedorTexto2.y = 0;
	contenedorTexto2.w = 640;
	contenedorTexto2.h = 50;
	SDL_BlitSurface(texto2,NULL,screen,&contenedorTexto2);
	strcpy(aux2,"Puntos2: ");
	sprintf(aux,"%d",miPaquete.jugadores[1].puntos);
	strcat(aux2,aux);
	texto2 = TTF_RenderText_Solid(fuente2,aux2,color);
	contenedorTexto2.x = 500;
	contenedorTexto2.y = 0;
	contenedorTexto2.w = 640;
	contenedorTexto2.h = 50;
	SDL_BlitSurface(texto2,NULL,screen,&contenedorTexto2);
}

void mostrarInformacion(SDL_Surface *screen) {
	SDL_Surface *texto2;
	SDL_Rect contenedorTexto2;
	SDL_Color color = {255,0,255};
	char aux[30];

	contenedorTexto2.x = 0;
	contenedorTexto2.y = 0;
	contenedorTexto2.w = 640;
	contenedorTexto2.h = 50;
	texto2 = TTF_RenderText_Solid(fuente2, miPaquete.jugadores[0].nombre, color);
	SDL_FillRect(screen,&contenedorTexto2,SDL_MapRGB(screen->format,0,0,0));
	SDL_BlitSurface(texto2,NULL,screen,&contenedorTexto2);

	contenedorTexto2.x = 290;
	contenedorTexto2.y = 0;
	contenedorTexto2.w = 640;
	contenedorTexto2.h = 50;
	texto2 = TTF_RenderText_Solid(fuente2, miPaquete.jugadores[1].nombre, color);
	SDL_FillRect(screen,&contenedorTexto2,SDL_MapRGB(screen->format,0,0,0));
	SDL_BlitSurface(texto2,NULL,screen,&contenedorTexto2);

	mostrarVidas();
	mostrarPuntos();
}

void ingresar_usuario() {
	SDL_Event evento;
	SDL_Surface *portada = SDL_LoadBMP(SPRITES_PORTADA);
	SDL_Rect cuad;
	cuad.x = 0;
	cuad.y = 0;
	cuad.w = 508;
	cuad.h = 552;
	SDL_BlitSurface(portada,NULL,screen,&cuad);
	TTF_Font *fuente;
	SDL_Surface *texto;

	fuente = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf",25);
	SDL_Color color = {255,255,255};
	texto = TTF_RenderText_Solid(fuente,"Ingrese su nombre: ",color);
	SDL_Rect contenedorTexto;
	contenedorTexto.x = 250;
	contenedorTexto.y = 400;
	contenedorTexto.w = 150;
	contenedorTexto.h = 50;
	SDL_BlitSurface(texto,NULL,screen,&contenedorTexto);
	SDL_Flip(screen);

	cadena[0]='\0';
	int pos = 0;
	//SDL_Event eventoPri;
	int mayuscula = 0;
	do
	{
		SDL_WaitEvent(&evento);
		if(evento.type == SDL_KEYDOWN)
		{
			if(pos<25 && ((evento.key.keysym.sym >= SDLK_a && evento.key.keysym.sym <= SDLK_z) || (evento.key.keysym.sym >= SDLK_0 && evento.key.keysym.sym <=SDLK_9)))
			{
				if(mayuscula && (evento.key.keysym.sym >= SDLK_a && evento.key.keysym.sym <= SDLK_z))
				{
					cadena[pos++] = (int)evento.key.keysym.sym -32;
					mayuscula--;
				}

				else
					cadena[pos++] = (int)evento.key.keysym.sym;
				cadena[pos] = '\0';
				texto = TTF_RenderText_Solid(fuente,cadena,color);
				contenedorTexto.y = 430;
				SDL_BlitSurface(texto,NULL,screen,&contenedorTexto);
				SDL_Flip(screen);
			}
			if(pos<25 && (evento.key.keysym.sym == SDLK_LSHIFT || evento.key.keysym.sym == SDLK_RSHIFT))//Para mayusculas
				mayuscula = 1;
			if(pos>0 && evento.key.keysym.sym == SDLK_BACKSPACE)
			{
				cadena[--pos] = '\0';
				texto = TTF_RenderText_Solid(fuente,cadena,color);
				contenedorTexto.y = 430;
				SDL_BlitSurface(portada,&contenedorTexto,screen,&contenedorTexto); //Cuando borra limpia fondo
				SDL_BlitSurface(texto,NULL,screen,&contenedorTexto);
				SDL_Flip(screen);
			}
			if(strlen(cadena)==0&&evento.key.keysym.sym == SDLK_RETURN)
			{
				pantallaNegra.x=0;
				pantallaNegra.y=0;
				pantallaNegra.w=504;
				pantallaNegra.h=540;
				SDL_FillRect(screen,&pantallaNegra,SDL_MapRGB(screen->format,0,0,0));
				texto = TTF_RenderText_Solid(fuente,"Debe especificar un nombre",color);
				//SDL_Rect contenedorTexto;
				contenedorTexto.x = 100;
				contenedorTexto.y = 250;
				contenedorTexto.w = 150;
				contenedorTexto.h = 50;
				SDL_BlitSurface(texto,NULL,screen,&contenedorTexto);
				SDL_Flip(screen);
				sleep(1);
				cuad.x = 0;
				cuad.y = 0;
				cuad.w = 508;
				cuad.h = 552;
				contenedorTexto.x = 250;
				contenedorTexto.y = 400;
				contenedorTexto.w = 150;
				contenedorTexto.h = 50;
				SDL_BlitSurface(portada,NULL,screen,&cuad);
				texto = TTF_RenderText_Solid(fuente,"Ingrese su nombre: ",color);
				SDL_BlitSurface(texto,NULL,screen,&contenedorTexto);
				SDL_Flip(screen);
			}
		}
	} while(evento.key.keysym.sym != SDLK_RETURN||strlen(cadena)==0);
}

void inicializar_marquesinas(){
	int primeraHilera = 221;
	int segundaHilera = 331;
	int i, j;
	for(i = 0; i<2;i++){
		int comienzoX1 = 125;
		for(j = 0; j<5; j++){
			posicionesMarquesinas[i][j].x = comienzoX1;
			posicionesMarquesinas[i][j].y = i == 0 ? primeraHilera : segundaHilera;
			comienzoX1+=78;
		}
	}
}

void dibujarTorta(SDL_Surface *screen){
	if(miPaquete.torta.dibujar == 1){
		dibujarSprite(torta[0], miPaquete.torta.coordenadas.x, miPaquete.torta.coordenadas.y,screen);
		if(codigoTorta == 0){
			dibujarSprite(torta[0], miPaquete.torta.coordenadas.x, miPaquete.torta.coordenadas.y,screen);
			codigoTorta = 1;
		}else{
			dibujarSprite(torta[1], miPaquete.torta.coordenadas.x, miPaquete.torta.coordenadas.y,screen);
			codigoTorta = 0;
		}
	}
}


void dibujarErrorConexion(SDL_Surface *screen){
	printf("CLIENTE: Error de conexion recibido\n");
	SDL_Surface *errorConexion = inicializarSprite(SPRITES_ERROR_CONEXION);
	dibujarSprite(errorConexion,0,0,screen);
	SDL_mutexP(mtx);
	SDL_Flip(screen);
	SDL_mutexV(mtx);

	sleep(2);
	printf("Cerrando el jugador\n");
	//finalizar();
}
