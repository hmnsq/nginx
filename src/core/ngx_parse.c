
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *سطر)
{
    u_char   unit;
    size_t   len;
    ssize_t  size, scale, max;

    len = سطر->len;

    اگر (len == 0) {
        بازگشت NGX_ERROR;
    }

    unit = سطر->data[len - 1];

    انتخاب (unit) {
    حالت 'K':
    حالت 'k':
        len--;
        max = NGX_MAX_SIZE_T_مقدار / 1024;
        scale = 1024;
        بشکن;

    حالت 'M':
    حالت 'm':
        len--;
        max = NGX_MAX_SIZE_T_مقدار / (1024 * 1024);
        scale = 1024 * 1024;
        بشکن;

    پیشفرض:
        max = NGX_MAX_SIZE_T_مقدار;
        scale = 1;
    }

    size = ngx_atosz(سطر->data, len);
    اگر (size == NGX_ERROR || size > max) {
        بازگشت NGX_ERROR;
    }

    size *= scale;

    بازگشت size;
}


off_t
ngx_parse_offset(ngx_str_t *سطر)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = سطر->len;

    اگر (len == 0) {
        بازگشت NGX_ERROR;
    }

    unit = سطر->data[len - 1];

    انتخاب (unit) {
    حالت 'K':
    حالت 'k':
        len--;
        max = NGX_MAX_OFF_T_مقدار / 1024;
        scale = 1024;
        بشکن;

    حالت 'M':
    حالت 'm':
        len--;
        max = NGX_MAX_OFF_T_مقدار / (1024 * 1024);
        scale = 1024 * 1024;
        بشکن;

    حالت 'G':
    حالت 'g':
        len--;
        max = NGX_MAX_OFF_T_مقدار / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        بشکن;

    پیشفرض:
        max = NGX_MAX_OFF_T_مقدار;
        scale = 1;
    }

    offset = ngx_atoof(سطر->data, len);
    اگر (offset == NGX_ERROR || offset > max) {
        بازگشت NGX_ERROR;
    }

    offset *= scale;

    بازگشت offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *سطر, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    مقدار, total, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    شمارش {
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
    مقدار = 0;
    total = 0;
    cutoff = NGX_MAX_INT_T_مقدار / 10;
    cutlim = NGX_MAX_INT_T_مقدار % 10;
    step = is_sec ? st_start : st_month;

    p = سطر->data;
    last = p + سطر->len;

    وقتی (p < last) {

        اگر (*p >= '0' && *p <= '9') {
            اگر (مقدار >= cutoff && (مقدار > cutoff || *p - '0' > cutlim)) {
                بازگشت NGX_ERROR;
            }

            مقدار = مقدار * 10 + (*p++ - '0');
            valid = 1;
            ادامه;
        }

        انتخاب (*p++) {

        حالت 'y':
            اگر (step > st_start) {
                بازگشت NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_مقدار / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            بشکن;

        حالت 'M':
            اگر (step >= st_month) {
                بازگشت NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_مقدار / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            بشکن;

        حالت 'w':
            اگر (step >= st_week) {
                بازگشت NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_مقدار / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            بشکن;

        حالت 'd':
            اگر (step >= st_day) {
                بازگشت NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_مقدار / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            بشکن;

        حالت 'h':
            اگر (step >= st_hour) {
                بازگشت NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_مقدار / (60 * 60);
            scale = 60 * 60;
            بشکن;

        حالت 'm':
            اگر (p < last && *p == 's') {
                اگر (is_sec || step >= st_msec) {
                    بازگشت NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_مقدار;
                scale = 1;
                بشکن;
            }

            اگر (step >= st_min) {
                بازگشت NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_مقدار / 60;
            scale = 60;
            بشکن;

        حالت 's':
            اگر (step >= st_sec) {
                بازگشت NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_مقدار;
            scale = 1;
            بشکن;

        حالت ' ':
            اگر (step >= st_sec) {
                بازگشت NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_مقدار;
            scale = 1;
            بشکن;

        پیشفرض:
            بازگشت NGX_ERROR;
        }

        اگر (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        اگر (مقدار > max) {
            بازگشت NGX_ERROR;
        }

        مقدار *= scale;

        اگر (total > NGX_MAX_INT_T_مقدار - مقدار) {
            بازگشت NGX_ERROR;
        }

        total += مقدار;

        مقدار = 0;

        وقتی (p < last && *p == ' ') {
            p++;
        }
    }

    اگر (!valid) {
        بازگشت NGX_ERROR;
    }

    اگر (!is_sec) {
        اگر (مقدار > NGX_MAX_INT_T_مقدار / 1000) {
            بازگشت NGX_ERROR;
        }

        مقدار *= 1000;
    }

    اگر (total > NGX_MAX_INT_T_مقدار - مقدار) {
        بازگشت NGX_ERROR;
    }

    بازگشت total + مقدار;
}
