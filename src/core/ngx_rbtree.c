
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


நிலையான ngx_inline வெறுமை ngx_rbtree_இடது_rotate(ngx_rbtree_முனை_t **வேர்,
    ngx_rbtree_முனை_t *sentinel, ngx_rbtree_முனை_t *முனை);
நிலையான ngx_inline வெறுமை ngx_rbtree_வலது_rotate(ngx_rbtree_முனை_t **வேர்,
    ngx_rbtree_முனை_t *sentinel, ngx_rbtree_முனை_t *முனை);


வெறுமை
ngx_rbtree_செருகு(ngx_rbtree_t *மரம், ngx_rbtree_முனை_t *முனை)
{
    ngx_rbtree_முனை_t  **வேர், *temp, *sentinel;

    /* a binary tree insert */

    வேர் = &மரம்->வேர்;
    sentinel = மரம்->sentinel;

    எனில் (*வேர் == sentinel) {
        முனை->parent = வெற்று;
        முனை->இடது = sentinel;
        முனை->வலது = sentinel;
        ngx_rbt_black(முனை);
        *வேர் = முனை;

        திரும்பு;
    }

    மரம்->செருகு(*வேர், முனை, sentinel);

    /* re-balance tree */

    போது (முனை != *வேர் && ngx_rbt_is_red(முனை->parent)) {

        எனில் (முனை->parent == முனை->parent->parent->இடது) {
            temp = முனை->parent->parent->வலது;

            எனில் (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(முனை->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(முனை->parent->parent);
                முனை = முனை->parent->parent;

            } இல்லையெனில் {
                எனில் (முனை == முனை->parent->வலது) {
                    முனை = முனை->parent;
                    ngx_rbtree_இடது_rotate(வேர், sentinel, முனை);
                }

                ngx_rbt_black(முனை->parent);
                ngx_rbt_red(முனை->parent->parent);
                ngx_rbtree_வலது_rotate(வேர், sentinel, முனை->parent->parent);
            }

        } இல்லையெனில் {
            temp = முனை->parent->parent->இடது;

            எனில் (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(முனை->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(முனை->parent->parent);
                முனை = முனை->parent->parent;

            } இல்லையெனில் {
                எனில் (முனை == முனை->parent->இடது) {
                    முனை = முனை->parent;
                    ngx_rbtree_வலது_rotate(வேர், sentinel, முனை);
                }

                ngx_rbt_black(முனை->parent);
                ngx_rbt_red(முனை->parent->parent);
                ngx_rbtree_இடது_rotate(வேர், sentinel, முனை->parent->parent);
            }
        }
    }

    ngx_rbt_black(*வேர்);
}


வெறுமை
ngx_rbtree_செருகு_மதிப்பு(ngx_rbtree_முனை_t *temp, ngx_rbtree_முனை_t *முனை,
    ngx_rbtree_முனை_t *sentinel)
{
    ngx_rbtree_முனை_t  **p;

    க்கு ( ;; ) {

        p = (முனை->சாவி < temp->சாவி) ? &temp->இடது : &temp->வலது;

        எனில் (*p == sentinel) {
            நிறுத்து;
        }

        temp = *p;
    }

    *p = முனை;
    முனை->parent = temp;
    முனை->இடது = sentinel;
    முனை->வலது = sentinel;
    ngx_rbt_red(முனை);
}


வெறுமை
ngx_rbtree_செருகு_timer_மதிப்பு(ngx_rbtree_முனை_t *temp, ngx_rbtree_முனை_t *முனை,
    ngx_rbtree_முனை_t *sentinel)
{
    ngx_rbtree_முனை_t  **p;

    க்கு ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_சாவி_int_t) (முனை->சாவி - temp->சாவி) < 0)
            ? &temp->இடது : &temp->வலது;

        எனில் (*p == sentinel) {
            நிறுத்து;
        }

        temp = *p;
    }

    *p = முனை;
    முனை->parent = temp;
    முனை->இடது = sentinel;
    முனை->வலது = sentinel;
    ngx_rbt_red(முனை);
}


