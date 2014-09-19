# ifndef __LAMBDA_H__
# define __LAMBDA_H__

# ifndef __cplusplus
typedef enum pt_lambda_type pt_lambda_type;
typedef struct pt_lambda pt_lambda;
# endif /* C++ */

enum pt_lambda_type {
	PLT_UNDEF	= -1,

	PLT_LAMBDA,
	PLT_BUILTIN
};

struct pt_lambda {
	pt_lambda_type type;

	int arg_count;
	char **arguments;

	union {
		pt_node *body;
		void *builtin;
	};
};

pt_lambda *createLambda(pt_lambda_type type);

void deleteLambda(pt_lambda *lambda);

# endif /* Lambda.h */
