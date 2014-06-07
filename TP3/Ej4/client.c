#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#define BUFFERSIZE 256
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
	int sockFileDescriptor; //Contiene los I/O Streams
	int portNumber;
	int estadoAlLeerEscribir;
	struct sockaddr_in serv_address; //dirección del server
	struct hostent *server; //información del host

	char buffer[BUFFERSIZE]; //buffer de mensajes
	if (argc < 3) {
		error("Error.");
	}
	portNumber = atoi(argv[2]);
	sockFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (sockFileDescriptor < 0) 
		error("ERROR abriendo el socket");
	server = gethostbyname(argv[1]); //asigna información del servidor al hostent
	if (server == NULL) {
		error("ERROR, no se encuentra el host.");
	}
	bzero((char *) &serv_address, sizeof(serv_address));
	
	serv_address.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_address.sin_addr.s_addr,	server->h_length); //copia bytes de un array a otro
	serv_address.sin_port = htons(portNumber);
	if (connect(sockFileDescriptor,(struct sockaddr *) &serv_address,sizeof(serv_address)) < 0) 
	error("ERROR al conectar");
	printf("Ingrese un mensaje: ");
	bzero(buffer,256);
	fgets(buffer,255,stdin);
	
	//Envia mensajes al servidor
	estadoAlLeerEscribir = write(sockFileDescriptor,buffer,strlen(buffer));
	if (estadoAlLeerEscribir < 0) 
		error("ERROR writing to socket");
	bzero(buffer,256);

	//Lee del servidor	TODO:Threar de escucha
	estadoAlLeerEscribir = read(sockFileDescriptor,buffer,255);
	if (estadoAlLeerEscribir < 0) 
		error("ERROR al leer del server");
	printf("%s\n",buffer);
	close(sockFileDescriptor);
	return 0;
}