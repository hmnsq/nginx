
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *সারি)
{
    u_char   unit;
    size_t   len;
    ssize_t  size, scale, max;

    len = সারি->len;

    যদি (len == 0) {
        প্রত্যাগম NGX_ERROR;
    }

    unit = সারি->data[len - 1];

    বিকল্প (unit) {
    পক্ষ 'K':
    পক্ষ 'k':
        len--;
        max = NGX_MAX_SIZE_T_মান / 1024;
        scale = 1024;
        ভঙ্গ;

    পক্ষ 'M':
    পক্ষ 'm':
        len--;
        max = NGX_MAX_SIZE_T_মান / (1024 * 1024);
        scale = 1024 * 1024;
        ভঙ্গ;

    স্বভাব:
        max = NGX_MAX_SIZE_T_মান;
        scale = 1;
    }

    size = ngx_atosz(সারি->data, len);
    যদি (size == NGX_ERROR || size > max) {
        প্রত্যাগম NGX_ERROR;
    }

    size *= scale;

    প্রত্যাগম size;
}


off_t
ngx_parse_offset(ngx_str_t *সারি)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = সারি->len;

    যদি (len == 0) {
        প্রত্যাগম NGX_ERROR;
    }

    unit = সারি->data[len - 1];

    বিকল্প (unit) {
    পক্ষ 'K':
    পক্ষ 'k':
        len--;
        max = NGX_MAX_OFF_T_মান / 1024;
        scale = 1024;
        ভঙ্গ;

    পক্ষ 'M':
    পক্ষ 'm':
        len--;
        max = NGX_MAX_OFF_T_মান / (1024 * 1024);
        scale = 1024 * 1024;
        ভঙ্গ;

    পক্ষ 'G':
    পক্ষ 'g':
        len--;
        max = NGX_MAX_OFF_T_মান / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        ভঙ্গ;

    স্বভাব:
        max = NGX_MAX_OFF_T_মান;
        scale = 1;
    }

    offset = ngx_atoof(সারি->data, len);
    যদি (offset == NGX_ERROR || offset > max) {
        প্রত্যাগম NGX_ERROR;
    }

    offset *= scale;

    প্রত্যাগম offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *সারি, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    মান, total, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    গণনা {
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
    মান = 0;
    total = 0;
    cutoff = NGX_MAX_INT_T_মান / 10;
    cutlim = NGX_MAX_INT_T_মান % 10;
    step = is_sec ? st_start : st_month;

    p = সারি->data;
    last = p + সারি->len;

    যাবত্ (p < last) {

        যদি (*p >= '0' && *p <= '9') {
            যদি (মান >= cutoff && (মান > cutoff || *p - '0' > cutlim)) {
                প্রত্যাগম NGX_ERROR;
            }

            মান = মান * 10 + (*p++ - '0');
            valid = 1;
            অনুবর্ত;
        }

        বিকল্প (*p++) {

        পক্ষ 'y':
            যদি (step > st_start) {
                প্রত্যাগম NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_মান / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            ভঙ্গ;

        পক্ষ 'M':
            যদি (step >= st_month) {
                প্রত্যাগম NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_মান / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            ভঙ্গ;

        পক্ষ 'w':
            যদি (step >= st_week) {
                প্রত্যাগম NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_মান / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            ভঙ্গ;

        পক্ষ 'd':
            যদি (step >= st_day) {
                প্রত্যাগম NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_মান / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            ভঙ্গ;

        পক্ষ 'h':
            যদি (step >= st_hour) {
                প্রত্যাগম NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_মান / (60 * 60);
            scale = 60 * 60;
            ভঙ্গ;

        পক্ষ 'm':
            যদি (p < last && *p == 's') {
                যদি (is_sec || step >= st_msec) {
                    প্রত্যাগম NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_মান;
                scale = 1;
                ভঙ্গ;
            }

            যদি (step >= st_min) {
                প্রত্যাগম NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_মান / 60;
            scale = 60;
            ভঙ্গ;

        পক্ষ 's':
            যদি (step >= st_sec) {
                প্রত্যাগম NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_মান;
            scale = 1;
            ভঙ্গ;

        পক্ষ ' ':
            যদি (step >= st_sec) {
                প্রত্যাগম NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_মান;
            scale = 1;
            ভঙ্গ;

        স্বভাব:
            প্রত্যাগম NGX_ERROR;
        }

        যদি (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        যদি (মান > max) {
            প্রত্যাগম NGX_ERROR;
        }

        মান *= scale;

        যদি (total > NGX_MAX_INT_T_মান - মান) {
            প্রত্যাগম NGX_ERROR;
        }

        total += মান;

        মান = 0;

        যাবত্ (p < last && *p == ' ') {
            p++;
        }
    }

    যদি (!valid) {
        প্রত্যাগম NGX_ERROR;
    }

    যদি (!is_sec) {
        যদি (মান > NGX_MAX_INT_T_মান / 1000) {
            প্রত্যাগম NGX_ERROR;
        }

        মান *= 1000;
    }

    যদি (total > NGX_MAX_INT_T_মান - মান) {
        প্রত্যাগম NGX_ERROR;
    }

    প্রত্যাগম total + মান;
}
