
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


ساكن ngx_inline فراغ ngx_rbtree_کيري_rotate(ngx_rbtree_نود_t **اکر,
    ngx_rbtree_نود_t *sentinel, ngx_rbtree_نود_t *نود);
ساكن ngx_inline فراغ ngx_rbtree_کانن_rotate(ngx_rbtree_نود_t **اکر,
    ngx_rbtree_نود_t *sentinel, ngx_rbtree_نود_t *نود);


فراغ
ngx_rbtree_سيسيڤ(ngx_rbtree_t *ڤوکوق, ngx_rbtree_نود_t *نود)
{
    ngx_rbtree_نود_t  **اکر, *temp, *sentinel;

    /* a binary tree insert */

    اکر = &ڤوکوق->اکر;
    sentinel = ڤوکوق->sentinel;

    إذا (*اکر == sentinel) {
        نود->parent = عدم;
        نود->کيري = sentinel;
        نود->کانن = sentinel;
        ngx_rbt_black(نود);
        *اکر = نود;

        أرجع;
    }

    ڤوکوق->سيسيڤ(*اکر, نود, sentinel);

    /* re-balance tree */

    طالما (نود != *اکر && ngx_rbt_is_red(نود->parent)) {

        إذا (نود->parent == نود->parent->parent->کيري) {
            temp = نود->parent->parent->کانن;

            إذا (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(نود->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(نود->parent->parent);
                نود = نود->parent->parent;

            } وإلا {
                إذا (نود == نود->parent->کانن) {
                    نود = نود->parent;
                    ngx_rbtree_کيري_rotate(اکر, sentinel, نود);
                }

                ngx_rbt_black(نود->parent);
                ngx_rbt_red(نود->parent->parent);
                ngx_rbtree_کانن_rotate(اکر, sentinel, نود->parent->parent);
            }

        } وإلا {
            temp = نود->parent->parent->کيري;

            إذا (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(نود->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(نود->parent->parent);
                نود = نود->parent->parent;

            } وإلا {
                إذا (نود == نود->parent->کيري) {
                    نود = نود->parent;
                    ngx_rbtree_کانن_rotate(اکر, sentinel, نود);
                }

                ngx_rbt_black(نود->parent);
                ngx_rbt_red(نود->parent->parent);
                ngx_rbtree_کيري_rotate(اکر, sentinel, نود->parent->parent);
            }
        }
    }

    ngx_rbt_black(*اکر);
}


فراغ
ngx_rbtree_سيسيڤ_نيلاي(ngx_rbtree_نود_t *temp, ngx_rbtree_نود_t *نود,
    ngx_rbtree_نود_t *sentinel)
{
    ngx_rbtree_نود_t  **p;

    لكل ( ;; ) {

        p = (نود->کونچي < temp->کونچي) ? &temp->کيري : &temp->کانن;

        إذا (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = نود;
    نود->parent = temp;
    نود->کيري = sentinel;
    نود->کانن = sentinel;
    ngx_rbt_red(نود);
}


فراغ
ngx_rbtree_سيسيڤ_timer_نيلاي(ngx_rbtree_نود_t *temp, ngx_rbtree_نود_t *نود,
    ngx_rbtree_نود_t *sentinel)
{
    ngx_rbtree_نود_t  **p;

    لكل ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_کونچي_int_t) (نود->کونچي - temp->کونچي) < 0)
            ? &temp->کيري : &temp->کانن;

        إذا (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = نود;
    نود->parent = temp;
    نود->کيري = sentinel;
    نود->کانن = sentinel;
    ngx_rbt_red(نود);
}


فراغ
ngx_rbtree_delete(ngx_rbtree_t *ڤوکوق, ngx_rbtree_نود_t *نود)
{
    ngx_uint_t           red;
    ngx_rbtree_نود_t  **اکر, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    اکر = &ڤوکوق->اکر;
    sentinel = ڤوکوق->sentinel;

    إذا (نود->کيري == sentinel) {
        temp = نود->کانن;
        subst = نود;

    } وإلا إذا (نود->کانن == sentinel) {
        temp = نود->کيري;
        subst = نود;

    } وإلا {
        subst = ngx_rbtree_min(نود->کانن, sentinel);
        temp = subst->کانن;
    }

    إذا (subst == *اکر) {
        *اکر = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        نود->کيري = عدم;
        نود->کانن = عدم;
        نود->parent = عدم;
        نود->کونچي = 0;

        أرجع;
    }

    red = ngx_rbt_is_red(subst);

    إذا (subst == subst->parent->کيري) {
        subst->parent->کيري = temp;

    } وإلا {
        subst->parent->کانن = temp;
    }

    إذا (subst == نود) {

        temp->parent = subst->parent;

    } وإلا {

        إذا (subst->parent == نود) {
            temp->parent = subst;

        } وإلا {
            temp->parent = subst->parent;
        }

        subst->کيري = نود->کيري;
        subst->کانن = نود->کانن;
        subst->parent = نود->parent;
        ngx_rbt_copy_color(subst, نود);

        إذا (نود == *اکر) {
            *اکر = subst;

        } وإلا {
            إذا (نود == نود->parent->کيري) {
                نود->parent->کيري = subst;
            } وإلا {
                نود->parent->کانن = subst;
            }
        }

        إذا (subst->کيري != sentinel) {
            subst->کيري->parent = subst;
        }

        إذا (subst->کانن != sentinel) {
            subst->کانن->parent = subst;
        }
    }

    /* DEBUG stuff */
    نود->کيري = عدم;
    نود->کانن = عدم;
    نود->parent = عدم;
    نود->کونچي = 0;

    إذا (red) {
        أرجع;
    }

    /* a delete fixup */

    طالما (temp != *اکر && ngx_rbt_is_black(temp)) {

        إذا (temp == temp->parent->کيري) {
            w = temp->parent->کانن;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_کيري_rotate(اکر, sentinel, temp->parent);
                w = temp->parent->کانن;
            }

            إذا (ngx_rbt_is_black(w->کيري) && ngx_rbt_is_black(w->کانن)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وإلا {
                إذا (ngx_rbt_is_black(w->کانن)) {
                    ngx_rbt_black(w->کيري);
                    ngx_rbt_red(w);
                    ngx_rbtree_کانن_rotate(اکر, sentinel, w);
                    w = temp->parent->کانن;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->کانن);
                ngx_rbtree_کيري_rotate(اکر, sentinel, temp->parent);
                temp = *اکر;
            }

        } وإلا {
            w = temp->parent->کيري;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_کانن_rotate(اکر, sentinel, temp->parent);
                w = temp->parent->کيري;
            }

            إذا (ngx_rbt_is_black(w->کيري) && ngx_rbt_is_black(w->کانن)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وإلا {
                إذا (ngx_rbt_is_black(w->کيري)) {
                    ngx_rbt_black(w->کانن);
                    ngx_rbt_red(w);
                    ngx_rbtree_کيري_rotate(اکر, sentinel, w);
                    w = temp->parent->کيري;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->کيري);
                ngx_rbtree_کانن_rotate(اکر, sentinel, temp->parent);
                temp = *اکر;
            }
        }
    }

    ngx_rbt_black(temp);
}


ساكن ngx_inline فراغ
ngx_rbtree_کيري_rotate(ngx_rbtree_نود_t **اکر, ngx_rbtree_نود_t *sentinel,
    ngx_rbtree_نود_t *نود)
{
    ngx_rbtree_نود_t  *temp;

    temp = نود->کانن;
    نود->کانن = temp->کيري;

    إذا (temp->کيري != sentinel) {
        temp->کيري->parent = نود;
    }

    temp->parent = نود->parent;

    إذا (نود == *اکر) {
        *اکر = temp;

    } وإلا إذا (نود == نود->parent->کيري) {
        نود->parent->کيري = temp;

    } وإلا {
        نود->parent->کانن = temp;
    }

    temp->کيري = نود;
    نود->parent = temp;
}


ساكن ngx_inline فراغ
ngx_rbtree_کانن_rotate(ngx_rbtree_نود_t **اکر, ngx_rbtree_نود_t *sentinel,
    ngx_rbtree_نود_t *نود)
{
    ngx_rbtree_نود_t  *temp;

    temp = نود->کيري;
    نود->کيري = temp->کانن;

    إذا (temp->کانن != sentinel) {
        temp->کانن->parent = نود;
    }

    temp->parent = نود->parent;

    إذا (نود == *اکر) {
        *اکر = temp;

    } وإلا إذا (نود == نود->parent->کانن) {
        نود->parent->کانن = temp;

    } وإلا {
        نود->parent->کيري = temp;
    }

    temp->کانن = نود;
    نود->parent = temp;
}


ngx_rbtree_نود_t *
ngx_rbtree_next(ngx_rbtree_t *ڤوکوق, ngx_rbtree_نود_t *نود)
{
    ngx_rbtree_نود_t  *اکر, *sentinel, *parent;

    sentinel = ڤوکوق->sentinel;

    إذا (نود->کانن != sentinel) {
        أرجع ngx_rbtree_min(نود->کانن, sentinel);
    }

    اکر = ڤوکوق->اکر;

    لكل ( ;; ) {
        parent = نود->parent;

        إذا (نود == اکر) {
            أرجع عدم;
        }

        إذا (نود == parent->کيري) {
            أرجع parent;
        }

        نود = parent;
    }
}
