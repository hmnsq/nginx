
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ssize_t
ngx_parse_ਮਾਪ(ngx_str_t *ਸਤਰ)
{
    u_char   unit;
    ਮਾਪ_t   len;
    ssize_t  ਮਾਪ, scale, max;

    len = ਸਤਰ->len;

    ਜੇ (len == 0) {
        ਵਾਪਸ NGX_ERROR;
    }

    unit = ਸਤਰ->ਅੰਕੜੇ[len - 1];

    ਚੋਣ (unit) {
    ਹਾਲਤ 'K':
    ਹਾਲਤ 'k':
        len--;
        max = NGX_MAX_ਮਾਪ_T_ਮੁੱਲ / 1024;
        scale = 1024;
        ਰੋਕੋ;

    ਹਾਲਤ 'M':
    ਹਾਲਤ 'm':
        len--;
        max = NGX_MAX_ਮਾਪ_T_ਮੁੱਲ / (1024 * 1024);
        scale = 1024 * 1024;
        ਰੋਕੋ;

    ਮੂਲ:
        max = NGX_MAX_ਮਾਪ_T_ਮੁੱਲ;
        scale = 1;
    }

    ਮਾਪ = ngx_atosz(ਸਤਰ->ਅੰਕੜੇ, len);
    ਜੇ (ਮਾਪ == NGX_ERROR || ਮਾਪ > max) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਮਾਪ *= scale;

    ਵਾਪਸ ਮਾਪ;
}


off_t
ngx_parse_ਵਿੱਥ(ngx_str_t *ਸਤਰ)
{
    u_char  unit;
    off_t   ਵਿੱਥ, scale, max;
    ਮਾਪ_t  len;

    len = ਸਤਰ->len;

    ਜੇ (len == 0) {
        ਵਾਪਸ NGX_ERROR;
    }

    unit = ਸਤਰ->ਅੰਕੜੇ[len - 1];

    ਚੋਣ (unit) {
    ਹਾਲਤ 'K':
    ਹਾਲਤ 'k':
        len--;
        max = NGX_MAX_OFF_T_ਮੁੱਲ / 1024;
        scale = 1024;
        ਰੋਕੋ;

    ਹਾਲਤ 'M':
    ਹਾਲਤ 'm':
        len--;
        max = NGX_MAX_OFF_T_ਮੁੱਲ / (1024 * 1024);
        scale = 1024 * 1024;
        ਰੋਕੋ;

    ਹਾਲਤ 'G':
    ਹਾਲਤ 'g':
        len--;
        max = NGX_MAX_OFF_T_ਮੁੱਲ / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        ਰੋਕੋ;

    ਮੂਲ:
        max = NGX_MAX_OFF_T_ਮੁੱਲ;
        scale = 1;
    }

    ਵਿੱਥ = ngx_atoof(ਸਤਰ->ਅੰਕੜੇ, len);
    ਜੇ (ਵਿੱਥ == NGX_ERROR || ਵਿੱਥ > max) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਵਿੱਥ *= scale;

    ਵਾਪਸ ਵਿੱਥ;
}


