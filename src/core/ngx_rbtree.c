
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


अटल ngx_inline व्योम ngx_rbtree_बायाँ_rotate(ngx_rbtree_गाँठ_t **मूल,
    ngx_rbtree_गाँठ_t *sentinel, ngx_rbtree_गाँठ_t *गाँठ);
अटल ngx_inline व्योम ngx_rbtree_दायाँ_rotate(ngx_rbtree_गाँठ_t **मूल,
    ngx_rbtree_गाँठ_t *sentinel, ngx_rbtree_गाँठ_t *गाँठ);


व्योम
ngx_rbtree_जोड़ो(ngx_rbtree_t *वृक्ष, ngx_rbtree_गाँठ_t *गाँठ)
{
    ngx_rbtree_गाँठ_t  **मूल, *अस्थायी, *sentinel;

    /* a binary tree insert */

    मूल = &वृक्ष->मूल;
    sentinel = वृक्ष->sentinel;

    अगर (*मूल == sentinel) {
        गाँठ->parent = शून्य;
        गाँठ->बायाँ = sentinel;
        गाँठ->दायाँ = sentinel;
        ngx_rbt_black(गाँठ);
        *मूल = गाँठ;

        वापस;
    }

    वृक्ष->जोड़ो(*मूल, गाँठ, sentinel);

    /* re-balance tree */

    जबतक (गाँठ != *मूल && ngx_rbt_is_red(गाँठ->parent)) {

        अगर (गाँठ->parent == गाँठ->parent->parent->बायाँ) {
            अस्थायी = गाँठ->parent->parent->दायाँ;

            अगर (ngx_rbt_is_red(अस्थायी)) {
                ngx_rbt_black(गाँठ->parent);
                ngx_rbt_black(अस्थायी);
                ngx_rbt_red(गाँठ->parent->parent);
                गाँठ = गाँठ->parent->parent;

            } अन्यथा {
                अगर (गाँठ == गाँठ->parent->दायाँ) {
                    गाँठ = गाँठ->parent;
                    ngx_rbtree_बायाँ_rotate(मूल, sentinel, गाँठ);
                }

                ngx_rbt_black(गाँठ->parent);
                ngx_rbt_red(गाँठ->parent->parent);
                ngx_rbtree_दायाँ_rotate(मूल, sentinel, गाँठ->parent->parent);
            }

        } अन्यथा {
            अस्थायी = गाँठ->parent->parent->बायाँ;

            अगर (ngx_rbt_is_red(अस्थायी)) {
                ngx_rbt_black(गाँठ->parent);
                ngx_rbt_black(अस्थायी);
                ngx_rbt_red(गाँठ->parent->parent);
                गाँठ = गाँठ->parent->parent;

            } अन्यथा {
                अगर (गाँठ == गाँठ->parent->बायाँ) {
                    गाँठ = गाँठ->parent;
                    ngx_rbtree_दायाँ_rotate(मूल, sentinel, गाँठ);
                }

                ngx_rbt_black(गाँठ->parent);
                ngx_rbt_red(गाँठ->parent->parent);
                ngx_rbtree_बायाँ_rotate(मूल, sentinel, गाँठ->parent->parent);
            }
        }
    }

    ngx_rbt_black(*मूल);
}


व्योम
ngx_rbtree_जोड़ो_मूल्य(ngx_rbtree_गाँठ_t *अस्थायी, ngx_rbtree_गाँठ_t *गाँठ,
    ngx_rbtree_गाँठ_t *sentinel)
{
    ngx_rbtree_गाँठ_t  **p;

    क्रम ( ;; ) {

        p = (गाँठ->कुंजी < अस्थायी->कुंजी) ? &अस्थायी->बायाँ : &अस्थायी->दायाँ;

        अगर (*p == sentinel) {
            अवरोध;
        }

        अस्थायी = *p;
    }

    *p = गाँठ;
    गाँठ->parent = अस्थायी;
    गाँठ->बायाँ = sentinel;
    गाँठ->दायाँ = sentinel;
    ngx_rbt_red(गाँठ);
}


