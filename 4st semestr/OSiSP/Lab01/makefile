CC = gcc
CFLAGS = -std=c11 -g2 -ggdb -pedantic -W -Wall -Wextra
.SUFFIXES:
.SUFFIXES: .c .o
DEBUG = ./build/debug
RELEASE = ./build/release
OUT_DIR = $(DEBUG)
vpath %.c src
vpath %.h src
vpath %.o $(DEBUG)
ifeq ($(MODE), release)
CFLAGS = -std=c11 -pedantic -W -Wall -Wextra -Werror
OUT_DIR = $(RELEASE)
vpath %.o $(RELEASE)
endif
objects = $(OUT_DIR)/main.o
prog = $(OUT_DIR)/test
all: $(prog)
$(prog): $(objects)
	$(CC) $(CFLAGS) $(objects) -o $@
$(OUT_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@
.PHONY: clean
clean:
	@rm -rf $(DEBUG)/* $(RELEASE)/* test