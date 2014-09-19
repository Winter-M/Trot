
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <Natives.h>
# include <Symbols.h>

# define CASE(v1, v2, T)	\
	if(((v1).type == (T)) || ((v2).type == (T)))

long int __integer_value(pt_value *value) {
	switch(value->type) {
		case PT_DECIMAL: {
			pt_double *decimal = (pt_double *)value;
			return (long int)decimal->decimal;
		}
		case PT_BOOLEAN:
			return (long int)value->u.boolean;
		case PT_INTEGER:
			return value->u.integer;
		case PT_STRING:
			/* TODO */
			return 0;
		case PT_UNDEF:
		case PT_LAMBDA:
		default:
			return 0;
	}
}

double __double_value(pt_value *value) {
	switch(value->type) {
		case PT_DECIMAL: {
			pt_double *decimal = (pt_double *)value;
			return decimal->decimal;
		}
		case PT_BOOLEAN:
			return (double)value->u.boolean;
		case PT_INTEGER:
			return (double)value->u.integer;
		case PT_STRING:
			/* TODO */
			return 0;
		case PT_UNDEF:
		case PT_LAMBDA:
		default:
			return 0;
	}
}

pt_value __operation_add(pt_value *v1, pt_value *v2) {
	pt_value result;

	CASE(*v1, *v2, PT_DECIMAL) {
		pt_double decimal;
		double d1 = __double_value(v1);
		double d2 = __double_value(v2);

		decimal.type = PT_DECIMAL;
		decimal.decimal = d1 + d2;

		memcpy(&result, &decimal, sizeof(pt_value));
	}

	CASE(*v1, *v2, PT_INTEGER) {
		long int i1 = __integer_value(v1);
		long int i2 = __integer_value(v2);

		result.type = PT_DECIMAL;
		result.u.integer = i1 + i2;
	}

	return result;
}

pt_value __native_operator_plus(pt_node *node, int count) {
	extern pt_value NIL, UNDEF;
	extern pt_value visitNode(pt_node *);

	pt_value result = NIL;
	pt_node *args = node->next;
	if(count < 1) return UNDEF;

	if(args != NULL) {
		result = visitNode(args);
		while((args = args->next) != NULL) {
			pt_value operand =  visitNode(args);
			if(operand.type == PT_UNDEF) return UNDEF;
			result = __operation_add(&result, &operand);
			if(result.type == PT_UNDEF) return UNDEF;
		}
	}

	return result;
}

static
pt_native *__create_operator_plus() {
	pt_native *native = createNative(PNT_EXTENSION, -1);
	if(native == NULL) return NULL;

	native->u.extension = __native_operator_plus;
	return native;
}

__attribute__((constructor))
void registerNatives() {
	pt_native *native;

	native = __create_operator_plus();
	if(native == NULL) {
		fprintf(stderr, "Failed to create builtin function.\n");
		exit(EXIT_FAILURE);
	}

	registerNative(native);
	fprintf(stderr, "Registered native.\n");
}
