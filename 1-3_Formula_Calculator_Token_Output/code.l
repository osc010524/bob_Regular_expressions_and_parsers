/* just like Unix wc */
%{
#include <stdio.h>
#include <string.h>

int yywrap(void) {
    return 1;
}
%}

%%
"+" { printf("PLUS\n"); }
"-" { printf("MINUS\n"); }
"*" { printf("TIMES\n"); }
"/" { printf("DIVIDE\n"); }
"!" { printf("ABS\n"); }
[0-9]+ { printf("NUMBER %s\n", yytext); }
\n { printf("NEWLINE\n"); }
[ \t] { }
. { printf("Mystery character %s\n", yytext); }
%%
int main(int argc, char **argv)
{
    yylex();
    return 0;
}

