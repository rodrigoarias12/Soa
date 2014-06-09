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

	int i = 0;

	if(argc == 2){
		if(!strcmp("-help", argv[1]))
			imprimirAyuda();
		else
			printf("No es necesario especificar ningún parámetro. Para consultar ejecución indique como parámetro -help\n");		
	}
	/*Defino la función que libera los recursos*/
	// atexit(liberarRecursos);
	/*Inicializo los semáforos y las memorias compartidas*/
	inicializar();
	
		int hijo = fork();
		if(hijo < 0)
			imprimirError(0);
		
		if(hijo == 0){
					printf("Estoy en el hijo A. Elijo Operadondo\n");
					sem_wait(sem1);
					printf("semId1: %d\n", semId1);
					memo1->op1 = generarNumAleatorio(1,10);
					printf("%.0f ", memo1->op1);
					usleep(300000);
					sem_post(sem2);
					// printf("Estoy esperando...\n");
					sem_wait(sem1);
					// printf("semId1_2: %d\n", semId1_2);
					printf("%.2f.\n", memo4->res);
					usleep(500000);
		}else{
			int hijo = fork();
				if(hijo < 0)
					imprimirError(0);
			if(hijo == 0){
				sem_wait(sem2);
					// printf("Estoy en el hijo B. Elijo Operador\n");
					// printf("semId2: %d\n", semId2);
					memo2->op1 = memo1->op1;
					memo2->operador = generarNumAleatorio(0,3);
					
					printf("%c ", operaciones[memo2->operador]);
					usleep(500000);
					sem_post(sem3);
			}else{
				int hijo = fork();
				if(hijo < 0)
					imprimirError(0);
				if(hijo == 0){
					printf("El valor de i es: %d\n", i);
					// printf("Estoy en el hijo C.Elijo Otro operando.\n");
					// int op2 = generarNumAleatorio(1,10);
					sem_wait(sem3);
					// printf("semId3: %d\n", semId3);
					memo3->op1 = memo2->op1;
					memo3->operador = memo2->operador;
					memo3->op2 = generarNumAleatorio(1,10);
					printf("%.0f ", memo3->op2);
					usleep(500000);
					sem_post(sem4);
				}else{
						int hijo = fork();
					if(hijo < 0)
						imprimirError(0);
					if(hijo == 0){
						printf("El valor de i es: %d\n", i);
					// printf("Estoy en el hijo D.Estoy resolviendo\n");
					sem_wait(sem4);
					// printf("semId4make: %d\n", semId4);
					memo4->op1 = memo3->op1;
					memo4->operador = memo3->operador;
					memo4->op2 = memo3->op2;
					operar(memo4);
					printf("%c ", '=');
					usleep(500000);
					sem_post(sem1);
					}
				}
			}
		}
	

	for(i = 0; i< CANTIDAD_PROCESOS; i++ ){
		wait(NULL);
	}

	liberarRecursos();
	exit(0);	
}