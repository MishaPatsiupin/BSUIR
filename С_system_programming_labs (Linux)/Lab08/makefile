# makefile
CC = gcc
CFLAGS = -W -Wall -Wextra -std=c11
.PHONY: clean

all: server client
server: server.c makefile
	$(CC) $(CFLAGS) server.c -o server
client: client.c makefile
	$(CC) $(CFLAGS) client.c -o client
clean:
	rm -f server client