வெறுமை
ngx_rbtree_delete(ngx_rbtree_t *மரம், ngx_rbtree_முனை_t *முனை)
{
    ngx_uint_t           red;
    ngx_rbtree_முனை_t  **வேர், *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    வேர் = &மரம்->வேர்;
    sentinel = மரம்->sentinel;

    எனில் (முனை->இடது == sentinel) {
        temp = முனை->வலது;
        subst = முனை;

    } இல்லையெனில் எனில் (முனை->வலது == sentinel) {
        temp = முனை->இடது;
        subst = முனை;

    } இல்லையெனில் {
        subst = ngx_rbtree_min(முனை->வலது, sentinel);
        temp = subst->வலது;
    }

    எனில் (subst == *வேர்) {
        *வேர் = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        முனை->இடது = வெற்று;
        முனை->வலது = வெற்று;
        முனை->parent = வெற்று;
        முனை->சாவி = 0;

        திரும்பு;
    }

    red = ngx_rbt_is_red(subst);

    எனில் (subst == subst->parent->இடது) {
        subst->parent->இடது = temp;

    } இல்லையெனில் {
        subst->parent->வலது = temp;
    }

    எனில் (subst == முனை) {

        temp->parent = subst->parent;

    } இல்லையெனில் {

        எனில் (subst->parent == முனை) {
            temp->parent = subst;

        } இல்லையெனில் {
            temp->parent = subst->parent;
        }

        subst->இடது = முனை->இடது;
        subst->வலது = முனை->வலது;
        subst->parent = முனை->parent;
        ngx_rbt_copy_color(subst, முனை);

        எனில் (முனை == *வேர்) {
            *வேர் = subst;

        } இல்லையெனில் {
            எனில் (முனை == முனை->parent->இடது) {
                முனை->parent->இடது = subst;
            } இல்லையெனில் {
                முனை->parent->வலது = subst;
            }
        }

        எனில் (subst->இடது != sentinel) {
            subst->இடது->parent = subst;
        }

        எனில் (subst->வலது != sentinel) {
            subst->வலது->parent = subst;
        }
    }

    /* DEBUG stuff */
    முனை->இடது = வெற்று;
    முனை->வலது = வெற்று;
    முனை->parent = வெற்று;
    முனை->சாவி = 0;

    எனில் (red) {
        திரும்பு;
    }

    /* a delete fixup */

    போது (temp != *வேர் && ngx_rbt_is_black(temp)) {

        எனில் (temp == temp->parent->இடது) {
            w = temp->parent->வலது;

            எனில் (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_இடது_rotate(வேர், sentinel, temp->parent);
                w = temp->parent->வலது;
            }

            எனில் (ngx_rbt_is_black(w->இடது) && ngx_rbt_is_black(w->வலது)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } இல்லையெனில் {
                எனில் (ngx_rbt_is_black(w->வலது)) {
                    ngx_rbt_black(w->இடது);
                    ngx_rbt_red(w);
                    ngx_rbtree_வலது_rotate(வேர், sentinel, w);
                    w = temp->parent->வலது;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->வலது);
                ngx_rbtree_இடது_rotate(வேர், sentinel, temp->parent);
                temp = *வேர்;
            }

        } இல்லையெனில் {
            w = temp->parent->இடது;

            எனில் (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_வலது_rotate(வேர், sentinel, temp->parent);
                w = temp->parent->இடது;
            }

            எனில் (ngx_rbt_is_black(w->இடது) && ngx_rbt_is_black(w->வலது)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } இல்லையெனில் {
                எனில் (ngx_rbt_is_black(w->இடது)) {
                    ngx_rbt_black(w->வலது);
                    ngx_rbt_red(w);
                    ngx_rbtree_இடது_rotate(வேர், sentinel, w);
                    w = temp->parent->இடது;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->இடது);
                ngx_rbtree_வலது_rotate(வேர், sentinel, temp->parent);
                temp = *வேர்;
            }
        }
    }

    ngx_rbt_black(temp);
}


நிலையான ngx_inline வெறுமை
ngx_rbtree_இடது_rotate(ngx_rbtree_முனை_t **வேர், ngx_rbtree_முனை_t *sentinel,
    ngx_rbtree_முனை_t *முனை)
{
    ngx_rbtree_முனை_t  *temp;

    temp = முனை->வலது;
    முனை->வலது = temp->இடது;

    எனில் (temp->இடது != sentinel) {
        temp->இடது->parent = முனை;
    }

    temp->parent = முனை->parent;

    எனில் (முனை == *வேர்) {
        *வேர் = temp;

    } இல்லையெனில் எனில் (முனை == முனை->parent->இடது) {
        முனை->parent->இடது = temp;

    } இல்லையெனில் {
        முனை->parent->வலது = temp;
    }

    temp->இடது = முனை;
    முனை->parent = temp;
}


நிலையான ngx_inline வெறுமை
ngx_rbtree_வலது_rotate(ngx_rbtree_முனை_t **வேர், ngx_rbtree_முனை_t *sentinel,
    ngx_rbtree_முனை_t *முனை)
{
    ngx_rbtree_முனை_t  *temp;

    temp = முனை->இடது;
    முனை->இடது = temp->வலது;

    எனில் (temp->வலது != sentinel) {
        temp->வலது->parent = முனை;
    }

    temp->parent = முனை->parent;

    எனில் (முனை == *வேர்) {
        *வேர் = temp;

    } இல்லையெனில் எனில் (முனை == முனை->parent->வலது) {
        முனை->parent->வலது = temp;

    } இல்லையெனில் {
        முனை->parent->இடது = temp;
    }

    temp->வலது = முனை;
    முனை->parent = temp;
}


ngx_rbtree_முனை_t *
ngx_rbtree_next(ngx_rbtree_t *மரம், ngx_rbtree_முனை_t *முனை)
{
    ngx_rbtree_முனை_t  *வேர், *sentinel, *parent;

    sentinel = மரம்->sentinel;

    எனில் (முனை->வலது != sentinel) {
        திரும்பு ngx_rbtree_min(முனை->வலது, sentinel);
    }

    வேர் = மரம்->வேர்;

    க்கு ( ;; ) {
        parent = முனை->parent;

        எனில் (முனை == வேர்) {
            திரும்பு வெற்று;
        }

        எனில் (முனை == parent->இடது) {
            திரும்பு parent;
        }

        முனை = parent;
    }
}
