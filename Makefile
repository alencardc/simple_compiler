CC=gcc
CFLAGS=


flex: scanner.l
	flex scanner.l

all: flex
	$(CC) -o etapa1 lex.yy.c main.c -lfl

flexGold:
		flex gold_scanner.l
allGold: flexGold
	$(CC) -o etapa1Gold lex.yy.c main.c -lfl

clear:
	rm -rf etapa* lex.yy.c *.o