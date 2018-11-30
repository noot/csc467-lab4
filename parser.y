%{
/***********************************************************************
 *   Interface to the parser module for CSC467 course project.
 * 
 *   Phase 2: Implement context free grammar for source language, and
 *            parse tracing functionality.
 *   Phase 3: Construct the AST for the source landguage program.
 ***********************************************************************/

/***********************************************************************
 *  C Definitions and external declarations for this module.
 *
 *  Phase 3: Include ast.h if needed, and declarations for other global or
 *           external vars, functions etc. as needed.
 ***********************************************************************/

#include <string.h>
#include "common.h"
#include "ast.h"
//#include "symbol.h"
#include "semantic.h"
//#include "codegen.h"

#define YYERROR_VERBOSE
#define yTRACE(x)    { if (traceParser) fprintf(traceFile, "%s\n", x); }

void yyerror(const char* s);    /* what to do in case of error            */
int yylex();              /* procedure for calling lexical analyzer */
extern int yyline;        /* variable holding current line number   */
extern int yyval;      /* text that is matched by scanner */

// typedef enum {
//   DP3 = 0,
//   LIT = 1, 
//   RSQ = 2
// } function_name;
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
// Can access me from flex using yyval

%union {
  int ival;
  float fval;
  bool bval;
  char* name;
  node* ast_t;
  int func;
}

%token <name> IDENTIFIER

%token <ival> INT
%token <fval> FLOAT

%token <bval> T 
%token <bval> F

%token <name> IF
%token <name> ELSE
%token <name> WHILE 

%token <name> INT_T
%token <name> FLOAT_T
%token <name> BOOL_T
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
%token <name> GL_FRAGCOORD
%token <name> GL_TEXCOORD
%token <name> GL_COLOR
%token <name> GL_SECONDARY
%token <name> GL_FOGFRAGCOORD
%token <name> GL_LIGHT_HALF
%token <name> GL_LIGHT_AMBIENT
%token <name> GL_MATERIAL_SHININESS
%token <name> ENV1
%token <name> ENV2
%token <name> ENV3
%token <name> CONST

%token <func> FUNC
// %token DP3
// %token LIT
// %token RSQ

%token <name> INC
%token <name> DEC
        
%token <name> COMMA
%token <name> ADD 
%token <name> SUB 
%token <name> MUL 
%token <name> DIV 
%token <name> EXP
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
%token <name> EXCLAM

%token <name> LBRACKET
%token <name> RBRACKET
%token <name> LCURL
%token <name> RCURL
%token <name> LSQUARE
%token <name> RSQUARE

%type <ast_t> program
%type <ast_t> scope
%type <ast_t> declarations
%type <ast_t> declaration
%type <ast_t> statements
%type <ast_t> statement
%type <ast_t> type
%type <ast_t> exp
%type <ast_t> else_statement
%type <ast_t> variable
%type <ast_t> unary_op
%type <ast_t> binary_op
%type <ast_t> constructor
%type <ast_t> function
%type <ast_t> arguments

%type <int> function_name
// %type <int> DP3
// %type <int> LIT
// %type <int> RSQ

//precedence rules
%left OR
%left AND
%left EQ NEQ LESS LEQ GREATER GEQ //no precendence
%left ADD SUB
%left MUL DIV
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
  :   scope                               { yTRACE("program -> scope");
                                            ast = $1;
                                          }     
  ;
scope
  :   LCURL declarations statements RCURL { yTRACE("scope -> { declarations statements }");
                                            $$ = ast_allocate(SCOPE_NODE, $2, $3); }
  ;
declarations
  :   declarations declaration            { yTRACE("declarations -> declarations declaration"); 
                                            $$ = ast_allocate(DECLARATIONS_NODE, $2);}
  |   /* empty */                         { yTRACE("declarations -> empty"); $$ = NULL; }                                              
  ; 
statements
  :   statements statement                { yTRACE("statements -> statements statement"); 
                                            $$ = ast_allocate(STATEMENTS_NODE, $1, $2);}
  |   /* empty */                         { yTRACE("statements -> empty"); $$ = NULL; }
  ;
declaration
  :   type IDENTIFIER COLON                  { yTRACE("declaration -> type identifier ;"); 
                                                $$ = ast_allocate(DECLARATION_NODE, 0, $2, $1, NULL);}
  |   type IDENTIFIER EQUAL exp COLON        { yTRACE("declaration -> type idenfifier = exp ;");
                                                $$ = ast_allocate(DECLARATION_NODE, 0, $2, $1, $4); }
  |   CONST type IDENTIFIER EQUAL exp COLON  { yTRACE("declaration -> const type identifier = exp ;");
                                                $$ = ast_allocate(DECLARATION_NODE, 1, $3, $2, $5); }
  |   /* empty */                            { yTRACE("declaration -> empty"); $$ = NULL; }
  ;
statement
  :   variable EQUAL exp COLON                            { yTRACE("statement -> variable = exp ;");
                                                            $$ = ast_allocate(ASSIGNMENT_NODE, $1, $3); } 
  |   IF LBRACKET exp RBRACKET statement else_statement   { yTRACE("statement -> if ( exp ) statement else_statement");
                                                            $$ = ast_allocate(STATEMENT_NODE, 1, NULL, $3, $5, $6); }
  |   COLON                                               { yTRACE("statement -> ;"); $$ = NULL; }
  |   scope                                               { yTRACE("statement -> scope");
                                                            $$ = ast_allocate(SCOPE_NODE, NULL, $1); }                      
  ;
else_statement
  :   ELSE statement                      { yTRACE("else_statement -> else statement");
                                            $$ = ast_allocate(ELSE_STATEMENT_NODE, $2); }
  |   /* empty */                         { yTRACE("else_statement -> empty"); $$ = NULL; }
  ;
type
  :   INT_T				  { yTRACE("type -> int"); 
                      $$ = ast_allocate(TYPE_NODE, 0, INT_T, 0, $1, 0); }
  |   IVEC2				  { yTRACE("type -> ivec2");
                      $$ = ast_allocate(TYPE_NODE, 0, IVEC2, 1, $1, 0); }
  |   IVEC3				  { yTRACE("type -> ivec3");
                      $$ = ast_allocate(TYPE_NODE, 0, IVEC3, 1, $1, 0); }
  |   IVEC4				  { yTRACE("type -> ivec4"); 
                      $$ = ast_allocate(TYPE_NODE, 0, IVEC4, 1, $1, 0); }
  |   BOOL_T				{ yTRACE("type -> bool"); 
                      $$ = ast_allocate(TYPE_NODE, 0, BOOL_T, 0, 0, $1); }
  |   BVEC2				  { yTRACE("type -> bvec2"); 
                      $$ = ast_allocate(TYPE_NODE, 0, BVEC2, 1, 0, $1); }
  |   BVEC3				  { yTRACE("type -> bvec3"); 
                      $$ = ast_allocate(TYPE_NODE, 0, BVEC3, 1, 0, $1); }
  |   BVEC4				  { yTRACE("type -> bvec4"); 
                      $$ = ast_allocate(TYPE_NODE, 0, BVEC4, 1, 0, $1); }
  |   FLOAT_T		    { yTRACE("type -> float");
                      $$ = ast_allocate(TYPE_NODE, 0, FLOAT_T, 0, $1, 0); }
  |   VEC2				  { yTRACE("type -> vec2");
                      $$ = ast_allocate(TYPE_NODE, 0, VEC2, 1, $1, 0); }
  |   VEC3				  { yTRACE("type -> vec3"); 
                      $$ = ast_allocate(TYPE_NODE, 0, VEC3, 1, $1, 0); }
  |   VEC4				  { yTRACE("type -> vec4");
                      $$ = ast_allocate(TYPE_NODE, 0, VEC4, 1, $1, 0); }
  ;				  
exp
  :  
  | constructor			{ yTRACE("exp -> constructor"); $$ = NULL; }
  | function				{ yTRACE("exp -> function"); $$ = NULL; }
  | INT				  	  { yTRACE("exp -> integer_literal"); 
                      $$ = ast_allocate(INT_NODE, $1); }
  | FLOAT				    { yTRACE("exp -> float_literal"); 
                      $$ = ast_allocate(FLOAT_NODE, $1); }
  | T 				  	  { yTRACE("exp -> true"); 
                      $$ = ast_allocate(BOOL_NODE, 1);}
  | F				  	    { yTRACE("exp -> false"); 
                      $$ = ast_allocate(BOOL_NODE, 0); }
  | variable				          { yTRACE("exp -> variable"); 
                                $$ = ast_allocate(EXP_NODE, $1); }
  | EXCLAM exp			        { yTRACE("exp -> unary_op ex");
                                $$ = ast_allocate(UNARY_OP_NODE, $1, $2); }
  | SUB exp                 { yTRACE("exp -> unary_op ex");
                                $$ = ast_allocate(UNARY_OP_NODE, $1, $2); }
  | LBRACKET exp RBRACKET		  { yTRACE("exp -> ( exp )");
                                $$ = ast_allocate(EXP_NODE, $2); }
  | exp ADD exp             { yTRACE("binary_op -> exp + exp");
                              $$ = ast_allocate(BINARY_OP_NODE, ADD, $1, $3); }
  | exp SUB exp             { yTRACE("binary_op -> exp - exp");
                              $$ = ast_allocate(BINARY_OP_NODE, SUB, $1, $3); }
  | exp MUL exp             { yTRACE("binary_op -> exp * exp");
                              $$ = ast_allocate(BINARY_OP_NODE, MUL, $1, $3); }
  | exp DIV exp             { yTRACE("binary_op -> exp / exp");
                              $$ = ast_allocate(BINARY_OP_NODE, DIV, $1, $3); }
  | exp EXP exp             { yTRACE("binary_op -> exp ^ exp");
                              $$ = ast_allocate(BINARY_OP_NODE, EXP, $1, $3); } //not in handout
  | exp AND exp             { yTRACE("binary_op -> exp &7xp");
                              $$ = ast_allocate(BINARY_OP_NODE, AND, $1, $3); }
  | exp OR exp              { yTRACE("binary_op -> exp || exp");
                              $$ = ast_allocate(BINARY_OP_NODE, OR, $1, $3); }
  | exp EQ exp              { yTRACE("binary_op -> exp == exp");
                              $$ = ast_allocate(BINARY_OP_NODE, EQ, $1, $3); }
  | exp NEQ exp             { yTRACE("binary_op -> exp != exp");
                              $$ = ast_allocate(BINARY_OP_NODE, NEQ, $1, $3); }
  | exp LESS exp            { yTRACE("binary_op -> exp < exp");
                              $$ = ast_allocate(BINARY_OP_NODE, LESS, $1, $3); }
  | exp LEQ exp             { yTRACE("binary_op -> exp <= exp");
                              $$ = ast_allocate(BINARY_OP_NODE, LEQ, $1, $3); }
  | exp GREATER exp         { yTRACE("binary_op -> exp > exp");
                              $$ = ast_allocate(BINARY_OP_NODE, GREATER, $1, $3); }
  | exp GEQ exp             { yTRACE("binary_op -> exp >= exp");
                              $$ = ast_allocate(BINARY_OP_NODE, GEQ, $1, $3); }
  ;
variable
  : IDENTIFIER		  		                    { yTRACE("variable -> identifier");
                                              $$ = ast_allocate(VAR_NODE, $1, 0, 0); }
  | IDENTIFIER LSQUARE INT RSQUARE	  	    { yTRACE("variable -> identifier[integer_literal]"); 
                                              $$ = ast_allocate(VAR_NODE, $1, 0, $3); }
  | GL_LIGHT_AMBIENT                        { yTRACE("variable -> GL_LIGHT_AMBIENT");
                                              $$ = ast_allocate(VAR_NODE, $1, 0, 0); }

  | GL_FRAGCOLOR                            { yTRACE("variable -> GL_FRAGCOLOR");
                                              $$ = ast_allocate(VAR_NODE, $1, 0, 0); }
  ;
constructor 
  : type LBRACKET arguments RBRACKET    		  { yTRACE("constructor -> type ( arguments )");
                                                $$ = ast_allocate(CONSTRUCTOR_NODE, $1, $3); }	
  ;
function
  : FUNC LBRACKET arguments RBRACKET       { yTRACE("function -> function_name ( arguments_opt )");
                                                  $$ = ast_allocate(FUNCTION_NODE, $1, $3); }
  ;
// arguments_opt
//   : arguments				{ yTRACE("arguments_opt -> arguments");
//                       $$ = ast_allocate(ARGUMENTS_OPT_NODE, $1); }
//   | /*empty*/				{ yTRACE("arguments_opt -> empty"); $$ = NULL; }
//   ;
arguments
  : arguments COMMA exp			  { yTRACE("arguments -> arguments , exp");
                                $$ = ast_allocate(ARGUMENTS_NODE, $1, $3); }
  | exp					              { yTRACE("arguments -> exp");
                                $$ = ast_allocate(ARGUMENTS_NODE, NULL, $1); }
  ;
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

