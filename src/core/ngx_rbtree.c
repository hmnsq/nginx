
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


स्थिर ngx_inline शून्य ngx_rbtree_वामः_rotate(ngx_rbtree_ग्रन्थिः_t **मूलम्,
    ngx_rbtree_ग्रन्थिः_t *sentinel, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः);
स्थिर ngx_inline शून्य ngx_rbtree_दक्षिणः_rotate(ngx_rbtree_ग्रन्थिः_t **मूलम्,
    ngx_rbtree_ग्रन्थिः_t *sentinel, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः);


शून्य
ngx_rbtree_निवेशय(ngx_rbtree_t *वृक्षः, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः)
{
    ngx_rbtree_ग्रन्थिः_t  **मूलम्, *temp, *sentinel;

    /* a binary tree insert */

    मूलम् = &वृक्षः->मूलम्;
    sentinel = वृक्षः->sentinel;

    जइ (*मूलम् == sentinel) {
        ग्रन्थिः->parent = रिक्त;
        ग्रन्थिः->वामः = sentinel;
        ग्रन्थिः->दक्षिणः = sentinel;
        ngx_rbt_black(ग्रन्थिः);
        *मूलम् = ग्रन्थिः;

        प्रत्यागम;
    }

    वृक्षः->निवेशय(*मूलम्, ग्रन्थिः, sentinel);

    /* re-balance tree */

    जाव (ग्रन्थिः != *मूलम् && ngx_rbt_is_red(ग्रन्थिः->parent)) {

        जइ (ग्रन्थिः->parent == ग्रन्थिः->parent->parent->वामः) {
            temp = ग्रन्थिः->parent->parent->दक्षिणः;

            जइ (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(ग्रन्थिः->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(ग्रन्थिः->parent->parent);
                ग्रन्थिः = ग्रन्थिः->parent->parent;

            } अण्णहा {
                जइ (ग्रन्थिः == ग्रन्थिः->parent->दक्षिणः) {
                    ग्रन्थिः = ग्रन्थिः->parent;
                    ngx_rbtree_वामः_rotate(मूलम्, sentinel, ग्रन्थिः);
                }

                ngx_rbt_black(ग्रन्थिः->parent);
                ngx_rbt_red(ग्रन्थिः->parent->parent);
                ngx_rbtree_दक्षिणः_rotate(मूलम्, sentinel, ग्रन्थिः->parent->parent);
            }

        } अण्णहा {
            temp = ग्रन्थिः->parent->parent->वामः;

            जइ (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(ग्रन्थिः->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(ग्रन्थिः->parent->parent);
                ग्रन्थिः = ग्रन्थिः->parent->parent;

            } अण्णहा {
                जइ (ग्रन्थिः == ग्रन्थिः->parent->वामः) {
                    ग्रन्थिः = ग्रन्थिः->parent;
                    ngx_rbtree_दक्षिणः_rotate(मूलम्, sentinel, ग्रन्थिः);
                }

                ngx_rbt_black(ग्रन्थिः->parent);
                ngx_rbt_red(ग्रन्थिः->parent->parent);
                ngx_rbtree_वामः_rotate(मूलम्, sentinel, ग्रन्थिः->parent->parent);
            }
        }
    }

    ngx_rbt_black(*मूलम्);
}


शून्य
ngx_rbtree_निवेशय_मानम्(ngx_rbtree_ग्रन्थिः_t *temp, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः,
    ngx_rbtree_ग्रन्थिः_t *sentinel)
{
    ngx_rbtree_ग्रन्थिः_t  **p;

    प्रति ( ;; ) {

        p = (ग्रन्थिः->कुञ्चिका < temp->कुञ्चिका) ? &temp->वामः : &temp->दक्षिणः;

        जइ (*p == sentinel) {
            भङ्ग;
        }

        temp = *p;
    }

    *p = ग्रन्थिः;
    ग्रन्थिः->parent = temp;
    ग्रन्थिः->वामः = sentinel;
    ग्रन्थिः->दक्षिणः = sentinel;
    ngx_rbt_red(ग्रन्थिः);
}


शून्य
ngx_rbtree_निवेशय_timer_मानम्(ngx_rbtree_ग्रन्थिः_t *temp, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः,
    ngx_rbtree_ग्रन्थिः_t *sentinel)
{
    ngx_rbtree_ग्रन्थिः_t  **p;

    प्रति ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_कुञ्चिका_int_t) (ग्रन्थिः->कुञ्चिका - temp->कुञ्चिका) < 0)
            ? &temp->वामः : &temp->दक्षिणः;

        जइ (*p == sentinel) {
            भङ्ग;
        }

        temp = *p;
    }

    *p = ग्रन्थिः;
    ग्रन्थिः->parent = temp;
    ग्रन्थिः->वामः = sentinel;
    ग्रन्थिः->दक्षिणः = sentinel;
    ngx_rbt_red(ग्रन्थिः);
}


