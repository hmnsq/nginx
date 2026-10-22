
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


ساكن ngx_inline فراغ ngx_rbtree_کاٻو_rotate(ngx_rbtree_ڳنڍ_t **پاڙ,
    ngx_rbtree_ڳنڍ_t *sentinel, ngx_rbtree_ڳنڍ_t *ڳنڍ);
ساكن ngx_inline فراغ ngx_rbtree_ساڄو_rotate(ngx_rbtree_ڳنڍ_t **پاڙ,
    ngx_rbtree_ڳنڍ_t *sentinel, ngx_rbtree_ڳنڍ_t *ڳنڍ);


فراغ
ngx_rbtree_وجهو(ngx_rbtree_t *وڻ, ngx_rbtree_ڳنڍ_t *ڳنڍ)
{
    ngx_rbtree_ڳنڍ_t  **پاڙ, *عارضي, *sentinel;

    /* a binary tree insert */

    پاڙ = &وڻ->پاڙ;
    sentinel = وڻ->sentinel;

    إذا (*پاڙ == sentinel) {
        ڳنڍ->والد = عدم;
        ڳنڍ->کاٻو = sentinel;
        ڳنڍ->ساڄو = sentinel;
        ngx_rbt_black(ڳنڍ);
        *پاڙ = ڳنڍ;

        أرجع;
    }

    وڻ->وجهو(*پاڙ, ڳنڍ, sentinel);

    /* re-balance tree */

    طالما (ڳنڍ != *پاڙ && ngx_rbt_is_red(ڳنڍ->والد)) {

        إذا (ڳنڍ->والد == ڳنڍ->والد->والد->کاٻو) {
            عارضي = ڳنڍ->والد->والد->ساڄو;

            إذا (ngx_rbt_is_red(عارضي)) {
                ngx_rbt_black(ڳنڍ->والد);
                ngx_rbt_black(عارضي);
                ngx_rbt_red(ڳنڍ->والد->والد);
                ڳنڍ = ڳنڍ->والد->والد;

            } وإلا {
                إذا (ڳنڍ == ڳنڍ->والد->ساڄو) {
                    ڳنڍ = ڳنڍ->والد;
                    ngx_rbtree_کاٻو_rotate(پاڙ, sentinel, ڳنڍ);
                }

                ngx_rbt_black(ڳنڍ->والد);
                ngx_rbt_red(ڳنڍ->والد->والد);
                ngx_rbtree_ساڄو_rotate(پاڙ, sentinel, ڳنڍ->والد->والد);
            }

        } وإلا {
            عارضي = ڳنڍ->والد->والد->کاٻو;

            إذا (ngx_rbt_is_red(عارضي)) {
                ngx_rbt_black(ڳنڍ->والد);
                ngx_rbt_black(عارضي);
                ngx_rbt_red(ڳنڍ->والد->والد);
                ڳنڍ = ڳنڍ->والد->والد;

            } وإلا {
                إذا (ڳنڍ == ڳنڍ->والد->کاٻو) {
                    ڳنڍ = ڳنڍ->والد;
                    ngx_rbtree_ساڄو_rotate(پاڙ, sentinel, ڳنڍ);
                }

                ngx_rbt_black(ڳنڍ->والد);
                ngx_rbt_red(ڳنڍ->والد->والد);
                ngx_rbtree_کاٻو_rotate(پاڙ, sentinel, ڳنڍ->والد->والد);
            }
        }
    }

    ngx_rbt_black(*پاڙ);
}


فراغ
ngx_rbtree_وجهو_قدر(ngx_rbtree_ڳنڍ_t *عارضي, ngx_rbtree_ڳنڍ_t *ڳنڍ,
    ngx_rbtree_ڳنڍ_t *sentinel)
{
    ngx_rbtree_ڳنڍ_t  **p;

    لكل ( ;; ) {

        p = (ڳنڍ->ڪنجي < عارضي->ڪنجي) ? &عارضي->کاٻو : &عارضي->ساڄو;

        إذا (*p == sentinel) {
            اكسر;
        }

        عارضي = *p;
    }

    *p = ڳنڍ;
    ڳنڍ->والد = عارضي;
    ڳنڍ->کاٻو = sentinel;
    ڳنڍ->ساڄو = sentinel;
    ngx_rbt_red(ڳنڍ);
}


