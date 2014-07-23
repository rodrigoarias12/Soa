/*#####################################
#Trabajo Practico N 4
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
int duracionTorneo; //En minutos

//Socket servidor
int socketEscucha; //Contiene los I/O Streams
int conectados=0;//Contiene la cantidad de usuarios activos
int partidas=0;//Contiene las partidas que se han jugado
char **parametrosAEnviar;
int pidGrande;
int memId_vectorCliente,memId_vectorPartidas, semId_vectorCliente, semId_partidosRealizados,semId_vectorPartidas;
struct s_datosCliente *v_datosCliente;
struct s_datosPartida *v_datosPartida;
t_semaforo *semaforosPartida;
int *partidosRealizados=NULL; // Aloja a todos los participantes y si realizararon partidos entre ellos;
int memId_vectorSemaforosParaEliminar, semId_vectorSemaforosParaEliminar;


int main(int argc, char *argv[]) {

	/*Variables SDL para tabla*/
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
		printf("%s\n",SDL_GetError());
	mtx = SDL_CreateMutex();
	TTF_Init();
	/*Fin SDL*/

	/*Variables*/
	int portNumber; //Numero de puerto
	int tiempoInmunidadTorta; //En segundos

	struct sockaddr_in serv_address; //estructura que contiene direccion del servidor
	pthread_t t_armaPartidas;
	pthread_t t_escuchaConexiones;
	pthread_t t_verificaEstadoPartidas;
	pthread_t t_dibujarPantalla;
	/*Fin variables*/
	if(!cargarConfiguracion(&portNumber, &duracionTorneo, &tiempoInmunidadTorta)) {
		imprimirError(0, "Ocurrio un error al levantar los parametros de configuracion.");
		exit(EXIT_FAILURE);
	}

	//Inicializa memoria compartida
	memId_vectorCliente = shmget(IPC_PRIVATE, sizeof(struct s_datosCliente) * MAXCONEXIONES, 0777 | IPC_CREAT);
	memId_vectorPartidas = shmget(IPC_PRIVATE, sizeof(struct s_datosPartida) * sumatoriaPartidas(MAXCONEXIONES), 0777 | IPC_CREAT);
	memId_vectorSemaforosParaEliminar = shmget(IPC_PRIVATE, sizeof(t_semaforo) * 100 , 0777 | IPC_CREAT);

	if(memId_vectorCliente == -1 || memId_vectorPartidas == -1 || memId_vectorSemaforosParaEliminar == -1) {
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

	//Semaforo datos de partidas
	semId_vectorPartidas = crear_sem(IPC_PRIVATE, 1);
	if(semId_vectorPartidas == -1) {
		imprimirError(0, "Error al crear el semaforo");
	}

	semId_vectorSemaforosParaEliminar = crear_sem(IPC_PRIVATE, 1);
	if(semId_vectorSemaforosParaEliminar == -1) {
		imprimirError(0, "Error al crear el semaforo");
	}

	// TODO: ver si esto no deberia ir directamente dentro del thread
	//Vector en memoria compartida que aloja los clientes conectados
	v_datosCliente = shmat(memId_vectorCliente,0,0);
	v_datosPartida = shmat(memId_vectorPartidas,0,0);
	semaforosPartida = shmat(memId_vectorSemaforosParaEliminar, 0, 0);
	// FIN TODO

	/*Inicializacion del servidor*/
	//System call Socket(dominio, tipo de socket, protocolo)
	//AF_INT dominio: Internet
	socketEscucha = socket(AF_INET, SOCK_STREAM, 0);
	if (socketEscucha < 0) {
		imprimirError(0, "ERROR abriendo el socket");
	}
	//buffers a cero (puntero del buffer, size)
	bzero((char *) &serv_address, sizeof(serv_address));
	conectarServidor(&serv_address, &socketEscucha, &portNumber);
	listen(socketEscucha, MAXCONEXIONES);

	signal(SIGCHLD, sigchld_handler);
	signal(SIGINT, exit_handler);
	//inicializa el conteo de tiempo del servidor
	signal(SIGALRM, terminarServer);
	alarm(duracionTorneo * 60);

	// Se crea thread de escucha de nuevos clientes para el torneo
	pthread_create(&t_escuchaConexiones, NULL, aceptaConexiones, NULL);
	//Se crea thread que arma partidas
	pthread_create(&t_armaPartidas, NULL, armaPartidas, NULL);
	//Se crea thread para chequear el estado de las partidas
	pthread_create(&t_verificaEstadoPartidas, NULL, verificaEstadoPartidas, NULL);
	//Inicializo la parte grafica del torneo
	pthread_create(&t_dibujarPantalla, NULL, dibujarTabla, NULL);

	pthread_join(t_escuchaConexiones, NULL);
	pthread_join(t_armaPartidas, NULL);
	pthread_join(t_verificaEstadoPartidas, NULL);
	pthread_join(t_dibujarPantalla, NULL);

	sleep(7);

	exit_handler(FIN_TORNEO);
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
	//exit(EXIT_FAILURE); //TODO: esta funcion debe tener el control de terminacion de ejec??
}

