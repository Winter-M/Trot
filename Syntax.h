# ifndef __SYNTAX_H__
# define __SYNTAX_H__

# ifndef __cplusplus
# include <stdint.h>
typedef uint32_t off_t;
typedef uint64_t off64_t;

typedef struct pt_node pt_node;
typedef enum pt_node_type pt_node_type;
typedef struct pt_node_double pt_node_double;
# endif /* C++ */

enum pt_node_type {
	PT_UNDEF = -1,

	PT_NIL,

	PT_BOOLEAN,
	PT_INTEGER,
	PT_DECIMAL,

	PT_STRING,
	PT_IDENT,

	PT_LAMBDA,
	PT_EXPR
};

struct pt_node {
	pt_node_type type;
	union {
		void *value;
		char pad[sizeof(double)];
	};

	int lineno, offset;
	pt_node *next, *last;

	int count;
	pt_node *child;
};

struct pt_node_double {
	pt_node_type type;
	double value;

	int lineno, offset;
	pt_node *next, *last;

	int count;
	pt_node *child;
};

pt_node *createNode(pt_node_type type, void *value);
# define createBoolean(N)	createNode(PT_BOOLEAN, (void *)(N))
# define createInteger(N)	createNode(PT_INTEGER, (void *)(N))
# define createString(N)	createNode(PT_STRING, (void *)(N))
# define createIdent(N)		createNode(PT_IDENT, (void *)(N))
# define createExpr()		createNode(PT_EXPR, (void *)(0))
pt_node *createDecimal(double value);

void deleteNode(pt_node *node);

# endif /* Syntax.h */
