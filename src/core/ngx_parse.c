
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ssize_t
ngx_parse_ಗಾತ್ರ(ngx_str_t *ಸಾಲು)
{
    u_char   unit;
    ಗಾತ್ರ_t   len;
    ssize_t  ಗಾತ್ರ_2, scale, max;

    len = ಸಾಲು->len;

    ಆದರೆ (len == 0) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    unit = ಸಾಲು->ದತ್ತಾಂಶ[len - 1];

    ಆಯ್ಕೆ (unit) {
    ಸ್ಥಿತಿ 'K':
    ಸ್ಥಿತಿ 'k':
        len--;
        max = NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ / 1024;
        scale = 1024;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'M':
    ಸ್ಥಿತಿ 'm':
        len--;
        max = NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ / (1024 * 1024);
        scale = 1024 * 1024;
        ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
        max = NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ;
        scale = 1;
    }

    ಗಾತ್ರ_2 = ngx_atosz(ಸಾಲು->ದತ್ತಾಂಶ, len);
    ಆದರೆ (ಗಾತ್ರ_2 == NGX_ERROR || ಗಾತ್ರ_2 > max) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಗಾತ್ರ_2 *= scale;

    ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2;
}


off_t
ngx_parse_ಸ್ಥಾನಾಂತರ(ngx_str_t *ಸಾಲು)
{
    u_char  unit;
    off_t   ಸ್ಥಾನಾಂತರ, scale, max;
    ಗಾತ್ರ_t  len;

    len = ಸಾಲು->len;

    ಆದರೆ (len == 0) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    unit = ಸಾಲು->ದತ್ತಾಂಶ[len - 1];

    ಆಯ್ಕೆ (unit) {
    ಸ್ಥಿತಿ 'K':
    ಸ್ಥಿತಿ 'k':
        len--;
        max = NGX_MAX_OFF_T_ಮೌಲ್ಯ / 1024;
        scale = 1024;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'M':
    ಸ್ಥಿತಿ 'm':
        len--;
        max = NGX_MAX_OFF_T_ಮೌಲ್ಯ / (1024 * 1024);
        scale = 1024 * 1024;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'G':
    ಸ್ಥಿತಿ 'g':
        len--;
        max = NGX_MAX_OFF_T_ಮೌಲ್ಯ / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
        max = NGX_MAX_OFF_T_ಮೌಲ್ಯ;
        scale = 1;
    }

    ಸ್ಥಾನಾಂತರ = ngx_atoof(ಸಾಲು->ದತ್ತಾಂಶ, len);
    ಆದರೆ (ಸ್ಥಾನಾಂತರ == NGX_ERROR || ಸ್ಥಾನಾಂತರ > max) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಸ್ಥಾನಾಂತರ *= scale;

    ಹಿಂತಿರುಗಿ ಸ್ಥಾನಾಂತರ;
}


ngx_int_t
ngx_parse_ಸಮಯ(ngx_str_t *ಸಾಲು, ngx_uint_t is_sec)
{
    u_char      *p, *ಕೊನೆಯ;
    ngx_int_t    ಮೌಲ್ಯ, ಒಟ್ಟು, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    ಎಣಿಕೆ {
        st_ಆರಂಭ = 0,
        st_year,
        st_month,
        st_week,
        st_day,
        st_hour,
        st_min,
        st_sec,
        st_msec,
        st_ಕೊನೆಯ
    } ಹೆಜ್ಜೆ_2;

    valid = 0;
    ಮೌಲ್ಯ = 0;
    ಒಟ್ಟು = 0;
    cutoff = NGX_MAX_INT_T_ಮೌಲ್ಯ / 10;
    cutlim = NGX_MAX_INT_T_ಮೌಲ್ಯ % 10;
    ಹೆಜ್ಜೆ_2 = is_sec ? st_ಆರಂಭ : st_month;

    p = ಸಾಲು->ದತ್ತಾಂಶ;
    ಕೊನೆಯ = p + ಸಾಲು->len;

    ಆಗ (p < ಕೊನೆಯ) {

        ಆದರೆ (*p >= '0' && *p <= '9') {
            ಆದರೆ (ಮೌಲ್ಯ >= cutoff && (ಮೌಲ್ಯ > cutoff || *p - '0' > cutlim)) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10 + (*p++ - '0');
            valid = 1;
            ಮುಂದುವರಿ;
        }

        ಆಯ್ಕೆ (*p++) {

        ಸ್ಥಿತಿ 'y':
            ಆದರೆ (ಹೆಜ್ಜೆ_2 > st_ಆರಂಭ) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
            ಹೆಜ್ಜೆ_2 = st_year;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'M':
            ಆದರೆ (ಹೆಜ್ಜೆ_2 >= st_month) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
            ಹೆಜ್ಜೆ_2 = st_month;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'w':
            ಆದರೆ (ಹೆಜ್ಜೆ_2 >= st_week) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
            ಹೆಜ್ಜೆ_2 = st_week;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'd':
            ಆದರೆ (ಹೆಜ್ಜೆ_2 >= st_day) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
            ಹೆಜ್ಜೆ_2 = st_day;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'h':
            ಆದರೆ (ಹೆಜ್ಜೆ_2 >= st_hour) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
            ಹೆಜ್ಜೆ_2 = st_hour;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ / (60 * 60);
            scale = 60 * 60;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'm':
            ಆದರೆ (p < ಕೊನೆಯ && *p == 's') {
                ಆದರೆ (is_sec || ಹೆಜ್ಜೆ_2 >= st_msec) {
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }
                p++;
                ಹೆಜ್ಜೆ_2 = st_msec;
                max = NGX_MAX_INT_T_ಮೌಲ್ಯ;
                scale = 1;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (ಹೆಜ್ಜೆ_2 >= st_min) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
            ಹೆಜ್ಜೆ_2 = st_min;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ / 60;
            scale = 60;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
            ಆದರೆ (ಹೆಜ್ಜೆ_2 >= st_sec) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
            ಹೆಜ್ಜೆ_2 = st_sec;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ;
            scale = 1;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ' ':
            ಆದರೆ (ಹೆಜ್ಜೆ_2 >= st_sec) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
            ಹೆಜ್ಜೆ_2 = st_ಕೊನೆಯ;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ;
            scale = 1;
            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (ಹೆಜ್ಜೆ_2 != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        ಆದರೆ (ಮೌಲ್ಯ > max) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಮೌಲ್ಯ *= scale;

        ಆದರೆ (ಒಟ್ಟು > NGX_MAX_INT_T_ಮೌಲ್ಯ - ಮೌಲ್ಯ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಒಟ್ಟು += ಮೌಲ್ಯ;

        ಮೌಲ್ಯ = 0;

        ಆಗ (p < ಕೊನೆಯ && *p == ' ') {
            p++;
        }
    }

    ಆದರೆ (!valid) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (!is_sec) {
        ಆದರೆ (ಮೌಲ್ಯ > NGX_MAX_INT_T_ಮೌಲ್ಯ / 1000) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಮೌಲ್ಯ *= 1000;
    }

    ಆದರೆ (ಒಟ್ಟು > NGX_MAX_INT_T_ಮೌಲ್ಯ - ಮೌಲ್ಯ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಹಿಂತಿರುಗಿ ಒಟ್ಟು + ಮೌಲ್ಯ;
}
