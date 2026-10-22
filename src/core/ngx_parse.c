
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


ssize_t
ngx_parse_आकार(ngx_str_t *पंक्ति)
{
    u_char   unit;
    माप_प्रकार   len;
    ssize_t  आकार, scale, max;

    len = पंक्ति->len;

    अगर (len == 0) {
        वापस NGX_ERROR;
    }

    unit = पंक्ति->तथ्य[len - 1];

    चयन (unit) {
    हाल 'K':
    हाल 'k':
        len--;
        max = NGX_MAX_आकार_T_मूल्य / 1024;
        scale = 1024;
        अवरोध;

    हाल 'M':
    हाल 'm':
        len--;
        max = NGX_MAX_आकार_T_मूल्य / (1024 * 1024);
        scale = 1024 * 1024;
        अवरोध;

    शेष:
        max = NGX_MAX_आकार_T_मूल्य;
        scale = 1;
    }

    आकार = ngx_atosz(पंक्ति->तथ्य, len);
    अगर (आकार == NGX_ERROR || आकार > max) {
        वापस NGX_ERROR;
    }

    आकार *= scale;

    वापस आकार;
}


off_t
ngx_parse_offset(ngx_str_t *पंक्ति)
{
    u_char  unit;
    off_t   offset, scale, max;
    माप_प्रकार  len;

    len = पंक्ति->len;

    अगर (len == 0) {
        वापस NGX_ERROR;
    }

    unit = पंक्ति->तथ्य[len - 1];

    चयन (unit) {
    हाल 'K':
    हाल 'k':
        len--;
        max = NGX_MAX_OFF_T_मूल्य / 1024;
        scale = 1024;
        अवरोध;

    हाल 'M':
    हाल 'm':
        len--;
        max = NGX_MAX_OFF_T_मूल्य / (1024 * 1024);
        scale = 1024 * 1024;
        अवरोध;

    हाल 'G':
    हाल 'g':
        len--;
        max = NGX_MAX_OFF_T_मूल्य / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        अवरोध;

    शेष:
        max = NGX_MAX_OFF_T_मूल्य;
        scale = 1;
    }

    offset = ngx_atoof(पंक्ति->तथ्य, len);
    अगर (offset == NGX_ERROR || offset > max) {
        वापस NGX_ERROR;
    }

    offset *= scale;

    वापस offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *पंक्ति, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    मूल्य, योग, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    क्रमागत {
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
    योग = 0;
    cutoff = NGX_MAX_INT_T_मूल्य / 10;
    cutlim = NGX_MAX_INT_T_मूल्य % 10;
    step = is_sec ? st_start : st_month;

    p = पंक्ति->तथ्य;
    last = p + पंक्ति->len;

    जबतक (p < last) {

        अगर (*p >= '0' && *p <= '9') {
            अगर (मूल्य >= cutoff && (मूल्य > cutoff || *p - '0' > cutlim)) {
                वापस NGX_ERROR;
            }

            मूल्य = मूल्य * 10 + (*p++ - '0');
            valid = 1;
            जारी;
        }

        चयन (*p++) {

        हाल 'y':
            अगर (step > st_start) {
                वापस NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            अवरोध;

        हाल 'M':
            अगर (step >= st_month) {
                वापस NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            अवरोध;

        हाल 'w':
            अगर (step >= st_week) {
                वापस NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            अवरोध;

        हाल 'd':
            अगर (step >= st_day) {
                वापस NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            अवरोध;

        हाल 'h':
            अगर (step >= st_hour) {
                वापस NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_मूल्य / (60 * 60);
            scale = 60 * 60;
            अवरोध;

        हाल 'm':
            अगर (p < last && *p == 's') {
                अगर (is_sec || step >= st_msec) {
                    वापस NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_मूल्य;
                scale = 1;
                अवरोध;
            }

            अगर (step >= st_min) {
                वापस NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_मूल्य / 60;
            scale = 60;
            अवरोध;

        हाल 's':
            अगर (step >= st_sec) {
                वापस NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_मूल्य;
            scale = 1;
            अवरोध;

        हाल ' ':
            अगर (step >= st_sec) {
                वापस NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_मूल्य;
            scale = 1;
            अवरोध;

        शेष:
            वापस NGX_ERROR;
        }

        अगर (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        अगर (मूल्य > max) {
            वापस NGX_ERROR;
        }

        मूल्य *= scale;

        अगर (योग > NGX_MAX_INT_T_मूल्य - मूल्य) {
            वापस NGX_ERROR;
        }

        योग += मूल्य;

        मूल्य = 0;

        जबतक (p < last && *p == ' ') {
            p++;
        }
    }

    अगर (!valid) {
        वापस NGX_ERROR;
    }

    अगर (!is_sec) {
        अगर (मूल्य > NGX_MAX_INT_T_मूल्य / 1000) {
            वापस NGX_ERROR;
        }

        मूल्य *= 1000;
    }

    अगर (योग > NGX_MAX_INT_T_मूल्य - मूल्य) {
        वापस NGX_ERROR;
    }

    वापस योग + मूल्य;
}
