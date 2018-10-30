#include <stdio.h>

/*Semantic Checking:
	- Implicit type conversions
	- Operators
	- Conditions
	- Function calls
	- Constructor calls
	- Vector indexing
	- Initialization
	- Assignment
	- Variables
	- Predefined variables
	- Report Semantic Errors*/

//pseudo-code
//traverse through AST
//if kind(node) == statement ]

void stack_{

}

void semantic_check(*node) {
	switch (kind(*node)) {
		case(scope_node):
		case(declarations):
			//pop onto stack
			//go to node on the left and right
			//can't have more than one declaration in same scope
			//needs access to symbol table
			//correct types need to be declared
		case(statements):
			//call semantic_check(*nodeLeft)
			//call semantic_check(*nodeRight)
			//add node to stack
		case(declaration):
			if (exists_in_scope(*node)) {
				printf("Error, already exists in this scope");
			}
			if (assign_predefined(*node)) {
				printf("Error you cannot assign to these predefined read only variables");
			}
			//note: declaration node will have an id
			//call semantic_check(*nodeLeft)
			//call semantic_check(*nodeRight)
			//add node to stack
		case(statement):
			//check assignment error
			if (incorrect_assignment(node)) {
				printf("Error, must assign value of same type");
			}
			//call semantic_check(*nodeLeft)
			//call semantic_check(*nodeRight)
			//add node to stack
		case(else_statement):check_condition();
		case(exp):
			//call semantic_check(*nodeLeft)
			//call semantic_check(*nodeRight)
			//add node to stack
		case(var):
		case(unary_op): 
		case(binary_op): check_operator();
		case(constructor):
		case(function):
		case(function_name):
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
int semantic_check() {

//implicit type conversions:
void implicit_type_check() {
	findVar = symbol_table[var] //var is the declared variable that we are looking for
		if findVar.type == right_hand_side.type{} //define what right_side is
		else { printf("Assigning wrong type to variable"); } //Error 
}

check_operator(*node) {
	//remove from stack
	switch (operatorType) {
		case(SUB):					//arithmetic
			if ((node.right.type == int && node.left.type == int) || (node.right.type == float && node.left.type == float)) {
				//
			}
			else {
				//error
			}
		case(ADD):					//arithmetic
		case(MULT):					//arithmetic
		case(DIV):					//arithmetic
		case(EXP):					//arithmetic
		case(NOT):					//logical
		case(AND):					//logical
		case(OR):					//logical
		case(LESS):					//comparison
		case(LEQ):					//comparison
		case(GREATER):				//comparison
		case(GEQ):					//comparison
		case(EQUAL):				//comparison
		case(NEQ):					//comparison

	}
}

check_conditions() {
	if (*cond_expression.type == bool) {
		//continue
	}
	else {
		//error
		printf("Conditional expression must be of type bool");
	}
}

check_function_calls() {
}


void type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
	ASSERT_UNSPEC;
	node->type(new basic_type(4, basic_type::TYPE_INT));
}

//go through AST
//at each expression, operand etc. do type checking
//write some inference rules

//if type(elem == type_:
//		type(elem + elem) should == type_
//		type(elem - elem) should == type_
//		type(elem * elem) should == type_
//		type(elem / elem) should == type_
//		type(elem = elem) should == type_
int a;
float b;

//wrong:
//a+b, a-b,a*b,a/b,b/a
//a = 4.23, b = 1


