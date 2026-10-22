
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ssize_t
ngx_parse_اندازه(ngx_str_t *کرښه)
{
    u_char   unit;
    اندازه_t   len;
    ssize_t  اندازه, scale, max;

    len = کرښه->len;

    إذا (len == 0) {
        أرجع NGX_ERROR;
    }

    unit = کرښه->معلومات[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_اندازه_T_ارزښت / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_اندازه_T_ارزښت / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_اندازه_T_ارزښت;
        scale = 1;
    }

    اندازه = ngx_atosz(کرښه->معلومات, len);
    إذا (اندازه == NGX_ERROR || اندازه > max) {
        أرجع NGX_ERROR;
    }

    اندازه *= scale;

    أرجع اندازه;
}


off_t
ngx_parse_دننه_واټن(ngx_str_t *کرښه)
{
    u_char  unit;
    off_t   دننه_واټن, scale, max;
    اندازه_t  len;

    len = کرښه->len;

    إذا (len == 0) {
        أرجع NGX_ERROR;
    }

    unit = کرښه->معلومات[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_OFF_T_ارزښت / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_OFF_T_ارزښت / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    حالة 'G':
    حالة 'g':
        len--;
        max = NGX_MAX_OFF_T_ارزښت / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_OFF_T_ارزښت;
        scale = 1;
    }

    دننه_واټن = ngx_atoof(کرښه->معلومات, len);
    إذا (دننه_واټن == NGX_ERROR || دننه_واټن > max) {
        أرجع NGX_ERROR;
    }

    دننه_واټن *= scale;

    أرجع دننه_واټن;
}


ngx_int_t
ngx_parse_وخت(ngx_str_t *کرښه, ngx_uint_t is_sec)
{
    u_char      *p, *وروستی;
    ngx_int_t    ارزښت, ټول, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    تعداد {
        st_پیل = 0,
        st_year,
        st_month,
        st_week,
        st_day,
        st_hour,
        st_min,
        st_sec,
        st_msec,
        st_وروستی
    } ګام_2;

    valid = 0;
    ارزښت = 0;
    ټول = 0;
    cutoff = NGX_MAX_INT_T_ارزښت / 10;
    cutlim = NGX_MAX_INT_T_ارزښت % 10;
    ګام_2 = is_sec ? st_پیل : st_month;

    p = کرښه->معلومات;
    وروستی = p + کرښه->len;

    طالما (p < وروستی) {

        إذا (*p >= '0' && *p <= '9') {
            إذا (ارزښت >= cutoff && (ارزښت > cutoff || *p - '0' > cutlim)) {
                أرجع NGX_ERROR;
            }

            ارزښت = ارزښت * 10 + (*p++ - '0');
            valid = 1;
            استمر;
        }

        بدل (*p++) {

        حالة 'y':
            إذا (ګام_2 > st_پیل) {
                أرجع NGX_ERROR;
            }
            ګام_2 = st_year;
            max = NGX_MAX_INT_T_ارزښت / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            اكسر;

        حالة 'M':
            إذا (ګام_2 >= st_month) {
                أرجع NGX_ERROR;
            }
            ګام_2 = st_month;
            max = NGX_MAX_INT_T_ارزښت / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            اكسر;

        حالة 'w':
            إذا (ګام_2 >= st_week) {
                أرجع NGX_ERROR;
            }
            ګام_2 = st_week;
            max = NGX_MAX_INT_T_ارزښت / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            اكسر;

        حالة 'd':
            إذا (ګام_2 >= st_day) {
                أرجع NGX_ERROR;
            }
            ګام_2 = st_day;
            max = NGX_MAX_INT_T_ارزښت / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            اكسر;

        حالة 'h':
            إذا (ګام_2 >= st_hour) {
                أرجع NGX_ERROR;
            }
            ګام_2 = st_hour;
            max = NGX_MAX_INT_T_ارزښت / (60 * 60);
            scale = 60 * 60;
            اكسر;

        حالة 'm':
            إذا (p < وروستی && *p == 's') {
                إذا (is_sec || ګام_2 >= st_msec) {
                    أرجع NGX_ERROR;
                }
                p++;
                ګام_2 = st_msec;
                max = NGX_MAX_INT_T_ارزښت;
                scale = 1;
                اكسر;
            }

            إذا (ګام_2 >= st_min) {
                أرجع NGX_ERROR;
            }
            ګام_2 = st_min;
            max = NGX_MAX_INT_T_ارزښت / 60;
            scale = 60;
            اكسر;

        حالة 's':
            إذا (ګام_2 >= st_sec) {
                أرجع NGX_ERROR;
            }
            ګام_2 = st_sec;
            max = NGX_MAX_INT_T_ارزښت;
            scale = 1;
            اكسر;

        حالة ' ':
            إذا (ګام_2 >= st_sec) {
                أرجع NGX_ERROR;
            }
            ګام_2 = st_وروستی;
            max = NGX_MAX_INT_T_ارزښت;
            scale = 1;
            اكسر;

        افتراضي:
            أرجع NGX_ERROR;
        }

        إذا (ګام_2 != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        إذا (ارزښت > max) {
            أرجع NGX_ERROR;
        }

        ارزښت *= scale;

        إذا (ټول > NGX_MAX_INT_T_ارزښت - ارزښت) {
            أرجع NGX_ERROR;
        }

        ټول += ارزښت;

        ارزښت = 0;

        طالما (p < وروستی && *p == ' ') {
            p++;
        }
    }

    إذا (!valid) {
        أرجع NGX_ERROR;
    }

    إذا (!is_sec) {
        إذا (ارزښت > NGX_MAX_INT_T_ارزښت / 1000) {
            أرجع NGX_ERROR;
        }

        ارزښت *= 1000;
    }

    إذا (ټول > NGX_MAX_INT_T_ارزښت - ارزښت) {
        أرجع NGX_ERROR;
    }

    أرجع ټول + ارزښت;
}
