#ifndef FB3_2_H
#define FB3_2_H

extern int yylineno;
void yyerror(const char *s);

struct ast {
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct numval {
    int nodetype;
    double number;
};

struct symref {
    int nodetype;
    struct symbol *s;
};

struct symasgn {
    int nodetype;
    struct symbol *s;
    struct ast *v;
};

struct symbol {
    char *name;
    double value;
    struct ast *func;
    struct symlist *syms;
};

#define NHASH 9997
struct symbol symtab[NHASH];

struct symbol *lookup(char*);

struct symlist {
    struct symbol *sym;
    struct symlist *next;
};

struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newnum(double d);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newcall(struct symbol *s, struct symlist *sl);
struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);
double eval(struct ast *);
void treefree(struct ast *);

#endif
