
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ସ୍ଥିର ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

time_t
ngx_parse_http_time(u_char *ମୂଲ୍ୟ, size_t len)
{
    u_char      *p, *end;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     time;
    ଗଣନା {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    end = ମୂଲ୍ୟ + len;

#ଯଦି (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#endif

    ପାଇଁ (p = ମୂଲ୍ୟ; p < end; p++) {
        ଯଦି (*p == ',') {
            ଭାଙ୍ଗ;
        }

        ଯଦି (*p == ' ') {
            fmt = isoc;
            ଭାଙ୍ଗ;
        }
    }

    ପାଇଁ (p++; p < end; p++) {
        ଯଦି (*p != ' ') {
            ଭାଙ୍ଗ;
        }
    }

    ଯଦି (end - p < 18) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (fmt != isoc) {
        ଯଦି (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        ଯଦି (*p == ' ') {
            ଯଦି (end - p < 18) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }
            fmt = rfc822;

        } ଅନ୍ୟଥା ଯଦି (*p == '-') {
            fmt = rfc850;

        } ଅନ୍ୟଥା {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        p++;
    }

    ବାଛ (*p) {

    ସ୍ଥିତି 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି 'F':
        month = 1;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି 'S':
        month = 8;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି 'O':
        month = 9;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି 'N':
        month = 10;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି 'D':
        month = 11;
        ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    p += 3;

    ଯଦି ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    p++;

    ଯଦି (fmt == rfc822) {
        ଯଦି (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } ଅନ୍ୟଥା ଯଦି (fmt == rfc850) {
        ଯଦି (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    ଯଦି (fmt == isoc) {
        ଯଦି (*p == ' ') {
            p++;
        }

        ଯଦି (*p < '0' || *p > '9') {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        day = *p++ - '0';

        ଯଦି (*p != ' ') {
            ଯଦି (*p < '0' || *p > '9') {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        ଯଦି (end - p < 14) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    ଯଦି (*p++ != ' ') {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ଯଦି (*p++ != ':') {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ଯଦି (*p++ != ':') {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    ଯଦି (fmt == isoc) {
        p += 2;

        ଯଦି (*p++ != ' ') {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    ଯଦି (hour > 23 || min > 59 || sec > 59) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (day == 29 && month == 1) {
        ଯଦି ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

    } ଅନ୍ୟଥା ଯଦି (day > mday[month]) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    ଯଦି (--month <= 0) {
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

#ଯଦି (NGX_TIME_T_SIZE <= 4)

    ଯଦି (time > 0x7fffffff) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

#endif

    ଫେରନ୍ତୁ (time_t) time;
}
