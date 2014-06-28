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


int memId_vectorCliente;
int semId_vectorCliente, semId_colaMensajesDeCliente, semId_colaMensajesACliente;
struct s_datosCliente *v_datosCliente;

struct s_datosPartida datosPartida;

//Cola de mensajes que se envian desde los clientes
struct tcola *c_mensajesDeCliente;
//Cola de mensajes que se envian a los clientes
struct tcola *c_mensajesACliente;



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

	datosPartida.idCliente1 = atoi(argv[3]);
	datosPartida.idCliente2 = atoi(argv[4]);
	datosPartida.flag_partidaViva = 1;

	//Vector en memoria compartida que aloja los clientes conectados
	v_datosCliente = shmat(memId_vectorCliente,0,0); 
	datosPartida.socketCliente1 = v_datosCliente[datosPartida.idCliente1].socket;
	datosPartida.socketCliente2 = v_datosCliente[datosPartida.idCliente2].socket;

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

	// Se crea thread de escucha de nuevos clientes para el torneo
	sem_P(semId_vectorCliente);
	pthread_create(&t_escuchaCliente1, NULL, leeCliente, &datosPartida.socketCliente1);
	pthread_create(&t_escuchaCliente2, NULL, leeCliente, &datosPartida.socketCliente2);
	pthread_create(&t_enviaClientes, NULL, enviaCliente, NULL);
	sem_V(semId_vectorCliente);

	pthread_join(t_escuchaCliente1, NULL);
	pthread_join(t_escuchaCliente2, NULL);
	pthread_join(t_enviaClientes, NULL);

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

	int socketCliente = *((int*) argumentos);
	char buffer[BUFFERSIZE]; //contendra los datos leidos o enviados, el tamaño esconfigurado con la variable BUFFERSIZE 
	bzero(buffer,BUFFERSIZE);
		
	//Lee datos del socket del cliente, leerá el tamaño del buffer o la cantidad indicada en el parametro 3 lo que sea que sea menor.
	//read(socket, buffer donde carga el mensaje, cantidad)
	int flagCliente = 1;
	while (datosPartida.flag_partidaViva && flagCliente) {
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
	while (datosPartida.flag_partidaViva && (flagCliente1 || flagCliente2)) {
		if (!vacia(c_mensajesACliente)) {
			sem_P(semId_colaMensajesACliente);
			desencolar(&c_mensajesACliente, &nodo);
			elementoDeCola = *((int*)nodo);
			sem_V(semId_colaMensajesACliente);

			//Envia mensajes a ambos clientes
			if (flagCliente1 && (write(datosPartida.socketCliente1, &elementoDeCola, sizeof(elementoDeCola))) < 0) {
				// TODO : ver si se debe cerrar el socket desde la partida
				close(datosPartida.socketCliente1);
				// FIN TODO
				flagCliente1 = 0;
				imprimirError(0, "ERROR escribiendo en el socket");
			}
			if (flagCliente2 && (write(datosPartida.socketCliente2, &elementoDeCola, sizeof(elementoDeCola))) < 0) {
				// TODO : ver si se debe cerrar el socket desde la partida
				close(datosPartida.socketCliente2);
				// FIN TODO
				flagCliente2 = 0;
				imprimirError(0, "ERROR escribiendo en el socket");
			}
			elementoDeCola = 0; //limpia el elemento
		}
	}
	pthread_exit(NULL);
}

