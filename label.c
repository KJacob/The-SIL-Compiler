#include <stdio.h>
#include <stdlib.h>

#include "label.h"

static int g_label_no = 0;

char*
label_new ()
{
	char *strlabel = (char *) malloc (LABEL_MAX_SIZE);
	sprintf (strlabel, "L%d", g_label_no);
	g_label_no++;

	return strlabel;
}

void
label_destroy (char *label)
{
	free (label);
}