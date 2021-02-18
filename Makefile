CC=gcc
CFLAGS=

all: flex bison
	$(CC) -o etapa1 lex.yy.c parser.tab.c main.c -lfl

flex: scanner.l
	flex scanner.l

clear:
	rm -rf etapa* lex.yy.c parser.tab.* *.o