
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_RBTREE_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_RBTREE_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ngx_uint_t  ngx_rbtree_ਕੁੰਜੀ_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ngx_int_t   ngx_rbtree_ਕੁੰਜੀ_int_t;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_rbtree_ਗੰਢ_s  ngx_rbtree_ਗੰਢ_t;

ਬਣਤਰ ngx_rbtree_ਗੰਢ_s {
    ngx_rbtree_ਕੁੰਜੀ_t       ਕੁੰਜੀ;
    ngx_rbtree_ਗੰਢ_t     *ਖੱਬਾ;
    ngx_rbtree_ਗੰਢ_t     *ਸੱਜਾ;
    ngx_rbtree_ਗੰਢ_t     *ਮਾਪਾ;
    u_char                 color;
    u_char                 ਅੰਕੜੇ;
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_rbtree_s  ngx_rbtree_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ngx_rbtree_ਪਾਓ_pt) (ngx_rbtree_ਗੰਢ_t *ਜੜ੍ਹ,
    ngx_rbtree_ਗੰਢ_t *ਗੰਢ, ngx_rbtree_ਗੰਢ_t *sentinel);

ਬਣਤਰ ngx_rbtree_s {
    ngx_rbtree_ਗੰਢ_t     *ਜੜ੍ਹ;
    ngx_rbtree_ਗੰਢ_t     *sentinel;
    ngx_rbtree_ਪਾਓ_pt   ਪਾਓ;
};


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_rbtree_init(ਰੁੱਖ, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (ਰੁੱਖ)->ਜੜ੍ਹ = s;                                                         \
    (ਰੁੱਖ)->sentinel = s;                                                     \
    (ਰੁੱਖ)->ਪਾਓ = i

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_rbtree_ਅੰਕੜੇ(ਗੰਢ, type, link)                                     \
    (type *) ((u_char *) (ਗੰਢ) - offsetof(type, link))


ਖਾਲੀ ngx_rbtree_ਪਾਓ(ngx_rbtree_t *ਰੁੱਖ, ngx_rbtree_ਗੰਢ_t *ਗੰਢ);
ਖਾਲੀ ngx_rbtree_delete(ngx_rbtree_t *ਰੁੱਖ, ngx_rbtree_ਗੰਢ_t *ਗੰਢ);
ਖਾਲੀ ngx_rbtree_ਪਾਓ_ਮੁੱਲ(ngx_rbtree_ਗੰਢ_t *ਜੜ੍ਹ, ngx_rbtree_ਗੰਢ_t *ਗੰਢ,
    ngx_rbtree_ਗੰਢ_t *sentinel);
ਖਾਲੀ ngx_rbtree_ਪਾਓ_timer_ਮੁੱਲ(ngx_rbtree_ਗੰਢ_t *ਜੜ੍ਹ,
    ngx_rbtree_ਗੰਢ_t *ਗੰਢ, ngx_rbtree_ਗੰਢ_t *sentinel);
ngx_rbtree_ਗੰਢ_t *ngx_rbtree_ਅਗਲਾ(ngx_rbtree_t *ਰੁੱਖ,
    ngx_rbtree_ਗੰਢ_t *ਗੰਢ);


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_rbt_red(ਗੰਢ)               ((ਗੰਢ)->color = 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_rbt_black(ਗੰਢ)             ((ਗੰਢ)->color = 0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_rbt_is_red(ਗੰਢ)            ((ਗੰਢ)->color)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_rbt_is_black(ਗੰਢ)          (!ngx_rbt_is_red(ਗੰਢ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_rbtree_sentinel_init(ਗੰਢ)  ngx_rbt_black(ਗੰਢ)


ਸਥਿਰ ngx_inline ngx_rbtree_ਗੰਢ_t *
ngx_rbtree_min(ngx_rbtree_ਗੰਢ_t *ਗੰਢ, ngx_rbtree_ਗੰਢ_t *sentinel)
{
    ਜਦੋਂ (ਗੰਢ->ਖੱਬਾ != sentinel) {
        ਗੰਢ = ਗੰਢ->ਖੱਬਾ;
    }

    ਵਾਪਸ ਗੰਢ;
}


#ਜੇ_ਅੰਤ /* _NGX_RBTREE_H_INCLUDED_ */
