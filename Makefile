
# -Wno-unused-X flags are necessary since flex and bison code triggers these warnings
CFLAGS = -O2 -Wall -Werror -Wextra -Wno-unused-parameter -Wno-unused-function -ansi

all: test

clean:
	rm -f Lexer.[ch] Parser.[ch] *.o

test: Lexer.o Parser.o Syntax.o Main.o

Lexer.o: Lexer.c Parser.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c -o Lexer.o Lexer.c

Lexer.h Lexer.c: Lexer.l
	flex Lexer.l

Parser.h Parser.c: Parser.y Lexer.c
	bison Parser.y
