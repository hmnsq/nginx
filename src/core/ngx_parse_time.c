
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


স্থির ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

time_t
ngx_parse_http_time(u_char *মান, size_t len)
{
    u_char      *p, *end;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     time;
    গণনা {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    end = মান + len;

#যদি (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#endif

    প্রতি (p = মান; p < end; p++) {
        যদি (*p == ',') {
            ভঙ্গ;
        }

        যদি (*p == ' ') {
            fmt = isoc;
            ভঙ্গ;
        }
    }

    প্রতি (p++; p < end; p++) {
        যদি (*p != ' ') {
            ভঙ্গ;
        }
    }

    যদি (end - p < 18) {
        প্রত্যাগম NGX_ERROR;
    }

    যদি (fmt != isoc) {
        যদি (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            প্রত্যাগম NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        যদি (*p == ' ') {
            যদি (end - p < 18) {
                প্রত্যাগম NGX_ERROR;
            }
            fmt = rfc822;

        } অন্যথা যদি (*p == '-') {
            fmt = rfc850;

        } অন্যথা {
            প্রত্যাগম NGX_ERROR;
        }

        p++;
    }

    বিকল্প (*p) {

    পক্ষ 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        ভঙ্গ;

    পক্ষ 'F':
        month = 1;
        ভঙ্গ;

    পক্ষ 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        ভঙ্গ;

    পক্ষ 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        ভঙ্গ;

    পক্ষ 'S':
        month = 8;
        ভঙ্গ;

    পক্ষ 'O':
        month = 9;
        ভঙ্গ;

    পক্ষ 'N':
        month = 10;
        ভঙ্গ;

    পক্ষ 'D':
        month = 11;
        ভঙ্গ;

    স্বভাব:
        প্রত্যাগম NGX_ERROR;
    }

    p += 3;

    যদি ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        প্রত্যাগম NGX_ERROR;
    }

    p++;

    যদি (fmt == rfc822) {
        যদি (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            প্রত্যাগম NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } অন্যথা যদি (fmt == rfc850) {
        যদি (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            প্রত্যাগম NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    যদি (fmt == isoc) {
        যদি (*p == ' ') {
            p++;
        }

        যদি (*p < '0' || *p > '9') {
            প্রত্যাগম NGX_ERROR;
        }

        day = *p++ - '0';

        যদি (*p != ' ') {
            যদি (*p < '0' || *p > '9') {
                প্রত্যাগম NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        যদি (end - p < 14) {
            প্রত্যাগম NGX_ERROR;
        }
    }

    যদি (*p++ != ' ') {
        প্রত্যাগম NGX_ERROR;
    }

    যদি (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        প্রত্যাগম NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    যদি (*p++ != ':') {
        প্রত্যাগম NGX_ERROR;
    }

    যদি (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        প্রত্যাগম NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    যদি (*p++ != ':') {
        প্রত্যাগম NGX_ERROR;
    }

    যদি (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        প্রত্যাগম NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    যদি (fmt == isoc) {
        p += 2;

        যদি (*p++ != ' ') {
            প্রত্যাগম NGX_ERROR;
        }

        যদি (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            প্রত্যাগম NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    যদি (hour > 23 || min > 59 || sec > 59) {
        প্রত্যাগম NGX_ERROR;
    }

    যদি (day == 29 && month == 1) {
        যদি ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            প্রত্যাগম NGX_ERROR;
        }

    } অন্যথা যদি (day > mday[month]) {
        প্রত্যাগম NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    যদি (--month <= 0) {
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

#যদি (NGX_TIME_T_SIZE <= 4)

    যদি (time > 0x7fffffff) {
        প্রত্যাগম NGX_ERROR;
    }

#endif

    প্রত্যাগম (time_t) time;
}
