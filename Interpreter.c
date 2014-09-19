
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>

# include "Syntax.h"
# include "Lambda.h"

# ifndef __cplusplus
typedef struct pt_value pt_value;
typedef struct pt_double pt_double;
# endif /* C++ */

struct pt_value {
	pt_node_type type;

	union {
		bool boolean;
		char *string;
		long int integer;

		pt_lambda *lambda;
		pt_node *expression;

		void *value;
		char pad[sizeof(double)];
	};
};

struct pt_double {
	pt_node_type type;
	double decimal;
};

pt_value ZERO = { type : PT_INTEGER, value : 0 };
pt_value NIL  = { type : PT_UNDEF,   value : 0 };

pt_value visitTerminal(pt_node *node) {
	pt_value value = { type : node->type, value : node->value };
	return value;
}

pt_value visitDecimal(pt_node *node) {
	pt_value value;
	pt_node_double *nd = (pt_node_double *)node;
	pt_double decimal = { type : PT_DECIMAL, nd->value };
	memcpy(&value, &decimal, sizeof(pt_node));
	return value;
}

pt_value visitLambda(pt_node *node) {
	int idx;
	pt_value value;
	pt_lambda *lambda;
	pt_node *keyword, *args, *body;
	if(node->count != 3) {
		/* TODO */
		return NIL;
	}

	body = (args = (keyword = node)->next)->next;
	if(args->type != PT_EXPR || body->type != PT_EXPR) {
		/* TODO */
		return NIL;
	}

	lambda = malloc(sizeof(pt_lambda));
	if(lambda == NULL) {
		/* TODO */
		return NIL;
	}

	args = args->child;
	lambda->arg_count = keyword->count;
	lambda->arguments = malloc(sizeof(char *) * lambda->arg_count);
	if(lambda->arguments == NULL) {
		/* TODO */
		free(lambda);
		return NIL;
	}

	for(idx = 0; idx < lambda->arg_count; idx++) {
		if(args->type != PT_IDENT) {
			/* TODO */
			free(lambda->arguments);
			free(lambda);
			return NIL;
		}

		pt_value name = visitTerminal(args);
		lambda->arguments[idx] = name.string;
	}

	value.type = PT_LAMBDA;
	value.lambda = lambda;

	return value;
}

pt_value visitIdentifier(pt_node *node) {
	return ZERO;
}

pt_value visitExpression(pt_node *node) {
	return ZERO;
}

pt_value visitNode(pt_node *node) {
	switch(node->type) {
		case PT_EXPR:
			return visitExpression(node);
		case PT_IDENT:
			return visitIdentifier(node);
		case PT_LAMBDA:
			return visitLambda(node);
		case PT_DECIMAL:
			return visitDecimal(node);
		default:
			return visitTerminal(node);
	}
}

