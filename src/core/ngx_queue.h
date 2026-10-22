
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


#ifndef _NGX_QUEUE_H_INCLUDED_
#define _NGX_QUEUE_H_INCLUDED_


প্রকারলেখ কাঠামো ngx_queue_s  ngx_queue_t;

কাঠামো ngx_queue_s {
    ngx_queue_t  *পূর্ব;
    ngx_queue_t  *পরবর্তী;
};


#define ngx_queue_init(q)                                                     \
    (q)->পূর্ব = q;                                                            \
    (q)->পরবর্তী = q


#define ngx_queue_empty(h)                                                    \
    (h == (h)->পূর্ব)


#define ngx_queue_ঢোকাও_শীর্ষ(h, x)                                           \
    (x)->পরবর্তী = (h)->পরবর্তী;                                                    \
    (x)->পরবর্তী->পূর্ব = x;                                                      \
    (x)->পূর্ব = h;                                                            \
    (h)->পরবর্তী = x


#define ngx_queue_ঢোকাও_after   ngx_queue_ঢোকাও_শীর্ষ


#define ngx_queue_ঢোকাও_লেজ(h, x)                                           \
    (x)->পূর্ব = (h)->পূর্ব;                                                    \
    (x)->পূর্ব->পরবর্তী = x;                                                      \
    (x)->পরবর্তী = h;                                                            \
    (h)->পূর্ব = x


#define ngx_queue_ঢোকাও_before   ngx_queue_ঢোকাও_লেজ


#define ngx_queue_শীর্ষ(h)                                                     \
    (h)->পরবর্তী


#define ngx_queue_last(h)                                                     \
    (h)->পূর্ব


#define ngx_queue_sentinel(h)                                                 \
    (h)


#define ngx_queue_পরবর্তী(q)                                                     \
    (q)->পরবর্তী


#define ngx_queue_পূর্ব(q)                                                     \
    (q)->পূর্ব


#যদি (NGX_DEBUG)

#define ngx_queue_remove(x)                                                   \
    (x)->পরবর্তী->পূর্ব = (x)->পূর্ব;                                              \
    (x)->পূর্ব->পরবর্তী = (x)->পরবর্তী;                                              \
    (x)->পূর্ব = ফাঁকা;                                                         \
    (x)->পরবর্তী = ফাঁকা

#অন্যথা

#define ngx_queue_remove(x)                                                   \
    (x)->পরবর্তী->পূর্ব = (x)->পূর্ব;                                              \
    (x)->পূর্ব->পরবর্তী = (x)->পরবর্তী

#endif


#define ngx_queue_split(h, q, n)                                              \
    (n)->পূর্ব = (h)->পূর্ব;                                                    \
    (n)->পূর্ব->পরবর্তী = n;                                                      \
    (n)->পরবর্তী = q;                                                            \
    (h)->পূর্ব = (q)->পূর্ব;                                                    \
    (h)->পূর্ব->পরবর্তী = h;                                                      \
    (q)->পূর্ব = n;


#define ngx_queue_add(h, n)                                                   \
    (h)->পূর্ব->পরবর্তী = (n)->পরবর্তী;                                              \
    (n)->পরবর্তী->পূর্ব = (h)->পূর্ব;                                              \
    (h)->পূর্ব = (n)->পূর্ব;                                                    \
    (h)->পূর্ব->পরবর্তী = h;


#define ngx_queue_তথ্য(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))


ngx_queue_t *ngx_queue_মধ্য(ngx_queue_t *queue);
শূন্য ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(ধ্রুবক ngx_queue_t *, ধ্রুবক ngx_queue_t *));


#endif /* _NGX_QUEUE_H_INCLUDED_ */
