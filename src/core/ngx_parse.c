
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


ssize_t
ngx_parse_आकार(ngx_str_t *पङ्क्तिः)
{
    u_char   unit;
    आकार_t   len;
    ssize_t  आकार, scale, max;

    len = पङ्क्तिः->len;

    यदि (len == 0) {
        प्रत्यागम NGX_ERROR;
    }

    unit = पङ्क्तिः->दत्त[len - 1];

    विकल्प (unit) {
    पक्ष 'K':
    पक्ष 'k':
        len--;
        max = NGX_MAX_आकार_T_मानम् / 1024;
        scale = 1024;
        भङ्ग;

    पक्ष 'M':
    पक्ष 'm':
        len--;
        max = NGX_MAX_आकार_T_मानम् / (1024 * 1024);
        scale = 1024 * 1024;
        भङ्ग;

    स्वभाव:
        max = NGX_MAX_आकार_T_मानम्;
        scale = 1;
    }

    आकार = ngx_atosz(पङ्क्तिः->दत्त, len);
    यदि (आकार == NGX_ERROR || आकार > max) {
        प्रत्यागम NGX_ERROR;
    }

    आकार *= scale;

    प्रत्यागम आकार;
}


off_t
ngx_parse_विस्थापनम्(ngx_str_t *पङ्क्तिः)
{
    u_char  unit;
    off_t   विस्थापनम्, scale, max;
    आकार_t  len;

    len = पङ्क्तिः->len;

    यदि (len == 0) {
        प्रत्यागम NGX_ERROR;
    }

    unit = पङ्क्तिः->दत्त[len - 1];

    विकल्प (unit) {
    पक्ष 'K':
    पक्ष 'k':
        len--;
        max = NGX_MAX_OFF_T_मानम् / 1024;
        scale = 1024;
        भङ्ग;

    पक्ष 'M':
    पक्ष 'm':
        len--;
        max = NGX_MAX_OFF_T_मानम् / (1024 * 1024);
        scale = 1024 * 1024;
        भङ्ग;

    पक्ष 'G':
    पक्ष 'g':
        len--;
        max = NGX_MAX_OFF_T_मानम् / (1024 * 1024 * 1024);
        scale = 1024 * 1024 * 1024;
        भङ्ग;

    स्वभाव:
        max = NGX_MAX_OFF_T_मानम्;
        scale = 1;
    }

    विस्थापनम् = ngx_atoof(पङ्क्तिः->दत्त, len);
    यदि (विस्थापनम् == NGX_ERROR || विस्थापनम् > max) {
        प्रत्यागम NGX_ERROR;
    }

    विस्थापनम् *= scale;

    प्रत्यागम विस्थापनम्;
}


ngx_int_t
ngx_parse_time(ngx_str_t *पङ्क्तिः, ngx_uint_t is_sec)
{
    u_char      *p, *चरमः;
    ngx_int_t    मानम्, योग, scale;
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
        st_चरमः
    } चरणः_2;

    valid = 0;
    मानम् = 0;
    योग = 0;
    cutoff = NGX_MAX_INT_T_मानम् / 10;
    cutlim = NGX_MAX_INT_T_मानम् % 10;
    चरणः_2 = is_sec ? st_start : st_month;

    p = पङ्क्तिः->दत्त;
    चरमः = p + पङ्क्तिः->len;

    यावत् (p < चरमः) {

        यदि (*p >= '0' && *p <= '9') {
            यदि (मानम् >= cutoff && (मानम् > cutoff || *p - '0' > cutlim)) {
                प्रत्यागम NGX_ERROR;
            }

            मानम् = मानम् * 10 + (*p++ - '0');
            valid = 1;
            अनुवर्त;
        }

        विकल्प (*p++) {

        पक्ष 'y':
            यदि (चरणः_2 > st_start) {
                प्रत्यागम NGX_ERROR;
            }
            चरणः_2 = st_year;
            max = NGX_MAX_INT_T_मानम् / (60 * 60 * 24 * 365);
            scale = 60 * 60 * 24 * 365;
            भङ्ग;

        पक्ष 'M':
            यदि (चरणः_2 >= st_month) {
                प्रत्यागम NGX_ERROR;
            }
            चरणः_2 = st_month;
            max = NGX_MAX_INT_T_मानम् / (60 * 60 * 24 * 30);
            scale = 60 * 60 * 24 * 30;
            भङ्ग;

        पक्ष 'w':
            यदि (चरणः_2 >= st_week) {
                प्रत्यागम NGX_ERROR;
            }
            चरणः_2 = st_week;
            max = NGX_MAX_INT_T_मानम् / (60 * 60 * 24 * 7);
            scale = 60 * 60 * 24 * 7;
            भङ्ग;

        पक्ष 'd':
            यदि (चरणः_2 >= st_day) {
                प्रत्यागम NGX_ERROR;
            }
            चरणः_2 = st_day;
            max = NGX_MAX_INT_T_मानम् / (60 * 60 * 24);
            scale = 60 * 60 * 24;
            भङ्ग;

        पक्ष 'h':
            यदि (चरणः_2 >= st_hour) {
                प्रत्यागम NGX_ERROR;
            }
            चरणः_2 = st_hour;
            max = NGX_MAX_INT_T_मानम् / (60 * 60);
            scale = 60 * 60;
            भङ्ग;

        पक्ष 'm':
            यदि (p < चरमः && *p == 's') {
                यदि (is_sec || चरणः_2 >= st_msec) {
                    प्रत्यागम NGX_ERROR;
                }
                p++;
                चरणः_2 = st_msec;
                max = NGX_MAX_INT_T_मानम्;
                scale = 1;
                भङ्ग;
            }

            यदि (चरणः_2 >= st_min) {
                प्रत्यागम NGX_ERROR;
            }
            चरणः_2 = st_min;
            max = NGX_MAX_INT_T_मानम् / 60;
            scale = 60;
            भङ्ग;

        पक्ष 's':
            यदि (चरणः_2 >= st_sec) {
                प्रत्यागम NGX_ERROR;
            }
            चरणः_2 = st_sec;
            max = NGX_MAX_INT_T_मानम्;
            scale = 1;
            भङ्ग;

        पक्ष ' ':
            यदि (चरणः_2 >= st_sec) {
                प्रत्यागम NGX_ERROR;
            }
            चरणः_2 = st_चरमः;
            max = NGX_MAX_INT_T_मानम्;
            scale = 1;
            भङ्ग;

        स्वभाव:
            प्रत्यागम NGX_ERROR;
        }

        यदि (चरणः_2 != st_msec && !is_sec) {
            scale *= 1000;
            max /= 1000;
        }

        यदि (मानम् > max) {
            प्रत्यागम NGX_ERROR;
        }

        मानम् *= scale;

        यदि (योग > NGX_MAX_INT_T_मानम् - मानम्) {
            प्रत्यागम NGX_ERROR;
        }

        योग += मानम्;

        मानम् = 0;

        यावत् (p < चरमः && *p == ' ') {
            p++;
        }
    }

    यदि (!valid) {
        प्रत्यागम NGX_ERROR;
    }

    यदि (!is_sec) {
        यदि (मानम् > NGX_MAX_INT_T_मानम् / 1000) {
            प्रत्यागम NGX_ERROR;
        }

        मानम् *= 1000;
    }

    यदि (योग > NGX_MAX_INT_T_मानम् - मानम्) {
        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम योग + मानम्;
}
