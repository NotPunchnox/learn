# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -O2 $(shell pkg-config --cflags sdl2)
LDFLAGS = $(shell pkg-config --libs sdl2) -lm

# Source file
SRC = main.c

# Output executable
TARGET = main

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all run clean
