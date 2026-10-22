
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


স্থির ngx_inline শূন্য ngx_rbtree_বাম_rotate(ngx_rbtree_গ্রন্থি_t **মূল,
    ngx_rbtree_গ্রন্থি_t *sentinel, ngx_rbtree_গ্রন্থি_t *গ্রন্থি);
স্থির ngx_inline শূন্য ngx_rbtree_ডান_rotate(ngx_rbtree_গ্রন্থি_t **মূল,
    ngx_rbtree_গ্রন্থি_t *sentinel, ngx_rbtree_গ্রন্থি_t *গ্রন্থি);


শূন্য
ngx_rbtree_ঢোকাও(ngx_rbtree_t *বৃক্ষ, ngx_rbtree_গ্রন্থি_t *গ্রন্থি)
{
    ngx_rbtree_গ্রন্থি_t  **মূল, *temp, *sentinel;

    /* a binary tree insert */

    মূল = &বৃক্ষ->মূল;
    sentinel = বৃক্ষ->sentinel;

    যদি (*মূল == sentinel) {
        গ্রন্থি->parent = রিক্ত;
        গ্রন্থি->বাম = sentinel;
        গ্রন্থি->ডান = sentinel;
        ngx_rbt_black(গ্রন্থি);
        *মূল = গ্রন্থি;

        প্রত্যাগম;
    }

    বৃক্ষ->ঢোকাও(*মূল, গ্রন্থি, sentinel);

    /* re-balance tree */

    যাবত্ (গ্রন্থি != *মূল && ngx_rbt_is_red(গ্রন্থি->parent)) {

        যদি (গ্রন্থি->parent == গ্রন্থি->parent->parent->বাম) {
            temp = গ্রন্থি->parent->parent->ডান;

            যদি (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(গ্রন্থি->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(গ্রন্থি->parent->parent);
                গ্রন্থি = গ্রন্থি->parent->parent;

            } অন্যথা {
                যদি (গ্রন্থি == গ্রন্থি->parent->ডান) {
                    গ্রন্থি = গ্রন্থি->parent;
                    ngx_rbtree_বাম_rotate(মূল, sentinel, গ্রন্থি);
                }

                ngx_rbt_black(গ্রন্থি->parent);
                ngx_rbt_red(গ্রন্থি->parent->parent);
                ngx_rbtree_ডান_rotate(মূল, sentinel, গ্রন্থি->parent->parent);
            }

        } অন্যথা {
            temp = গ্রন্থি->parent->parent->বাম;

            যদি (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(গ্রন্থি->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(গ্রন্থি->parent->parent);
                গ্রন্থি = গ্রন্থি->parent->parent;

            } অন্যথা {
                যদি (গ্রন্থি == গ্রন্থি->parent->বাম) {
                    গ্রন্থি = গ্রন্থি->parent;
                    ngx_rbtree_ডান_rotate(মূল, sentinel, গ্রন্থি);
                }

                ngx_rbt_black(গ্রন্থি->parent);
                ngx_rbt_red(গ্রন্থি->parent->parent);
                ngx_rbtree_বাম_rotate(মূল, sentinel, গ্রন্থি->parent->parent);
            }
        }
    }

    ngx_rbt_black(*মূল);
}


শূন্য
ngx_rbtree_ঢোকাও_মান(ngx_rbtree_গ্রন্থি_t *temp, ngx_rbtree_গ্রন্থি_t *গ্রন্থি,
    ngx_rbtree_গ্রন্থি_t *sentinel)
{
    ngx_rbtree_গ্রন্থি_t  **p;

    প্রতি ( ;; ) {

        p = (গ্রন্থি->চাবি < temp->চাবি) ? &temp->বাম : &temp->ডান;

        যদি (*p == sentinel) {
            ভঙ্গ;
        }

        temp = *p;
    }

    *p = গ্রন্থি;
    গ্রন্থি->parent = temp;
    গ্রন্থি->বাম = sentinel;
    গ্রন্থি->ডান = sentinel;
    ngx_rbt_red(গ্রন্থি);
}


শূন্য
ngx_rbtree_ঢোকাও_timer_মান(ngx_rbtree_গ্রন্থি_t *temp, ngx_rbtree_গ্রন্থি_t *গ্রন্থি,
    ngx_rbtree_গ্রন্থি_t *sentinel)
{
    ngx_rbtree_গ্রন্থি_t  **p;

    প্রতি ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_চাবি_int_t) (গ্রন্থি->চাবি - temp->চাবি) < 0)
            ? &temp->বাম : &temp->ডান;

        যদি (*p == sentinel) {
            ভঙ্গ;
        }

        temp = *p;
    }

    *p = গ্রন্থি;
    গ্রন্থি->parent = temp;
    গ্রন্থি->বাম = sentinel;
    গ্রন্থি->ডান = sentinel;
    ngx_rbt_red(গ্রন্থি);
}


