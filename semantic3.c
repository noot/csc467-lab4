#include <stdio.h>
#include "semantic.h"
#include "ast.h"

void ast_semantic_check(node* current) {

	if (current == NULL) {
		errorOccurred = 1;
		fprintf(errorFile, "Error: Empty tree visited");
		return;
	}
	node_kind kind = current->kind;

	switch (kind) {
		case(SCOPE_NODE): scope_exit(); break;
		case(DECLARATIONS_NODE): break;
		case(STATEMENTS_NODE): break;
	}
}
