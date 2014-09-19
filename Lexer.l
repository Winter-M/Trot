
%{
# include <stdio.h>
# include <string.h>

# include "Syntax.h"
# include "Parser.h"
%}

%option outfile = "Lexer.c" header-file = "Lexer.h"
%option warn nodefault

%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

LPAREN		"("
RPAREN		")"
LAMBDA		@|lambda

NULL		null
BOOLEAN		true|false
INTEGER		[-]?[0-9]+
FLOAT		[-]?[0-9]+[.][0-9]+
STRING		["][^"]*["]
IDENTIFIER	[a-zA-Z!@#$%^&*_+\-={|}~:;<>./?][0-9a-zA-Z!@#$%^&*_+\-={|}~:;<>./?]*

WHITESPACE	[ \r\n\t]*

%%

{WHITESPACE}	{ /* Skip */ }
{NULL}			{ return TOKEN_NULL; }
{LAMBDA}		{ return TOKEN_LAMBDA; }

{BOOLEAN}		{
	char buf[8];
	sscanf(yytext, "%s", buf);
	yylval->boolean = !strcmp(buf, "true");
	return TOKEN_BOOLEAN;
}

{INTEGER}		{
	sscanf(yytext, "%ld", &yylval->integer);
	return TOKEN_INTEGER;
}

{FLOAT}			{
	sscanf(yytext, "%lf", &yylval->decimal);
	return TOKEN_FLOAT;
}

{STRING}		{
	yylval->string = malloc(256);
	sscanf(yytext, "\"%s\"", yylval->string);
	return TOKEN_STRING;
}

{IDENTIFIER}	{
	yylval->string = malloc(256);
	sscanf(yytext, "%s", yylval->string);
	return TOKEN_IDENT;
}

{LPAREN}		{ return TOKEN_LPAREN; }
{RPAREN}		{ return TOKEN_RPAREN; }
.				{  }

%%
