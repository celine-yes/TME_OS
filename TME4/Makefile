CFLAGS = -Wall -g
CC = /usr/bin/gcc

all: mgrep

mgrep: mgrep.o
	$(CC) $(CFLAGS) -o mgrep mgrep.o

mgrep.o: mgrep.c
	$(CC) $(CFLAGS) -c mgrep.c

clean:
	rm -f mgrep mgrep.o