#include <stdio.h>
#include "semantic.h"
#include "ast.h"

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


bool assign_predefined(node* curr) {
	if (strcmp(curr->declaration.id, "gl_TexCoord") == 0 || strcmp(curr->declaration.id, "gl_Color") == 0 || strcmp(curr->declaration.id, "gl_Secondary") == 0 || 
		strcmp(curr->declaration.id, "gl_FogFragCoord") == 0 || strcmp(curr->declaration.id, "gl_Light_Half") == 0 || strcmp(curr->declaration.id, "gl_Light_Ambient") == 0 ||
		strcmp(curr->declaration.id, "gl_Material_Shininess") == 0 || strcmp(curr->declaration.id, "env1") == 0 || strcmp(curr->declaration.id, "env2") == 0 || 
		strcmp(curr->declaration.id, "env3") == 0 ){
		return false;
	}
	else {
		return true;
	}
}



bool check_function(node* current) {
	
	
	//pointer to the arguments of the function
	node* next_arg = current->func.args;

	//if no arguments, return error
	if (next_arg == NULL){
		fprintf(errorFile, "Error: must have arguments, cannot be void");
		return false;
	}

	//get next arguments
	next_arg = next_arg->args.args;
	
	if (current->func.name == 'rsq') {
		if (next_arg->type.type_code != FLOAT_T) {
			fprintf(errorFile,"Error: Expecting float for first argument");
			return false;
		}
		else if (next_arg == NULL){
			fprintf(errorFile, "Expecting input for first argument of function - it can't be void");
			return false;
		}
		next_arg = next_arg->args.args;
	
		if (next_arg->type.type_code != INT_T){
			fprintf(errorFile, "Expecting integer for second argument");
			return false;
		}
		else if (next_arg == NULL){
			fprintf(errorFile, "Error: Expecting input for second argument of function - it can't be void");
			return false;
		}
	}

	
	else if (current->func.name == 'dp3') {

		//case:vec4
		if (next_arg == NULL){
			fprintf(errorFile, "Input argument cannot be void");	
			return false;	
		}
		if (!(next_arg->type.type_code == VEC_T && next_arg->type.vec == 4)){
			fprintf(errorFile, "First argument is of incorrect type");	
			return false;	
		}
		else if (next_arg->type.type_code == VEC_T && next_arg->type.vec == 4)){
			next_arg = next_arg->args.args;
			if (next_arg == NULL){
				fprintf(errorFile, "Input argument cannot be void");	
				return false;	
			}
			if (!next_arg->type.type_code == VEC_T && next_arg->type.vec == 4)) {
				fprintf(errorFile, "Second argument is of incorrect type");	
				return false;
			}
	
		}
		
		//other vector type case:vec3
		if (next_arg == NULL){
			fprintf(errorFile, "Input argument cannot be void");		
			return false;
		}
		if (!(next_arg->type.type_code == VEC_T && next_arg->type.vec == 3)){
			fprintf(errorFile, "First argument is of incorrect type");		
			return false;
		}
		else if (next_arg->type.type_code == VEC_T && next_arg->type.vec == 3)){
			next_arg = next_arg->args.args;
			if (next_arg == NULL){
				fprintf(errorFile, "Input argument cannot be void");	
				return false;	
			}
			if (!(next_arg->type.type_code == VEC_T && next_arg->type.vec == 3)) {
				fprintf(errorFile, "Second argument is of incorrect type");
				return false;	
			}
	
		}
		//other vector type case:ivec4
		if (!(next_arg->type.type_code == IVEC_T && next_arg->type.vec == 4)){
			fprintf(errorFile, "First argument is of incorrect type");	
			return false;	
		}
		else if (next_arg->type.type_code == IVEC_T) && next_arg->type.vec == 4)){
			next_arg = next_arg->args.args;
			if (next_arg == NULL){
				fprintf(errorFile, "Input argument cannot be void");
				return false;		
			}
			if (!(next_arg->type.type_code == IVEC_T && next_arg->type.vec == 4)) {
				fprintf(errorFile, "Second argument is of incorrect type");
				return false;	
			}
	
		}
		//last vector type case: ivec4
		if (!(next_arg->type.type_code == IVEC_T && next_arg->type.vec == 3)){
			fprintf(errorFile, "First argument is of incorrect type");
			return false;		
		}
		else if (next_arg->type.type_code == IVEC_T) && next_arg->type.vec == 3)){
			next_arg = next_arg->args.args;
			if (next_arg == NULL){
				fprintf(errorFile, "Input argument cannot be void");
				return false;		
			}
			if (!(next_arg->type.type_code == IVEC_T && next_arg->type.vec == 3)) {
				fprintf(errorFile, "Second argument is of incorrect type");
				return false;	
			}
	
		}


	else if (current->func.name == 'lit') {
		if (!(next_arg->type.type_code == VEC_T && next_arg->type.type_vec==4)) {
			fprintf(errorFile,"Error: LIT function only takes in arguments of type vec4");
			return false;
		}
	}
	else {
		fprintf(errorFile,"Error: Function name not recognized");
		return false;
	}
	return true;
}

