
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
    ngx_rbtree_ग्रन्थिः_t  **मूलम्, *अस्थायी, *sentinel;

    /* a binary tree insert */

    मूलम् = &वृक्षः->मूलम्;
    sentinel = वृक्षः->sentinel;

    यदि (*मूलम् == sentinel) {
        ग्रन्थिः->parent = रिक्त;
        ग्रन्थिः->वामः = sentinel;
        ग्रन्थिः->दक्षिणः = sentinel;
        ngx_rbt_black(ग्रन्थिः);
        *मूलम् = ग्रन्थिः;

        प्रत्यागम;
    }

    वृक्षः->निवेशय(*मूलम्, ग्रन्थिः, sentinel);

    /* re-balance tree */

    यावत् (ग्रन्थिः != *मूलम् && ngx_rbt_is_red(ग्रन्थिः->parent)) {

        यदि (ग्रन्थिः->parent == ग्रन्थिः->parent->parent->वामः) {
            अस्थायी = ग्रन्थिः->parent->parent->दक्षिणः;

            यदि (ngx_rbt_is_red(अस्थायी)) {
                ngx_rbt_black(ग्रन्थिः->parent);
                ngx_rbt_black(अस्थायी);
                ngx_rbt_red(ग्रन्थिः->parent->parent);
                ग्रन्थिः = ग्रन्थिः->parent->parent;

            } अन्यथा {
                यदि (ग्रन्थिः == ग्रन्थिः->parent->दक्षिणः) {
                    ग्रन्थिः = ग्रन्थिः->parent;
                    ngx_rbtree_वामः_rotate(मूलम्, sentinel, ग्रन्थिः);
                }

                ngx_rbt_black(ग्रन्थिः->parent);
                ngx_rbt_red(ग्रन्थिः->parent->parent);
                ngx_rbtree_दक्षिणः_rotate(मूलम्, sentinel, ग्रन्थिः->parent->parent);
            }

        } अन्यथा {
            अस्थायी = ग्रन्थिः->parent->parent->वामः;

            यदि (ngx_rbt_is_red(अस्थायी)) {
                ngx_rbt_black(ग्रन्थिः->parent);
                ngx_rbt_black(अस्थायी);
                ngx_rbt_red(ग्रन्थिः->parent->parent);
                ग्रन्थिः = ग्रन्थिः->parent->parent;

            } अन्यथा {
                यदि (ग्रन्थिः == ग्रन्थिः->parent->वामः) {
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
ngx_rbtree_निवेशय_मानम्(ngx_rbtree_ग्रन्थिः_t *अस्थायी, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः,
    ngx_rbtree_ग्रन्थिः_t *sentinel)
{
    ngx_rbtree_ग्रन्थिः_t  **p;

    प्रति ( ;; ) {

        p = (ग्रन्थिः->कुञ्चिका < अस्थायी->कुञ्चिका) ? &अस्थायी->वामः : &अस्थायी->दक्षिणः;

        यदि (*p == sentinel) {
            भङ्ग;
        }

        अस्थायी = *p;
    }

    *p = ग्रन्थिः;
    ग्रन्थिः->parent = अस्थायी;
    ग्रन्थिः->वामः = sentinel;
    ग्रन्थिः->दक्षिणः = sentinel;
    ngx_rbt_red(ग्रन्थिः);
}


शून्य
ngx_rbtree_निवेशय_timer_मानम्(ngx_rbtree_ग्रन्थिः_t *अस्थायी, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः,
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

        p = ((ngx_rbtree_कुञ्चिका_int_t) (ग्रन्थिः->कुञ्चिका - अस्थायी->कुञ्चिका) < 0)
            ? &अस्थायी->वामः : &अस्थायी->दक्षिणः;

        यदि (*p == sentinel) {
            भङ्ग;
        }

        अस्थायी = *p;
    }

    *p = ग्रन्थिः;
    ग्रन्थिः->parent = अस्थायी;
    ग्रन्थिः->वामः = sentinel;
    ग्रन्थिः->दक्षिणः = sentinel;
    ngx_rbt_red(ग्रन्थिः);
}


शून्य
ngx_rbtree_delete(ngx_rbtree_t *वृक्षः, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः)
{
    ngx_uint_t           red;
    ngx_rbtree_ग्रन्थिः_t  **मूलम्, *sentinel, *subst, *अस्थायी, *w;

    /* a binary tree delete */

    मूलम् = &वृक्षः->मूलम्;
    sentinel = वृक्षः->sentinel;

    यदि (ग्रन्थिः->वामः == sentinel) {
        अस्थायी = ग्रन्थिः->दक्षिणः;
        subst = ग्रन्थिः;

    } अन्यथा यदि (ग्रन्थिः->दक्षिणः == sentinel) {
        अस्थायी = ग्रन्थिः->वामः;
        subst = ग्रन्थिः;

    } अन्यथा {
        subst = ngx_rbtree_min(ग्रन्थिः->दक्षिणः, sentinel);
        अस्थायी = subst->दक्षिणः;
    }

    यदि (subst == *मूलम्) {
        *मूलम् = अस्थायी;
        ngx_rbt_black(अस्थायी);

        /* DEBUG stuff */
        ग्रन्थिः->वामः = रिक्त;
        ग्रन्थिः->दक्षिणः = रिक्त;
        ग्रन्थिः->parent = रिक्त;
        ग्रन्थिः->कुञ्चिका = 0;

        प्रत्यागम;
    }

    red = ngx_rbt_is_red(subst);

    यदि (subst == subst->parent->वामः) {
        subst->parent->वामः = अस्थायी;

    } अन्यथा {
        subst->parent->दक्षिणः = अस्थायी;
    }

    यदि (subst == ग्रन्थिः) {

        अस्थायी->parent = subst->parent;

    } अन्यथा {

        यदि (subst->parent == ग्रन्थिः) {
            अस्थायी->parent = subst;

        } अन्यथा {
            अस्थायी->parent = subst->parent;
        }

        subst->वामः = ग्रन्थिः->वामः;
        subst->दक्षिणः = ग्रन्थिः->दक्षिणः;
        subst->parent = ग्रन्थिः->parent;
        ngx_rbt_copy_color(subst, ग्रन्थिः);

        यदि (ग्रन्थिः == *मूलम्) {
            *मूलम् = subst;

        } अन्यथा {
            यदि (ग्रन्थिः == ग्रन्थिः->parent->वामः) {
                ग्रन्थिः->parent->वामः = subst;
            } अन्यथा {
                ग्रन्थिः->parent->दक्षिणः = subst;
            }
        }

        यदि (subst->वामः != sentinel) {
            subst->वामः->parent = subst;
        }

        यदि (subst->दक्षिणः != sentinel) {
            subst->दक्षिणः->parent = subst;
        }
    }

    /* DEBUG stuff */
    ग्रन्थिः->वामः = रिक्त;
    ग्रन्थिः->दक्षिणः = रिक्त;
    ग्रन्थिः->parent = रिक्त;
    ग्रन्थिः->कुञ्चिका = 0;

    यदि (red) {
        प्रत्यागम;
    }

    /* a delete fixup */

    यावत् (अस्थायी != *मूलम् && ngx_rbt_is_black(अस्थायी)) {

        यदि (अस्थायी == अस्थायी->parent->वामः) {
            w = अस्थायी->parent->दक्षिणः;

            यदि (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(अस्थायी->parent);
                ngx_rbtree_वामः_rotate(मूलम्, sentinel, अस्थायी->parent);
                w = अस्थायी->parent->दक्षिणः;
            }

            यदि (ngx_rbt_is_black(w->वामः) && ngx_rbt_is_black(w->दक्षिणः)) {
                ngx_rbt_red(w);
                अस्थायी = अस्थायी->parent;

            } अन्यथा {
                यदि (ngx_rbt_is_black(w->दक्षिणः)) {
                    ngx_rbt_black(w->वामः);
                    ngx_rbt_red(w);
                    ngx_rbtree_दक्षिणः_rotate(मूलम्, sentinel, w);
                    w = अस्थायी->parent->दक्षिणः;
                }

                ngx_rbt_copy_color(w, अस्थायी->parent);
                ngx_rbt_black(अस्थायी->parent);
                ngx_rbt_black(w->दक्षिणः);
                ngx_rbtree_वामः_rotate(मूलम्, sentinel, अस्थायी->parent);
                अस्थायी = *मूलम्;
            }

        } अन्यथा {
            w = अस्थायी->parent->वामः;

            यदि (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(अस्थायी->parent);
                ngx_rbtree_दक्षिणः_rotate(मूलम्, sentinel, अस्थायी->parent);
                w = अस्थायी->parent->वामः;
            }

            यदि (ngx_rbt_is_black(w->वामः) && ngx_rbt_is_black(w->दक्षिणः)) {
                ngx_rbt_red(w);
                अस्थायी = अस्थायी->parent;

            } अन्यथा {
                यदि (ngx_rbt_is_black(w->वामः)) {
                    ngx_rbt_black(w->दक्षिणः);
                    ngx_rbt_red(w);
                    ngx_rbtree_वामः_rotate(मूलम्, sentinel, w);
                    w = अस्थायी->parent->वामः;
                }

                ngx_rbt_copy_color(w, अस्थायी->parent);
                ngx_rbt_black(अस्थायी->parent);
                ngx_rbt_black(w->वामः);
                ngx_rbtree_दक्षिणः_rotate(मूलम्, sentinel, अस्थायी->parent);
                अस्थायी = *मूलम्;
            }
        }
    }

    ngx_rbt_black(अस्थायी);
}


स्थिर ngx_inline शून्य
ngx_rbtree_वामः_rotate(ngx_rbtree_ग्रन्थिः_t **मूलम्, ngx_rbtree_ग्रन्थिः_t *sentinel,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः)
{
    ngx_rbtree_ग्रन्थिः_t  *अस्थायी;

    अस्थायी = ग्रन्थिः->दक्षिणः;
    ग्रन्थिः->दक्षिणः = अस्थायी->वामः;

    यदि (अस्थायी->वामः != sentinel) {
        अस्थायी->वामः->parent = ग्रन्थिः;
    }

    अस्थायी->parent = ग्रन्थिः->parent;

    यदि (ग्रन्थिः == *मूलम्) {
        *मूलम् = अस्थायी;

    } अन्यथा यदि (ग्रन्थिः == ग्रन्थिः->parent->वामः) {
        ग्रन्थिः->parent->वामः = अस्थायी;

    } अन्यथा {
        ग्रन्थिः->parent->दक्षिणः = अस्थायी;
    }

    अस्थायी->वामः = ग्रन्थिः;
    ग्रन्थिः->parent = अस्थायी;
}


स्थिर ngx_inline शून्य
ngx_rbtree_दक्षिणः_rotate(ngx_rbtree_ग्रन्थिः_t **मूलम्, ngx_rbtree_ग्रन्थिः_t *sentinel,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः)
{
    ngx_rbtree_ग्रन्थिः_t  *अस्थायी;

    अस्थायी = ग्रन्थिः->वामः;
    ग्रन्थिः->वामः = अस्थायी->दक्षिणः;

    यदि (अस्थायी->दक्षिणः != sentinel) {
        अस्थायी->दक्षिणः->parent = ग्रन्थिः;
    }

    अस्थायी->parent = ग्रन्थिः->parent;

    यदि (ग्रन्थिः == *मूलम्) {
        *मूलम् = अस्थायी;

    } अन्यथा यदि (ग्रन्थिः == ग्रन्थिः->parent->दक्षिणः) {
        ग्रन्थिः->parent->दक्षिणः = अस्थायी;

    } अन्यथा {
        ग्रन्थिः->parent->वामः = अस्थायी;
    }

    अस्थायी->दक्षिणः = ग्रन्थिः;
    ग्रन्थिः->parent = अस्थायी;
}


ngx_rbtree_ग्रन्थिः_t *
ngx_rbtree_अग्र(ngx_rbtree_t *वृक्षः, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः)
{
    ngx_rbtree_ग्रन्थिः_t  *मूलम्, *sentinel, *parent;

    sentinel = वृक्षः->sentinel;

    यदि (ग्रन्थिः->दक्षिणः != sentinel) {
        प्रत्यागम ngx_rbtree_min(ग्रन्थिः->दक्षिणः, sentinel);
    }

    मूलम् = वृक्षः->मूलम्;

    प्रति ( ;; ) {
        parent = ग्रन्थिः->parent;

        यदि (ग्रन्थिः == मूलम्) {
            प्रत्यागम रिक्त;
        }

        यदि (ग्रन्थिः == parent->वामः) {
            प्रत्यागम parent;
        }

        ग्रन्थिः = parent;
    }
}
