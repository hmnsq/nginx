
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_RBTREE_H_INCLUDED_
#تعریف_کن _NGX_RBTREE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ngx_uint_t  ngx_rbtree_کلید_t;
تعريفنوع ngx_int_t   ngx_rbtree_کلید_int_t;


تعريفنوع بنية ngx_rbtree_گره_s  ngx_rbtree_گره_t;

بنية ngx_rbtree_گره_s {
    ngx_rbtree_کلید_t       کلید;
    ngx_rbtree_گره_t     *چپ;
    ngx_rbtree_گره_t     *راست;
    ngx_rbtree_گره_t     *والد;
    u_char                 color;
    u_char                 داده;
};


تعريفنوع بنية ngx_rbtree_s  ngx_rbtree_t;

تعريفنوع فراغ (*ngx_rbtree_درج_pt) (ngx_rbtree_گره_t *ریشه,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel);

بنية ngx_rbtree_s {
    ngx_rbtree_گره_t     *ریشه;
    ngx_rbtree_گره_t     *sentinel;
    ngx_rbtree_درج_pt   درج;
};


#تعریف_کن ngx_rbtree_init(درخت, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (درخت)->ریشه = s;                                                         \
    (درخت)->sentinel = s;                                                     \
    (درخت)->درج = i

#تعریف_کن ngx_rbtree_داده(گره, type, link)                                     \
    (type *) ((u_char *) (گره) - offsetof(type, link))


فراغ ngx_rbtree_درج(ngx_rbtree_t *درخت, ngx_rbtree_گره_t *گره);
فراغ ngx_rbtree_delete(ngx_rbtree_t *درخت, ngx_rbtree_گره_t *گره);
فراغ ngx_rbtree_درج_مقدار(ngx_rbtree_گره_t *ریشه, ngx_rbtree_گره_t *گره,
    ngx_rbtree_گره_t *sentinel);
فراغ ngx_rbtree_درج_timer_مقدار(ngx_rbtree_گره_t *ریشه,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel);
ngx_rbtree_گره_t *ngx_rbtree_بعدی(ngx_rbtree_t *درخت,
    ngx_rbtree_گره_t *گره);


#تعریف_کن ngx_rbt_red(گره)               ((گره)->color = 1)
#تعریف_کن ngx_rbt_black(گره)             ((گره)->color = 0)
#تعریف_کن ngx_rbt_is_red(گره)            ((گره)->color)
#تعریف_کن ngx_rbt_is_black(گره)          (!ngx_rbt_is_red(گره))
#تعریف_کن ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#تعریف_کن ngx_rbtree_sentinel_init(گره)  ngx_rbt_black(گره)


ساكن ngx_inline ngx_rbtree_گره_t *
ngx_rbtree_min(ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel)
{
    طالما (گره->چپ != sentinel) {
        گره = گره->چپ;
    }

    أرجع گره;
}


#پایان_اگر /* _NGX_RBTREE_H_INCLUDED_ */
