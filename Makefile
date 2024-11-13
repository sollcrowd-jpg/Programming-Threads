# Compiler
CC = gcc

# Compiler flags
CFLAGS = -pthread -Wall -std=c99

# Target executable
TARGET = matrix_program

# Source file
SRC = matrix.c

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean target
clean:
	rm -f $(TARGET)
