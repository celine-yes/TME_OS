CFLAGS = -Wall -g
CC = /usr/bin/gcc

all: arbre minishell

arbre: arbre.o
	$(CC) $(CFLAGS) -o arbre arbre.o

arbre.o: arbre.c
	$(CC) $(CFLAGS) -c arbre.c

minishell: minishell.o
	$(CC) $(CFLAGS) -o minishell minishell.o

minishell.o: minishell.c
	$(CC) $(CFLAGS) -c minishell.c

clean:
	rm -f arbre arbre.o minishell minishell.o
