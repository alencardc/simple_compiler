CC = gcc
CFLAGS =

SUBMITION_NUMBER = 3

UTIL_FILES := $(wildcard utils/**/*.c)
UTIL_FILES := $(filter-out $(wildcard utils/tests/*.c), $(UTIL_FILES))

all: flex bison
	$(CC) $(CFLAGS) -o etapa$(SUBMITION_NUMBER) lex.yy.c parser.tab.c $(UTIL_FILES) main.c -lfl

debug: flex bison
	$(CC) $(CFLAGS) -o etapa$(SUBMITION_NUMBER) lex.yy.c parser.tab.c $(UTIL_FILES) main.c -lfl -DDEBUG

flex: scanner.l
	flex scanner.l

bison: parser.y
	bison -d parser.y

submit: clean
	mkdir -p submit
	tar cvzf submit/etapa$(SUBMITION_NUMBER).tgz --exclude=.gitignore --exclude=.git --exclude=tests --exclude=submit .

clean:
	rm -rf etapa* lex.yy.c parser.tab.* parser.output *.o submit *.tgz ./tests/ast/*.png .tests/ast/*.txt