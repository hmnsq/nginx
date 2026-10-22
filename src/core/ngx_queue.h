
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#که_نه_تعریف_شوی NGX_کتار_H_INCLUDED
#تعریف_کړه NGX_کتار_H_INCLUDED


تعريفنوع بنية ngx_کتار_s  ngx_کتار_t;

بنية ngx_کتار_s {
    ngx_کتار_t  *prev;
    ngx_کتار_t  *بل;
};


#تعریف_کړه ngx_کتار_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->بل = q


#تعریف_کړه ngx_کتار_تشه(h)                                                    \
    (h == (h)->prev)


#تعریف_کړه ngx_کتار_ورننباسه_سر(h, x)                                           \
    (x)->بل = (h)->بل;                                                    \
    (x)->بل->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->بل = x


#تعریف_کړه ngx_کتار_ورننباسه_after   ngx_کتار_ورننباسه_سر


#تعریف_کړه ngx_کتار_ورننباسه_لکۍ(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->بل = x;                                                      \
    (x)->بل = h;                                                            \
    (h)->prev = x


#تعریف_کړه ngx_کتار_ورننباسه_before   ngx_کتار_ورننباسه_لکۍ


#تعریف_کړه ngx_کتار_سر(h)                                                     \
    (h)->بل


#تعریف_کړه ngx_کتار_وروستی(h)                                                     \
    (h)->prev


#تعریف_کړه ngx_کتار_sentinel(h)                                                 \
    (h)


#تعریف_کړه ngx_کتار_بل(q)                                                     \
    (q)->بل


#تعریف_کړه ngx_کتار_prev(q)                                                     \
    (q)->prev


#إذا (NGX_DEBUG)

#تعریف_کړه ngx_کتار_remove(x)                                                   \
    (x)->بل->prev = (x)->prev;                                              \
    (x)->prev->بل = (x)->بل;                                              \
    (x)->prev = عدم;                                                         \
    (x)->بل = عدم

#وإلا

#تعریف_کړه ngx_کتار_remove(x)                                                   \
    (x)->بل->prev = (x)->prev;                                              \
    (x)->prev->بل = (x)->بل

#د_که_پای


#تعریف_کړه ngx_کتار_وېشل(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->بل = n;                                                      \
    (n)->بل = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->بل = h;                                                      \
    (q)->prev = n;


#تعریف_کړه ngx_کتار_جمع_کړه(h, n)                                                   \
    (h)->prev->بل = (n)->بل;                                              \
    (n)->بل->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->بل = h;


#تعریف_کړه ngx_کتار_معلومات(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))


ngx_کتار_t *ngx_کتار_منځ(ngx_کتار_t *کتار);
فراغ ngx_کتار_ترتیبول(ngx_کتار_t *کتار,
    ngx_int_t (*cmp)(ثابت ngx_کتار_t *, ثابت ngx_کتار_t *));


#د_که_پای /* _NGX_QUEUE_H_INCLUDED_ */
