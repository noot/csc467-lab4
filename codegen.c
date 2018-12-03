#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "codegen.h"
#include "parser.tab.h"
#include "ast.h"
#include "semantic.h"

instr *ins_list; // beginning instruction

// number literals
char *ZERO = "$0";
char *ONE = "$1";

void gen_code(node *ast) {
	ast_visit(0, ast, NULL, gen_code_post);
	print_code();
}

void print_code() {
	while(ins_list) {
		print_instr(ins_list);
		ins_list = ins_list->next;
	}
}

int temp_count = 0;

char* get_temp_reg(node *n) {
	char *temp = (char *)calloc(16, sizeof(char)); 
	sprintf(temp, "tempVar%d", temp_count++);
	return temp;
}

char *get_idx(int i) {
	switch(i) {
		case 0: return "x";
		case 1: return "y";
		case 2: return "z";
		case 3: return "w";
		default: return "";
	}
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
	switch(curr->kind) {
		case SCOPE_NODE: 
			break;

		case BINARY_OP_NODE: {
			char *temp = get_temp_reg(curr);
			//curr->reg_name = temp;
			//char *temp = "temp";
			int op = curr->binary_expr.op;
			node *left = curr->binary_expr.left;
			node *right = curr->binary_expr.right;
			//append_instr(DECLARATION, NONE, NULL, NULL, NULL, temp);

			char* reg1;
			char* reg2;
			if(curr->binary_expr.left && curr->binary_expr.right->exp.variable) {
				reg1 = left->reg_name;
				reg2 = right->reg_name;
				//int len = right->reg_name_len;
				//reg2[len] = '\0';	
			} else {

			}

			//printf("%d\n", len);
			//printf("%s\n", reg2);

			switch(op){
				case ADD: {
					append_instr(OPERATION, ADD_, reg1, reg2, NULL, temp);
					append_instr(OPERATION, MOV, temp, NULL, NULL, curr->reg_name);
					break;
				}
				case SUB: {
					append_instr(OPERATION, SUB_, reg1, reg2, NULL, temp);
					append_instr(OPERATION, MOV, temp, NULL, NULL, curr->reg_name);
					break;
				}
				
				case MUL:{	
					append_instr(OPERATION, MUL_, reg1, reg2, NULL, temp);
					append_instr(OPERATION, MOV, temp, NULL, NULL, curr->reg_name);
					break;
				}

				case EXP:{	
					append_instr(OPERATION, POW, reg1, reg2, NULL, temp);
					append_instr(OPERATION, MOV, temp, NULL, NULL, curr->reg_name);
					break;
				}

				case LEQ:{	
					append_instr(OPERATION, SUB_, reg1, reg2, NULL, temp);
					append_instr(OPERATION, CMP, temp, ONE, ZERO, temp);
					append_instr(OPERATION, MOV, temp, NULL, NULL, curr->reg_name);
					break;
				}

				case GEQ:{	
					append_instr(OPERATION, SUB_, reg1, reg2, NULL, temp);
					append_instr(OPERATION, CMP, temp, ZERO, ONE, temp);
					append_instr(OPERATION, MOV, temp, NULL, NULL, curr->reg_name);
					break;
				}

				case EQ: {
					append_instr(OPERATION, SUB_, reg1, reg2, NULL, temp);
					append_instr(OPERATION, CMP, temp, ZERO, ONE, temp);
					append_instr(OPERATION, MOV, temp, NULL, NULL, curr->reg_name);
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
			curr->reg_name = temp;
			int op = curr->unary_expr.op;
			node *right = curr->unary_expr.right;
			char* reg = right->reg_name;

			switch(op) {
				case SUB: {
					append_instr(OPERATION, SUB_, ZERO, reg, NULL, temp);
					append_instr(OPERATION, MOV, temp, NULL, NULL, curr->reg_name);
					break;
				}
				default:
					break;
			}
			break;
		}

		case ASSIGNMENT_NODE: {
			if(curr->assignment.variable && curr->assignment.exp) {
				char* var_reg = curr->assignment.exp->reg_name;
				append_instr(OPERATION, MOV, var_reg, NULL, NULL, curr->assignment.variable->reg_name);
			}
			break; 
		}
	
		case DECLARATION_NODE: {
			if(!curr->reg_name) {
				append_instr(DECLARATION, NONE, NULL, NULL, NULL, curr->declaration.id);
				curr->reg_name = curr->declaration.id;
			}
			// int len = strlen(curr->declaration.id);
			// char *val = (char *)malloc(len);
			// move initial value to register
			//sprintf(val, "%d", curr->declaration.type->type.int_val);
			if(curr->declaration.exp) {
				char* reg = curr->declaration.exp->reg_name;
				append_instr(OPERATION, MOV, reg, NULL, NULL, curr->reg_name);
			}
			break;
		}


		case STATEMENT_NODE: {
			if (curr->statement.is_if){
				// branch
				//append_instr(DECLARATION, NONE, NULL, NULL, NULL, curr->statement.variable->variable.id);
			}
			break;			
		}


		case EXP_NODE: {
			//if(sizeof(curr->exp.variable->reg_name) > 0) break;
			if(curr->exp.variable) {
				if(curr->exp.variable->variable.is_vec) {
					char *idx = get_idx(curr->exp.variable->variable.idx);
					char *reg_name = (char *)malloc(strlen(curr->exp.variable->variable.id) + 2);
					sprintf(reg_name, "%s.%s", curr->exp.variable->variable.id, idx);
					int len = strlen(reg_name);
					//printf("%d", len);
					reg_name[len] = '\0';

					curr->reg_name = reg_name;
					//printf("%s\n",reg_name);
				} else {
					curr->reg_name = curr->exp.variable->variable.id;
				}
			} else {
				curr->reg_name = get_temp_reg(curr);
			}
			break;
		}

		// case TYPE_NODE: {
		// 	append_instr(DECLARATION, NONE, NULL, NULL, NULL, curr->type.id)
		// 	break;
		// }

		case VAR_NODE: {
			char *id = curr->variable.id;
			//curr->reg_name = id;
			if(strcmp(curr->variable.id, "gl_Color") == 0) {
				id = "fragment.color";
			} else if (strcmp(curr->variable.id, "gl_FragCoord") == 0) {
				//printf("noot");
				id = "fragment.position";
			} else if (strcmp(curr->variable.id, "gl_FragColor") == 0) {
				id = "result.color";
			} else if (strcmp(curr->variable.id, "gl_TexCoord") == 0) {
				id = "fragment.texcoord";
			} else if (strcmp(curr->variable.id, "gl_FragDepth") == 0) {
				id = "result.depth";
			} else if (strcmp(curr->variable.id, "gl_Secondary") == 0) {
				id = "fragment.color.secondary";
			} else if (strcmp(curr->variable.id, "gl_FogFragCoord") == 0) {
				id = "fragment.fogcoord";
			} else if (strcmp(curr->variable.id, "gl_Light_Half") == 0) {
				id = "state.light[0].half";
			} else if (strcmp(curr->variable.id, "gl_Light_Ambient") == 0) {
				id = "state.lightmodel.ambient";
			} else if (strcmp(curr->variable.id, "gl_Material_Shininess") == 0) {
				id = "state.material.shininess";
			} else if (strcmp(curr->variable.id, "env1") == 0) {
				id = "program.env[1]";
			} else if (strcmp(curr->variable.id, "env2") == 0) {
				id = "program.env[2]";
			} else if (strcmp(curr->variable.id, "env3") == 0) {
				id = "program.env[3]";
			} 

			if(curr->variable.is_vec) {
				char *idx = get_idx(curr->variable.idx);
				char *reg_name = (char *)malloc(strlen(id) + 2);
				sprintf(reg_name, "%s.%s", id, idx);
				id = reg_name;
				int len = strlen(reg_name);
				//printf("%d\n", len);
				curr->reg_name_len = len;
				//reg_name[len] = '\0';
				//printf("%s\n", reg_name);
			}

			curr->reg_name = id;

			//printf(curr->reg_name);
			break;
		}

		case ARGUMENTS_NODE: {
			if(curr->arguments.exp) {
				append_instr(DECLARATION, NONE, NULL, NULL, NULL, curr->arguments.exp->reg_name);
			}
			break;
		}
		
		case CONSTRUCTOR_NODE: {
			node* type = curr->constructor.type;
			node* args = curr->constructor.args->arguments.args;
			
			break;

		}

		case FUNCTION_NODE: {
			char *temp = get_temp_reg(curr);
			curr->reg_name = temp;
			switch(curr->function.function_name) {
				case DP3: { // 0
					node *arg1 = curr->function.args->arguments.exp;
					node *arg2 = curr->function.args->arguments.args->arguments.exp;
					// declarate the arguments
					append_instr(DECLARATION, NONE, NULL, NULL, NULL, arg1->reg_name);
					append_instr(DECLARATION, NONE, NULL, NULL, NULL, arg2->reg_name);
					// move args to temp regs
					append_instr(OPERATION, DP3, arg1->reg_name, arg2->reg_name, NULL, temp);
					break;
				}
				case RSQ: { // 1
					node *arg1 = curr->function.args->arguments.exp;
					// declarate the arguments
					append_instr(DECLARATION, NONE, NULL, NULL, NULL, arg1->reg_name);
					// move args to temp regs
					append_instr(OPERATION, RSQ, arg1->reg_name, NULL, NULL, temp);
					break;
				}
				case LIT: { // 2
					node *arg1 = curr->function.args->arguments.exp;
					// declarate the arguments
					append_instr(DECLARATION, NONE, NULL, NULL, NULL, arg1->reg_name);
					// move args to temp regs
					append_instr(OPERATION, LIT, arg1->reg_name, NULL, NULL, temp);
					break;
				}
				default: break;
			}
			break;
		}

		case TYPE_NODE: {
			if(curr->type.vec == 1) {
				char *idx = get_idx(curr->type.vec);
				//strcat(id, idx);
				printf(idx);
			}		
		}

		// todo: need to figure out how to store INT in registers
		// case INT_NODE: {
		// 	char *temp = get_temp_reg(curr);
		// 	append_instr(DECLARATION, NONE, NULL, NULL, NULL, temp);
		// 	char val[64] = {'\0'};
		// 	sprintf(val, "%d", curr->int_v);
		// 	//append_instr(OPERATION, MOV, val, NULL, NULL, temp);
		// 	break;
		// }

		// case FLOAT_NODE: {
		// 	char *temp = get_temp_reg(curr);
		// 	append_instr(DECLARATION, NONE, NULL, NULL, NULL, temp);
		// 	char val[64] = {'\0'};
		// 	sprintf(val, "%d", curr->int_v);
		// 	append_instr(OPERATION, MOV, val, NULL, NULL, temp);
		// 	break;		
		// }

		// case BOOL_NODE: {
		// 	char *temp = get_temp_reg(curr);
		// 	append_instr(DECLARATION, NONE, NULL, NULL, NULL, temp);
		// 	char val[64];
		// 	sprintf(val, "%d", curr->int_v);
		// 	append_instr(OPERATION, MOV, val, NULL, NULL, temp);
		// }

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

void append_instr(int is_op, op_type op, char *in1, char *in2, char *in3, char *out) {
	instr *ins = (instr *)malloc(sizeof(instr));
	ins->is_op = is_op;
	ins->op = op;
	ins->in1 = in1;
	ins->in2 = in2;
	ins->in3 = in3;
	
	ins->out = out;

	//printf("%s\n", out);

	if (!ins_list) {
		ins_list = ins;
	} else {
		ins_list->next = ins;
		ins_list = ins;
	}

	print_instr(ins);
} 

void print_instr(instr *ins) {
	if(ins->is_op == OPERATION) {
		if(ins->out != NULL && ins->in1 != NULL && ins->in2 != NULL && ins->in3 != NULL) {
			fprintf(dumpFile, "%s %s %s\n", get_op_char(ins->op), ins->out, ins->in1);
		} else if (ins->out != NULL && ins->in1 != NULL && ins->in2 != NULL) {
			fprintf(dumpFile, "%s %s %s %s\n", get_op_char(ins->op), ins->out, ins->in1, ins->in2);
		} else if (ins->out != NULL && ins->in1 != NULL) {
			fprintf(dumpFile, "%s %s %s %s %s\n", get_op_char(ins->op), ins->out, ins->in1, ins->in2, ins->in3);
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
