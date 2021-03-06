#####################################
#dependencias.sh
#Trabajo Práctico Nº1
#Re entrega 
#Ejercicio Número 5
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#REENTREGA
#####################################

#!/bin/bash

AYUDA="AYUDA\n=====\n\n
	   Modo de uso del script:\n1- ./dependencias.sh -a -d [path de proyecto] -f [path de salida]\n
	   2- ./dependencias.sh -c [nombreDeClase] -d [path de proyecto] -f [path de salida]\n
	   3- ./dependencias.sh -?\nMuestra la ayuda del script\n
	   El path de salida es opcional en todos los casos [-f], de lo contrario sólo se mostrará por pantalla.\n"

function error (){
	if test $1 -eq 0; then
		echo -e "La cantidad de parámetros es incorrecta."
	elif test $1 -eq 1; then
		echo -e "Las opciones a y c no pueden ir juntas."
	elif test $1 -eq 2; then
		echo  -e "El parámetro de d no es un directorio."
	elif test $1 -eq 3; then
		echo  -e "El parámetro de f no es un archivo."
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
		NombreArchivo="${OPCIONF##*/}"
		if   [ "$NombreArchivo" == "" ];then
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
#Chequeo este el directorio de busqueda

if ! test -d $OPCIOND ; then
 error 2
 exit 1
fi

#Comienzo del script

#Si se ingresó la opcion -c

IMPORT=`echo import $OPCIONC`
hayresultado=0
echo "Se procesó el proyecto :"  $OPCIOND
if [ "$OPCIONC" != "0" ] ; then
        #OPCION C   
        RESULTADO="Archivos donde se ha importado la clase/interface:"
	for archivo_a_buscar in `find $OPCIOND -name '*.java'` ; do
		NombreArchivoB="${archivo_a_buscar##*/}"
		if test -f $archivo_a_buscar ; then

			RESULTADO=`echo $RESULTADO $(awk 'BEGIN{
				count=0;
			}
			
			
			$0 ~/^import|static/ {
				var="[import|import static]" r ";";
				flag=$0 ~ var;
				if(flag == 1){
					count=1;
					
				}
			}
			END{
				if(count == 1){
					
					printf(a"\n");
				}
				
			}'-v r=$OPCIONC  a=$NombreArchivoB  < $archivo_a_buscar )`

                    hayresultado=1
		fi
	done
       
       
    
elif [ "$OPCIONA" != "0" ] ; then
        #OPCION A
	#Recorro la carpeta analizando arhivo por archivo
	#Corro el awk y agarro todo lo q sea import

	if [ "$OPCIONF" == "0" ] ; then
		OPCIONF=""
	fi

	CADENA_ARCHIVOS=""
	for archivo in `find $OPCIOND -name '*.java'` ; do
		NombreArchivoC="${archivo##*/}"
		if test -f $archivo ; then
 
			CADENA_ARCHIVOS="$CADENA_ARCHIVOS $archivo"
                    hayresultado=1
		fi
	done
   
    if test $hayresultado -eq 1 ; then 
   #Le paso al awk todos los archivos juntos.
	RESULTADO=$(awk '
	{
		if($1=="import"){
			if(NF > 2)
		 		num[$3]++
		 	else
		 		num[$2]++
			}
	}
	END{
		printf("Apariciones \t\t\t\t\t Clases \n")
		for(palabra in num)
		{
			printf("%60s %d \n",palabra, num[palabra]);
		}
	}' $CADENA_ARCHIVOS)
    fi
fi




if test $hayresultado -eq 1 ; then 
  #Validamos si está la opcion F y en ese caso imprimimos el contenido en un archivo
  if [ "$OPCIONF" != "" ] && [ "$OPCIONF" != "0" ] ; then
	echo "Salida: $OPCIONF"
        echo " "
	DIRECTORIO=$(dirname ${OPCIONF})
	mkdir -p $DIRECTORIO
	echo "$RESULTADO" > $OPCIONF
  fi
  if [ "$RESULTADO" != "Archivos donde se ha importado la clase/interface:" ] ; then
	echo "$RESULTADO"
  fi

else

 echo " "
 echo "No hay aciertos"
 echo "Recuerde el que el script solo busca en archivos con extencion '.java'"
 echo " "
fi