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
      ast->declarations.declarations = va_arg(args, node *);
      ast->declarations.declaration = va_arg(args, node *);
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

//do preorder traversal for generating symbol tree
void traverse_ast(node *ast, bool enterScope) {
	switch (node.kind) {
	case(scope_node):
		traverse_ast(node->scope_node,enterScope);
	case(declarations):
		traverse_ast(node->declarations, enterScope);
	case(statements):
		traverse_ast(node->statements, enterScope);
	case(declaration):
		traverse_ast(node->statement, enterScope);
	case(statement):
		traverse_ast(scope_node, enterScope);
	case(else_statement):check_condition();
	case(exp):
	case(var):check_var_scope();
	case(unary_op):
	case(binary_op): 
		traverse_ast(node->binary_op.right, enterScope);
		traverse_ast(node->binary_op.left, enterScope);
	case(constructor): check_constructor();
	case(function): check_function();
	case(function_name): check_func_name();
	case(arguments_opt):
	case(arguments):
	case(int_node):
	case(float_node):
	case(indent):
	case(type):
	case(bool_node):
	case(assignment):
	case(nested_scope):
	}

	
}

void ast_free(node *ast) {

}

//do post order traversal 
void ast_print(node * ast) {
	struct node
	{
		int data;
		struct node* left;
		struct node* right;
	};

	struct node* newNode(int data)
	{
		struct node* node = (struct node*)
			malloc(sizeof(struct node));
		node->data = data;
		node->left = NULL;
		node->right = NULL;

		return(node);

}
