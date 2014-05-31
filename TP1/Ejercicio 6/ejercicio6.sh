#####################################
#ejercicio6.sh
#Trabajo Práctico Nº1
#Ejercicio Número 6
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#SEGUNDA ENTREGA
#####################################

#!/bin/bash

AYUDA="AYUDA\n=====\n
	   Modo de uso del script: $(basename "$0") [OPCION] [ARCHIVO_SALIDA] \n
	   Donde:\n
	   \t -help             \t\t\t muestra la ayuda \n
	   \t -h    		\t\t\t muestra la ayuda \n
	   \t -?    		\t\t\t muestra la ayuda \n
	   \t [nombreDelArhivo.txt] \t lee el archivo y genera un archivo de salida [ARCHIVO_SALIDA] (out.txt si no se ingresa) el \n
	   \t\t\t\t                    cual contendra una lista ordenada de mayor a menor con el total de goles de cada jugador. \n
	   \t\t\t\t                    En caso de que 2 jugadores tengan igual cantidad de goles se ordenaran en segundo\n
	   \t\t\t\t                    orden alfabeticamente por nombre \n"

function error () {
	if (($1 == 0)); then
		echo  -e "ERROR: La cantidad de parametros es incorrecta."
	elif (($1 == 1)); then
		echo  -e "ERROR: El parametro '$2' no es un parametro valido."
	elif (($1 == 2)); then
		echo  -e "ERROR: El archivo de entrada '$2' no existe"
	elif (($1 == 3)); then
		echo  -e "ERROR: El formato del archivo de entrada debe ser '.txt'"
	elif (($1 == 4)); then
		echo  -e "ERROR: El archivo de salida '$2' ya existe"
	elif (($1 == 5)); then
		echo  -e "ERROR: El formato del registro del archivo de entrada es invalido"
	fi
	echo -e "Intente './$(basename "$0") -h' para mas ayuda"
}

if (($# == 2)); then
	OUTPUT=$2;
else
	OUTPUT="out.txt";
fi

#Chequeo de parámetros mínimos requeridos
if (($# != 1)) && (($# != 2)); then
	error 0
	exit 1
fi

#Chequea el parámetro de ayuda

if [ "$1" == "-?" ] ||
   [ "$1" == "-help" ] ||
   [ "$1" == "-h" ];then
   	echo -e $AYUDA
   	exit 1
fi

#Valida que los parametros sean validos
FILE=$1
   
if [ "$1" != "-?" ] && 
   [ "$1" != "-help" ] && 
   [ "$1" != "-h" ] &&
   [[ $1 =~ ^- ]];then
	   error 1 $1
	   exit 1
fi

if [ ! -f $FILE ]; then
	error 2 $FILE
	exit 1
fi

if [[ $FILE != *.txt ]]; then
	error 3
	exit 1
fi

if [ -f $OUTPUT ]; then
	error 4 $OUTPUT
	exit 1
fi


#Valida el formato del archivo de entrada
#Utiliza el comando grep para buscar mediante una expresion regular las filas validas
while read line; do
    	LINEAS_CORRECTAS=$((LINEAS_CORRECTAS+1))
done < <(LANG=en_US.iso88591 grep -E "[0-9]{1,2}\/[0-9]{1,2}\/[0-9]{4}([ |	]*)[a-zA-ZñáéíóúÁÉÍÓÚ](.*)[a-zA-ZñáéíóúÁÉÍÓÚ]([ |	]*)[0-9]+" $FILE)

#Utilizamos el comando cat y wc para obtener la cantidad de lineas
LINEAS_TOTALES=$(cat $FILE | wc -l)
LINEAS_TOTALES=$((LINEAS_TOTALES-1))
if (($LINEAS_CORRECTAS != $LINEAS_TOTALES)); then
	error 5
	exit 1
fi

#awk Suma los goles por jugador y los devuelve en un vector asociativo
# la clave del vector es el nombre completo y el valor es la cantidad de goles
awk '$1 ~/^[0-9]/ {
            clave=""

            for(i=2;i<NF;i++)
            {
              if(i<4)
                clave=clave $(i);

              else
                clave=clave " " $(i);
            }
            goles[clave]+=$(NF)

          }
	END {
		print "Jugador			Total"			
		for (jugador in goles) { 

			print goles[jugador] "			" jugador;
	}   
}' $1 | sort -k1r,1 -k2n,2 > $OUTPUT; #el orden lo realiza el bash, ordena la cantidad de goles descendente y los nombres ascendentes
