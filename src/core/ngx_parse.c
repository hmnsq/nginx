
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

    གལ་སྲིད (len == 0) {
        ལོག NGX_ERROR;
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
    གལ་སྲིད (size == NGX_ERROR || size > max) {
        ལོག NGX_ERROR;
    }

    size *= scale;

    ལོག size;
}


off_t
ngx_parse_offset(ngx_str_t *line)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = line->len;

    གལ་སྲིད (len == 0) {
        ལོག NGX_ERROR;
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
    གལ་སྲིད (offset == NGX_ERROR || offset > max) {
        ལོག NGX_ERROR;
    }

    offset *= scale;

    ལོག offset;
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

    སྐབས (p < last) {

        གལ་སྲིད (*p >= '0' && *p <= '9') {
            གལ་སྲིད (value >= cutoff && (value > cutoff || *p - '0' > cutlim)) {
                ལོག NGX_ERROR;
            }

            value = value * 10 + (*p++ - '0');
            valid = 1;
            continue;
        }

        switch (*p++) {

        pak_sa 'y':
            གལ་སྲིད (step > st_start) {
                ལོག NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            bha_nga;

        pak_sa 'M':
            གལ་སྲིད (step >= st_month) {
                ལོག NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            bha_nga;

        pak_sa 'w':
            གལ་སྲིད (step >= st_week) {
                ལོག NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            bha_nga;

        pak_sa 'd':
            གལ་སྲིད (step >= st_day) {
                ལོག NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            bha_nga;

        pak_sa 'h':
            གལ་སྲིད (step >= st_hour) {
                ལོག NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_VALUE / (60 * 60);
            scale = 60 * 60;
            bha_nga;

        pak_sa 'm':
            གལ་སྲིད (p < last && *p == 's') {
                གལ་སྲིད (is_sec || step >= st_msec) {
                    ལོག NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_VALUE;
                scale = 1;
                bha_nga;
            }

            གལ་སྲིད (step >= st_min) {
                ལོག NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_VALUE / 60;
            scale = 60;
            bha_nga;

        pak_sa 's':
            གལ་སྲིད (step >= st_sec) {
                ལོག NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            bha_nga;

        pak_sa ' ':
            གལ་སྲིད (step >= st_sec) {
                ལོག NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            bha_nga;

        default:
            ལོག NGX_ERROR;
        }

        གལ་སྲིད (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        གལ་སྲིད (value > max) {
            ལོག NGX_ERROR;
        }

        value *= scale;

        གལ་སྲིད (total > NGX_MAX_INT_T_VALUE - value) {
            ལོག NGX_ERROR;
        }

        total += value;

        value = 0;

        སྐབས (p < last && *p == ' ') {
            p++;
        }
    }

    གལ་སྲིད (!valid) {
        ལོག NGX_ERROR;
    }

    གལ་སྲིད (!is_sec) {
        གལ་སྲིད (value > NGX_MAX_INT_T_VALUE / 1000) {
            ལོག NGX_ERROR;
        }

        value *= 1000;
    }

    གལ་སྲིད (total > NGX_MAX_INT_T_VALUE - value) {
        ལོག NGX_ERROR;
    }

    ལོག total + value;
}
