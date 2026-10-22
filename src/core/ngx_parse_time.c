
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ساکن ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

وقت_t
ngx_parse_http_وقت(u_char *قدر, سائز_t len)
{
    u_char      *p, *انتہا;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     وقت;
    شمار {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    انتہا = قدر + len;

#اگر (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#اگر_ختم

    برائے (p = قدر; p < انتہا; p++) {
        اگر (*p == ',') {
            توڑو;
        }

        اگر (*p == ' ') {
            fmt = isoc;
            توڑو;
        }
    }

    برائے (p++; p < انتہا; p++) {
        اگر (*p != ' ') {
            توڑو;
        }
    }

    اگر (انتہا - p < 18) {
        واپس NGX_ERROR;
    }

    اگر (fmt != isoc) {
        اگر (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            واپس NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        اگر (*p == ' ') {
            اگر (انتہا - p < 18) {
                واپس NGX_ERROR;
            }
            fmt = rfc822;

        } ورنہ اگر (*p == '-') {
            fmt = rfc850;

        } ورنہ {
            واپس NGX_ERROR;
        }

        p++;
    }

    انتخاب (*p) {

    حالت 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        توڑو;

    حالت 'F':
        month = 1;
        توڑو;

    حالت 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        توڑو;

    حالت 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        توڑو;

    حالت 'S':
        month = 8;
        توڑو;

    حالت 'O':
        month = 9;
        توڑو;

    حالت 'N':
        month = 10;
        توڑو;

    حالت 'D':
        month = 11;
        توڑو;

    طےشدہ:
        واپس NGX_ERROR;
    }

    p += 3;

    اگر ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        واپس NGX_ERROR;
    }

    p++;

    اگر (fmt == rfc822) {
        اگر (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            واپس NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } ورنہ اگر (fmt == rfc850) {
        اگر (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            واپس NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    اگر (fmt == isoc) {
        اگر (*p == ' ') {
            p++;
        }

        اگر (*p < '0' || *p > '9') {
            واپس NGX_ERROR;
        }

        day = *p++ - '0';

        اگر (*p != ' ') {
            اگر (*p < '0' || *p > '9') {
                واپس NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        اگر (انتہا - p < 14) {
            واپس NGX_ERROR;
        }
    }

    اگر (*p++ != ' ') {
        واپس NGX_ERROR;
    }

    اگر (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        واپس NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    اگر (*p++ != ':') {
        واپس NGX_ERROR;
    }

    اگر (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        واپس NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    اگر (*p++ != ':') {
        واپس NGX_ERROR;
    }

    اگر (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        واپس NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    اگر (fmt == isoc) {
        p += 2;

        اگر (*p++ != ' ') {
            واپس NGX_ERROR;
        }

        اگر (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            واپس NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    اگر (hour > 23 || min > 59 || sec > 59) {
        واپس NGX_ERROR;
    }

    اگر (day == 29 && month == 1) {
        اگر ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            واپس NGX_ERROR;
        }

    } ورنہ اگر (day > mday[month]) {
        واپس NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    اگر (--month <= 0) {
        month += 12;
        year -= 1;
    }

    /* Gauss' formula for Gregorian days since March 1, 1 BC */

    وقت = (uint64_t) (
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

#اگر (NGX_وقت_T_سائز <= 4)

    اگر (وقت > 0x7fffffff) {
        واپس NGX_ERROR;
    }

#اگر_ختم

    واپس (وقت_t) وقت;
}
