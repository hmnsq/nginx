
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਸਥਿਰ ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

ਸਮਾਂ_t
ngx_parse_http_ਸਮਾਂ(u_char *ਮੁੱਲ, ਮਾਪ_t len)
{
    u_char      *p, *ਸਿਰਾ;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     ਸਮਾਂ;
    ਗਿਣਤੀ {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    ਸਿਰਾ = ਮੁੱਲ + len;

#ਜੇ (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#ਜੇ_ਅੰਤ

    ਲਈ (p = ਮੁੱਲ; p < ਸਿਰਾ; p++) {
        ਜੇ (*p == ',') {
            ਰੋਕੋ;
        }

        ਜੇ (*p == ' ') {
            fmt = isoc;
            ਰੋਕੋ;
        }
    }

    ਲਈ (p++; p < ਸਿਰਾ; p++) {
        ਜੇ (*p != ' ') {
            ਰੋਕੋ;
        }
    }

    ਜੇ (ਸਿਰਾ - p < 18) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (fmt != isoc) {
        ਜੇ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            ਵਾਪਸ NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        ਜੇ (*p == ' ') {
            ਜੇ (ਸਿਰਾ - p < 18) {
                ਵਾਪਸ NGX_ERROR;
            }
            fmt = rfc822;

        } ਨਹੀਂਤਾਂ ਜੇ (*p == '-') {
            fmt = rfc850;

        } ਨਹੀਂਤਾਂ {
            ਵਾਪਸ NGX_ERROR;
        }

        p++;
    }

    ਚੋਣ (*p) {

    ਹਾਲਤ 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        ਰੋਕੋ;

    ਹਾਲਤ 'F':
        month = 1;
        ਰੋਕੋ;

    ਹਾਲਤ 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        ਰੋਕੋ;

    ਹਾਲਤ 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        ਰੋਕੋ;

    ਹਾਲਤ 'S':
        month = 8;
        ਰੋਕੋ;

    ਹਾਲਤ 'O':
        month = 9;
        ਰੋਕੋ;

    ਹਾਲਤ 'N':
        month = 10;
        ਰੋਕੋ;

    ਹਾਲਤ 'D':
        month = 11;
        ਰੋਕੋ;

    ਮੂਲ:
        ਵਾਪਸ NGX_ERROR;
    }

    p += 3;

    ਜੇ ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        ਵਾਪਸ NGX_ERROR;
    }

    p++;

    ਜੇ (fmt == rfc822) {
        ਜੇ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            ਵਾਪਸ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } ਨਹੀਂਤਾਂ ਜੇ (fmt == rfc850) {
        ਜੇ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            ਵਾਪਸ NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    ਜੇ (fmt == isoc) {
        ਜੇ (*p == ' ') {
            p++;
        }

        ਜੇ (*p < '0' || *p > '9') {
            ਵਾਪਸ NGX_ERROR;
        }

        day = *p++ - '0';

        ਜੇ (*p != ' ') {
            ਜੇ (*p < '0' || *p > '9') {
                ਵਾਪਸ NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        ਜੇ (ਸਿਰਾ - p < 14) {
            ਵਾਪਸ NGX_ERROR;
        }
    }

    ਜੇ (*p++ != ' ') {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ਵਾਪਸ NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ਜੇ (*p++ != ':') {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ਵਾਪਸ NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ਜੇ (*p++ != ':') {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ਵਾਪਸ NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    ਜੇ (fmt == isoc) {
        p += 2;

        ਜੇ (*p++ != ' ') {
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            ਵਾਪਸ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    ਜੇ (hour > 23 || min > 59 || sec > 59) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (day == 29 && month == 1) {
        ਜੇ ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            ਵਾਪਸ NGX_ERROR;
        }

    } ਨਹੀਂਤਾਂ ਜੇ (day > mday[month]) {
        ਵਾਪਸ NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    ਜੇ (--month <= 0) {
        month += 12;
        year -= 1;
    }

    /* Gauss' formula for Gregorian days since March 1, 1 BC */

    ਸਮਾਂ = (uint64_t) (
            /* days in years including leap years since March 1, 1 BC */

            365 * year + year / 4 - year / 100 + year / 400

            /* days before the month */

            + 367 * month / 12 - 30

            /* days before the day */

            + day - 1

            /*
             * 719527 days were between March 1, 1 BC and March 1, 1970,
             * 31 and 28 days were in January and February 1970
             */

            - 719527 + 31 + 28) * 86400 + hour * 3600 + min * 60 + sec;

#ਜੇ (NGX_ਸਮਾਂ_T_ਮਾਪ <= 4)

    ਜੇ (ਸਮਾਂ > 0x7fffffff) {
        ਵਾਪਸ NGX_ERROR;
    }

#ਜੇ_ਅੰਤ

    ਵਾਪਸ (ਸਮਾਂ_t) ਸਮਾਂ;
}
