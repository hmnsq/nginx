
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
    ngx_rbtree_ಗಂಟು_t  **ಬೇರು, *temp, *sentinel;

    /* a binary tree insert */

    ಬೇರು = &ವೃಕ್ಷ->ಬೇರು;
    sentinel = ವೃಕ್ಷ->sentinel;

    ಯದಿ (*ಬೇರು == sentinel) {
        ಗಂಟು->parent = ರಿಕ್ತ;
        ಗಂಟು->ಎಡ = sentinel;
        ಗಂಟು->ಬಲ = sentinel;
        ngx_rbt_black(ಗಂಟು);
        *ಬೇರು = ಗಂಟು;

        ಪ್ರತ್ಯಾಗಮ;
    }

    ವೃಕ್ಷ->ಸೇರಿಸು(*ಬೇರು, ಗಂಟು, sentinel);

    /* re-balance tree */

    ಯಾವತ್ (ಗಂಟು != *ಬೇರು && ngx_rbt_is_red(ಗಂಟು->parent)) {

        ಯದಿ (ಗಂಟು->parent == ಗಂಟು->parent->parent->ಎಡ) {
            temp = ಗಂಟು->parent->parent->ಬಲ;

            ಯದಿ (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(ಗಂಟು->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(ಗಂಟು->parent->parent);
                ಗಂಟು = ಗಂಟು->parent->parent;

            } ಅನ್ಯಥಾ {
                ಯದಿ (ಗಂಟು == ಗಂಟು->parent->ಬಲ) {
                    ಗಂಟು = ಗಂಟು->parent;
                    ngx_rbtree_ಎಡ_rotate(ಬೇರು, sentinel, ಗಂಟು);
                }

                ngx_rbt_black(ಗಂಟು->parent);
                ngx_rbt_red(ಗಂಟು->parent->parent);
                ngx_rbtree_ಬಲ_rotate(ಬೇರು, sentinel, ಗಂಟು->parent->parent);
            }

        } ಅನ್ಯಥಾ {
            temp = ಗಂಟು->parent->parent->ಎಡ;

            ಯದಿ (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(ಗಂಟು->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(ಗಂಟು->parent->parent);
                ಗಂಟು = ಗಂಟು->parent->parent;

            } ಅನ್ಯಥಾ {
                ಯದಿ (ಗಂಟು == ಗಂಟು->parent->ಎಡ) {
                    ಗಂಟು = ಗಂಟು->parent;
                    ngx_rbtree_ಬಲ_rotate(ಬೇರು, sentinel, ಗಂಟು);
                }

                ngx_rbt_black(ಗಂಟು->parent);
                ngx_rbt_red(ಗಂಟು->parent->parent);
                ngx_rbtree_ಎಡ_rotate(ಬೇರು, sentinel, ಗಂಟು->parent->parent);
            }
        }
    }

    ngx_rbt_black(*ಬೇರು);
}


ಶೂನ್ಯ
ngx_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *temp, ngx_rbtree_ಗಂಟು_t *ಗಂಟು,
    ngx_rbtree_ಗಂಟು_t *sentinel)
{
    ngx_rbtree_ಗಂಟು_t  **p;

    ಪ್ರತಿ ( ;; ) {

        p = (ಗಂಟು->ಕೀಲಿ < temp->ಕೀಲಿ) ? &temp->ಎಡ : &temp->ಬಲ;

        ಯದಿ (*p == sentinel) {
            ಭಙ್ಗ;
        }

        temp = *p;
    }

    *p = ಗಂಟು;
    ಗಂಟು->parent = temp;
    ಗಂಟು->ಎಡ = sentinel;
    ಗಂಟು->ಬಲ = sentinel;
    ngx_rbt_red(ಗಂಟು);
}


ಶೂನ್ಯ
ngx_rbtree_ಸೇರಿಸು_timer_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *temp, ngx_rbtree_ಗಂಟು_t *ಗಂಟು,
    ngx_rbtree_ಗಂಟು_t *sentinel)
{
    ngx_rbtree_ಗಂಟು_t  **p;

    ಪ್ರತಿ ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_ಕೀಲಿ_int_t) (ಗಂಟು->ಕೀಲಿ - temp->ಕೀಲಿ) < 0)
            ? &temp->ಎಡ : &temp->ಬಲ;

        ಯದಿ (*p == sentinel) {
            ಭಙ್ಗ;
        }

        temp = *p;
    }

    *p = ಗಂಟು;
    ಗಂಟು->parent = temp;
    ಗಂಟು->ಎಡ = sentinel;
    ಗಂಟು->ಬಲ = sentinel;
    ngx_rbt_red(ಗಂಟು);
}


