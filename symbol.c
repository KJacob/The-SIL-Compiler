#include "symbol.h"
#include <stdlib.h>
#include <stdio.h>

sym_t*
symbol_construct (char* name, int ref, int callable, int size, arg_list *args)
{
    sym_t *ptr;

    ptr = symtable_construct_entry(name, 0);
    ptr->name = name;
    ptr->reference = ref;
    ptr->callable = callable;
    ptr->arglist = args;
    ptr->next = NULL;

    return ptr;
}

sym_t*
symbol_list_append (sym_t *symlist, sym_t* symbol)
{
    if (!symlist)
        return symbol;

    symlist->next = symbol;
    return symlist;
}

void
symbol_list_apply_type (sym_t *symlist, int type)
{
    sym_t *ptr;

    ptr = symlist;

    while(ptr)
    {
        ptr->type = type;
        ptr = ptr->next;
    }
}

/* Symbol table functions begin here. */
sym_struct*
symtable_construct ()
{
    sym_struct *obj;
    
    obj = (sym_struct *)malloc (sizeof(sym_struct));
    obj->sym_table = NULL;
    obj->binding_base = 0;
    
    return obj;
}

sym_t*
symtable_construct_entry (const char *name, int value)
{
    sym_t *entry;
    
    entry = (sym_t *) malloc (sizeof(sym_t));
    
    entry->name = strdup(name);
    entry->val = value;
    
    return entry;
}

sym_t*
symtable_insert (sym_struct *obj, const char *sym_name, int value)
{
    sym_t *entry = symtable_construct_entry (sym_name, value);
    entry->next = NULL;
    
    if (!obj->sym_table) {
        obj->sym_table = entry;
    }
    else {
        sym_t *currp;
        
        currp = obj->sym_table;
        
        while (currp->next) {
            currp = currp->next;
        }
        
        currp->next = entry;
    }
    
    return entry;
}

sym_t*
symtable_search (sym_struct *obj, const char *name)
{
    sym_t *currp;
    
    currp = obj->sym_table;
    
    while (currp) {
        if (!strcmp (currp->name, name))
            return currp;
        currp=currp->next;
    }
    
    return NULL;
}

int
symtable_symbol_by_name(sym_struct *obj, int *val, const char *name)
{
    sym_t *sym_pt;
    
    sym_pt = symtable_search (obj, name);
    
    if (!sym_pt)
        return SYMT_FAILED;
    
    *val = sym_pt->val;
    return SYMT_SUCCESS;
}

int
symtable_exists (sym_struct *obj, const char *name)
{
    return symtable_search(obj, name) != NULL;
}

int
symtable_update (sym_struct *obj, const char *name, int newval)
{
    sym_t *currp;
    
    currp = symtable_search(obj, name);
    
    if (!currp)
    {
        return SYMT_FAILED;
    }
    
    currp->val = newval;
    return SYMT_SUCCESS;
}

void
symtable_destruct (sym_struct *obj)
{
    sym_t *currp, *nextp;
    
    currp = obj->sym_table;
    
    while (currp) {
        free (currp->name);
        nextp = currp->next;
        free(currp);
        currp = nextp;
    }
    
    free(obj);
}

void
symtable_symbol_install(sym_struct *obj, char *name, int size, int type)
{
    sym_t *entry;

    entry = symtable_search (obj, name);

    if (entry)
    {
        fprintf(stderr, "Redefinition of variable %s.\n", name);
        return;
    }
    
    entry->name = strdup(name);
    entry->val = 0;
    entry->type = type;
    entry->size = size;
    entry->binding = obj->binding_base;
    obj->binding_base = obj->binding_base + size;
    
    entry->next = obj->sym_table;
    obj->sym_table = entry;
    
    return;
}

void
symtable_install_arguments (sym_struct *obj, arg_list *args)
{
    /* The difference is that the bindings are negative. */
    int bind = -1;
    arg_list *ptr;
    sym_t *symbol;

    ptr = args;

    /* Calculate the depth of the stack. */

    while (ptr)
    {
        bind = bind - ptr->size;
        ptr = ptr->next;
    }

    ptr = args;

    /* Perform the installation. */
    while (ptr)
    {
        symbol = symtable_construct_entry(ptr->name, 0);
        symbol->size = ptr->size;
        symbol->type = ptr->type;
        symbol->reference = ptr->reference;
        bind = bind + symbol->size;
        symbol->scope = SYMBOL_SCOPE_LOCAL;

        ptr = ptr->next;
    }
}

/* Argument list functions. Might be soon moved. */
arg_list*
arglist_from_symbol (sym_t *symbol, int type, int reference)
{
    arg_list *ptr;

    ptr = (arg_list *) malloc (sizeof(arg_list));

    ptr->name = symbol->name;
    ptr->type = type;
    ptr->size = symbol->size;
    ptr->reference = reference;
    ptr->next = NULL;

    /* Since there must be only one symbol, destroy the symbol list and reset the collector. */
    //symcollect_symbols_destroy (collector);
    return ptr;
}

arg_list*
arglist_append(arg_list *list, arg_list *item)
{
    if (!list) 
    {
        /* Oops! */
        return item;
    }

    list->next = item;
    return list;
}