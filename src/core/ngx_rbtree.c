
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


ساكن ngx_inline فراغ ngx_rbtree_left_rotate(ngx_rbtree_node_t **root,
    ngx_rbtree_node_t *sentinel, ngx_rbtree_node_t *node);
ساكن ngx_inline فراغ ngx_rbtree_right_rotate(ngx_rbtree_node_t **root,
    ngx_rbtree_node_t *sentinel, ngx_rbtree_node_t *node);


فراغ
ngx_rbtree_insert(ngx_rbtree_t *tree, ngx_rbtree_node_t *node)
{
    ngx_rbtree_node_t  **root, *temp, *sentinel;

    /* a binary tree insert */

    root = &tree->root;
    sentinel = tree->sentinel;

    ܐܢ (*root == sentinel) {
        node->parent = عدم;
        node->left = sentinel;
        node->right = sentinel;
        ngx_rbt_black(node);
        *root = node;

        ܦܢܐ;
    }

    tree->insert(*root, node, sentinel);

    /* re-balance tree */

    ܟܡܐ (node != *root && ngx_rbt_is_red(node->parent)) {

        ܐܢ (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;

            ܐܢ (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(node->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } ܐܚܪܢܐ {
                ܐܢ (node == node->parent->right) {
                    node = node->parent;
                    ngx_rbtree_left_rotate(root, sentinel, node);
                }

                ngx_rbt_black(node->parent);
                ngx_rbt_red(node->parent->parent);
                ngx_rbtree_right_rotate(root, sentinel, node->parent->parent);
            }

        } ܐܚܪܢܐ {
            temp = node->parent->parent->left;

            ܐܢ (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(node->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } ܐܚܪܢܐ {
                ܐܢ (node == node->parent->left) {
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


فراغ
ngx_rbtree_insert_value(ngx_rbtree_node_t *temp, ngx_rbtree_node_t *node,
    ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t  **p;

    لكل ( ;; ) {

        p = (node->key < temp->key) ? &temp->left : &temp->right;

        ܐܢ (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}


فراغ
ngx_rbtree_insert_timer_value(ngx_rbtree_node_t *temp, ngx_rbtree_node_t *node,
    ngx_rbtree_node_t *sentinel)
{
    ngx_rbtree_node_t  **p;

    لكل ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_key_int_t) (node->key - temp->key) < 0)
            ? &temp->left : &temp->right;

        ܐܢ (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}


فراغ
ngx_rbtree_delete(ngx_rbtree_t *tree, ngx_rbtree_node_t *node)
{
    ngx_uint_t           red;
    ngx_rbtree_node_t  **root, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    root = &tree->root;
    sentinel = tree->sentinel;

    ܐܢ (node->left == sentinel) {
        temp = node->right;
        subst = node;

    } ܐܚܪܢܐ ܐܢ (node->right == sentinel) {
        temp = node->left;
        subst = node;

    } ܐܚܪܢܐ {
        subst = ngx_rbtree_min(node->right, sentinel);
        temp = subst->right;
    }

    ܐܢ (subst == *root) {
        *root = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        node->left = عدم;
        node->right = عدم;
        node->parent = عدم;
        node->key = 0;

        ܦܢܐ;
    }

    red = ngx_rbt_is_red(subst);

    ܐܢ (subst == subst->parent->left) {
        subst->parent->left = temp;

    } ܐܚܪܢܐ {
        subst->parent->right = temp;
    }

    ܐܢ (subst == node) {

        temp->parent = subst->parent;

    } ܐܚܪܢܐ {

        ܐܢ (subst->parent == node) {
            temp->parent = subst;

        } ܐܚܪܢܐ {
            temp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        ngx_rbt_copy_color(subst, node);

        ܐܢ (node == *root) {
            *root = subst;

        } ܐܚܪܢܐ {
            ܐܢ (node == node->parent->left) {
                node->parent->left = subst;
            } ܐܚܪܢܐ {
                node->parent->right = subst;
            }
        }

        ܐܢ (subst->left != sentinel) {
            subst->left->parent = subst;
        }

        ܐܢ (subst->right != sentinel) {
            subst->right->parent = subst;
        }
    }

    /* DEBUG stuff */
    node->left = عدم;
    node->right = عدم;
    node->parent = عدم;
    node->key = 0;

    ܐܢ (red) {
        ܦܢܐ;
    }

    /* a delete fixup */

    ܟܡܐ (temp != *root && ngx_rbt_is_black(temp)) {

        ܐܢ (temp == temp->parent->left) {
            w = temp->parent->right;

            ܐܢ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_left_rotate(root, sentinel, temp->parent);
                w = temp->parent->right;
            }

            ܐܢ (ngx_rbt_is_black(w->left) && ngx_rbt_is_black(w->right)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } ܐܚܪܢܐ {
                ܐܢ (ngx_rbt_is_black(w->right)) {
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

        } ܐܚܪܢܐ {
            w = temp->parent->left;

            ܐܢ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_right_rotate(root, sentinel, temp->parent);
                w = temp->parent->left;
            }

            ܐܢ (ngx_rbt_is_black(w->left) && ngx_rbt_is_black(w->right)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } ܐܚܪܢܐ {
                ܐܢ (ngx_rbt_is_black(w->left)) {
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


ساكن ngx_inline فراغ
ngx_rbtree_left_rotate(ngx_rbtree_node_t **root, ngx_rbtree_node_t *sentinel,
    ngx_rbtree_node_t *node)
{
    ngx_rbtree_node_t  *temp;

    temp = node->right;
    node->right = temp->left;

    ܐܢ (temp->left != sentinel) {
        temp->left->parent = node;
    }

    temp->parent = node->parent;

    ܐܢ (node == *root) {
        *root = temp;

    } ܐܚܪܢܐ ܐܢ (node == node->parent->left) {
        node->parent->left = temp;

    } ܐܚܪܢܐ {
        node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;
}


ساكن ngx_inline فراغ
ngx_rbtree_right_rotate(ngx_rbtree_node_t **root, ngx_rbtree_node_t *sentinel,
    ngx_rbtree_node_t *node)
{
    ngx_rbtree_node_t  *temp;

    temp = node->left;
    node->left = temp->right;

    ܐܢ (temp->right != sentinel) {
        temp->right->parent = node;
    }

    temp->parent = node->parent;

    ܐܢ (node == *root) {
        *root = temp;

    } ܐܚܪܢܐ ܐܢ (node == node->parent->right) {
        node->parent->right = temp;

    } ܐܚܪܢܐ {
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

    ܐܢ (node->right != sentinel) {
        ܦܢܐ ngx_rbtree_min(node->right, sentinel);
    }

    root = tree->root;

    لكل ( ;; ) {
        parent = node->parent;

        ܐܢ (node == root) {
            ܦܢܐ عدم;
        }

        ܐܢ (node == parent->left) {
            ܦܢܐ parent;
        }

        node = parent;
    }
}
