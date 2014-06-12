/*#####################################
#c_grep
#Trabajo Práctico Nº2
#Ejercicio Número 4
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#1 REENTREGA
#####################################*/
#include "c_grep.h"


int main(int argc, char *argv[])
{
//declaracion de variables



// VALIDACION DE PARAMETROS NECESARIOS
    if(argc<3 || argc<2) {
        if(argc==2 && (!strcmp(argv[1],"-?" ) || !strcmp(argv[1],"-help" ) ) )
        {
            errorMessage();
            exit(1);
        }
        else
            printf("ERROR EN LA CANTIDAD DE PARAMETROS \n ");
        errorMessage();
        exit(1);
    }



// VALIDAR SI ES O NO RECURSIVO

    if(!strcmp(argv[argc-1],"-r"))
    {
        procesarrecursivo(argc,argv);
        return 0;
    }
    else
    {
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

        for (x=0; x<argc; x++)
            wait(NULL);

        return 0;
    }

    return 0;

}
//procesa archivos no recursivos
int norecur(int argc, char *argv[],int x)
{
//variables archivo
    FILE *fp;//entrada
    FILE *fps;//salida
    char linea[500];
    char nombrearchivo[500];
    int nl=1;
    int numeroaciertos=0;
//variables busqueda
    int rv;
    regex_t exp; //Our compiled expression

//Validar texto
    char extencion[2000]; // va contener la extencion
    char path[2000]; // tiene el directorio
    unsigned num;
    size_t end = strlen(argv[2])-1;
    size_t end_path;
    int bandera=0;
    if(strchr(argv[2],'/'))
    {
        end_path =strlen(argv[2])- (strlen(strrchr(argv[2],'/')+1));
        strncpy(path,argv[2],end_path);
        path[end_path]='\0';
    }
    else {
        sprintf(path,"./%s",argv[x]);
        bandera=1;
    }


    strncpy(extencion , strrchr(argv[2],'.')+1, end);

    //verifico apertura
    if((fp=fopen(argv[x], "r")) == NULL)
    {
        printf("El directorio no es valido o no existe : %s \n",argv[x]);
        errorMessage();
        exit(1);
    }

    //1. CARGAR LA EXPRE
    rv = regcomp(&exp,argv[1], REG_EXTENDED);
    if (rv != 0) {
        printf("regcomp fallo con %d\n", rv);
    }
    //2. CORRER LA BUSQUEDA

    while(fgets(linea,sizeof(linea),fp))
    {
        if(match(&exp,linea))
        {   if(!numeroaciertos)
            {
                sprintf(nombrearchivo,"./%d.txt",getpid());
                if((fps=fopen(nombrearchivo, "w")) == NULL)
                {
                    printf("El directorio no es valido o no existe : %s \n",nombrearchivo);
                    errorMessage();
                    exit(1);
                }
                fprintf(fps,"%s \n",argv[x]);
                fprintf(fps,"Linea %d : %s \n",nl,linea);
                printf("%s \n",argv[x]);
                printf("Linea %d : %s \n",nl,linea);
                numeroaciertos++;
            }
            else {
                fprintf(fps,"Linea %d : %s\n",nl,linea);
                printf("Linea %d : %s\n",nl,linea);
                numeroaciertos++;
            }
        }//fin del if de acierto

        nl++;
    }//fin de recorrer lineas

//si hay aciertos cierro el archivo q se abrio para escribir
    if(numeroaciertos)
        fclose(fps);

    //3. LIBERAR
    regfree(&exp);
//cierro el arhivo de lectura
    fclose(fp);

    return 1;
}
//separa la extencion y el path de los arhivos recursivos
int procesarrecursivo(int argc, char *argv[])
{
    FILE *archivos;
    char extencion[20000]; // va contener la extencion
    char path[20000]; // tiene el directorio
    char *nombreArchivo;
    unsigned num;
    size_t end = strlen(argv[2])-1;
    size_t end_path =strlen(argv[2])- (strlen(strrchr(argv[2],'/')+1));
    int x=0;
    if(strchr(argv[2],'/'))
    {
        strncpy(path,argv[2],end_path);
        path[end_path]='\0';
    }

    strncpy(extencion , strrchr(argv[2],'.')+1, end);

    nombreArchivo = basename(argv[2]);
    num=cuentaArchivos(path, 1,extencion,argv[1],nombreArchivo,argc);

    char linea[50000];
    char nombrecompleto[50000];
    //verifico apertura
    if((archivos=fopen("/tmp/temporaldenombres", "r")) == NULL)
    {
        printf("El directorio no es valido o no existe \n");
        errorMessage();
        exit(1);
    }
    while(fgets(linea,sizeof(linea),archivos))
    {
        linea[strlen(linea)-1] ='\0';
        if(!fork()) {
            recur(linea,argv[1]);
            return 0;
        }

    }
    for(x=0; x<num; x++)
        wait(NULL);


    fclose(archivos);
    remove("/tmp/temporaldenombres");
}

