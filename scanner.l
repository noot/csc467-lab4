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

%}
%option noyywrap

WS          	[ \t]

%%
"/*"       		{ while(yyinput() != "*/") {
					yyinput();
				}}

[ \t\n]			;

[0-9]+			{ 	if (yylval > 32767) {
						yERROR("exceeds maximum size of 32767")
					} else {
						yTRACE(INT); return yylval 
					}
				}

([0-9]+\.[0-9]+)|([0-9]*\.[0-9]+)  { 
										yTRACE(FLOAT); return FLOAT 
									}

[a-zA-Z_][a-zA-Z0-9_]+ 		{ 	if (yyleng > 32) {
									yERROR("identifier exceeds maximum length")
								} else {
									yylval = strtoi(yytext)
									yTRACE(IDENTIFIER); return IDENTIFIER 
								}
							}

"if"			{ yTRACE(IF); return IF }
"else"			{ yTRACE(ELSE); return ELSE }
"while"			{ yTRACE(WHILE); return WHILE }

"vec2" 			{ yTRACE(VEC2); return VEC2}
"vec3"			{ yTRACE(VEC3); return VEC3}
"vec4"			{ yTRACE(VEC4); return VEC4}
"ivec2" 		{ yTRACE(IVEC2); return IVEC2}
"ivec3"			{ yTRACE(IVEC3); return IVEC3}
"ivec4"			{ yTRACE(IVEC4); return IVEC4}
"bvec2"			{ yTRACE(BVEC2); return BVEC2}
"bvec3"			{ yTRACE(BVEC3); return BVEC3}
"bvec4"			{ yTRACE(BVEC4); return BVEC4}

. 				( yERROR("invalid token") )
%%
yylex();