
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


ਸਥਿਰ ngx_inline ਖਾਲੀ ngx_rbtree_ਖੱਬਾ_rotate(ngx_rbtree_ਗੰਢ_t **ਜੜ੍ਹ,
    ngx_rbtree_ਗੰਢ_t *sentinel, ngx_rbtree_ਗੰਢ_t *ਗੰਢ);
ਸਥਿਰ ngx_inline ਖਾਲੀ ngx_rbtree_ਸੱਜਾ_rotate(ngx_rbtree_ਗੰਢ_t **ਜੜ੍ਹ,
    ngx_rbtree_ਗੰਢ_t *sentinel, ngx_rbtree_ਗੰਢ_t *ਗੰਢ);


ਖਾਲੀ
ngx_rbtree_ਪਾਓ(ngx_rbtree_t *ਰੁੱਖ, ngx_rbtree_ਗੰਢ_t *ਗੰਢ)
{
    ngx_rbtree_ਗੰਢ_t  **ਜੜ੍ਹ, *ਆਰਜ਼ੀ, *sentinel;

    /* a binary tree insert */

    ਜੜ੍ਹ = &ਰੁੱਖ->ਜੜ੍ਹ;
    sentinel = ਰੁੱਖ->sentinel;

    ਜੇ (*ਜੜ੍ਹ == sentinel) {
        ਗੰਢ->ਮਾਪਾ = ਨਿਰਾ;
        ਗੰਢ->ਖੱਬਾ = sentinel;
        ਗੰਢ->ਸੱਜਾ = sentinel;
        ngx_rbt_black(ਗੰਢ);
        *ਜੜ੍ਹ = ਗੰਢ;

        ਵਾਪਸ;
    }

    ਰੁੱਖ->ਪਾਓ(*ਜੜ੍ਹ, ਗੰਢ, sentinel);

    /* re-balance tree */

    ਜਦੋਂ (ਗੰਢ != *ਜੜ੍ਹ && ngx_rbt_is_red(ਗੰਢ->ਮਾਪਾ)) {

        ਜੇ (ਗੰਢ->ਮਾਪਾ == ਗੰਢ->ਮਾਪਾ->ਮਾਪਾ->ਖੱਬਾ) {
            ਆਰਜ਼ੀ = ਗੰਢ->ਮਾਪਾ->ਮਾਪਾ->ਸੱਜਾ;

            ਜੇ (ngx_rbt_is_red(ਆਰਜ਼ੀ)) {
                ngx_rbt_black(ਗੰਢ->ਮਾਪਾ);
                ngx_rbt_black(ਆਰਜ਼ੀ);
                ngx_rbt_red(ਗੰਢ->ਮਾਪਾ->ਮਾਪਾ);
                ਗੰਢ = ਗੰਢ->ਮਾਪਾ->ਮਾਪਾ;

            } ਨਹੀਂਤਾਂ {
                ਜੇ (ਗੰਢ == ਗੰਢ->ਮਾਪਾ->ਸੱਜਾ) {
                    ਗੰਢ = ਗੰਢ->ਮਾਪਾ;
                    ngx_rbtree_ਖੱਬਾ_rotate(ਜੜ੍ਹ, sentinel, ਗੰਢ);
                }

                ngx_rbt_black(ਗੰਢ->ਮਾਪਾ);
                ngx_rbt_red(ਗੰਢ->ਮਾਪਾ->ਮਾਪਾ);
                ngx_rbtree_ਸੱਜਾ_rotate(ਜੜ੍ਹ, sentinel, ਗੰਢ->ਮਾਪਾ->ਮਾਪਾ);
            }

        } ਨਹੀਂਤਾਂ {
            ਆਰਜ਼ੀ = ਗੰਢ->ਮਾਪਾ->ਮਾਪਾ->ਖੱਬਾ;

            ਜੇ (ngx_rbt_is_red(ਆਰਜ਼ੀ)) {
                ngx_rbt_black(ਗੰਢ->ਮਾਪਾ);
                ngx_rbt_black(ਆਰਜ਼ੀ);
                ngx_rbt_red(ਗੰਢ->ਮਾਪਾ->ਮਾਪਾ);
                ਗੰਢ = ਗੰਢ->ਮਾਪਾ->ਮਾਪਾ;

            } ਨਹੀਂਤਾਂ {
                ਜੇ (ਗੰਢ == ਗੰਢ->ਮਾਪਾ->ਖੱਬਾ) {
                    ਗੰਢ = ਗੰਢ->ਮਾਪਾ;
                    ngx_rbtree_ਸੱਜਾ_rotate(ਜੜ੍ਹ, sentinel, ਗੰਢ);
                }

                ngx_rbt_black(ਗੰਢ->ਮਾਪਾ);
                ngx_rbt_red(ਗੰਢ->ਮਾਪਾ->ਮਾਪਾ);
                ngx_rbtree_ਖੱਬਾ_rotate(ਜੜ੍ਹ, sentinel, ਗੰਢ->ਮਾਪਾ->ਮਾਪਾ);
            }
        }
    }

    ngx_rbt_black(*ਜੜ੍ਹ);
}


