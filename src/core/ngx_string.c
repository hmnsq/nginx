
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


స్థిర u_char *ngx_sprintf_అంకె(u_char *buf, u_char *చివరి, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t వెడల్పు);
స్థిర u_char *ngx_sprintf_str(u_char *buf, u_char *చివరి, u_char *src,
    కొలత_t len, ngx_uint_t hexadecimal);
స్థిర శూన్య ngx_encode_base64_అంతర్గత(ngx_str_t *dst, ngx_str_t *src,
    స్థిరరాశి u_char *basis, ngx_uint_t padding);
స్థిర ngx_int_t ngx_decode_base64_అంతర్గత(ngx_str_t *dst, ngx_str_t *src,
    స్థిరరాశి u_char *basis);


శూన్య
ngx_strlow(u_char *dst, u_char *src, కొలత_t n)
{
    అయితే (n) {
        *dst = ngx_tolower(*src);
        dst++;
        src++;
        n--;
    }
}


కొలత_t
ngx_strnlen(u_char *p, కొలత_t n)
{
    కొలత_t  i;

    కొరకు (i = 0; i < n; i++) {

        ఒకవేళ (p[i] == '\0') {
            తిరిగి i;
        }
    }

    తిరిగి n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, కొలత_t n)
{
    ఒకవేళ (n == 0) {
        తిరిగి dst;
    }

    అయితే (--n) {
        *dst = *src;

        ఒకవేళ (*dst == '\0') {
            తిరిగి dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    తిరిగి dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    ఒకవేళ (dst == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    ngx_memcpy(dst, src->దత్తాంశం, src->len);

    తిరిగి dst;
}


/*
 * supported formats:
 *    %[0][width][x][X]O        off_t
 *    %[0][width]T              time_t
 *    %[0][width][u][x|X]z      ssize_t/size_t
 *    %[0][width][u][x|X]d      int/u_int
 *    %[0][width][u][x|X]l      long
 *    %[0][width|m][u][x|X]i    ngx_int_t/ngx_uint_t
 *    %[0][width][u][x|X]D      int32_t/uint32_t
 *    %[0][width][u][x|X]L      int64_t/uint64_t
 *    %[0][width|m][u][x|X]A    ngx_atomic_int_t/ngx_atomic_uint_t
 *    %[0][width][.width]f      double, max valid number fits to %18.15f
 *    %P                        ngx_pid_t
 *    %M                        ngx_msec_t
 *    %r                        rlim_t
 *    %p                        void *
 *    %[x|X]V                   ngx_str_t *
 *    %[x|X]v                   ngx_variable_value_t *
 *    %[x|X]s                   null-terminated string
 *    %*[x|X]s                  length and string
 *    %Z                        '\0'
 *    %N                        '\n'
 *    %c                        char
 *    %%                        %
 *
 *  reserved:
 *    %t                        ptrdiff_t
 *    %S                        null-terminated wchar string
 *    %C                        wchar
 */


u_char * ngx_cdecl
ngx_sprintf(u_char *buf, స్థిరరాశి అక్షరం *fmt, ...)
{
    u_char   *p;
    va_list   వాదనలు;

    va_ప్రారంభం(వాదనలు, fmt);
    p = ngx_vslprintf(buf, (శూన్య *) -1, fmt, వాదనలు);
    va_చివర(వాదనలు);

    తిరిగి p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, కొలత_t max, స్థిరరాశి అక్షరం *fmt, ...)
{
    u_char   *p;
    va_list   వాదనలు;

    va_ప్రారంభం(వాదనలు, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, వాదనలు);
    va_చివర(వాదనలు);

    తిరిగి p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *చివరి, స్థిరరాశి అక్షరం *fmt, ...)
{
    u_char   *p;
    va_list   వాదనలు;

    va_ప్రారంభం(వాదనలు, fmt);
    p = ngx_vslprintf(buf, చివరి, fmt, వాదనలు);
    va_చివర(వాదనలు);

    తిరిగి p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *చివరి, స్థిరరాశి అక్షరం *fmt, va_list వాదనలు)
{
    u_char                *p, zero;
    పూర్ణాంకం                    d;
    ద్విగుణ                 f;
    కొలత_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             వెడల్పు, sign, hex, max_వెడల్పు, frac_వెడల్పు, scale, n;
    ngx_str_t             *v;
    ngx_variable_విలువ_t  *vv;

    అయితే (*fmt && buf < చివరి) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        ఒకవేళ (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            వెడల్పు = 0;
            sign = 1;
            hex = 0;
            max_వెడల్పు = 0;
            frac_వెడల్పు = 0;
            slen = (కొలత_t) -1;

            అయితే (*fmt >= '0' && *fmt <= '9') {
                వెడల్పు = వెడల్పు * 10 + (*fmt++ - '0');
            }


            కొరకు ( ;; ) {
                ఎంపిక (*fmt) {

                స్థితి 'u':
                    sign = 0;
                    fmt++;
                    కొనసాగు;

                స్థితి 'm':
                    max_వెడల్పు = 1;
                    fmt++;
                    కొనసాగు;

                స్థితి 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    కొనసాగు;

                స్థితి 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    కొనసాగు;

                స్థితి '.':
                    fmt++;

                    అయితే (*fmt >= '0' && *fmt <= '9') {
                        frac_వెడల్పు = frac_వెడల్పు * 10 + (*fmt++ - '0');
                    }

                    ఆపు;

                స్థితి '*':
                    slen = va_arg(వాదనలు, కొలత_t);
                    fmt++;
                    కొనసాగు;

                అప్రమేయ:
                    ఆపు;
                }

                ఆపు;
            }


            ఎంపిక (*fmt) {

            స్థితి 'V':
                v = va_arg(వాదనలు, ngx_str_t *);

                buf = ngx_sprintf_str(buf, చివరి, v->దత్తాంశం, v->len, hex);
                fmt++;

                కొనసాగు;

            స్థితి 'v':
                vv = va_arg(వాదనలు, ngx_variable_విలువ_t *);

                buf = ngx_sprintf_str(buf, చివరి, vv->దత్తాంశం, vv->len, hex);
                fmt++;

                కొనసాగు;

            స్థితి 's':
                p = va_arg(వాదనలు, u_char *);

                buf = ngx_sprintf_str(buf, చివరి, p, slen, hex);
                fmt++;

                కొనసాగు;

            స్థితి 'O':
                i64 = (int64_t) va_arg(వాదనలు, off_t);
                sign = 1;
                ఆపు;

            స్థితి 'P':
                i64 = (int64_t) va_arg(వాదనలు, ngx_pid_t);
                sign = 1;
                ఆపు;

            స్థితి 'T':
                i64 = (int64_t) va_arg(వాదనలు, సమయం_t);
                sign = 1;
                ఆపు;

            స్థితి 'M':
                ms = (ngx_msec_t) va_arg(వాదనలు, ngx_msec_t);
                ఒకవేళ ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } లేకపోతే {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                ఆపు;

            స్థితి 'z':
                ఒకవేళ (sign) {
                    i64 = (int64_t) va_arg(వాదనలు, ssize_t);
                } లేకపోతే {
                    ui64 = (uint64_t) va_arg(వాదనలు, కొలత_t);
                }
                ఆపు;

            స్థితి 'i':
                ఒకవేళ (sign) {
                    i64 = (int64_t) va_arg(వాదనలు, ngx_int_t);
                } లేకపోతే {
                    ui64 = (uint64_t) va_arg(వాదనలు, ngx_uint_t);
                }

                ఒకవేళ (max_వెడల్పు) {
                    వెడల్పు = NGX_INT_T_LEN;
                }

                ఆపు;

            స్థితి 'd':
                ఒకవేళ (sign) {
                    i64 = (int64_t) va_arg(వాదనలు, పూర్ణాంకం);
                } లేకపోతే {
                    ui64 = (uint64_t) va_arg(వాదనలు, u_int);
                }
                ఆపు;

            స్థితి 'l':
                ఒకవేళ (sign) {
                    i64 = (int64_t) va_arg(వాదనలు, దీర్ఘ);
                } లేకపోతే {
                    ui64 = (uint64_t) va_arg(వాదనలు, u_long);
                }
                ఆపు;

            స్థితి 'D':
                ఒకవేళ (sign) {
                    i64 = (int64_t) va_arg(వాదనలు, int32_t);
                } లేకపోతే {
                    ui64 = (uint64_t) va_arg(వాదనలు, uint32_t);
                }
                ఆపు;

            స్థితి 'L':
                ఒకవేళ (sign) {
                    i64 = va_arg(వాదనలు, int64_t);
                } లేకపోతే {
                    ui64 = va_arg(వాదనలు, uint64_t);
                }
                ఆపు;

            స్థితి 'A':
                ఒకవేళ (sign) {
                    i64 = (int64_t) va_arg(వాదనలు, ngx_atomic_int_t);
                } లేకపోతే {
                    ui64 = (uint64_t) va_arg(వాదనలు, ngx_atomic_uint_t);
                }

                ఒకవేళ (max_వెడల్పు) {
                    వెడల్పు = NGX_ATOMIC_T_LEN;
                }

                ఆపు;

            స్థితి 'f':
                f = va_arg(వాదనలు, ద్విగుణ);

                ఒకవేళ (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                ఒకవేళ (frac_వెడల్పు) {

                    scale = 1;
                    కొరకు (n = frac_వెడల్పు; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (ద్విగుణ) ui64) * scale + 0.5);

                    ఒకవేళ (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_అంకె(buf, చివరి, ui64, zero, 0, వెడల్పు);

                ఒకవేళ (frac_వెడల్పు) {
                    ఒకవేళ (buf < చివరి) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_అంకె(buf, చివరి, frac, '0', 0, frac_వెడల్పు);
                }

                fmt++;

                కొనసాగు;

#ఒకవేళ !(NGX_WIN32)
            స్థితి 'r':
                i64 = (int64_t) va_arg(వాదనలు, rlim_t);
                sign = 1;
                ఆపు;
#అయితే_ముగింపు

            స్థితి 'p':
                ui64 = (uintptr_t) va_arg(వాదనలు, శూన్య *);
                hex = 2;
                sign = 0;
                zero = '0';
                వెడల్పు = 2 * పరిమాణం(శూన్య *);
                ఆపు;

            స్థితి 'c':
                d = va_arg(వాదనలు, పూర్ణాంకం);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                కొనసాగు;

            స్థితి 'Z':
                *buf++ = '\0';
                fmt++;

                కొనసాగు;

            స్థితి 'N':
#ఒకవేళ (NGX_WIN32)
                *buf++ = CR;
                ఒకవేళ (buf < చివరి) {
                    *buf++ = LF;
                }
#లేకపోతే
                *buf++ = LF;
#అయితే_ముగింపు
                fmt++;

                కొనసాగు;

            స్థితి '%':
                *buf++ = '%';
                fmt++;

                కొనసాగు;

            అప్రమేయ:
                *buf++ = *fmt++;

                కొనసాగు;
            }

            ఒకవేళ (sign) {
                ఒకవేళ (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } లేకపోతే {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_అంకె(buf, చివరి, ui64, zero, hex, వెడల్పు);

            fmt++;

        } లేకపోతే {
            *buf++ = *fmt++;
        }
    }

    తిరిగి buf;
}


స్థిర u_char *
ngx_sprintf_అంకె(u_char *buf, u_char *చివరి, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t వెడల్పు)
{
    u_char         *p, తాత్కాలిక[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    కొలత_t          len;
    uint32_t        ui32;
    స్థిర u_char   hex[] = "0123456789abcdef";
    స్థిర u_char   HEX[] = "0123456789ABCDEF";

    p = తాత్కాలిక + NGX_INT64_LEN;

    ఒకవేళ (hexadecimal == 0) {

        ఒకవేళ (ui64 <= (uint64_t) NGX_MAX_UINT32_విలువ) {

            /*
             * To divide 64-bit numbers and to find remainders
             * on the x86 platform gcc and icc call the libc functions
             * [u]divdi3() and [u]moddi3(), they call another function
             * in its turn.  On FreeBSD it is the qdivrem() function,
             * its source code is about 170 lines of the code.
             * The glibc counterpart is about 150 lines of the code.
             *
             * For 32-bit numbers and some divisors gcc and icc use
             * a inlined multiplication and shifts.  For example,
             * unsigned "i32 / 10" is compiled to
             *
             *     (i32 * 0xCCCCCCCD) >> 35
             */

            ui32 = (uint32_t) ui64;

            చేయి {
                *--p = (u_char) (ui32 % 10 + '0');
            } అయితే (ui32 /= 10);

        } లేకపోతే {
            చేయి {
                *--p = (u_char) (ui64 % 10 + '0');
            } అయితే (ui64 /= 10);
        }

    } లేకపోతే ఒకవేళ (hexadecimal == 1) {

        చేయి {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } అయితే (ui64 >>= 4);

    } లేకపోతే { /* hexadecimal == 2 */

        చేయి {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } అయితే (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (తాత్కాలిక + NGX_INT64_LEN) - p;

    అయితే (len++ < వెడల్పు && buf < చివరి) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (తాత్కాలిక + NGX_INT64_LEN) - p;

    ఒకవేళ (buf + len > చివరి) {
        len = చివరి - buf;
    }

    తిరిగి ngx_cpymem(buf, p, len);
}


స్థిర u_char *
ngx_sprintf_str(u_char *buf, u_char *చివరి, u_char *src, కొలత_t len,
    ngx_uint_t hexadecimal)
{
    స్థిర u_char   hex[] = "0123456789abcdef";
    స్థిర u_char   HEX[] = "0123456789ABCDEF";

    ఒకవేళ (hexadecimal == 0) {

        ఒకవేళ (len == (కొలత_t) -1) {
            అయితే (*src && buf < చివరి) {
                *buf++ = *src++;
            }

        } లేకపోతే {
            len = ngx_min((కొలత_t) (చివరి - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } లేకపోతే ఒకవేళ (hexadecimal == 1) {

        ఒకవేళ (len == (కొలత_t) -1) {

            అయితే (*src && buf < చివరి - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } లేకపోతే {

            అయితే (len-- && buf < చివరి - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } లేకపోతే { /* hexadecimal == 2 */

        ఒకవేళ (len == (కొలత_t) -1) {

            అయితే (*src && buf < చివరి - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } లేకపోతే {

            అయితే (len-- && buf < చివరి - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    తిరిగి buf;
}


/*
 * We use ngx_strcasecmp()/ngx_strncasecmp() for 7-bit ASCII strings only,
 * and implement our own ngx_strcasecmp()/ngx_strncasecmp()
 * to avoid libc locale overhead.  Besides, we use the ngx_uint_t's
 * instead of the u_char's, because they are slightly faster.
 */

ngx_int_t
ngx_strcasecmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    కొరకు ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ఒకవేళ (c1 == c2) {

            ఒకవేళ (c1) {
                కొనసాగు;
            }

            తిరిగి 0;
        }

        తిరిగి c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, కొలత_t n)
{
    ngx_uint_t  c1, c2;

    అయితే (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ఒకవేళ (c1 == c2) {

            ఒకవేళ (c1) {
                n--;
                కొనసాగు;
            }

            తిరిగి 0;
        }

        తిరిగి c1 - c2;
    }

    తిరిగి 0;
}


u_char *
ngx_strnstr(u_char *s1, అక్షరం *s2, కొలత_t len)
{
    u_char  c1, c2;
    కొలత_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    చేయి {
        చేయి {
            ఒకవేళ (len-- == 0) {
                తిరిగి ఖాళీ;
            }

            c1 = *s1++;

            ఒకవేళ (c1 == 0) {
                తిరిగి ఖాళీ;
            }

        } అయితే (c1 != c2);

        ఒకవేళ (n > len) {
            తిరిగి ఖాళీ;
        }

    } అయితే (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    తిరిగి --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, అక్షరం *s2, కొలత_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    చేయి {
        చేయి {
            c1 = *s1++;

            ఒకవేళ (c1 == 0) {
                తిరిగి ఖాళీ;
            }

        } అయితే (c1 != c2);

    } అయితే (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    తిరిగి --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, అక్షరం *s2, కొలత_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    చేయి {
        చేయి {
            c1 = (ngx_uint_t) *s1++;

            ఒకవేళ (c1 == 0) {
                తిరిగి ఖాళీ;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } అయితే (c1 != c2);

    } అయితే (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    తిరిగి --s1;
}


/*
 * ngx_strlcasestrn() is intended to search for static substring
 * with known length in string until the argument last. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strlcasestrn(u_char *s1, u_char *చివరి, u_char *s2, కొలత_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;
    చివరి -= n;

    చేయి {
        చేయి {
            ఒకవేళ (s1 >= చివరి) {
                తిరిగి ఖాళీ;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } అయితే (c1 != c2);

    } అయితే (ngx_strncasecmp(s1, s2, n) != 0);

    తిరిగి --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, కొలత_t n)
{
    ఒకవేళ (n == 0) {
        తిరిగి 0;
    }

    n--;

    కొరకు ( ;; ) {
        ఒకవేళ (s1[n] != s2[n]) {
            తిరిగి s1[n] - s2[n];
        }

        ఒకవేళ (n == 0) {
            తిరిగి 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, కొలత_t n)
{
    u_char  c1, c2;

    ఒకవేళ (n == 0) {
        తిరిగి 0;
    }

    n--;

    కొరకు ( ;; ) {
        c1 = s1[n];
        ఒకవేళ (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        ఒకవేళ (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        ఒకవేళ (c1 != c2) {
            తిరిగి c1 - c2;
        }

        ఒకవేళ (n == 0) {
            తిరిగి 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, కొలత_t n1, కొలత_t n2)
{
    కొలత_t     n;
    ngx_int_t  m, z;

    ఒకవేళ (n1 <= n2) {
        n = n1;
        z = -1;

    } లేకపోతే {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    ఒకవేళ (m || n1 == n2) {
        తిరిగి m;
    }

    తిరిగి z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    కొరకు ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ఒకవేళ (c1 == c2) {

            ఒకవేళ (c1) {
                కొనసాగు;
            }

            తిరిగి 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        తిరిగి c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, కొలత_t n)
{
    ngx_uint_t  c1, c2;

    అయితే (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#ఒకవేళ (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#అయితే_ముగింపు

        ఒకవేళ (c1 == c2) {

            ఒకవేళ (c1) {
                n--;
                కొనసాగు;
            }

            తిరిగి 0;
        }

        /* we need '/' to be the lowest character */

        ఒకవేళ (c1 == 0 || c2 == 0) {
            తిరిగి c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        తిరిగి c1 - c2;
    }

    తిరిగి 0;
}


ngx_int_t
ngx_atoi(u_char *పంక్తి, కొలత_t n)
{
    ngx_int_t  విలువ, cutoff, cutlim;

    ఒకవేళ (n == 0) {
        తిరిగి NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_విలువ / 10;
    cutlim = NGX_MAX_INT_T_విలువ % 10;

    కొరకు (విలువ = 0; n--; పంక్తి++) {
        ఒకవేళ (*పంక్తి < '0' || *పంక్తి > '9') {
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (విలువ >= cutoff && (విలువ > cutoff || *పంక్తి - '0' > cutlim)) {
            తిరిగి NGX_ERROR;
        }

        విలువ = విలువ * 10 + (*పంక్తి - '0');
    }

    తిరిగి విలువ;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *పంక్తి, కొలత_t n, కొలత_t బిందువు)
{
    ngx_int_t   విలువ, cutoff, cutlim;
    ngx_uint_t  dot;

    ఒకవేళ (n == 0) {
        తిరిగి NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_విలువ / 10;
    cutlim = NGX_MAX_INT_T_విలువ % 10;

    dot = 0;

    కొరకు (విలువ = 0; n--; పంక్తి++) {

        ఒకవేళ (బిందువు == 0) {
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (*పంక్తి == '.') {
            ఒకవేళ (dot) {
                తిరిగి NGX_ERROR;
            }

            dot = 1;
            కొనసాగు;
        }

        ఒకవేళ (*పంక్తి < '0' || *పంక్తి > '9') {
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (విలువ >= cutoff && (విలువ > cutoff || *పంక్తి - '0' > cutlim)) {
            తిరిగి NGX_ERROR;
        }

        విలువ = విలువ * 10 + (*పంక్తి - '0');
        బిందువు -= dot;
    }

    అయితే (బిందువు--) {
        ఒకవేళ (విలువ > cutoff) {
            తిరిగి NGX_ERROR;
        }

        విలువ = విలువ * 10;
    }

    తిరిగి విలువ;
}


ssize_t
ngx_atosz(u_char *పంక్తి, కొలత_t n)
{
    ssize_t  విలువ, cutoff, cutlim;

    ఒకవేళ (n == 0) {
        తిరిగి NGX_ERROR;
    }

    cutoff = NGX_MAX_కొలత_T_విలువ / 10;
    cutlim = NGX_MAX_కొలత_T_విలువ % 10;

    కొరకు (విలువ = 0; n--; పంక్తి++) {
        ఒకవేళ (*పంక్తి < '0' || *పంక్తి > '9') {
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (విలువ >= cutoff && (విలువ > cutoff || *పంక్తి - '0' > cutlim)) {
            తిరిగి NGX_ERROR;
        }

        విలువ = విలువ * 10 + (*పంక్తి - '0');
    }

    తిరిగి విలువ;
}


off_t
ngx_atoof(u_char *పంక్తి, కొలత_t n)
{
    off_t  విలువ, cutoff, cutlim;

    ఒకవేళ (n == 0) {
        తిరిగి NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_విలువ / 10;
    cutlim = NGX_MAX_OFF_T_విలువ % 10;

    కొరకు (విలువ = 0; n--; పంక్తి++) {
        ఒకవేళ (*పంక్తి < '0' || *పంక్తి > '9') {
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (విలువ >= cutoff && (విలువ > cutoff || *పంక్తి - '0' > cutlim)) {
            తిరిగి NGX_ERROR;
        }

        విలువ = విలువ * 10 + (*పంక్తి - '0');
    }

    తిరిగి విలువ;
}


సమయం_t
ngx_atotm(u_char *పంక్తి, కొలత_t n)
{
    సమయం_t  విలువ, cutoff, cutlim;

    ఒకవేళ (n == 0) {
        తిరిగి NGX_ERROR;
    }

    cutoff = NGX_MAX_సమయం_T_విలువ / 10;
    cutlim = NGX_MAX_సమయం_T_విలువ % 10;

    కొరకు (విలువ = 0; n--; పంక్తి++) {
        ఒకవేళ (*పంక్తి < '0' || *పంక్తి > '9') {
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (విలువ >= cutoff && (విలువ > cutoff || *పంక్తి - '0' > cutlim)) {
            తిరిగి NGX_ERROR;
        }

        విలువ = విలువ * 10 + (*పంక్తి - '0');
    }

    తిరిగి విలువ;
}


ngx_int_t
ngx_hextoi(u_char *పంక్తి, కొలత_t n)
{
    u_char     c, ch;
    ngx_int_t  విలువ, cutoff;

    ఒకవేళ (n == 0) {
        తిరిగి NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_విలువ / 16;

    కొరకు (విలువ = 0; n--; పంక్తి++) {
        ఒకవేళ (విలువ > cutoff) {
            తిరిగి NGX_ERROR;
        }

        ch = *పంక్తి;

        ఒకవేళ (ch >= '0' && ch <= '9') {
            విలువ = విలువ * 16 + (ch - '0');
            కొనసాగు;
        }

        c = (u_char) (ch | 0x20);

        ఒకవేళ (c >= 'a' && c <= 'f') {
            విలువ = విలువ * 16 + (c - 'a' + 10);
            కొనసాగు;
        }

        తిరిగి NGX_ERROR;
    }

    తిరిగి విలువ;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, కొలత_t len)
{
    స్థిర u_char  hex[] = "0123456789abcdef";

    అయితే (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    తిరిగి dst;
}


శూన్య
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    స్థిర u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_అంతర్గత(dst, src, basis64, 1);
}


శూన్య
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    స్థిర u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_అంతర్గత(dst, src, basis64, 0);
}


స్థిర శూన్య
ngx_encode_base64_అంతర్గత(ngx_str_t *dst, ngx_str_t *src, స్థిరరాశి u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    కొలత_t          len;

    len = src->len;
    s = src->దత్తాంశం;
    d = dst->దత్తాంశం;

    అయితే (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    ఒకవేళ (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        ఒకవేళ (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            ఒకవేళ (padding) {
                *d++ = '=';
            }

        } లేకపోతే {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        ఒకవేళ (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->దత్తాంశం;
}


ngx_int_t
ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    స్థిర u_char   basis64[] = {
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 62, 77, 77, 77, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 77, 77, 77, 77, 77, 77,
        77,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 77, 77, 77, 77, 77,
        77, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 77, 77, 77, 77, 77,

        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77
    };

    తిరిగి ngx_decode_base64_అంతర్గత(dst, src, basis64);
}


ngx_int_t
ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    స్థిర u_char   basis64[] = {
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 62, 77, 77,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 77, 77, 77, 77, 77, 77,
        77,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 77, 77, 77, 77, 63,
        77, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 77, 77, 77, 77, 77,

        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77
    };

    తిరిగి ngx_decode_base64_అంతర్గత(dst, src, basis64);
}


స్థిర ngx_int_t
ngx_decode_base64_అంతర్గత(ngx_str_t *dst, ngx_str_t *src, స్థిరరాశి u_char *basis)
{
    కొలత_t          len;
    u_char         *d, *s;

    కొరకు (len = 0; len < src->len; len++) {
        ఒకవేళ (src->దత్తాంశం[len] == '=') {
            ఆపు;
        }

        ఒకవేళ (basis[src->దత్తాంశం[len]] == 77) {
            తిరిగి NGX_ERROR;
        }
    }

    ఒకవేళ (len % 4 == 1) {
        తిరిగి NGX_ERROR;
    }

    s = src->దత్తాంశం;
    d = dst->దత్తాంశం;

    అయితే (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    ఒకవేళ (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    ఒకవేళ (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->దత్తాంశం;

    తిరిగి NGX_OK;
}


/*
 * ngx_utf8_decode() decodes two and more bytes UTF sequences only
 * the return values:
 *    0x80 - 0x10ffff         valid character
 *    0x110000 - 0xfffffffd   invalid sequence
 *    0xfffffffe              incomplete sequence
 *    0xffffffff              error
 */

uint32_t
ngx_utf8_decode(u_char **p, కొలత_t n)
{
    కొలత_t    len;
    uint32_t  u, i, valid;

    u = **p;

    ఒకవేళ (u >= 0xf8) {

        (*p)++;
        తిరిగి 0xffffffff;

    } లేకపోతే ఒకవేళ (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } లేకపోతే ఒకవేళ (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } లేకపోతే ఒకవేళ (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } లేకపోతే {
        (*p)++;
        తిరిగి 0xffffffff;
    }

    ఒకవేళ (n - 1 < len) {
        తిరిగి 0xfffffffe;
    }

    (*p)++;

    అయితే (len) {
        i = *(*p)++;

        ఒకవేళ (i < 0x80) {
            తిరిగి 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    ఒకవేళ (u > valid) {
        తిరిగి u;
    }

    తిరిగి 0xffffffff;
}


కొలత_t
ngx_utf8_నిడివి(u_char *p, కొలత_t n)
{
    u_char  c, *చివరి;
    కొలత_t  len;

    చివరి = p + n;

    కొరకు (len = 0; p < చివరి; len++) {

        c = *p;

        ఒకవేళ (c < 0x80) {
            p++;
            కొనసాగు;
        }

        ఒకవేళ (ngx_utf8_decode(&p, చివరి - p) > 0x10ffff) {
            /* invalid UTF-8 */
            తిరిగి n;
        }
    }

    తిరిగి len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, కొలత_t n, కొలత_t len)
{
    u_char  c, *తదుపరి;

    ఒకవేళ (n == 0) {
        తిరిగి dst;
    }

    అయితే (--n) {

        c = *src;
        *dst = c;

        ఒకవేళ (c < 0x80) {

            ఒకవేళ (c != '\0') {
                dst++;
                src++;
                len--;

                కొనసాగు;
            }

            తిరిగి dst;
        }

        తదుపరి = src;

        ఒకవేళ (ngx_utf8_decode(&తదుపరి, len) > 0x10ffff) {
            /* invalid UTF-8 */
            ఆపు;
        }

        అయితే (src < తదుపరి) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    తిరిగి dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, కొలత_t కొలత, ngx_uint_t type)
{
    u_char          ఉపసర్గ;
    uint32_t       *escape;
    ngx_uint_t      n;
    స్థిర u_char   hex[] = "0123456789ABCDEF";

    /*
     * Per RFC 3986 only the following chars are allowed in URIs unescaped:
     *
     * unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
     * gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
     * sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
     *               / "*" / "+" / "," / ";" / "="
     *
     * And "%" can appear as a part of escaping itself.  The following
     * characters are not allowed and need to be escaped: %00-%1F, %7F-%FF,
     * " ", """, "<", ">", "\", "^", "`", "{", "|", "}".
     */

                    /* " ", "#", "%", "?", not allowed */

    స్థిర uint32_t   uri[] = {
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */

                    /* ?>=< ;:98 7654 3210  /.-, +*)( '&%$ #"!  */
        0xd000002d, /* 1101 0000 0000 0000  0000 0000 0010 1101 */

                    /* _^]\ [ZYX WVUT SRQP  ONML KJIH GFED CBA@ */
        0x50000000, /* 0101 0000 0000 0000  0000 0000 0000 0000 */

                    /*  ~}| {zyx wvut srqp  onml kjih gfed cba` */
        0xb8000001, /* 1011 1000 0000 0000  0000 0000 0000 0001 */

        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff  /* 1111 1111 1111 1111  1111 1111 1111 1111 */
    };

                    /* " ", "#", "%", "&", "+", ";", "?", not allowed */

    స్థిర uint32_t   వాదనలు[] = {
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */

                    /* ?>=< ;:98 7654 3210  /.-, +*)( '&%$ #"!  */
        0xd800086d, /* 1101 1000 0000 0000  0000 1000 0110 1101 */

                    /* _^]\ [ZYX WVUT SRQP  ONML KJIH GFED CBA@ */
        0x50000000, /* 0101 0000 0000 0000  0000 0000 0000 0000 */

                    /*  ~}| {zyx wvut srqp  onml kjih gfed cba` */
        0xb8000001, /* 1011 1000 0000 0000  0000 0000 0000 0001 */

        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff  /* 1111 1111 1111 1111  1111 1111 1111 1111 */
    };

                    /* not ALPHA, DIGIT, "-", ".", "_", "~" */

    స్థిర uint32_t   uri_component[] = {
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */

                    /* ?>=< ;:98 7654 3210  /.-, +*)( '&%$ #"!  */
        0xfc009fff, /* 1111 1100 0000 0000  1001 1111 1111 1111 */

                    /* _^]\ [ZYX WVUT SRQP  ONML KJIH GFED CBA@ */
        0x78000001, /* 0111 1000 0000 0000  0000 0000 0000 0001 */

                    /*  ~}| {zyx wvut srqp  onml kjih gfed cba` */
        0xb8000001, /* 1011 1000 0000 0000  0000 0000 0000 0001 */

        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff  /* 1111 1111 1111 1111  1111 1111 1111 1111 */
    };

                    /* " ", "#", """, "%", "'", not allowed */

    స్థిర uint32_t   html[] = {
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */

                    /* ?>=< ;:98 7654 3210  /.-, +*)( '&%$ #"!  */
        0x500000ad, /* 0101 0000 0000 0000  0000 0000 1010 1101 */

                    /* _^]\ [ZYX WVUT SRQP  ONML KJIH GFED CBA@ */
        0x50000000, /* 0101 0000 0000 0000  0000 0000 0000 0000 */

                    /*  ~}| {zyx wvut srqp  onml kjih gfed cba` */
        0xb8000001, /* 1011 1000 0000 0000  0000 0000 0000 0001 */

        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff  /* 1111 1111 1111 1111  1111 1111 1111 1111 */
    };

                    /* " ", """, "'", not allowed */

    స్థిర uint32_t   refresh[] = {
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */

                    /* ?>=< ;:98 7654 3210  /.-, +*)( '&%$ #"!  */
        0x50000085, /* 0101 0000 0000 0000  0000 0000 1000 0101 */

                    /* _^]\ [ZYX WVUT SRQP  ONML KJIH GFED CBA@ */
        0x50000000, /* 0101 0000 0000 0000  0000 0000 0000 0000 */

                    /*  ~}| {zyx wvut srqp  onml kjih gfed cba` */
        0xd8000001, /* 1011 1000 0000 0000  0000 0000 0000 0001 */

        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff  /* 1111 1111 1111 1111  1111 1111 1111 1111 */
    };

                    /* " ", "%", %00-%1F */

    స్థిర uint32_t   memcached[] = {
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */

                    /* ?>=< ;:98 7654 3210  /.-, +*)( '&%$ #"!  */
        0x00000021, /* 0000 0000 0000 0000  0000 0000 0010 0001 */

                    /* _^]\ [ZYX WVUT SRQP  ONML KJIH GFED CBA@ */
        0x00000000, /* 0000 0000 0000 0000  0000 0000 0000 0000 */

                    /*  ~}| {zyx wvut srqp  onml kjih gfed cba` */
        0x00000000, /* 0000 0000 0000 0000  0000 0000 0000 0000 */

        0x00000000, /* 0000 0000 0000 0000  0000 0000 0000 0000 */
        0x00000000, /* 0000 0000 0000 0000  0000 0000 0000 0000 */
        0x00000000, /* 0000 0000 0000 0000  0000 0000 0000 0000 */
        0x00000000, /* 0000 0000 0000 0000  0000 0000 0000 0000 */
    };

                    /* mail_auth is the same as memcached */

                    /* " ", "+", "=", not allowed */

    స్థిర uint32_t   mail_xtext[] = {
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */

                    /* ?>=< ;:98 7654 3210  /.-, +*)( '&%$ #"!  */
        0x20000801, /* 0010 0000 0000 0000  0000 1000 0000 0001 */

                    /* _^]\ [ZYX WVUT SRQP  ONML KJIH GFED CBA@ */
        0x00000000, /* 0000 0000 0000 0000  0000 0000 0000 0000 */

                    /*  ~}| {zyx wvut srqp  onml kjih gfed cba` */
        0x80000000, /* 1000 0000 0000 0000  0000 0000 0000 0000 */

        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
        0xffffffff, /* 1111 1111 1111 1111  1111 1111 1111 1111 */
    };

    స్థిర uint32_t  *map[] =
        { uri, వాదనలు, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    స్థిర u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    ఉపసర్గ = map_char[type];

    ఒకవేళ (dst == ఖాళీ) {

        /* find the number of the characters to be escaped */

        n = 0;

        అయితే (కొలత) {
            ఒకవేళ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            కొలత--;
        }

        తిరిగి (uintptr_t) n;
    }

    అయితే (కొలత) {
        ఒకవేళ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = ఉపసర్గ;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } లేకపోతే {
            *dst++ = *src++;
        }
        కొలత--;
    }

    తిరిగి (uintptr_t) dst;
}


శూన్య
ngx_unescape_uri(u_char **dst, u_char **src, కొలత_t కొలత, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    గణన {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_రెండవ
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    అయితే (కొలత--) {

        ch = *s++;

        ఎంపిక (state) {
        స్థితి sw_usual:
            ఒకవేళ (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                వెళ్ళు పూర్తి;
            }

            ఒకవేళ (ch == '%') {
                state = sw_quoted;
                ఆపు;
            }

            *d++ = ch;
            ఆపు;

        స్థితి sw_quoted:

            ఒకవేళ (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_రెండవ;
                ఆపు;
            }

            c = (u_char) (ch | 0x20);
            ఒకవేళ (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_రెండవ;
                ఆపు;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            ఆపు;

        స్థితి sw_quoted_రెండవ:

            state = sw_usual;

            ఒకవేళ (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                ఒకవేళ (type & NGX_UNESCAPE_REDIRECT) {
                    ఒకవేళ (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        ఆపు;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    ఆపు;
                }

                *d++ = ch;

                ఆపు;
            }

            c = (u_char) (ch | 0x20);
            ఒకవేళ (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                ఒకవేళ (type & NGX_UNESCAPE_URI) {
                    ఒకవేళ (ch == '?') {
                        *d++ = ch;
                        వెళ్ళు పూర్తి;
                    }

                    *d++ = ch;
                    ఆపు;
                }

                ఒకవేళ (type & NGX_UNESCAPE_REDIRECT) {
                    ఒకవేళ (ch == '?') {
                        *d++ = ch;
                        వెళ్ళు పూర్తి;
                    }

                    ఒకవేళ (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        ఆపు;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    ఆపు;
                }

                *d++ = ch;

                ఆపు;
            }

            /* the invalid quoted character */

            ఆపు;
        }
    }

పూర్తి:

    *dst = d;
    *src = s;
}


uintptr_t
ngx_escape_html(u_char *dst, u_char *src, కొలత_t కొలత)
{
    u_char      ch;
    ngx_uint_t  len;

    ఒకవేళ (dst == ఖాళీ) {

        len = 0;

        అయితే (కొలత) {
            ఎంపిక (*src++) {

            స్థితి '<':
                len += పరిమాణం("&lt;") - 2;
                ఆపు;

            స్థితి '>':
                len += పరిమాణం("&gt;") - 2;
                ఆపు;

            స్థితి '&':
                len += పరిమాణం("&amp;") - 2;
                ఆపు;

            స్థితి '"':
                len += పరిమాణం("&quot;") - 2;
                ఆపు;

            అప్రమేయ:
                ఆపు;
            }
            కొలత--;
        }

        తిరిగి (uintptr_t) len;
    }

    అయితే (కొలత) {
        ch = *src++;

        ఎంపిక (ch) {

        స్థితి '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            ఆపు;

        స్థితి '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            ఆపు;

        స్థితి '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            ఆపు;

        స్థితి '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            ఆపు;

        అప్రమేయ:
            *dst++ = ch;
            ఆపు;
        }
        కొలత--;
    }

    తిరిగి (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, కొలత_t కొలత)
{
    u_char      ch;
    ngx_uint_t  len;

    ఒకవేళ (dst == ఖాళీ) {
        len = 0;

        అయితే (కొలత) {
            ch = *src++;

            ఒకవేళ (ch == '\\' || ch == '"') {
                len++;

            } లేకపోతే ఒకవేళ (ch <= 0x1f) {

                ఎంపిక (ch) {
                స్థితి '\n':
                స్థితి '\r':
                స్థితి '\t':
                స్థితి '\b':
                స్థితి '\f':
                    len++;
                    ఆపు;

                అప్రమేయ:
                    len += పరిమాణం("\\u001F") - 2;
                }
            }

            కొలత--;
        }

        తిరిగి (uintptr_t) len;
    }

    అయితే (కొలత) {
        ch = *src++;

        ఒకవేళ (ch > 0x1f) {

            ఒకవేళ (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } లేకపోతే {
            *dst++ = '\\';

            ఎంపిక (ch) {
            స్థితి '\n':
                *dst++ = 'n';
                ఆపు;

            స్థితి '\r':
                *dst++ = 'r';
                ఆపు;

            స్థితి '\t':
                *dst++ = 't';
                ఆపు;

            స్థితి '\b':
                *dst++ = 'b';
                ఆపు;

            స్థితి '\f':
                *dst++ = 'f';
                ఆపు;

            అప్రమేయ:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        కొలత--;
    }

    తిరిగి (uintptr_t) dst;
}


శూన్య
ngx_str_rbtree_చొప్పించు_విలువ(ngx_rbtree_కణుపు_t *తాత్కాలిక,
    ngx_rbtree_కణుపు_t *కణుపు, ngx_rbtree_కణుపు_t *sentinel)
{
    ngx_str_కణుపు_t      *n, *t;
    ngx_rbtree_కణుపు_t  **p;

    కొరకు ( ;; ) {

        n = (ngx_str_కణుపు_t *) కణుపు;
        t = (ngx_str_కణుపు_t *) తాత్కాలిక;

        ఒకవేళ (కణుపు->కీలకం != తాత్కాలిక->కీలకం) {

            p = (కణుపు->కీలకం < తాత్కాలిక->కీలకం) ? &తాత్కాలిక->ఎడమ : &తాత్కాలిక->కుడి;

        } లేకపోతే ఒకవేళ (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &తాత్కాలిక->ఎడమ : &తాత్కాలిక->కుడి;

        } లేకపోతే {
            p = (ngx_memcmp(n->str.దత్తాంశం, t->str.దత్తాంశం, n->str.len) < 0)
                 ? &తాత్కాలిక->ఎడమ : &తాత్కాలిక->కుడి;
        }

        ఒకవేళ (*p == sentinel) {
            ఆపు;
        }

        తాత్కాలిక = *p;
    }

    *p = కణుపు;
    కణుపు->మాతృ = తాత్కాలిక;
    కణుపు->ఎడమ = sentinel;
    కణుపు->కుడి = sentinel;
    ngx_rbt_red(కణుపు);
}


ngx_str_కణుపు_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_కణుపు_t     *n;
    ngx_rbtree_కణుపు_t  *కణుపు, *sentinel;

    కణుపు = rbtree->మూలం;
    sentinel = rbtree->sentinel;

    అయితే (కణుపు != sentinel) {

        n = (ngx_str_కణుపు_t *) కణుపు;

        ఒకవేళ (hash != కణుపు->కీలకం) {
            కణుపు = (hash < కణుపు->కీలకం) ? కణుపు->ఎడమ : కణుపు->కుడి;
            కొనసాగు;
        }

        ఒకవేళ (val->len != n->str.len) {
            కణుపు = (val->len < n->str.len) ? కణుపు->ఎడమ : కణుపు->కుడి;
            కొనసాగు;
        }

        rc = ngx_memcmp(val->దత్తాంశం, n->str.దత్తాంశం, val->len);

        ఒకవేళ (rc < 0) {
            కణుపు = కణుపు->ఎడమ;
            కొనసాగు;
        }

        ఒకవేళ (rc > 0) {
            కణుపు = కణుపు->కుడి;
            కొనసాగు;
        }

        తిరిగి n;
    }

    తిరిగి ఖాళీ;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

శూన్య
ngx_క్రమీకరించు(శూన్య *పునాది, కొలత_t n, కొలత_t కొలత,
    ngx_int_t (*cmp)(స్థిరరాశి శూన్య *, స్థిరరాశి శూన్య *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(కొలత, ngx_చక్రం->సంవర్గమానం);
    ఒకవేళ (p == ఖాళీ) {
        తిరిగి;
    }

    కొరకు (p1 = (u_char *) పునాది + కొలత;
         p1 < (u_char *) పునాది + n * కొలత;
         p1 += కొలత)
    {
        ngx_memcpy(p, p1, కొలత);

        కొరకు (p2 = p1;
             p2 > (u_char *) పునాది && cmp(p2 - కొలత, p) > 0;
             p2 -= కొలత)
        {
            ngx_memcpy(p2, p2 - కొలత, కొలత);
        }

        ngx_memcpy(p2, p, కొలత);
    }

    ngx_free(p);
}


శూన్య
ngx_explicit_memzero(శూన్య *buf, కొలత_t n)
{
    ngx_memzero(buf, n);
    ngx_జ్ఞాపకం_barrier();
}


#ఒకవేళ (NGX_MEMCPY_హద్దు)

శూన్య *
ngx_memcpy(శూన్య *dst, స్థిరరాశి శూన్య *src, కొలత_t n)
{
    ఒకవేళ (n > NGX_MEMCPY_హద్దు) {
        ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, 0, "memcpy %uz bytes", n);
        ngx_debug_బిందువు();
    }

    తిరిగి జ్ఞాపకం_నకలు(dst, src, n);
}

#అయితే_ముగింపు
