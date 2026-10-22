
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


स्थिर ngx_inline शून्य ngx_rbtree_डावा_rotate(ngx_rbtree_गाठ_t **मूळ,
    ngx_rbtree_गाठ_t *sentinel, ngx_rbtree_गाठ_t *गाठ);
स्थिर ngx_inline शून्य ngx_rbtree_उजवा_rotate(ngx_rbtree_गाठ_t **मूळ,
    ngx_rbtree_गाठ_t *sentinel, ngx_rbtree_गाठ_t *गाठ);


शून्य
ngx_rbtree_घाला(ngx_rbtree_t *वृक्ष, ngx_rbtree_गाठ_t *गाठ)
{
    ngx_rbtree_गाठ_t  **मूळ, *temp, *sentinel;

    /* a binary tree insert */

    मूळ = &वृक्ष->मूळ;
    sentinel = वृक्ष->sentinel;

    यदि (*मूळ == sentinel) {
        गाठ->parent = रिक्त;
        गाठ->डावा = sentinel;
        गाठ->उजवा = sentinel;
        ngx_rbt_black(गाठ);
        *मूळ = गाठ;

        प्रत्यागम;
    }

    वृक्ष->घाला(*मूळ, गाठ, sentinel);

    /* re-balance tree */

    यावत् (गाठ != *मूळ && ngx_rbt_is_red(गाठ->parent)) {

        यदि (गाठ->parent == गाठ->parent->parent->डावा) {
            temp = गाठ->parent->parent->उजवा;

            यदि (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(गाठ->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(गाठ->parent->parent);
                गाठ = गाठ->parent->parent;

            } अन्यथा {
                यदि (गाठ == गाठ->parent->उजवा) {
                    गाठ = गाठ->parent;
                    ngx_rbtree_डावा_rotate(मूळ, sentinel, गाठ);
                }

                ngx_rbt_black(गाठ->parent);
                ngx_rbt_red(गाठ->parent->parent);
                ngx_rbtree_उजवा_rotate(मूळ, sentinel, गाठ->parent->parent);
            }

        } अन्यथा {
            temp = गाठ->parent->parent->डावा;

            यदि (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(गाठ->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(गाठ->parent->parent);
                गाठ = गाठ->parent->parent;

            } अन्यथा {
                यदि (गाठ == गाठ->parent->डावा) {
                    गाठ = गाठ->parent;
                    ngx_rbtree_उजवा_rotate(मूळ, sentinel, गाठ);
                }

                ngx_rbt_black(गाठ->parent);
                ngx_rbt_red(गाठ->parent->parent);
                ngx_rbtree_डावा_rotate(मूळ, sentinel, गाठ->parent->parent);
            }
        }
    }

    ngx_rbt_black(*मूळ);
}


शून्य
ngx_rbtree_घाला_मूल्य(ngx_rbtree_गाठ_t *temp, ngx_rbtree_गाठ_t *गाठ,
    ngx_rbtree_गाठ_t *sentinel)
{
    ngx_rbtree_गाठ_t  **p;

    प्रति ( ;; ) {

        p = (गाठ->किल्ली < temp->किल्ली) ? &temp->डावा : &temp->उजवा;

        यदि (*p == sentinel) {
            भङ्ग;
        }

        temp = *p;
    }

    *p = गाठ;
    गाठ->parent = temp;
    गाठ->डावा = sentinel;
    गाठ->उजवा = sentinel;
    ngx_rbt_red(गाठ);
}


शून्य
ngx_rbtree_घाला_timer_मूल्य(ngx_rbtree_गाठ_t *temp, ngx_rbtree_गाठ_t *गाठ,
    ngx_rbtree_गाठ_t *sentinel)
{
    ngx_rbtree_गाठ_t  **p;

    प्रति ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_किल्ली_int_t) (गाठ->किल्ली - temp->किल्ली) < 0)
            ? &temp->डावा : &temp->उजवा;

        यदि (*p == sentinel) {
            भङ्ग;
        }

        temp = *p;
    }

    *p = गाठ;
    गाठ->parent = temp;
    गाठ->डावा = sentinel;
    गाठ->उजवा = sentinel;
    ngx_rbt_red(गाठ);
}


