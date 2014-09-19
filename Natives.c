
# include <stdlib.h>
# include <string.h>

# include "Natives.h"

# ifndef __cplusplus
typedef struct pt_native_list pt_native_list;
typedef struct pt_native_node pt_native_node;
# endif /* C++ */

struct pt_native_list {
	pt_native_node *head;
	pt_native_node *tail;
};

struct pt_native_node {
	pt_native *native;
	pt_native_node *next;
};

pt_native_list natives_list;

static
pt_native_node *createNativeNode(pt_native *native) {
	pt_native_node *node = malloc(sizeof(pt_native_node));
	if(node == NULL) return NULL;

	node->next = NULL;
	node->native = native;

	return node;
}

static
void deleteNativeNode(pt_native_node *node) {
	if(node == NULL) return;
	if(node->next != NULL) deleteNativeNode(node);
	if(node->native != NULL) deleteNative(node->native);
	free(node);
}

pt_native *createNative(pt_native_type type) {
	pt_native *native = malloc(sizeof(pt_native));
	if(native == NULL) return NULL;

	native->type = type;
	native->name = NULL;
	native->u.function = NULL;

	return native;
}

void deleteNative(pt_native *native) {
	if(native == NULL) return;
	free(native);
}

void registerNative(pt_native *native) {
	pt_native_node *node;
	if(native == NULL) return;

	node = createNativeNode(native);
	if(node == NULL) return;

	if(natives_list.tail == NULL) {
		natives_list.head = natives_list.tail = node;
	} else {
		natives_list.tail->next = node;
		natives_list.tail = node;
	}
}

pt_native *resolveNative(char *name) {
	pt_native_node *node;
	if(name == NULL) return NULL;

	node = natives_list.head;
	while(node != NULL) {
		if(!strcmp(name, node->native->name)) 
			return node->native;
		node = node->next;
	}

	return NULL;
}

pt_value invokeNative(pt_native *native, pt_node *node) {
	extern pt_value UNDEF;
	extern pt_value visitNode(pt_node *node);
	extern pt_value visitTerminal(pt_node *node);

	if(native == NULL || node == NULL) return UNDEF;

	switch(native->type) {
		case PNT_FUNCTION: {
				pt_node *args = node->next;
				int idx, count = node->count;
				pt_value *values = malloc(sizeof(pt_value) * count);
				if(values == NULL) return UNDEF;

				values[0] = visitTerminal(node);
				for(idx = 1; idx < count; idx++) {
					values[idx] = visitNode(args);
					if(values[idx].type == PT_UNDEF) {
						/* TODO */
						free(values);
						return UNDEF;
					}

					args = args->next;
				}

				return native->u.function(values, count);
			}
		case PNT_EXTENSION:
			return native->u.extension(node, node->count);
		default:
			/* TODO */
			return UNDEF;
	}
}
