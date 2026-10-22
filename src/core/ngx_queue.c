
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ ಶೂನ್ಯ ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(ಧ್ರುವ ngx_queue_t *, ಧ್ರುವ ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_ಮಧ್ಯ(ngx_queue_t *queue)
{
    ngx_queue_t  *ಮಧ್ಯ, *next;

    ಮಧ್ಯ = ngx_queue_head(queue);

    ಯದಿ (ಮಧ್ಯ == ngx_queue_last(queue)) {
        ಪ್ರತ್ಯಾಗಮ ಮಧ್ಯ;
    }

    next = ngx_queue_head(queue);

    ಪ್ರತಿ ( ;; ) {
        ಮಧ್ಯ = ngx_queue_next(ಮಧ್ಯ);

        next = ngx_queue_next(next);

        ಯದಿ (next == ngx_queue_last(queue)) {
            ಪ್ರತ್ಯಾಗಮ ಮಧ್ಯ;
        }

        next = ngx_queue_next(next);

        ಯದಿ (next == ngx_queue_last(queue)) {
            ಪ್ರತ್ಯಾಗಮ ಮಧ್ಯ;
        }
    }
}


/* the stable merge sort */

ಶೂನ್ಯ
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(ಧ್ರುವ ngx_queue_t *, ಧ್ರುವ ngx_queue_t *))
{
    ngx_queue_t  *q, tail;

    q = ngx_queue_head(queue);

    ಯದಿ (q == ngx_queue_last(queue)) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    q = ngx_queue_ಮಧ್ಯ(queue);

    ngx_queue_split(queue, q, &tail);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&tail, cmp);

    ngx_queue_merge(queue, &tail, cmp);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(ಧ್ರುವ ngx_queue_t *, ಧ್ರುವ ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_head(queue);
    q2 = ngx_queue_head(tail);

    ಪ್ರತಿ ( ;; ) {
        ಯದಿ (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, tail);
            ಭಙ್ಗ;
        }

        ಯದಿ (q2 == ngx_queue_sentinel(tail)) {
            ಭಙ್ಗ;
        }

        ಯದಿ (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_next(q1);
            ಅನುವರ್ತ;
        }

        ngx_queue_remove(q2);
        ngx_queue_ಸೇರಿಸು_before(q1, q2);

        q2 = ngx_queue_head(tail);
    }
}
