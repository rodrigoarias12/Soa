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

int cont = 10;

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

	for(i = 0; i< CANTIDAD_PROCESOS; i++ ){
		int hijo = fork();
		if(hijo < 0)
			imprimirError(0);
		
		if(hijo == 0){
			int j;
			switch(i){
				case 0:	
						sem_P(semId1);
						// printf("Valor del semaforo 1: %d\n",semctl(semId1,0,GETVAL,NULL));
						sem_P(semId1_2);
						memo1->op1 = generarNumAleatorio(1,10);
						printf("%.0f ", memo1->op1);
						usleep(300000);
						sem_V(semId1_2);
						// printf("Valor del semaforo 2: %d\n",semctl(semId2,0,GETVAL,NULL));
						sem_V(semId2);
						exit(0);
				break;
				case 1:
						// printf("Valor del semaforo 2: %d\n",semctl(semId2,0,GETVAL,NULL));
						sem_P(semId2);
						sem_P(semId1_2);
						memo2->op1 = memo1->op1;
						memo2->operador = generarNumAleatorio(0,3);
						sem_V(semId1_2);
						printf("%c ", operaciones[memo2->operador]);
						usleep(300000);
						sem_V(semId3);
						exit(0);
				break;
				case 2:
						sem_P(semId3);
						sem_P(semId1_2);
						memo3->op1 = memo2->op1;
						memo3->operador = memo2->operador;
						memo3->op2 = generarNumAleatorio(1,10);
						printf("%.0f ", memo3->op2);
						usleep(300000);
						sem_V(semId1_2);
						sem_V(semId4);
						exit(0);
				break;
				case 3:
						sem_P(semId4);
						sem_P(semId1_2);
						memo4->op1 = memo3->op1;
						memo4->operador = memo3->operador;
						memo4->op2 = memo3->op2;
						operar(memo4);
						printf("%c ", '=');
						usleep(300000);
						sem_V(semId1_2);
						// sem_V(semId1_2);
						printf("%.2f.\n", memo4->res);
						usleep(300000);
						sem_V(semId1);
						printf("Valor del semaforo 1: %d\n",semctl(semId1,0,GETVAL,NULL));
						printf("Valor del semaforo 2: %d\n",semctl(semId2,0,GETVAL,NULL));
						printf("Valor del semaforo 3: %d\n",semctl(semId3,0,GETVAL,NULL));
						printf("Valor del semaforo 4: %d\n",semctl(semId4,0,GETVAL,NULL));
						printf("Valor del semaforo 1_2: %d\n",semctl(semId1_2,0,GETVAL,NULL));
						exit(0);
				break;
			}
		}	
	}

	for(i = 0; i< CANTIDAD_PROCESOS; i++ ){
		wait(NULL);
	}

	liberarRecursos();
	exit(0);
}