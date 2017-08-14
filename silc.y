%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "actions.h"
#include "type.h"
#include "compile.h"
#include "symcollect.h"

static silc_struct *g_silc;
static symcollect_struct *g_collect;

int g_err_flag;
extern FILE *yyin;

int yylex();
int yyparse();
int lexer_currline();

void yyerror(const char *str)
{
        g_err_flag = 1;
        fprintf(stderr,"Line %d: %s\n", lexer_currline(), str);
}
 
int yywrap()
{
        return 1;
}

int  
main(int argc, char **argv)
{
   FILE *fp;

   g_err_flag = 0;
   
   if (argc == 2)
   {
       fp = fopen (*(argv + 1), "r");
       
       if (!fp)
            fprintf (stderr, "Can not open input file.\n");
       else
            yyin = fp;
   }
   
   g_silc = silc_construct ();
   g_collect = symcollect_construct ();

   yyparse();
   
   silc_destruct (g_silc);
   symcollect_destruct (g_collect);
   
   if (fp)
       fclose(fp);
   return 0;
} 

%}

%union{
    struct _tree *syn_tree;
    struct _arg_list *args;
    int data_type;
    struct _sym_t *symbol;
}

%start program

%token LITERAL IF IDENT ELSE ENDIF THEN WHILE DO ENDWHILE RELOP INTEGER BOOLEAN DECL ENDDECL READ WRITE LOGOP RETURN

%type <syn_tree> expr LITERAL IDENT statement statementseq RELOP LOGOP identifier
%type <data_type> type
%type <symbol> declitem declarator
%type <args> arglist argument

%right '='
%left LOGOP
%left RELOP
%left '+' '-'
%left '*' '/' '%'

%%

program:
  g_declarations functions {}
;

functions:
  |
  functions function
;

function:
  type IDENT '(' arglist ')' '{' l_declarations statementseq '}' {}
;

g_declarations:
  declarations {silc_install_symbols(g_silc, g_collect); }
;

l_declarations:
  declarations
;

declarations:
  DECL declist ENDDECL ';'
  |
;

declist:
  declist declaration ';'
  |
  declaration ';'
  |
  error ';' {}
;

declaration:
  type declarator {symcollect_insert_symbols(g_collect, $1, $2);}
;

declarator:
  declarator ',' declitem {$$ = symbol_list_append($1, $3); }
  |
  declitem {$$ = $1; }
;

declitem:
  IDENT {$$ = symbol_construct($1->name, FALSE, FALSE, 1, NULL); }
  |
  IDENT '[' LITERAL ']' {$$ = symbol_construct($1->name, FALSE, FALSE, $3->val, NULL); }
  |
  IDENT '(' arglist ')' {$$ = symbol_construct($1->name, FALSE, FALSE, 0, $3); }
;

arglist:
  {$$ = NULL; }
  |
  arglist ',' argument {$$ = arglist_append($1, $3); }
;

argument:
  type declitem {$$ = arglist_from_symbol($2, $1, FALSE); }
  |
  type '&' declitem {$$ = arglist_from_symbol($3, $1, TRUE); }
;

type:
  INTEGER {$$ = DATA_INTEGER; }
  |
  BOOLEAN {$$ = DATA_BOOLEAN; }
;

statementseq:
  statementseq statement ';' { $$ = node_statementseq($1, $2); }
  |
  statementseq error ';' {}
  |
  statement ';' {$$ = node_statementseq (NULL, $1); }
;

statement:
  identifier '=' expr {$$ = node_assignment($1, $3);}
  |
  identifier '[' expr ']' '=' expr {$$ = node_assignment_array($1, $3, $6);}
  |
  IF '(' expr ')' THEN statementseq ENDIF {$$ = node_if ($3, $6, NULL); }
  |
  IF '(' expr ')' THEN statementseq ELSE statementseq ENDIF {$$ = node_if($3, $6, $8); }
  |
  WHILE '(' expr ')' DO statementseq ENDWHILE {$$ = node_while($3, $6); }
  |
  READ '(' identifier ')' {$$ = node_read ($3);}
  |
  READ '(' identifier '[' expr ']' ')' {$$ = node_read_array ($3, $5);}
  |
  WRITE '(' expr ')' {$$ = node_write ($3); }
  |
  RETURN expr {$$ = node_return($2);}
;
				
expr:
  LITERAL { $$ = $1; }
  |
  identifier {$$ = $1;}
  |
  identifier '[' expr ']' {$$ = node_dereference($1, $3);}
	|
  expr '+' expr	{ $$ = node_arithmetic(CH_PLUS, $1, $3); }
	|
	expr '-' expr	{ $$ = node_arithmetic(CH_MINUS, $1, $3);}
	|
	expr '*' expr	{ $$ = node_arithmetic(CH_MUL, $1, $3);}
	|
  expr '/' expr	{ $$ = node_arithmetic(CH_DIV, $1, $3);}
  |
  expr '%' expr   {$$ = node_arithmetic(CH_MOD, $1, $3);}
  |
  '(' expr ')'    { $$ = $2; }
  |
  expr RELOP expr {$$ = $2; $$->left = $1; $$->right = $3; node_typecheck_relop ($$); }
  |
  expr LOGOP expr {$$ = $2; $$->left = $1; $$->right = $3; node_typecheck_logop ($$);}
;

identifier:
  IDENT {$$ = $1; silc_resolve_ident(g_silc, $1); }
%%
