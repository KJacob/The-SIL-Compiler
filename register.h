#ifndef _REGISTER_H
#define _REGISTER_H

int
register_acquire ();

void
register_release (int register);

int
register_peek ();

#endif