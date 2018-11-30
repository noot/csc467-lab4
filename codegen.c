#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "codegen.h"
#include "parser.tab.h"
#include "ast.h"
#include "symbol.h"
#include <string.h>
#include "semantic.h"

instr *ins_list; // beginning instruction

char *ZERO = "0";

void gen_code(node *ast) {
	ast_visit(0, ast, NULL, gen_code_post);
}

// void generateAssembly(node* curr, int regNum) {
// 	if (isArithmeticOp(label(&curr))) {
// 		generateAssembly(curr->left, regNum);
// 		generateAssembly(curr->right, regNum +1);
// 		generateArithmeticOp(label(&curr),regNum,regNum+1);
// 	} 
// 	else if (curr == assignment){
// 		print_mov(label(&curr), regNum);
// 	}
// }

// void print_mov(char *val, int reg) {
// 	fprintf(dumpFile, 'MOV %s r%d\n', val, reg);
// }

bool t1_taken = false;
bool t2_taken = false;
char* tempRegName;

char* get_temp_reg(node *n) {
	if (!t1_taken) return "t1";
	else if (!t2_taken) return "t2";
	else return NULL;
}

char* assign_temp_variable(node *n){
	//if (n == variable){
	//	return variable.id;
	//}
}



char* get_assigned_reg(int var_name) { //or create a hash map
	switch (var_name) {
		case GL_FRAGCOLOR:
			return "result.color";
		case GL_FRAGDEPTH:
			return "result.depth";
		case GL_FRAGCOORD:
			return "fragment.position";
		case GL_TEXCOORD:
			return "fragment.texcoord";
		case GL_COLOR:
			return "fragment.color";
		case GL_SECONDARY:
			return "fragment.color.secondary";
		case GL_FOGFRAGCOORD:
			return "fragment.fogcoord";
		case GL_LIGHT_HALF:
			return "state.light[0].half";
		case GL_LIGHT_AMBIENT:
			return "state.lightmodel.ambient";
		case GL_MATERIAL_SHININESS:
			return "state.material.shininess";
		case ENV1:
			return "program.env[1]";
		case ENV2:
			return "program.env[2]";
		case ENV3:
			return "program.env[3]";
		default:
			return NULL;
	}
}

