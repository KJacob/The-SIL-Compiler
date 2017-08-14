#Our compiler.
CC = cc
CFLAGS = -g
LEX = lex
YACC = yacc

#Default rule.

default: silc

silc: actions.o tree.o lex.yy.o symbol.o y.tab.o label.o register.o compile.o symcollect.o
	$(CC) $(CFLAGS) -o silc actions.o tree.o lex.yy.o symbol.o y.tab.o label.o register.o compile.o symcollect.o

symcollect.o: symcollect.c symcollect.h
	$(CC) $(CFLAGS) -c symcollect.c

label.o: label.c label.h
	$(CC) $(CFLAGS) -c label.c

register.o: register.c register.h
	$(CC) $(CFLAGS) -c register.c

compile.o: compile.c compile.h
	$(CC) $(CFLAGS) -c compile.c

actions.o: actions.h actions.c type.h
	$(CC) $(CFLAGS) -c actions.c

tree.o: tree.c tree.h type.h
	$(CC) $(CFLAGS) -c tree.c

lex.yy.o: lex.yy.c y.tab.c
	$(CC) $(CFLAGS) -c lex.yy.c

symbol.o: symbol.c symbol.h
	$(CC) $(CFLAGS) -c symbol.c

y.tab.o: y.tab.c y.tab.h
	$(CC) $(CFLAGS) -c y.tab.c

lex.yy.c: silc.l
	$(LEX) silc.l

y.tab.c: silc.y
	$(YACC) -d silc.y

clean:
	$(RM) silc *.o lex.yy.c y.tab.h y.tab.c