فراغ
ngx_rbtree_وجهو_timer_قدر(ngx_rbtree_ڳنڍ_t *عارضي, ngx_rbtree_ڳنڍ_t *ڳنڍ,
    ngx_rbtree_ڳنڍ_t *sentinel)
{
    ngx_rbtree_ڳنڍ_t  **p;

    لكل ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_ڪنجي_int_t) (ڳنڍ->ڪنجي - عارضي->ڪنجي) < 0)
            ? &عارضي->کاٻو : &عارضي->ساڄو;

        إذا (*p == sentinel) {
            اكسر;
        }

        عارضي = *p;
    }

    *p = ڳنڍ;
    ڳنڍ->والد = عارضي;
    ڳنڍ->کاٻو = sentinel;
    ڳنڍ->ساڄو = sentinel;
    ngx_rbt_red(ڳنڍ);
}


فراغ
ngx_rbtree_delete(ngx_rbtree_t *وڻ, ngx_rbtree_ڳنڍ_t *ڳنڍ)
{
    ngx_uint_t           red;
    ngx_rbtree_ڳنڍ_t  **پاڙ, *sentinel, *subst, *عارضي, *w;

    /* a binary tree delete */

    پاڙ = &وڻ->پاڙ;
    sentinel = وڻ->sentinel;

    إذا (ڳنڍ->کاٻو == sentinel) {
        عارضي = ڳنڍ->ساڄو;
        subst = ڳنڍ;

    } وإلا إذا (ڳنڍ->ساڄو == sentinel) {
        عارضي = ڳنڍ->کاٻو;
        subst = ڳنڍ;

    } وإلا {
        subst = ngx_rbtree_min(ڳنڍ->ساڄو, sentinel);
        عارضي = subst->ساڄو;
    }

    إذا (subst == *پاڙ) {
        *پاڙ = عارضي;
        ngx_rbt_black(عارضي);

        /* DEBUG stuff */
        ڳنڍ->کاٻو = عدم;
        ڳنڍ->ساڄو = عدم;
        ڳنڍ->والد = عدم;
        ڳنڍ->ڪنجي = 0;

        أرجع;
    }

    red = ngx_rbt_is_red(subst);

    إذا (subst == subst->والد->کاٻو) {
        subst->والد->کاٻو = عارضي;

    } وإلا {
        subst->والد->ساڄو = عارضي;
    }

    إذا (subst == ڳنڍ) {

        عارضي->والد = subst->والد;

    } وإلا {

        إذا (subst->والد == ڳنڍ) {
            عارضي->والد = subst;

        } وإلا {
            عارضي->والد = subst->والد;
        }

        subst->کاٻو = ڳنڍ->کاٻو;
        subst->ساڄو = ڳنڍ->ساڄو;
        subst->والد = ڳنڍ->والد;
        ngx_rbt_copy_color(subst, ڳنڍ);

        إذا (ڳنڍ == *پاڙ) {
            *پاڙ = subst;

        } وإلا {
            إذا (ڳنڍ == ڳنڍ->والد->کاٻو) {
                ڳنڍ->والد->کاٻو = subst;
            } وإلا {
                ڳنڍ->والد->ساڄو = subst;
            }
        }

        إذا (subst->کاٻو != sentinel) {
            subst->کاٻو->والد = subst;
        }

        إذا (subst->ساڄو != sentinel) {
            subst->ساڄو->والد = subst;
        }
    }

    /* DEBUG stuff */
    ڳنڍ->کاٻو = عدم;
    ڳنڍ->ساڄو = عدم;
    ڳنڍ->والد = عدم;
    ڳنڍ->ڪنجي = 0;

    إذا (red) {
        أرجع;
    }

    /* a delete fixup */

    طالما (عارضي != *پاڙ && ngx_rbt_is_black(عارضي)) {

        إذا (عارضي == عارضي->والد->کاٻو) {
            w = عارضي->والد->ساڄو;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(عارضي->والد);
                ngx_rbtree_کاٻو_rotate(پاڙ, sentinel, عارضي->والد);
                w = عارضي->والد->ساڄو;
            }

            إذا (ngx_rbt_is_black(w->کاٻو) && ngx_rbt_is_black(w->ساڄو)) {
                ngx_rbt_red(w);
                عارضي = عارضي->والد;

            } وإلا {
                إذا (ngx_rbt_is_black(w->ساڄو)) {
                    ngx_rbt_black(w->کاٻو);
                    ngx_rbt_red(w);
                    ngx_rbtree_ساڄو_rotate(پاڙ, sentinel, w);
                    w = عارضي->والد->ساڄو;
                }

                ngx_rbt_copy_color(w, عارضي->والد);
                ngx_rbt_black(عارضي->والد);
                ngx_rbt_black(w->ساڄو);
                ngx_rbtree_کاٻو_rotate(پاڙ, sentinel, عارضي->والد);
                عارضي = *پاڙ;
            }

        } وإلا {
            w = عارضي->والد->کاٻو;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(عارضي->والد);
                ngx_rbtree_ساڄو_rotate(پاڙ, sentinel, عارضي->والد);
                w = عارضي->والد->کاٻو;
            }

            إذا (ngx_rbt_is_black(w->کاٻو) && ngx_rbt_is_black(w->ساڄو)) {
                ngx_rbt_red(w);
                عارضي = عارضي->والد;

            } وإلا {
                إذا (ngx_rbt_is_black(w->کاٻو)) {
                    ngx_rbt_black(w->ساڄو);
                    ngx_rbt_red(w);
                    ngx_rbtree_کاٻو_rotate(پاڙ, sentinel, w);
                    w = عارضي->والد->کاٻو;
                }

                ngx_rbt_copy_color(w, عارضي->والد);
                ngx_rbt_black(عارضي->والد);
                ngx_rbt_black(w->کاٻو);
                ngx_rbtree_ساڄو_rotate(پاڙ, sentinel, عارضي->والد);
                عارضي = *پاڙ;
            }
        }
    }

    ngx_rbt_black(عارضي);
}


