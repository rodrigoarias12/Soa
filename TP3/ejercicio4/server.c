/*#####################################
#Trabajo Práctico Nº3
#Ejercicio Número 4
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/

/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "utils.validaciones.c"
#define BUFFERSIZE 1000

/**
*FUNCION DE AYUDA
*/
void help() {
	printf("Uso: ./SERVER.exe <arg0>\n");
	printf("\t<arg0> (obligatorio): es un entero positivo que indica el PUERTO a donde se conectaran los clientes\n");
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
		perror(msg);
	}
	exit(1);
}

int main(int argc, char *argv[]) {

	/*Variables*/
	int sockFileDescriptor; //Contiene los I/O Streams
	int clientSockFileDescriptor;  //I/O Strams del cliente
	int portNumber; //Numero de puerto
	socklen_t clilen; //struct client
	char buffer[BUFFERSIZE]; //contendra los datos leidos o enviados, el tamaño esconfigurado con la variable BUFFERSIZE 
	//sockaddr_in contiene una dirección 	
	struct sockaddr_in serv_address; //estructura que contiene dirección del servidor
	struct sockaddr_in cli_address; 	//estructura que contiene dirección del cliente
	int n;
	/*Fin variables*/

	/*Validación de Parametros*/
	/*Cantidad de Parámetros 1 :: puerto de escucha de conexiones.*/
	if(argc == 2 && strcmp(argv[1],"-h") == 0) {
		help();
		exit(0);
	}
	if (argc < 2) {
		imprimirError(1, NULL);
	}
	if (!esCaracterEnteroPositivo(argv[1])) {
		imprimirError(2, NULL);
	}
	/*Fin validación de parámtros*/
	portNumber = atoi(argv[1]);


	//System call Socket params: socket(dominio, tipo de socket, protocolo)
	//AF_INT dominio= internet	
	sockFileDescriptor = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockFileDescriptor < 0) {
		imprimirError(0, "ERROR abriendo el socket");
	}

	//bzero args: bzero(puntero del buffer,size)
	bzero((char *) &serv_address, sizeof(serv_address));	//buffers a cero

	serv_address.sin_family = AF_INET;
	serv_address.sin_addr.s_addr = INADDR_ANY; 
	serv_address.sin_port = htons(portNumber); //Convierte el portnumber en un host y lo asigna al servidor

	//bind une un socket a una dirección  
	if (bind(sockFileDescriptor, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0) {
		imprimirError(0, "ERROR al conectar");
	}

	//Escucha del puerto TODO:esto debería hacerlo un manejador. Deberíamos lanzarhas more information. un thread.
	listen(sockFileDescriptor,5);

	clilen = sizeof(cli_address);

	//Reliza la conexión.Bloquea el proceso hasta que la conexión se realiza con exito	
	clientSockFileDescriptor = accept(sockFileDescriptor,(struct sockaddr *) &cli_address, &clilen);
	if (clientSockFileDescriptor < 0) {
		imprimirError(0, "ERROR al aceptar conexiones por el puerto");
	}

	bzero(buffer,256); 
	
	//Lee datos del socket del cliente, leerá el tamaño del buffer o la cantidad indicada en el parametro 3 lo que sea que sea menor. read agrs= read(socket,buffer donde carga el mensaje, cantidad)
	//TODO: ver que poner en el parametro 3
	n = read(clientSockFileDescriptor, buffer, 255);
	
	if (n < 0) {
		imprimirError(0, "ERROR no se puede leer del cliente.");
	}
	printf("Mensaje del cliente: %s\n", buffer);
	
	n = write(clientSockFileDescriptor, "Entendido", 18);
	
	if (n < 0) {
		imprimirError(0, "ERROR al escribir en el socket");
	}

	close(clientSockFileDescriptor);
	close(sockFileDescriptor);
	return 0; 
}