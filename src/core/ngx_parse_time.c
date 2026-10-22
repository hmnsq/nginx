
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

#ꦪꦼꦤ꧀ (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#endif

    ꦏꦁꦒꦺꦴ (p = value; p < end; p++) {
        ꦪꦼꦤ꧀ (*p == ',') {
            bha_nga;
        }

        ꦪꦼꦤ꧀ (*p == ' ') {
            fmt = isoc;
            bha_nga;
        }
    }

    ꦏꦁꦒꦺꦴ (p++; p < end; p++) {
        ꦪꦼꦤ꧀ (*p != ' ') {
            bha_nga;
        }
    }

    ꦪꦼꦤ꧀ (end - p < 18) {
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦪꦼꦤ꧀ (fmt != isoc) {
        ꦪꦼꦤ꧀ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            ꦮꦭꦶ NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        ꦪꦼꦤ꧀ (*p == ' ') {
            ꦪꦼꦤ꧀ (end - p < 18) {
                ꦮꦭꦶ NGX_ERROR;
            }
            fmt = rfc822;

        } ꦭꦶꦪ ꦪꦼꦤ꧀ (*p == '-') {
            fmt = rfc850;

        } ꦭꦶꦪ {
            ꦮꦭꦶ NGX_ERROR;
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
        ꦮꦭꦶ NGX_ERROR;
    }

    p += 3;

    ꦪꦼꦤ꧀ ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        ꦮꦭꦶ NGX_ERROR;
    }

    p++;

    ꦪꦼꦤ꧀ (fmt == rfc822) {
        ꦪꦼꦤ꧀ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            ꦮꦭꦶ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } ꦭꦶꦪ ꦪꦼꦤ꧀ (fmt == rfc850) {
        ꦪꦼꦤ꧀ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            ꦮꦭꦶ NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    ꦪꦼꦤ꧀ (fmt == isoc) {
        ꦪꦼꦤ꧀ (*p == ' ') {
            p++;
        }

        ꦪꦼꦤ꧀ (*p < '0' || *p > '9') {
            ꦮꦭꦶ NGX_ERROR;
        }

        day = *p++ - '0';

        ꦪꦼꦤ꧀ (*p != ' ') {
            ꦪꦼꦤ꧀ (*p < '0' || *p > '9') {
                ꦮꦭꦶ NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        ꦪꦼꦤ꧀ (end - p < 14) {
            ꦮꦭꦶ NGX_ERROR;
        }
    }

    ꦪꦼꦤ꧀ (*p++ != ' ') {
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦪꦼꦤ꧀ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ꦮꦭꦶ NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ꦪꦼꦤ꧀ (*p++ != ':') {
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦪꦼꦤ꧀ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ꦮꦭꦶ NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ꦪꦼꦤ꧀ (*p++ != ':') {
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦪꦼꦤ꧀ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ꦮꦭꦶ NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    ꦪꦼꦤ꧀ (fmt == isoc) {
        p += 2;

        ꦪꦼꦤ꧀ (*p++ != ' ') {
            ꦮꦭꦶ NGX_ERROR;
        }

        ꦪꦼꦤ꧀ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            ꦮꦭꦶ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    ꦪꦼꦤ꧀ (hour > 23 || min > 59 || sec > 59) {
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦪꦼꦤ꧀ (day == 29 && month == 1) {
        ꦪꦼꦤ꧀ ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            ꦮꦭꦶ NGX_ERROR;
        }

    } ꦭꦶꦪ ꦪꦼꦤ꧀ (day > mday[month]) {
        ꦮꦭꦶ NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    ꦪꦼꦤ꧀ (--month <= 0) {
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

#ꦪꦼꦤ꧀ (NGX_TIME_T_SIZE <= 4)

    ꦪꦼꦤ꧀ (time > 0x7fffffff) {
        ꦮꦭꦶ NGX_ERROR;
    }

#endif

    ꦮꦭꦶ (time_t) time;
}
