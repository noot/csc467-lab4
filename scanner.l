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

([0-9]+)		{ 	
					int tmp = atoi(yytext);
					if (tmp > 32767 || tmp < -32767) {
						yERROR("value does not fall in range -32767 to 32767");
					} else {
						yTRACE(INT); return INT;
					}
				}

([0-9]+\.[0-9]+)|([0-9]*\.[0-9]+)  { 
										float tmp = strtof(yytext, NULL);
										if (tmp > 32767 || tmp < -32767) {
											yERROR("value does not fall in range -32767 to 32767");
									 	} else {
											yTRACE(FLOAT); return FLOAT; //ask what to do for error checking with floats
										}
									}

[a-zA-Z_][a-zA-Z0-9_]+ 		{ 	if (yyleng > 32) {
									yERROR("identifier exceeds maximum length");
								} else {
									yylval.name = strtoi(yytext);
									yTRACE(IDENTIFIER); return IDENTIFIER;
								}
							}

[0-9]+[a-zA-z_]+ { yERROR("identifier cannot start with a number");}	

gl_FragColor/" "  {yTRACE(GL_FRAGCOLOR); return GL_FRAGCOLOR;}	
gl_FragDepth/" "  {yTRACE(GL_FRAGDEPTH); return GL_FRAGDEPTH;}

if/" "				{ yTRACE(IF); return IF; }
else/" "			{ yTRACE(ELSE); return ELSE; }
while/" " 			{ yTRACE(WHILE); return WHILE; }

vec2/" " 			{ yTRACE(VEC2); return VEC2; }
vec3/" " 			{ yTRACE(VEC3); return VEC3; }
vec4/" " 			{ yTRACE(VEC4); return VEC4; }
ivec2/" " 			{ yTRACE(IVEC2); return IVEC2; }
ivec3/" " 			{ yTRACE(IVEC3); return IVEC3; }
ivec4/" " 			{ yTRACE(IVEC4); return IVEC4; }
bvec2/" " 			{ yTRACE(BVEC2); return BVEC2; }
bvec3/" " 			{ yTRACE(BVEC3); return BVEC3; }
bvec4/" " 			{ yTRACE(BVEC4); return BVEC4; }

"+"				{ yTRACE(ADD); return ADD;}
"-"				{ yTRACE(SUB); return SUB;}
"*"				{ yTRACE(MUL); return MUL;}
"/"				{ yTRACE(DIV); return DIV;}
"^"				{ yTRACE(EXP); return EXP;}
true/" "			{ yTRACE(T); return T;}
false/" "			{ yTRACE(F); return F;}

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
yylex();
