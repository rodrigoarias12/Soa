all: compTorneo compPartida compCliente

compTorneo: objTorneo
	gcc -o torneo.exe torneo.o -pthread
objTorneo:
	gcc -c torneo.c torneo.h

compPartida: objPartida
	gcc -o partida.exe partida.o -pthread
objPartida:
	gcc -c partida.c partida.h

compCliente: objCliente
	g++ -o cliente.exe cliente.o -lSDL -lpthread -lSDL_ttf
objCliente:
	g++ -c cliente.cpp cliente.h

debugTorneo:
	gcc -g torneo.c -o torneo.dbg -pthread
logTorneo:
	gdb torneo.dbg core

debugPartida:
	gcc -g partida.c -o partida.dbg -pthread
logPartida:
	gdb partida.dbg core

debugCliente:
	gcc -g cliente.cpp -o cliente.dbg -lSDL -lpthread -lSDL_ttf
logCliente:
	gdb cliente.dbg core

clean:
	rm -f *.o *.exe *.dbg *.h.gch core
