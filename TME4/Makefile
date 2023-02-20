CFLAGS = -Wall -g
CC = /usr/bin/gcc

all: mgrep zombie

mgrep: mgrep.o
	$(CC) $(CFLAGS) -o mgrep mgrep.o

zombie: zombie.o
	$(CC) $(CFLAGS) -o zombie zombie.o

zombie.o: zombie.c
	$(CC) $(CFLAGS) -c zombie.c

clean:
	rm -f mgrep mgrep.o zombie zombie.o
