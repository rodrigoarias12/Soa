#include "partida.h"

// SDL_Rect pajaroCoordenadas1,
// 	 pajaroCoordenadas2,
// 	 pajaroCoordenadas3;


void Finalizare(int)
{

	close(sockFileDescriptor);
	printf("Finalizado.\n");
	exit(0);
}

int main(int argc, char *argv[]) {
        fflush(NULL);
        signal(2,Finalizare);//si llega señal ctrl-c
        int nro_jug = 1;
	/*Variables*/
	SDL_Thread* acciones[2];
	SDL_Thread* moverJugadores[2];
	mtx = SDL_CreateMutex();
	
	int portNumber=53211; //Numero de puerto
	
	
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
        printf("conectado 1\n");
		//Reliza la conexión. Bloquea el proceso hasta que la conexión se realiza con exito
		clientSockFileDescriptor = accept(sockFileDescriptor, (struct sockaddr *) &cli_address, &clilen);
		if (clientSockFileDescriptor < 0 && flagTiempo) {
			imprimirError(0, "ERROR al aceptar conexiones por el puerto\n");
		} else if(clientSockFileDescriptor > 0) {
		    printf("conectado 5\n");
		    nro_jug=1;
		    send(clientSockFileDescriptor, &nro_jug, sizeof(int), 0);
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
comm_socket2 = v_datosCliente[1].socket;

// ejemplo de enviar al servidor de cliente 

// if(send(comm_socket, &nro_jug, sizeof(int), 0)){}
// nro_jug = 2;
// if(send(comm_socket2, &nro_jug, sizeof(int), 0)){}
// if(send(comm_socket, &duracionTorneo, sizeof(int), 0)){}
// if(send(comm_socket2, &duracionTorneo, sizeof(int), 0)){}



printf("Servidor Partida: Socket1%d %d        socket2%d   %d\n",comm_socket,v_datosCliente[0].id,comm_socket2,v_datosCliente[1].id);
// moverjugador deberia tener un receive activo todo el tiempo 
moverJugadores[0] = SDL_CreateThread(moverJugador1,NULL);
moverJugadores[1] = SDL_CreateThread(moverJugador2,NULL);


//falta la logica del cliente .
int contador = 0;
int recibirNivelTerminado;
int codigoPasoNivel;
	miPaquete.jugador1.coordenadas.x = 125;
	miPaquete.jugador1.coordenadas.y = 365;
	miPaquete.jugador2.coordenadas.x = 430;
	miPaquete.jugador2.coordenadas.y = 365;
while(contador < 2000){
// 	printf("Contador: %d\n", contador);
// 	fflush(NULL);
	miPaquete.codigoPaquete = 1;
// 	dibujarVentanas();
	dibujarVidrios(tipoEdificio);	
	miPaquete.ladrillos[0].x = 200;	
	miPaquete.ladrillos[0].y = 150;
	miPaquete.ladrillos[1].x = 180;	
	miPaquete.ladrillos[1].y = 200;
	miPaquete.ladrillos[2].x = 350;	
	miPaquete.ladrillos[2].y = 50;

		
	if(mov_paj1<640)mov_paj1+=3;else mov_paj1=-random()%100;
        if(mov_paj2<640)mov_paj2+=3;else mov_paj2=-random()%100-100;
        if(mov_paj3<640)mov_paj3+=3;else mov_paj3=-random()%100-50;     
	
	miPaquete.gaviotas[0].x = mov_paj1;	
	miPaquete.gaviotas[0].y = 316;
	miPaquete.gaviotas[1].x = mov_paj2;	
	miPaquete.gaviotas[1].y = 200;
	miPaquete.gaviotas[2].x = mov_paj3;	
	miPaquete.gaviotas[2].y = 100;

	if(contador == 50){
		printf("El contador llego a 500\n");
		codigoPasoNivel = 2;
		tipoEdificio = 1;
		send(comm_socket, &codigoPasoNivel, sizeof(t_paquete), 0);
		send(comm_socket2, &codigoPasoNivel, sizeof(t_paquete), 0);
	}

	if(send(comm_socket, &miPaquete, sizeof(t_paquete), 0)< 0){
		printf("Error en el socket: %d\n", comm_socket);
	}
	if(send(comm_socket2, &miPaquete, sizeof(t_paquete), 0)< 0){
		printf("Error en el socket: %d\n", comm_socket);
	}
	// send(comm_socket2, &miPaquete, sizeof(int), 0);
	usleep(75000);
	contador++;
//dentro de este while deberia refrescar todo el tiempo por lo que se necesita todo el tiempo enviar datos y hacer toda la magia del juego
//y deberia ser while hasta que se termine , uno salga , uno gane o no se si hay otras variables .
}
// 	send(comm_socket, &miPaquete, sizeof(int), 0);
	// send(comm_socket2, &miPaquete, sizeof(int), 0);

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
void dibujarVidrios(int completo){
    int i,x,y,comienzoX,comienzoY,distanciaEntreVidrios,totalVidrios;
    comienzoX = 140;
    comienzoY = 31;
    x = comienzoX;
    y = comienzoY;
    distanciaEntreVidrios = 78;
    totalVidrios = 36;
    if(completo == 1){
        totalVidrios = 40;
    }
    for(i=0;i<totalVidrios;i+=2){
        if((completo == 0 && (i==10 || i==20 || i==28 || i==36)) ||
           (completo == 1 && (i==10 || i==20 || i==30 || i==40))){
            x = comienzoX;
            switch(i){
                case 20: y+=110; break;
                case 28: if(completo == 0) y+=135; break;
                case 30: if(completo == 1) y+=135; break;
                default: y+=120; break;
            }
        }

        if(completo != 1 && (i == 24 || i == 32)){
            x+=distanciaEntreVidrios;
        }
        
        //TODO mandar paquete

	 	miPaquete.vidrios[i].coordenadas.x = x;
		miPaquete.vidrios[i].coordenadas.y = y;
		miPaquete.vidrios[i+1].coordenadas.x = x;
		miPaquete.vidrios[i+1].coordenadas.y = y+30;
		if(partidaPrimeraVez){
			miPaquete.vidrios[i].roto = (rand() % 2);
			miPaquete.vidrios[i+1].roto = (rand() % 2);
		}
        //dibujarSprite(vidrios[i], x, y,screen);
        //dibujarSprite(vidrios[i], x, y+30,screen);
        x+=distanciaEntreVidrios;
    }
    partidaPrimeraVez = 0;
}

int moverJugador2(void * n){
int recibir=0;
	while(recibir!=1000){
		    if(recv(comm_socket2,&recibir,sizeof(int),0)>=0)
		     {   switch(recibir){
		    case 10:printf("llego al servidor de partida el msj 10 \n");
					
				        	if((miPaquete.jugador2.coordenadas.y - 120) >= 0){
					                                                                                   
						  miPaquete.jugador2.coordenadas.y =  miPaquete.jugador2.coordenadas.y - 120;
						}
					
 				break;
		    case 20:			
                                                 if((  miPaquete.jugador2.coordenadas.y + 120) <= 450){
						 
						   miPaquete.jugador2.coordenadas.y = miPaquete.jugador2.coordenadas.y + 120; 
						
						}
					
                                 break;    
                    case 30:printf("llego al servidor de partida el msj 30 \n");
					
                                                if(( miPaquete.jugador2.coordenadas.x - 85) >= 120 ){
					  
						     miPaquete.jugador2.coordenadas.x =  miPaquete.jugador2.coordenadas.x -85;
			
						}   
 					
   				break;
			case 40:		printf("llego al servidor de partida el msj 40 \n");		
						if(( miPaquete.jugador2.coordenadas.x + 85) <= 420 ){
						
						   miPaquete.jugador2.coordenadas.x =  miPaquete.jugador2.coordenadas.x +85;
				
						} 
			case 50:
				printf("Termino el cambio de nivel. \n");
				break;
					
                case 1000: break;				
		
                case 1:	printf("llego al servidor de partida el msj %d del jugador 2 \n",recibir);
			fflush(stdout);      

		
			default:
					break;
                      
                            }
                                send(comm_socket2, &miPaquete, sizeof(t_paquete), 0);
                                send(comm_socket, &miPaquete, sizeof(t_paquete), 0);
		      }
                     else{printf("error en la conexion del cliente\n"); 
                          exit(0);
                          } 
                  }

	 	
}
int moverJugador1(void * n){
int recibir=0;
	while(recibir!=1000){
		    if(recv(comm_socket,&recibir,sizeof(int),0)>=0)
		     {   switch(recibir){
		    case 10:printf("llego al servidor de partida el msj 10 \n");
					
				        	if((miPaquete.jugador1.coordenadas.y - 120) >= 0){
					                                                                                   
						  miPaquete.jugador1.coordenadas.y =  miPaquete.jugador1.coordenadas.y - 120;
						}
					
 				break;
		    case 20:			
                                                 if((  miPaquete.jugador1.coordenadas.y + 120) <= 450){
						 
						   miPaquete.jugador1.coordenadas.y = miPaquete.jugador1.coordenadas.y + 120; 
						
						}
					
                                 break;    
                    case 30:printf("llego al servidor de partida el msj 30 \n");
					
                                                if(( miPaquete.jugador1.coordenadas.x - 85) >= 120 ){
					  
						     miPaquete.jugador1.coordenadas.x =  miPaquete.jugador1.coordenadas.x -85;
			
						}   
 					
   				break;
		case 40:		printf("llego al servidor de partida el msj 40 \n");		
						if(( miPaquete.jugador1.coordenadas.x + 85) <= 420 ){
						
						   miPaquete.jugador1.coordenadas.x =  miPaquete.jugador1.coordenadas.x +85;
				
						} 
					
                case 1000: break;				
		
                case 1:	printf("llego al servidor de partida el msj %d del jugador 2 \n",recibir);
			fflush(stdout);      

		
			default:
					break;
                      
                            }
                                send(comm_socket2, &miPaquete, sizeof(t_paquete), 0);
                                send(comm_socket, &miPaquete, sizeof(t_paquete), 0);
		      }
                     else{printf("error en la conexion del cliente"); 
                          exit(0);
                          } 
                  }

	 
}