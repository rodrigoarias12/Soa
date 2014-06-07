#ifndef _funciones_h
#define _funciones_h

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define ITERACIONES 10
#define CANTIDAD_PROCESOS 4

/*Definición de estructuras de datos*/
/*Proceso 1*/
typedef struct {
	int op1;
} SMEM1;

/*Proceso 2*/
typedef struct {
	int op1;
	int operador;
} SMEM2;

/*Proceso 3*/
typedef struct {
	int op1;
	int op2;
	int operador;
} SMEM3;

/*Proceso 4*/
typedef struct {
	int op1;
	int op2;
	int operador;
	float res;
} SMEM4;

/*Fin definición estructuras de datos*/

/*Definición de Funciones*/
int inicializar(void);
float operar(SMEM4 *);
void liberarRecursos(void);
void imprimirError(int);
void imprimirAyuda(void);

/*Fin de división de funciones*/

/*Funcion encargada de liberar todos los recursos asignados*/
void liberarRecursos(void){
	printf("Estoy liberando los recursos.\n");
}
/*Imprime los mensajes de error y finaliza la aplicacion*/
void imprimirError(int cod){
	switch(cod){
		case 0: 
			printf("No se pudo crear el hijo.\n");
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


/* Función que inicializa los semáforos
*Proceso A = 1
*Proceso B = 0
*Proceso C = 0
*Proceso D = 0
*Proceso A_1 = 0 -Este semáforo es para la segunda acción de C-
*Valores iniciales de los semáforos*/
int inicializar(){
	printf("Estoy inicializando las variables.\n");
}

void imprimirAyuda(void){
	printf("Esta es la ayuda.\n");	
	exit(0);
}

#endif