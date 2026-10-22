
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


પ્રકારવ્યાખ્યા ngx_uint_t  ngx_rbtree_ચાવી_t;
પ્રકારવ્યાખ્યા ngx_int_t   ngx_rbtree_ચાવી_int_t;


પ્રકારવ્યાખ્યા સંરચના ngx_rbtree_ગાંઠ_s  ngx_rbtree_ગાંઠ_t;

સંરચના ngx_rbtree_ગાંઠ_s {
    ngx_rbtree_ચાવી_t       ચાવી;
    ngx_rbtree_ગાંઠ_t     *ડાબું;
    ngx_rbtree_ગાંઠ_t     *જમણું;
    ngx_rbtree_ગાંઠ_t     *parent;
    u_char                 color;
    u_char                 data;
};


પ્રકારવ્યાખ્યા સંરચના ngx_rbtree_s  ngx_rbtree_t;

પ્રકારવ્યાખ્યા શૂન્ય (*ngx_rbtree_ઉમેરો_pt) (ngx_rbtree_ગાંઠ_t *મૂળ,
    ngx_rbtree_ગાંઠ_t *ગાંઠ, ngx_rbtree_ગાંઠ_t *sentinel);

સંરચના ngx_rbtree_s {
    ngx_rbtree_ગાંઠ_t     *મૂળ;
    ngx_rbtree_ગાંઠ_t     *sentinel;
    ngx_rbtree_ઉમેરો_pt   ઉમેરો;
};


#define ngx_rbtree_init(વૃક્ષ, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (વૃક્ષ)->મૂળ = s;                                                         \
    (વૃક્ષ)->sentinel = s;                                                     \
    (વૃક્ષ)->ઉમેરો = i

#define ngx_rbtree_data(ગાંઠ, type, link)                                     \
    (type *) ((u_char *) (ગાંઠ) - offsetof(type, link))


શૂન્ય ngx_rbtree_ઉમેરો(ngx_rbtree_t *વૃક્ષ, ngx_rbtree_ગાંઠ_t *ગાંઠ);
શૂન્ય ngx_rbtree_delete(ngx_rbtree_t *વૃક્ષ, ngx_rbtree_ગાંઠ_t *ગાંઠ);
શૂન્ય ngx_rbtree_ઉમેરો_કિંમત(ngx_rbtree_ગાંઠ_t *મૂળ, ngx_rbtree_ગાંઠ_t *ગાંઠ,
    ngx_rbtree_ગાંઠ_t *sentinel);
શૂન્ય ngx_rbtree_ઉમેરો_timer_કિંમત(ngx_rbtree_ગાંઠ_t *મૂળ,
    ngx_rbtree_ગાંઠ_t *ગાંઠ, ngx_rbtree_ગાંઠ_t *sentinel);
ngx_rbtree_ગાંઠ_t *ngx_rbtree_next(ngx_rbtree_t *વૃક્ષ,
    ngx_rbtree_ગાંઠ_t *ગાંઠ);


#define ngx_rbt_red(ગાંઠ)               ((ગાંઠ)->color = 1)
#define ngx_rbt_black(ગાંઠ)             ((ગાંઠ)->color = 0)
#define ngx_rbt_is_red(ગાંઠ)            ((ગાંઠ)->color)
#define ngx_rbt_is_black(ગાંઠ)          (!ngx_rbt_is_red(ગાંઠ))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(ગાંઠ)  ngx_rbt_black(ગાંઠ)


સ્થિર ngx_inline ngx_rbtree_ગાંઠ_t *
ngx_rbtree_min(ngx_rbtree_ગાંઠ_t *ગાંઠ, ngx_rbtree_ગાંઠ_t *sentinel)
{
    જ્યારે (ગાંઠ->ડાબું != sentinel) {
        ગાંઠ = ગાંઠ->ડાબું;
    }

    પાછા ગાંઠ;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
