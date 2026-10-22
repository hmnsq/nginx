
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


अटल व्योम ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *पुच्छ,
    ngx_int_t (*cmp)(स्थिर ngx_queue_t *, स्थिर ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_मध्य(ngx_queue_t *queue)
{
    ngx_queue_t  *मध्य, *अग्र;

    मध्य = ngx_queue_शीर्ष(queue);

    अगर (मध्य == ngx_queue_last(queue)) {
        वापस मध्य;
    }

    अग्र = ngx_queue_शीर्ष(queue);

    क्रम ( ;; ) {
        मध्य = ngx_queue_अग्र(मध्य);

        अग्र = ngx_queue_अग्र(अग्र);

        अगर (अग्र == ngx_queue_last(queue)) {
            वापस मध्य;
        }

        अग्र = ngx_queue_अग्र(अग्र);

        अगर (अग्र == ngx_queue_last(queue)) {
            वापस मध्य;
        }
    }
}


/* the stable merge sort */

व्योम
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(स्थिर ngx_queue_t *, स्थिर ngx_queue_t *))
{
    ngx_queue_t  *q, पुच्छ;

    q = ngx_queue_शीर्ष(queue);

    अगर (q == ngx_queue_last(queue)) {
        वापस;
    }

    q = ngx_queue_मध्य(queue);

    ngx_queue_split(queue, q, &पुच्छ);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&पुच्छ, cmp);

    ngx_queue_merge(queue, &पुच्छ, cmp);
}


अटल व्योम
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *पुच्छ,
    ngx_int_t (*cmp)(स्थिर ngx_queue_t *, स्थिर ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_शीर्ष(queue);
    q2 = ngx_queue_शीर्ष(पुच्छ);

    क्रम ( ;; ) {
        अगर (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, पुच्छ);
            अवरोध;
        }

        अगर (q2 == ngx_queue_sentinel(पुच्छ)) {
            अवरोध;
        }

        अगर (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_अग्र(q1);
            जारी;
        }

        ngx_queue_remove(q2);
        ngx_queue_जोड़ो_before(q1, q2);

        q2 = ngx_queue_शीर्ष(पुच्छ);
    }
}
