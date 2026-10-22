
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


अटल ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

समय_प्रकार
ngx_parse_http_time(u_char *मूल्य, माप_प्रकार len)
{
    u_char      *p, *end;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     समय;
    क्रमागत {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    end = मूल्य + len;

#अगर (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#पूर्ण_अगर

    क्रम (p = मूल्य; p < end; p++) {
        अगर (*p == ',') {
            अवरोध;
        }

        अगर (*p == ' ') {
            fmt = isoc;
            अवरोध;
        }
    }

    क्रम (p++; p < end; p++) {
        अगर (*p != ' ') {
            अवरोध;
        }
    }

    अगर (end - p < 18) {
        वापस NGX_ERROR;
    }

    अगर (fmt != isoc) {
        अगर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            वापस NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        अगर (*p == ' ') {
            अगर (end - p < 18) {
                वापस NGX_ERROR;
            }
            fmt = rfc822;

        } अन्यथा अगर (*p == '-') {
            fmt = rfc850;

        } अन्यथा {
            वापस NGX_ERROR;
        }

        p++;
    }

    चयन (*p) {

    हाल 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        अवरोध;

    हाल 'F':
        month = 1;
        अवरोध;

    हाल 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        अवरोध;

    हाल 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        अवरोध;

    हाल 'S':
        month = 8;
        अवरोध;

    हाल 'O':
        month = 9;
        अवरोध;

    हाल 'N':
        month = 10;
        अवरोध;

    हाल 'D':
        month = 11;
        अवरोध;

    शेष:
        वापस NGX_ERROR;
    }

    p += 3;

    अगर ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        वापस NGX_ERROR;
    }

    p++;

    अगर (fmt == rfc822) {
        अगर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            वापस NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } अन्यथा अगर (fmt == rfc850) {
        अगर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            वापस NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    अगर (fmt == isoc) {
        अगर (*p == ' ') {
            p++;
        }

        अगर (*p < '0' || *p > '9') {
            वापस NGX_ERROR;
        }

        day = *p++ - '0';

        अगर (*p != ' ') {
            अगर (*p < '0' || *p > '9') {
                वापस NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        अगर (end - p < 14) {
            वापस NGX_ERROR;
        }
    }

    अगर (*p++ != ' ') {
        वापस NGX_ERROR;
    }

    अगर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        वापस NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    अगर (*p++ != ':') {
        वापस NGX_ERROR;
    }

    अगर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        वापस NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    अगर (*p++ != ':') {
        वापस NGX_ERROR;
    }

    अगर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        वापस NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    अगर (fmt == isoc) {
        p += 2;

        अगर (*p++ != ' ') {
            वापस NGX_ERROR;
        }

        अगर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            वापस NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    अगर (hour > 23 || min > 59 || sec > 59) {
        वापस NGX_ERROR;
    }

    अगर (day == 29 && month == 1) {
        अगर ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            वापस NGX_ERROR;
        }

    } अन्यथा अगर (day > mday[month]) {
        वापस NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    अगर (--month <= 0) {
        month += 12;
        year -= 1;
    }

    /* Gauss' formula for Gregorian days since March 1, 1 BC */

    समय = (uint64_t) (
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

#अगर (NGX_TIME_T_आकार <= 4)

    अगर (समय > 0x7fffffff) {
        वापस NGX_ERROR;
    }

#पूर्ण_अगर

    वापस (समय_प्रकार) समय;
}