ngx_int_t
ngx_parse_ਸਮਾਂ(ngx_str_t *ਸਤਰ, ngx_uint_t is_sec)
{
    u_char      *p, *ਆਖਰੀ;
    ngx_int_t    ਮੁੱਲ, ਕੁੱਲ, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    ਗਿਣਤੀ {
        st_ਸ਼ੁਰੂ = 0,
        st_year,
        st_month,
        st_week,
        st_day,
        st_hour,
        st_min,
        st_sec,
        st_msec,
        st_ਆਖਰੀ
    } ਕਦਮ_2;

    valid = 0;
    ਮੁੱਲ = 0;
    ਕੁੱਲ = 0;
    cutoff = NGX_MAX_INT_T_ਮੁੱਲ / 10;
    cutlim = NGX_MAX_INT_T_ਮੁੱਲ % 10;
    ਕਦਮ_2 = is_sec ? st_ਸ਼ੁਰੂ : st_month;

    p = ਸਤਰ->ਅੰਕੜੇ;
    ਆਖਰੀ = p + ਸਤਰ->len;

    ਜਦੋਂ (p < ਆਖਰੀ) {

        ਜੇ (*p >= '0' && *p <= '9') {
            ਜੇ (ਮੁੱਲ >= cutoff && (ਮੁੱਲ > cutoff || *p - '0' > cutlim)) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਮੁੱਲ = ਮੁੱਲ * 10 + (*p++ - '0');
            valid = 1;
            ਜਾਰੀ;
        }

        ਚੋਣ (*p++) {

        ਹਾਲਤ 'y':
            ਜੇ (ਕਦਮ_2 > st_ਸ਼ੁਰੂ) {
                ਵਾਪਸ NGX_ERROR;
            }
            ਕਦਮ_2 = st_year;
            max = NGX_MAX_INT_T_ਮੁੱਲ / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            ਰੋਕੋ;

        ਹਾਲਤ 'M':
            ਜੇ (ਕਦਮ_2 >= st_month) {
                ਵਾਪਸ NGX_ERROR;
            }
            ਕਦਮ_2 = st_month;
            max = NGX_MAX_INT_T_ਮੁੱਲ / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            ਰੋਕੋ;

        ਹਾਲਤ 'w':
            ਜੇ (ਕਦਮ_2 >= st_week) {
                ਵਾਪਸ NGX_ERROR;
            }
            ਕਦਮ_2 = st_week;
            max = NGX_MAX_INT_T_ਮੁੱਲ / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            ਰੋਕੋ;

        ਹਾਲਤ 'd':
            ਜੇ (ਕਦਮ_2 >= st_day) {
                ਵਾਪਸ NGX_ERROR;
            }
            ਕਦਮ_2 = st_day;
            max = NGX_MAX_INT_T_ਮੁੱਲ / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            ਰੋਕੋ;

        ਹਾਲਤ 'h':
            ਜੇ (ਕਦਮ_2 >= st_hour) {
                ਵਾਪਸ NGX_ERROR;
            }
            ਕਦਮ_2 = st_hour;
            max = NGX_MAX_INT_T_ਮੁੱਲ / (60 * 60);
            scale = 60 * 60;
            ਰੋਕੋ;

        ਹਾਲਤ 'm':
            ਜੇ (p < ਆਖਰੀ && *p == 's') {
                ਜੇ (is_sec || ਕਦਮ_2 >= st_msec) {
                    ਵਾਪਸ NGX_ERROR;
                }
                p++;
                ਕਦਮ_2 = st_msec;
                max = NGX_MAX_INT_T_ਮੁੱਲ;
                scale = 1;
                ਰੋਕੋ;
            }

            ਜੇ (ਕਦਮ_2 >= st_min) {
                ਵਾਪਸ NGX_ERROR;
            }
            ਕਦਮ_2 = st_min;
            max = NGX_MAX_INT_T_ਮੁੱਲ / 60;
            scale = 60;
            ਰੋਕੋ;

        ਹਾਲਤ 's':
            ਜੇ (ਕਦਮ_2 >= st_sec) {
                ਵਾਪਸ NGX_ERROR;
            }
            ਕਦਮ_2 = st_sec;
            max = NGX_MAX_INT_T_ਮੁੱਲ;
            scale = 1;
            ਰੋਕੋ;

        ਹਾਲਤ ' ':
            ਜੇ (ਕਦਮ_2 >= st_sec) {
                ਵਾਪਸ NGX_ERROR;
            }
            ਕਦਮ_2 = st_ਆਖਰੀ;
            max = NGX_MAX_INT_T_ਮੁੱਲ;
            scale = 1;
            ਰੋਕੋ;

        ਮੂਲ:
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (ਕਦਮ_2 != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        ਜੇ (ਮੁੱਲ > max) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਮੁੱਲ *= scale;

        ਜੇ (ਕੁੱਲ > NGX_MAX_INT_T_ਮੁੱਲ - ਮੁੱਲ) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਕੁੱਲ += ਮੁੱਲ;

        ਮੁੱਲ = 0;

        ਜਦੋਂ (p < ਆਖਰੀ && *p == ' ') {
            p++;
        }
    }

    ਜੇ (!valid) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (!is_sec) {
        ਜੇ (ਮੁੱਲ > NGX_MAX_INT_T_ਮੁੱਲ / 1000) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਮੁੱਲ *= 1000;
    }

    ਜੇ (ਕੁੱਲ > NGX_MAX_INT_T_ਮੁੱਲ - ਮੁੱਲ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਵਾਪਸ ਕੁੱਲ + ਮੁੱਲ;
}
