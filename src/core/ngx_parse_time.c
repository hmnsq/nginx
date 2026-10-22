
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


સ્થિર ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

time_t
ngx_parse_http_time(u_char *કિંમત, size_t len)
{
    u_char      *p, *end;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     time;
    ગણના {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    end = કિંમત + len;

#જો (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#endif

    માટે (p = કિંમત; p < end; p++) {
        જો (*p == ',') {
            તોડો;
        }

        જો (*p == ' ') {
            fmt = isoc;
            તોડો;
        }
    }

    માટે (p++; p < end; p++) {
        જો (*p != ' ') {
            તોડો;
        }
    }

    જો (end - p < 18) {
        પાછા NGX_ERROR;
    }

    જો (fmt != isoc) {
        જો (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            પાછા NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        જો (*p == ' ') {
            જો (end - p < 18) {
                પાછા NGX_ERROR;
            }
            fmt = rfc822;

        } નહીંતર જો (*p == '-') {
            fmt = rfc850;

        } નહીંતર {
            પાછા NGX_ERROR;
        }

        p++;
    }

    પસંદગી (*p) {

    સ્થિતિ 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        તોડો;

    સ્થિતિ 'F':
        month = 1;
        તોડો;

    સ્થિતિ 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        તોડો;

    સ્થિતિ 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        તોડો;

    સ્થિતિ 'S':
        month = 8;
        તોડો;

    સ્થિતિ 'O':
        month = 9;
        તોડો;

    સ્થિતિ 'N':
        month = 10;
        તોડો;

    સ્થિતિ 'D':
        month = 11;
        તોડો;

    મૂળભૂત:
        પાછા NGX_ERROR;
    }

    p += 3;

    જો ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        પાછા NGX_ERROR;
    }

    p++;

    જો (fmt == rfc822) {
        જો (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            પાછા NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } નહીંતર જો (fmt == rfc850) {
        જો (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            પાછા NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    જો (fmt == isoc) {
        જો (*p == ' ') {
            p++;
        }

        જો (*p < '0' || *p > '9') {
            પાછા NGX_ERROR;
        }

        day = *p++ - '0';

        જો (*p != ' ') {
            જો (*p < '0' || *p > '9') {
                પાછા NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        જો (end - p < 14) {
            પાછા NGX_ERROR;
        }
    }

    જો (*p++ != ' ') {
        પાછા NGX_ERROR;
    }

    જો (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        પાછા NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    જો (*p++ != ':') {
        પાછા NGX_ERROR;
    }

    જો (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        પાછા NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    જો (*p++ != ':') {
        પાછા NGX_ERROR;
    }

    જો (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        પાછા NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    જો (fmt == isoc) {
        p += 2;

        જો (*p++ != ' ') {
            પાછા NGX_ERROR;
        }

        જો (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            પાછા NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    જો (hour > 23 || min > 59 || sec > 59) {
        પાછા NGX_ERROR;
    }

    જો (day == 29 && month == 1) {
        જો ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            પાછા NGX_ERROR;
        }

    } નહીંતર જો (day > mday[month]) {
        પાછા NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    જો (--month <= 0) {
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

#જો (NGX_TIME_T_SIZE <= 4)

    જો (time > 0x7fffffff) {
        પાછા NGX_ERROR;
    }

#endif

    પાછા (time_t) time;
}
