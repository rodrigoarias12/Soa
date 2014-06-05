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

#include "ejercicio5.h"

/**
*FUNCION DE AYUDA
*/
void help() {
	printf("Uso: ./EJERCICIO5 <arg0> -c <arg1> -p <arg2> <arg3> ...\n");
	printf("\t<arg0> (obligatorio): es el path del directorio que contiene el lote de archivos\n");
	printf("\t<arg1> (obligatorio): es un entero que indica la cantidad de archivos por proceso\n");
	printf("\t<arg2> (obligatorio): es un entero que indica la cantidad de procesos concurrentes\n");
	printf("\t<arg3> (obligatorio): es el comando a ejecutar con sus parámetros\n");
}
/**
*FUNCION DE MANEJO DE ERRORES
*/
void imprimirError(int codigo) {
	switch(codigo) {
		case 0: printf("Falta completar algun/os parametros.\n"); break;
		case 1: printf("Error. Carpeta no Existente.\n"); break;
		case 2: printf("Error. Los parametros ingresados no son validos.\n"); break;
		case 3: printf("No se pudo crear el proceso.\n"); break;
		case 4: printf("Error en la apertura del archivo.\n"); break;
		case 5: printf("No se pudo crear memoria.\n"); break;
		default: break;
	}
	if (codigo <= 2) {
		printf("Consulte la ayuda con -h para mas informacion\n");
	}
	exit(1);
}

/*Valida el directorio*/
int validarDirectorio(char *directorio) {
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

/*Valida que sea un entero*/
int validarEntero(char *entero) {
	int ret = 1, ii;
	int tam = strlen(entero);
	for(ii = 0; ii < tam ; ii++){
		if(*entero < '0' || *entero > '9') {
			ret = 0;
		}
		entero++;
	}
	return ret;
}

/*Obtiene la cantidad de archivos*/
void calcularCantidadArchivos(char * directorio) {
	DIR *dir;
	int ret = 1;
	dir = opendir(directorio);
	struct dirent *ent;
	/* Miramos que no haya error */
	while ((ent = readdir (dir)) != NULL) {
		if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
			cantidadTotalDeArchivos++;
	}
	closedir(dir);
}

/*Carga los archivos en el vector*/
char ** cargarArchivosEnVector(char *rDir) {
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

	while ((ent = readdir(dir)) != NULL) {
		if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ) {
			vArchivos[cont] = (char *) malloc(sizeof(char) *(strlen(rDir) + (strlen(ent->d_name)+1)));
			strcpy(vArchivos[cont], rDir);
			strcat(vArchivos[cont], ent->d_name);
			cont++;
		}
	}
	closedir(dir);
	return vArchivos;
}

/*Aviso la finalizacion*/
void avisarFinalizacion(int signal) {
	pid_t pidE = wait(NULL);
	printf("Finalizo un proceso\n");
	if (pidE > 0) {
		procesosEsperados--;
		procesosActivos--;
		procesosTotales--;
	}
}

