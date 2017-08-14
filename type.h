#ifndef _TYPE_H
#define _TYPE_H

#define TYPE_LVALUE 1
#define TYPE_RVALUE 2

#define DATA_BOOLEAN 4
#define DATA_INTEGER 8
#define DATA_VOID 16
#define DATA_COMPOUND 32
#define DATA_MISMATCH 64

void yyerror (const char *str);

#endif