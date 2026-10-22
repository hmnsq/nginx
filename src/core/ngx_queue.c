
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


நிலையான வெறுமை ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(மாறிலி ngx_queue_t *, மாறிலி ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_நடு(ngx_queue_t *queue)
{
    ngx_queue_t  *நடு, *next;

    நடு = ngx_queue_head(queue);

    எனில் (நடு == ngx_queue_last(queue)) {
        திரும்பு நடு;
    }

    next = ngx_queue_head(queue);

    க்கு ( ;; ) {
        நடு = ngx_queue_next(நடு);

        next = ngx_queue_next(next);

        எனில் (next == ngx_queue_last(queue)) {
            திரும்பு நடு;
        }

        next = ngx_queue_next(next);

        எனில் (next == ngx_queue_last(queue)) {
            திரும்பு நடு;
        }
    }
}


/* the stable merge sort */

வெறுமை
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(மாறிலி ngx_queue_t *, மாறிலி ngx_queue_t *))
{
    ngx_queue_t  *q, tail;

    q = ngx_queue_head(queue);

    எனில் (q == ngx_queue_last(queue)) {
        திரும்பு;
    }

    q = ngx_queue_நடு(queue);

    ngx_queue_split(queue, q, &tail);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&tail, cmp);

    ngx_queue_merge(queue, &tail, cmp);
}


நிலையான வெறுமை
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(மாறிலி ngx_queue_t *, மாறிலி ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_head(queue);
    q2 = ngx_queue_head(tail);

    க்கு ( ;; ) {
        எனில் (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, tail);
            நிறுத்து;
        }

        எனில் (q2 == ngx_queue_sentinel(tail)) {
            நிறுத்து;
        }

        எனில் (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_next(q1);
            தொடர்;
        }

        ngx_queue_remove(q2);
        ngx_queue_செருகு_before(q1, q2);

        q2 = ngx_queue_head(tail);
    }
}
