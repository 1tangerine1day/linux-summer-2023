#include "stree.h"

struct st_node *st_first(struct st_node *n)
{
    if (!st_left(n))
        return n;

    return st_first(st_left(n));
}

struct st_node *st_last(struct st_node *n)
{
    if (!st_right(n))
        return n;

    return st_last(st_right(n));
}

static inline void st_rotate_left(struct st_node *n)
{
    struct st_node *l = st_left(n), *p = st_parent(n);

    st_parent(l) = st_parent(n);
    st_left(n) = st_right(l);
    st_parent(n) = l;
    st_right(l) = n;

    if (p && st_left(p) == n)
        st_left(p) = l;
    else if (p)
        st_right(p) = l;

    if (st_left(n))
        st_lparent(n) = n;
}

static inline void st_rotate_right(struct st_node *n)
{
    struct st_node *r = st_right(n), *p = st_parent(n);

    st_parent(r) = st_parent(n);
    st_right(n) = st_left(r);
    st_parent(n) = r;
    st_left(r) = n;

    if (p && st_left(p) == n)
        st_left(p) = r;
    else if (p)
        st_right(p) = r;

    if (st_right(n))
        st_rparent(n) = n;
}

static inline int st_balance(struct st_node *n)
{
    int l = 0, r = 0;

    if (st_left(n))
        l = st_left(n)->hint + 1;

    if (st_right(n))
        r = st_right(n)->hint + 1;

    return l - r;
}

static inline int st_max_hint(struct st_node *n)
{
    int l = 0, r = 0;

    if (st_left(n))
        l = st_left(n)->hint + 1;

    if (st_right(n))
        r = st_right(n)->hint + 1;

    return l > r ? l : r;
}

static inline void st_update(struct st_node **root, struct st_node *n)
{
    if (!n)
        return;

    int b = st_balance(n);
    int prev_hint = n->hint;
    struct st_node *p = st_parent(n);

    if (b < -1) {
        /* leaning to the right */
        if (n == *root)
            *root = st_right(n);
        st_rotate_right(n);
    }

    else if (b > 1) {
        /* leaning to the left */
        if (n == *root)
            *root = st_left(n);
        st_rotate_left(n);
    }

    n->hint = st_max_hint(n);
    if (n->hint == 0 || n->hint != prev_hint)
        st_update(root, p);
}

/* The process of insertion is straightforward and follows the standard approach
 * used in any BST. After inserting a new node into the tree using conventional
 * BST insertion techniques, an update operation is invoked on the newly
 * inserted node.
 */
void st_insert(struct st_node **root,
               struct st_node *p,
               struct st_node *n,
               enum st_dir d)
{
    if (d == LEFT)
        st_left(p) = n;
    else
        st_right(p) = n;

    st_parent(n) = p;
    st_update(root, n);
}

static inline void st_replace_right(struct st_node *n, struct st_node *r)
{
    struct st_node *p = st_parent(n), *rp = st_parent(r);

    if (st_left(rp) == r) {
        st_left(rp) = st_right(r);
        if (st_right(r))
            st_rparent(r) = rp;
    }

    if (st_parent(rp) == n)
        st_parent(rp) = r;

    st_parent(r) = p;
    st_left(r) = st_left(n);

    if (st_right(n) != r) {
        st_right(r) = st_right(n);
        st_rparent(n) = r;
    }

    if (p && st_left(p) == n)
        st_left(p) = r;
    else if (p)
        st_right(p) = r;

    if (st_left(n))
        st_lparent(n) = r;
}

static inline void st_replace_left(struct st_node *n, struct st_node *l)
{
    struct st_node *p = st_parent(n), *lp = st_parent(l);

    if (st_right(lp) == l) {
        st_right(lp) = st_left(l);
        if (st_left(l))
            st_lparent(l) = lp;
    }

    if (st_parent(lp) == n)
        st_parent(lp) = l;

    st_parent(l) = p;
    st_right(l) = st_right(n);

    if (st_left(n) != l) {
        st_left(l) = st_left(n);
        st_lparent(n) = l;
    }

    if (p && st_left(p) == n)
        st_left(p) = l;
    else if (p)
        st_right(p) = l;

    if (st_right(n))
        st_rparent(n) = l;
}

/* The process of deletion in this tree structure is relatively more intricate,
 * although it shares similarities with deletion methods employed in other BST.
 * When removing a node, if the node to be deleted has a right child, the
 * deletion process entails replacing the node to be removed with the first node
 * encountered in the right subtree. Following this replacement, an update
 * operation is invoked on the right child of the newly inserted node.
 *
 * Similarly, if the node to be deleted does not have a right child, the
 * replacement process involves utilizing the first node found in the left
 * subtree. Subsequently, an update operation is called on the left child of th
 * replacement node.
 *
 * In scenarios where the node to be deleted has no children (neither left nor
 * right), it can be directly removed from the tree, and an update operation is
 * invoked on the parent node of the deleted node.
 */
void st_remove(struct st_node **root, struct st_node *del)
{
    if (st_right(del)) {
        struct st_node *least = st_first(st_right(del));
        if (del == *root)
            *root = least;

		/* AAAA */
		st_replace_right(del, least);
		/* BBBB */
		st_update(root, st_parent(least));
        return;
    }

    if (st_left(del)) {
        struct st_node *most = st_last(st_left(del));
        if (del == *root)
            *root = most;

		/* CCCC */
		st_replace_left(del, most);
		/* DDDD */
		st_update(root, st_parent(most));
        return;
    }

    if (del == *root) {
        *root = 0;
        return;
    }

    /* empty node */
    struct st_node *parent = st_parent(del);

    if (st_left(parent) == del)
        st_left(parent) = 0;
    else
        st_right(parent) = 0;

    /* EEEE */
	st_update(root, parent);
}
