#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>

#define PERMISOS 0644

union semun {
	int val;                  /* valor para SETVAL */
	struct semid_ds *buf;     /* buffer para IPC_STAT, IPC_SET */
	unsigned short *array;    /* array para GETALL, SETALL */
	struct seminfo *__buf;    /* buffer para IPC_INFO */
};

/*Definición*/
int crear_sem ( key_t , int  );
void sem_P ( int );	
void sem_V ( int );
int abre_sem (key_t);

/*Creación de un semáforo*/
int crear_sem ( key_t clave, int valor_inicial ){
	union semun args;
	args.val = valor_inicial;
	int semId = semget(clave, 1, IPC_CREAT | PERMISOS); // | IPC_EXCL

	if(semId == -1) return -1;
	printf("Inicializo SEM con %d.\n", valor_inicial);
	
	semctl(semId, 0 , SETVAL , args);
	return semId;
}

/*Función que decrementa el semáforo*/
void sem_P ( int semId ){
	struct sembuf op_P [] = {0, -1, 0};
	semop(semId, op_P, 1);
	// printf("Pido semaforo %d.\n", semId);
}	

/*Función que incrementa el semáforo*/
void sem_V ( int semId ){
	struct sembuf op_V [] = {0, 1, 0};
	semop(semId, op_V, 1);
	// printf("Libero semaforo %d.\n", semId);
}

int abre_sem (key_t clave){
	return semget(clave, 1, 0);
}

int cerrar_sem(int semId){
	 if((semctl(semId, 0, IPC_RMID, 0)) == -1)
	 	return -1;
	 return 1;
}
