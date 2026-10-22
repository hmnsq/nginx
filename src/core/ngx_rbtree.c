
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


ساکن ngx_inline خالی ngx_rbtree_بایاں_rotate(ngx_rbtree_گرہ_t **جڑ,
    ngx_rbtree_گرہ_t *sentinel, ngx_rbtree_گرہ_t *گرہ);
ساکن ngx_inline خالی ngx_rbtree_دایاں_rotate(ngx_rbtree_گرہ_t **جڑ,
    ngx_rbtree_گرہ_t *sentinel, ngx_rbtree_گرہ_t *گرہ);


خالی
ngx_rbtree_داخل_کرو(ngx_rbtree_t *درخت, ngx_rbtree_گرہ_t *گرہ)
{
    ngx_rbtree_گرہ_t  **جڑ, *عارضی, *sentinel;

    /* a binary tree insert */

    جڑ = &درخت->جڑ;
    sentinel = درخت->sentinel;

    اگر (*جڑ == sentinel) {
        گرہ->والد = تہی;
        گرہ->بایاں = sentinel;
        گرہ->دایاں = sentinel;
        ngx_rbt_black(گرہ);
        *جڑ = گرہ;

        واپس;
    }

    درخت->داخل_کرو(*جڑ, گرہ, sentinel);

    /* re-balance tree */

    جب (گرہ != *جڑ && ngx_rbt_is_red(گرہ->والد)) {

        اگر (گرہ->والد == گرہ->والد->والد->بایاں) {
            عارضی = گرہ->والد->والد->دایاں;

            اگر (ngx_rbt_is_red(عارضی)) {
                ngx_rbt_black(گرہ->والد);
                ngx_rbt_black(عارضی);
                ngx_rbt_red(گرہ->والد->والد);
                گرہ = گرہ->والد->والد;

            } ورنہ {
                اگر (گرہ == گرہ->والد->دایاں) {
                    گرہ = گرہ->والد;
                    ngx_rbtree_بایاں_rotate(جڑ, sentinel, گرہ);
                }

                ngx_rbt_black(گرہ->والد);
                ngx_rbt_red(گرہ->والد->والد);
                ngx_rbtree_دایاں_rotate(جڑ, sentinel, گرہ->والد->والد);
            }

        } ورنہ {
            عارضی = گرہ->والد->والد->بایاں;

            اگر (ngx_rbt_is_red(عارضی)) {
                ngx_rbt_black(گرہ->والد);
                ngx_rbt_black(عارضی);
                ngx_rbt_red(گرہ->والد->والد);
                گرہ = گرہ->والد->والد;

            } ورنہ {
                اگر (گرہ == گرہ->والد->بایاں) {
                    گرہ = گرہ->والد;
                    ngx_rbtree_دایاں_rotate(جڑ, sentinel, گرہ);
                }

                ngx_rbt_black(گرہ->والد);
                ngx_rbt_red(گرہ->والد->والد);
                ngx_rbtree_بایاں_rotate(جڑ, sentinel, گرہ->والد->والد);
            }
        }
    }

    ngx_rbt_black(*جڑ);
}


خالی
ngx_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *عارضی, ngx_rbtree_گرہ_t *گرہ,
    ngx_rbtree_گرہ_t *sentinel)
{
    ngx_rbtree_گرہ_t  **p;

    برائے ( ;; ) {

        p = (گرہ->کلید < عارضی->کلید) ? &عارضی->بایاں : &عارضی->دایاں;

        اگر (*p == sentinel) {
            توڑو;
        }

        عارضی = *p;
    }

    *p = گرہ;
    گرہ->والد = عارضی;
    گرہ->بایاں = sentinel;
    گرہ->دایاں = sentinel;
    ngx_rbt_red(گرہ);
}


