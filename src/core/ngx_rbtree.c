
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


स्थिर ngx_inline शून्य ngx_rbtree_बायाँ_rotate(ngx_rbtree_गाँठो_t **जरा,
    ngx_rbtree_गाँठो_t *sentinel, ngx_rbtree_गाँठो_t *गाँठो);
स्थिर ngx_inline शून्य ngx_rbtree_दायाँ_rotate(ngx_rbtree_गाँठो_t **जरा,
    ngx_rbtree_गाँठो_t *sentinel, ngx_rbtree_गाँठो_t *गाँठो);


शून्य
ngx_rbtree_घुसाउ(ngx_rbtree_t *रूख, ngx_rbtree_गाँठो_t *गाँठो)
{
    ngx_rbtree_गाँठो_t  **जरा, *temp, *sentinel;

    /* a binary tree insert */

    जरा = &रूख->जरा;
    sentinel = रूख->sentinel;

    यदि (*जरा == sentinel) {
        गाँठो->parent = रिक्त;
        गाँठो->बायाँ = sentinel;
        गाँठो->दायाँ = sentinel;
        ngx_rbt_black(गाँठो);
        *जरा = गाँठो;

        फर्काऊ;
    }

    रूख->घुसाउ(*जरा, गाँठो, sentinel);

    /* re-balance tree */

    जबसम्म (गाँठो != *जरा && ngx_rbt_is_red(गाँठो->parent)) {

        यदि (गाँठो->parent == गाँठो->parent->parent->बायाँ) {
            temp = गाँठो->parent->parent->दायाँ;

            यदि (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(गाँठो->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(गाँठो->parent->parent);
                गाँठो = गाँठो->parent->parent;

            } अन्यथा {
                यदि (गाँठो == गाँठो->parent->दायाँ) {
                    गाँठो = गाँठो->parent;
                    ngx_rbtree_बायाँ_rotate(जरा, sentinel, गाँठो);
                }

                ngx_rbt_black(गाँठो->parent);
                ngx_rbt_red(गाँठो->parent->parent);
                ngx_rbtree_दायाँ_rotate(जरा, sentinel, गाँठो->parent->parent);
            }

        } अन्यथा {
            temp = गाँठो->parent->parent->बायाँ;

            यदि (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(गाँठो->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(गाँठो->parent->parent);
                गाँठो = गाँठो->parent->parent;

            } अन्यथा {
                यदि (गाँठो == गाँठो->parent->बायाँ) {
                    गाँठो = गाँठो->parent;
                    ngx_rbtree_दायाँ_rotate(जरा, sentinel, गाँठो);
                }

                ngx_rbt_black(गाँठो->parent);
                ngx_rbt_red(गाँठो->parent->parent);
                ngx_rbtree_बायाँ_rotate(जरा, sentinel, गाँठो->parent->parent);
            }
        }
    }

    ngx_rbt_black(*जरा);
}


शून्य
ngx_rbtree_घुसाउ_मान(ngx_rbtree_गाँठो_t *temp, ngx_rbtree_गाँठो_t *गाँठो,
    ngx_rbtree_गाँठो_t *sentinel)
{
    ngx_rbtree_गाँठो_t  **p;

    लागि ( ;; ) {

        p = (गाँठो->साँचो < temp->साँचो) ? &temp->बायाँ : &temp->दायाँ;

        यदि (*p == sentinel) {
            रोक;
        }

        temp = *p;
    }

    *p = गाँठो;
    गाँठो->parent = temp;
    गाँठो->बायाँ = sentinel;
    गाँठो->दायाँ = sentinel;
    ngx_rbt_red(गाँठो);
}


शून्य
ngx_rbtree_घुसाउ_timer_मान(ngx_rbtree_गाँठो_t *temp, ngx_rbtree_गाँठो_t *गाँठो,
    ngx_rbtree_गाँठो_t *sentinel)
{
    ngx_rbtree_गाँठो_t  **p;

    लागि ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_साँचो_int_t) (गाँठो->साँचो - temp->साँचो) < 0)
            ? &temp->बायाँ : &temp->दायाँ;

        यदि (*p == sentinel) {
            रोक;
        }

        temp = *p;
    }

    *p = गाँठो;
    गाँठो->parent = temp;
    गाँठो->बायाँ = sentinel;
    गाँठो->दायाँ = sentinel;
    ngx_rbt_red(गाँठो);
}


