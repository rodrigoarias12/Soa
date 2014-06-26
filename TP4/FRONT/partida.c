#include "partida.h"
int comm_socket = 0;
int comm_socket2=0;

int conectados=0;
int flagTiempo=1;
int sockFileDescriptor; //Contiene los I/O Streams
struct s_datosCliente v_datosCliente[2];
void *moverJugador1(void * n);
void *moverJugador2(void * n);
void Finalizare(int)
{

	close(sockFileDescriptor);
	printf("Finalizado.\n");
	exit(0);
}

int main(int argc, char *argv[]) {
        fflush(NULL);
        signal(2,Finalizare);//si llega señal ctrl-c

	/*Variables*/
	int portNumber=53210; //Numero de puerto
	int duracionTorneo; //En minutos
	int tiempoInmunidadTorta; //En segundos
	struct sockaddr_in serv_address; //estructura que contiene dirección del servidor
	/*Fin variables*/

	// TODO: debe levantarse desde un archivo de configuraciones	
	duracionTorneo = 60;
	tiempoInmunidadTorta = 60;
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
	//le digo cuantos voy a escuchar
        listen(sockFileDescriptor, 2);     
	

       // escuchar dos clientes  
        struct sockaddr_in cli_address; //estructura que contiene dirección del cliente
	socklen_t clilen = sizeof(cli_address); //struct client
	int clientSockFileDescriptor;  //I/O Streams del cliente
       //voy a escuchar dos conexiones solo para probar el cliente
	while (flagTiempo && conectados < 2) {
                 printf("conectado 1");
		//Reliza la conexión. Bloquea el proceso hasta que la conexión se realiza con exito
		clientSockFileDescriptor = accept(sockFileDescriptor, (struct sockaddr *) &cli_address, &clilen);
		if (clientSockFileDescriptor < 0 && flagTiempo) {
			imprimirError(0, "ERROR al aceptar conexiones por el puerto");
		} else if(clientSockFileDescriptor > 0) {
		        printf("conectado 5");
			//Ejecuta si se realizo una conexión			
			v_datosCliente[conectados].id = conectados;
			v_datosCliente[conectados].socket = clientSockFileDescriptor;		
			conectados++;
                        printf("se conecto la conexion numero  %d\n",conectados);
		}
	}
  
// estos socket tendria q traerlos del servidor 
//jugador 1
comm_socket = v_datosCliente[0].socket;
//jugador 2
comm_socket2 =  v_datosCliente[1].socket;

// ejemplo de enviar al servidor de cliente 
int nro_jug = 1;
if(send(comm_socket, &nro_jug, sizeof(int), 0)){}
nro_jug = 2;
if(send(comm_socket2, &nro_jug, sizeof(int), 0)){}
if(send(comm_socket, &duracionTorneo, sizeof(int), 0)){}
if(send(comm_socket2, &duracionTorneo, sizeof(int), 0)){}



printf("Servidor Partida: Socket1%d %d        socket2%d   %d\n",comm_socket,v_datosCliente[0].id,comm_socket2,v_datosCliente[1].id);
// moverjugador deberia tener un receive activo todo el tiempo 
/*pthread_t thread_id[3];
	pthread_create(&thread_id[0],NULL,moverJugador1,NULL);
	pthread_create(&thread_id[1],NULL,moverJugador2,NULL);
*/

//falta la logica del cliente .

while(1)
{
//dentro de este while deberia refrescar todo el tiempo por lo que se necesita todo el tiempo enviar datos y hacer toda la magia del juego
//y deberia ser while hasta que se termine , uno salga , uno gane o no se si hay otras variables .
}
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

void *moverJugador2(void * n)
{
int recibir;
	
		if(recv(comm_socket,&recibir,sizeof(recibir),0)==0)
		{
			
		}	 	
}
void *moverJugador1(void * n)
{
int recibir;
	
		if(recv(comm_socket2,&recibir,sizeof(recibir),0)==0)
		{
			
		}

}
