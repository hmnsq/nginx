
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


స్థిర ngx_inline శూన్య ngx_rbtree_ఎడమ_rotate(ngx_rbtree_కణుపు_t **మూలం,
    ngx_rbtree_కణుపు_t *sentinel, ngx_rbtree_కణుపు_t *కణుపు);
స్థిర ngx_inline శూన్య ngx_rbtree_కుడి_rotate(ngx_rbtree_కణుపు_t **మూలం,
    ngx_rbtree_కణుపు_t *sentinel, ngx_rbtree_కణుపు_t *కణుపు);


శూన్య
ngx_rbtree_చొప్పించు(ngx_rbtree_t *వృక్షం, ngx_rbtree_కణుపు_t *కణుపు)
{
    ngx_rbtree_కణుపు_t  **మూలం, *తాత్కాలిక, *sentinel;

    /* a binary tree insert */

    మూలం = &వృక్షం->మూలం;
    sentinel = వృక్షం->sentinel;

    ఒకవేళ (*మూలం == sentinel) {
        కణుపు->మాతృ = ఖాళీ;
        కణుపు->ఎడమ = sentinel;
        కణుపు->కుడి = sentinel;
        ngx_rbt_black(కణుపు);
        *మూలం = కణుపు;

        తిరిగి;
    }

    వృక్షం->చొప్పించు(*మూలం, కణుపు, sentinel);

    /* re-balance tree */

    అయితే (కణుపు != *మూలం && ngx_rbt_is_red(కణుపు->మాతృ)) {

        ఒకవేళ (కణుపు->మాతృ == కణుపు->మాతృ->మాతృ->ఎడమ) {
            తాత్కాలిక = కణుపు->మాతృ->మాతృ->కుడి;

            ఒకవేళ (ngx_rbt_is_red(తాత్కాలిక)) {
                ngx_rbt_black(కణుపు->మాతృ);
                ngx_rbt_black(తాత్కాలిక);
                ngx_rbt_red(కణుపు->మాతృ->మాతృ);
                కణుపు = కణుపు->మాతృ->మాతృ;

            } లేకపోతే {
                ఒకవేళ (కణుపు == కణుపు->మాతృ->కుడి) {
                    కణుపు = కణుపు->మాతృ;
                    ngx_rbtree_ఎడమ_rotate(మూలం, sentinel, కణుపు);
                }

                ngx_rbt_black(కణుపు->మాతృ);
                ngx_rbt_red(కణుపు->మాతృ->మాతృ);
                ngx_rbtree_కుడి_rotate(మూలం, sentinel, కణుపు->మాతృ->మాతృ);
            }

        } లేకపోతే {
            తాత్కాలిక = కణుపు->మాతృ->మాతృ->ఎడమ;

            ఒకవేళ (ngx_rbt_is_red(తాత్కాలిక)) {
                ngx_rbt_black(కణుపు->మాతృ);
                ngx_rbt_black(తాత్కాలిక);
                ngx_rbt_red(కణుపు->మాతృ->మాతృ);
                కణుపు = కణుపు->మాతృ->మాతృ;

            } లేకపోతే {
                ఒకవేళ (కణుపు == కణుపు->మాతృ->ఎడమ) {
                    కణుపు = కణుపు->మాతృ;
                    ngx_rbtree_కుడి_rotate(మూలం, sentinel, కణుపు);
                }

                ngx_rbt_black(కణుపు->మాతృ);
                ngx_rbt_red(కణుపు->మాతృ->మాతృ);
                ngx_rbtree_ఎడమ_rotate(మూలం, sentinel, కణుపు->మాతృ->మాతృ);
            }
        }
    }

    ngx_rbt_black(*మూలం);
}


శూన్య
ngx_rbtree_చొప్పించు_విలువ(ngx_rbtree_కణుపు_t *తాత్కాలిక, ngx_rbtree_కణుపు_t *కణుపు,
    ngx_rbtree_కణుపు_t *sentinel)
{
    ngx_rbtree_కణుపు_t  **p;

    కొరకు ( ;; ) {

        p = (కణుపు->కీలకం < తాత్కాలిక->కీలకం) ? &తాత్కాలిక->ఎడమ : &తాత్కాలిక->కుడి;

        ఒకవేళ (*p == sentinel) {
            ఆపు;
        }

        తాత్కాలిక = *p;
    }

    *p = కణుపు;
    కణుపు->మాతృ = తాత్కాలిక;
    కణుపు->ఎడమ = sentinel;
    కణుపు->కుడి = sentinel;
    ngx_rbt_red(కణుపు);
}


