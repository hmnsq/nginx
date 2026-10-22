
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

    အကယ်၍ (len == 0) {
        ပြန်ပေး NGX_ERROR;
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
    အကယ်၍ (size == NGX_ERROR || size > max) {
        ပြန်ပေး NGX_ERROR;
    }

    size *= scale;

    ပြန်ပေး size;
}


off_t
ngx_parse_offset(ngx_str_t *line)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = line->len;

    အကယ်၍ (len == 0) {
        ပြန်ပေး NGX_ERROR;
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
    အကယ်၍ (offset == NGX_ERROR || offset > max) {
        ပြန်ပေး NGX_ERROR;
    }

    offset *= scale;

    ပြန်ပေး offset;
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

    နေစဉ် (p < last) {

        အကယ်၍ (*p >= '0' && *p <= '9') {
            အကယ်၍ (value >= cutoff && (value > cutoff || *p - '0' > cutlim)) {
                ပြန်ပေး NGX_ERROR;
            }

            value = value * 10 + (*p++ - '0');
            valid = 1;
            continue;
        }

        switch (*p++) {

        pak_sa 'y':
            အကယ်၍ (step > st_start) {
                ပြန်ပေး NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            bha_nga;

        pak_sa 'M':
            အကယ်၍ (step >= st_month) {
                ပြန်ပေး NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            bha_nga;

        pak_sa 'w':
            အကယ်၍ (step >= st_week) {
                ပြန်ပေး NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            bha_nga;

        pak_sa 'd':
            အကယ်၍ (step >= st_day) {
                ပြန်ပေး NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            bha_nga;

        pak_sa 'h':
            အကယ်၍ (step >= st_hour) {
                ပြန်ပေး NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_VALUE / (60 * 60);
            scale = 60 * 60;
            bha_nga;

        pak_sa 'm':
            အကယ်၍ (p < last && *p == 's') {
                အကယ်၍ (is_sec || step >= st_msec) {
                    ပြန်ပေး NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_VALUE;
                scale = 1;
                bha_nga;
            }

            အကယ်၍ (step >= st_min) {
                ပြန်ပေး NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_VALUE / 60;
            scale = 60;
            bha_nga;

        pak_sa 's':
            အကယ်၍ (step >= st_sec) {
                ပြန်ပေး NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            bha_nga;

        pak_sa ' ':
            အကယ်၍ (step >= st_sec) {
                ပြန်ပေး NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            bha_nga;

        default:
            ပြန်ပေး NGX_ERROR;
        }

        အကယ်၍ (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        အကယ်၍ (value > max) {
            ပြန်ပေး NGX_ERROR;
        }

        value *= scale;

        အကယ်၍ (total > NGX_MAX_INT_T_VALUE - value) {
            ပြန်ပေး NGX_ERROR;
        }

        total += value;

        value = 0;

        နေစဉ် (p < last && *p == ' ') {
            p++;
        }
    }

    အကယ်၍ (!valid) {
        ပြန်ပေး NGX_ERROR;
    }

    အကယ်၍ (!is_sec) {
        အကယ်၍ (value > NGX_MAX_INT_T_VALUE / 1000) {
            ပြန်ပေး NGX_ERROR;
        }

        value *= 1000;
    }

    အကယ်၍ (total > NGX_MAX_INT_T_VALUE - value) {
        ပြန်ပေး NGX_ERROR;
    }

    ပြန်ပေး total + value;
}
