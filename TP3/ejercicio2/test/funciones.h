#ifndef _funciones_h
#define _funciones_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>
// #include "semaforo.h"

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
	float op1;
	float op2;
	int operador;
	float res;
} SMEM4;

/*Fin definición estructuras de datos*/

/*Variables globales*/
sem_t *sem1, *sem2, *sem3, *sem4;
key_t mem1, mem2, mem3, mem4, mem1_2;
int semId1, semId2, semId3, semId4, semId1_2;
int memId1, memId2, memId3, memId4, memId1_2;

char operaciones[] = {'+','-','*','/'};

SMEM1 *memo1;
SMEM2 *memo2;
SMEM3 *memo3;
SMEM4 *memo4;
SMEM4 *memo1_2;

/*Fin definición de variables globales*/


/*Definición de Funciones*/
int inicializar(void);
float operar(SMEM4 *);
void liberarRecursos(void);
void imprimirError(int);
void imprimirAyuda(void);
int generarNumAleatorio(int, int);


/*Fin de división de funciones*/

/* Función que inicializa los semáforos
*Proceso A = 1
*Proceso B = 0
*Proceso C = 0
*Proceso D = 0
*Proceso A_1 = 0 -Este semáforo es para la segunda acción de C-
*Valores iniciales de los semáforos*/
int inicializar(){
	printf("Inicializando los semáforos...\n");
	/*Defino las claves*/
	sem1= sem_open("/sem1", O_CREAT, 0644, 1);
	if(sem1==(sem_t *)-1)
		perror("Error creando semáforo 1");
	
	sem2= sem_open ("/sem2", O_CREAT, 0644, 0);
	if(sem2==(sem_t *)-1)
		perror("Error creando semáforo 2");
	
	sem3= sem_open ("/sem3", O_CREAT, 0644, 0);
	if(sem3==(sem_t *)-1)
		perror("Error creando semáforo 3");
	
	sem4= sem_open ("/sem4", O_CREAT, 0644, 0);
	if(sem4==(sem_t *)-1)
		perror("Error creando semáforo 4");

	printf("Inicializando la Memoria Compartida...\n");
	mem1 = ftok ("/bin/cp", 123);
	mem2 = ftok ("/bin/cp", 124);
	mem3 = ftok ("/bin/cp", 125);
	mem4 = ftok ("/bin/cp", 126);
	mem1_2 = ftok ("/bin/cp", 127);

	if(mem1 == -1 || mem2 == -1 || mem3 == -1 || mem4 == -1 || mem1_2 == -1){
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
	memId1_2 = shmget(mem1_2, sizeof(SMEM4), 0777 | IPC_CREAT);
	if(memId1_2 == -1)
		imprimirError(4);


	memo1 = shmat(memId1,0,0);
	memo2 = shmat(memId2,0,0);
	memo3 = shmat(memId3,0,0);
	memo4 = shmat(memId4,0,0);
	memo1_2 = shmat(memId1_2,0,0);

}

int generarNumAleatorio(int min, int max){
	srand(getpid());
	return rand() % (max - min + 1) + min ;
}

/*Funcion encargada de liberar todos los recursos asignados*/
void liberarRecursos(void){
	printf("Estoy liberando los recursos.\n");
	// if(cerrar_sem(semId1) == -1)
	// 	imprimirError(3);
	// if(cerrar_sem(semId2) == -1)
	// 	imprimirError(3);
	// if(cerrar_sem(semId3) == -1)
	// 	imprimirError(3);
	// if(cerrar_sem(semId4) == -1)
	// 	imprimirError(3);
	// if(cerrar_sem(semId1_2) == -1)
	// 	imprimirError(3);

	sem_close(sem1);
	sem_unlink("/sem1");
	sem_close(sem2);
	sem_unlink("/sem2");
	sem_close(sem3);
	sem_unlink("/sem3");
	sem_close(sem4);
	sem_unlink("/sem4");

	shmdt ((char *)memo1);
	shmctl (memId1, IPC_RMID, (struct shmid_ds *)NULL);

	shmdt ((char *)memo2);
	shmctl (memId2, IPC_RMID, (struct shmid_ds *)NULL);
	
	shmdt ((char *)memo3);
	shmctl (memId3, IPC_RMID, (struct shmid_ds *)NULL);
	
	shmdt ((char *)memo4);
	shmctl (memId4, IPC_RMID, (struct shmid_ds *)NULL);
	
	shmdt ((char *)memo1_2);
	shmctl (memId1_2, IPC_RMID, (struct shmid_ds *)NULL);

}
/*Imprime los mensajes de error y finaliza la aplicacion*/
void imprimirError(int cod){
	switch(cod){
		case 0: printf("No se pudo crear el hijo.\n");
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

float operar(SMEM4 * datos){
	
	switch(datos->operador){
		case 0: 
			datos->res = datos->op1 + datos->op2;
		break;
		case 1: 
			datos->res = datos->op1 - datos->op2;
		break;
		case 2: 
			datos->res = datos->op1 * datos->op2;
		break;
		case 3: 
			datos->res = datos->op1 / datos->op2;
		break;
	}

	return datos->res;
}


void imprimirAyuda(void){
	printf("Ejecución del programa:\n./EJERCICIO2 para la ejecución normal.\n./EJERCICIO2 -help para la ayuda.\nEn caso de ingresar otros parámetros estos no serán tenidos en cuenta, ya que el programa no los requiere.\n");	
	exit(0);
}

#endif