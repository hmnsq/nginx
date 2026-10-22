
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


ساكن ngx_inline فراغ ngx_rbtree_هغو_rotate(ngx_rbtree_كلي_t **سيو,
    ngx_rbtree_كلي_t *sentinel, ngx_rbtree_كلي_t *كلي);
ساكن ngx_inline فراغ ngx_rbtree_دام_rotate(ngx_rbtree_كلي_t **سيو,
    ngx_rbtree_كلي_t *sentinel, ngx_rbtree_كلي_t *كلي);


فراغ
ngx_rbtree_سك(ngx_rbtree_t *بشيا, ngx_rbtree_كلي_t *كلي)
{
    ngx_rbtree_كلي_t  **سيو, *temp, *sentinel;

    /* a binary tree insert */

    سيو = &بشيا->سيو;
    sentinel = بشيا->sentinel;

    إذا (*سيو == sentinel) {
        كلي->parent = عدم;
        كلي->هغو = sentinel;
        كلي->دام = sentinel;
        ngx_rbt_black(كلي);
        *سيو = كلي;

        أرجع;
    }

    بشيا->سك(*سيو, كلي, sentinel);

    /* re-balance tree */

    طالما (كلي != *سيو && ngx_rbt_is_red(كلي->parent)) {

        إذا (كلي->parent == كلي->parent->parent->هغو) {
            temp = كلي->parent->parent->دام;

            إذا (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(كلي->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(كلي->parent->parent);
                كلي = كلي->parent->parent;

            } وإلا {
                إذا (كلي == كلي->parent->دام) {
                    كلي = كلي->parent;
                    ngx_rbtree_هغو_rotate(سيو, sentinel, كلي);
                }

                ngx_rbt_black(كلي->parent);
                ngx_rbt_red(كلي->parent->parent);
                ngx_rbtree_دام_rotate(سيو, sentinel, كلي->parent->parent);
            }

        } وإلا {
            temp = كلي->parent->parent->هغو;

            إذا (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(كلي->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(كلي->parent->parent);
                كلي = كلي->parent->parent;

            } وإلا {
                إذا (كلي == كلي->parent->هغو) {
                    كلي = كلي->parent;
                    ngx_rbtree_دام_rotate(سيو, sentinel, كلي);
                }

                ngx_rbt_black(كلي->parent);
                ngx_rbt_red(كلي->parent->parent);
                ngx_rbtree_هغو_rotate(سيو, sentinel, كلي->parent->parent);
            }
        }
    }

    ngx_rbt_black(*سيو);
}


فراغ
ngx_rbtree_سك_درج(ngx_rbtree_كلي_t *temp, ngx_rbtree_كلي_t *كلي,
    ngx_rbtree_كلي_t *sentinel)
{
    ngx_rbtree_كلي_t  **p;

    لكل ( ;; ) {

        p = (كلي->مبودي < temp->مبودي) ? &temp->هغو : &temp->دام;

        إذا (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = كلي;
    كلي->parent = temp;
    كلي->هغو = sentinel;
    كلي->دام = sentinel;
    ngx_rbt_red(كلي);
}


فراغ
ngx_rbtree_سك_timer_درج(ngx_rbtree_كلي_t *temp, ngx_rbtree_كلي_t *كلي,
    ngx_rbtree_كلي_t *sentinel)
{
    ngx_rbtree_كلي_t  **p;

    لكل ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_مبودي_int_t) (كلي->مبودي - temp->مبودي) < 0)
            ? &temp->هغو : &temp->دام;

        إذا (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = كلي;
    كلي->parent = temp;
    كلي->هغو = sentinel;
    كلي->دام = sentinel;
    ngx_rbt_red(كلي);
}


فراغ
ngx_rbtree_delete(ngx_rbtree_t *بشيا, ngx_rbtree_كلي_t *كلي)
{
    ngx_uint_t           red;
    ngx_rbtree_كلي_t  **سيو, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    سيو = &بشيا->سيو;
    sentinel = بشيا->sentinel;

    إذا (كلي->هغو == sentinel) {
        temp = كلي->دام;
        subst = كلي;

    } وإلا إذا (كلي->دام == sentinel) {
        temp = كلي->هغو;
        subst = كلي;

    } وإلا {
        subst = ngx_rbtree_min(كلي->دام, sentinel);
        temp = subst->دام;
    }

    إذا (subst == *سيو) {
        *سيو = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        كلي->هغو = عدم;
        كلي->دام = عدم;
        كلي->parent = عدم;
        كلي->مبودي = 0;

        أرجع;
    }

    red = ngx_rbt_is_red(subst);

    إذا (subst == subst->parent->هغو) {
        subst->parent->هغو = temp;

    } وإلا {
        subst->parent->دام = temp;
    }

    إذا (subst == كلي) {

        temp->parent = subst->parent;

    } وإلا {

        إذا (subst->parent == كلي) {
            temp->parent = subst;

        } وإلا {
            temp->parent = subst->parent;
        }

        subst->هغو = كلي->هغو;
        subst->دام = كلي->دام;
        subst->parent = كلي->parent;
        ngx_rbt_copy_color(subst, كلي);

        إذا (كلي == *سيو) {
            *سيو = subst;

        } وإلا {
            إذا (كلي == كلي->parent->هغو) {
                كلي->parent->هغو = subst;
            } وإلا {
                كلي->parent->دام = subst;
            }
        }

        إذا (subst->هغو != sentinel) {
            subst->هغو->parent = subst;
        }

        إذا (subst->دام != sentinel) {
            subst->دام->parent = subst;
        }
    }

    /* DEBUG stuff */
    كلي->هغو = عدم;
    كلي->دام = عدم;
    كلي->parent = عدم;
    كلي->مبودي = 0;

    إذا (red) {
        أرجع;
    }

    /* a delete fixup */

    طالما (temp != *سيو && ngx_rbt_is_black(temp)) {

        إذا (temp == temp->parent->هغو) {
            w = temp->parent->دام;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_هغو_rotate(سيو, sentinel, temp->parent);
                w = temp->parent->دام;
            }

            إذا (ngx_rbt_is_black(w->هغو) && ngx_rbt_is_black(w->دام)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وإلا {
                إذا (ngx_rbt_is_black(w->دام)) {
                    ngx_rbt_black(w->هغو);
                    ngx_rbt_red(w);
                    ngx_rbtree_دام_rotate(سيو, sentinel, w);
                    w = temp->parent->دام;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->دام);
                ngx_rbtree_هغو_rotate(سيو, sentinel, temp->parent);
                temp = *سيو;
            }

        } وإلا {
            w = temp->parent->هغو;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_دام_rotate(سيو, sentinel, temp->parent);
                w = temp->parent->هغو;
            }

            إذا (ngx_rbt_is_black(w->هغو) && ngx_rbt_is_black(w->دام)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وإلا {
                إذا (ngx_rbt_is_black(w->هغو)) {
                    ngx_rbt_black(w->دام);
                    ngx_rbt_red(w);
                    ngx_rbtree_هغو_rotate(سيو, sentinel, w);
                    w = temp->parent->هغو;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->هغو);
                ngx_rbtree_دام_rotate(سيو, sentinel, temp->parent);
                temp = *سيو;
            }
        }
    }

    ngx_rbt_black(temp);
}


ساكن ngx_inline فراغ
ngx_rbtree_هغو_rotate(ngx_rbtree_كلي_t **سيو, ngx_rbtree_كلي_t *sentinel,
    ngx_rbtree_كلي_t *كلي)
{
    ngx_rbtree_كلي_t  *temp;

    temp = كلي->دام;
    كلي->دام = temp->هغو;

    إذا (temp->هغو != sentinel) {
        temp->هغو->parent = كلي;
    }

    temp->parent = كلي->parent;

    إذا (كلي == *سيو) {
        *سيو = temp;

    } وإلا إذا (كلي == كلي->parent->هغو) {
        كلي->parent->هغو = temp;

    } وإلا {
        كلي->parent->دام = temp;
    }

    temp->هغو = كلي;
    كلي->parent = temp;
}


ساكن ngx_inline فراغ
ngx_rbtree_دام_rotate(ngx_rbtree_كلي_t **سيو, ngx_rbtree_كلي_t *sentinel,
    ngx_rbtree_كلي_t *كلي)
{
    ngx_rbtree_كلي_t  *temp;

    temp = كلي->هغو;
    كلي->هغو = temp->دام;

    إذا (temp->دام != sentinel) {
        temp->دام->parent = كلي;
    }

    temp->parent = كلي->parent;

    إذا (كلي == *سيو) {
        *سيو = temp;

    } وإلا إذا (كلي == كلي->parent->دام) {
        كلي->parent->دام = temp;

    } وإلا {
        كلي->parent->هغو = temp;
    }

    temp->دام = كلي;
    كلي->parent = temp;
}


ngx_rbtree_كلي_t *
ngx_rbtree_next(ngx_rbtree_t *بشيا, ngx_rbtree_كلي_t *كلي)
{
    ngx_rbtree_كلي_t  *سيو, *sentinel, *parent;

    sentinel = بشيا->sentinel;

    إذا (كلي->دام != sentinel) {
        أرجع ngx_rbtree_min(كلي->دام, sentinel);
    }

    سيو = بشيا->سيو;

    لكل ( ;; ) {
        parent = كلي->parent;

        إذا (كلي == سيو) {
            أرجع عدم;
        }

        إذا (كلي == parent->هغو) {
            أرجع parent;
        }

        كلي = parent;
    }
}
