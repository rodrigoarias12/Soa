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

#endif
