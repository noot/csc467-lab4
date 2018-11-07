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


//checking that we're not assigning to predefined read only variables 
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



//checking function and function arguments
bool check_function(node* current) { //do too many arguments case
	
	
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
		else if (next_arg->type.type_code == VEC_T && next_arg->type.vec == 4){
			next_arg = next_arg->args.args;
			if (next_arg == NULL){
				fprintf(errorFile, "Input argument cannot be void");	
				return false;	
			}
			if (!next_arg->type.type_code == VEC_T && next_arg->type.vec == 4) {
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
		else if (next_arg->type.type_code == VEC_T && next_arg->type.vec == 3){
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
		else if (next_arg->type.type_code == IVEC_T && next_arg->type.vec == 4){
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
		else if (next_arg->type.type_code == IVEC_T && next_arg->type.vec == 3){
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
		if (!(next_arg->type.type_code == VEC_T && next_arg->type.vec==4)) {
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


//checking for predefined variables
bool predefined_variables(node* current){
	curr = current->var_node.id;
	if (strcmp(curr->declaration.id, "gl_TexCoord") == 0 || strcmp(curr->declaration.id, "gl_Color") == 0 || strcmp(curr->declaration.id, "gl_Secondary") == 0 || 
	strcmp(curr->declaration.id, "gl_FogFragCoord") == 0){
		curr->type.type_code = VEC_T;
		curr->type.vec = 4;
		curr->type.is_const = 0;
		return false;
	}
	
	else if (strcmp(curr->declaration.id, "gl_Light_Half") == 0 || strcmp(curr->declaration.id, "gl_Light_Ambient") == 0 ||
	strcmp(curr->declaration.id, "gl_Material_Shininess") == 0 || strcmp(curr->declaration.id, "env1") == 0 || strcmp(curr->declaration.id, "env2") == 0 || 
	strcmp(curr->declaration.id, "env3") == 0){
		curr->type.type_code = VEC_T;
		curr->type.vec = 4;
		curr->type.is_const = 1;
		return false;
	}


	else if (strcmp(curr->declaration.id, "gl_FragColor") == 0 || strcmp(curr->declaration.id, "gl_FragDepth") == 0 || 
	strcmp(curr->declaration.id, "gl_FragCoord") == 0){
		curr->type.is_const = 1;
		if (strcmp(curr->declaration.id, "gl_FragDepth") == 0){
			curr->type.type_code = BOOL_T;
			curr->type.vec = 1;

		}
		else{
			curr->type.type_code = VEC_T;
			curr->type.vec = 4;
		}

		return false;
	}
	return true;
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
		case UNARY_EXPRESION_NODE: if(!DEBUG_UNARY) break; //TODO
		case BINARY_EXPRESSION_NODE: if(!DEBUG_BINARY) break; //TOFINISH
		switch (current->binary_expr.op) {
			case('+')://arithmetic
				if ((current->binary_expr.right->type.type_code != INT_T && current->binary_expr.left->type.type_code != INT_T) && (current->binary_expr.right->type.type_code != FLOAT_T && current->binary_expr.left->type.type_code != FLOAT_T)) {
					fprintf(errorFile, "Error: both sides of expression must be of same type");
					break; }
			case(NEQ):					//logical
				if (current->binary_expr.right->type.type_code != BOOL_T && current->binary_expr.left->type.type_code != BOOL_T) {
					fprintf(errorFile, "Error: both sides of expression must be of type boolean");
					break;
				}
		}
		case INT_NODE:
			current->type.type_code = INT_T;
			current->type.vec = 1;
			current->type.is_const = 1;
			break;
		case FLOAT_NODE:
			current->type.type_code = FLOAT_T;
			current->type.vec = 1;
			current->type.is_const = 1;
			break; 
		//case IDENT_NODE: break; //not needed I think
		case VAR_NODE: 
			if(!DEBUG_VAR_NODE) break; 
			//check if it exists in symbol table first
			symbol_table_entry *findVar;
			findVar = exists_in_symbol_table(current->var_node.id);

			if(findVar!= NULL){
				//if value is const then you cannot re-assign to it
				if (findVar.is_const = 1 && current->var_node.id != findVar.id){ //this depends on how we're storing the entries
					fprintf(errorFile, "Error: you cannot assign a new value to a const variable");	
					break;
				}

				//if it has been initialized
				if (findVar.init = 1){
					findVar->type.is_const = 0;
					findVar->type.type_code = current->var_node.id->type.type_code;
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
			if(!predefined_variables(node* current)){
				fprintf(errorFile, "Error you cannot use predefined variables as variable names");
				break; 			
			}
				

		case FUNCTION_NODE: 
			if(!DEBUG_FUNCTION) break; 
			if (!check_function_name(node* current)) break;
		case CONSTRUCTOR_NODE: if(!DEBUG_CONSTRUCTOR) break; 
			//check that the type of the contructor matches the type of the arguments
			node *next_arg = current->args;
			while(next_arg->args.args){
				if (current->ctor.type_node != next_args.expr->type.type_code){
					fprintf(errorFile, "Error: incorrect type for constructor");
					break;
				}
				else{
					next_args = next_args->args.args;
				}
			}

		case ARGUMENTS_NODE: 
			if(!DEBUG_ARGUMENTS)break; 
			//arguments must all be of same type
			node *next_arg = current->args.args;

			if (next_arg==NULL){
				fprintf(errorFile, "Error: not enough arguments");
			}

			node* type_exp1 = next_arg->type.type_code;
			next_arg = next_arg->args.args;
			if (next_arg!=NULL){
				node *type_exp2 = next_arg->type.type_code; ;
			}
			else{
				if (*type_exp1 != *type_exp2){
					fprintf(errorFile, "arguments have to be of same type");
				}
			}

			next_arg = next_arg->args.args;
			if (next_arg != NULL){
				node *type_exp3 = next_arg->type.type_code;;
			}
			next_arg = next_arg->args.args;
			if (next_arg!=NULL){
				node *type_exp4 = next_arg->type.type_code;;
			}
			next_arg = next_arg->args.args;
			if (next_arg!=NULL){
				fprintf(errorFile, "Error: too many arguments");
				break;
			}
		
			//arguments must be of expected type

		case TYPE_NODE: break; //TODO?
		case BOOL_NODE:
			current->type.type_code = BOOL_T;
			current->type.vec = 1;
			current->type.is_const = 1;
			break;
		case IF_STATEMENT_NODE: 
			if (!(current->condition_expr->type.type_code == BOOL_T && current->condition_expr->type.vec == 1)){ //can't be bvec
				fprintf(errorFile, "Error: conditional expression must be of type boolean");	
				break;		
			}


		case ASSIGNMENT_NODE:
			if(!DEBUG_ASSIGNMENT) break; 
			symbol_table_entry* findVar;
			if (current->assignment.variable->type.type_code != current->exp_var_node.var_node->type.type_code){
				fprintf(errorFile, "Error: both sides of assignment must be of same type");
			}

		case NESTED_SCOPE_NODE: break; //TODO
		case NESTED_EXPRESSION_NODE: break; //TODO
		case EXP_VAR_NODE: //TODO 
			break; 
		case DECLARATION_NODE: if(!DEBUG_DECLARATION) break; 
			if (symbol_exists_in_this_scope(current->declaration.id)) {
				fprintf(errorFile, "Error, declaration already exists in this scope");
			}
			if (!assign_predefined(&current)) {
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
  

