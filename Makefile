
# -Wno-unused-X flags are necessary since flex and bison code triggers these warnings
CFLAGS = -O2 -Wall -Werror -Wextra -Wno-unused-parameter -Wno-unused-function -ansi -pedantic
CPPFLAGS = -I .
LDFLAGS = -O2 -Wall -Werror -Wextra

objects = Lexer.o Parser.o Syntax.o Interpreter.o Lambda.o Symbols.o Natives.o Main.o
natives = natives/Builtin.o

all: test

clean:
	rm -f Lexer.[ch] Parser.[ch] *.o natives/*.o

test: $(objects) $(natives)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c %.h Parser.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c -o $@ $<

natives/%.o: natives/%.c natives/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c -o $@ $<

Lexer.o: Lexer.c Parser.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c -o Lexer.o Lexer.c

Lexer.h Lexer.c: Lexer.l Syntax.h
	flex Lexer.l

Parser.h Parser.c: Parser.y Lexer.c Syntax.h
	bison Parser.y
