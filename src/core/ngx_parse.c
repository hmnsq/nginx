
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

    ܐܢ (len == 0) {
        ܦܢܐ NGX_ERROR;
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
    ܐܢ (size == NGX_ERROR || size > max) {
        ܦܢܐ NGX_ERROR;
    }

    size *= scale;

    ܦܢܐ size;
}


off_t
ngx_parse_offset(ngx_str_t *line)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = line->len;

    ܐܢ (len == 0) {
        ܦܢܐ NGX_ERROR;
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
    ܐܢ (offset == NGX_ERROR || offset > max) {
        ܦܢܐ NGX_ERROR;
    }

    offset *= scale;

    ܦܢܐ offset;
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

    ܟܡܐ (p < last) {

        ܐܢ (*p >= '0' && *p <= '9') {
            ܐܢ (value >= cutoff && (value > cutoff || *p - '0' > cutlim)) {
                ܦܢܐ NGX_ERROR;
            }

            value = value * 10 + (*p++ - '0');
            valid = 1;
            استمر;
        }

        بدل (*p++) {

        حالة 'y':
            ܐܢ (step > st_start) {
                ܦܢܐ NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            اكسر;

        حالة 'M':
            ܐܢ (step >= st_month) {
                ܦܢܐ NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            اكسر;

        حالة 'w':
            ܐܢ (step >= st_week) {
                ܦܢܐ NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            اكسر;

        حالة 'd':
            ܐܢ (step >= st_day) {
                ܦܢܐ NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            اكسر;

        حالة 'h':
            ܐܢ (step >= st_hour) {
                ܦܢܐ NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_VALUE / (60 * 60);
            scale = 60 * 60;
            اكسر;

        حالة 'm':
            ܐܢ (p < last && *p == 's') {
                ܐܢ (is_sec || step >= st_msec) {
                    ܦܢܐ NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_VALUE;
                scale = 1;
                اكسر;
            }

            ܐܢ (step >= st_min) {
                ܦܢܐ NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_VALUE / 60;
            scale = 60;
            اكسر;

        حالة 's':
            ܐܢ (step >= st_sec) {
                ܦܢܐ NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            اكسر;

        حالة ' ':
            ܐܢ (step >= st_sec) {
                ܦܢܐ NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            اكسر;

        افتراضي:
            ܦܢܐ NGX_ERROR;
        }

        ܐܢ (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        ܐܢ (value > max) {
            ܦܢܐ NGX_ERROR;
        }

        value *= scale;

        ܐܢ (total > NGX_MAX_INT_T_VALUE - value) {
            ܦܢܐ NGX_ERROR;
        }

        total += value;

        value = 0;

        ܟܡܐ (p < last && *p == ' ') {
            p++;
        }
    }

    ܐܢ (!valid) {
        ܦܢܐ NGX_ERROR;
    }

    ܐܢ (!is_sec) {
        ܐܢ (value > NGX_MAX_INT_T_VALUE / 1000) {
            ܦܢܐ NGX_ERROR;
        }

        value *= 1000;
    }

    ܐܢ (total > NGX_MAX_INT_T_VALUE - value) {
        ܦܢܐ NGX_ERROR;
    }

    ܦܢܐ total + value;
}
