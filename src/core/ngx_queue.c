
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर शून्य ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *पुच्छ,
    ngx_int_t (*cmp)(ध्रुव ngx_queue_t *, ध्रुव ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_मध्यः(ngx_queue_t *queue)
{
    ngx_queue_t  *मध्यः, *अग्र;

    मध्यः = ngx_queue_शीर्ष(queue);

    यदि (मध्यः == ngx_queue_चरमः(queue)) {
        प्रत्यागम मध्यः;
    }

    अग्र = ngx_queue_शीर्ष(queue);

    प्रति ( ;; ) {
        मध्यः = ngx_queue_अग्र(मध्यः);

        अग्र = ngx_queue_अग्र(अग्र);

        यदि (अग्र == ngx_queue_चरमः(queue)) {
            प्रत्यागम मध्यः;
        }

        अग्र = ngx_queue_अग्र(अग्र);

        यदि (अग्र == ngx_queue_चरमः(queue)) {
            प्रत्यागम मध्यः;
        }
    }
}


/* the stable merge sort */

शून्य
ngx_queue_क्रमय(ngx_queue_t *queue,
    ngx_int_t (*cmp)(ध्रुव ngx_queue_t *, ध्रुव ngx_queue_t *))
{
    ngx_queue_t  *q, पुच्छ;

    q = ngx_queue_शीर्ष(queue);

    यदि (q == ngx_queue_चरमः(queue)) {
        प्रत्यागम;
    }

    q = ngx_queue_मध्यः(queue);

    ngx_queue_विभज(queue, q, &पुच्छ);

    ngx_queue_क्रमय(queue, cmp);
    ngx_queue_क्रमय(&पुच्छ, cmp);

    ngx_queue_merge(queue, &पुच्छ, cmp);
}


स्थिर शून्य
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *पुच्छ,
    ngx_int_t (*cmp)(ध्रुव ngx_queue_t *, ध्रुव ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_शीर्ष(queue);
    q2 = ngx_queue_शीर्ष(पुच्छ);

    प्रति ( ;; ) {
        यदि (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_सङ्कलय(queue, पुच्छ);
            भङ्ग;
        }

        यदि (q2 == ngx_queue_sentinel(पुच्छ)) {
            भङ्ग;
        }

        यदि (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_अग्र(q1);
            अनुवर्त;
        }

        ngx_queue_remove(q2);
        ngx_queue_निवेशय_before(q1, q2);

        q2 = ngx_queue_शीर्ष(पुच्छ);
    }
}