خالی
ngx_rbtree_داخل_کرو_timer_قدر(ngx_rbtree_گرہ_t *عارضی, ngx_rbtree_گرہ_t *گرہ,
    ngx_rbtree_گرہ_t *sentinel)
{
    ngx_rbtree_گرہ_t  **p;

    برائے ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_کلید_int_t) (گرہ->کلید - عارضی->کلید) < 0)
            ? &عارضی->بایاں : &عارضی->دایاں;

        اگر (*p == sentinel) {
            توڑو;
        }

        عارضی = *p;
    }

    *p = گرہ;
    گرہ->والد = عارضی;
    گرہ->بایاں = sentinel;
    گرہ->دایاں = sentinel;
    ngx_rbt_red(گرہ);
}


خالی
ngx_rbtree_delete(ngx_rbtree_t *درخت, ngx_rbtree_گرہ_t *گرہ)
{
    ngx_uint_t           red;
    ngx_rbtree_گرہ_t  **جڑ, *sentinel, *subst, *عارضی, *w;

    /* a binary tree delete */

    جڑ = &درخت->جڑ;
    sentinel = درخت->sentinel;

    اگر (گرہ->بایاں == sentinel) {
        عارضی = گرہ->دایاں;
        subst = گرہ;

    } ورنہ اگر (گرہ->دایاں == sentinel) {
        عارضی = گرہ->بایاں;
        subst = گرہ;

    } ورنہ {
        subst = ngx_rbtree_min(گرہ->دایاں, sentinel);
        عارضی = subst->دایاں;
    }

    اگر (subst == *جڑ) {
        *جڑ = عارضی;
        ngx_rbt_black(عارضی);

        /* DEBUG stuff */
        گرہ->بایاں = تہی;
        گرہ->دایاں = تہی;
        گرہ->والد = تہی;
        گرہ->کلید = 0;

        واپس;
    }

    red = ngx_rbt_is_red(subst);

    اگر (subst == subst->والد->بایاں) {
        subst->والد->بایاں = عارضی;

    } ورنہ {
        subst->والد->دایاں = عارضی;
    }

    اگر (subst == گرہ) {

        عارضی->والد = subst->والد;

    } ورنہ {

        اگر (subst->والد == گرہ) {
            عارضی->والد = subst;

        } ورنہ {
            عارضی->والد = subst->والد;
        }

        subst->بایاں = گرہ->بایاں;
        subst->دایاں = گرہ->دایاں;
        subst->والد = گرہ->والد;
        ngx_rbt_copy_color(subst, گرہ);

        اگر (گرہ == *جڑ) {
            *جڑ = subst;

        } ورنہ {
            اگر (گرہ == گرہ->والد->بایاں) {
                گرہ->والد->بایاں = subst;
            } ورنہ {
                گرہ->والد->دایاں = subst;
            }
        }

        اگر (subst->بایاں != sentinel) {
            subst->بایاں->والد = subst;
        }

        اگر (subst->دایاں != sentinel) {
            subst->دایاں->والد = subst;
        }
    }

    /* DEBUG stuff */
    گرہ->بایاں = تہی;
    گرہ->دایاں = تہی;
    گرہ->والد = تہی;
    گرہ->کلید = 0;

    اگر (red) {
        واپس;
    }

    /* a delete fixup */

    جب (عارضی != *جڑ && ngx_rbt_is_black(عارضی)) {

        اگر (عارضی == عارضی->والد->بایاں) {
            w = عارضی->والد->دایاں;

            اگر (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(عارضی->والد);
                ngx_rbtree_بایاں_rotate(جڑ, sentinel, عارضی->والد);
                w = عارضی->والد->دایاں;
            }

            اگر (ngx_rbt_is_black(w->بایاں) && ngx_rbt_is_black(w->دایاں)) {
                ngx_rbt_red(w);
                عارضی = عارضی->والد;

            } ورنہ {
                اگر (ngx_rbt_is_black(w->دایاں)) {
                    ngx_rbt_black(w->بایاں);
                    ngx_rbt_red(w);
                    ngx_rbtree_دایاں_rotate(جڑ, sentinel, w);
                    w = عارضی->والد->دایاں;
                }

                ngx_rbt_copy_color(w, عارضی->والد);
                ngx_rbt_black(عارضی->والد);
                ngx_rbt_black(w->دایاں);
                ngx_rbtree_بایاں_rotate(جڑ, sentinel, عارضی->والد);
                عارضی = *جڑ;
            }

        } ورنہ {
            w = عارضی->والد->بایاں;

            اگر (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(عارضی->والد);
                ngx_rbtree_دایاں_rotate(جڑ, sentinel, عارضی->والد);
                w = عارضی->والد->بایاں;
            }

            اگر (ngx_rbt_is_black(w->بایاں) && ngx_rbt_is_black(w->دایاں)) {
                ngx_rbt_red(w);
                عارضی = عارضی->والد;

            } ورنہ {
                اگر (ngx_rbt_is_black(w->بایاں)) {
                    ngx_rbt_black(w->دایاں);
                    ngx_rbt_red(w);
                    ngx_rbtree_بایاں_rotate(جڑ, sentinel, w);
                    w = عارضی->والد->بایاں;
                }

                ngx_rbt_copy_color(w, عارضی->والد);
                ngx_rbt_black(عارضی->والد);
                ngx_rbt_black(w->بایاں);
                ngx_rbtree_دایاں_rotate(جڑ, sentinel, عارضی->والد);
                عارضی = *جڑ;
            }
        }
    }

    ngx_rbt_black(عارضی);
}


