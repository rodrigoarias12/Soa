/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define BUFFERSIZE 1000
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	//Declaración de Variables	
	int sockFileDescriptor; //Contiene los I/O Streams
	int clientSockFileDescriptor;  //I/O Strams del cliente
	int portNumber; //Numero de puerto
	socklen_t clilen; //struct client
	char buffer[BUFFERSIZE]; //contendra los datos leidos o enviados, el tamaño esconfigurado con la variable BUFFERSIZE 
	//sockaddr_in contiene una dirección 	
	struct sockaddr_in serv_address; //estructura que contiene dirección del servidor
	struct sockaddr_in cli_address; 	//estructura que contiene dirección del cliente
	int n;
	if (argc < 2) {	
			error("Error, debe especificar un puerto.");
	}
	//System call Socket params: socket(dominio,tipo de socket,protocolo)
	//AF_INT dominio= internet	
	sockFileDescriptor = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockFileDescriptor < 0) 
		error("ERROR opening socket");
	
	//bzero args: bzero(puntero del buffer,size)
	bzero((char *) &serv_address, sizeof(serv_address));	//buffers a cero
	
	portNumber = atoi(argv[1]);

	serv_address.sin_family = AF_INET;
	serv_address.sin_addr.s_addr = INADDR_ANY; 
	serv_address.sin_port = htons(portNumber); //Convierte el portnumber en un host y lo asigna al servidor
   
	//bind une un socket a una dirección  
	if (bind(sockFileDescriptor, (struct sockaddr *) &serv_address,sizeof(serv_address)) < 0) 
		error("ERROR on binding");
	
	//Escucha del puerto TODO:esto debería hacerlo un manejador. Deberíamos lanzarhas more information. un thread.
	listen(sockFileDescriptor,5);
	
	clilen = sizeof(cli_address);

	//Reliza la conexión.Bloquea el proceso hasta que la conexión se realiza con exito	
	clientSockFileDescriptor = accept(sockFileDescriptor,(struct sockaddr *) &cli_address,&clilen);
	
	if (clientSockFileDescriptor < 0) 
		error("ERROR on accept");
	
	bzero(buffer,256); 
	
	//Lee datos del socket del cliente, leerá el tamaño del buffer o la cantidad indicada en el parametro 3 lo que sea que sea menor. read agrs= read(socket,buffer donde carga el mensaje, cantidad)
	//TODO: ver que poner en el parametro 3
	n = read(clientSockFileDescriptor,buffer,255);
	
	if (n < 0) error("ERROR no se puede leer del cliente.");
		printf("Mensaje del cliente: %s\n",buffer);
	
	n = write(clientSockFileDescriptor,"Entendido",18);
	
	if (n < 0) error("ERROR al escribir en el socket");
	
	close(clientSockFileDescriptor);
	close(sockFileDescriptor);
	return 0; 
}