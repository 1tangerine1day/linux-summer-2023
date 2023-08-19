/* Test program */
#include <assert.h>
#include <stddef.h> /* offsetof */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rbtree.h"

#define container_of(ptr, type, member) \
    ((type *) ((char *) (ptr) - (offsetof(type, member))))

#define treeint_entry(ptr) container_of(ptr, struct treeint, rb_n)

struct treeint {
    int value;
    struct rb_node rb_n;
};

static struct rb_root *tree;

int treeint_init()
{
    tree = calloc(sizeof(struct rb_root), 1);
    assert(tree);
    return 0;
}

static void __treeint_destroy(struct rb_node *n)
{
    if (rb_left(n))
        __treeint_destroy(rb_left(n));

    if (rb_right(n))
        __treeint_destroy(rb_right(n));

    struct treeint *i = treeint_entry(n);
    free(i);
}

int treeint_destroy()
{
    assert(tree);
    if (tree)
        __treeint_destroy(tree);

    free(tree);
    return 0;
}

struct treeint *treeint_insert(int a)
{
    struct treeint *i = calloc(sizeof(struct treeint), 1);
    if (rb_root(tree))
        rb_insert(rb_root(tree), &i->rb_n);
    else
        rb_root(tree) = &i->rb_n;

    i->value = a;
    return i;
}

struct treeint *treeint_find(int a)
{
    struct rb_node *n = rb_root(tree);
    while (n) {
        struct treeint *t = treeint_entry(n);
        if (a == t->value)
            return t;

        if (a < t->value)
            n = rb_left(n);
        else if (a > t->value)
            n = rb_right(n);
    }

    return 0;
}

int treeint_remove(int a)
{
    struct treeint *n = treeint_find(a);
    if (!n)
        return -1;

    rb_remove(rb_root(tree), &n->rb_n);
    free(n);
    return 0;
}

/* ascending order */
static void __treeint_dump(struct rb_node *n, int depth)
{
    if (!n)
        return;

    __treeint_dump(/* FFFF */rb_left(n), depth + 1);

    struct treeint *v = treeint_entry(n);
    printf("%d\n", v->value);

    __treeint_dump(/* GGGG */rb_right(n), depth + 1);
}

void treeint_dump()
{
    __treeint_dump(rb_root(tree), 0);
}

int main()
{
    srand(time(0));

    treeint_init();

    for (int i = 0; i < 100; ++i)
        treeint_insert(rand() % 99);

    printf("[ After insertions ]\n");
    treeint_dump();

    printf("Removing...\n");
    for (int i = 0; i < 100; ++i) {
        int v = rand() % 99;
        printf("%2d  ", v);
        if ((i + 1) % 10 == 0)
            printf("\n");
        treeint_remove(v);
    }
    printf("\n");

    printf("[ After removals ]\n");
    treeint_dump();

    treeint_destroy();

    return 0;
}

