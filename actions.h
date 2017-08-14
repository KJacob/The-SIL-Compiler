#ifndef _ACTIONS_H
#define _ACTIONS_H

#include "tree.h"
#include "symcollect.h"

silc_struct*
silc_construct ();

void
silc_destruct (silc_struct *calc);

void
silc_install_symbols (silc_struct *obj, symcollect_struct *collector);

int
silc_resolve_ident (silc_struct *obj, tree *ident);

void
silc_handle_local_decl (silc_struct *obj, symcollect_struct *collector);

#endif
