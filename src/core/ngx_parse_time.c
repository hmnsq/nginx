
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

time_t
ngx_parse_http_time(u_char *ಮೌಲ್ಯ, size_t len)
{
    u_char      *p, *end;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     time;
    ಗಣನಾ {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    end = ಮೌಲ್ಯ + len;

#ಯದಿ (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#endif

    ಪ್ರತಿ (p = ಮೌಲ್ಯ; p < end; p++) {
        ಯದಿ (*p == ',') {
            ಭಙ್ಗ;
        }

        ಯದಿ (*p == ' ') {
            fmt = isoc;
            ಭಙ್ಗ;
        }
    }

    ಪ್ರತಿ (p++; p < end; p++) {
        ಯದಿ (*p != ' ') {
            ಭಙ್ಗ;
        }
    }

    ಯದಿ (end - p < 18) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (fmt != isoc) {
        ಯದಿ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        ಯದಿ (*p == ' ') {
            ಯದಿ (end - p < 18) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
            fmt = rfc822;

        } ಅನ್ಯಥಾ ಯದಿ (*p == '-') {
            fmt = rfc850;

        } ಅನ್ಯಥಾ {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        p++;
    }

    ವಿಕಲ್ಪ (*p) {

    ಪಕ್ಷ 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        ಭಙ್ಗ;

    ಪಕ್ಷ 'F':
        month = 1;
        ಭಙ್ಗ;

    ಪಕ್ಷ 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        ಭಙ್ಗ;

    ಪಕ್ಷ 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        ಭಙ್ಗ;

    ಪಕ್ಷ 'S':
        month = 8;
        ಭಙ್ಗ;

    ಪಕ್ಷ 'O':
        month = 9;
        ಭಙ್ಗ;

    ಪಕ್ಷ 'N':
        month = 10;
        ಭಙ್ಗ;

    ಪಕ್ಷ 'D':
        month = 11;
        ಭಙ್ಗ;

    ಸ್ವಭಾವ:
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    p += 3;

    ಯದಿ ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    p++;

    ಯದಿ (fmt == rfc822) {
        ಯದಿ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } ಅನ್ಯಥಾ ಯದಿ (fmt == rfc850) {
        ಯದಿ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    ಯದಿ (fmt == isoc) {
        ಯದಿ (*p == ' ') {
            p++;
        }

        ಯದಿ (*p < '0' || *p > '9') {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        day = *p++ - '0';

        ಯದಿ (*p != ' ') {
            ಯದಿ (*p < '0' || *p > '9') {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        ಯದಿ (end - p < 14) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    ಯದಿ (*p++ != ' ') {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ಯದಿ (*p++ != ':') {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ಯದಿ (*p++ != ':') {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    ಯದಿ (fmt == isoc) {
        p += 2;

        ಯದಿ (*p++ != ' ') {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    ಯದಿ (hour > 23 || min > 59 || sec > 59) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (day == 29 && month == 1) {
        ಯದಿ ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

    } ಅನ್ಯಥಾ ಯದಿ (day > mday[month]) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    ಯದಿ (--month <= 0) {
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

#ಯದಿ (NGX_TIME_T_SIZE <= 4)

    ಯದಿ (time > 0x7fffffff) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

#endif

    ಪ್ರತ್ಯಾಗಮ (time_t) time;
}
