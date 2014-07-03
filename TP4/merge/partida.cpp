/*#####################################
#Trabajo Practico Nº4
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#Garcia Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/

#include "partida.h"
#define TRUE 1
#define FALSE 0


int partida,memId_vectorCliente,memId_vectorPartidas,pidServer;
int semId_vectorCliente, semId_colaMensajesDeCliente, semId_colaMensajesACliente,semId_vectorPartidas;
struct s_datosCliente *v_datosCliente;
struct s_datosPartida *v_datosPartida;
//Cola de mensajes que se envian desde los clientes
struct tcola *c_mensajesDeCliente;
//Cola de mensajes que se envian a los clientes
struct tcola *c_mensajesACliente;

//partida negro
int VentanasArregladas =0; 
int matrizladrillos[3][3]={{120,140,200},{220,260,300},{420,450,480}};
int movimiento=random()%3;


int main(int argc, char *argv[]) {
	/*VARIABLES*/
	//Threads
	pthread_t t_escuchaCliente1;
	pthread_t t_escuchaCliente2;
	pthread_t t_enviaClientes;
	/*FIN VARIABLES*/
	/*INICIALIZACION de VARIABLES con los datos dados por el torneo*/
	memId_vectorCliente = atoi(argv[1]);
	semId_vectorCliente = atoi(argv[2]);
	semId_vectorPartidas=atoi(argv[3]);
	memId_vectorPartidas=atoi(argv[4]);
	partida = atoi(argv[7]);
	
	//(memId_vectorCliente, semId_vectorCliente,semId_vectorPartidas,memId_vectorPartidas, a, b)
	//datosPartida.flag_partidaViva = 1;
	
	//Vector en memoria compartida que aloja los clientes conectados
	v_datosCliente = (s_datosCliente *)shmat(memId_vectorCliente,0,0);
	v_datosPartida = (s_datosPartida *)shmat(memId_vectorPartidas,0,0);	
	
	sem_P(semId_vectorPartidas);
	v_datosPartida[partida].idCliente1 = atoi(argv[5]);
	v_datosPartida[partida].idCliente2 = atoi(argv[6]);
	v_datosPartida[partida].pidPartida=getpid();	
	v_datosPartida[partida].flag_partidaViva = 1;
	
	pidServer=atoi(argv[8]);	
	sem_P(semId_vectorCliente);
	v_datosPartida[partida].socketCliente1 = v_datosCliente[v_datosPartida[partida].idCliente1].socket;
	v_datosPartida[partida].socketCliente2 = v_datosCliente[v_datosPartida[partida].idCliente2].socket;
	comm_socket=v_datosPartida[partida].socketCliente1;
	comm_socket2=v_datosPartida[partida].socketCliente2;	
	//envia idClienteEn Partida
	int id;
	id=v_datosCliente[v_datosPartida[partida].idCliente1].idEnPartida;
	send(comm_socket, &id, sizeof(int), 0);	
	id=v_datosCliente[v_datosPartida[partida].idCliente2].idEnPartida;
	send(comm_socket2, &id, sizeof(int), 0);			
	sem_V(semId_vectorCliente);	
	sem_V(semId_vectorPartidas);
		
	//Inicializo la cola de mensajes enviados por el cliente y su semaforo
	/*semId_colaMensajesDeCliente = crear_sem(IPC_PRIVATE, 1);
	if(semId_colaMensajesDeCliente == -1) {
		imprimirError(0, "Error al crear el semaforo");
	}
	crear(&c_mensajesDeCliente);
	//Inicializo la cola de mensajes para enviar a los clientes y su semaforo
	semId_colaMensajesACliente = crear_sem(IPC_PRIVATE, 1);
	if(semId_colaMensajesACliente == -1) {
		imprimirError(0, "Error al crear el semaforo");
	}*/
	//crear(&c_mensajesACliente);
	/*FIN INICIALIZACION de VARIABLES*/
	// Se crea thread de escucha de nuevos clientes para el torneo
	//sem_P(semId_vectorCliente);
	pthread_create(&t_escuchaCliente1, NULL, moverJugador1, NULL); 
	pthread_create(&t_escuchaCliente2, NULL, moverJugador2, NULL);
	//pthread_create(&t_enviaClientes, NULL, enviaCliente, NULL);
	//sem_V(semId_vectorCliente);
	
	
	//Magia negro TODO: Meter en funcion
	int contador = 0;
	int recibirNivelTerminado;
	int codigoPasoNivel;
    //esto lo levanto desdes el configurador 
    miPaquete.jugador1.vidas=3;
    miPaquete.jugador2.vidas=3;
    // pongo todo en su lugar antes de arrancar.
    inicializar();
	while(partidaActiva){	
        if(miPaquete.nivel==3){miPaquete.codigoPaquete = 4;		
            partidaActiva=0;
		}
		switch(miPaquete.codigoPaquete){
			case 0: break;
			case 1:  //partida inicial Nivel 1   
                dibujarVidrios(tipoEdificio);              
                //Hace que los pajaros se muevan , los ladrillos y ralph                  
                movimientoPajarosLadrillosRalph(miPaquete.nivel);
                //verifico colision 
                colisicionPajaros();
                colisicionLadrillos();  
                //verifico si la cantidad de ventanas a arreglar fue superada 
                if( VentanasArregladas > 7){
					printf("Voy a pasar de nivel \n");
                    miPaquete.nivel++;
					miPaquete.codigoPaquete = 2;
					//imagino q esto sirve para algo pero no se para que 
                    partidaPrimeraVez = 1;
					//tipoEdificio = 1;
				}
				break;
			case 2:
				printf("MI PAQUETE 2\n"); // cambiando al NIVEL 2
				break;
			case 3: break;
			case 4: break;
		}	    
           
	    //hago el envio a los cliente con la nueva informacion esto se hace todo el tiempo 
        if(send(comm_socket, &miPaquete, sizeof(t_paquete), 0)< 0){
			printf("Error en el socket: %d\n", comm_socket);
		}
		if(send(comm_socket2, &miPaquete, sizeof(t_paquete), 0)< 0){
			printf("Error en el socket: %d\n", comm_socket);
		}	
		usleep(75000);           
	}// fin del while de partida
	sleep(60);
	
	//fin magia negro

	
	pthread_join(t_escuchaCliente1, NULL);
	pthread_join(t_escuchaCliente2, NULL);
	//pthread_join(t_enviaClientes, NULL);
	
	verificaEsServerAlive();
	//cerramos el semaforo de la cola de mensajes del cliente
	if(cerrar_sem(semId_colaMensajesDeCliente) == -1) {
		imprimirError(0, "Error al cerrar los semaforos");
	}
	//cerramos el semaforo de la cola de mensajes para el cliente
	if(cerrar_sem(semId_colaMensajesACliente) == -1) {
		imprimirError(0, "Error al cerrar los semaforos");
	}
	//Se libera el uso del vector por este proceso
	shmdt((char *) v_datosCliente);
	exit(EXIT_SUCCESS);
}

