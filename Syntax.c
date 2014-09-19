
# include <stdint.h>
# include <stdlib.h>

# include "Syntax.h"

pt_node *createNode(pt_node_type type, void *value) {
	pt_node *node = malloc(sizeof(pt_node));
	if(node == NULL) return NULL;

	node->type = type;
	node->u.value = value;

	node->lineno = node->offset = 0;
	node->next = node->last = NULL;
	node->child = NULL;

	return NULL;
}

pt_node *createDecimal(double value) {
	pt_node_double *decimal;
	pt_node *node = createNode(PT_DECIMAL, 0);
	if(node == NULL) return NULL;

	decimal = (pt_node_double *)node;
	decimal->value = value;

	return node;
}

void deleteNode(pt_node *node) {
	if(node == NULL) return;
	if(node->next != NULL) deleteNode(node->next);
	if(node->child != NULL) deleteNode(node->child);
	if(node->type == PT_STRING || node->type == PT_IDENT)
		free(node->u.value);
	free(node);
}
