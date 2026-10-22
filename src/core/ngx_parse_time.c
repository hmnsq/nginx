
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


സ്ഥിതം ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

time_t
ngx_parse_http_time(u_char *മൂല്യം, size_t len)
{
    u_char      *p, *end;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     time;
    എണ്ണൽ {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    end = മൂല്യം + len;

#എങ്കിൽ (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#endif

    വേണ്ടി (p = മൂല്യം; p < end; p++) {
        എങ്കിൽ (*p == ',') {
            നിർത്തുക;
        }

        എങ്കിൽ (*p == ' ') {
            fmt = isoc;
            നിർത്തുക;
        }
    }

    വേണ്ടി (p++; p < end; p++) {
        എങ്കിൽ (*p != ' ') {
            നിർത്തുക;
        }
    }

    എങ്കിൽ (end - p < 18) {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (fmt != isoc) {
        എങ്കിൽ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            തിരികെ NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        എങ്കിൽ (*p == ' ') {
            എങ്കിൽ (end - p < 18) {
                തിരികെ NGX_ERROR;
            }
            fmt = rfc822;

        } അല്ലെങ്കിൽ എങ്കിൽ (*p == '-') {
            fmt = rfc850;

        } അല്ലെങ്കിൽ {
            തിരികെ NGX_ERROR;
        }

        p++;
    }

    തിരഞ്ഞെടുപ്പ് (*p) {

    അവസ്ഥ 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        നിർത്തുക;

    അവസ്ഥ 'F':
        month = 1;
        നിർത്തുക;

    അവസ്ഥ 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        നിർത്തുക;

    അവസ്ഥ 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        നിർത്തുക;

    അവസ്ഥ 'S':
        month = 8;
        നിർത്തുക;

    അവസ്ഥ 'O':
        month = 9;
        നിർത്തുക;

    അവസ്ഥ 'N':
        month = 10;
        നിർത്തുക;

    അവസ്ഥ 'D':
        month = 11;
        നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
        തിരികെ NGX_ERROR;
    }

    p += 3;

    എങ്കിൽ ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        തിരികെ NGX_ERROR;
    }

    p++;

    എങ്കിൽ (fmt == rfc822) {
        എങ്കിൽ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            തിരികെ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } അല്ലെങ്കിൽ എങ്കിൽ (fmt == rfc850) {
        എങ്കിൽ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            തിരികെ NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    എങ്കിൽ (fmt == isoc) {
        എങ്കിൽ (*p == ' ') {
            p++;
        }

        എങ്കിൽ (*p < '0' || *p > '9') {
            തിരികെ NGX_ERROR;
        }

        day = *p++ - '0';

        എങ്കിൽ (*p != ' ') {
            എങ്കിൽ (*p < '0' || *p > '9') {
                തിരികെ NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        എങ്കിൽ (end - p < 14) {
            തിരികെ NGX_ERROR;
        }
    }

    എങ്കിൽ (*p++ != ' ') {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        തിരികെ NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    എങ്കിൽ (*p++ != ':') {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        തിരികെ NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    എങ്കിൽ (*p++ != ':') {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        തിരികെ NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    എങ്കിൽ (fmt == isoc) {
        p += 2;

        എങ്കിൽ (*p++ != ' ') {
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            തിരികെ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    എങ്കിൽ (hour > 23 || min > 59 || sec > 59) {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (day == 29 && month == 1) {
        എങ്കിൽ ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            തിരികെ NGX_ERROR;
        }

    } അല്ലെങ്കിൽ എങ്കിൽ (day > mday[month]) {
        തിരികെ NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    എങ്കിൽ (--month <= 0) {
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

#എങ്കിൽ (NGX_TIME_T_SIZE <= 4)

    എങ്കിൽ (time > 0x7fffffff) {
        തിരികെ NGX_ERROR;
    }

#endif

    തിരികെ (time_t) time;
}