/**
*FUNCION DE MANEJO DE ERRORES
*/
void imprimirError(int codigo, const char *msg) {
	switch(codigo) {
		case 1: printf("Falta completar algun/os parametros.\n");
				printf("Consulte la ayuda con -h para mas informacion\n"); break;
		case 2: printf("Error. Los parametros ingresados no son validos.\n");
				printf("Consulte la ayuda con -h para mas informacion\n"); break;
		case 3: printf("Error. No se pudo escribir en el socket.\n"); break;
		case 4: printf("Error en la apertura del archivo.\n"); break;
		default: break;
	}
	fflush(NULL);
	if (msg != NULL) {
		printf("%s \n",msg);
	}
	if (errno) {
		printf("\n %d : %s \n", errno, strerror(errno));
	}
	//exit(EXIT_FAILURE); //TODO: esta funcion debe tener el control de terminación de ejec??
}

/**
*FUNCION QUE LEE LOS MENSAJES ENVIADOS DEL CLIENTE
*/

void *leeCliente(void* argumentos) {
	/*int socketCliente = *((int*) argumentos);
	char buffer[BUFFERSIZE]; //contendra los datos leidos o enviados, el tamaño esconfigurado con la variable BUFFERSIZE 
	bzero(buffer,BUFFERSIZE);
		
	//Lee datos del socket del cliente, leerá el tamaño del buffer o la cantidad indicada en el parametro 3 lo que sea que sea menor.
	//read(socket, buffer donde carga el mensaje, cantidad)
	int flagCliente = 1;
	while (v_datosPartida[partida].flag_partidaViva && flagCliente) {
		if ((read(socketCliente, buffer, BUFFERSIZE)) <= 0) {
			// TODO : ver si se debe cerrar el socket desde la partida
			close(socketCliente);
			// FIN TODO
			flagCliente=0;
			imprimirError(0, "ERROR no se puede leer del cliente.");
		// TODO: El siguiente "else if" debe volar cuando se termine de testear
		} else if (strcmp(buffer, "SALIR\n") == 0) {
			flagCliente = 0;
			printf("\nESCRIBIO SALIR\n");
			fflush(NULL);
			if ((write(socketCliente, "Se ha desconectado exitosamente", 31)) < 0) {
				imprimirError(3, "");
			}
		// FIN TODO
		} else {
			sem_P(semId_colaMensajesDeCliente);
			encolar(&c_mensajesDeCliente, (void*) (atoi(buffer)));
			sem_V(semId_colaMensajesDeCliente);
		}
		bzero(buffer, BUFFERSIZE);
	}*/
	int recibir=0;
	while(recibir!=1000){
		if(recv(comm_socket,&recibir,sizeof(int),0)>=0)
		{   
			switch(recibir){
				case 10://arriba
						if((miPaquete.jugador1.coordenadas.y - 120) >= 80){
							miPaquete.jugador1.coordenadas.y =  miPaquete.jugador1.coordenadas.y - 120;
						}
						break;
				case 20://abajo
						if((  miPaquete.jugador1.coordenadas.y + 120) <= 450){
							miPaquete.jugador1.coordenadas.y = miPaquete.jugador1.coordenadas.y + 120;
						}
						break;
				case 30://izq
						if(( miPaquete.jugador1.coordenadas.x - 85) >= 120 ){
							miPaquete.jugador1.coordenadas.x =  miPaquete.jugador1.coordenadas.x -75;
						}
						break;
				case 40://der
						if(( miPaquete.jugador1.coordenadas.x + 85) <= 500 ){
							miPaquete.jugador1.coordenadas.x =  miPaquete.jugador1.coordenadas.x +75;
						}
						break;
				case 1000:  break;
                case 500:
						printf("Nivel %d \n",miPaquete.nivel);
                        VentanasArregladas =0;
                        miPaquete.codigoPaquete = 1;
                        inicializar();
                        //esto esta muy mal pero si no no sigue
                        miPaquete.jugador2.puntos = miPaquete.jugador1.puntos=0;
                        break;
				case 1:	
						/*Llego arreglo de ventana desde el jugador 1*/
						arregloVentana(0);
						break;
				default:
						break;

			}
			send(comm_socket2, &miPaquete, sizeof(t_paquete), 0);
			send(comm_socket, &miPaquete, sizeof(t_paquete), 0);
		}
		else{
			printf("error en la conexion del cliente");
			exit(0);
		}
	}
	pthread_exit(NULL);
}