void gen_code_post(node *curr, int i) {
	instr *ins;

	switch(curr->kind) {
		case SCOPE_NODE: 
			break;

		case BINARY_OP_NODE: {
			char *temp = get_temp_reg(curr);
			int op = curr->binary_expr.op;
			node *left = curr->binary_expr.left;
			node *right = curr->binary_expr.right;
			char* regType = "TEMP";
			append_instr(DECLARATION, NONE, temp, NULL, NULL, NULL,regType);

			char* reg1 = get_temp_reg(left);
			char* reg2 = get_temp_reg(right);


			switch(op){
				case ADD: {
					append_instr(OPERATION, ADD_, reg1, reg2, NULL, temp, regType);
					break;
				}
				case SUB: {
					append_instr(OPERATION, SUB_, reg1, reg2, NULL, temp, regType);
					break;
				}
				
				case MUL:{	
					append_instr(OPERATION, MUL_, reg1, reg2, NULL, temp, regType);
					break;
				}

				case EXP:{	
					append_instr(OPERATION, POW, reg1, reg2, NULL, temp, regType);
					break;
				}

				case LEQ:{	
					append_instr(OPERATION, CMP, reg1, reg2, NULL, temp, regType);
					break;
				}

				case GEQ:{	
					append_instr(OPERATION, CMP, reg1, reg2, NULL, temp, regType);
					break;
				}

				case EQ:{
					append_instr(OPERATION, CMP, reg1, reg2, NULL, temp, regType);
					break;
				}
				default: {
					break;
				}
			}

			break;
		}

		case UNARY_OP_NODE: {
			char *temp = get_temp_reg(curr);
			char* regType = "TEMP";

			int op = curr->unary_expr.op;
			node *right = curr->unary_expr.right;
			append_instr(DECLARATION, NONE, temp, NULL, NULL, NULL, regType);
			append_instr(OPERATION, MOV, temp, "%s", NULL, NULL,regType);

			char* reg = get_temp_reg(right);

			switch(op) {
				case SUB: {
					append_instr(OPERATION, SUB_, temp, ZERO, reg, NULL, regType);
					break;
				}
				default:
					break;
			}

			break;
		}

		case ASSIGNMENT_NODE: {
			_entry *findVar;
			_entry *findExp;

			node* leftHandSide = curr->assignment.variable;
			node* rightHandSide = curr->assignment.exp;
			char* regType = "TEMP";
			char* inputReg = '/0';

			//need to fetch that variable from the symbol table
			findVar = find_entry(leftHandSide->variable.id); //call to findEntry
			findExp = find_entry(rightHandSide->variable.id); //call to findEntry
			
			char *resultReg = get_temp_reg(findVar); //if variable exists, assign to this 
			if (findExp){
				char *inputReg = get_temp_reg(findExp); //if the right hand side is also a declared variable, use that name
			}
			else{
				char* inputReg = get_temp_reg(rightHandSide); //else use the expression variable
			}
		
			char* outputReg = get_temp_reg(findVar);
			//char* reg1 = assign_temp_variable(leftHandSide);
			//char* reg2 = assign_temp_variable(rightHandSide);

			append_instr(DECLARATION, NONE, inputReg, NULL, NULL, outputReg, regType); //this would be a move operation 
			break; 
		}
	
		case DECLARATION_NODE: {
			int is_const = curr->declaration.is_const;
			char id = curr->declaration.id; 
			node* type = curr->declaration.type;
			node* exp = curr->declaration.exp;
			char* regType = "TEMP";

			//is constant				
			if (is_const){
				regType = "CONST";
			}

			//if it's initialized
			if (exp){
				node* leftHandSide = id;
				node* rightHandSide = exp;

				resultReg = get_temp_reg(leftHandSide);
				inputReg = get_temp_reg(rightHandSide);

				append_instr(DECLARATION, NONE, reg1, NULL, NULL, resultReg, regType); //mov instruction
			}
			//not initialized, so set to zero
			else{
				node* leftHandSide = id;
				node* rightHandSide = 0;

				resultReg = get_temp_reg(leftHandSide); //change this to variable name
				inputReg = get_temp_reg(rightHandSide);

				append_instr(DECLARATION, NONE, inputReg, NULL, NULL, resultReg, regType); //mov instruction 
			}
			// assign expression to variable
			//append_instr(OPERATION, MOV, curr->assignment.exp->variable.id, NULL, NULL, curr->assignment.variable->variable.id);
			break;
		}


		case STATEMENT_NODE: {
			if (curr->statement.is_if){
				// branch
			}
			break;			
		}


		case EXP_NODE: {
			break;
		}

		case VAR_NODE: {
			char* regType = "TEMP";
			/*char* id = curr->variable.id;
			int is_vec = curr->is_vec;
			int idx = curr->idx;*/

			//other variables need to be able to access the name

			//this is where we assign the final name of the variable --> i.e. register name will be variable name
			
			char regname
			if (var.idx == 0){
			}
			else if (var.idx == 1){
				curr = regname.x
			}
			else if (var.idx == 2){
				curr = regname.y
			}
			else if (var.idx == 3){
				curr = regname.z
			}
			else if (var.idx == 4){

			}
			// assign register with same name as variable
			append_instr(DECLARATION, NONE, curr->variable.id, NULL, NULL, NULL,regType);
			break;
		}

		case ARGUMENTS_NODE: {
			
			break;
		}
		
		case CONSTRUCTOR_NODE: {
			char* regType = "TEMP";
			node* type = curr->constructor.type;
			node* args = curr->constructor.args->arguments;
			
			node* ptr = args;
			int counter = 0;

			while(ptr!=NULL){
				counter = counter + 1;
				if (counter == 1){
					regname.x = exp;
				}
				else if (counter == 2){
					regname.y = exp;
				}
				else if (counter  == 3){
					regname.z = exp;
				}
				else if(counter == 4){
					regname.w = exp;
				}
				ptr = ptr->arguments.args;
			}
			
			break;
		}

		case FUNCTION_NODE: {
			char* regType = "TEMP";
			char *temp = get_temp_reg(curr);
			switch(curr->function.function_name) {
				case DP3: {
					// move this part to ARGUMENTS_NODE ?
					node *arg1 = curr->function.args->arguments.exp;
					node *arg2 = curr->function.args->arguments.args->arguments.exp;
					// declarate the arguments
					append_instr(DECLARATION, NONE, arg1->exp.variable->variable.id, NULL, NULL, NULL,regType);
					append_instr(DECLARATION, NONE, arg2->exp.variable->variable.id, NULL, NULL, NULL,regType);
					// move args to temp regs
					// call DP3
					append_instr(OPERATION, DP3, arg1->exp.variable->variable.id, arg2->exp.variable->variable.id, NULL, temp);
					break;
				}
				case RSQ: {
					break;
				}
				case LIT: {
					break;
				}
			}
			break;
		}

		// todo: need to figure out how to store INT in registers
		case INT_NODE: {
			char* regType = "TEMP";
			char *temp = get_temp_reg(curr);
			append_instr(DECLARATION, NONE, temp, NULL, NULL, NULL,regType);
			break;
		}

		case FLOAT_NODE: {
			char* regType = "TEMP";
			char *temp = get_temp_reg(curr);
			append_instr(DECLARATION, NONE, temp, NULL, NULL, NULL,regType);
			break;		
		}

		case BOOL_NODE: {
			char* regType = "TEMP";
			char *temp = get_temp_reg(curr);
			append_instr(DECLARATION, NONE, temp, NULL, NULL, NULL,regType);
		}

		default: break;
	}
}

