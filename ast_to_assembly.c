
void generateAssembly(node* curr, int regNum) {
	if (isArithmeticOp(label(&curr)) {
		generateAssembly(curr->left, regNum);
		generateAssembly(curr->right, regNum +1);
		generateArithmeticOp(label(&curr),regNum,regNum+1);
	}
	else {
		generateLoad(label(&curr), regNum);
	}
}

bool isArithmeticOp(int opTokenId) {
	if (opTokenId == SUB || opTokenId == ADD || opTokenId == DIV || opTokenId == MUL) {
		return true;
	}
}


main() {
	curr = ast;
	generateAssembly(curr, regNum);
}


//output the assembly to a file

//types of cases to handle
/*
declarations--> in assembly this would just be a move statement
if,else statements --> normally this is a branching statement
vectors --> the registers that can access the elements of a vector
operations --> SUB,ADD,MUL,DIV operations on registers in assembly





*/
