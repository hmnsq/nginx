
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ssize_t
ngx_parse_سائز(ngx_str_t *سطر)
{
    u_char   unit;
    سائز_t   len;
    ssize_t  سائز, scale, max;

    len = سطر->len;

    اگر (len == 0) {
        واپس NGX_ERROR;
    }

    unit = سطر->ڈیٹا[len - 1];

    انتخاب (unit) {
    حالت 'K':
    حالت 'k':
        len--;
        max = NGX_MAX_سائز_T_قدر / 1024;
        scale = 1024;
        توڑو;

    حالت 'M':
    حالت 'm':
        len--;
        max = NGX_MAX_سائز_T_قدر / (1024 * 1024);
        scale = 1024 * 1024;
        توڑو;

    طےشدہ:
        max = NGX_MAX_سائز_T_قدر;
        scale = 1;
    }

    سائز = ngx_atosz(سطر->ڈیٹا, len);
    اگر (سائز == NGX_ERROR || سائز > max) {
        واپس NGX_ERROR;
    }

    سائز *= scale;

    واپس سائز;
}


off_t
ngx_parse_انحراف(ngx_str_t *سطر)
{
    u_char  unit;
    off_t   انحراف, scale, max;
    سائز_t  len;

    len = سطر->len;

    اگر (len == 0) {
        واپس NGX_ERROR;
    }

    unit = سطر->ڈیٹا[len - 1];

    انتخاب (unit) {
    حالت 'K':
    حالت 'k':
        len--;
        max = NGX_MAX_OFF_T_قدر / 1024;
        scale = 1024;
        توڑو;

    حالت 'M':
    حالت 'm':
        len--;
        max = NGX_MAX_OFF_T_قدر / (1024 * 1024);
        scale = 1024 * 1024;
        توڑو;

    حالت 'G':
    حالت 'g':
        len--;
        max = NGX_MAX_OFF_T_قدر / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        توڑو;

    طےشدہ:
        max = NGX_MAX_OFF_T_قدر;
        scale = 1;
    }

    انحراف = ngx_atoof(سطر->ڈیٹا, len);
    اگر (انحراف == NGX_ERROR || انحراف > max) {
        واپس NGX_ERROR;
    }

    انحراف *= scale;

    واپس انحراف;
}


ngx_int_t
ngx_parse_وقت(ngx_str_t *سطر, ngx_uint_t is_sec)
{
    u_char      *p, *آخری;
    ngx_int_t    قدر, کل, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    شمار {
        st_آغاز = 0,
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
    قدم_2 = is_sec ? st_آغاز : st_month;

    p = سطر->ڈیٹا;
    آخری = p + سطر->len;

    جب (p < آخری) {

        اگر (*p >= '0' && *p <= '9') {
            اگر (قدر >= cutoff && (قدر > cutoff || *p - '0' > cutlim)) {
                واپس NGX_ERROR;
            }

            قدر = قدر * 10 + (*p++ - '0');
            valid = 1;
            جاری;
        }

        انتخاب (*p++) {

        حالت 'y':
            اگر (قدم_2 > st_آغاز) {
                واپس NGX_ERROR;
            }
            قدم_2 = st_year;
            max = NGX_MAX_INT_T_قدر / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            توڑو;

        حالت 'M':
            اگر (قدم_2 >= st_month) {
                واپس NGX_ERROR;
            }
            قدم_2 = st_month;
            max = NGX_MAX_INT_T_قدر / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            توڑو;

        حالت 'w':
            اگر (قدم_2 >= st_week) {
                واپس NGX_ERROR;
            }
            قدم_2 = st_week;
            max = NGX_MAX_INT_T_قدر / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            توڑو;

        حالت 'd':
            اگر (قدم_2 >= st_day) {
                واپس NGX_ERROR;
            }
            قدم_2 = st_day;
            max = NGX_MAX_INT_T_قدر / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            توڑو;

        حالت 'h':
            اگر (قدم_2 >= st_hour) {
                واپس NGX_ERROR;
            }
            قدم_2 = st_hour;
            max = NGX_MAX_INT_T_قدر / (60 * 60);
            scale = 60 * 60;
            توڑو;

        حالت 'm':
            اگر (p < آخری && *p == 's') {
                اگر (is_sec || قدم_2 >= st_msec) {
                    واپس NGX_ERROR;
                }
                p++;
                قدم_2 = st_msec;
                max = NGX_MAX_INT_T_قدر;
                scale = 1;
                توڑو;
            }

            اگر (قدم_2 >= st_min) {
                واپس NGX_ERROR;
            }
            قدم_2 = st_min;
            max = NGX_MAX_INT_T_قدر / 60;
            scale = 60;
            توڑو;

        حالت 's':
            اگر (قدم_2 >= st_sec) {
                واپس NGX_ERROR;
            }
            قدم_2 = st_sec;
            max = NGX_MAX_INT_T_قدر;
            scale = 1;
            توڑو;

        حالت ' ':
            اگر (قدم_2 >= st_sec) {
                واپس NGX_ERROR;
            }
            قدم_2 = st_آخری;
            max = NGX_MAX_INT_T_قدر;
            scale = 1;
            توڑو;

        طےشدہ:
            واپس NGX_ERROR;
        }

        اگر (قدم_2 != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        اگر (قدر > max) {
            واپس NGX_ERROR;
        }

        قدر *= scale;

        اگر (کل > NGX_MAX_INT_T_قدر - قدر) {
            واپس NGX_ERROR;
        }

        کل += قدر;

        قدر = 0;

        جب (p < آخری && *p == ' ') {
            p++;
        }
    }

    اگر (!valid) {
        واپس NGX_ERROR;
    }

    اگر (!is_sec) {
        اگر (قدر > NGX_MAX_INT_T_قدر / 1000) {
            واپس NGX_ERROR;
        }

        قدر *= 1000;
    }

    اگر (کل > NGX_MAX_INT_T_قدر - قدر) {
        واپس NGX_ERROR;
    }

    واپس کل + قدر;
}
