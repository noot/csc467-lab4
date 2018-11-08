#include <stdio.h>
#include "semantic.h"
#include "ast.h"



/*
1. Please define the nested scope and assignment nodes because I use them
1. I call a function called find_var() which takes in a variable id and should return an element of type _entry -->please implement
2. I call your function called in_scope() which should return whether or not a variable has already been declared in that scope --> you already implemented
3. I call your function new_entry() which adds the entry to the symbol table --> you already implemented
4. I call enter_scope and exit_scope but I don't know how to call them properly --> I'm using a boolean called enterScope but don't know where I should call it from
5. I try to pass your my function to traverse
6. Please check the function traverse_and_check, I don't know if I implemented it right

*/

void traverse_and_check(){
	if (ast == NULL){
		errorOccured = 1;
		fprintf(errorFile, "Does not exist");
		return;
	}
	else traverse(ast, &ast_sementic_check);
}
void ast_semantic_check(node* current) {

	if (current == NULL) {
		errorOccurred = 1;
		fprintf(errorFile, "Error: Empty tree visited");
		return;
	}
	node_kind kind = current->kind;

	switch (kind) {
		case UNKNOWN: {break;}
		case SCOPE_NODE: {
			if (enterScope){ enter_scope();} //
			else exit_scope();
			fprintf(errorFile,"Reached scope\n");
			break;}

		case DECLARATIONS_NODE: {fprintf(errorFile,"Reached declarations node\n");break; } //break
		case STATEMENTS_NODE: {fprintf(errorFile,"Reached statements node\n");break;} //break
		case UNARY_EXPRESION_NODE: {
			fprintf(errorFile,"Reached unary");	
			int right = current->unary_expr.right->type.type_name;
			int op = current->unary_expr.op;
			if (op == EXCLAM ){
				if (right!=BOOL_T){
					fprintf(errorFile,"Error: unary expression ! expects a boolean type");
				}
				
			}
			else{
				if (right == BOOL_T){
					fprintf(errorFile,"Error: unary expression - expects an arithmetic type");				
				}
			}

			current->type.type_name = right;
			current->type.vec = current->unary_expr.right->type.vec;
		break;} 
		case BINARY_EXPRESSION_NODE: {
			fprintf(errorFile,"Reached binary node\n");
			int left = current->binary_expr.left->type.type_name;
			int right = current->binary_expr.right->type.type_name;
			int op = current->binary_expr.op;

			fprintf(errorFile,"Type codes of binary expression Val left:%d Val right:%d\n",left,right);

			if (op == EQ||op == NEQ){
				if (left == right){
					current->type.type_name = BOOL_T;
					current->type.vec = 1;
					fprintf(errorFile,"Assigning type code of boolean: %d\n",current->type.type_name);
					break;//this wasn't there before and was causing problems!!! remember to break!!!
				}	
				else{
					fprintf(errorFile,"Error: Expecting same type on both sides\n");
					//current->type.type_name = -1;
					break;
				}
			}
			else if (op == ADD||op == SUB||op == MULT||DIV||EXP){
				if (left == right){
					current->type.type_name = left;
					current->type.vec = 1;
					fprintf(errorFile,"Assigning type code of left side: %d\n",current->type.type_name);
				}
				else{
					fprintf(errorFile,"Error: Expecting same type on both sides");
				}
			}
			else if (op == GEQ || op == LEQ || op == LESS || op == GREATER){
				if (current->binary_expr.left->type.vec != 1){
					fprintf(errorFile, "Error: you cannot use these operators with vectors");
				}
				if (left == right){
					current->type.type_name = left;
					current->type.vec = 1;
				}
				else{
					fprintf(errorFile,"Error: Expecting same type on both sides");
				}
			}
			
		}
			
		case INT_NODE:{
			fprintf(errorFile,"Reached int node\n");
			current->type.type_name = INT_T;
			current->type.vec = 1;
			current->type.is_const = 1;
			break;}
		case FLOAT_NODE:{
			fprintf(errorFile,"Reached float node\n");
			current->type.type_name = FLOAT_T;
			current->type.vec = 1;
			current->type.is_const = 1;
			break; }
		case VAR_NODE: {
			//check if it exists in symbol table first
			_entry *findVar;
			findVar = find_var(current->var_node.id); //find_var function calls from scope

			if(findVar!= NULL){
				fprintf(errorFile, "this has been declared already\n");	
				//if it has been initialized
				if (findVar.is_init == 0){
					fprintf(errorFile, "Error this variable hasn't been initialized"); //does it have to be initialized?
					break;
					
				}

			}
			
			else{
				if ((!strcmp(current->var_node.id, "gl_TexCoord") == 0) && (!strcmp(current->var_node.id, "gl_Color") == 0) && !strcmp(current->var_node.id, "gl_Secondary") == 0 && 
					!strcmp(current->var_node.id, "gl_FogFragCoord") == 0 && !strcmp(current->var_node.id, "gl_Light_Half") == 0 && !strcmp(current->var_node.id, "gl_Light_Ambient") == 0 &&
					!strcmp(current->var_node.id, "gl_Material_Shininess") == 0 && !strcmp(current->var_node.id, "env1") == 0 && !strcmp(current->var_node.id, "env2") == 0 && 
					!strcmp(current->var_node.id, "env3") == 0 && !strcmp(current->var_node.id, "gl_FragColor") == 0 && !strcmp(current->var_node.id, "gl_FragDepth") == 0 && 
					!strcmp(current->var_node.id, "gl_FragCoord") == 0){
					fprintf(errorFile, "Error: this variable has not been declared...\n");
					break;
				}
				break;
			}
			current->type.is_const = findVar.is_const;
			current->type.type_name = findVar.type_name;

			if((findVar.type_name == BOOL_T || findVar.type_name == FLOAT_T || findVar.type_name == INT_T) && findVar.is_vec == 2){
				int max_index = findVar.is_vec;
				if (current->variable.idx >= max_index){
					fprintf(errorFile, "Error: Index exceeds bounds");
					break;
				}
			}

			if((findVar.type_name == BOOL_T || findVar.type_name == FLOAT_T || findVar.type_name == INT_T) && findVar.is_vec == 3){
				int max_index = findVar.is_vec;
				if (current->variable.idx >= max_index){
					fprintf(errorFile, "Error: Index exceeds bounds");
					break;
				}
			}
			if((findVar.type_name == BOOL_T || findVar.type_name == FLOAT_T || findVar.type_name == INT_T) && findVar.is_vec == 4){
				int max_index = findVar.is_vec;
				if (current->variable.idx >= max_index){
					fprintf(errorFile, "Error: Index exceeds bounds");
					break;
				}
			}
			//make sure you are not using predefined variables
			char* curr;
			curr = current->variable.id;
			if (strcmp(curr, "gl_TexCoord") == 0 || strcmp(curr, "gl_Color") == 0 || strcmp(curr, "gl_Secondary") == 0 || 
			strcmp(curr, "gl_FogFragCoord") == 0){
				current->type.type_name = VEC4_T;
				current->type.vec = 4;
				current->type.is_const = 0;
				fprintf(errorFile, "Error you cannot use predefined variables as variable names");
				break; 	
			}
	
			else if (strcmp(curr, "gl_Light_Half") == 0 || strcmp(curr, "gl_Light_Ambient") == 0 ||
			strcmp(curr, "gl_Material_Shininess") == 0 || strcmp(curr, "env1") == 0 || strcmp(curr, "env2") == 0 || 
			strcmp(curr, "env3") == 0){
				current->type.type_name = VEC4_T;
				current->type.vec = 4;
				current->type.is_const = 1;
				fprintf(errorFile, "Error you cannot use predefined variables as variable names");
				break; 	
			}


			else if (strcmp(curr, "gl_FragColor") == 0 || strcmp(curr, "gl_FragDepth") == 0 || 
			strcmp(curr, "gl_FragCoord") == 0){
				current->type.is_const = 1;
				if (strcmp(curr, "gl_FragDepth") == 0){
					current->type.type_name = BOOL_T;
					current->type.vec = 1;

				}
				else{
					current->type.type_name = VEC4_T;
					current->type.vec = 4;
				}

				fprintf(errorFile, "Error you cannot use predefined variables as variable names");
				break; 	
			}

			
			break;
		}
				

		case FUNCTION_NODE:{
			//pointer to the arguments of the function
			node* next_arg = current->function.args;

			//if no arguments, return error
			if (current->function.args == NULL){
				fprintf(errorFile, "Error: must have arguments, cannot be void\n");
				break;
			}

			//get next arguments
			next_arg = next_arg->arguments.args;
	
			//current->type.type_name will be the return type of the function
			if (current->function.function_name == 2) { //rsq
				fprintf(errorFile,"Type of args:%d \n",next_arg->type.type_name);
				if (!(next_arg->type.type_name == FLOAT_T ||next_arg->type.type_name == INT_T )) {
					fprintf(errorFile,"Error: Expecting float or integer for first argument\n");
					break;
				}
				
				next_arg = next_arg->arguments.args;
				if (next_arg != NULL){
					fprintf(errorFile,"Error: Too many arguments for this function\n");					
				}

			}

	
			else if (current->function.function_name == 0) { //dp3

				//case:vec4
				if (next_arg == NULL){
					fprintf(errorFile, "Input argument cannot be void\n");	
					break;
				}
				if (!(next_arg->type.type_name == VEC4_T && next_arg->type.vec == 4)){
					fprintf(errorFile, "First argument is of incorrect type\n");	
					break;
				}
				else if (next_arg->type.type_name == VEC4_T && next_arg->type.vec == 4){
					next_arg = next_arg->arguments.args;
					if (next_arg == NULL){
						fprintf(errorFile, "Input argument cannot be void\n");	
						break;
					}
					if (!(next_arg->type.type_name == VEC4_T && next_arg->type.vec == 4)) {
						fprintf(errorFile, "Second argument is of incorrect type\n");	
						break;
					}
	
				}
		
				//other vector type case:vec3
				if (next_arg == NULL){
					fprintf(errorFile, "Input argument cannot be void\n");		
					break;
				}
				if (!(next_arg->type.type_name == VEC3_T && next_arg->type.vec == 3)){
					fprintf(errorFile, "First argument is of incorrect type\n");		
					break;
				}
				else if (next_arg->type.type_name == VEC3_T && next_arg->type.vec == 3){
					next_arg = next_arg->arguments.args;
					if (next_arg == NULL){
						fprintf(errorFile, "Input argument cannot be void\n");	
						break;
					}
					if (!(next_arg->type.type_name == VEC3_T && next_arg->type.vec == 3)) {
						fprintf(errorFile, "Second argument is of incorrect type\n");
						break;
					}
	
				}
				//other vector type case:ivec4
				if (!(next_arg->type.type_name == IVEC4_T && next_arg->type.vec == 4)){
					fprintf(errorFile, "First argument is of incorrect type\n");	
					break;
				}
				else if (next_arg->type.type_name == IVEC4_T && next_arg->type.vec == 4){
					next_arg = next_arg->arguments.args;
					if (next_arg == NULL){
						fprintf(errorFile, "Input argument cannot be void\n");
						break;
					}
					if (!(next_arg->type.type_name == IVEC4_T && next_arg->type.vec == 4)) {
						fprintf(errorFile, "Second argument is of incorrect type\n");
						break;
					}
	
				}
				//last vector type case: ivec3
				if (!(next_arg->type.type_name == IVEC3_T && next_arg->type.vec == 3)){
					fprintf(errorFile, "First argument is of incorrect type\n");
					break;
				}
				else if (next_arg->type.type_name == IVEC3_T && next_arg->type.vec == 3){
					next_arg = next_arg->arguments.args;
					if (next_arg == NULL){
						fprintf(errorFile, "Input argument cannot be void\n");
						break;
					}
					if (!(next_arg->type.type_name == IVEC3_T && next_arg->type.vec == 3)) {
						fprintf(errorFile, "Second argument is of incorrect type\n");
						break;
					}
	
				}
			}

			else if (current->function.function_name == 1) { //lit
				if (!(next_arg->type.type_name == VEC4_T && next_arg->type.vec == 4)) {
					fprintf(errorFile,"Error: LIT function only takes in arguments of type vec4\n");
					break;
				}
			}
			else {
				fprintf(errorFile,"Error: Function name not recognized\n");
					break;
			}
		}


            

        	case CONSTRUCTOR_NODE:{
			    fprintf(errorFile, "Entering constructor node\n");
		     	int num_args, num_type;
			    int i = 0;
			    node* next_arg = current->constructor.args;
			    if (next_arg == NULL) {
					fprintf(errorFile, "Error constructor cannot be empty");
				break;
			    }

			    switch (current->constructor.type->type.type_name) {
				    case(IVEC4_T): {num_args = 4; num_type = INT_T;break;}
				    case(IVEC3_T): {num_args = 3; num_type = INT_T;break;}
				    case(IVEC2_T): {num_args = 2; num_type = INT_T; fprintf(errorFile, "ivec case reached,numer args = %d and num_type = %d \n",num_args,num_type);break;}
				    case(BVEC4_T): {num_args = 4; num_type = BOOL_T;break;}
				    case(BVEC3_T): {num_args = 3; num_type = BOOL_T;break;}
				    case(BVEC2_T): {num_args = 2; num_type = BOOL_T;break;}
				    case(VEC4_T): {num_args = 4; num_type = FLOAT_T;break;}
				    case(VEC3_T): {num_args = 3; num_type = FLOAT_T;break;}
				    case(VEC2_T): {num_args = 2; num_type = FLOAT_T;break;}
			    }


				//259 is integer
				
			    do{
					if (next_arg == NULL) {
						fprintf(errorFile, "Error: constructor arguments cannot be null\n");

					}
					fprintf(errorFile, "ivec case reached,numer args = %d and num_type = %d \n",num_args,num_type);
					fprintf(errorFile, "CTOR arguments type:%d, CTOR type:%d \n",current->constructor.args->type.type_name, num_type);

					if (current->constructor.args->type.type_name != num_type) { //type of constructor itself
						fprintf(errorFile, "Error: incorrect constructor argument type\n");

					}
					else{
						fprintf(errorFile, "This argument type is correct\n");

					}
					i++;
					next_arg = next_arg->arguments.args;

			    }while (i < num_args && next_arg!=NULL);

			    current->type.vec = num_args;
			    current->type.type_name = num_type;
			    //current->variable.is_vec = 1;		
			break;} 


		case ARGUMENTS_NODE:{	
			fprintf(errorFile,"Reached arguments node\n");		
			if (current->arguments.exp){
				//arguments must be of expected type  
				current->type.is_const = current->arguments.exp->type.is_const;	
				fprintf(errorFile,"Type code args: %d\n",current->arguments.exp->type.type_name);
				current->type.type_name = current->arguments.exp->type.type_name;
				current->type.vec = current->arguments.exp->type.vec;
				break;
			}
			else{
				current->type.is_const = 0; //verify
				current->type.type_name = -1; //verify
				fprintf(errorFile,"Type code args: %d\n",current->type.type_name);
				current->type.vec = 1;
				break;
				
			}
		}
		case BOOL_NODE:{
			fprintf(errorFile,"Reached bool node\n");
			current->type.type_name = BOOL_T;
			current->type.vec = 1;
			current->type.is_const = 1;
			break;}
		case IF_STATEMENT_NODE:{

			//current->statement.is_if = 1;
			fprintf(errorFile,"Reached if condition\n");		
			fprintf(errorFile,"Type code of conditional expression: %d\n",current->statement.exp->type.type_name);
			if (!(current->statement.exp->type.type_name == BOOL_T && current->statement.exp->type.vec == 1)){ //can't be bvec
				fprintf(errorFile, "Error: conditional expression must be of type boolean\n");	
				break;		
			}
		}
		case ASSIGNMENT_NODE:{

			_entry *findVar;
			findVar = find_var(current->assignment.variable->variable.id); //call to find var


			if (findVar == NULL){
				fprintf(errorFile, "Error: this variable has not been declared\n");  //Implicit type conversions
			}
			if (findVar->is_init){
				fprintf(errorFile,"Variable:%d, Expression:%d \n",current->assignment.variable->type.type_name, current->assignment.exp->type.type_name);
				//if (current->assignment.variable->type.type_name != current->assignment.expr->type.type_name){
				if (findVar->type_name != current->assignment.exp->type.type_name){
					fprintf(errorFile, "Error: both sides of assignment must be of same type\n");  //Implicit type conversions
					break;
				}
			
			}

			if (findVar->is_const){
				if(current->assignment.exp){
					fprintf(errorFile, "Error: cannot assign to a const variable\n");
				}
			}

			if (strcmp(current->assignment.variable->variable.id, "gl_TexCoord") == 0 || strcmp(current->assignment.variable->variable.id, "gl_Color") == 0 || strcmp(current->assignment.variable->variable.id, "gl_Secondary") == 0 || 
				strcmp(current->assignment.variable->variable.id, "gl_FogFragCoord") == 0 || strcmp(current->assignment.variable->variable.id, "gl_Light_Half") == 0 || strcmp(current->assignment.variable->variable.id, "gl_Light_Ambient") == 0 ||
				strcmp(current->assignment.variable->variable.id, "gl_Material_Shininess") == 0 || strcmp(current->assignment.variable->variable.id, "env1") == 0 || strcmp(current->assignment.variable->variable.id, "env2") == 0 || 
				strcmp(current->assignment.variable->variable.id, "env3") == 0 ){
				fprintf(errorFile, "Error you cannot assign to these predefined read only variables\n");
				break;
			}

			if (strcmp(current->assignment.exp->variable.id,"gl_FragColor") ==0 || strcmp(current->assignment.exp->variable.id,"gl_FragDepth") == 0|| strcmp(current->assignment.exp->variable.id, "gl_FragCoord")	==0)	{
				fprintf(errorFile, "Error you cannot read these write only variables\n");
			}

			break;
		}
		case NESTED_SCOPE_NODE: {fprintf(errorFile,"Reached nested_scope\n");break;} //TODO
		case NESTED_EXPRESSION_NODE: {
			fprintf(errorFile,"Reached nested_exp node\n");
			current->type.is_const = current->nested_exp.exp->type.is_const;
		 	current->type.type_name = current->nested_exp.exp->type.type_name;
			fprintf(errorFile,"Type code nested: %d\n",current->type.type_name);
			current->type.vec = current->nested_exp.exp->type.vec;
			
		} 
		case EXP: {
			fprintf(errorFile,"Reached exp_var node\n");
			current->type.is_const = current->exp.variable->type.is_const;
		  	current->type.type_name = current->exp.variable->type.type_name;
			fprintf(errorFile,"Type code exp_var: %d\n",current->type.type_name);
			current->type.vec = current->exp.variable->type.vec; 
			break; }
		case DECLARATION_NODE:{		
			fprintf(errorFile,"Reached declaration\n");

			if (strcmp(current->declaration.id, "gl_TexCoord") == 0 || strcmp(current->declaration.id, "gl_Color") == 0 || strcmp(current->declaration.id, "gl_Secondary") == 0 || 
				strcmp(current->declaration.id, "gl_FogFragCoord") == 0 || strcmp(current->declaration.id, "gl_Light_Half") == 0 || strcmp(current->declaration.id, "gl_Light_Ambient") == 0 ||
				strcmp(current->declaration.id, "gl_Material_Shininess") == 0 || strcmp(current->declaration.id, "env1") == 0 || strcmp(current->declaration.id, "env2") == 0 || 
				strcmp(current->declaration.id, "env3") == 0 ){
				fprintf(errorFile, "Error you cannot assign to these predefined read only variables\n");
				break;
			}

			if (in_scope(current->declaration.id)) { //call functiont that tells me whether it is already in that scope
				fprintf(errorFile, "Error, declaration already exists in this scope\n");
				break;
			}

			if (current->declaration.exp) {
				//it's initialized
				new_element.is_init = 1;
				if (current->declaration.exp->exp.variable->type.type_name != current->declaration.type_node->type.type_name) {
					fprintf(errorFile, "Error: both sides of expression must be of same type\n");
					break;
				}
			}

			_entry new_element;

			new_element.id = current->declaration.id;
			new_element.is_const = current->declaration.is_const;
			new_element.type_name = current->declaration.type_node->type.type_name;
			new_element.vec = current->declaration.type_node->type.vec;

			new_entry(new_element); //add it to the symbol table
		 	break;}
		default: {break;}
	}
	
}
  
