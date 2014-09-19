
# include <stdlib.h>

# include "Syntax.h"
# include "Lambda.h"

pt_lambda *createLambda(pt_lambda_type type) {
	pt_lambda *lambda = malloc(sizeof(pt_lambda));
	if(lambda == NULL) return NULL;

	lambda->type = type;
	lambda->arg_count = 0;
	lambda->arguments = NULL;
	lambda->body = NULL;

	return lambda;
}

void deleteLambda(pt_lambda *lambda) {
	if(lambda == NULL) return;
	if(lambda->type == PLT_LAMBDA)
		deleteNode(lambda->body);
	free(lambda);
}
