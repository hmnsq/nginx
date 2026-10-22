
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
    ngx_rbtree_গ্রন্থি_t  **মূল, *অস্থায়ী, *sentinel;

    /* a binary tree insert */

    মূল = &বৃক্ষ->মূল;
    sentinel = বৃক্ষ->sentinel;

    যদি (*মূল == sentinel) {
        গ্রন্থি->parent = ফাঁকা;
        গ্রন্থি->বাম = sentinel;
        গ্রন্থি->ডান = sentinel;
        ngx_rbt_black(গ্রন্থি);
        *মূল = গ্রন্থি;

        ফেরত;
    }

    বৃক্ষ->ঢোকাও(*মূল, গ্রন্থি, sentinel);

    /* re-balance tree */

    যখন (গ্রন্থি != *মূল && ngx_rbt_is_red(গ্রন্থি->parent)) {

        যদি (গ্রন্থি->parent == গ্রন্থি->parent->parent->বাম) {
            অস্থায়ী = গ্রন্থি->parent->parent->ডান;

            যদি (ngx_rbt_is_red(অস্থায়ী)) {
                ngx_rbt_black(গ্রন্থি->parent);
                ngx_rbt_black(অস্থায়ী);
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
            অস্থায়ী = গ্রন্থি->parent->parent->বাম;

            যদি (ngx_rbt_is_red(অস্থায়ী)) {
                ngx_rbt_black(গ্রন্থি->parent);
                ngx_rbt_black(অস্থায়ী);
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
ngx_rbtree_ঢোকাও_মান(ngx_rbtree_গ্রন্থি_t *অস্থায়ী, ngx_rbtree_গ্রন্থি_t *গ্রন্থি,
    ngx_rbtree_গ্রন্থি_t *sentinel)
{
    ngx_rbtree_গ্রন্থি_t  **p;

    জন্য ( ;; ) {

        p = (গ্রন্থি->চাবি < অস্থায়ী->চাবি) ? &অস্থায়ী->বাম : &অস্থায়ী->ডান;

        যদি (*p == sentinel) {
            বিরতি;
        }

        অস্থায়ী = *p;
    }

    *p = গ্রন্থি;
    গ্রন্থি->parent = অস্থায়ী;
    গ্রন্থি->বাম = sentinel;
    গ্রন্থি->ডান = sentinel;
    ngx_rbt_red(গ্রন্থি);
}


শূন্য
ngx_rbtree_ঢোকাও_timer_মান(ngx_rbtree_গ্রন্থি_t *অস্থায়ী, ngx_rbtree_গ্রন্থি_t *গ্রন্থি,
    ngx_rbtree_গ্রন্থি_t *sentinel)
{
    ngx_rbtree_গ্রন্থি_t  **p;

    জন্য ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_চাবি_int_t) (গ্রন্থি->চাবি - অস্থায়ী->চাবি) < 0)
            ? &অস্থায়ী->বাম : &অস্থায়ী->ডান;

        যদি (*p == sentinel) {
            বিরতি;
        }

        অস্থায়ী = *p;
    }

    *p = গ্রন্থি;
    গ্রন্থি->parent = অস্থায়ী;
    গ্রন্থি->বাম = sentinel;
    গ্রন্থি->ডান = sentinel;
    ngx_rbt_red(গ্রন্থি);
}


শূন্য
ngx_rbtree_delete(ngx_rbtree_t *বৃক্ষ, ngx_rbtree_গ্রন্থি_t *গ্রন্থি)
{
    ngx_uint_t           red;
    ngx_rbtree_গ্রন্থি_t  **মূল, *sentinel, *subst, *অস্থায়ী, *w;

    /* a binary tree delete */

    মূল = &বৃক্ষ->মূল;
    sentinel = বৃক্ষ->sentinel;

    যদি (গ্রন্থি->বাম == sentinel) {
        অস্থায়ী = গ্রন্থি->ডান;
        subst = গ্রন্থি;

    } অন্যথা যদি (গ্রন্থি->ডান == sentinel) {
        অস্থায়ী = গ্রন্থি->বাম;
        subst = গ্রন্থি;

    } অন্যথা {
        subst = ngx_rbtree_min(গ্রন্থি->ডান, sentinel);
        অস্থায়ী = subst->ডান;
    }

    যদি (subst == *মূল) {
        *মূল = অস্থায়ী;
        ngx_rbt_black(অস্থায়ী);

        /* DEBUG stuff */
        গ্রন্থি->বাম = ফাঁকা;
        গ্রন্থি->ডান = ফাঁকা;
        গ্রন্থি->parent = ফাঁকা;
        গ্রন্থি->চাবি = 0;

        ফেরত;
    }

    red = ngx_rbt_is_red(subst);

    যদি (subst == subst->parent->বাম) {
        subst->parent->বাম = অস্থায়ী;

    } অন্যথা {
        subst->parent->ডান = অস্থায়ী;
    }

    যদি (subst == গ্রন্থি) {

        অস্থায়ী->parent = subst->parent;

    } অন্যথা {

        যদি (subst->parent == গ্রন্থি) {
            অস্থায়ী->parent = subst;

        } অন্যথা {
            অস্থায়ী->parent = subst->parent;
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
    গ্রন্থি->বাম = ফাঁকা;
    গ্রন্থি->ডান = ফাঁকা;
    গ্রন্থি->parent = ফাঁকা;
    গ্রন্থি->চাবি = 0;

    যদি (red) {
        ফেরত;
    }

    /* a delete fixup */

    যখন (অস্থায়ী != *মূল && ngx_rbt_is_black(অস্থায়ী)) {

        যদি (অস্থায়ী == অস্থায়ী->parent->বাম) {
            w = অস্থায়ী->parent->ডান;

            যদি (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(অস্থায়ী->parent);
                ngx_rbtree_বাম_rotate(মূল, sentinel, অস্থায়ী->parent);
                w = অস্থায়ী->parent->ডান;
            }

            যদি (ngx_rbt_is_black(w->বাম) && ngx_rbt_is_black(w->ডান)) {
                ngx_rbt_red(w);
                অস্থায়ী = অস্থায়ী->parent;

            } অন্যথা {
                যদি (ngx_rbt_is_black(w->ডান)) {
                    ngx_rbt_black(w->বাম);
                    ngx_rbt_red(w);
                    ngx_rbtree_ডান_rotate(মূল, sentinel, w);
                    w = অস্থায়ী->parent->ডান;
                }

                ngx_rbt_copy_color(w, অস্থায়ী->parent);
                ngx_rbt_black(অস্থায়ী->parent);
                ngx_rbt_black(w->ডান);
                ngx_rbtree_বাম_rotate(মূল, sentinel, অস্থায়ী->parent);
                অস্থায়ী = *মূল;
            }

        } অন্যথা {
            w = অস্থায়ী->parent->বাম;

            যদি (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(অস্থায়ী->parent);
                ngx_rbtree_ডান_rotate(মূল, sentinel, অস্থায়ী->parent);
                w = অস্থায়ী->parent->বাম;
            }

            যদি (ngx_rbt_is_black(w->বাম) && ngx_rbt_is_black(w->ডান)) {
                ngx_rbt_red(w);
                অস্থায়ী = অস্থায়ী->parent;

            } অন্যথা {
                যদি (ngx_rbt_is_black(w->বাম)) {
                    ngx_rbt_black(w->ডান);
                    ngx_rbt_red(w);
                    ngx_rbtree_বাম_rotate(মূল, sentinel, w);
                    w = অস্থায়ী->parent->বাম;
                }

                ngx_rbt_copy_color(w, অস্থায়ী->parent);
                ngx_rbt_black(অস্থায়ী->parent);
                ngx_rbt_black(w->বাম);
                ngx_rbtree_ডান_rotate(মূল, sentinel, অস্থায়ী->parent);
                অস্থায়ী = *মূল;
            }
        }
    }

    ngx_rbt_black(অস্থায়ী);
}


স্থির ngx_inline শূন্য
ngx_rbtree_বাম_rotate(ngx_rbtree_গ্রন্থি_t **মূল, ngx_rbtree_গ্রন্থি_t *sentinel,
    ngx_rbtree_গ্রন্থি_t *গ্রন্থি)
{
    ngx_rbtree_গ্রন্থি_t  *অস্থায়ী;

    অস্থায়ী = গ্রন্থি->ডান;
    গ্রন্থি->ডান = অস্থায়ী->বাম;

    যদি (অস্থায়ী->বাম != sentinel) {
        অস্থায়ী->বাম->parent = গ্রন্থি;
    }

    অস্থায়ী->parent = গ্রন্থি->parent;

    যদি (গ্রন্থি == *মূল) {
        *মূল = অস্থায়ী;

    } অন্যথা যদি (গ্রন্থি == গ্রন্থি->parent->বাম) {
        গ্রন্থি->parent->বাম = অস্থায়ী;

    } অন্যথা {
        গ্রন্থি->parent->ডান = অস্থায়ী;
    }

    অস্থায়ী->বাম = গ্রন্থি;
    গ্রন্থি->parent = অস্থায়ী;
}


স্থির ngx_inline শূন্য
ngx_rbtree_ডান_rotate(ngx_rbtree_গ্রন্থি_t **মূল, ngx_rbtree_গ্রন্থি_t *sentinel,
    ngx_rbtree_গ্রন্থি_t *গ্রন্থি)
{
    ngx_rbtree_গ্রন্থি_t  *অস্থায়ী;

    অস্থায়ী = গ্রন্থি->বাম;
    গ্রন্থি->বাম = অস্থায়ী->ডান;

    যদি (অস্থায়ী->ডান != sentinel) {
        অস্থায়ী->ডান->parent = গ্রন্থি;
    }

    অস্থায়ী->parent = গ্রন্থি->parent;

    যদি (গ্রন্থি == *মূল) {
        *মূল = অস্থায়ী;

    } অন্যথা যদি (গ্রন্থি == গ্রন্থি->parent->ডান) {
        গ্রন্থি->parent->ডান = অস্থায়ী;

    } অন্যথা {
        গ্রন্থি->parent->বাম = অস্থায়ী;
    }

    অস্থায়ী->ডান = গ্রন্থি;
    গ্রন্থি->parent = অস্থায়ী;
}


ngx_rbtree_গ্রন্থি_t *
ngx_rbtree_পরবর্তী(ngx_rbtree_t *বৃক্ষ, ngx_rbtree_গ্রন্থি_t *গ্রন্থি)
{
    ngx_rbtree_গ্রন্থি_t  *মূল, *sentinel, *parent;

    sentinel = বৃক্ষ->sentinel;

    যদি (গ্রন্থি->ডান != sentinel) {
        ফেরত ngx_rbtree_min(গ্রন্থি->ডান, sentinel);
    }

    মূল = বৃক্ষ->মূল;

    জন্য ( ;; ) {
        parent = গ্রন্থি->parent;

        যদি (গ্রন্থি == মূল) {
            ফেরত ফাঁকা;
        }

        যদি (গ্রন্থি == parent->বাম) {
            ফেরত parent;
        }

        গ্রন্থি = parent;
    }
}
