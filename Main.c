
# include "Syntax.h"
# include "Parser.h"
# include "Lexer.h"

pt_node *buildAST(const char *expr) {
	extern int yyparse(pt_node **, yyscan_t);

	FILE *file;
	pt_node *node;
	yyscan_t scanner;
	YY_BUFFER_STATE state;

	if(yylex_init(&scanner)) {
		fprintf(stderr, "Failed to initialize parser.\n");
		exit(-1);
	}

	file = fopen(expr, "r");
	if(file == NULL) {
		perror("fopen");
		exit(-1);
	}

	state = yy_create_buffer(file, 1024, scanner);
	yy_switch_to_buffer(state, scanner);

	if(yyparse(&node, scanner)) {
		fprintf(stderr, "Error parsing input.\n");
		exit(-1);
	}

	yy_delete_buffer(state, scanner);
	yylex_destroy(scanner);

	return node;
}

int main() {
	return 0;
}
