
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */


સ્થિર ngx_inline શૂન્ય ngx_rbtree_ડાબું_rotate(ngx_rbtree_ગાંઠ_t **મૂળ,
    ngx_rbtree_ગાંઠ_t *sentinel, ngx_rbtree_ગાંઠ_t *ગાંઠ);
સ્થિર ngx_inline શૂન્ય ngx_rbtree_જમણું_rotate(ngx_rbtree_ગાંઠ_t **મૂળ,
    ngx_rbtree_ગાંઠ_t *sentinel, ngx_rbtree_ગાંઠ_t *ગાંઠ);


શૂન્ય
ngx_rbtree_ઉમેરો(ngx_rbtree_t *વૃક્ષ, ngx_rbtree_ગાંઠ_t *ગાંઠ)
{
    ngx_rbtree_ગાંઠ_t  **મૂળ, *temp, *sentinel;

    /* a binary tree insert */

    મૂળ = &વૃક્ષ->મૂળ;
    sentinel = વૃક્ષ->sentinel;

    જો (*મૂળ == sentinel) {
        ગાંઠ->parent = ખાલી;
        ગાંઠ->ડાબું = sentinel;
        ગાંઠ->જમણું = sentinel;
        ngx_rbt_black(ગાંઠ);
        *મૂળ = ગાંઠ;

        પાછા;
    }

    વૃક્ષ->ઉમેરો(*મૂળ, ગાંઠ, sentinel);

    /* re-balance tree */

    જ્યારે (ગાંઠ != *મૂળ && ngx_rbt_is_red(ગાંઠ->parent)) {

        જો (ગાંઠ->parent == ગાંઠ->parent->parent->ડાબું) {
            temp = ગાંઠ->parent->parent->જમણું;

            જો (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(ગાંઠ->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(ગાંઠ->parent->parent);
                ગાંઠ = ગાંઠ->parent->parent;

            } નહીંતર {
                જો (ગાંઠ == ગાંઠ->parent->જમણું) {
                    ગાંઠ = ગાંઠ->parent;
                    ngx_rbtree_ડાબું_rotate(મૂળ, sentinel, ગાંઠ);
                }

                ngx_rbt_black(ગાંઠ->parent);
                ngx_rbt_red(ગાંઠ->parent->parent);
                ngx_rbtree_જમણું_rotate(મૂળ, sentinel, ગાંઠ->parent->parent);
            }

        } નહીંતર {
            temp = ગાંઠ->parent->parent->ડાબું;

            જો (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(ગાંઠ->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(ગાંઠ->parent->parent);
                ગાંઠ = ગાંઠ->parent->parent;

            } નહીંતર {
                જો (ગાંઠ == ગાંઠ->parent->ડાબું) {
                    ગાંઠ = ગાંઠ->parent;
                    ngx_rbtree_જમણું_rotate(મૂળ, sentinel, ગાંઠ);
                }

                ngx_rbt_black(ગાંઠ->parent);
                ngx_rbt_red(ગાંઠ->parent->parent);
                ngx_rbtree_ડાબું_rotate(મૂળ, sentinel, ગાંઠ->parent->parent);
            }
        }
    }

    ngx_rbt_black(*મૂળ);
}


શૂન્ય
ngx_rbtree_ઉમેરો_કિંમત(ngx_rbtree_ગાંઠ_t *temp, ngx_rbtree_ગાંઠ_t *ગાંઠ,
    ngx_rbtree_ગાંઠ_t *sentinel)
{
    ngx_rbtree_ગાંઠ_t  **p;

    માટે ( ;; ) {

        p = (ગાંઠ->ચાવી < temp->ચાવી) ? &temp->ડાબું : &temp->જમણું;

        જો (*p == sentinel) {
            તોડો;
        }

        temp = *p;
    }

    *p = ગાંઠ;
    ગાંઠ->parent = temp;
    ગાંઠ->ડાબું = sentinel;
    ગાંઠ->જમણું = sentinel;
    ngx_rbt_red(ગાંઠ);
}


શૂન્ય
ngx_rbtree_ઉમેરો_timer_કિંમત(ngx_rbtree_ગાંઠ_t *temp, ngx_rbtree_ગાંઠ_t *ગાંઠ,
    ngx_rbtree_ગાંઠ_t *sentinel)
{
    ngx_rbtree_ગાંઠ_t  **p;

    માટે ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */

        p = ((ngx_rbtree_ચાવી_int_t) (ગાંઠ->ચાવી - temp->ચાવી) < 0)
            ? &temp->ડાબું : &temp->જમણું;

        જો (*p == sentinel) {
            તોડો;
        }

        temp = *p;
    }

    *p = ગાંઠ;
    ગાંઠ->parent = temp;
    ગાંઠ->ડાબું = sentinel;
    ગાંઠ->જમણું = sentinel;
    ngx_rbt_red(ગાંઠ);
}


શૂન્ય
ngx_rbtree_delete(ngx_rbtree_t *વૃક્ષ, ngx_rbtree_ગાંઠ_t *ગાંઠ)
{
    ngx_uint_t           red;
    ngx_rbtree_ગાંઠ_t  **મૂળ, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    મૂળ = &વૃક્ષ->મૂળ;
    sentinel = વૃક્ષ->sentinel;

    જો (ગાંઠ->ડાબું == sentinel) {
        temp = ગાંઠ->જમણું;
        subst = ગાંઠ;

    } નહીંતર જો (ગાંઠ->જમણું == sentinel) {
        temp = ગાંઠ->ડાબું;
        subst = ગાંઠ;

    } નહીંતર {
        subst = ngx_rbtree_min(ગાંઠ->જમણું, sentinel);
        temp = subst->જમણું;
    }

    જો (subst == *મૂળ) {
        *મૂળ = temp;
        ngx_rbt_black(temp);

        /* DEBUG stuff */
        ગાંઠ->ડાબું = ખાલી;
        ગાંઠ->જમણું = ખાલી;
        ગાંઠ->parent = ખાલી;
        ગાંઠ->ચાવી = 0;

        પાછા;
    }

    red = ngx_rbt_is_red(subst);

    જો (subst == subst->parent->ડાબું) {
        subst->parent->ડાબું = temp;

    } નહીંતર {
        subst->parent->જમણું = temp;
    }

    જો (subst == ગાંઠ) {

        temp->parent = subst->parent;

    } નહીંતર {

        જો (subst->parent == ગાંઠ) {
            temp->parent = subst;

        } નહીંતર {
            temp->parent = subst->parent;
        }

        subst->ડાબું = ગાંઠ->ડાબું;
        subst->જમણું = ગાંઠ->જમણું;
        subst->parent = ગાંઠ->parent;
        ngx_rbt_copy_color(subst, ગાંઠ);

        જો (ગાંઠ == *મૂળ) {
            *મૂળ = subst;

        } નહીંતર {
            જો (ગાંઠ == ગાંઠ->parent->ડાબું) {
                ગાંઠ->parent->ડાબું = subst;
            } નહીંતર {
                ગાંઠ->parent->જમણું = subst;
            }
        }

        જો (subst->ડાબું != sentinel) {
            subst->ડાબું->parent = subst;
        }

        જો (subst->જમણું != sentinel) {
            subst->જમણું->parent = subst;
        }
    }

    /* DEBUG stuff */
    ગાંઠ->ડાબું = ખાલી;
    ગાંઠ->જમણું = ખાલી;
    ગાંઠ->parent = ખાલી;
    ગાંઠ->ચાવી = 0;

    જો (red) {
        પાછા;
    }

    /* a delete fixup */

    જ્યારે (temp != *મૂળ && ngx_rbt_is_black(temp)) {

        જો (temp == temp->parent->ડાબું) {
            w = temp->parent->જમણું;

            જો (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_ડાબું_rotate(મૂળ, sentinel, temp->parent);
                w = temp->parent->જમણું;
            }

            જો (ngx_rbt_is_black(w->ડાબું) && ngx_rbt_is_black(w->જમણું)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } નહીંતર {
                જો (ngx_rbt_is_black(w->જમણું)) {
                    ngx_rbt_black(w->ડાબું);
                    ngx_rbt_red(w);
                    ngx_rbtree_જમણું_rotate(મૂળ, sentinel, w);
                    w = temp->parent->જમણું;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->જમણું);
                ngx_rbtree_ડાબું_rotate(મૂળ, sentinel, temp->parent);
                temp = *મૂળ;
            }

        } નહીંતર {
            w = temp->parent->ડાબું;

            જો (ngx_rbt_is_red(w)) {
                ngx_rbt_black(w);
                ngx_rbt_red(temp->parent);
                ngx_rbtree_જમણું_rotate(મૂળ, sentinel, temp->parent);
                w = temp->parent->ડાબું;
            }

            જો (ngx_rbt_is_black(w->ડાબું) && ngx_rbt_is_black(w->જમણું)) {
                ngx_rbt_red(w);
                temp = temp->parent;

            } નહીંતર {
                જો (ngx_rbt_is_black(w->ડાબું)) {
                    ngx_rbt_black(w->જમણું);
                    ngx_rbt_red(w);
                    ngx_rbtree_ડાબું_rotate(મૂળ, sentinel, w);
                    w = temp->parent->ડાબું;
                }

                ngx_rbt_copy_color(w, temp->parent);
                ngx_rbt_black(temp->parent);
                ngx_rbt_black(w->ડાબું);
                ngx_rbtree_જમણું_rotate(મૂળ, sentinel, temp->parent);
                temp = *મૂળ;
            }
        }
    }

    ngx_rbt_black(temp);
}


સ્થિર ngx_inline શૂન્ય
ngx_rbtree_ડાબું_rotate(ngx_rbtree_ગાંઠ_t **મૂળ, ngx_rbtree_ગાંઠ_t *sentinel,
    ngx_rbtree_ગાંઠ_t *ગાંઠ)
{
    ngx_rbtree_ગાંઠ_t  *temp;

    temp = ગાંઠ->જમણું;
    ગાંઠ->જમણું = temp->ડાબું;

    જો (temp->ડાબું != sentinel) {
        temp->ડાબું->parent = ગાંઠ;
    }

    temp->parent = ગાંઠ->parent;

    જો (ગાંઠ == *મૂળ) {
        *મૂળ = temp;

    } નહીંતર જો (ગાંઠ == ગાંઠ->parent->ડાબું) {
        ગાંઠ->parent->ડાબું = temp;

    } નહીંતર {
        ગાંઠ->parent->જમણું = temp;
    }

    temp->ડાબું = ગાંઠ;
    ગાંઠ->parent = temp;
}


સ્થિર ngx_inline શૂન્ય
ngx_rbtree_જમણું_rotate(ngx_rbtree_ગાંઠ_t **મૂળ, ngx_rbtree_ગાંઠ_t *sentinel,
    ngx_rbtree_ગાંઠ_t *ગાંઠ)
{
    ngx_rbtree_ગાંઠ_t  *temp;

    temp = ગાંઠ->ડાબું;
    ગાંઠ->ડાબું = temp->જમણું;

    જો (temp->જમણું != sentinel) {
        temp->જમણું->parent = ગાંઠ;
    }

    temp->parent = ગાંઠ->parent;

    જો (ગાંઠ == *મૂળ) {
        *મૂળ = temp;

    } નહીંતર જો (ગાંઠ == ગાંઠ->parent->જમણું) {
        ગાંઠ->parent->જમણું = temp;

    } નહીંતર {
        ગાંઠ->parent->ડાબું = temp;
    }

    temp->જમણું = ગાંઠ;
    ગાંઠ->parent = temp;
}


ngx_rbtree_ગાંઠ_t *
ngx_rbtree_next(ngx_rbtree_t *વૃક્ષ, ngx_rbtree_ગાંઠ_t *ગાંઠ)
{
    ngx_rbtree_ગાંઠ_t  *મૂળ, *sentinel, *parent;

    sentinel = વૃક્ષ->sentinel;

    જો (ગાંઠ->જમણું != sentinel) {
        પાછા ngx_rbtree_min(ગાંઠ->જમણું, sentinel);
    }

    મૂળ = વૃક્ષ->મૂળ;

    માટે ( ;; ) {
        parent = ગાંઠ->parent;

        જો (ગાંઠ == મૂળ) {
            પાછા ખાલી;
        }

        જો (ગાંઠ == parent->ડાબું) {
            પાછા parent;
        }

        ગાંઠ = parent;
    }
}
