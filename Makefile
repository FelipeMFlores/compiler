CC = gcc

build: scanner.l main.c tokens.h
	flex scanner.l
	gcc main.c lex.yy.c -o analisador -lfl

run: analisador
	./analisador

test: analisador in.txt
	./analisador < in.txt

clean:
	rm -f analisador lex.yy.c
