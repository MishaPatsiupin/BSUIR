# makefile
CC = gcc
CFLAGS = -W -Wall -Wextra -std=c11
.PHONY: clean

all: first
first: first.c makefile
	$(CC) $(CFLAGS) first.c -o first
clean:
	rm -rf first