शून्य
ngx_rbtree_delete(ngx_rbtree_t *वृक्षः, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः)
{
    ngx_uint_t           red;
    ngx_rbtree_ग्रन्थिः_t  **मूलम्, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    मूलम् = &वृक्षः->मूलम्;
    sentinel = वृक्षः->sentinel;

    जइ (ग्रन्थिः->वामः == sentinel) {
        temp = ग्रन्थिः->दक्षिणः;
        subst = ग्रन्थिः;

    } अण्णहा जइ (ग्रन्थिः->दक्षिणः == sentinel) {
        temp = ग्रन्थिः->वामः;
        subst = ग्रन्थिः;

    } अण्णहा {
        subst = ngx_rbtree_min(ग्रन्थिः->दक्षिणः, sentinel);
        temp = subst->दक्षिणः;
    }

    जइ (subst == *मूलम्) {
        *मूलम् = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        ग्रन्थिः->वामः = रिक्त;
        ग्रन्थिः->दक्षिणः = रिक्त;
        ग्रन्थिः->parent = रिक्त;
        ग्रन्थिः->कुञ्चिका = 0;

        प्रत्यागम;
    }

    red = ngx_rbt_is_red(subst);

    जइ (subst == subst->parent->वामः) {
        subst->parent->वामः = temp;

    } अण्णहा {
        subst->parent->दक्षिणः = temp;
    }

    जइ (subst == ग्रन्थिः) {

        temp->parent = subst->parent;

    } अण्णहा {

        जइ (subst->parent == ग्रन्थिः) {
            temp->parent = subst;

        } अण्णहा {
            temp->parent = subst->parent;
        }

        subst->वामः = ग्रन्थिः->वामः;
        subst->दक्षिणः = ग्रन्थिः->दक्षिणः;
        subst->parent = ग्रन्थिः->parent;
        ngx_rbt_copy_color(subst, ग्रन्थिः);

        जइ (ग्रन्थिः == *मूलम्) {
            *मूलम् = subst;

        } अण्णहा {
            जइ (ग्रन्थिः == ग्रन्थिः->parent->वामः) {
                ग्रन्थिः->parent->वामः = subst;
            } अण्णहा {
                ग्रन्थिः->parent->दक्षिणः = subst;
            }
        }

        जइ (subst->वामः != sentinel) {
            subst->वामः->parent = subst;
        }

        जइ (subst->दक्षिणः != sentinel) {
            subst->दक्षिणः->parent = subst;
        }
    }

    /* DEBUG stuff */
    ग्रन्थिः->वामः = रिक्त;
    ग्रन्थिः->दक्षिणः = रिक्त;
    ग्रन्थिः->parent = रिक्त;
    ग्रन्थिः->कुञ्चिका = 0;

    जइ (red) {
        प्रत्यागम;
    }

    /* a delete fixup */

    जाव (temp != *मूलम् && ngx_rbt_is_black(temp)) {

        जइ (temp == temp->parent->वामः) {
            w = temp->parent->दक्षिणः;

            जइ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_वामः_rotate(मूलम्, sentinel, temp->parent);
                w = temp->parent->दक्षिणः;
            }

            जइ (ngx_rbt_is_black(w->वामः) && ngx_rbt_is_black(w->दक्षिणः)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } अण्णहा {
                जइ (ngx_rbt_is_black(w->दक्षिणः)) {
                    ngx_rbt_black(w->वामः);
                    ngx_rbt_red(w);
                    ngx_rbtree_दक्षिणः_rotate(मूलम्, sentinel, w);
                    w = temp->parent->दक्षिणः;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->दक्षिणः);
                ngx_rbtree_वामः_rotate(मूलम्, sentinel, temp->parent);
                temp = *मूलम्;
            }

        } अण्णहा {
            w = temp->parent->वामः;

            जइ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_दक्षिणः_rotate(मूलम्, sentinel, temp->parent);
                w = temp->parent->वामः;
            }

            जइ (ngx_rbt_is_black(w->वामः) && ngx_rbt_is_black(w->दक्षिणः)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } अण्णहा {
                जइ (ngx_rbt_is_black(w->वामः)) {
                    ngx_rbt_black(w->दक्षिणः);
                    ngx_rbt_red(w);
                    ngx_rbtree_वामः_rotate(मूलम्, sentinel, w);
                    w = temp->parent->वामः;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->वामः);
                ngx_rbtree_दक्षिणः_rotate(मूलम्, sentinel, temp->parent);
                temp = *मूलम्;
            }
        }
    }

    ngx_rbt_black(temp);
}


