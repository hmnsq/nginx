
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश ngx_uint_t  ngx_rbtree_साँचो_t;
प्रकारनिर्देश ngx_int_t   ngx_rbtree_साँचो_int_t;


प्रकारनिर्देश संरचना ngx_rbtree_गाँठो_s  ngx_rbtree_गाँठो_t;

संरचना ngx_rbtree_गाँठो_s {
    ngx_rbtree_साँचो_t       साँचो;
    ngx_rbtree_गाँठो_t     *बायाँ;
    ngx_rbtree_गाँठो_t     *दायाँ;
    ngx_rbtree_गाँठो_t     *parent;
    u_char                 color;
    u_char                 data;
};


प्रकारनिर्देश संरचना ngx_rbtree_s  ngx_rbtree_t;

प्रकारनिर्देश शून्य (*ngx_rbtree_घुसाउ_pt) (ngx_rbtree_गाँठो_t *जरा,
    ngx_rbtree_गाँठो_t *गाँठो, ngx_rbtree_गाँठो_t *sentinel);

संरचना ngx_rbtree_s {
    ngx_rbtree_गाँठो_t     *जरा;
    ngx_rbtree_गाँठो_t     *sentinel;
    ngx_rbtree_घुसाउ_pt   घुसाउ;
};


#define ngx_rbtree_init(रूख, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (रूख)->जरा = s;                                                         \
    (रूख)->sentinel = s;                                                     \
    (रूख)->घुसाउ = i

#define ngx_rbtree_data(गाँठो, type, link)                                     \
    (type *) ((u_char *) (गाँठो) - दुरत्व(type, link))


शून्य ngx_rbtree_घुसाउ(ngx_rbtree_t *रूख, ngx_rbtree_गाँठो_t *गाँठो);
शून्य ngx_rbtree_delete(ngx_rbtree_t *रूख, ngx_rbtree_गाँठो_t *गाँठो);
शून्य ngx_rbtree_घुसाउ_मान(ngx_rbtree_गाँठो_t *जरा, ngx_rbtree_गाँठो_t *गाँठो,
    ngx_rbtree_गाँठो_t *sentinel);
शून्य ngx_rbtree_घुसाउ_timer_मान(ngx_rbtree_गाँठो_t *जरा,
    ngx_rbtree_गाँठो_t *गाँठो, ngx_rbtree_गाँठो_t *sentinel);
ngx_rbtree_गाँठो_t *ngx_rbtree_next(ngx_rbtree_t *रूख,
    ngx_rbtree_गाँठो_t *गाँठो);


#define ngx_rbt_red(गाँठो)               ((गाँठो)->color = 1)
#define ngx_rbt_black(गाँठो)             ((गाँठो)->color = 0)
#define ngx_rbt_is_red(गाँठो)            ((गाँठो)->color)
#define ngx_rbt_is_black(गाँठो)          (!ngx_rbt_is_red(गाँठो))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(गाँठो)  ngx_rbt_black(गाँठो)


स्थिर ngx_inline ngx_rbtree_गाँठो_t *
ngx_rbtree_min(ngx_rbtree_गाँठो_t *गाँठो, ngx_rbtree_गाँठो_t *sentinel)
{
    जबसम्म (गाँठो->बायाँ != sentinel) {
        गाँठो = गाँठो->बायाँ;
    }

    फर्काऊ गाँठो;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
