#####################################
#dependencias.sh
#Trabajo Práctico Nº1
#Ejercicio Número 5
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################

#!/bin/bash

AYUDA="AYUDA\n=====\n\n
	   Modo de uso del script:\n1- ./dependencias.sh -a -d [path de proyecto] -f [path de salida]\n
	   2- ./dependencias.sh -c [nombreDeClase] -d [path de proyecto] -f [path de salida]\n
	   3- .dependencias.sh -?\nMuestra la ayuda del script\n
	   El path de salida es opcional en todos los casos [-f], de lo contrario sólo se mostrará por pantalla.\n"

function error (){
	if test $1 -eq 0; then
		echo -e "La cantidad de parámetros es incorrecta."
	elif test $1 -eq 1; then
		echo -e "Las opciones a y c no pueden ir juntas."
	elif test $1 -eq 2; then
		echo  -e "El parámetro de d no es un directorio."
	elif test $1 -eq 3; then
		echo  -e "El parámetro de f no es un directorio."
	elif test $1 -eq 4; then
		echo -e "Se esperaba la opcion -? o más opciones."
	fi
	echo -e $AYUDA
}

#Chequeo de parámetros mínimos requeridos
if test $# -eq 0 || test $# -eq 2; then
	error 0
	exit 1
fi

if test $# -eq 1 ; then
	if [ "$1" != "-?" ] ; then
		error 4
		exit 1
	fi
fi
#Fin chequeo

OPCIONA=0
OPCIONC=0
OPCIOND=0
OPCIONF=0


while getopts ":ac:d:f:?" opt; do	
  case $opt in
    f) OPCIONF=$OPTARG
		if ! test -d $OPCIONF ; then
			error 3
			exit 1
		fi
	;;
	d)	if  [[ "$OPTARG" = "-f" || "$OPTARG" = "-a" || "$OPTARG" = "-?" ]] ; then
			echo "Opción -d necesita un parámetro."
			OPTIND=OPTIND-1
		else
			OPCIOND=$OPTARG
			if ! test -d $OPCIOND ; then
				error 2
				exit 1
			fi
  		fi
	;;
    c) 	if  [[ "$OPTARG" = "-d" || "$OPTARG" = "-a" || "$OPTARG" = "-?" ]] ; then
			echo "Opción -c necesita un parámetro."
			OPTIND=OPTIND-1
		else
			OPCIONC=$OPTARG
			echo "$OPCIONC"
  		fi		
	;;
    a) OPCIONA=1
	;;
    :) echo "Opción -$OPTARG necesita un parámetro."
	   echo -e $AYUDA
	   exit 1
  	;;
  	?) echo -e $AYUDA
	   exit 1
	;;
  esac
done

#Fin manejo de opciones de ingreso

#Chequeo que a y c no se hayan insertado juntas

if test ! $OPCIONA -eq 0 && ! test $OPCIONC -eq 0  ; then
	error 1
	exit 1
fi

#Comienzo del script
if  [ "$OPCIONF" != "0" ] ; then
	NOMBREARCHIVOSALIDA=`echo $OPCIONF/reporte_proyecto.txt`
else
	NOMBREARCHIVOSALIDA=""
fi

#Si se ingresó la opcion -c
#if [ "$OPCIOND" != "0" ] ; then
#	SRC=`echo $OPCIOND/src/`
#
#	for archivo_a_buscar in `ls $SRC` ; do
#		echo $archivo_a_buscar
#		awk '{
#			print "var"
#		}' var="$OPCIOND" $archivo_a_buscar 
#	done
#fi

# #Si se ingresó la opción -a
#1 pongo el source en la carpeta que me pasaron	
SRC=`echo $OPCIOND/src/`
#2 recorro la carpeta analisando arhivo por archivo
#!!! tal vez haga falta validar que sean .java , aunque no deberina si estan en esta carpeta . quedaria mas pro
#corro el awk y me agarro todo lo q sea import
# despues creo un archivo en temporal 
    for archivo_a_buscar in `ls $SRC` ; do
	 awk '{
              if(NR!=1){
                 if($1=="import")
                  print($0) 
                       }			
		}' $SRC/$archivo_a_buscar >>/tmp/rodrigo
	done
 #ahora agarro el archivo que arme y hago un vector con clave y le sumo uno cada ves q se hace un match    
 awk '{ for(i=1 ; i<=NF;i++)
         num[$0]++

      } 
              END{printf("Clase                        Apariciones \n")
                  for(palabra in num)
                  {printf("%s %d \n",palabra, num[palabra])  
                  }
                  }			
		'  /tmp/rodrigo > $OPCIONF/reporte_proyecto
#guardo el reporte
#!!!antes de esto deberia preguntar si estaba el parametro -f para guardar el reporte o solo que salga por pantalla .

#elimino el archivo temporal 
rm /tmp/rodrigo