char* check_function_name(node* current) {
	if (current->func.name == 'rsq') {
		return 'rsq';
	}
	else if (current->func.name == 'dp3') {
		return 'dp3';
	}
	else if (current->func.name == 'lit') {
		return 'lit';
	}
}

void ast_sementic_check(node* current, int x) {

	if (current == NULL) {
		errorOccurred = 1;
		fprintf(errorFile, "Error: Empty tree visited");
		return;
	}
	node_kind kind = current->kind;

	switch (kind) {
	case UNKNOWN: break;
	case SCOPE_NODE: scope_exit(); break;
	case DECLARATIONS_NODE: break; //break
	case STATEMENTS_NODE: break; //break
	case UNARY_EXPRESION_NODE: break;
	case BINARY_EXPRESSION_NODE:
		switch (current->binary_expr.op) {
		case('+')://arithmetic
			if ((current->binary_expr.right->type.type_code != INT_T && current->binary_expr.left->type.type_code != INT_T) && (current->binary_expr.right->type.type_code != FLOAT_T && current->binary_expr.left->type.type_code != FLOAT_T)) {
				fprintf(errorFile, "Error: both sides of expression must be of same type");
				break;
		case(NEQ):					//logical
			if (current->binary_expr.right->type.type_code != BOOL_T && current->binary_expr.left->type.type_code != BOOL_T) {
				fprintf(errorFile, "Error: both sides of expression must be of type boolean");
				break;
			}
			}
		case INT_NODE:
			current->type.type_code = INT_T;
			break;
		case FLOAT_NODE:
			current->type.type_code = FLOAT_T;
			break; 
		case IDENT_NODE: break; 
		case VAR_NODE: break;
		case FUNCTION_NODE: if (!check_function_name(node* current)) break;
		case CONSTRUCTOR_NODE: break;
		case ARGUMENTS_NODE: break;
		case TYPE_NODE: break;
		case BOOL_NODE:
			current->type.type_code = BOOL_T;
			break;
		case IF_STATEMENT_NODE: break;
		case ASSIGNMENT_NODE:
			symbol_table_entry* findVar;
			if (current->assignment.variable->type.type_code != current->exp_var_node.var_node->type.type_code){
				fprintf(errorFile, "Error: both sides of assignment must be of same type");
			}

		case NESTED_SCOPE_NODE: break;
		case NESTED_EXPRESSION_NODE: break;
		case EXP_VAR_NODE: break;
		case DECLARATION_NODE:
			if (symbol_exists_in_this_scope(current->declaration.id)) {
				fprintf(errorFile, "Error, declaration already exists in this scope");
			}
			if (!assign_predefined(current)) {
				fprintf(errorFile, "Error you cannot assign to these predefined read only variables\n");
				break;
			}


			//when the declaration is initialized to a value
			if (current->declaration.expr) {
				if (current->declaration.expr->exp_var_node.var_node->type.type_code != current->declaration.type_node->type.type_code) {
					fprintf(errorFile, "Error: both sides of expression must be of same type");
				}
			}
			else {

			}
			symbol_table_entry entry;
			entry.type_code = current->declaration.type_node->type.type_code;
			entry.is_const = current->declaration.is_const;
			entry.id = current->declaration.id;

		default: break;
		}
	}
}

