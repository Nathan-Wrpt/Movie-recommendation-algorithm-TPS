# Makefile
CC = gcc
CFLAGS = -Wall -g

# List of source files
SRC = main.c ./algo/graphcreation.c ./bin_creation/movies.c ./bin_creation/user.c ./util/maxadvices.c

# List of object files
OBJ = $(SRC:.c=.o)

# Target executable
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
