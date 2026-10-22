
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


#ifndef _NGX_QUEUE_H_INCLUDED_
#define _NGX_QUEUE_H_INCLUDED_


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ngx_queue_s  ngx_queue_t;

ସଂରଚନା ngx_queue_s {
    ngx_queue_t  *prev;
    ngx_queue_t  *next;
};


#define ngx_queue_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->next = q


#define ngx_queue_empty(h)                                                    \
    (h == (h)->prev)


#define ngx_queue_ଭର୍ତ୍ତି_head(h, x)                                           \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x


#define ngx_queue_ଭର୍ତ୍ତି_after   ngx_queue_ଭର୍ତ୍ତି_head


#define ngx_queue_ଭର୍ତ୍ତି_tail(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x


#define ngx_queue_ଭର୍ତ୍ତି_before   ngx_queue_ଭର୍ତ୍ତି_tail


#define ngx_queue_head(h)                                                     \
    (h)->next


#define ngx_queue_last(h)                                                     \
    (h)->prev


#define ngx_queue_sentinel(h)                                                 \
    (h)


#define ngx_queue_next(q)                                                     \
    (q)->next


#define ngx_queue_prev(q)                                                     \
    (q)->prev


#ଯଦି (NGX_DEBUG)

#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
    (x)->prev = ଖାଲି;                                                         \
    (x)->next = ଖାଲି

#ଅନ୍ୟଥା

#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next

#endif


#define ngx_queue_split(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;


#define ngx_queue_add(h, n)                                                   \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;


#define ngx_queue_data(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))


ngx_queue_t *ngx_queue_ମଧ୍ୟ(ngx_queue_t *queue);
ଶୂନ୍ୟ ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(ଅଚଳ ngx_queue_t *, ଅଚଳ ngx_queue_t *));


#endif /* _NGX_QUEUE_H_INCLUDED_ */
