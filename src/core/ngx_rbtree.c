
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


സ്ഥിതം ngx_inline ശൂന്യം ngx_rbtree_ഇടത്_rotate(ngx_rbtree_കണ്ണി_t **വേര്,
    ngx_rbtree_കണ്ണി_t *sentinel, ngx_rbtree_കണ്ണി_t *കണ്ണി);
സ്ഥിതം ngx_inline ശൂന്യം ngx_rbtree_വലത്_rotate(ngx_rbtree_കണ്ണി_t **വേര്,
    ngx_rbtree_കണ്ണി_t *sentinel, ngx_rbtree_കണ്ണി_t *കണ്ണി);


ശൂന്യം
ngx_rbtree_തിരുകുക(ngx_rbtree_t *വൃക്ഷം, ngx_rbtree_കണ്ണി_t *കണ്ണി)
{
    ngx_rbtree_കണ്ണി_t  **വേര്, *temp, *sentinel;

    /* a binary tree insert */

    വേര് = &വൃക്ഷം->വേര്;
    sentinel = വൃക്ഷം->sentinel;

    എങ്കിൽ (*വേര് == sentinel) {
        കണ്ണി->parent = ശൂന്യ;
        കണ്ണി->ഇടത് = sentinel;
        കണ്ണി->വലത് = sentinel;
        ngx_rbt_black(കണ്ണി);
        *വേര് = കണ്ണി;

        തിരികെ;
    }

    വൃക്ഷം->തിരുകുക(*വേര്, കണ്ണി, sentinel);

    /* re-balance tree */

    ആകുമ്പോൾ (കണ്ണി != *വേര് && ngx_rbt_is_red(കണ്ണി->parent)) {

        എങ്കിൽ (കണ്ണി->parent == കണ്ണി->parent->parent->ഇടത്) {
            temp = കണ്ണി->parent->parent->വലത്;

            എങ്കിൽ (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(കണ്ണി->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(കണ്ണി->parent->parent);
                കണ്ണി = കണ്ണി->parent->parent;

            } അല്ലെങ്കിൽ {
                എങ്കിൽ (കണ്ണി == കണ്ണി->parent->വലത്) {
                    കണ്ണി = കണ്ണി->parent;
                    ngx_rbtree_ഇടത്_rotate(വേര്, sentinel, കണ്ണി);
                }

                ngx_rbt_black(കണ്ണി->parent);
                ngx_rbt_red(കണ്ണി->parent->parent);
                ngx_rbtree_വലത്_rotate(വേര്, sentinel, കണ്ണി->parent->parent);
            }

        } അല്ലെങ്കിൽ {
            temp = കണ്ണി->parent->parent->ഇടത്;

            എങ്കിൽ (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(കണ്ണി->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(കണ്ണി->parent->parent);
                കണ്ണി = കണ്ണി->parent->parent;

            } അല്ലെങ്കിൽ {
                എങ്കിൽ (കണ്ണി == കണ്ണി->parent->ഇടത്) {
                    കണ്ണി = കണ്ണി->parent;
                    ngx_rbtree_വലത്_rotate(വേര്, sentinel, കണ്ണി);
                }

                ngx_rbt_black(കണ്ണി->parent);
                ngx_rbt_red(കണ്ണി->parent->parent);
                ngx_rbtree_ഇടത്_rotate(വേര്, sentinel, കണ്ണി->parent->parent);
            }
        }
    }

    ngx_rbt_black(*വേര്);
}


ശൂന്യം
ngx_rbtree_തിരുകുക_മൂല്യം(ngx_rbtree_കണ്ണി_t *temp, ngx_rbtree_കണ്ണി_t *കണ്ണി,
    ngx_rbtree_കണ്ണി_t *sentinel)
{
    ngx_rbtree_കണ്ണി_t  **p;

    വേണ്ടി ( ;; ) {

        p = (കണ്ണി->താക്കോൽ < temp->താക്കോൽ) ? &temp->ഇടത് : &temp->വലത്;

        എങ്കിൽ (*p == sentinel) {
            നിർത്തുക;
        }

        temp = *p;
    }

    *p = കണ്ണി;
    കണ്ണി->parent = temp;
    കണ്ണി->ഇടത് = sentinel;
    കണ്ണി->വലത് = sentinel;
    ngx_rbt_red(കണ്ണി);
}


ശൂന്യം
ngx_rbtree_തിരുകുക_timer_മൂല്യം(ngx_rbtree_കണ്ണി_t *temp, ngx_rbtree_കണ്ണി_t *കണ്ണി,
    ngx_rbtree_കണ്ണി_t *sentinel)
{
    ngx_rbtree_കണ്ണി_t  **p;

    വേണ്ടി ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_താക്കോൽ_int_t) (കണ്ണി->താക്കോൽ - temp->താക്കോൽ) < 0)
            ? &temp->ഇടത് : &temp->വലത്;

        എങ്കിൽ (*p == sentinel) {
            നിർത്തുക;
        }

        temp = *p;
    }

    *p = കണ്ണി;
    കണ്ണി->parent = temp;
    കണ്ണി->ഇടത് = sentinel;
    കണ്ണി->വലത് = sentinel;
    ngx_rbt_red(കണ്ണി);
}


