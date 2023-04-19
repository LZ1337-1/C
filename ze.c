#include <stdio.h>
#include <stdlib.h>

// Définition du type expr_t
typedef enum
{
    CONST,
    VAR,
    UNARY,
    BINARY
} expr_type;

typedef struct expr_t
{
    expr_type type;
    struct expr_t *parent;
    union
    {
        int i;
        char x;
        struct
        {
            struct expr_t *se;
            int (*f)(int);
        } unary;
        struct
        {
            struct expr_t *se1;
            struct expr_t *se2;
            int (*f)(int, int);
        } binary;
    } value;
} expr_t;

// Constructeurs

// Constructeur abstrait
expr_t *expression()
{
    expr_t *e = (expr_t *)malloc(sizeof(expr_t));
    e->parent = NULL;
    return e;
}

// Constructeur pour une constante
expr_t *constant(int i)
{
    expr_t *e = expression();
    e->type = CONST;
    e->value.i = i;
    return e;
}

// Constructeur pour une variable
expr_t *variable(char x)
{
    expr_t *e = expression();
    e->type = VAR;
    e->value.x = x;
    return e;
}

// Constructeur pour une opération unaire
expr_t *unary_expr(expr_t *se, int (*f)(int))
{
    if (se->parent != NULL)
    {
        fprintf(stderr, "Error: expression already has a parent.\n");
        exit(EXIT_FAILURE);
    }

    expr_t *e = expression();
    e->type = UNARY;
    e->value.unary.se = se;
    e->value.unary.f = f;
    se->parent = e;
    return e;
}

// Constructeur pour une opération binaire
expr_t *binary_expr(expr_t *se1, expr_t *se2, int (*f)(int, int))
{
    if (se1->parent != NULL || se2->parent != NULL)
    {
        fprintf(stderr, "Error: one or both expressions already have a parent.\n");
        exit(EXIT_FAILURE);
    }

    expr_t *e = expression();
    e->type = BINARY;
    e->value.binary.se1 = se1;
    e->value.binary.se2 = se2;
    e->value.binary.f = f;
    se1->parent = e;
    se2->parent = e;
    return e;
}

// Destructeur
void delete_expr(expr_t *e)
{
    if (e->type == UNARY)
    {
        delete_expr(e->value.unary.se);
    }
    else if (e->type == BINARY)
    {
        delete_expr(e->value.binary.se1);
        delete_expr(e->value.binary.se2);
    }
    free(e);
}

// Evaluateur
int eval_expr(expr_t *e, int *env)
{
    if (e->type == CONST)
    {
        return e->value.i;
    }
    else if (e->type == VAR)
    {
        return env[e->value.x];
    }
    else if (e->type == UNARY)
    {
        return e->value.unary.f(eval_expr(e->value.unary.se, env));
    }
    else if (e->type == BINARY)
    {
        return e->value.binary.f(eval_expr(e->value.binary.se1, env), eval_expr(e->value.binary.se2, env));
    }
    return 0;
}

// Fonctions pour les opérations
int neg(int a) { return -a; }
int add(int a, int b) { return a + b; }
int mult(int a, int b) { return a * b; }

int main()
{
    // Expression: -(3 * x + y)
    expr_t *e =
        unary_expr(
            binary_expr(
                binary_expr(
                    constant(3),
                    variable('x'),
                    mult),
                variable('y'), add),
            neg);

    int env[256];
    int x, y;
    for (x = -2; x <= 3; x++)
        for (y = -2; y <= 3; y++)
        {
            env['x'] = x;
            env['y'] = y;
            printf("(x = %d, y = %d) -> %d\n", x, y, eval_expr(e, env));
        }

    return EXIT_SUCCESS;
}