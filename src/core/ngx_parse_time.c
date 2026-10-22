
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ ngx_uint_t  mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

ಸಮಯ_t
ngx_parse_http_ಸಮಯ(u_char *ಮೌಲ್ಯ, ಗಾತ್ರ_t len)
{
    u_char      *p, *ಕೊನೆ;
    ngx_int_t    month;
    ngx_uint_t   day, year, hour, min, sec;
    uint64_t     ಸಮಯ;
    ಎಣಿಕೆ {
        no = 0,
        rfc822,   /* Tue, 10 Nov 2002 23:50:13   */
        rfc850,   /* Tuesday, 10-Dec-02 23:50:13 */
        isoc      /* Tue Dec 10 23:50:13 2002    */
    } fmt;

    fmt = 0;
    ಕೊನೆ = ಮೌಲ್ಯ + len;

#ಆದರೆ (NGX_SUPPRESS_WARN)
    day = 32;
    year = 2038;
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ (p = ಮೌಲ್ಯ; p < ಕೊನೆ; p++) {
        ಆದರೆ (*p == ',') {
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (*p == ' ') {
            fmt = isoc;
            ನಿಲ್ಲಿಸು;
        }
    }

    ಗಾಗಿ (p++; p < ಕೊನೆ; p++) {
        ಆದರೆ (*p != ' ') {
            ನಿಲ್ಲಿಸು;
        }
    }

    ಆದರೆ (ಕೊನೆ - p < 18) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (fmt != isoc) {
        ಆದರೆ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        day = (*p - '0') * 10 + (*(p + 1) - '0');
        p += 2;

        ಆದರೆ (*p == ' ') {
            ಆದರೆ (ಕೊನೆ - p < 18) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
            fmt = rfc822;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*p == '-') {
            fmt = rfc850;

        } ಇಲ್ಲದಿದ್ದರೆ {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        p++;
    }

    ಆಯ್ಕೆ (*p) {

    ಸ್ಥಿತಿ 'J':
        month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'F':
        month = 1;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'M':
        month = *(p + 2) == 'r' ? 2 : 4;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'A':
        month = *(p + 1) == 'p' ? 3 : 7;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'S':
        month = 8;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'O':
        month = 9;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'N':
        month = 10;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'D':
        month = 11;
        ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    p += 3;

    ಆದರೆ ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    p++;

    ಆದರೆ (fmt == rfc822) {
        ಆದರೆ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
        p += 4;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fmt == rfc850) {
        ಆದರೆ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        year = (*p - '0') * 10 + (*(p + 1) - '0');
        year += (year < 70) ? 2000 : 1900;
        p += 2;
    }

    ಆದರೆ (fmt == isoc) {
        ಆದರೆ (*p == ' ') {
            p++;
        }

        ಆದರೆ (*p < '0' || *p > '9') {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        day = *p++ - '0';

        ಆದರೆ (*p != ' ') {
            ಆದರೆ (*p < '0' || *p > '9') {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            day = day * 10 + (*p++ - '0');
        }

        ಆದರೆ (ಕೊನೆ - p < 14) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    ಆದರೆ (*p++ != ' ') {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    hour = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ಆದರೆ (*p++ != ':') {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    min = (*p - '0') * 10 + (*(p + 1) - '0');
    p += 2;

    ಆದರೆ (*p++ != ':') {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    sec = (*p - '0') * 10 + (*(p + 1) - '0');

    ಆದರೆ (fmt == isoc) {
        p += 2;

        ಆದರೆ (*p++ != ' ') {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
            || *(p + 2) < '0' || *(p + 2) > '9'
            || *(p + 3) < '0' || *(p + 3) > '9')
        {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
               + (*(p + 2) - '0') * 10 + (*(p + 3) - '0');
    }

    ಆದರೆ (hour > 23 || min > 59 || sec > 59) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (day == 29 && month == 1) {
        ಆದರೆ ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (day > mday[month]) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    /*
     * shift new year to March 1 and start months from 1 (not 0),
     * it is needed for Gauss' formula
     */

    ಆದರೆ (--month <= 0) {
        month += 12;
        year -= 1;
    }

    /* Gauss' formula for Gregorian days since March 1, 1 BC */

    ಸಮಯ = (uint64_t) (
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

#ಆದರೆ (NGX_ಸಮಯ_T_ಗಾತ್ರ <= 4)

    ಆದರೆ (ಸಮಯ > 0x7fffffff) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ (ಸಮಯ_t) ಸಮಯ;
}
