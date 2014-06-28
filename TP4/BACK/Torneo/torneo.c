/*#####################################
#Trabajo Practico N�4
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#Garcia Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/

#include "torneo.h"

int flagTiempo=1;
time_t tiempoFin;

//Socket servidor
int sockFileDescriptor; //Contiene los I/O Streams
int conectados=0;//Contiene la cantidad de usuarios activos
char **parametrosAEnviar;

int memId_vectorCliente, semId_vectorCliente, semId_partidosRealizados;
struct s_datosCliente *v_datosCliente;
int *partidosRealizados=NULL; // Aloja a todos los participantes y si realizarar�n partidos entre ellos;


int main(int argc, char *argv[]) {
	/*Variables*/
	int portNumber; //Numero de puerto
	int duracionTorneo; //En minutos
	int tiempoInmunidadTorta; //En segundos

	struct sockaddr_in serv_address; //estructura que contiene direcci�n del servidor
	pthread_t t_armaPartidas;
	pthread_t t_escuchaConexiones;
	/*Fin variables*/

	// TODO: debe levantarse desde un archivo de configuraciones
	portNumber = atoi(argv[1]);
	duracionTorneo = atoi(argv[2]);
	tiempoInmunidadTorta = atoi(argv[3]);
	// FIN TODO

	//Inicializa memoria compartida
	memId_vectorCliente = shmget(IPC_PRIVATE, sizeof(struct s_datosCliente) * MAXCONEXIONES, 0777 | IPC_CREAT);
	
	if(memId_vectorCliente == -1) {
		imprimirError(0, "Error al crear memoria Compartida");
	}
	
	//Semaforo vector de clientes, en memoria compartida	
	semId_vectorCliente = crear_sem(IPC_PRIVATE, 1);
	if(semId_vectorCliente == -1) {
		imprimirError(0, "Error al crear el semaforo");
	}

	//Semaforo vector de partidas
	semId_partidosRealizados = crear_sem(IPC_PRIVATE, 1);
	if(semId_partidosRealizados == -1) {
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
	

	//TODO: Quitar el signal
	//inicializa el conteo de tiempo del servidor
	/*signal(SIGALRM, terminarServer);
	alarm(duracionTorneo * 60);*/
	tiempoFin = time(NULL) + (duracionTorneo*60);
	// Se crea thread de escucha de nuevos clientes para el torneo
	pthread_create(&t_escuchaConexiones, NULL, aceptaConexiones, NULL);

	//Se crea thread que arma partidas
	pthread_create(&t_armaPartidas, NULL, armaPartidas, NULL);


	pthread_join(t_escuchaConexiones, NULL);
	pthread_join(t_armaPartidas, NULL);
	

	if(cerrar_sem(semId_vectorCliente) == -1) {
		imprimirError(0, "Error al cerrar los semaforos");
	}
	if(cerrar_sem(semId_partidosRealizados) == -1) {
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
	fflush(NULL);
	if (msg != NULL) {
		printf("%s \n",msg);
	}
	if (errno) {
		printf("\n %d : %s \n", errno, strerror(errno));
	}
	//exit(EXIT_FAILURE); //TODO: esta funcion debe tener el control de terminaci�n de ejec??
}

/**
*FUNCION DE ESCUCHA DE SE�AL
*/
void terminarServer(int signal) {
	flagTiempo = 0;
	close(sockFileDescriptor); //cierra el socket
	cierraClientes();
}

/**
*FUNCION CONEXION DEL SERVIDOR
*/
void conectarServidor(struct sockaddr_in *serv_address, int *sockFileDescriptor, int *portNumber) {
	serv_address->sin_family = AF_INET;
	serv_address->sin_addr.s_addr = INADDR_ANY; 
	serv_address->sin_port = htons(*portNumber); //Convierte el portnumber en un host y lo asigna al servidor

	//bind: une un socket a una direcci�n  
	if (bind(*sockFileDescriptor, (struct sockaddr *) serv_address, sizeof(*serv_address)) < 0) {
		imprimirError(0, "ERROR al conectar el socket.");
		exit(0);
	}
	/*Fin inicializacion del servidor*/
}


void inicializaVector(int **vec,int cantidad){
	int suma=sumatoriaPartidas(cantidad);
	if(*vec==NULL)
	{
		*vec=malloc(sizeof(int));
		*(*vec)=1; //guarda uno en el primer elemento, pertenece a la diagonal principal
	}
	else
	{
		*vec=(int *)realloc(*vec,suma*sizeof(int));
		int i=sumatoriaPartidas(cantidad-1);
		for(i;i<suma-1;i++)
		{
			*(*vec+i)=0;
		}
		*(*vec+i)=1;//guarda uno en el ultimo elemento, pertenece a la diagonal principal
	}
}

int sumatoriaPartidas(int num){
	if(num==1 || num==0){
		 return 1;
	}
	else{
		return num+sumatoriaPartidas(num-1);
	}
}

/**
*FUNCION ESCUCHA CONEXIONES DE CLIENTES
*/
void *aceptaConexiones() {
	struct sockaddr_in cli_address; //estructura que contiene direcci�n del cliente
	socklen_t clilen = sizeof(cli_address); //struct client
	int clientSockFileDescriptor;  //I/O Streams del cliente

	double tiempoRestante = difftime(tiempoFin, time(NULL));
	while (tiempoRestante >= 0 && conectados < MAXCONEXIONES) {
		printf("\ntiempo restante %f\n", tiempoRestante);
		//Reliza la conexi�n. Bloquea el proceso hasta que la conexi�n se realiza con exito
		clientSockFileDescriptor = accept(sockFileDescriptor, (struct sockaddr *) &cli_address, &clilen);
		if (clientSockFileDescriptor < 0 && tiempoRestante >= 0) {
			imprimirError(0, "ERROR al aceptar conexiones por el puerto");
		} else if(clientSockFileDescriptor > 0) {
			sem_P(semId_vectorCliente);
			//Ejecuta si se realizo una conexi�n			
			v_datosCliente[conectados].id = conectados;
			v_datosCliente[conectados].socket = clientSockFileDescriptor;
			v_datosCliente[conectados].ip = (char *) malloc(sizeof(inet_ntoa(cli_address.sin_addr))+1);
			v_datosCliente[conectados].ip = (char *) inet_ntoa(cli_address.sin_addr);
			v_datosCliente[conectados].jugando=0;
			v_datosCliente[conectados].activo=1;
			conectados++;
			sem_V(semId_vectorCliente);
			sem_P(semId_partidosRealizados);			
			inicializaVector(&partidosRealizados, conectados);
			sem_V(semId_partidosRealizados);
		}
		tiempoRestante = difftime(tiempoFin, time(NULL));
	}
	pthread_exit(NULL);
}

void *armaPartidas() {
	while(difftime(tiempoFin, time(NULL))>=0){
		int i=1,k=0,partidasJugadas=0;	
		int tamVector=sumatoriaPartidas(conectados);	//guarda la cantidad de posiciones del vector		
		sem_P(semId_partidosRealizados);
		/* Se recorre el vector mientras tengo mas de un conectado, la variable i se posiciona en un jugador (pivot) y la variable j indica el enfrentamiento con cada uno de los contrincantes posibles para ese jugador, se controla que exista el vector de partidos realizados y que no se se exceda la dimensi�n del mismo*/
		while(conectados>1 && i<=conectados && k<tamVector && partidosRealizados!=NULL) 
		{
			int j=0;			
			for(j;j<i;j++){//recorro todos los posibles partidos			
				k++;				
				if(*(partidosRealizados+k)==0){ //no se realizo un partido entre ambos jugadores
					sem_P(semId_vectorCliente);
					if(v_datosCliente[i].activo==1 && v_datosCliente[j].activo==1) {//ambos jugadores estan activos
						if(v_datosCliente[i].jugando==0 && v_datosCliente[j].jugando==0) //ambos jugadores disponibles para jugar
						{
							v_datosCliente[i].jugando=1;
							v_datosCliente[j].jugando=1;
							*(partidosRealizados+k)=1;
							fflush(NULL);
							printf("Juegan normal %d , %d\n",i,j);

							pid_t pID = vfork();
							if (pID == 0) {
								// Proceso hijo
								parametrosAEnviar = generaParametrosPartida(memId_vectorCliente, semId_vectorCliente, i, j);
								execv(EJECUTABLEPARTIDA, parametrosAEnviar);
								imprimirError(0, "ERROR al crear el servidor de partida.");
								exit(EXIT_FAILURE);
							} else if (pID < 0) {
								// Fallo el fork
								imprimirError(0, "ERROR al crear el servidor de partida.");
							}
						}
					}
					sem_V(semId_vectorCliente);
				} else {
					partidasJugadas++; //cuenta cantidad de partidas ya realizadas
				}
			}
			i++; //incrementa el pivot
		}
		if(conectados>1 && partidasJugadas==(k+1-conectados)) { //posiciones de vector+1-CantidadJugadores==cantPartidasJugadas
			partidasRandom();
		}
		sem_V(semId_partidosRealizados);	//Libera el semaforo de la memoria de partidos realizados
	}
	pthread_exit(NULL);
}

void partidasRandom(){
	int i=0;
	while(i<=(conectados/2)){ //intentara lanzar una partida random cada dos jugadores
		int a=rand()%(conectados); //devuelve un random entre 0 - conectados
		int b=rand()%(conectados);
		sem_P(semId_vectorCliente);
		if(a!=b && v_datosCliente[a].activo==1 && v_datosCliente[b].activo==1) { //ambos jugadores estan activos
			if(v_datosCliente[a].jugando==0 && v_datosCliente[b].jugando==0) //ambos jugadores disponibles para jugar
			{											
				v_datosCliente[a].jugando=1;
				v_datosCliente[b].jugando=1;
				fflush(NULL);
				printf("Juegan random %d , %d\n",a,b);

				pid_t pID = vfork();
				if (pID == 0) {
					// Proceso hijo
					parametrosAEnviar = generaParametrosPartida(memId_vectorCliente, semId_vectorCliente, a, b);
					execv(EJECUTABLEPARTIDA, parametrosAEnviar);
					imprimirError(0, "ERROR al crear el servidor de partida.");
					exit(EXIT_FAILURE);
				} else if (pID < 0) {
					// Fallo el fork
					imprimirError(0, "ERROR al crear el servidor de partida.");
				}
			}
		}
		sem_V(semId_vectorCliente);
		i++;
	}
}

void cierraClientes(){
	int i;
	for(i=0;i<conectados;i++){
			close(v_datosCliente[i].socket);
	}
}

char ** generaParametrosPartida(int param1, int param2, int param3, int param4) {
	char** parametros = (char **) calloc(sizeof(char *), 6);;
	parametros[0] = (char *) malloc(sizeof(char) * (strlen(EJECUTABLEPARTIDA)+1));
	strcpy(parametros[0], EJECUTABLEPARTIDA);

	parametros[1] = (char *) malloc(sizeof(char) * sizeof(param1)+1);
	sprintf(parametros[1], "%d", param1);

	parametros[2] = (char *) malloc(sizeof(char) * sizeof(param2)+1);
	sprintf(parametros[2], "%d", param2);

	parametros[3] = (char *) malloc(sizeof(char) * sizeof(param3)+1);
	sprintf(parametros[3], "%d", param3);

	parametros[4] = (char *) malloc(sizeof(char) * sizeof(param4)+1);
	sprintf(parametros[4], "%d", param4);

	parametros[5] = NULL;

	return parametros;
}
