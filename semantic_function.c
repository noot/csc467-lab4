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
		case BINARY_EXPRESSION_NODE: {break;} //TOFINISH
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
		case FUNCTION_NODE:{ 
			//pointer to the arguments of the function
			node* next_arg = current->func.args;

			//if no arguments, return error
			if (next_arg == NULL){
				fprintf(errorFile, "Error: must have arguments, cannot be void\n");
				break;
			}

			//get next arguments
			//next_arg = next_arg->args.args;
	
			if (current->func.name == 2) {
				if (next_arg->type.type_code != FLOAT_T) {
					fprintf(errorFile,"Error: Expecting float for first argument\n");
					break;
				}
				else if (next_arg == NULL){
					fprintf(errorFile, "Expecting input for first argument of function - it can't be void\n");
					break;
				}
				next_arg = next_arg->args.args;
	
				if (next_arg->type.type_code != INT_T){
					fprintf(errorFile, "Expecting integer for second argument\n");
					break;
				}
				else if (next_arg == NULL){
					fprintf(errorFile, "Error: Expecting input for second argument of function - it can't be void\n");
					break;
				}
			}

	
			else if (current->func.name == 0) {

				//case:vec4
				if (next_arg == NULL){
					fprintf(errorFile, "Input argument cannot be void\n");	
					break;
				}
				if (!(next_arg->type.type_code == VEC_T && next_arg->type.vec == 4)){
					fprintf(errorFile, "First argument is of incorrect type\n");	
					break;
				}
				else if (next_arg->type.type_code == VEC_T && next_arg->type.vec == 4){
					next_arg = next_arg->args.args;
					if (next_arg == NULL){
						fprintf(errorFile, "Input argument cannot be void\n");	
						break;
					}
					if (!(next_arg->type.type_code == VEC_T && next_arg->type.vec == 4)) {
						fprintf(errorFile, "Second argument is of incorrect type\n");	
						break;
					}
	
				}
		
				//other vector type case:vec3
				if (next_arg == NULL){
					fprintf(errorFile, "Input argument cannot be void\n");		
					break;
				}
				if (!(next_arg->type.type_code == VEC_T && next_arg->type.vec == 3)){
					fprintf(errorFile, "First argument is of incorrect type\n");		
					break;
				}
				else if (next_arg->type.type_code == VEC_T && next_arg->type.vec == 3){
					next_arg = next_arg->args.args;
					if (next_arg == NULL){
						fprintf(errorFile, "Input argument cannot be void\n");	
						break;
					}
					if (!(next_arg->type.type_code == VEC_T && next_arg->type.vec == 3)) {
						fprintf(errorFile, "Second argument is of incorrect type\n");
						break;
					}
	
				}
				//other vector type case:ivec4
				if (!(next_arg->type.type_code == IVEC_T && next_arg->type.vec == 4)){
					fprintf(errorFile, "First argument is of incorrect type\n");	
					break;
				}
				else if (next_arg->type.type_code == IVEC_T && next_arg->type.vec == 4){
					next_arg = next_arg->args.args;
					if (next_arg == NULL){
						fprintf(errorFile, "Input argument cannot be void\n");
						break;
					}
					if (!(next_arg->type.type_code == IVEC_T && next_arg->type.vec == 4)) {
						fprintf(errorFile, "Second argument is of incorrect type\n");
						break;
					}
	
				}
				//last vector type case: ivec4
				if (!(next_arg->type.type_code == IVEC_T && next_arg->type.vec == 3)){
					fprintf(errorFile, "First argument is of incorrect type\n");
					break;
				}
				else if (next_arg->type.type_code == IVEC_T && next_arg->type.vec == 3){
					next_arg = next_arg->args.args;
					if (next_arg == NULL){
						fprintf(errorFile, "Input argument cannot be void\n");
						break;
					}
					if (!(next_arg->type.type_code == IVEC_T && next_arg->type.vec == 3)) {
						fprintf(errorFile, "Second argument is of incorrect type\n");
						break;
					}
	
				}
			}

			else if (current->func.name == 1) {
				if (!(next_arg->type.type_code == VEC_T && next_arg->type.vec==4)) {
					fprintf(errorFile,"Error: LIT function only takes in arguments of type vec4\n");
					break;
				}
			}
			else {
				fprintf(errorFile,"Error: Function name not recognized\n");
					break;
			}
		}

		case CONSTRUCTOR_NODE: {break;} 
		case ARGUMENTS_NODE:{break;}
		case TYPE_NODE: {break;} //TODO?
		case BOOL_NODE:{
			current->type.type_code = BOOL_T;
			current->type.vec = 1;
			current->type.is_const = 1;
			break;}
		case IF_STATEMENT_NODE:{break;}
		case ASSIGNMENT_NODE:{break;}
		case NESTED_SCOPE_NODE: {break;} //TODO
		case NESTED_EXPRESSION_NODE: {break;} //TODO
		case EXP_VAR_NODE: {//TODO 
			break; }
		case DECLARATION_NODE:{break;}
		default: {break;}
	}
	
}
  
