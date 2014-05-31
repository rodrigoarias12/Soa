/*#####################################
#EJERCICIO3
#Trabajo Práctico Nº2
#Ejercicio Número 3
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF 1024
#define LECTURA 0
#define ESCRITURA 1

int hijos[10000];

/*Variables Globales*/

int CANTIDAD_HIJOS = 0;
unsigned long long HASH_TOTAL = 0;
int FIN_NORMAL = 1;

/*Fin variables globales*/

/**
*FUNCIÓN DE MANEJO DE ERRORES
*/
void imprimirError(int codigoError){
	switch(codigoError){
		case 0: printf("La carpera indicada no existe.\n"); break;
		case 1: printf("Error. Carpeta no Existente.\n"); break;
		case 3: printf("No se pudo crear el hijo.\n"); break;
		case 4: printf("Error en la apertura del archivo.\n"); break;
		case 5: printf("No se pudo crear memoria.\n"); break;
		default: break;
	}
	FIN_NORMAL=0;
	exit(1);
}
/*Calcular Cantidad de Hijos necesarios*/
int calcularHijosNecesarios(char *ruta)
{
  DIR *dir;
  struct dirent *ent;
  unsigned numfiles=0;
  dir = opendir(ruta);

  /* Miramos que no haya error */
  if (dir == NULL) 
    imprimirError(0);
  
  while ((ent = readdir (dir)) != NULL) 
    {
    	if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
			numfiles++;
    }
  closedir(dir);
  
  return numfiles;
}

/**
*FUNCIÓN DE FINALIZACIÓN DE HIJOS
*/

void finalizarHijos(int sig){
	int i = 0;
	for(i = 0; i< CANTIDAD_HIJOS; i++){
		kill(hijos[i], SIGUSR1);
	}
}

/**
*Informa de los hijos terminados
*/
void salidaHijos(int sig){
	printf("Proceso Hijo Terminado. Calculo de Hash Abortado.\n");
	exit(0);
}
/*Función principal*/
int main(int argc, char * argv[]){
	/*Manejo de Señales*/
     signal(SIGINT, finalizarHijos);
    /*Fin manejo de señales*/
	
	int i = 0, pd[2];
	FILE *archivoResultadoFinal;
	CANTIDAD_HIJOS = calcularHijosNecesarios(argv[1]);
	pid_t pids[CANTIDAD_HIJOS];

	// hijos = (int *)malloc(CANTIDAD_HIJOS * sizeof(int));
	if(hijos<0)
		imprimirError(4);

	DIR *directorio;
	struct dirent *archivo;
	
	/*Validación de Parámetros*/
	if(argc == 1){
	 	imprimirError(0);
	}

	if(argc == 2){
		directorio = opendir(argv[1]);
	}

	if(!directorio){
		imprimirError(1);
	}
	/*Fin validación de Parámetros*/

	printf("Estoy en el padre. MI PID: %d\n\n", getpid());

	/*Creo el Pipe*/
	pipe(pd);

	while ((archivo = readdir (directorio)) != NULL){

		if((strcmp(archivo->d_name, ".") != 0) && (strcmp(archivo->d_name, "..") != 0)){			
			
			hijos[i] = fork();
			if(hijos[i]==0){
				printf("Calculando Hash de %s en: %d.\n", archivo->d_name, getpid());

				FILE *salida;
				int fichero = 0;
				struct stat fileStat;
				unsigned long long ftamanio = 0;
				char nombreSalida[10];
				sprintf(nombreSalida,"%d", getpid());

				char pathCompleto[255];
				strcat(pathCompleto,argv[1]);
				strcat(pathCompleto,archivo->d_name);
				/*Tratamiento de señales [HIJOS]*/
				signal(SIGUSR1, salidaHijos);

				fichero=open(pathCompleto, O_RDONLY);
				if(fichero < 0)
					return(0);
				  if (fstat(fichero,&fileStat) == 0){

				  		close(fichero);
				  		ftamanio = fileStat.st_size;
				    	close(pd[LECTURA]);
						write(pd[ESCRITURA], &ftamanio, sizeof(ftamanio));

				    	salida = fopen(nombreSalida, "wt");
				    	if(salida < 0)
				    		return(0);
				    	if(salida){
				    		fprintf(salida, "Archivo: %s\n", archivo->d_name);
				    		fprintf(salida, "%llu bytes." , ftamanio);
				    	}else{
				    		imprimirError(4);
				    	}
				    	fclose(salida);
				 }else{
				 	imprimirError(4);

				 }

				 /*Espero unos segundos para poder enviar señal de SIGINT al padre.*/
				 sleep(2);
				 return(0);
			}else{
				i++;
			}
		}
	 	
	} 
	
	closedir(directorio);
	/*Espero que finalicen todos hijos*/
	int ii;
	for(ii = 0; ii< CANTIDAD_HIJOS; ii++){
		if(FIN_NORMAL){
			unsigned long long hashParcial;
			wait(NULL);
			close(pd[ESCRITURA]);
			read(pd[LECTURA], &hashParcial, sizeof(hashParcial));
			HASH_TOTAL+=hashParcial;
			printf("Hash parcial: %llu.\n", HASH_TOTAL);
		}
	}

	/*Informo el hash total y lo guardo en un archivo de texto.*/

	printf("HASH TOTAL: %llu bytes.\n", HASH_TOTAL);

		archivoResultadoFinal = fopen("RES_FINAL.txt", "w");
		if(archivoResultadoFinal){
			fprintf(archivoResultadoFinal, "%llu" , HASH_TOTAL);
		}else{
			imprimirError(4);
		}

	fclose(archivoResultadoFinal);
	
	return(0);
}
