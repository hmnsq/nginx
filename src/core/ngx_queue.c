
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن فراغ ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(ثابت ngx_queue_t *, ثابت ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_درمیان(ngx_queue_t *queue)
{
    ngx_queue_t  *درمیان, *next;

    درمیان = ngx_queue_head(queue);

    إذا (درمیان == ngx_queue_last(queue)) {
        أرجع درمیان;
    }

    next = ngx_queue_head(queue);

    لكل ( ;; ) {
        درمیان = ngx_queue_next(درمیان);

        next = ngx_queue_next(next);

        إذا (next == ngx_queue_last(queue)) {
            أرجع درمیان;
        }

        next = ngx_queue_next(next);

        إذا (next == ngx_queue_last(queue)) {
            أرجع درمیان;
        }
    }
}


/* the stable merge sort */

فراغ
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(ثابت ngx_queue_t *, ثابت ngx_queue_t *))
{
    ngx_queue_t  *q, tail;

    q = ngx_queue_head(queue);

    إذا (q == ngx_queue_last(queue)) {
        أرجع;
    }

    q = ngx_queue_درمیان(queue);

    ngx_queue_split(queue, q, &tail);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&tail, cmp);

    ngx_queue_merge(queue, &tail, cmp);
}


ساكن فراغ
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(ثابت ngx_queue_t *, ثابت ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_head(queue);
    q2 = ngx_queue_head(tail);

    لكل ( ;; ) {
        إذا (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, tail);
            اكسر;
        }

        إذا (q2 == ngx_queue_sentinel(tail)) {
            اكسر;
        }

        إذا (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_next(q1);
            استمر;
        }

        ngx_queue_remove(q2);
        ngx_queue_داخل_کرو_before(q1, q2);

        q2 = ngx_queue_head(tail);
    }
}
