all: servidor.exe cliente.exe

servidor.exe: server.o
	gcc -o servidor.exe server.o -pthread
server.o: server.c
	gcc -c server.c

cliente.exe: client.o
	gcc -o cliente.exe client.o
client.o: client.c
	gcc -c client.c

debug:
	gcc -g client.c -o cliente.dbg
	gcc -g server.c -o server.dbg -pthread
logCliente:
	gdb cliente.dbg core
logServer:
	gdb server.dbg core

clean:
	rm -f *.o *.exe *.dbg core
