#####################################
#ejercicio2.sh
#Trabajo Práctico Nº1
#Ejercicio Número 2
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################

#!/bin/bash

#comprobar si el valor es numérico

function error (){
	echo "El parámetro no es válido."
	if test $1 -eq 0; then
		echo "Debe especificar números enteros en los parámetros."
	elif test $1 -eq 1; then
		echo "No es un archivo válido de salida."
	fi
	echo -e "Utilización de script:\n./ejercicio2.sh [numero entero] [numero entero] [path archivo salida]"
}
function es_entero (){
	        if [ $1 -eq $1 2> /dev/null ]; then			
			valido=1	
		else
                  echo "Error en uno de los parametros , no es un numero entero"
			exit 1         
		fi
}
#Comienzo del programa
#Comprobación de parámetros
if test $# -lt 3 ; then
	echo "Cantidad de Parámetros incorrectos."
	echo -e "Utilización de script:\n./ejercicio2.sh [numero entero] [numero entero] [path archivo salida]"
	exit 1;
fi

if ! test -e $3; then
	echo -e "Utilización de script:\n./ejercicio2.sh [numero entero] [numero entero] [path archivo salida]"
	error 1;
	exit 1;
fi

es_entero $1
es_entero $2
#Fin comprobación de parametros
RESULTADO="SUMA: $(($1+$2))\nRESTA: $(($1-$2))\nMULTIPLICACION: $(($1*$2))\nPROMEDIO: $((($1+$2)/2))"
echo -e "$RESULTADO" > $3

echo -e "Salida Exitosa\n"

######################################
