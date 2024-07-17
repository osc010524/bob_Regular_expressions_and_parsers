/* just like Unix wc */
%{
#include <stdio.h>
#include <string.h>

int yywrap(void) {
    return 1;
}
%}

/* English -> American */
%%
"colour"       { printf("color"); }
"flavour"      { printf("flavor"); }
"clever"       { printf("smart"); }
"smart"        { printf("elegant"); }
"conservative" { printf("liberal"); }

.              { printf("%s", yytext); }
%%
int main(int argc, char **argv)
{
    yylex();
    return 0;
}

