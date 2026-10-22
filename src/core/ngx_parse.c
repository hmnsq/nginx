
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


ssize_t
ngx_parse_আকার(ngx_str_t *সারি)
{
    u_char   unit;
    আকার_t   len;
    ssize_t  আকার_2, scale, max;

    len = সারি->len;

    যদি (len == 0) {
        ফেরত NGX_ERROR;
    }

    unit = সারি->তথ্য[len - 1];

    নির্বাচন (unit) {
    ক্ষেত্র 'K':
    ক্ষেত্র 'k':
        len--;
        max = NGX_MAX_আকার_T_মান / 1024;
        scale = 1024;
        বিরতি;

    ক্ষেত্র 'M':
    ক্ষেত্র 'm':
        len--;
        max = NGX_MAX_আকার_T_মান / (1024 * 1024);
        scale = 1024 * 1024;
        বিরতি;

    পূর্বনির্ধারিত:
        max = NGX_MAX_আকার_T_মান;
        scale = 1;
    }

    আকার_2 = ngx_atosz(সারি->তথ্য, len);
    যদি (আকার_2 == NGX_ERROR || আকার_2 > max) {
        ফেরত NGX_ERROR;
    }

    আকার_2 *= scale;

    ফেরত আকার_2;
}


off_t
ngx_parse_offset(ngx_str_t *সারি)
{
    u_char  unit;
    off_t   offset, scale, max;
    আকার_t  len;

    len = সারি->len;

    যদি (len == 0) {
        ফেরত NGX_ERROR;
    }

    unit = সারি->তথ্য[len - 1];

    নির্বাচন (unit) {
    ক্ষেত্র 'K':
    ক্ষেত্র 'k':
        len--;
        max = NGX_MAX_OFF_T_মান / 1024;
        scale = 1024;
        বিরতি;

    ক্ষেত্র 'M':
    ক্ষেত্র 'm':
        len--;
        max = NGX_MAX_OFF_T_মান / (1024 * 1024);
        scale = 1024 * 1024;
        বিরতি;

    ক্ষেত্র 'G':
    ক্ষেত্র 'g':
        len--;
        max = NGX_MAX_OFF_T_মান / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        বিরতি;

    পূর্বনির্ধারিত:
        max = NGX_MAX_OFF_T_মান;
        scale = 1;
    }

    offset = ngx_atoof(সারি->তথ্য, len);
    যদি (offset == NGX_ERROR || offset > max) {
        ফেরত NGX_ERROR;
    }

    offset *= scale;

    ফেরত offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *সারি, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    মান, যোগ, scale;
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
    যোগ = 0;
    cutoff = NGX_MAX_INT_T_মান / 10;
    cutlim = NGX_MAX_INT_T_মান % 10;
    step = is_sec ? st_start : st_month;

    p = সারি->তথ্য;
    last = p + সারি->len;

    যখন (p < last) {

        যদি (*p >= '0' && *p <= '9') {
            যদি (মান >= cutoff && (মান > cutoff || *p - '0' > cutlim)) {
                ফেরত NGX_ERROR;
            }

            মান = মান * 10 + (*p++ - '0');
            valid = 1;
            চালু;
        }

        নির্বাচন (*p++) {

        ক্ষেত্র 'y':
            যদি (step > st_start) {
                ফেরত NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_মান / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            বিরতি;

        ক্ষেত্র 'M':
            যদি (step >= st_month) {
                ফেরত NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_মান / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            বিরতি;

        ক্ষেত্র 'w':
            যদি (step >= st_week) {
                ফেরত NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_মান / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            বিরতি;

        ক্ষেত্র 'd':
            যদি (step >= st_day) {
                ফেরত NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_মান / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            বিরতি;

        ক্ষেত্র 'h':
            যদি (step >= st_hour) {
                ফেরত NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_মান / (60 * 60);
            scale = 60 * 60;
            বিরতি;

        ক্ষেত্র 'm':
            যদি (p < last && *p == 's') {
                যদি (is_sec || step >= st_msec) {
                    ফেরত NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_মান;
                scale = 1;
                বিরতি;
            }

            যদি (step >= st_min) {
                ফেরত NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_মান / 60;
            scale = 60;
            বিরতি;

        ক্ষেত্র 's':
            যদি (step >= st_sec) {
                ফেরত NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_মান;
            scale = 1;
            বিরতি;

        ক্ষেত্র ' ':
            যদি (step >= st_sec) {
                ফেরত NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_মান;
            scale = 1;
            বিরতি;

        পূর্বনির্ধারিত:
            ফেরত NGX_ERROR;
        }

        যদি (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        যদি (মান > max) {
            ফেরত NGX_ERROR;
        }

        মান *= scale;

        যদি (যোগ > NGX_MAX_INT_T_মান - মান) {
            ফেরত NGX_ERROR;
        }

        যোগ += মান;

        মান = 0;

        যখন (p < last && *p == ' ') {
            p++;
        }
    }

    যদি (!valid) {
        ফেরত NGX_ERROR;
    }

    যদি (!is_sec) {
        যদি (মান > NGX_MAX_INT_T_মান / 1000) {
            ফেরত NGX_ERROR;
        }

        মান *= 1000;
    }

    যদি (যোগ > NGX_MAX_INT_T_মান - মান) {
        ফেরত NGX_ERROR;
    }

    ফেরত যোগ + মান;
}
