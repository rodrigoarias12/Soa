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
			echo -e $AYUDA
			exit 1
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

#Si se ingresó la opcion -c

IMPORT=`echo import $OPCIONC`
SRC=`echo $OPCIOND/src`

if [ "$OPCIONC" != "0" ] ; then

	for archivo_a_buscar in `ls -R $SRC` ; do
		ARCH=`echo $SRC/$archivo_a_buscar`
		if test -f $ARCH ; then
			
			awk 'BEGIN{
				cont=0;
			}
			/r/{
				cont++;
			}
			END{
				if(cont > 0) {
					printf(a"\n")
				}
			}' r=$OPCIONC  a=$archivo_a_buscar  < $ARCH >> /tmp/opf
		fi
	done

	echo -e "Archivo/s que importan la clase/interface $OPCIONC son:\n" `cat /tmp/opf`

	if [ "$OPCIONF" != "0" ] ; then
		NOMBREARCHIVOSALIDA=`echo $OPCIONF/reporte_proyecto.txt`
		echo -e "Archivo/s a mostrar:\n" `cat /tmp/opf` > $NOMBREARCHIVOSALIDA
	fi

	rm /tmp/opf

elif [ "$OPCIONA" != "0" ] ; then
	# 2 recorro la carpeta analizando arhivo por archivo
	#corro el awk y me agarro todo lo q sea import
	# despues creo un archivo en temporal 
	for archivo_a_buscar in `ls -R $SRC` ; do
	  	ARCH=`echo $SRC/$archivo_a_buscar`
		if test -f $ARCH ; then	
	   		awk '{
	                 if($1=="import")
	                 	if(NF > 2)
	                  		{print($3)}
	                  	else{
	                  		print $2
	                  	}             
	        }' $ARCH >>/tmp/opa
	    fi
	echo $archivo_a_buscar
	cat /tmp/opa
	done
	echo arranco
	cat /tmp/opa
	echo aca arranco la gilada 
	
	 #ahora agarro el archivo que arme y hago un vector con clave y le sumo uno cada ves q se hace un match    
	awk '{ 
	        	num[$0]++
         } 
	     END{printf("Clase\t\t\tApariciones \n")
	          for(palabra in num)
	           	{
	           		printf("%s\t\t\t%d \n",palabra, num[palabra])  
	           	}
       	}' /tmp/opa > $OPCIONF/reporte_proyecto
       	cat $OPCIONF/reporte_proyecto
	#guardo el reporte
	#!!!antes de esto deberia preguntar si estaba el parametro -f para guardar el reporte o solo que salga por pantalla .
	rm /tmp/opa
fi 