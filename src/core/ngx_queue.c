
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ساکن خالی ngx_قطار_ملاؤ(ngx_قطار_t *قطار, ngx_قطار_t *دم,
    ngx_int_t (*cmp)(ثابت ngx_قطار_t *, ثابت ngx_قطار_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_قطار_t *
ngx_قطار_درمیان(ngx_قطار_t *قطار)
{
    ngx_قطار_t  *درمیان, *اگلا;

    درمیان = ngx_قطار_سر(قطار);

    اگر (درمیان == ngx_قطار_آخری(قطار)) {
        واپس درمیان;
    }

    اگلا = ngx_قطار_سر(قطار);

    برائے ( ;; ) {
        درمیان = ngx_قطار_اگلا(درمیان);

        اگلا = ngx_قطار_اگلا(اگلا);

        اگر (اگلا == ngx_قطار_آخری(قطار)) {
            واپس درمیان;
        }

        اگلا = ngx_قطار_اگلا(اگلا);

        اگر (اگلا == ngx_قطار_آخری(قطار)) {
            واپس درمیان;
        }
    }
}


/* the stable merge sort */

خالی
ngx_قطار_ترتیب_دو(ngx_قطار_t *قطار,
    ngx_int_t (*cmp)(ثابت ngx_قطار_t *, ثابت ngx_قطار_t *))
{
    ngx_قطار_t  *q, دم;

    q = ngx_قطار_سر(قطار);

    اگر (q == ngx_قطار_آخری(قطار)) {
        واپس;
    }

    q = ngx_قطار_درمیان(قطار);

    ngx_قطار_بانٹو(قطار, q, &دم);

    ngx_قطار_ترتیب_دو(قطار, cmp);
    ngx_قطار_ترتیب_دو(&دم, cmp);

    ngx_قطار_ملاؤ(قطار, &دم, cmp);
}


ساکن خالی
ngx_قطار_ملاؤ(ngx_قطار_t *قطار, ngx_قطار_t *دم,
    ngx_int_t (*cmp)(ثابت ngx_قطار_t *, ثابت ngx_قطار_t *))
{
    ngx_قطار_t  *q1, *q2;

    q1 = ngx_قطار_سر(قطار);
    q2 = ngx_قطار_سر(دم);

    برائے ( ;; ) {
        اگر (q1 == ngx_قطار_sentinel(قطار)) {
            ngx_قطار_جوڑو(قطار, دم);
            توڑو;
        }

        اگر (q2 == ngx_قطار_sentinel(دم)) {
            توڑو;
        }

        اگر (cmp(q1, q2) <= 0) {
            q1 = ngx_قطار_اگلا(q1);
            جاری;
        }

        ngx_قطار_remove(q2);
        ngx_قطار_داخل_کرو_before(q1, q2);

        q2 = ngx_قطار_سر(دم);
    }
}