শূন্য
ngx_rbtree_delete(ngx_rbtree_t *বৃক্ষ, ngx_rbtree_গ্রন্থি_t *গ্রন্থি)
{
    ngx_uint_t           red;
    ngx_rbtree_গ্রন্থি_t  **মূল, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    মূল = &বৃক্ষ->মূল;
    sentinel = বৃক্ষ->sentinel;

    যদি (গ্রন্থি->বাম == sentinel) {
        temp = গ্রন্থি->ডান;
        subst = গ্রন্থি;

    } অন্যথা যদি (গ্রন্থি->ডান == sentinel) {
        temp = গ্রন্থি->বাম;
        subst = গ্রন্থি;

    } অন্যথা {
        subst = ngx_rbtree_min(গ্রন্থি->ডান, sentinel);
        temp = subst->ডান;
    }

    যদি (subst == *মূল) {
        *মূল = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        গ্রন্থি->বাম = রিক্ত;
        গ্রন্থি->ডান = রিক্ত;
        গ্রন্থি->parent = রিক্ত;
        গ্রন্থি->চাবি = 0;

        প্রত্যাগম;
    }

    red = ngx_rbt_is_red(subst);

    যদি (subst == subst->parent->বাম) {
        subst->parent->বাম = temp;

    } অন্যথা {
        subst->parent->ডান = temp;
    }

    যদি (subst == গ্রন্থি) {

        temp->parent = subst->parent;

    } অন্যথা {

        যদি (subst->parent == গ্রন্থি) {
            temp->parent = subst;

        } অন্যথা {
            temp->parent = subst->parent;
        }

        subst->বাম = গ্রন্থি->বাম;
        subst->ডান = গ্রন্থি->ডান;
        subst->parent = গ্রন্থি->parent;
        ngx_rbt_copy_color(subst, গ্রন্থি);

        যদি (গ্রন্থি == *মূল) {
            *মূল = subst;

        } অন্যথা {
            যদি (গ্রন্থি == গ্রন্থি->parent->বাম) {
                গ্রন্থি->parent->বাম = subst;
            } অন্যথা {
                গ্রন্থি->parent->ডান = subst;
            }
        }

        যদি (subst->বাম != sentinel) {
            subst->বাম->parent = subst;
        }

        যদি (subst->ডান != sentinel) {
            subst->ডান->parent = subst;
        }
    }

    /* DEBUG stuff */
    গ্রন্থি->বাম = রিক্ত;
    গ্রন্থি->ডান = রিক্ত;
    গ্রন্থি->parent = রিক্ত;
    গ্রন্থি->চাবি = 0;

    যদি (red) {
        প্রত্যাগম;
    }

    /* a delete fixup */

    যাবত্ (temp != *মূল && ngx_rbt_is_black(temp)) {

        যদি (temp == temp->parent->বাম) {
            w = temp->parent->ডান;

            যদি (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_বাম_rotate(মূল, sentinel, temp->parent);
                w = temp->parent->ডান;
            }

            যদি (ngx_rbt_is_black(w->বাম) && ngx_rbt_is_black(w->ডান)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } অন্যথা {
                যদি (ngx_rbt_is_black(w->ডান)) {
                    ngx_rbt_black(w->বাম);
                    ngx_rbt_red(w);
                    ngx_rbtree_ডান_rotate(মূল, sentinel, w);
                    w = temp->parent->ডান;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->ডান);
                ngx_rbtree_বাম_rotate(মূল, sentinel, temp->parent);
                temp = *মূল;
            }

        } অন্যথা {
            w = temp->parent->বাম;

            যদি (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_ডান_rotate(মূল, sentinel, temp->parent);
                w = temp->parent->বাম;
            }

            যদি (ngx_rbt_is_black(w->বাম) && ngx_rbt_is_black(w->ডান)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } অন্যথা {
                যদি (ngx_rbt_is_black(w->বাম)) {
                    ngx_rbt_black(w->ডান);
                    ngx_rbt_red(w);
                    ngx_rbtree_বাম_rotate(মূল, sentinel, w);
                    w = temp->parent->বাম;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->বাম);
                ngx_rbtree_ডান_rotate(মূল, sentinel, temp->parent);
                temp = *মূল;
            }
        }
    }

    ngx_rbt_black(temp);
}


