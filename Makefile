CC = gcc

build: scanner.l main.c parser.y
	bison -d parser.y  -v --report-file=report.txt
	flex scanner.l
	gcc -c main.c lex.yy.c parser.tab.c tree.c -ggdb -Og
	gcc -o etapa3 main.o lex.yy.o parser.tab.o tree.o -lfl -ggdb -Og   

run: etapa2
	./etapa2

test: etapa2 in.txt
	./etapa2 < in.txt

test3: etapa3 in3.txt
	./etapa3 < in3.txt

tree: tree.c
	gcc tree.c -o tree
	./tree
clean:
	rm -f etapa2 *.o parser.tab.h parser.tab.c lex.yy.c report.txt tree