ശൂന്യം
ngx_rbtree_delete(ngx_rbtree_t *വൃക്ഷം, ngx_rbtree_കണ്ണി_t *കണ്ണി)
{
    ngx_uint_t           red;
    ngx_rbtree_കണ്ണി_t  **വേര്, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    വേര് = &വൃക്ഷം->വേര്;
    sentinel = വൃക്ഷം->sentinel;

    എങ്കിൽ (കണ്ണി->ഇടത് == sentinel) {
        temp = കണ്ണി->വലത്;
        subst = കണ്ണി;

    } അല്ലെങ്കിൽ എങ്കിൽ (കണ്ണി->വലത് == sentinel) {
        temp = കണ്ണി->ഇടത്;
        subst = കണ്ണി;

    } അല്ലെങ്കിൽ {
        subst = ngx_rbtree_min(കണ്ണി->വലത്, sentinel);
        temp = subst->വലത്;
    }

    എങ്കിൽ (subst == *വേര്) {
        *വേര് = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        കണ്ണി->ഇടത് = ശൂന്യ;
        കണ്ണി->വലത് = ശൂന്യ;
        കണ്ണി->parent = ശൂന്യ;
        കണ്ണി->താക്കോൽ = 0;

        തിരികെ;
    }

    red = ngx_rbt_is_red(subst);

    എങ്കിൽ (subst == subst->parent->ഇടത്) {
        subst->parent->ഇടത് = temp;

    } അല്ലെങ്കിൽ {
        subst->parent->വലത് = temp;
    }

    എങ്കിൽ (subst == കണ്ണി) {

        temp->parent = subst->parent;

    } അല്ലെങ്കിൽ {

        എങ്കിൽ (subst->parent == കണ്ണി) {
            temp->parent = subst;

        } അല്ലെങ്കിൽ {
            temp->parent = subst->parent;
        }

        subst->ഇടത് = കണ്ണി->ഇടത്;
        subst->വലത് = കണ്ണി->വലത്;
        subst->parent = കണ്ണി->parent;
        ngx_rbt_copy_color(subst, കണ്ണി);

        എങ്കിൽ (കണ്ണി == *വേര്) {
            *വേര് = subst;

        } അല്ലെങ്കിൽ {
            എങ്കിൽ (കണ്ണി == കണ്ണി->parent->ഇടത്) {
                കണ്ണി->parent->ഇടത് = subst;
            } അല്ലെങ്കിൽ {
                കണ്ണി->parent->വലത് = subst;
            }
        }

        എങ്കിൽ (subst->ഇടത് != sentinel) {
            subst->ഇടത്->parent = subst;
        }

        എങ്കിൽ (subst->വലത് != sentinel) {
            subst->വലത്->parent = subst;
        }
    }

    /* DEBUG stuff */
    കണ്ണി->ഇടത് = ശൂന്യ;
    കണ്ണി->വലത് = ശൂന്യ;
    കണ്ണി->parent = ശൂന്യ;
    കണ്ണി->താക്കോൽ = 0;

    എങ്കിൽ (red) {
        തിരികെ;
    }

    /* a delete fixup */

    ആകുമ്പോൾ (temp != *വേര് && ngx_rbt_is_black(temp)) {

        എങ്കിൽ (temp == temp->parent->ഇടത്) {
            w = temp->parent->വലത്;

            എങ്കിൽ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_ഇടത്_rotate(വേര്, sentinel, temp->parent);
                w = temp->parent->വലത്;
            }

            എങ്കിൽ (ngx_rbt_is_black(w->ഇടത്) && ngx_rbt_is_black(w->വലത്)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } അല്ലെങ്കിൽ {
                എങ്കിൽ (ngx_rbt_is_black(w->വലത്)) {
                    ngx_rbt_black(w->ഇടത്);
                    ngx_rbt_red(w);
                    ngx_rbtree_വലത്_rotate(വേര്, sentinel, w);
                    w = temp->parent->വലത്;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->വലത്);
                ngx_rbtree_ഇടത്_rotate(വേര്, sentinel, temp->parent);
                temp = *വേര്;
            }

        } അല്ലെങ്കിൽ {
            w = temp->parent->ഇടത്;

            എങ്കിൽ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_വലത്_rotate(വേര്, sentinel, temp->parent);
                w = temp->parent->ഇടത്;
            }

            എങ്കിൽ (ngx_rbt_is_black(w->ഇടത്) && ngx_rbt_is_black(w->വലത്)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } അല്ലെങ്കിൽ {
                എങ്കിൽ (ngx_rbt_is_black(w->ഇടത്)) {
                    ngx_rbt_black(w->വലത്);
                    ngx_rbt_red(w);
                    ngx_rbtree_ഇടത്_rotate(വേര്, sentinel, w);
                    w = temp->parent->ഇടത്;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->ഇടത്);
                ngx_rbtree_വലത്_rotate(വേര്, sentinel, temp->parent);
                temp = *വേര്;
            }
        }
    }

    ngx_rbt_black(temp);
}


