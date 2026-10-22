
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


స్థిర ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

సమయం_t
ngx_parse_http_సమయం(u_char *విలువ, కొలత_t len)
{
    u_char      *p, *చివర;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     సమయం;
    గణన {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    చివర = విలువ + len;

#ఒకవేళ (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#అయితే_ముగింపు

    కొరకు (p = విలువ; p < చివర; p++) {
        ఒకవేళ (*p == ',') {
            ఆపు;
        }

        ఒకవేళ (*p == ' ') {
            fmt = isoc;
            ఆపు;
        }
    }

    కొరకు (p++; p < చివర; p++) {
        ఒకవేళ (*p != ' ') {
            ఆపు;
        }
    }

    ఒకవేళ (చివర - p < 18) {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (fmt != isoc) {
        ఒకవేళ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            తిరిగి NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        ఒకవేళ (*p == ' ') {
            ఒకవేళ (చివర - p < 18) {
                తిరిగి NGX_ERROR;
            }
            fmt = rfc822;

        } లేకపోతే ఒకవేళ (*p == '-') {
            fmt = rfc850;

        } లేకపోతే {
            తిరిగి NGX_ERROR;
        }

        p++;
    }

    ఎంపిక (*p) {

    స్థితి 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        ఆపు;

    స్థితి 'F':
        month = 1;
        ఆపు;

    స్థితి 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        ఆపు;

    స్థితి 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        ఆపు;

    స్థితి 'S':
        month = 8;
        ఆపు;

    స్థితి 'O':
        month = 9;
        ఆపు;

    స్థితి 'N':
        month = 10;
        ఆపు;

    స్థితి 'D':
        month = 11;
        ఆపు;

    అప్రమేయ:
        తిరిగి NGX_ERROR;
    }

    p += 3;

    ఒకవేళ ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        తిరిగి NGX_ERROR;
    }

    p++;

    ఒకవేళ (fmt == rfc822) {
        ఒకవేళ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            తిరిగి NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } లేకపోతే ఒకవేళ (fmt == rfc850) {
        ఒకవేళ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            తిరిగి NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    ఒకవేళ (fmt == isoc) {
        ఒకవేళ (*p == ' ') {
            p++;
        }

        ఒకవేళ (*p < '0' || *p > '9') {
            తిరిగి NGX_ERROR;
        }

        day = *p++ - '0';

        ఒకవేళ (*p != ' ') {
            ఒకవేళ (*p < '0' || *p > '9') {
                తిరిగి NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        ఒకవేళ (చివర - p < 14) {
            తిరిగి NGX_ERROR;
        }
    }

    ఒకవేళ (*p++ != ' ') {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        తిరిగి NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ఒకవేళ (*p++ != ':') {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        తిరిగి NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ఒకవేళ (*p++ != ':') {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        తిరిగి NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    ఒకవేళ (fmt == isoc) {
        p += 2;

        ఒకవేళ (*p++ != ' ') {
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            తిరిగి NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    ఒకవేళ (hour > 23 || min > 59 || sec > 59) {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (day == 29 && month == 1) {
        ఒకవేళ ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            తిరిగి NGX_ERROR;
        }

    } లేకపోతే ఒకవేళ (day > mday[month]) {
        తిరిగి NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    ఒకవేళ (--month <= 0) {
        month += 12;
        year -= 1;
    }

    /* Gauss' formula for Gregorian days since March 1, 1 BC */

    సమయం = (uint64_t) (
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

#ఒకవేళ (NGX_సమయం_T_కొలత <= 4)

    ఒకవేళ (సమయం > 0x7fffffff) {
        తిరిగి NGX_ERROR;
    }

#అయితే_ముగింపు

    తిరిగి (సమయం_t) సమయం;
}
