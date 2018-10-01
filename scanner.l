%{
/**********************************************************************
 *  Interface to the scanner module for CSC467 course project
 **********************************************************************/


#include "common.h"
#include "parser.tab.h"
#include <string.h>

#define YY_USER_INIT { yyin = inputFile; }
#define	yyinput      input
#define yTRACE(x)    { if (traceScanner) fprintf(traceFile, "TOKEN %3d : %s\n", x, yytext); }
#define yERROR(x)    { fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: %s\n", yyline, x); errorOccurred = TRUE; yyterminate(); }

int CheckInt(void);
int yyline = 1;

%}
%option noyywrap

WS          	[ \t\n]

%%

"/*"       		{ 
					while(strncmp(yytext, "*/", yyleng) != 0) {
						yyinput();
					}
				}

[ \t\n]			;

([0-9]+)		{ 	
					int tmp = atoi(yytext);
					if (tmp > 32767 || tmp < -32767) {
						yERROR("value does not fall in range -32767 to 32767");
					} else {
						yylval.ival = tmp;
						yTRACE(INT); return INT;
					}
				}

([0-9]+\.[0-9]+)|([0-9]*\.[0-9]+)  { 
										float tmp = atof(yytext);
										if (tmp > 32767 || tmp < -32767) {
											yERROR("value does not fall in range -32767 to 32767");
									 	} else {
									 		yylval.fval = tmp;
											yTRACE(FLOAT); return FLOAT; //ask what to do for error checking with floats
										}
									}

[a-zA-Z_][a-zA-Z0-9_]+ 		{ 	if (yyleng > 32) {
									yERROR("identifier exceeds maximum length");
								} else {
									*yylval.name = atoi(yytext);
									yTRACE(IDENTIFIER); return IDENTIFIER;
								}
							}

[0-9]+[a-zA-z_]+ { yERROR("identifier cannot start with a number");}	

gl_FragColor/{WS}  {yTRACE(GL_FRAGCOLOR); return GL_FRAGCOLOR;}	
gl_FragDepth/{WS}  {yTRACE(GL_FRAGDEPTH); return GL_FRAGDEPTH;}

if/{WS}			{ yTRACE(IF); return IF; }
else/{WS}			{ yTRACE(ELSE); return ELSE; }
while/{WS} 			{ yTRACE(WHILE); return WHILE; }

int/{WS}			{ yTRACE(INT_ID); return INT_ID;}
float/{WS}			{ yTRACE(FLOAT_ID); return FLOAT_ID;}
bool/{WS}			{ yTRACE(BOOL_ID); return BOOL_ID;}

vec2/{WS}			{ yTRACE(VEC2); return VEC2; }
vec3/{WS}			{ yTRACE(VEC3); return VEC3; }
vec4/{WS}			{ yTRACE(VEC4); return VEC4; }
ivec2/{WS} 			{ yTRACE(IVEC2); return IVEC2; }
ivec3/{WS}			{ yTRACE(IVEC3); return IVEC3; }
ivec4/{WS}			{ yTRACE(IVEC4); return IVEC4; }
bvec2/{WS}			{ yTRACE(BVEC2); return BVEC2; }
bvec3/{WS}			{ yTRACE(BVEC3); return BVEC3; }
bvec4/{WS}			{ yTRACE(BVEC4); return BVEC4; }

"+"				{ yTRACE(ADD); return ADD;}
"-"				{ yTRACE(SUB); return SUB;}
"*"				{ yTRACE(MUL); return MUL;}
"/"				{ yTRACE(DIV); return DIV;}
"^"				{ yTRACE(EXP); return EXP;}
true/" "			{ yTRACE(T); return T;}
false/" "			{ yTRACE(F); return F;}
bool/" "			{ yTRACE(BOOL); return BOOL;}
"&&"			{ yTRACE(AND); return AND;}
"||"			{ yTRACE(OR); return OR;}
"=="			{ yTRACE(EQ); return EQ;}
"!="			{ yTRACE(NEQ); return NEQ;}
"<"				{ yTRACE(LESS); return LESS;}
">"				{ yTRACE(GREATER); return GREATER;}
"<="			{ yTRACE(LEQ); return LEQ;}
">="			{ yTRACE(GEQ); return GEQ;}
";"				{ yTRACE(COLON);return COLON;}
"="				{ yTRACE(EQUAL); return EQUAL;}

const/" "		{ yTRACE(CONST); return CONST;}
"."				{ yERROR("Character not recognized");}
"("				{ yTRACE(LBRACKET); return LBRACKET; return LBRACKET;}
")"				{ yTRACE(RBRACKET); return RBRACKET; return RBRACKET;}

. 				{ yERROR("invalid token"); yyterminate(); }

%%