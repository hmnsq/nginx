
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

    ပေႃး (*root == sentinel) {
        node->parent = NULL;
        node->left = sentinel;
        node->right = sentinel;
        ngx_rbt_black(node);
        *root = node;

        ပွၵ်ႈ;
    }

    tree->insert(*root, node, sentinel);

    /* re-balance tree */

    ၶၢဝ်း (node != *root && ngx_rbt_is_red(node->parent)) {

        ပေႃး (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;

            ပေႃး (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(node->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } ပေႃးဢမ်ႇ {
                ပေႃး (node == node->parent->right) {
                    node = node->parent;
                    ngx_rbtree_left_rotate(root, sentinel, node);
                }

                ngx_rbt_black(node->parent);
                ngx_rbt_red(node->parent->parent);
                ngx_rbtree_right_rotate(root, sentinel, node->parent->parent);
            }

        } ပေႃးဢမ်ႇ {
            temp = node->parent->parent->left;

            ပေႃး (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(node->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } ပေႃးဢမ်ႇ {
                ပေႃး (node == node->parent->left) {
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

        ပေႃး (*p == sentinel) {
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

        ပေႃး (*p == sentinel) {
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

    ပေႃး (node->left == sentinel) {
        temp = node->right;
        subst = node;

    } ပေႃးဢမ်ႇ ပေႃး (node->right == sentinel) {
        temp = node->left;
        subst = node;

    } ပေႃးဢမ်ႇ {
        subst = ngx_rbtree_min(node->right, sentinel);
        temp = subst->right;
    }

    ပေႃး (subst == *root) {
        *root = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->key = 0;

        ပွၵ်ႈ;
    }

    red = ngx_rbt_is_red(subst);

    ပေႃး (subst == subst->parent->left) {
        subst->parent->left = temp;

    } ပေႃးဢမ်ႇ {
        subst->parent->right = temp;
    }

    ပေႃး (subst == node) {

        temp->parent = subst->parent;

    } ပေႃးဢမ်ႇ {

        ပေႃး (subst->parent == node) {
            temp->parent = subst;

        } ပေႃးဢမ်ႇ {
            temp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        ngx_rbt_copy_color(subst, node);

        ပေႃး (node == *root) {
            *root = subst;

        } ပေႃးဢမ်ႇ {
            ပေႃး (node == node->parent->left) {
                node->parent->left = subst;
            } ပေႃးဢမ်ႇ {
                node->parent->right = subst;
            }
        }

        ပေႃး (subst->left != sentinel) {
            subst->left->parent = subst;
        }

        ပေႃး (subst->right != sentinel) {
            subst->right->parent = subst;
        }
    }

    /* DEBUG stuff */
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = 0;

    ပေႃး (red) {
        ပွၵ်ႈ;
    }

    /* a delete fixup */

    ၶၢဝ်း (temp != *root && ngx_rbt_is_black(temp)) {

        ပေႃး (temp == temp->parent->left) {
            w = temp->parent->right;

            ပေႃး (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_left_rotate(root, sentinel, temp->parent);
                w = temp->parent->right;
            }

            ပေႃး (ngx_rbt_is_black(w->left) && ngx_rbt_is_black(w->right)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } ပေႃးဢမ်ႇ {
                ပေႃး (ngx_rbt_is_black(w->right)) {
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

        } ပေႃးဢမ်ႇ {
            w = temp->parent->left;

            ပေႃး (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_right_rotate(root, sentinel, temp->parent);
                w = temp->parent->left;
            }

            ပေႃး (ngx_rbt_is_black(w->left) && ngx_rbt_is_black(w->right)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } ပေႃးဢမ်ႇ {
                ပေႃး (ngx_rbt_is_black(w->left)) {
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

    ပေႃး (temp->left != sentinel) {
        temp->left->parent = node;
    }

    temp->parent = node->parent;

    ပေႃး (node == *root) {
        *root = temp;

    } ပေႃးဢမ်ႇ ပေႃး (node == node->parent->left) {
        node->parent->left = temp;

    } ပေႃးဢမ်ႇ {
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

    ပေႃး (temp->right != sentinel) {
        temp->right->parent = node;
    }

    temp->parent = node->parent;

    ပေႃး (node == *root) {
        *root = temp;

    } ပေႃးဢမ်ႇ ပေႃး (node == node->parent->right) {
        node->parent->right = temp;

    } ပေႃးဢမ်ႇ {
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

    ပေႃး (node->right != sentinel) {
        ပွၵ်ႈ ngx_rbtree_min(node->right, sentinel);
    }

    root = tree->root;

    for ( ;; ) {
        parent = node->parent;

        ပေႃး (node == root) {
            ပွၵ်ႈ NULL;
        }

        ပေႃး (node == parent->left) {
            ပွၵ်ႈ parent;
        }

        node = parent;
    }
}