व्योम
ngx_rbtree_जोड़ो_timer_मूल्य(ngx_rbtree_गाँठ_t *अस्थायी, ngx_rbtree_गाँठ_t *गाँठ,
    ngx_rbtree_गाँठ_t *sentinel)
{
    ngx_rbtree_गाँठ_t  **p;

    क्रम ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_कुंजी_int_t) (गाँठ->कुंजी - अस्थायी->कुंजी) < 0)
            ? &अस्थायी->बायाँ : &अस्थायी->दायाँ;

        अगर (*p == sentinel) {
            अवरोध;
        }

        अस्थायी = *p;
    }

    *p = गाँठ;
    गाँठ->parent = अस्थायी;
    गाँठ->बायाँ = sentinel;
    गाँठ->दायाँ = sentinel;
    ngx_rbt_red(गाँठ);
}


व्योम
ngx_rbtree_delete(ngx_rbtree_t *वृक्ष, ngx_rbtree_गाँठ_t *गाँठ)
{
    ngx_uint_t           red;
    ngx_rbtree_गाँठ_t  **मूल, *sentinel, *subst, *अस्थायी, *w;

    /* a binary tree delete */

    मूल = &वृक्ष->मूल;
    sentinel = वृक्ष->sentinel;

    अगर (गाँठ->बायाँ == sentinel) {
        अस्थायी = गाँठ->दायाँ;
        subst = गाँठ;

    } अन्यथा अगर (गाँठ->दायाँ == sentinel) {
        अस्थायी = गाँठ->बायाँ;
        subst = गाँठ;

    } अन्यथा {
        subst = ngx_rbtree_min(गाँठ->दायाँ, sentinel);
        अस्थायी = subst->दायाँ;
    }

    अगर (subst == *मूल) {
        *मूल = अस्थायी;
        ngx_rbt_black(अस्थायी);

        /* DEBUG stuff */
        गाँठ->बायाँ = शून्य;
        गाँठ->दायाँ = शून्य;
        गाँठ->parent = शून्य;
        गाँठ->कुंजी = 0;

        वापस;
    }

    red = ngx_rbt_is_red(subst);

    अगर (subst == subst->parent->बायाँ) {
        subst->parent->बायाँ = अस्थायी;

    } अन्यथा {
        subst->parent->दायाँ = अस्थायी;
    }

    अगर (subst == गाँठ) {

        अस्थायी->parent = subst->parent;

    } अन्यथा {

        अगर (subst->parent == गाँठ) {
            अस्थायी->parent = subst;

        } अन्यथा {
            अस्थायी->parent = subst->parent;
        }

        subst->बायाँ = गाँठ->बायाँ;
        subst->दायाँ = गाँठ->दायाँ;
        subst->parent = गाँठ->parent;
        ngx_rbt_copy_color(subst, गाँठ);

        अगर (गाँठ == *मूल) {
            *मूल = subst;

        } अन्यथा {
            अगर (गाँठ == गाँठ->parent->बायाँ) {
                गाँठ->parent->बायाँ = subst;
            } अन्यथा {
                गाँठ->parent->दायाँ = subst;
            }
        }

        अगर (subst->बायाँ != sentinel) {
            subst->बायाँ->parent = subst;
        }

        अगर (subst->दायाँ != sentinel) {
            subst->दायाँ->parent = subst;
        }
    }

    /* DEBUG stuff */
    गाँठ->बायाँ = शून्य;
    गाँठ->दायाँ = शून्य;
    गाँठ->parent = शून्य;
    गाँठ->कुंजी = 0;

    अगर (red) {
        वापस;
    }

    /* a delete fixup */

    जबतक (अस्थायी != *मूल && ngx_rbt_is_black(अस्थायी)) {

        अगर (अस्थायी == अस्थायी->parent->बायाँ) {
            w = अस्थायी->parent->दायाँ;

            अगर (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(अस्थायी->parent);
                ngx_rbtree_बायाँ_rotate(मूल, sentinel, अस्थायी->parent);
                w = अस्थायी->parent->दायाँ;
            }

            अगर (ngx_rbt_is_black(w->बायाँ) && ngx_rbt_is_black(w->दायाँ)) {
                ngx_rbt_red(w);
                अस्थायी = अस्थायी->parent;

            } अन्यथा {
                अगर (ngx_rbt_is_black(w->दायाँ)) {
                    ngx_rbt_black(w->बायाँ);
                    ngx_rbt_red(w);
                    ngx_rbtree_दायाँ_rotate(मूल, sentinel, w);
                    w = अस्थायी->parent->दायाँ;
                }

                ngx_rbt_copy_color(w, अस्थायी->parent);
                ngx_rbt_black(अस्थायी->parent);
                ngx_rbt_black(w->दायाँ);
                ngx_rbtree_बायाँ_rotate(मूल, sentinel, अस्थायी->parent);
                अस्थायी = *मूल;
            }

        } अन्यथा {
            w = अस्थायी->parent->बायाँ;

            अगर (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(अस्थायी->parent);
                ngx_rbtree_दायाँ_rotate(मूल, sentinel, अस्थायी->parent);
                w = अस्थायी->parent->बायाँ;
            }

            अगर (ngx_rbt_is_black(w->बायाँ) && ngx_rbt_is_black(w->दायाँ)) {
                ngx_rbt_red(w);
                अस्थायी = अस्थायी->parent;

            } अन्यथा {
                अगर (ngx_rbt_is_black(w->बायाँ)) {
                    ngx_rbt_black(w->दायाँ);
                    ngx_rbt_red(w);
                    ngx_rbtree_बायाँ_rotate(मूल, sentinel, w);
                    w = अस्थायी->parent->बायाँ;
                }

                ngx_rbt_copy_color(w, अस्थायी->parent);
                ngx_rbt_black(अस्थायी->parent);
                ngx_rbt_black(w->बायाँ);
                ngx_rbtree_दायाँ_rotate(मूल, sentinel, अस्थायी->parent);
                अस्थायी = *मूल;
            }
        }
    }

    ngx_rbt_black(अस्थायी);
}


