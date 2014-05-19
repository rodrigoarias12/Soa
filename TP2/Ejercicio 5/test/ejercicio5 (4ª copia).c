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

void imprimirError(int codigo){
	switch(codigo){
		case 0: printf("Cantidad mínima de parámetros requerida.\n"); break;
		case 1: printf("Error. Carpeta no Existente.\n"); break;
		case 3: printf("Error de Parámetros Incorrectos\n"); break;
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
		if(*entero < '0' || *entero > '9'){
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

int main(int argc, char *argv[]){

char rutaDirectorio[255];
char **parametrosDeFuncion;
int cantidadDeArchivosPorProceso;
int cantidadDeProcesosConcurrentes;
char comando[100];

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
	strcpy(comando, argv[6]);

	/*Parámetros si los hubiera*/
	if(argc > MINIMO_PARAM){
		int dif = abs((argc-1)-MINIMO_PARAM); 
		int i;

		parametrosDeFuncion = (char **)calloc(sizeof(char *), dif);
		if(!parametrosDeFuncion)
			imprimirError(5);
		for(i = 0; i<dif; i++){

			parametrosDeFuncion[i] = (char *) calloc(sizeof(char), strlen(argv[(MINIMO_PARAM+1) + i]));
			// if(parametrosDeFuncion[i] < 0);
			// 	imprimirError(5);

			strcpy(parametrosDeFuncion[i], argv[(MINIMO_PARAM+1) + i]);
			printf("Parámetro: %s\n", parametrosDeFuncion[i]);
		}

	}
	/*Fin validación de parámtros*/

	calcularCantidadArchivos(rutaDirectorio);
	printf("Total: %d\n", cantidadTotalDeArchivos);

}