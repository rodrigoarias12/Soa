all: allTorneo allPartida

allTorneo: compTorneo linkTorneo
allPartida: compPartida linkPartida

compTorneo:
	gcc -c torneo.c
linkTorneo:
	gcc -o torneo.exe torneo.o -pthread

compPartida:
	gcc -c partida.c
linkPartida:
	gcc -o partida.exe partida.o -pthread

debugTorneo:
	gcc -g torneo.c -o torneo.dbg -pthread
logTorneo:
	gdb torneo.dbg core

debugPartida:
	gcc -g partida.c -o partida.dbg -pthread
logPartida:
	gdb partida.dbg core

limpiar:
	rm -f *.o *.exe *.dbg core
