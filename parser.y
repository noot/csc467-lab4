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
#include "symbol.h"
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
// Can access me from flex using yyval

%union {
  int ival;
  float fval;
  bool bval;
  char* name;
  node *ast_t;
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

%token <name> DP3
%token <name> LIT
%token <name> RSQ

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
%type <ast_t> arguments_opt
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
                                            //ast = (node *)malloc(sizeof(node));
                                            //$$ = ast_allocate($1); 
                                            ast = $1;
                                          }     
  ;
scope
  :   LCURL declarations statements RCURL { yTRACE("scope -> { declarations statements }");
                                            $$ = ast_allocate(SCOPE_NODE, $2, $3); }
  ;
declarations
  :   declarations declaration            { yTRACE("declarations -> declarations declaration"); 
                                            $$ = ast_allocate(DECLARATIONS_NODE, $1, $2);}
  |   /* empty */                         { yTRACE("declarations -> empty"); }                                              
  ; 
statements
  :   statements statement                { yTRACE("statements -> statements statement"); 
                                            $$ = ast_allocate(STATEMENTS_NODE, $1, $2);}
  |   /* empty */                         { yTRACE("statements -> empty"); }
  ;
declaration
  :   type IDENTIFIER COLON               { yTRACE("declaration -> type identifier ;"); 
                                            $$ = ast_allocate(DECLARATION_NODE, $1);}
  |   type IDENTIFIER EQUAL exp COLON        { yTRACE("declaration -> type idenfifier = exp ;");
                                                $$ = ast_allocate(DECLARATION_NODE, $1, $4); }
  |   CONST type IDENTIFIER EQUAL exp COLON  { yTRACE("declaration -> const type identifier = exp ;");
                                                $$ = ast_allocate(DECLARATION_NODE, $2, $5); }
  |   /* empty */                         { yTRACE("declaration -> empty"); }
  ;
statement
  :   variable EQUAL exp COLON               { yTRACE("statement -> variable = exp ;");
                                                $$ = ast_allocate(STATEMENT_NODE, $1, $3); } 
  |   IF LBRACKET exp RBRACKET statement else_statement   { yTRACE("statement -> if ( exp ) statement else_statement");
                                                            $$ = ast_allocate(STATEMENT_NODE, $3. $5, $6); }
  |   WHILE LBRACKET exp RBRACKET statement               { yTRACE("statement -> while ( exp ) statement"); 
                                                            $$ = ast_allocate(STATEMENT_NODE, $3, $5);}
  |   COLON                                { yTRACE("statement -> ;"); }
  |   scope
  ;
else_statement
  :   ELSE statement                      { yTRACE("else_statement -> else statement");
                                            $$ = ast_allocate(ELSE_STATEMENT_NODE, $1); }
  |   /* empty */                         { yTRACE("else_statement -> empty"); }
  ;
type
  :   INT_T				  { yTRACE("type -> int"); 
                      $$ = ast_allocate(TYPE_NODE, INT_T, $1); }
  |   IVEC2				  { yTRACE("type -> ivec2");
                      $$ = ast_allocate(TYPE_NODE, IVEC2, $1); }
  |   IVEC3				  { yTRACE("type -> ivec3");
                      $$ = ast_allocate(TYPE_NODE, IVEC3, $1); }
  |   IVEC4				  { yTRACE("type -> ivec4"); 
                      $$ = ast_allocate(TYPE_NODE, IVEC4, $1); }
  |   BOOL_T				{ yTRACE("type -> bool"); 
                      $$ = ast_allocate(TYPE_NODE, BOOL_T, $1); }
  |   BVEC2				  { yTRACE("type -> bvec2"); 
                      $$ = ast_allocate(TYPE_NODE, BVEC2, $1); }
  |   BVEC3				  { yTRACE("type -> bvec3"); 
                      $$ = ast_allocate(TYPE_NODE, BVEC3, $1); }
  |   BVEC4				  { yTRACE("type -> bvec4"); 
                      $$ = ast_allocate(TYPE_NODE, BVEC4, $1); }
  |   FLOAT_T		    { yTRACE("type -> float");
                      $$ = ast_allocate(TYPE_NODE, FLOAT_T, $1); }
  |   VEC2				  { yTRACE("type -> vec2");
                      $$ = ast_allocate(TYPE_NODE, VEC2, $1); }
  |   VEC3				  { yTRACE("type -> vec3"); 
                      $$ = ast_allocate(TYPE_NODE, VEC3, $1); }
  |   VEC4				  { yTRACE("type -> vec4");
                      $$ = ast_allocate(TYPE_NODE, VEC4, $1); }
  ;				  
