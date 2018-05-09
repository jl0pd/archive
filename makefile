all: prog

prog: main.o
	gcc -std=c11 bin/main.o -o prog.test
main.o: src/main.c bin
	gcc -std=c11 -c src/main.c -o bin/main.o
bin:
	mkdir bin
clean:
	rm -rf bin
