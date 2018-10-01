%{
/***********************************************************************
 *   Interface to the parser module for CSC467 course project.
 * 
 *   Phase 2: Implement context free grammar for source language, and
 *            parse tracing functionality.
 *   Phase 3: Construct the AST for the source language program.
 ***********************************************************************/

/***********************************************************************
 *  C Definitions and external declarations for this module.
 *
 *  Phase 3: Include ast.h if needed, and declarations for other global or
 *           external vars, functions etc. as needed.
 ***********************************************************************/

#include <string.h>
#include "common.h"
//#include "ast.h"
//#include "symbol.h"
//#include "semantic.h"
#define YYERROR_VERBOSE
#define yTRACE(x)    { if (traceParser) fprintf(traceFile, "%s\n", x); }

void yyerror(const char* s);    /* what to do in case of error            */
int yylex();              /* procedure for calling lexical analyzer */
extern int yyline;        /* variable holding current line number   */
extern int yyval;      /* text that is matched by scanner */

%}

/***********************************************************************
 *  Yacc/Bison declarations.
 *  Phase 2:
 *    1. Add precedence declarations for operators (after %start declaration)
 *    2. If necessary, add %type declarations for some nonterminals
 *  Phase 3:
 *    1. Add fields to the union below to facilitate the construction of the
 *       AST (the two existing fields allow the lexical analyzer to pass back
 *       semantic info, so they shouldn't be touched).
 *    2. Add <type> modifiers to appropriate %token declarations (using the
 *       fields of the union) so that semantic information can by passed back
 *       by the scanner.
 *    3. Make the %type declarations for the language non-terminals, utilizing
 *       the fields of the union as well.
 ***********************************************************************/

%{
#define YYDEBUG 1
%}

// TODO:Modify me to add more data types
// Can access me from flex useing yyval

%union {
  int ival;
  float fval;
  bool bval;
  char* name;
}

%token <name> IDENTIFIER

%token <ival> INT
%token <bval> BOOL
%token <fval> FLOAT

%token <name> IF
%token <name> ELSE
%token <name> WHILE 

%token <name> INT_ID
%token <name> FLOAT_ID
%token <name> BOOL_ID

%token <name> VEC2
%token <name> VEC3
%token <name> VEC4
%token <name> BVEC2
%token <name> BVEC3
%token <name> BVEC4
%token <name> IVEC2
%token <name> IVEC3
%token <name> IVEC4

%token <name> GL_FRAGCOLOR
%token <name> GL_FRAGDEPTH
%token <name> CONST
        
%token <name> ADD 
%token <name> SUB 
%token <name> MUL 
%token <name> DIV 
%token <name> EXP
%token <name> T 
%token <name> F
%token <name> AND 
%token <name> OR 
%token <name> EQ 
%token <name> NEQ 
%token <name> LESS 
%token <name> GREATER 
%token <name> LEQ 
%token <name> GEQ
%token <name> COLON
%token <name> EQUAL 

%token <name> LBRACKET
%token <name> RBRACKET

//precedence rules
%left OR
%left AND
%left EQ NEQ LESS LEQ GREATER GEQ //no precendence
%left ADD SUB
%left MULT DIV
%right EXP
//the !?
%left '['']''('')'

%start    program

%%

/***********************************************************************
 *  Yacc/Bison rules
 *  Phase 2:
 *    1. Replace grammar found here with something reflecting the source
 *       language grammar
 *    2. Implement the trace parser option of the compiler
 *  Phase 3:
 *    1. Add code to rules for construction of AST.
 ***********************************************************************/
program
  :   tokens       
  ;
tokens
  :  tokens token  
  |      
  ;
// TODO: replace myToken with the token the you defined.
token
  :      
  |                          
  ;
//keep in mind BEDMAS rule of operations
// exp
//   :  
//   | INT
//   | FLOAT
//   | '-' exp {} //this would be negative
//   | exp ADD exp {yTRACE($$ = $1 + $3);}
//   | exp SUB exp {yTRACE($$ = $1 - $3);}
//   | exp MULT exp {yTRACE($$ = $1 * $3);}
//   | exp DIV exp {yTRACE($$ = $1 / $3);}
//   | exp EXP exp {yTRACE($$ = $1 ^ %3);}
//   | TRUE 
//   | FALSE
//   | exp AND exp {yTRACE($$ = $1 && $3);}
//   | exp OR exp {yTRACE($$ = $1 || $3);}
//   | exp EQ exp {yTRACE($$ = $ == $3);}
//   | exp NEQ exp {yTRACE($$ = $1 != $3);}
//   | exp LESS exp  {yTRACE($$ = $1 < $3);}
//   | exp LEQ exp {yTRACE($$ = $1 <= $3);}
//   | exp GREATER exp {yTRACE($$ = $1 > $3);}
//   | exp GEQ exp {yTRACE($$ = $1 >= $3);}
//   | '(' exp ')'
//   | VARIABLE
//   | CONSTRUCTOR
//   | FUNCTION
//   ;
// declaration
//   : type ID COLON {yTRACE($$);}
//   | type ID EQ exp COLON {yTRACE($$ = =$1)}
//   | CONST type ID EQ exp COLON {yTRACE()}

// statement:
//     COLON
//   | VARIABLE EQ exp COLON {yTRACE($$ = $1 = $3;);}
//   | IF '(' exp ')' statement ELSE statement {yTRACE($$ = 
//   | WHILE '(' exp ')' statement
//   | scope 

%%


/***********************************************************************ol
 * Extra C code.
 *
 * The given yyerror function should not be touched. You may add helper
 * functions as necessary in subsequent phases.
 ***********************************************************************/
void yyerror(const char* s) {
  if (errorOccurred)
    return;    /* Error has already been reported by scanner */
  else
    errorOccurred = 1;
        
  fprintf(errorFile, "\nPARSER ERROR, LINE %d",yyline);
  if (strcmp(s, "parse error")) {
    if (strncmp(s, "parse error, ", 13))
      fprintf(errorFile, ": %s\n", s);
    else
      fprintf(errorFile, ": %s\n", s+13);
  } else
    fprintf(errorFile, ": Reading token %s\n", yytname[YYTRANSLATE(yychar)]);
}

