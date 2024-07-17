#include <stdio.h>
#include <stdlib.h>
#include "fb3-1.h"

struct ast *newast(int nodetype, struct ast *l, struct ast *r) {
    struct ast *a = malloc(sizeof(struct ast));
    if (!a) {
        yyerror("out of space");
        exit(0);
    }
    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    return a;
}

struct ast *newnum(double d) {
    struct numval *a = malloc(sizeof(struct numval));
    if (!a) {
        yyerror("out of space");
        exit(0);
    }
    a->nodetype = 'K';
    a->number = d;
    return (struct ast *)a;
}

double eval(struct ast *a) {
    double v;
    switch (a->nodetype) {
        case 'K': v = ((struct numval *)a)->number; break;
        case '+': v = eval(a->l) + eval(a->r); break;
        case '-': v = eval(a->l) - eval(a->r); break;
        case '*': v = eval(a->l) * eval(a->r); break;
        case '/': v = eval(a->l) / eval(a->r); break;
        case '|': v = eval(a->l); if (v < 0) v = -v; break;
        default: printf("internal error: bad node %c\n", a->nodetype);
    }
    return v;
}

void treefree(struct ast *a) {
    switch (a->nodetype) {
        case '+':
        case '-':
        case '*':
        case '/':
            treefree(a->r);
        case '|':
            treefree(a->l);
        case 'K':
            free(a);
            break;
        default: printf("internal error: free bad node %c\n", a->nodetype);
    }
}

void yyerror(const char *s) {
    fprintf(stderr, "error: %s\n", s);
}
