CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
SRC = main.c terminal.c editor.c
BIN = editor

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(BIN)