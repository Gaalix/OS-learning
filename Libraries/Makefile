CC=gcc
CFLAGS=

all: build run

build: build_static build_dyn_imp build_dyn_exp

run: run_static run_dyn_imp run_dyn_exp

clean:
	rm -f *.o *.a *.so prog_static prog_dyn_imp prog_dyn_exp libmath.so.2.4 libmath.so.2

build_static: add.o mul.o
	ar rcs libmath.a add.o mul.o
	$(CC) -o prog_static main.c libmath.a

build_dyn_imp: add.o mul.o
	$(CC) -shared -o libmath.so.2.4 -Wl,-soname,libmath.so.2 add.o mul.o
	ln -sf libmath.so.2.4 libmath.so.2
	ln -sf libmath.so.2 libmath.so
	$(CC) -o prog_dyn_imp main.c -L. -lmath

build_dyn_exp: add.o mul.o
	$(CC) -shared -o libmath.so.2.4 -Wl,-soname,libmath.so.2 add.o mul.o
	ln -sf libmath.so.2.4 libmath.so.2
	ln -sf libmath.so.2 libmath.so
	$(CC) -o prog_dyn_exp main_exp.c -ldl

run_static:
	./prog_static

run_dyn_imp:
	LD_LIBRARY_PATH=. ./prog_dyn_imp

run_dyn_exp:
	LD_LIBRARY_PATH=. ./prog_dyn_exp

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)