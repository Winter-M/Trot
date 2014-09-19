
# include <stdlib.h>
# include <string.h>

# include "Symbols.h"

# ifndef __cplusplus
typedef struct pt_context_node pt_context_node;
typedef struct pt_context_elem pt_context_elem;
# endif /* C++ */

struct pt_context {
	pt_context_node *head;
	pt_context_node *tail;
};

struct pt_context_node {
	pt_context_node *next;
	pt_context_node *prev;

	pt_context_elem *head;
	pt_context_elem *tail;
};

struct pt_context_elem {
	long int hash;
	pt_value value;

	pt_context_elem *next;
};

pt_context *createContext() {
	pt_context_node *createContextNode();

	pt_context_node *node;
	pt_context *context = malloc(sizeof(pt_context));
	if(context == NULL) return NULL;

	node = createContextNode();
	context->head = context->tail = node;

	return context;
}

void deleteContext(pt_context *context) {
	void deleteContextNode(pt_context_node *);
	if(context == NULL) return;
	if(context->head != NULL)
		deleteContextNode(context->head);
	free(context);
}

pt_context_node *createContextNode() {
	pt_context_node *node = malloc(sizeof(pt_context_node));
	if(node == NULL) return NULL;

	node->next = node->prev = NULL;
	node->head = node->tail = NULL;

	return node;
}

void deleteContextNode(pt_context_node *node) {
	void deleteContextElem(pt_context_elem *);
	if(node == NULL) return;
	if(node->next != NULL) deleteContextNode(node->next);
	if(node->head != NULL) deleteContextElem(node->head);
	free(node);
}

pt_context_elem *createContextElem(pt_value value) {
	pt_context_elem *elem = malloc(sizeof(pt_context_elem));
	if(elem == NULL) return NULL;

	elem->next = NULL;
	memcpy(&elem->value, &value, sizeof(pt_value));

	return elem;
}

void deleteContextElem(pt_context_elem *elem) {
	if(elem == NULL) return;
	if(elem->next != NULL)
		deleteContextElem(elem->next);
	free(elem);
}

pt_context *pushContext(pt_context *context) {
	pt_context_node *node;
	if(context == NULL) return NULL;

	node = createContextNode();
	if(node == NULL) return NULL;

	if(context->tail == NULL) {
		context->head = context->tail = NULL;
	} else {
		context->tail->next = node;
		context->tail = node;
	}

	return context;
}

pt_context *popContext(pt_context *context) {
	if(context == NULL) return NULL;

	if(context->tail->prev != NULL) {
		pt_context_node *node = context->tail;
		context->tail = node->prev;
		context->tail->next = NULL;
		deleteContextNode(node);
	}

	return NULL;
}

static
long int computeHash(char *name) {
	int hash = 7;
	while(*name)
		hash += 17 * *name++;
	return hash;
}

static
pt_value *contextElement(pt_context *context, char *name) {
	extern pt_value UNDEF;

	pt_context_node *node = context->tail;
	pt_context_elem *elem = node->head;
	int hash = computeHash(name);

	while(elem != NULL) {
		if(elem->hash == hash) {
			return &elem->value;
		}
	}

	elem = createContextElem(UNDEF);
	if(elem == NULL) return NULL;

	elem->hash = hash;
	if(node->tail != NULL) {
		node->head = node->tail = elem;
	} else {
		node->tail->next = elem;
		node->tail = elem;
	}

	return &elem->value;
}

void registerSymbol(pt_context *context, char *name, pt_value value) {
	pt_value *value_ref;
	if(context == NULL || name == NULL) return;

	value_ref = contextElement(context, name);
	if(value_ref == NULL) return;

	memcpy(value_ref, &value, sizeof(pt_value));
}

pt_value resolveSymbol(pt_context *context, char *name) {
	extern pt_value UNDEF;

	pt_context_node *node = context->tail;
	int hash = computeHash(name);

	while(node != NULL) {
		pt_context_elem *elem = node->head;

		while(elem != NULL) {
			if(elem->hash == hash) {
				return elem->value;
			}
		}

		node = node->prev;
	}

	return UNDEF;
}
