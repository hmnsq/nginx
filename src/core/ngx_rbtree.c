
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


ساكن ngx_inline فراغ ngx_rbtree_سول_rotate(ngx_rbtree_تۈگۈن_t **يىلتىز,
    ngx_rbtree_تۈگۈن_t *sentinel, ngx_rbtree_تۈگۈن_t *تۈگۈن);
ساكن ngx_inline فراغ ngx_rbtree_ئوڭ_rotate(ngx_rbtree_تۈگۈن_t **يىلتىز,
    ngx_rbtree_تۈگۈن_t *sentinel, ngx_rbtree_تۈگۈن_t *تۈگۈن);


فراغ
ngx_rbtree_قىستۇرۇش(ngx_rbtree_t *دەرەخ, ngx_rbtree_تۈگۈن_t *تۈگۈن)
{
    ngx_rbtree_تۈگۈن_t  **يىلتىز, *temp, *sentinel;

    /* a binary tree insert */

    يىلتىز = &دەرەخ->يىلتىز;
    sentinel = دەرەخ->sentinel;

    إذا (*يىلتىز == sentinel) {
        تۈگۈن->parent = عدم;
        تۈگۈن->سول = sentinel;
        تۈگۈن->ئوڭ = sentinel;
        ngx_rbt_black(تۈگۈن);
        *يىلتىز = تۈگۈن;

        أرجع;
    }

    دەرەخ->قىستۇرۇش(*يىلتىز, تۈگۈن, sentinel);

    /* re-balance tree */

    طالما (تۈگۈن != *يىلتىز && ngx_rbt_is_red(تۈگۈن->parent)) {

        إذا (تۈگۈن->parent == تۈگۈن->parent->parent->سول) {
            temp = تۈگۈن->parent->parent->ئوڭ;

            إذا (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(تۈگۈن->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(تۈگۈن->parent->parent);
                تۈگۈن = تۈگۈن->parent->parent;

            } وإلا {
                إذا (تۈگۈن == تۈگۈن->parent->ئوڭ) {
                    تۈگۈن = تۈگۈن->parent;
                    ngx_rbtree_سول_rotate(يىلتىز, sentinel, تۈگۈن);
                }

                ngx_rbt_black(تۈگۈن->parent);
                ngx_rbt_red(تۈگۈن->parent->parent);
                ngx_rbtree_ئوڭ_rotate(يىلتىز, sentinel, تۈگۈن->parent->parent);
            }

        } وإلا {
            temp = تۈگۈن->parent->parent->سول;

            إذا (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(تۈگۈن->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(تۈگۈن->parent->parent);
                تۈگۈن = تۈگۈن->parent->parent;

            } وإلا {
                إذا (تۈگۈن == تۈگۈن->parent->سول) {
                    تۈگۈن = تۈگۈن->parent;
                    ngx_rbtree_ئوڭ_rotate(يىلتىز, sentinel, تۈگۈن);
                }

                ngx_rbt_black(تۈگۈن->parent);
                ngx_rbt_red(تۈگۈن->parent->parent);
                ngx_rbtree_سول_rotate(يىلتىز, sentinel, تۈگۈن->parent->parent);
            }
        }
    }

    ngx_rbt_black(*يىلتىز);
}


فراغ
ngx_rbtree_قىستۇرۇش_قىممەت(ngx_rbtree_تۈگۈن_t *temp, ngx_rbtree_تۈگۈن_t *تۈگۈن,
    ngx_rbtree_تۈگۈن_t *sentinel)
{
    ngx_rbtree_تۈگۈن_t  **p;

    لكل ( ;; ) {

        p = (تۈگۈن->ئاچقۇچ < temp->ئاچقۇچ) ? &temp->سول : &temp->ئوڭ;

        إذا (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = تۈگۈن;
    تۈگۈن->parent = temp;
    تۈگۈن->سول = sentinel;
    تۈگۈن->ئوڭ = sentinel;
    ngx_rbt_red(تۈگۈن);
}


فراغ
ngx_rbtree_قىستۇرۇش_timer_قىممەت(ngx_rbtree_تۈگۈن_t *temp, ngx_rbtree_تۈگۈن_t *تۈگۈن,
    ngx_rbtree_تۈگۈن_t *sentinel)
{
    ngx_rbtree_تۈگۈن_t  **p;

    لكل ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_ئاچقۇچ_int_t) (تۈگۈن->ئاچقۇچ - temp->ئاچقۇچ) < 0)
            ? &temp->سول : &temp->ئوڭ;

        إذا (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = تۈگۈن;
    تۈگۈن->parent = temp;
    تۈگۈن->سول = sentinel;
    تۈگۈن->ئوڭ = sentinel;
    ngx_rbt_red(تۈگۈن);
}


فراغ
ngx_rbtree_delete(ngx_rbtree_t *دەرەخ, ngx_rbtree_تۈگۈن_t *تۈگۈن)
{
    ngx_uint_t           red;
    ngx_rbtree_تۈگۈن_t  **يىلتىز, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    يىلتىز = &دەرەخ->يىلتىز;
    sentinel = دەرەخ->sentinel;

    إذا (تۈگۈن->سول == sentinel) {
        temp = تۈگۈن->ئوڭ;
        subst = تۈگۈن;

    } وإلا إذا (تۈگۈن->ئوڭ == sentinel) {
        temp = تۈگۈن->سول;
        subst = تۈگۈن;

    } وإلا {
        subst = ngx_rbtree_min(تۈگۈن->ئوڭ, sentinel);
        temp = subst->ئوڭ;
    }

    إذا (subst == *يىلتىز) {
        *يىلتىز = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        تۈگۈن->سول = عدم;
        تۈگۈن->ئوڭ = عدم;
        تۈگۈن->parent = عدم;
        تۈگۈن->ئاچقۇچ = 0;

        أرجع;
    }

    red = ngx_rbt_is_red(subst);

    إذا (subst == subst->parent->سول) {
        subst->parent->سول = temp;

    } وإلا {
        subst->parent->ئوڭ = temp;
    }

    إذا (subst == تۈگۈن) {

        temp->parent = subst->parent;

    } وإلا {

        إذا (subst->parent == تۈگۈن) {
            temp->parent = subst;

        } وإلا {
            temp->parent = subst->parent;
        }

        subst->سول = تۈگۈن->سول;
        subst->ئوڭ = تۈگۈن->ئوڭ;
        subst->parent = تۈگۈن->parent;
        ngx_rbt_copy_color(subst, تۈگۈن);

        إذا (تۈگۈن == *يىلتىز) {
            *يىلتىز = subst;

        } وإلا {
            إذا (تۈگۈن == تۈگۈن->parent->سول) {
                تۈگۈن->parent->سول = subst;
            } وإلا {
                تۈگۈن->parent->ئوڭ = subst;
            }
        }

        إذا (subst->سول != sentinel) {
            subst->سول->parent = subst;
        }

        إذا (subst->ئوڭ != sentinel) {
            subst->ئوڭ->parent = subst;
        }
    }

    /* DEBUG stuff */
    تۈگۈن->سول = عدم;
    تۈگۈن->ئوڭ = عدم;
    تۈگۈن->parent = عدم;
    تۈگۈن->ئاچقۇچ = 0;

    إذا (red) {
        أرجع;
    }

    /* a delete fixup */

    طالما (temp != *يىلتىز && ngx_rbt_is_black(temp)) {

        إذا (temp == temp->parent->سول) {
            w = temp->parent->ئوڭ;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_سول_rotate(يىلتىز, sentinel, temp->parent);
                w = temp->parent->ئوڭ;
            }

            إذا (ngx_rbt_is_black(w->سول) && ngx_rbt_is_black(w->ئوڭ)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وإلا {
                إذا (ngx_rbt_is_black(w->ئوڭ)) {
                    ngx_rbt_black(w->سول);
                    ngx_rbt_red(w);
                    ngx_rbtree_ئوڭ_rotate(يىلتىز, sentinel, w);
                    w = temp->parent->ئوڭ;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->ئوڭ);
                ngx_rbtree_سول_rotate(يىلتىز, sentinel, temp->parent);
                temp = *يىلتىز;
            }

        } وإلا {
            w = temp->parent->سول;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_ئوڭ_rotate(يىلتىز, sentinel, temp->parent);
                w = temp->parent->سول;
            }

            إذا (ngx_rbt_is_black(w->سول) && ngx_rbt_is_black(w->ئوڭ)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وإلا {
                إذا (ngx_rbt_is_black(w->سول)) {
                    ngx_rbt_black(w->ئوڭ);
                    ngx_rbt_red(w);
                    ngx_rbtree_سول_rotate(يىلتىز, sentinel, w);
                    w = temp->parent->سول;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->سول);
                ngx_rbtree_ئوڭ_rotate(يىلتىز, sentinel, temp->parent);
                temp = *يىلتىز;
            }
        }
    }

    ngx_rbt_black(temp);
}


ساكن ngx_inline فراغ
ngx_rbtree_سول_rotate(ngx_rbtree_تۈگۈن_t **يىلتىز, ngx_rbtree_تۈگۈن_t *sentinel,
    ngx_rbtree_تۈگۈن_t *تۈگۈن)
{
    ngx_rbtree_تۈگۈن_t  *temp;

    temp = تۈگۈن->ئوڭ;
    تۈگۈن->ئوڭ = temp->سول;

    إذا (temp->سول != sentinel) {
        temp->سول->parent = تۈگۈن;
    }

    temp->parent = تۈگۈن->parent;

    إذا (تۈگۈن == *يىلتىز) {
        *يىلتىز = temp;

    } وإلا إذا (تۈگۈن == تۈگۈن->parent->سول) {
        تۈگۈن->parent->سول = temp;

    } وإلا {
        تۈگۈن->parent->ئوڭ = temp;
    }

    temp->سول = تۈگۈن;
    تۈگۈن->parent = temp;
}


ساكن ngx_inline فراغ
ngx_rbtree_ئوڭ_rotate(ngx_rbtree_تۈگۈن_t **يىلتىز, ngx_rbtree_تۈگۈن_t *sentinel,
    ngx_rbtree_تۈگۈن_t *تۈگۈن)
{
    ngx_rbtree_تۈگۈن_t  *temp;

    temp = تۈگۈن->سول;
    تۈگۈن->سول = temp->ئوڭ;

    إذا (temp->ئوڭ != sentinel) {
        temp->ئوڭ->parent = تۈگۈن;
    }

    temp->parent = تۈگۈن->parent;

    إذا (تۈگۈن == *يىلتىز) {
        *يىلتىز = temp;

    } وإلا إذا (تۈگۈن == تۈگۈن->parent->ئوڭ) {
        تۈگۈن->parent->ئوڭ = temp;

    } وإلا {
        تۈگۈن->parent->سول = temp;
    }

    temp->ئوڭ = تۈگۈن;
    تۈگۈن->parent = temp;
}


ngx_rbtree_تۈگۈن_t *
ngx_rbtree_next(ngx_rbtree_t *دەرەخ, ngx_rbtree_تۈگۈن_t *تۈگۈن)
{
    ngx_rbtree_تۈگۈن_t  *يىلتىز, *sentinel, *parent;

    sentinel = دەرەخ->sentinel;

    إذا (تۈگۈن->ئوڭ != sentinel) {
        أرجع ngx_rbtree_min(تۈگۈن->ئوڭ, sentinel);
    }

    يىلتىز = دەرەخ->يىلتىز;

    لكل ( ;; ) {
        parent = تۈگۈن->parent;

        إذا (تۈگۈن == يىلتىز) {
            أرجع عدم;
        }

        إذا (تۈگۈن == parent->سول) {
            أرجع parent;
        }

        تۈگۈن = parent;
    }
}
