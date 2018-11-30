#include "ast.h"

#ifndef CODEGEN_H_
#define CODEGEN_H_ 1

typedef enum {
	OPERATION = 1,
	DECLARATION = 0
} is_op;

typedef enum {
	NONE = 0,
	ABS = 1,
	ADD_ = 2,
	CMP = 3,
	COS = 4,
	DP3 = 5,
	DP4 = 6,
	DPH = 7,
	DST = 8,
	EX2 = 9,
	FLR = 10,
	FRC = 11,
	KIL = 12,
	LG2 = 13,
	LIT = 14,
	LRP = 15,
	MAD = 16,
	MAX = 17,
	MIN = 18,
	MOV = 19,
	MUL_ = 20,
	POW = 21,
	RCP = 22,
	RSQ = 23,
	SCS = 24,
	SGE = 25,
	SIN = 26,
	SLT = 27,
	SUB_ = 28,
	SWZ = 29,
	TEX = 30,
	TXB = 31,
	TXP = 32,
	XPD = 33
} op_type;

// typedef enum {
// 	GL_FRAGCOLOR = 1,
// 	GL_FRAGDEPTH = 2,
// 	GL_FRAGCOORD = 3,
// 	GL_TEXCOORD = 4,
// 	GL_COLOR = 5,
// 	GL_SECONDARY = 6,
// 	GL_FOGFRAGCOORD = 7,
// 	GL_LIGHT_HALF = 8,
// 	GL_LIGHT_AMBIENT = 9,
// 	GL_MATERIAL_SHININESS = 10,
// 	ENV1 = 11,
// 	ENV2 = 12,
// 	ENV3 = 13
// } reg_type;

typdef struct constructorReg{
	char* x,y,z,w;
}constructorReg;

typedef struct instr {
	int is_op; // 1 if operation, 0 if declaration
	op_type op;
	char *in1, *in2, *in3;
	char *out;
	struct instr *next;
	char* regType;
} instr;

void gen_code(node *ast);
void gen_code_post(node *ast, int i);
void append_instr(int is_op, op_type op, char *in1, char *in2, char *in3, char *out, char* regType);

#endif 