স্থির ngx_inline শূন্য
ngx_rbtree_বাম_rotate(ngx_rbtree_গ্রন্থি_t **মূল, ngx_rbtree_গ্রন্থি_t *sentinel,
    ngx_rbtree_গ্রন্থি_t *গ্রন্থি)
{
    ngx_rbtree_গ্রন্থি_t  *temp;

    temp = গ্রন্থি->ডান;
    গ্রন্থি->ডান = temp->বাম;

    যদি (temp->বাম != sentinel) {
        temp->বাম->parent = গ্রন্থি;
    }

    temp->parent = গ্রন্থি->parent;

    যদি (গ্রন্থি == *মূল) {
        *মূল = temp;

    } অন্যথা যদি (গ্রন্থি == গ্রন্থি->parent->বাম) {
        গ্রন্থি->parent->বাম = temp;

    } অন্যথা {
        গ্রন্থি->parent->ডান = temp;
    }

    temp->বাম = গ্রন্থি;
    গ্রন্থি->parent = temp;
}


স্থির ngx_inline শূন্য
ngx_rbtree_ডান_rotate(ngx_rbtree_গ্রন্থি_t **মূল, ngx_rbtree_গ্রন্থি_t *sentinel,
    ngx_rbtree_গ্রন্থি_t *গ্রন্থি)
{
    ngx_rbtree_গ্রন্থি_t  *temp;

    temp = গ্রন্থি->বাম;
    গ্রন্থি->বাম = temp->ডান;

    যদি (temp->ডান != sentinel) {
        temp->ডান->parent = গ্রন্থি;
    }

    temp->parent = গ্রন্থি->parent;

    যদি (গ্রন্থি == *মূল) {
        *মূল = temp;

    } অন্যথা যদি (গ্রন্থি == গ্রন্থি->parent->ডান) {
        গ্রন্থি->parent->ডান = temp;

    } অন্যথা {
        গ্রন্থি->parent->বাম = temp;
    }

    temp->ডান = গ্রন্থি;
    গ্রন্থি->parent = temp;
}


ngx_rbtree_গ্রন্থি_t *
ngx_rbtree_next(ngx_rbtree_t *বৃক্ষ, ngx_rbtree_গ্রন্থি_t *গ্রন্থি)
{
    ngx_rbtree_গ্রন্থি_t  *মূল, *sentinel, *parent;

    sentinel = বৃক্ষ->sentinel;

    যদি (গ্রন্থি->ডান != sentinel) {
        প্রত্যাগম ngx_rbtree_min(গ্রন্থি->ডান, sentinel);
    }

    মূল = বৃক্ষ->মূল;

    প্রতি ( ;; ) {
        parent = গ্রন্থি->parent;

        যদি (গ্রন্থি == মূল) {
            প্রত্যাগম রিক্ত;
        }

        যদি (গ্রন্থি == parent->বাম) {
            প্রত্যাগম parent;
        }

        গ্রন্থি = parent;
    }
}
