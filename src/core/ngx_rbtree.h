
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश ngx_uint_t  ngx_rbtree_कुंजी_t;
प्रकारनिर्देश ngx_int_t   ngx_rbtree_कुंजी_int_t;


प्रकारनिर्देश संरचना ngx_rbtree_गाँठ_s  ngx_rbtree_गाँठ_t;

संरचना ngx_rbtree_गाँठ_s {
    ngx_rbtree_कुंजी_t       कुंजी;
    ngx_rbtree_गाँठ_t     *बायाँ;
    ngx_rbtree_गाँठ_t     *दायाँ;
    ngx_rbtree_गाँठ_t     *parent;
    u_char                 color;
    u_char                 data;
};


प्रकारनिर्देश संरचना ngx_rbtree_s  ngx_rbtree_t;

प्रकारनिर्देश शून्य (*ngx_rbtree_जोड़ो_pt) (ngx_rbtree_गाँठ_t *मूल,
    ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel);

संरचना ngx_rbtree_s {
    ngx_rbtree_गाँठ_t     *मूल;
    ngx_rbtree_गाँठ_t     *sentinel;
    ngx_rbtree_जोड़ो_pt   जोड़ो;
};


#define ngx_rbtree_init(वृक्ष, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (वृक्ष)->मूल = s;                                                         \
    (वृक्ष)->sentinel = s;                                                     \
    (वृक्ष)->जोड़ो = i

#define ngx_rbtree_data(गाँठ, type, link)                                     \
    (type *) ((u_char *) (गाँठ) - दुरत्व(type, link))


शून्य ngx_rbtree_जोड़ो(ngx_rbtree_t *वृक्ष, ngx_rbtree_गाँठ_t *गाँठ);
शून्य ngx_rbtree_delete(ngx_rbtree_t *वृक्ष, ngx_rbtree_गाँठ_t *गाँठ);
शून्य ngx_rbtree_जोड़ो_मूल्य(ngx_rbtree_गाँठ_t *मूल, ngx_rbtree_गाँठ_t *गाँठ,
    ngx_rbtree_गाँठ_t *sentinel);
शून्य ngx_rbtree_जोड़ो_timer_मूल्य(ngx_rbtree_गाँठ_t *मूल,
    ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel);
ngx_rbtree_गाँठ_t *ngx_rbtree_next(ngx_rbtree_t *वृक्ष,
    ngx_rbtree_गाँठ_t *गाँठ);


#define ngx_rbt_red(गाँठ)               ((गाँठ)->color = 1)
#define ngx_rbt_black(गाँठ)             ((गाँठ)->color = 0)
#define ngx_rbt_is_red(गाँठ)            ((गाँठ)->color)
#define ngx_rbt_is_black(गाँठ)          (!ngx_rbt_is_red(गाँठ))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(गाँठ)  ngx_rbt_black(गाँठ)


स्थिर ngx_inline ngx_rbtree_गाँठ_t *
ngx_rbtree_min(ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel)
{
    यावत् (गाँठ->बायाँ != sentinel) {
        गाँठ = गाँठ->बायाँ;
    }

    प्रत्यागम गाँठ;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
