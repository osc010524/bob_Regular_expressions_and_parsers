/* just like Unix wc */
%{
#include <stdio.h>
#include <string.h>

int chars = 0;
int words = 0;
int lines = 0;

int yywrap(void) {
    return 1;
}
%}

%%
[a-zA-Z]+   { words++; chars += strlen(yytext); }
\n          { chars++; lines++; }
.           { chars++; }

%%
int main(int argc, char **argv)
{
    yylex();
    printf("%d_%d_%d\n", lines, words, chars);
    return 0;
}

