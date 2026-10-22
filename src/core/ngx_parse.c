
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *വരി)
{
    u_char   unit;
    size_t   len;
    ssize_t  size, scale, max;

    len = വരി->len;

    എങ്കിൽ (len == 0) {
        തിരികെ NGX_ERROR;
    }

    unit = വരി->data[len - 1];

    തിരഞ്ഞെടുപ്പ് (unit) {
    അവസ്ഥ 'K':
    അവസ്ഥ 'k':
        len--;
        max = NGX_MAX_SIZE_T_മൂല്യം / 1024;
        scale = 1024;
        നിർത്തുക;

    അവസ്ഥ 'M':
    അവസ്ഥ 'm':
        len--;
        max = NGX_MAX_SIZE_T_മൂല്യം / (1024 * 1024);
        scale = 1024 * 1024;
        നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
        max = NGX_MAX_SIZE_T_മൂല്യം;
        scale = 1;
    }

    size = ngx_atosz(വരി->data, len);
    എങ്കിൽ (size == NGX_ERROR || size > max) {
        തിരികെ NGX_ERROR;
    }

    size *= scale;

    തിരികെ size;
}


off_t
ngx_parse_offset(ngx_str_t *വരി)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = വരി->len;

    എങ്കിൽ (len == 0) {
        തിരികെ NGX_ERROR;
    }

    unit = വരി->data[len - 1];

    തിരഞ്ഞെടുപ്പ് (unit) {
    അവസ്ഥ 'K':
    അവസ്ഥ 'k':
        len--;
        max = NGX_MAX_OFF_T_മൂല്യം / 1024;
        scale = 1024;
        നിർത്തുക;

    അവസ്ഥ 'M':
    അവസ്ഥ 'm':
        len--;
        max = NGX_MAX_OFF_T_മൂല്യം / (1024 * 1024);
        scale = 1024 * 1024;
        നിർത്തുക;

    അവസ്ഥ 'G':
    അവസ്ഥ 'g':
        len--;
        max = NGX_MAX_OFF_T_മൂല്യം / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
        max = NGX_MAX_OFF_T_മൂല്യം;
        scale = 1;
    }

    offset = ngx_atoof(വരി->data, len);
    എങ്കിൽ (offset == NGX_ERROR || offset > max) {
        തിരികെ NGX_ERROR;
    }

    offset *= scale;

    തിരികെ offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *വരി, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    മൂല്യം, total, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    എണ്ണൽ {
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
    മൂല്യം = 0;
    total = 0;
    cutoff = NGX_MAX_INT_T_മൂല്യം / 10;
    cutlim = NGX_MAX_INT_T_മൂല്യം % 10;
    step = is_sec ? st_start : st_month;

    p = വരി->data;
    last = p + വരി->len;

    ആകുമ്പോൾ (p < last) {

        എങ്കിൽ (*p >= '0' && *p <= '9') {
            എങ്കിൽ (മൂല്യം >= cutoff && (മൂല്യം > cutoff || *p - '0' > cutlim)) {
                തിരികെ NGX_ERROR;
            }

            മൂല്യം = മൂല്യം * 10 + (*p++ - '0');
            valid = 1;
            തുടരുക;
        }

        തിരഞ്ഞെടുപ്പ് (*p++) {

        അവസ്ഥ 'y':
            എങ്കിൽ (step > st_start) {
                തിരികെ NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_മൂല്യം / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            നിർത്തുക;

        അവസ്ഥ 'M':
            എങ്കിൽ (step >= st_month) {
                തിരികെ NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_മൂല്യം / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            നിർത്തുക;

        അവസ്ഥ 'w':
            എങ്കിൽ (step >= st_week) {
                തിരികെ NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_മൂല്യം / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            നിർത്തുക;

        അവസ്ഥ 'd':
            എങ്കിൽ (step >= st_day) {
                തിരികെ NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_മൂല്യം / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            നിർത്തുക;

        അവസ്ഥ 'h':
            എങ്കിൽ (step >= st_hour) {
                തിരികെ NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_മൂല്യം / (60 * 60);
            scale = 60 * 60;
            നിർത്തുക;

        അവസ്ഥ 'm':
            എങ്കിൽ (p < last && *p == 's') {
                എങ്കിൽ (is_sec || step >= st_msec) {
                    തിരികെ NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_മൂല്യം;
                scale = 1;
                നിർത്തുക;
            }

            എങ്കിൽ (step >= st_min) {
                തിരികെ NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_മൂല്യം / 60;
            scale = 60;
            നിർത്തുക;

        അവസ്ഥ 's':
            എങ്കിൽ (step >= st_sec) {
                തിരികെ NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_മൂല്യം;
            scale = 1;
            നിർത്തുക;

        അവസ്ഥ ' ':
            എങ്കിൽ (step >= st_sec) {
                തിരികെ NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_മൂല്യം;
            scale = 1;
            നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        എങ്കിൽ (മൂല്യം > max) {
            തിരികെ NGX_ERROR;
        }

        മൂല്യം *= scale;

        എങ്കിൽ (total > NGX_MAX_INT_T_മൂല്യം - മൂല്യം) {
            തിരികെ NGX_ERROR;
        }

        total += മൂല്യം;

        മൂല്യം = 0;

        ആകുമ്പോൾ (p < last && *p == ' ') {
            p++;
        }
    }

    എങ്കിൽ (!valid) {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (!is_sec) {
        എങ്കിൽ (മൂല്യം > NGX_MAX_INT_T_മൂല്യം / 1000) {
            തിരികെ NGX_ERROR;
        }

        മൂല്യം *= 1000;
    }

    എങ്കിൽ (total > NGX_MAX_INT_T_മൂല്യം - മൂല്യം) {
        തിരികെ NGX_ERROR;
    }

    തിരികെ total + മൂല്യം;
}
