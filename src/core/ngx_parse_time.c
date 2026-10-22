
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

time_t
ngx_parse_http_time(u_char *मानम्, size_t len)
{
    u_char      *p, *end;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     time;
    गणना {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    end = मानम् + len;

#सचे (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#endif

    अत्थाय (p = मानम्; p < end; p++) {
        सचे (*p == ',') {
            भङ्ग;
        }

        सचे (*p == ' ') {
            fmt = isoc;
            भङ्ग;
        }
    }

    अत्थाय (p++; p < end; p++) {
        सचे (*p != ' ') {
            भङ्ग;
        }
    }

    सचे (end - p < 18) {
        पटिनिवत्त NGX_ERROR;
    }

    सचे (fmt != isoc) {
        सचे (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            पटिनिवत्त NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        सचे (*p == ' ') {
            सचे (end - p < 18) {
                पटिनिवत्त NGX_ERROR;
            }
            fmt = rfc822;

        } अञ्ञथा सचे (*p == '-') {
            fmt = rfc850;

        } अञ्ञथा {
            पटिनिवत्त NGX_ERROR;
        }

        p++;
    }

    विकल्प (*p) {

    पक्ष 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        भङ्ग;

    पक्ष 'F':
        month = 1;
        भङ्ग;

    पक्ष 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        भङ्ग;

    पक्ष 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        भङ्ग;

    पक्ष 'S':
        month = 8;
        भङ्ग;

    पक्ष 'O':
        month = 9;
        भङ्ग;

    पक्ष 'N':
        month = 10;
        भङ्ग;

    पक्ष 'D':
        month = 11;
        भङ्ग;

    स्वभाव:
        पटिनिवत्त NGX_ERROR;
    }

    p += 3;

    सचे ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        पटिनिवत्त NGX_ERROR;
    }

    p++;

    सचे (fmt == rfc822) {
        सचे (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            पटिनिवत्त NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } अञ्ञथा सचे (fmt == rfc850) {
        सचे (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            पटिनिवत्त NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    सचे (fmt == isoc) {
        सचे (*p == ' ') {
            p++;
        }

        सचे (*p < '0' || *p > '9') {
            पटिनिवत्त NGX_ERROR;
        }

        day = *p++ - '0';

        सचे (*p != ' ') {
            सचे (*p < '0' || *p > '9') {
                पटिनिवत्त NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        सचे (end - p < 14) {
            पटिनिवत्त NGX_ERROR;
        }
    }

    सचे (*p++ != ' ') {
        पटिनिवत्त NGX_ERROR;
    }

    सचे (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        पटिनिवत्त NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    सचे (*p++ != ':') {
        पटिनिवत्त NGX_ERROR;
    }

    सचे (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        पटिनिवत्त NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    सचे (*p++ != ':') {
        पटिनिवत्त NGX_ERROR;
    }

    सचे (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        पटिनिवत्त NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    सचे (fmt == isoc) {
        p += 2;

        सचे (*p++ != ' ') {
            पटिनिवत्त NGX_ERROR;
        }

        सचे (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            पटिनिवत्त NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    सचे (hour > 23 || min > 59 || sec > 59) {
        पटिनिवत्त NGX_ERROR;
    }

    सचे (day == 29 && month == 1) {
        सचे ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            पटिनिवत्त NGX_ERROR;
        }

    } अञ्ञथा सचे (day > mday[month]) {
        पटिनिवत्त NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    सचे (--month <= 0) {
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

#सचे (NGX_TIME_T_SIZE <= 4)

    सचे (time > 0x7fffffff) {
        पटिनिवत्त NGX_ERROR;
    }

#endif

    पटिनिवत्त (time_t) time;
}
