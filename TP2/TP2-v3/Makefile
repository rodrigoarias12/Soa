all: c_grep

c_grep: c_grep.o
	gcc -o c_grep c_grep.o

c_grep.o: c_grep.c
	gcc -c c_grep.c

clean:
	rm *.o c_grep
