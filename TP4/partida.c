/*#####################################
#Trabajo Practico N 4
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#Garcia Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/

#include "partida.h"


int partida, memId_vectorCliente, memId_vectorPartidas, pidServer;
int semId_vectorCliente, semId_colaMensajesDeCliente, semId_colaMensajesACliente, semId_vectorPartidas;

struct s_datosCliente *v_datosCliente;
struct s_datosPartida *v_datosPartida;

//Cola de mensajes que se envian desde los clientes
struct tcola *c_mensajesDeCliente;
//Cola de mensajes que se envian a los clientes
struct tcola *c_mensajesACliente;

pthread_t t_escuchaCliente1;
pthread_t t_escuchaCliente2;
pthread_t t_procesamientoMensajes;
pthread_t t_enviaClientes;
pthread_t t_verificaEstadoServer;

t_paquete miPaquete;


int main(int argc, char *argv[]) {

	/*VARIABLES*/
	mov_paj1=random();
	mov_paj2=random();
	mov_paj3=random();
	//mov ladrillos
	mov_lad1=random();
	mov_lad2=random();
	mov_lad3=random();

	movimiento=random()%3;
	/*FIN VARIABLES*/

	/*INICIALIZACION de VARIABLES con los datos dados por el torneo*/
	memId_vectorCliente = atoi(argv[1]);
	semId_vectorCliente = atoi(argv[2]);
	semId_vectorPartidas = atoi(argv[3]);
	memId_vectorPartidas = atoi(argv[4]);

	partida = atoi(argv[7]);
	//parametrosAEnviar = generaParametrosPartida(memId_vectorCliente, semId_vectorCliente,semId_vectorPartidas,memId_vectorPartidas, a, b,partidas-1,getpid());
	//datosPartida.flag_partidaViva = 1;

	//Vector en memoria compartida que aloja los clientes conectados
	v_datosCliente = shmat(memId_vectorCliente,0,0);
	v_datosPartida = shmat(memId_vectorPartidas,0,0);

	sem_P(semId_vectorPartidas);
	printf("PARTIDA: crea partida %d\n", getpid());
	v_datosPartida[partida].idCliente1 = atoi(argv[5]);
	v_datosPartida[partida].idCliente2 = atoi(argv[6]);
	v_datosPartida[partida].pidPartida=getpid();
	v_datosPartida[partida].flag_partidaViva = 1;

	printf("PARTIDA: cliente 1 tiene id %d || cliente 2 tiene id %d \n", atoi(argv[5]), atoi(argv[6]));
	pidServer=atoi(argv[8]);

	sem_P(semId_vectorCliente);
	v_datosPartida[partida].socketCliente1 = v_datosCliente[atoi(argv[5])].socket;
	v_datosPartida[partida].socketCliente2 = v_datosCliente[atoi(argv[6])].socket;
	sem_V(semId_vectorCliente);
	fflush(NULL);
	printf("PARTIDA: Socket: %d cliente %d\n", v_datosPartida[partida].socketCliente1,1);
	printf("PARTIDA: Socket: %d cliente %d\n", v_datosPartida[partida].socketCliente2,2);
	sem_V(semId_vectorPartidas);

	fflush(NULL);
	printf("PARTIDA: Arranca una partida\n");
	//Inicializo la cola de mensajes enviados por el cliente y su semaforo
	semId_colaMensajesDeCliente = crear_sem(IPC_PRIVATE, 1);
	if(semId_colaMensajesDeCliente == -1) {
		imprimirError(0, "Error al crear el semaforo");
	}
	crear(&c_mensajesDeCliente);

	//Inicializo la cola de mensajes para enviar a los clientes y su semaforo
	semId_colaMensajesACliente = crear_sem(IPC_PRIVATE, 1);
	if(semId_colaMensajesACliente == -1) {
		imprimirError(0, "Error al crear el semaforo");
	}
	crear(&c_mensajesACliente);
	/*FIN INICIALIZACION de VARIABLES*/

	signal(SIGINT, sigint_handler);

	/** Se crea thread de escucha de clientes **/
	sem_P(semId_vectorPartidas);
	pthread_create(&t_escuchaCliente1, NULL, leeCliente, &v_datosPartida[partida].idCliente1);
	pthread_create(&t_escuchaCliente2, NULL, leeCliente, &v_datosPartida[partida].idCliente2);
	sem_V(semId_vectorPartidas);

	/** Inicializacion del paquete de la pantalla **/
	// Pongo todo en su lugar antes de arrancar.
	inicializar();
	generarMarquesinasRandom();
	inicializar_marquesinas();
	setearPosicionMarquesinasParaComparar();

	miPaquete.jugadores[numeroJugador1-1].vidas=3;
	miPaquete.jugadores[numeroJugador2-1].vidas=3;
	strcpy(miPaquete.jugadores[numeroJugador1-1].nombre, v_datosCliente[atoi(argv[5])].nombre);
	strcpy(miPaquete.jugadores[numeroJugador2-1].nombre, v_datosCliente[atoi(argv[6])].nombre);
	miPaquete.codigoPaquete = 1;

	/** Se crea los threads de procesamiento y envio **/
	pthread_create(&t_procesamientoMensajes, NULL, procesamientoMensajes, NULL);
	pthread_create(&t_enviaClientes, NULL, enviaCliente, NULL);
	pthread_create(&t_verificaEstadoServer, NULL, verificaEstadoServer, NULL);

	/** Se espera a que todos los threads terminen de ejecutar. **/
	pthread_join(t_escuchaCliente1, NULL);
	pthread_join(t_escuchaCliente2, NULL);
	pthread_join(t_procesamientoMensajes, NULL);
	pthread_join(t_enviaClientes, NULL);
	pthread_join(t_verificaEstadoServer, NULL);

	printf("PARTIDA: Se acabo la partida \n");

	while(1) {
		sleep(1);
	}
}


