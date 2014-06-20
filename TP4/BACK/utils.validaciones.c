#include <stdio.h>

#include <stdlib.h>

#include <string.h>



/**

Las siguientes macros son mensajes de errores correspondientes a las posibles validaciones hechas.

*/

#define ERROR_CARACTER_RANGO "no es un caracter del rango"

#define ERROR_ENTERO "no es un entero"

#define ERROR_ENTERO_POSITIVO "no es un entero positivo"

#define ERROR_NUMERO_RANGO "no es un numero del rango"



/**

El siguiente metodo valida que el String dado contenga todos sus caracteres dentro del rango especificado.

Ej de uso:

	int f = esCaracterEnRango(argv[1], '5', '9');

	printf("esCaracterEnRango 5 a 9: es %d \n", f);

*/

int esCaracterEnRango(char *string, char min, char max) {

	int ret = 1, ii;

	int tam = strlen(string);

	for(ii = 0; ii < tam ; ii++){

		if(*string < min || *string > max) {

			ret = 0;

		}

		string++;

	}

	return ret;

}



/**

El siguiente metodo valida que el String dado contenga todos sus caracteres dentro del rango de posibles numeros enteros

Ej de uso:

	int s = esCaracterEntero(argv[1]);

	printf("esCaracterEntero : es %d \n", s);

*/

int esCaracterEntero(char *entero) {

	return esCaracterEnRango(entero, '0', '9');

}



/**

El siguiente metodo valida que el String dado contenga todos sus caracteres dentro del rango de posibles numeros enteros positivos

Ej de uso:

	int d = esCaracterEnteroPositivo(argv[1]);

	printf("esCaracterEnteroPositivo : es %d \n", d);

*/

int esCaracterEnteroPositivo(char *entero) {

	return esCaracterEntero(entero) && atoi(entero) >= 1;

}



/**

El siguiente metodo valida que el String dado contenga todos sus caracteres dentro del rango de posibles numeros enteros

y que se encuentre dentro del rango de enteros dados.

Ej de uso:

	int g = esNumeroEnRango(argv[1], "50", "90");

	printf("esNumeroEnRango 50 a 90: es %d \n", g);

*/

int esNumeroEnRango(char *entero, char *min, char *max) {

	int ret = 0;

	if(esCaracterEntero(entero) && esCaracterEntero(min) && esCaracterEntero(max)) {

		int nMin = atoi(min);

		int nMax = atoi(max);

		int num = atoi(entero);

		if (num >= nMin && num <= nMax) {

			ret = 1;

		}

	}

	return ret;

}