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

WS          	[ \t]

%%

"/*"       		{ while(yyinput() != "*/") {
					yyinput();
				}}

[ \t\n]			;

[-+]?[0-9]+			{ 	yylval = strtoi(yytext);
						if (yylval > 32767 || yylval < -32767) {
						yERROR("value does not fall in range -32767 to 32767");
					} else {

						yTRACE(INT); return INT;
					}
				}

([0-9]+\.[0-9]+)|([0-9]*\.[0-9]+)  { 
										yylval = strtof(yytext);
										if (yylval > 32767 || yylval < -32767) {
											yERROR("value does not fall in range -32767 to 32767");
									 	} else {
										
											yTRACE(FLOAT); return FLOAT; //ask what to do for error checking with floats
										
										}
									}

[a-zA-Z_][a-zA-Z0-9_]+ 		{ 	if (yyleng > 32) {
									yERROR("identifier exceeds maximum length");
								} else {
									yylval = strtoi(yytext);
									yTRACE(IDENTIFIER); return IDENTIFIER;
								}
							}

[0-9]+[a-zA-z_]+ { yERROR("identifier cannot start with a number");}	
"gl_FragColor"  {yTRACE(GL_FRAGCOLOR); return GL_FRAGCOLOR;}	
"gl_FragDepth"  {yTRACE(GL_FRAGDEPTH); return GL_FRAGDEPTH;}

"if"			{ yTRACE(IF); return IF;}
"else"			{ yTRACE(ELSE); return ELSE;}
"while"			{ yTRACE(WHILE); return WHILE;}

"vec2" 			{ yTRACE(VEC2); return VEC2;}
"vec3"			{ yTRACE(VEC3); return VEC3;}
"vec4"			{ yTRACE(VEC4); return VEC4;}
"ivec2" 		{ yTRACE(IVEC2); return IVEC2;}
"ivec3"			{ yTRACE(IVEC3); return IVEC3;}
"ivec4"			{ yTRACE(IVEC4); return IVEC4;}
"bvec2"			{ yTRACE(BVEC2); return BVEC2;}
"bvec3"			{ yTRACE(BVEC3); return BVEC3;}
"bvec4"			{ yTRACE(BVEC4); return BVEC4;}

"+"			{ yTRACE(ADD); return ADD;}
"-"			{ yTRACE(SUB); return SUB;}
"*"			{ yTRACE(MULT); return MULT;}
"/"			{ yTRACE(DIV); return DIV;}
"^"			{ yTRACE(EXP); return EXP;}

"true"			{ yTRACE(True); return True;}
"false"			{ yTRACE(False); return False;}
"!"				{ yTRACE(EXCLAM); return EXCLAM;}
"&&"			{ yTRACE(AND); return AND;}
"||"			{ yTRACE(OR); return OR;}
"=="			{ yTRACE(EQUAL); return EQUAL;}
"!="			{ yTRACE(NOT_EQUAL); return NOT_EQUAL;}
"<"				{ yTRACE(LESS_THAN); return LESS_THAN;}
">"				{ yTRACE(MORE_THAN); return MORE_THAN;}
"<="			{ yTRACE(LESS_OR_EQUAL); return LESS_OR_EQUAL;}
">="			{ yTRACE(MORE_OR_EQUAL); return MORE_OR_EQUAL;}
";"				{ yTRACE(COLON);return COLON;}
"="				{ yTRACE(EQUAL); return EQUAL;}
"const"			{ yTRACE(CONST); return CONST;}

"."			{ yERROR("Character not recognized");}
"("			{ yTRACE(LBRACKET); return LBRACKET; return LBRACKET;}
")"			{ yTRACE(RBRACKET); return RBRACKET; return RBRACKET;}
. 			{ yERROR("invalid token"); }

%%
yylex();