/**
*FUNCIONES DE ESCUCHA DE SENIALES
*/
void terminarServer(int sig) {
	flagTiempo = 0;
	printf("TORNEO: Termino el tiempo del torneo.\n");
	shutdown(socketEscucha, SHUT_RDWR); //Indica que debe cerrar la escucha y envio de info por el socket
	close(socketEscucha); //cierra el socket
	signal(SIGALRM, SIG_IGN);

	//cierraClientes();
}

void sigchld_handler(int sig) {
	pid_t pidE = wait(NULL);
	printf("TORNEO: Finalizo el proceso %d\n", pidE);
	int i , j;
	sem_P(semId_vectorSemaforosParaEliminar);
	for(i = 0; i< partidas; i++){
		if(semaforosPartida[i].pidId == pidE){
			if(cerrar_sem(semaforosPartida[i].sem1) == -1) {
				imprimirError(0, "Error al cerrar los semaforos Partidas Realizadas\n");
			}
		}
	}
	sem_V(semId_vectorSemaforosParaEliminar);
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

	//bind: une un socket a una direccion
	if (bind(*socketEscucha, (struct sockaddr *) serv_address, sizeof(*serv_address)) < 0) {
		imprimirError(0, "ERROR al conectar el socket.");
		exit(EXIT_FAILURE);
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
	int datosLeidos, tamBuffer = 25;
	char bufferNombre[tamBuffer]; //contendra los datos del nombre del cliente
	struct sockaddr_in cli_address; //estructura que contiene direccion del cliente
	socklen_t clilen = sizeof(cli_address); //struct client
	int socketCliente;  //I/O Streams del cliente
	while (flagTiempo && conectados < MAXCONEXIONES) {
		//Reliza la conexion. Bloquea el proceso hasta que la conexion se realiza con exito
		socketCliente = accept(socketEscucha, (struct sockaddr *) &cli_address, &clilen);
		if (socketCliente < 0 && flagTiempo) {
			imprimirError(0, "ERROR al aceptar conexiones por el puerto");
		} else if(socketCliente >= 0) {
			bzero(bufferNombre, tamBuffer);
			datosLeidos = read(socketCliente, bufferNombre, tamBuffer);
			if (datosLeidos <= 0) {
				close(socketCliente);
				imprimirError(5, NULL);
			} else {
				sem_P(semId_vectorCliente);
				//Ejecuta si se realizo una conexion
				v_datosCliente[conectados].id = conectados;
				v_datosCliente[conectados].socket = socketCliente;
				v_datosCliente[conectados].ip = (char *) malloc(sizeof(inet_ntoa(cli_address.sin_addr))+1);
				v_datosCliente[conectados].ip = (char *) inet_ntoa(cli_address.sin_addr);
				strcpy(v_datosCliente[conectados].nombre, bufferNombre);
				v_datosCliente[conectados].jugando=0;
				v_datosCliente[conectados].activo=1;
				conectados++;
				sem_V(semId_vectorCliente);
				sem_P(semId_partidosRealizados);
				inicializaVector(&partidosRealizados, conectados);
				sem_V(semId_partidosRealizados);
				fflush(NULL);
				printf("TORNEO: Se ha conectado el cliente %d en el Socket: %d\n", conectados-1, socketCliente);
				usleep(1000000);
			}
		}
	}
	pthread_exit(NULL);
}

void *armaPartidas() {
	while(flagTiempo) {
		int i=1,k=0,partidasJugadas=0;
		int tamVector=sumatoriaPartidas(conectados);	//guarda la cantidad de posiciones del vector
		sem_P(semId_partidosRealizados);
		/* Se recorre el vector mientras tengo mas de un conectado, la variable i se posiciona en un jugador (pivot) y la variable j indica el enfrentamiento con cada uno de los contrincantes posibles para ese jugador, se controla que exista el vector de partidos realizados y que no se se exceda la dimension del mismo*/
		while(conectados>1 && i<=conectados && k<tamVector && partidosRealizados!=NULL)
		{
			int j=0;
			while(j<i && k<tamVector) {//recorro todos los posibles partidos
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
							printf("TORNEO: Juegan normal %d , %d\n",i,j);
							creaPartida(i,j);
						}
					}
					sem_V(semId_vectorCliente);
				} else {
					partidasJugadas++; //cuenta cantidad de partidas ya realizadas
				}
				j++;
			}
			i++; //incrementa el pivot
			k++;
		}
		if(conectados>1 && (partidasJugadas==(tamVector-conectados) || conectados ==2)) { //posiciones de vector+1-CantidadJugadores==cantPartidasJugadas
			partidasRandom();
		}

		sem_V(semId_partidosRealizados);	//Libera el semaforo de la memoria de partidos realizados
		usleep(1000000);
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
				printf("TORNEO: Juegan random %d , %d\n",a,b);
				fflush(NULL);
				creaPartida(a,b);
			}
		}
		sem_V(semId_vectorCliente);
		i++;
	}
}