ਖਾਲੀ
ngx_rbtree_ਪਾਓ_ਮੁੱਲ(ngx_rbtree_ਗੰਢ_t *ਆਰਜ਼ੀ, ngx_rbtree_ਗੰਢ_t *ਗੰਢ,
    ngx_rbtree_ਗੰਢ_t *sentinel)
{
    ngx_rbtree_ਗੰਢ_t  **p;

    ਲਈ ( ;; ) {

        p = (ਗੰਢ->ਕੁੰਜੀ < ਆਰਜ਼ੀ->ਕੁੰਜੀ) ? &ਆਰਜ਼ੀ->ਖੱਬਾ : &ਆਰਜ਼ੀ->ਸੱਜਾ;

        ਜੇ (*p == sentinel) {
            ਰੋਕੋ;
        }

        ਆਰਜ਼ੀ = *p;
    }

    *p = ਗੰਢ;
    ਗੰਢ->ਮਾਪਾ = ਆਰਜ਼ੀ;
    ਗੰਢ->ਖੱਬਾ = sentinel;
    ਗੰਢ->ਸੱਜਾ = sentinel;
    ngx_rbt_red(ਗੰਢ);
}


ਖਾਲੀ
ngx_rbtree_ਪਾਓ_timer_ਮੁੱਲ(ngx_rbtree_ਗੰਢ_t *ਆਰਜ਼ੀ, ngx_rbtree_ਗੰਢ_t *ਗੰਢ,
    ngx_rbtree_ਗੰਢ_t *sentinel)
{
    ngx_rbtree_ਗੰਢ_t  **p;

    ਲਈ ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_ਕੁੰਜੀ_int_t) (ਗੰਢ->ਕੁੰਜੀ - ਆਰਜ਼ੀ->ਕੁੰਜੀ) < 0)
            ? &ਆਰਜ਼ੀ->ਖੱਬਾ : &ਆਰਜ਼ੀ->ਸੱਜਾ;

        ਜੇ (*p == sentinel) {
            ਰੋਕੋ;
        }

        ਆਰਜ਼ੀ = *p;
    }

    *p = ਗੰਢ;
    ਗੰਢ->ਮਾਪਾ = ਆਰਜ਼ੀ;
    ਗੰਢ->ਖੱਬਾ = sentinel;
    ਗੰਢ->ਸੱਜਾ = sentinel;
    ngx_rbt_red(ਗੰਢ);
}


