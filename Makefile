CC = gcc
CFLAGS =

SUBMITION_NUMBER=2

all: flex bison
	$(CC) $(CFLAGS) -o etapa$(SUBMITION_NUMBER) lex.yy.c parser.tab.c main.c -lfl

flex: scanner.l
	flex scanner.l

bison: parser.y
	bison -d parser.y

submit: clean
	mkdir -p submit
	tar cvzf submit/etapa$(SUBMITION_NUMBER).tgz --exclude=.gitignore --exclude=.git --exclude=tests --exclude=submit .

clean:
	rm -rf etapa* lex.yy.c parser.tab.* parser.output *.o submit *.tgz