शून्य
ngx_rbtree_delete(ngx_rbtree_t *वृक्ष, ngx_rbtree_गाठ_t *गाठ)
{
    ngx_uint_t           red;
    ngx_rbtree_गाठ_t  **मूळ, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    मूळ = &वृक्ष->मूळ;
    sentinel = वृक्ष->sentinel;

    यदि (गाठ->डावा == sentinel) {
        temp = गाठ->उजवा;
        subst = गाठ;

    } अन्यथा यदि (गाठ->उजवा == sentinel) {
        temp = गाठ->डावा;
        subst = गाठ;

    } अन्यथा {
        subst = ngx_rbtree_min(गाठ->उजवा, sentinel);
        temp = subst->उजवा;
    }

    यदि (subst == *मूळ) {
        *मूळ = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        गाठ->डावा = रिक्त;
        गाठ->उजवा = रिक्त;
        गाठ->parent = रिक्त;
        गाठ->किल्ली = 0;

        प्रत्यागम;
    }

    red = ngx_rbt_is_red(subst);

    यदि (subst == subst->parent->डावा) {
        subst->parent->डावा = temp;

    } अन्यथा {
        subst->parent->उजवा = temp;
    }

    यदि (subst == गाठ) {

        temp->parent = subst->parent;

    } अन्यथा {

        यदि (subst->parent == गाठ) {
            temp->parent = subst;

        } अन्यथा {
            temp->parent = subst->parent;
        }

        subst->डावा = गाठ->डावा;
        subst->उजवा = गाठ->उजवा;
        subst->parent = गाठ->parent;
        ngx_rbt_copy_color(subst, गाठ);

        यदि (गाठ == *मूळ) {
            *मूळ = subst;

        } अन्यथा {
            यदि (गाठ == गाठ->parent->डावा) {
                गाठ->parent->डावा = subst;
            } अन्यथा {
                गाठ->parent->उजवा = subst;
            }
        }

        यदि (subst->डावा != sentinel) {
            subst->डावा->parent = subst;
        }

        यदि (subst->उजवा != sentinel) {
            subst->उजवा->parent = subst;
        }
    }

    /* DEBUG stuff */
    गाठ->डावा = रिक्त;
    गाठ->उजवा = रिक्त;
    गाठ->parent = रिक्त;
    गाठ->किल्ली = 0;

    यदि (red) {
        प्रत्यागम;
    }

    /* a delete fixup */

    यावत् (temp != *मूळ && ngx_rbt_is_black(temp)) {

        यदि (temp == temp->parent->डावा) {
            w = temp->parent->उजवा;

            यदि (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_डावा_rotate(मूळ, sentinel, temp->parent);
                w = temp->parent->उजवा;
            }

            यदि (ngx_rbt_is_black(w->डावा) && ngx_rbt_is_black(w->उजवा)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } अन्यथा {
                यदि (ngx_rbt_is_black(w->उजवा)) {
                    ngx_rbt_black(w->डावा);
                    ngx_rbt_red(w);
                    ngx_rbtree_उजवा_rotate(मूळ, sentinel, w);
                    w = temp->parent->उजवा;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->उजवा);
                ngx_rbtree_डावा_rotate(मूळ, sentinel, temp->parent);
                temp = *मूळ;
            }

        } अन्यथा {
            w = temp->parent->डावा;

            यदि (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_उजवा_rotate(मूळ, sentinel, temp->parent);
                w = temp->parent->डावा;
            }

            यदि (ngx_rbt_is_black(w->डावा) && ngx_rbt_is_black(w->उजवा)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } अन्यथा {
                यदि (ngx_rbt_is_black(w->डावा)) {
                    ngx_rbt_black(w->उजवा);
                    ngx_rbt_red(w);
                    ngx_rbtree_डावा_rotate(मूळ, sentinel, w);
                    w = temp->parent->डावा;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->डावा);
                ngx_rbtree_उजवा_rotate(मूळ, sentinel, temp->parent);
                temp = *मूळ;
            }
        }
    }

    ngx_rbt_black(temp);
}


स्थिर ngx_inline शून्य
ngx_rbtree_डावा_rotate(ngx_rbtree_गाठ_t **मूळ, ngx_rbtree_गाठ_t *sentinel,
    ngx_rbtree_गाठ_t *गाठ)
{
    ngx_rbtree_गाठ_t  *temp;

    temp = गाठ->उजवा;
    गाठ->उजवा = temp->डावा;

    यदि (temp->डावा != sentinel) {
        temp->डावा->parent = गाठ;
    }

    temp->parent = गाठ->parent;

    यदि (गाठ == *मूळ) {
        *मूळ = temp;

    } अन्यथा यदि (गाठ == गाठ->parent->डावा) {
        गाठ->parent->डावा = temp;

    } अन्यथा {
        गाठ->parent->उजवा = temp;
    }

    temp->डावा = गाठ;
    गाठ->parent = temp;
}


स्थिर ngx_inline शून्य
ngx_rbtree_उजवा_rotate(ngx_rbtree_गाठ_t **मूळ, ngx_rbtree_गाठ_t *sentinel,
    ngx_rbtree_गाठ_t *गाठ)
{
    ngx_rbtree_गाठ_t  *temp;

    temp = गाठ->डावा;
    गाठ->डावा = temp->उजवा;

    यदि (temp->उजवा != sentinel) {
        temp->उजवा->parent = गाठ;
    }

    temp->parent = गाठ->parent;

    यदि (गाठ == *मूळ) {
        *मूळ = temp;

    } अन्यथा यदि (गाठ == गाठ->parent->उजवा) {
        गाठ->parent->उजवा = temp;

    } अन्यथा {
        गाठ->parent->डावा = temp;
    }

    temp->उजवा = गाठ;
    गाठ->parent = temp;
}


ngx_rbtree_गाठ_t *
ngx_rbtree_next(ngx_rbtree_t *वृक्ष, ngx_rbtree_गाठ_t *गाठ)
{
    ngx_rbtree_गाठ_t  *मूळ, *sentinel, *parent;

    sentinel = वृक्ष->sentinel;

    यदि (गाठ->उजवा != sentinel) {
        प्रत्यागम ngx_rbtree_min(गाठ->उजवा, sentinel);
    }

    मूळ = वृक्ष->मूळ;

    प्रति ( ;; ) {
        parent = गाठ->parent;

        यदि (गाठ == मूळ) {
            प्रत्यागम रिक्त;
        }

        यदि (गाठ == parent->डावा) {
            प्रत्यागम parent;
        }

        गाठ = parent;
    }
}
