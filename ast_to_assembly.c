
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

Example: for demo1
/*

// assign temp an address in memory
// if condition --> NEED TO DO

MUL R2 fragment.color.x fragment.position.x
MOV temp.x R2
MUL R2 fragment.color.y fragment.position.y
MOV temp.y R2
MOV temp.z fragment.color.z
MUL R2 fragment.color.w fragment.position.x
MUL R2 R2 fragment.position.y
MOV temp.z R2

//else condition --> NEED TO DO
MOV result.color temp



*/



*/
