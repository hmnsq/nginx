
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

    만약 (len == 0) {
        반환 NGX_ERROR;
    }

    unit = line->data[len - 1];

    switch (unit) {
    case 'K':
    case 'k':
        len--;
        max = NGX_MAX_SIZE_T_VALUE / 1024;
        scale = 1024;
        break;

    case 'M':
    case 'm':
        len--;
        max = NGX_MAX_SIZE_T_VALUE / (1024 * 1024);
        scale = 1024 * 1024;
        break;

    default:
        max = NGX_MAX_SIZE_T_VALUE;
        scale = 1;
    }

    size = ngx_atosz(line->data, len);
    만약 (size == NGX_ERROR || size > max) {
        반환 NGX_ERROR;
    }

    size *= scale;

    반환 size;
}


off_t
ngx_parse_offset(ngx_str_t *line)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = line->len;

    만약 (len == 0) {
        반환 NGX_ERROR;
    }

    unit = line->data[len - 1];

    switch (unit) {
    case 'K':
    case 'k':
        len--;
        max = NGX_MAX_OFF_T_VALUE / 1024;
        scale = 1024;
        break;

    case 'M':
    case 'm':
        len--;
        max = NGX_MAX_OFF_T_VALUE / (1024 * 1024);
        scale = 1024 * 1024;
        break;

    case 'G':
    case 'g':
        len--;
        max = NGX_MAX_OFF_T_VALUE / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        break;

    default:
        max = NGX_MAX_OFF_T_VALUE;
        scale = 1;
    }

    offset = ngx_atoof(line->data, len);
    만약 (offset == NGX_ERROR || offset > max) {
        반환 NGX_ERROR;
    }

    offset *= scale;

    반환 offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *line, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    value, total, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    enum {
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

    동안 (p < last) {

        만약 (*p >= '0' && *p <= '9') {
            만약 (value >= cutoff && (value > cutoff || *p - '0' > cutlim)) {
                반환 NGX_ERROR;
            }

            value = value * 10 + (*p++ - '0');
            valid = 1;
            continue;
        }

        switch (*p++) {

        case 'y':
            만약 (step > st_start) {
                반환 NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            break;

        case 'M':
            만약 (step >= st_month) {
                반환 NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            break;

        case 'w':
            만약 (step >= st_week) {
                반환 NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            break;

        case 'd':
            만약 (step >= st_day) {
                반환 NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            break;

        case 'h':
            만약 (step >= st_hour) {
                반환 NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_VALUE / (60 * 60);
            scale = 60 * 60;
            break;

        case 'm':
            만약 (p < last && *p == 's') {
                만약 (is_sec || step >= st_msec) {
                    반환 NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_VALUE;
                scale = 1;
                break;
            }

            만약 (step >= st_min) {
                반환 NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_VALUE / 60;
            scale = 60;
            break;

        case 's':
            만약 (step >= st_sec) {
                반환 NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            break;

        case ' ':
            만약 (step >= st_sec) {
                반환 NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            break;

        default:
            반환 NGX_ERROR;
        }

        만약 (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        만약 (value > max) {
            반환 NGX_ERROR;
        }

        value *= scale;

        만약 (total > NGX_MAX_INT_T_VALUE - value) {
            반환 NGX_ERROR;
        }

        total += value;

        value = 0;

        동안 (p < last && *p == ' ') {
            p++;
        }
    }

    만약 (!valid) {
        반환 NGX_ERROR;
    }

    만약 (!is_sec) {
        만약 (value > NGX_MAX_INT_T_VALUE / 1000) {
            반환 NGX_ERROR;
        }

        value *= 1000;
    }

    만약 (total > NGX_MAX_INT_T_VALUE - value) {
        반환 NGX_ERROR;
    }

    반환 total + value;
}
