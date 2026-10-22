
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *ओळ)
{
    u_char   unit;
    माप_प्रकार   len;
    ssize_t  size, scale, max;

    len = ओळ->len;

    जर (len == 0) {
        परत NGX_ERROR;
    }

    unit = ओळ->data[len - 1];

    निवड (unit) {
    स्थिती 'K':
    स्थिती 'k':
        len--;
        max = NGX_MAX_SIZE_T_मूल्य / 1024;
        scale = 1024;
        थांबा;

    स्थिती 'M':
    स्थिती 'm':
        len--;
        max = NGX_MAX_SIZE_T_मूल्य / (1024 * 1024);
        scale = 1024 * 1024;
        थांबा;

    पूर्वनिर्धारित:
        max = NGX_MAX_SIZE_T_मूल्य;
        scale = 1;
    }

    size = ngx_atosz(ओळ->data, len);
    जर (size == NGX_ERROR || size > max) {
        परत NGX_ERROR;
    }

    size *= scale;

    परत size;
}


off_t
ngx_parse_offset(ngx_str_t *ओळ)
{
    u_char  unit;
    off_t   offset, scale, max;
    माप_प्रकार  len;

    len = ओळ->len;

    जर (len == 0) {
        परत NGX_ERROR;
    }

    unit = ओळ->data[len - 1];

    निवड (unit) {
    स्थिती 'K':
    स्थिती 'k':
        len--;
        max = NGX_MAX_OFF_T_मूल्य / 1024;
        scale = 1024;
        थांबा;

    स्थिती 'M':
    स्थिती 'm':
        len--;
        max = NGX_MAX_OFF_T_मूल्य / (1024 * 1024);
        scale = 1024 * 1024;
        थांबा;

    स्थिती 'G':
    स्थिती 'g':
        len--;
        max = NGX_MAX_OFF_T_मूल्य / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        थांबा;

    पूर्वनिर्धारित:
        max = NGX_MAX_OFF_T_मूल्य;
        scale = 1;
    }

    offset = ngx_atoof(ओळ->data, len);
    जर (offset == NGX_ERROR || offset > max) {
        परत NGX_ERROR;
    }

    offset *= scale;

    परत offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *ओळ, ngx_uint_t is_sec)
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

    p = ओळ->data;
    last = p + ओळ->len;

    पर्यंत (p < last) {

        जर (*p >= '0' && *p <= '9') {
            जर (मूल्य >= cutoff && (मूल्य > cutoff || *p - '0' > cutlim)) {
                परत NGX_ERROR;
            }

            मूल्य = मूल्य * 10 + (*p++ - '0');
            valid = 1;
            सुरूठेवा;
        }

        निवड (*p++) {

        स्थिती 'y':
            जर (step > st_start) {
                परत NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            थांबा;

        स्थिती 'M':
            जर (step >= st_month) {
                परत NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            थांबा;

        स्थिती 'w':
            जर (step >= st_week) {
                परत NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            थांबा;

        स्थिती 'd':
            जर (step >= st_day) {
                परत NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            थांबा;

        स्थिती 'h':
            जर (step >= st_hour) {
                परत NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60);
            scale = 60 * 60;
            थांबा;

        स्थिती 'm':
            जर (p < last && *p == 's') {
                जर (is_sec || step >= st_msec) {
                    परत NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_मूल्य;
                scale = 1;
                थांबा;
            }

            जर (step >= st_min) {
                परत NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_मूल्य / 60;
            scale = 60;
            थांबा;

        स्थिती 's':
            जर (step >= st_sec) {
                परत NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_मूल्य;
            scale = 1;
            थांबा;

        स्थिती ' ':
            जर (step >= st_sec) {
                परत NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_मूल्य;
            scale = 1;
            थांबा;

        पूर्वनिर्धारित:
            परत NGX_ERROR;
        }

        जर (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        जर (मूल्य > max) {
            परत NGX_ERROR;
        }

        मूल्य *= scale;

        जर (total > NGX_MAX_INT_T_मूल्य - मूल्य) {
            परत NGX_ERROR;
        }

        total += मूल्य;

        मूल्य = 0;

        पर्यंत (p < last && *p == ' ') {
            p++;
        }
    }

    जर (!valid) {
        परत NGX_ERROR;
    }

    जर (!is_sec) {
        जर (मूल्य > NGX_MAX_INT_T_मूल्य / 1000) {
            परत NGX_ERROR;
        }

        मूल्य *= 1000;
    }

    जर (total > NGX_MAX_INT_T_मूल्य - मूल्य) {
        परत NGX_ERROR;
    }

    परत total + मूल्य;
}