സ്ഥിതം ngx_inline ശൂന്യം
ngx_rbtree_ഇടത്_rotate(ngx_rbtree_കണ്ണി_t **വേര്, ngx_rbtree_കണ്ണി_t *sentinel,
    ngx_rbtree_കണ്ണി_t *കണ്ണി)
{
    ngx_rbtree_കണ്ണി_t  *temp;

    temp = കണ്ണി->വലത്;
    കണ്ണി->വലത് = temp->ഇടത്;

    എങ്കിൽ (temp->ഇടത് != sentinel) {
        temp->ഇടത്->parent = കണ്ണി;
    }

    temp->parent = കണ്ണി->parent;

    എങ്കിൽ (കണ്ണി == *വേര്) {
        *വേര് = temp;

    } അല്ലെങ്കിൽ എങ്കിൽ (കണ്ണി == കണ്ണി->parent->ഇടത്) {
        കണ്ണി->parent->ഇടത് = temp;

    } അല്ലെങ്കിൽ {
        കണ്ണി->parent->വലത് = temp;
    }

    temp->ഇടത് = കണ്ണി;
    കണ്ണി->parent = temp;
}


സ്ഥിതം ngx_inline ശൂന്യം
ngx_rbtree_വലത്_rotate(ngx_rbtree_കണ്ണി_t **വേര്, ngx_rbtree_കണ്ണി_t *sentinel,
    ngx_rbtree_കണ്ണി_t *കണ്ണി)
{
    ngx_rbtree_കണ്ണി_t  *temp;

    temp = കണ്ണി->ഇടത്;
    കണ്ണി->ഇടത് = temp->വലത്;

    എങ്കിൽ (temp->വലത് != sentinel) {
        temp->വലത്->parent = കണ്ണി;
    }

    temp->parent = കണ്ണി->parent;

    എങ്കിൽ (കണ്ണി == *വേര്) {
        *വേര് = temp;

    } അല്ലെങ്കിൽ എങ്കിൽ (കണ്ണി == കണ്ണി->parent->വലത്) {
        കണ്ണി->parent->വലത് = temp;

    } അല്ലെങ്കിൽ {
        കണ്ണി->parent->ഇടത് = temp;
    }

    temp->വലത് = കണ്ണി;
    കണ്ണി->parent = temp;
}


ngx_rbtree_കണ്ണി_t *
ngx_rbtree_next(ngx_rbtree_t *വൃക്ഷം, ngx_rbtree_കണ്ണി_t *കണ്ണി)
{
    ngx_rbtree_കണ്ണി_t  *വേര്, *sentinel, *parent;

    sentinel = വൃക്ഷം->sentinel;

    എങ്കിൽ (കണ്ണി->വലത് != sentinel) {
        തിരികെ ngx_rbtree_min(കണ്ണി->വലത്, sentinel);
    }

    വേര് = വൃക്ഷം->വേര്;

    വേണ്ടി ( ;; ) {
        parent = കണ്ണി->parent;

        എങ്കിൽ (കണ്ണി == വേര്) {
            തിരികെ ശൂന്യ;
        }

        എങ്കിൽ (കണ്ണി == parent->ഇടത്) {
            തിരികെ parent;
        }

        കണ്ണി = parent;
    }
}
