
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
	generaateAssembly(curr, regNum);
}