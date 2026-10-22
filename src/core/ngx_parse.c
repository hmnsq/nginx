
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *سطر)
{
    u_char   unit;
    size_t   len;
    ssize_t  size, scale, max;

    len = سطر->len;

    إذا (len == 0) {
        إرجاع NGX_ERROR;
    }

    unit = سطر->data[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_SIZE_T_قيمة / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_SIZE_T_قيمة / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_SIZE_T_قيمة;
        scale = 1;
    }

    size = ngx_atosz(سطر->data, len);
    إذا (size == NGX_ERROR || size > max) {
        إرجاع NGX_ERROR;
    }

    size *= scale;

    إرجاع size;
}


off_t
ngx_parse_offset(ngx_str_t *سطر)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = سطر->len;

    إذا (len == 0) {
        إرجاع NGX_ERROR;
    }

    unit = سطر->data[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_OFF_T_قيمة / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_OFF_T_قيمة / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    حالة 'G':
    حالة 'g':
        len--;
        max = NGX_MAX_OFF_T_قيمة / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_OFF_T_قيمة;
        scale = 1;
    }

    offset = ngx_atoof(سطر->data, len);
    إذا (offset == NGX_ERROR || offset > max) {
        إرجاع NGX_ERROR;
    }

    offset *= scale;

    إرجاع offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *سطر, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    قيمة, total, scale;
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
    قيمة = 0;
    total = 0;
    cutoff = NGX_MAX_INT_T_قيمة / 10;
    cutlim = NGX_MAX_INT_T_قيمة % 10;
    step = is_sec ? st_start : st_month;

    p = سطر->data;
    last = p + سطر->len;

    بينما (p < last) {

        إذا (*p >= '0' && *p <= '9') {
            إذا (قيمة >= cutoff && (قيمة > cutoff || *p - '0' > cutlim)) {
                إرجاع NGX_ERROR;
            }

            قيمة = قيمة * 10 + (*p++ - '0');
            valid = 1;
            استمر;
        }

        بدل (*p++) {

        حالة 'y':
            إذا (step > st_start) {
                إرجاع NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_قيمة / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            اكسر;

        حالة 'M':
            إذا (step >= st_month) {
                إرجاع NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_قيمة / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            اكسر;

        حالة 'w':
            إذا (step >= st_week) {
                إرجاع NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_قيمة / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            اكسر;

        حالة 'd':
            إذا (step >= st_day) {
                إرجاع NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_قيمة / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            اكسر;

        حالة 'h':
            إذا (step >= st_hour) {
                إرجاع NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_قيمة / (60 * 60);
            scale = 60 * 60;
            اكسر;

        حالة 'm':
            إذا (p < last && *p == 's') {
                إذا (is_sec || step >= st_msec) {
                    إرجاع NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_قيمة;
                scale = 1;
                اكسر;
            }

            إذا (step >= st_min) {
                إرجاع NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_قيمة / 60;
            scale = 60;
            اكسر;

        حالة 's':
            إذا (step >= st_sec) {
                إرجاع NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_قيمة;
            scale = 1;
            اكسر;

        حالة ' ':
            إذا (step >= st_sec) {
                إرجاع NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_قيمة;
            scale = 1;
            اكسر;

        افتراضي:
            إرجاع NGX_ERROR;
        }

        إذا (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        إذا (قيمة > max) {
            إرجاع NGX_ERROR;
        }

        قيمة *= scale;

        إذا (total > NGX_MAX_INT_T_قيمة - قيمة) {
            إرجاع NGX_ERROR;
        }

        total += قيمة;

        قيمة = 0;

        بينما (p < last && *p == ' ') {
            p++;
        }
    }

    إذا (!valid) {
        إرجاع NGX_ERROR;
    }

    إذا (!is_sec) {
        إذا (قيمة > NGX_MAX_INT_T_قيمة / 1000) {
            إرجاع NGX_ERROR;
        }

        قيمة *= 1000;
    }

    إذا (total > NGX_MAX_INT_T_قيمة - قيمة) {
        إرجاع NGX_ERROR;
    }

    إرجاع total + قيمة;
}
