
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


प्रकारनिर्देश ngx_uint_t  ngx_rbtree_कुञ्चिका_t;
प्रकारनिर्देश ngx_int_t   ngx_rbtree_कुञ्चिका_int_t;


प्रकारनिर्देश संरचना ngx_rbtree_ग्रन्थिः_s  ngx_rbtree_ग्रन्थिः_t;

संरचना ngx_rbtree_ग्रन्थिः_s {
    ngx_rbtree_कुञ्चिका_t       कुञ्चिका;
    ngx_rbtree_ग्रन्थिः_t     *वामः;
    ngx_rbtree_ग्रन्थिः_t     *दक्षिणः;
    ngx_rbtree_ग्रन्थिः_t     *parent;
    u_char                 color;
    u_char                 data;
};


प्रकारनिर्देश संरचना ngx_rbtree_s  ngx_rbtree_t;

प्रकारनिर्देश शून्य (*ngx_rbtree_निवेशय_pt) (ngx_rbtree_ग्रन्थिः_t *मूलम्,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel);

संरचना ngx_rbtree_s {
    ngx_rbtree_ग्रन्थिः_t     *मूलम्;
    ngx_rbtree_ग्रन्थिः_t     *sentinel;
    ngx_rbtree_निवेशय_pt   निवेशय;
};


#define ngx_rbtree_init(वृक्षः, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (वृक्षः)->मूलम् = s;                                                         \
    (वृक्षः)->sentinel = s;                                                     \
    (वृक्षः)->निवेशय = i

#define ngx_rbtree_data(ग्रन्थिः, type, link)                                     \
    (type *) ((u_char *) (ग्रन्थिः) - offsetof(type, link))


शून्य ngx_rbtree_निवेशय(ngx_rbtree_t *वृक्षः, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः);
शून्य ngx_rbtree_delete(ngx_rbtree_t *वृक्षः, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः);
शून्य ngx_rbtree_निवेशय_मानम्(ngx_rbtree_ग्रन्थिः_t *मूलम्, ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः,
    ngx_rbtree_ग्रन्थिः_t *sentinel);
शून्य ngx_rbtree_निवेशय_timer_मानम्(ngx_rbtree_ग्रन्थिः_t *मूलम्,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel);
ngx_rbtree_ग्रन्थिः_t *ngx_rbtree_next(ngx_rbtree_t *वृक्षः,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः);


#define ngx_rbt_red(ग्रन्थिः)               ((ग्रन्थिः)->color = 1)
#define ngx_rbt_black(ग्रन्थिः)             ((ग्रन्थिः)->color = 0)
#define ngx_rbt_is_red(ग्रन्थिः)            ((ग्रन्थिः)->color)
#define ngx_rbt_is_black(ग्रन्थिः)          (!ngx_rbt_is_red(ग्रन्थिः))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(ग्रन्थिः)  ngx_rbt_black(ग्रन्थिः)


स्थिर ngx_inline ngx_rbtree_ग्रन्थिः_t *
ngx_rbtree_min(ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel)
{
    याव (ग्रन्थिः->वामः != sentinel) {
        ग्रन्थिः = ग्रन्थिः->वामः;
    }

    पटिनिवत्त ग्रन्थिः;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
