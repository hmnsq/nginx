
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


ساكن ngx_inline فراغ ngx_rbtree_بایاں_rotate(ngx_rbtree_گرہ_t **جڑ,
    ngx_rbtree_گرہ_t *sentinel, ngx_rbtree_گرہ_t *گرہ);
ساكن ngx_inline فراغ ngx_rbtree_دایاں_rotate(ngx_rbtree_گرہ_t **جڑ,
    ngx_rbtree_گرہ_t *sentinel, ngx_rbtree_گرہ_t *گرہ);


فراغ
ngx_rbtree_داخل_کرو(ngx_rbtree_t *درخت, ngx_rbtree_گرہ_t *گرہ)
{
    ngx_rbtree_گرہ_t  **جڑ, *عارضی, *sentinel;

    /* a binary tree insert */

    جڑ = &درخت->جڑ;
    sentinel = درخت->sentinel;

    إذا (*جڑ == sentinel) {
        گرہ->ماپا = عدم;
        گرہ->بایاں = sentinel;
        گرہ->دایاں = sentinel;
        ngx_rbt_black(گرہ);
        *جڑ = گرہ;

        أرجع;
    }

    درخت->داخل_کرو(*جڑ, گرہ, sentinel);

    /* re-balance tree */

    طالما (گرہ != *جڑ && ngx_rbt_is_red(گرہ->ماپا)) {

        إذا (گرہ->ماپا == گرہ->ماپا->ماپا->بایاں) {
            عارضی = گرہ->ماپا->ماپا->دایاں;

            إذا (ngx_rbt_is_red(عارضی)) {
                ngx_rbt_black(گرہ->ماپا);
                ngx_rbt_black(عارضی);
                ngx_rbt_red(گرہ->ماپا->ماپا);
                گرہ = گرہ->ماپا->ماپا;

            } وإلا {
                إذا (گرہ == گرہ->ماپا->دایاں) {
                    گرہ = گرہ->ماپا;
                    ngx_rbtree_بایاں_rotate(جڑ, sentinel, گرہ);
                }

                ngx_rbt_black(گرہ->ماپا);
                ngx_rbt_red(گرہ->ماپا->ماپا);
                ngx_rbtree_دایاں_rotate(جڑ, sentinel, گرہ->ماپا->ماپا);
            }

        } وإلا {
            عارضی = گرہ->ماپا->ماپا->بایاں;

            إذا (ngx_rbt_is_red(عارضی)) {
                ngx_rbt_black(گرہ->ماپا);
                ngx_rbt_black(عارضی);
                ngx_rbt_red(گرہ->ماپا->ماپا);
                گرہ = گرہ->ماپا->ماپا;

            } وإلا {
                إذا (گرہ == گرہ->ماپا->بایاں) {
                    گرہ = گرہ->ماپا;
                    ngx_rbtree_دایاں_rotate(جڑ, sentinel, گرہ);
                }

                ngx_rbt_black(گرہ->ماپا);
                ngx_rbt_red(گرہ->ماپا->ماپا);
                ngx_rbtree_بایاں_rotate(جڑ, sentinel, گرہ->ماپا->ماپا);
            }
        }
    }

    ngx_rbt_black(*جڑ);
}


فراغ
ngx_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *عارضی, ngx_rbtree_گرہ_t *گرہ,
    ngx_rbtree_گرہ_t *sentinel)
{
    ngx_rbtree_گرہ_t  **p;

    لكل ( ;; ) {

        p = (گرہ->کلید < عارضی->کلید) ? &عارضی->بایاں : &عارضی->دایاں;

        إذا (*p == sentinel) {
            اكسر;
        }

        عارضی = *p;
    }

    *p = گرہ;
    گرہ->ماپا = عارضی;
    گرہ->بایاں = sentinel;
    گرہ->دایاں = sentinel;
    ngx_rbt_red(گرہ);
}


