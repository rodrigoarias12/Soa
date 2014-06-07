/*#####################################
#EJERCICIO2
#Trabajo Práctico Nº3
#Ejercicio Número 2
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/
#include "funciones.h"

void main(int argc, char *argv[]){

	if(argc == 2){
		if(!strcmp("-help", argv[1]))
			imprimirAyuda();
		else
			printf("No es necesario especificar ningún parámetro. Para consultar ejecución indique como parámetro -help\n");		
	}

	atexit(liberarRecursos);
	int i = 0;
	for(i = 0; i< CANTIDAD_PROCESOS; i++ ){
		int hijo = fork();
		if(hijo < 0)
			imprimirError(0);
		
		if(hijo == 0){
			switch(i){
				case 0: 
					printf("Estoy en el hijo A\n");
					exit(0);
				break;
				case 1:
					printf("Estoy en el hijo B\n");
					exit(0);
				break;
				case 2:
					printf("Estoy en el hijo C\n");
					exit(0);
				break;
				case 3:
					printf("Estoy en el hijo D\n");
					exit(0);
				break;
			}
		}	
	}

	for(i = 0; i< CANTIDAD_PROCESOS; i++ ){
		wait(NULL);
	}
}