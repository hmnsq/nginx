
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *पङ्क्तिः)
{
    u_char   unit;
    size_t   len;
    ssize_t  size, scale, max;

    len = पङ्क्तिः->len;

    जइ (len == 0) {
        प्रत्यागम NGX_ERROR;
    }

    unit = पङ्क्तिः->data[len - 1];

    विकल्प (unit) {
    पक्ष 'K':
    पक्ष 'k':
        len--;
        max = NGX_MAX_SIZE_T_मानम् / 1024;
        scale = 1024;
        भङ्ग;

    पक्ष 'M':
    पक्ष 'm':
        len--;
        max = NGX_MAX_SIZE_T_मानम् / (1024 * 1024);
        scale = 1024 * 1024;
        भङ्ग;

    स्वभाव:
        max = NGX_MAX_SIZE_T_मानम्;
        scale = 1;
    }

    size = ngx_atosz(पङ्क्तिः->data, len);
    जइ (size == NGX_ERROR || size > max) {
        प्रत्यागम NGX_ERROR;
    }

    size *= scale;

    प्रत्यागम size;
}


off_t
ngx_parse_offset(ngx_str_t *पङ्क्तिः)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = पङ्क्तिः->len;

    जइ (len == 0) {
        प्रत्यागम NGX_ERROR;
    }

    unit = पङ्क्तिः->data[len - 1];

    विकल्प (unit) {
    पक्ष 'K':
    पक्ष 'k':
        len--;
        max = NGX_MAX_OFF_T_मानम् / 1024;
        scale = 1024;
        भङ्ग;

    पक्ष 'M':
    पक्ष 'm':
        len--;
        max = NGX_MAX_OFF_T_मानम् / (1024 * 1024);
        scale = 1024 * 1024;
        भङ्ग;

    पक्ष 'G':
    पक्ष 'g':
        len--;
        max = NGX_MAX_OFF_T_मानम् / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        भङ्ग;

    स्वभाव:
        max = NGX_MAX_OFF_T_मानम्;
        scale = 1;
    }

    offset = ngx_atoof(पङ्क्तिः->data, len);
    जइ (offset == NGX_ERROR || offset > max) {
        प्रत्यागम NGX_ERROR;
    }

    offset *= scale;

    प्रत्यागम offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *पङ्क्तिः, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    मानम्, total, scale;
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
    मानम् = 0;
    total = 0;
    cutoff = NGX_MAX_INT_T_मानम् / 10;
    cutlim = NGX_MAX_INT_T_मानम् % 10;
    step = is_sec ? st_start : st_month;

    p = पङ्क्तिः->data;
    last = p + पङ्क्तिः->len;

    जाव (p < last) {

        जइ (*p >= '0' && *p <= '9') {
            जइ (मानम् >= cutoff && (मानम् > cutoff || *p - '0' > cutlim)) {
                प्रत्यागम NGX_ERROR;
            }

            मानम् = मानम् * 10 + (*p++ - '0');
            valid = 1;
            अनुवर्त;
        }

        विकल्प (*p++) {

        पक्ष 'y':
            जइ (step > st_start) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_मानम् / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            भङ्ग;

        पक्ष 'M':
            जइ (step >= st_month) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_मानम् / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            भङ्ग;

        पक्ष 'w':
            जइ (step >= st_week) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_मानम् / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            भङ्ग;

        पक्ष 'd':
            जइ (step >= st_day) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_मानम् / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            भङ्ग;

        पक्ष 'h':
            जइ (step >= st_hour) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_मानम् / (60 * 60);
            scale = 60 * 60;
            भङ्ग;

        पक्ष 'm':
            जइ (p < last && *p == 's') {
                जइ (is_sec || step >= st_msec) {
                    प्रत्यागम NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_मानम्;
                scale = 1;
                भङ्ग;
            }

            जइ (step >= st_min) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_मानम् / 60;
            scale = 60;
            भङ्ग;

        पक्ष 's':
            जइ (step >= st_sec) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_मानम्;
            scale = 1;
            भङ्ग;

        पक्ष ' ':
            जइ (step >= st_sec) {
                प्रत्यागम NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_मानम्;
            scale = 1;
            भङ्ग;

        स्वभाव:
            प्रत्यागम NGX_ERROR;
        }

        जइ (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        जइ (मानम् > max) {
            प्रत्यागम NGX_ERROR;
        }

        मानम् *= scale;

        जइ (total > NGX_MAX_INT_T_मानम् - मानम्) {
            प्रत्यागम NGX_ERROR;
        }

        total += मानम्;

        मानम् = 0;

        जाव (p < last && *p == ' ') {
            p++;
        }
    }

    जइ (!valid) {
        प्रत्यागम NGX_ERROR;
    }

    जइ (!is_sec) {
        जइ (मानम् > NGX_MAX_INT_T_मानम् / 1000) {
            प्रत्यागम NGX_ERROR;
        }

        मानम् *= 1000;
    }

    जइ (total > NGX_MAX_INT_T_मानम् - मानम्) {
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम total + मानम्;
}
