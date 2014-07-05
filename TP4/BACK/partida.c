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


int partida,memId_vectorCliente,memId_vectorPartidas,pidServer;
int semId_vectorCliente, semId_colaMensajesDeCliente, semId_colaMensajesACliente,semId_vectorPartidas;
struct s_datosCliente *v_datosCliente;

struct s_datosPartida *v_datosPartida;

//Cola de mensajes que se envian desde los clientes
struct tcola *c_mensajesDeCliente;
//Cola de mensajes que se envian a los clientes
struct tcola *c_mensajesACliente;

pthread_t t_escuchaCliente1;
pthread_t t_escuchaCliente2;
pthread_t t_enviaClientes;
pthread_t t_verificaEstadoServer;

int main(int argc, char *argv[]) {

	/*VARIABLES*/
	//Threads
	
	/*FIN VARIABLES*/

	/*INICIALIZACION de VARIABLES con los datos dados por el torneo*/
	memId_vectorCliente = atoi(argv[1]);
	semId_vectorCliente = atoi(argv[2]);
	semId_vectorPartidas=atoi(argv[3]);
	memId_vectorPartidas=atoi(argv[4]);
	
	partida = atoi(argv[7]);
	//parametrosAEnviar = generaParametrosPartida(memId_vectorCliente, semId_vectorCliente,semId_vectorPartidas,memId_vectorPartidas, a, b,partidas-1,getpid());
	//datosPartida.flag_partidaViva = 1;

	//Vector en memoria compartida que aloja los clientes conectados
	v_datosCliente = shmat(memId_vectorCliente,0,0);
	v_datosPartida = shmat(memId_vectorPartidas,0,0);	
	
	sem_P(semId_vectorPartidas);
	printf("crea partida %d\n", getpid());
	v_datosPartida[partida].idCliente1 = atoi(argv[5]);
	v_datosPartida[partida].idCliente2 = atoi(argv[6]);
	v_datosPartida[partida].pidPartida=getpid();	
	v_datosPartida[partida].flag_partidaViva = 1;
	
	printf("cliente 1 %d cliente 2 %d \n",atoi(argv[5]),atoi(argv[6]));
	pidServer=atoi(argv[8]);	

	sem_P(semId_vectorCliente);
	v_datosPartida[partida].socketCliente1 = v_datosCliente[atoi(argv[5])].socket;
	v_datosPartida[partida].socketCliente2 = v_datosCliente[atoi(argv[6])].socket;
	fflush(NULL);	
	printf("Socket: %d cliente %d\n",v_datosPartida[partida].socketCliente1,1);	
	printf("Socket: %d cliente %d\n",v_datosPartida[partida].socketCliente2,2);	
	sem_V(semId_vectorCliente);	
	sem_V(semId_vectorPartidas);
	fflush(NULL);
	printf("Arranca una partida\n");
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
	pthread_create(&t_escuchaCliente1, NULL, leeCliente, &v_datosPartida[partida].socketCliente1);
	pthread_create(&t_escuchaCliente2, NULL, leeCliente, &v_datosPartida[partida].socketCliente2);
	pthread_create(&t_enviaClientes, NULL, enviaCliente, NULL);
	pthread_create(&t_verificaEstadoServer, NULL, verificaEstadoServer, NULL);	
	sem_V(semId_vectorCliente);

	pthread_join(t_escuchaCliente1, NULL);
	pthread_join(t_escuchaCliente2, NULL);
	pthread_join(t_enviaClientes, NULL);
	pthread_join(t_verificaEstadoServer, NULL);	
	
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
	shmdt((char *) v_datosPartida);
	//exit(EXIT_SUCCESS);
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

void * verificaEstadoServer(){
	while(1){ //TODO: mergear con la viarable de control del negro	
		kill(pidServer,0);
		printf("El pid del server %d esta %d",pidServer,errno);	
		if(errno==ESRCH){
			//TODO: Enviar a los clientes que se termino todo. Ver como hacer en el paquete	
			sem_P(semId_vectorCliente);		
			pthread_kill(t_escuchaCliente1,SIGKILL);
			pthread_kill(t_escuchaCliente2,SIGKILL);
			pthread_kill(t_enviaClientes,0);
			pthread_kill(t_verificaEstadoServer,0);
			close(v_datosPartida[partida].socketCliente1);
			close(v_datosPartida[partida].socketCliente2);
			sem_V(semId_vectorCliente);	
			return;	
		}
		usleep(1000000);
	}
}
