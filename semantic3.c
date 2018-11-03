#include <stdio.h>
#include "semantic.h"
#include "ast.h"

void ast_check_semantics() {
	if (DEBUG_SEMANTIC) {
		printf("Semantic check starting\n");
		int i = 0;
		for (i = 0; i < 20; i++) {
			printf("-");
		}
		printf("\n");
	}

	if (ast == NULL) {
		errorOccurred = 1;
		fprintf(errorFile, "Main scope not found.\n");
		return;
	}
	else {
		ast_traverse(ast, 0, &ast_scope_generator, &ast_sementic_check);

	}

}
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

void ast_scope_generator(node *cur, int x) { //Done pre-post.

	if (cur->kind == SCOPE_NODE) {
		scope_enter();
	}
}