/**
*FUNCION DE MANEJO DE SENIAL
*/
void sigint_handler(int sig) {

	//cerramos el semaforo de la cola de mensajes del cliente
	if(cerrar_sem(semId_colaMensajesDeCliente) == -1) {
		imprimirError(0, "Error al cerrar los semaforos");
	}
	//cerramos el semaforo de la cola de mensajes para el cliente
	if(cerrar_sem(semId_colaMensajesACliente) == -1) {
		imprimirError(0, "Error al cerrar los semaforos");
	}

	//Se libera el uso de la mem compartida por este proceso
	shmdt((char *) v_datosCliente);
	shmdt((char *) v_datosPartida);

	//Se elimina las colas creadas
	vaciar(c_mensajesDeCliente);
	vaciar(c_mensajesACliente);

	_exit(EXIT_SUCCESS);
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
	//exit(EXIT_FAILURE); //TODO: esta funcion debe tener el control de terminacion de ejec??
}


/**
*FUNCION QUE LEE LOS MENSAJES ENVIADOS DEL CLIENTE
*/
void *leeCliente(void* argumentos) {

	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	int idCliente = *((int*) argumentos);
	int socketCliente = v_datosCliente[idCliente].socket;

	int bufferInt;
	struct msjDeCliente msj;
	int flagCliente = 1;
	int datosLeidos;

	//Lee datos del socket del cliente, leera el tamanio del buffer o la cantidad indicada en el parametro 3 lo que sea que sea menor.
	//read(socket, buffer donde carga el mensaje, cantidad)
	while (v_datosPartida[partida].flag_partidaViva && flagCliente) {
		datosLeidos = read(socketCliente, &bufferInt, sizeof(bufferInt));
		if (datosLeidos < 0) {
			// TODO : ver si se debe cerrar el socket desde la partida
			close(socketCliente);
			// FIN TODO
			v_datosCliente[idCliente].activo=0;
			flagCliente=0;
			imprimirError(0, "ERROR no se puede leer del cliente.");
		} else if (datosLeidos == 0) {
			close(socketCliente);
			v_datosCliente[idCliente].activo=0;
			flagCliente=0;
			imprimirError(0, "Un cliente se desconecto abruptamente.");
		} else {
			if (idCliente == v_datosPartida[partida].idCliente1) {
				msj.nroCliente = numeroJugador1;
			} else {
				msj.nroCliente = numeroJugador2;
			}
			msj.codigo = bufferInt;
			sem_P(semId_colaMensajesDeCliente);
			encolar(&c_mensajesDeCliente, (void*) &msj);
			sem_V(semId_colaMensajesDeCliente);
		}
		bufferInt = 0;
	}
	printf("PARTIDA: fin leeCliente\n");
	pthread_exit(NULL);
}


