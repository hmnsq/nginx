
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#جيڪڏهن_وصف_نه_ٿيل NGX_قطار_H_INCLUDED
#وصف_ڪريو NGX_قطار_H_INCLUDED


تعريفنوع بنية ngx_قطار_s  ngx_قطار_t;

بنية ngx_قطار_s {
    ngx_قطار_t  *prev;
    ngx_قطار_t  *ايندڙ;
};


#وصف_ڪريو ngx_قطار_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->ايندڙ = q


#وصف_ڪريو ngx_قطار_خالي_جايون(h)                                                    \
    (h == (h)->prev)


#وصف_ڪريو ngx_قطار_وجهو_سر(h, x)                                           \
    (x)->ايندڙ = (h)->ايندڙ;                                                    \
    (x)->ايندڙ->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->ايندڙ = x


#وصف_ڪريو ngx_قطار_وجهو_after   ngx_قطار_وجهو_سر


#وصف_ڪريو ngx_قطار_وجهو_پڇ(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->ايندڙ = x;                                                      \
    (x)->ايندڙ = h;                                                            \
    (h)->prev = x


#وصف_ڪريو ngx_قطار_وجهو_before   ngx_قطار_وجهو_پڇ


#وصف_ڪريو ngx_قطار_سر(h)                                                     \
    (h)->ايندڙ


#وصف_ڪريو ngx_قطار_آخري(h)                                                     \
    (h)->prev


#وصف_ڪريو ngx_قطار_sentinel(h)                                                 \
    (h)


#وصف_ڪريو ngx_قطار_ايندڙ(q)                                                     \
    (q)->ايندڙ


#وصف_ڪريو ngx_قطار_prev(q)                                                     \
    (q)->prev


#إذا (NGX_DEBUG)

#وصف_ڪريو ngx_قطار_remove(x)                                                   \
    (x)->ايندڙ->prev = (x)->prev;                                              \
    (x)->prev->ايندڙ = (x)->ايندڙ;                                              \
    (x)->prev = عدم;                                                         \
    (x)->ايندڙ = عدم

#وإلا

#وصف_ڪريو ngx_قطار_remove(x)                                                   \
    (x)->ايندڙ->prev = (x)->prev;                                              \
    (x)->prev->ايندڙ = (x)->ايندڙ

#جيڪڏهن_پورو


#وصف_ڪريو ngx_قطار_ورهايو(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->ايندڙ = n;                                                      \
    (n)->ايندڙ = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->ايندڙ = h;                                                      \
    (q)->prev = n;


#وصف_ڪريو ngx_قطار_جوڙيو(h, n)                                                   \
    (h)->prev->ايندڙ = (n)->ايندڙ;                                              \
    (n)->ايندڙ->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->ايندڙ = h;


#وصف_ڪريو ngx_قطار_معلومات(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))


ngx_قطار_t *ngx_قطار_وچ(ngx_قطار_t *قطار);
فراغ ngx_قطار_ترتيب_ڏيو(ngx_قطار_t *قطار,
    ngx_int_t (*cmp)(ثابت ngx_قطار_t *, ثابت ngx_قطار_t *));


#جيڪڏهن_پورو /* _NGX_QUEUE_H_INCLUDED_ */
