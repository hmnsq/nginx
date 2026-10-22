
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ସ୍ଥିର ଶୂନ୍ୟ ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(ଅଚଳ ngx_queue_t *, ଅଚଳ ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_ମଧ୍ୟ(ngx_queue_t *queue)
{
    ngx_queue_t  *ମଧ୍ୟ, *next;

    ମଧ୍ୟ = ngx_queue_head(queue);

    ଯଦି (ମଧ୍ୟ == ngx_queue_last(queue)) {
        ଫେରନ୍ତୁ ମଧ୍ୟ;
    }

    next = ngx_queue_head(queue);

    ପାଇଁ ( ;; ) {
        ମଧ୍ୟ = ngx_queue_next(ମଧ୍ୟ);

        next = ngx_queue_next(next);

        ଯଦି (next == ngx_queue_last(queue)) {
            ଫେରନ୍ତୁ ମଧ୍ୟ;
        }

        next = ngx_queue_next(next);

        ଯଦି (next == ngx_queue_last(queue)) {
            ଫେରନ୍ତୁ ମଧ୍ୟ;
        }
    }
}


/* the stable merge sort */

ଶୂନ୍ୟ
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(ଅଚଳ ngx_queue_t *, ଅଚଳ ngx_queue_t *))
{
    ngx_queue_t  *q, tail;

    q = ngx_queue_head(queue);

    ଯଦି (q == ngx_queue_last(queue)) {
        ଫେରନ୍ତୁ;
    }

    q = ngx_queue_ମଧ୍ୟ(queue);

    ngx_queue_split(queue, q, &tail);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&tail, cmp);

    ngx_queue_merge(queue, &tail, cmp);
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(ଅଚଳ ngx_queue_t *, ଅଚଳ ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_head(queue);
    q2 = ngx_queue_head(tail);

    ପାଇଁ ( ;; ) {
        ଯଦି (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, tail);
            ଭାଙ୍ଗ;
        }

        ଯଦି (q2 == ngx_queue_sentinel(tail)) {
            ଭାଙ୍ଗ;
        }

        ଯଦି (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_next(q1);
            ଜାରି;
        }

        ngx_queue_remove(q2);
        ngx_queue_ଭର୍ତ୍ତି_before(q1, q2);

        q2 = ngx_queue_head(tail);
    }
}
