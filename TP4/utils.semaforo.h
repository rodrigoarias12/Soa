#include <sys/ipc.h>
#include <sys/sem.h>

#define PERMISOS 0777

/*Estructura de semaforos*/
union semun {
	int val;
	struct semid_ds *buf; 
	unsigned short *array;    
	struct seminfo *__buf;  
};

/*Definicion de las funciones basicas para semaforos*/
int crear_sem ( key_t , int  );
void sem_P ( int );	
void sem_V ( int );
int cerrar_sem ( int );

/*Creacion de un semaforo*/
int crear_sem ( key_t clave, int valor_inicial ){
	union semun args;
	args.val = valor_inicial;
	int semId = semget(clave, 1, IPC_CREAT | PERMISOS); // | IPC_EXCL

	if(semId == -1) return -1;
	
	semctl(semId, 0 , SETVAL , args);
	return semId;
}

/*Funcion que decrementa el semaforo*/
void sem_P ( int semId ){
	struct sembuf op_P [] = {0, -1, 0};
	semop(semId, op_P, 1);
}	

/*Funcion que incrementa el semaforo*/
void sem_V ( int semId ){
	struct sembuf op_V [] = {0, 1, 0};
	semop(semId, op_V, 1);
}

/*Funcion que cierra el semaforo creado*/
int cerrar_sem(int semId){
	 if((semctl(semId, 0, IPC_RMID, 0)) == -1)
	 	return -1;
	 return 1;
}
