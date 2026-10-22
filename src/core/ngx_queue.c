
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर शून्य ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(अचर ngx_queue_t *, अचर ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_बीच(ngx_queue_t *queue)
{
    ngx_queue_t  *बीच, *next;

    बीच = ngx_queue_head(queue);

    यदि (बीच == ngx_queue_last(queue)) {
        फर्काऊ बीच;
    }

    next = ngx_queue_head(queue);

    लागि ( ;; ) {
        बीच = ngx_queue_next(बीच);

        next = ngx_queue_next(next);

        यदि (next == ngx_queue_last(queue)) {
            फर्काऊ बीच;
        }

        next = ngx_queue_next(next);

        यदि (next == ngx_queue_last(queue)) {
            फर्काऊ बीच;
        }
    }
}


/* the stable merge sort */

शून्य
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(अचर ngx_queue_t *, अचर ngx_queue_t *))
{
    ngx_queue_t  *q, tail;

    q = ngx_queue_head(queue);

    यदि (q == ngx_queue_last(queue)) {
        फर्काऊ;
    }

    q = ngx_queue_बीच(queue);

    ngx_queue_split(queue, q, &tail);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&tail, cmp);

    ngx_queue_merge(queue, &tail, cmp);
}


स्थिर शून्य
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(अचर ngx_queue_t *, अचर ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_head(queue);
    q2 = ngx_queue_head(tail);

    लागि ( ;; ) {
        यदि (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, tail);
            रोक;
        }

        यदि (q2 == ngx_queue_sentinel(tail)) {
            रोक;
        }

        यदि (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_next(q1);
            जारी;
        }

        ngx_queue_remove(q2);
        ngx_queue_घुसाउ_before(q1, q2);

        q2 = ngx_queue_head(tail);
    }
}
