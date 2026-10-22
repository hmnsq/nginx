
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारव्याख्या ngx_uint_t  ngx_rbtree_किल्ली_t;
प्रकारव्याख्या ngx_int_t   ngx_rbtree_किल्ली_int_t;


प्रकारव्याख्या संरचना ngx_rbtree_गाठ_s  ngx_rbtree_गाठ_t;

संरचना ngx_rbtree_गाठ_s {
    ngx_rbtree_किल्ली_t       किल्ली;
    ngx_rbtree_गाठ_t     *डावा;
    ngx_rbtree_गाठ_t     *उजवा;
    ngx_rbtree_गाठ_t     *parent;
    u_char                 color;
    u_char                 data;
};


प्रकारव्याख्या संरचना ngx_rbtree_s  ngx_rbtree_t;

प्रकारव्याख्या शून्य (*ngx_rbtree_घाला_pt) (ngx_rbtree_गाठ_t *मूळ,
    ngx_rbtree_गाठ_t *गाठ, ngx_rbtree_गाठ_t *sentinel);

संरचना ngx_rbtree_s {
    ngx_rbtree_गाठ_t     *मूळ;
    ngx_rbtree_गाठ_t     *sentinel;
    ngx_rbtree_घाला_pt   घाला;
};


#define ngx_rbtree_init(वृक्ष, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (वृक्ष)->मूळ = s;                                                         \
    (वृक्ष)->sentinel = s;                                                     \
    (वृक्ष)->घाला = i

#define ngx_rbtree_data(गाठ, type, link)                                     \
    (type *) ((u_char *) (गाठ) - दुरत्व(type, link))


शून्य ngx_rbtree_घाला(ngx_rbtree_t *वृक्ष, ngx_rbtree_गाठ_t *गाठ);
शून्य ngx_rbtree_delete(ngx_rbtree_t *वृक्ष, ngx_rbtree_गाठ_t *गाठ);
शून्य ngx_rbtree_घाला_मूल्य(ngx_rbtree_गाठ_t *मूळ, ngx_rbtree_गाठ_t *गाठ,
    ngx_rbtree_गाठ_t *sentinel);
शून्य ngx_rbtree_घाला_timer_मूल्य(ngx_rbtree_गाठ_t *मूळ,
    ngx_rbtree_गाठ_t *गाठ, ngx_rbtree_गाठ_t *sentinel);
ngx_rbtree_गाठ_t *ngx_rbtree_next(ngx_rbtree_t *वृक्ष,
    ngx_rbtree_गाठ_t *गाठ);


#define ngx_rbt_red(गाठ)               ((गाठ)->color = 1)
#define ngx_rbt_black(गाठ)             ((गाठ)->color = 0)
#define ngx_rbt_is_red(गाठ)            ((गाठ)->color)
#define ngx_rbt_is_black(गाठ)          (!ngx_rbt_is_red(गाठ))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(गाठ)  ngx_rbt_black(गाठ)


स्थिर ngx_inline ngx_rbtree_गाठ_t *
ngx_rbtree_min(ngx_rbtree_गाठ_t *गाठ, ngx_rbtree_गाठ_t *sentinel)
{
    पर्यंत (गाठ->डावा != sentinel) {
        गाठ = गाठ->डावा;
    }

    परत गाठ;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
