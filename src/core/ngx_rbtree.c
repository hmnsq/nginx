
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ ngx_rbtree_ಎಡ_rotate(ngx_rbtree_ಗಂಟು_t **ಬೇರು,
    ngx_rbtree_ಗಂಟು_t *sentinel, ngx_rbtree_ಗಂಟು_t *ಗಂಟು);
ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ ngx_rbtree_ಬಲ_rotate(ngx_rbtree_ಗಂಟು_t **ಬೇರು,
    ngx_rbtree_ಗಂಟು_t *sentinel, ngx_rbtree_ಗಂಟು_t *ಗಂಟು);


ಶೂನ್ಯ
ngx_rbtree_ಸೇರಿಸು(ngx_rbtree_t *ವೃಕ್ಷ, ngx_rbtree_ಗಂಟು_t *ಗಂಟು)
{
    ngx_rbtree_ಗಂಟು_t  **ಬೇರು, *ತಾತ್ಕಾಲಿಕ, *sentinel;

    /* a binary tree insert */

    ಬೇರು = &ವೃಕ್ಷ->ಬೇರು;
    sentinel = ವೃಕ್ಷ->sentinel;

    ಆದರೆ (*ಬೇರು == sentinel) {
        ಗಂಟು->ಪೋಷಕ = ಖಾಲಿ;
        ಗಂಟು->ಎಡ = sentinel;
        ಗಂಟು->ಬಲ = sentinel;
        ngx_rbt_black(ಗಂಟು);
        *ಬೇರು = ಗಂಟು;

        ಹಿಂತಿರುಗಿ;
    }

    ವೃಕ್ಷ->ಸೇರಿಸು(*ಬೇರು, ಗಂಟು, sentinel);

    /* re-balance tree */

    ಆಗ (ಗಂಟು != *ಬೇರು && ngx_rbt_is_red(ಗಂಟು->ಪೋಷಕ)) {

        ಆದರೆ (ಗಂಟು->ಪೋಷಕ == ಗಂಟು->ಪೋಷಕ->ಪೋಷಕ->ಎಡ) {
            ತಾತ್ಕಾಲಿಕ = ಗಂಟು->ಪೋಷಕ->ಪೋಷಕ->ಬಲ;

            ಆದರೆ (ngx_rbt_is_red(ತಾತ್ಕಾಲಿಕ)) {
                ngx_rbt_black(ಗಂಟು->ಪೋಷಕ);
                ngx_rbt_black(ತಾತ್ಕಾಲಿಕ);
                ngx_rbt_red(ಗಂಟು->ಪೋಷಕ->ಪೋಷಕ);
                ಗಂಟು = ಗಂಟು->ಪೋಷಕ->ಪೋಷಕ;

            } ಇಲ್ಲದಿದ್ದರೆ {
                ಆದರೆ (ಗಂಟು == ಗಂಟು->ಪೋಷಕ->ಬಲ) {
                    ಗಂಟು = ಗಂಟು->ಪೋಷಕ;
                    ngx_rbtree_ಎಡ_rotate(ಬೇರು, sentinel, ಗಂಟು);
                }

                ngx_rbt_black(ಗಂಟು->ಪೋಷಕ);
                ngx_rbt_red(ಗಂಟು->ಪೋಷಕ->ಪೋಷಕ);
                ngx_rbtree_ಬಲ_rotate(ಬೇರು, sentinel, ಗಂಟು->ಪೋಷಕ->ಪೋಷಕ);
            }

        } ಇಲ್ಲದಿದ್ದರೆ {
            ತಾತ್ಕಾಲಿಕ = ಗಂಟು->ಪೋಷಕ->ಪೋಷಕ->ಎಡ;

            ಆದರೆ (ngx_rbt_is_red(ತಾತ್ಕಾಲಿಕ)) {
                ngx_rbt_black(ಗಂಟು->ಪೋಷಕ);
                ngx_rbt_black(ತಾತ್ಕಾಲಿಕ);
                ngx_rbt_red(ಗಂಟು->ಪೋಷಕ->ಪೋಷಕ);
                ಗಂಟು = ಗಂಟು->ಪೋಷಕ->ಪೋಷಕ;

            } ಇಲ್ಲದಿದ್ದರೆ {
                ಆದರೆ (ಗಂಟು == ಗಂಟು->ಪೋಷಕ->ಎಡ) {
                    ಗಂಟು = ಗಂಟು->ಪೋಷಕ;
                    ngx_rbtree_ಬಲ_rotate(ಬೇರು, sentinel, ಗಂಟು);
                }

                ngx_rbt_black(ಗಂಟು->ಪೋಷಕ);
                ngx_rbt_red(ಗಂಟು->ಪೋಷಕ->ಪೋಷಕ);
                ngx_rbtree_ಎಡ_rotate(ಬೇರು, sentinel, ಗಂಟು->ಪೋಷಕ->ಪೋಷಕ);
            }
        }
    }

    ngx_rbt_black(*ಬೇರು);
}


