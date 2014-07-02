all: cliente.exe partida.exe

cliente.exe: cliente.o
	g++ -o cliente.exe cliente.o -lSDL -lpthread -lSDL_ttf -pthread


cliente.o: cliente.cpp
	g++ -c cliente.cpp cliente.h

partida.exe: partida.o
	g++ -o partida.exe partida.o -lpthread -lSDL -lSDL_ttf -pthread

partida.o: partida.cpp
	g++ -c partida.cpp partida.h

clean:
	rm -f *.o cliente.exe cliente.h.gch partida.h.gch partida.exe
