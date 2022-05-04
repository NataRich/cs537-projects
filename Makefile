CC = gcc
CFLAGS = -Wall -Wextra
DFLAGS = -g
DEPENDENCIES.C = read_ext2.c linkedlist.c jpg.c reader.c
EXEC = runscan
MAIN.C = runscan.c

default: main

clean:
	rm -f $(EXEC)

main: $(MAIN.C)
	$(CC) $(CFLAGS) $(DFLAGS) $(MAIN.C) $(DEPENDENCIES.C) -o $(EXEC)
