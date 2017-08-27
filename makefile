CXX = gcc
BIN = trappist
INC = main.c fen.c
OBJ = main.o fen.o
FLAGS = -Wall -Wextra

all: $(BIN)

$(BIN):
	$(CXX) $(FLAGS) $(OBJ) -o $(BIN)
	
main: 
	$(CXX) $(FLAGS) $(INC) -o $(BIN)


clean:
	rm *.o trappist.exe