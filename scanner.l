%{
/**********************************************************************
 *  Interface to the scanner module for CSC467 course project
 **********************************************************************/


#include "common.h"
#include "parser.tab.h"
#include "string.h"


#define YY_USER_INIT { yyin = inputFile; }
#define	yyinput      input
#define yTRACE(x)    { if (traceScanner) fprintf(traceFile, "TOKEN %3d : %s\n", x, yytext); }
#define yERROR(x)    { fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: %s\n", yyline, x); errorOccurred = TRUE; yyterminate(); }

int CheckInt(void);
int yyline = 1;

   enum yytokentype {
     NUMBER = 256,
     INT = 257,
     FLOAT = 258,
     ADD = 259,
     SUB = 260,
     MUL = 261,
     DIV = 262,
     EXP = 263,
     TRUE = 264;
     FALSE = 265;
     EXCLAM = 266;
     AND = 267;
     OR = 268;
     DOUBLE_EQUAL = 269;
     NOT_EQUAL = 270;
     LESS_THAN = 271;
     MORE_THAN = 272;
     LESS_OR_EQUAL = 273;
     MORE_OR_EQUAL = 274;
     COLON = 275;
     EQUAL = 276;
     CONST = 277;
   };

%}
%option noyywrap

WS          [ \t]

%%
"/*"        	{ yERROR("Change this - comment unimplemented."); }
[ \t\n]			;
[0-9]+		{ yTRACE(INT);}
[0-9]+\.[0-9]+  { yTRACE(FLOAT);}
"+"		{ yTRACE(ADD);}
"-"		{ yTRACE(SUB);}
"*"		{ yTRACE(MULT);}
"/"		{ yTRACE(DIV);}
"^"		{ yTRACE(EXP);}
"true"		{ yTRACE(TRUE);}
"false"		{ yTRACE(FALSE);}
"!"		{ yTRACE(EXCLAM);}
"&&"		{ yTRACE(AND);}
"||"		{ yTRACE(OR);)
"=="		{ yTRACE(EQUAL);}
"!="		{ yTRACE(NOT_EQUAL);}
"<"		{ yTRACE(LESS_THAN);}
">"		{ yTRACE(MORE_THAN);]
"<="		{ yTRACE(LESS_OR_EQUAL);}
">="		{ yTRACE(MORE_OR_EQUAL);}
";"		{ yTRACE(COLON);}
"="		{ yTRACE(EQUAL);}
"const"		{ yTRACE(CONST);}
%%
