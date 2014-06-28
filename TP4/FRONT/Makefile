all: cliente.exe partida.exe

cliente.exe: cliente.o
	g++ -o cliente.exe cliente.o -lSDL -lpthread

cliente.o: cliente.cpp
	g++ -c cliente.cpp cliente.h

partida.exe: partida.o
	g++ -o partida.exe partida.o -lpthread -lSDL

partida.o: partida.cpp
	g++ -c partida.cpp partida.h

clean:
	rm *.o cliente.exe cliente.h.gch partida.exe