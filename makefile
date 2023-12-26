CC = gcc
CFLAGS = -Wall -Wextra -I./algo -I./bin_creation -I./util -Wpedantic -fsanitize=address,undefined -lm

SRC = main.c \
      algo/graphcreation.c \
      bin_creation/movies.c \
      bin_creation/user.c \
      util/maxadvices.c \
      util/progressbar.c \
      util/getmovietitle.c \

OBJ = $(SRC:.c=.o)

TARGET = main

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)