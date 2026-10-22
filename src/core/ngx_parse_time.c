
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

समय_प्रकार
ngx_parse_http_time(u_char *मूल्य, माप_प्रकार len)
{
    u_char      *p, *end;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     समय;
    गणना {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    end = मूल्य + len;

#जर (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#endif

    साठी (p = मूल्य; p < end; p++) {
        जर (*p == ',') {
            थांबा;
        }

        जर (*p == ' ') {
            fmt = isoc;
            थांबा;
        }
    }

    साठी (p++; p < end; p++) {
        जर (*p != ' ') {
            थांबा;
        }
    }

    जर (end - p < 18) {
        परत NGX_ERROR;
    }

    जर (fmt != isoc) {
        जर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            परत NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        जर (*p == ' ') {
            जर (end - p < 18) {
                परत NGX_ERROR;
            }
            fmt = rfc822;

        } अन्यथा जर (*p == '-') {
            fmt = rfc850;

        } अन्यथा {
            परत NGX_ERROR;
        }

        p++;
    }

    निवड (*p) {

    स्थिती 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        थांबा;

    स्थिती 'F':
        month = 1;
        थांबा;

    स्थिती 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        थांबा;

    स्थिती 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        थांबा;

    स्थिती 'S':
        month = 8;
        थांबा;

    स्थिती 'O':
        month = 9;
        थांबा;

    स्थिती 'N':
        month = 10;
        थांबा;

    स्थिती 'D':
        month = 11;
        थांबा;

    पूर्वनिर्धारित:
        परत NGX_ERROR;
    }

    p += 3;

    जर ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        परत NGX_ERROR;
    }

    p++;

    जर (fmt == rfc822) {
        जर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            परत NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } अन्यथा जर (fmt == rfc850) {
        जर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            परत NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    जर (fmt == isoc) {
        जर (*p == ' ') {
            p++;
        }

        जर (*p < '0' || *p > '9') {
            परत NGX_ERROR;
        }

        day = *p++ - '0';

        जर (*p != ' ') {
            जर (*p < '0' || *p > '9') {
                परत NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        जर (end - p < 14) {
            परत NGX_ERROR;
        }
    }

    जर (*p++ != ' ') {
        परत NGX_ERROR;
    }

    जर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        परत NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    जर (*p++ != ':') {
        परत NGX_ERROR;
    }

    जर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        परत NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    जर (*p++ != ':') {
        परत NGX_ERROR;
    }

    जर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        परत NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    जर (fmt == isoc) {
        p += 2;

        जर (*p++ != ' ') {
            परत NGX_ERROR;
        }

        जर (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            परत NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    जर (hour > 23 || min > 59 || sec > 59) {
        परत NGX_ERROR;
    }

    जर (day == 29 && month == 1) {
        जर ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            परत NGX_ERROR;
        }

    } अन्यथा जर (day > mday[month]) {
        परत NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    जर (--month <= 0) {
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

#जर (NGX_TIME_T_SIZE <= 4)

    जर (समय > 0x7fffffff) {
        परत NGX_ERROR;
    }

#endif

    परत (समय_प्रकार) समय;
}
