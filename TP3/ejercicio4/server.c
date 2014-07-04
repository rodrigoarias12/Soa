/*#####################################
#Trabajo Pr�ctico N�3
#Ejercicio N�mero 4
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#Garc�a Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/

#include "server.h"

int flag=1;

//Socket servidor
int socketEscucha; //Contiene los I/O Streams

struct arg_struct {
	int id;
	int socket;
	char *ip;
	time_t timeInicio;
	int cantidadMsg;
};


/**
*FUNCION DE AYUDA
*/
void help() {
	printf("Uso: ./SERVER.exe <arg0> [arg1]\n");
	printf("\t<arg0> (obligatorio): es un entero positivo que indica el PUERTO a donde se conectaran los clientes\n");
	printf("\t[arg1] (opcional): es un entero positivo que indica la cantidad de clientes que se permitiran, si se omite o es invalido se tomara 5 por default\n");
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
		case 3: printf("Error. No se pudo escribir en el socket.\n"); break;
		case 4: printf("Error en la apertura del archivo.\n"); break;
		default: break;
	}
	fflush(NULL);
	if (msg != NULL) {
		printf("\n %s \n", msg);
	}
	if (errno) {
		printf("\n %d : %s \n", errno, strerror(errno));
	}
	//exit(EXIT_FAILURE);
}

/**
*FUNCION DE ARMADO DEL ESQUELETO DEL DEMONIO
*/
void armarEsqueletoDemonio() {
	//Fork para generar el demonio
	pid_t pid = fork();
	if (pid < 0) {
		imprimirError(0, "ERROR al levantar el servidor como servicio");
	}
	if (pid > 0) {
		// Terminamos el proceso padre, para que el hijo se haga demonio
		exit(EXIT_SUCCESS);
	}

	//Cambio del modo de archivo
	umask(0);

	// Crea un nuevo SID para el proceso hijo para que no quede como huerfano
	if (setsid() < 0) {
		imprimirError(0, "ERROR al levantar el servidor como servicio");
	}

	// Cambia el directorio de trabajo
	/** if ((chdir("/")) < 0) {
		imprimirError(0, "ERROR al levantar el servidor como servicio");
	} */

	// Cierra las salidas estandar
	close(STDIN_FILENO);
	close(STDERR_FILENO);
	/** close(STDOUT_FILENO); */
}

/**
*FUNCION CONEXION DEL SERVIDOR
*/
void conectarServidor(struct sockaddr_in *serv_address, int *socketEscucha, int *portNumber) {
	serv_address->sin_family = AF_INET;
	serv_address->sin_addr.s_addr = INADDR_ANY; 
	serv_address->sin_port = htons(*portNumber); //Convierte el portnumber en un host y lo asigna al servidor

	int var = 1;
	// configura el socket para poder reutilizar el puerto en caso de que el server caiga por error
	setsockopt(*socketEscucha, SOL_SOCKET, SO_REUSEADDR, &var, sizeof(int));

	//bind: une un socket a una direcci�n  
	if (bind(*socketEscucha, (struct sockaddr *) serv_address, sizeof(*serv_address)) < 0) {
		imprimirError(0, "ERROR al conectar el socket.");
		exit(0);
	}
	/*Fin inicializacion del servidor*/
}

/**
*FUNCION DE MANEJO DEL CLIENTE
*/
void *manejadorCliente(void *argumentos) {

	struct arg_struct *args = argumentos;
	char buffer[BUFFERSIZE]; //contendra los datos leidos o enviados, el tama�o esconfigurado con la variable BUFFERSIZE 
	bzero(buffer,BUFFERSIZE);

	FILE *archivoSalida = fopen("log.txt", "a");
	if(!archivoSalida) {
		imprimirError(4, NULL);
	}

	char hsInicio[20];
	strftime(hsInicio, 20, "%Y-%m-%d %H:%M:%S", localtime(&(args->timeInicio)));

	//Lee datos del socket del cliente, leer� el tama�o del buffer o la cantidad indicada en el parametro 3 lo que sea que sea menor.
	//read(socket, buffer donde carga el mensaje, cantidad)
	int flagCliente = 1;
	int datosLeidos;
	while (flagCliente) {
		datosLeidos = read(args->socket, buffer, BUFFERSIZE);
		if (datosLeidos < 0) {
			close(args->socket);
			flagCliente=0;
			imprimirError(0, "ERROR no se puede leer del cliente.");
		} else if (datosLeidos == 0) {
			close(args->socket);
			flagCliente=0;
			imprimirError(0, "Un cliente se desconecto abruptamente.");
		} else {
			if (strcmp(buffer, "SALIR\n") == 0) {
				flagCliente = 0;
				if ((write(args->socket, "Se ha desconectado exitosamente", 31)) < 0) {
					imprimirError(3, "");
				}
			} else if (strcmp(buffer, "TIME\n") == 0) {
				char * str = malloc(sizeof(difftime(time(NULL), args->timeInicio)));
				sprintf(str, "%.4g", difftime(time(NULL), args->timeInicio));
				if ((write(args->socket, str, strlen(str))) < 0) {
					imprimirError(3, "");
				}
			} else if (strcmp(buffer, "MSJ\n") == 0) {
				char * str2 = malloc(sizeof(args->cantidadMsg));
				sprintf(str2, "%d", args->cantidadMsg);
				if ((write(args->socket, str2, strlen(str2))) < 0) {
					imprimirError(3, "");
				}
			} else if (strcmp(buffer, "INI\n") == 0) {
				if ((write(args->socket, ctime(&(args->timeInicio)), strlen(ctime(&(args->timeInicio))))) < 0) {
					imprimirError(3, "");
				}
			} else if (strcmp(buffer, "DIP\n") == 0) {
				if ((write(args->socket, args->ip, strlen(args->ip))) < 0) {
					imprimirError(3, "");
				}
			} else if (strcmp(buffer, "LOG\n") == 0) {
				char * strLog = malloc(200*sizeof(char));
				sprintf(strLog, "LOG: IP %s \t Hs conexion %s \t Tiempo total %f \t Cant msj %d\n", args->ip, hsInicio, difftime(time(NULL), args->timeInicio), args->cantidadMsg);
				if ((write(args->socket, strLog, strlen(strLog))) < 0) {
					imprimirError(3, "");
				}
			} else {
				if ((write(args->socket, "", 18)) < 0) {
					imprimirError(3, "");
				}
			}
			args->cantidadMsg++;
		}

		bzero(buffer, BUFFERSIZE);
	}
	printf("Se ha desconectado un cliente\n");
	printf("LOG: IP %s \t Hs conexion %s \t Tiempo total %f \t Cant msj %d\n", args->ip, hsInicio, difftime(time(NULL), args->timeInicio), args->cantidadMsg);
	fprintf(archivoSalida, "%d, %s, %s, %f, %d\n", args->id, args->ip, hsInicio, difftime(time(NULL), args->timeInicio), args->cantidadMsg);
	fclose(archivoSalida);
	close(args->socket);
	pthread_exit(NULL);
}

