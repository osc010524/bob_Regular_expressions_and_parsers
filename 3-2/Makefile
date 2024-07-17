all: fb3-2

fb3-2: fb3-2.l fb3-2.y fb3-2.h fb3-2funcs.c
	bison -d fb3-2.y
	flex fb3-2.l
	gcc -o $@ fb3-2.tab.c lex.yy.c fb3-2funcs.c

clean:
	rm -f fb3-2 fb3-2.tab.c fb3-2.tab.h lex.yy.c

.SILENT: clean
