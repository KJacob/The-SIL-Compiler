static int g_currreg = -1;

int
register_acquire ()
{
	return ++g_currreg;
}

void
register_release (int regno)
{
	g_currreg--;
}

int
register_peek ()
{
	return g_currreg;
}