CC=gcc
CFLAGS=-Wall -std=c99

all: build

build:
	$(CC) $(CFLAGS) ./src/main.c -lSDL2 -lSDL2_ttf -lm -o pong
run:
	make build && ./pong
clean:
	rm pong