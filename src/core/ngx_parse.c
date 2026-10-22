
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ssize_t
ngx_parse_سائيز(ngx_str_t *سٽ)
{
    u_char   unit;
    سائيز_t   len;
    ssize_t  سائيز, scale, max;

    len = سٽ->len;

    إذا (len == 0) {
        أرجع NGX_ERROR;
    }

    unit = سٽ->معلومات[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_سائيز_T_قدر / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_سائيز_T_قدر / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_سائيز_T_قدر;
        scale = 1;
    }

    سائيز = ngx_atosz(سٽ->معلومات, len);
    إذا (سائيز == NGX_ERROR || سائيز > max) {
        أرجع NGX_ERROR;
    }

    سائيز *= scale;

    أرجع سائيز;
}


off_t
ngx_parse_وٿي(ngx_str_t *سٽ)
{
    u_char  unit;
    off_t   وٿي, scale, max;
    سائيز_t  len;

    len = سٽ->len;

    إذا (len == 0) {
        أرجع NGX_ERROR;
    }

    unit = سٽ->معلومات[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_OFF_T_قدر / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_OFF_T_قدر / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    حالة 'G':
    حالة 'g':
        len--;
        max = NGX_MAX_OFF_T_قدر / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_OFF_T_قدر;
        scale = 1;
    }

    وٿي = ngx_atoof(سٽ->معلومات, len);
    إذا (وٿي == NGX_ERROR || وٿي > max) {
        أرجع NGX_ERROR;
    }

    وٿي *= scale;

    أرجع وٿي;
}


ngx_int_t
ngx_parse_وقت(ngx_str_t *سٽ, ngx_uint_t is_sec)
{
    u_char      *p, *آخري;
    ngx_int_t    قدر, ڪل, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    تعداد {
        st_شروعات = 0,
        st_year,
        st_month,
        st_week,
        st_day,
        st_hour,
        st_min,
        st_sec,
        st_msec,
        st_آخري
    } قدم_2;

    valid = 0;
    قدر = 0;
    ڪل = 0;
    cutoff = NGX_MAX_INT_T_قدر / 10;
    cutlim = NGX_MAX_INT_T_قدر % 10;
    قدم_2 = is_sec ? st_شروعات : st_month;

    p = سٽ->معلومات;
    آخري = p + سٽ->len;

    طالما (p < آخري) {

        إذا (*p >= '0' && *p <= '9') {
            إذا (قدر >= cutoff && (قدر > cutoff || *p - '0' > cutlim)) {
                أرجع NGX_ERROR;
            }

            قدر = قدر * 10 + (*p++ - '0');
            valid = 1;
            استمر;
        }

        بدل (*p++) {

        حالة 'y':
            إذا (قدم_2 > st_شروعات) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_year;
            max = NGX_MAX_INT_T_قدر / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            اكسر;

        حالة 'M':
            إذا (قدم_2 >= st_month) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_month;
            max = NGX_MAX_INT_T_قدر / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            اكسر;

        حالة 'w':
            إذا (قدم_2 >= st_week) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_week;
            max = NGX_MAX_INT_T_قدر / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            اكسر;

        حالة 'd':
            إذا (قدم_2 >= st_day) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_day;
            max = NGX_MAX_INT_T_قدر / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            اكسر;

        حالة 'h':
            إذا (قدم_2 >= st_hour) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_hour;
            max = NGX_MAX_INT_T_قدر / (60 * 60);
            scale = 60 * 60;
            اكسر;

        حالة 'm':
            إذا (p < آخري && *p == 's') {
                إذا (is_sec || قدم_2 >= st_msec) {
                    أرجع NGX_ERROR;
                }
                p++;
                قدم_2 = st_msec;
                max = NGX_MAX_INT_T_قدر;
                scale = 1;
                اكسر;
            }

            إذا (قدم_2 >= st_min) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_min;
            max = NGX_MAX_INT_T_قدر / 60;
            scale = 60;
            اكسر;

        حالة 's':
            إذا (قدم_2 >= st_sec) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_sec;
            max = NGX_MAX_INT_T_قدر;
            scale = 1;
            اكسر;

        حالة ' ':
            إذا (قدم_2 >= st_sec) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_آخري;
            max = NGX_MAX_INT_T_قدر;
            scale = 1;
            اكسر;

        افتراضي:
            أرجع NGX_ERROR;
        }

        إذا (قدم_2 != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        إذا (قدر > max) {
            أرجع NGX_ERROR;
        }

        قدر *= scale;

        إذا (ڪل > NGX_MAX_INT_T_قدر - قدر) {
            أرجع NGX_ERROR;
        }

        ڪل += قدر;

        قدر = 0;

        طالما (p < آخري && *p == ' ') {
            p++;
        }
    }

    إذا (!valid) {
        أرجع NGX_ERROR;
    }

    إذا (!is_sec) {
        إذا (قدر > NGX_MAX_INT_T_قدر / 1000) {
            أرجع NGX_ERROR;
        }

        قدر *= 1000;
    }

    إذا (ڪل > NGX_MAX_INT_T_قدر - قدر) {
        أرجع NGX_ERROR;
    }

    أرجع ڪل + قدر;
}
