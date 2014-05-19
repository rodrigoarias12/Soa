/*#####################################
#EJERCICIO5
#Trabajo Práctico Nº2
#Ejercicio Número 5
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

#define MINIMO_PARAM 6

/*Variables Globales*/
int cantidadTotalDeArchivos = 0;
int procesosTotales = 0;
int procesosActivos = 0;
int archivosProcesados = 0;
int procesosEsperados = 0;

int *pids;

void imprimirError(int codigo){
	switch(codigo){
		case 0: printf("Cantidad mínima de parámetros requerida.\n"); break;
		case 1: printf("Error. Carpeta no Existente.\n"); break;
		case 2: printf("Error. Los parametros no ingresados no son validos.\n"); break;
		case 3: printf("No se pudo crear el proceso.\n"); break;
		case 4: printf("Error en la apertura del archivo.\n"); break;
		case 5: printf("No se pudo crear memoria.\n"); break;
		default: break;
	}
	exit(1);
}

int validarDirectorio(char *directorio){
  DIR *dir;  
  int ret = 1;
  dir = opendir(directorio);
  /* Miramos que no haya error */
  if (dir == NULL){ 
      imprimirError(1);
   	  ret = 0;
    }
  closedir(dir);
  return ret;
}

int validarEntero(char *entero){
	int ret = 1, ii;
	int tam = strlen(entero);
	for(ii = 0; ii < tam ; ii++){
		if(*entero < '1' || *entero > '9'){
			ret = 0;
		}
		entero++;
	}
	return ret;
}

void calcularCantidadArchivos(char * directorio){
	DIR *dir;  
	int ret = 1;
	dir = opendir(directorio);
	struct dirent *ent;
	  /* Miramos que no haya error */
	while ((ent = readdir (dir)) != NULL) 
    {
    	if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
			cantidadTotalDeArchivos++;
    }
	  closedir(dir);
}

char ** cargarArchivosEnVector(char *rDir){
  DIR *dir;
  struct dirent *ent;
  dir = opendir(rDir);
  int cont = 0;
  char **vArchivos;
  /* Miramos que no haya error */
  if (dir == NULL) 
    imprimirError(0);
  
/*Almaceno espacio para archivos en vector*/
vArchivos = (char **) calloc(sizeof(char *), cantidadTotalDeArchivos);

  while ((ent = readdir (dir)) != NULL) 
    {
    	if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){
    	    	vArchivos[cont] = (char *) malloc(sizeof(char) *(strlen(rDir) + (strlen(ent->d_name)+1)));
    	    	strcpy(vArchivos[cont], rDir);
    	    	strcat(vArchivos[cont],ent->d_name);
    			printf("Archivo %s cargado. \n", vArchivos[cont]);
    			cont++;
    	}
    }
  closedir(dir);
  return vArchivos;
 }

void avisarFinalizacion(int signal){
	printf("Finalizó : %d.\n", wait(NULL));
	procesosEsperados--;
	procesosActivos--;
	procesosTotales--;
}

