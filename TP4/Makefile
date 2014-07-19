all: torneo.exe partida.exe jugador.exe

torneo.exe: torneo.o
	gcc -o torneo.exe torneo.o -pthread -lSDL -lSDL_ttf
torneo.o: torneo.c torneo.h
	gcc -c torneo.c torneo.h

partida.exe: partida.o
	gcc -o partida.exe partida.o -pthread
partida.o: partida.c partida.h
	gcc -c partida.c partida.h

jugador.exe: jugador.o
	g++ -o jugador.exe jugador.o -lSDL -lpthread -lSDL_ttf
jugador.o: jugador.cpp jugador.h
	g++ -c jugador.cpp jugador.h

debugTorneo:
	gcc -g torneo.c -o torneo.dbg -pthread
logTorneo:
	gdb torneo.dbg core

debugPartida:
	gcc -g partida.c -o partida.dbg -pthread
logPartida:
	gdb partida.dbg core

debugJugador:
	gcc -g jugador.cpp -o jugador.dbg -lSDL -lpthread -lSDL_ttf
logJugador:
	gdb jugador.dbg core

clean:
	rm -f *.o *.exe *.dbg *.h.gch core