/**
*FUNCION QUE PROCESA LOS MENSAJES ENVIADOS DESDE LOS CLIENTE
*/
void *procesamientoMensajes() {
	void* nodo;
	struct msjDeCliente elementoDeCola;
	while (v_datosPartida[partida].flag_partidaViva) {
		if (!vacia(c_mensajesDeCliente)) {
			sem_P(semId_colaMensajesDeCliente);
			desencolar(&c_mensajesDeCliente, &nodo);
			elementoDeCola = *((struct msjDeCliente*)nodo);
			sem_V(semId_colaMensajesDeCliente);

			if (elementoDeCola.codigo == 1000) {
				//TODO: El cliente cerro el juego
			} else if (elementoDeCola.codigo == 600) {
				// El cliente informa que termino de dibujar el techo y se termina la partida
				miPaquete.codigoPaquete = 5;
			} else if (elementoDeCola.codigo == 500) {
				printf("PARTIDA: Nivel %d \n", miPaquete.nivel);
				VentanasArregladas = 0;
				inicializar();
				miPaquete.codigoPaquete = 1;
			} else {
				moverJugador(elementoDeCola.codigo, elementoDeCola.nroCliente-1);
			}
		}
		switch(miPaquete.codigoPaquete) {
			case 0: break;
			case 1: //partida inicial Nivel 1
				dibujarVidrios(tipoEdificio);
				//Hace que los pajaros se muevan , los ladrillos y ralph
				movimientoPajarosLadrillosRalph(miPaquete.nivel);
				//verifico colision
				colisicionPajaros();
				colisicionLadrillos();

				if(tortaComida == 0) {
					colisionTorta();
				}
				if(ventanasArregladasParaTorta != VentanasArregladas &&
							VentanasArregladas%cantidadVidriosParaTorta == 0 && (tortaComida == 1 || primeraTorta == 1)){
					miPaquete.torta.dibujar = 1;
					setearCoordenadasTorta();
					primeraTorta = 0;
					ventanasArregladasParaTorta = VentanasArregladas;
				}

				//verifico si la cantidad de ventanas a arreglar fue superada
				if( VentanasArregladas >= ventanasAReparar && ventanasAReparar > 0){
					printf("PARTIDA: Voy a pasar de nivel \n");
					miPaquete.nivel++;
					miPaquete.codigoPaquete = 2;
					//imagino q esto sirve para algo pero no se para que
					partidaPrimeraVez = 1;
					tipoEdificio = 1;
					ventanasAReparar = 0;
					//Marquesina 1 en nivel uno. 2 Marquesinas en el nivel 3.
					marquesinas++;
				}
				break;
			case 2:
				printf("PARTIDA: cambiando al NIVEL \n");  // cambiando al NIVEL 2
				break;
			case 4:
				printf("PARTIDA: codigo paquete es 4\n");
				miPaquete.codigoPaquete = 0;
				break;
			case 5:
				pthread_cancel(t_escuchaCliente1);
				pthread_cancel(t_escuchaCliente2);

				sem_P(semId_vectorCliente);
				v_datosCliente[v_datosPartida[partida].idCliente1].puntos += miPaquete.jugadores[0].puntos;
				v_datosCliente[v_datosPartida[partida].idCliente2].puntos += miPaquete.jugadores[1].puntos;

				v_datosCliente[v_datosPartida[partida].idCliente1].jugando=0;
				v_datosCliente[v_datosPartida[partida].idCliente2].jugando=0;
				sem_V(semId_vectorCliente);

				sem_P(semId_vectorPartidas);
				v_datosPartida[partida].flag_partidaViva=0;
				sem_V(semId_vectorPartidas);
				printf("PARTIDA: flag partida %d\n", v_datosPartida[partida].flag_partidaViva);
				break;
		}
		if(miPaquete.nivel==3 && miPaquete.codigoPaquete == 2) {
			printf("PARTIDA: paso al nivel 3 y termino la partida\n");
			miPaquete.codigoPaquete = 4;
		}
		if(miPaquete.codigoPaquete != 0 && miPaquete.codigoPaquete != 5 && miPaquete.jugadores[0].vidas<=0 && miPaquete.jugadores[1].vidas<=0 ) {
			printf("PARTIDA: fin de juego por que los dos murieron\n");
			miPaquete.codigoPaquete = 4;
		}

		if (miPaquete.codigoPaquete != 0 && miPaquete.codigoPaquete != 5) {
			sem_P(semId_colaMensajesACliente);
			t_paquete paqAux = miPaquete;
			encolar(&c_mensajesACliente, (void*) &paqAux);
			sem_V(semId_colaMensajesACliente);
		}
		usleep(75000);
	}
	printf("PARTIDA: fin procesamientoMensajes\n");
	pthread_exit(NULL);
}

