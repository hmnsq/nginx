
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#اگر_تعریف_نشده NGX_صف_H_INCLUDED
#تعریف_کن NGX_صف_H_INCLUDED


تعريفنوع بنية ngx_صف_s  ngx_صف_t;

بنية ngx_صف_s {
    ngx_صف_t  *prev;
    ngx_صف_t  *بعدی;
};


#تعریف_کن ngx_صف_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->بعدی = q


#تعریف_کن ngx_صف_خالی(h)                                                    \
    (h == (h)->prev)


#تعریف_کن ngx_صف_درج_سر(h, x)                                           \
    (x)->بعدی = (h)->بعدی;                                                    \
    (x)->بعدی->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->بعدی = x


#تعریف_کن ngx_صف_درج_after   ngx_صف_درج_سر


#تعریف_کن ngx_صف_درج_دم(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->بعدی = x;                                                      \
    (x)->بعدی = h;                                                            \
    (h)->prev = x


#تعریف_کن ngx_صف_درج_before   ngx_صف_درج_دم


#تعریف_کن ngx_صف_سر(h)                                                     \
    (h)->بعدی


#تعریف_کن ngx_صف_آخرین(h)                                                     \
    (h)->prev


#تعریف_کن ngx_صف_sentinel(h)                                                 \
    (h)


#تعریف_کن ngx_صف_بعدی(q)                                                     \
    (q)->بعدی


#تعریف_کن ngx_صف_prev(q)                                                     \
    (q)->prev


#إذا (NGX_DEBUG)

#تعریف_کن ngx_صف_remove(x)                                                   \
    (x)->بعدی->prev = (x)->prev;                                              \
    (x)->prev->بعدی = (x)->بعدی;                                              \
    (x)->prev = عدم;                                                         \
    (x)->بعدی = عدم

#وإلا

#تعریف_کن ngx_صف_remove(x)                                                   \
    (x)->بعدی->prev = (x)->prev;                                              \
    (x)->prev->بعدی = (x)->بعدی

#پایان_اگر


#تعریف_کن ngx_صف_تقسیم(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->بعدی = n;                                                      \
    (n)->بعدی = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->بعدی = h;                                                      \
    (q)->prev = n;


#تعریف_کن ngx_صف_جمع_کن(h, n)                                                   \
    (h)->prev->بعدی = (n)->بعدی;                                              \
    (n)->بعدی->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->بعدی = h;


#تعریف_کن ngx_صف_داده(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))


ngx_صف_t *ngx_صف_میانه(ngx_صف_t *صف);
فراغ ngx_صف_مرتب_سازی(ngx_صف_t *صف,
    ngx_int_t (*cmp)(ثابت ngx_صف_t *, ثابت ngx_صف_t *));


#پایان_اگر /* _NGX_QUEUE_H_INCLUDED_ */
