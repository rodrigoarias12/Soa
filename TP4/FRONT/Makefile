cliente: cliente.o
	g++ -o cliente.exe cliente.o -lSDL -lpthread

cliente.o: cliente.cpp
	g++ -c cliente.cpp cliente.h

clean:
	rm *.o cliente.exe