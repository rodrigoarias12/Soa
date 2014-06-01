all: PROGRAMA_A PROGRAMA_B

PROGRAMA_A: programaA.o
	g++ -o PROGRAMA_A programaA.o

programaA.o: programaA.c
	g++ -c programaA.c

PROGRAMA_B: programaB.o
	g++ -o PROGRAMA_B programaB.o

programaB.o: programaB.c
	g++ -c programaB.c

clean:
	rm *.o PROGRAMA_B PROGRAMA_A