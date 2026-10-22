
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *line)
{
    u_char   unit;
    size_t   len;
    ssize_t  size, scale, max;

    len = line->len;

    אם (len == 0) {
        החזר NGX_ERROR;
    }

    unit = line->data[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_SIZE_T_VALUE / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_SIZE_T_VALUE / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_SIZE_T_VALUE;
        scale = 1;
    }

    size = ngx_atosz(line->data, len);
    אם (size == NGX_ERROR || size > max) {
        החזר NGX_ERROR;
    }

    size *= scale;

    החזר size;
}


off_t
ngx_parse_offset(ngx_str_t *line)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = line->len;

    אם (len == 0) {
        החזר NGX_ERROR;
    }

    unit = line->data[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_OFF_T_VALUE / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_OFF_T_VALUE / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    حالة 'G':
    حالة 'g':
        len--;
        max = NGX_MAX_OFF_T_VALUE / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_OFF_T_VALUE;
        scale = 1;
    }

    offset = ngx_atoof(line->data, len);
    אם (offset == NGX_ERROR || offset > max) {
        החזר NGX_ERROR;
    }

    offset *= scale;

    החזר offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *line, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    value, total, scale;
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
    value = 0;
    total = 0;
    cutoff = NGX_MAX_INT_T_VALUE / 10;
    cutlim = NGX_MAX_INT_T_VALUE % 10;
    step = is_sec ? st_start : st_month;

    p = line->data;
    last = p + line->len;

    כלעוד (p < last) {

        אם (*p >= '0' && *p <= '9') {
            אם (value >= cutoff && (value > cutoff || *p - '0' > cutlim)) {
                החזר NGX_ERROR;
            }

            value = value * 10 + (*p++ - '0');
            valid = 1;
            استمر;
        }

        بدل (*p++) {

        حالة 'y':
            אם (step > st_start) {
                החזר NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            اكسر;

        حالة 'M':
            אם (step >= st_month) {
                החזר NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            اكسر;

        حالة 'w':
            אם (step >= st_week) {
                החזר NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            اكسر;

        حالة 'd':
            אם (step >= st_day) {
                החזר NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            اكسر;

        حالة 'h':
            אם (step >= st_hour) {
                החזר NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_VALUE / (60 * 60);
            scale = 60 * 60;
            اكسر;

        حالة 'm':
            אם (p < last && *p == 's') {
                אם (is_sec || step >= st_msec) {
                    החזר NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_VALUE;
                scale = 1;
                اكسر;
            }

            אם (step >= st_min) {
                החזר NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_VALUE / 60;
            scale = 60;
            اكسر;

        حالة 's':
            אם (step >= st_sec) {
                החזר NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            اكسر;

        حالة ' ':
            אם (step >= st_sec) {
                החזר NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            اكسر;

        افتراضي:
            החזר NGX_ERROR;
        }

        אם (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        אם (value > max) {
            החזר NGX_ERROR;
        }

        value *= scale;

        אם (total > NGX_MAX_INT_T_VALUE - value) {
            החזר NGX_ERROR;
        }

        total += value;

        value = 0;

        כלעוד (p < last && *p == ' ') {
            p++;
        }
    }

    אם (!valid) {
        החזר NGX_ERROR;
    }

    אם (!is_sec) {
        אם (value > NGX_MAX_INT_T_VALUE / 1000) {
            החזר NGX_ERROR;
        }

        value *= 1000;
    }

    אם (total > NGX_MAX_INT_T_VALUE - value) {
        החזר NGX_ERROR;
    }

    החזר total + value;
}
