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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "utils.validaciones.c"
#define BUFFERSIZE 256

/**
*FUNCION DE AYUDA
*/
void help() {
	printf("Uso: ./CLIENTE.exe <arg0> <arg1> \n");
	printf("\t<arg0> (obligatorio): es un string que indica el HOSTNAME del servidor a donde se debe conectar\n");
	printf("\t<arg1> (obligatorio): es un entero positivo que indica el PUERTO del servidor a donde se debe conectar\n");
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
	int portNumber;
	int estadoAlLeerEscribir;
	struct sockaddr_in serv_address; //dirección del server
	struct hostent *server; //información del host

	char buffer[BUFFERSIZE]; //buffer de mensajes
	/*Fin variables*/

	/*Validación de Parametros*/
	/*Cantidad de Parámetros 2 :: hostname, puerto de conexion.*/
	if(argc == 2 && strcmp(argv[1],"-h") == 0) {
		help();
		exit(0);
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
		imprimirError(0, "ERROR al conectar");
	}

	printf("Ingrese un mensaje: ");
	bzero(buffer,256);
	fgets(buffer,255,stdin);
	
	//Envia mensajes al servidor
	estadoAlLeerEscribir = write(sockFileDescriptor, buffer, strlen(buffer));
	if (estadoAlLeerEscribir < 0) {
		imprimirError(0, "ERROR writing to socket");
	}
	bzero(buffer,256);

	//Lee del servidor	TODO:Threar de escucha
	estadoAlLeerEscribir = read(sockFileDescriptor, buffer, 255);
	if (estadoAlLeerEscribir < 0) {
		imprimirError(0, "ERROR al leer del server");
	}
	printf("%s\n", buffer);
	close(sockFileDescriptor);
	return 0;
}