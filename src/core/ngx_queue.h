
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#جے_بنا_تعریف NGX_قطار_H_INCLUDED
#تعریف_کرو NGX_قطار_H_INCLUDED


تعريفنوع بنية ngx_قطار_s  ngx_قطار_t;

بنية ngx_قطار_s {
    ngx_قطار_t  *prev;
    ngx_قطار_t  *اگلا;
};


#تعریف_کرو ngx_قطار_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->اگلا = q


#تعریف_کرو ngx_قطار_خالی_تھاواں(h)                                                    \
    (h == (h)->prev)


#تعریف_کرو ngx_قطار_داخل_کرو_سر(h, x)                                           \
    (x)->اگلا = (h)->اگلا;                                                    \
    (x)->اگلا->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->اگلا = x


#تعریف_کرو ngx_قطار_داخل_کرو_after   ngx_قطار_داخل_کرو_سر


#تعریف_کرو ngx_قطار_داخل_کرو_پوچھ(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->اگلا = x;                                                      \
    (x)->اگلا = h;                                                            \
    (h)->prev = x


#تعریف_کرو ngx_قطار_داخل_کرو_before   ngx_قطار_داخل_کرو_پوچھ


#تعریف_کرو ngx_قطار_سر(h)                                                     \
    (h)->اگلا


#تعریف_کرو ngx_قطار_آخری(h)                                                     \
    (h)->prev


#تعریف_کرو ngx_قطار_sentinel(h)                                                 \
    (h)


#تعریف_کرو ngx_قطار_اگلا(q)                                                     \
    (q)->اگلا


#تعریف_کرو ngx_قطار_prev(q)                                                     \
    (q)->prev


#إذا (NGX_DEBUG)

#تعریف_کرو ngx_قطار_remove(x)                                                   \
    (x)->اگلا->prev = (x)->prev;                                              \
    (x)->prev->اگلا = (x)->اگلا;                                              \
    (x)->prev = عدم;                                                         \
    (x)->اگلا = عدم

#وإلا

#تعریف_کرو ngx_قطار_remove(x)                                                   \
    (x)->اگلا->prev = (x)->prev;                                              \
    (x)->prev->اگلا = (x)->اگلا

#جے_انت


#تعریف_کرو ngx_قطار_ونڈو(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->اگلا = n;                                                      \
    (n)->اگلا = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->اگلا = h;                                                      \
    (q)->prev = n;


#تعریف_کرو ngx_قطار_جوڑو(h, n)                                                   \
    (h)->prev->اگلا = (n)->اگلا;                                              \
    (n)->اگلا->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->اگلا = h;


#تعریف_کرو ngx_قطار_انکڑے(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))


ngx_قطار_t *ngx_قطار_درمیان(ngx_قطار_t *قطار);
فراغ ngx_قطار_ترتیب_دیو(ngx_قطار_t *قطار,
    ngx_int_t (*cmp)(ثابت ngx_قطار_t *, ثابت ngx_قطار_t *));


#جے_انت /* _NGX_QUEUE_H_INCLUDED_ */