/**
*FUNCION QUE ENVIA LOS MENSAJES AL CLIENTE
*/
void *enviaCliente(void* argumentos) {

	int flagCliente1 = 1, flagCliente2 = 1;
	void* nodo;
	t_paquete elementoDeCola;
	while (v_datosPartida[partida].flag_partidaViva && (flagCliente1 || flagCliente2)) {
		if (!vacia(c_mensajesACliente)) {
			sem_P(semId_colaMensajesACliente);
			desencolar(&c_mensajesACliente, &nodo);
			elementoDeCola = *((t_paquete*)nodo);
			sem_V(semId_colaMensajesACliente);

			/*if(elementoDeCola.jugadores[numeroJugador1-1].vidas <= 0) {
				flagCliente1 = 0;
			}
			if(elementoDeCola.jugadores[numeroJugador2-1].vidas <= 0) {
				flagCliente2 = 0;
			}*/
			if(elementoDeCola.codigoPaquete != 1)
				printf("Estoy mandando codigo de paquete : %d\n", elementoDeCola.codigoPaquete);

			//Envia mensajes a ambos clientes
			if (flagCliente1) {
				elementoDeCola.nroJugador = numeroJugador1;
				if ( write(v_datosPartida[partida].socketCliente1, &elementoDeCola, sizeof(elementoDeCola)) <0 ) {
					close(v_datosPartida[partida].socketCliente1);
					flagCliente1 = 0;
					imprimirError(0, "ERROR escribiendo en el socket");					
				}
			}
			if (flagCliente2) {
				elementoDeCola.nroJugador = numeroJugador2;
				if ( write(v_datosPartida[partida].socketCliente2, &elementoDeCola, sizeof(elementoDeCola)) <0 ) {
					close(v_datosPartida[partida].socketCliente2);
					flagCliente2 = 0;
					imprimirError(0, "ERROR escribiendo en el socket");					
				}
			}
		}
		usleep(75000);
	}
	printf("PARTIDA: fin envioClientes\n");
	pthread_exit(NULL);
}


void *verificaEstadoServer() {
	while(v_datosPartida[partida].flag_partidaViva){ //TODO: mergear con la viarable de control del negro
		kill(pidServer,0);
		if(errno==ESRCH){
			//TODO: Enviar a los clientes que se termino todo. Ver como hacer en el paquete
			//sem_P(semId_vectorCliente);
			pthread_kill(t_escuchaCliente1, SIGKILL);
			pthread_kill(t_escuchaCliente2, SIGKILL);
			pthread_kill(t_procesamientoMensajes, SIGKILL);
			pthread_kill(t_enviaClientes, SIGKILL);
			//pthread_kill(t_verificaEstadoServer, 0);
			close(v_datosPartida[partida].socketCliente1);
			close(v_datosPartida[partida].socketCliente2);
			//TODO: Ver como liberar de ser necesario la memoria compartida
			shmdt((char *)v_datosCliente);
			shmdt((char *)v_datosPartida);
			//sem_V(semId_vectorCliente);
			return;
		}
		usleep(75000);
	}
	printf("PARTIDA: fin verificaEstadoServer\n");
	pthread_exit(NULL);
}


int chequearPosicion(t_jugador jugador, int direccion, int nivel){
	//direccion 1 arriba
	//direccion 0 abajo
	//Obtengo x,y de la marquesina
	if(nivel == 1){
		//Dibujo la maquesina del nivel 1
		if(direccion == 1){
			if(((jugador.coordenadas.y - 120) < mUno.y && jugador.coordenadas.y > mUno.y) && ((jugador.coordenadas.x + 58 >= mUno.x && jugador.coordenadas.x + 58 < mUno.x + 60) || (jugador.coordenadas.x >= mUno.x && jugador.coordenadas.x < mUno.x + 60)))
				res =  0;
			else
				res =  1;
		}else if(direccion == 0){
			if(((jugador.coordenadas.y + 120) > mUno.y && jugador.coordenadas.y < mUno.y) && ((jugador.coordenadas.x + 58 >= mUno.x && jugador.coordenadas.x + 58 < mUno.x + 60) || (jugador.coordenadas.x >= mUno.x && jugador.coordenadas.x < mUno.x + 60)))
				res =  0;
			else
				res =  1;
		}
		return res;
	}else if(nivel == 2){
		//Calculo lo anterior verificando las dos marquesinas restantes
		if(direccion == 1){
			if(((jugador.coordenadas.y - 120) < mDos.y && jugador.coordenadas.y > mDos.y) && ((jugador.coordenadas.x + 58 >= mDos.x && jugador.coordenadas.x + 58 < mDos.x + 60) || (jugador.coordenadas.x >= mDos.x && jugador.coordenadas.x < mDos.x + 60))) //||
				res = 0;
			else if(((jugador.coordenadas.y - 120) < mTres.y && jugador.coordenadas.y > mTres.y) && ((jugador.coordenadas.x + 58 >= mTres.x && jugador.coordenadas.x + 58 < mTres.x + 60) || (jugador.coordenadas.x >= mTres.x && jugador.coordenadas.x < mTres.x + 60)))
				res = 0;
			else
				res = 1;
		}else if(direccion == 0){
			if(((jugador.coordenadas.y + 120) > mDos.y && jugador.coordenadas.y < mDos.y) && ((jugador.coordenadas.x + 58 >= mDos.x && jugador.coordenadas.x + 58 < mDos.x + 55) || (jugador.coordenadas.x >= mDos.x && jugador.coordenadas.x < mDos.x + 60))) //||
				res = 0;
			else if(((jugador.coordenadas.y + 120) > mTres.y && jugador.coordenadas.y < mTres.y) && ((jugador.coordenadas.x + 58 >= mTres.x && jugador.coordenadas.x + 58 < mTres.x + 60) || (jugador.coordenadas.x >= mTres.x && jugador.coordenadas.x < mTres.x + 60)))
				res = 0;
			else
				res = 1;
		}
		return res;
	}
}

