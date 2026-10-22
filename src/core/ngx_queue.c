
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर शून्य ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(ध्रुव ngx_queue_t *, ध्रुव ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_मध्यः(ngx_queue_t *queue)
{
    ngx_queue_t  *मध्यः, *next;

    मध्यः = ngx_queue_head(queue);

    सचे (मध्यः == ngx_queue_last(queue)) {
        पटिनिवत्त मध्यः;
    }

    next = ngx_queue_head(queue);

    अत्थाय ( ;; ) {
        मध्यः = ngx_queue_next(मध्यः);

        next = ngx_queue_next(next);

        सचे (next == ngx_queue_last(queue)) {
            पटिनिवत्त मध्यः;
        }

        next = ngx_queue_next(next);

        सचे (next == ngx_queue_last(queue)) {
            पटिनिवत्त मध्यः;
        }
    }
}


/* the stable merge sort */

शून्य
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(ध्रुव ngx_queue_t *, ध्रुव ngx_queue_t *))
{
    ngx_queue_t  *q, tail;

    q = ngx_queue_head(queue);

    सचे (q == ngx_queue_last(queue)) {
        पटिनिवत्त;
    }

    q = ngx_queue_मध्यः(queue);

    ngx_queue_split(queue, q, &tail);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&tail, cmp);

    ngx_queue_merge(queue, &tail, cmp);
}


स्थिर शून्य
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(ध्रुव ngx_queue_t *, ध्रुव ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_head(queue);
    q2 = ngx_queue_head(tail);

    अत्थाय ( ;; ) {
        सचे (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, tail);
            भङ्ग;
        }

        सचे (q2 == ngx_queue_sentinel(tail)) {
            भङ्ग;
        }

        सचे (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_next(q1);
            अनुवर्त;
        }

        ngx_queue_remove(q2);
        ngx_queue_निवेशय_before(q1, q2);

        q2 = ngx_queue_head(tail);
    }
}
