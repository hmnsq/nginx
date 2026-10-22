
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


ساكن ngx_inline فراغ ngx_rbtree_چپ_rotate(ngx_rbtree_گره_t **ریشه,
    ngx_rbtree_گره_t *sentinel, ngx_rbtree_گره_t *گره);
ساكن ngx_inline فراغ ngx_rbtree_راست_rotate(ngx_rbtree_گره_t **ریشه,
    ngx_rbtree_گره_t *sentinel, ngx_rbtree_گره_t *گره);


فراغ
ngx_rbtree_درج(ngx_rbtree_t *درخت, ngx_rbtree_گره_t *گره)
{
    ngx_rbtree_گره_t  **ریشه, *موقت, *sentinel;

    /* a binary tree insert */

    ریشه = &درخت->ریشه;
    sentinel = درخت->sentinel;

    إذا (*ریشه == sentinel) {
        گره->والد = عدم;
        گره->چپ = sentinel;
        گره->راست = sentinel;
        ngx_rbt_black(گره);
        *ریشه = گره;

        أرجع;
    }

    درخت->درج(*ریشه, گره, sentinel);

    /* re-balance tree */

    طالما (گره != *ریشه && ngx_rbt_is_red(گره->والد)) {

        إذا (گره->والد == گره->والد->والد->چپ) {
            موقت = گره->والد->والد->راست;

            إذا (ngx_rbt_is_red(موقت)) {
                ngx_rbt_black(گره->والد);
                ngx_rbt_black(موقت);
                ngx_rbt_red(گره->والد->والد);
                گره = گره->والد->والد;

            } وإلا {
                إذا (گره == گره->والد->راست) {
                    گره = گره->والد;
                    ngx_rbtree_چپ_rotate(ریشه, sentinel, گره);
                }

                ngx_rbt_black(گره->والد);
                ngx_rbt_red(گره->والد->والد);
                ngx_rbtree_راست_rotate(ریشه, sentinel, گره->والد->والد);
            }

        } وإلا {
            موقت = گره->والد->والد->چپ;

            إذا (ngx_rbt_is_red(موقت)) {
                ngx_rbt_black(گره->والد);
                ngx_rbt_black(موقت);
                ngx_rbt_red(گره->والد->والد);
                گره = گره->والد->والد;

            } وإلا {
                إذا (گره == گره->والد->چپ) {
                    گره = گره->والد;
                    ngx_rbtree_راست_rotate(ریشه, sentinel, گره);
                }

                ngx_rbt_black(گره->والد);
                ngx_rbt_red(گره->والد->والد);
                ngx_rbtree_چپ_rotate(ریشه, sentinel, گره->والد->والد);
            }
        }
    }

    ngx_rbt_black(*ریشه);
}


فراغ
ngx_rbtree_درج_مقدار(ngx_rbtree_گره_t *موقت, ngx_rbtree_گره_t *گره,
    ngx_rbtree_گره_t *sentinel)
{
    ngx_rbtree_گره_t  **p;

    لكل ( ;; ) {

        p = (گره->کلید < موقت->کلید) ? &موقت->چپ : &موقت->راست;

        إذا (*p == sentinel) {
            اكسر;
        }

        موقت = *p;
    }

    *p = گره;
    گره->والد = موقت;
    گره->چپ = sentinel;
    گره->راست = sentinel;
    ngx_rbt_red(گره);
}


فراغ
ngx_rbtree_درج_timer_مقدار(ngx_rbtree_گره_t *موقت, ngx_rbtree_گره_t *گره,
    ngx_rbtree_گره_t *sentinel)
{
    ngx_rbtree_گره_t  **p;

    لكل ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_کلید_int_t) (گره->کلید - موقت->کلید) < 0)
            ? &موقت->چپ : &موقت->راست;

        إذا (*p == sentinel) {
            اكسر;
        }

        موقت = *p;
    }

    *p = گره;
    گره->والد = موقت;
    گره->چپ = sentinel;
    گره->راست = sentinel;
    ngx_rbt_red(گره);
}


