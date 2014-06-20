/*#####################################

#Trabajo Práctico Nº3

#Ejercicio Número 4

#Arias, Rodrigo DNI: 34.712.865

#Culen, Fernando DNI: 35.229.859

#García Alves, Pablo DNI: 34.394.775

#Juffar, Sebastian DNI: 34.497.148

#Nogueiras, Jorge DNI: 34.670.613

#PRIMERA ENTREGA

#####################################*/

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <netdb.h>

#include <signal.h>



#include "utils.validaciones.c"

#define BUFFERSIZE 1000



void help();

void imprimirError(int codigo, const char *msg);