/**
*FUNCION DE ESCUCHA DE SE�AL
*/
void terminarServer(int signal) {
	flag = 0;
	close(socketEscucha); //cierra el socket
}


int main(int argc, char *argv[]) {

	/*Variables*/
	int portNumber; //Numero de puerto
	int cantConexiones = 5;

	int socketCliente;  //I/O Streams del cliente
	socklen_t clilen; //struct client
	struct sockaddr_in serv_address; //estructura que contiene direcci�n del servidor
	struct sockaddr_in cli_address; //estructura que contiene direcci�n del cliente

	int conectados=0, desconectados=0;
	struct arg_struct *args;
	pthread_t *thread;
	/*Fin variables*/


	/*Validaci�n de Parametros*/
	//Cantidad de par�metros minimos 1 :: puerto de escucha de conexiones.
	if(argc == 2 && strcmp(argv[1],"-h") == 0) {
		help();
		exit(EXIT_SUCCESS);
	}
	if (argc < 2) {
		imprimirError(1, NULL);
	}
	if (!esCaracterEnteroPositivo(argv[1])) {
		imprimirError(2, NULL);
	}
	if(argc == 3 && esCaracterEnteroPositivo(argv[2])) {
		cantConexiones = atoi(argv[2]);
	}
	/*Fin validaci�n de par�mtros*/
	portNumber = atoi(argv[1]);
	thread = malloc(sizeof(pthread_t) * cantConexiones);
	args = (struct arg_struct *) malloc(sizeof(struct arg_struct) * cantConexiones);
	if (thread == NULL || args == NULL) {
		imprimirError(0, "Memoria insuficiente!");
	}


	/*Inicializacion del servidor*/
	//System call Socket(dominio, tipo de socket, protocolo)
	//AF_INT dominio: Internet	
	socketEscucha = socket(AF_INET, SOCK_STREAM, 0); 
	if (socketEscucha < 0) {
		imprimirError(0, "ERROR abriendo el socket");
	}

	//bzero args: bzero(puntero del buffer,size)
	bzero((char *) &serv_address, sizeof(serv_address));	//buffers a cero

	conectarServidor(&serv_address, &socketEscucha, &portNumber);


	/*Creacion del demonio*/
	armarEsqueletoDemonio();

	listen(socketEscucha, cantConexiones);
	clilen = sizeof(cli_address);

	signal(SIGUSR1, terminarServer);

	while (flag && conectados < cantConexiones) {
		//Reliza la conexi�n.Bloquea el proceso hasta que la conexi�n se realiza con exito
		socketCliente = accept(socketEscucha,(struct sockaddr *) &cli_address, &clilen);
		if (socketCliente < 0 && flag) {
			imprimirError(0, "ERROR al aceptar conexiones por el puerto");
		} else if(socketCliente >= 0) {
			//Ejecuta si se realizo una conexi�n			
			args[conectados].id = conectados;
			args[conectados].socket = socketCliente;
			args[conectados].ip = (char *) malloc(sizeof(inet_ntoa(cli_address.sin_addr))+1);
			args[conectados].ip = (char *) inet_ntoa(cli_address.sin_addr);
			args[conectados].timeInicio = time(NULL);
			args[conectados].cantidadMsg = 0;
			pthread_create(&(thread[conectados]), NULL, manejadorCliente, (void*) &args[conectados]);
			conectados++;
		}
	}
	while (desconectados < conectados) {
		pthread_join(thread[desconectados], NULL);
		desconectados++;
	}

	printf("\nTODOS LOS CLIENTES SE HAN DESCONECTADO\n");
	int posicion = 0;
	char hsInicio[20];
	while (posicion < conectados) {
		strftime(hsInicio, 20, "%Y-%m-%d %H:%M:%S", localtime(&(args[posicion].timeInicio)));
		printf("ID %d \t IP %s \t Hs conexion %s \t Tiempo total %f \t Cant msj %d\n", args[posicion].id, args[posicion].ip, hsInicio, difftime(time(NULL), args[posicion].timeInicio), args[posicion].cantidadMsg);
		posicion++;
	}

	free(thread);
	free(args);
	close(socketEscucha);

	exit(EXIT_SUCCESS);
}
