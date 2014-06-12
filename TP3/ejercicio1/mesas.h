#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include<sys/types.h> // Se define el tipo de dato pid_t
#include<unistd.h> // Contiene las definiciones de fork() y sleep().
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <time.h>
//crear semaforos
#include <sys/sem.h>
#include <sys/ipc.h>

//validar parametros
int validacion(int argc ,char * argv[])
{	
int f=0;

 if(argc<2)
 {
  printf("Error en la cantidad de parametros\n");
  ayuda();
  exit(1);
 }

 if(strcmp(argv[1],"-?")==0)
 { 

  ayuda();
  exit(1);
 }

char *num=  (char *)malloc(sizeof(char) * 1024);
strcpy(num,argv[1]);
 while(*num) 
 { 
  if (!isdigit(*num)) 
     { 
        printf("ERROR: La cantidad de mesas debe ser un numero entero\n");
        ayuda();
        exit(1);
      } 
  num++; 
 } 

}

ayuda()
{
printf("AYUDA \n");
printf("El programa simula el funcionamiento de una fabrica de mesas,");
printf("se recibe como parametro la cantidad de mesas ,");
printf(" ");
printf("MODO DE USO \n");
printf("1-$ ./mesas [numero_de_mesas] \n");
printf("2-$ ./mesas -? \n");
printf("PARAMETROS \n");
printf("-? Para pedir la ayuda\n");
printf("[numero_de_mesas] La cantidad de mesas que se quieren crear con los procesos\n");
printf("ACLARACIONES: \n");
printf("1-Presione CTRL-C para terminar\n");
printf("2-Una vez terminadas las mesas los procesos anteriores se van cortando \n");

}
// Funcion para crear los semaforos
int crear_semaforo(k, n)
key_t k; int n;
{
	int semid, i;
	if((semid=semget(k,n,0)) != -1)
		semctl(semid, 0, IPC_RMID);
	if((semid=semget(k,n,IPC_CREAT | 0666)) != -1)
	{
		for(i=0; i<n; i++)
			V(semid, i);
	}
	else
		return(-1);
	return semid;
}
// Funcio P de semaforos
P(sem,n)
int sem, n;
{
struct sembuf sop;

sop.sem_num = n;
sop.sem_op = -1;
sop.sem_flg = 0;
semop(sem, &sop, 1);
}
//Funcion V de semaforos
V(sem,n)
int sem, n;
{
struct sembuf sop;

sop.sem_num = n;
sop.sem_op = 1;
sop.sem_flg = 0;
semop(sem, &sop, 1);
}

int imp_semval(sem)
int sem;
{
int semval;
union{
	int val;
	struct semid_ds *buf;
	ushort *array;
} semctl_arg;

semval = semctl(sem,0,GETVAL,semctl_arg);
if(semval<0)
	return (-1);
else
	return(semval);
}
// Funcion para inicializar semaforos
int inicializar_semaforo(sem,n,x) 
int sem,n,x;
{
union{
	int val;
	struct semid_ds *buf;
	ushort *array;
} semctl_arg;

semctl_arg.val=x;
if (semctl(sem,n,SETVAL,semctl_arg)<0)
	return (-1);
else
	return(0);
}
// Funcion para borrar semaforos
int borrar_semaforo(sem)
int sem;
{
int status;
status = semctl(sem,0,IPC_RMID);
if(status < 0)
	return(-1);
else
	return(status);
}
