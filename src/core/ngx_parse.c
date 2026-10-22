
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


ssize_t
ngx_parse_కొలత(ngx_str_t *పంక్తి)
{
    u_char   unit;
    కొలత_t   len;
    ssize_t  కొలత, scale, max;

    len = పంక్తి->len;

    ఒకవేళ (len == 0) {
        తిరిగి NGX_ERROR;
    }

    unit = పంక్తి->దత్తాంశం[len - 1];

    ఎంపిక (unit) {
    స్థితి 'K':
    స్థితి 'k':
        len--;
        max = NGX_MAX_కొలత_T_విలువ / 1024;
        scale = 1024;
        ఆపు;

    స్థితి 'M':
    స్థితి 'm':
        len--;
        max = NGX_MAX_కొలత_T_విలువ / (1024 * 1024);
        scale = 1024 * 1024;
        ఆపు;

    అప్రమేయ:
        max = NGX_MAX_కొలత_T_విలువ;
        scale = 1;
    }

    కొలత = ngx_atosz(పంక్తి->దత్తాంశం, len);
    ఒకవేళ (కొలత == NGX_ERROR || కొలత > max) {
        తిరిగి NGX_ERROR;
    }

    కొలత *= scale;

    తిరిగి కొలత;
}


off_t
ngx_parse_స్థానభ్రంశం(ngx_str_t *పంక్తి)
{
    u_char  unit;
    off_t   స్థానభ్రంశం, scale, max;
    కొలత_t  len;

    len = పంక్తి->len;

    ఒకవేళ (len == 0) {
        తిరిగి NGX_ERROR;
    }

    unit = పంక్తి->దత్తాంశం[len - 1];

    ఎంపిక (unit) {
    స్థితి 'K':
    స్థితి 'k':
        len--;
        max = NGX_MAX_OFF_T_విలువ / 1024;
        scale = 1024;
        ఆపు;

    స్థితి 'M':
    స్థితి 'm':
        len--;
        max = NGX_MAX_OFF_T_విలువ / (1024 * 1024);
        scale = 1024 * 1024;
        ఆపు;

    స్థితి 'G':
    స్థితి 'g':
        len--;
        max = NGX_MAX_OFF_T_విలువ / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        ఆపు;

    అప్రమేయ:
        max = NGX_MAX_OFF_T_విలువ;
        scale = 1;
    }

    స్థానభ్రంశం = ngx_atoof(పంక్తి->దత్తాంశం, len);
    ఒకవేళ (స్థానభ్రంశం == NGX_ERROR || స్థానభ్రంశం > max) {
        తిరిగి NGX_ERROR;
    }

    స్థానభ్రంశం *= scale;

    తిరిగి స్థానభ్రంశం;
}


ngx_int_t
ngx_parse_సమయం(ngx_str_t *పంక్తి, ngx_uint_t is_sec)
{
    u_char      *p, *చివరి;
    ngx_int_t    విలువ, మొత్తం, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    గణన {
        st_ప్రారంభం = 0,
        st_year,
        st_month,
        st_week,
        st_day,
        st_hour,
        st_min,
        st_sec,
        st_msec,
        st_చివరి
    } అడుగు_2;

    valid = 0;
    విలువ = 0;
    మొత్తం = 0;
    cutoff = NGX_MAX_INT_T_విలువ / 10;
    cutlim = NGX_MAX_INT_T_విలువ % 10;
    అడుగు_2 = is_sec ? st_ప్రారంభం : st_month;

    p = పంక్తి->దత్తాంశం;
    చివరి = p + పంక్తి->len;

    అయితే (p < చివరి) {

        ఒకవేళ (*p >= '0' && *p <= '9') {
            ఒకవేళ (విలువ >= cutoff && (విలువ > cutoff || *p - '0' > cutlim)) {
                తిరిగి NGX_ERROR;
            }

            విలువ = విలువ * 10 + (*p++ - '0');
            valid = 1;
            కొనసాగు;
        }

        ఎంపిక (*p++) {

        స్థితి 'y':
            ఒకవేళ (అడుగు_2 > st_ప్రారంభం) {
                తిరిగి NGX_ERROR;
            }
            అడుగు_2 = st_year;
            max = NGX_MAX_INT_T_విలువ / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            ఆపు;

        స్థితి 'M':
            ఒకవేళ (అడుగు_2 >= st_month) {
                తిరిగి NGX_ERROR;
            }
            అడుగు_2 = st_month;
            max = NGX_MAX_INT_T_విలువ / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            ఆపు;

        స్థితి 'w':
            ఒకవేళ (అడుగు_2 >= st_week) {
                తిరిగి NGX_ERROR;
            }
            అడుగు_2 = st_week;
            max = NGX_MAX_INT_T_విలువ / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            ఆపు;

        స్థితి 'd':
            ఒకవేళ (అడుగు_2 >= st_day) {
                తిరిగి NGX_ERROR;
            }
            అడుగు_2 = st_day;
            max = NGX_MAX_INT_T_విలువ / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            ఆపు;

        స్థితి 'h':
            ఒకవేళ (అడుగు_2 >= st_hour) {
                తిరిగి NGX_ERROR;
            }
            అడుగు_2 = st_hour;
            max = NGX_MAX_INT_T_విలువ / (60 * 60);
            scale = 60 * 60;
            ఆపు;

        స్థితి 'm':
            ఒకవేళ (p < చివరి && *p == 's') {
                ఒకవేళ (is_sec || అడుగు_2 >= st_msec) {
                    తిరిగి NGX_ERROR;
                }
                p++;
                అడుగు_2 = st_msec;
                max = NGX_MAX_INT_T_విలువ;
                scale = 1;
                ఆపు;
            }

            ఒకవేళ (అడుగు_2 >= st_min) {
                తిరిగి NGX_ERROR;
            }
            అడుగు_2 = st_min;
            max = NGX_MAX_INT_T_విలువ / 60;
            scale = 60;
            ఆపు;

        స్థితి 's':
            ఒకవేళ (అడుగు_2 >= st_sec) {
                తిరిగి NGX_ERROR;
            }
            అడుగు_2 = st_sec;
            max = NGX_MAX_INT_T_విలువ;
            scale = 1;
            ఆపు;

        స్థితి ' ':
            ఒకవేళ (అడుగు_2 >= st_sec) {
                తిరిగి NGX_ERROR;
            }
            అడుగు_2 = st_చివరి;
            max = NGX_MAX_INT_T_విలువ;
            scale = 1;
            ఆపు;

        అప్రమేయ:
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (అడుగు_2 != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        ఒకవేళ (విలువ > max) {
            తిరిగి NGX_ERROR;
        }

        విలువ *= scale;

        ఒకవేళ (మొత్తం > NGX_MAX_INT_T_విలువ - విలువ) {
            తిరిగి NGX_ERROR;
        }

        మొత్తం += విలువ;

        విలువ = 0;

        అయితే (p < చివరి && *p == ' ') {
            p++;
        }
    }

    ఒకవేళ (!valid) {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (!is_sec) {
        ఒకవేళ (విలువ > NGX_MAX_INT_T_విలువ / 1000) {
            తిరిగి NGX_ERROR;
        }

        విలువ *= 1000;
    }

    ఒకవేళ (మొత్తం > NGX_MAX_INT_T_విలువ - విలువ) {
        తిరిగి NGX_ERROR;
    }

    తిరిగి మొత్తం + విలువ;
}