void cargaVectorPartidas(int idJugador1,int idJugador2){
	//Carga datos de la partida
	sem_P(semId_vectorPartidas);
	v_datosPartida[partidas].idCliente1=idJugador1;
	v_datosPartida[partidas].idCliente2=idJugador2;
	v_datosPartida[partidas].puntosCliente1=0;
	v_datosPartida[partidas].puntosCliente1=0;
	//v_datosPartida[partidas].flag_partidaViva=1;
	//v_datosPartida[partidas].socketCliente1=v_datosCliente[idJugador1].socket;
	//v_datosPartida[partidas].socketCliente2=v_datosCliente[idJugador2].socket;
	//v_datosPartida[partidas].pidTorneo=getpid();
	sem_V(semId_vectorPartidas);
	partidas++;
}

void cierraClientes() {
	int i;
	for(i=0;i<conectados;i++) {
		close(v_datosCliente[i].socket);
	}
}

char** generaParametrosPartida(int param1, int param2, int param3, int param4, int param5, int param6, int param7, int param8, int param9, int param10, int param11) {
	char** parametros = (char **) calloc(sizeof(char *), 10);;
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

	parametros[5] = (char *) malloc(sizeof(char) * sizeof(param5)+1);
	sprintf(parametros[5], "%d", param5);

	parametros[6] = (char *) malloc(sizeof(char) * sizeof(param6)+1);
	sprintf(parametros[6], "%d", param6);

	parametros[7] = (char *) malloc(sizeof(char) * sizeof(param7)+1);
	sprintf(parametros[7], "%d", param7);

	parametros[8] = (char *) malloc(sizeof(char) * sizeof(param8)+1);
	sprintf(parametros[8], "%d", param8);

	parametros[9] = (char *) malloc(sizeof(char) * sizeof(param9)+1);
	sprintf(parametros[9], "%d", param9);

	parametros[10] = (char *) malloc(sizeof(char) * sizeof(param10)+1);
	sprintf(parametros[10], "%d", param10);

	parametros[11] = (char *) malloc(sizeof(char) * sizeof(param11)+1);
	sprintf(parametros[11], "%d", param11);
	parametros[12] = NULL;
	return parametros;
}

