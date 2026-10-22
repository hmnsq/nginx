
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *લીટી)
{
    u_char   unit;
    size_t   len;
    ssize_t  size, scale, max;

    len = લીટી->len;

    જો (len == 0) {
        પાછા NGX_ERROR;
    }

    unit = લીટી->data[len - 1];

    પસંદગી (unit) {
    સ્થિતિ 'K':
    સ્થિતિ 'k':
        len--;
        max = NGX_MAX_SIZE_T_કિંમત / 1024;
        scale = 1024;
        તોડો;

    સ્થિતિ 'M':
    સ્થિતિ 'm':
        len--;
        max = NGX_MAX_SIZE_T_કિંમત / (1024 * 1024);
        scale = 1024 * 1024;
        તોડો;

    મૂળભૂત:
        max = NGX_MAX_SIZE_T_કિંમત;
        scale = 1;
    }

    size = ngx_atosz(લીટી->data, len);
    જો (size == NGX_ERROR || size > max) {
        પાછા NGX_ERROR;
    }

    size *= scale;

    પાછા size;
}


off_t
ngx_parse_offset(ngx_str_t *લીટી)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = લીટી->len;

    જો (len == 0) {
        પાછા NGX_ERROR;
    }

    unit = લીટી->data[len - 1];

    પસંદગી (unit) {
    સ્થિતિ 'K':
    સ્થિતિ 'k':
        len--;
        max = NGX_MAX_OFF_T_કિંમત / 1024;
        scale = 1024;
        તોડો;

    સ્થિતિ 'M':
    સ્થિતિ 'm':
        len--;
        max = NGX_MAX_OFF_T_કિંમત / (1024 * 1024);
        scale = 1024 * 1024;
        તોડો;

    સ્થિતિ 'G':
    સ્થિતિ 'g':
        len--;
        max = NGX_MAX_OFF_T_કિંમત / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        તોડો;

    મૂળભૂત:
        max = NGX_MAX_OFF_T_કિંમત;
        scale = 1;
    }

    offset = ngx_atoof(લીટી->data, len);
    જો (offset == NGX_ERROR || offset > max) {
        પાછા NGX_ERROR;
    }

    offset *= scale;

    પાછા offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *લીટી, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    કિંમત, total, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    ગણના {
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
    કિંમત = 0;
    total = 0;
    cutoff = NGX_MAX_INT_T_કિંમત / 10;
    cutlim = NGX_MAX_INT_T_કિંમત % 10;
    step = is_sec ? st_start : st_month;

    p = લીટી->data;
    last = p + લીટી->len;

    જ્યારે (p < last) {

        જો (*p >= '0' && *p <= '9') {
            જો (કિંમત >= cutoff && (કિંમત > cutoff || *p - '0' > cutlim)) {
                પાછા NGX_ERROR;
            }

            કિંમત = કિંમત * 10 + (*p++ - '0');
            valid = 1;
            ચાલુ;
        }

        પસંદગી (*p++) {

        સ્થિતિ 'y':
            જો (step > st_start) {
                પાછા NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_કિંમત / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            તોડો;

        સ્થિતિ 'M':
            જો (step >= st_month) {
                પાછા NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_કિંમત / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            તોડો;

        સ્થિતિ 'w':
            જો (step >= st_week) {
                પાછા NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_કિંમત / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            તોડો;

        સ્થિતિ 'd':
            જો (step >= st_day) {
                પાછા NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_કિંમત / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            તોડો;

        સ્થિતિ 'h':
            જો (step >= st_hour) {
                પાછા NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_કિંમત / (60 * 60);
            scale = 60 * 60;
            તોડો;

        સ્થિતિ 'm':
            જો (p < last && *p == 's') {
                જો (is_sec || step >= st_msec) {
                    પાછા NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_કિંમત;
                scale = 1;
                તોડો;
            }

            જો (step >= st_min) {
                પાછા NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_કિંમત / 60;
            scale = 60;
            તોડો;

        સ્થિતિ 's':
            જો (step >= st_sec) {
                પાછા NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_કિંમત;
            scale = 1;
            તોડો;

        સ્થિતિ ' ':
            જો (step >= st_sec) {
                પાછા NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_કિંમત;
            scale = 1;
            તોડો;

        મૂળભૂત:
            પાછા NGX_ERROR;
        }

        જો (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        જો (કિંમત > max) {
            પાછા NGX_ERROR;
        }

        કિંમત *= scale;

        જો (total > NGX_MAX_INT_T_કિંમત - કિંમત) {
            પાછા NGX_ERROR;
        }

        total += કિંમત;

        કિંમત = 0;

        જ્યારે (p < last && *p == ' ') {
            p++;
        }
    }

    જો (!valid) {
        પાછા NGX_ERROR;
    }

    જો (!is_sec) {
        જો (કિંમત > NGX_MAX_INT_T_કિંમત / 1000) {
            પાછા NGX_ERROR;
        }

        કિંમત *= 1000;
    }

    જો (total > NGX_MAX_INT_T_કિંમત - કિંમત) {
        પાછા NGX_ERROR;
    }

    પાછા total + કિંમત;
}