char* get_op_char(int op) {
	switch(op) {
		case ABS: return "ABS";
		case ADD_: return "ADD";
		case CMP: return "CMP";
		case COS: return "COS";
		case DP3: return "DP3";
		case DP4: return "DP4";
		case DPH: return "DPH";
		case DST: return "DST";
		case EX2: return "EX2";
		case FLR: return "FLR";
		case FRC: return "FRC";
		case KIL: return "KIL";
		case LG2: return "LG2";
		case LIT: return "LIT";
		case LRP: return "LRP";
		case MAD: return "MAD";
		case MAX: return "MAX";
		case MIN: return "MIN";
		case MOV: return "MOV";
		case MUL_: return "MUL"; 
		case POW: return "POW";
		case RCP: return "RCP";
		case RSQ: return "RSQ";
		case SCS: return "SCS";
		case SGE: return "SGE";
		case SIN: return "SIN";
		case SLT: return "SLT";
		case SUB_: return "SUB";
		case SWZ: return "SWZ";
		case TEX: return "TEX";
		case TXB: return "TXB";
		case TXP: return "TXP";
		default: return NULL;
	}
}

void append_instr(int is_op, op_type op, char *in1, char *in2, char *in3, char *out, char* regType) {
	instr *ins = (instr *)malloc(sizeof(instr));
	ins->is_op = is_op;
	ins->op = op;
	ins->in1 = in1;
	ins->in2 = in2;
	ins->in3 = in3;
	ins->out = out;
	ins->regType = regType;


	if (!ins_list) {
		ins_list = ins;
	} else {
		ins_list->next = ins;
		ins_list = ins;
	}
} 

void print_instr(instr *ins) {

	if(ins->is_op == OPERATION) {
		if(ins->out != NULL && ins->in1 != NULL) {
			fprintf(dumpFile, "%s %s %s\n", get_op_char(ins->op), ins->out, ins->in1);
		} else if (ins->out != NULL && ins->in1 != NULL && ins->in2 != NULL) {
			fprintf(dumpFile, "%s %s %s %s\n", get_op_char(ins->op), ins->out, ins->in1, ins->in2);
		} else if (ins->out != NULL && ins->in1 != NULL && ins->in2 != NULL && ins->in3 != NULL) {
			fprintf(dumpFile, "%s %s %s %s %s\n", get_op_char(ins->op), ins->out, ins->in1, ins->in2, ins->in3);
		}
	} else if (ins->is_op == DECLARATION) {
		if (ins->regType == "TEMP"){
			fprintf(dumpFile, "TEMP MOV %s %s\n", ins->in1, ins->out);
		}
		else if (ins->regType == "CONST"){
			fprintf(dumpFile, "CONST MOV %s %s\n", ins->in1, ins->out);
		}
	}
}

// void generateArithmeticOp(int op, char* reg1, char* reg2) {
// 	switch (op) {
// 		case ADD:
// 			fprintf(dumpFile, 'ADD %s %s\n', reg1, reg2);
// 		case SUB:
// 			fprintf(dumpFile, 'SUB %s %s\n', reg1, reg2);
// 		case MUL:
// 			fprintf(dumpFile, 'MUL %s %s\n', reg1, reg2);
// 		case DIV:
// 			fprintf(dumpFile, 'DIV %s %s', reg1, reg2);
// 		}
// }

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

// https://viswesh.github.io/astVisualizer/
// https://resources.jointjs.com/demos/javascript-ast
// https://godbolt.org/
// http://www.eecg.toronto.edu/~enright/teaching/ece243S/notes/
// https://astexplorer.net/
