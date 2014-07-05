/*#####################################
#Trabajo Practico Nº4
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#Garcia Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/

#include "client.h"


/**		Variables		*/
int flagCliente = 1, errorServidor=0;


/**
*FUNCION DE AYUDA
*/
void help() {
	printf("Uso: ./CLIENTE.exe <arg0> <arg1> \n");
	printf("\t<arg0> (obligatorio): es un string que indica el HOSTNAME del servidor a donde se debe conectar\n");
	printf("\t<arg1> (obligatorio): es un entero positivo que indica el PUERTO del servidor a donde se debe conectar\n");
	printf("\tConsulte el archivo 'Modos de Uso.txt' para mayor informacion\n");
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
		default: break;
	}
	if (msg != NULL) {
		printf("%s", msg);
	}
	//exit(EXIT_FAILURE);
}

void * escuchaServidor(void *argumentos) {	
	int *socketServer = (int*)argumentos;	
	char buffer[BUFFERSIZE];
	while(1) {	
		bzero(buffer,BUFFERSIZE);		
		int resp=read(*socketServer, buffer, BUFFERSIZE);				
		if (resp <= 0) {	
			imprimirError(0, "\nERROR al leer del server.\n");			
			errorServidor=1;
			return;
		}
	}
}

void * envioAServidor(void *argumentos) {	
	int *socketServer = (int*)argumentos;	
	char buffer[BUFFERSIZE];
	while(flagCliente) {	
		printf("Ingrese un mensaje: ");
		bzero(buffer,BUFFERSIZE);
		fgets(buffer,BUFFERSIZE,stdin);
		//Envia mensajes al servidor
		if ((write(*socketServer, buffer, BUFFERSIZE)) < 0) {
			imprimirError(0, "ERROR escribiendo en el socket");
		}
		if (strcmp(buffer, "SALIR\n") == 0) {
			flagCliente = 0;
		}
		bzero(buffer,BUFFERSIZE);
	}
}

int main(int argc, char *argv[]) {

	/*Variables*/
	pthread_t *threadEscuchaServer;
	pthread_t *threadEnvioAServer;
	int sockFileDescriptor; //Contiene los I/O Streams
	int portNumber;
	struct sockaddr_in serv_address; //dirección del server
	struct hostent *server; //información del host

	char buffer[BUFFERSIZE]; //buffer de mensajes
	/*Fin variables*/

	/*Validación de Parametros*/
	/*Cantidad de Parámetros 2 :: hostname, puerto de conexion.*/
	if(argc == 2 && strcmp(argv[1],"-h") == 0) {
		help();
		exit(EXIT_SUCCESS);
	}
	if (argc < 3) {
		imprimirError(1, NULL);
	}
	if (!esCaracterEnteroPositivo(argv[2])) {
		imprimirError(2, NULL);
	}
	/*Fin validación de parámtros*/
	portNumber = atoi(argv[2]);
	server = gethostbyname(argv[1]); //asigna información del servidor al hostent
	if (server == NULL) {
		imprimirError(0, "ERROR, no se encuentra el host.");
	}


	// Establece el socket de conexion
	sockFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (sockFileDescriptor < 0) {
		imprimirError(0, "ERROR abriendo el socket");
	}

	bzero((char *) &serv_address, sizeof(serv_address));
	serv_address.sin_family = AF_INET;
	//copia bytes de un array a otro
	bcopy((char *) server->h_addr, (char *) &serv_address.sin_addr.s_addr, server->h_length);
	serv_address.sin_port = htons(portNumber);
	// Establece la conexion con el server a traves del socket.
	if (connect(sockFileDescriptor, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0) {
		imprimirError(0, "\nERROR al conectar con el servidor.\n");
		exit(EXIT_SUCCESS);
	}
	
	//Crea Thread de escucha de mensajes	
	threadEscuchaServer = malloc(sizeof(pthread_t));
	if (threadEscuchaServer == NULL) {
		imprimirError(0, "Memoria insuficiente!");
	}
	pthread_create(threadEscuchaServer, NULL, escuchaServidor, (void *) &sockFileDescriptor);
			
	//Crea Thread Envio de mensajes
	threadEnvioAServer = malloc(sizeof(pthread_t));
	if (threadEscuchaServer == NULL) {
		imprimirError(0, "Memoria insuficiente!");
	}
	pthread_create(threadEscuchaServer, NULL, envioAServidor, (void *) &sockFileDescriptor);	
	
	while (flagCliente && !errorServidor) {//error servidor es un ejemplo pero podriamos usar una pthread_join para esperar que termine alguno de los dos threads	
		//TODO ver acciones: podriamos  reconectar el cliente a otro servidor (de partidas a torneo) y cosas por el estilo según un mensaje particular	
	}
	if(errorServidor) {
			pthread_kill(*threadEnvioAServer, SIGKILL);
	}	

	close(sockFileDescriptor);
	exit(EXIT_SUCCESS);
}
