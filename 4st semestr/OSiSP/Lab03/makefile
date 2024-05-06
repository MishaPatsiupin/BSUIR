CC = gcc
CFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic -Werror -ggdb
SRC_DIR = src
BUILD_DIR = build

.PHONY: clean all

all: $(BUILD_DIR)/parent $(BUILD_DIR)/child
    
$(BUILD_DIR)/parent: $(SRC_DIR)/parent.c makefile
	$(CC) $(CFLAGS) $(SRC_DIR)/parent.c -o $(BUILD_DIR)/parent
    
$(BUILD_DIR)/child: $(SRC_DIR)/child.c makefile
	$(CC) $(CFLAGS) $(SRC_DIR)/child.c -o $(BUILD_DIR)/child

clean:
	rm -f $(BUILD_DIR)/parent $(BUILD_DIR)/child

run: all
	./$(BUILD_DIR)/parent