ಶೂನ್ಯ
ngx_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *ತಾತ್ಕಾಲಿಕ, ngx_rbtree_ಗಂಟು_t *ಗಂಟು,
    ngx_rbtree_ಗಂಟು_t *sentinel)
{
    ngx_rbtree_ಗಂಟು_t  **p;

    ಗಾಗಿ ( ;; ) {

        p = (ಗಂಟು->ಕೀಲಿ < ತಾತ್ಕಾಲಿಕ->ಕೀಲಿ) ? &ತಾತ್ಕಾಲಿಕ->ಎಡ : &ತಾತ್ಕಾಲಿಕ->ಬಲ;

        ಆದರೆ (*p == sentinel) {
            ನಿಲ್ಲಿಸು;
        }

        ತಾತ್ಕಾಲಿಕ = *p;
    }

    *p = ಗಂಟು;
    ಗಂಟು->ಪೋಷಕ = ತಾತ್ಕಾಲಿಕ;
    ಗಂಟು->ಎಡ = sentinel;
    ಗಂಟು->ಬಲ = sentinel;
    ngx_rbt_red(ಗಂಟು);
}


ಶೂನ್ಯ
ngx_rbtree_ಸೇರಿಸು_timer_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *ತಾತ್ಕಾಲಿಕ, ngx_rbtree_ಗಂಟು_t *ಗಂಟು,
    ngx_rbtree_ಗಂಟು_t *sentinel)
{
    ngx_rbtree_ಗಂಟು_t  **p;

    ಗಾಗಿ ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_ಕೀಲಿ_int_t) (ಗಂಟು->ಕೀಲಿ - ತಾತ್ಕಾಲಿಕ->ಕೀಲಿ) < 0)
            ? &ತಾತ್ಕಾಲಿಕ->ಎಡ : &ತಾತ್ಕಾಲಿಕ->ಬಲ;

        ಆದರೆ (*p == sentinel) {
            ನಿಲ್ಲಿಸು;
        }

        ತಾತ್ಕಾಲಿಕ = *p;
    }

    *p = ಗಂಟು;
    ಗಂಟು->ಪೋಷಕ = ತಾತ್ಕಾಲಿಕ;
    ಗಂಟು->ಎಡ = sentinel;
    ಗಂಟು->ಬಲ = sentinel;
    ngx_rbt_red(ಗಂಟು);
}


