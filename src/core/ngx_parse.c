
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ssize_t
ngx_parse_ناپ(ngx_str_t *سطر)
{
    u_char   unit;
    ناپ_t   len;
    ssize_t  ناپ, scale, max;

    len = سطر->len;

    إذا (len == 0) {
        أرجع NGX_ERROR;
    }

    unit = سطر->انکڑے[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_ناپ_T_قدر / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_ناپ_T_قدر / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_ناپ_T_قدر;
        scale = 1;
    }

    ناپ = ngx_atosz(سطر->انکڑے, len);
    إذا (ناپ == NGX_ERROR || ناپ > max) {
        أرجع NGX_ERROR;
    }

    ناپ *= scale;

    أرجع ناپ;
}


off_t
ngx_parse_وتھ(ngx_str_t *سطر)
{
    u_char  unit;
    off_t   وتھ, scale, max;
    ناپ_t  len;

    len = سطر->len;

    إذا (len == 0) {
        أرجع NGX_ERROR;
    }

    unit = سطر->انکڑے[len - 1];

    بدل (unit) {
    حالة 'K':
    حالة 'k':
        len--;
        max = NGX_MAX_OFF_T_قدر / 1024;
        scale = 1024;
        اكسر;

    حالة 'M':
    حالة 'm':
        len--;
        max = NGX_MAX_OFF_T_قدر / (1024 * 1024);
        scale = 1024 * 1024;
        اكسر;

    حالة 'G':
    حالة 'g':
        len--;
        max = NGX_MAX_OFF_T_قدر / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        اكسر;

    افتراضي:
        max = NGX_MAX_OFF_T_قدر;
        scale = 1;
    }

    وتھ = ngx_atoof(سطر->انکڑے, len);
    إذا (وتھ == NGX_ERROR || وتھ > max) {
        أرجع NGX_ERROR;
    }

    وتھ *= scale;

    أرجع وتھ;
}


ngx_int_t
ngx_parse_ویلا(ngx_str_t *سطر, ngx_uint_t is_sec)
{
    u_char      *p, *آخری;
    ngx_int_t    قدر, کل, scale;
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
        st_آخری
    } قدم_2;

    valid = 0;
    قدر = 0;
    کل = 0;
    cutoff = NGX_MAX_INT_T_قدر / 10;
    cutlim = NGX_MAX_INT_T_قدر % 10;
    قدم_2 = is_sec ? st_شروع : st_month;

    p = سطر->انکڑے;
    آخری = p + سطر->len;

    طالما (p < آخری) {

        إذا (*p >= '0' && *p <= '9') {
            إذا (قدر >= cutoff && (قدر > cutoff || *p - '0' > cutlim)) {
                أرجع NGX_ERROR;
            }

            قدر = قدر * 10 + (*p++ - '0');
            valid = 1;
            استمر;
        }

        بدل (*p++) {

        حالة 'y':
            إذا (قدم_2 > st_شروع) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_year;
            max = NGX_MAX_INT_T_قدر / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            اكسر;

        حالة 'M':
            إذا (قدم_2 >= st_month) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_month;
            max = NGX_MAX_INT_T_قدر / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            اكسر;

        حالة 'w':
            إذا (قدم_2 >= st_week) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_week;
            max = NGX_MAX_INT_T_قدر / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            اكسر;

        حالة 'd':
            إذا (قدم_2 >= st_day) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_day;
            max = NGX_MAX_INT_T_قدر / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            اكسر;

        حالة 'h':
            إذا (قدم_2 >= st_hour) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_hour;
            max = NGX_MAX_INT_T_قدر / (60 * 60);
            scale = 60 * 60;
            اكسر;

        حالة 'm':
            إذا (p < آخری && *p == 's') {
                إذا (is_sec || قدم_2 >= st_msec) {
                    أرجع NGX_ERROR;
                }
                p++;
                قدم_2 = st_msec;
                max = NGX_MAX_INT_T_قدر;
                scale = 1;
                اكسر;
            }

            إذا (قدم_2 >= st_min) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_min;
            max = NGX_MAX_INT_T_قدر / 60;
            scale = 60;
            اكسر;

        حالة 's':
            إذا (قدم_2 >= st_sec) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_sec;
            max = NGX_MAX_INT_T_قدر;
            scale = 1;
            اكسر;

        حالة ' ':
            إذا (قدم_2 >= st_sec) {
                أرجع NGX_ERROR;
            }
            قدم_2 = st_آخری;
            max = NGX_MAX_INT_T_قدر;
            scale = 1;
            اكسر;

        افتراضي:
            أرجع NGX_ERROR;
        }

        إذا (قدم_2 != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        إذا (قدر > max) {
            أرجع NGX_ERROR;
        }

        قدر *= scale;

        إذا (کل > NGX_MAX_INT_T_قدر - قدر) {
            أرجع NGX_ERROR;
        }

        کل += قدر;

        قدر = 0;

        طالما (p < آخری && *p == ' ') {
            p++;
        }
    }

    إذا (!valid) {
        أرجع NGX_ERROR;
    }

    إذا (!is_sec) {
        إذا (قدر > NGX_MAX_INT_T_قدر / 1000) {
            أرجع NGX_ERROR;
        }

        قدر *= 1000;
    }

    إذا (کل > NGX_MAX_INT_T_قدر - قدر) {
        أرجع NGX_ERROR;
    }

    أرجع کل + قدر;
}
