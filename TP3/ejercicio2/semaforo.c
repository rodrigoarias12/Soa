#include "semaforo.h"
/*Creación de un semáforo*/
int crear_sem ( key_t clave, int valor_inicial ){
	int semId = semget(clave, 1, IPC_CREAT | IPC_EXCL | PERMISOS);

	if(semId == -1) return -1;

	semctl(semId, 0 , SETVAL , valor_inicial);
	return semId;
}

/*Función que decrementa el semáforo*/
void sem_P ( int semId ){
	struct sembuf op_P [] = {0, -1, 0};
	semop(semId, op_P, 1);
}	

/*Función que incrementa el semáforo*/
void sem_V ( int semId ){
	struct sembuf op_V [] = {0, 1, 0} ;
	semop(semId, op_V, 1);
}

int abre_sem (key_t clave){
	return semget(clave, 1, 0);
}