ಶೂನ್ಯ
ngx_rbtree_delete(ngx_rbtree_t *ವೃಕ್ಷ, ngx_rbtree_ಗಂಟು_t *ಗಂಟು)
{
    ngx_uint_t           red;
    ngx_rbtree_ಗಂಟು_t  **ಬೇರು, *sentinel, *subst, *ತಾತ್ಕಾಲಿಕ, *w;

    /* a binary tree delete */

    ಬೇರು = &ವೃಕ್ಷ->ಬೇರು;
    sentinel = ವೃಕ್ಷ->sentinel;

    ಆದರೆ (ಗಂಟು->ಎಡ == sentinel) {
        ತಾತ್ಕಾಲಿಕ = ಗಂಟು->ಬಲ;
        subst = ಗಂಟು;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಂಟು->ಬಲ == sentinel) {
        ತಾತ್ಕಾಲಿಕ = ಗಂಟು->ಎಡ;
        subst = ಗಂಟು;

    } ಇಲ್ಲದಿದ್ದರೆ {
        subst = ngx_rbtree_min(ಗಂಟು->ಬಲ, sentinel);
        ತಾತ್ಕಾಲಿಕ = subst->ಬಲ;
    }

    ಆದರೆ (subst == *ಬೇರು) {
        *ಬೇರು = ತಾತ್ಕಾಲಿಕ;
        ngx_rbt_black(ತಾತ್ಕಾಲಿಕ);

        /* DEBUG stuff */
        ಗಂಟು->ಎಡ = ಖಾಲಿ;
        ಗಂಟು->ಬಲ = ಖಾಲಿ;
        ಗಂಟು->ಪೋಷಕ = ಖಾಲಿ;
        ಗಂಟು->ಕೀಲಿ = 0;

        ಹಿಂತಿರುಗಿ;
    }

    red = ngx_rbt_is_red(subst);

    ಆದರೆ (subst == subst->ಪೋಷಕ->ಎಡ) {
        subst->ಪೋಷಕ->ಎಡ = ತಾತ್ಕಾಲಿಕ;

    } ಇಲ್ಲದಿದ್ದರೆ {
        subst->ಪೋಷಕ->ಬಲ = ತಾತ್ಕಾಲಿಕ;
    }

    ಆದರೆ (subst == ಗಂಟು) {

        ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ = subst->ಪೋಷಕ;

    } ಇಲ್ಲದಿದ್ದರೆ {

        ಆದರೆ (subst->ಪೋಷಕ == ಗಂಟು) {
            ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ = subst;

        } ಇಲ್ಲದಿದ್ದರೆ {
            ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ = subst->ಪೋಷಕ;
        }

        subst->ಎಡ = ಗಂಟು->ಎಡ;
        subst->ಬಲ = ಗಂಟು->ಬಲ;
        subst->ಪೋಷಕ = ಗಂಟು->ಪೋಷಕ;
        ngx_rbt_copy_color(subst, ಗಂಟು);

        ಆದರೆ (ಗಂಟು == *ಬೇರು) {
            *ಬೇರು = subst;

        } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (ಗಂಟು == ಗಂಟು->ಪೋಷಕ->ಎಡ) {
                ಗಂಟು->ಪೋಷಕ->ಎಡ = subst;
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಗಂಟು->ಪೋಷಕ->ಬಲ = subst;
            }
        }

        ಆದರೆ (subst->ಎಡ != sentinel) {
            subst->ಎಡ->ಪೋಷಕ = subst;
        }

        ಆದರೆ (subst->ಬಲ != sentinel) {
            subst->ಬಲ->ಪೋಷಕ = subst;
        }
    }

    /* DEBUG stuff */
    ಗಂಟು->ಎಡ = ಖಾಲಿ;
    ಗಂಟು->ಬಲ = ಖಾಲಿ;
    ಗಂಟು->ಪೋಷಕ = ಖಾಲಿ;
    ಗಂಟು->ಕೀಲಿ = 0;

    ಆದರೆ (red) {
        ಹಿಂತಿರುಗಿ;
    }

    /* a delete fixup */

    ಆಗ (ತಾತ್ಕಾಲಿಕ != *ಬೇರು && ngx_rbt_is_black(ತಾತ್ಕಾಲಿಕ)) {

        ಆದರೆ (ತಾತ್ಕಾಲಿಕ == ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ->ಎಡ) {
            w = ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ->ಬಲ;

            ಆದರೆ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ);
                ngx_rbtree_ಎಡ_rotate(ಬೇರು, sentinel, ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ);
                w = ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ->ಬಲ;
            }

            ಆದರೆ (ngx_rbt_is_black(w->ಎಡ) && ngx_rbt_is_black(w->ಬಲ)) {
                ngx_rbt_red(w);
                ತಾತ್ಕಾಲಿಕ = ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ;

            } ಇಲ್ಲದಿದ್ದರೆ {
                ಆದರೆ (ngx_rbt_is_black(w->ಬಲ)) {
                    ngx_rbt_black(w->ಎಡ);
                    ngx_rbt_red(w);
                    ngx_rbtree_ಬಲ_rotate(ಬೇರು, sentinel, w);
                    w = ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ->ಬಲ;
                }

                ngx_rbt_copy_color(w, ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ);
                ngx_rbt_black(ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ);
                ngx_rbt_black(w->ಬಲ);
                ngx_rbtree_ಎಡ_rotate(ಬೇರು, sentinel, ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ);
                ತಾತ್ಕಾಲಿಕ = *ಬೇರು;
            }

        } ಇಲ್ಲದಿದ್ದರೆ {
            w = ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ->ಎಡ;

            ಆದರೆ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ);
                ngx_rbtree_ಬಲ_rotate(ಬೇರು, sentinel, ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ);
                w = ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ->ಎಡ;
            }

            ಆದರೆ (ngx_rbt_is_black(w->ಎಡ) && ngx_rbt_is_black(w->ಬಲ)) {
                ngx_rbt_red(w);
                ತಾತ್ಕಾಲಿಕ = ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ;

            } ಇಲ್ಲದಿದ್ದರೆ {
                ಆದರೆ (ngx_rbt_is_black(w->ಎಡ)) {
                    ngx_rbt_black(w->ಬಲ);
                    ngx_rbt_red(w);
                    ngx_rbtree_ಎಡ_rotate(ಬೇರು, sentinel, w);
                    w = ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ->ಎಡ;
                }

                ngx_rbt_copy_color(w, ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ);
                ngx_rbt_black(ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ);
                ngx_rbt_black(w->ಎಡ);
                ngx_rbtree_ಬಲ_rotate(ಬೇರು, sentinel, ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ);
                ತಾತ್ಕಾಲಿಕ = *ಬೇರು;
            }
        }
    }

    ngx_rbt_black(ತಾತ್ಕಾಲಿಕ);
}


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ
ngx_rbtree_ಎಡ_rotate(ngx_rbtree_ಗಂಟು_t **ಬೇರು, ngx_rbtree_ಗಂಟು_t *sentinel,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು)
{
    ngx_rbtree_ಗಂಟು_t  *ತಾತ್ಕಾಲಿಕ;

    ತಾತ್ಕಾಲಿಕ = ಗಂಟು->ಬಲ;
    ಗಂಟು->ಬಲ = ತಾತ್ಕಾಲಿಕ->ಎಡ;

    ಆದರೆ (ತಾತ್ಕಾಲಿಕ->ಎಡ != sentinel) {
        ತಾತ್ಕಾಲಿಕ->ಎಡ->ಪೋಷಕ = ಗಂಟು;
    }

    ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ = ಗಂಟು->ಪೋಷಕ;

    ಆದರೆ (ಗಂಟು == *ಬೇರು) {
        *ಬೇರು = ತಾತ್ಕಾಲಿಕ;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಂಟು == ಗಂಟು->ಪೋಷಕ->ಎಡ) {
        ಗಂಟು->ಪೋಷಕ->ಎಡ = ತಾತ್ಕಾಲಿಕ;

    } ಇಲ್ಲದಿದ್ದರೆ {
        ಗಂಟು->ಪೋಷಕ->ಬಲ = ತಾತ್ಕಾಲಿಕ;
    }

    ತಾತ್ಕಾಲಿಕ->ಎಡ = ಗಂಟು;
    ಗಂಟು->ಪೋಷಕ = ತಾತ್ಕಾಲಿಕ;
}


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ
ngx_rbtree_ಬಲ_rotate(ngx_rbtree_ಗಂಟು_t **ಬೇರು, ngx_rbtree_ಗಂಟು_t *sentinel,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು)
{
    ngx_rbtree_ಗಂಟು_t  *ತಾತ್ಕಾಲಿಕ;

    ತಾತ್ಕಾಲಿಕ = ಗಂಟು->ಎಡ;
    ಗಂಟು->ಎಡ = ತಾತ್ಕಾಲಿಕ->ಬಲ;

    ಆದರೆ (ತಾತ್ಕಾಲಿಕ->ಬಲ != sentinel) {
        ತಾತ್ಕಾಲಿಕ->ಬಲ->ಪೋಷಕ = ಗಂಟು;
    }

    ತಾತ್ಕಾಲಿಕ->ಪೋಷಕ = ಗಂಟು->ಪೋಷಕ;

    ಆದರೆ (ಗಂಟು == *ಬೇರು) {
        *ಬೇರು = ತಾತ್ಕಾಲಿಕ;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಂಟು == ಗಂಟು->ಪೋಷಕ->ಬಲ) {
        ಗಂಟು->ಪೋಷಕ->ಬಲ = ತಾತ್ಕಾಲಿಕ;

    } ಇಲ್ಲದಿದ್ದರೆ {
        ಗಂಟು->ಪೋಷಕ->ಎಡ = ತಾತ್ಕಾಲಿಕ;
    }

    ತಾತ್ಕಾಲಿಕ->ಬಲ = ಗಂಟು;
    ಗಂಟು->ಪೋಷಕ = ತಾತ್ಕಾಲಿಕ;
}


ngx_rbtree_ಗಂಟು_t *
ngx_rbtree_ಮುಂದಿನ(ngx_rbtree_t *ವೃಕ್ಷ, ngx_rbtree_ಗಂಟು_t *ಗಂಟು)
{
    ngx_rbtree_ಗಂಟು_t  *ಬೇರು, *sentinel, *ಪೋಷಕ;

    sentinel = ವೃಕ್ಷ->sentinel;

    ಆದರೆ (ಗಂಟು->ಬಲ != sentinel) {
        ಹಿಂತಿರುಗಿ ngx_rbtree_min(ಗಂಟು->ಬಲ, sentinel);
    }

    ಬೇರು = ವೃಕ್ಷ->ಬೇರು;

    ಗಾಗಿ ( ;; ) {
        ಪೋಷಕ = ಗಂಟು->ಪೋಷಕ;

        ಆದರೆ (ಗಂಟು == ಬೇರು) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ಆದರೆ (ಗಂಟು == ಪೋಷಕ->ಎಡ) {
            ಹಿಂತಿರುಗಿ ಪೋಷಕ;
        }

        ಗಂಟು = ಪೋಷಕ;
    }
}