فراغ
ngx_rbtree_داخل_کرو_timer_قدر(ngx_rbtree_گرہ_t *عارضی, ngx_rbtree_گرہ_t *گرہ,
    ngx_rbtree_گرہ_t *sentinel)
{
    ngx_rbtree_گرہ_t  **p;

    لكل ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_کلید_int_t) (گرہ->کلید - عارضی->کلید) < 0)
            ? &عارضی->بایاں : &عارضی->دایاں;

        إذا (*p == sentinel) {
            اكسر;
        }

        عارضی = *p;
    }

    *p = گرہ;
    گرہ->ماپا = عارضی;
    گرہ->بایاں = sentinel;
    گرہ->دایاں = sentinel;
    ngx_rbt_red(گرہ);
}


فراغ
ngx_rbtree_delete(ngx_rbtree_t *درخت, ngx_rbtree_گرہ_t *گرہ)
{
    ngx_uint_t           red;
    ngx_rbtree_گرہ_t  **جڑ, *sentinel, *subst, *عارضی, *w;

    /* a binary tree delete */

    جڑ = &درخت->جڑ;
    sentinel = درخت->sentinel;

    إذا (گرہ->بایاں == sentinel) {
        عارضی = گرہ->دایاں;
        subst = گرہ;

    } وإلا إذا (گرہ->دایاں == sentinel) {
        عارضی = گرہ->بایاں;
        subst = گرہ;

    } وإلا {
        subst = ngx_rbtree_min(گرہ->دایاں, sentinel);
        عارضی = subst->دایاں;
    }

    إذا (subst == *جڑ) {
        *جڑ = عارضی;
        ngx_rbt_black(عارضی);

        /* DEBUG stuff */
        گرہ->بایاں = عدم;
        گرہ->دایاں = عدم;
        گرہ->ماپا = عدم;
        گرہ->کلید = 0;

        أرجع;
    }

    red = ngx_rbt_is_red(subst);

    إذا (subst == subst->ماپا->بایاں) {
        subst->ماپا->بایاں = عارضی;

    } وإلا {
        subst->ماپا->دایاں = عارضی;
    }

    إذا (subst == گرہ) {

        عارضی->ماپا = subst->ماپا;

    } وإلا {

        إذا (subst->ماپا == گرہ) {
            عارضی->ماپا = subst;

        } وإلا {
            عارضی->ماپا = subst->ماپا;
        }

        subst->بایاں = گرہ->بایاں;
        subst->دایاں = گرہ->دایاں;
        subst->ماپا = گرہ->ماپا;
        ngx_rbt_copy_color(subst, گرہ);

        إذا (گرہ == *جڑ) {
            *جڑ = subst;

        } وإلا {
            إذا (گرہ == گرہ->ماپا->بایاں) {
                گرہ->ماپا->بایاں = subst;
            } وإلا {
                گرہ->ماپا->دایاں = subst;
            }
        }

        إذا (subst->بایاں != sentinel) {
            subst->بایاں->ماپا = subst;
        }

        إذا (subst->دایاں != sentinel) {
            subst->دایاں->ماپا = subst;
        }
    }

    /* DEBUG stuff */
    گرہ->بایاں = عدم;
    گرہ->دایاں = عدم;
    گرہ->ماپا = عدم;
    گرہ->کلید = 0;

    إذا (red) {
        أرجع;
    }

    /* a delete fixup */

    طالما (عارضی != *جڑ && ngx_rbt_is_black(عارضی)) {

        إذا (عارضی == عارضی->ماپا->بایاں) {
            w = عارضی->ماپا->دایاں;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(عارضی->ماپا);
                ngx_rbtree_بایاں_rotate(جڑ, sentinel, عارضی->ماپا);
                w = عارضی->ماپا->دایاں;
            }

            إذا (ngx_rbt_is_black(w->بایاں) && ngx_rbt_is_black(w->دایاں)) {
                ngx_rbt_red(w);
                عارضی = عارضی->ماپا;

            } وإلا {
                إذا (ngx_rbt_is_black(w->دایاں)) {
                    ngx_rbt_black(w->بایاں);
                    ngx_rbt_red(w);
                    ngx_rbtree_دایاں_rotate(جڑ, sentinel, w);
                    w = عارضی->ماپا->دایاں;
                }

                ngx_rbt_copy_color(w, عارضی->ماپا);
                ngx_rbt_black(عارضی->ماپا);
                ngx_rbt_black(w->دایاں);
                ngx_rbtree_بایاں_rotate(جڑ, sentinel, عارضی->ماپا);
                عارضی = *جڑ;
            }

        } وإلا {
            w = عارضی->ماپا->بایاں;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(عارضی->ماپا);
                ngx_rbtree_دایاں_rotate(جڑ, sentinel, عارضی->ماپا);
                w = عارضی->ماپا->بایاں;
            }

            إذا (ngx_rbt_is_black(w->بایاں) && ngx_rbt_is_black(w->دایاں)) {
                ngx_rbt_red(w);
                عارضی = عارضی->ماپا;

            } وإلا {
                إذا (ngx_rbt_is_black(w->بایاں)) {
                    ngx_rbt_black(w->دایاں);
                    ngx_rbt_red(w);
                    ngx_rbtree_بایاں_rotate(جڑ, sentinel, w);
                    w = عارضی->ماپا->بایاں;
                }

                ngx_rbt_copy_color(w, عارضی->ماپا);
                ngx_rbt_black(عارضی->ماپا);
                ngx_rbt_black(w->بایاں);
                ngx_rbtree_دایاں_rotate(جڑ, sentinel, عارضی->ماپا);
                عارضی = *جڑ;
            }
        }
    }

    ngx_rbt_black(عارضی);
}


