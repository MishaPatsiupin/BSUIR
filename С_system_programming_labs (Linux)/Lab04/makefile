# Compiler
CC = gcc

# Compiler flags
CFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic -Werror -ggdb

# Source directory
SRC_DIR = src

# Build directory
BUILD_DIR = build

# Phony targets
.PHONY: clean all

# Default target
all: $(BUILD_DIR)/main

# Target to build the main executable
$(BUILD_DIR)/main: $(SRC_DIR)/main.c $(SRC_DIR)/list.c $(SRC_DIR)/ring.c makefile
	$(CC) $(CFLAGS) $(SRC_DIR)/main.c $(SRC_DIR)/list.c $(SRC_DIR)/ring.c -o $(BUILD_DIR)/main

# Target to clean the build directory
clean:
	rm -f $(BUILD_DIR)/main

# Target to build and run the main executable
run: all
	./$(BUILD_DIR)/main
