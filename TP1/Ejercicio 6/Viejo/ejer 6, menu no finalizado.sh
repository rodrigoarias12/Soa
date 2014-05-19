#####################################
#rankingGoles.sh
#Trabajo Práctico Nº1
#Ejercicio Número 6
#Arias, Rodrigo DNI:
#Culen, Fernando DNI
#García Alvez, Pablo DNI: 
#Juffar, Sebastian DNI:
#Nogueiras, Jorge DNI: 34.670.613
#SEGUNDA ENTREGA
#####################################

#!/bin/bash

AYUDA="AYUDA\n=====\n\n
	   Modo de uso del script:\n1- ./rankingGoles.sh -a -d [path de proyecto] -f [path de salida]\n
	   2- ./dependencias.sh -c [nombreDeClase] -d [path de proyecto] -f [path de salida]\n
	   3- .dependencias.sh -?\nMuestra la ayuda del script\n
	   El path de salida es opcional en todos los casos [-f], de lo contrario sólo se mostrará por pantalla.\n"

function error () {
	if test $1 -eq 0; then
		echo -e "La cantidad de parámetros es incorrecta."
	elif test $1 -eq 1; then
		echo -e "Las opciones a y c no pueden ir juntas."
	elif test $1 -eq 2; then
		echo  -e "El parámetro -f no es un archivo de texto valido."
	elif test $1 -eq 3; then
		echo  -e "El parámetro de f no es un directorio."
	elif test $1 -eq 4; then
		echo -e "Se esperaba la opcion -? o más opciones."
	fi
	echo -e $AYUDA
}

#Chequeo de parámetros mínimos requeridos
if test $# -eq 0 || test $# -gt 2; then
	error 0
	exit 1
fi
//LO ARREGLAMO
if test $# -eq 1 ; then
	if [ "$1" != "-?" || "$1" != "-h" || "$1" != "-help" || "$1" != "-o" || "$1" != "-d"] ; then
		error 4
		exit 1
	fi
fi

function es_archivo_valido (){
 
    if ! ("$1"=="-o" && test -e $1) ; then
        error 2
        exit 1
    fi
}

#Fin chequeo

awk '$1 ~ /^[0-9]/ {
	clave=$2$3;
	goles[clave]+=$(NF);
}
END {
	for (jugador in goles) { 
		print goles[jugador] " " jugador;
	}   
}' input.txt | sort -k1r,1 -k2n,2
