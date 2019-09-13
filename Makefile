CC = gcc

build: scanner.l main.c parser.y
	bison -d parser.y  -v --report-file=report.txt
	flex scanner.l
	gcc -c main.c lex.yy.c parser.tab.c 
	gcc -o parser main.o lex.yy.o parser.tab.o -lfl   

run: parser
	./parser

test: parser in.txt
	./parser < in.txt

clean:
	rm -f parser *.o parser.tab.h parser.tab.c lex.yy.c report.txt
