
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

    nếu (len == 0) {
        trả_về NGX_ERROR;
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
    nếu (size == NGX_ERROR || size > max) {
        trả_về NGX_ERROR;
    }

    size *= scale;

    trả_về size;
}


off_t
ngx_parse_offset(ngx_str_t *line)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = line->len;

    nếu (len == 0) {
        trả_về NGX_ERROR;
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
    nếu (offset == NGX_ERROR || offset > max) {
        trả_về NGX_ERROR;
    }

    offset *= scale;

    trả_về offset;
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

    trong_khi (p < last) {

        nếu (*p >= '0' && *p <= '9') {
            nếu (value >= cutoff && (value > cutoff || *p - '0' > cutlim)) {
                trả_về NGX_ERROR;
            }

            value = value * 10 + (*p++ - '0');
            valid = 1;
            continue;
        }

        switch (*p++) {

        case 'y':
            nếu (step > st_start) {
                trả_về NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            break;

        case 'M':
            nếu (step >= st_month) {
                trả_về NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            break;

        case 'w':
            nếu (step >= st_week) {
                trả_về NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            break;

        case 'd':
            nếu (step >= st_day) {
                trả_về NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_VALUE / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            break;

        case 'h':
            nếu (step >= st_hour) {
                trả_về NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_VALUE / (60 * 60);
            scale = 60 * 60;
            break;

        case 'm':
            nếu (p < last && *p == 's') {
                nếu (is_sec || step >= st_msec) {
                    trả_về NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_VALUE;
                scale = 1;
                break;
            }

            nếu (step >= st_min) {
                trả_về NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_VALUE / 60;
            scale = 60;
            break;

        case 's':
            nếu (step >= st_sec) {
                trả_về NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            break;

        case ' ':
            nếu (step >= st_sec) {
                trả_về NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_VALUE;
            scale = 1;
            break;

        default:
            trả_về NGX_ERROR;
        }

        nếu (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        nếu (value > max) {
            trả_về NGX_ERROR;
        }

        value *= scale;

        nếu (total > NGX_MAX_INT_T_VALUE - value) {
            trả_về NGX_ERROR;
        }

        total += value;

        value = 0;

        trong_khi (p < last && *p == ' ') {
            p++;
        }
    }

    nếu (!valid) {
        trả_về NGX_ERROR;
    }

    nếu (!is_sec) {
        nếu (value > NGX_MAX_INT_T_VALUE / 1000) {
            trả_về NGX_ERROR;
        }

        value *= 1000;
    }

    nếu (total > NGX_MAX_INT_T_VALUE - value) {
        trả_về NGX_ERROR;
    }

    trả_về total + value;
}
