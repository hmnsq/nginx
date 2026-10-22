
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_RBTREE_H_INCLUDED_
#घोषणा _NGX_RBTREE_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


प्रकार ngx_uint_t  ngx_rbtree_कुंजी_t;
प्रकार ngx_int_t   ngx_rbtree_कुंजी_int_t;


प्रकार काष्ठा ngx_rbtree_गाँठ_s  ngx_rbtree_गाँठ_t;

काष्ठा ngx_rbtree_गाँठ_s {
    ngx_rbtree_कुंजी_t       कुंजी;
    ngx_rbtree_गाँठ_t     *बायाँ;
    ngx_rbtree_गाँठ_t     *दायाँ;
    ngx_rbtree_गाँठ_t     *parent;
    u_char                 color;
    u_char                 तथ्य;
};


प्रकार काष्ठा ngx_rbtree_s  ngx_rbtree_t;

प्रकार व्योम (*ngx_rbtree_जोड़ो_pt) (ngx_rbtree_गाँठ_t *मूल,
    ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel);

काष्ठा ngx_rbtree_s {
    ngx_rbtree_गाँठ_t     *मूल;
    ngx_rbtree_गाँठ_t     *sentinel;
    ngx_rbtree_जोड़ो_pt   जोड़ो;
};


#घोषणा ngx_rbtree_init(वृक्ष, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (वृक्ष)->मूल = s;                                                         \
    (वृक्ष)->sentinel = s;                                                     \
    (वृक्ष)->जोड़ो = i

#घोषणा ngx_rbtree_तथ्य(गाँठ, type, link)                                     \
    (type *) ((u_char *) (गाँठ) - दुरत्व(type, link))


व्योम ngx_rbtree_जोड़ो(ngx_rbtree_t *वृक्ष, ngx_rbtree_गाँठ_t *गाँठ);
व्योम ngx_rbtree_delete(ngx_rbtree_t *वृक्ष, ngx_rbtree_गाँठ_t *गाँठ);
व्योम ngx_rbtree_जोड़ो_मूल्य(ngx_rbtree_गाँठ_t *मूल, ngx_rbtree_गाँठ_t *गाँठ,
    ngx_rbtree_गाँठ_t *sentinel);
व्योम ngx_rbtree_जोड़ो_timer_मूल्य(ngx_rbtree_गाँठ_t *मूल,
    ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel);
ngx_rbtree_गाँठ_t *ngx_rbtree_अग्र(ngx_rbtree_t *वृक्ष,
    ngx_rbtree_गाँठ_t *गाँठ);


#घोषणा ngx_rbt_red(गाँठ)               ((गाँठ)->color = 1)
#घोषणा ngx_rbt_black(गाँठ)             ((गाँठ)->color = 0)
#घोषणा ngx_rbt_is_red(गाँठ)            ((गाँठ)->color)
#घोषणा ngx_rbt_is_black(गाँठ)          (!ngx_rbt_is_red(गाँठ))
#घोषणा ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#घोषणा ngx_rbtree_sentinel_init(गाँठ)  ngx_rbt_black(गाँठ)


अटल ngx_inline ngx_rbtree_गाँठ_t *
ngx_rbtree_min(ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel)
{
    जबतक (गाँठ->बायाँ != sentinel) {
        गाँठ = गाँठ->बायाँ;
    }

    वापस गाँठ;
}


#पूर्ण_अगर /* _NGX_RBTREE_H_INCLUDED_ */
