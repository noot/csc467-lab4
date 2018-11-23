#include "ast.h"
#include "symbol.h"
#include "common.h"

void traverse_and_check();
void ast_semantic_check(node* current, int i);