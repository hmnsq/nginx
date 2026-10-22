
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


static ngx_inline void ngx_rbtree_left_rotate(ngx_rbtree_node_t **root,
    ngx_rbtree_node_t *sentinel, ngx_rbtree_node_t *node);
static ngx_inline void ngx_rbtree_right_rotate(ngx_rbtree_node_t **root,
    ngx_rbtree_node_t *sentinel, ngx_rbtree_node_t *node);


void
ngx_rbtree_insert(ngx_rbtree_t *tree, ngx_rbtree_node_t *node)
{
    ngx_rbtree_node_t  **root, *temp, *sentinel;

    /* a binary tree insert */

    root = &tree->root;
    sentinel = tree->sentinel;

    ᯀᯮᯝ (*root == sentinel) {
        node->parent = NULL;
        node->left = sentinel;
        node->right = sentinel;
        ngx_rbt_black(node);
        *root = node;

        ᯀᯮᯞ;
    }

    tree->insert(*root, node, sentinel);

    /* re-balance tree */

    ᯀᯬᯝ (node != *root && ngx_rbt_is_red(node->parent)) {

        ᯀᯮᯝ (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;

            ᯀᯮᯝ (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(node->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } ᯀᯮᯝᯂᯪ {
                ᯀᯮᯝ (node == node->parent->right) {
                    node = node->parent;
                    ngx_rbtree_left_rotate(root, sentinel, node);
                }

                ngx_rbt_black(node->parent);
                ngx_rbt_red(node->parent->parent);
                ngx_rbtree_right_rotate(root, sentinel, node->parent->parent);
            }

        } ᯀᯮᯝᯂᯪ {
            temp = node->parent->parent->left;

            ᯀᯮᯝ (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(node->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } ᯀᯮᯝᯂᯪ {
                ᯀᯮᯝ (node == node->parent->left) {
                    node = node->parent;
                    ngx_rbtree_right_rotate(root, sentinel, node);
                }

                ngx_rbt_black(node->parent);
                ngx_rbt_red(node->parent->parent);
                ngx_rbtree_left_rotate(root, sentinel, node->parent->parent);
            }
        }
    }

    ngx_rbt_black(*root);
}


void
ngx_rbtree_insert_value(ngx_rbtree_node_t *temp, ngx_rbtree_node_t *node,
    ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t  **p;

    for ( ;; ) {

        p = (node->key < temp->key) ? &temp->left : &temp->right;

        ᯀᯮᯝ (*p == sentinel) {
            bha_nga;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}


void
ngx_rbtree_insert_timer_value(ngx_rbtree_node_t *temp, ngx_rbtree_node_t *node,
    ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t  **p;

    for ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_key_int_t) (node->key - temp->key) < 0)
            ? &temp->left : &temp->right;

        ᯀᯮᯝ (*p == sentinel) {
            bha_nga;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}


void
ngx_rbtree_delete(ngx_rbtree_t *tree, ngx_rbtree_node_t *node)
{
    ngx_uint_t           red;
    ngx_rbtree_node_t  **root, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    root = &tree->root;
    sentinel = tree->sentinel;

    ᯀᯮᯝ (node->left == sentinel) {
        temp = node->right;
        subst = node;

    } ᯀᯮᯝᯂᯪ ᯀᯮᯝ (node->right == sentinel) {
        temp = node->left;
        subst = node;

    } ᯀᯮᯝᯂᯪ {
        subst = ngx_rbtree_min(node->right, sentinel);
        temp = subst->right;
    }

    ᯀᯮᯝ (subst == *root) {
        *root = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->key = 0;

        ᯀᯮᯞ;
    }

    red = ngx_rbt_is_red(subst);

    ᯀᯮᯝ (subst == subst->parent->left) {
        subst->parent->left = temp;

    } ᯀᯮᯝᯂᯪ {
        subst->parent->right = temp;
    }

    ᯀᯮᯝ (subst == node) {

        temp->parent = subst->parent;

    } ᯀᯮᯝᯂᯪ {

        ᯀᯮᯝ (subst->parent == node) {
            temp->parent = subst;

        } ᯀᯮᯝᯂᯪ {
            temp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        ngx_rbt_copy_color(subst, node);

        ᯀᯮᯝ (node == *root) {
            *root = subst;

        } ᯀᯮᯝᯂᯪ {
            ᯀᯮᯝ (node == node->parent->left) {
                node->parent->left = subst;
            } ᯀᯮᯝᯂᯪ {
                node->parent->right = subst;
            }
        }

        ᯀᯮᯝ (subst->left != sentinel) {
            subst->left->parent = subst;
        }

        ᯀᯮᯝ (subst->right != sentinel) {
            subst->right->parent = subst;
        }
    }

    /* DEBUG stuff */
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = 0;

    ᯀᯮᯝ (red) {
        ᯀᯮᯞ;
    }

    /* a delete fixup */

    ᯀᯬᯝ (temp != *root && ngx_rbt_is_black(temp)) {

        ᯀᯮᯝ (temp == temp->parent->left) {
            w = temp->parent->right;

            ᯀᯮᯝ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_left_rotate(root, sentinel, temp->parent);
                w = temp->parent->right;
            }

            ᯀᯮᯝ (ngx_rbt_is_black(w->left) && ngx_rbt_is_black(w->right)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } ᯀᯮᯝᯂᯪ {
                ᯀᯮᯝ (ngx_rbt_is_black(w->right)) {
                    ngx_rbt_black(w->left);
                    ngx_rbt_red(w);
                    ngx_rbtree_right_rotate(root, sentinel, w);
                    w = temp->parent->right;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->right);
                ngx_rbtree_left_rotate(root, sentinel, temp->parent);
                temp = *root;
            }

        } ᯀᯮᯝᯂᯪ {
            w = temp->parent->left;

            ᯀᯮᯝ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_right_rotate(root, sentinel, temp->parent);
                w = temp->parent->left;
            }

            ᯀᯮᯝ (ngx_rbt_is_black(w->left) && ngx_rbt_is_black(w->right)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } ᯀᯮᯝᯂᯪ {
                ᯀᯮᯝ (ngx_rbt_is_black(w->left)) {
                    ngx_rbt_black(w->right);
                    ngx_rbt_red(w);
                    ngx_rbtree_left_rotate(root, sentinel, w);
                    w = temp->parent->left;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->left);
                ngx_rbtree_right_rotate(root, sentinel, temp->parent);
                temp = *root;
            }
        }
    }

    ngx_rbt_black(temp);
}


static ngx_inline void
ngx_rbtree_left_rotate(ngx_rbtree_node_t **root, ngx_rbtree_node_t *sentinel,
    ngx_rbtree_node_t *node)
{
    ngx_rbtree_node_t  *temp;

    temp = node->right;
    node->right = temp->left;

    ᯀᯮᯝ (temp->left != sentinel) {
        temp->left->parent = node;
    }

    temp->parent = node->parent;

    ᯀᯮᯝ (node == *root) {
        *root = temp;

    } ᯀᯮᯝᯂᯪ ᯀᯮᯝ (node == node->parent->left) {
        node->parent->left = temp;

    } ᯀᯮᯝᯂᯪ {
        node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;
}


static ngx_inline void
ngx_rbtree_right_rotate(ngx_rbtree_node_t **root, ngx_rbtree_node_t *sentinel,
    ngx_rbtree_node_t *node)
{
    ngx_rbtree_node_t  *temp;

    temp = node->left;
    node->left = temp->right;

    ᯀᯮᯝ (temp->right != sentinel) {
        temp->right->parent = node;
    }

    temp->parent = node->parent;

    ᯀᯮᯝ (node == *root) {
        *root = temp;

    } ᯀᯮᯝᯂᯪ ᯀᯮᯝ (node == node->parent->right) {
        node->parent->right = temp;

    } ᯀᯮᯝᯂᯪ {
        node->parent->left = temp;
    }

    temp->right = node;
    node->parent = temp;
}


ngx_rbtree_node_t *
ngx_rbtree_next(ngx_rbtree_t *tree, ngx_rbtree_node_t *node)
{
    ngx_rbtree_node_t  *root, *sentinel, *parent;

    sentinel = tree->sentinel;

    ᯀᯮᯝ (node->right != sentinel) {
        ᯀᯮᯞ ngx_rbtree_min(node->right, sentinel);
    }

    root = tree->root;

    for ( ;; ) {
        parent = node->parent;

        ᯀᯮᯝ (node == root) {
            ᯀᯮᯞ NULL;
        }

        ᯀᯮᯝ (node == parent->left) {
            ᯀᯮᯞ parent;
        }

        node = parent;
    }
}
