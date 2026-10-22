
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

    ᬬᬾᬦ᭄ (len == 0) {
        ᬯᬮᬶ NGX_ERROR;
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
    ᬬᬾᬦ᭄ (size == NGX_ERROR || size > max) {
        ᬯᬮᬶ NGX_ERROR;
    }

    size *= scale;

    ᬯᬮᬶ size;
}


off_t
ngx_parse_offset(ngx_str_t *line)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = line->len;

    ᬬᬾᬦ᭄ (len == 0) {
        ᬯᬮᬶ NGX_ERROR;
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
    ᬬᬾᬦ᭄ (offset == NGX_ERROR || offset > max) {
        ᬯᬮᬶ NGX_ERROR;
    }

    offset *= scale;

    ᬯᬮᬶ offset;
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

    ᬲᭂᬤᭂᬮ (p < last) {

        ᬬᬾᬦ᭄ (*p >= '0' && *p <= '9') {
            ᬬᬾᬦ᭄ (value >= cutoff && (value > cutoff || *p - '0' > cutlim)) {
                ᬯᬮᬶ NGX_ERROR;
            }

            value = value * 10 + (*p++ - '0');
            valid = 1;
            continue;
        }

        switch (*p++) {

        pak_sa 'y':
            ᬬᬾᬦ᭄ (step > st_start) {
                ᬯᬮᬶ NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            bha_nga;

        pak_sa 'M':
            ᬬᬾᬦ᭄ (step >= st_month) {
                ᬯᬮᬶ NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            bha_nga;

        pak_sa 'w':
            ᬬᬾᬦ᭄ (step >= st_week) {
                ᬯᬮᬶ NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            bha_nga;

        pak_sa 'd':
            ᬬᬾᬦ᭄ (step >= st_day) {
                ᬯᬮᬶ NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            bha_nga;

        pak_sa 'h':
            ᬬᬾᬦ᭄ (step >= st_hour) {
                ᬯᬮᬶ NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_VALUE / (60 * 60);
            scale = 60 * 60;
            bha_nga;

        pak_sa 'm':
            ᬬᬾᬦ᭄ (p < last && *p == 's') {
                ᬬᬾᬦ᭄ (is_sec || step >= st_msec) {
                    ᬯᬮᬶ NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_VALUE;
                scale = 1;
                bha_nga;
            }

            ᬬᬾᬦ᭄ (step >= st_min) {
                ᬯᬮᬶ NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_VALUE / 60;
            scale = 60;
            bha_nga;

        pak_sa 's':
            ᬬᬾᬦ᭄ (step >= st_sec) {
                ᬯᬮᬶ NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            bha_nga;

        pak_sa ' ':
            ᬬᬾᬦ᭄ (step >= st_sec) {
                ᬯᬮᬶ NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            bha_nga;

        default:
            ᬯᬮᬶ NGX_ERROR;
        }

        ᬬᬾᬦ᭄ (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        ᬬᬾᬦ᭄ (value > max) {
            ᬯᬮᬶ NGX_ERROR;
        }

        value *= scale;

        ᬬᬾᬦ᭄ (total > NGX_MAX_INT_T_VALUE - value) {
            ᬯᬮᬶ NGX_ERROR;
        }

        total += value;

        value = 0;

        ᬲᭂᬤᭂᬮ (p < last && *p == ' ') {
            p++;
        }
    }

    ᬬᬾᬦ᭄ (!valid) {
        ᬯᬮᬶ NGX_ERROR;
    }

    ᬬᬾᬦ᭄ (!is_sec) {
        ᬬᬾᬦ᭄ (value > NGX_MAX_INT_T_VALUE / 1000) {
            ᬯᬮᬶ NGX_ERROR;
        }

        value *= 1000;
    }

    ᬬᬾᬦ᭄ (total > NGX_MAX_INT_T_VALUE - value) {
        ᬯᬮᬶ NGX_ERROR;
    }

    ᬯᬮᬶ total + value;
}