exp
  :  
  | constructor			{ yTRACE("exp -> constructor"); 
                      $$ = ast_allocate(EXP_NODE, CONSTRUCTOR_NODE, $1); }
  | function				{ yTRACE("exp -> function");
                      $$ = ast_allocate(EXP_NODE, FUNCTION_NODE, $1); }
  | INT				  	  { yTRACE("exp -> integer_literal"); 
                      $$ = ast_allocate(EXP_NODE, INT_NODE, $1); }
  | FLOAT				    { yTRACE("exp -> float_literal"); 
                      $$ = ast_allocate(EXP_NODE, FLOAT_NODE, $1); }
  | T 				  	  { yTRACE("exp -> true"); 
                      $$ = ast_allocate(EXP_NODE, BOOL_NODE, $1);}
  | F				  	    { yTRACE("exp -> false"); 
                      $$ = ast_allocate(EXP_NODE, BOOL_NODE, $1); }
  | variable				          { yTRACE("exp -> variable"); 
                                $$ = ast_allocate(EXP_NODE, VAR_NODE, $1);}
  | unary_op exp			        { yTRACE("exp -> unary_op ex");
                                $$ = ast_allocate(EXP_NODE, UNARY_OP_NODE, $1, $2); }
  | LBRACKET exp RBRACKET		  { yTRACE("exp -> ( exp )");
                                $$ = ast_allocate(EXP_NODE, EXP_NODE, $2); }
  | exp ADD exp             { yTRACE("binary_op -> exp + exp");
                              $$ = ast_allocate(EXP_NODE, ADD, $1, $3); }
  | exp SUB exp             { yTRACE("binary_op -> exp - exp");
                              $$ = ast_allocate(EXP_NODE, SUB, $1, $3;) }
  | exp MUL exp             { yTRACE("binary_op -> exp * exp");
                              $$ = ast_allocate(EXP_NODE, MUL, $1, $3;) }
  | exp DIV exp             { yTRACE("binary_op -> exp / exp");
                              $$ = ast_allocate(EXP_NODE, DIV, $1, $3;) }
  | exp EXP exp             { yTRACE("binary_op -> exp ^ exp");
                              $$ = ast_allocate(EXP_NODE, EXP, $1, $3;) } //not in handout
  | exp AND exp             { yTRACE("binary_op -> exp &7xp");
                              $$ = ast_allocate(EXP_NODE, AND, $1, $3;) }
  | exp OR exp              { yTRACE("binary_op -> exp || exp");
                              $$ = ast_allocate(EXP_NODE, OR, $1, $3;) }
  | exp EQ exp              { yTRACE("binary_op -> exp == exp");
                              $$ = ast_allocate(EXP_NODE, EQ, $1, $3;) }
  | exp NEQ exp             { yTRACE("binary_op -> exp != exp");
                              $$ = ast_allocate(EXP_NODE, NEQ, $1, $3;) }
  | exp LESS exp            { yTRACE("binary_op -> exp < exp");
                              $$ = ast_allocate(EXP_NODE, LESS, $1, $3;)}
  | exp LEQ exp             { yTRACE("binary_op -> exp <= exp");
                              $$ = ast_allocate(EXP_NODE, LEQ, $1, $3;) }
  | exp GREATER exp         { yTRACE("binary_op -> exp > exp");
                              $$ = ast_allocate(EXP_NODE, GREATER, $1, $3;)}
  | exp GEQ exp             { yTRACE("binary_op -> exp >= exp");
                              $$ = ast_allocate(EXP_NODE, GEQ, $1, $3;)}
  ;
variable
  : IDENTIFIER		  		      { yTRACE("variable -> identifier");
                                $$ = ast_allocate(VAR_NODE, $1) }
  | IDENTIFIER LSQUARE INT RSQUARE	  	    { yTRACE("variable -> identifier[integer_literal]"); 
                                              $$ = ast_allocate(VAR_NODE, $1, $3;)}
  ;
unary_op
  : EXCLAM		  		{ yTRACE("unary_op -> !"); 
                      $$ = ast_allocate(UNARY_OP_NODE, EXCLAM)}
  | SUB		  			{ yTRACE("unary_op -> -");
                    $$ = ast_allocate(UNARY_OP_NODE, SUB) }
  ; 
constructor 
  : type LBRACKET arguments RBRACKET    		{ yTRACE("constructor -> type ( arguments )");
                                              $$ = ast_allocate(CONSTRUCTOR_NODE, $1, $3) }	
  ;
function
  : function_name LBRACKET arguments_opt RBRACKET       { yTRACE("function -> function_name ( arguments_opt )");
                                                          $$ = ast_allocate(FUNCTION_NODE, $1, $3); }
  ;
function_name
  : DP3        				{ yTRACE("function_name -> dp3");
                        $$ = ast_allocate(FUNCTION_NODE, DP3) }
  | LIT        				{ yTRACE("function_name -> lit");
                        $$ = ast_allocate(FUNCTION_NODE, LIT) }
  | RSQ        				{ yTRACE("function_name -> rsq");
                        $$ = ast_allocate(FUNCTION_NODE, RSQ) }
  ;
arguments_opt
  : arguments				{ yTRACE("arguments_opt -> arguments");
                      $$ = ast_allocate(ARGUMENTS_OPT_NODE, $1) }
  | /*empty*/				{ yTRACE("arguments_opt -> empty"); }
  ;
arguments
  : arguments COMMA exp			  { yTRACE("arguments -> arguments , exp");
                                $$ = ast_allocate(ARGUMENTS_NODE, $1, $3) }
  | exp					              { yTRACE("arguments -> exp");
                                $$ = ast_allocate(ARGUMENTS_NODE, $1) }
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

