/*#####################################
#Trabajo Practico Nº4
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#Garcia Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/

#include "server.h"

int flag=1;

//Socket servidor
int sockFileDescriptor; //Contiene los I/O Streams


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
	if (msg != NULL) {
		printf("%s \n",msg);
	}
	//exit(EXIT_FAILURE); //TODO: esta funcion debe tener el control de terminación de ejec??
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
void conectarServidor(struct sockaddr_in *serv_address,int *sockFileDescriptor,int *portNumber){
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
*FUNCION DE MANEJO DEL CLIENTE
*/
void *manejadorCliente(void *argumentos) {

	struct arg_struct *args = argumentos;
	char buffer[BUFFERSIZE]; //contendra los datos leidos o enviados, el tamaño esconfigurado con la variable BUFFERSIZE 
	bzero(buffer,BUFFERSIZE);

	FILE *archivoSalida = fopen("log.txt", "a");
	if(!archivoSalida) {
		imprimirError(4, NULL);
	}
		
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
			if ((write(args->socket, "pediste LOG COMPLETO", 18)) < 0) {
				imprimirError(3, "");
			}
		} else {
			args->cantidadMsg++;
			if ((write(args->socket, "", 18)) < 0) {
				imprimirError(3, "");
			}
		}
		bzero(buffer, BUFFERSIZE);
	}
	char *currentTime = ctime(&(args->timeInicio));
	char *p = strchr(currentTime,'\n'); 
	if (p) {
		*p = '\0';
	}
	printf("Se ha desconectado un cliente\nLOG: IP %s \t Hs conexion %s \t Tiempo total %f \t Cant msj %d\n", args->ip, currentTime, difftime(time(NULL), args->timeInicio), args->cantidadMsg);
	fprintf(archivoSalida, "%d, %s, %s, %f, %d\n", args->id, args->ip, currentTime, difftime(time(NULL), args->timeInicio), args->cantidadMsg);
	fclose(archivoSalida);
	close(args->socket);
	pthread_exit(NULL);
}

/**
*FUNCION DE ESCUCHA DE SEÑAL
*/
void terminarServer(int signal) {
	flag = 0;
	close(sockFileDescriptor); //cierra el socket
}


int main(int argc, char *argv[]) {

	/*Variables*/
	int portNumber; //Numero de puerto
	int cantConexiones = 5;

	int clientSockFileDescriptor;  //I/O Streams del cliente
	socklen_t clilen; //struct client

	struct sockaddr_in serv_address; //estructura que contiene dirección del servidor
	struct sockaddr_in cli_address; //estructura que contiene dirección del cliente

	int conectados=0, desconectados=0;
	struct arg_struct *args;
	pthread_t *thread;
	/*Fin variables*/


	/*Validación de Parametros*/
	//Cantidad de parámetros minimos 1 :: puerto de escucha de conexiones.
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
	/*Fin validación de parámtros*/
	portNumber = atoi(argv[1]);
	thread = malloc(sizeof(pthread_t) * cantConexiones);
	args = (struct arg_struct *) malloc(sizeof(struct arg_struct) * cantConexiones);
	if (thread == NULL || args == NULL) {
		imprimirError(0, "Memoria insuficiente!");
	}


	/*Inicializacion del servidor*/
	//System call Socket(dominio, tipo de socket, protocolo)
	//AF_INT dominio: Internet	
	sockFileDescriptor = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockFileDescriptor < 0) {
		imprimirError(0, "ERROR abriendo el socket");
	}

	//bzero args: bzero(puntero del buffer,size)
	bzero((char *) &serv_address, sizeof(serv_address));	//buffers a cero

	conectarServidor(&serv_address, &sockFileDescriptor, &portNumber);


	/*Creacion del demonio*/
	//armarEsqueletoDemonio();

	listen(sockFileDescriptor, cantConexiones);
	clilen = sizeof(cli_address);

	signal(SIGUSR1, terminarServer);

	while (flag && conectados < cantConexiones) {
		//Reliza la conexión.Bloquea el proceso hasta que la conexión se realiza con exito
		clientSockFileDescriptor = accept(sockFileDescriptor,(struct sockaddr *) &cli_address, &clilen);
		if (clientSockFileDescriptor < 0 && flag) {
			imprimirError(0, "ERROR al aceptar conexiones por el puerto");
		} else if(clientSockFileDescriptor > 0) {			
			//Ejecuta si se realizo una conexión			
			args[conectados].id = conectados;
			args[conectados].socket = clientSockFileDescriptor;
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
	free(thread);
	free(args);
	close(sockFileDescriptor);

	exit(EXIT_SUCCESS);
}
