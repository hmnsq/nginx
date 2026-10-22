
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *ଧାଡ଼ି)
{
    u_char   unit;
    size_t   len;
    ssize_t  size, scale, max;

    len = ଧାଡ଼ି->len;

    ଯଦି (len == 0) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    unit = ଧାଡ଼ି->data[len - 1];

    ବାଛ (unit) {
    ସ୍ଥିତି 'K':
    ସ୍ଥିତି 'k':
        len--;
        max = NGX_MAX_SIZE_T_ମୂଲ୍ୟ / 1024;
        scale = 1024;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି 'M':
    ସ୍ଥିତି 'm':
        len--;
        max = NGX_MAX_SIZE_T_ମୂଲ୍ୟ / (1024 * 1024);
        scale = 1024 * 1024;
        ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        max = NGX_MAX_SIZE_T_ମୂଲ୍ୟ;
        scale = 1;
    }

    size = ngx_atosz(ଧାଡ଼ି->data, len);
    ଯଦି (size == NGX_ERROR || size > max) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    size *= scale;

    ଫେରନ୍ତୁ size;
}


off_t
ngx_parse_offset(ngx_str_t *ଧାଡ଼ି)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = ଧାଡ଼ି->len;

    ଯଦି (len == 0) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    unit = ଧାଡ଼ି->data[len - 1];

    ବାଛ (unit) {
    ସ୍ଥିତି 'K':
    ସ୍ଥିତି 'k':
        len--;
        max = NGX_MAX_OFF_T_ମୂଲ୍ୟ / 1024;
        scale = 1024;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି 'M':
    ସ୍ଥିତି 'm':
        len--;
        max = NGX_MAX_OFF_T_ମୂଲ୍ୟ / (1024 * 1024);
        scale = 1024 * 1024;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି 'G':
    ସ୍ଥିତି 'g':
        len--;
        max = NGX_MAX_OFF_T_ମୂଲ୍ୟ / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        max = NGX_MAX_OFF_T_ମୂଲ୍ୟ;
        scale = 1;
    }

    offset = ngx_atoof(ଧାଡ଼ି->data, len);
    ଯଦି (offset == NGX_ERROR || offset > max) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    offset *= scale;

    ଫେରନ୍ତୁ offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *ଧାଡ଼ି, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    ମୂଲ୍ୟ, total, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    ଗଣନା {
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
    ମୂଲ୍ୟ = 0;
    total = 0;
    cutoff = NGX_MAX_INT_T_ମୂଲ୍ୟ / 10;
    cutlim = NGX_MAX_INT_T_ମୂଲ୍ୟ % 10;
    step = is_sec ? st_start : st_month;

    p = ଧାଡ଼ି->data;
    last = p + ଧାଡ଼ି->len;

    ଯେତେବେଳେ (p < last) {

        ଯଦି (*p >= '0' && *p <= '9') {
            ଯଦି (ମୂଲ୍ୟ >= cutoff && (ମୂଲ୍ୟ > cutoff || *p - '0' > cutlim)) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ମୂଲ୍ୟ = ମୂଲ୍ୟ * 10 + (*p++ - '0');
            valid = 1;
            ଜାରି;
        }

        ବାଛ (*p++) {

        ସ୍ଥିତି 'y':
            ଯଦି (step > st_start) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_ମୂଲ୍ୟ / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି 'M':
            ଯଦି (step >= st_month) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_ମୂଲ୍ୟ / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି 'w':
            ଯଦି (step >= st_week) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_ମୂଲ୍ୟ / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି 'd':
            ଯଦି (step >= st_day) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_ମୂଲ୍ୟ / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି 'h':
            ଯଦି (step >= st_hour) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_ମୂଲ୍ୟ / (60 * 60);
            scale = 60 * 60;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି 'm':
            ଯଦି (p < last && *p == 's') {
                ଯଦି (is_sec || step >= st_msec) {
                    ଫେରନ୍ତୁ NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_ମୂଲ୍ୟ;
                scale = 1;
                ଭାଙ୍ଗ;
            }

            ଯଦି (step >= st_min) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_ମୂଲ୍ୟ / 60;
            scale = 60;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି 's':
            ଯଦି (step >= st_sec) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_ମୂଲ୍ୟ;
            scale = 1;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି ' ':
            ଯଦି (step >= st_sec) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_ମୂଲ୍ୟ;
            scale = 1;
            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        ଯଦି (ମୂଲ୍ୟ > max) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ମୂଲ୍ୟ *= scale;

        ଯଦି (total > NGX_MAX_INT_T_ମୂଲ୍ୟ - ମୂଲ୍ୟ) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        total += ମୂଲ୍ୟ;

        ମୂଲ୍ୟ = 0;

        ଯେତେବେଳେ (p < last && *p == ' ') {
            p++;
        }
    }

    ଯଦି (!valid) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (!is_sec) {
        ଯଦି (ମୂଲ୍ୟ > NGX_MAX_INT_T_ମୂଲ୍ୟ / 1000) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ମୂଲ୍ୟ *= 1000;
    }

    ଯଦି (total > NGX_MAX_INT_T_ମୂଲ୍ୟ - ମୂଲ୍ୟ) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଫେରନ୍ତୁ total + ମୂଲ୍ୟ;
}