/**
*FUNCION QUE ENVIA LOS MENSAJES AL CLIENTE
*/
void *enviaCliente(void* argumentos) {
	int flagCliente1 = 1, flagCliente2 = 1;
	void* nodo;
	int elementoDeCola = 0;
	while (v_datosPartida[partida].flag_partidaViva && (flagCliente1 || flagCliente2)) {
		if (!vacia(c_mensajesACliente)) {
			sem_P(semId_colaMensajesACliente);
			desencolar(&c_mensajesACliente, &nodo);
			elementoDeCola = *((int*)nodo);
			sem_V(semId_colaMensajesACliente);
			//Envia mensajes a ambos clientes
			if (flagCliente1 && (write(v_datosPartida[partida].socketCliente1, &elementoDeCola, sizeof(elementoDeCola))) < 0) {
				// TODO : ver si se debe cerrar el socket desde la partida
				close(v_datosPartida[partida].socketCliente1);
				// FIN TODO
				flagCliente1 = 0;
				imprimirError(0, "ERROR escribiendo en el socket");
			}
			if (flagCliente2 && (write(v_datosPartida[partida].socketCliente2, &elementoDeCola, sizeof(elementoDeCola))) < 0) {
				// TODO : ver si se debe cerrar el socket desde la partida
				close(v_datosPartida[partida].socketCliente2);
				// FIN TODO
				flagCliente2 = 0;
				imprimirError(0, "ERROR escribiendo en el socket");
			}
			elementoDeCola = 0; //limpia el elemento
		}
	}
	pthread_exit(NULL);
}