ਖਾਲੀ
ngx_rbtree_delete(ngx_rbtree_t *ਰੁੱਖ, ngx_rbtree_ਗੰਢ_t *ਗੰਢ)
{
    ngx_uint_t           red;
    ngx_rbtree_ਗੰਢ_t  **ਜੜ੍ਹ, *sentinel, *subst, *ਆਰਜ਼ੀ, *w;

    /* a binary tree delete */

    ਜੜ੍ਹ = &ਰੁੱਖ->ਜੜ੍ਹ;
    sentinel = ਰੁੱਖ->sentinel;

    ਜੇ (ਗੰਢ->ਖੱਬਾ == sentinel) {
        ਆਰਜ਼ੀ = ਗੰਢ->ਸੱਜਾ;
        subst = ਗੰਢ;

    } ਨਹੀਂਤਾਂ ਜੇ (ਗੰਢ->ਸੱਜਾ == sentinel) {
        ਆਰਜ਼ੀ = ਗੰਢ->ਖੱਬਾ;
        subst = ਗੰਢ;

    } ਨਹੀਂਤਾਂ {
        subst = ngx_rbtree_min(ਗੰਢ->ਸੱਜਾ, sentinel);
        ਆਰਜ਼ੀ = subst->ਸੱਜਾ;
    }

    ਜੇ (subst == *ਜੜ੍ਹ) {
        *ਜੜ੍ਹ = ਆਰਜ਼ੀ;
        ngx_rbt_black(ਆਰਜ਼ੀ);

        /* DEBUG stuff */
        ਗੰਢ->ਖੱਬਾ = ਨਿਰਾ;
        ਗੰਢ->ਸੱਜਾ = ਨਿਰਾ;
        ਗੰਢ->ਮਾਪਾ = ਨਿਰਾ;
        ਗੰਢ->ਕੁੰਜੀ = 0;

        ਵਾਪਸ;
    }

    red = ngx_rbt_is_red(subst);

    ਜੇ (subst == subst->ਮਾਪਾ->ਖੱਬਾ) {
        subst->ਮਾਪਾ->ਖੱਬਾ = ਆਰਜ਼ੀ;

    } ਨਹੀਂਤਾਂ {
        subst->ਮਾਪਾ->ਸੱਜਾ = ਆਰਜ਼ੀ;
    }

    ਜੇ (subst == ਗੰਢ) {

        ਆਰਜ਼ੀ->ਮਾਪਾ = subst->ਮਾਪਾ;

    } ਨਹੀਂਤਾਂ {

        ਜੇ (subst->ਮਾਪਾ == ਗੰਢ) {
            ਆਰਜ਼ੀ->ਮਾਪਾ = subst;

        } ਨਹੀਂਤਾਂ {
            ਆਰਜ਼ੀ->ਮਾਪਾ = subst->ਮਾਪਾ;
        }

        subst->ਖੱਬਾ = ਗੰਢ->ਖੱਬਾ;
        subst->ਸੱਜਾ = ਗੰਢ->ਸੱਜਾ;
        subst->ਮਾਪਾ = ਗੰਢ->ਮਾਪਾ;
        ngx_rbt_copy_color(subst, ਗੰਢ);

        ਜੇ (ਗੰਢ == *ਜੜ੍ਹ) {
            *ਜੜ੍ਹ = subst;

        } ਨਹੀਂਤਾਂ {
            ਜੇ (ਗੰਢ == ਗੰਢ->ਮਾਪਾ->ਖੱਬਾ) {
                ਗੰਢ->ਮਾਪਾ->ਖੱਬਾ = subst;
            } ਨਹੀਂਤਾਂ {
                ਗੰਢ->ਮਾਪਾ->ਸੱਜਾ = subst;
            }
        }

        ਜੇ (subst->ਖੱਬਾ != sentinel) {
            subst->ਖੱਬਾ->ਮਾਪਾ = subst;
        }

        ਜੇ (subst->ਸੱਜਾ != sentinel) {
            subst->ਸੱਜਾ->ਮਾਪਾ = subst;
        }
    }

    /* DEBUG stuff */
    ਗੰਢ->ਖੱਬਾ = ਨਿਰਾ;
    ਗੰਢ->ਸੱਜਾ = ਨਿਰਾ;
    ਗੰਢ->ਮਾਪਾ = ਨਿਰਾ;
    ਗੰਢ->ਕੁੰਜੀ = 0;

    ਜੇ (red) {
        ਵਾਪਸ;
    }

    /* a delete fixup */

    ਜਦੋਂ (ਆਰਜ਼ੀ != *ਜੜ੍ਹ && ngx_rbt_is_black(ਆਰਜ਼ੀ)) {

        ਜੇ (ਆਰਜ਼ੀ == ਆਰਜ਼ੀ->ਮਾਪਾ->ਖੱਬਾ) {
            w = ਆਰਜ਼ੀ->ਮਾਪਾ->ਸੱਜਾ;

            ਜੇ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(ਆਰਜ਼ੀ->ਮਾਪਾ);
                ngx_rbtree_ਖੱਬਾ_rotate(ਜੜ੍ਹ, sentinel, ਆਰਜ਼ੀ->ਮਾਪਾ);
                w = ਆਰਜ਼ੀ->ਮਾਪਾ->ਸੱਜਾ;
            }

            ਜੇ (ngx_rbt_is_black(w->ਖੱਬਾ) && ngx_rbt_is_black(w->ਸੱਜਾ)) {
                ngx_rbt_red(w);
                ਆਰਜ਼ੀ = ਆਰਜ਼ੀ->ਮਾਪਾ;

            } ਨਹੀਂਤਾਂ {
                ਜੇ (ngx_rbt_is_black(w->ਸੱਜਾ)) {
                    ngx_rbt_black(w->ਖੱਬਾ);
                    ngx_rbt_red(w);
                    ngx_rbtree_ਸੱਜਾ_rotate(ਜੜ੍ਹ, sentinel, w);
                    w = ਆਰਜ਼ੀ->ਮਾਪਾ->ਸੱਜਾ;
                }

                ngx_rbt_copy_color(w, ਆਰਜ਼ੀ->ਮਾਪਾ);
                ngx_rbt_black(ਆਰਜ਼ੀ->ਮਾਪਾ);
                ngx_rbt_black(w->ਸੱਜਾ);
                ngx_rbtree_ਖੱਬਾ_rotate(ਜੜ੍ਹ, sentinel, ਆਰਜ਼ੀ->ਮਾਪਾ);
                ਆਰਜ਼ੀ = *ਜੜ੍ਹ;
            }

        } ਨਹੀਂਤਾਂ {
            w = ਆਰਜ਼ੀ->ਮਾਪਾ->ਖੱਬਾ;

            ਜੇ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(ਆਰਜ਼ੀ->ਮਾਪਾ);
                ngx_rbtree_ਸੱਜਾ_rotate(ਜੜ੍ਹ, sentinel, ਆਰਜ਼ੀ->ਮਾਪਾ);
                w = ਆਰਜ਼ੀ->ਮਾਪਾ->ਖੱਬਾ;
            }

            ਜੇ (ngx_rbt_is_black(w->ਖੱਬਾ) && ngx_rbt_is_black(w->ਸੱਜਾ)) {
                ngx_rbt_red(w);
                ਆਰਜ਼ੀ = ਆਰਜ਼ੀ->ਮਾਪਾ;

            } ਨਹੀਂਤਾਂ {
                ਜੇ (ngx_rbt_is_black(w->ਖੱਬਾ)) {
                    ngx_rbt_black(w->ਸੱਜਾ);
                    ngx_rbt_red(w);
                    ngx_rbtree_ਖੱਬਾ_rotate(ਜੜ੍ਹ, sentinel, w);
                    w = ਆਰਜ਼ੀ->ਮਾਪਾ->ਖੱਬਾ;
                }

                ngx_rbt_copy_color(w, ਆਰਜ਼ੀ->ਮਾਪਾ);
                ngx_rbt_black(ਆਰਜ਼ੀ->ਮਾਪਾ);
                ngx_rbt_black(w->ਖੱਬਾ);
                ngx_rbtree_ਸੱਜਾ_rotate(ਜੜ੍ਹ, sentinel, ਆਰਜ਼ੀ->ਮਾਪਾ);
                ਆਰਜ਼ੀ = *ਜੜ੍ਹ;
            }
        }
    }

    ngx_rbt_black(ਆਰਜ਼ੀ);
}


