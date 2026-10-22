
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

#もし (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#endif

    ために (p = value; p < end; p++) {
        もし (*p == ',') {
            break;
        }

        もし (*p == ' ') {
            fmt = isoc;
            break;
        }
    }

    ために (p++; p < end; p++) {
        もし (*p != ' ') {
            break;
        }
    }

    もし (end - p < 18) {
        戻る NGX_ERROR;
    }

    もし (fmt != isoc) {
        もし (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            戻る NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        もし (*p == ' ') {
            もし (end - p < 18) {
                戻る NGX_ERROR;
            }
            fmt = rfc822;

        } 否则 もし (*p == '-') {
            fmt = rfc850;

        } 否则 {
            戻る NGX_ERROR;
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
        戻る NGX_ERROR;
    }

    p += 3;

    もし ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        戻る NGX_ERROR;
    }

    p++;

    もし (fmt == rfc822) {
        もし (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            戻る NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } 否则 もし (fmt == rfc850) {
        もし (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            戻る NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    もし (fmt == isoc) {
        もし (*p == ' ') {
            p++;
        }

        もし (*p < '0' || *p > '9') {
            戻る NGX_ERROR;
        }

        day = *p++ - '0';

        もし (*p != ' ') {
            もし (*p < '0' || *p > '9') {
                戻る NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        もし (end - p < 14) {
            戻る NGX_ERROR;
        }
    }

    もし (*p++ != ' ') {
        戻る NGX_ERROR;
    }

    もし (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        戻る NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    もし (*p++ != ':') {
        戻る NGX_ERROR;
    }

    もし (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        戻る NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    もし (*p++ != ':') {
        戻る NGX_ERROR;
    }

    もし (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        戻る NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    もし (fmt == isoc) {
        p += 2;

        もし (*p++ != ' ') {
            戻る NGX_ERROR;
        }

        もし (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            戻る NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    もし (hour > 23 || min > 59 || sec > 59) {
        戻る NGX_ERROR;
    }

    もし (day == 29 && month == 1) {
        もし ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            戻る NGX_ERROR;
        }

    } 否则 もし (day > mday[month]) {
        戻る NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    もし (--month <= 0) {
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

#もし (NGX_TIME_T_SIZE <= 4)

    もし (time > 0x7fffffff) {
        戻る NGX_ERROR;
    }

#endif

    戻る (time_t) time;
}
