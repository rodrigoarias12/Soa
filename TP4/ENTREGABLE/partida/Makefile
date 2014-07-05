all: partida.exe

partida.exe: partida.o
	g++ -o partida.exe partida.o -pthread -lSDL -lSDL_ttf

partida.o: partida.cpp
	g++ -c partida.cpp partida.h

clean:
	rm -f *.o  partida.h.gch partida.exe
	



