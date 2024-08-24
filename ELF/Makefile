CC=gcc
CFLAGS=-m32 -Wall -Wextra -Wpedantic -Werror

all: build run

build: build_elf_changer build_hello

build_elf_changer:
	$(CC) $(CFLAGS) elf_changer.c -o elf_changer

build_hello:
	$(CC) $(CFLAGS) hello.c -o hello

run:
	./elf_changer -h hello
	./elf_changer -l hello
	./elf_changer -c g1,g2,g3 hello

clean:
	rm -f elf_changer hello