void verificaEsServerAlive(){
	if(kill(pidServer,0)<0){
		//TODO: cerrar todo	
	}
}

int colision(int x1,int w1,int h1,int y1 ,int x2,int w2,int h2,int y2){
	if (((x1+w1)>x2)&&((y1+h1)>y2)&&((x2+w2)>x1)&&((y2+h2)>y1)) {
		return TRUE;
	} else {
		return FALSE;
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
		totalVidrios = 40;
	}
	for(i=0;i<totalVidrios;i+=2){
		if((completo == 0 && (i==10 || i==20 || i==28 || i==36)) ||
		(completo == 1 && (i==10 || i==20 || i==30 || i==40))){
			x = comienzoX;
			switch(i){
				case 20: y+=110; break;
				case 28: if(completo == 0) y+=135; break;
				case 30: if(completo == 1) y+=135; break;
				default: y+=120; break;
			}
		}
		if(completo != 1 && (i == 24 || i == 32)){
			x+=distanciaEntreVidrios;
		}
		//TODO mandar paquete
		miPaquete.vidrios[i].coordenadas.x = x;
		miPaquete.vidrios[i].coordenadas.y = y;
		miPaquete.vidrios[i+1].coordenadas.x = x;
		miPaquete.vidrios[i+1].coordenadas.y = y+30;
		if(partidaPrimeraVez){
			int random;

			random = rand()%2;
			ventanasParesRotas[i][0] = random;
			miPaquete.vidrios[i].roto = random;

			random = rand()%2;
			ventanasParesRotas[i][1] = random;
			miPaquete.vidrios[i+1].roto = random;
		}
		//dibujarSprite(vidrios[i], x, y,screen);
		//dibujarSprite(vidrios[i], x, y+30,screen);
		x+=distanciaEntreVidrios;
	}
	partidaPrimeraVez = 0;
}


void * moverJugador1(void *n){
	int recibir=0;
	while(recibir!=1000){
		if(recv(comm_socket,&recibir,sizeof(int),0)>=0)
		{   
			switch(recibir){
				case 10://arriba
						if((miPaquete.jugador1.coordenadas.y - 120) >= 80){
							miPaquete.jugador1.coordenadas.y =  miPaquete.jugador1.coordenadas.y - 120;
						}
						break;
				case 20://abajo
						if((  miPaquete.jugador1.coordenadas.y + 120) <= 450){
							miPaquete.jugador1.coordenadas.y = miPaquete.jugador1.coordenadas.y + 120;
						}
						break;
				case 30://izq
						if(( miPaquete.jugador1.coordenadas.x - 85) >= 120 ){
							miPaquete.jugador1.coordenadas.x =  miPaquete.jugador1.coordenadas.x -75;
						}
						break;
				case 40://der
						if(( miPaquete.jugador1.coordenadas.x + 85) <= 500 ){
							miPaquete.jugador1.coordenadas.x =  miPaquete.jugador1.coordenadas.x +75;
						}
						break;
				case 1000:  break;
                case 500:
						printf("Nivel %d \n",miPaquete.nivel);
                        VentanasArregladas =0;
                        miPaquete.codigoPaquete = 1;
                        inicializar();
                        //esto esta muy mal pero si no no sigue
                        miPaquete.jugador2.puntos = miPaquete.jugador1.puntos=0;
                        break;
				case 1:	
						/*Llego arreglo de ventana desde el jugador 1*/
						arregloVentana(0);
						break;
				default:
						break;

		}
		send(comm_socket2, &miPaquete, sizeof(t_paquete), 0);
		send(comm_socket, &miPaquete, sizeof(t_paquete), 0);
	}
	else{
		printf("error en la conexion del cliente");
		exit(0);
	}
	}
}

