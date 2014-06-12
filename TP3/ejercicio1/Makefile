all: mesas

c_grep: mesas.o
	gcc -o mesas mesas.o

c_grep.o: mesas.c
	gcc -c mesas.c

clean:
	rm *.o mesas
