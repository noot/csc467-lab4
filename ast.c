#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "ast.h"
#include "common.h"
#include "parser.tab.h"
#include "semantic.h"

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
      //ast->declarations.declarations = va_arg(args, node *);
      ast->declarations.declaration = va_arg(args, node *);
      break;

    case STATEMENTS_NODE:
      ast->statements.statements = va_arg(args, node *);
      ast->statements.statement = va_arg(args, node *);
      break;

    case DECLARATION_NODE:
      ast->declaration.is_const = va_arg(args, int);
      ast->declaration.id = va_arg(args, char *);
      ast->declaration.type = va_arg(args, node *);
      ast->declaration.exp = va_arg(args, node *);
      break;

    case STATEMENT_NODE:
      ast->statement.is_if = va_arg(args, int);
      ast->statement.variable = va_arg(args, node *);
      ast->statement.exp = va_arg(args, node *);
      ast->statement.statement = va_arg(args, node *);
      ast->statement.else_statement = va_arg(args, node *);
      break;

    case ASSIGNMENT_NODE:
      ast->assignment.variable = va_arg(args, node *);
      ast->assignment.exp = va_arg(args, node *);
      break;

    case ELSE_STATEMENT_NODE:
      ast->else_statement.statement = va_arg(args, node *);
      break;

    case TYPE_NODE:
      ast->type.type_name = va_arg(args, int);
      ast->type.vec = va_arg(args, int);
      break;

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
      ast->type.bool_val = va_arg(args, int);
      break;

    case VAR_NODE:
      ast->variable.id = va_arg(args, char *);
      ast->variable.is_vec = va_arg(args, int);
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

    // case ARGUMENTS_OPT_NODE:
    //   ast->arguments_opt.args = va_arg(args, node *);
    //   break;

    case ARGUMENTS_NODE:
      ast->arguments.args = va_arg(args, node *);
      ast->arguments.exp = va_arg(args, node *);
      break;

    case NESTED_SCOPE_NODE:
      ast->nested_scope = va_arg(args, node *);

    default: break;
  }

  va_end(args);

  return ast;
}

void ast_visit(int depth, node *curr, func pre, func post) {
  if (NULL == curr) return;

  depth++;
  //fprintf(dumpFile, "%d\n", depth);
  if(pre) pre(curr, depth);

  // in the traversal, we recursively check if each node contains another node,
  // if so, visit that node.
  switch(curr->kind) {
    case SCOPE_NODE:
      if(curr->scope.declarations) ast_visit(depth, curr->scope.declarations, pre, post);
      if(curr->scope.statements) ast_visit(depth, curr->scope.statements, pre, post);
      break;

    case DECLARATIONS_NODE:
      if(curr->declarations.declarations) ast_visit(depth, curr->declarations.declarations, pre, post); 
      if(curr->declarations.declaration) ast_visit(depth, curr->declarations.declaration, pre, post);
      break;

    case STATEMENTS_NODE:
      if(curr->statements.statements) ast_visit(depth, curr->statements.statements, pre, post); 
      if(curr->statements.statement) ast_visit(depth, curr->statements.statement, pre, post); 
      break;

    case DECLARATION_NODE:
      if(curr->declaration.type) ast_visit(depth, curr->declaration.type, pre, post); 
      if(curr->declaration.exp) ast_visit(depth, curr->declaration.exp, pre, post); 
      break;

    case STATEMENT_NODE:
      if(curr->statement.variable) ast_visit(depth, curr->statement.variable, pre, post); 
      if(curr->statement.exp) ast_visit(depth, curr->statement.exp, pre, post); 
      if(curr->statement.statement) ast_visit(depth, curr->statement.statement, pre, post); 
      if(curr->statement.else_statement) ast_visit(depth, curr->statement.else_statement, pre, post); 
      break;

    case ELSE_STATEMENT_NODE:
      if(curr->else_statement.statement) ast_visit(depth, curr->else_statement.statement, pre, post); 
      break;

    case ASSIGNMENT_NODE:
      if(curr->assignment.variable) ast_visit(depth, curr->assignment.variable, pre, post);
      if(curr->assignment.exp) ast_visit(depth, curr->assignment.exp, pre, post);
      break;

    case EXP_NODE:
      if(curr->exp.variable) ast_visit(depth, curr->exp.variable, pre, post); 
      break;

    case UNARY_OP_NODE:
      if(curr->unary_expr.right) ast_visit(depth, curr->unary_expr.right, pre, post); 
      break;

    case BINARY_OP_NODE:
      if(curr->binary_expr.right) ast_visit(depth, curr->binary_expr.right, pre, post); 
      if(curr->binary_expr.left) ast_visit(depth, curr->binary_expr.left, pre, post); 
      break;

    case CONSTRUCTOR_NODE:
      if(curr->constructor.type) ast_visit(depth, curr->constructor.type, pre, post); 
      if(curr->constructor.args) ast_visit(depth, curr->constructor.args, pre, post); 
      break;

    case FUNCTION_NODE:
      if(curr->function.args) ast_visit(depth, curr->function.args, pre, post); 
      break;

    // case ARGUMENTS_OPT_NODE:
    //   if(curr->arguments_opt.args) ast_visit(depth, curr->arguments_opt.args, pre, post); 
    //   break;

    case ARGUMENTS_NODE:
      if(curr->arguments.args) ast_visit(depth, curr->arguments.args, pre, post); 
      if(curr->arguments.exp) ast_visit(depth, curr->arguments.exp, pre, post); 
      break;

    case NESTED_SCOPE_NODE:
      ast_visit(depth, curr->nested_scope, pre, post);
      break;

    default:
      break;
  }

  if(post) post(curr, depth);
  depth--;

  //printf("finished visiting ast");
}

