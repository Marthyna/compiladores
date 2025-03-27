target: etapa1

etapa1: lex.yy.c
	gcc lex.yy.c -o etapa1

lex.yy.c: scanner.l
	flex scanner.l

clean:
	rm etapa1 lex.yy.c