अटल ngx_inline व्योम
ngx_rbtree_बायाँ_rotate(ngx_rbtree_गाँठ_t **मूल, ngx_rbtree_गाँठ_t *sentinel,
    ngx_rbtree_गाँठ_t *गाँठ)
{
    ngx_rbtree_गाँठ_t  *अस्थायी;

    अस्थायी = गाँठ->दायाँ;
    गाँठ->दायाँ = अस्थायी->बायाँ;

    अगर (अस्थायी->बायाँ != sentinel) {
        अस्थायी->बायाँ->parent = गाँठ;
    }

    अस्थायी->parent = गाँठ->parent;

    अगर (गाँठ == *मूल) {
        *मूल = अस्थायी;

    } अन्यथा अगर (गाँठ == गाँठ->parent->बायाँ) {
        गाँठ->parent->बायाँ = अस्थायी;

    } अन्यथा {
        गाँठ->parent->दायाँ = अस्थायी;
    }

    अस्थायी->बायाँ = गाँठ;
    गाँठ->parent = अस्थायी;
}


अटल ngx_inline व्योम
ngx_rbtree_दायाँ_rotate(ngx_rbtree_गाँठ_t **मूल, ngx_rbtree_गाँठ_t *sentinel,
    ngx_rbtree_गाँठ_t *गाँठ)
{
    ngx_rbtree_गाँठ_t  *अस्थायी;

    अस्थायी = गाँठ->बायाँ;
    गाँठ->बायाँ = अस्थायी->दायाँ;

    अगर (अस्थायी->दायाँ != sentinel) {
        अस्थायी->दायाँ->parent = गाँठ;
    }

    अस्थायी->parent = गाँठ->parent;

    अगर (गाँठ == *मूल) {
        *मूल = अस्थायी;

    } अन्यथा अगर (गाँठ == गाँठ->parent->दायाँ) {
        गाँठ->parent->दायाँ = अस्थायी;

    } अन्यथा {
        गाँठ->parent->बायाँ = अस्थायी;
    }

    अस्थायी->दायाँ = गाँठ;
    गाँठ->parent = अस्थायी;
}


ngx_rbtree_गाँठ_t *
ngx_rbtree_अग्र(ngx_rbtree_t *वृक्ष, ngx_rbtree_गाँठ_t *गाँठ)
{
    ngx_rbtree_गाँठ_t  *मूल, *sentinel, *parent;

    sentinel = वृक्ष->sentinel;

    अगर (गाँठ->दायाँ != sentinel) {
        वापस ngx_rbtree_min(गाँठ->दायाँ, sentinel);
    }

    मूल = वृक्ष->मूल;

    क्रम ( ;; ) {
        parent = गाँठ->parent;

        अगर (गाँठ == मूल) {
            वापस शून्य;
        }

        अगर (गाँठ == parent->बायाँ) {
            वापस parent;
        }

        गाँठ = parent;
    }
}
