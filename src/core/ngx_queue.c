
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


સ્થિર શૂન્ય ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(અચલ ngx_queue_t *, અચલ ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_મધ્ય(ngx_queue_t *queue)
{
    ngx_queue_t  *મધ્ય, *next;

    મધ્ય = ngx_queue_head(queue);

    જો (મધ્ય == ngx_queue_last(queue)) {
        પાછા મધ્ય;
    }

    next = ngx_queue_head(queue);

    માટે ( ;; ) {
        મધ્ય = ngx_queue_next(મધ્ય);

        next = ngx_queue_next(next);

        જો (next == ngx_queue_last(queue)) {
            પાછા મધ્ય;
        }

        next = ngx_queue_next(next);

        જો (next == ngx_queue_last(queue)) {
            પાછા મધ્ય;
        }
    }
}


/* the stable merge sort */

શૂન્ય
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(અચલ ngx_queue_t *, અચલ ngx_queue_t *))
{
    ngx_queue_t  *q, tail;

    q = ngx_queue_head(queue);

    જો (q == ngx_queue_last(queue)) {
        પાછા;
    }

    q = ngx_queue_મધ્ય(queue);

    ngx_queue_split(queue, q, &tail);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&tail, cmp);

    ngx_queue_merge(queue, &tail, cmp);
}


સ્થિર શૂન્ય
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(અચલ ngx_queue_t *, અચલ ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_head(queue);
    q2 = ngx_queue_head(tail);

    માટે ( ;; ) {
        જો (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, tail);
            તોડો;
        }

        જો (q2 == ngx_queue_sentinel(tail)) {
            તોડો;
        }

        જો (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_next(q1);
            ચાલુ;
        }

        ngx_queue_remove(q2);
        ngx_queue_ઉમેરો_before(q1, q2);

        q2 = ngx_queue_head(tail);
    }
}