शून्य
ngx_rbtree_delete(ngx_rbtree_t *रूख, ngx_rbtree_गाँठो_t *गाँठो)
{
    ngx_uint_t           red;
    ngx_rbtree_गाँठो_t  **जरा, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    जरा = &रूख->जरा;
    sentinel = रूख->sentinel;

    यदि (गाँठो->बायाँ == sentinel) {
        temp = गाँठो->दायाँ;
        subst = गाँठो;

    } अन्यथा यदि (गाँठो->दायाँ == sentinel) {
        temp = गाँठो->बायाँ;
        subst = गाँठो;

    } अन्यथा {
        subst = ngx_rbtree_min(गाँठो->दायाँ, sentinel);
        temp = subst->दायाँ;
    }

    यदि (subst == *जरा) {
        *जरा = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        गाँठो->बायाँ = रिक्त;
        गाँठो->दायाँ = रिक्त;
        गाँठो->parent = रिक्त;
        गाँठो->साँचो = 0;

        फर्काऊ;
    }

    red = ngx_rbt_is_red(subst);

    यदि (subst == subst->parent->बायाँ) {
        subst->parent->बायाँ = temp;

    } अन्यथा {
        subst->parent->दायाँ = temp;
    }

    यदि (subst == गाँठो) {

        temp->parent = subst->parent;

    } अन्यथा {

        यदि (subst->parent == गाँठो) {
            temp->parent = subst;

        } अन्यथा {
            temp->parent = subst->parent;
        }

        subst->बायाँ = गाँठो->बायाँ;
        subst->दायाँ = गाँठो->दायाँ;
        subst->parent = गाँठो->parent;
        ngx_rbt_copy_color(subst, गाँठो);

        यदि (गाँठो == *जरा) {
            *जरा = subst;

        } अन्यथा {
            यदि (गाँठो == गाँठो->parent->बायाँ) {
                गाँठो->parent->बायाँ = subst;
            } अन्यथा {
                गाँठो->parent->दायाँ = subst;
            }
        }

        यदि (subst->बायाँ != sentinel) {
            subst->बायाँ->parent = subst;
        }

        यदि (subst->दायाँ != sentinel) {
            subst->दायाँ->parent = subst;
        }
    }

    /* DEBUG stuff */
    गाँठो->बायाँ = रिक्त;
    गाँठो->दायाँ = रिक्त;
    गाँठो->parent = रिक्त;
    गाँठो->साँचो = 0;

    यदि (red) {
        फर्काऊ;
    }

    /* a delete fixup */

    जबसम्म (temp != *जरा && ngx_rbt_is_black(temp)) {

        यदि (temp == temp->parent->बायाँ) {
            w = temp->parent->दायाँ;

            यदि (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_बायाँ_rotate(जरा, sentinel, temp->parent);
                w = temp->parent->दायाँ;
            }

            यदि (ngx_rbt_is_black(w->बायाँ) && ngx_rbt_is_black(w->दायाँ)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } अन्यथा {
                यदि (ngx_rbt_is_black(w->दायाँ)) {
                    ngx_rbt_black(w->बायाँ);
                    ngx_rbt_red(w);
                    ngx_rbtree_दायाँ_rotate(जरा, sentinel, w);
                    w = temp->parent->दायाँ;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->दायाँ);
                ngx_rbtree_बायाँ_rotate(जरा, sentinel, temp->parent);
                temp = *जरा;
            }

        } अन्यथा {
            w = temp->parent->बायाँ;

            यदि (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_दायाँ_rotate(जरा, sentinel, temp->parent);
                w = temp->parent->बायाँ;
            }

            यदि (ngx_rbt_is_black(w->बायाँ) && ngx_rbt_is_black(w->दायाँ)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } अन्यथा {
                यदि (ngx_rbt_is_black(w->बायाँ)) {
                    ngx_rbt_black(w->दायाँ);
                    ngx_rbt_red(w);
                    ngx_rbtree_बायाँ_rotate(जरा, sentinel, w);
                    w = temp->parent->बायाँ;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->बायाँ);
                ngx_rbtree_दायाँ_rotate(जरा, sentinel, temp->parent);
                temp = *जरा;
            }
        }
    }

    ngx_rbt_black(temp);
}


स्थिर ngx_inline शून्य
ngx_rbtree_बायाँ_rotate(ngx_rbtree_गाँठो_t **जरा, ngx_rbtree_गाँठो_t *sentinel,
    ngx_rbtree_गाँठो_t *गाँठो)
{
    ngx_rbtree_गाँठो_t  *temp;

    temp = गाँठो->दायाँ;
    गाँठो->दायाँ = temp->बायाँ;

    यदि (temp->बायाँ != sentinel) {
        temp->बायाँ->parent = गाँठो;
    }

    temp->parent = गाँठो->parent;

    यदि (गाँठो == *जरा) {
        *जरा = temp;

    } अन्यथा यदि (गाँठो == गाँठो->parent->बायाँ) {
        गाँठो->parent->बायाँ = temp;

    } अन्यथा {
        गाँठो->parent->दायाँ = temp;
    }

    temp->बायाँ = गाँठो;
    गाँठो->parent = temp;
}


स्थिर ngx_inline शून्य
ngx_rbtree_दायाँ_rotate(ngx_rbtree_गाँठो_t **जरा, ngx_rbtree_गाँठो_t *sentinel,
    ngx_rbtree_गाँठो_t *गाँठो)
{
    ngx_rbtree_गाँठो_t  *temp;

    temp = गाँठो->बायाँ;
    गाँठो->बायाँ = temp->दायाँ;

    यदि (temp->दायाँ != sentinel) {
        temp->दायाँ->parent = गाँठो;
    }

    temp->parent = गाँठो->parent;

    यदि (गाँठो == *जरा) {
        *जरा = temp;

    } अन्यथा यदि (गाँठो == गाँठो->parent->दायाँ) {
        गाँठो->parent->दायाँ = temp;

    } अन्यथा {
        गाँठो->parent->बायाँ = temp;
    }

    temp->दायाँ = गाँठो;
    गाँठो->parent = temp;
}


ngx_rbtree_गाँठो_t *
ngx_rbtree_next(ngx_rbtree_t *रूख, ngx_rbtree_गाँठो_t *गाँठो)
{
    ngx_rbtree_गाँठो_t  *जरा, *sentinel, *parent;

    sentinel = रूख->sentinel;

    यदि (गाँठो->दायाँ != sentinel) {
        फर्काऊ ngx_rbtree_min(गाँठो->दायाँ, sentinel);
    }

    जरा = रूख->जरा;

    लागि ( ;; ) {
        parent = गाँठो->parent;

        यदि (गाँठो == जरा) {
            फर्काऊ रिक्त;
        }

        यदि (गाँठो == parent->बायाँ) {
            फर्काऊ parent;
        }

        गाँठो = parent;
    }
}
