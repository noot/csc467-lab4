#include "ast.h"

typedef enum {
	ABS = 1,
	ADD = 2,
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
	MUL = 20,
	POW = 21,
	RCP = 22,
	RSQ = 22,
	SCS = 23,
	SGE = 24,
	SIN = 25,
	SLT = 26,
	SUB = 27,
	SWZ = 28,
	TEX = 29,
	TXB = 30,
	TXP = 31,
	XPD = 32
} op_type;

typedef struct instr {
	op_type op;
	char *in1, *in2, *in3;
	char *out;
	struct instr *next;
} instr;

void generateCode(node *ast);
