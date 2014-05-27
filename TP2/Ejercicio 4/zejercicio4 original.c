/*Se quiere simular el funcionamiento del comando grep, 
//por lo que se pide realizar un proceso que reciba como parámetros el patrón a buscar,
 //un filtro para los archivos que tiene que evaluar y un flag que indique si la búsqueda debe incluir subdirectorios. Ej.:
//$ c_grep /patron/ *.txt
$ c_grep /patron/ ./*.dat [-r]
El proceso principal debe generar un proceso por cada archivo a evaluar y 
estos procesos deben generar un archivo donde se indique el archivo leído,
el número de línea en que se encontró coincidencia y el contenido de la misma,
para corroborar que sea correcto. Si no se encuentran coincidencias no se deben generar los archivos.
El nombre de los archivos generados debe ser igual al pid del proceso que los genera.
$ c_grep TP *.txt
diccionario.txt
línea 15: Sistemas Operativos TP 2.
Tip: Investigar la biblioteca regex.h para evaluar los patrones de búsqueda.

Criterios de corrección:
Control Criticidad No se permite utilizar system()Obligatorio  
Verificar que sean archivos de texto Obligatorio 
Validar los parámetros Obligatorio 
Permitir patrones para filtrar la lista de archivos (como haría al usar ls) Obligatorio
No deben quedar procesos huérfanos o zombies. Obligatorio 
Makefile Obligatorio 
Generar un único archivo en base a los archivos generados por los procesos hijos. Opcional
*/
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_MATCHES 1 //The maximum number of matches allowed in a single string
#include<sys/types.h> // Se define el tipo de dato pid_t
#include<unistd.h> // Contiene las definiciones de fork() y sleep().
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

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

/* Genera una cadena de espacios, para dibujar el árbol */
char *generaPosStr(int niv);

/* Función principal, que cuenta archivos */
unsigned cuentaArchivos(char *ruta, int niv,char *);


int match(regex_t *pexp, char *sz) {
	regmatch_t matches[MAX_MATCHES]; //A list of the matches in the string (a list of 1)
	//Compare the string to the expression
	//regexec() returns 0 on match, otherwise REG_NOMATCH
	if (regexec(pexp, sz, MAX_MATCHES, matches, 0) == 0) {
		//printf("\"%s\" matches characters %d - %d\n", sz, matches[0].rm_so, matches[0].rm_eo);
return 1;
	} else {
return 0;
		//printf("\"%s\" does not match\n", sz);
	}
}
 int errorMessage(const char *msg) {
printf("%s\n",msg);
return 1; }

int main(int argc, char *argv[]) 
{
//declaracion de variables
int x;
char extencion[20]; // va contener la extencion   
char path[200]; // tiene el directorio
//tendria q validar si es archivo
printf("cantidad de archivos a procesar :%d \n",argc-2);


// VALIDACION DE PARAMETROS NECESARIOS
if(argc<3){
printf("ERROR EN LA CANTIDAD DE PARAMETROS \n ");
printf("1-$ ./c_grep /patron/ *.txt \n ");
printf("2-$ ./c_grep /patron/ ./*.dat [-r] <Busca en subdirectorios > \n");
exit(1);
}
// VALIDAR SI ES O NO RECURSIVO
if(argc>4)
{
if(!strcmp(argv[argc-1],"-r")){

size_t end = strlen(argv[2])-1; 
size_t end_path =strlen(argv[2])- (strlen(strrchr(argv[2],'/')+1));
printf("es recursivo  \n ");
exit(1);
  if(strchr(argv[2],'/'))
             {
 strncpy(path,argv[2],end_path);
path[end_path]='\0';
printf("antes de entrar soy el padre %s \n", argv[2]);
 printf("PATH : %s\n",path); 
} 
             strcpy(path,"./");
            
             strncpy(extencion , strrchr(argv[2],'.')+1, end);         
             printf("EXTENCION : %s\n",extencion);      
   fflush(stdin);

unsigned num;
  printf("Entrando en: %s\n", path);

   num=cuentaArchivos(path, 1,extencion);
printf("cantidad de archivos: %u\n",num);
exit(1);}
else
   {
  printf("paso mas de 4  \n ");
int x;
   //funcion simpre sin el -r
     for( x=2; x<argc; x++)
     {
     //Creo un hijo por cada archivo que tengo
     if(!fork())
       {
        norecur(argc,argv,x);
        return 0;
       }//fin if fork
     }//fin for archivos
     
     for (x=0;x<argc;x++)
     wait(NULL);

   }
return 0;
}
else
{
printf("es un arhivo solo \n ");
int x;
   //funcion simpre sin el -r
     for( x=2; x<argc; x++)
     {
     //Creo un hijo por cada archivo que tengo
     if(!fork())
       {
        norecur(argc,argv,x);
        return 0;
       }//fin if fork
     }//fin for archivos
     
     for (x=0;x<argc;x++)
     wait(NULL);

return 0;
}

return 0; 

}
int norecur(int argc, char *argv[],int x)
{
printf("path de ejecucion :%s \n",argv[x]);
 FILE *fp;//entrada
FILE *fps;//salida

char linea[500];
int nl=1;
 if((fp=fopen(argv[x], "r")) == NULL)
  {
   printf("Error Apertura archivo de entrada %s \n",argv[x]);
   scanf("%*c");
   exit(1);
  }

int rv;
int numeroaciertos=0;
regex_t exp; //Our compiled expression
	//1. CARGAR LA EXPRE
	rv = regcomp(&exp,argv[1], REG_EXTENDED);
	if (rv != 0) {
		printf("regcomp fallo con %d\n", rv);
	}
	//2. CORRER LA BUSQUEDA
	
     while(fgets(linea,sizeof(linea),fp))
 {
      if(  match(&exp,linea))
       {  
         if(!numeroaciertos)
         {
char *nombrearchivo;
sprintf(nombrearchivo,"./%d.txt",getpid());
 if((fps=fopen(nombrearchivo, "w")) == NULL)
  {
   printf("Error Apertura archivo de salida %s \n",nombrearchivo);
   scanf("%*c");
   exit(1);
  }
 fprintf(fps,"%s \n",argv[x]);
 fprintf(fps,"Linea %d : %s \n",nl,linea);
numeroaciertos++;
          }else{ 
          fprintf(fps,"Linea %d : %s\n",nl,linea);
            numeroaciertos++;}
        }
   
 nl++;
 }

if(numeroaciertos)
fclose(fps);

    //3. LIBERAR
	regfree(&exp);
//cierro el arhivo
fclose(fp);
printf("entre soy el hijo %d \n", getpid());
return 1;



}
void error(const char *s)
{
  /* perror() devuelve la cadena S y el error (en cadena de caracteres) que tenga errno */
  perror (s);
  exit(EXIT_FAILURE);
}

