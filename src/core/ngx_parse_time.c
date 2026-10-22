
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

#если (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#endif

    для (p = value; p < end; p++) {
        если (*p == ',') {
            break;
        }

        если (*p == ' ') {
            fmt = isoc;
            break;
        }
    }

    для (p++; p < end; p++) {
        если (*p != ' ') {
            break;
        }
    }

    если (end - p < 18) {
        вернуть NGX_ERROR;
    }

    если (fmt != isoc) {
        если (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            вернуть NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        если (*p == ' ') {
            если (end - p < 18) {
                вернуть NGX_ERROR;
            }
            fmt = rfc822;

        } иначе если (*p == '-') {
            fmt = rfc850;

        } иначе {
            вернуть NGX_ERROR;
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
        вернуть NGX_ERROR;
    }

    p += 3;

    если ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        вернуть NGX_ERROR;
    }

    p++;

    если (fmt == rfc822) {
        если (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            вернуть NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } иначе если (fmt == rfc850) {
        если (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            вернуть NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    если (fmt == isoc) {
        если (*p == ' ') {
            p++;
        }

        если (*p < '0' || *p > '9') {
            вернуть NGX_ERROR;
        }

        day = *p++ - '0';

        если (*p != ' ') {
            если (*p < '0' || *p > '9') {
                вернуть NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        если (end - p < 14) {
            вернуть NGX_ERROR;
        }
    }

    если (*p++ != ' ') {
        вернуть NGX_ERROR;
    }

    если (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        вернуть NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    если (*p++ != ':') {
        вернуть NGX_ERROR;
    }

    если (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        вернуть NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    если (*p++ != ':') {
        вернуть NGX_ERROR;
    }

    если (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        вернуть NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    если (fmt == isoc) {
        p += 2;

        если (*p++ != ' ') {
            вернуть NGX_ERROR;
        }

        если (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            вернуть NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    если (hour > 23 || min > 59 || sec > 59) {
        вернуть NGX_ERROR;
    }

    если (day == 29 && month == 1) {
        если ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            вернуть NGX_ERROR;
        }

    } иначе если (day > mday[month]) {
        вернуть NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    если (--month <= 0) {
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

#если (NGX_TIME_T_SIZE <= 4)

    если (time > 0x7fffffff) {
        вернуть NGX_ERROR;
    }

#endif

    вернуть (time_t) time;
}
