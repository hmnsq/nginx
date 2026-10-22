
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
    enum {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    end = value + len;

#nếu (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#endif

    cho (p = value; p < end; p++) {
        nếu (*p == ',') {
            break;
        }

        nếu (*p == ' ') {
            fmt = isoc;
            break;
        }
    }

    cho (p++; p < end; p++) {
        nếu (*p != ' ') {
            break;
        }
    }

    nếu (end - p < 18) {
        trả_về NGX_ERROR;
    }

    nếu (fmt != isoc) {
        nếu (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            trả_về NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        nếu (*p == ' ') {
            nếu (end - p < 18) {
                trả_về NGX_ERROR;
            }
            fmt = rfc822;

        } không_thì nếu (*p == '-') {
            fmt = rfc850;

        } không_thì {
            trả_về NGX_ERROR;
        }

        p++;
    }

    switch (*p) {

    case 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        break;

    case 'F':
        month = 1;
        break;

    case 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        break;

    case 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        break;

    case 'S':
        month = 8;
        break;

    case 'O':
        month = 9;
        break;

    case 'N':
        month = 10;
        break;

    case 'D':
        month = 11;
        break;

    default:
        trả_về NGX_ERROR;
    }

    p += 3;

    nếu ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        trả_về NGX_ERROR;
    }

    p++;

    nếu (fmt == rfc822) {
        nếu (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            trả_về NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } không_thì nếu (fmt == rfc850) {
        nếu (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            trả_về NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    nếu (fmt == isoc) {
        nếu (*p == ' ') {
            p++;
        }

        nếu (*p < '0' || *p > '9') {
            trả_về NGX_ERROR;
        }

        day = *p++ - '0';

        nếu (*p != ' ') {
            nếu (*p < '0' || *p > '9') {
                trả_về NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        nếu (end - p < 14) {
            trả_về NGX_ERROR;
        }
    }

    nếu (*p++ != ' ') {
        trả_về NGX_ERROR;
    }

    nếu (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        trả_về NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    nếu (*p++ != ':') {
        trả_về NGX_ERROR;
    }

    nếu (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        trả_về NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    nếu (*p++ != ':') {
        trả_về NGX_ERROR;
    }

    nếu (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        trả_về NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    nếu (fmt == isoc) {
        p += 2;

        nếu (*p++ != ' ') {
            trả_về NGX_ERROR;
        }

        nếu (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            trả_về NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    nếu (hour > 23 || min > 59 || sec > 59) {
        trả_về NGX_ERROR;
    }

    nếu (day == 29 && month == 1) {
        nếu ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            trả_về NGX_ERROR;
        }

    } không_thì nếu (day > mday[month]) {
        trả_về NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    nếu (--month <= 0) {
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

#nếu (NGX_TIME_T_SIZE <= 4)

    nếu (time > 0x7fffffff) {
        trả_về NGX_ERROR;
    }

#endif

    trả_về (time_t) time;
}
