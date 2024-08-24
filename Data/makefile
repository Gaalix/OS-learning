all: build run

build:
	gcc -Wall -Wextra -Werror -Wpedantic -o data_buffer main.c buffer.c

run:
	./data_buffer

clean:
	rm -f data_buffer