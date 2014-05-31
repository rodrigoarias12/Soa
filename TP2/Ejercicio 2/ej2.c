/*#####################################
#ej2
#Trabajo Práctico Nº2
#Ejercicio Número 2
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/
#include "ej2.h"



/**
*FUNCION DE AYUDA
*/
void help() {
	printf("Uso: ej2 <arg0> <arg1>\n");
	printf("\t<arg0>: es un entero positivo\n");
	printf("\t<arg1>: es un entero positivo o cero\n");
}
/**
*FUNCION DE MANEJO DE ERRORES
*/
void imprimirError(int codigoError){
	switch(codigoError){
		case 0: printf("La cantidad de parametros dados es incorrecta.\n"); break;
		case 1: printf("El primer parametro debe ser un entero positivo\n"); break;
		case 2: printf("El segundo parametro debe ser un entero positivo o cero\n"); break;
		case 3: printf("Error generando los procesos hijos\n"); break;
		case 4: printf("Error en la apertura del archivo.\n"); break;
		default: break;
	}
	if (codigoError < 3) {
		printf("Consulte la ayuda con -h para mas informacion\n");		
	}
	exit(1);
}
/*FUNCIONES DE MANEJO DE SENIALES*/
void levantaMuertos(int sig) {
	wait(NULL);
}
/*FIN FUNCIONES DE MANEJO DE SENIALES*/



int main(int argc, char *argv[]) {

	/*Manejo de Señales*/
	signal(SIGCHLD, levantaMuertos);
	/*Fin manejo de señales*/

	/*Variables*/
	int s, e;
	int nivel=1;
	int childPid;
	FILE *archivoSalida;
	/*Fin variables*

	/*Validación de Parametros*/
	if(argc == 2 && strcmp(argv[1],"-h") == 0) {
		help();
		exit(0);
	}
	if(argc != 3) {
		imprimirError(0);
	}

	s = atoi(argv[1]);
	e = atoi(argv[2]);

	if (s <= 0) {
		imprimirError(1);
	}

	if (strcmp(argv[2],"0") != 0 && e <= 0) {
		imprimirError(2);
	}
	/*Fin validación de Parametros*/
	
	//guarda el momento en el que la ejecución debería terminar	
	time_t tiempoFin = time(NULL) + s;
	
	while(difftime(tiempoFin, time(NULL)) >= 0) {		//loop while la diferencia de la hora actual y el tiempo de fin calculado sea  mayor a cero
		childPid=fork();
		if (childPid == -1) {
			imprimirError(3);
		} else {
			//luego de lanzar un hijo el proceso duerme e
			double dif=difftime(tiempoFin, time(NULL));			
			/*Si la diferencia entre el tiempo actual y el tiempo de finalización calculado es menor a "e", se agregan 10 seg más a la ejecución*/			
			dif<e ? sleep(dif+10) : sleep(e);
			if (childPid == 0) {
				nivel++; //incrementa el nivel de los hijos
			}
		}
	}

	archivoSalida = fopen("SALIDA.txt", "a");
	if(!archivoSalida) {
		imprimirError(4);
	}
	if(childPid==0) {
		printf("Nivel alcanzado: %d PID: %d\n", nivel, getpid());
		fprintf(archivoSalida, "Nivel alcanzado: %d PID: %d\n", nivel, getpid());
	}
	fclose(archivoSalida);

}

