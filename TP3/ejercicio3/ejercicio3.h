#ifndef _ejercicio3_h
#define _ejercicio3_h

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <time.h>

// ------ Definición de funciones ------ //

void imprimirAyuda(void);
void *calcularPrimos(void *);

// ---- Fin definición de funciones ---- //

struct arg_struct {
    int desde;
    int hasta;
};

void imprimirAyuda(void){
	printf("AYUDA:\n");
	printf("\t./EJERCICIO3 [threads]\t Calcula los números primos con la cantidad de threads que se le indique\n");
	printf("\t./EJERCICIO3 -help \t Muesta la ayuda\n");
	printf("\t./EJERCICIO3 \t\t Calcula los números primos sin utilizar threads\n");
}

void *calcularPrimos(void *argumentos){

    int i,j,comienzo;
    int esPrimo=1;
    struct arg_struct *args = argumentos;
//    printf("\nInicializa thread. Inicio: %d - Fin: %d\n",args->desde,args->hasta);
    // Se calculan los numeros primos y se imprime cada uno en pantalla
    comienzo = args->desde;
    if(args->desde == 1){
        comienzo = args->desde+1;
    }
    for(i = comienzo; i <= args->hasta; i++){
        for(j=2;j<i;j++){
            if(i%j==0){
                esPrimo=0;
                break;
            }
        }
        if(esPrimo==1){
            printf("%d ",i);
        }
        esPrimo=1;
    }
    pthread_exit(NULL);
}

#endif