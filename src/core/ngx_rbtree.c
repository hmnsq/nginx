
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


ساكن ngx_inline فراغ ngx_rbtree_چەپ_rotate(ngx_rbtree_گرێ_t **ڕەگ,
    ngx_rbtree_گرێ_t *sentinel, ngx_rbtree_گرێ_t *گرێ);
ساكن ngx_inline فراغ ngx_rbtree_ڕاست_rotate(ngx_rbtree_گرێ_t **ڕەگ,
    ngx_rbtree_گرێ_t *sentinel, ngx_rbtree_گرێ_t *گرێ);


فراغ
ngx_rbtree_زیادکردن(ngx_rbtree_t *درەخت, ngx_rbtree_گرێ_t *گرێ)
{
    ngx_rbtree_گرێ_t  **ڕەگ, *temp, *sentinel;

    /* a binary tree insert */

    ڕەگ = &درەخت->ڕەگ;
    sentinel = درەخت->sentinel;

    إذا (*ڕەگ == sentinel) {
        گرێ->parent = عدم;
        گرێ->چەپ = sentinel;
        گرێ->ڕاست = sentinel;
        ngx_rbt_black(گرێ);
        *ڕەگ = گرێ;

        أرجع;
    }

    درەخت->زیادکردن(*ڕەگ, گرێ, sentinel);

    /* re-balance tree */

    طالما (گرێ != *ڕەگ && ngx_rbt_is_red(گرێ->parent)) {

        إذا (گرێ->parent == گرێ->parent->parent->چەپ) {
            temp = گرێ->parent->parent->ڕاست;

            إذا (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(گرێ->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(گرێ->parent->parent);
                گرێ = گرێ->parent->parent;

            } وإلا {
                إذا (گرێ == گرێ->parent->ڕاست) {
                    گرێ = گرێ->parent;
                    ngx_rbtree_چەپ_rotate(ڕەگ, sentinel, گرێ);
                }

                ngx_rbt_black(گرێ->parent);
                ngx_rbt_red(گرێ->parent->parent);
                ngx_rbtree_ڕاست_rotate(ڕەگ, sentinel, گرێ->parent->parent);
            }

        } وإلا {
            temp = گرێ->parent->parent->چەپ;

            إذا (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(گرێ->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(گرێ->parent->parent);
                گرێ = گرێ->parent->parent;

            } وإلا {
                إذا (گرێ == گرێ->parent->چەپ) {
                    گرێ = گرێ->parent;
                    ngx_rbtree_ڕاست_rotate(ڕەگ, sentinel, گرێ);
                }

                ngx_rbt_black(گرێ->parent);
                ngx_rbt_red(گرێ->parent->parent);
                ngx_rbtree_چەپ_rotate(ڕەگ, sentinel, گرێ->parent->parent);
            }
        }
    }

    ngx_rbt_black(*ڕەگ);
}


فراغ
ngx_rbtree_زیادکردن_بەها(ngx_rbtree_گرێ_t *temp, ngx_rbtree_گرێ_t *گرێ,
    ngx_rbtree_گرێ_t *sentinel)
{
    ngx_rbtree_گرێ_t  **p;

    لكل ( ;; ) {

        p = (گرێ->کلیل < temp->کلیل) ? &temp->چەپ : &temp->ڕاست;

        إذا (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = گرێ;
    گرێ->parent = temp;
    گرێ->چەپ = sentinel;
    گرێ->ڕاست = sentinel;
    ngx_rbt_red(گرێ);
}


فراغ
ngx_rbtree_زیادکردن_timer_بەها(ngx_rbtree_گرێ_t *temp, ngx_rbtree_گرێ_t *گرێ,
    ngx_rbtree_گرێ_t *sentinel)
{
    ngx_rbtree_گرێ_t  **p;

    لكل ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_کلیل_int_t) (گرێ->کلیل - temp->کلیل) < 0)
            ? &temp->چەپ : &temp->ڕاست;

        إذا (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = گرێ;
    گرێ->parent = temp;
    گرێ->چەپ = sentinel;
    گرێ->ڕاست = sentinel;
    ngx_rbt_red(گرێ);
}


فراغ
ngx_rbtree_delete(ngx_rbtree_t *درەخت, ngx_rbtree_گرێ_t *گرێ)
{
    ngx_uint_t           red;
    ngx_rbtree_گرێ_t  **ڕەگ, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    ڕەگ = &درەخت->ڕەگ;
    sentinel = درەخت->sentinel;

    إذا (گرێ->چەپ == sentinel) {
        temp = گرێ->ڕاست;
        subst = گرێ;

    } وإلا إذا (گرێ->ڕاست == sentinel) {
        temp = گرێ->چەپ;
        subst = گرێ;

    } وإلا {
        subst = ngx_rbtree_min(گرێ->ڕاست, sentinel);
        temp = subst->ڕاست;
    }

    إذا (subst == *ڕەگ) {
        *ڕەگ = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        گرێ->چەپ = عدم;
        گرێ->ڕاست = عدم;
        گرێ->parent = عدم;
        گرێ->کلیل = 0;

        أرجع;
    }

    red = ngx_rbt_is_red(subst);

    إذا (subst == subst->parent->چەپ) {
        subst->parent->چەپ = temp;

    } وإلا {
        subst->parent->ڕاست = temp;
    }

    إذا (subst == گرێ) {

        temp->parent = subst->parent;

    } وإلا {

        إذا (subst->parent == گرێ) {
            temp->parent = subst;

        } وإلا {
            temp->parent = subst->parent;
        }

        subst->چەپ = گرێ->چەپ;
        subst->ڕاست = گرێ->ڕاست;
        subst->parent = گرێ->parent;
        ngx_rbt_copy_color(subst, گرێ);

        إذا (گرێ == *ڕەگ) {
            *ڕەگ = subst;

        } وإلا {
            إذا (گرێ == گرێ->parent->چەپ) {
                گرێ->parent->چەپ = subst;
            } وإلا {
                گرێ->parent->ڕاست = subst;
            }
        }

        إذا (subst->چەپ != sentinel) {
            subst->چەپ->parent = subst;
        }

        إذا (subst->ڕاست != sentinel) {
            subst->ڕاست->parent = subst;
        }
    }

    /* DEBUG stuff */
    گرێ->چەپ = عدم;
    گرێ->ڕاست = عدم;
    گرێ->parent = عدم;
    گرێ->کلیل = 0;

    إذا (red) {
        أرجع;
    }

    /* a delete fixup */

    طالما (temp != *ڕەگ && ngx_rbt_is_black(temp)) {

        إذا (temp == temp->parent->چەپ) {
            w = temp->parent->ڕاست;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_چەپ_rotate(ڕەگ, sentinel, temp->parent);
                w = temp->parent->ڕاست;
            }

            إذا (ngx_rbt_is_black(w->چەپ) && ngx_rbt_is_black(w->ڕاست)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وإلا {
                إذا (ngx_rbt_is_black(w->ڕاست)) {
                    ngx_rbt_black(w->چەپ);
                    ngx_rbt_red(w);
                    ngx_rbtree_ڕاست_rotate(ڕەگ, sentinel, w);
                    w = temp->parent->ڕاست;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->ڕاست);
                ngx_rbtree_چەپ_rotate(ڕەگ, sentinel, temp->parent);
                temp = *ڕەگ;
            }

        } وإلا {
            w = temp->parent->چەپ;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_ڕاست_rotate(ڕەگ, sentinel, temp->parent);
                w = temp->parent->چەپ;
            }

            إذا (ngx_rbt_is_black(w->چەپ) && ngx_rbt_is_black(w->ڕاست)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وإلا {
                إذا (ngx_rbt_is_black(w->چەپ)) {
                    ngx_rbt_black(w->ڕاست);
                    ngx_rbt_red(w);
                    ngx_rbtree_چەپ_rotate(ڕەگ, sentinel, w);
                    w = temp->parent->چەپ;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->چەپ);
                ngx_rbtree_ڕاست_rotate(ڕەگ, sentinel, temp->parent);
                temp = *ڕەگ;
            }
        }
    }

    ngx_rbt_black(temp);
}


ساكن ngx_inline فراغ
ngx_rbtree_چەپ_rotate(ngx_rbtree_گرێ_t **ڕەگ, ngx_rbtree_گرێ_t *sentinel,
    ngx_rbtree_گرێ_t *گرێ)
{
    ngx_rbtree_گرێ_t  *temp;

    temp = گرێ->ڕاست;
    گرێ->ڕاست = temp->چەپ;

    إذا (temp->چەپ != sentinel) {
        temp->چەپ->parent = گرێ;
    }

    temp->parent = گرێ->parent;

    إذا (گرێ == *ڕەگ) {
        *ڕەگ = temp;

    } وإلا إذا (گرێ == گرێ->parent->چەپ) {
        گرێ->parent->چەپ = temp;

    } وإلا {
        گرێ->parent->ڕاست = temp;
    }

    temp->چەپ = گرێ;
    گرێ->parent = temp;
}


ساكن ngx_inline فراغ
ngx_rbtree_ڕاست_rotate(ngx_rbtree_گرێ_t **ڕەگ, ngx_rbtree_گرێ_t *sentinel,
    ngx_rbtree_گرێ_t *گرێ)
{
    ngx_rbtree_گرێ_t  *temp;

    temp = گرێ->چەپ;
    گرێ->چەپ = temp->ڕاست;

    إذا (temp->ڕاست != sentinel) {
        temp->ڕاست->parent = گرێ;
    }

    temp->parent = گرێ->parent;

    إذا (گرێ == *ڕەگ) {
        *ڕەگ = temp;

    } وإلا إذا (گرێ == گرێ->parent->ڕاست) {
        گرێ->parent->ڕاست = temp;

    } وإلا {
        گرێ->parent->چەپ = temp;
    }

    temp->ڕاست = گرێ;
    گرێ->parent = temp;
}


ngx_rbtree_گرێ_t *
ngx_rbtree_next(ngx_rbtree_t *درەخت, ngx_rbtree_گرێ_t *گرێ)
{
    ngx_rbtree_گرێ_t  *ڕەگ, *sentinel, *parent;

    sentinel = درەخت->sentinel;

    إذا (گرێ->ڕاست != sentinel) {
        أرجع ngx_rbtree_min(گرێ->ڕاست, sentinel);
    }

    ڕەگ = درەخت->ڕەگ;

    لكل ( ;; ) {
        parent = گرێ->parent;

        إذا (گرێ == ڕەگ) {
            أرجع عدم;
        }

        إذا (گرێ == parent->چەپ) {
            أرجع parent;
        }

        گرێ = parent;
    }
}
