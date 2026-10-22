
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


تعریفنوع ngx_uint_t  ngx_rbtree_کلید_t;
تعریفنوع ngx_int_t   ngx_rbtree_کلید_int_t;


تعریفنوع ساختار ngx_rbtree_گره_s  ngx_rbtree_گره_t;

ساختار ngx_rbtree_گره_s {
    ngx_rbtree_کلید_t       کلید;
    ngx_rbtree_گره_t     *چپ;
    ngx_rbtree_گره_t     *راست;
    ngx_rbtree_گره_t     *parent;
    u_char                 color;
    u_char                 data;
};


تعریفنوع ساختار ngx_rbtree_s  ngx_rbtree_t;

تعریفنوع تهی (*ngx_rbtree_درج_pt) (ngx_rbtree_گره_t *ریشه,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel);

ساختار ngx_rbtree_s {
    ngx_rbtree_گره_t     *ریشه;
    ngx_rbtree_گره_t     *sentinel;
    ngx_rbtree_درج_pt   درج;
};


#define ngx_rbtree_init(درخت, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (درخت)->ریشه = s;                                                         \
    (درخت)->sentinel = s;                                                     \
    (درخت)->درج = i

#define ngx_rbtree_data(گره, type, link)                                     \
    (type *) ((u_char *) (گره) - offsetof(type, link))


تهی ngx_rbtree_درج(ngx_rbtree_t *درخت, ngx_rbtree_گره_t *گره);
تهی ngx_rbtree_delete(ngx_rbtree_t *درخت, ngx_rbtree_گره_t *گره);
تهی ngx_rbtree_درج_مقدار(ngx_rbtree_گره_t *ریشه, ngx_rbtree_گره_t *گره,
    ngx_rbtree_گره_t *sentinel);
تهی ngx_rbtree_درج_timer_مقدار(ngx_rbtree_گره_t *ریشه,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel);
ngx_rbtree_گره_t *ngx_rbtree_next(ngx_rbtree_t *درخت,
    ngx_rbtree_گره_t *گره);


#define ngx_rbt_red(گره)               ((گره)->color = 1)
#define ngx_rbt_black(گره)             ((گره)->color = 0)
#define ngx_rbt_is_red(گره)            ((گره)->color)
#define ngx_rbt_is_black(گره)          (!ngx_rbt_is_red(گره))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(گره)  ngx_rbt_black(گره)


ایستا ngx_inline ngx_rbtree_گره_t *
ngx_rbtree_min(ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel)
{
    وقتی (گره->چپ != sentinel) {
        گره = گره->چپ;
    }

    بازگشت گره;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
