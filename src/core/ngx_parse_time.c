
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

زمان_t
ngx_parse_http_زمان(u_char *مقدار, اندازه_t len)
{
    u_char      *p, *انتها;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     زمان;
    تعداد {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    انتها = مقدار + len;

#إذا (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#پایان_اگر

    لكل (p = مقدار; p < انتها; p++) {
        إذا (*p == ',') {
            اكسر;
        }

        إذا (*p == ' ') {
            fmt = isoc;
            اكسر;
        }
    }

    لكل (p++; p < انتها; p++) {
        إذا (*p != ' ') {
            اكسر;
        }
    }

    إذا (انتها - p < 18) {
        أرجع NGX_ERROR;
    }

    إذا (fmt != isoc) {
        إذا (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            أرجع NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        إذا (*p == ' ') {
            إذا (انتها - p < 18) {
                أرجع NGX_ERROR;
            }
            fmt = rfc822;

        } وإلا إذا (*p == '-') {
            fmt = rfc850;

        } وإلا {
            أرجع NGX_ERROR;
        }

        p++;
    }

    بدل (*p) {

    حالة 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        اكسر;

    حالة 'F':
        month = 1;
        اكسر;

    حالة 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        اكسر;

    حالة 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        اكسر;

    حالة 'S':
        month = 8;
        اكسر;

    حالة 'O':
        month = 9;
        اكسر;

    حالة 'N':
        month = 10;
        اكسر;

    حالة 'D':
        month = 11;
        اكسر;

    افتراضي:
        أرجع NGX_ERROR;
    }

    p += 3;

    إذا ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        أرجع NGX_ERROR;
    }

    p++;

    إذا (fmt == rfc822) {
        إذا (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            أرجع NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } وإلا إذا (fmt == rfc850) {
        إذا (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            أرجع NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    إذا (fmt == isoc) {
        إذا (*p == ' ') {
            p++;
        }

        إذا (*p < '0' || *p > '9') {
            أرجع NGX_ERROR;
        }

        day = *p++ - '0';

        إذا (*p != ' ') {
            إذا (*p < '0' || *p > '9') {
                أرجع NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        إذا (انتها - p < 14) {
            أرجع NGX_ERROR;
        }
    }

    إذا (*p++ != ' ') {
        أرجع NGX_ERROR;
    }

    إذا (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        أرجع NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    إذا (*p++ != ':') {
        أرجع NGX_ERROR;
    }

    إذا (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        أرجع NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    إذا (*p++ != ':') {
        أرجع NGX_ERROR;
    }

    إذا (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        أرجع NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    إذا (fmt == isoc) {
        p += 2;

        إذا (*p++ != ' ') {
            أرجع NGX_ERROR;
        }

        إذا (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            أرجع NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    إذا (hour > 23 || min > 59 || sec > 59) {
        أرجع NGX_ERROR;
    }

    إذا (day == 29 && month == 1) {
        إذا ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            أرجع NGX_ERROR;
        }

    } وإلا إذا (day > mday[month]) {
        أرجع NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    إذا (--month <= 0) {
        month += 12;
        year -= 1;
    }

    /* Gauss' formula for Gregorian days since March 1, 1 BC */

    زمان = (uint64_t) (
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

#إذا (NGX_زمان_T_اندازه <= 4)

    إذا (زمان > 0x7fffffff) {
        أرجع NGX_ERROR;
    }

#پایان_اگر

    أرجع (زمان_t) زمان;
}
