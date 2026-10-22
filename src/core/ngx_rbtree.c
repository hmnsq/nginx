
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


ساكن ngx_inline فراغ ngx_rbtree_کیڼ_rotate(ngx_rbtree_غوټه_t **ریښه,
    ngx_rbtree_غوټه_t *sentinel, ngx_rbtree_غوټه_t *غوټه);
ساكن ngx_inline فراغ ngx_rbtree_ښی_rotate(ngx_rbtree_غوټه_t **ریښه,
    ngx_rbtree_غوټه_t *sentinel, ngx_rbtree_غوټه_t *غوټه);


فراغ
ngx_rbtree_ورننباسه(ngx_rbtree_t *ونه, ngx_rbtree_غوټه_t *غوټه)
{
    ngx_rbtree_غوټه_t  **ریښه, *لنډمهاله, *sentinel;

    /* a binary tree insert */

    ریښه = &ونه->ریښه;
    sentinel = ونه->sentinel;

    إذا (*ریښه == sentinel) {
        غوټه->پلار = عدم;
        غوټه->کیڼ = sentinel;
        غوټه->ښی = sentinel;
        ngx_rbt_black(غوټه);
        *ریښه = غوټه;

        أرجع;
    }

    ونه->ورننباسه(*ریښه, غوټه, sentinel);

    /* re-balance tree */

    طالما (غوټه != *ریښه && ngx_rbt_is_red(غوټه->پلار)) {

        إذا (غوټه->پلار == غوټه->پلار->پلار->کیڼ) {
            لنډمهاله = غوټه->پلار->پلار->ښی;

            إذا (ngx_rbt_is_red(لنډمهاله)) {
                ngx_rbt_black(غوټه->پلار);
                ngx_rbt_black(لنډمهاله);
                ngx_rbt_red(غوټه->پلار->پلار);
                غوټه = غوټه->پلار->پلار;

            } وإلا {
                إذا (غوټه == غوټه->پلار->ښی) {
                    غوټه = غوټه->پلار;
                    ngx_rbtree_کیڼ_rotate(ریښه, sentinel, غوټه);
                }

                ngx_rbt_black(غوټه->پلار);
                ngx_rbt_red(غوټه->پلار->پلار);
                ngx_rbtree_ښی_rotate(ریښه, sentinel, غوټه->پلار->پلار);
            }

        } وإلا {
            لنډمهاله = غوټه->پلار->پلار->کیڼ;

            إذا (ngx_rbt_is_red(لنډمهاله)) {
                ngx_rbt_black(غوټه->پلار);
                ngx_rbt_black(لنډمهاله);
                ngx_rbt_red(غوټه->پلار->پلار);
                غوټه = غوټه->پلار->پلار;

            } وإلا {
                إذا (غوټه == غوټه->پلار->کیڼ) {
                    غوټه = غوټه->پلار;
                    ngx_rbtree_ښی_rotate(ریښه, sentinel, غوټه);
                }

                ngx_rbt_black(غوټه->پلار);
                ngx_rbt_red(غوټه->پلار->پلار);
                ngx_rbtree_کیڼ_rotate(ریښه, sentinel, غوټه->پلار->پلار);
            }
        }
    }

    ngx_rbt_black(*ریښه);
}


فراغ
ngx_rbtree_ورننباسه_ارزښت(ngx_rbtree_غوټه_t *لنډمهاله, ngx_rbtree_غوټه_t *غوټه,
    ngx_rbtree_غوټه_t *sentinel)
{
    ngx_rbtree_غوټه_t  **p;

    لكل ( ;; ) {

        p = (غوټه->کلی < لنډمهاله->کلی) ? &لنډمهاله->کیڼ : &لنډمهاله->ښی;

        إذا (*p == sentinel) {
            اكسر;
        }

        لنډمهاله = *p;
    }

    *p = غوټه;
    غوټه->پلار = لنډمهاله;
    غوټه->کیڼ = sentinel;
    غوټه->ښی = sentinel;
    ngx_rbt_red(غوټه);
}


