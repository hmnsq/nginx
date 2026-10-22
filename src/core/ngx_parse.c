
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ssize_t
ngx_parse_size(ngx_str_t *ಸಾಲು)
{
    u_char   unit;
    size_t   len;
    ssize_t  size, scale, max;

    len = ಸಾಲು->len;

    ಯದಿ (len == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    unit = ಸಾಲು->data[len - 1];

    ವಿಕಲ್ಪ (unit) {
    ಪಕ್ಷ 'K':
    ಪಕ್ಷ 'k':
        len--;
        max = NGX_MAX_SIZE_T_ಮೌಲ್ಯ / 1024;
        scale = 1024;
        ಭಙ್ಗ;

    ಪಕ್ಷ 'M':
    ಪಕ್ಷ 'm':
        len--;
        max = NGX_MAX_SIZE_T_ಮೌಲ್ಯ / (1024 * 1024);
        scale = 1024 * 1024;
        ಭಙ್ಗ;

    ಸ್ವಭಾವ:
        max = NGX_MAX_SIZE_T_ಮೌಲ್ಯ;
        scale = 1;
    }

    size = ngx_atosz(ಸಾಲು->data, len);
    ಯದಿ (size == NGX_ERROR || size > max) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    size *= scale;

    ಪ್ರತ್ಯಾಗಮ size;
}


off_t
ngx_parse_offset(ngx_str_t *ಸಾಲು)
{
    u_char  unit;
    off_t   offset, scale, max;
    size_t  len;

    len = ಸಾಲು->len;

    ಯದಿ (len == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    unit = ಸಾಲು->data[len - 1];

    ವಿಕಲ್ಪ (unit) {
    ಪಕ್ಷ 'K':
    ಪಕ್ಷ 'k':
        len--;
        max = NGX_MAX_OFF_T_ಮೌಲ್ಯ / 1024;
        scale = 1024;
        ಭಙ್ಗ;

    ಪಕ್ಷ 'M':
    ಪಕ್ಷ 'm':
        len--;
        max = NGX_MAX_OFF_T_ಮೌಲ್ಯ / (1024 * 1024);
        scale = 1024 * 1024;
        ಭಙ್ಗ;

    ಪಕ್ಷ 'G':
    ಪಕ್ಷ 'g':
        len--;
        max = NGX_MAX_OFF_T_ಮೌಲ್ಯ / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        ಭಙ್ಗ;

    ಸ್ವಭಾವ:
        max = NGX_MAX_OFF_T_ಮೌಲ್ಯ;
        scale = 1;
    }

    offset = ngx_atoof(ಸಾಲು->data, len);
    ಯದಿ (offset == NGX_ERROR || offset > max) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    offset *= scale;

    ಪ್ರತ್ಯಾಗಮ offset;
}


ngx_int_t
ngx_parse_time(ngx_str_t *ಸಾಲು, ngx_uint_t is_sec)
{
    u_char      *p, *last;
    ngx_int_t    ಮೌಲ್ಯ, total, scale;
    ngx_int_t    max, cutoff, cutlim;
    ngx_uint_t   valid;
    ಗಣನಾ {
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
    ಮೌಲ್ಯ = 0;
    total = 0;
    cutoff = NGX_MAX_INT_T_ಮೌಲ್ಯ / 10;
    cutlim = NGX_MAX_INT_T_ಮೌಲ್ಯ % 10;
    step = is_sec ? st_start : st_month;

    p = ಸಾಲು->data;
    last = p + ಸಾಲು->len;

    ಯಾವತ್ (p < last) {

        ಯದಿ (*p >= '0' && *p <= '9') {
            ಯದಿ (ಮೌಲ್ಯ >= cutoff && (ಮೌಲ್ಯ > cutoff || *p - '0' > cutlim)) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10 + (*p++ - '0');
            valid = 1;
            ಅನುವರ್ತ;
        }

        ವಿಕಲ್ಪ (*p++) {

        ಪಕ್ಷ 'y':
            ಯದಿ (step > st_start) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
            step = st_year;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            ಭಙ್ಗ;

        ಪಕ್ಷ 'M':
            ಯದಿ (step >= st_month) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
            step = st_month;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            ಭಙ್ಗ;

        ಪಕ್ಷ 'w':
            ಯದಿ (step >= st_week) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
            step = st_week;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            ಭಙ್ಗ;

        ಪಕ್ಷ 'd':
            ಯದಿ (step >= st_day) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
            step = st_day;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            ಭಙ್ಗ;

        ಪಕ್ಷ 'h':
            ಯದಿ (step >= st_hour) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
            step = st_hour;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ / (60 * 60);
            scale = 60 * 60;
            ಭಙ್ಗ;

        ಪಕ್ಷ 'm':
            ಯದಿ (p < last && *p == 's') {
                ಯದಿ (is_sec || step >= st_msec) {
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }
                p++;
                step = st_msec;
                max = NGX_MAX_INT_T_ಮೌಲ್ಯ;
                scale = 1;
                ಭಙ್ಗ;
            }

            ಯದಿ (step >= st_min) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
            step = st_min;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ / 60;
            scale = 60;
            ಭಙ್ಗ;

        ಪಕ್ಷ 's':
            ಯದಿ (step >= st_sec) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
            step = st_sec;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ;
            scale = 1;
            ಭಙ್ಗ;

        ಪಕ್ಷ ' ':
            ಯದಿ (step >= st_sec) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
            step = st_last;
            max = NGX_MAX_INT_T_ಮೌಲ್ಯ;
            scale = 1;
            ಭಙ್ಗ;

        ಸ್ವಭಾವ:
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (step != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        ಯದಿ (ಮೌಲ್ಯ > max) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಮೌಲ್ಯ *= scale;

        ಯದಿ (total > NGX_MAX_INT_T_ಮೌಲ್ಯ - ಮೌಲ್ಯ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        total += ಮೌಲ್ಯ;

        ಮೌಲ್ಯ = 0;

        ಯಾವತ್ (p < last && *p == ' ') {
            p++;
        }
    }

    ಯದಿ (!valid) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (!is_sec) {
        ಯದಿ (ಮೌಲ್ಯ > NGX_MAX_INT_T_ಮೌಲ್ಯ / 1000) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಮೌಲ್ಯ *= 1000;
    }

    ಯದಿ (total > NGX_MAX_INT_T_ಮೌಲ್ಯ - ಮೌಲ್ಯ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ total + ಮೌಲ್ಯ;
}
