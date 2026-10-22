
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


ସ୍ଥିର ngx_inline ଶୂନ୍ୟ ngx_rbtree_ବାମ_rotate(ngx_rbtree_ଗଣ୍ଠି_t **ମୂଳ,
    ngx_rbtree_ଗଣ୍ଠି_t *sentinel, ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି);
ସ୍ଥିର ngx_inline ଶୂନ୍ୟ ngx_rbtree_ଡାହାଣ_rotate(ngx_rbtree_ଗଣ୍ଠି_t **ମୂଳ,
    ngx_rbtree_ଗଣ୍ଠି_t *sentinel, ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି);


ଶୂନ୍ୟ
ngx_rbtree_ଭର୍ତ୍ତି(ngx_rbtree_t *ବୃକ୍ଷ, ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି)
{
    ngx_rbtree_ଗଣ୍ଠି_t  **ମୂଳ, *temp, *sentinel;

    /* a binary tree insert */

    ମୂଳ = &ବୃକ୍ଷ->ମୂଳ;
    sentinel = ବୃକ୍ଷ->sentinel;

    ଯଦି (*ମୂଳ == sentinel) {
        ଗଣ୍ଠି->parent = ଖାଲି;
        ଗଣ୍ଠି->ବାମ = sentinel;
        ଗଣ୍ଠି->ଡାହାଣ = sentinel;
        ngx_rbt_black(ଗଣ୍ଠି);
        *ମୂଳ = ଗଣ୍ଠି;

        ଫେରନ୍ତୁ;
    }

    ବୃକ୍ଷ->ଭର୍ତ୍ତି(*ମୂଳ, ଗଣ୍ଠି, sentinel);

    /* re-balance tree */

    ଯେତେବେଳେ (ଗଣ୍ଠି != *ମୂଳ && ngx_rbt_is_red(ଗଣ୍ଠି->parent)) {

        ଯଦି (ଗଣ୍ଠି->parent == ଗଣ୍ଠି->parent->parent->ବାମ) {
            temp = ଗଣ୍ଠି->parent->parent->ଡାହାଣ;

            ଯଦି (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(ଗଣ୍ଠି->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(ଗଣ୍ଠି->parent->parent);
                ଗଣ୍ଠି = ଗଣ୍ଠି->parent->parent;

            } ଅନ୍ୟଥା {
                ଯଦି (ଗଣ୍ଠି == ଗଣ୍ଠି->parent->ଡାହାଣ) {
                    ଗଣ୍ଠି = ଗଣ୍ଠି->parent;
                    ngx_rbtree_ବାମ_rotate(ମୂଳ, sentinel, ଗଣ୍ଠି);
                }

                ngx_rbt_black(ଗଣ୍ଠି->parent);
                ngx_rbt_red(ଗଣ୍ଠି->parent->parent);
                ngx_rbtree_ଡାହାଣ_rotate(ମୂଳ, sentinel, ଗଣ୍ଠି->parent->parent);
            }

        } ଅନ୍ୟଥା {
            temp = ଗଣ୍ଠି->parent->parent->ବାମ;

            ଯଦି (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(ଗଣ୍ଠି->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(ଗଣ୍ଠି->parent->parent);
                ଗଣ୍ଠି = ଗଣ୍ଠି->parent->parent;

            } ଅନ୍ୟଥା {
                ଯଦି (ଗଣ୍ଠି == ଗଣ୍ଠି->parent->ବାମ) {
                    ଗଣ୍ଠି = ଗଣ୍ଠି->parent;
                    ngx_rbtree_ଡାହାଣ_rotate(ମୂଳ, sentinel, ଗଣ୍ଠି);
                }

                ngx_rbt_black(ଗଣ୍ଠି->parent);
                ngx_rbt_red(ଗଣ୍ଠି->parent->parent);
                ngx_rbtree_ବାମ_rotate(ମୂଳ, sentinel, ଗଣ୍ଠି->parent->parent);
            }
        }
    }

    ngx_rbt_black(*ମୂଳ);
}


ଶୂନ୍ୟ
ngx_rbtree_ଭର୍ତ୍ତି_ମୂଲ୍ୟ(ngx_rbtree_ଗଣ୍ଠି_t *temp, ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି,
    ngx_rbtree_ଗଣ୍ଠି_t *sentinel)
{
    ngx_rbtree_ଗଣ୍ଠି_t  **p;

    ପାଇଁ ( ;; ) {

        p = (ଗଣ୍ଠି->ଚାବି < temp->ଚାବି) ? &temp->ବାମ : &temp->ଡାହାଣ;

        ଯଦି (*p == sentinel) {
            ଭାଙ୍ଗ;
        }

        temp = *p;
    }

    *p = ଗଣ୍ଠି;
    ଗଣ୍ଠି->parent = temp;
    ଗଣ୍ଠି->ବାମ = sentinel;
    ଗଣ୍ଠି->ଡାହାଣ = sentinel;
    ngx_rbt_red(ଗଣ୍ଠି);
}


ଶୂନ୍ୟ
ngx_rbtree_ଭର୍ତ୍ତି_timer_ମୂଲ୍ୟ(ngx_rbtree_ଗଣ୍ଠି_t *temp, ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି,
    ngx_rbtree_ଗଣ୍ଠି_t *sentinel)
{
    ngx_rbtree_ଗଣ୍ଠି_t  **p;

    ପାଇଁ ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_ଚାବି_int_t) (ଗଣ୍ଠି->ଚାବି - temp->ଚାବି) < 0)
            ? &temp->ବାମ : &temp->ଡାହାଣ;

        ଯଦି (*p == sentinel) {
            ଭାଙ୍ଗ;
        }

        temp = *p;
    }

    *p = ଗଣ୍ଠି;
    ଗଣ୍ଠି->parent = temp;
    ଗଣ୍ଠି->ବାମ = sentinel;
    ଗଣ୍ଠି->ଡାହାଣ = sentinel;
    ngx_rbt_red(ଗଣ୍ଠି);
}


