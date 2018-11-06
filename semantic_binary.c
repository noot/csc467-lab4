#include <stdio.h>
#include "semantic.h"
#include "ast.h"

#define DEBUG_FUNCTION 1
#define DEBUG_DECLARATION 0
#define DEBUG_CONSTRUCTOR 0
#define DEBUG_BINARY 0
#define DEBUG_VAR_NODE 0
#define	DEBUG_ARGUMENTS 0
#define DEBUG_UNARY 0
#define DEBUG_ASSIGNMENT 0

bool assign_predefined(node* curr);
bool check_function(node* current);
bool predefined_variables(node* current);

void ast_check_semantics() {
	if (ast == NULL) {
		errorOccurred = 1;
		fprintf(errorFile, "Main scope not found.\n");
		return;
	}
	else {
		ast_traverse(ast, 0, &ast_scope_generator, &ast_sementic_check);

	}

}
void ast_scope_generator(node *cur, int x) { //Done pre-post.

	if (cur->kind == SCOPE_NODE) {
		scope_enter();
	}
}




//checking function and function arguments

void ast_sementic_check(node* current, int x) {

	if (current == NULL) {
		errorOccurred = 1;
		fprintf(errorFile, "Error: Empty tree visited");
		return;
	}
	node_kind kind = current->kind;

	switch (kind) {
		case UNKNOWN: {break;}
		case SCOPE_NODE: {scope_exit(); break;}
		case DECLARATIONS_NODE: {break;} //break
		case STATEMENTS_NODE: {break;} //break
		case UNARY_EXPRESION_NODE: {break;} //TODO
		case BINARY_EXPRESSION_NODE: {
			int left = current->binary_expr.left->type.type_code;
			int right = current->binary_expr.right->type.type_code;
			int op = current->binary_expr.op;

			if (left != BOOL_T && right!=BOOL_T){
				if (op == EQ){
					fprintf(errorFile,"Error: Expecting boolean on both sides\n");
				}
			}
			
			break;} //TOFINISH
		case INT_NODE:{
			current->type.type_code = INT_T;
			current->type.vec = 1;
			current->type.is_const = 1;
			break;}
		case FLOAT_NODE:{
			current->type.type_code = FLOAT_T;
			current->type.vec = 1;
			current->type.is_const = 1;
			break; }
		case IDENT_NODE: {break;} //not needed I think
		case VAR_NODE:{break;}
		case FUNCTION_NODE:{ break;}
		case CONSTRUCTOR_NODE: {break;} 
		case ARGUMENTS_NODE:{break;}
		case TYPE_NODE: {break;} //TODO?
		case BOOL_NODE:{
			current->type.type_code = BOOL_T;
			current->type.vec = 1;
			current->type.is_const = 1;
			break;}
		case IF_STATEMENT_NODE:{
			if (!(current->if_stmt.condition_expr->type.type_code == BOOL_T && current->if_stmt.condition_expr->type.vec == 1)){ //can't be bvec
				fprintf(errorFile, "Error: conditional expression must be of type boolean\n");	
				break;		
			}
}
		case ASSIGNMENT_NODE:{break;}
		case NESTED_SCOPE_NODE: {break;} //TODO
		case NESTED_EXPRESSION_NODE: {break;} //TODO
		case EXP_VAR_NODE: {//TODO 
			break; }
		case DECLARATION_NODE:{break;}
		default: {break;}
	}
	
}
  
