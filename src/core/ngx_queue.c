
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ایستا تهی ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(ثابت ngx_queue_t *, ثابت ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_میانه(ngx_queue_t *queue)
{
    ngx_queue_t  *میانه, *next;

    میانه = ngx_queue_head(queue);

    اگر (میانه == ngx_queue_last(queue)) {
        بازگشت میانه;
    }

    next = ngx_queue_head(queue);

    برای ( ;; ) {
        میانه = ngx_queue_next(میانه);

        next = ngx_queue_next(next);

        اگر (next == ngx_queue_last(queue)) {
            بازگشت میانه;
        }

        next = ngx_queue_next(next);

        اگر (next == ngx_queue_last(queue)) {
            بازگشت میانه;
        }
    }
}


/* the stable merge sort */

تهی
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(ثابت ngx_queue_t *, ثابت ngx_queue_t *))
{
    ngx_queue_t  *q, tail;

    q = ngx_queue_head(queue);

    اگر (q == ngx_queue_last(queue)) {
        بازگشت;
    }

    q = ngx_queue_میانه(queue);

    ngx_queue_split(queue, q, &tail);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&tail, cmp);

    ngx_queue_merge(queue, &tail, cmp);
}


ایستا تهی
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(ثابت ngx_queue_t *, ثابت ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_head(queue);
    q2 = ngx_queue_head(tail);

    برای ( ;; ) {
        اگر (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, tail);
            بشکن;
        }

        اگر (q2 == ngx_queue_sentinel(tail)) {
            بشکن;
        }

        اگر (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_next(q1);
            ادامه;
        }

        ngx_queue_remove(q2);
        ngx_queue_درج_before(q1, q2);

        q2 = ngx_queue_head(tail);
    }
}
