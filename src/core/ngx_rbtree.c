
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


static ngx_inline void ngx_rbtree_कालिवा_rotate(ngx_rbtree_बुहोल_t **उगात,
    ngx_rbtree_बुहोल_t *sentinel, ngx_rbtree_बुहोल_t *बुहोल);
static ngx_inline void ngx_rbtree_कानान_rotate(ngx_rbtree_बुहोल_t **उगात,
    ngx_rbtree_बुहोल_t *sentinel, ngx_rbtree_बुहोल_t *बुहोल);


void
ngx_rbtree_इसिंगित(ngx_rbtree_t *पुनो, ngx_rbtree_बुहोल_t *बुहोल)
{
    ngx_rbtree_बुहोल_t  **उगात, *temp, *sentinel;

    /* a binary tree insert */

    उगात = &पुनो->उगात;
    sentinel = पुनो->sentinel;

    if (*उगात == sentinel) {
        बुहोल->parent = NULL;
        बुहोल->कालिवा = sentinel;
        बुहोल->कानान = sentinel;
        ngx_rbt_black(बुहोल);
        *उगात = बुहोल;

        return;
    }

    पुनो->इसिंगित(*उगात, बुहोल, sentinel);

    /* re-balance tree */

    while (बुहोल != *उगात && ngx_rbt_is_red(बुहोल->parent)) {

        if (बुहोल->parent == बुहोल->parent->parent->कालिवा) {
            temp = बुहोल->parent->parent->कानान;

            if (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(बुहोल->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(बुहोल->parent->parent);
                बुहोल = बुहोल->parent->parent;

            } else {
                if (बुहोल == बुहोल->parent->कानान) {
                    बुहोल = बुहोल->parent;
                    ngx_rbtree_कालिवा_rotate(उगात, sentinel, बुहोल);
                }

                ngx_rbt_black(बुहोल->parent);
                ngx_rbt_red(बुहोल->parent->parent);
                ngx_rbtree_कानान_rotate(उगात, sentinel, बुहोल->parent->parent);
            }

        } else {
            temp = बुहोल->parent->parent->कालिवा;

            if (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(बुहोल->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(बुहोल->parent->parent);
                बुहोल = बुहोल->parent->parent;

            } else {
                if (बुहोल == बुहोल->parent->कालिवा) {
                    बुहोल = बुहोल->parent;
                    ngx_rbtree_कानान_rotate(उगात, sentinel, बुहोल);
                }

                ngx_rbt_black(बुहोल->parent);
                ngx_rbt_red(बुहोल->parent->parent);
                ngx_rbtree_कालिवा_rotate(उगात, sentinel, बुहोल->parent->parent);
            }
        }
    }

    ngx_rbt_black(*उगात);
}


void
ngx_rbtree_इसिंगित_हालागा(ngx_rbtree_बुहोल_t *temp, ngx_rbtree_बुहोल_t *बुहोल,
    ngx_rbtree_बुहोल_t *sentinel)
{
    ngx_rbtree_बुहोल_t  **p;

    for ( ;; ) {

        p = (बुहोल->सुसि < temp->सुसि) ? &temp->कालिवा : &temp->कानान;

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = बुहोल;
    बुहोल->parent = temp;
    बुहोल->कालिवा = sentinel;
    बुहोल->कानान = sentinel;
    ngx_rbt_red(बुहोल);
}


void
ngx_rbtree_इसिंगित_timer_हालागा(ngx_rbtree_बुहोल_t *temp, ngx_rbtree_बुहोल_t *बुहोल,
    ngx_rbtree_बुहोल_t *sentinel)
{
    ngx_rbtree_बुहोल_t  **p;

    for ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_सुसि_int_t) (बुहोल->सुसि - temp->सुसि) < 0)
            ? &temp->कालिवा : &temp->कानान;

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = बुहोल;
    बुहोल->parent = temp;
    बुहोल->कालिवा = sentinel;
    बुहोल->कानान = sentinel;
    ngx_rbt_red(बुहोल);
}


void
ngx_rbtree_delete(ngx_rbtree_t *पुनो, ngx_rbtree_बुहोल_t *बुहोल)
{
    ngx_uint_t           red;
    ngx_rbtree_बुहोल_t  **उगात, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    उगात = &पुनो->उगात;
    sentinel = पुनो->sentinel;

    if (बुहोल->कालिवा == sentinel) {
        temp = बुहोल->कानान;
        subst = बुहोल;

    } else if (बुहोल->कानान == sentinel) {
        temp = बुहोल->कालिवा;
        subst = बुहोल;

    } else {
        subst = ngx_rbtree_min(बुहोल->कानान, sentinel);
        temp = subst->कानान;
    }

    if (subst == *उगात) {
        *उगात = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        बुहोल->कालिवा = NULL;
        बुहोल->कानान = NULL;
        बुहोल->parent = NULL;
        बुहोल->सुसि = 0;

        return;
    }

    red = ngx_rbt_is_red(subst);

    if (subst == subst->parent->कालिवा) {
        subst->parent->कालिवा = temp;

    } else {
        subst->parent->कानान = temp;
    }

    if (subst == बुहोल) {

        temp->parent = subst->parent;

    } else {

        if (subst->parent == बुहोल) {
            temp->parent = subst;

        } else {
            temp->parent = subst->parent;
        }

        subst->कालिवा = बुहोल->कालिवा;
        subst->कानान = बुहोल->कानान;
        subst->parent = बुहोल->parent;
        ngx_rbt_copy_color(subst, बुहोल);

        if (बुहोल == *उगात) {
            *उगात = subst;

        } else {
            if (बुहोल == बुहोल->parent->कालिवा) {
                बुहोल->parent->कालिवा = subst;
            } else {
                बुहोल->parent->कानान = subst;
            }
        }

        if (subst->कालिवा != sentinel) {
            subst->कालिवा->parent = subst;
        }

        if (subst->कानान != sentinel) {
            subst->कानान->parent = subst;
        }
    }

    /* DEBUG stuff */
    बुहोल->कालिवा = NULL;
    बुहोल->कानान = NULL;
    बुहोल->parent = NULL;
    बुहोल->सुसि = 0;

    if (red) {
        return;
    }

    /* a delete fixup */

    while (temp != *उगात && ngx_rbt_is_black(temp)) {

        if (temp == temp->parent->कालिवा) {
            w = temp->parent->कानान;

            if (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_कालिवा_rotate(उगात, sentinel, temp->parent);
                w = temp->parent->कानान;
            }

            if (ngx_rbt_is_black(w->कालिवा) && ngx_rbt_is_black(w->कानान)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } else {
                if (ngx_rbt_is_black(w->कानान)) {
                    ngx_rbt_black(w->कालिवा);
                    ngx_rbt_red(w);
                    ngx_rbtree_कानान_rotate(उगात, sentinel, w);
                    w = temp->parent->कानान;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->कानान);
                ngx_rbtree_कालिवा_rotate(उगात, sentinel, temp->parent);
                temp = *उगात;
            }

        } else {
            w = temp->parent->कालिवा;

            if (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_कानान_rotate(उगात, sentinel, temp->parent);
                w = temp->parent->कालिवा;
            }

            if (ngx_rbt_is_black(w->कालिवा) && ngx_rbt_is_black(w->कानान)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } else {
                if (ngx_rbt_is_black(w->कालिवा)) {
                    ngx_rbt_black(w->कानान);
                    ngx_rbt_red(w);
                    ngx_rbtree_कालिवा_rotate(उगात, sentinel, w);
                    w = temp->parent->कालिवा;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->कालिवा);
                ngx_rbtree_कानान_rotate(उगात, sentinel, temp->parent);
                temp = *उगात;
            }
        }
    }

    ngx_rbt_black(temp);
}


static ngx_inline void
ngx_rbtree_कालिवा_rotate(ngx_rbtree_बुहोल_t **उगात, ngx_rbtree_बुहोल_t *sentinel,
    ngx_rbtree_बुहोल_t *बुहोल)
{
    ngx_rbtree_बुहोल_t  *temp;

    temp = बुहोल->कानान;
    बुहोल->कानान = temp->कालिवा;

    if (temp->कालिवा != sentinel) {
        temp->कालिवा->parent = बुहोल;
    }

    temp->parent = बुहोल->parent;

    if (बुहोल == *उगात) {
        *उगात = temp;

    } else if (बुहोल == बुहोल->parent->कालिवा) {
        बुहोल->parent->कालिवा = temp;

    } else {
        बुहोल->parent->कानान = temp;
    }

    temp->कालिवा = बुहोल;
    बुहोल->parent = temp;
}


static ngx_inline void
ngx_rbtree_कानान_rotate(ngx_rbtree_बुहोल_t **उगात, ngx_rbtree_बुहोल_t *sentinel,
    ngx_rbtree_बुहोल_t *बुहोल)
{
    ngx_rbtree_बुहोल_t  *temp;

    temp = बुहोल->कालिवा;
    बुहोल->कालिवा = temp->कानान;

    if (temp->कानान != sentinel) {
        temp->कानान->parent = बुहोल;
    }

    temp->parent = बुहोल->parent;

    if (बुहोल == *उगात) {
        *उगात = temp;

    } else if (बुहोल == बुहोल->parent->कानान) {
        बुहोल->parent->कानान = temp;

    } else {
        बुहोल->parent->कालिवा = temp;
    }

    temp->कानान = बुहोल;
    बुहोल->parent = temp;
}


ngx_rbtree_बुहोल_t *
ngx_rbtree_next(ngx_rbtree_t *पुनो, ngx_rbtree_बुहोल_t *बुहोल)
{
    ngx_rbtree_बुहोल_t  *उगात, *sentinel, *parent;

    sentinel = पुनो->sentinel;

    if (बुहोल->कानान != sentinel) {
        return ngx_rbtree_min(बुहोल->कानान, sentinel);
    }

    उगात = पुनो->उगात;

    for ( ;; ) {
        parent = बुहोल->parent;

        if (बुहोल == उगात) {
            return NULL;
        }

        if (बुहोल == parent->कालिवा) {
            return parent;
        }

        बुहोल = parent;
    }
}
