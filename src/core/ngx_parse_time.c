
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


நிலையான ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

time_t
ngx_parse_http_time(u_char *மதிப்பு, size_t len)
{
    u_char      *p, *end;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     time;
    எண்ணிக்கை {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    end = மதிப்பு + len;

#எனில் (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#endif

    க்கு (p = மதிப்பு; p < end; p++) {
        எனில் (*p == ',') {
            நிறுத்து;
        }

        எனில் (*p == ' ') {
            fmt = isoc;
            நிறுத்து;
        }
    }

    க்கு (p++; p < end; p++) {
        எனில் (*p != ' ') {
            நிறுத்து;
        }
    }

    எனில் (end - p < 18) {
        திரும்பு NGX_ERROR;
    }

    எனில் (fmt != isoc) {
        எனில் (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            திரும்பு NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        எனில் (*p == ' ') {
            எனில் (end - p < 18) {
                திரும்பு NGX_ERROR;
            }
            fmt = rfc822;

        } இல்லையெனில் எனில் (*p == '-') {
            fmt = rfc850;

        } இல்லையெனில் {
            திரும்பு NGX_ERROR;
        }

        p++;
    }

    தேர்வு (*p) {

    நிலை 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        நிறுத்து;

    நிலை 'F':
        month = 1;
        நிறுத்து;

    நிலை 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        நிறுத்து;

    நிலை 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        நிறுத்து;

    நிலை 'S':
        month = 8;
        நிறுத்து;

    நிலை 'O':
        month = 9;
        நிறுத்து;

    நிலை 'N':
        month = 10;
        நிறுத்து;

    நிலை 'D':
        month = 11;
        நிறுத்து;

    இயல்பு:
        திரும்பு NGX_ERROR;
    }

    p += 3;

    எனில் ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        திரும்பு NGX_ERROR;
    }

    p++;

    எனில் (fmt == rfc822) {
        எனில் (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            திரும்பு NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } இல்லையெனில் எனில் (fmt == rfc850) {
        எனில் (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            திரும்பு NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    எனில் (fmt == isoc) {
        எனில் (*p == ' ') {
            p++;
        }

        எனில் (*p < '0' || *p > '9') {
            திரும்பு NGX_ERROR;
        }

        day = *p++ - '0';

        எனில் (*p != ' ') {
            எனில் (*p < '0' || *p > '9') {
                திரும்பு NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        எனில் (end - p < 14) {
            திரும்பு NGX_ERROR;
        }
    }

    எனில் (*p++ != ' ') {
        திரும்பு NGX_ERROR;
    }

    எனில் (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        திரும்பு NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    எனில் (*p++ != ':') {
        திரும்பு NGX_ERROR;
    }

    எனில் (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        திரும்பு NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    எனில் (*p++ != ':') {
        திரும்பு NGX_ERROR;
    }

    எனில் (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        திரும்பு NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    எனில் (fmt == isoc) {
        p += 2;

        எனில் (*p++ != ' ') {
            திரும்பு NGX_ERROR;
        }

        எனில் (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            திரும்பு NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    எனில் (hour > 23 || min > 59 || sec > 59) {
        திரும்பு NGX_ERROR;
    }

    எனில் (day == 29 && month == 1) {
        எனில் ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            திரும்பு NGX_ERROR;
        }

    } இல்லையெனில் எனில் (day > mday[month]) {
        திரும்பு NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    எனில் (--month <= 0) {
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

#எனில் (NGX_TIME_T_SIZE <= 4)

    எனில் (time > 0x7fffffff) {
        திரும்பு NGX_ERROR;
    }

#endif

    திரும்பு (time_t) time;
}