void creaPartida(int a,int b){
	cargaVectorPartidas(a,b); //carga informacion de la partida en el vector de partidas
	pid_t pID = vfork();
	if (pID == 0) {
		// Proceso hijo
		parametrosAEnviar = generaParametrosPartida(memId_vectorCliente, semId_vectorCliente,semId_vectorPartidas,memId_vectorPartidas, a, b,partidas-1,getppid(), semId_partidosRealizados, memId_vectorSemaforosParaEliminar, semId_vectorSemaforosParaEliminar);
		execv(EJECUTABLEPARTIDA, parametrosAEnviar);
		imprimirError(0, "ERROR al crear el servidor de partida.");
		exit(EXIT_FAILURE);
	} else if (pID < 0) {
		// Fallo el fork
		imprimirError(0, "ERROR al crear el servidor de partida.");
	}
}

void reLanzarPartida(int a,int b,int partida){
	pid_t pID = vfork();
	if (pID == 0) {
		// Proceso hijo
		parametrosAEnviar = generaParametrosPartida(memId_vectorCliente, semId_vectorCliente,semId_vectorPartidas,memId_vectorPartidas, a, b,partida,getppid(), semId_partidosRealizados, memId_vectorSemaforosParaEliminar, semId_vectorSemaforosParaEliminar);
		execv(EJECUTABLEPARTIDA, parametrosAEnviar);
		imprimirError(0, "ERROR al crear el servidor de partida.");
		exit(EXIT_FAILURE);
	} else if (pID < 0) {
		// Fallo el fork
		imprimirError(0, "ERROR al crear el servidor de partida.");
	}
}
void *verificaEstadoPartidas(){
	int flagFinalizo=0;
	while(!flagFinalizo){
		int i=0,partidasInactivas=0;
		while(i<partidas ){
			sem_P(semId_vectorPartidas);
			int status = kill(v_datosPartida[i].pidPartida,0);
			if(v_datosPartida[i].pidPartida!=0 && v_datosPartida[i].flag_partidaViva==1 && status == -1 && errno==ESRCH){
				//Si la partida esta activa y el proceso murio se lanza nuevamente la partida
				errno = 0;
				printf("TORNEO: pId partida : %d\n",v_datosPartida[i].pidPartida);
				fflush(NULL);
				v_datosPartida[i].flag_partidaViva=0;
				v_datosPartida[i].pidPartida=0;
				int a=v_datosPartida[i].idCliente1;
				int b=v_datosPartida[i].idCliente2;
				reLanzarPartida(a,b,i);
			}
			else if(v_datosPartida[i].pidPartida!=0 && v_datosPartida[i].flag_partidaViva==0 && status != -1){
				partidasInactivas++;
				kill(v_datosPartida[i].pidPartida, SIGINT);
			}
			else if(v_datosPartida[i].flag_partidaViva==0){
				partidasInactivas++;
			}
			sem_V(semId_vectorPartidas);
			i++;
		}
		if(!flagTiempo && partidasInactivas==partidas){ //Si todas las partidas estan inactivas y no hay mas tiempo finaliza
			flagFinalizo=1;
		}
		usleep(1000000);
	}
}


/* Recibo : Puerto, tiempo torneo, tiempo inmunidad, vidas */
int cargarConfiguracion(int *portNumber, int *duracionTorneo, int *tiempoInmunidadTorta){
	char aux[30];
	int intAux;
	char auxChar[30];
	FILE *ARTXT;
	ARTXT = fopen("config_server","r+");
	if(!ARTXT)
		return 0;
	/*Leo la Ip del Servidor*/
	fscanf(ARTXT,"%s",aux);
	intAux = extraerNumero(aux);
	if(intAux)
	{
		*portNumber = intAux;
		printf("El Puerto es: %d\n",*portNumber);
	}
	else
		return 0;

	//Extraigo el tiempo del torneo
	fscanf(ARTXT,"%s",aux);
	intAux = extraerNumero(aux);
	if(intAux)
	{
		*duracionTorneo = intAux;
		printf("La Duracion del Torneo es: %d\n",*duracionTorneo);
	}
	else
		return 0;
	fscanf(ARTXT,"%s",aux);
	intAux = extraerNumero(aux);
	if(intAux)
	{
		*tiempoInmunidadTorta = intAux;
		printf("La inmunidad es: %d\n",*tiempoInmunidadTorta);
	}
	else
		return 0;

	fclose(ARTXT);
	return 1;
}
int extraerNumero(char *cad)
{
	char *aux;
	aux = strchr(cad,':');
	if(*aux)
	{
		aux++;
		if(validarNumero(aux))
		{
			return atoi(aux); //retorno el valor encontrado//
		}
	}
	return -1;
}
int validarNumero(char *num)
{
	while(*num)
	{
		if(*num < '0' || *num > '9')
			return 0;
		num++;
	}
	return 1;
}


