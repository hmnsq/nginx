
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

    𐡄𐡍 (len == 0) {
        أرجع NGX_ERROR;
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
    𐡄𐡍 (size == NGX_ERROR || size > max) {
        أرجع NGX_ERROR;
    }

    size *= scale;

    أرجع size;
}


off_t
ngx_parse_offset(ngx_str_t *line)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = line->len;

    𐡄𐡍 (len == 0) {
        أرجع NGX_ERROR;
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
    𐡄𐡍 (offset == NGX_ERROR || offset > max) {
        أرجع NGX_ERROR;
    }

    offset *= scale;

    أرجع offset;
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

    طالما (p < last) {

        𐡄𐡍 (*p >= '0' && *p <= '9') {
            𐡄𐡍 (value >= cutoff && (value > cutoff || *p - '0' > cutlim)) {
                أرجع NGX_ERROR;
            }

            value = value * 10 + (*p++ - '0');
            valid = 1;
            استمر;
        }

        بدل (*p++) {

        حالة 'y':
            𐡄𐡍 (step > st_start) {
                أرجع NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            اكسر;

        حالة 'M':
            𐡄𐡍 (step >= st_month) {
                أرجع NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            اكسر;

        حالة 'w':
            𐡄𐡍 (step >= st_week) {
                أرجع NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            اكسر;

        حالة 'd':
            𐡄𐡍 (step >= st_day) {
                أرجع NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            اكسر;

        حالة 'h':
            𐡄𐡍 (step >= st_hour) {
                أرجع NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_VALUE / (60 * 60);
            scale = 60 * 60;
            اكسر;

        حالة 'm':
            𐡄𐡍 (p < last && *p == 's') {
                𐡄𐡍 (is_sec || step >= st_msec) {
                    أرجع NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_VALUE;
                scale = 1;
                اكسر;
            }

            𐡄𐡍 (step >= st_min) {
                أرجع NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_VALUE / 60;
            scale = 60;
            اكسر;

        حالة 's':
            𐡄𐡍 (step >= st_sec) {
                أرجع NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            اكسر;

        حالة ' ':
            𐡄𐡍 (step >= st_sec) {
                أرجع NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            اكسر;

        افتراضي:
            أرجع NGX_ERROR;
        }

        𐡄𐡍 (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        𐡄𐡍 (value > max) {
            أرجع NGX_ERROR;
        }

        value *= scale;

        𐡄𐡍 (total > NGX_MAX_INT_T_VALUE - value) {
            أرجع NGX_ERROR;
        }

        total += value;

        value = 0;

        طالما (p < last && *p == ' ') {
            p++;
        }
    }

    𐡄𐡍 (!valid) {
        أرجع NGX_ERROR;
    }

    𐡄𐡍 (!is_sec) {
        𐡄𐡍 (value > NGX_MAX_INT_T_VALUE / 1000) {
            أرجع NGX_ERROR;
        }

        value *= 1000;
    }

    𐡄𐡍 (total > NGX_MAX_INT_T_VALUE - value) {
        أرجع NGX_ERROR;
    }

    أرجع total + value;
}
