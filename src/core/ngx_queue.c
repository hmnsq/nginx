
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


স্থির শূন্য ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *লেজ,
    ngx_int_t (*cmp)(ধ্রুবক ngx_queue_t *, ধ্রুবক ngx_queue_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_queue_t *
ngx_queue_মধ্য(ngx_queue_t *queue)
{
    ngx_queue_t  *মধ্য, *পরবর্তী;

    মধ্য = ngx_queue_শীর্ষ(queue);

    যদি (মধ্য == ngx_queue_last(queue)) {
        ফেরত মধ্য;
    }

    পরবর্তী = ngx_queue_শীর্ষ(queue);

    জন্য ( ;; ) {
        মধ্য = ngx_queue_পরবর্তী(মধ্য);

        পরবর্তী = ngx_queue_পরবর্তী(পরবর্তী);

        যদি (পরবর্তী == ngx_queue_last(queue)) {
            ফেরত মধ্য;
        }

        পরবর্তী = ngx_queue_পরবর্তী(পরবর্তী);

        যদি (পরবর্তী == ngx_queue_last(queue)) {
            ফেরত মধ্য;
        }
    }
}


/* the stable merge sort */

শূন্য
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(ধ্রুবক ngx_queue_t *, ধ্রুবক ngx_queue_t *))
{
    ngx_queue_t  *q, লেজ;

    q = ngx_queue_শীর্ষ(queue);

    যদি (q == ngx_queue_last(queue)) {
        ফেরত;
    }

    q = ngx_queue_মধ্য(queue);

    ngx_queue_split(queue, q, &লেজ);

    ngx_queue_sort(queue, cmp);
    ngx_queue_sort(&লেজ, cmp);

    ngx_queue_merge(queue, &লেজ, cmp);
}


স্থির শূন্য
ngx_queue_merge(ngx_queue_t *queue, ngx_queue_t *লেজ,
    ngx_int_t (*cmp)(ধ্রুবক ngx_queue_t *, ধ্রুবক ngx_queue_t *))
{
    ngx_queue_t  *q1, *q2;

    q1 = ngx_queue_শীর্ষ(queue);
    q2 = ngx_queue_শীর্ষ(লেজ);

    জন্য ( ;; ) {
        যদি (q1 == ngx_queue_sentinel(queue)) {
            ngx_queue_add(queue, লেজ);
            বিরতি;
        }

        যদি (q2 == ngx_queue_sentinel(লেজ)) {
            বিরতি;
        }

        যদি (cmp(q1, q2) <= 0) {
            q1 = ngx_queue_পরবর্তী(q1);
            চালু;
        }

        ngx_queue_remove(q2);
        ngx_queue_ঢোকাও_before(q1, q2);

        q2 = ngx_queue_শীর্ষ(লেজ);
    }
}
