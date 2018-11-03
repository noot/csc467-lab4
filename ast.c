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
      ast->statements.statements = va_arg(args, node *);
      ast->statements.statement = va_arg(args, node *);
      break;

    case DECLARATION_NODE:
      ast->declaration.is_const = va_arg(args, bool);
      ast->declaration.id = va_arg(args, char *);
      ast->declaration.type = va_arg(args, node *);
      ast->declaration.exp = va_arg(args, node *);
      break;

    // might need to break up statement grammar into assignment,
    // if statement, nested scope
    case STATEMENT_NODE:
      ast->statement.is_if = va_arg(args, bool);
      ast->statement.variable = va_arg(args, node *);
      ast->statement.exp = va_arg(args, node *);
      ast->statement.statement = va_arg(args, node *);
      ast->statement.else_statement = va_arg(args, node *);
      break;

    case ELSE_STATEMENT_NODE:
      ast->else_statement.statement = va_arg(args, node *);
      break;

    case TYPE_NODE:
      ast->type.type_name = va_arg(args, int);
      ast->type.vec = va_arg(args, int); // how to do this?
      break;

    // expressions
    case EXP_NODE:
      ast->exp.variable = va_arg(args, node *);
      break;

    case INT_NODE:
      ast->type.is_const = false;
      ast->type.type_name = INT_T;
      ast->type.vec = false;
      ast->type.int_val = va_arg(args, int);
      break;

    case FLOAT_NODE:
      ast->type.is_const = false;
      ast->type.type_name = FLOAT_T;
      ast->type.vec = false;
      ast->type.int_val = va_arg(args, int);
      break;

    case BOOL_NODE:
      ast->type.is_const = false;
      ast->type.type_name = BOOL_T;
      ast->type.vec = false;
      ast->type.bool_val = va_arg(args, bool);
      break;

    case VAR_NODE:
      ast->variable.id = va_arg(args, char *);
      ast->variable.is_vec = va_arg(args, bool);
      ast->variable.idx = va_arg(args, int);
      break;

    case BINARY_OP_NODE:
      ast->binary_expr.op = va_arg(args, int);
      ast->binary_expr.left = va_arg(args, node *);
      ast->binary_expr.right = va_arg(args, node *);
      break;

    case UNARY_OP_NODE:
      ast->unary_expr.op = va_arg(args, int);
      ast->unary_expr.right = va_arg(args, node *);
      break;

    case CONSTRUCTOR_NODE:
      ast->constructor.type = va_arg(args, node *);
      ast->constructor.args = va_arg(args, node *);
      break;

    case FUNCTION_NODE:
      ast->function.function_name = va_arg(args, int);
      ast->function.args = va_arg(args, node *);
      break;

    case ARGUMENTS_OPT_NODE:
      ast->arguments_opt.args = va_arg(args, node *);
      break;

    case ARGUMENTS_NODE:
      ast->arguments.args = va_arg(args, node *);
      ast->arguments.exp = va_arg(args, node *);
      break;

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
