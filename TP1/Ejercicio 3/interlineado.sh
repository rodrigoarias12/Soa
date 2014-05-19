#####################################
#interlineado.sh
#Trabajo Práctico Nº1
#Ejercicio Número 3
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################

#!/bin/bash

#Funciones
AYUDA="AYUDA\n=====\n\n
	  	Modo de uso del script:\n1- ./interlineado.sh -i n -f [path de archivo]\n
	   La (n) es opcional: Indica la cantidad de líneas en blanco a insertar. Si no se indica
	   por defecto es uno (1).\n
	   2- ./interlineado.sh -d -f [path del archivo]\n
	   con la opcion d elimina todas las líneas vacías en el archivo.\n
	   3- .interlineado.sh -?\nMuestra la ayuda del script"

function error (){
	if test $1 -eq 0; then
		echo -e "La cantidad de parámetros es incorrecta."
	elif test $1 -eq 1; then
		echo -e "El parámetro de i no es un valor entero."
	elif test $1 -eq 2; then
		echo "Archivo no existente."
	elif test $1 -eq 3; then
		echo "Opciones i y d no válidas al mismo tiempo."
	elif test $1 -eq 4; then
		echo "El parámetro de i debe ser mayor o igual que 1."
	fi
	echo -e $AYUDA
}

function es_entero (){
	
	VARIABLE=`echo $1 | grep -o [A-Za-z]`
	if [ "$VARIABLE" != "" ] ; then
		error 1
		exit 1
	fi
}

function es_archivo_valido (){

	if ! test -e $1 ; then
		error 2
		exit 1
	fi
}
#Fin de funciones

#Chequeo de parámetros mínimos requeridos
if test $# -lt 1; then
	error 0
	exit 1
fi
#Fin chequeo

#Manejo de opciones de ingreso
OPCIONI=0    #NO INSERTADA
OPCIOND=0    #NO INSERTADA

while getopts ":df:i:?" opt; do
	
  case $opt in
    f)  echo "Parámetro $OPTARG insertado para -f"
		OPCIONF=$OPTARG
	;;
    i) 
		if  [[ "$OPTARG" = "-f" || "$OPTARG" = "-d" || "$OPTARG" = "-?" ]] ; then
			echo "Parámetro para i = 1 por defecto."
			OPCIONI=1
			OPTIND=OPTIND-1
		else
			echo "Parámetro $OPTARG insertado para i"
			OPCIONI=$OPTARG
			if test $OPCIONI -lt 1 ; then
				error 4
				exit 1
			fi 
  		fi
	;;
    d) echo "Opcion d insertada"
		OPCIOND=1
		echo $OPTARG
	;;
    :) echo "Opción -$OPTARG necesita un parámetro"
	   echo -e $AYUDA
	   exit 1
  	;;
  	?) echo -e $AYUDA
	   exit 1
	;;
  esac
done

#Fin manejo de opciones de ingreso

#Chequeo de tipo de parámetros
es_entero $OPCIONI

es_archivo_valido $OPCIONF

for RutaCompleta in $OPCIONF ;
do
	NombreArchivo="${RutaCompleta##*/}"
	LargoRuta="${#RutaCompleta} - ${#NombreArchivo}"
	RutaSola="${RutaCompleta:0:$LargoRuta}"
	NombreSolo="${NombreArchivo%.[^.]*}"
	Extension="${NombreArchivo:${#NombreSolo} + 1}"
	if [[ -z "$NombreSolo" && -n "$Extension" ]]; then
		NombreSolo=".$Extension"
		Extension=""
	fi
done

RUTA_DEFAULT='/tmp/'
NNOMBRE=`echo out_$NombreSolo.$Extension`
DIR_D=0

if [ "$RutaSola" = "" ] ; then
	error 0
	exit 1
fi

perm=$(stat -c %a "$RutaSola")

if [ "$perm" = "777" ]; then
  	DIRECTORIO=`echo $RutaSola$NNOMBRE`
else
	DIRECTORIO=`echo $RUTA_DEFAULT$NNOMBRE`
	DIR_D=1
fi


#Chequeo que los parametros i y d no se hayan insertado juntos
if ! test $OPCIONI -eq 0 && ! test $OPCIOND -eq 0 ; then
	error 3
	exit 1
fi

#Fin chequeo tipo de parámetros

#Comienzo del script

if ! test $OPCIOND -eq 0 ; then
#Elinino las líneas en blanco
	sed '/^$/d' $OPCIONF > $DIRECTORIO
	echo "Se borraron las líneas vacías en el directorio $OPCIONF."
	echo "Salida: $DIRECTORIO"
fi

if ! test $OPCIONI -eq 0 ; then
	#Hago el interlineado
	LINEAS=`wc -l $OPCIONF`
	awk '{
		print
		for(i=0;i<var;i++){
			print ""
		}
	}' var="$OPCIONI" $OPCIONF > $DIRECTORIO

	echo "Se modificó el interlineado del archivo $OPCIONF."

	if test $DIR_D -eq 1 ; then
		echo "No se pudo escribir en el directorio $RutaSola."
		echo "Salida: $DIRECTORIO"
	fi

fi

#Fin de script