void _ast_free(node *ast, int i) {
  //if(ast->kind == DECLARATION_NODE) free(ast->declaration.id);
  //else if (ast->kind == VAR_NODE) free(ast->variable.id);
  free(ast);
}

void ast_free(node *ast) {
  ast_visit(0, ast, NULL, &_ast_free);
}

void _ast_print(node *curr, int i) {
  for(int j = 1; j < i; j++) fprintf(dumpFile, "\t");
  fprintf(dumpFile, "(");

  if(!curr) return;

  if((unsigned long long)curr <= 10) {
    printf("Invalid curr val: %p\n", curr);
  }

  switch(curr->kind) {
    case SCOPE_NODE:
      fprintf(dumpFile, "SCOPE\n");
      break;

    case DECLARATIONS_NODE:
      fprintf(dumpFile, "DECLARATIONS\n");
      break;

    case STATEMENTS_NODE:
      fprintf(dumpFile, "STATEMENTS\n");
      break;

    case DECLARATION_NODE:
      fprintf(dumpFile, "DECLARATION %s\n", strsep(&curr->declaration.id, "\n"));
      break;

    case STATEMENT_NODE:
      fprintf(dumpFile, "STATEMENT\n");
      break;

    case ELSE_STATEMENT_NODE:
      fprintf(dumpFile, "ELSE STATEMENT\n");
      break;

    case EXP_NODE:
      fprintf(dumpFile, "EXPRESSION");
      break;

    case UNARY_OP_NODE:
      fprintf(dumpFile, "UNARY OP %s", get_op(curr->unary_expr.op));
      break;

    case BINARY_OP_NODE:
      fprintf(dumpFile, "BINARY OP %s", get_op(curr->binary_expr.op));  
      break;

    case CONSTRUCTOR_NODE:
      fprintf(dumpFile, "CONSTRUCTOR");
      break;

    case FUNCTION_NODE:
      fprintf(dumpFile, "FUNCTION %s", get_func(curr->function.function_name));
      break;

    case ARGUMENTS_OPT_NODE:
      fprintf(dumpFile, "ARGUMENTS_OPT");
      break;

    case ARGUMENTS_NODE:
      fprintf(dumpFile, "ARGUMENTS");
      break;

    case NESTED_SCOPE_NODE:
      fprintf(dumpFile, "NESTED_SCOPE_NODE");
      break;

    case VAR_NODE:
      fprintf(dumpFile, "VARIABLE %s", strsep(&curr->variable.id, "\n"));
      break;

    case TYPE_NODE:
      fprintf(dumpFile, "TYPE %s", get_type(curr->type.type_name, curr->type.vec));
      break;

    case INT_NODE:
      fprintf(dumpFile, "INT %d", curr->int_v);
      break;

    case FLOAT_NODE:
      fprintf(dumpFile, "FLOAT %f", curr->float_v);
      break;

    case BOOL_NODE:
      fprintf(dumpFile, "BOOL %d", curr->bool_v);
      break;

    default:
      break;
  }
  //fprintf(dumpFile, "\n");
}

void _ast_print_post(node *curr, int i) {
  if(curr->kind != TYPE_NODE && curr->kind != VAR_NODE && curr->kind != INT_NODE &&
     curr->kind != FLOAT_NODE && curr->kind != BOOL_NODE) for(int j = 1; j < i; j++) fprintf(dumpFile, "\t");
  fprintf(dumpFile, ")\n");
}

//do post order traversal
void ast_print(node *ast) {
  ast_visit(0, ast, &_ast_print, &_ast_print_post);
  fprintf(dumpFile, "\n");
}

//do post order traversal
void semantic_check() {
  ast_visit(0, ast, &ast_semantic_check, NULL);
}

char* get_op(int op) {
  switch(op) {
    case SUB:
      return "-";
    case EXCLAM: 
      return "!";
    case ADD:
      return "+";
    case MUL:
      return "*";
    case DIV:
      return "/";
    case AND:
      return "&&";
    case OR:
      return "||";
    case EQ:
      return "==";
    case NEQ:
      return "!=";
    case LESS:
      return "<";
    case GREATER:
      return ">";
    case LEQ:
      return "<=";
    case GEQ:
      return ">=";
    case EQUAL:
      return "=";
    default:
      return "";
  }
}

char* get_type(int type, int vec) {
  switch(type) {
    case 0:
      if (vec == 275) return "ivec";
      else return "int";
    case 1:
      if (vec == 275) return "vec";
      else return "float";
    case 2:
      if (vec == 275) return "bvec";
      else return "bool";
    default:
      return "unknown_type";
    }
}

char* get_func(int func) {
  fprintf(dumpFile, "%d", func);
  switch(func) {
    case 0:
      return "dp3";
    case 1:
      return "lit";
    case 2:
      return "rsq";
    default:
      return "unknown_func";
    }
}