int main(int argc, char *argv[]){

char rutaDirectorio[255];
char **parametrosDeFuncion;
int cantidadDeArchivosPorProceso;
int cantidadDeProcesosConcurrentes;
char comando[100];
int i = 0, j = 0;
int contarArchivos = 0;
char **parametrosAEnviar;
int dif;
char ** archivos;
int newDiff;

/*Validación de Parámetros de entrada*/
/*Cantidad mínima de Parámetros
* MINIMO: 6, directorio, -c, máx de archivos, -p , máx de procesos, comando
*/

	if(argc < MINIMO_PARAM)
		imprimirError(0);

	if(validarDirectorio(argv[1])){
		strcpy(rutaDirectorio,argv[1]);
	}

	if(!strcmp(argv[2],"-c") && validarEntero(argv[3])){
		cantidadDeArchivosPorProceso=atoi(argv[3]);
	}
	else{
		imprimirError(2);
	}

	if(!strcmp(argv[4],"-p") && validarEntero(argv[5])){
		cantidadDeProcesosConcurrentes=atoi(argv[5]);
	}else{
		imprimirError(2);
	}

	/*Me guardo el comando a utilizar*/
	strcpy(comando, "/bin/"); //Chequear si es necesario
	strcat(comando, argv[6]);

	/*Parámetros si los hubiera*/
	if(argc > MINIMO_PARAM){
		dif = abs((argc-1)-MINIMO_PARAM); 
		int i;

		parametrosDeFuncion = (char **)calloc(sizeof(char *), dif);
		if(!parametrosDeFuncion)
			imprimirError(5);
		for(i = 0; i<dif; i++){

			parametrosDeFuncion[i] = (char *) calloc(sizeof(char), strlen(argv[(MINIMO_PARAM+1) + i])+1);
			// if(parametrosDeFuncion[i] == NULL);
			//  	imprimirError(5);

			strcpy(parametrosDeFuncion[i], argv[(MINIMO_PARAM+1) + i]);
		}

	}

	
	/*Fin validación de parámtros*/

	calcularCantidadArchivos(rutaDirectorio);

	if(cantidadTotalDeArchivos % cantidadDeArchivosPorProceso != 0){
		procesosTotales = (cantidadTotalDeArchivos / cantidadDeArchivosPorProceso) + 1;
	}else{
		procesosTotales = (cantidadTotalDeArchivos / cantidadDeArchivosPorProceso);
	}

	procesosEsperados = procesosTotales;
	pids = (int *) calloc(sizeof(int), procesosTotales);

	printf("Cantidad Total de Archivos: %d\n", cantidadTotalDeArchivos);
	printf("Cantidad Total de Procesos: %d\n", procesosTotales);
	printf("Cantidad de Archivos por Proceso: %d\n", cantidadDeArchivosPorProceso);
	/*Los cargo en un vector para poder manejarlos mejor*/
	archivos = cargarArchivosEnVector(rutaDirectorio);

	/*Asigno memoria para almacenar los pids*/
	pids = (int *) calloc(sizeof(int), procesosTotales);
	
	/*Actualizo los parámetros a enviar*/
	/*comando-parámetros-NULL*/
	newDiff = dif+1;
	parametrosAEnviar = (char **) calloc(sizeof(char *), newDiff + cantidadDeArchivosPorProceso + 1);

	parametrosAEnviar[0] = (char *) malloc(sizeof(char) * (strlen(comando)+1));
	strcpy(parametrosAEnviar[0], comando);

	for(j = 1; j < newDiff; j++){
		parametrosAEnviar[j] = (char *) malloc(sizeof(char) * (strlen(parametrosDeFuncion[j-1])+1));
		strcpy(parametrosAEnviar[j], parametrosDeFuncion[j-1]);
		printf("Parámetros: %s\n", parametrosAEnviar[j-1]);
	}

	while((procesosTotales > 0) && (archivosProcesados < cantidadTotalDeArchivos)){ //Procesos secuenciales
		while((procesosActivos < cantidadDeProcesosConcurrentes) && (archivosProcesados < cantidadTotalDeArchivos)){ //Procesos activos a la vez
			int t = 0, k;

			for(k = newDiff; k < (cantidadDeArchivosPorProceso + newDiff); k++){
				if(archivosProcesados <= cantidadTotalDeArchivos){
					parametrosAEnviar[k] = (char *) malloc(sizeof(char) * (strlen(archivos[archivosProcesados])+1));
					strcpy(parametrosAEnviar[k], archivos[archivosProcesados]);
					archivosProcesados++;
					printf("Archivos Procesados: %d\n", archivosProcesados);
					printf("Archivos: %s\n", parametrosAEnviar[k]);
				}	
			}

			parametrosAEnviar[cantidadDeArchivosPorProceso+newDiff] = NULL;

			pids[t] = fork();
			if(pids[t] < 0)
				imprimirError(3);

			if(pids[t] == 0){ //Hijo
				printf("Soy el Hijo con pid: %d\n", getpid());
				execve(comando,parametrosAEnviar,NULL);
				// kill(SIGCHLD, getppid());
				exit(0);
			}else{ //Padre
				printf("Soy el Padre con PID: %d\n", getpid());
				signal(SIGCHLD, avisarFinalizacion);
			 	procesosActivos++;
			}
		}
	}
	printf("Procesos esperados: %d\n", procesosEsperados);
	while(procesosEsperados > 0){
		pid_t pidE = wait(NULL);
		printf("Finalizó : %d.\n", pidE);
		procesosEsperados--;
	}

}