#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "fb3-2.h"

// Symbol table
static unsigned symhash(char *sym) {
    unsigned int hash = 0;
    unsigned c;
    while ((c = *sym++)) hash = hash * 9 ^ c;
    return hash;
}

struct ufncall {
    int nodetype;
    struct symbol *s;
    struct symlist *sl;
};

struct symbol *lookup(char *sym) {
    struct symbol *sp = &symtab[symhash(sym) % NHASH];
    int scount = NHASH;  // how many have we looked at
    while (--scount >= 0) {
        if (sp->name && !strcmp(sp->name, sym)) return sp;
        if (!sp->name) {  // new entry
            sp->name = strdup(sym);
            sp->value = 0;
            sp->func = NULL;
            sp->syms = NULL;
            return sp;
        }
        if (++sp >= symtab + NHASH) sp = symtab;  // try the next entry
    }
    yyerror("symbol table overflow\n");
    abort();  // tried them all, table is full
}

// create an AST node
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

// create a number node
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

// create a symbol reference node
struct ast *newref(struct symbol *s) {
    struct symref *a = malloc(sizeof(struct symref));
    if (!a) {
        yyerror("out of space");
        exit(0);
    }
    a->nodetype = 'N';
    a->s = s;
    return (struct ast *)a;
}

// create a symbol assignment node
struct ast *newasgn(struct symbol *s, struct ast *v) {
    struct symasgn *a = malloc(sizeof(struct symasgn));
    if (!a) {
        yyerror("out of space");
        exit(0);
    }
    a->nodetype = '=';
    a->s = s;
    a->v = v;
    return (struct ast *)a;
}

// create a function call node
struct ast *newcall(struct symbol *s, struct symlist *sl) {
    struct ufncall *a = malloc(sizeof(struct ufncall));
    if (!a) {
        yyerror("out of space");
        exit(0);
    }
    a->nodetype = 'C';
    a->s = s;
    a->sl = sl;
    return (struct ast *)a;
}

// create a list of symbols
struct symlist *newsymlist(struct symbol *sym, struct symlist *next) {
    struct symlist *sl = malloc(sizeof(struct symlist));
    if (!sl) {
        yyerror("out of space");
        exit(0);
    }
    sl->sym = sym;
    sl->next = next;
    return sl;
}

// free a list of symbols
void symlistfree(struct symlist *sl) {
    struct symlist *nsl;
    while (sl) {
        nsl = sl->next;
        free(sl);
        sl = nsl;
    }
}

// evaluation of an AST node
double eval(struct ast *a) {
    double v;
    switch (a->nodetype) {
        case 'K': v = ((struct numval *)a)->number; break;
        case 'N': v = ((struct symref *)a)->s->value; break;
        case '=': v = ((struct symasgn *)a)->s->value = eval(((struct symasgn *)a)->v); break;
        case '+': v = eval(a->l) + eval(a->r); break;
        case '-': v = eval(a->l) - eval(a->r); break;
        case '*': v = eval(a->l) * eval(a->r); break;
        case '/': v = eval(a->l) / eval(a->r); break;
        case '|': v = eval(a->l); if (v < 0) v = -v; break;
        case 'M': v = -eval(a->l); break; // 부정 연산자
        case 'C': // 함수 호출
            if (strcmp(((struct ufncall *)a)->s->name, "sum") == 0) {
                v = 0;
                struct symlist *sl;
                for (sl = ((struct ufncall *)a)->sl; sl; sl = sl->next)
                    v += eval(newref(sl->sym));
            } else if (strcmp(((struct ufncall *)a)->s->name, "product") == 0) {
                v = 1;
                struct symlist *sl;
                for (sl = ((struct ufncall *)a)->sl; sl; sl = sl->next)
                    v *= eval(newref(sl->sym));
            } else if (strcmp(((struct ufncall *)a)->s->name, "max") == 0) {
                v = -DBL_MAX;
                struct symlist *sl;
                for (sl = ((struct ufncall *)a)->sl; sl; sl = sl->next) {
                    double val = eval(newref(sl->sym));
                    if (val > v) v = val;
                }
            } else if (strcmp(((struct ufncall *)a)->s->name, "min") == 0) {
                v = DBL_MAX;
                struct symlist *sl;
                for (sl = ((struct ufncall *)a)->sl; sl; sl = sl->next) {
                    double val = eval(newref(sl->sym));
                    if (val < v) v = val;
                }
            } else {
                yyerror("Unknown function");
                v = 0;
            }
            break;
        default: 
            printf("internal error: bad node %c\n", a->nodetype);
            v = 0;
            break;
    }
    return v;
}

// free an AST node
void treefree(struct ast *a) {
    switch (a->nodetype) {
        case '+':
        case '-':
        case '*':
        case '/':
            treefree(a->r);
        case '|':
        case 'M':
            treefree(a->l);
        case 'K':
        case 'N':
            free(a);
            break;
        case '=':
            free(((struct symasgn *)a)->v);
            break;
        case 'C':
            if (((struct ufncall *)a)->sl) symlistfree(((struct ufncall *)a)->sl);
            break;
        default: 
            printf("internal error: free bad node %c\n", a->nodetype);
            break;
    }
}

void yyerror(const char *s) {
    fprintf(stderr, "error: %s\n", s);
}
