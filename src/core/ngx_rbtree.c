
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


ساكن ngx_inline فراغ ngx_rbtree_يسار_rotate(ngx_rbtree_عقدة_t **جذر,
    ngx_rbtree_عقدة_t *sentinel, ngx_rbtree_عقدة_t *عقدة);
ساكن ngx_inline فراغ ngx_rbtree_يمين_rotate(ngx_rbtree_عقدة_t **جذر,
    ngx_rbtree_عقدة_t *sentinel, ngx_rbtree_عقدة_t *عقدة);


فراغ
ngx_rbtree_أدرج(ngx_rbtree_t *شجرة, ngx_rbtree_عقدة_t *عقدة)
{
    ngx_rbtree_عقدة_t  **جذر, *temp, *sentinel;

    /* a binary tree insert */

    جذر = &شجرة->جذر;
    sentinel = شجرة->sentinel;

    إذا (*جذر == sentinel) {
        عقدة->parent = عدم;
        عقدة->يسار = sentinel;
        عقدة->يمين = sentinel;
        ngx_rbt_black(عقدة);
        *جذر = عقدة;

        أرجع;
    }

    شجرة->أدرج(*جذر, عقدة, sentinel);

    /* re-balance tree */

    طالما (عقدة != *جذر && ngx_rbt_is_red(عقدة->parent)) {

        إذا (عقدة->parent == عقدة->parent->parent->يسار) {
            temp = عقدة->parent->parent->يمين;

            إذا (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(عقدة->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(عقدة->parent->parent);
                عقدة = عقدة->parent->parent;

            } وإلا {
                إذا (عقدة == عقدة->parent->يمين) {
                    عقدة = عقدة->parent;
                    ngx_rbtree_يسار_rotate(جذر, sentinel, عقدة);
                }

                ngx_rbt_black(عقدة->parent);
                ngx_rbt_red(عقدة->parent->parent);
                ngx_rbtree_يمين_rotate(جذر, sentinel, عقدة->parent->parent);
            }

        } وإلا {
            temp = عقدة->parent->parent->يسار;

            إذا (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(عقدة->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(عقدة->parent->parent);
                عقدة = عقدة->parent->parent;

            } وإلا {
                إذا (عقدة == عقدة->parent->يسار) {
                    عقدة = عقدة->parent;
                    ngx_rbtree_يمين_rotate(جذر, sentinel, عقدة);
                }

                ngx_rbt_black(عقدة->parent);
                ngx_rbt_red(عقدة->parent->parent);
                ngx_rbtree_يسار_rotate(جذر, sentinel, عقدة->parent->parent);
            }
        }
    }

    ngx_rbt_black(*جذر);
}


فراغ
ngx_rbtree_أدرج_قيمة(ngx_rbtree_عقدة_t *temp, ngx_rbtree_عقدة_t *عقدة,
    ngx_rbtree_عقدة_t *sentinel)
{
    ngx_rbtree_عقدة_t  **p;

    لكل ( ;; ) {

        p = (عقدة->مفتاح < temp->مفتاح) ? &temp->يسار : &temp->يمين;

        إذا (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = عقدة;
    عقدة->parent = temp;
    عقدة->يسار = sentinel;
    عقدة->يمين = sentinel;
    ngx_rbt_red(عقدة);
}


فراغ
ngx_rbtree_أدرج_timer_قيمة(ngx_rbtree_عقدة_t *temp, ngx_rbtree_عقدة_t *عقدة,
    ngx_rbtree_عقدة_t *sentinel)
{
    ngx_rbtree_عقدة_t  **p;

    لكل ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_مفتاح_int_t) (عقدة->مفتاح - temp->مفتاح) < 0)
            ? &temp->يسار : &temp->يمين;

        إذا (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = عقدة;
    عقدة->parent = temp;
    عقدة->يسار = sentinel;
    عقدة->يمين = sentinel;
    ngx_rbt_red(عقدة);
}


فراغ
ngx_rbtree_delete(ngx_rbtree_t *شجرة, ngx_rbtree_عقدة_t *عقدة)
{
    ngx_uint_t           red;
    ngx_rbtree_عقدة_t  **جذر, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    جذر = &شجرة->جذر;
    sentinel = شجرة->sentinel;

    إذا (عقدة->يسار == sentinel) {
        temp = عقدة->يمين;
        subst = عقدة;

    } وإلا إذا (عقدة->يمين == sentinel) {
        temp = عقدة->يسار;
        subst = عقدة;

    } وإلا {
        subst = ngx_rbtree_min(عقدة->يمين, sentinel);
        temp = subst->يمين;
    }

    إذا (subst == *جذر) {
        *جذر = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        عقدة->يسار = عدم;
        عقدة->يمين = عدم;
        عقدة->parent = عدم;
        عقدة->مفتاح = 0;

        أرجع;
    }

    red = ngx_rbt_is_red(subst);

    إذا (subst == subst->parent->يسار) {
        subst->parent->يسار = temp;

    } وإلا {
        subst->parent->يمين = temp;
    }

    إذا (subst == عقدة) {

        temp->parent = subst->parent;

    } وإلا {

        إذا (subst->parent == عقدة) {
            temp->parent = subst;

        } وإلا {
            temp->parent = subst->parent;
        }

        subst->يسار = عقدة->يسار;
        subst->يمين = عقدة->يمين;
        subst->parent = عقدة->parent;
        ngx_rbt_copy_color(subst, عقدة);

        إذا (عقدة == *جذر) {
            *جذر = subst;

        } وإلا {
            إذا (عقدة == عقدة->parent->يسار) {
                عقدة->parent->يسار = subst;
            } وإلا {
                عقدة->parent->يمين = subst;
            }
        }

        إذا (subst->يسار != sentinel) {
            subst->يسار->parent = subst;
        }

        إذا (subst->يمين != sentinel) {
            subst->يمين->parent = subst;
        }
    }

    /* DEBUG stuff */
    عقدة->يسار = عدم;
    عقدة->يمين = عدم;
    عقدة->parent = عدم;
    عقدة->مفتاح = 0;

    إذا (red) {
        أرجع;
    }

    /* a delete fixup */

    طالما (temp != *جذر && ngx_rbt_is_black(temp)) {

        إذا (temp == temp->parent->يسار) {
            w = temp->parent->يمين;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_يسار_rotate(جذر, sentinel, temp->parent);
                w = temp->parent->يمين;
            }

            إذا (ngx_rbt_is_black(w->يسار) && ngx_rbt_is_black(w->يمين)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وإلا {
                إذا (ngx_rbt_is_black(w->يمين)) {
                    ngx_rbt_black(w->يسار);
                    ngx_rbt_red(w);
                    ngx_rbtree_يمين_rotate(جذر, sentinel, w);
                    w = temp->parent->يمين;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->يمين);
                ngx_rbtree_يسار_rotate(جذر, sentinel, temp->parent);
                temp = *جذر;
            }

        } وإلا {
            w = temp->parent->يسار;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_يمين_rotate(جذر, sentinel, temp->parent);
                w = temp->parent->يسار;
            }

            إذا (ngx_rbt_is_black(w->يسار) && ngx_rbt_is_black(w->يمين)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وإلا {
                إذا (ngx_rbt_is_black(w->يسار)) {
                    ngx_rbt_black(w->يمين);
                    ngx_rbt_red(w);
                    ngx_rbtree_يسار_rotate(جذر, sentinel, w);
                    w = temp->parent->يسار;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->يسار);
                ngx_rbtree_يمين_rotate(جذر, sentinel, temp->parent);
                temp = *جذر;
            }
        }
    }

    ngx_rbt_black(temp);
}


ساكن ngx_inline فراغ
ngx_rbtree_يسار_rotate(ngx_rbtree_عقدة_t **جذر, ngx_rbtree_عقدة_t *sentinel,
    ngx_rbtree_عقدة_t *عقدة)
{
    ngx_rbtree_عقدة_t  *temp;

    temp = عقدة->يمين;
    عقدة->يمين = temp->يسار;

    إذا (temp->يسار != sentinel) {
        temp->يسار->parent = عقدة;
    }

    temp->parent = عقدة->parent;

    إذا (عقدة == *جذر) {
        *جذر = temp;

    } وإلا إذا (عقدة == عقدة->parent->يسار) {
        عقدة->parent->يسار = temp;

    } وإلا {
        عقدة->parent->يمين = temp;
    }

    temp->يسار = عقدة;
    عقدة->parent = temp;
}


ساكن ngx_inline فراغ
ngx_rbtree_يمين_rotate(ngx_rbtree_عقدة_t **جذر, ngx_rbtree_عقدة_t *sentinel,
    ngx_rbtree_عقدة_t *عقدة)
{
    ngx_rbtree_عقدة_t  *temp;

    temp = عقدة->يسار;
    عقدة->يسار = temp->يمين;

    إذا (temp->يمين != sentinel) {
        temp->يمين->parent = عقدة;
    }

    temp->parent = عقدة->parent;

    إذا (عقدة == *جذر) {
        *جذر = temp;

    } وإلا إذا (عقدة == عقدة->parent->يمين) {
        عقدة->parent->يمين = temp;

    } وإلا {
        عقدة->parent->يسار = temp;
    }

    temp->يمين = عقدة;
    عقدة->parent = temp;
}


ngx_rbtree_عقدة_t *
ngx_rbtree_next(ngx_rbtree_t *شجرة, ngx_rbtree_عقدة_t *عقدة)
{
    ngx_rbtree_عقدة_t  *جذر, *sentinel, *parent;

    sentinel = شجرة->sentinel;

    إذا (عقدة->يمين != sentinel) {
        أرجع ngx_rbtree_min(عقدة->يمين, sentinel);
    }

    جذر = شجرة->جذر;

    لكل ( ;; ) {
        parent = عقدة->parent;

        إذا (عقدة == جذر) {
            أرجع عدم;
        }

        إذا (عقدة == parent->يسار) {
            أرجع parent;
        }

        عقدة = parent;
    }
}
