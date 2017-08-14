#ifndef _TREE_H
#define _TREE_H

#include "symbol.h"
#include "type.h"

#define CH_NUMBER 0
#define CH_PLUS 1
#define CH_MINUS 2
#define CH_MUL 3
#define CH_DIV 4
#define CH_GT 5
#define CH_LT 6
#define CH_LTE 7
#define CH_GTE 8
#define CH_NOTEQ 9
#define CH_ASSIGN 10
#define CH_WRITE 11
#define CH_IF 12
#define CH_WHILE 13
#define CH_EQ 14
#define CH_STATEMENTSEQ 15
#define CH_IDENTIFIER 16
#define CH_MOD 17
#define CH_READ 18
#define CH_ARRAYDEREF 19
#define CH_ASSIGNARRAY 20
#define CH_READARRAY 21
#define CH_LOGAND 22
#define CH_LOGOR 23

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define INTERPRET_SUCCESS 1
#define INTERPRET_FAILED 0

typedef struct _tree{
    int op;
    struct _tree *left, *middle, *right;
    int val, type;
    char *name;
    sym_t *symbol;
} tree;

typedef struct _silc_struct{
    int on_error;
    char *err_message;
    sym_struct *local_sym_table; /* Current local symbol table. */
    sym_struct *sym_table;
} silc_struct;

silc_struct*
silc_construct ();

/* Various nodes. */
tree*
node_literal (int number, int type);

tree*
node_identifier (char *name);

tree *
node_arithmetic (int op, tree *left, tree *right);

tree*
node_statementseq (tree *statementlist, tree *statement);

tree*
node_assignment (tree *left, tree *right);

tree*
node_if (tree *exp, tree *ifpart, tree *elsepart);

tree*
node_while (tree *left, tree *right);

tree*
construct_node (int op, tree *left, tree *right, int val);

tree*
construct_node6 (int op, tree *left, tree *right, tree *mid, int val, const char *name);

void
node_set_type (tree *t, int type, sym_t *symbol);

tree*
node_read (tree *variable);

tree*
node_read_array (tree *var, tree *index);

tree*
node_assignment_array (tree *ident, tree *index, tree *expression);

tree*
node_dereference (tree *ident, tree *expression);

tree*
node_write (tree *expr);

void
node_typecheck_relop (tree *root);

void
node_typecheck_logop (tree *root);

void
node_assert_type (tree *root, int type);

#endif