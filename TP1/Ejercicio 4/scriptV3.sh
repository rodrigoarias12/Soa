#!/bin/bash
# Sistemas de Operativos 
# TP N:1
# Ejercicio:4
# Objetivo: utilizar el comando awk para manejo de matriz
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################

#Funciones
AYUDA="AYUDA\n=====\n\n
	   Modo de uso del script:\n
           1- \n\t
             ./script.sh -c n -o r \n\t
             -c Separador de columnas. Obligatorio.\n\t
             -o Operación a realizar: (Opcional).\n\t
	     La (n) es obligatorio: Indica separador de columnas. \n \t
             La (r) es opcional:Operación a realizar : \n\t
             \t trasp: indica trasponer la matriz \n\t
             \t prod: realizar el producto de la matriz por un escalar,      indicado en el  parámetro siguiente.\n
           
           2- \n\t
             ./script.sh -? \n\t
             Muestra la ayuda del script
"

function error (){
	if test $1 -eq 0; then
		echo -e "La cantidad de parámetros es incorrecta."
	elif test $1 -eq 1; then
		echo -e "El parámetro escalar no es un valor entero."
	elif test $1 -eq 2; then
		echo "Archivo no existente."
	elif test $1 -eq 3; then
		echo "Error en  parametro -o "
	elif test $1 -eq 4; then
		echo "El parámetro de prod debe ser acompañado por un escalar."
	fi
	echo -e $AYUDA
}

function es_entero (){
	        if [ $1 -eq $1 2> /dev/null ]; then			
			valido=1	
		else
                  echo "Error en uno de los parametros , no es un numero entero"
                   echo -e $AYUDA
                  exit 1         
		fi
}
#Fin de funciones

#Validar parametros
if test $# -lt 2 ; then 
   if [ $1 = "-?" ]; then
      echo -e $AYUDA
   else 
      error 0
   fi
   exit 1 
fi

solicita_trasponer=0
operacion=0
escalar=0
while getopts ":c:o:" opt ; do
case $opt in 
c)
    separador=$OPTARG
if test $# -eq 3 ; then 
     error 3
     exit 2
fi
;;
o)    
echo entro

if [ $4 = "prod" ] ;then 
   #validar q sea escalar y q exista
   if test $# -lt 5 ; then 
     error 4
     exit 2
 else 
     es_entero $5  
   fi 
    escalar=$5
    operacion=1

else if [ $4 = "trasp" ]; then
        solicita_trasponer=1
        operacion=1
     else 
       error 3
       exit 1
       
    fi
fi
;;
   esac
done
#Fin de validar parametros

#Ejecucion del enunciado 
#Ejecuto el comando awk y le paso los parametros 
awk -F $separador '{
    // NF es el numero de columnas de la matriz
    // NR es el numero de fila
    for (x = 0; x < NF; x++){
        entero=x+1
        columna=NR-1
        vector[x, columna] = $entero
    }
}
END{

    //Matriz Fila

    if(NR==1 && NF>1)
        printf("\n La matriz es fila")
    else
        printf("\n La matriz no es fila")

    //Matriz Columna
    if(NF==1&& NR>1)
        printf("\n La matriz es columna")
    else
        printf("\n La matriz no es columna")

    //Matriz Nula
    es_nula=1;
    for (x = 0; x < NF; x++){
        for (y = 0; y < NR; y++){
            if( vector[x, y]!=0)
                es_nula=0
        }
    }

    if(es_nula==1)
        printf("\n La matriz es nula")
    else
        printf("\n La matriz no es nula")

    //Matriz Identidad
    es_identidad=1;

    for (x = 0; x < NF; x++){
        for (y = 0; y < NR; y++){
            if((x==y && vector[x,y]!=1) || (x!=y && vector[x,y]!=0))
                es_identidad=0;
        }
    }
    if(es_identidad==1)
        printf("\n La matriz es identidad")
    else
        printf("\n la matriz no es identidad")

    //Matriz Diagonal
    es_diagonal=1;

    for (x = 0; x < NF; x++){
        for (y = 0; y < NR; y++){
            if((x==y && vector[x,y]==0) || (x!=y && vector[x,y]!=0))
                es_diagonal=0;
        }
    }
    if(es_diagonal==1)
        printf("\n La matriz es diagonal")
    else
        printf("\n la matriz no es diagonal")

    //Matriz Cuadrada y rectangular
    if(NR==NF)
        printf("\n La matriz es cuadrada y de orden %d\n",NR)
    else
        printf("\n La matriz es rectangular")

    // Verifico que se se pida una operacion
    if (1=='$operacion'){
        //verifico que se pida trasponer o producto
        if(1=='$solicita_trasponer'){
            printf("\nMATRIZ TRASPUESTA")
            for (x = 0; x < NF; x++){
                printf("\n")
                for (y = 0; y < NR; y++){
                    printf(" %d",vector[x,y])
                }
            }
        }else{
            //si no es trasponer entonces es un producto por un escalar
            printf("\nMATRIZ X UN ESCALAR")
            for (x = 0; x < NR; x++){
                printf("\n")
                for (y = 0; y < NF; y++){
                    numero=vector[y,x]*'$escalar'
                    printf(" %d" ,numero)
                }
            }
        }
    }
    printf("\n")
}
' /home/administrador/Escritorio/TP1/Ejercicio4/matriz