శూన్య
ngx_rbtree_చొప్పించు_timer_విలువ(ngx_rbtree_కణుపు_t *తాత్కాలిక, ngx_rbtree_కణుపు_t *కణుపు,
    ngx_rbtree_కణుపు_t *sentinel)
{
    ngx_rbtree_కణుపు_t  **p;

    కొరకు ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_కీలకం_int_t) (కణుపు->కీలకం - తాత్కాలిక->కీలకం) < 0)
            ? &తాత్కాలిక->ఎడమ : &తాత్కాలిక->కుడి;

        ఒకవేళ (*p == sentinel) {
            ఆపు;
        }

        తాత్కాలిక = *p;
    }

    *p = కణుపు;
    కణుపు->మాతృ = తాత్కాలిక;
    కణుపు->ఎడమ = sentinel;
    కణుపు->కుడి = sentinel;
    ngx_rbt_red(కణుపు);
}


శూన్య
ngx_rbtree_delete(ngx_rbtree_t *వృక్షం, ngx_rbtree_కణుపు_t *కణుపు)
{
    ngx_uint_t           red;
    ngx_rbtree_కణుపు_t  **మూలం, *sentinel, *subst, *తాత్కాలిక, *w;

    /* a binary tree delete */

    మూలం = &వృక్షం->మూలం;
    sentinel = వృక్షం->sentinel;

    ఒకవేళ (కణుపు->ఎడమ == sentinel) {
        తాత్కాలిక = కణుపు->కుడి;
        subst = కణుపు;

    } లేకపోతే ఒకవేళ (కణుపు->కుడి == sentinel) {
        తాత్కాలిక = కణుపు->ఎడమ;
        subst = కణుపు;

    } లేకపోతే {
        subst = ngx_rbtree_min(కణుపు->కుడి, sentinel);
        తాత్కాలిక = subst->కుడి;
    }

    ఒకవేళ (subst == *మూలం) {
        *మూలం = తాత్కాలిక;
        ngx_rbt_black(తాత్కాలిక);

        /* DEBUG stuff */
        కణుపు->ఎడమ = ఖాళీ;
        కణుపు->కుడి = ఖాళీ;
        కణుపు->మాతృ = ఖాళీ;
        కణుపు->కీలకం = 0;

        తిరిగి;
    }

    red = ngx_rbt_is_red(subst);

    ఒకవేళ (subst == subst->మాతృ->ఎడమ) {
        subst->మాతృ->ఎడమ = తాత్కాలిక;

    } లేకపోతే {
        subst->మాతృ->కుడి = తాత్కాలిక;
    }

    ఒకవేళ (subst == కణుపు) {

        తాత్కాలిక->మాతృ = subst->మాతృ;

    } లేకపోతే {

        ఒకవేళ (subst->మాతృ == కణుపు) {
            తాత్కాలిక->మాతృ = subst;

        } లేకపోతే {
            తాత్కాలిక->మాతృ = subst->మాతృ;
        }

        subst->ఎడమ = కణుపు->ఎడమ;
        subst->కుడి = కణుపు->కుడి;
        subst->మాతృ = కణుపు->మాతృ;
        ngx_rbt_copy_color(subst, కణుపు);

        ఒకవేళ (కణుపు == *మూలం) {
            *మూలం = subst;

        } లేకపోతే {
            ఒకవేళ (కణుపు == కణుపు->మాతృ->ఎడమ) {
                కణుపు->మాతృ->ఎడమ = subst;
            } లేకపోతే {
                కణుపు->మాతృ->కుడి = subst;
            }
        }

        ఒకవేళ (subst->ఎడమ != sentinel) {
            subst->ఎడమ->మాతృ = subst;
        }

        ఒకవేళ (subst->కుడి != sentinel) {
            subst->కుడి->మాతృ = subst;
        }
    }

    /* DEBUG stuff */
    కణుపు->ఎడమ = ఖాళీ;
    కణుపు->కుడి = ఖాళీ;
    కణుపు->మాతృ = ఖాళీ;
    కణుపు->కీలకం = 0;

    ఒకవేళ (red) {
        తిరిగి;
    }

    /* a delete fixup */

    అయితే (తాత్కాలిక != *మూలం && ngx_rbt_is_black(తాత్కాలిక)) {

        ఒకవేళ (తాత్కాలిక == తాత్కాలిక->మాతృ->ఎడమ) {
            w = తాత్కాలిక->మాతృ->కుడి;

            ఒకవేళ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(తాత్కాలిక->మాతృ);
                ngx_rbtree_ఎడమ_rotate(మూలం, sentinel, తాత్కాలిక->మాతృ);
                w = తాత్కాలిక->మాతృ->కుడి;
            }

            ఒకవేళ (ngx_rbt_is_black(w->ఎడమ) && ngx_rbt_is_black(w->కుడి)) {
                ngx_rbt_red(w);
                తాత్కాలిక = తాత్కాలిక->మాతృ;

            } లేకపోతే {
                ఒకవేళ (ngx_rbt_is_black(w->కుడి)) {
                    ngx_rbt_black(w->ఎడమ);
                    ngx_rbt_red(w);
                    ngx_rbtree_కుడి_rotate(మూలం, sentinel, w);
                    w = తాత్కాలిక->మాతృ->కుడి;
                }

                ngx_rbt_copy_color(w, తాత్కాలిక->మాతృ);
                ngx_rbt_black(తాత్కాలిక->మాతృ);
                ngx_rbt_black(w->కుడి);
                ngx_rbtree_ఎడమ_rotate(మూలం, sentinel, తాత్కాలిక->మాతృ);
                తాత్కాలిక = *మూలం;
            }

        } లేకపోతే {
            w = తాత్కాలిక->మాతృ->ఎడమ;

            ఒకవేళ (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(తాత్కాలిక->మాతృ);
                ngx_rbtree_కుడి_rotate(మూలం, sentinel, తాత్కాలిక->మాతృ);
                w = తాత్కాలిక->మాతృ->ఎడమ;
            }

            ఒకవేళ (ngx_rbt_is_black(w->ఎడమ) && ngx_rbt_is_black(w->కుడి)) {
                ngx_rbt_red(w);
                తాత్కాలిక = తాత్కాలిక->మాతృ;

            } లేకపోతే {
                ఒకవేళ (ngx_rbt_is_black(w->ఎడమ)) {
                    ngx_rbt_black(w->కుడి);
                    ngx_rbt_red(w);
                    ngx_rbtree_ఎడమ_rotate(మూలం, sentinel, w);
                    w = తాత్కాలిక->మాతృ->ఎడమ;
                }

                ngx_rbt_copy_color(w, తాత్కాలిక->మాతృ);
                ngx_rbt_black(తాత్కాలిక->మాతృ);
                ngx_rbt_black(w->ఎడమ);
                ngx_rbtree_కుడి_rotate(మూలం, sentinel, తాత్కాలిక->మాతృ);
                తాత్కాలిక = *మూలం;
            }
        }
    }

    ngx_rbt_black(తాత్కాలిక);
}