ଶୂନ୍ୟ
ngx_rbtree_delete(ngx_rbtree_t *ବୃକ୍ଷ, ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି)
{
    ngx_uint_t           red;
    ngx_rbtree_ଗଣ୍ଠି_t  **ମୂଳ, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    ମୂଳ = &ବୃକ୍ଷ->ମୂଳ;
    sentinel = ବୃକ୍ଷ->sentinel;

    ଯଦି (ଗଣ୍ଠି->ବାମ == sentinel) {
        temp = ଗଣ୍ଠି->ଡାହାଣ;
        subst = ଗଣ୍ଠି;

    } ଅନ୍ୟଥା ଯଦି (ଗଣ୍ଠି->ଡାହାଣ == sentinel) {
        temp = ଗଣ୍ଠି->ବାମ;
        subst = ଗଣ୍ଠି;

    } ଅନ୍ୟଥା {
        subst = ngx_rbtree_min(ଗଣ୍ଠି->ଡାହାଣ, sentinel);
        temp = subst->ଡାହାଣ;
    }

    ଯଦି (subst == *ମୂଳ) {
        *ମୂଳ = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        ଗଣ୍ଠି->ବାମ = ଖାଲି;
        ଗଣ୍ଠି->ଡାହାଣ = ଖାଲି;
        ଗଣ୍ଠି->parent = ଖାଲି;
        ଗଣ୍ଠି->ଚାବି = 0;

        ଫେରନ୍ତୁ;
    }

    red = ngx_rbt_is_red(subst);

    ଯଦି (subst == subst->parent->ବାମ) {
        subst->parent->ବାମ = temp;

    } ଅନ୍ୟଥା {
        subst->parent->ଡାହାଣ = temp;
    }

    ଯଦି (subst == ଗଣ୍ଠି) {

        temp->parent = subst->parent;

    } ଅନ୍ୟଥା {

        ଯଦି (subst->parent == ଗଣ୍ଠି) {
            temp->parent = subst;

        } ଅନ୍ୟଥା {
            temp->parent = subst->parent;
        }

        subst->ବାମ = ଗଣ୍ଠି->ବାମ;
        subst->ଡାହାଣ = ଗଣ୍ଠି->ଡାହାଣ;
        subst->parent = ଗଣ୍ଠି->parent;
        ngx_rbt_copy_color(subst, ଗଣ୍ଠି);

        ଯଦି (ଗଣ୍ଠି == *ମୂଳ) {
            *ମୂଳ = subst;

        } ଅନ୍ୟଥା {
            ଯଦି (ଗଣ୍ଠି == ଗଣ୍ଠି->parent->ବାମ) {
                ଗଣ୍ଠି->parent->ବାମ = subst;
            } ଅନ୍ୟଥା {
                ଗଣ୍ଠି->parent->ଡାହାଣ = subst;
            }
        }

        ଯଦି (subst->ବାମ != sentinel) {
            subst->ବାମ->parent = subst;
        }

        ଯଦି (subst->ଡାହାଣ != sentinel) {
            subst->ଡାହାଣ->parent = subst;
        }
    }

    /* DEBUG stuff */
    ଗଣ୍ଠି->ବାମ = ଖାଲି;
    ଗଣ୍ଠି->ଡାହାଣ = ଖାଲି;
    ଗଣ୍ଠି->parent = ଖାଲି;
    ଗଣ୍ଠି->ଚାବି = 0;

    ଯଦି (red) {
        ଫେରନ୍ତୁ;
    }

    /* a delete fixup */

    ଯେତେବେଳେ (temp != *ମୂଳ && ngx_rbt_is_black(temp)) {

        ଯଦି (temp == temp->parent->ବାମ) {
            w = temp->parent->ଡାହାଣ;

            ଯଦି (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_ବାମ_rotate(ମୂଳ, sentinel, temp->parent);
                w = temp->parent->ଡାହାଣ;
            }

            ଯଦି (ngx_rbt_is_black(w->ବାମ) && ngx_rbt_is_black(w->ଡାହାଣ)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } ଅନ୍ୟଥା {
                ଯଦି (ngx_rbt_is_black(w->ଡାହାଣ)) {
                    ngx_rbt_black(w->ବାମ);
                    ngx_rbt_red(w);
                    ngx_rbtree_ଡାହାଣ_rotate(ମୂଳ, sentinel, w);
                    w = temp->parent->ଡାହାଣ;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->ଡାହାଣ);
                ngx_rbtree_ବାମ_rotate(ମୂଳ, sentinel, temp->parent);
                temp = *ମୂଳ;
            }

        } ଅନ୍ୟଥା {
            w = temp->parent->ବାମ;

            ଯଦି (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_ଡାହାଣ_rotate(ମୂଳ, sentinel, temp->parent);
                w = temp->parent->ବାମ;
            }

            ଯଦି (ngx_rbt_is_black(w->ବାମ) && ngx_rbt_is_black(w->ଡାହାଣ)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } ଅନ୍ୟଥା {
                ଯଦି (ngx_rbt_is_black(w->ବାମ)) {
                    ngx_rbt_black(w->ଡାହାଣ);
                    ngx_rbt_red(w);
                    ngx_rbtree_ବାମ_rotate(ମୂଳ, sentinel, w);
                    w = temp->parent->ବାମ;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->ବାମ);
                ngx_rbtree_ଡାହାଣ_rotate(ମୂଳ, sentinel, temp->parent);
                temp = *ମୂଳ;
            }
        }
    }

    ngx_rbt_black(temp);
}


