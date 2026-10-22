
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


സ്ഥിതം ശൂന്യം ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(സ്ഥിരം ngx_queue_t *, സ്ഥിരം ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_നടുവ്(ngx_queue_t *queue)
{
    ngx_queue_t  *നടുവ്, *next;

    നടുവ് = ngx_queue_head(queue);

    എങ്കിൽ (നടുവ് == ngx_queue_last(queue)) {
        തിരികെ നടുവ്;
    }

    next = ngx_queue_head(queue);

    വേണ്ടി ( ;; ) {
        നടുവ് = ngx_queue_next(നടുവ്);

        next = ngx_queue_next(next);

        എങ്കിൽ (next == ngx_queue_last(queue)) {
            തിരികെ നടുവ്;
        }

        next = ngx_queue_next(next);

        എങ്കിൽ (next == ngx_queue_last(queue)) {
            തിരികെ നടുവ്;
        }
    }
}


/* the stable merge sort */

ശൂന്യം
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(സ്ഥിരം ngx_queue_t *, സ്ഥിരം ngx_queue_t *))
{
    ngx_queue_t  *q, tail;

    q = ngx_queue_head(queue);

    എങ്കിൽ (q == ngx_queue_last(queue)) {
        തിരികെ;
    }

    q = ngx_queue_നടുവ്(queue);

    ngx_queue_split(queue, q, &tail);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&tail, cmp);

    ngx_queue_merge(queue, &tail, cmp);
}


സ്ഥിതം ശൂന്യം
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(സ്ഥിരം ngx_queue_t *, സ്ഥിരം ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_head(queue);
    q2 = ngx_queue_head(tail);

    വേണ്ടി ( ;; ) {
        എങ്കിൽ (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, tail);
            നിർത്തുക;
        }

        എങ്കിൽ (q2 == ngx_queue_sentinel(tail)) {
            നിർത്തുക;
        }

        എങ്കിൽ (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_next(q1);
            തുടരുക;
        }

        ngx_queue_remove(q2);
        ngx_queue_തിരുകുക_before(q1, q2);

        q2 = ngx_queue_head(tail);
    }
}
