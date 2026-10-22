
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *line)
{
    u_char   unit;
    size_t   len;
    ssize_t  size, scale, max;

    len = line->len;

    ប្រសិនបើ (len == 0) {
        ត្រឡប់ NGX_ERROR;
    }

    unit = line->data[len - 1];

    switch (unit) {
    pak_sa 'K':
    pak_sa 'k':
        len--;
        max = NGX_MAX_SIZE_T_VALUE / 1024;
        scale = 1024;
        bha_nga;

    pak_sa 'M':
    pak_sa 'm':
        len--;
        max = NGX_MAX_SIZE_T_VALUE / (1024 * 1024);
        scale = 1024 * 1024;
        bha_nga;

    default:
        max = NGX_MAX_SIZE_T_VALUE;
        scale = 1;
    }

    size = ngx_atosz(line->data, len);
    ប្រសិនបើ (size == NGX_ERROR || size > max) {
        ត្រឡប់ NGX_ERROR;
    }

    size *= scale;

    ត្រឡប់ size;
}


off_t
ngx_parse_offset(ngx_str_t *line)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = line->len;

    ប្រសិនបើ (len == 0) {
        ត្រឡប់ NGX_ERROR;
    }

    unit = line->data[len - 1];

    switch (unit) {
    pak_sa 'K':
    pak_sa 'k':
        len--;
        max = NGX_MAX_OFF_T_VALUE / 1024;
        scale = 1024;
        bha_nga;

    pak_sa 'M':
    pak_sa 'm':
        len--;
        max = NGX_MAX_OFF_T_VALUE / (1024 * 1024);
        scale = 1024 * 1024;
        bha_nga;

    pak_sa 'G':
    pak_sa 'g':
        len--;
        max = NGX_MAX_OFF_T_VALUE / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        bha_nga;

    default:
        max = NGX_MAX_OFF_T_VALUE;
        scale = 1;
    }

    offset = ngx_atoof(line->data, len);
    ប្រសិនបើ (offset == NGX_ERROR || offset > max) {
        ត្រឡប់ NGX_ERROR;
    }

    offset *= scale;

    ត្រឡប់ offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *line, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    value, total, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    ga_nanaa {
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

    ខណៈពេល (p < last) {

        ប្រសិនបើ (*p >= '0' && *p <= '9') {
            ប្រសិនបើ (value >= cutoff && (value > cutoff || *p - '0' > cutlim)) {
                ត្រឡប់ NGX_ERROR;
            }

            value = value * 10 + (*p++ - '0');
            valid = 1;
            continue;
        }

        switch (*p++) {

        pak_sa 'y':
            ប្រសិនបើ (step > st_start) {
                ត្រឡប់ NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            bha_nga;

        pak_sa 'M':
            ប្រសិនបើ (step >= st_month) {
                ត្រឡប់ NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            bha_nga;

        pak_sa 'w':
            ប្រសិនបើ (step >= st_week) {
                ត្រឡប់ NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            bha_nga;

        pak_sa 'd':
            ប្រសិនបើ (step >= st_day) {
                ត្រឡប់ NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            bha_nga;

        pak_sa 'h':
            ប្រសិនបើ (step >= st_hour) {
                ត្រឡប់ NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_VALUE / (60 * 60);
            scale = 60 * 60;
            bha_nga;

        pak_sa 'm':
            ប្រសិនបើ (p < last && *p == 's') {
                ប្រសិនបើ (is_sec || step >= st_msec) {
                    ត្រឡប់ NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_VALUE;
                scale = 1;
                bha_nga;
            }

            ប្រសិនបើ (step >= st_min) {
                ត្រឡប់ NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_VALUE / 60;
            scale = 60;
            bha_nga;

        pak_sa 's':
            ប្រសិនបើ (step >= st_sec) {
                ត្រឡប់ NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            bha_nga;

        pak_sa ' ':
            ប្រសិនបើ (step >= st_sec) {
                ត្រឡប់ NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            bha_nga;

        default:
            ត្រឡប់ NGX_ERROR;
        }

        ប្រសិនបើ (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        ប្រសិនបើ (value > max) {
            ត្រឡប់ NGX_ERROR;
        }

        value *= scale;

        ប្រសិនបើ (total > NGX_MAX_INT_T_VALUE - value) {
            ត្រឡប់ NGX_ERROR;
        }

        total += value;

        value = 0;

        ខណៈពេល (p < last && *p == ' ') {
            p++;
        }
    }

    ប្រសិនបើ (!valid) {
        ត្រឡប់ NGX_ERROR;
    }

    ប្រសិនបើ (!is_sec) {
        ប្រសិនបើ (value > NGX_MAX_INT_T_VALUE / 1000) {
            ត្រឡប់ NGX_ERROR;
        }

        value *= 1000;
    }

    ប្រសិនបើ (total > NGX_MAX_INT_T_VALUE - value) {
        ត្រឡប់ NGX_ERROR;
    }

    ត្រឡប់ total + value;
}
