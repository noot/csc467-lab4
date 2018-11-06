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
		case BINARY_EXPRESSION_NODE: {break;}
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
		case VAR_NODE:{
			//check if it exists in symbol table first
			symbol_table_entry *findVar;
			findVar = symbol_find(current->var_node.id);

			if(findVar!= NULL){
				//if value is const then you cannot re-assign to it
				if (findVar->is_const = 1 && (current->var_node.id != findVar->id)){ //this depends on how we're storing the entries
					fprintf(errorFile, "Error: you cannot assign a new value to a const variable");	
					break;
				}

				//if it has been initialized
				if (findVar->is_init == 1){
					findVar->is_const = 0;
					//findVar->type_code = current->var_node.id->type.type_code;
				}

				//if it hasn't been initialized
				else{
					fprintf(errorFile, "Error this variable hasn't been initialized"); //does it have to be initialized?
					break;
				}

			}

			else{
				fprintf(errorFile, "Error: this variable has not been declared");
				break;
			}

			//make sure you are not using predefined variables
			//current = current->var_node.id;
			if (strcmp(current->declaration.id, "gl_TexCoord") == 0 || strcmp(current->declaration.id, "gl_Color") == 0 || strcmp(current->declaration.id, "gl_Secondary") == 0 || 
			strcmp(current->declaration.id, "gl_FogFragCoord") == 0){
				current->type.type_code = VEC_T;
				current->type.vec = 4;
				current->type.is_const = 0;
				fprintf(errorFile, "Error you cannot use predefined variables as variable names");
				break; 	
			}
	
			else if (strcmp(current->declaration.id, "gl_Light_Half") == 0 || strcmp(current->declaration.id, "gl_Light_Ambient") == 0 ||
			strcmp(current->declaration.id, "gl_Material_Shininess") == 0 || strcmp(current->declaration.id, "env1") == 0 || strcmp(current->declaration.id, "env2") == 0 || 
			strcmp(current->declaration.id, "env3") == 0){
				current->type.type_code = VEC_T;
				current->type.vec = 4;
				current->type.is_const = 1;
				fprintf(errorFile, "Error you cannot use predefined variables as variable names");
				break; 	
			}


			else if (strcmp(current->declaration.id, "gl_FragColor") == 0 || strcmp(current->declaration.id, "gl_FragDepth") == 0 || 
				strcmp(current->declaration.id, "gl_FragCoord") == 0){
				current->type.is_const = 1;
				if (strcmp(current->declaration.id, "gl_FragDepth") == 0){
					current->type.type_code = BOOL_T;
					current->type.vec = 1;

				}
				else{
					current->type.type_code = VEC_T;
					current->type.vec = 4;
				}

				fprintf(errorFile, "Error you cannot use predefined variables as variable names");
				break; 	
			}
				
		}
		case FUNCTION_NODE:{ break;}
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
		case EXP_VAR_NODE: {
			current->type.type_code = current->exp_var_node.var_node->type.type_code;
			current->type.is_const = current->exp_var_node.var_node->type.is_const;
			current->type.vec = current->exp_var_node.var_node->type.vec;
			//what about id?
			break; }
		case DECLARATION_NODE:{			
			if (symbol_exists_in_this_scope(current->declaration.id)) {
				fprintf(errorFile, "Error, declaration already exists in this scope");
				break;
			}
			if (strcmp(current->declaration.id, "gl_TexCoord") == 0 || strcmp(current->declaration.id, "gl_Color") == 0 || strcmp(current->declaration.id, "gl_Secondary") == 0 || 
				strcmp(current->declaration.id, "gl_FogFragCoord") == 0 || strcmp(current->declaration.id, "gl_Light_Half") == 0 || strcmp(current->declaration.id, "gl_Light_Ambient") == 0 ||
				strcmp(current->declaration.id, "gl_Material_Shininess") == 0 || strcmp(current->declaration.id, "env1") == 0 || strcmp(current->declaration.id, "env2") == 0 || 
				strcmp(current->declaration.id, "env3") == 0 ){
				fprintf(errorFile, "Error you cannot assign to these predefined read only variables\n");
				break;
			}

			//when the declaration is initialized to a value
			if (current->declaration.expr) {
				if (current->declaration.expr->exp_var_node.var_node->type.type_code != current->declaration.type_node->type.type_code) {
					fprintf(errorFile, "Error: both sides of expression must be of same type");
					break;
				}
			}

		}

		default: {break;}
	}
	
}
  

