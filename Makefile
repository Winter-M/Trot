
# -Wno-unused-X flags are necessary since flex and bison code triggers these warnings
CFLAGS = -O2 -Wall -Werror -Wextra -Wno-unused-parameter -Wno-unused-function -ansi -pedantic

all: test

clean:
	rm -f Lexer.[ch] Parser.[ch] *.o

test: Lexer.o Parser.o Syntax.o Interpreter.o Lambda.o Symbols.o Natives.o Main.o

%.o: %.c %.h Parser.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c -o $@ $<

Lexer.o: Lexer.c Parser.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c -o Lexer.o Lexer.c

Lexer.h Lexer.c: Lexer.l Syntax.h
	flex Lexer.l

Parser.h Parser.c: Parser.y Lexer.c Syntax.h
	bison Parser.y