unsigned cuentaArchivos(char *ruta, int niv,char *extencion,char*patron,char *nombreArchivo, int cantidadArgumentos)
{
    /* Con un puntero a DIR abriremos el directorio */
    DIR *dir;
    /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
    struct dirent *ent;
    unsigned numfiles=0;          /* Ficheros en el directorio actual */
    unsigned char tipo;       /* Tipo: fichero /directorio/enlace/etc */
    char *nombrecompleto;     /* Nombre completo del fichero */
    char *posstr;         /* Cadena usada para posicionarnos horizontalmente */
    int validarTodos = 0;
    dir = opendir (ruta);
    /* Miramos que no haya error */
    if (dir == NULL) {
        error("No puedo abrir el directorio");
        int errorMessage() ;
    }
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
                    char substr[2000];

                    strncpy(substr , strchr(ent->d_name,'.')+1, end);

                    if(nombreArchivo[0] == '*' && nombreArchivo[1] == '.'){
                        validarTodos = 1;
                    }

                    if(cantidadArgumentos >= 5){
                        validarTodos = 1;
                    }

                    if(strcmp(substr,extencion)==0)
                    {
                        if(validarTodos == 1){
                            printf("\n\n Validar Todos\n\n");
                            grabar_nombre(nombrecompleto);
                            ++numfiles;
                        }else if(!strcmp(nombreArchivo,ent->d_name)){
                            printf("\n\n No Validar Todos\n\n");
                            grabar_nombre(nombrecompleto);
                            ++numfiles;
                        }
                    }
                }
            }
            else
            {   posstr=generaPosStr(niv);
                // printf("Entrando en: %s\n",nombrecompleto);
                numfiles+=cuentaArchivos(nombrecompleto, niv+1,extencion,patron,nombreArchivo,cantidadArgumentos);

                /* Podemos poner las líneas que queramos */

                free(posstr);
            }
            free(nombrecompleto);
        }//FIN IF PRINCIPAL

    }//FIN WHILE
    closedir (dir);

    return numfiles;
}