فراغ
ngx_rbtree_ورننباسه_timer_ارزښت(ngx_rbtree_غوټه_t *لنډمهاله, ngx_rbtree_غوټه_t *غوټه,
    ngx_rbtree_غوټه_t *sentinel)
{
    ngx_rbtree_غوټه_t  **p;

    لكل ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_کلی_int_t) (غوټه->کلی - لنډمهاله->کلی) < 0)
            ? &لنډمهاله->کیڼ : &لنډمهاله->ښی;

        إذا (*p == sentinel) {
            اكسر;
        }

        لنډمهاله = *p;
    }

    *p = غوټه;
    غوټه->پلار = لنډمهاله;
    غوټه->کیڼ = sentinel;
    غوټه->ښی = sentinel;
    ngx_rbt_red(غوټه);
}


فراغ
ngx_rbtree_delete(ngx_rbtree_t *ونه, ngx_rbtree_غوټه_t *غوټه)
{
    ngx_uint_t           red;
    ngx_rbtree_غوټه_t  **ریښه, *sentinel, *subst, *لنډمهاله, *w;

    /* a binary tree delete */

    ریښه = &ونه->ریښه;
    sentinel = ونه->sentinel;

    إذا (غوټه->کیڼ == sentinel) {
        لنډمهاله = غوټه->ښی;
        subst = غوټه;

    } وإلا إذا (غوټه->ښی == sentinel) {
        لنډمهاله = غوټه->کیڼ;
        subst = غوټه;

    } وإلا {
        subst = ngx_rbtree_min(غوټه->ښی, sentinel);
        لنډمهاله = subst->ښی;
    }

    إذا (subst == *ریښه) {
        *ریښه = لنډمهاله;
        ngx_rbt_black(لنډمهاله);

        /* DEBUG stuff */
        غوټه->کیڼ = عدم;
        غوټه->ښی = عدم;
        غوټه->پلار = عدم;
        غوټه->کلی = 0;

        أرجع;
    }

    red = ngx_rbt_is_red(subst);

    إذا (subst == subst->پلار->کیڼ) {
        subst->پلار->کیڼ = لنډمهاله;

    } وإلا {
        subst->پلار->ښی = لنډمهاله;
    }

    إذا (subst == غوټه) {

        لنډمهاله->پلار = subst->پلار;

    } وإلا {

        إذا (subst->پلار == غوټه) {
            لنډمهاله->پلار = subst;

        } وإلا {
            لنډمهاله->پلار = subst->پلار;
        }

        subst->کیڼ = غوټه->کیڼ;
        subst->ښی = غوټه->ښی;
        subst->پلار = غوټه->پلار;
        ngx_rbt_copy_color(subst, غوټه);

        إذا (غوټه == *ریښه) {
            *ریښه = subst;

        } وإلا {
            إذا (غوټه == غوټه->پلار->کیڼ) {
                غوټه->پلار->کیڼ = subst;
            } وإلا {
                غوټه->پلار->ښی = subst;
            }
        }

        إذا (subst->کیڼ != sentinel) {
            subst->کیڼ->پلار = subst;
        }

        إذا (subst->ښی != sentinel) {
            subst->ښی->پلار = subst;
        }
    }

    /* DEBUG stuff */
    غوټه->کیڼ = عدم;
    غوټه->ښی = عدم;
    غوټه->پلار = عدم;
    غوټه->کلی = 0;

    إذا (red) {
        أرجع;
    }

    /* a delete fixup */

    طالما (لنډمهاله != *ریښه && ngx_rbt_is_black(لنډمهاله)) {

        إذا (لنډمهاله == لنډمهاله->پلار->کیڼ) {
            w = لنډمهاله->پلار->ښی;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(لنډمهاله->پلار);
                ngx_rbtree_کیڼ_rotate(ریښه, sentinel, لنډمهاله->پلار);
                w = لنډمهاله->پلار->ښی;
            }

            إذا (ngx_rbt_is_black(w->کیڼ) && ngx_rbt_is_black(w->ښی)) {
                ngx_rbt_red(w);
                لنډمهاله = لنډمهاله->پلار;

            } وإلا {
                إذا (ngx_rbt_is_black(w->ښی)) {
                    ngx_rbt_black(w->کیڼ);
                    ngx_rbt_red(w);
                    ngx_rbtree_ښی_rotate(ریښه, sentinel, w);
                    w = لنډمهاله->پلار->ښی;
                }

                ngx_rbt_copy_color(w, لنډمهاله->پلار);
                ngx_rbt_black(لنډمهاله->پلار);
                ngx_rbt_black(w->ښی);
                ngx_rbtree_کیڼ_rotate(ریښه, sentinel, لنډمهاله->پلار);
                لنډمهاله = *ریښه;
            }

        } وإلا {
            w = لنډمهاله->پلار->کیڼ;

            إذا (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(لنډمهاله->پلار);
                ngx_rbtree_ښی_rotate(ریښه, sentinel, لنډمهاله->پلار);
                w = لنډمهاله->پلار->کیڼ;
            }

            إذا (ngx_rbt_is_black(w->کیڼ) && ngx_rbt_is_black(w->ښی)) {
                ngx_rbt_red(w);
                لنډمهاله = لنډمهاله->پلار;

            } وإلا {
                إذا (ngx_rbt_is_black(w->کیڼ)) {
                    ngx_rbt_black(w->ښی);
                    ngx_rbt_red(w);
                    ngx_rbtree_کیڼ_rotate(ریښه, sentinel, w);
                    w = لنډمهاله->پلار->کیڼ;
                }

                ngx_rbt_copy_color(w, لنډمهاله->پلار);
                ngx_rbt_black(لنډمهاله->پلار);
                ngx_rbt_black(w->کیڼ);
                ngx_rbtree_ښی_rotate(ریښه, sentinel, لنډمهاله->پلار);
                لنډمهاله = *ریښه;
            }
        }
    }

    ngx_rbt_black(لنډمهاله);
}


