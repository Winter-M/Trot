
# include "Syntax.h"
# include "Parser.h"
# include "Lexer.h"

# include "Interpreter.h"

pt_node *buildAST(const char *expr) {
	extern int yyparse(pt_node **, yyscan_t);

	FILE *file;
	yyscan_t scanner;
	pt_node *node = NULL;
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

int main(int argc, char **argv) {
	pt_node *node;
	if(argc < 2) {
		fprintf(stderr, "Argument required.\n");
		return -1;
	}

	node = buildAST(argv[1]);
	return visitRoot(node);
}