/* TODO Tareas para cierre por sigint:
Avisar servidor de partida que mataron el servidor de torneo.
Cerrar todos los semaforos.
Liberar todas las memorias compartidas
Cerrar todos los sockets
¿Puntaje y lo demas?
*/
void exit_handler(int sig) {
	printf("TORNEO: Finalizando Torneo con señal %d\n", sig);
	int i;

	if (sig == 2) {
		// significa que entro por un SIGINT
		sig = DISCONNECT;
	}

	/*Liberar las partidas*/
	printf("TORNEO: Cantidad de partidas a liberar: %d\n", partidas);
	for(i = 0; i < partidas ; i++){
		kill(SIGINT, v_datosPartida[i].pidPartida);
	}

	/*Funcion que captura la senial de finalizacion Ctrol + C*/
	printf("TORNEO: Liberacion segura semaforos\n");
	if(cerrar_sem(semId_partidosRealizados) == -1) {
		imprimirError(0, "Error al cerrar los semaforos Partidas Realizadas\n");
	}
	if(cerrar_sem(semId_vectorCliente) == -1) {
		imprimirError(0, "Error al cerrar los semaforos Clientes\n");
	}
	if(cerrar_sem(semId_vectorPartidas) == -1) {
		imprimirError(0, "Error al cerrar los semaforos Vector Partidas\n");
	}
	if(cerrar_sem(semId_vectorSemaforosParaEliminar) == -1) {
		imprimirError(0, "Error al cerrar los semaforos Vector Partidas\n");
	}

	// Recorre los clientes, enviadoles un mensaje de que el servidor se murio o termino.
	printf("TORNEO: Cierre de sockets involucrados. Cantidad de Jugadores a liberar: %d\n", conectados);
	for(i = 0; i < conectados ; i++){
		if(v_datosCliente[i].activo==1){
			write(v_datosCliente[i].socket, &sig, sizeof(int));
		}
	}

	printf("TORNEO: Liberacion de memoria compartida\n");
	shmdt((char *) v_datosCliente);
	shmdt((char *) v_datosPartida);
	shmdt((char *) semaforosPartida);
	shmctl(memId_vectorCliente, IPC_RMID, (struct shmid_ds *) NULL);
	shmctl(memId_vectorPartidas, IPC_RMID, (struct shmid_ds *) NULL);
	shmctl(memId_vectorSemaforosParaEliminar, IPC_RMID, (struct shmid_ds *) NULL);

	SDL_Quit();
	SDL_DestroyMutex(mtx);

	_exit(EXIT_SUCCESS);
}

void *dibujarTabla(void *n) {
	/*Inicializo todas las variables*/
	pthread_t t_dibujarTiempoTorneo;
	pthread_t t_dibujarTabla;

	int posIniTexto = 75;
	TTF_Font *fuente = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf",24);
	SDL_Color color = {255,255,255};
	//La posicion del tiempo es fija
	char cadena[1000];  //Cuidar los tamaños de los datos
	sprintf(cadena,"%s","Tiempo de Torneo");
	texto = TTF_RenderText_Solid(fuente,cadena,color);
	SDL_WM_SetIcon(SDL_LoadBMP("./sprites/icon.bmp"), NULL); //Pongo el icono de la ventana
	SDL_WM_SetCaption("WreckIt Ralph Server",NULL); //Pongo el titulo de la ventana
	ventana = SDL_SetVideoMode(600,400,8,SDL_HWSURFACE | SDL_DOUBLEBUF); //Ventana principal

	SDL_FillRect(ventana, NULL, 0x000000);

	SDL_Rect contenedorTexto;
	contenedorTexto.x = 20;
	contenedorTexto.y = 10;
	contenedorTexto.w = 150;
	contenedorTexto.h = 24;

	SDL_BlitSurface(texto,NULL,ventana,&contenedorTexto);
	sprintf(cadena, "%s", "Nombre Jugadores                              Puntos");
	texto = TTF_RenderText_Solid(fuente,cadena,color);

	contenedorTexto.x = 20;
	contenedorTexto.y = 50;
	contenedorTexto.w = 450;
	contenedorTexto.h = 24;

	SDL_BlitSurface(texto,NULL,ventana,&contenedorTexto);

	inicializarPosicionesTorneo();
	/*Dibujo ventana por siempre*/
	pthread_create(&t_dibujarTiempoTorneo, NULL, dibujarTiempoTorneo, NULL);
	pthread_create(&t_dibujarTabla, NULL, dibujarContenidoTabla, NULL);
}

