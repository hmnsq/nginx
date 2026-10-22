
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *पंक्ति)
{
    u_char   unit;
    माप_प्रकार   len;
    ssize_t  size, scale, max;

    len = पंक्ति->len;

    यदि (len == 0) {
        प्रत्यागम NGX_ERROR;
    }

    unit = पंक्ति->data[len - 1];

    विकल्प (unit) {
    पक्ष 'K':
    पक्ष 'k':
        len--;
        max = NGX_MAX_SIZE_T_मूल्य / 1024;
        scale = 1024;
        भङ्ग;

    पक्ष 'M':
    पक्ष 'm':
        len--;
        max = NGX_MAX_SIZE_T_मूल्य / (1024 * 1024);
        scale = 1024 * 1024;
        भङ्ग;

    स्वभाव:
        max = NGX_MAX_SIZE_T_मूल्य;
        scale = 1;
    }

    size = ngx_atosz(पंक्ति->data, len);
    यदि (size == NGX_ERROR || size > max) {
        प्रत्यागम NGX_ERROR;
    }

    size *= scale;

    प्रत्यागम size;
}


off_t
ngx_parse_offset(ngx_str_t *पंक्ति)
{
    u_char  unit;
    off_t   offset, scale, max;
    माप_प्रकार  len;

    len = पंक्ति->len;

    यदि (len == 0) {
        प्रत्यागम NGX_ERROR;
    }

    unit = पंक्ति->data[len - 1];

    विकल्प (unit) {
    पक्ष 'K':
    पक्ष 'k':
        len--;
        max = NGX_MAX_OFF_T_मूल्य / 1024;
        scale = 1024;
        भङ्ग;

    पक्ष 'M':
    पक्ष 'm':
        len--;
        max = NGX_MAX_OFF_T_मूल्य / (1024 * 1024);
        scale = 1024 * 1024;
        भङ्ग;

    पक्ष 'G':
    पक्ष 'g':
        len--;
        max = NGX_MAX_OFF_T_मूल्य / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        भङ्ग;

    स्वभाव:
        max = NGX_MAX_OFF_T_मूल्य;
        scale = 1;
    }

    offset = ngx_atoof(पंक्ति->data, len);
    यदि (offset == NGX_ERROR || offset > max) {
        प्रत्यागम NGX_ERROR;
    }

    offset *= scale;

    प्रत्यागम offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *पंक्ति, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    मूल्य, total, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    गणना {
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
    मूल्य = 0;
    total = 0;
    cutoff = NGX_MAX_INT_T_मूल्य / 10;
    cutlim = NGX_MAX_INT_T_मूल्य % 10;
    step = is_sec ? st_start : st_month;

    p = पंक्ति->data;
    last = p + पंक्ति->len;

    यावत् (p < last) {

        यदि (*p >= '0' && *p <= '9') {
            यदि (मूल्य >= cutoff && (मूल्य > cutoff || *p - '0' > cutlim)) {
                प्रत्यागम NGX_ERROR;
            }

            मूल्य = मूल्य * 10 + (*p++ - '0');
            valid = 1;
            अनुवर्त;
        }

        विकल्प (*p++) {

        पक्ष 'y':
            यदि (step > st_start) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            भङ्ग;

        पक्ष 'M':
            यदि (step >= st_month) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            भङ्ग;

        पक्ष 'w':
            यदि (step >= st_week) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            भङ्ग;

        पक्ष 'd':
            यदि (step >= st_day) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            भङ्ग;

        पक्ष 'h':
            यदि (step >= st_hour) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60);
            scale = 60 * 60;
            भङ्ग;

        पक्ष 'm':
            यदि (p < last && *p == 's') {
                यदि (is_sec || step >= st_msec) {
                    प्रत्यागम NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_मूल्य;
                scale = 1;
                भङ्ग;
            }

            यदि (step >= st_min) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_मूल्य / 60;
            scale = 60;
            भङ्ग;

        पक्ष 's':
            यदि (step >= st_sec) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_मूल्य;
            scale = 1;
            भङ्ग;

        पक्ष ' ':
            यदि (step >= st_sec) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_मूल्य;
            scale = 1;
            भङ्ग;

        स्वभाव:
            प्रत्यागम NGX_ERROR;
        }

        यदि (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        यदि (मूल्य > max) {
            प्रत्यागम NGX_ERROR;
        }

        मूल्य *= scale;

        यदि (total > NGX_MAX_INT_T_मूल्य - मूल्य) {
            प्रत्यागम NGX_ERROR;
        }

        total += मूल्य;

        मूल्य = 0;

        यावत् (p < last && *p == ' ') {
            p++;
        }
    }

    यदि (!valid) {
        प्रत्यागम NGX_ERROR;
    }

    यदि (!is_sec) {
        यदि (मूल्य > NGX_MAX_INT_T_मूल्य / 1000) {
            प्रत्यागम NGX_ERROR;
        }

        मूल्य *= 1000;
    }

    यदि (total > NGX_MAX_INT_T_मूल्य - मूल्य) {
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम total + मूल्य;
}
