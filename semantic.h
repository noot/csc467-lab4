#ifndef SEMANTIC_H_
#define SEMANTIC_H_

#include "ast.h"
#include "symbol.h"
#include "common.h"
#include "parser.tab.h"

void traverse_and_check();
void ast_semantic_check(node *current, int i);

#endif