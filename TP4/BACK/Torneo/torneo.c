/*#####################################
#Trabajo Practico Nº4
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#Garcia Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/

#include "torneo.h"

int flagTiempo=1;

//Socket servidor
int sockFileDescriptor; //Contiene los I/O Streams

int memId_vectorCliente, semId_vectorCliente;
struct s_datosCliente *v_datosCliente;


int main(int argc, char *argv[]) {

	/*Variables*/
	int portNumber; //Numero de puerto
	int duracionTorneo; //En minutos
	int tiempoInmunidadTorta; //En segundos

	struct sockaddr_in serv_address; //estructura que contiene dirección del servidor

	pthread_t t_escuchaConexiones;
	/*Fin variables*/

	// TODO: debe levantarse desde un archivo de configuraciones
	portNumber = atoi(argv[1]);
	duracionTorneo = atoi(argv[2]);
	tiempoInmunidadTorta = atoi(argv[3]);
	// FIN TODO


	memId_vectorCliente = shmget(IPC_PRIVATE, sizeof(struct s_datosCliente) * MAXCONEXIONES, 0777 | IPC_CREAT);
	if(memId_vectorCliente == -1) {
		imprimirError(0, "Error al crear memoria Compartida");
	}
	semId_vectorCliente = crear_sem(IPC_PRIVATE, 1);
	if(semId_vectorCliente == -1) {
		imprimirError(0, "Error al crear el semaforo");
	}

	// TODO: ver si esto no deberia ir directamente dentro del thread
	//Vector en memoria compartida que aloja los clientes conectados
	v_datosCliente = shmat(memId_vectorCliente,0,0);
	// FIN TODO

	/*Inicializacion del servidor*/
	//System call Socket(dominio, tipo de socket, protocolo)
	//AF_INT dominio: Internet	
	sockFileDescriptor = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockFileDescriptor < 0) {
		imprimirError(0, "ERROR abriendo el socket");
	}
	//buffers a cero (puntero del buffer, size)
	bzero((char *) &serv_address, sizeof(serv_address));

	conectarServidor(&serv_address, &sockFileDescriptor, &portNumber);
	listen(sockFileDescriptor, MAXCONEXIONES);

	//inicializa el conteo de tiempo del servidor
	signal(SIGALRM, terminarServer);
	alarm(duracionTorneo * 60);

	// Se crea thread de escucha de nuevos clientes para el torneo
	pthread_create(&t_escuchaConexiones, NULL, aceptaConexiones, NULL);
	pthread_join(t_escuchaConexiones, NULL);


	if(cerrar_sem(semId_vectorCliente) == -1) {
		imprimirError(0, "Error al cerrar los semaforos");
	}
	// TODO: ver si esto no deberia ir directamente dentro del thread
	shmdt((char *) v_datosCliente);
	// FIN TODO
	shmctl(memId_vectorCliente, IPC_RMID, (struct shmid_ds *) NULL);

	close(sockFileDescriptor);

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
	if (msg != NULL) {
		printf("%s \n",msg);
	}
	//exit(EXIT_FAILURE); //TODO: esta funcion debe tener el control de terminación de ejec??
}

/**
*FUNCION DE ESCUCHA DE SEÑAL
*/
void terminarServer(int signal) {
	flagTiempo = 0;
	close(sockFileDescriptor); //cierra el socket
}

/**
*FUNCION CONEXION DEL SERVIDOR
*/
void conectarServidor(struct sockaddr_in *serv_address, int *sockFileDescriptor, int *portNumber) {
	serv_address->sin_family = AF_INET;
	serv_address->sin_addr.s_addr = INADDR_ANY; 
	serv_address->sin_port = htons(*portNumber); //Convierte el portnumber en un host y lo asigna al servidor

	//bind: une un socket a una dirección  
	if (bind(*sockFileDescriptor, (struct sockaddr *) serv_address, sizeof(*serv_address)) < 0) {
		imprimirError(0, "ERROR al conectar el socket.");
		exit(0);
	}
	/*Fin inicializacion del servidor*/
}

/**
*FUNCION ESCUCHA CONEXIONES DE CLIENTES
*/
void *aceptaConexiones() {

	struct sockaddr_in cli_address; //estructura que contiene dirección del cliente
	socklen_t clilen = sizeof(cli_address); //struct client
	int clientSockFileDescriptor;  //I/O Streams del cliente

	int conectados=0;

	while (flagTiempo && conectados < MAXCONEXIONES) {
		//Reliza la conexión. Bloquea el proceso hasta que la conexión se realiza con exito
		clientSockFileDescriptor = accept(sockFileDescriptor, (struct sockaddr *) &cli_address, &clilen);
		if (clientSockFileDescriptor < 0 && flagTiempo) {
			imprimirError(0, "ERROR al aceptar conexiones por el puerto");
		} else if(clientSockFileDescriptor > 0) {
			sem_P(semId_vectorCliente);
			//Ejecuta si se realizo una conexión			
			v_datosCliente[conectados].id = conectados;
			v_datosCliente[conectados].socket = clientSockFileDescriptor;
			v_datosCliente[conectados].ip = (char *) malloc(sizeof(inet_ntoa(cli_address.sin_addr))+1);
			v_datosCliente[conectados].ip = (char *) inet_ntoa(cli_address.sin_addr);
			conectados++;
			sem_V(semId_vectorCliente);
		}
	}
	pthread_exit(NULL);
}