void moverJugador(int codigo, int numJugador) {
	switch(codigo) {
		case 10://arriba
			if(miPaquete.nivel == 0) {
				if((miPaquete.jugadores[numJugador].coordenadas.y - 120) >= 80) {
					miPaquete.jugadores[numJugador].coordenadas.y -= 120;
				}
			} else {
				if((miPaquete.jugadores[numJugador].coordenadas.y - 120) >= 80 && chequearPosicion(miPaquete.jugadores[numJugador], 1, miPaquete.nivel)) {
					miPaquete.jugadores[numJugador].coordenadas.y -= 120;
				}
			}
			break;
		case 20://abajo
			if(miPaquete.nivel == 0){
				if((miPaquete.jugadores[numJugador].coordenadas.y + 120) <= 450) {
					miPaquete.jugadores[numJugador].coordenadas.y += 120;
				}
			} else {
				if((miPaquete.jugadores[numJugador].coordenadas.y + 120) <= 450 && chequearPosicion(miPaquete.jugadores[numJugador], 0, miPaquete.nivel)) {
					miPaquete.jugadores[numJugador].coordenadas.y += 120;
				}
			}
			break;
		case 30://izq
			if((miPaquete.jugadores[numJugador].coordenadas.x - 85) >= 90 ) {
				miPaquete.jugadores[numJugador].coordenadas.x -= 75;
			}
			break;
		case 40: //der
			if((miPaquete.jugadores[numJugador].coordenadas.x + 85) <= 500 ) {
				miPaquete.jugadores[numJugador].coordenadas.x += 75;
			}
			break;
		case 1: // Arreglo ventana
			arregloVentana(numJugador);
			break;
		default:
			break;
	}
}

