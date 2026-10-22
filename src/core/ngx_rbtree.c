
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


ایستا ngx_inline تهی ngx_rbtree_چپ_rotate(ngx_rbtree_گره_t **ریشه,
    ngx_rbtree_گره_t *sentinel, ngx_rbtree_گره_t *گره);
ایستا ngx_inline تهی ngx_rbtree_راست_rotate(ngx_rbtree_گره_t **ریشه,
    ngx_rbtree_گره_t *sentinel, ngx_rbtree_گره_t *گره);


تهی
ngx_rbtree_درج(ngx_rbtree_t *درخت, ngx_rbtree_گره_t *گره)
{
    ngx_rbtree_گره_t  **ریشه, *temp, *sentinel;

    /* a binary tree insert */

    ریشه = &درخت->ریشه;
    sentinel = درخت->sentinel;

    اگر (*ریشه == sentinel) {
        گره->parent = پوچ;
        گره->چپ = sentinel;
        گره->راست = sentinel;
        ngx_rbt_black(گره);
        *ریشه = گره;

        بازگشت;
    }

    درخت->درج(*ریشه, گره, sentinel);

    /* re-balance tree */

    وقتی (گره != *ریشه && ngx_rbt_is_red(گره->parent)) {

        اگر (گره->parent == گره->parent->parent->چپ) {
            temp = گره->parent->parent->راست;

            اگر (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(گره->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(گره->parent->parent);
                گره = گره->parent->parent;

            } وگرنه {
                اگر (گره == گره->parent->راست) {
                    گره = گره->parent;
                    ngx_rbtree_چپ_rotate(ریشه, sentinel, گره);
                }

                ngx_rbt_black(گره->parent);
                ngx_rbt_red(گره->parent->parent);
                ngx_rbtree_راست_rotate(ریشه, sentinel, گره->parent->parent);
            }

        } وگرنه {
            temp = گره->parent->parent->چپ;

            اگر (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(گره->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(گره->parent->parent);
                گره = گره->parent->parent;

            } وگرنه {
                اگر (گره == گره->parent->چپ) {
                    گره = گره->parent;
                    ngx_rbtree_راست_rotate(ریشه, sentinel, گره);
                }

                ngx_rbt_black(گره->parent);
                ngx_rbt_red(گره->parent->parent);
                ngx_rbtree_چپ_rotate(ریشه, sentinel, گره->parent->parent);
            }
        }
    }

    ngx_rbt_black(*ریشه);
}


تهی
ngx_rbtree_درج_مقدار(ngx_rbtree_گره_t *temp, ngx_rbtree_گره_t *گره,
    ngx_rbtree_گره_t *sentinel)
{
    ngx_rbtree_گره_t  **p;

    برای ( ;; ) {

        p = (گره->کلید < temp->کلید) ? &temp->چپ : &temp->راست;

        اگر (*p == sentinel) {
            بشکن;
        }

        temp = *p;
    }

    *p = گره;
    گره->parent = temp;
    گره->چپ = sentinel;
    گره->راست = sentinel;
    ngx_rbt_red(گره);
}


تهی
ngx_rbtree_درج_timer_مقدار(ngx_rbtree_گره_t *temp, ngx_rbtree_گره_t *گره,
    ngx_rbtree_گره_t *sentinel)
{
    ngx_rbtree_گره_t  **p;

    برای ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_کلید_int_t) (گره->کلید - temp->کلید) < 0)
            ? &temp->چپ : &temp->راست;

        اگر (*p == sentinel) {
            بشکن;
        }

        temp = *p;
    }

    *p = گره;
    گره->parent = temp;
    گره->چپ = sentinel;
    گره->راست = sentinel;
    ngx_rbt_red(گره);
}