void * moverJugador2(void *n){
	int recibir=0;
	while(recibir!=1000){
		if(recv(comm_socket2,&recibir,sizeof(int),0)>=0)
		{  //prueba de colision
			switch(recibir){
				case 10: //arriba
					if((miPaquete.jugador2.coordenadas.y - 120) >= 80){
						miPaquete.jugador2.coordenadas.y =  miPaquete.jugador2.coordenadas.y - 120;
					}
					break;
				case 20: //abajo
					if((  miPaquete.jugador2.coordenadas.y + 120) <= 450){
						miPaquete.jugador2.coordenadas.y = miPaquete.jugador2.coordenadas.y + 120;
					}
					break;
				case 30://izq
					if(( miPaquete.jugador2.coordenadas.x - 75) >= 90 ){
						miPaquete.jugador2.coordenadas.x =  miPaquete.jugador2.coordenadas.x -75;
					}
					break;
				case 40: //der
					if(( miPaquete.jugador2.coordenadas.x + 75) <= 450 ){
						miPaquete.jugador2.coordenadas.x =  miPaquete.jugador2.coordenadas.x +75;
				}
				case 1000:
					/*El jugador 2 se fue.*/
					break;
                case 500:printf("Nivel %d \n",miPaquete.nivel);
                    VentanasArregladas =0;
					miPaquete.codigoPaquete = 1;
					inicializar();
                    //esto esta muy mal pero si no no sigue
                    miPaquete.jugador2.puntos = miPaquete.jugador1.puntos=0;
                    break;    
				case 1:
					/*RecibÃ­ la tecla para arreglar la ventana desde el jugador 2*/
					arregloVentana(1);
					break;
				default:
					break;
			}
			send(comm_socket2, &miPaquete, sizeof(t_paquete), 0);
			send(comm_socket, &miPaquete, sizeof(t_paquete), 0);
		}
		else{
			printf("error en la conexion del cliente\n");
			exit(0);
		}
	}


}


int arregloVentana(int jugador){
	/*Verifico si Felix estÃ¡ parado en alguna ventana y hay ventanas por reparar.
	En caso de verdadero, las reparo, sumo puntos y cambio el sprite*/
	/*Las ventanas siempre van de a pares, cuando encuentro que colisionÃ© con una
	ventana, me fijo de arreglar sÃ³lo una ventana por vez.*/
	int i;
	/*Recorro todas las ventanas*/
	for(i = 0; i < 40; i+=2){
		if(jugador == 0){
			if(colision(miPaquete.vidrios[i].coordenadas.x,28,25,miPaquete.vidrios[i].coordenadas.y, miPaquete.jugador1.coordenadas.x,68,90,miPaquete.jugador1.coordenadas.y)==TRUE){
					printf("Colisiona con ventana\n");
					printf("Ventanas: %d, rota: %d, rota: %d\n", i, ventanasParesRotas[i][0], ventanasParesRotas[i][1] );
					if(ventanasParesRotas[i][0] == 1){
                                                        miPaquete.jugador1.puntos++;VentanasArregladas++;
							miPaquete.vidrios[i].roto = 0; //Lo arreglo
							ventanasParesRotas[i][0] = 0;
					}else if(ventanasParesRotas[i][1] == 1){
                                                        miPaquete.jugador1.puntos++;VentanasArregladas++;						
							miPaquete.vidrios[i+1].roto = 0;
							ventanasParesRotas[i][1] = 0;
					}
				/*TODO Recibo premio*/
				/*Sumo Puntos*/
			}
		}
		else{
			if(colision(miPaquete.vidrios[i].coordenadas.x,28,25,miPaquete.vidrios[i].coordenadas.y, miPaquete.jugador2.coordenadas.x,68,90,miPaquete.jugador2.coordenadas.y)==TRUE){
				printf("Colisiona con ventana\n");

				printf("Ventanas: %d, rota: %d, rota: %d\n", i, ventanasParesRotas[i][0], ventanasParesRotas[i][1] );
				if(ventanasParesRotas[i][0] == 1){VentanasArregladas++;
                                        miPaquete.jugador2.puntos++;
					ventanasParesRotas[i][0] = 0;
					miPaquete.vidrios[i].roto = 0; //Lo arreglo
				}else if(ventanasParesRotas[i][1] == 1){miPaquete.jugador2.puntos++;VentanasArregladas++;
					printf("Estoy arreglando la ventana de abajo!\n");
						ventanasParesRotas[i][1] = 0;
						miPaquete.vidrios[i+1].roto = 0; //Lo arreglo
				}
				/*TODO Recibo premio*/
				/*Sumo puntos*/
			}
		}
	}
}

