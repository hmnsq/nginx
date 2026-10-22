
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *வரி)
{
    u_char   unit;
    size_t   len;
    ssize_t  size, scale, max;

    len = வரி->len;

    எனில் (len == 0) {
        திரும்பு NGX_ERROR;
    }

    unit = வரி->data[len - 1];

    தேர்வு (unit) {
    நிலை 'K':
    நிலை 'k':
        len--;
        max = NGX_MAX_SIZE_T_மதிப்பு / 1024;
        scale = 1024;
        நிறுத்து;

    நிலை 'M':
    நிலை 'm':
        len--;
        max = NGX_MAX_SIZE_T_மதிப்பு / (1024 * 1024);
        scale = 1024 * 1024;
        நிறுத்து;

    இயல்பு:
        max = NGX_MAX_SIZE_T_மதிப்பு;
        scale = 1;
    }

    size = ngx_atosz(வரி->data, len);
    எனில் (size == NGX_ERROR || size > max) {
        திரும்பு NGX_ERROR;
    }

    size *= scale;

    திரும்பு size;
}


off_t
ngx_parse_offset(ngx_str_t *வரி)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = வரி->len;

    எனில் (len == 0) {
        திரும்பு NGX_ERROR;
    }

    unit = வரி->data[len - 1];

    தேர்வு (unit) {
    நிலை 'K':
    நிலை 'k':
        len--;
        max = NGX_MAX_OFF_T_மதிப்பு / 1024;
        scale = 1024;
        நிறுத்து;

    நிலை 'M':
    நிலை 'm':
        len--;
        max = NGX_MAX_OFF_T_மதிப்பு / (1024 * 1024);
        scale = 1024 * 1024;
        நிறுத்து;

    நிலை 'G':
    நிலை 'g':
        len--;
        max = NGX_MAX_OFF_T_மதிப்பு / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        நிறுத்து;

    இயல்பு:
        max = NGX_MAX_OFF_T_மதிப்பு;
        scale = 1;
    }

    offset = ngx_atoof(வரி->data, len);
    எனில் (offset == NGX_ERROR || offset > max) {
        திரும்பு NGX_ERROR;
    }

    offset *= scale;

    திரும்பு offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *வரி, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    மதிப்பு, total, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    எண்ணிக்கை {
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
    மதிப்பு = 0;
    total = 0;
    cutoff = NGX_MAX_INT_T_மதிப்பு / 10;
    cutlim = NGX_MAX_INT_T_மதிப்பு % 10;
    step = is_sec ? st_start : st_month;

    p = வரி->data;
    last = p + வரி->len;

    போது (p < last) {

        எனில் (*p >= '0' && *p <= '9') {
            எனில் (மதிப்பு >= cutoff && (மதிப்பு > cutoff || *p - '0' > cutlim)) {
                திரும்பு NGX_ERROR;
            }

            மதிப்பு = மதிப்பு * 10 + (*p++ - '0');
            valid = 1;
            தொடர்;
        }

        தேர்வு (*p++) {

        நிலை 'y':
            எனில் (step > st_start) {
                திரும்பு NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_மதிப்பு / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            நிறுத்து;

        நிலை 'M':
            எனில் (step >= st_month) {
                திரும்பு NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_மதிப்பு / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            நிறுத்து;

        நிலை 'w':
            எனில் (step >= st_week) {
                திரும்பு NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_மதிப்பு / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            நிறுத்து;

        நிலை 'd':
            எனில் (step >= st_day) {
                திரும்பு NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_மதிப்பு / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            நிறுத்து;

        நிலை 'h':
            எனில் (step >= st_hour) {
                திரும்பு NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_மதிப்பு / (60 * 60);
            scale = 60 * 60;
            நிறுத்து;

        நிலை 'm':
            எனில் (p < last && *p == 's') {
                எனில் (is_sec || step >= st_msec) {
                    திரும்பு NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_மதிப்பு;
                scale = 1;
                நிறுத்து;
            }

            எனில் (step >= st_min) {
                திரும்பு NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_மதிப்பு / 60;
            scale = 60;
            நிறுத்து;

        நிலை 's':
            எனில் (step >= st_sec) {
                திரும்பு NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_மதிப்பு;
            scale = 1;
            நிறுத்து;

        நிலை ' ':
            எனில் (step >= st_sec) {
                திரும்பு NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_மதிப்பு;
            scale = 1;
            நிறுத்து;

        இயல்பு:
            திரும்பு NGX_ERROR;
        }

        எனில் (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        எனில் (மதிப்பு > max) {
            திரும்பு NGX_ERROR;
        }

        மதிப்பு *= scale;

        எனில் (total > NGX_MAX_INT_T_மதிப்பு - மதிப்பு) {
            திரும்பு NGX_ERROR;
        }

        total += மதிப்பு;

        மதிப்பு = 0;

        போது (p < last && *p == ' ') {
            p++;
        }
    }

    எனில் (!valid) {
        திரும்பு NGX_ERROR;
    }

    எனில் (!is_sec) {
        எனில் (மதிப்பு > NGX_MAX_INT_T_மதிப்பு / 1000) {
            திரும்பு NGX_ERROR;
        }

        மதிப்பு *= 1000;
    }

    எனில் (total > NGX_MAX_INT_T_மதிப்பு - மதிப்பு) {
        திரும்பு NGX_ERROR;
    }

    திரும்பு total + மதிப்பு;
}
