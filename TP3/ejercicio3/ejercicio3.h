#ifndef _ejercicio3_h
#define _ejercicio3_h

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <time.h>
# include <math.h>

// ------ Definición de funciones ------ //

void imprimirAyuda(void);
void *calcularPrimos(void *);

// ---- Fin definición de funciones ---- //

struct arg_struct {
    long desde;
    long hasta;
};

void imprimirAyuda(void){
	printf("AYUDA:\n");
	printf("\t./EJERCICIO3 [threads]\t Calcula los números primos con la cantidad de threads que se le indique\n");
	printf("\t./EJERCICIO3 -help \t Muesta la ayuda\n");
	printf("\t./EJERCICIO3 \t\t Calcula los números primos sin utilizar threads\n");
}

void *calcularPrimos(void *argumentos){

    long i,j,comienzo;
    int esPrimo=1;
    struct arg_struct *args = argumentos;
//    printf("\nInicializa thread. Inicio: %d - Fin: %d\n",args->desde,args->hasta);
    // Se calculan los numeros primos y se imprime cada uno en pantalla
    if(args->desde == 1){
      comienzo = args->desde + 1;
    }else{
      comienzo = args->desde;
    }
    
    for(i = comienzo; i <= args->hasta; i++){
        if(i%2 != 0 && i%3 != 0 && i%5 != 0 && i%7 != 0 && i%11 !=0){
	   for(j = 2;j < sqrt(i);j++){
               if(i%j==0){
                   esPrimo=0;
                   break;
               }
           }
           if(esPrimo==1){
               printf("%ld ",i);
           }
           esPrimo=1;
	}else if(i==2 || i==3 || i==5 || i==7 || i==11){
            printf("%ld ",i);
        }
    }
    pthread_exit(NULL);
}

#endif