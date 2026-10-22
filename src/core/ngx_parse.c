
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ssize_t
ngx_parse_اندازه(ngx_str_t *سطر)
{
    u_char   unit;
    اندازه_t   len;
    ssize_t  اندازه, scale, max;

    len = سطر->len;

    إذا (len == 0) {
        أرجع NGX_ERROR;
    }

    unit = سطر->داده[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_اندازه_T_مقدار / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_اندازه_T_مقدار / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_اندازه_T_مقدار;
        scale = 1;
    }

    اندازه = ngx_atosz(سطر->داده, len);
    إذا (اندازه == NGX_ERROR || اندازه > max) {
        أرجع NGX_ERROR;
    }

    اندازه *= scale;

    أرجع اندازه;
}


off_t
ngx_parse_جابه_جایی(ngx_str_t *سطر)
{
    u_char  unit;
    off_t   جابه_جایی, scale, max;
    اندازه_t  len;

    len = سطر->len;

    إذا (len == 0) {
        أرجع NGX_ERROR;
    }

    unit = سطر->داده[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_OFF_T_مقدار / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_OFF_T_مقدار / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    حالة 'G':
    حالة 'g':
        len--;
        max = NGX_MAX_OFF_T_مقدار / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_OFF_T_مقدار;
        scale = 1;
    }

    جابه_جایی = ngx_atoof(سطر->داده, len);
    إذا (جابه_جایی == NGX_ERROR || جابه_جایی > max) {
        أرجع NGX_ERROR;
    }

    جابه_جایی *= scale;

    أرجع جابه_جایی;
}


ngx_int_t
ngx_parse_زمان(ngx_str_t *سطر, ngx_uint_t is_sec)
{
    u_char      *p, *آخرین;
    ngx_int_t    مقدار, مجموع, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    تعداد {
        st_شروع = 0,
        st_year,
        st_month,
        st_week,
        st_day,
        st_hour,
        st_min,
        st_sec,
        st_msec,
        st_آخرین
    } گام_2;

    valid = 0;
    مقدار = 0;
    مجموع = 0;
    cutoff = NGX_MAX_INT_T_مقدار / 10;
    cutlim = NGX_MAX_INT_T_مقدار % 10;
    گام_2 = is_sec ? st_شروع : st_month;

    p = سطر->داده;
    آخرین = p + سطر->len;

    طالما (p < آخرین) {

        إذا (*p >= '0' && *p <= '9') {
            إذا (مقدار >= cutoff && (مقدار > cutoff || *p - '0' > cutlim)) {
                أرجع NGX_ERROR;
            }

            مقدار = مقدار * 10 + (*p++ - '0');
            valid = 1;
            استمر;
        }

        بدل (*p++) {

        حالة 'y':
            إذا (گام_2 > st_شروع) {
                أرجع NGX_ERROR;
            }
            گام_2 = st_year;
            max = NGX_MAX_INT_T_مقدار / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            اكسر;

        حالة 'M':
            إذا (گام_2 >= st_month) {
                أرجع NGX_ERROR;
            }
            گام_2 = st_month;
            max = NGX_MAX_INT_T_مقدار / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            اكسر;

        حالة 'w':
            إذا (گام_2 >= st_week) {
                أرجع NGX_ERROR;
            }
            گام_2 = st_week;
            max = NGX_MAX_INT_T_مقدار / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            اكسر;

        حالة 'd':
            إذا (گام_2 >= st_day) {
                أرجع NGX_ERROR;
            }
            گام_2 = st_day;
            max = NGX_MAX_INT_T_مقدار / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            اكسر;

        حالة 'h':
            إذا (گام_2 >= st_hour) {
                أرجع NGX_ERROR;
            }
            گام_2 = st_hour;
            max = NGX_MAX_INT_T_مقدار / (60 * 60);
            scale = 60 * 60;
            اكسر;

        حالة 'm':
            إذا (p < آخرین && *p == 's') {
                إذا (is_sec || گام_2 >= st_msec) {
                    أرجع NGX_ERROR;
                }
                p++;
                گام_2 = st_msec;
                max = NGX_MAX_INT_T_مقدار;
                scale = 1;
                اكسر;
            }

            إذا (گام_2 >= st_min) {
                أرجع NGX_ERROR;
            }
            گام_2 = st_min;
            max = NGX_MAX_INT_T_مقدار / 60;
            scale = 60;
            اكسر;

        حالة 's':
            إذا (گام_2 >= st_sec) {
                أرجع NGX_ERROR;
            }
            گام_2 = st_sec;
            max = NGX_MAX_INT_T_مقدار;
            scale = 1;
            اكسر;

        حالة ' ':
            إذا (گام_2 >= st_sec) {
                أرجع NGX_ERROR;
            }
            گام_2 = st_آخرین;
            max = NGX_MAX_INT_T_مقدار;
            scale = 1;
            اكسر;

        افتراضي:
            أرجع NGX_ERROR;
        }

        إذا (گام_2 != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        إذا (مقدار > max) {
            أرجع NGX_ERROR;
        }

        مقدار *= scale;

        إذا (مجموع > NGX_MAX_INT_T_مقدار - مقدار) {
            أرجع NGX_ERROR;
        }

        مجموع += مقدار;

        مقدار = 0;

        طالما (p < آخرین && *p == ' ') {
            p++;
        }
    }

    إذا (!valid) {
        أرجع NGX_ERROR;
    }

    إذا (!is_sec) {
        إذا (مقدار > NGX_MAX_INT_T_مقدار / 1000) {
            أرجع NGX_ERROR;
        }

        مقدار *= 1000;
    }

    إذا (مجموع > NGX_MAX_INT_T_مقدار - مقدار) {
        أرجع NGX_ERROR;
    }

    أرجع مجموع + مقدار;
}
