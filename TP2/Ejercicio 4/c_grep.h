/*#####################################
#c_grep
#Trabajo Práctico Nº2
#Ejercicio Número 4
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include<sys/types.h> // Se define el tipo de dato pid_t
#include<unistd.h> // Contiene las definiciones de fork() y sleep().
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

/* Hace el procesamiento de archivos */
int recur(char *path,char *patron);

/* Se encarga de crear los procesos */
int procesarrecursivo(int argc, char *argv[]);

//procesa los no recursivos
int norecur(int argc, char *argv[],int x);

/* Función para devolver un error en caso de que ocurra */
void error(const char *s);

/* Calculamos el tamaño del archivo */
long fileSize(char *fname);

/* Sacamos el tipo de archivo haciendo un stat(), es como el stat de la línea de comandos */
unsigned char statFileType(char *fname);

/* Intenta sacar el tipo de archivo del ent */
unsigned char getFileType(char *ruta, struct dirent *ent);

/* Obtiene el nombre del fichero con la ruta completa */
char *getFullName(char *ruta, struct dirent *ent);

/* Grabo directorio de archivos*/
void grabar_nombre(char * nombre);

/* Genera una cadena de espacios, para dibujar el árbol */
char *generaPosStr(int niv);

/* Función principal, que cuenta archivos */
unsigned cuentaArchivos(char *ruta, int niv,char *,char *);

/* Se encarga de hacer la busqueda */
int match(regex_t *pexp, char *sz);
/* Mensaje de error */
 int errorMessage();