فراغ
ngx_rbtree_delete(ngx_rbtree_t *درخت, ngx_rbtree_گره_t *گره)
{
    ngx_uint_t           red;
    ngx_rbtree_گره_t  **ریشه, *sentinel, *subst, *موقت, *w;

    /* a binary tree delete */

    ریشه = &درخت->ریشه;
    sentinel = درخت->sentinel;

    إذا (گره->چپ == sentinel) {
        موقت = گره->راست;
        subst = گره;

    } وإلا إذا (گره->راست == sentinel) {
        موقت = گره->چپ;
        subst = گره;

    } وإلا {
        subst = ngx_rbtree_min(گره->راست, sentinel);
        موقت = subst->راست;
    }

    إذا (subst == *ریشه) {
        *ریشه = موقت;
        ngx_rbt_black(موقت);

        /* DEBUG stuff */
        گره->چپ = عدم;
        گره->راست = عدم;
        گره->والد = عدم;
        گره->کلید = 0;

        أرجع;
    }

    red = ngx_rbt_is_red(subst);

    إذا (subst == subst->والد->چپ) {
        subst->والد->چپ = موقت;

    } وإلا {
        subst->والد->راست = موقت;
    }

    إذا (subst == گره) {

        موقت->والد = subst->والد;

    } وإلا {

        إذا (subst->والد == گره) {
            موقت->والد = subst;

        } وإلا {
            موقت->والد = subst->والد;
        }

        subst->چپ = گره->چپ;
        subst->راست = گره->راست;
        subst->والد = گره->والد;
        ngx_rbt_copy_color(subst, گره);

        إذا (گره == *ریشه) {
            *ریشه = subst;

        } وإلا {
            إذا (گره == گره->والد->چپ) {
                گره->والد->چپ = subst;
            } وإلا {
                گره->والد->راست = subst;
            }
        }

        إذا (subst->چپ != sentinel) {
            subst->چپ->والد = subst;
        }

        إذا (subst->راست != sentinel) {
            subst->راست->والد = subst;
        }
    }

    /* DEBUG stuff */
    گره->چپ = عدم;
    گره->راست = عدم;
    گره->والد = عدم;
    گره->کلید = 0;

    إذا (red) {
        أرجع;
    }

    /* a delete fixup */

    طالما (موقت != *ریشه && ngx_rbt_is_black(موقت)) {

        إذا (موقت == موقت->والد->چپ) {
            w = موقت->والد->راست;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(موقت->والد);
                ngx_rbtree_چپ_rotate(ریشه, sentinel, موقت->والد);
                w = موقت->والد->راست;
            }

            إذا (ngx_rbt_is_black(w->چپ) && ngx_rbt_is_black(w->راست)) {
                ngx_rbt_red(w);
                موقت = موقت->والد;

            } وإلا {
                إذا (ngx_rbt_is_black(w->راست)) {
                    ngx_rbt_black(w->چپ);
                    ngx_rbt_red(w);
                    ngx_rbtree_راست_rotate(ریشه, sentinel, w);
                    w = موقت->والد->راست;
                }

                ngx_rbt_copy_color(w, موقت->والد);
                ngx_rbt_black(موقت->والد);
                ngx_rbt_black(w->راست);
                ngx_rbtree_چپ_rotate(ریشه, sentinel, موقت->والد);
                موقت = *ریشه;
            }

        } وإلا {
            w = موقت->والد->چپ;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(موقت->والد);
                ngx_rbtree_راست_rotate(ریشه, sentinel, موقت->والد);
                w = موقت->والد->چپ;
            }

            إذا (ngx_rbt_is_black(w->چپ) && ngx_rbt_is_black(w->راست)) {
                ngx_rbt_red(w);
                موقت = موقت->والد;

            } وإلا {
                إذا (ngx_rbt_is_black(w->چپ)) {
                    ngx_rbt_black(w->راست);
                    ngx_rbt_red(w);
                    ngx_rbtree_چپ_rotate(ریشه, sentinel, w);
                    w = موقت->والد->چپ;
                }

                ngx_rbt_copy_color(w, موقت->والد);
                ngx_rbt_black(موقت->والد);
                ngx_rbt_black(w->چپ);
                ngx_rbtree_راست_rotate(ریشه, sentinel, موقت->والد);
                موقت = *ریشه;
            }
        }
    }

    ngx_rbt_black(موقت);
}


ساكن ngx_inline فراغ
ngx_rbtree_چپ_rotate(ngx_rbtree_گره_t **ریشه, ngx_rbtree_گره_t *sentinel,
    ngx_rbtree_گره_t *گره)
{
    ngx_rbtree_گره_t  *موقت;

    موقت = گره->راست;
    گره->راست = موقت->چپ;

    إذا (موقت->چپ != sentinel) {
        موقت->چپ->والد = گره;
    }

    موقت->والد = گره->والد;

    إذا (گره == *ریشه) {
        *ریشه = موقت;

    } وإلا إذا (گره == گره->والد->چپ) {
        گره->والد->چپ = موقت;

    } وإلا {
        گره->والد->راست = موقت;
    }

    موقت->چپ = گره;
    گره->والد = موقت;
}


ساكن ngx_inline فراغ
ngx_rbtree_راست_rotate(ngx_rbtree_گره_t **ریشه, ngx_rbtree_گره_t *sentinel,
    ngx_rbtree_گره_t *گره)
{
    ngx_rbtree_گره_t  *موقت;

    موقت = گره->چپ;
    گره->چپ = موقت->راست;

    إذا (موقت->راست != sentinel) {
        موقت->راست->والد = گره;
    }

    موقت->والد = گره->والد;

    إذا (گره == *ریشه) {
        *ریشه = موقت;

    } وإلا إذا (گره == گره->والد->راست) {
        گره->والد->راست = موقت;

    } وإلا {
        گره->والد->چپ = موقت;
    }

    موقت->راست = گره;
    گره->والد = موقت;
}


ngx_rbtree_گره_t *
ngx_rbtree_بعدی(ngx_rbtree_t *درخت, ngx_rbtree_گره_t *گره)
{
    ngx_rbtree_گره_t  *ریشه, *sentinel, *والد;

    sentinel = درخت->sentinel;

    إذا (گره->راست != sentinel) {
        أرجع ngx_rbtree_min(گره->راست, sentinel);
    }

    ریشه = درخت->ریشه;

    لكل ( ;; ) {
        والد = گره->والد;

        إذا (گره == ریشه) {
            أرجع عدم;
        }

        إذا (گره == والد->چپ) {
            أرجع والد;
        }

        گره = والد;
    }
}
