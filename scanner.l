%{
/**********************************************************************
 *  Interface to the scanner module for CSC467 course project
 **********************************************************************/


#include "common.h"
#include "ast.h"
//#include "codegen.h"
//#include "symbol.h"
#include "parser.tab.h"
//#include "semantic.h"
#include <string.h>
#include <stdlib.h>
#include <cmath>

#define YY_USER_INIT { yyin = inputFile; }
#define	yyinput      input
#define yTRACE(x)    { if (traceScanner) fprintf(traceFile, "TOKEN %3d : %s\n", x, yytext); }
#define yERROR(x)    { fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: %s\n", yyline, x); errorOccurred = TRUE; yyterminate(); }

int CheckInt(void);
int yyline = 1;

%}
%option noyywrap

WS          	[ \t\n]
DELIM		[ \t\n\(\)\[\]\{\},;\+\-|*\/\^=!\<\?]|&&|"||"|==|!=|"<"=|">"=
%%

[\n] 			{
					yyline++;
				}

"/*"       		{ 
					int tmp1 = 0;
					int tmp2 = yyinput(); // yyinput gives ascii value of next character
					while(tmp2 != 0) { // while not EOF
						if(tmp1 == 42 && tmp2 == 47) { /* ascii values of "*" and "/" respectively */
							break;
						} else if (tmp1 == '\n' && tmp2 != 0) { /* ascii values of "\" and "n" respectively */
							yyline++; /* if "\n" found, increment line number */
						} 
						tmp1 = tmp2;
						tmp2 = yyinput();
					}
					if(tmp2 == 0) {
						yERROR("unclosed comment") // reach EOF without finding `*/`
					}
				}

[ \t\n]			;

if				{ yTRACE(IF); return IF; }
else			{ yTRACE(ELSE); return ELSE; }
while 			{ yTRACE(WHILE); return WHILE; }

([0-9]*)		{ 	
					char *tmp;
					long ret = strtol(yytext, &tmp, 10);
					if (ret > 32767 || ret < -32767) {
						yERROR("integer value does not fall in range -32767 to 32767");
					} else {
						yylval.ival = ret;
						yTRACE(INT); return INT;
					}
				}

([0-9]+\.[0-9]+){2,} 								 { yERROR("not a valid float value");}
([0-9]+\.[0-9]+)|([0-9]*\.[0-9]+)|([0-9]+"E"[0-9]+)  { 
														float threshold = pow(10,37);
														char* tmp = yytext;
														float ret = strtof(tmp,NULL);
														if ((ret < (threshold)) && (ret > -(threshold))) {
													 		yylval.fval = ret;
															yTRACE(FLOAT); return FLOAT; 
													 	} else {
															yERROR("float value does not fall in range -E37 to E37");
														}
													}

"gl_FragColor"/{WS} 		{yTRACE(GL_FRAGCOLOR); return GL_FRAGCOLOR;}	
"gl_FragDepth"/{WS}  		{yTRACE(GL_FRAGDEPTH); return GL_FRAGDEPTH;}
"gl_FragCoord"/{WS}  		{yTRACE(GL_FRAGCOORD); return GL_FRAGCOORD;}
"gl_TexCoord"/{WS}  		{yTRACE(GL_TEXCOORD); return GL_TEXCOORD;}
"gl_Color"/{WS}  			{yTRACE(GL_COLOR); return GL_COLOR;}
"gl_Secondary"/{WS}  		{yTRACE(GL_SECONDARY); return GL_SECONDARY;}
"gl_FogFragCoord"/{WS}  	{yTRACE(GL_FOGFRAGCOORD); return GL_FOGFRAGCOORD;}
"gl_Light_Half"/{WS}  		{yTRACE(GL_LIGHT_HALF); return GL_LIGHT_HALF;}
"gl_Light_Ambient"/{WS}  	{yTRACE(GL_LIGHT_AMBIENT); return GL_LIGHT_AMBIENT;}
"gl_Material_Shininess"/{WS} {yTRACE(GL_MATERIAL_SHININESS); return GL_MATERIAL_SHININESS;}
"env1"/{WS} 				{yTRACE(ENV1); return ENV1;}
"env2"/{WS}  				{yTRACE(ENV2); return ENV2;}
"env3"/{WS}  				{yTRACE(ENV3); return ENV3;}

dp3/{DELIM}				{ yylval.func = 0; yTRACE(FUNC); return FUNC; }
lit/{DELIM}				{ yylval.func = 1; yTRACE(FUNC); return FUNC; }
rsq/{DELIM}				{ yylval.func = 2; yTRACE(FUNC); return FUNC; }

"int"/{WS}			{ yTRACE(INT_T); return INT_T;}
"float"/{WS}			{ yTRACE(FLOAT_T); return FLOAT_T;}
"bool"/{WS}			{ yTRACE(BOOL_T); return BOOL_T;}

vec2			{ yTRACE(VEC2); return VEC2; }
vec3			{ yTRACE(VEC3); return VEC3; }
vec4			{ yTRACE(VEC4); return VEC4; }
ivec2 			{ yTRACE(IVEC2); return IVEC2; }
ivec3			{ yTRACE(IVEC3); return IVEC3; }
ivec4			{ yTRACE(IVEC4); return IVEC4; }
bvec2			{ yTRACE(BVEC2); return BVEC2; }
bvec3			{ yTRACE(BVEC3); return BVEC3; }
bvec4			{ yTRACE(BVEC4); return BVEC4; }

"++" 				{ yTRACE(INC); return INC; }
"--"				{ yTRACE(DEC); return DEC; }
","					{ yTRACE(COMMA); return COMMA;}
"+"					{ yTRACE(ADD); return ADD;}
"-"					{ yTRACE(SUB); return SUB;}
"*"					{ yTRACE(MUL); return MUL;}
"/"					{ yTRACE(DIV); return DIV;}
"^"					{ yTRACE(EXP); return EXP;}
'true'				{ yTRACE(T); return T;}
'false'				{ yTRACE(F); return F;}
"&&"				{ yTRACE(AND); return AND;}
"||"				{ yTRACE(OR); return OR;}
"=="				{ yTRACE(EQ); return EQ;}
"!="				{ yTRACE(NEQ); return NEQ;}
"<"					{ yTRACE(LESS); return LESS;}
">"					{ yTRACE(GREATER); return GREATER;}
"<="				{ yTRACE(LEQ); return LEQ;}
">="				{ yTRACE(GEQ); return GEQ;}
";"					{ yTRACE(COLON);return COLON;}
"="					{ yTRACE(EQUAL); return EQUAL;}
"!"					{ yTRACE(EXCLAM); return EXCLAM;}

const/{WS}			{ yTRACE(CONST); return CONST;}
"("					{ yTRACE(LBRACKET); return LBRACKET;}
")"					{ yTRACE(RBRACKET); return RBRACKET;}
"{"					{ yTRACE(LCURL); return LCURL; }
"}"					{ yTRACE(RCURL); return RCURL; }
"["					{ yTRACE(LSQUARE); return LSQUARE; }
"]"					{ yTRACE(RSQUARE); return RSQUARE; }

[a-zA-Z_][a-zA-Z0-9_]* 		{ 	if (yyleng > 32) {
									yERROR("identifier exceeds maximum length");
								} else {
									yylval.name = strdup(yytext);
									yTRACE(IDENTIFIER); return IDENTIFIER;
								}
							}

.         			{ yERROR("invalid token");} 

%%