ಶೂನ್ಯ
ngx_rbtree_delete(ngx_rbtree_t *ವೃಕ್ಷ, ngx_rbtree_ಗಂಟು_t *ಗಂಟು)
{
    ngx_uint_t           red;
    ngx_rbtree_ಗಂಟು_t  **ಬೇರು, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    ಬೇರು = &ವೃಕ್ಷ->ಬೇರು;
    sentinel = ವೃಕ್ಷ->sentinel;

    ಯದಿ (ಗಂಟು->ಎಡ == sentinel) {
        temp = ಗಂಟು->ಬಲ;
        subst = ಗಂಟು;

    } ಅನ್ಯಥಾ ಯದಿ (ಗಂಟು->ಬಲ == sentinel) {
        temp = ಗಂಟು->ಎಡ;
        subst = ಗಂಟು;

    } ಅನ್ಯಥಾ {
        subst = ngx_rbtree_min(ಗಂಟು->ಬಲ, sentinel);
        temp = subst->ಬಲ;
    }

    ಯದಿ (subst == *ಬೇರು) {
        *ಬೇರು = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        ಗಂಟು->ಎಡ = ರಿಕ್ತ;
        ಗಂಟು->ಬಲ = ರಿಕ್ತ;
        ಗಂಟು->parent = ರಿಕ್ತ;
        ಗಂಟು->ಕೀಲಿ = 0;

        ಪ್ರತ್ಯಾಗಮ;
    }

    red = ngx_rbt_is_red(subst);

    ಯದಿ (subst == subst->parent->ಎಡ) {
        subst->parent->ಎಡ = temp;

    } ಅನ್ಯಥಾ {
        subst->parent->ಬಲ = temp;
    }

    ಯದಿ (subst == ಗಂಟು) {

        temp->parent = subst->parent;

    } ಅನ್ಯಥಾ {

        ಯದಿ (subst->parent == ಗಂಟು) {
            temp->parent = subst;

        } ಅನ್ಯಥಾ {
            temp->parent = subst->parent;
        }

        subst->ಎಡ = ಗಂಟು->ಎಡ;
        subst->ಬಲ = ಗಂಟು->ಬಲ;
        subst->parent = ಗಂಟು->parent;
        ngx_rbt_copy_color(subst, ಗಂಟು);

        ಯದಿ (ಗಂಟು == *ಬೇರು) {
            *ಬೇರು = subst;

        } ಅನ್ಯಥಾ {
            ಯದಿ (ಗಂಟು == ಗಂಟು->parent->ಎಡ) {
                ಗಂಟು->parent->ಎಡ = subst;
            } ಅನ್ಯಥಾ {
                ಗಂಟು->parent->ಬಲ = subst;
            }
        }

        ಯದಿ (subst->ಎಡ != sentinel) {
            subst->ಎಡ->parent = subst;
        }

        ಯದಿ (subst->ಬಲ != sentinel) {
            subst->ಬಲ->parent = subst;
        }
    }

    /* DEBUG stuff */
    ಗಂಟು->ಎಡ = ರಿಕ್ತ;
    ಗಂಟು->ಬಲ = ರಿಕ್ತ;
    ಗಂಟು->parent = ರಿಕ್ತ;
    ಗಂಟು->ಕೀಲಿ = 0;

    ಯದಿ (red) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    /* a delete fixup */

    ಯಾವತ್ (temp != *ಬೇರು && ngx_rbt_is_black(temp)) {

        ಯದಿ (temp == temp->parent->ಎಡ) {
            w = temp->parent->ಬಲ;

            ಯದಿ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_ಎಡ_rotate(ಬೇರು, sentinel, temp->parent);
                w = temp->parent->ಬಲ;
            }

            ಯದಿ (ngx_rbt_is_black(w->ಎಡ) && ngx_rbt_is_black(w->ಬಲ)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } ಅನ್ಯಥಾ {
                ಯದಿ (ngx_rbt_is_black(w->ಬಲ)) {
                    ngx_rbt_black(w->ಎಡ);
                    ngx_rbt_red(w);
                    ngx_rbtree_ಬಲ_rotate(ಬೇರು, sentinel, w);
                    w = temp->parent->ಬಲ;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->ಬಲ);
                ngx_rbtree_ಎಡ_rotate(ಬೇರು, sentinel, temp->parent);
                temp = *ಬೇರು;
            }

        } ಅನ್ಯಥಾ {
            w = temp->parent->ಎಡ;

            ಯದಿ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_ಬಲ_rotate(ಬೇರು, sentinel, temp->parent);
                w = temp->parent->ಎಡ;
            }

            ಯದಿ (ngx_rbt_is_black(w->ಎಡ) && ngx_rbt_is_black(w->ಬಲ)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } ಅನ್ಯಥಾ {
                ಯದಿ (ngx_rbt_is_black(w->ಎಡ)) {
                    ngx_rbt_black(w->ಬಲ);
                    ngx_rbt_red(w);
                    ngx_rbtree_ಎಡ_rotate(ಬೇರು, sentinel, w);
                    w = temp->parent->ಎಡ;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->ಎಡ);
                ngx_rbtree_ಬಲ_rotate(ಬೇರು, sentinel, temp->parent);
                temp = *ಬೇರು;
            }
        }
    }

    ngx_rbt_black(temp);
}


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ
ngx_rbtree_ಎಡ_rotate(ngx_rbtree_ಗಂಟು_t **ಬೇರು, ngx_rbtree_ಗಂಟು_t *sentinel,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು)
{
    ngx_rbtree_ಗಂಟು_t  *temp;

    temp = ಗಂಟು->ಬಲ;
    ಗಂಟು->ಬಲ = temp->ಎಡ;

    ಯದಿ (temp->ಎಡ != sentinel) {
        temp->ಎಡ->parent = ಗಂಟು;
    }

    temp->parent = ಗಂಟು->parent;

    ಯದಿ (ಗಂಟು == *ಬೇರು) {
        *ಬೇರು = temp;

    } ಅನ್ಯಥಾ ಯದಿ (ಗಂಟು == ಗಂಟು->parent->ಎಡ) {
        ಗಂಟು->parent->ಎಡ = temp;

    } ಅನ್ಯಥಾ {
        ಗಂಟು->parent->ಬಲ = temp;
    }

    temp->ಎಡ = ಗಂಟು;
    ಗಂಟು->parent = temp;
}


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ
ngx_rbtree_ಬಲ_rotate(ngx_rbtree_ಗಂಟು_t **ಬೇರು, ngx_rbtree_ಗಂಟು_t *sentinel,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು)
{
    ngx_rbtree_ಗಂಟು_t  *temp;

    temp = ಗಂಟು->ಎಡ;
    ಗಂಟು->ಎಡ = temp->ಬಲ;

    ಯದಿ (temp->ಬಲ != sentinel) {
        temp->ಬಲ->parent = ಗಂಟು;
    }

    temp->parent = ಗಂಟು->parent;

    ಯದಿ (ಗಂಟು == *ಬೇರು) {
        *ಬೇರು = temp;

    } ಅನ್ಯಥಾ ಯದಿ (ಗಂಟು == ಗಂಟು->parent->ಬಲ) {
        ಗಂಟು->parent->ಬಲ = temp;

    } ಅನ್ಯಥಾ {
        ಗಂಟು->parent->ಎಡ = temp;
    }

    temp->ಬಲ = ಗಂಟು;
    ಗಂಟು->parent = temp;
}


ngx_rbtree_ಗಂಟು_t *
ngx_rbtree_next(ngx_rbtree_t *ವೃಕ್ಷ, ngx_rbtree_ಗಂಟು_t *ಗಂಟು)
{
    ngx_rbtree_ಗಂಟು_t  *ಬೇರು, *sentinel, *parent;

    sentinel = ವೃಕ್ಷ->sentinel;

    ಯದಿ (ಗಂಟು->ಬಲ != sentinel) {
        ಪ್ರತ್ಯಾಗಮ ngx_rbtree_min(ಗಂಟು->ಬಲ, sentinel);
    }

    ಬೇರು = ವೃಕ್ಷ->ಬೇರು;

    ಪ್ರತಿ ( ;; ) {
        parent = ಗಂಟು->parent;

        ಯದಿ (ಗಂಟು == ಬೇರು) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        ಯದಿ (ಗಂಟು == parent->ಎಡ) {
            ಪ್ರತ್ಯಾಗಮ parent;
        }

        ಗಂಟು = parent;
    }
}
