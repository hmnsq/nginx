
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

    सचे (*मूलम् == sentinel) {
        ग्रन्थिः->parent = रिक्त;
        ग्रन्थिः->वामः = sentinel;
        ग्रन्थिः->दक्षिणः = sentinel;
        ngx_rbt_black(ग्रन्थिः);
        *मूलम् = ग्रन्थिः;

        पटिनिवत्त;
    }

    वृक्षः->निवेशय(*मूलम्, ग्रन्थिः, sentinel);

    /* re-balance tree */

    याव (ग्रन्थिः != *मूलम् && ngx_rbt_is_red(ग्रन्थिः->parent)) {

        सचे (ग्रन्थिः->parent == ग्रन्थिः->parent->parent->वामः) {
            temp = ग्रन्थिः->parent->parent->दक्षिणः;

            सचे (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(ग्रन्थिः->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(ग्रन्थिः->parent->parent);
                ग्रन्थिः = ग्रन्थिः->parent->parent;

            } अञ्ञथा {
                सचे (ग्रन्थिः == ग्रन्थिः->parent->दक्षिणः) {
                    ग्रन्थिः = ग्रन्थिः->parent;
                    ngx_rbtree_वामः_rotate(मूलम्, sentinel, ग्रन्थिः);
                }

                ngx_rbt_black(ग्रन्थिः->parent);
                ngx_rbt_red(ग्रन्थिः->parent->parent);
                ngx_rbtree_दक्षिणः_rotate(मूलम्, sentinel, ग्रन्थिः->parent->parent);
            }

        } अञ्ञथा {
            temp = ग्रन्थिः->parent->parent->वामः;

            सचे (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(ग्रन्थिः->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(ग्रन्थिः->parent->parent);
                ग्रन्थिः = ग्रन्थिः->parent->parent;

            } अञ्ञथा {
                सचे (ग्रन्थिः == ग्रन्थिः->parent->वामः) {
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

    अत्थाय ( ;; ) {

        p = (ग्रन्थिः->कुञ्चिका < temp->कुञ्चिका) ? &temp->वामः : &temp->दक्षिणः;

        सचे (*p == sentinel) {
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

    अत्थाय ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_कुञ्चिका_int_t) (ग्रन्थिः->कुञ्चिका - temp->कुञ्चिका) < 0)
            ? &temp->वामः : &temp->दक्षिणः;

        सचे (*p == sentinel) {
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

    सचे (ग्रन्थिः->वामः == sentinel) {
        temp = ग्रन्थिः->दक्षिणः;
        subst = ग्रन्थिः;

    } अञ्ञथा सचे (ग्रन्थिः->दक्षिणः == sentinel) {
        temp = ग्रन्थिः->वामः;
        subst = ग्रन्थिः;

    } अञ्ञथा {
        subst = ngx_rbtree_min(ग्रन्थिः->दक्षिणः, sentinel);
        temp = subst->दक्षिणः;
    }

    सचे (subst == *मूलम्) {
        *मूलम् = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        ग्रन्थिः->वामः = रिक्त;
        ग्रन्थिः->दक्षिणः = रिक्त;
        ग्रन्थिः->parent = रिक्त;
        ग्रन्थिः->कुञ्चिका = 0;

        पटिनिवत्त;
    }

    red = ngx_rbt_is_red(subst);

    सचे (subst == subst->parent->वामः) {
        subst->parent->वामः = temp;

    } अञ्ञथा {
        subst->parent->दक्षिणः = temp;
    }

    सचे (subst == ग्रन्थिः) {

        temp->parent = subst->parent;

    } अञ्ञथा {

        सचे (subst->parent == ग्रन्थिः) {
            temp->parent = subst;

        } अञ्ञथा {
            temp->parent = subst->parent;
        }

        subst->वामः = ग्रन्थिः->वामः;
        subst->दक्षिणः = ग्रन्थिः->दक्षिणः;
        subst->parent = ग्रन्थिः->parent;
        ngx_rbt_copy_color(subst, ग्रन्थिः);

        सचे (ग्रन्थिः == *मूलम्) {
            *मूलम् = subst;

        } अञ्ञथा {
            सचे (ग्रन्थिः == ग्रन्थिः->parent->वामः) {
                ग्रन्थिः->parent->वामः = subst;
            } अञ्ञथा {
                ग्रन्थिः->parent->दक्षिणः = subst;
            }
        }

        सचे (subst->वामः != sentinel) {
            subst->वामः->parent = subst;
        }

        सचे (subst->दक्षिणः != sentinel) {
            subst->दक्षिणः->parent = subst;
        }
    }

    /* DEBUG stuff */
    ग्रन्थिः->वामः = रिक्त;
    ग्रन्थिः->दक्षिणः = रिक्त;
    ग्रन्थिः->parent = रिक्त;
    ग्रन्थिः->कुञ्चिका = 0;

    सचे (red) {
        पटिनिवत्त;
    }

    /* a delete fixup */

    याव (temp != *मूलम् && ngx_rbt_is_black(temp)) {

        सचे (temp == temp->parent->वामः) {
            w = temp->parent->दक्षिणः;

            सचे (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_वामः_rotate(मूलम्, sentinel, temp->parent);
                w = temp->parent->दक्षिणः;
            }

            सचे (ngx_rbt_is_black(w->वामः) && ngx_rbt_is_black(w->दक्षिणः)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } अञ्ञथा {
                सचे (ngx_rbt_is_black(w->दक्षिणः)) {
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

        } अञ्ञथा {
            w = temp->parent->वामः;

            सचे (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_दक्षिणः_rotate(मूलम्, sentinel, temp->parent);
                w = temp->parent->वामः;
            }

            सचे (ngx_rbt_is_black(w->वामः) && ngx_rbt_is_black(w->दक्षिणः)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } अञ्ञथा {
                सचे (ngx_rbt_is_black(w->वामः)) {
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

    सचे (temp->वामः != sentinel) {
        temp->वामः->parent = ग्रन्थिः;
    }

    temp->parent = ग्रन्थिः->parent;

    सचे (ग्रन्थिः == *मूलम्) {
        *मूलम् = temp;

    } अञ्ञथा सचे (ग्रन्थिः == ग्रन्थिः->parent->वामः) {
        ग्रन्थिः->parent->वामः = temp;

    } अञ्ञथा {
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

    सचे (temp->दक्षिणः != sentinel) {
        temp->दक्षिणः->parent = ग्रन्थिः;
    }

    temp->parent = ग्रन्थिः->parent;

    सचे (ग्रन्थिः == *मूलम्) {
        *मूलम् = temp;

    } अञ्ञथा सचे (ग्रन्थिः == ग्रन्थिः->parent->दक्षिणः) {
        ग्रन्थिः->parent->दक्षिणः = temp;

    } अञ्ञथा {
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

    सचे (ग्रन्थिः->दक्षिणः != sentinel) {
        पटिनिवत्त ngx_rbtree_min(ग्रन्थिः->दक्षिणः, sentinel);
    }

    मूलम् = वृक्षः->मूलम्;

    अत्थाय ( ;; ) {
        parent = ग्रन्थिः->parent;

        सचे (ग्रन्थिः == मूलम्) {
            पटिनिवत्त रिक्त;
        }

        सचे (ग्रन्थिः == parent->वामः) {
            पटिनिवत्त parent;
        }

        ग्रन्थिः = parent;
    }
}
