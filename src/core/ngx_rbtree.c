
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

    အကယ်၍ (*root == sentinel) {
        node->parent = NULL;
        node->left = sentinel;
        node->right = sentinel;
        ngx_rbt_black(node);
        *root = node;

        ပြန်ပေး;
    }

    tree->insert(*root, node, sentinel);

    /* re-balance tree */

    နေစဉ် (node != *root && ngx_rbt_is_red(node->parent)) {

        အကယ်၍ (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;

            အကယ်၍ (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(node->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } မဟုတ်ပါက {
                အကယ်၍ (node == node->parent->right) {
                    node = node->parent;
                    ngx_rbtree_left_rotate(root, sentinel, node);
                }

                ngx_rbt_black(node->parent);
                ngx_rbt_red(node->parent->parent);
                ngx_rbtree_right_rotate(root, sentinel, node->parent->parent);
            }

        } မဟုတ်ပါက {
            temp = node->parent->parent->left;

            အကယ်၍ (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(node->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } မဟုတ်ပါက {
                အကယ်၍ (node == node->parent->left) {
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

    အတွက် ( ;; ) {

        p = (node->key < temp->key) ? &temp->left : &temp->right;

        အကယ်၍ (*p == sentinel) {
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

    အတွက် ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_key_int_t) (node->key - temp->key) < 0)
            ? &temp->left : &temp->right;

        အကယ်၍ (*p == sentinel) {
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

    အကယ်၍ (node->left == sentinel) {
        temp = node->right;
        subst = node;

    } မဟုတ်ပါက အကယ်၍ (node->right == sentinel) {
        temp = node->left;
        subst = node;

    } မဟုတ်ပါက {
        subst = ngx_rbtree_min(node->right, sentinel);
        temp = subst->right;
    }

    အကယ်၍ (subst == *root) {
        *root = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->key = 0;

        ပြန်ပေး;
    }

    red = ngx_rbt_is_red(subst);

    အကယ်၍ (subst == subst->parent->left) {
        subst->parent->left = temp;

    } မဟုတ်ပါက {
        subst->parent->right = temp;
    }

    အကယ်၍ (subst == node) {

        temp->parent = subst->parent;

    } မဟုတ်ပါက {

        အကယ်၍ (subst->parent == node) {
            temp->parent = subst;

        } မဟုတ်ပါက {
            temp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        ngx_rbt_copy_color(subst, node);

        အကယ်၍ (node == *root) {
            *root = subst;

        } မဟုတ်ပါက {
            အကယ်၍ (node == node->parent->left) {
                node->parent->left = subst;
            } မဟုတ်ပါက {
                node->parent->right = subst;
            }
        }

        အကယ်၍ (subst->left != sentinel) {
            subst->left->parent = subst;
        }

        အကယ်၍ (subst->right != sentinel) {
            subst->right->parent = subst;
        }
    }

    /* DEBUG stuff */
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = 0;

    အကယ်၍ (red) {
        ပြန်ပေး;
    }

    /* a delete fixup */

    နေစဉ် (temp != *root && ngx_rbt_is_black(temp)) {

        အကယ်၍ (temp == temp->parent->left) {
            w = temp->parent->right;

            အကယ်၍ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_left_rotate(root, sentinel, temp->parent);
                w = temp->parent->right;
            }

            အကယ်၍ (ngx_rbt_is_black(w->left) && ngx_rbt_is_black(w->right)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } မဟုတ်ပါက {
                အကယ်၍ (ngx_rbt_is_black(w->right)) {
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

        } မဟုတ်ပါက {
            w = temp->parent->left;

            အကယ်၍ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_right_rotate(root, sentinel, temp->parent);
                w = temp->parent->left;
            }

            အကယ်၍ (ngx_rbt_is_black(w->left) && ngx_rbt_is_black(w->right)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } မဟုတ်ပါက {
                အကယ်၍ (ngx_rbt_is_black(w->left)) {
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

    အကယ်၍ (temp->left != sentinel) {
        temp->left->parent = node;
    }

    temp->parent = node->parent;

    အကယ်၍ (node == *root) {
        *root = temp;

    } မဟုတ်ပါက အကယ်၍ (node == node->parent->left) {
        node->parent->left = temp;

    } မဟုတ်ပါက {
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

    အကယ်၍ (temp->right != sentinel) {
        temp->right->parent = node;
    }

    temp->parent = node->parent;

    အကယ်၍ (node == *root) {
        *root = temp;

    } မဟုတ်ပါက အကယ်၍ (node == node->parent->right) {
        node->parent->right = temp;

    } မဟုတ်ပါက {
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

    အကယ်၍ (node->right != sentinel) {
        ပြန်ပေး ngx_rbtree_min(node->right, sentinel);
    }

    root = tree->root;

    အတွက် ( ;; ) {
        parent = node->parent;

        အကယ်၍ (node == root) {
            ပြန်ပေး NULL;
        }

        အကယ်၍ (node == parent->left) {
            ပြန်ပေး parent;
        }

        node = parent;
    }
}