int main(int argc, char *argv[]) {

	/*Manejo de Señales*/
	signal(SIGCHLD, avisarFinalizacion);
	/*Fin manejo de señales*/

	/*Variables*/
	char rutaDirectorio[255];
	char **parametrosDeFuncion;
	int cantidadDeArchivosPorProceso;
	int cantidadDeProcesosConcurrentes;
	char comando[100];
	int i = 0, j = 0;
	int contarArchivos = 0;
	int archivosProcesados = 0;
	char **parametrosAEnviar;
	int dif = 0;
	char ** archivos;
	int newDiff;
	/*Fin variables*/

	/*Validación de Parametros*/
	/*Cantidad mínima de Parámetros
	* MINIMO: 6, directorio, -c, máx de archivos, -p , máx de procesos, comando
	*/
	if(argc == 2 && strcmp(argv[1],"-h") == 0) {
		help();
		exit(0);
	}

	if(argc < MINIMO_PARAM)
		imprimirError(0);

	if(validarDirectorio(argv[1])) {
		strcpy(rutaDirectorio, argv[1]);
	}

	if(!strcmp(argv[2],"-c") && validarEntero(argv[3])) {
		cantidadDeArchivosPorProceso=atoi(argv[3]);
	} else {
		imprimirError(2);
	}

	if(!strcmp(argv[4],"-p") && validarEntero(argv[5])) {
		cantidadDeProcesosConcurrentes=atoi(argv[5]);
	} else {
		imprimirError(2);
	}

	/*Me guardo el comando a utilizar*/
	strcpy(comando, "/bin/"); //Chequear si es necesario
	strcat(comando, argv[6]);

	/*Parámetros si los hubiera*/
	if(argc > MINIMO_PARAM){
		dif = abs(argc-MINIMO_PARAM); 
		int i;

		parametrosDeFuncion = (char **)calloc(sizeof(char *), dif);
		if(!parametrosDeFuncion)
			imprimirError(5);
		for(i = 0; i<dif; i++) {
			parametrosDeFuncion[i] = (char *) calloc(sizeof(char), strlen(argv[MINIMO_PARAM + i])+1);
			strcpy(parametrosDeFuncion[i], argv[MINIMO_PARAM + i]);
		}
	}
	/*Fin validación de parámtros*/

	calcularCantidadArchivos(rutaDirectorio);

	if(cantidadTotalDeArchivos % cantidadDeArchivosPorProceso != 0) {
		procesosTotales = (cantidadTotalDeArchivos / cantidadDeArchivosPorProceso) + 1;
	} else {
		procesosTotales = (cantidadTotalDeArchivos / cantidadDeArchivosPorProceso);
	}
	procesosEsperados = procesosTotales;

	printf("Cantidad Total de Archivos a procesar: %d\n", cantidadTotalDeArchivos);
	printf("Cantidad Total de Procesos a ejecutar: %d\n", procesosTotales);
	printf("Cantidad de Archivos por Proceso indicado por el usuario: %d\n", cantidadDeArchivosPorProceso);

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
	/*Cargo los parametros de entrada para el comando*/
	for(j = 1; j < newDiff; j++) {
		parametrosAEnviar[j] = (char *) malloc(sizeof(char) * (strlen(parametrosDeFuncion[j-1])+1));
		strcpy(parametrosAEnviar[j], parametrosDeFuncion[j-1]);
	}

	while((procesosTotales > 0) && (archivosProcesados < cantidadTotalDeArchivos)) { //Procesos secuenciales
		while((procesosActivos < cantidadDeProcesosConcurrentes) && (archivosProcesados < cantidadTotalDeArchivos)){ //Procesos activos a la vez
			int t = 0, k;

			for(k = newDiff; k < (cantidadDeArchivosPorProceso + newDiff); k++) {
				free(parametrosAEnviar[k]);
				parametrosAEnviar[k] = NULL;
				if(archivosProcesados < cantidadTotalDeArchivos) {
					parametrosAEnviar[k] = (char *) malloc(sizeof(char) * (strlen(archivos[archivosProcesados])+1));
					strcpy(parametrosAEnviar[k], archivos[archivosProcesados]);
					archivosProcesados++;
					printf("Procesando archivo: %d\n", archivosProcesados);
				}	
			}

			printf("Se lanza un proceso nuevo\n");
			pids[t] = fork();
			if(pids[t] < 0) {
				imprimirError(3);
			} else if(pids[t] == 0) { //Hijo
				execve(comando, parametrosAEnviar, NULL);
				exit(0);
			} else { //Padre
			 	procesosActivos++;
			}
		}
	}
	while(procesosEsperados > 0) {
		pid_t pidE = wait(NULL);
		printf("Finalizo un proceso\n");
		if (pidE > 0) {
			procesosEsperados--;			
		}
	}

}