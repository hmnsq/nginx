
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

    אם (*root == sentinel) {
        node->parent = عدم;
        node->left = sentinel;
        node->right = sentinel;
        ngx_rbt_black(node);
        *root = node;

        החזר;
    }

    tree->insert(*root, node, sentinel);

    /* re-balance tree */

    כלעוד (node != *root && ngx_rbt_is_red(node->parent)) {

        אם (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;

            אם (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(node->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } אחרת {
                אם (node == node->parent->right) {
                    node = node->parent;
                    ngx_rbtree_left_rotate(root, sentinel, node);
                }

                ngx_rbt_black(node->parent);
                ngx_rbt_red(node->parent->parent);
                ngx_rbtree_right_rotate(root, sentinel, node->parent->parent);
            }

        } אחרת {
            temp = node->parent->parent->left;

            אם (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(node->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } אחרת {
                אם (node == node->parent->left) {
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

    עבור ( ;; ) {

        p = (node->key < temp->key) ? &temp->left : &temp->right;

        אם (*p == sentinel) {
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

    עבור ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_key_int_t) (node->key - temp->key) < 0)
            ? &temp->left : &temp->right;

        אם (*p == sentinel) {
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

    אם (node->left == sentinel) {
        temp = node->right;
        subst = node;

    } אחרת אם (node->right == sentinel) {
        temp = node->left;
        subst = node;

    } אחרת {
        subst = ngx_rbtree_min(node->right, sentinel);
        temp = subst->right;
    }

    אם (subst == *root) {
        *root = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        node->left = عدم;
        node->right = عدم;
        node->parent = عدم;
        node->key = 0;

        החזר;
    }

    red = ngx_rbt_is_red(subst);

    אם (subst == subst->parent->left) {
        subst->parent->left = temp;

    } אחרת {
        subst->parent->right = temp;
    }

    אם (subst == node) {

        temp->parent = subst->parent;

    } אחרת {

        אם (subst->parent == node) {
            temp->parent = subst;

        } אחרת {
            temp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        ngx_rbt_copy_color(subst, node);

        אם (node == *root) {
            *root = subst;

        } אחרת {
            אם (node == node->parent->left) {
                node->parent->left = subst;
            } אחרת {
                node->parent->right = subst;
            }
        }

        אם (subst->left != sentinel) {
            subst->left->parent = subst;
        }

        אם (subst->right != sentinel) {
            subst->right->parent = subst;
        }
    }

    /* DEBUG stuff */
    node->left = عدم;
    node->right = عدم;
    node->parent = عدم;
    node->key = 0;

    אם (red) {
        החזר;
    }

    /* a delete fixup */

    כלעוד (temp != *root && ngx_rbt_is_black(temp)) {

        אם (temp == temp->parent->left) {
            w = temp->parent->right;

            אם (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_left_rotate(root, sentinel, temp->parent);
                w = temp->parent->right;
            }

            אם (ngx_rbt_is_black(w->left) && ngx_rbt_is_black(w->right)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } אחרת {
                אם (ngx_rbt_is_black(w->right)) {
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

        } אחרת {
            w = temp->parent->left;

            אם (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_right_rotate(root, sentinel, temp->parent);
                w = temp->parent->left;
            }

            אם (ngx_rbt_is_black(w->left) && ngx_rbt_is_black(w->right)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } אחרת {
                אם (ngx_rbt_is_black(w->left)) {
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

    אם (temp->left != sentinel) {
        temp->left->parent = node;
    }

    temp->parent = node->parent;

    אם (node == *root) {
        *root = temp;

    } אחרת אם (node == node->parent->left) {
        node->parent->left = temp;

    } אחרת {
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

    אם (temp->right != sentinel) {
        temp->right->parent = node;
    }

    temp->parent = node->parent;

    אם (node == *root) {
        *root = temp;

    } אחרת אם (node == node->parent->right) {
        node->parent->right = temp;

    } אחרת {
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

    אם (node->right != sentinel) {
        החזר ngx_rbtree_min(node->right, sentinel);
    }

    root = tree->root;

    עבור ( ;; ) {
        parent = node->parent;

        אם (node == root) {
            החזר عدم;
        }

        אם (node == parent->left) {
            החזר parent;
        }

        node = parent;
    }
}