స్థిర ngx_inline శూన్య
ngx_rbtree_ఎడమ_rotate(ngx_rbtree_కణుపు_t **మూలం, ngx_rbtree_కణుపు_t *sentinel,
    ngx_rbtree_కణుపు_t *కణుపు)
{
    ngx_rbtree_కణుపు_t  *తాత్కాలిక;

    తాత్కాలిక = కణుపు->కుడి;
    కణుపు->కుడి = తాత్కాలిక->ఎడమ;

    ఒకవేళ (తాత్కాలిక->ఎడమ != sentinel) {
        తాత్కాలిక->ఎడమ->మాతృ = కణుపు;
    }

    తాత్కాలిక->మాతృ = కణుపు->మాతృ;

    ఒకవేళ (కణుపు == *మూలం) {
        *మూలం = తాత్కాలిక;

    } లేకపోతే ఒకవేళ (కణుపు == కణుపు->మాతృ->ఎడమ) {
        కణుపు->మాతృ->ఎడమ = తాత్కాలిక;

    } లేకపోతే {
        కణుపు->మాతృ->కుడి = తాత్కాలిక;
    }

    తాత్కాలిక->ఎడమ = కణుపు;
    కణుపు->మాతృ = తాత్కాలిక;
}


స్థిర ngx_inline శూన్య
ngx_rbtree_కుడి_rotate(ngx_rbtree_కణుపు_t **మూలం, ngx_rbtree_కణుపు_t *sentinel,
    ngx_rbtree_కణుపు_t *కణుపు)
{
    ngx_rbtree_కణుపు_t  *తాత్కాలిక;

    తాత్కాలిక = కణుపు->ఎడమ;
    కణుపు->ఎడమ = తాత్కాలిక->కుడి;

    ఒకవేళ (తాత్కాలిక->కుడి != sentinel) {
        తాత్కాలిక->కుడి->మాతృ = కణుపు;
    }

    తాత్కాలిక->మాతృ = కణుపు->మాతృ;

    ఒకవేళ (కణుపు == *మూలం) {
        *మూలం = తాత్కాలిక;

    } లేకపోతే ఒకవేళ (కణుపు == కణుపు->మాతృ->కుడి) {
        కణుపు->మాతృ->కుడి = తాత్కాలిక;

    } లేకపోతే {
        కణుపు->మాతృ->ఎడమ = తాత్కాలిక;
    }

    తాత్కాలిక->కుడి = కణుపు;
    కణుపు->మాతృ = తాత్కాలిక;
}


ngx_rbtree_కణుపు_t *
ngx_rbtree_తదుపరి(ngx_rbtree_t *వృక్షం, ngx_rbtree_కణుపు_t *కణుపు)
{
    ngx_rbtree_కణుపు_t  *మూలం, *sentinel, *మాతృ;

    sentinel = వృక్షం->sentinel;

    ఒకవేళ (కణుపు->కుడి != sentinel) {
        తిరిగి ngx_rbtree_min(కణుపు->కుడి, sentinel);
    }

    మూలం = వృక్షం->మూలం;

    కొరకు ( ;; ) {
        మాతృ = కణుపు->మాతృ;

        ఒకవేళ (కణుపు == మూలం) {
            తిరిగి ఖాళీ;
        }

        ఒకవేళ (కణుపు == మాతృ->ఎడమ) {
            తిరిగి మాతృ;
        }

        కణుపు = మాతృ;
    }
}
