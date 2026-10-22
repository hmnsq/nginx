
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


স্থির শূন্য ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(ধ্রুব ngx_queue_t *, ধ্রুব ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_মধ্য(ngx_queue_t *queue)
{
    ngx_queue_t  *মধ্য, *next;

    মধ্য = ngx_queue_head(queue);

    যদি (মধ্য == ngx_queue_last(queue)) {
        প্রত্যাগম মধ্য;
    }

    next = ngx_queue_head(queue);

    প্রতি ( ;; ) {
        মধ্য = ngx_queue_next(মধ্য);

        next = ngx_queue_next(next);

        যদি (next == ngx_queue_last(queue)) {
            প্রত্যাগম মধ্য;
        }

        next = ngx_queue_next(next);

        যদি (next == ngx_queue_last(queue)) {
            প্রত্যাগম মধ্য;
        }
    }
}


/* the stable merge sort */

শূন্য
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(ধ্রুব ngx_queue_t *, ধ্রুব ngx_queue_t *))
{
    ngx_queue_t  *q, tail;

    q = ngx_queue_head(queue);

    যদি (q == ngx_queue_last(queue)) {
        প্রত্যাগম;
    }

    q = ngx_queue_মধ্য(queue);

    ngx_queue_split(queue, q, &tail);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&tail, cmp);

    ngx_queue_merge(queue, &tail, cmp);
}


স্থির শূন্য
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(ধ্রুব ngx_queue_t *, ধ্রুব ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_head(queue);
    q2 = ngx_queue_head(tail);

    প্রতি ( ;; ) {
        যদি (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, tail);
            ভঙ্গ;
        }

        যদি (q2 == ngx_queue_sentinel(tail)) {
            ভঙ্গ;
        }

        যদি (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_next(q1);
            অনুবর্ত;
        }

        ngx_queue_remove(q2);
        ngx_queue_ঢোকাও_before(q1, q2);

        q2 = ngx_queue_head(tail);
    }
}
