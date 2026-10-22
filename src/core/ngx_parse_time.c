
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


static ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

time_t
ngx_parse_http_time(u_char *value, size_t len)
{
    u_char      *p, *end;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     time;
    ga_nanaa {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    end = value + len;

#ᤀᤡ᤺ (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#endif

    for (p = value; p < end; p++) {
        ᤀᤡ᤺ (*p == ',') {
            bha_nga;
        }

        ᤀᤡ᤺ (*p == ' ') {
            fmt = isoc;
            bha_nga;
        }
    }

    for (p++; p < end; p++) {
        ᤀᤡ᤺ (*p != ' ') {
            bha_nga;
        }
    }

    ᤀᤡ᤺ (end - p < 18) {
        ᤀᤡᤴ NGX_ERROR;
    }

    ᤀᤡ᤺ (fmt != isoc) {
        ᤀᤡ᤺ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            ᤀᤡᤴ NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        ᤀᤡ᤺ (*p == ' ') {
            ᤀᤡ᤺ (end - p < 18) {
                ᤀᤡᤴ NGX_ERROR;
            }
            fmt = rfc822;

        } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (*p == '-') {
            fmt = rfc850;

        } ᤀᤡ᤺ᤆᤡ {
            ᤀᤡᤴ NGX_ERROR;
        }

        p++;
    }

    switch (*p) {

    pak_sa 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        bha_nga;

    pak_sa 'F':
        month = 1;
        bha_nga;

    pak_sa 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        bha_nga;

    pak_sa 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        bha_nga;

    pak_sa 'S':
        month = 8;
        bha_nga;

    pak_sa 'O':
        month = 9;
        bha_nga;

    pak_sa 'N':
        month = 10;
        bha_nga;

    pak_sa 'D':
        month = 11;
        bha_nga;

    default:
        ᤀᤡᤴ NGX_ERROR;
    }

    p += 3;

    ᤀᤡ᤺ ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        ᤀᤡᤴ NGX_ERROR;
    }

    p++;

    ᤀᤡ᤺ (fmt == rfc822) {
        ᤀᤡ᤺ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            ᤀᤡᤴ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (fmt == rfc850) {
        ᤀᤡ᤺ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            ᤀᤡᤴ NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    ᤀᤡ᤺ (fmt == isoc) {
        ᤀᤡ᤺ (*p == ' ') {
            p++;
        }

        ᤀᤡ᤺ (*p < '0' || *p > '9') {
            ᤀᤡᤴ NGX_ERROR;
        }

        day = *p++ - '0';

        ᤀᤡ᤺ (*p != ' ') {
            ᤀᤡ᤺ (*p < '0' || *p > '9') {
                ᤀᤡᤴ NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        ᤀᤡ᤺ (end - p < 14) {
            ᤀᤡᤴ NGX_ERROR;
        }
    }

    ᤀᤡ᤺ (*p++ != ' ') {
        ᤀᤡᤴ NGX_ERROR;
    }

    ᤀᤡ᤺ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ᤀᤡᤴ NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ᤀᤡ᤺ (*p++ != ':') {
        ᤀᤡᤴ NGX_ERROR;
    }

    ᤀᤡ᤺ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ᤀᤡᤴ NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ᤀᤡ᤺ (*p++ != ':') {
        ᤀᤡᤴ NGX_ERROR;
    }

    ᤀᤡ᤺ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ᤀᤡᤴ NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    ᤀᤡ᤺ (fmt == isoc) {
        p += 2;

        ᤀᤡ᤺ (*p++ != ' ') {
            ᤀᤡᤴ NGX_ERROR;
        }

        ᤀᤡ᤺ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            ᤀᤡᤴ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    ᤀᤡ᤺ (hour > 23 || min > 59 || sec > 59) {
        ᤀᤡᤴ NGX_ERROR;
    }

    ᤀᤡ᤺ (day == 29 && month == 1) {
        ᤀᤡ᤺ ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            ᤀᤡᤴ NGX_ERROR;
        }

    } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (day > mday[month]) {
        ᤀᤡᤴ NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    ᤀᤡ᤺ (--month <= 0) {
        month += 12;
        year -= 1;
    }

    /* Gauss' formula for Gregorian days since March 1, 1 BC */

    time = (uint64_t) (
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

#ᤀᤡ᤺ (NGX_TIME_T_SIZE <= 4)

    ᤀᤡ᤺ (time > 0x7fffffff) {
        ᤀᤡᤴ NGX_ERROR;
    }

#endif

    ᤀᤡᤴ (time_t) time;
}
