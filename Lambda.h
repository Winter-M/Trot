# ifndef __LAMBDA_H__
# define __LAMBDA_H__

# include "Syntax.h"

# ifndef __cplusplus
typedef struct pt_lambda pt_lambda;
# endif /* C++ */

typedef enum {
	PLT_UNDEF	= -1,

	PLT_LAMBDA,
	PLT_BUILTIN
} pt_lambda_type;

struct pt_lambda {
	pt_lambda_type type;

	int arg_count;
	char **arguments;

	union {
		pt_node *body;
		void *builtin;
	} u;
};

pt_lambda *createLambda(pt_lambda_type type);

void deleteLambda(pt_lambda *lambda);

# endif /* Lambda.h */
