#ifndef FB3_1_H
#define FB3_1_H

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

struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newnum(double d);
double eval(struct ast *);
void treefree(struct ast *);

#endif
