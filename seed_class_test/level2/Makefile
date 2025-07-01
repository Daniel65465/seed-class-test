CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
SRC = main.c terminal.c editor.c file_io.c buffer.c display.c
OBJ = $(SRC:.c=.o)
BIN = editor

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN) $(OBJ)

.PHONY: all clean