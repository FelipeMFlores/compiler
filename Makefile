CC = gcc

build: scanner.l main.c tokens.h
	flex scanner.l
	gcc main.c lex.yy.c -o etapa1 -lfl

run: etapa1
	./etapa1

test: etapa1 in.txt
	./etapa1 < in.txt > out.txt
	diff result.txt out.txt

test2: etapa1 in2.txt
	./etapa1 < in2.txt

clean:
	rm -f etapa1 lex.yy.c out.txt
