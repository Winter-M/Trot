# ifndef __SYMBOLS_H__
# define __SYMBOLS_H__

# include <stdbool.h>

# include "Syntax.h"
# include "Lambda.h"

# ifndef __cplusplus
typedef struct pt_value pt_value;
typedef struct pt_double pt_double;

typedef struct pt_context pt_context;
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

pt_context *createContext();

void deleteContext(pt_context *context);

pt_context *pushContext(pt_context *context);

pt_context *popContext(pt_context *context);

void registerSymbol(pt_context *context, char *name, pt_value);

pt_value resolveSymbol(pt_context *context, char *name);

# endif /* Symbols.h */
