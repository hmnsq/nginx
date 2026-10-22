
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

    ᰀᰧ (len == 0) {
        ᰀᰧᰌ NGX_ERROR;
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
    ᰀᰧ (size == NGX_ERROR || size > max) {
        ᰀᰧᰌ NGX_ERROR;
    }

    size *= scale;

    ᰀᰧᰌ size;
}


off_t
ngx_parse_offset(ngx_str_t *line)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = line->len;

    ᰀᰧ (len == 0) {
        ᰀᰧᰌ NGX_ERROR;
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
    ᰀᰧ (offset == NGX_ERROR || offset > max) {
        ᰀᰧᰌ NGX_ERROR;
    }

    offset *= scale;

    ᰀᰧᰌ offset;
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

    ᰀᰬ (p < last) {

        ᰀᰧ (*p >= '0' && *p <= '9') {
            ᰀᰧ (value >= cutoff && (value > cutoff || *p - '0' > cutlim)) {
                ᰀᰧᰌ NGX_ERROR;
            }

            value = value * 10 + (*p++ - '0');
            valid = 1;
            continue;
        }

        switch (*p++) {

        pak_sa 'y':
            ᰀᰧ (step > st_start) {
                ᰀᰧᰌ NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            bha_nga;

        pak_sa 'M':
            ᰀᰧ (step >= st_month) {
                ᰀᰧᰌ NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            bha_nga;

        pak_sa 'w':
            ᰀᰧ (step >= st_week) {
                ᰀᰧᰌ NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            bha_nga;

        pak_sa 'd':
            ᰀᰧ (step >= st_day) {
                ᰀᰧᰌ NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            bha_nga;

        pak_sa 'h':
            ᰀᰧ (step >= st_hour) {
                ᰀᰧᰌ NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_VALUE / (60 * 60);
            scale = 60 * 60;
            bha_nga;

        pak_sa 'm':
            ᰀᰧ (p < last && *p == 's') {
                ᰀᰧ (is_sec || step >= st_msec) {
                    ᰀᰧᰌ NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_VALUE;
                scale = 1;
                bha_nga;
            }

            ᰀᰧ (step >= st_min) {
                ᰀᰧᰌ NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_VALUE / 60;
            scale = 60;
            bha_nga;

        pak_sa 's':
            ᰀᰧ (step >= st_sec) {
                ᰀᰧᰌ NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            bha_nga;

        pak_sa ' ':
            ᰀᰧ (step >= st_sec) {
                ᰀᰧᰌ NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            bha_nga;

        default:
            ᰀᰧᰌ NGX_ERROR;
        }

        ᰀᰧ (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        ᰀᰧ (value > max) {
            ᰀᰧᰌ NGX_ERROR;
        }

        value *= scale;

        ᰀᰧ (total > NGX_MAX_INT_T_VALUE - value) {
            ᰀᰧᰌ NGX_ERROR;
        }

        total += value;

        value = 0;

        ᰀᰬ (p < last && *p == ' ') {
            p++;
        }
    }

    ᰀᰧ (!valid) {
        ᰀᰧᰌ NGX_ERROR;
    }

    ᰀᰧ (!is_sec) {
        ᰀᰧ (value > NGX_MAX_INT_T_VALUE / 1000) {
            ᰀᰧᰌ NGX_ERROR;
        }

        value *= 1000;
    }

    ᰀᰧ (total > NGX_MAX_INT_T_VALUE - value) {
        ᰀᰧᰌ NGX_ERROR;
    }

    ᰀᰧᰌ total + value;
}
