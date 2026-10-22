
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_RBTREE_H_INCLUDED_
#وصف_ڪريو _NGX_RBTREE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع ngx_uint_t  ngx_rbtree_ڪنجي_t;
تعريفنوع ngx_int_t   ngx_rbtree_ڪنجي_int_t;


تعريفنوع بنية ngx_rbtree_ڳنڍ_s  ngx_rbtree_ڳنڍ_t;

بنية ngx_rbtree_ڳنڍ_s {
    ngx_rbtree_ڪنجي_t       ڪنجي;
    ngx_rbtree_ڳنڍ_t     *کاٻو;
    ngx_rbtree_ڳنڍ_t     *ساڄو;
    ngx_rbtree_ڳنڍ_t     *والد;
    u_char                 color;
    u_char                 معلومات;
};


تعريفنوع بنية ngx_rbtree_s  ngx_rbtree_t;

تعريفنوع فراغ (*ngx_rbtree_وجهو_pt) (ngx_rbtree_ڳنڍ_t *پاڙ,
    ngx_rbtree_ڳنڍ_t *ڳنڍ, ngx_rbtree_ڳنڍ_t *sentinel);

بنية ngx_rbtree_s {
    ngx_rbtree_ڳنڍ_t     *پاڙ;
    ngx_rbtree_ڳنڍ_t     *sentinel;
    ngx_rbtree_وجهو_pt   وجهو;
};


#وصف_ڪريو ngx_rbtree_init(وڻ, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (وڻ)->پاڙ = s;                                                         \
    (وڻ)->sentinel = s;                                                     \
    (وڻ)->وجهو = i

#وصف_ڪريو ngx_rbtree_معلومات(ڳنڍ, type, link)                                     \
    (type *) ((u_char *) (ڳنڍ) - offsetof(type, link))


فراغ ngx_rbtree_وجهو(ngx_rbtree_t *وڻ, ngx_rbtree_ڳنڍ_t *ڳنڍ);
فراغ ngx_rbtree_delete(ngx_rbtree_t *وڻ, ngx_rbtree_ڳنڍ_t *ڳنڍ);
فراغ ngx_rbtree_وجهو_قدر(ngx_rbtree_ڳنڍ_t *پاڙ, ngx_rbtree_ڳنڍ_t *ڳنڍ,
    ngx_rbtree_ڳنڍ_t *sentinel);
فراغ ngx_rbtree_وجهو_timer_قدر(ngx_rbtree_ڳنڍ_t *پاڙ,
    ngx_rbtree_ڳنڍ_t *ڳنڍ, ngx_rbtree_ڳنڍ_t *sentinel);
ngx_rbtree_ڳنڍ_t *ngx_rbtree_ايندڙ(ngx_rbtree_t *وڻ,
    ngx_rbtree_ڳنڍ_t *ڳنڍ);


#وصف_ڪريو ngx_rbt_red(ڳنڍ)               ((ڳنڍ)->color = 1)
#وصف_ڪريو ngx_rbt_black(ڳنڍ)             ((ڳنڍ)->color = 0)
#وصف_ڪريو ngx_rbt_is_red(ڳنڍ)            ((ڳنڍ)->color)
#وصف_ڪريو ngx_rbt_is_black(ڳنڍ)          (!ngx_rbt_is_red(ڳنڍ))
#وصف_ڪريو ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#وصف_ڪريو ngx_rbtree_sentinel_init(ڳنڍ)  ngx_rbt_black(ڳنڍ)


ساكن ngx_inline ngx_rbtree_ڳنڍ_t *
ngx_rbtree_min(ngx_rbtree_ڳنڍ_t *ڳنڍ, ngx_rbtree_ڳنڍ_t *sentinel)
{
    طالما (ڳنڍ->کاٻو != sentinel) {
        ڳنڍ = ڳنڍ->کاٻو;
    }

    أرجع ڳنڍ;
}


#جيڪڏهن_پورو /* _NGX_RBTREE_H_INCLUDED_ */
