CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wpedantic -m64

all: build run

build: build_o
	$(CC) $(CFLAGS) main.o -o demo_gcc

run:
	./demo_gcc

clean:
	rm -f demo_gcc main.i main.s main.o

build_i: main.c
	$(CC) $(CFLAGS) -E main.c -o main.i

build_s: build_i
	$(CC) $(CFLAGS) -S main.i -o main.s

build_o: build_s
	$(CC) $(CFLAGS) -c main.s -o main.o