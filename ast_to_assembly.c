
assemblyFile = 'assembly.txt'

void generateAssembly(node* curr, int regNum) {
	if (isArithmeticOp(label(&curr)) {
		generateAssembly(curr->left, regNum);
		generateAssembly(curr->right, regNum +1);
		generateArithmeticOp(label(&curr),regNum,regNum+1);
	}
	else if (curr == assignment){
		generateLoad(label(&curr), regNum);
	}
}


void generateCode(node *curr){
	node_kind kind = curr->kind;
	switch(kind){
		case(BINARY_OP_NODE):{
			op = curr->binary_expr.op;
			left = curr->binary_expr.left;
			right = curr->binary_expr.right;
			switch(op){
				case(ADD):{
					reg1 = assignReg(left);
					reg2 = assignReg(right);		
					fprintf(assemblyFile, 'ADD %s %s', reg1, reg2);
				}
				case(SUB):{}
				case(MUL):{}
				case(DIV):{}
			}
			
		}

		case(ASSIGNMENT_NODE):{
		}
	
		case (DECLARATION_NODE):{
		}

		case (STATEMENT_NODE):{
			if (curr->statement.is_if){
				if (curr->statement.exp == true){
					//branching
				}
			}
		}	

		case(EXP_NODE):{
		}

		case(VAR_NODE):{
		}

		case(ARGUMENTS_NODE):{
			if (curr->arguments
		}
		case(CONSTRUCTOR_NODE):{
		}

		case(FUNCTION_NODE):{
		}

	}
}

bool isArithmeticOp(int opTokenId) {
	if (opTokenId == SUB || opTokenId == ADD || opTokenId == DIV || opTokenId == MUL) {
		return true;
	}
}

void generateArithmeticOp(int op, char* reg1, char* reg2) {
	switch (op) {
		case ADD:
			fprintf(assemblyFile, 'ADD %s %s', reg1, reg2);
		case SUB:
			fprintf(assemblyFile, 'SUB %s %s', reg1, reg2);
		case MUL:
			fprintf(assemblyFile, 'MUL %s %s', reg1, reg2);
		case DIV:
			fprintf(assemblyFile, 'DIV %s %s', reg1, reg2);
		}
}

char* assignReg(char* varName) { //or create a hash map
	switch (varName) {
		case gl_FragColor:
			return "result.color";
		case gl_FragDepth:
			return "result.depth";
		case gl_FragCoord:
			return "fragment.position";
		case gl_TexCoord:
			return "fragment.texcoord";
		case gl_Color:
			return "fragment.color";
		case gl_Secondary:
			return "fragment.color.secondary";
		case gl_FogFragCoord:
			return "fragment.fogcoord";
		case gl_Light_Half:
			return "state.light[0].half";
		case gl_Light_Ambient:
			return "state.lightmodel.ambient";
		case gl_Material_Shininess:
			return "state.material.shininess";
		case env1:
			return "program.env[1]";
		case env2:
			return "program.env[2]";
		case env3:
			return "program.env[3]";
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
//how to manage scope?
//how to manage if else conditions
//moving stack pointer?
//how to manage float to int conversions

https://viswesh.github.io/astVisualizer/
https://resources.jointjs.com/demos/javascript-ast
https://godbolt.org/
http://www.eecg.toronto.edu/~enright/teaching/ece243S/notes/
https://astexplorer.net/


*/
