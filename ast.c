#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "ast.h"
#include "common.h"
#include "parser.tab.h"

#define DEBUG_PRINT_TREE 0

node *ast = NULL;

node *ast_allocate(node_kind kind, ...) {
  va_list args;

  // make the node
  node *ast = (node *) malloc(sizeof(node));
  memset(ast, 0, sizeof *ast);
  ast->kind = kind;

  va_start(args, kind); 

  switch(kind) {
    case SCOPE_NODE:
      ast->scope.declarations = va_arg(args, node *);
      ast->scope.statements = va_arg(args, node *);
      break;
    case DECLARATIONS_NODE:
      ast->scope.declarations = va_arg(args, node *);
      ast->scope.declaration = va_arg(args, node *);
      break;
    case STATEMENTS_NODE:
    case DECLARATION_NODE:
    case STATEMENT_NODE:
    case ELSE_STATEMENT_NODE:
    case TYPE_NODE:
    case EXP_NODE:

    case INT_NODE:
    case FLOAT_NODE:
    case BOOL_NODE:
    case VAR_NODE:
    
    case UNARY_OP_NODE:
    case CONSTRUCTOR_NODE:
    case FUNCTION_NODE:
    case FUNCTION_NAME_NODE:
    case ARGUMENTS_OPT_NODE:
    case ARGUMENTS_NODE:
    // case BINARY_EXPRESSION_NODE:
    //   ast->binary_expr.op = va_arg(args, int);
    //   ast->binary_expr.left = va_arg(args, node *);
    //   ast->binary_expr.right = va_arg(args, node *);
    //   break;
    default: break;
  }

  va_end(args);

  return ast;
}

void ast_free(node *ast) {

}

void ast_print(node * ast) {

}
