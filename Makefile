# Copyright 2023 Nichita-Adrian Bunu <bununichita@gmail.com>
CC = gcc
CFLAGS = -Wall -g

build: main

main: main.o operations.o
	$(CC) -o main $^

main.o: main.c
	$(CC) $(CFLAGS) -c $^

operations.o: operations.c
	$(CC) $(CFLAGS) -c $^

run: build
	./main

pack:
	zip -FSr 313CA_BunuNichita-Adrian.zip README Makefile *.c *.h

clean:
	rm -f *.o main
