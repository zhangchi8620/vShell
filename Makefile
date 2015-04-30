
OBJECT = test.o block.o filesystem.o interpreter.o

LIB = ./Blocks/block.h ./Types/types.h ./Filesystem/filesystem.h ./Interpreter/interpreter.h

CFLAG = -g -c -O
CC = g++

all: vShell

vShell: $(OBJECT)
	$(CC) $(OBJECT) -o vShell

test.o: test.c ./Blocks/block.h ./Types/types.h ./Filesystem/filesystem.h
	$(CC) $(CFLAG) test.c

block.o: ./Blocks/block.c ./Blocks/block.h ./Types/types.h
	$(CC) $(CFLAG) ./Blocks/block.c

filesystem.o: ./Filesystem/filesystem.c ./Filesystem/filesystem.h ./Blocks/block.h ./Types/types.h
	$(CC) $(CFLAG) ./Filesystem/filesystem.c

interpreter.o: ./Interpreter/interpreter.h ./Interpreter/interpreter.c ./Filesystem/filesystem.c ./Filesystem/filesystem.h ./Types/types.h
	$(CC) $(CFLAG) ./Interpreter/interpreter.c

clean:
	rm -rf *o $(OBJECT)