///funcion que sive para ver si los pajaros colisionan con los jugadores
//no recibe parametros
void colisicionPajaros(){
	if(colision(miPaquete.gaviotas[0].x,54,20,miPaquete.gaviotas[0].y, miPaquete.jugador2.coordenadas.x,60,80,miPaquete.jugador2.coordenadas.y)==TRUE){
		miPaquete.jugador2.coordenadas.x = 430;miPaquete.jugador2.vidas--;
	    miPaquete.jugador2.coordenadas.y = 365;
	}
	if(colision(miPaquete.gaviotas[1].x,54,20,miPaquete.gaviotas[1].y, miPaquete.jugador2.coordenadas.x,60,80,miPaquete.jugador2.coordenadas.y)==TRUE){
	miPaquete.jugador2.coordenadas.x = 430;miPaquete.jugador2.vidas--;
	miPaquete.jugador2.coordenadas.y = 365;
	}
	if(colision(miPaquete.gaviotas[2].x,54,20,miPaquete.gaviotas[2].y, miPaquete.jugador2.coordenadas.x,60,80,miPaquete.jugador2.coordenadas.y)==TRUE){
	miPaquete.jugador2.coordenadas.x = 430;miPaquete.jugador2.vidas--;
	miPaquete.jugador2.coordenadas.y = 365;
	}
	if(colision(miPaquete.gaviotas[0].x,54,20,miPaquete.gaviotas[0].y, miPaquete.jugador1.coordenadas.x,60,80,miPaquete.jugador1.coordenadas.y)==TRUE){
	miPaquete.jugador1.coordenadas.x = 125;miPaquete.jugador1.vidas--;
	miPaquete.jugador1.coordenadas.y = 365;	
	}
	if(colision(miPaquete.gaviotas[1].x,54,20,miPaquete.gaviotas[1].y, miPaquete.jugador1.coordenadas.x,60,80,miPaquete.jugador1.coordenadas.y)==TRUE){
	miPaquete.jugador1.coordenadas.x = 125;miPaquete.jugador1.vidas--;
	miPaquete.jugador1.coordenadas.y = 365;
	}
	if(colision(miPaquete.gaviotas[2].x,54,20,miPaquete.gaviotas[2].y, miPaquete.jugador1.coordenadas.x,60,80,miPaquete.jugador1.coordenadas.y)==TRUE){
	miPaquete.jugador1.coordenadas.x = 125;miPaquete.jugador1.vidas--;
	miPaquete.jugador1.coordenadas.y = 365;
	}
}


///funcion que sive para ver si los ladrillos colisionan con los jugadores
//no recibe parametros
void colisicionLadrillos(){
	if(colision(miPaquete.ladrillos[0].x,54,30,miPaquete.ladrillos[0].y, miPaquete.jugador2.coordenadas.x,60,90,miPaquete.jugador2.coordenadas.y)==TRUE){
	miPaquete.jugador2.coordenadas.x = 430;miPaquete.jugador2.vidas--;
	miPaquete.jugador2.coordenadas.y = 365;
	}
	if(colision(miPaquete.ladrillos[1].x,45,30,miPaquete.ladrillos[1].y, miPaquete.jugador2.coordenadas.x,60,80,miPaquete.jugador2.coordenadas.y)==TRUE){
	miPaquete.jugador2.coordenadas.x = 430;miPaquete.jugador2.vidas--;
	miPaquete.jugador2.coordenadas.y = 365;
	}
	if(colision(miPaquete.ladrillos[2].x,45,30,miPaquete.ladrillos[2].y, miPaquete.jugador2.coordenadas.x,60,80,miPaquete.jugador2.coordenadas.y)==TRUE){
	miPaquete.jugador2.coordenadas.x = 430;miPaquete.jugador2.vidas--;
	miPaquete.jugador2.coordenadas.y = 365;
	}
	if(colision(miPaquete.ladrillos[0].x,45,30,miPaquete.ladrillos[0].y, miPaquete.jugador1.coordenadas.x,60,80,miPaquete.jugador1.coordenadas.y)==TRUE){
	miPaquete.jugador1.coordenadas.x = 125;miPaquete.jugador1.vidas--;
	miPaquete.jugador1.coordenadas.y = 365;	
	}
	if(colision(miPaquete.ladrillos[1].x,45,30,miPaquete.ladrillos[1].y, miPaquete.jugador1.coordenadas.x,60,80,miPaquete.jugador1.coordenadas.y)==TRUE){
	miPaquete.jugador1.coordenadas.x = 125;miPaquete.jugador1.vidas--;
	miPaquete.jugador1.coordenadas.y = 365;
	}
	if(colision(miPaquete.ladrillos[2].x,45,30,miPaquete.ladrillos[2].y, miPaquete.jugador1.coordenadas.x,60,80,miPaquete.jugador1.coordenadas.y)==TRUE){
	miPaquete.jugador1.coordenadas.x = 125;miPaquete.jugador1.vidas--;
	miPaquete.jugador1.coordenadas.y = 365;
	}
}


