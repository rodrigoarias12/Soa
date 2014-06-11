#ifndef _funciones_h
#define _funciones_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include "semaforo.h"

#define ITERACIONES 10
#define CANTIDAD_PROCESOS 4

/*Definición de estructuras de datos*/
/*Proceso 1*/
typedef struct {
	float op1;
} SMEM1;

/*Proceso 2*/
typedef struct {
	float op1;
	int operador;
} SMEM2;

/*Proceso 3*/
typedef struct {
	float op1;
	float op2;
	int operador;
} SMEM3;

/*Proceso 4*/
typedef struct {
	float res;
} SMEM4;

/*Fin definición estructuras de datos*/

/*Variables globales*/
key_t sem1, sem2, sem3, sem4;
key_t mem1, mem2, mem3, mem4;
int semId1, semId2, semId3, semId4;
int memId1, memId2, memId3, memId4;

char operaciones[] = {'+','-','*','/'};

/*Segmentos de memoria a utilizar*/
SMEM1 *memo1;
SMEM2 *memo2;
SMEM3 *memo3;
SMEM4 *memo4;

/*Fin definición de variables globales*/


/*Definición de Funciones*/
int inicializar(void);
float operar(SMEM3 *);
void liberarRecursos(void);
void imprimirError(int);
void imprimirAyuda(void);
/*Fin definición de funciones*/

/*Fin de división de funciones*/

/* Función que inicializa los semáforos y los segmentos
*de memoria compartida
*Proceso A = 1
*Proceso B = 0
*Proceso C = 0
*Proceso D = 0
*Valores iniciales de los semáforos*/
int inicializar(){
	printf("***************************\nInicializando los semáforos.\n");
	fflush(NULL);
	/*Defino las claves*/
	sem1 = ftok("/bin/ls", 29);
	sem2 = ftok("/bin/ls", 30);
	sem3 = ftok("/bin/ls", 31);
	sem4 = ftok("/bin/ls", 32);

	if(sem1 == -1 || sem2 == -1 || sem3 == -1 || sem4 == -1){
		imprimirError(1);
	}

	semId1 = crear_sem(sem1, 1);
	if(semId1 == -1) 
		imprimirError(2);

	semId2 = crear_sem(sem2, 0);
	if(semId2 == -1) 
		imprimirError(2);
	
	semId3 = crear_sem(sem3, 0);
	if(semId3 == -1) 
		imprimirError(2);
	
	semId4 = crear_sem(sem4, 0);
	if(semId4 == -1) 
		imprimirError(2);

	printf("Inicializando Memoria Compartida.\n***************************\n");
	fflush(NULL);
	mem1 = ftok ("/bin/cp", 123);
	mem2 = ftok ("/bin/cp", 124);
	mem3 = ftok ("/bin/cp", 125);
	mem4 = ftok ("/bin/cp", 126);

	if(mem1 == -1 || mem2 == -1 || mem3 == -1 || mem4 == -1){
		imprimirError(1);
	}

	memId1 = shmget(mem1, sizeof(SMEM1), 0777 | IPC_CREAT);
	if(memId1 == -1)
		imprimirError(4);
	memId2 = shmget(mem2, sizeof(SMEM2), 0777 | IPC_CREAT);
	if(memId2 == -1)
		imprimirError(4);
	memId3 = shmget(mem3, sizeof(SMEM3), 0777 | IPC_CREAT);
	if(memId3 == -1)
		imprimirError(4);
	memId4 = shmget(mem4, sizeof(SMEM4), 0777 | IPC_CREAT);
	if(memId4 == -1)
		imprimirError(4);


	memo1 = shmat(memId1,0,0);
	memo2 = shmat(memId2,0,0);
	memo3 = shmat(memId3,0,0);
	memo4 = shmat(memId4,0,0);

}

/*Funcion encargada de liberar todos los recursos asignados*/
void liberarRecursos(void){
	printf("***********************\nLiberando los recursos.\n***********************\n");
	fflush(NULL);
	if(cerrar_sem(semId1) == -1)
		imprimirError(3);
	if(cerrar_sem(semId2) == -1)
		imprimirError(3);
	if(cerrar_sem(semId3) == -1)
		imprimirError(3);
	if(cerrar_sem(semId4) == -1)
		imprimirError(3);

	shmdt ((char *)memo1);
	shmctl (memId1, IPC_RMID, (struct shmid_ds *)NULL);

	shmdt ((char *)memo2);
	shmctl (memId2, IPC_RMID, (struct shmid_ds *)NULL);
	
	shmdt ((char *)memo3);
	shmctl (memId3, IPC_RMID, (struct shmid_ds *)NULL);
	
	shmdt ((char *)memo4);
	shmctl (memId4, IPC_RMID, (struct shmid_ds *)NULL);

}
/*Imprime los mensajes de error y finaliza la aplicacion*/
void imprimirError(int cod){
	switch(cod){
		case 0: printf("No se pudo crear el proceso.\n");
		break;
		case 1: printf("Error al crear la clave.\n");
		break;
		case 2: printf("Error al crear los semáforos.\n");
		break;
		case 3: printf("Error al cerrar los semáforos.\n");
		break;
		case 4: printf("Error al crear memoria Compartida.\n");
		break;
	}
	exit(0);
}

/*	
	Función que realiza las operaciones seleccionadas.
	A utilizar por el proceso 4.
	Operadores:
		0 SUMA
		1 RESTA
		2 MULTIPLICACION
		3 DIVISION
*/

float operar(SMEM3 * datos){
	
	switch(datos->operador){
		case 0: 
			return (datos->op1 + datos->op2);
		break;
		case 1: 
			return (datos->op1 - datos->op2);
		break;
		case 2: 
			return (datos->op1 * datos->op2);
		break;
		case 3: 
			return (datos->op1 / datos->op2);
		break;
	}
}

/*Imprime una ayuda en caso de necesitarla.*/
void imprimirAyuda(void){
	printf("Ejecución del programa:\n./EJERCICIO2 para la ejecución normal.\n./EJERCICIO2 -help para la ayuda.\nEn caso de ingresar otros parámetros estos no serán tenidos en cuenta, ya que el programa no los requiere.\n");	
	exit(0);
}

#endif