int colision(int x1,int w1,int h1,int y1 ,int x2,int w2,int h2,int y2){
	if (((x1+w1)>x2)&&((y1+h1)>y2)&&((x2+w2)>x1)&&((y2+h2)>y1)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

int arregloVentana(int jugador){
	/*Verifico si Felix esta parado en alguna ventana y hay ventanas por reparar.
	En caso de verdadero, las reparo, sumo puntos y cambio el sprite*/
	/*Las ventanas siempre van de a pares, cuando encuentro que colisione con una
	ventana, me fijo de arreglar solo una ventana por vez.*/
	int i, numeroVentana = 0;
	/*Recorro todas las ventanas*/
	for(i = 0; i < 40; i+=2){
		if(jugador == 0){
			if(colision(miPaquete.vidrios[i].coordenadas.x, 28, 25, miPaquete.vidrios[i].coordenadas.y, miPaquete.jugadores[0].coordenadas.x, 68, 90, miPaquete.jugadores[0].coordenadas.y)==TRUE){
				if(ventanasParesRotas[numeroVentana][0] == 1){
					miPaquete.jugadores[0].puntos++;
					VentanasArregladas++;
					miPaquete.vidrios[i].roto = 0; //Lo arreglo
					ventanasParesRotas[numeroVentana][0] = 0;
				}else if(ventanasParesRotas[numeroVentana][1] == 1){
					miPaquete.jugadores[0].puntos++;
					VentanasArregladas++;
					miPaquete.vidrios[i+1].roto = 0;
					ventanasParesRotas[numeroVentana][1] = 0;
				}
				/*TODO Recibo premio*/
				/*Sumo Puntos*/
			}
		}
		else{
			if(colision(miPaquete.vidrios[i].coordenadas.x,28,25,miPaquete.vidrios[i].coordenadas.y, miPaquete.jugadores[1].coordenadas.x,68,90,miPaquete.jugadores[1].coordenadas.y)==TRUE){

				if(ventanasParesRotas[numeroVentana][0] == 1){VentanasArregladas++;
				miPaquete.jugadores[1].puntos++;
				ventanasParesRotas[numeroVentana][0] = 0;
				miPaquete.vidrios[i].roto = 0; //Lo arreglo
				}else if(ventanasParesRotas[numeroVentana][1] == 1){miPaquete.jugadores[1].puntos++;VentanasArregladas++;
				ventanasParesRotas[numeroVentana][1] = 0;
				miPaquete.vidrios[i+1].roto = 0; //Lo arreglo
				}
				/*TODO Recibo premio*/
				/*Sumo puntos*/
			}
		}
		numeroVentana++;
	}
}

void inicializar() {
	miPaquete.jugadores[0].coordenadas.x = 125;
	miPaquete.jugadores[0].coordenadas.y = 365;
	miPaquete.jugadores[1].coordenadas.x = 430;
	miPaquete.jugadores[1].coordenadas.y = 365;
	mov_lad1=random()%100;
	mov_lad2=random()%100-100;
	mov_lad3=random()%1000-500;
	miPaquete.ralph.y=50;
	miPaquete.ralph.x=vectorderalph[movimiento];
	miPaquete.ladrillos[0].x =matrizladrillos[movimiento][0];
	miPaquete.ladrillos[1].x =matrizladrillos[movimiento][1];
	miPaquete.ladrillos[2].x =matrizladrillos[movimiento][2];
	miPaquete.ladrillos[0].y =-30;
	miPaquete.ladrillos[1].y =-30;
	miPaquete.ladrillos[2].y =-30;
}

void generarMarquesinasRandom() {
	srand(getpid());
	int i, r[3] = {-1,-1,-1};
	int random;
	for(i = 0; i < 3 ; i++) {
		do {
			random = (int) (rand() % 10);
		} while(random == r[0] || random == r[1] || random == r[2]);
		miPaquete.marquesina[i] = random;
		r[i] = random;
	}
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

void setearPosicionMarquesinasParaComparar(){
		if(miPaquete.marquesina[0] < 5){
			hilera = 0;
			ventana = miPaquete.marquesina[0];
		}else if(miPaquete.marquesina[0] >= 5 && miPaquete.marquesina[0] < 10){
			hilera = 1;
			ventana = miPaquete.marquesina[0] - 5;
		}
		mUno.x = posicionesMarquesinas[hilera][ventana].x;
		mUno.y = posicionesMarquesinas[hilera][ventana].y;
		if(miPaquete.marquesina[1] < 5){
			hilera = 0;
			ventana = miPaquete.marquesina[1];
		}else if(miPaquete.marquesina[1] >= 5 && miPaquete.marquesina[1] < 10){
			hilera = 1;
			ventana = miPaquete.marquesina[1] - 5;
		}
		mDos.x = posicionesMarquesinas[hilera][ventana].x;
		mDos.y = posicionesMarquesinas[hilera][ventana].y;
		if(miPaquete.marquesina[2] < 5){
			hilera = 0;
			ventana = miPaquete.marquesina[2];
		}else if(miPaquete.marquesina[2] >= 5 && miPaquete.marquesina[2] < 10){
			hilera = 1;
			ventana = miPaquete.marquesina[2] - 5;
		}
		mTres.x = posicionesMarquesinas[hilera][ventana].x;
		mTres.y = posicionesMarquesinas[hilera][ventana].y;
}

void dibujarVidrios(int completo) {
	int i,x,y,comienzoX,comienzoY,distanciaEntreVidrios,totalVidrios,numeroVentana;
	comienzoX = 140;
	comienzoY = 31;
	x = comienzoX;
	y = comienzoY;
	distanciaEntreVidrios = 78;
	totalVidrios = 36;
	if(completo == 1){
		totalVidrios = 40;
	}
	numeroVentana = 0;
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
		if(partidaPrimeraVez && i >= 10){
			int random;
			random = rand() %2;
			if(random){
				ventanasAReparar++;
			}
			ventanasParesRotas[numeroVentana][0] = random;
			miPaquete.vidrios[i].roto = random;
			random = rand() %2;
			if(random){
				ventanasAReparar++;
			}
			ventanasParesRotas[numeroVentana][1] = random;
			miPaquete.vidrios[i+1].roto = random;
		}
		//dibujarSprite(vidrios[i], x, y,screen);
		//dibujarSprite(vidrios[i], x, y+30,screen);
		x+=distanciaEntreVidrios;
		numeroVentana++;
	}
	partidaPrimeraVez = 0;
}

//Hace que los pajaros se muevan , los ladrillos y ralph
//se puede aumentar el nivel del movimiento para ver si lo hacemos mas rapido segun su nivel
void movimientoPajarosLadrillosRalph(int nivel){
	int variacion=3;
	if(nivel==1)variacion=6;
	if(nivel==2)variacion=9;
	if(mov_lad3<480) {
		if(mov_lad1>100) miPaquete.ladrillos[0].y = mov_lad1+=variacion;else mov_lad1+=variacion;
		if(mov_lad2>100) miPaquete.ladrillos[1].y = mov_lad2+=variacion;else mov_lad2+=variacion;
		if(mov_lad3>100) miPaquete.ladrillos[2].y = mov_lad3+=variacion;else mov_lad3+=variacion;
	} else {
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
	if(nivel>0) {
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

///funcion que sive para ver si los pajaros colisionan con los jugadores
void colisicionPajaros() {
	if(colision(miPaquete.gaviotas[0].x,54,20,miPaquete.gaviotas[0].y, miPaquete.jugadores[1].coordenadas.x,60,80,miPaquete.jugadores[1].coordenadas.y)==TRUE)
	{
		miPaquete.jugadores[1].coordenadas.x = 430;
		if(miPaquete.jugadores[1].vidas > 0)
			miPaquete.jugadores[1].vidas--;
		miPaquete.jugadores[1].coordenadas.y = 365;
	}
	if(colision(miPaquete.gaviotas[1].x,54,20,miPaquete.gaviotas[1].y, miPaquete.jugadores[1].coordenadas.x,60,80,miPaquete.jugadores[1].coordenadas.y)==TRUE)
	{
		miPaquete.jugadores[1].coordenadas.x = 430;
		if(miPaquete.jugadores[1].vidas > 0)
			miPaquete.jugadores[1].vidas--;
		miPaquete.jugadores[1].coordenadas.y = 365;
	}
	if(colision(miPaquete.gaviotas[2].x,54,20,miPaquete.gaviotas[2].y, miPaquete.jugadores[1].coordenadas.x,60,80,miPaquete.jugadores[1].coordenadas.y)==TRUE)
	{
		miPaquete.jugadores[1].coordenadas.x = 430;
		if(miPaquete.jugadores[1].vidas > 0)
			miPaquete.jugadores[1].vidas--;
		miPaquete.jugadores[1].coordenadas.y = 365;
	}
	if(colision(miPaquete.gaviotas[0].x,54,20,miPaquete.gaviotas[0].y, miPaquete.jugadores[0].coordenadas.x,60,80,miPaquete.jugadores[0].coordenadas.y)==TRUE)
	{
		miPaquete.jugadores[0].coordenadas.x = 125;
		if(miPaquete.jugadores[0].vidas > 0)
			miPaquete.jugadores[0].vidas--;
		miPaquete.jugadores[0].coordenadas.y = 365;
	}
	if(colision(miPaquete.gaviotas[1].x,54,20,miPaquete.gaviotas[1].y, miPaquete.jugadores[0].coordenadas.x,60,80,miPaquete.jugadores[0].coordenadas.y)==TRUE)
	{
		miPaquete.jugadores[0].coordenadas.x = 125;
		if(miPaquete.jugadores[0].vidas > 0)
			miPaquete.jugadores[0].vidas--;
		miPaquete.jugadores[0].coordenadas.y = 365;
	}
	if(colision(miPaquete.gaviotas[2].x,54,20,miPaquete.gaviotas[2].y, miPaquete.jugadores[0].coordenadas.x,60,80,miPaquete.jugadores[0].coordenadas.y)==TRUE)
	{
		miPaquete.jugadores[0].coordenadas.x = 125;
		if(miPaquete.jugadores[0].vidas > 0)
			miPaquete.jugadores[0].vidas--;
		miPaquete.jugadores[0].coordenadas.y = 365;
	}
}

///funcion que sive para ver si los ladrillos colisionan con los jugadores
void colisicionLadrillos() {
	if(colision(miPaquete.ladrillos[0].x,54,30,miPaquete.ladrillos[0].y, miPaquete.jugadores[1].coordenadas.x,60,90,miPaquete.jugadores[1].coordenadas.y)==TRUE)
	{
		miPaquete.jugadores[1].coordenadas.x = 430;
		if(miPaquete.jugadores[1].vidas > 0)
			miPaquete.jugadores[1].vidas--;
		miPaquete.jugadores[1].coordenadas.y = 365;
	}
	if(colision(miPaquete.ladrillos[1].x,45,30,miPaquete.ladrillos[1].y, miPaquete.jugadores[1].coordenadas.x,60,80,miPaquete.jugadores[1].coordenadas.y)==TRUE)
	{
		miPaquete.jugadores[1].coordenadas.x = 430;
		if(miPaquete.jugadores[1].vidas > 0)
			miPaquete.jugadores[1].vidas--;
		miPaquete.jugadores[1].coordenadas.y = 365;
	}
	if(colision(miPaquete.ladrillos[2].x,45,30,miPaquete.ladrillos[2].y, miPaquete.jugadores[1].coordenadas.x,60,80,miPaquete.jugadores[1].coordenadas.y)==TRUE)
	{
		miPaquete.jugadores[1].coordenadas.x = 430;
		if(miPaquete.jugadores[1].vidas > 0)
			miPaquete.jugadores[1].vidas--;
		miPaquete.jugadores[1].coordenadas.y = 365;
	}
	if(colision(miPaquete.ladrillos[0].x,45,30,miPaquete.ladrillos[0].y, miPaquete.jugadores[0].coordenadas.x,60,80,miPaquete.jugadores[0].coordenadas.y)==TRUE)
	{
		miPaquete.jugadores[0].coordenadas.x = 125;
		if(miPaquete.jugadores[0].vidas > 0)
			miPaquete.jugadores[0].vidas--;
		miPaquete.jugadores[0].coordenadas.y = 365;
	}
	if(colision(miPaquete.ladrillos[1].x,45,30,miPaquete.ladrillos[1].y, miPaquete.jugadores[0].coordenadas.x,60,80,miPaquete.jugadores[0].coordenadas.y)==TRUE)
	{
		miPaquete.jugadores[0].coordenadas.x = 125;
		if(miPaquete.jugadores[0].vidas > 0)
			miPaquete.jugadores[0].vidas--;
		miPaquete.jugadores[0].coordenadas.y = 365;
	}
	if(colision(miPaquete.ladrillos[2].x,45,30,miPaquete.ladrillos[2].y, miPaquete.jugadores[0].coordenadas.x,60,80,miPaquete.jugadores[0].coordenadas.y)==TRUE)
	{
		miPaquete.jugadores[0].coordenadas.x = 125;
		if(miPaquete.jugadores[0].vidas > 0)
			miPaquete.jugadores[0].vidas--;
		miPaquete.jugadores[0].coordenadas.y = 365;
	}
	//hago el envio a los cliente con la nueva informacion esto se hace todo el tiempo
}

void setearCoordenadasTorta() {
    t_coordenadas coordenadasTorta[13];
    int i,posicionX,posicionY,mostrarTorta;
    int comienzoX = 141;
    int comienzoY = 173;
    int distanciaEntreTortas = 78;

    posicionX = comienzoX;
    posicionY = comienzoY;

    for(i=0;i<12;i++){

        switch(i){
            case 4:  posicionY+= 110; posicionX=comienzoX; break;
            case 8:  posicionY+= 135; posicionX=comienzoX; break;
            case 6:  posicionX+= distanciaEntreTortas;break;
            case 10: posicionX+= distanciaEntreTortas; break;
        }

        coordenadasTorta[i].x = posicionX;
        coordenadasTorta[i].y = posicionY;

        posicionX+= distanciaEntreTortas;
    }

    mostrarTorta = random()%11;

    miPaquete.torta.coordenadas.x = coordenadasTorta[mostrarTorta].x;
    miPaquete.torta.coordenadas.y = coordenadasTorta[mostrarTorta].y;
    tortaComida = 0;
}

void colisionTorta() {

    int anchoTorta = 26;
    int altoTorta = 31;
    int anchoJugador = 60;
    int altoJugador = 90;

    if(colision(miPaquete.torta.coordenadas.x,anchoTorta,altoTorta,miPaquete.torta.coordenadas.y, miPaquete.jugadores[0].coordenadas.x,anchoJugador,altoJugador,miPaquete.jugadores[0].coordenadas.y) == TRUE){
        miPaquete.jugadores[0].vidas++;
        miPaquete.torta.coordenadas.x = 0;
        miPaquete.torta.coordenadas.y = 0;
        tortaComida = 1;
    }

    if(colision(miPaquete.torta.coordenadas.x,anchoTorta,altoTorta,miPaquete.torta.coordenadas.y, miPaquete.jugadores[1].coordenadas.x,anchoJugador,altoJugador,miPaquete.jugadores[1].coordenadas.y) == TRUE){
        miPaquete.jugadores[1].vidas++;
        miPaquete.torta.coordenadas.x = 0;
        miPaquete.torta.coordenadas.y = 0;
        tortaComida = 1;
    }
}
