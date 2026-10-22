
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
    ngx_rbtree_گرہ_t  **جڑ, *temp, *sentinel;

    /* a binary tree insert */

    جڑ = &درخت->جڑ;
    sentinel = درخت->sentinel;

    إذا (*جڑ == sentinel) {
        گرہ->parent = عدم;
        گرہ->بایاں = sentinel;
        گرہ->دایاں = sentinel;
        ngx_rbt_black(گرہ);
        *جڑ = گرہ;

        أرجع;
    }

    درخت->داخل_کرو(*جڑ, گرہ, sentinel);

    /* re-balance tree */

    طالما (گرہ != *جڑ && ngx_rbt_is_red(گرہ->parent)) {

        إذا (گرہ->parent == گرہ->parent->parent->بایاں) {
            temp = گرہ->parent->parent->دایاں;

            إذا (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(گرہ->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(گرہ->parent->parent);
                گرہ = گرہ->parent->parent;

            } وإلا {
                إذا (گرہ == گرہ->parent->دایاں) {
                    گرہ = گرہ->parent;
                    ngx_rbtree_بایاں_rotate(جڑ, sentinel, گرہ);
                }

                ngx_rbt_black(گرہ->parent);
                ngx_rbt_red(گرہ->parent->parent);
                ngx_rbtree_دایاں_rotate(جڑ, sentinel, گرہ->parent->parent);
            }

        } وإلا {
            temp = گرہ->parent->parent->بایاں;

            إذا (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(گرہ->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(گرہ->parent->parent);
                گرہ = گرہ->parent->parent;

            } وإلا {
                إذا (گرہ == گرہ->parent->بایاں) {
                    گرہ = گرہ->parent;
                    ngx_rbtree_دایاں_rotate(جڑ, sentinel, گرہ);
                }

                ngx_rbt_black(گرہ->parent);
                ngx_rbt_red(گرہ->parent->parent);
                ngx_rbtree_بایاں_rotate(جڑ, sentinel, گرہ->parent->parent);
            }
        }
    }

    ngx_rbt_black(*جڑ);
}


فراغ
ngx_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *temp, ngx_rbtree_گرہ_t *گرہ,
    ngx_rbtree_گرہ_t *sentinel)
{
    ngx_rbtree_گرہ_t  **p;

    لكل ( ;; ) {

        p = (گرہ->کلید < temp->کلید) ? &temp->بایاں : &temp->دایاں;

        إذا (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = گرہ;
    گرہ->parent = temp;
    گرہ->بایاں = sentinel;
    گرہ->دایاں = sentinel;
    ngx_rbt_red(گرہ);
}


فراغ
ngx_rbtree_داخل_کرو_timer_قدر(ngx_rbtree_گرہ_t *temp, ngx_rbtree_گرہ_t *گرہ,
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

        p = ((ngx_rbtree_کلید_int_t) (گرہ->کلید - temp->کلید) < 0)
            ? &temp->بایاں : &temp->دایاں;

        إذا (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = گرہ;
    گرہ->parent = temp;
    گرہ->بایاں = sentinel;
    گرہ->دایاں = sentinel;
    ngx_rbt_red(گرہ);
}


فراغ
ngx_rbtree_delete(ngx_rbtree_t *درخت, ngx_rbtree_گرہ_t *گرہ)
{
    ngx_uint_t           red;
    ngx_rbtree_گرہ_t  **جڑ, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    جڑ = &درخت->جڑ;
    sentinel = درخت->sentinel;

    إذا (گرہ->بایاں == sentinel) {
        temp = گرہ->دایاں;
        subst = گرہ;

    } وإلا إذا (گرہ->دایاں == sentinel) {
        temp = گرہ->بایاں;
        subst = گرہ;

    } وإلا {
        subst = ngx_rbtree_min(گرہ->دایاں, sentinel);
        temp = subst->دایاں;
    }

    إذا (subst == *جڑ) {
        *جڑ = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        گرہ->بایاں = عدم;
        گرہ->دایاں = عدم;
        گرہ->parent = عدم;
        گرہ->کلید = 0;

        أرجع;
    }

    red = ngx_rbt_is_red(subst);

    إذا (subst == subst->parent->بایاں) {
        subst->parent->بایاں = temp;

    } وإلا {
        subst->parent->دایاں = temp;
    }

    إذا (subst == گرہ) {

        temp->parent = subst->parent;

    } وإلا {

        إذا (subst->parent == گرہ) {
            temp->parent = subst;

        } وإلا {
            temp->parent = subst->parent;
        }

        subst->بایاں = گرہ->بایاں;
        subst->دایاں = گرہ->دایاں;
        subst->parent = گرہ->parent;
        ngx_rbt_copy_color(subst, گرہ);

        إذا (گرہ == *جڑ) {
            *جڑ = subst;

        } وإلا {
            إذا (گرہ == گرہ->parent->بایاں) {
                گرہ->parent->بایاں = subst;
            } وإلا {
                گرہ->parent->دایاں = subst;
            }
        }

        إذا (subst->بایاں != sentinel) {
            subst->بایاں->parent = subst;
        }

        إذا (subst->دایاں != sentinel) {
            subst->دایاں->parent = subst;
        }
    }

    /* DEBUG stuff */
    گرہ->بایاں = عدم;
    گرہ->دایاں = عدم;
    گرہ->parent = عدم;
    گرہ->کلید = 0;

    إذا (red) {
        أرجع;
    }

    /* a delete fixup */

    طالما (temp != *جڑ && ngx_rbt_is_black(temp)) {

        إذا (temp == temp->parent->بایاں) {
            w = temp->parent->دایاں;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_بایاں_rotate(جڑ, sentinel, temp->parent);
                w = temp->parent->دایاں;
            }

            إذا (ngx_rbt_is_black(w->بایاں) && ngx_rbt_is_black(w->دایاں)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وإلا {
                إذا (ngx_rbt_is_black(w->دایاں)) {
                    ngx_rbt_black(w->بایاں);
                    ngx_rbt_red(w);
                    ngx_rbtree_دایاں_rotate(جڑ, sentinel, w);
                    w = temp->parent->دایاں;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->دایاں);
                ngx_rbtree_بایاں_rotate(جڑ, sentinel, temp->parent);
                temp = *جڑ;
            }

        } وإلا {
            w = temp->parent->بایاں;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_دایاں_rotate(جڑ, sentinel, temp->parent);
                w = temp->parent->بایاں;
            }

            إذا (ngx_rbt_is_black(w->بایاں) && ngx_rbt_is_black(w->دایاں)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وإلا {
                إذا (ngx_rbt_is_black(w->بایاں)) {
                    ngx_rbt_black(w->دایاں);
                    ngx_rbt_red(w);
                    ngx_rbtree_بایاں_rotate(جڑ, sentinel, w);
                    w = temp->parent->بایاں;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->بایاں);
                ngx_rbtree_دایاں_rotate(جڑ, sentinel, temp->parent);
                temp = *جڑ;
            }
        }
    }

    ngx_rbt_black(temp);
}


ساكن ngx_inline فراغ
ngx_rbtree_بایاں_rotate(ngx_rbtree_گرہ_t **جڑ, ngx_rbtree_گرہ_t *sentinel,
    ngx_rbtree_گرہ_t *گرہ)
{
    ngx_rbtree_گرہ_t  *temp;

    temp = گرہ->دایاں;
    گرہ->دایاں = temp->بایاں;

    إذا (temp->بایاں != sentinel) {
        temp->بایاں->parent = گرہ;
    }

    temp->parent = گرہ->parent;

    إذا (گرہ == *جڑ) {
        *جڑ = temp;

    } وإلا إذا (گرہ == گرہ->parent->بایاں) {
        گرہ->parent->بایاں = temp;

    } وإلا {
        گرہ->parent->دایاں = temp;
    }

    temp->بایاں = گرہ;
    گرہ->parent = temp;
}


ساكن ngx_inline فراغ
ngx_rbtree_دایاں_rotate(ngx_rbtree_گرہ_t **جڑ, ngx_rbtree_گرہ_t *sentinel,
    ngx_rbtree_گرہ_t *گرہ)
{
    ngx_rbtree_گرہ_t  *temp;

    temp = گرہ->بایاں;
    گرہ->بایاں = temp->دایاں;

    إذا (temp->دایاں != sentinel) {
        temp->دایاں->parent = گرہ;
    }

    temp->parent = گرہ->parent;

    إذا (گرہ == *جڑ) {
        *جڑ = temp;

    } وإلا إذا (گرہ == گرہ->parent->دایاں) {
        گرہ->parent->دایاں = temp;

    } وإلا {
        گرہ->parent->بایاں = temp;
    }

    temp->دایاں = گرہ;
    گرہ->parent = temp;
}


ngx_rbtree_گرہ_t *
ngx_rbtree_next(ngx_rbtree_t *درخت, ngx_rbtree_گرہ_t *گرہ)
{
    ngx_rbtree_گرہ_t  *جڑ, *sentinel, *parent;

    sentinel = درخت->sentinel;

    إذا (گرہ->دایاں != sentinel) {
        أرجع ngx_rbtree_min(گرہ->دایاں, sentinel);
    }

    جڑ = درخت->جڑ;

    لكل ( ;; ) {
        parent = گرہ->parent;

        إذا (گرہ == جڑ) {
            أرجع عدم;
        }

        إذا (گرہ == parent->بایاں) {
            أرجع parent;
        }

        گرہ = parent;
    }
}
