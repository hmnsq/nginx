
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर शून्य ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(स्थिरांक ngx_queue_t *, स्थिरांक ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_मधला(ngx_queue_t *queue)
{
    ngx_queue_t  *मधला, *next;

    मधला = ngx_queue_head(queue);

    जर (मधला == ngx_queue_last(queue)) {
        परत मधला;
    }

    next = ngx_queue_head(queue);

    साठी ( ;; ) {
        मधला = ngx_queue_next(मधला);

        next = ngx_queue_next(next);

        जर (next == ngx_queue_last(queue)) {
            परत मधला;
        }

        next = ngx_queue_next(next);

        जर (next == ngx_queue_last(queue)) {
            परत मधला;
        }
    }
}


/* the stable merge sort */

शून्य
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(स्थिरांक ngx_queue_t *, स्थिरांक ngx_queue_t *))
{
    ngx_queue_t  *q, tail;

    q = ngx_queue_head(queue);

    जर (q == ngx_queue_last(queue)) {
        परत;
    }

    q = ngx_queue_मधला(queue);

    ngx_queue_split(queue, q, &tail);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&tail, cmp);

    ngx_queue_merge(queue, &tail, cmp);
}


स्थिर शून्य
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *tail,
    ngx_int_t (*cmp)(स्थिरांक ngx_queue_t *, स्थिरांक ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_head(queue);
    q2 = ngx_queue_head(tail);

    साठी ( ;; ) {
        जर (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, tail);
            थांबा;
        }

        जर (q2 == ngx_queue_sentinel(tail)) {
            थांबा;
        }

        जर (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_next(q1);
            सुरूठेवा;
        }

        ngx_queue_remove(q2);
        ngx_queue_घाला_before(q1, q2);

        q2 = ngx_queue_head(tail);
    }
}
