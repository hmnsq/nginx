
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *لي)
{
    u_char   unit;
    size_t   len;
    ssize_t  size, scale, max;

    len = لي->len;

    إذا (len == 0) {
        أرجع NGX_ERROR;
    }

    unit = لي->data[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_SIZE_T_درج / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_SIZE_T_درج / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_SIZE_T_درج;
        scale = 1;
    }

    size = ngx_atosz(لي->data, len);
    إذا (size == NGX_ERROR || size > max) {
        أرجع NGX_ERROR;
    }

    size *= scale;

    أرجع size;
}


off_t
ngx_parse_offset(ngx_str_t *لي)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = لي->len;

    إذا (len == 0) {
        أرجع NGX_ERROR;
    }

    unit = لي->data[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_OFF_T_درج / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_OFF_T_درج / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    حالة 'G':
    حالة 'g':
        len--;
        max = NGX_MAX_OFF_T_درج / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_OFF_T_درج;
        scale = 1;
    }

    offset = ngx_atoof(لي->data, len);
    إذا (offset == NGX_ERROR || offset > max) {
        أرجع NGX_ERROR;
    }

    offset *= scale;

    أرجع offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *لي, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    درج, total, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    تعداد {
        st_start = 0,
        st_year,
        st_month,
        st_week,
        st_day,
        st_hour,
        st_min,
        st_sec,
        st_msec,
        st_last
    } step;

    valid = 0;
    درج = 0;
    total = 0;
    cutoff = NGX_MAX_INT_T_درج / 10;
    cutlim = NGX_MAX_INT_T_درج % 10;
    step = is_sec ? st_start : st_month;

    p = لي->data;
    last = p + لي->len;

    طالما (p < last) {

        إذا (*p >= '0' && *p <= '9') {
            إذا (درج >= cutoff && (درج > cutoff || *p - '0' > cutlim)) {
                أرجع NGX_ERROR;
            }

            درج = درج * 10 + (*p++ - '0');
            valid = 1;
            استمر;
        }

        بدل (*p++) {

        حالة 'y':
            إذا (step > st_start) {
                أرجع NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_درج / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            اكسر;

        حالة 'M':
            إذا (step >= st_month) {
                أرجع NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_درج / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            اكسر;

        حالة 'w':
            إذا (step >= st_week) {
                أرجع NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_درج / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            اكسر;

        حالة 'd':
            إذا (step >= st_day) {
                أرجع NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_درج / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            اكسر;

        حالة 'h':
            إذا (step >= st_hour) {
                أرجع NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_درج / (60 * 60);
            scale = 60 * 60;
            اكسر;

        حالة 'm':
            إذا (p < last && *p == 's') {
                إذا (is_sec || step >= st_msec) {
                    أرجع NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_درج;
                scale = 1;
                اكسر;
            }

            إذا (step >= st_min) {
                أرجع NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_درج / 60;
            scale = 60;
            اكسر;

        حالة 's':
            إذا (step >= st_sec) {
                أرجع NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_درج;
            scale = 1;
            اكسر;

        حالة ' ':
            إذا (step >= st_sec) {
                أرجع NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_درج;
            scale = 1;
            اكسر;

        افتراضي:
            أرجع NGX_ERROR;
        }

        إذا (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        إذا (درج > max) {
            أرجع NGX_ERROR;
        }

        درج *= scale;

        إذا (total > NGX_MAX_INT_T_درج - درج) {
            أرجع NGX_ERROR;
        }

        total += درج;

        درج = 0;

        طالما (p < last && *p == ' ') {
            p++;
        }
    }

    إذا (!valid) {
        أرجع NGX_ERROR;
    }

    إذا (!is_sec) {
        إذا (درج > NGX_MAX_INT_T_درج / 1000) {
            أرجع NGX_ERROR;
        }

        درج *= 1000;
    }

    إذا (total > NGX_MAX_INT_T_درج - درج) {
        أرجع NGX_ERROR;
    }

    أرجع total + درج;
}
