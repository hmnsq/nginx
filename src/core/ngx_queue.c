
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن فراغ ngx_صف_ادغام(ngx_صف_t *صف, ngx_صف_t *دم,
    ngx_int_t (*cmp)(ثابت ngx_صف_t *, ثابت ngx_صف_t *));


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

ngx_صف_t *
ngx_صف_میانه(ngx_صف_t *صف)
{
    ngx_صف_t  *میانه, *بعدی;

    میانه = ngx_صف_سر(صف);

    إذا (میانه == ngx_صف_آخرین(صف)) {
        أرجع میانه;
    }

    بعدی = ngx_صف_سر(صف);

    لكل ( ;; ) {
        میانه = ngx_صف_بعدی(میانه);

        بعدی = ngx_صف_بعدی(بعدی);

        إذا (بعدی == ngx_صف_آخرین(صف)) {
            أرجع میانه;
        }

        بعدی = ngx_صف_بعدی(بعدی);

        إذا (بعدی == ngx_صف_آخرین(صف)) {
            أرجع میانه;
        }
    }
}


/* the stable merge sort */

فراغ
ngx_صف_مرتب_سازی(ngx_صف_t *صف,
    ngx_int_t (*cmp)(ثابت ngx_صف_t *, ثابت ngx_صف_t *))
{
    ngx_صف_t  *q, دم;

    q = ngx_صف_سر(صف);

    إذا (q == ngx_صف_آخرین(صف)) {
        أرجع;
    }

    q = ngx_صف_میانه(صف);

    ngx_صف_تقسیم(صف, q, &دم);

    ngx_صف_مرتب_سازی(صف, cmp);
    ngx_صف_مرتب_سازی(&دم, cmp);

    ngx_صف_ادغام(صف, &دم, cmp);
}


ساكن فراغ
ngx_صف_ادغام(ngx_صف_t *صف, ngx_صف_t *دم,
    ngx_int_t (*cmp)(ثابت ngx_صف_t *, ثابت ngx_صف_t *))
{
    ngx_صف_t  *q1, *q2;

    q1 = ngx_صف_سر(صف);
    q2 = ngx_صف_سر(دم);

    لكل ( ;; ) {
        إذا (q1 == ngx_صف_sentinel(صف)) {
            ngx_صف_جمع_کن(صف, دم);
            اكسر;
        }

        إذا (q2 == ngx_صف_sentinel(دم)) {
            اكسر;
        }

        إذا (cmp(q1, q2) <= 0) {
            q1 = ngx_صف_بعدی(q1);
            استمر;
        }

        ngx_صف_remove(q2);
        ngx_صف_درج_before(q1, q2);

        q2 = ngx_صف_سر(دم);
    }
}
