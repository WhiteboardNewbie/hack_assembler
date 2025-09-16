CC = gcc
CFLAGS = -Wall -g -Iinclude

SRC = src/main.c src/symbol_table.c src/assembler.c src/utils.c
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = build/main

# Default target
all: $(TARGET)

# Compile .c -> .o
build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

# Link object file -> executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Create build directory if it doesn't exist
build:
	mkdir -p build

# Clean objects and executable
clean:
	rm -rf build