ਸਥਿਰ ngx_inline ਖਾਲੀ
ngx_rbtree_ਖੱਬਾ_rotate(ngx_rbtree_ਗੰਢ_t **ਜੜ੍ਹ, ngx_rbtree_ਗੰਢ_t *sentinel,
    ngx_rbtree_ਗੰਢ_t *ਗੰਢ)
{
    ngx_rbtree_ਗੰਢ_t  *ਆਰਜ਼ੀ;

    ਆਰਜ਼ੀ = ਗੰਢ->ਸੱਜਾ;
    ਗੰਢ->ਸੱਜਾ = ਆਰਜ਼ੀ->ਖੱਬਾ;

    ਜੇ (ਆਰਜ਼ੀ->ਖੱਬਾ != sentinel) {
        ਆਰਜ਼ੀ->ਖੱਬਾ->ਮਾਪਾ = ਗੰਢ;
    }

    ਆਰਜ਼ੀ->ਮਾਪਾ = ਗੰਢ->ਮਾਪਾ;

    ਜੇ (ਗੰਢ == *ਜੜ੍ਹ) {
        *ਜੜ੍ਹ = ਆਰਜ਼ੀ;

    } ਨਹੀਂਤਾਂ ਜੇ (ਗੰਢ == ਗੰਢ->ਮਾਪਾ->ਖੱਬਾ) {
        ਗੰਢ->ਮਾਪਾ->ਖੱਬਾ = ਆਰਜ਼ੀ;

    } ਨਹੀਂਤਾਂ {
        ਗੰਢ->ਮਾਪਾ->ਸੱਜਾ = ਆਰਜ਼ੀ;
    }

    ਆਰਜ਼ੀ->ਖੱਬਾ = ਗੰਢ;
    ਗੰਢ->ਮਾਪਾ = ਆਰਜ਼ੀ;
}


