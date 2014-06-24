all: compilarServer linkearServer

compilarServer:
	gcc -c torneo.c
linkearServer:
	gcc -o torneo.exe torneo.o -pthread

debug:
	gcc -g torneo.c -o torneo.dbg -pthread
logServer:
	gdb torneo.dbg core

limpiar:
	rm -f *.o *.exe *.dbg core