ساكن ngx_inline فراغ
ngx_rbtree_بایاں_rotate(ngx_rbtree_گرہ_t **جڑ, ngx_rbtree_گرہ_t *sentinel,
    ngx_rbtree_گرہ_t *گرہ)
{
    ngx_rbtree_گرہ_t  *عارضی;

    عارضی = گرہ->دایاں;
    گرہ->دایاں = عارضی->بایاں;

    إذا (عارضی->بایاں != sentinel) {
        عارضی->بایاں->ماپا = گرہ;
    }

    عارضی->ماپا = گرہ->ماپا;

    إذا (گرہ == *جڑ) {
        *جڑ = عارضی;

    } وإلا إذا (گرہ == گرہ->ماپا->بایاں) {
        گرہ->ماپا->بایاں = عارضی;

    } وإلا {
        گرہ->ماپا->دایاں = عارضی;
    }

    عارضی->بایاں = گرہ;
    گرہ->ماپا = عارضی;
}


ساكن ngx_inline فراغ
ngx_rbtree_دایاں_rotate(ngx_rbtree_گرہ_t **جڑ, ngx_rbtree_گرہ_t *sentinel,
    ngx_rbtree_گرہ_t *گرہ)
{
    ngx_rbtree_گرہ_t  *عارضی;

    عارضی = گرہ->بایاں;
    گرہ->بایاں = عارضی->دایاں;

    إذا (عارضی->دایاں != sentinel) {
        عارضی->دایاں->ماپا = گرہ;
    }

    عارضی->ماپا = گرہ->ماپا;

    إذا (گرہ == *جڑ) {
        *جڑ = عارضی;

    } وإلا إذا (گرہ == گرہ->ماپا->دایاں) {
        گرہ->ماپا->دایاں = عارضی;

    } وإلا {
        گرہ->ماپا->بایاں = عارضی;
    }

    عارضی->دایاں = گرہ;
    گرہ->ماپا = عارضی;
}


ngx_rbtree_گرہ_t *
ngx_rbtree_اگلا(ngx_rbtree_t *درخت, ngx_rbtree_گرہ_t *گرہ)
{
    ngx_rbtree_گرہ_t  *جڑ, *sentinel, *ماپا;

    sentinel = درخت->sentinel;

    إذا (گرہ->دایاں != sentinel) {
        أرجع ngx_rbtree_min(گرہ->دایاں, sentinel);
    }

    جڑ = درخت->جڑ;

    لكل ( ;; ) {
        ماپا = گرہ->ماپا;

        إذا (گرہ == جڑ) {
            أرجع عدم;
        }

        إذا (گرہ == ماپا->بایاں) {
            أرجع ماپا;
        }

        گرہ = ماپا;
    }
}