تهی
ngx_rbtree_delete(ngx_rbtree_t *درخت, ngx_rbtree_گره_t *گره)
{
    ngx_uint_t           red;
    ngx_rbtree_گره_t  **ریشه, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    ریشه = &درخت->ریشه;
    sentinel = درخت->sentinel;

    اگر (گره->چپ == sentinel) {
        temp = گره->راست;
        subst = گره;

    } وگرنه اگر (گره->راست == sentinel) {
        temp = گره->چپ;
        subst = گره;

    } وگرنه {
        subst = ngx_rbtree_min(گره->راست, sentinel);
        temp = subst->راست;
    }

    اگر (subst == *ریشه) {
        *ریشه = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        گره->چپ = پوچ;
        گره->راست = پوچ;
        گره->parent = پوچ;
        گره->کلید = 0;

        بازگشت;
    }

    red = ngx_rbt_is_red(subst);

    اگر (subst == subst->parent->چپ) {
        subst->parent->چپ = temp;

    } وگرنه {
        subst->parent->راست = temp;
    }

    اگر (subst == گره) {

        temp->parent = subst->parent;

    } وگرنه {

        اگر (subst->parent == گره) {
            temp->parent = subst;

        } وگرنه {
            temp->parent = subst->parent;
        }

        subst->چپ = گره->چپ;
        subst->راست = گره->راست;
        subst->parent = گره->parent;
        ngx_rbt_copy_color(subst, گره);

        اگر (گره == *ریشه) {
            *ریشه = subst;

        } وگرنه {
            اگر (گره == گره->parent->چپ) {
                گره->parent->چپ = subst;
            } وگرنه {
                گره->parent->راست = subst;
            }
        }

        اگر (subst->چپ != sentinel) {
            subst->چپ->parent = subst;
        }

        اگر (subst->راست != sentinel) {
            subst->راست->parent = subst;
        }
    }

    /* DEBUG stuff */
    گره->چپ = پوچ;
    گره->راست = پوچ;
    گره->parent = پوچ;
    گره->کلید = 0;

    اگر (red) {
        بازگشت;
    }

    /* a delete fixup */

    وقتی (temp != *ریشه && ngx_rbt_is_black(temp)) {

        اگر (temp == temp->parent->چپ) {
            w = temp->parent->راست;

            اگر (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_چپ_rotate(ریشه, sentinel, temp->parent);
                w = temp->parent->راست;
            }

            اگر (ngx_rbt_is_black(w->چپ) && ngx_rbt_is_black(w->راست)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وگرنه {
                اگر (ngx_rbt_is_black(w->راست)) {
                    ngx_rbt_black(w->چپ);
                    ngx_rbt_red(w);
                    ngx_rbtree_راست_rotate(ریشه, sentinel, w);
                    w = temp->parent->راست;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->راست);
                ngx_rbtree_چپ_rotate(ریشه, sentinel, temp->parent);
                temp = *ریشه;
            }

        } وگرنه {
            w = temp->parent->چپ;

            اگر (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_راست_rotate(ریشه, sentinel, temp->parent);
                w = temp->parent->چپ;
            }

            اگر (ngx_rbt_is_black(w->چپ) && ngx_rbt_is_black(w->راست)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } وگرنه {
                اگر (ngx_rbt_is_black(w->چپ)) {
                    ngx_rbt_black(w->راست);
                    ngx_rbt_red(w);
                    ngx_rbtree_چپ_rotate(ریشه, sentinel, w);
                    w = temp->parent->چپ;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->چپ);
                ngx_rbtree_راست_rotate(ریشه, sentinel, temp->parent);
                temp = *ریشه;
            }
        }
    }

    ngx_rbt_black(temp);
}


ایستا ngx_inline تهی
ngx_rbtree_چپ_rotate(ngx_rbtree_گره_t **ریشه, ngx_rbtree_گره_t *sentinel,
    ngx_rbtree_گره_t *گره)
{
    ngx_rbtree_گره_t  *temp;

    temp = گره->راست;
    گره->راست = temp->چپ;

    اگر (temp->چپ != sentinel) {
        temp->چپ->parent = گره;
    }

    temp->parent = گره->parent;

    اگر (گره == *ریشه) {
        *ریشه = temp;

    } وگرنه اگر (گره == گره->parent->چپ) {
        گره->parent->چپ = temp;

    } وگرنه {
        گره->parent->راست = temp;
    }

    temp->چپ = گره;
    گره->parent = temp;
}


ایستا ngx_inline تهی
ngx_rbtree_راست_rotate(ngx_rbtree_گره_t **ریشه, ngx_rbtree_گره_t *sentinel,
    ngx_rbtree_گره_t *گره)
{
    ngx_rbtree_گره_t  *temp;

    temp = گره->چپ;
    گره->چپ = temp->راست;

    اگر (temp->راست != sentinel) {
        temp->راست->parent = گره;
    }

    temp->parent = گره->parent;

    اگر (گره == *ریشه) {
        *ریشه = temp;

    } وگرنه اگر (گره == گره->parent->راست) {
        گره->parent->راست = temp;

    } وگرنه {
        گره->parent->چپ = temp;
    }

    temp->راست = گره;
    گره->parent = temp;
}


ngx_rbtree_گره_t *
ngx_rbtree_next(ngx_rbtree_t *درخت, ngx_rbtree_گره_t *گره)
{
    ngx_rbtree_گره_t  *ریشه, *sentinel, *parent;

    sentinel = درخت->sentinel;

    اگر (گره->راست != sentinel) {
        بازگشت ngx_rbtree_min(گره->راست, sentinel);
    }

    ریشه = درخت->ریشه;

    برای ( ;; ) {
        parent = گره->parent;

        اگر (گره == ریشه) {
            بازگشت پوچ;
        }

        اگر (گره == parent->چپ) {
            بازگشت parent;
        }

        گره = parent;
    }
}
