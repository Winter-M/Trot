
all: test

clean:
	rm -f Lexer.[ch] Parser.[ch]

test: Lexer.c Parser.c

Lexer.c: Lexer.l
	flex Lexer.l

Parser.c: Parser.y Lexer.c
	bison Parser.y
