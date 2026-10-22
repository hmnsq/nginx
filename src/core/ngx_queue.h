
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NGX_ਕਤਾਰ_H_INCLUDED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ਕਤਾਰ_H_INCLUDED


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ngx_ਕਤਾਰ_s  ngx_ਕਤਾਰ_t;

ਬਣਤਰ ngx_ਕਤਾਰ_s {
    ngx_ਕਤਾਰ_t  *prev;
    ngx_ਕਤਾਰ_t  *ਅਗਲਾ;
};


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->ਅਗਲਾ = q


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ(h)                                                    \
    (h == (h)->prev)


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ(h, x)                                           \
    (x)->ਅਗਲਾ = (h)->ਅਗਲਾ;                                                    \
    (x)->ਅਗਲਾ->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->ਅਗਲਾ = x


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_ਪਾਓ_after   ngx_ਕਤਾਰ_ਪਾਓ_ਸਿਰ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_ਪਾਓ_ਪੂਛ(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->ਅਗਲਾ = x;                                                      \
    (x)->ਅਗਲਾ = h;                                                            \
    (h)->prev = x


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_ਪਾਓ_before   ngx_ਕਤਾਰ_ਪਾਓ_ਪੂਛ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_ਸਿਰ(h)                                                     \
    (h)->ਅਗਲਾ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_ਆਖਰੀ(h)                                                     \
    (h)->prev


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_sentinel(h)                                                 \
    (h)


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_ਅਗਲਾ(q)                                                     \
    (q)->ਅਗਲਾ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_prev(q)                                                     \
    (q)->prev


#ਜੇ (NGX_DEBUG)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_remove(x)                                                   \
    (x)->ਅਗਲਾ->prev = (x)->prev;                                              \
    (x)->prev->ਅਗਲਾ = (x)->ਅਗਲਾ;                                              \
    (x)->prev = ਨਿਰਾ;                                                         \
    (x)->ਅਗਲਾ = ਨਿਰਾ

#ਨਹੀਂਤਾਂ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_remove(x)                                                   \
    (x)->ਅਗਲਾ->prev = (x)->prev;                                              \
    (x)->prev->ਅਗਲਾ = (x)->ਅਗਲਾ

#ਜੇ_ਅੰਤ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_ਵੰਡੋ(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->ਅਗਲਾ = n;                                                      \
    (n)->ਅਗਲਾ = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->ਅਗਲਾ = h;                                                      \
    (q)->prev = n;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_ਜੋੜੋ(h, n)                                                   \
    (h)->prev->ਅਗਲਾ = (n)->ਅਗਲਾ;                                              \
    (n)->ਅਗਲਾ->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->ਅਗਲਾ = h;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਕਤਾਰ_ਅੰਕੜੇ(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))


ngx_ਕਤਾਰ_t *ngx_ਕਤਾਰ_ਵਿਚਕਾਰ(ngx_ਕਤਾਰ_t *ਕਤਾਰ);
ਖਾਲੀ ngx_ਕਤਾਰ_ਛਾਂਟੋ(ngx_ਕਤਾਰ_t *ਕਤਾਰ,
    ngx_int_t (*cmp)(ਅਚਲ ngx_ਕਤਾਰ_t *, ਅਚਲ ngx_ਕਤਾਰ_t *));


#ਜੇ_ਅੰਤ /* _NGX_QUEUE_H_INCLUDED_ */
