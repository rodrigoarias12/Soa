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

struct s_datosPartida {
	int id;
	int socket;
};


/**
*FUNCION DE MANEJO CONEXION TORNEO
*/
void *manejadorConexionTorneo(void *argumentos) {

	struct s_datosPartida *args = argumentos;

	int flagConexionTorneo = 1;
	while (flagConexionTorneo) {
		//lee de la memoria compartida si hay algun dato para el.
	}
	pthread_exit(NULL);
}


/**
*FUNCION DE MANEJO DEL CLIENTE
*/
void *manejadorCliente(void *argumentos) {

	struct s_datosPartida *args = argumentos;
	char buffer[BUFFERSIZE]; //contendra los datos leidos o enviados, el tamaño esconfigurado con la variable BUFFERSIZE 
	bzero(buffer,BUFFERSIZE);
		
	//Lee datos del socket del cliente, leerá el tamaño del buffer o la cantidad indicada en el parametro 3 lo que sea que sea menor.
	//read(socket, buffer donde carga el mensaje, cantidad)
	int flagCliente = 1;
	while (flagCliente) {
		if ((read(args->socket, buffer, BUFFERSIZE)) <= 0) {		
			close(args->socket);
			flagCliente=0;
			imprimirError(0, "ERROR no se puede leer del cliente.");
		}
		else if (strcmp(buffer, "SALIR\n") == 0) {
			flagCliente = 0;
			if ((write(args->socket, "Se ha desconectado exitosamente", 31)) < 0) {
				imprimirError(3, "");
			}
		}
		if ((write(args->socket, "", 18)) < 0) {
			imprimirError(3, "");
		}
		bzero(buffer, BUFFERSIZE);
	}
	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {

	

}
