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
//if kind(node) == statement 

void semantic_check() {
	switch (kind(*node)) {
		case(scope_node):
		case(declarations):
		case(statements):
		case(declaration):
		case(statement):
		case(else_statement):
		case(exp):
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


