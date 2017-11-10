CC=	g++
OBJ=	./obj
INCLUDE=./include
CFLAGS=	-I$(INCLUDE)
_DEPS= 	utils.h
DEPS=	$(patsubst %, $(INCLUDE)/%, $(_DEPS))
SRC= 	./src

all:
	make utils generate_predicates

$(OBJ)/utils.o: $(SRC)/utils.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ)/generate_predicates.o: $(SRC)/generate_predicates.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

utils: $(OBJ)/generate_predicates.o $(OBJ)/utils.o
	$(CC) -o generate_predicates $(OBJ)/utils.o $(OBJ)/generate_predicates.o $(CFLAGS)

.PHONY: clean run

clean:
	rm -f $(OBJ)/* utils generate_predicates test_predicates.py

run:
	./generate_predicates && cat test_predicates.py