ساكن ngx_inline فراغ
ngx_rbtree_کیڼ_rotate(ngx_rbtree_غوټه_t **ریښه, ngx_rbtree_غوټه_t *sentinel,
    ngx_rbtree_غوټه_t *غوټه)
{
    ngx_rbtree_غوټه_t  *لنډمهاله;

    لنډمهاله = غوټه->ښی;
    غوټه->ښی = لنډمهاله->کیڼ;

    إذا (لنډمهاله->کیڼ != sentinel) {
        لنډمهاله->کیڼ->پلار = غوټه;
    }

    لنډمهاله->پلار = غوټه->پلار;

    إذا (غوټه == *ریښه) {
        *ریښه = لنډمهاله;

    } وإلا إذا (غوټه == غوټه->پلار->کیڼ) {
        غوټه->پلار->کیڼ = لنډمهاله;

    } وإلا {
        غوټه->پلار->ښی = لنډمهاله;
    }

    لنډمهاله->کیڼ = غوټه;
    غوټه->پلار = لنډمهاله;
}


ساكن ngx_inline فراغ
ngx_rbtree_ښی_rotate(ngx_rbtree_غوټه_t **ریښه, ngx_rbtree_غوټه_t *sentinel,
    ngx_rbtree_غوټه_t *غوټه)
{
    ngx_rbtree_غوټه_t  *لنډمهاله;

    لنډمهاله = غوټه->کیڼ;
    غوټه->کیڼ = لنډمهاله->ښی;

    إذا (لنډمهاله->ښی != sentinel) {
        لنډمهاله->ښی->پلار = غوټه;
    }

    لنډمهاله->پلار = غوټه->پلار;

    إذا (غوټه == *ریښه) {
        *ریښه = لنډمهاله;

    } وإلا إذا (غوټه == غوټه->پلار->ښی) {
        غوټه->پلار->ښی = لنډمهاله;

    } وإلا {
        غوټه->پلار->کیڼ = لنډمهاله;
    }

    لنډمهاله->ښی = غوټه;
    غوټه->پلار = لنډمهاله;
}


ngx_rbtree_غوټه_t *
ngx_rbtree_بل(ngx_rbtree_t *ونه, ngx_rbtree_غوټه_t *غوټه)
{
    ngx_rbtree_غوټه_t  *ریښه, *sentinel, *پلار;

    sentinel = ونه->sentinel;

    إذا (غوټه->ښی != sentinel) {
        أرجع ngx_rbtree_min(غوټه->ښی, sentinel);
    }

    ریښه = ونه->ریښه;

    لكل ( ;; ) {
        پلار = غوټه->پلار;

        إذا (غوټه == ریښه) {
            أرجع عدم;
        }

        إذا (غوټه == پلار->کیڼ) {
            أرجع پلار;
        }

        غوټه = پلار;
    }
}
