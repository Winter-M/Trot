
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "Lambda.h"
# include "Syntax.h"
# include "Symbols.h"
# include "Interpreter.h"

pt_context *context;

pt_value UNDEF = { type : PT_UNDEF, value : 0 };
pt_value NIL   = { type : PT_NIL,   value : 0 };

__attribute__((constructor))
void prepareInterpreter() {
	context = createContext();
	if(context == NULL) {
		/* TODO */
		exit(-1);
	}
}

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

	body = (args = (keyword = node)->next)->next;
	if(args->count != 2) {
		fprintf(stderr, "Lambda declarations must have 2 arguments, %d found.\n", node->count);
		return UNDEF;
	}

	if(args->type != PT_EXPR || body->type != PT_EXPR) {
		fprintf(stderr, "Argument type mismatch.\n");
		return UNDEF;
	}

	lambda = malloc(sizeof(pt_lambda));
	if(lambda == NULL) {
		perror("malloc");
		return UNDEF;
	}

	args = args->child;
	lambda->arg_count = keyword->count;
	lambda->arguments = malloc(sizeof(char *) * lambda->arg_count);
	if(lambda->arguments == NULL) {
		perror("malloc");
		free(lambda);
		return UNDEF;
	}

	for(idx = 0; idx < lambda->arg_count; idx++) {
		if(args->type != PT_IDENT) {
			/* TODO */
			free(lambda->arguments);
			free(lambda);
			return UNDEF;
		}

		pt_value name = visitTerminal(args);
		lambda->arguments[idx] = name.string;
	}

	value.type = PT_LAMBDA;
	value.lambda = lambda;

	return value;
}

pt_value visitIdentifier(pt_node *node) {
	pt_value name = visitTerminal(node);
	return resolveSymbol(context, name.string);
}

pt_value visitExpression(pt_node *node) {
	int idx;
	pt_value value;
	pt_lambda *lambda;
	pt_node *ident = node;
	pt_node *args = ident->next;

	value = visitNode(ident);
	if(value.type != PT_LAMBDA) {
		/* TODO */
		return UNDEF;
	}

	lambda = value.lambda;
	if(lambda->arg_count != -1 &&
			lambda->arg_count != ident->count - 1) {
		/* TODO */
		return UNDEF;
	}

	switch(lambda->type) {
		case PLT_LAMBDA:
			pushContext(context);
			for(idx = 0; idx < lambda->arg_count; idx++) {
				char *name = lambda->arguments[idx];
				pt_value value = visitNode(args);
				if(value.type == PT_UNDEF) {
					/* TODO */
					return UNDEF;
				}

				registerSymbol(context, name, value);
				args = args->next;
			}

			value = visitNode(lambda->body);
			popContext(context);
			break;
		case PLT_BUILTIN:
			/* TODO */
			break;
		default:
			/* TODO */
			break;
	}

	return value;
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

