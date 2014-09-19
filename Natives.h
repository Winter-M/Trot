# ifndef __NATIVES_H__
# define __NATIVES_H__

# include "Syntax.h"
# include "Symbols.h"

# ifndef __cplusplus
typedef struct pt_native pt_native;
# endif /* C++ */

typedef pt_value (* pt_native_function)(pt_value *values, int count);
typedef pt_value (* pt_native_extension)(pt_node *node, int count);

typedef enum {
	PNT_UNDEF = -1,

	PNT_FUNCTION,
	PNT_EXTENSION
} pt_native_type;

struct pt_native {
	pt_native_type type;
	void *lambda;
	char *name;

	union {
		pt_native_function function;
		pt_native_extension extension;
	} u;
};

pt_native *createNative(pt_native_type type, int argc);

void deleteNative(pt_native *native);

void registerNative(pt_native *native);

pt_native *resolveNative(char *name);

pt_value invokeNative(pt_native *native, pt_node *node);

# endif /* Natives.h */
