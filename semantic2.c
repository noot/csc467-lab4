#include <stdio.h>


void check_var_scope(node) {
	if node.name not in symbol_table{
		printf("Error you cannot declare variable more than once in same scope");
	}
}
void check_function(node) {
	if (node.name == 'rsq') {
		if (len(args) == 1 && node.args[0].type != float && node.args[0].type != int) {
			printf("Error: Rsq function takes in either a float or an integer type argument");
		}
	}
	else if (node.name == 'dp3') {
		if (len(node.args == 2) && node.args[0].type = vec4 && node.args[1].tpe == vec4 || len(node.args) == 2 && node.args[0].type == vec3 && node.args[1].type == vec3) {
			continue;
		}
		else if (len(node.args == 2) && node.args[0].type = ivec4 && node.args[1].type == ivec4 || len(node.args) == 2 && node.args[0].type == ivec3 && node.args[1].type == ivec3) {
			continue;
		}
		else {
			printf("Error: DP3 function takes in arguments of type vec and ivec");
		}
	}
	else if (node.name == 'lit') {
		if (len(node.args == 1) node.args[0].type != vec4) {
			printf("Error: LIT function only takes in arguments of type vec4");
		}
	}
	else {
		printf("Error: Function name not recognized");
	}
}

char* check_function_name(node) {
	if (node.name == 'rsq') {
		return 'rsq';
	}
	else if (node.name == 'dp3') {
		return 'dp3';
	}
	else if (node.name == 'lit') {
		return 'lit';
	}
}
void implicit_type_check(id_node, exp_node) {
	var = id_node.name;
	findVar = symbol_table[var]; //look in symbol table
	if (findVar.type != exp_node.type) {
		printf("Assigning wrong type to variable");
	}
}

void check_operator(*node) {
	//remove from stack
	switch (operatorType) {
	case(SUB):					//arithmetic
		if ((node.right.type != int && node.left.type != int) && (node.right.type != float && node.left.type != float)) {
			printf("Error: both sides of expression must be of same type");
	case(NOT):					//logical
		if (node.right.type != bool && node.left.type != bool) {
			printf("Error: both sides of expression must be of type boolean");
		}
		}
	}

	void check_conditions() {
		if (*cond_expression.type != bool) {
			printf("Conditional expression must be of type bool");
		}
	}

	bool assign_predefined(*node.id) {
		switch (*node.id) {
		case ('gl_TexCoord'): return false;
		case ('gl_Color'): return false;
		case ('gk_Secondary'): return false;
		case ('gl_FogFragCoord'): return false;
		case ('gl_Light_Half'): return false;
		case ('gl_Light_Ambient'): return false;
		case ('gk_Material_Shininess'): return false;
		case ('env1'): return false;
		case ('env2'): return false;
		case ('env3'): return false;
		}
	}

	void semantic_check(*node) {
		switch (kind(*node)) {
		case(scope_node): exit_scope();
		case(declarations): break;
		case(statements): break;
		case(declaration):
			if (exists_in_scope(*node.id)) {
				printf("Error, declaration already exists in this scope");
			}
			if (!assign_predefined(*node.id)) {
				printf("Error you cannot assign to these predefined read only variables");
			}
			if (node.is_const) {
				if (node.exp.type != int && node.exp.type != bool && node.exp.type && float) {
					printf("Error: you cannot assign an expression to a const variable");
				}
			}
		case(statement):
			if (incorrect_assignment(node)) {
				printf("Error, must assign value of same type");
			}
			implicit_type_check(node.id, node.exp);
		case(else_statement):
			if (condition_exp.type != bool) {
				printf("Error: the conditional expression must be of type boolean");
			}
		case(var):check_var_scope(node);
		case(binary_op): check_operator(node);
			//case(constructor): check_constructor();
		case(function): check_function(node);
		case(function_name): check_function_name(node);
		}
	}
