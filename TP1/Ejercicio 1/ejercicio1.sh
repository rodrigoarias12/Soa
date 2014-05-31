#####################################
#ejercicio1.sh
#Trabajo Práctico Nº1
#Ejercicio Número 1
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#SEGUNDA ENTREGA
#####################################

#!/bin/bash

if test $# -lt 2; then
	echo "Se han ingresado menos de dos parámetros."
	exit 1;
fi

if ! test -e $1; then
	echo "El primer parámetro no es un archivo existente."
	exit 1;
fi

FILE=$1 #guardo el contenido del parámetro $1 en FILE
shift 1 #cambio el orden de los parámetros
COUNTER=0 #contador a cero
while test $COUNTER -le $# ; do #Mientras el contador sea menor a la cantidad de parámtros
	SET[$COUNTER]=$1 #guardo el valor del parámetro $1 en el array SET en la posición COUNTER
	shift 1 #Vuelvo a poner en $1 el siguiente parámetro
	((COUNTER++)) #Sumo 1 al contador
done

for X in "${SET[@]}" ;do #Por cada palabra por parámetro (${SET[@]} retorna cada línea como una palabra individual)
	CANT=0
	for WORD in `cat $FILE` ;do #y por cada palabra del archivo
		if [[ "$WORD" == "$X" ]]; then #Me fijo si existe en el archivo
			((CANT++)) #y la cuento
		fi
	done
echo "$X : $CANT" #al final imprimo la cantidad de veces que se repite una palabra en el archivo
done

exit 0
#################################