# Makefile for Assembler Project
CC = gcc
CFLAGS = -Wall -ansi -pedantic
OBJ = assembler.o utils.o pre_assembler.o first_pass.o second_pass.o

assembler: $(OBJ)
	$(CC) $(CFLAGS) -o assembler $(OBJ)

assembler.o: assembler.c globals.h
	$(CC) $(CFLAGS) -c assembler.c

utils.o: utils.c globals.h
	$(CC) $(CFLAGS) -c utils.c

pre_assembler.o: pre_assembler.c globals.h
	$(CC) $(CFLAGS) -c pre_assembler.c

first_pass.o: first_pass.c globals.h
	$(CC) $(CFLAGS) -c first_pass.c

second_pass.o: second_pass.c globals.h
	$(CC) $(CFLAGS) -c second_pass.c

clean:
	rm -f *.o assembler