/***
Dibuja el tiempo del torneo
*/
void *dibujarTiempoTorneo(void * n){
	SDL_Surface *texto;
	TTF_Font *fuente = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf",24);
	SDL_Color color = {255,255,255};
	//La posicion del tiempo es fija
	SDL_Rect contenedorTexto;
	contenedorTexto.x = 280;
	contenedorTexto.y = 10;
	contenedorTexto.w = 75;
	contenedorTexto.h = 24;


	//Rectangulo para una barra de tiempo
	SDL_Rect barraTiempo;
	barraTiempo.x = 350;
	barraTiempo.y = 15;
	barraTiempo.w = 10;
	barraTiempo.h = 20;


	int minutos = duracionTorneo;
	int segundos = 0;
	char cadena[8];
	barraTiempo.w = 210;//(minutos*60+segundos); //Ejemplo en una escala de a 2
	float unidad = (210 /(minutos*60)) + 1;
	printf("Unidad: %.2f\n", unidad);
	fflush(NULL);
	for(;;)
	{
		sprintf(cadena,(segundos>=10)?"%d:%d":"%d:0%d",minutos,segundos);

		SDL_mutexP(mtx); //El fill rect dibuja en la ventana y renderText parece que tambien
		texto = TTF_RenderText_Solid(fuente,cadena,color);
		SDL_FillRect(ventana,&barraTiempo,SDL_MapRGB(ventana->format,0,146,85)); //Borro el anterior estado de la barra
		SDL_FillRect(ventana,&contenedorTexto,SDL_MapRGB(ventana->format,0,0,0)); //Borro el anterior estado de la hora

		barraTiempo.w -= unidad;
		if((int)barraTiempo.w < 65300)
		SDL_FillRect(ventana,&barraTiempo,SDL_MapRGB(ventana->format,0,200,0)); //Actualizo la barra
		SDL_BlitSurface(texto,NULL,ventana,&contenedorTexto); //Actualizo la hora
		SDL_Flip(ventana);  //Refresca, este no iria ya que solo refresca un thread de los otros
		SDL_mutexV(mtx);
		sleep(1);
		if(minutos == 0 && segundos == 0)
		{
			SDL_mutexP(mtx);
			barraTiempo.w=210;
			SDL_FillRect(ventana,&barraTiempo,SDL_MapRGB(ventana->format,0,146,85)); //Borro el largo de la barra
			SDL_Flip(ventana);
			SDL_mutexV(mtx);
			break;
		}
		segundos--;
		if(segundos == -1)
		{
			minutos--;
			segundos = 59;
		}
	}
	return NULL;
}

