CC=gcc
CFLAGS=-g2 -ggdb -std=c11 -pedantic -W -Wall -Wextra

.SUFFIXES:
.SUFFIXES: .c .o

DEBUG=./build/debug
RELEASE=./build/release
OUT_DIR=$(DEBUG)

vpath %.c src
vpath %.h src
vpath %.o build/debug

ifeq ($(MODE), release)
	CFLAGS = -std=c11 -pedantic -W -Wall -Wno-unused-parameter -Wno-unused-variable
	OUT_DIR = $(RELEASE)
	vpath %.o build/release
endif

objects = $(OUT_DIR)/parent.o $(OUT_DIR)/util.o
#objects = main.o parser.o
prog = $(OUT_DIR)/parent

ifeq ($(TARGET), child)
	objects = $(OUT_DIR)/child.o $(OUT_DIR)/util.o
	prog = $(OUT_DIR)/child
endif

.PHONY: all

all: $(OUT_DIR) $(prog)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(prog): $(objects)
	$(CC) $(CFLAGS) $(objects) -o $@

$(OUT_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	@rm -rf $(DEBUG)/* $(RELEASE)/*
