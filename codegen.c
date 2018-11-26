#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "codegen.h"

instr *ins_list; // beginning instruction

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

char* get_reg(node *n) {
	return 0;
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
	}
}

void gen_code_post(node *curr, int i) {
	instr *ins;

	switch(curr->kind) {
		case SCOPE_NODE: 
			break;

		case BINARY_OP_NODE: {
			int op = curr->binary_expr.op;
			node *left = curr->binary_expr.left;
			node *right = curr->binary_expr.right;
			switch(op){
				case ADD_: {
					char* reg1 = get_reg(left);
					char* reg2 = get_reg(right);		
					//fprintf(dumpFile, 'ADD %s %s', reg1, reg2);
					instr *ins = new_instr(1, ADD_, reg1, reg2, NULL, reg1);
				}
				case SUB_: {

				}
				
				case MUL_:{

				}

				default: {

				}
			}

			break;
		}

		case UNARY_OP_NODE: {
			char *tmp = get_reg(curr->unary_expr.right);
			break;
		}

		case ASSIGNMENT_NODE: {
			break;
		}
	
		case DECLARATION_NODE: {
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
			break;
		}

		case ARGUMENTS_NODE: {
			break;
		}
		
		case CONSTRUCTOR_NODE: {
			break;
		}

		case FUNCTION_NODE: {
			break;
		}

		default:
			break;
		
	}

	if (!ins_list) {
		ins_list = ins;
	} else {
		ins_list->next = ins;
		ins_list = ins;
	}

}

// bool isArithmeticOp(int opTokenId) {
// 	if (opTokenId == SUB || opTokenId == ADD || opTokenId == DIV || opTokenId == MUL) {
// 		return true;
// 	}
// }

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

instr *new_instr(int is_op, op_type op, char *in1, char *in2, char *in3, char *out) {
	instr *ins = (instr *)malloc(sizeof(instr));
	ins->is_op = is_op;
	ins->op = op;
	ins->in1 = in1;
	ins->in2 = in2;
	ins->in3 = in3;
	ins->out = out;
	return ins;
} 

void print_instr(instr *ins) {
	if(ins->is_op == OPERATION) {
		if(ins->out != NULL && ins->in1 != NULL) {
			fprintf(dumpFile, "%s %s %s\n", get_op(ins->op), ins->out, ins->in1);
		} else if (ins->out != NULL && ins->in1 != NULL && ins->in2 != NULL) {
			fprintf(dumpFile, "%s %s %s %s\n", get_op(ins->op), ins->out, ins->in1, ins->in2);
		} else if (ins->out != NULL && ins->in1 != NULL && ins->in2 != NULL && ins->in3 != NULL) {
			fprintf(dumpFile, "%s %s %s %s %s\n", get_op(ins->op), ins->out, ins->in1, ins->in2, ins->in3);
		}
	} else if (ins->is_op == DECLARATION) {
		fprintf(dumpFile, "TEMP %s\n", ins->out);
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