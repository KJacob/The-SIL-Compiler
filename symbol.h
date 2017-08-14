#ifndef _SYMBOL_H
#define _SYMBOL_H

#include <stdlib.h>
#include <string.h>

#define SYMT_FAILED 0
#define SYMT_SUCCESS 1

#define SYMBOL_SCOPE_GLOBAL 0
#define SYMBOL_SCOPE_LOCAL 1

typedef struct _arg_list
{
    char *name;
	int type;
	int size;
    int reference;

	struct _arg_list *next;
} arg_list;

typedef struct _sym_t
{
    char *name;
    int val;
    int type; /* Type of the variable. */
    int size;
    int scope;
    int binding;
    int reference;
    int callable;
    arg_list *arglist;
    
    struct _sym_t *next;
} sym_t;

typedef struct _sym_struct
{
    sym_t *sym_table;
    int binding_base;
}sym_struct;

/* Symbol functions. */
sym_t*
symbol_construct (char* name, int ref, int callable, int size, arg_list *args);

sym_t*
symbol_list_append (sym_t *symlist, sym_t* symbol);

void
symbol_list_apply_type (sym_t *symlist, int type);

sym_struct*
symtable_construct ();

static
sym_t*
symtable_construct_entry (const char *name, int value);

sym_t*
symtable_insert (sym_struct* obj, const char *sym_name, int value);

sym_t*
symtable_search (sym_struct *obj, const char *name);

int
symtable_symbol_by_name(sym_struct *obj, int *val, const char *name);

int
symtable_exists (sym_struct *obj, const char *name);

int
symtable_update (sym_struct *obj, const char *name, int newval);

void
symtable_destruct (sym_struct *obj);

void
symtable_symbol_install (sym_struct *obj, char *name, int type, int size);

/* Functions for srgument list manipulation, might be soon moved. */
arg_list*
arglist_from_symbol (sym_t *symbol, int type, int reference);

arg_list*
arglist_append(arg_list *list, arg_list *item);

#endif