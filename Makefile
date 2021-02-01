CC=gcc
CFLAGS=


flex: scanner.l
	flex scanner.l

all: flex
	$(CC) -o etapa1 lex.yy.c main.c -lfl

clear:
	rm -rf etapa* lex.yy.c *.o