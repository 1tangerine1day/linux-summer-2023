#ifndef _STREE_H
#define _STREE_H
/*
 * S-Tree: A self-balancing binary search tree.
 *
 * AVL-trees promise a close-to-optimal tree layout for lookup, but they
 * consume a significant amount of memory and require relatively slow
 * balancing operations. Red-black trees offer quicker manipulation with
 * a slightly less optimal tree layout, and the proposed S-Tree offers
 * fast insertion and deletion by balancing trees during lookup.
 *
 * S-trees rely on four fundamental Binary Search Tree (BST) operations:
 * rotate_left, rotate_right, replace_right, and replace_left. The latter
 * two, replace_right and replace_left, are exclusively employed during node
 * removal, following the conventional BST approach. They identify the
 * next/last node in the right/left subtree, respectively, and perform the
 * substitution of the node scheduled for deletion with the identified node.
 *
 * In contrast, rotate_left and rotate_right are integral to a dedicated update
 * phase aimed at rebalancing the tree. This update phase follows both insert
 * and remove phases in the current implementation. Nonetheless, it is
 * theoretically possible to have arbitrary sequences comprising insert,
 * remove, lookup, and update operations. Notably, the frequency of updates
 * directly influences the extent to which the tree layout approaches
 * optimality. However, it is important to consider that each update operation
 * incurs a certain time penalty.
 *
 * The update function exhibits a relatively straightforward process: When a
 * specific node leans to the right or left beyond a defined threshold, a left
 * or right rotation is performed on the node, respectively. Concurrently, the
 * node's hint is consistently updated. Additionally, if the node's hint becomes
 * zero or experiences a change compared to its previous state during the
 * update, modifications are made to the node's parent, as it existed before
 * these update operations.
 */

/* S-Tree uses hints to decide whether to perform a balancing operation or not.
 * Hints are similar to AVL-trees' height property, but they are not
 * required to be absolutely accurate. A hint provides an approximation
 * of the longest chain of nodes under the node to which the hint is attached.
 */

struct st_node {
    short hint;
    struct st_node *parent;
    struct st_node *left, *right;
};

struct st_root {
    struct st_node *root;
};

enum st_dir { LEFT, RIGHT };

#define st_root(r) (r->root)
#define st_left(n) (n->left)
#define st_right(n) (n->right)
#define st_rparent(n) (st_right(n)->parent)
#define st_lparent(n) (st_left(n)->parent)
#define st_parent(n) (n->parent)

struct st_node *st_first(struct st_node *n);

struct st_node *st_last(struct st_node *n);

/* The process of insertion is straightforward and follows the standard approach
 * used in any BST. After inserting a new node into the tree using conventional
 * BST insertion techniques, an update operation is invoked on the newly
 * inserted node.
 */
void st_insert(struct st_node **root,
               struct st_node *p,
               struct st_node *n,
               enum st_dir d);

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
void st_remove(struct st_node **root, struct st_node *del);

#endif /* STREE_H */
