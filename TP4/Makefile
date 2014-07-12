all: torneo.exe partida.exe cliente.exe

torneo.exe: torneo.o
	gcc -o torneo.exe torneo.o -pthread -lSDL -lSDL_ttf
torneo.o: torneo.c
	gcc -c torneo.c torneo.h

partida.exe: partida.o
	gcc -o partida.exe partida.o -pthread
partida.o: partida.c
	gcc -c partida.c partida.h

cliente.exe: cliente.o
	g++ -o cliente.exe cliente.o -lSDL -lpthread -lSDL_ttf
cliente.o: cliente.cpp
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