ସ୍ଥିର ngx_inline ଶୂନ୍ୟ
ngx_rbtree_ବାମ_rotate(ngx_rbtree_ଗଣ୍ଠି_t **ମୂଳ, ngx_rbtree_ଗଣ୍ଠି_t *sentinel,
    ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି)
{
    ngx_rbtree_ଗଣ୍ଠି_t  *temp;

    temp = ଗଣ୍ଠି->ଡାହାଣ;
    ଗଣ୍ଠି->ଡାହାଣ = temp->ବାମ;

    ଯଦି (temp->ବାମ != sentinel) {
        temp->ବାମ->parent = ଗଣ୍ଠି;
    }

    temp->parent = ଗଣ୍ଠି->parent;

    ଯଦି (ଗଣ୍ଠି == *ମୂଳ) {
        *ମୂଳ = temp;

    } ଅନ୍ୟଥା ଯଦି (ଗଣ୍ଠି == ଗଣ୍ଠି->parent->ବାମ) {
        ଗଣ୍ଠି->parent->ବାମ = temp;

    } ଅନ୍ୟଥା {
        ଗଣ୍ଠି->parent->ଡାହାଣ = temp;
    }

    temp->ବାମ = ଗଣ୍ଠି;
    ଗଣ୍ଠି->parent = temp;
}


ସ୍ଥିର ngx_inline ଶୂନ୍ୟ
ngx_rbtree_ଡାହାଣ_rotate(ngx_rbtree_ଗଣ୍ଠି_t **ମୂଳ, ngx_rbtree_ଗଣ୍ଠି_t *sentinel,
    ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି)
{
    ngx_rbtree_ଗଣ୍ଠି_t  *temp;

    temp = ଗଣ୍ଠି->ବାମ;
    ଗଣ୍ଠି->ବାମ = temp->ଡାହାଣ;

    ଯଦି (temp->ଡାହାଣ != sentinel) {
        temp->ଡାହାଣ->parent = ଗଣ୍ଠି;
    }

    temp->parent = ଗଣ୍ଠି->parent;

    ଯଦି (ଗଣ୍ଠି == *ମୂଳ) {
        *ମୂଳ = temp;

    } ଅନ୍ୟଥା ଯଦି (ଗଣ୍ଠି == ଗଣ୍ଠି->parent->ଡାହାଣ) {
        ଗଣ୍ଠି->parent->ଡାହାଣ = temp;

    } ଅନ୍ୟଥା {
        ଗଣ୍ଠି->parent->ବାମ = temp;
    }

    temp->ଡାହାଣ = ଗଣ୍ଠି;
    ଗଣ୍ଠି->parent = temp;
}


ngx_rbtree_ଗଣ୍ଠି_t *
ngx_rbtree_next(ngx_rbtree_t *ବୃକ୍ଷ, ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି)
{
    ngx_rbtree_ଗଣ୍ଠି_t  *ମୂଳ, *sentinel, *parent;

    sentinel = ବୃକ୍ଷ->sentinel;

    ଯଦି (ଗଣ୍ଠି->ଡାହାଣ != sentinel) {
        ଫେରନ୍ତୁ ngx_rbtree_min(ଗଣ୍ଠି->ଡାହାଣ, sentinel);
    }

    ମୂଳ = ବୃକ୍ଷ->ମୂଳ;

    ପାଇଁ ( ;; ) {
        parent = ଗଣ୍ଠି->parent;

        ଯଦି (ଗଣ୍ଠି == ମୂଳ) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        ଯଦି (ଗଣ୍ଠି == parent->ବାମ) {
            ଫେରନ୍ତୁ parent;
        }

        ଗଣ୍ଠି = parent;
    }
}
