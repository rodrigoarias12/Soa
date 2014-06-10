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
		else{
			printf("No es necesario especificar ningún parámetro. Para consultar ejecución indique como parámetro -help\n");
			imprimirAyuda();
		}		
	}
	/*Inicializo los semáforos y las memorias compartidas*/
	inicializar();
	/*Creación de los procesos que van a realizar la tarea*/
	for(i = 0; i< CANTIDAD_PROCESOS; i++ ){
		int hijo = fork();
		if(hijo < 0)
			imprimirError(0);
		
		if(hijo == 0){
			int j;
			switch(i){
				case 0:	
						srand(getpid());
					for(j = 0; j < 10 ; j++){
						/*Genero el primer valor, lo imprimo y lo guardo en la memoria 1*/
						sem_P(semId1);
						
						memo1->op1 = rand() % (10) + 1 ;
						printf("%.0f ", memo1->op1);
						fflush(NULL);
						usleep(300000);
						
						sem_V(semId2);
						
						/*Espero que el proceso número 4 haga el cálculo para poder imprimir.*/
						sem_P(semId1);
						/*Imprimo el valor del resultado de la cuenta*/
						printf("%.2f.\n", memo4->res);
						fflush(NULL);
						usleep(300000);
						sem_V(semId1);
						
					}
						exit(0);
				break;
				case 1:
						srand(getpid());
					for(j = 0; j < 10 ; j++){
						/*Genero la operación aleatoria y la guardo en la memoria 2 junto con el valor anterior*/
						sem_P(semId2);
						
						memo2->op1 = memo1->op1;
						memo2->operador = rand() % 4 ;
						
						printf("%c ", operaciones[memo2->operador]);
						fflush(NULL);
						usleep(300000);

						sem_V(semId3);
					}
						exit(0);
				break;
				case 2:
						srand(getpid());
					for(j = 0; j < 10 ; j++){
						/*Genero el segundo valor y lo guardo en la memoria 3 junto con los datos de la memoria 2*/
						sem_P(semId3);
						
						memo3->op1 = memo2->op1;
						memo3->operador = memo2->operador;
						memo3->op2 = memo1->op1 = rand() % (10) + 1 ;
						printf("%.0f ", memo3->op2);
						fflush(NULL);
						usleep(300000);
						
						sem_V(semId4);
					}
						exit(0);
				break;
				case 3:
						srand(getpid());
					for(j = 0; j < 10 ; j++){
						/*Hago la cuenta, lo guardo en memoria 4 y le paso el turno al primer proceso*/
						sem_P(semId4);
						
						memo4->res = operar(memo3);
						printf("%c ", '=');
						fflush(NULL);
						usleep(300000);
						
						sem_V(semId1);
					}
						exit(0);
				break;
			}
		}	
	}
	/*Espero por todos los procesos*/
	for(i = 0; i< CANTIDAD_PROCESOS; i++ ){
		wait(NULL);
	}
	/*Libero los recursos pedidos (Semáforos y Segmentos de Memoria Compartida*/
	liberarRecursos();
	exit(0);
}