ساكن ngx_inline فراغ
ngx_rbtree_کاٻو_rotate(ngx_rbtree_ڳنڍ_t **پاڙ, ngx_rbtree_ڳنڍ_t *sentinel,
    ngx_rbtree_ڳنڍ_t *ڳنڍ)
{
    ngx_rbtree_ڳنڍ_t  *عارضي;

    عارضي = ڳنڍ->ساڄو;
    ڳنڍ->ساڄو = عارضي->کاٻو;

    إذا (عارضي->کاٻو != sentinel) {
        عارضي->کاٻو->والد = ڳنڍ;
    }

    عارضي->والد = ڳنڍ->والد;

    إذا (ڳنڍ == *پاڙ) {
        *پاڙ = عارضي;

    } وإلا إذا (ڳنڍ == ڳنڍ->والد->کاٻو) {
        ڳنڍ->والد->کاٻو = عارضي;

    } وإلا {
        ڳنڍ->والد->ساڄو = عارضي;
    }

    عارضي->کاٻو = ڳنڍ;
    ڳنڍ->والد = عارضي;
}


ساكن ngx_inline فراغ
ngx_rbtree_ساڄو_rotate(ngx_rbtree_ڳنڍ_t **پاڙ, ngx_rbtree_ڳنڍ_t *sentinel,
    ngx_rbtree_ڳنڍ_t *ڳنڍ)
{
    ngx_rbtree_ڳنڍ_t  *عارضي;

    عارضي = ڳنڍ->کاٻو;
    ڳنڍ->کاٻو = عارضي->ساڄو;

    إذا (عارضي->ساڄو != sentinel) {
        عارضي->ساڄو->والد = ڳنڍ;
    }

    عارضي->والد = ڳنڍ->والد;

    إذا (ڳنڍ == *پاڙ) {
        *پاڙ = عارضي;

    } وإلا إذا (ڳنڍ == ڳنڍ->والد->ساڄو) {
        ڳنڍ->والد->ساڄو = عارضي;

    } وإلا {
        ڳنڍ->والد->کاٻو = عارضي;
    }

    عارضي->ساڄو = ڳنڍ;
    ڳنڍ->والد = عارضي;
}


ngx_rbtree_ڳنڍ_t *
ngx_rbtree_ايندڙ(ngx_rbtree_t *وڻ, ngx_rbtree_ڳنڍ_t *ڳنڍ)
{
    ngx_rbtree_ڳنڍ_t  *پاڙ, *sentinel, *والد;

    sentinel = وڻ->sentinel;

    إذا (ڳنڍ->ساڄو != sentinel) {
        أرجع ngx_rbtree_min(ڳنڍ->ساڄو, sentinel);
    }

    پاڙ = وڻ->پاڙ;

    لكل ( ;; ) {
        والد = ڳنڍ->والد;

        إذا (ڳنڍ == پاڙ) {
            أرجع عدم;
        }

        إذا (ڳنڍ == والد->کاٻو) {
            أرجع والد;
        }

        ڳنڍ = والد;
    }
}
