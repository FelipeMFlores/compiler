CC = gcc

build: scanner.l main.c parser.y
	bison -d parser.y  -v --report-file=report.txt
	flex scanner.l
	$(CC) -c main.c lex.yy.c parser.tab.c tree.c etapa4files/auxiliary.c etapa4files/hashtable.c etapa5files/codeGenerator.c -ggdb -Og
	$(CC) -o etapa5 *.o -lfl -ggdb -Og

clean:
	rm -f etapa5 *.o parser.tab.h parser.tab.c lex.yy.c report.txt
