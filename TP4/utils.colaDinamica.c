#include <stdio.h>
#include <stdlib.h>

/*Estructura de datos*/
struct tcola {
	void* clave;
	struct tcola *sig;
};

/*Definicion de las funciones basicas*/
void crear(struct tcola **cola);
int vacia(struct tcola **cola);
void encolar(struct tcola **cola, void* elem);
void desencolar(struct tcola **c1, void* *elem);
void vaciar(struct tcola *cola);

/*Implementacion de las funciones*/
void crear(struct tcola **cola) {
	*cola = NULL;
}

int vacia(struct tcola **cola) {
	return (*cola == NULL);
}

void encolar(struct tcola **cola, void* elem) {

	struct tcola *nuevo;

	nuevo = (struct tcola *) malloc(sizeof(struct tcola));
	nuevo->clave = elem;
	if (*cola == NULL) {
		nuevo->sig = nuevo;
	} else {
		nuevo->sig = (*cola)->sig;
		(*cola)->sig = nuevo;
	}
	(*cola) = nuevo;
}

void desencolar(struct tcola **c1, void* *elem) {

	struct tcola *aux;

	*elem = (*c1)->sig->clave;
	if ((*c1) == (*c1)->sig) {
		free(*c1);
		*c1 = NULL;
	} else {
		aux = (*c1)->sig;
		(*c1)->sig = aux->sig;
		free(aux);
	}
}

void vaciar(struct tcola *cola) {

	void* nodo;
	while(!vacia(&cola)) {
		desencolar(&cola, &nodo);
	}
}