ساکن ngx_inline خالی
ngx_rbtree_بایاں_rotate(ngx_rbtree_گرہ_t **جڑ, ngx_rbtree_گرہ_t *sentinel,
    ngx_rbtree_گرہ_t *گرہ)
{
    ngx_rbtree_گرہ_t  *عارضی;

    عارضی = گرہ->دایاں;
    گرہ->دایاں = عارضی->بایاں;

    اگر (عارضی->بایاں != sentinel) {
        عارضی->بایاں->والد = گرہ;
    }

    عارضی->والد = گرہ->والد;

    اگر (گرہ == *جڑ) {
        *جڑ = عارضی;

    } ورنہ اگر (گرہ == گرہ->والد->بایاں) {
        گرہ->والد->بایاں = عارضی;

    } ورنہ {
        گرہ->والد->دایاں = عارضی;
    }

    عارضی->بایاں = گرہ;
    گرہ->والد = عارضی;
}


ساکن ngx_inline خالی
ngx_rbtree_دایاں_rotate(ngx_rbtree_گرہ_t **جڑ, ngx_rbtree_گرہ_t *sentinel,
    ngx_rbtree_گرہ_t *گرہ)
{
    ngx_rbtree_گرہ_t  *عارضی;

    عارضی = گرہ->بایاں;
    گرہ->بایاں = عارضی->دایاں;

    اگر (عارضی->دایاں != sentinel) {
        عارضی->دایاں->والد = گرہ;
    }

    عارضی->والد = گرہ->والد;

    اگر (گرہ == *جڑ) {
        *جڑ = عارضی;

    } ورنہ اگر (گرہ == گرہ->والد->دایاں) {
        گرہ->والد->دایاں = عارضی;

    } ورنہ {
        گرہ->والد->بایاں = عارضی;
    }

    عارضی->دایاں = گرہ;
    گرہ->والد = عارضی;
}


ngx_rbtree_گرہ_t *
ngx_rbtree_اگلا(ngx_rbtree_t *درخت, ngx_rbtree_گرہ_t *گرہ)
{
    ngx_rbtree_گرہ_t  *جڑ, *sentinel, *والد;

    sentinel = درخت->sentinel;

    اگر (گرہ->دایاں != sentinel) {
        واپس ngx_rbtree_min(گرہ->دایاں, sentinel);
    }

    جڑ = درخت->جڑ;

    برائے ( ;; ) {
        والد = گرہ->والد;

        اگر (گرہ == جڑ) {
            واپس تہی;
        }

        اگر (گرہ == والد->بایاں) {
            واپس والد;
        }

        گرہ = والد;
    }
}
