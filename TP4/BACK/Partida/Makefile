all: compilarServer linkearServer

compilarServer:
	gcc -c partida.c
linkearServer:
	gcc -o partida.exe partida.o -pthread

debug:
	gcc -g partida.c -o partida.dbg -pthread
logServer:
	gdb partida.dbg core

limpiar:
	rm -f *.o *.exe *.dbg core
