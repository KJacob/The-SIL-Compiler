#ifndef _COMPILE_H
#define _COMPILE_H

#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

#define COMPILE_STACK_BASE 3600

int
silc_compile (silc_struct *silc, tree *root, FILE *outstream);

int
silc_generate_code (silc_struct *silc, tree *root);

int
silc_generate_code_arith (silc_struct *silc, tree *root);

int
silc_generate_code_logical (silc_struct *silc, tree *root);

#endif