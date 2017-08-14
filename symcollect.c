#include "symcollect.h"
#include "type.h"

#include <stdlib.h>
#include <string.h>

void
symcollect_rinse (symcollect_struct *obj)
{
	obj->symbol_list = NULL;
}

symcollect_struct*
symcollect_construct ()
{
	symcollect_struct *obj;

	obj = (symcollect_struct *) malloc (sizeof(symcollect_struct));
	obj->symbol_list = NULL;

	return obj;
}

void
symcollect_destruct (symcollect_struct *obj)
{
	/* We don't own anything but obj. */
	free (obj);
}

sym_t*
symcollect_symbol_list(symcollect_struct *obj)
{
	return obj->symbol_list;
}

void
symcollect_symbols_destroy(symcollect_struct *obj)
{
	sym_t *ptr, *next;

	ptr = symcollect_symbol_list(obj);

	while (ptr)
	{
		next = ptr->next;

		free (ptr->name);
		free(ptr);

		ptr = next;
	}

	symcollect_rinse(obj);
}

void
symcollect_insert_symbols(symcollect_struct *obj, int type, sym_t *symbols)
{
	sym_t *ptr;

	symbol_list_apply_type (symbols, type);

	ptr = symbols;

	if (ptr)
	{
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = obj->symbol_list;
		obj->symbol_list = symbols;
	}
}

void
symcollect_install_symbols(symcollect_struct *obj, sym_struct *dest)
{
    sym_t *symbols;

    symbols = symcollect_symbol_list(obj);
    dest->sym_table = symbols; /* Installation complete. */

    /* Calculate bindings. */
    while(symbols)
    {
        symbols->binding = dest->binding_base;
        dest->binding_base = symbols->size + dest->binding_base;

        symbols = symbols->next;
    }
}