CC = gcc
ASM = nasm
CFLAGS = -m32 -fPIE
LDFLAGS = -m32 -fPIE
ASMFLAGS = -f elf32

all: build_c_27255 run_c_27255 build_asm_27255 run_asm_27255

build_c_27255:
	$(CC) $(CFLAGS) -o sys_calls_c main.c

run_c_27255:
	./sys_calls_c

build_asm_27255:
	$(ASM) $(ASMFLAGS) -o main_asm.o main_asm.s
	$(CC) $(LDFLAGS) -nostartfiles -o sys_calls_asm main_asm.o

run_asm_27255:
	./sys_calls_asm

clean:
	rm -f sys_calls_c sys_calls_asm main_asm.o
	rm -rf SysDir
