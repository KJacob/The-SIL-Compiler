#include "compile.h"
#include "register.h"
#include "label.h"

static FILE *g_out;

int
silc_compile (silc_struct *silc, tree *root, FILE *f_out)
{
	g_out = f_out;

	fprintf(g_out, "START\n");
	silc_generate_code (silc, root);
	fprintf(g_out, "HALT\n");

	return 0;
}

int
silc_generate_code (silc_struct *silc, tree *root)
{
	char *label1, *label2;
	int reg1, reg2, reg3;

	if (!root)
		return register_peek(); /* Nothing to do. */

	switch(root->op)
	{
		case CH_STATEMENTSEQ:
			silc_generate_code(silc, root->left);
			silc_generate_code(silc, root->right);
			break;

		case CH_NUMBER:
			reg1 = register_acquire();
			fprintf(g_out, "MOV R%d, %d\n", reg1, root->val);
			return reg1;

		case CH_PLUS:
		case CH_MINUS:
		case CH_DIV:
		case CH_MUL:
		case CH_MOD:
		case CH_LT:
		case CH_GT:
		case CH_GTE:
		case CH_LTE:
		case CH_NOTEQ:
		case CH_EQ:
			return silc_generate_code_arith(silc, root);

		case CH_WRITE:
			reg1 = silc_generate_code(silc, root->left);
			fprintf(g_out, "OUT R%d\n", reg1);
			register_release(reg1);
			return register_peek();

		case CH_IDENTIFIER:
			reg1 = register_acquire();
			fprintf(g_out, "MOV R%d, [%d]\n", reg1, root->symbol->binding);
			return reg1;

		case CH_ARRAYDEREF:
			reg1 = register_acquire();
			fprintf (g_out, "MOV R%d, %d\n", reg1, root->left->symbol->binding);
			reg2 = silc_generate_code(silc, root->right);
			fprintf (g_out, "ADD R%d, R%d\n", reg2, reg1);
			fprintf (g_out, "MOV R%d, [R%d]\n", reg1, reg2);
			register_release(reg2);
			return reg1;

		case CH_ASSIGN:
			reg1 = silc_generate_code (silc, root->right);
			fprintf (g_out, "MOV [%d], R%d\n", root->left->symbol->binding, reg1);
			register_release(reg1);
			return register_peek();

		case CH_ASSIGNARRAY:
			reg1 = silc_generate_code (silc, root->right);
			reg2 = silc_generate_code (silc, root->middle);
			reg3 = register_acquire();

			fprintf(g_out, "MOV R%d, %d\n", reg3, root->left->symbol->binding);
			fprintf (g_out, "ADD R%d, R%d\n", reg3, reg2);
			fprintf (g_out, "MOV [R%d], R%d\n", reg3, reg1);

			register_release(reg3);
			register_release(reg2);
			register_release(reg1);
			return register_peek();

		case CH_READ:
			reg1 = register_acquire();
			fprintf(g_out, "IN R%d\n", reg1);
			fprintf(g_out, "MOV [%d], R%d\n", root->left->symbol->binding, reg1);
			register_release(reg1);
			return register_peek();

		case CH_READARRAY:
			reg1 = silc_generate_code(silc, root->right);
			reg2 = register_acquire();
			fprintf(g_out, "MOV R%d, %d\n", reg2, root->left->symbol->binding);
			fprintf(g_out, "ADD R%d, R%d\n", reg1, reg2);
			fprintf(g_out, "IN R%d\n", reg2);
			fprintf(g_out, "MOV [R%d], R%d\n", reg1, reg2);
			register_release(reg2);
			register_release(reg1);
			return register_peek();

		case CH_IF:
			reg1 = silc_generate_code(silc, root->left);
			label1 = label_new ();
			label2 = label_new ();
			fprintf(g_out, "JZ R%d, %s\n", reg1, label1);
			register_release(reg1);
			silc_generate_code(silc, root->middle);
			fprintf(g_out, "JMP %s\n", label2);
			fprintf(g_out, "%s:\n", label1);
			silc_generate_code (silc, root->right);
			fprintf(g_out, "%s:\n", label2);
			label_destroy(label2);
			label_destroy(label1);
			return register_peek();

		case CH_WHILE:
			label1 = label_new();
			label2 = label_new();
			fprintf(g_out, "%s:\n", label1);
			reg1 = silc_generate_code(silc, root->left);
			fprintf(g_out, "JZ R%d, %s\n", reg1, label2);
			register_release(reg1);
			silc_generate_code(silc, root->right);
			fprintf(g_out, "JMP %s\n", label1);
			fprintf(g_out, "%s:\n", label2);
			label_destroy(label2);
			label_destroy(label1);
			return register_peek();

		case CH_LOGAND:
		case CH_LOGOR:
			return silc_generate_code_logical (silc, root);

		default:
			printf("Not implemented. \n");
	}

	return 0;
}

int
silc_generate_code_logical(silc_struct *silc, tree *root)
{
	int reg1, reg2;
	char *label;

	label = label_new();

	reg1 = silc_generate_code(silc, root->left);

	switch (root->op)
	{
		case CH_LOGAND:
			fprintf(g_out, "JZ R%d, %s\n", reg1, label);
			break;

		case CH_LOGOR:
			fprintf (g_out, "JNZ R%d, %s\n", reg1, label);
	}

	reg2 = silc_generate_code(silc, root->right);
	fprintf(g_out, "%s:\n", label);

	switch (root->op)
	{
		case CH_LOGAND:
			fprintf(g_out, "MUL R%d, R%d\n", reg1, reg2);
			break;

		case CH_LOGOR:
			fprintf(g_out, "ADD R%d, R%d\n", reg2, reg1);
			fprintf(g_out, "MOV R%d, 0\n", reg1);
			fprintf(g_out, "NE R%d, R%d\n", reg1, reg2);
			break;
	}

	label_destroy(label);
	register_release(reg2);
	return reg1;
}

int
silc_generate_code_arith (silc_struct *silc, tree *root)
{
	int reg1, reg2;

	reg1 = silc_generate_code(silc, root->left);
	reg2 = silc_generate_code(silc, root->right);
	switch (root->op)
	{
		case CH_PLUS:
			fprintf(g_out, "ADD R%d, R%d\n", reg1, reg2);
			break;

		case CH_MINUS:
			fprintf(g_out, "SUB R%d, R%d\n", reg1, reg2);
			break;

		case CH_MUL:
			fprintf(g_out, "MUL R%d, R%d\n", reg1, reg2);
			break;

		case CH_DIV:
			fprintf(g_out, "DIV R%d, R%d\n", reg1, reg2);
			break;

		case CH_MOD:
			fprintf(g_out, "MOD R%d, R%d\n", reg1, reg2);
			break;

		case CH_GT:
			fprintf(g_out, "GT R%d, R%d\n", reg1, reg2);
			break;

		case CH_LT:
			fprintf(g_out, "LT R%d, R%d\n", reg1, reg2);
			break;

		case CH_LTE:
			fprintf(g_out, "LE R%d, R%d\n", reg1, reg2);
			break;

		case CH_GTE:
			fprintf(g_out, "GE R%d, R%d\n", reg1, reg2);
			break;

		case CH_EQ:
			fprintf(g_out, "EQ R%d, R%d\n", reg1, reg2);
			break;

		case CH_NOTEQ:
			fprintf(g_out, "NE R%d, R%d\n", reg1, reg2);
			break;

		default:
			;
	}

	register_release(reg2);
	return reg1;
}