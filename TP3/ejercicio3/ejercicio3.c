/*#####################################
#EJERCICIO3
#Trabajo Práctico Nº3
#Ejercicio Número 3
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/


# include "ejercicio3.h"



int main(int argc, char *argv[]){

    // Se declara e inicializa el tiempo
    time_t tiempoComienzo, tiempoFin;
    tiempoComienzo = time(NULL);

    long cantidadNumerosPrimos=10000000;
    long cantidadThreads = 0;

    if(argc != 1){
        //Se valida el ingreso del parámetro -help
        if(argc == 2 && strcmp("-help", argv[1]) == 0){
            imprimirAyuda();
            exit(0);
        }
        //Se valida la cantidad de parámetros
        if(argc != 2){
            printf("\nERROR: La cantidad de parámetros ingresados es incorrecta.\n\n");
            imprimirAyuda();
            exit(0);
        }
        cantidadThreads = atoi(argv[1]);
        // Se valida que no se ingresen caracteres como parámetro
        if(cantidadThreads <= 0 && strcmp("0", argv[1])){
            printf("\nERROR: El parámetro ingresado debe ser un número positivo. \n\n");
            imprimirAyuda();
            exit(0);
        }
        // La cantidad de threads ingresados no puede ser mayor 350
        if(cantidadThreads > 350){
            printf("\nERROR: La cantidad de threads no puede superar los 350 \n\n");
            imprimirAyuda();
            exit(0);
        }
	}

    double tiempoTotal;
    long i,j,countThreads;
    int esPrimo=1;
    int intervalo = 0;
    int inicio,fin,diferencia,hayMemoria;
    struct arg_struct *args;
    pthread_t *thread;

    // Se valida la cantidad de threads según si se ingresó o no un parámetro.
    // En el caso de que se ingrese parámetro se disparan todos los threads necesarios.
    // En el caso contrario se calculan los números primos sin utilizar threads.
    if(cantidadThreads > 0){
        // Se reserva memoria para la cantidad de threads que se van a utilizar.
        thread = malloc(sizeof(pthread_t)*cantidadThreads);
        args = (struct arg_struct *) malloc(sizeof(struct arg_struct)*cantidadThreads);
        if (thread == NULL || args == NULL){
           printf("Memoria insuficiente!\n");
           exit(0);
        }
        // Se calcula el intervalo que va a usar cada thread.
        intervalo = cantidadNumerosPrimos / cantidadThreads;
        printf("Cantidad de números por intervalo: %d \n",intervalo);
        // Se inicializa el inicio y fin del primer thread.
        inicio = 1;
        fin = intervalo;
        for(countThreads = 0; countThreads < cantidadThreads; countThreads++){
            // Se valida si es el ultimo thread.
            // En caso de serlo se le asigna el último número para evaluar a fin.
            if(countThreads == cantidadThreads-1){
                fin = cantidadNumerosPrimos;
            }
            args[countThreads].desde = inicio;
            args[countThreads].hasta = fin;
//            printf("Intervalo %d: %d - %d \n",countThreads,args[countThreads].desde,args[countThreads].hasta);
            if(pthread_create(&(thread[countThreads]), NULL, &calcularPrimos,(void*) &args[countThreads])!=0){
                printf("ERROR: fallo la creacion del hilo");
                exit(0);
            }            
            inicio = fin + 1;
            fin += intervalo;
        }

        for(countThreads = 0; countThreads < cantidadThreads; countThreads++){
            pthread_join(thread[countThreads],NULL);
        }
        free(thread);
        free(args);
    }else{
        //Se calculan los números primos y se imprime cada uno por pantalla
        for(i=2;i<=cantidadNumerosPrimos;i++){
            if(i%2 != 0 && i%3 != 0 && i%5 != 0 && i%7 != 0 && i%11 !=0){
                for(j=2;j<sqrt(i);j++){
                    if(i%j==0){
                        esPrimo=0;
                        break;
                    }
                }
                if(esPrimo==1){
                    printf("%ld ",i);
                }else{
                    esPrimo=1;
                }
            }else if(i==2 || i==3 || i==5 || i==7 || i==11){
                printf("%ld ",i);
            }
        }
    }

    printf("\n\nCantidad de threads utilizados: %ld \n",cantidadThreads);

    // Se calcula el tiempo total de ejecución
    tiempoTotal = time(NULL) - tiempoComienzo;
    printf("Tiempo de calculo: %.0f segundos \n",tiempoTotal);
    return 0;
}