//procesa archivos recursivos
int recur(char *argv,char *patron)
{
//variables archivo
    FILE *fp;//entrada
    FILE *fps;//salida
    char linea[5000];
    int nl=1;
    int numeroaciertos=0;
    char nombrearchivo[5000];
//variables busqueda
    int rv;
    regex_t exp; //Our compiled expression
    //verifico apertura
    if((fp=fopen(argv, "r")) == NULL)
    {
        printf("El directorio no es valido o no existe %s \n",argv);
        int errorMessage();
        exit(1);
    }
    //1. CARGAR LA EXPRE
    rv = regcomp(&exp,patron, REG_EXTENDED);//esta mal cambiar busqueda
    if (rv != 0) {
        printf("regcomp fallo con %d\n", rv);
        int errorMessage() ;
    }
    //2. CORRER LA BUSQUEDA

    while(fgets(linea,sizeof(linea),fp))
    {
        if(match(&exp,linea))
        {
            if(!numeroaciertos)
            {

                sprintf(nombrearchivo,"./%d.txt",getpid());
                if((fps=fopen(nombrearchivo,"w")) == NULL)
                {
                    printf("El directorio no es valido o no existe  %s \n",nombrearchivo);
                    errorMessage();
                    exit(1);
                }
                fprintf(fps,"%s \n",argv);
                fprintf(fps,"Linea %d : %s \n",nl,linea);
                printf("%s \n",argv);
                printf("Linea %d : %s \n",nl,linea);
                numeroaciertos++;
            }
            else {

                fprintf(fps,"Linea %d : %s\n",nl,linea);
                printf("Linea %d : %s\n",nl,linea);
                numeroaciertos++;
            }
        }//fin del if de acierto

        nl++;
    }//fin de recorrer lineas
    if(numeroaciertos) {
//si hay aciertos cierro el archivo q se abrio para escrib
        fclose(fps);
    }
//cierro el arhivo de lectura
    fclose(fp);

    return 1;
}
//
void grabar_nombre(char * nombre)
{
    FILE *fp1;//salida

    if((fp1=fopen("/tmp/temporaldenombres","a+")) == NULL)
    {
        printf("El directorio no es valido o no existe \n");
        int errorMessage();
        exit(1);
    }
    fprintf(fp1,"%s\n",nombre);
    fclose(fp1);
}

//tipo de archivo
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

//////////////////// funciones para validar archivos  //////////////////////////

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
    case S_IFBLK:
        return DT_BLK;
    case S_IFCHR:
        return DT_CHR;
    case S_IFDIR:
        return DT_DIR;
    case S_IFIFO:
        return DT_FIFO;
    case S_IFLNK:
        return DT_LNK;
    case S_IFREG:
        return DT_REG;
    case S_IFSOCK:
        return DT_SOCK;
    default:
        return DT_UNKNOWN;
    }
}
//funcion para manejo de errores
void error(const char *s)
{
    /* perror() devuelve la cadena S y el error (en cadena de caracteres) que tenga errno */
    perror (s);
    exit(EXIT_FAILURE);
}

//nombre completo
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
/* Se encarga de hacer la busqueda */
int match(regex_t *pexp, char *sz) {
    regmatch_t matches[1];
    if (regexec(pexp, sz, 1, matches, 0) == 0) {

        return 1;
    } else {
        return 0;
    }
}
int errorMessage() {

    printf("AYUDA \n");
    printf("El programa simula el funcionamiento del comando grep,");
    printf("se recibe como parámetros el patrón a buscar,");
    printf("un filtro para los archivos que tiene que evaluar\n");
    printf("OPCIONAL indique si la búsqueda debe incluir subdirectorios\n");

    printf("MODO DE USO \n");
    printf("1-$ ./c_grep /patron/ *.txt \n");
    printf("2-$ ./c_grep /patron/ ./*.dat [-r] <Busca en subdirectorios \n");
    printf("PARAMETROS \n");

    printf("-? Para pedir la ayuda\n");
    printf("/patron/ Se pude utilizar patrones de busqueda para la busqueda en los archivos\n");
    printf("path/*.dat  Direccion de archivo/s a buscar con su extencion  \n");
    printf("[-r] <Busca en subdirectorios  \n");
    printf("ACLARACIONES: \n");

    printf("1-El programa solo busca en archivos de texto\n");
    printf("2-Si se encuentran aciertos se crea un archivo  que contiene : el archivo leído,");
    printf("el número de línea en que se encontró coincidencia y el contenido de la misma");
    printf("\n3-Estos archivos con los aciertos se alojaran en el direcotorio donde se ejecuto el comando.\n");

    return 1;
}