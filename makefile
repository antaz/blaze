CXX = gcc
BIN = trappist
INC = main.c board.c move.c attack.c 
OBJ = main.o board.o move.o attack.o
FLAGS = -Wall -Wextra

all: $(BIN)

$(BIN):
	$(CXX) $(FLAGS) $(OBJ) -o $(BIN)
	
main: 
	$(CXX) $(FLAGS) $(INC) -o $(BIN)


clean:
	rm -f *.o trappist
