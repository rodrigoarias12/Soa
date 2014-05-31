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

#define MAX_BUF 1024
#define LECTURA 0
#define ESCRITURA 1

void imprimirError(int);
int calcularHijosNecesarios(char *);
void finalizarHijos(int);
void salidaHijos(int);