CC = gcc
BIN = trappist
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
FLAGS = -DNDEBUG -g -Wall -Wextra

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(FLAGS) -o $(BIN) $(OBJ)

.PHONY: debug
debug:
	$(CC) $(FLAGS) $(SRC) -g

.PHONY: clean
clean:
	rm -f $(OBJ) $(BIN) *.exe *.out
