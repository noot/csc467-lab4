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
     True = 264;
     False = 265;
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
     LBRACKET = 288;
     RBRACKET = 289;
   };

%}
%option noyywrap

WS          [ \t]

%%
"/*"        		{ yERROR("Change this - comment unimplemented."); }
[ \t\n]				;
[-+]?[0-9]+		{ yylval = atoi(yytext); yTRACE(INT); return INT;} //accepts negative integers as well 
[-+]?[0-9]+\.[0-9]+  	{ yylval = atoi(yytext); yTRACE(FLOAT); return FLOAT;} //accepts negative floats as well //don't use atoi, instead use strtoi //can I write it as [0-9]*\.[0-9] //i.e. zero or more 
\.[0-9]+        	{ yTRACE(FLOAT); return FLOAT;} //Float can also start with just a decimal place
"+"			{ yTRACE(ADD); return ADD;}
"-"			{ yTRACE(SUB); return SUB;}
"*"			{ yTRACE(MULT); return MULT;}
"/"			{ yTRACE(DIV); return DIV;}
"^"			{ yTRACE(EXP); return EXP;}
"true"			{ yTRACE(True); return True;}
"false"			{ yTRACE(False); return False;}
"!"			{ yTRACE(EXCLAM); return EXCLAM;}
"&&"			{ yTRACE(AND); return AND;}
"||"			{ yTRACE(OR); return OR;}
"=="			{ yTRACE(EQUAL); return EQUAL;}
"!="			{ yTRACE(NOT_EQUAL); return NOT_EQUAL;}
"<"			{ yTRACE(LESS_THAN); return LESS_THAN;}
">"			{ yTRACE(MORE_THAN); return MORE_THAN;}
"<="			{ yTRACE(LESS_OR_EQUAL); return LESS_OR_EQUAL;}
">="			{ yTRACE(MORE_OR_EQUAL); return MORE_OR_EQUAL;}
";"			{ yTRACE(COLON);return COLON;}
"="			{ yTRACE(EQUAL); return EQUAL;}
"const"			{ yTRACE(CONST); return CONST;}
"."			{ yERROR("Character not recognized");}
"("			{ yTRACE(LBRACKET); return LBRACKET; return LBRACKET;}
")"			{ yTRACE(RBRACKET); return RBRACKET; return RBRACKET;}



//Error Checking
//Text strings that are too long
//Integer numbers that are too long
//Integer constants out of bounds (range is 0 to 32767)
//Float numbers with more than one decimal
//Variables that contain symbols that are not allowed
//Variable names that start with a number
//Text strings that span more than one line
//Text strings that are too long (max is 256 characters)
//Identifier names that are too long (max is 32 chars)
//Unmatched commenter delimeters




[0-9]+[a-zA-z]  {yERROR("Variable names cannot start with a digit");}


%%
