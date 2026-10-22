
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *पङ्क्ति)
{
    u_char   unit;
    माप_प्रकार   len;
    ssize_t  size, scale, max;

    len = पङ्क्ति->len;

    यदि (len == 0) {
        फर्काऊ NGX_ERROR;
    }

    unit = पङ्क्ति->data[len - 1];

    विकल्प (unit) {
    पक्ष 'K':
    पक्ष 'k':
        len--;
        max = NGX_MAX_SIZE_T_मान / 1024;
        scale = 1024;
        रोक;

    पक्ष 'M':
    पक्ष 'm':
        len--;
        max = NGX_MAX_SIZE_T_मान / (1024 * 1024);
        scale = 1024 * 1024;
        रोक;

    स्वभाव:
        max = NGX_MAX_SIZE_T_मान;
        scale = 1;
    }

    size = ngx_atosz(पङ्क्ति->data, len);
    यदि (size == NGX_ERROR || size > max) {
        फर्काऊ NGX_ERROR;
    }

    size *= scale;

    फर्काऊ size;
}


off_t
ngx_parse_offset(ngx_str_t *पङ्क्ति)
{
    u_char  unit;
    off_t   offset, scale, max;
    माप_प्रकार  len;

    len = पङ्क्ति->len;

    यदि (len == 0) {
        फर्काऊ NGX_ERROR;
    }

    unit = पङ्क्ति->data[len - 1];

    विकल्प (unit) {
    पक्ष 'K':
    पक्ष 'k':
        len--;
        max = NGX_MAX_OFF_T_मान / 1024;
        scale = 1024;
        रोक;

    पक्ष 'M':
    पक्ष 'm':
        len--;
        max = NGX_MAX_OFF_T_मान / (1024 * 1024);
        scale = 1024 * 1024;
        रोक;

    पक्ष 'G':
    पक्ष 'g':
        len--;
        max = NGX_MAX_OFF_T_मान / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        रोक;

    स्वभाव:
        max = NGX_MAX_OFF_T_मान;
        scale = 1;
    }

    offset = ngx_atoof(पङ्क्ति->data, len);
    यदि (offset == NGX_ERROR || offset > max) {
        फर्काऊ NGX_ERROR;
    }

    offset *= scale;

    फर्काऊ offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *पङ्क्ति, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    मान, total, scale;
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
    मान = 0;
    total = 0;
    cutoff = NGX_MAX_INT_T_मान / 10;
    cutlim = NGX_MAX_INT_T_मान % 10;
    step = is_sec ? st_start : st_month;

    p = पङ्क्ति->data;
    last = p + पङ्क्ति->len;

    जबसम्म (p < last) {

        यदि (*p >= '0' && *p <= '9') {
            यदि (मान >= cutoff && (मान > cutoff || *p - '0' > cutlim)) {
                फर्काऊ NGX_ERROR;
            }

            मान = मान * 10 + (*p++ - '0');
            valid = 1;
            जारी;
        }

        विकल्प (*p++) {

        पक्ष 'y':
            यदि (step > st_start) {
                फर्काऊ NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_मान / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            रोक;

        पक्ष 'M':
            यदि (step >= st_month) {
                फर्काऊ NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_मान / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            रोक;

        पक्ष 'w':
            यदि (step >= st_week) {
                फर्काऊ NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_मान / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            रोक;

        पक्ष 'd':
            यदि (step >= st_day) {
                फर्काऊ NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_मान / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            रोक;

        पक्ष 'h':
            यदि (step >= st_hour) {
                फर्काऊ NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_मान / (60 * 60);
            scale = 60 * 60;
            रोक;

        पक्ष 'm':
            यदि (p < last && *p == 's') {
                यदि (is_sec || step >= st_msec) {
                    फर्काऊ NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_मान;
                scale = 1;
                रोक;
            }

            यदि (step >= st_min) {
                फर्काऊ NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_मान / 60;
            scale = 60;
            रोक;

        पक्ष 's':
            यदि (step >= st_sec) {
                फर्काऊ NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_मान;
            scale = 1;
            रोक;

        पक्ष ' ':
            यदि (step >= st_sec) {
                फर्काऊ NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_मान;
            scale = 1;
            रोक;

        स्वभाव:
            फर्काऊ NGX_ERROR;
        }

        यदि (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        यदि (मान > max) {
            फर्काऊ NGX_ERROR;
        }

        मान *= scale;

        यदि (total > NGX_MAX_INT_T_मान - मान) {
            फर्काऊ NGX_ERROR;
        }

        total += मान;

        मान = 0;

        जबसम्म (p < last && *p == ' ') {
            p++;
        }
    }

    यदि (!valid) {
        फर्काऊ NGX_ERROR;
    }

    यदि (!is_sec) {
        यदि (मान > NGX_MAX_INT_T_मान / 1000) {
            फर्काऊ NGX_ERROR;
        }

        मान *= 1000;
    }

    यदि (total > NGX_MAX_INT_T_मान - मान) {
        फर्काऊ NGX_ERROR;
    }

    फर्काऊ total + मान;
}
