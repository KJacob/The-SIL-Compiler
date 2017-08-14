#ifndef _SYMCOLLECT_H
#define _SYMCOLLECT_H

#include "symbol.h"

typedef struct _symcollect_struct
{
	sym_t *symbol_list;

} symcollect_struct;

void
symcollect_insert_symbols(symcollect_struct *obj, int type, sym_t *symbols);

void
symcollect_rinse (symcollect_struct *obj);

symcollect_struct*
symcollect_construct ();

void
symcollect_destruct (symcollect_struct *obj);

sym_t*
symcollect_symbol_list(symcollect_struct *obj);

void
symcollect_symbols_destroy(symcollect_struct *obj);

void
symcollect_install_symbols(symcollect_struct *collector, sym_struct *dest);

#endif