ਸਥਿਰ ngx_inline ਖਾਲੀ
ngx_rbtree_ਸੱਜਾ_rotate(ngx_rbtree_ਗੰਢ_t **ਜੜ੍ਹ, ngx_rbtree_ਗੰਢ_t *sentinel,
    ngx_rbtree_ਗੰਢ_t *ਗੰਢ)
{
    ngx_rbtree_ਗੰਢ_t  *ਆਰਜ਼ੀ;

    ਆਰਜ਼ੀ = ਗੰਢ->ਖੱਬਾ;
    ਗੰਢ->ਖੱਬਾ = ਆਰਜ਼ੀ->ਸੱਜਾ;

    ਜੇ (ਆਰਜ਼ੀ->ਸੱਜਾ != sentinel) {
        ਆਰਜ਼ੀ->ਸੱਜਾ->ਮਾਪਾ = ਗੰਢ;
    }

    ਆਰਜ਼ੀ->ਮਾਪਾ = ਗੰਢ->ਮਾਪਾ;

    ਜੇ (ਗੰਢ == *ਜੜ੍ਹ) {
        *ਜੜ੍ਹ = ਆਰਜ਼ੀ;

    } ਨਹੀਂਤਾਂ ਜੇ (ਗੰਢ == ਗੰਢ->ਮਾਪਾ->ਸੱਜਾ) {
        ਗੰਢ->ਮਾਪਾ->ਸੱਜਾ = ਆਰਜ਼ੀ;

    } ਨਹੀਂਤਾਂ {
        ਗੰਢ->ਮਾਪਾ->ਖੱਬਾ = ਆਰਜ਼ੀ;
    }

    ਆਰਜ਼ੀ->ਸੱਜਾ = ਗੰਢ;
    ਗੰਢ->ਮਾਪਾ = ਆਰਜ਼ੀ;
}


ngx_rbtree_ਗੰਢ_t *
ngx_rbtree_ਅਗਲਾ(ngx_rbtree_t *ਰੁੱਖ, ngx_rbtree_ਗੰਢ_t *ਗੰਢ)
{
    ngx_rbtree_ਗੰਢ_t  *ਜੜ੍ਹ, *sentinel, *ਮਾਪਾ;

    sentinel = ਰੁੱਖ->sentinel;

    ਜੇ (ਗੰਢ->ਸੱਜਾ != sentinel) {
        ਵਾਪਸ ngx_rbtree_min(ਗੰਢ->ਸੱਜਾ, sentinel);
    }

    ਜੜ੍ਹ = ਰੁੱਖ->ਜੜ੍ਹ;

    ਲਈ ( ;; ) {
        ਮਾਪਾ = ਗੰਢ->ਮਾਪਾ;

        ਜੇ (ਗੰਢ == ਜੜ੍ਹ) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        ਜੇ (ਗੰਢ == ਮਾਪਾ->ਖੱਬਾ) {
            ਵਾਪਸ ਮਾਪਾ;
        }

        ਗੰਢ = ਮਾਪਾ;
    }
}
