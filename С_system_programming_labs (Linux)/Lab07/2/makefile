# makefile
CC = gcc
CFLAGS = -W -Wall -Wextra -std=c11
.PHONY: clean

all: second generator
second: second.c makefile
	$(CC) $(CFLAGS) second.c -o second
generator: generator.c makefile
	$(CC) $(CFLAGS) generator.c -o generator
clean:
	rm -rf second generator