void inicializar(){
	miPaquete.jugador1.coordenadas.x = 125;
	miPaquete.jugador1.coordenadas.y = 365;
	miPaquete.jugador2.coordenadas.x = 430;
	miPaquete.jugador2.coordenadas.y = 365;
	mov_lad1=random()%100;
        mov_lad2=random()%100-100;
        mov_lad3=random()%1000-500;
        miPaquete.ralph.y=0;
        miPaquete.ralph.x=vectorderalph[movimiento];
        miPaquete.ladrillos[0].x =matrizladrillos[movimiento][0];
	miPaquete.ladrillos[1].x =matrizladrillos[movimiento][1];
 	miPaquete.ladrillos[2].x =matrizladrillos[movimiento][2];
        miPaquete.ladrillos[0].y =-30;
        miPaquete.ladrillos[1].y =-30;
        miPaquete.ladrillos[2].y =-30;
        //si no pongo esto no arranca
        miPaquete.codigoPaquete = 1;
}

//Hace que los pajaros se muevan , los ladrillos y ralph
//se puede aumentar el nivel del movimiento para ver si lo hacemos mas rapido segun su nivel   
void movimientoPajarosLadrillosRalph(int nivel){
	int variacion=3;
	if(nivel==1)variacion=4;
	if(nivel==2)variacion=6;

    if(mov_lad3<480)
    {
		if(mov_lad1>100) miPaquete.ladrillos[0].y = mov_lad1+=variacion;else mov_lad1+=variacion;
		if(mov_lad2>100) miPaquete.ladrillos[1].y = mov_lad2+=variacion;else mov_lad2+=variacion;
		if(mov_lad3>100) miPaquete.ladrillos[2].y = mov_lad3+=variacion;else mov_lad3+=variacion;
    }
    else {
		mov_lad1=-random()%100;
      	mov_lad2=-random()%100;
       	mov_lad3=-random()%100;                     
       	movimiento=random()%3;
		miPaquete.ladrillos[0].x =matrizladrillos[movimiento][0];
		miPaquete.ladrillos[1].x =matrizladrillos[movimiento][1];
      	miPaquete.ladrillos[2].x =matrizladrillos[movimiento][2];
       	miPaquete.ladrillos[0].y =-20;
     	miPaquete.ladrillos[1].y =-20;
     	miPaquete.ladrillos[2].y =-20;
       	miPaquete.ralph.x=vectorderalph[movimiento];
 	}
	if(nivel>0){
		if(mov_paj1<640)mov_paj1+=variacion;else mov_paj1=-random()%100;
		if(mov_paj2<640)mov_paj2+=variacion;else mov_paj2=-random()%100-100;
		if(mov_paj3<640)mov_paj3+=variacion;else mov_paj3=-random()%100-50;

		miPaquete.gaviotas[0].x = mov_paj1;
		miPaquete.gaviotas[0].y = 316;
		miPaquete.gaviotas[1].x = mov_paj2;
		miPaquete.gaviotas[1].y = 200;
		miPaquete.gaviotas[2].x = mov_paj3;
		miPaquete.gaviotas[2].y = 100;
	}
}

