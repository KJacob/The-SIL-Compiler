#include "actions.h"
#include <stdio.h>
#include <stdlib.h>

silc_struct*
silc_construct ()
{
    silc_struct *obj;
    
    obj = (silc_struct *) malloc (sizeof(silc_struct));
    
    obj->on_error = FALSE;
    obj->err_message = (char *) malloc (sizeof(char) * 100);
    obj->sym_table = symtable_construct();

    return obj;
}

void
silc_destruct (silc_struct *obj)
{
    free(obj->err_message);
    symtable_destruct (obj->sym_table);
    free(obj);
}

void
silc_install_symbols (silc_struct *obj, symcollect_struct *collector)
{
    symcollect_install_symbols (collector, obj->sym_table);
}

int
silc_resolve_ident (silc_struct *obj, tree *ident)
{
    sym_t *symbol;

    symbol = symtable_search (obj->sym_table, ident->name);
    ident->symbol = symbol;

    if (!symbol)
    {
        char buffer[100];
        sprintf(buffer, "Variable %s is undefined.", ident->name);
        yyerror(buffer);
        return 0;
    }

    ident->type = symbol->type;

    return symbol != NULL;
}

void
silc_handle_local_decl (silc_struct *obj, symcollect_struct *collector)
{
    obj->local_sym_table = symtable_construct ();

    symcollect_install_symbols(collector, obj->local_sym_table);
}