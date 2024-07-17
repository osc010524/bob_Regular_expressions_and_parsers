%{
#include <stdio.h>
#include <stdlib.h>
#include "fb3-2.h"

// // 디버깅 모드를 활성화
// #define YYDEBUG 1
// int yydebug = 1;

void yyerror(const char *s);
int yylex(void);
%}

%union {
    struct ast *a;
    double d;
    struct symbol *s;
    struct symlist *sl;
}

%token <d> NUMBER
%token <s> NAME
%type <a> exp stmt
%type <sl> symlist
%right '='
%left '+' '-'
%left '*' '/'
%right UMINUS
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%token EOL

%%
calclist:
    | calclist stmt EOL { if ($2) { double result = eval($2); printf("= %f\n", result); treefree($2); } }
    ;
stmt:
    exp { $$ = $1; }
    | NAME '=' exp { $$ = newasgn($1, $3); }
    | NAME '(' ')' { $$ = newcall($1, NULL); }
    | NAME '(' symlist ')' { $$ = newcall($1, $3); }
    ;
exp:
    NUMBER { $$ = newnum($1); }
    | NAME { $$ = newref($1); }
    | exp '+' exp { $$ = newast('+', $1, $3); }
    | exp '-' exp { $$ = newast('-', $1, $3); }
    | exp '*' exp { $$ = newast('*', $1, $3); }
    | exp '/' exp { $$ = newast('/', $1, $3); }
    | '|' exp { $$ = newast('|', $2, NULL); }
    | '(' exp ')' { $$ = $2; }
    | '-' exp %prec UMINUS { $$ = newast('M', $2, NULL); }
    ;
symlist:
    NAME { $$ = newsymlist($1, NULL); }
    | symlist ',' NAME { $$ = newsymlist($3, $1); }
    ;
%%
int main(int argc, char **argv) {
    // yydebug = 1; // 디버깅 출력 활성화
    yyparse();
    return 0;
}

// void yyerror(const char *s) {
//     fprintf(stderr, "error: %s\n", s);
// }
