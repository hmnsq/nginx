
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
ngx_queue_मधला(ngx_queue_t *queue)
{
    ngx_queue_t  *मधला, *next;

    मधला = ngx_queue_head(queue);

    यदि (मधला == ngx_queue_last(queue)) {
        प्रत्यागम मधला;
    }

    next = ngx_queue_head(queue);

    प्रति ( ;; ) {
        मधला = ngx_queue_next(मधला);

        next = ngx_queue_next(next);

        यदि (next == ngx_queue_last(queue)) {
            प्रत्यागम मधला;
        }

        next = ngx_queue_next(next);

        यदि (next == ngx_queue_last(queue)) {
            प्रत्यागम मधला;
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

    यदि (q == ngx_queue_last(queue)) {
        प्रत्यागम;
    }

    q = ngx_queue_मधला(queue);

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

    प्रति ( ;; ) {
        यदि (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, tail);
            भङ्ग;
        }

        यदि (q2 == ngx_queue_sentinel(tail)) {
            भङ्ग;
        }

        यदि (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_next(q1);
            अनुवर्त;
        }

        ngx_queue_remove(q2);
        ngx_queue_घाला_before(q1, q2);

        q2 = ngx_queue_head(tail);
    }
}