स्थिर ngx_inline शून्य
ngx_rbtree_वामः_rotate(ngx_rbtree_ग्रन्थिः_t **मूलम्, ngx_rbtree_ग्रन्थिः_t *sentinel,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः)
{
    ngx_rbtree_ग्रन्थिः_t  *temp;

    temp = ग्रन्थिः->दक्षिणः;
    ग्रन्थिः->दक्षिणः = temp->वामः;

    जइ (temp->वामः != sentinel) {
        temp->वामः->parent = ग्रन्थिः;
    }

    temp->parent = ग्रन्थिः->parent;

    जइ (ग्रन्थिः == *मूलम्) {
        *मूलम् = temp;

    } अण्णहा जइ (ग्रन्थिः == ग्रन्थिः->parent->वामः) {
        ग्रन्थिः->parent->वामः = temp;

    } अण्णहा {
        ग्रन्थिः->parent->दक्षिणः = temp;
    }

    temp->वामः = ग्रन्थिः;
    ग्रन्थिः->parent = temp;
}


स्थिर ngx_inline शून्य
ngx_rbtree_दक्षिणः_rotate(ngx_rbtree_ग्रन्थिः_t **मूलम्, ngx_rbtree_ग्रन्थिः_t *sentinel,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः)
{
    ngx_rbtree_ग्रन्थिः_t  *temp;

    temp = ग्रन्थिः->वामः;
    ग्रन्थिः->वामः = temp->दक्षिणः;

    जइ (temp->दक्षिणः != sentinel) {
        temp->दक्षिणः->parent = ग्रन्थिः;
    }

    temp->parent = ग्रन्थिः->parent;

    जइ (ग्रन्थिः == *मूलम्) {
        *मूलम् = temp;

    } अण्णहा जइ (ग्रन्थिः == ग्रन्थिः->parent->दक्षिणः) {
        ग्रन्थिः->parent->दक्षिणः = temp;

    } अण्णहा {
        ग्रन्थिः->parent->वामः = temp;
    }

    temp->दक्षिणः = ग्रन्थिः;
    ग्रन्थिः->parent = temp;
}


ngx_rbtree_ग्रन्थिः_t *
ngx_rbtree_next(ngx_rbtree_t *वृक्षः, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः)
{
    ngx_rbtree_ग्रन्थिः_t  *मूलम्, *sentinel, *parent;

    sentinel = वृक्षः->sentinel;

    जइ (ग्रन्थिः->दक्षिणः != sentinel) {
        प्रत्यागम ngx_rbtree_min(ग्रन्थिः->दक्षिणः, sentinel);
    }

    मूलम् = वृक्षः->मूलम्;

    प्रति ( ;; ) {
        parent = ग्रन्थिः->parent;

        जइ (ग्रन्थिः == मूलम्) {
            प्रत्यागम रिक्त;
        }

        जइ (ग्रन्थिः == parent->वामः) {
            प्रत्यागम parent;
        }

        ग्रन्थिः = parent;
    }
}
