CC=	gcc
OBJ=	./obj
INCLUDE=./include
CFLAGS=	-I$(INCLUDE)
_DEPS= standard.h
DEPS=	$(patsubst %, $(INCLUDE)/%, $(_DEPS))

all:
	make server_exe
	make client_exe

$(OBJ)/server.o: server/server.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ)/client.o: client/client.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server_exe: $(OBJ)/server.o
	$(CC) -o server_exe $(OBJ)/server.o $(CFLAGS)

client_exe: $(OBJ)/client.o
	$(CC) -o client_exe $(OBJ)/client.o $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ)/*
	rm -f server_exe client_exe file1.txt file2.txt binary_file
