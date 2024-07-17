%{
#include "parser.tab.h"
#include <stdlib.h>
%}

%%
[0-9]+ { yylval = atoi(yytext); return NUMBER; }
"+" { return ADD; }
"-" { return SUB; }
"*" { return MUL; }
"/" { return DIV; }
"!" { return ABS; }
\n { return EOL; }
[ \t] { /* ignore whitespace */ }
. { printf("Mystery character %c\n", *yytext); }

%%
int yywrap() {
    return 1;
}
