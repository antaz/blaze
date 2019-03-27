CC = gcc
BIN = trappist
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
OPT = -march=native -O3
FLAGS = -std=c11 -Wall -Wextra

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OPT) $(FLAGS) -o $(BIN) $(OBJ)

.PHONY: debug
debug:
	$(CC) $(SRC) -g -DNDEBUG $(FLAGS)

.PHONY: clean
clean:
	rm -f $(OBJ) $(BIN) *.exe *.out
