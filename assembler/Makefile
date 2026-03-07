CC = gcc
CFLAGS = -Wall -ansi -pedantic
OBJ = assembler.o utils.o macro.o pre_assembler.o first_pass.o second_pass.o symbols.o

assembler: $(OBJ)
	$(CC) $(CFLAGS) -o assembler $(OBJ)

assembler.o: assembler.c globals.h
	$(CC) $(CFLAGS) -c assembler.c

utils.o: utils.c globals.h
	$(CC) $(CFLAGS) -c utils.c

macro.o: macro.c macro.h globals.h
	$(CC) $(CFLAGS) -c macro.c

pre_assembler.o: pre_assembler.c pre_assembler.h macro.h globals.h
	$(CC) $(CFLAGS) -c pre_assembler.c

first_pass.o: first_pass.c first_pass.h globals.h symbols.h
	$(CC) $(CFLAGS) -c first_pass.c

second_pass.o: second_pass.c second_pass.h globals.h symbols.h
	$(CC) $(CFLAGS) -c second_pass.c

symbols.o: symbols.c symbols.h globals.h
	$(CC) $(CFLAGS) -c symbols.c

clean:
	rm -f *.o assembler *.am *.ob *.ent *.ext