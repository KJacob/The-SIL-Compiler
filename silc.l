%{
#include <stdio.h>
#include "y.tab.h"
#include "tree.h"
#include "type.h"

static int g_line = 0;

int
lexer_currline ()
{
	return g_line;
}

%}
%%
	/* integers */
[0-9]+	{
				yylval.syn_tree = node_literal(atoi(yytext), DATA_INTEGER);
				return LITERAL;
			}

	/* operators */
[+*-/()=;%\[\]]	{
				return *yytext;
			}

"true" { 
	yylval.syn_tree = node_literal (1, DATA_BOOLEAN);
	return LITERAL;
}
"false" {
	yylval.syn_tree = node_literal (0, DATA_BOOLEAN);
	return LITERAL;
}
"integer" return INTEGER;
"boolean" return BOOLEAN;
"decl" return DECL;
"enddecl" return ENDDECL;
"if" return IF;
"then" return THEN;
"else" return ELSE;
"endif" return ENDIF;
"while" return WHILE;
"endwhile" return ENDWHILE;
"do" return DO;
"read" return READ;
"write" return WRITE;
"return" return RETURN;

">=" {
yylval.syn_tree = construct_node (CH_GTE, NULL, NULL, 0);
return RELOP;
}

"<=" {
	yylval.syn_tree = construct_node (CH_LTE, NULL, NULL, 0);
	return RELOP;
}

">" {
	yylval.syn_tree = construct_node(CH_GT, NULL, NULL, 0);
	return RELOP;
}

"<" {
	yylval.syn_tree = construct_node(CH_LT, NULL, NULL, 0);
	return RELOP;
}

"==" {
	yylval.syn_tree = construct_node(CH_EQ, NULL, NULL, 0);
	return RELOP;
}

"!=" {
	yylval.syn_tree = construct_node(CH_NOTEQ, NULL, NULL, 0);
	return RELOP;
}

"&&" {
    yylval.syn_tree = construct_node (CH_LOGAND, NULL, NULL, 0);
    return LOGOP;
}

"||" {
    yylval.syn_tree = construct_node (CH_LOGOR, NULL, NULL, 0);
    return LOGOP;
}

[a-zA-Z][a-zA-Z0-9]*	{
			yylval.syn_tree = node_identifier(strdup(yytext));
			return IDENT;
		}

[ \t]	{}
[\n] {g_line++;}
			
.	{yyerror("Unrecognised token.\n");}				
%%
