CFLAGS = -std=c11 -Wall -Werror -g

all: bin build bin/prog.test

bin/prog.test: build/main.o build/decode.o build/encode.o
	gcc $(CFLAGS) build/main.o build/command.o build/files.o build/decode.o build/encode.o -o bin/prog.test

build/main.o: src/main.c
	gcc $(CFLAGS) -c src/main.c -o build/main.o
build/command.o: build/files.o src/command.c src/command.h
	gcc $(CFLAGS) -c src/command.c -o build/command.o
build/files.o: src/files.c src/files.h
	gcc $(CFLAGS) -c src/files.c -o build/files.o
build/decode.o: build/command.o src/decode.c src/decode.h
	gcc $(CFLAGS) -c src/decode.c -o build/decode.o
build/encode.o: build/command.o src/encode.c src/encode.h
	gcc $(CFLAGS) -c src/encode.c -o build/encode.o

bin:
	mkdir -p bin
build:
	mkdir -p build

clean:
	rm -r bin
	rm -r build

run: makefile build bin bin/prog.test
	./bin/prog.test 	
