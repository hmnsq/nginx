
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


static void ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(const ngx_queue_t *, const ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_middle(ngx_queue_t *queue)
{
    ngx_queue_t  *middle, *next;

    middle = ngx_queue_head(queue);

    如果 (middle == ngx_queue_last(queue)) {
        返回 middle;
    }

    next = ngx_queue_head(queue);

    为 ( ;; ) {
        middle = ngx_queue_next(middle);

        next = ngx_queue_next(next);

        如果 (next == ngx_queue_last(queue)) {
            返回 middle;
        }

        next = ngx_queue_next(next);

        如果 (next == ngx_queue_last(queue)) {
            返回 middle;
        }
    }
}


/* the stable merge sort */

void
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(const ngx_queue_t *, const ngx_queue_t *))
{
    ngx_queue_t  *q, tail;

    q = ngx_queue_head(queue);

    如果 (q == ngx_queue_last(queue)) {
        返回;
    }

    q = ngx_queue_middle(queue);

    ngx_queue_split(queue, q, &tail);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&tail, cmp);

    ngx_queue_merge(queue, &tail, cmp);
}


static void
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(const ngx_queue_t *, const ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_head(queue);
    q2 = ngx_queue_head(tail);

    为 ( ;; ) {
        如果 (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, tail);
            break;
        }

        如果 (q2 == ngx_queue_sentinel(tail)) {
            break;
        }

        如果 (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_next(q1);
            continue;
        }

        ngx_queue_remove(q2);
        ngx_queue_insert_before(q1, q2);

        q2 = ngx_queue_head(tail);
    }
}
