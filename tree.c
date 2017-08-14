#include <stdlib.h>
#include <stdio.h>

#include "tree.h"

tree*
construct_node (int op, tree *left, tree *right, int val)
{
    tree* root = (tree *) malloc (sizeof(tree));
    
    root->left = left;
    root->right = right;
    root->op = op;
    root->val = val;
    
    return root;
}

tree*
node_read (tree *variable)
{
    tree* ptr;
    
    ptr = construct_node (CH_READ, variable, NULL, 0);
    node_assert_type(variable, DATA_INTEGER);
    ptr->type = DATA_INTEGER;
    
    return ptr;
}

tree*
node_read_array (tree *var, tree *index)
{
    tree *ptr;
    
    ptr = construct_node (CH_READARRAY, var, index, 0);
    node_assert_type(var, DATA_INTEGER);
    node_assert_type(index, DATA_INTEGER);
    ptr->type = DATA_INTEGER;
    
    return ptr;
}

tree*
node_assignment_array (tree *ident, tree *index, tree *expression)
{
    tree *ptr;
    
    ptr = construct_node (CH_ASSIGNARRAY, ident, expression, 0);
    
    ptr->middle = index;

    /* Perform type checking. */
    node_assert_type(index, DATA_INTEGER);
    node_assert_type (expression, ident->type);
    ptr->type = expression->type;
    
    return ptr;
}

tree*
node_dereference (tree *ident, tree *expression)
{
    tree *ptr;
    
    ptr = construct_node(CH_ARRAYDEREF, ident, expression, 0);
    node_assert_type(expression, DATA_INTEGER);
    ptr->type = ident->type;
    
    return ptr;
}

tree*
node_literal (int number, int type)
{
    tree *ptr;
    
    ptr = construct_node (CH_NUMBER, NULL, NULL, number);
    ptr->type = type;
    
    return ptr;
}

tree*
node_statementseq (tree *statementlist, tree *statement)
{
    tree *ptr;
    
    ptr = construct_node (CH_STATEMENTSEQ, statementlist, statement, 0);
    ptr->type = DATA_VOID;
    
    return ptr;
}

tree*
node_identifier (char *name)
{
    tree *ptr;
    
    ptr = construct_node (CH_IDENTIFIER, NULL, NULL, 0);
    node_set_type (ptr, DATA_INTEGER, NULL);
    ptr->name = name;
    
    return ptr;
}

tree*
node_assignment (tree *left, tree *right)
{
    tree *ptr;
    
    ptr = construct_node (CH_ASSIGN, left, right, 0);
    node_assert_type(left, right->type);
    ptr->type = left->type;
    
    return ptr;
}

tree *
node_arithmetic (int op, tree *left, tree *right)
{
    tree *ptr;
    
    ptr = construct_node (op, left, right, 0);

    node_assert_type(left, DATA_INTEGER);
    node_assert_type(right, DATA_INTEGER);
    ptr->type = DATA_INTEGER;
    
    return ptr;
}

tree*
node_if (tree *exp, tree *ifpart, tree *elsepart)
{
    tree *ptr;
    
    ptr = construct_node (CH_IF, exp, elsepart, 0);

    node_assert_type(exp, DATA_BOOLEAN);
    ptr->type = DATA_VOID;
    
    ptr->middle = ifpart;
    
    return ptr;
}

tree*
node_while (tree *left, tree *right)
{
    tree *ptr;
    
    ptr = construct_node (CH_WHILE, left, right, 0);
    node_assert_type(left, DATA_BOOLEAN);
    ptr->type = DATA_VOID;
    
    return ptr;
}

void
node_set_type (tree *t, int type, sym_t *symbol)
{
    t->type = type;
    t->symbol = symbol;
    
    return;
}

tree*
node_write (tree *expr)
{
    tree *ptr;

    ptr = construct_node (CH_WRITE, expr, NULL, 0);
    ptr->type = DATA_VOID;

    return ptr;
}

void
node_typecheck_logop (tree *root)
{
    if (root->left->type != DATA_BOOLEAN || root->right->type != DATA_BOOLEAN)
    {
        yyerror("Logical operator type mismatch.");
        return;
    }

    root->type = DATA_BOOLEAN;
}

void
node_typecheck_relop (tree *root)
{
    if (root->left->type != DATA_INTEGER || root->right->type != DATA_INTEGER)
    {
        yyerror("Relational operator type mismatch.");
        return;
    }

    root->type = DATA_BOOLEAN;
}

void
node_assert_type(tree *root, int type)
{
    if (type == root->type)
        return;

    yyerror("Incorrect type: Type mismatch.");
}