void *dibujarContenidoTabla(void *n){
	/*Chequeo el contenido de la tabla cada 1 segundo
	para detectar si hay clientes conectados, y los dibujo
	en la pantalla con sus respectivos nombres*/
	SDL_Rect contenedorNombre, contenedorPuntos;
	SDL_Surface *nombreS, *puntosS;
	TTF_Font *fuente = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf",24);
	SDL_Color color = {255,255,255};
	char nombre[25], puntos[4];

	int idJugadoresOrdenadosPorPuntos[MAXCONEXIONES];
	for(;;){
		int t = 0;
		int i = conectados - 1;
		//sem_wait(&semId_vectorCliente);
		bubbleSort(idJugadoresOrdenadosPorPuntos, conectados);
		//shellSort(idJugadoresOrdenadosPorPuntos);
		while(i >= 0 && t < MAX_JUGADORES_PANTALLA){
			contenedorNombre.x = tablaJugadores[idJugadoresOrdenadosPorPuntos[i]].nombre.x;
			contenedorNombre.y = tablaJugadores[idJugadoresOrdenadosPorPuntos[i]].nombre.y;
			contenedorNombre.h = tablaJugadores[idJugadoresOrdenadosPorPuntos[i]].nombre.h;
			contenedorNombre.w = tablaJugadores[idJugadoresOrdenadosPorPuntos[i]].nombre.w;

			contenedorPuntos.x = tablaJugadores[idJugadoresOrdenadosPorPuntos[i]].puntos.x;
			contenedorPuntos.y = tablaJugadores[idJugadoresOrdenadosPorPuntos[i]].puntos.y;
			contenedorPuntos.h = tablaJugadores[idJugadoresOrdenadosPorPuntos[i]].puntos.h;
			contenedorPuntos.w = tablaJugadores[idJugadoresOrdenadosPorPuntos[i]].puntos.w;

			strcpy(nombre,v_datosCliente[i].nombre);
			sprintf(puntos, "%d", v_datosCliente[i].puntos);

			nombreS = TTF_RenderText_Solid(fuente,nombre,color);
			puntosS = TTF_RenderText_Solid(fuente,puntos,color);

			SDL_mutexP(mtx);
			SDL_FillRect(ventana,&contenedorNombre,SDL_MapRGB(ventana->format,0,0,0));
			SDL_FillRect(ventana,&contenedorPuntos,SDL_MapRGB(ventana->format,0,0,0));
			SDL_BlitSurface(nombreS,NULL,ventana,&contenedorNombre);
			SDL_BlitSurface(puntosS,NULL,ventana,&contenedorPuntos);
			SDL_FreeSurface(nombreS);
			SDL_FreeSurface(puntosS);
			SDL_Flip(ventana);
			SDL_mutexV(mtx);
			i--;
			t++;
		}
		sleep(5);
		//sem_post(&semId_vectorCliente);
	}

}

void inicializarPosicionesTorneo(){
	/*Inicializo las posiciones del torneo*/
	int xInicio = 20, yInicio = 80, xPuntos = 450, yDesplazamiento = 30;
	int i = 0;
	for(i = 0; i < MAX_JUGADORES_PANTALLA; i++){
		tablaJugadores[i].nombre.x = xInicio;
		tablaJugadores[i].nombre.y = yInicio + (i * yDesplazamiento);
		tablaJugadores[i].nombre.h = 24;
		tablaJugadores[i].nombre.w = 150;
		tablaJugadores[i].puntos.x = xPuntos;
		tablaJugadores[i].puntos.y = yInicio + (i * yDesplazamiento);
		tablaJugadores[i].puntos.h = 24;
		tablaJugadores[i].puntos.w = 150;
	}
}


void bubbleSort (int *vec, long n) {
  long c, d, t,i;
 	for(i=0;i<n;i++){
		*(vec+i)=i;
	}
  for (c = 0 ; c < n; c++){
    for (d = c ; d < n; d++){
      if (v_datosCliente[vec[c]].puntos < v_datosCliente[vec[d]].puntos){
        /* Swapping */
        t = *(vec+c);
        *(vec+c) = *(vec+d);
        *(vec+(d)) = t;
      }
    }
  }
}

void shellSort (int *idJugadoresOrdenadosPorPuntos) {
	int h, i, j, k;
	struct s_datosCliente datos;
   	for (h = conectados; h /= 2;) {
        	for (i = h; i < conectados; i++) {
            		k = v_datosCliente[idJugadoresOrdenadosPorPuntos[i]].id;
					datos=v_datosCliente[idJugadoresOrdenadosPorPuntos[i]];
            		for (j = i; j >= h && datos.puntos < v_datosCliente[idJugadoresOrdenadosPorPuntos[j - h]].puntos; j -= h) {
                		idJugadoresOrdenadosPorPuntos[j] = v_datosCliente[idJugadoresOrdenadosPorPuntos[j - h]].id;
            		}
					idJugadoresOrdenadosPorPuntos[j] = datos.id;
        }
    }
}
