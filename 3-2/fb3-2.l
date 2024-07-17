%{
#include "fb3-2.tab.h"
#include <stdlib.h>
#include <ctype.h>

// lookup 함수 선언 추가
struct symbol *lookup(char *sym);

void yyerror(const char *s);
int yylineno;
%}

%%
[0-9]+([.][0-9]*)? { yylval.d = atof(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.s = lookup(yytext); return NAME; }
"=" { return '='; }
"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"(" { return '('; }
")" { return ')'; }
"," { return ','; }
"|" { return '|'; }
\n { return EOL; }
[ \t] { /* ignore whitespace */ }
. { printf("Mystery character %c\n", *yytext); }

%%
int yywrap() {
    return 1;
}
