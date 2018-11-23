#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "semantic.h"

void ast_semantic_check(node* current, int i) {
	if (current == NULL) {
		errorOccurred = 1;
		fprintf(errorFile, "Error: Empty tree visited");
		return;
	}
	node_kind kind = current->kind;

	switch (kind) {
		case UNKNOWN: {break;}
		case SCOPE_NODE: {
			//enter_scope();
			//if (enterScope){ enter_scope();} //
			//else exit_scope();
			fprintf(errorFile,"Reached scope\n");
			break;}

		case DECLARATIONS_NODE: {break; } //break
		case STATEMENTS_NODE: {break;} //break
		//in the unary op node we check whether the appropriate type is pointed to by the node
		case UNARY_OP_NODE: {
			int right = current->unary_expr.right->type.type_name;
			int op = current->unary_expr.op;
			if (op == EXCLAM ){
				if (right!=BOOL_T){
					fprintf(errorFile,"Error: unary expression '!' expects a boolean type");
				}
				
			}
			else{
				if (right == BOOL_T){
					fprintf(errorFile,"Error: unary expression '-' expects an arithmetic type");				
				}
			}

			current->type.type_name = right;
			current->type.vec = current->unary_expr.right->type.vec;
		break;} 
		//in the binary node we check multiple cases according to Table 1 of Lab 3 handout
		// 1) whether the right category types are present
		// 2) whether the correct operand classes are present

		case BINARY_OP_NODE: {
			int left = current->binary_expr.left->type.type_name;
			int right = current->binary_expr.right->type.type_name;
			int op = current->binary_expr.op;

			int num_argsL, num_argsR, num_typeL,num_typeR;

			//convert vectors to their base type

			switch (left) {
				    case(BOOL_T): {num_argsL = 1; num_typeL = BOOL_T;break;}
				    case(FLOAT_T): {num_argsL = 1; num_typeL = FLOAT_T;break;}
				    case(INT_T): {num_argsL = 1; num_typeL = INT_T;break;}
				    case(IVEC4): {num_argsL = 4; num_typeL = INT_T;break;}
				    case(IVEC3): {num_argsL = 3; num_typeL = INT_T;break;}
				    case(IVEC2): {num_argsL = 2; num_typeL = INT_T;break;}
				    case(BVEC4): {num_argsL = 4; num_typeL = BOOL_T;break;}
				    case(BVEC3): {num_argsL = 3; num_typeL = BOOL_T;break;}
				    case(BVEC2): {num_argsL = 2; num_typeL = BOOL_T;break;}
				    case(VEC4): {num_argsL = 4; num_typeL = FLOAT_T;break;}
				    case(VEC3): {num_argsL = 3; num_typeL = FLOAT_T;break;}
				    case(VEC2): {num_argsL = 2; num_typeL = FLOAT_T;break;}
			 }

			switch (right) {
				    case(BOOL_T): {num_argsR = 1; num_typeR = BOOL_T;break;}
				    case(FLOAT_T): {num_argsR = 1; num_typeR = FLOAT_T;break;}
				    case(INT_T): {num_argsR = 1; num_typeR = INT_T;break;}
				    case(IVEC4): {num_argsR = 4; num_typeR = INT_T;break;}
				    case(IVEC3): {num_argsR = 3; num_typeR = INT_T;break;}
				    case(IVEC2): {num_argsR = 2; num_typeR = INT_T; break;}
				    case(BVEC4): {num_argsR = 4; num_typeR = BOOL_T;break;}
				    case(BVEC3): {num_argsR = 3; num_typeR = BOOL_T;break;}
				    case(BVEC2): {num_argsR = 2; num_typeR = BOOL_T;break;}
				    case(VEC4): {num_argsR = 4; num_typeR = FLOAT_T;break;}
				    case(VEC3): {num_argsR = 3; num_typeR = FLOAT_T;break;}
				    case(VEC2): {num_argsR = 2; num_typeR = FLOAT_T;break;}
			 }
			if (op == EQ||op == NEQ){
				if (left == right){
					current->type.type_name = BOOL_T; //assign it bool and not bvec for if conditions
					if (num_argsL > 1 && num_argsR > 1){
						if (num_argsL != num_argsR){
							fprintf(errorFile,"Error: Expecting same size vectors on both sides of operator\n");
							break;		
							//FIX ME: ASSIGN THE VECTOR TYPE TO CURRENT											
						}	
						else{
							current->type.vec = num_argsL;
							break;
						}
					}

					else{
						current->type.vec = 1;
						break;
					}

					
				}	
				else{
					fprintf(errorFile,"Error: Expecting same type on both sides of operator\n");
					break;
				}
			}

			else if (op == MUL ){
				if (right == left){
					if (num_argsL > 1 && num_argsR > 1 ){
						current->type.type_name = left;
						current->type.vec = num_argsL;
						break;
					}							
				
					else{
						current->type.type_name = left;
						current->type.vec = 1;
						break;			
					}

				}
				else if (num_typeL == num_typeR){ //base type is same
					if (num_argsL > 1 && num_argsR >1){ //vector*vector
						if (num_argsL != num_argsR){
							fprintf(errorFile,"Error: Expecting same size vectors on both sides of multiplication operator");	
							break;						
						}
						current->type.type_name = left;
						current->type.vec = num_argsL;
						break;

					}
					else if (num_argsL == 1 && num_argsR == 1){ //both scalars
						current->type.type_name = left;
						current->type.vec = 1;
						break;	
						
					}
					else if ((num_argsL == 1 && num_argsR > 1) || (num_argsR == 1 && num_argsL > 1)){ //vector*scalar or scalar*vector
						if (num_argsL > 1){ //vector on left side
							current->type.type_name = left;
							current->type.vec = num_argsL;
							break;
						}
						else{
							current->type.type_name = right; //vector on right side
							current->type.vec = num_argsR;
							break;
						}
						
					}
				}

				else{
					fprintf(errorFile,"Error: these types are not compatible for multiplication\n");
					break;	
				}
			}
			else if (op == ADD || op == SUB){
				if (left == right){
					if (num_argsL == 1 && num_argsR == 1){ //scalars and scalar
						current->type.type_name = left;
						current->type.vec = 1;
						break;
					}
					else if (num_argsL >1 && num_argsR >1 ){ //vector and vector
						current->type.type_name = left;
						current->type.vec = num_argsL;
						break;	
					}

					else{
						fprintf(errorFile,"Error: Expecting same type on both sides\n");
						break;			
					}

				}

				else if (num_typeR == num_typeL){
					if (num_argsL != num_argsR){
						fprintf(errorFile,"Error: Vectors must be of same length\n");	
						break;					
					}

					current->type.type_name = BOOL_T;
					current->type.vec = 1;
					break;
				}	
				else{
					fprintf(errorFile,"Error: Expecting same type on both sides\n");
					break;
				}
			}
			else if (op == ADD||op == SUB||op == MUL||DIV||EXP){
				if (left == right){
					current->type.type_name = left;
					current->type.vec = 1;
				}
				else{
					fprintf(errorFile,"Error: Expecting same type on both sides\n");
					break;
				}
			}


			else if (op == DIV || op == EXP){
				if (right!=left){
					fprintf(errorFile,"Error: Vectors must be of same type\n");
					break;						
				}
				if (num_argsR != 1 && num_argsL!=1){
					fprintf(errorFile,"Error: Only scalar values are acceptable here\n");
					break;						
				}
			}
			else if (op == GEQ || op == LEQ || op == GREATER || op == LESS){
				if ((num_argsL != 1 && num_argsR != 1) || num_argsL != 1 || num_argsR !=1){
					fprintf(errorFile, "Error: you cannot use these operators with vectors");
					break;
				}
				if (left == right){
					current->type.type_name = BOOL_T;
					current->type.vec = 1;
					break;
				}
				else{
					fprintf(errorFile,"Error: Expecting same type on both sides");
					break;}
			}

			else if (op == AND || op == OR){
				if (left != right){ //not the same types
					fprintf(errorFile,"Error: Expecting same type on both sides");
					break;	
				}

				if (num_argsL >1 && num_argsR > 1){ //vector-vector
					if (num_argsL != num_argsR){
						fprintf(errorFile,"Error: Vectors must be of same length");
						break;
					}
					
				}
				else if ((num_argsL > 1 && num_argsR == 1) || (num_argsL == 1 &&num_argsR > 1)){ //vector-scalar or scalar-vector
					fprintf(errorFile,"Error: Cannot do scalar-vector or vector-scalar \n");	
					break;
				}
				else if (num_argsL == 1 && num_argsR ==1 ){ //scalar-scalar
					break;
				}
			}

			break;
			
		}
			
		case INT_NODE:{
			current->type.type_name = INT_T;
			current->type.vec = 1;
			current->type.is_const = 1;
			break;}
		case FLOAT_NODE:{
			current->type.type_name = FLOAT_T;
			current->type.vec = 1;
			current->type.is_const = 1;
			break; }
		//for var node we check if the variable has been declared in the scope, whether it has been initialized
		//and whether it is a predefined variable. we also check that sorrect vector indexing is done.
		case VAR_NODE: {
			//check if it exists in symbol table first
			_entry *findVar;
			findVar = find_entry(current->variable.id);

			if(findVar!= NULL){
				//declared already
				findVar = find_entry(current->variable.id); //find_entry function calls from scope

			if(findVar!= NULL){
				//if it has been initialized
				if (findVar->is_init == 0){
					fprintf(errorFile, "Error: this variable hasn't been initialized");
					break;
					
				}

			}

			else if (findVar == NULL){
				fprintf(errorFile, "Error: this variable hasn't been declared"); 
			}
			
			else{
				if ((!strcmp(current->variable.id, "gl_TexCoord") == 0) && (!strcmp(current->variable.id, "gl_Color") == 0) && !strcmp(current->variable.id, "gl_Secondary") == 0 && 
					!strcmp(current->variable.id, "gl_FogFragCoord") == 0 && !strcmp(current->variable.id, "gl_Light_Half") == 0 && !strcmp(current->variable.id, "gl_Light_Ambient") == 0 &&
					!strcmp(current->variable.id, "gl_Material_Shininess") == 0 && !strcmp(current->variable.id, "env1") == 0 && !strcmp(current->variable.id, "env2") == 0 && 
					!strcmp(current->variable.id, "env3") == 0 && !strcmp(current->variable.id, "gl_FragColor") == 0 && !strcmp(current->variable.id, "gl_FragDepth") == 0 && 
					!strcmp(current->variable.id, "gl_FragCoord") == 0){
					fprintf(errorFile, "Error: this variable has not been declared\n");
					break;
				}
				break;
			}
			current->type.is_const = findVar->is_const;
			current->type.type_name = findVar->type_name;

			if((findVar->type_name == BOOL_T || findVar->type_name == FLOAT_T || findVar->type_name == INT_T) && findVar->is_vec == 2){
				int max_index = findVar->is_vec;
				if (current->variable.idx >= max_index){
					fprintf(errorFile, "Error: Index exceeds bounds");
					break;
				}
			}

			if((findVar->type_name == BOOL_T || findVar->type_name == FLOAT_T || findVar->type_name == INT_T) && findVar->is_vec == 3){
				int max_index = findVar->is_vec;
				if (current->variable.idx >= max_index){
					fprintf(errorFile, "Error: Index exceeds bounds");
					break;
				}
			}
			if((findVar->type_name == BOOL_T || findVar->type_name == FLOAT_T || findVar->type_name == INT_T) && findVar->is_vec == 4){
				int max_index = findVar->is_vec;
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
				current->type.type_name = VEC4;
				current->type.vec = 4;
				current->type.is_const = 0;
				fprintf(errorFile, "Error: you cannot use predefined variables as variable names");
				break; 	
			}
	
			else if (strcmp(curr, "gl_Light_Half") == 0 || strcmp(curr, "gl_Light_Ambient") == 0 ||
			strcmp(curr, "gl_Material_Shininess") == 0 || strcmp(curr, "env1") == 0 || strcmp(curr, "env2") == 0 || 
			strcmp(curr, "env3") == 0){
				current->type.type_name = VEC4;
				current->type.vec = 4;
				current->type.is_const = 1;
				fprintf(errorFile, "Error: you cannot use predefined variables as variable names");
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
					current->type.type_name = VEC4;
					current->type.vec = 4;
				}

				fprintf(errorFile, "Error: you cannot use predefined variables as variable names");
				break; 	
			}

			
			break;
		}
				
		//for all three functions we make sure that the correct function names are being called and 
		//that their return types and argument types are correct according to Lab 3 handout
		case FUNCTION_NODE:{
			node* next_arg = current->function.args;

			//if no arguments, return error
			if (current->function.args == NULL){
				fprintf(errorFile, "Error: must have arguments, cannot be void\n");
				break;
			}

			//get next arguments
			next_arg = next_arg->arguments.args;
	

			//current->type.type_name will be the return type of the functiontion
			if (current->function.function_name == 2) { //rsq
				if (!(next_arg->type.type_name == FLOAT_T ||next_arg->type.type_name == INT_T )) {
					fprintf(errorFile,"Error: Expecting float or integer for first argument\n");
					break;
				}
				
				next_arg = next_arg->arguments.args;
				if (next_arg != NULL){
					fprintf(errorFile,"Error: Too many arguments for this function\n");					
				}

				current->type.type_name = FLOAT_T;
				current->type.vec = 1;

			}

			else if (current->function.function_name == 0) { //dp3
				//case:vec4
				if (next_arg == NULL){
					fprintf(errorFile, "Error: Input argument cannot be void\n");	
					break;
				}
				if (!(next_arg->type.type_name == VEC4 && next_arg->type.vec == 4)){
					fprintf(errorFile, "Error: First argument is of incorrect type\n");	
					break;
				}
				else if (next_arg->type.type_name == VEC4 && next_arg->type.vec == 4){
					next_arg = next_arg->arguments.args;
					if (next_arg == NULL){
						fprintf(errorFile, "Error: Input argument cannot be void\n");	
						break;
					}
					if (!(next_arg->type.type_name == VEC4 && next_arg->type.vec == 4)) {
						fprintf(errorFile, "Error: Second argument is of incorrect type\n");	
						break;
					}
	
				}
		
				//other vector type case:vec3
				if (next_arg == NULL){
					fprintf(errorFile, "Error: Input argument cannot be void\n");		
					break;
				}

				if (!(next_arg->type.type_name == VEC3 && next_arg->type.vec == 3)){
					fprintf(errorFile, "Error: First argument is of incorrect type\n");		
					break;
				}
				else if (next_arg->type.type_name == VEC3 && next_arg->type.vec == 3){
					next_arg = next_arg->arguments.args;
					if (next_arg == NULL){
						fprintf(errorFile, "Error: Input argument cannot be void\n");	
						break;
					}
					if (!(next_arg->type.type_name == VEC3 && next_arg->type.vec == 3)) {
						fprintf(errorFile, "Error: Second argument is of incorrect type\n");
						break;
					}
	
				}
				//other vector type case:ivec4
				if (!(next_arg->type.type_name == IVEC4 && next_arg->type.vec == 4)){
					fprintf(errorFile, "Error: First argument is of incorrect type\n");	
					break;
				}

				else if (next_arg->type.type_name == IVEC4 && next_arg->type.vec == 4){
					next_arg = next_arg->arguments.args;
					if (next_arg == NULL){
						fprintf(errorFile, "Error: Input argument cannot be void\n");
						break;
					}

					if (!(next_arg->type.type_name == IVEC4 && next_arg->type.vec == 4)) {
						fprintf(errorFile, "Error: Second argument is of incorrect type\n");
						break;
					}
	
				}
				//last vector type case: ivec3
				if (!(next_arg->type.type_name == IVEC3 && next_arg->type.vec == 3)){
					fprintf(errorFile, "Error: First argument is of incorrect type\n");
					break;
				}
				else if (next_arg->type.type_name == IVEC3 && next_arg->type.vec == 3){
					next_arg = next_arg->arguments.args;
					if (next_arg == NULL){
						fprintf(errorFile, "Error: Input argument cannot be void\n");
						break;
					}

					if (!(next_arg->type.type_name == IVEC3 && next_arg->type.vec == 3)) {
						fprintf(errorFile, "Error: Second argument is of incorrect type\n");
						break;
					}
	
				}
				current->type.type_name = FLOAT_T;
				current->type.vec = 1;
				break;
			}

			else if (current->function.function_name == 1) { //lit
				if (!(next_arg->type.type_name == VEC4 && next_arg->type.vec == 4)) {
					fprintf(errorFile,"Error: LIT function only takes in arguments of type vec4\n");
					break;
				}

				current->type.type_name = VEC4;
				current->type.type_name = 4;
				break;
			}
			else {
				fprintf(errorFile,"Error: Function name not recognized\n");
					break;
			}
		}
		//we make sure that the type and arguments type of the contructor is correct
        	case CONSTRUCTOR_NODE:{
		     	    int num_args, num_type;
			    int i = 0;
			    node* next_arg = current->constructor.args;
			    if (next_arg == NULL) {
					fprintf(errorFile, "Error: constructor cannot be empty");
				break;
			    }

			    switch (current->constructor.type->type.type_name) {
				    case(BOOL_T): {num_args = 1; num_type = BOOL_T;break;}
				    case(FLOAT_T): {num_args = 1; num_type = FLOAT_T;break;}
				    case(INT_T): {num_args = 1; num_type = INT_T;break;}
				    case(IVEC4): {num_args = 4; num_type = INT_T;break;}
				    case(IVEC3): {num_args = 3; num_type = INT_T;break;}
				    case(IVEC2): {num_args = 2; num_type = INT_T; break;}
				    case(BVEC4): {num_args = 4; num_type = BOOL_T;break;}
				    case(BVEC3): {num_args = 3; num_type = BOOL_T;break;}
				    case(BVEC2): {num_args = 2; num_type = BOOL_T;break;}
				    case(VEC4): {num_args = 4; num_type = FLOAT_T;break;}
				    case(VEC3): {num_args = 3; num_type = FLOAT_T;break;}
				    case(VEC2): {num_args = 2; num_type = FLOAT_T;break;}
			    }
				
			    do{
					if (next_arg == NULL || next_arg->arguments.args == NULL) {
						fprintf(errorFile, "Error: constructor arguments cannot be null\n");
						break;

					}
					if (next_arg->arguments.args->type.type_name!= num_type) { //type of construconstructor itself
						fprintf(errorFile, "Error: incorrect constructor argument type\n");
					}

					else{
						//fprintf(errorFile, "This argument type is correct\n");

					}
					i++;
					next_arg = next_arg->arguments.args
;

			    }while (i < num_args && next_arg);

			    current->type.vec = num_args;
			    current->type.type_name = current->constructor.type->type.type_name; //num_type;
			    //current->variable.is_vec = 1;		
			break;} 

		case ARGUMENTS_NODE:{	
			if (current->arguments.exp){
				//arguments must be of expected type  
				current->type.is_const = current->arguments.exp->type.is_const;	
				current->type.type_name = current->arguments.exp->type.type_name;
				current->type.vec = current->arguments.exp->type.vec;
				break;
			}
			else{
				current->type.is_const = 0; 
				current->type.type_name = -1; //assign a random non-token number
				current->type.vec = 1;
				break;
				
			}
		}
		case BOOL_NODE:{
			current->type.type_name = BOOL_T;
			current->type.vec = 1;
			current->type.is_const = 1;
			break;}

		case STATEMENT_NODE:{		
			if (!(current->statement.exp->type.type_name == BOOL_T && current->statement.exp->type.vec == 1)){ //can't be bvec
				fprintf(errorFile, "Error: conditional expression must be of type boolean\n");	
				break;		
			}
		}
		//we make sure that both sides of the assignment have the same type and that predefined variables are dealt with properly
		case ASSIGNMENT_NODE:{ 
			_entry *findVar;
			findVar = find_entry(current->assignment.variable->variable.id); //call to find var
			if (findVar == NULL){
				fprintf(errorFile, "Error: this variable has not been declared\n");  //Implicit type conversions
			}
			if (findVar->is_init){
				if (findVar->type_name != current->assignment.exp->type.type_name){
					fprintf(errorFile, "Error: both sides of assignment must be of same type\n");  //Implicit type conversions
					break;
				}
			
			}
			if (findVar->is_const){
				if(current->assignment.exp){
					fprintf(errorFile, "Error: cannot assign to a const variable\n");
					break;
				}
			}

			if (strcmp(current->assignment.variable->variable.id, "gl_TexCoord") == 0 || strcmp(current->assignment.variable->variable.id, "gl_Color") == 0 || strcmp(current->assignment.variable->variable.id, "gl_Secondary") == 0 || 
				strcmp(current->assignment.variable->variable.id, "gl_FogFragCoord") == 0 || strcmp(current->assignment.variable->variable.id, "gl_Light_Half") == 0 || strcmp(current->assignment.variable->variable.id, "gl_Light_Ambient") == 0 ||
				strcmp(current->assignment.variable->variable.id, "gl_Material_Shininess") == 0 || strcmp(current->assignment.variable->variable.id, "env1") == 0 || strcmp(current->assignment.variable->variable.id, "env2") == 0 || 
				strcmp(current->assignment.variable->variable.id, "env3") == 0 ){
				fprintf(errorFile, "Error you cannot assign to these predefined read only variables\n");
				break;
			}


			//if (strcmp(current->assignment.exp->variable.id,"gl_FragColor") ==0 || strcmp(current->assignment.exp->variable.id,"gl_FragDepth") == 0|| strcmp(current->assignment.exp->variable.id, "gl_FragCoord")	==0)	{
			//	fprintf(errorFile, "Error you cannot read these write only variables\n");
			//}
			break;
		}
		case EXP: {
			if (strcmp(current->assignment.exp->variable.id,"gl_FragColor") ==0 || strcmp(current->assignment.exp->variable.id,"gl_FragDepth") == 0|| strcmp(current->assignment.exp->variable.id, "gl_FragCoord")	==0)	{
				fprintf(errorFile, "Error you cannot read these write only variables\n");
			}

			current->type.is_const = current->exp.variable->type.is_const;
		  	current->type.type_name = current->exp.variable->type.type_name;
			current->type.vec = current->exp.variable->type.vec; 
			break; }

		//we make sure that declarations do not exist more than once in same scope and that predefined variables are handled
		//according to Lab 3 handout
		case DECLARATION_NODE:{		

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



			entry *new_element = (entry *)malloc(sizeof(entry));

			if (current->declaration.exp) {
				//it's initialized
				new_element->is_init = 1;
				if (current->declaration.exp->exp.variable->type.type_name != current->declaration.type->type.type_name) {
					fprintf(errorFile, "Error: both sides of expression must be of same type\n");
					break;
				}
			}


			new_element->id = current->declaration.id;
			new_element->is_const = current->declaration.is_const;
			new_element->type_name = current->declaration.type->type.type_name;
			new_element->is_vec = current->declaration.type->type.vec;

			// new_entry(new_element); //add it to the symbol table
		 	break;}
		default: {break;}
	}
	
}	
}
  

			
