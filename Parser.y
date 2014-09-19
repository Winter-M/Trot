
%{
# include <stdio.h>

# include "Syntax.h"
# include "Parser.h"
# include "Lexer.h"

int yyerror(pt_node **node, yyscan_t scanner, const char *msg) {
	fprintf(stderr, "Error : %s.\n", msg);
	return 0;
}
%}

%code requires {
# ifndef YY_TYPEDEF_YY_SCANNER_T
#	define YY_TYPEDEF_YY_SCANNER_T
	typedef void *yyscan_t;
# endif
}

%output  "Parser.c"
%defines "Parser.h"

%define api.pure
%lex-param		{ yyscan_t scanner }
%parse-param	{ pt_node **node }
%parse-param	{ yyscan_t scanner }

%union {
	int boolean;
	long integer;
	double decimal;
	char *string;

	pt_node *node;
}

%token TOKEN_NULL
%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_LAMBDA

%type <node> expression
%type <node> expression_list

%token <string> TOKEN_IDENT
%token <string> TOKEN_STRING
%token <decimal> TOKEN_FLOAT
%token <integer> TOKEN_INTEGER
%token <boolean> TOKEN_BOOLEAN

%%

compilationUnit
	: expression_list {
		*node = $1;
	}
	;

expression
	: TOKEN_LPAREN expression_list TOKEN_RPAREN {
		$$ = createExpr();
		$$->child = $2;
	}
	| TOKEN_LPAREN TOKEN_LAMBDA expression_list TOKEN_RPAREN {
		$$ = createExpr();
		$$->type = PT_LAMBDA;
		$$->child = $3;
	}
	| TOKEN_BOOLEAN {
		$$ = createBoolean($1);
	}
	| TOKEN_INTEGER {
		$$ = createInteger($1);
	}
	| TOKEN_FLOAT {
		$$ = createDecimal($1);
	}
	| TOKEN_STRING {
		$$ = createString($1);
	}
	| TOKEN_IDENT {
		$$ = createIdent($1);
	}
	;

expression_list
	: expression_list expression {
		$$ = $1;
		$1->last->next = $2;
		$1->last = $2;
		$1->count++;
	}
	| expression {
		$$ = $1;
		$1->last = $1;
		$1->count = 1;
	}
	;

%%