char *getFullName(char *ruta, struct dirent *ent)
{
  char *nombrecompleto;
  int tmp;

  tmp=strlen(ruta);
  nombrecompleto=malloc(tmp+strlen(ent->d_name)+2); /* Sumamos 2, por el \0 y la barra de directorios (/) no sabemos si falta */
  if (ruta[tmp-1]=='/')
    sprintf(nombrecompleto,"%s%s", ruta, ent->d_name);
  else
    sprintf(nombrecompleto,"%s/%s", ruta, ent->d_name);
  
  return nombrecompleto;
}

char *generaPosStr(int niv)
{
  int i;
  char *tmp=malloc(niv*2+1);    /* Dos espacios por nivel más terminador */
  for (i=0; i<niv*2; ++i)
    tmp[i]=' ';
  tmp[niv*2]='\0';
  return tmp;
}

unsigned cuentaArchivos(char *ruta, int niv,char *extencion)
{
  /* Con un puntero a DIR abriremos el directorio */
  DIR *dir;
  /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
  struct dirent *ent;
  unsigned numfiles=0;          /* Ficheros en el directorio actual */
  unsigned char tipo;       /* Tipo: fichero /directorio/enlace/etc */
  char *nombrecompleto;     /* Nombre completo del fichero */
  char *posstr;         /* Cadena usada para posicionarnos horizontalmente */
  dir = opendir (ruta);

  /* Miramos que no haya error */
  if (dir == NULL) 
    error("No puedo abrir el directorio");
  
  while ((ent = readdir (dir)) != NULL) 
    {
      if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
    {
      nombrecompleto=getFullName(ruta, ent);
      tipo=getFileType(nombrecompleto, ent);
      if (tipo!=DT_DIR)
        {   if(strchr(ent->d_name,'.'))
             {
             //size_t      begin  = strlen(ent->d_name)-3; 
             size_t      end    = strlen(ent->d_name)-1; 
             char substr[20];
           
             strncpy(substr , strchr(ent->d_name,'.')+1, end);         
             printf("NOMBRE : %s\n",substr);      
             if(strcmp(substr,extencion)==0) 
               ++numfiles;      
                
           }
        } 
        else           
        { posstr=generaPosStr(niv);
         // printf("Entrando en: %s\n",nombrecompleto);          
          numfiles+=cuentaArchivos(nombrecompleto, niv+1,extencion);
         
          /* Podemos poner las líneas que queramos */
   
          free(posstr);
        }
     free(nombrecompleto); 
    }//FIN IF PRINCIPAL

    }//FIN WHILE
  closedir (dir);  
  return numfiles;
}

unsigned char getFileType(char *nombre, struct dirent *ent)
{
  unsigned char tipo;

  tipo=ent->d_type;
  if (tipo==DT_UNKNOWN)
    {
      tipo=statFileType(nombre);
    }

  return tipo;
}

          

/* stat() vale para mucho más, pero sólo queremos el tipo ahora */
unsigned char statFileType(char *fname)
{
  struct stat sdata;

  /* Intentamos el stat() si no funciona, devolvemos tipo desconocido */
  if (stat(fname, &sdata)==-1)
    {
      return DT_UNKNOWN;
    }

  switch (sdata.st_mode & S_IFMT) 
    {
    case S_IFBLK:  return DT_BLK;
    case S_IFCHR:  return DT_CHR;
    case S_IFDIR:  return DT_DIR;
    case S_IFIFO:  return DT_FIFO;
    case S_IFLNK:  return DT_LNK;
    case S_IFREG:  return DT_REG;
    case S_IFSOCK: return DT_SOCK;
    default:       return DT_UNKNOWN;
    }
}