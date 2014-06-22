cliente: cliente.o
	g++ -o cliente cliente.o -lSDL -lpthread

cliente.o: cliente.cpp
	g++ -c cliente.cpp cliente.h

clean:
	rm *.o cliente