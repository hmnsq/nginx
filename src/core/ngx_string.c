
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ایستا u_char *ngx_sprintf_رقم(u_char *buf, u_char *last, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
ایستا u_char *ngx_sprintf_str(u_char *buf, u_char *last, u_char *src,
    size_t len, ngx_uint_t hexadecimal);
ایستا تهی ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ثابت u_char *basis, ngx_uint_t padding);
ایستا ngx_int_t ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ثابت u_char *basis);


تهی
ngx_strlow(u_char *dst, u_char *src, size_t n)
{
    وقتی (n) {
        *dst = ngx_tolower(*src);
        dst++;
        src++;
        n--;
    }
}


size_t
ngx_strnlen(u_char *p, size_t n)
{
    size_t  i;

    برای (i = 0; i < n; i++) {

        اگر (p[i] == '\0') {
            بازگشت i;
        }
    }

    بازگشت n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, size_t n)
{
    اگر (n == 0) {
        بازگشت dst;
    }

    وقتی (--n) {
        *dst = *src;

        اگر (*dst == '\0') {
            بازگشت dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    بازگشت dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    اگر (dst == پوچ) {
        بازگشت پوچ;
    }

    ngx_memcpy(dst, src->data, src->len);

    بازگشت dst;
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
ngx_sprintf(u_char *buf, ثابت نویسه *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, (تهی *) -1, fmt, args);
    va_end(args);

    بازگشت p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, size_t max, ثابت نویسه *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, args);
    va_end(args);

    بازگشت p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *last, ثابت نویسه *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, last, fmt, args);
    va_end(args);

    بازگشت p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *last, ثابت نویسه *fmt, va_list args)
{
    u_char                *p, zero;
    صحیح                    d;
    دوگانه                 f;
    size_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             width, sign, hex, max_width, frac_width, scale, n;
    ngx_str_t             *v;
    ngx_variable_مقدار_t  *vv;

    وقتی (*fmt && buf < last) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        اگر (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (size_t) -1;

            وقتی (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }


            برای ( ;; ) {
                انتخاب (*fmt) {

                حالت 'u':
                    sign = 0;
                    fmt++;
                    ادامه;

                حالت 'm':
                    max_width = 1;
                    fmt++;
                    ادامه;

                حالت 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    ادامه;

                حالت 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    ادامه;

                حالت '.':
                    fmt++;

                    وقتی (*fmt >= '0' && *fmt <= '9') {
                        frac_width = frac_width * 10 + (*fmt++ - '0');
                    }

                    بشکن;

                حالت '*':
                    slen = va_arg(args, size_t);
                    fmt++;
                    ادامه;

                پیشفرض:
                    بشکن;
                }

                بشکن;
            }


            انتخاب (*fmt) {

            حالت 'V':
                v = va_arg(args, ngx_str_t *);

                buf = ngx_sprintf_str(buf, last, v->data, v->len, hex);
                fmt++;

                ادامه;

            حالت 'v':
                vv = va_arg(args, ngx_variable_مقدار_t *);

                buf = ngx_sprintf_str(buf, last, vv->data, vv->len, hex);
                fmt++;

                ادامه;

            حالت 's':
                p = va_arg(args, u_char *);

                buf = ngx_sprintf_str(buf, last, p, slen, hex);
                fmt++;

                ادامه;

            حالت 'O':
                i64 = (int64_t) va_arg(args, off_t);
                sign = 1;
                بشکن;

            حالت 'P':
                i64 = (int64_t) va_arg(args, ngx_pid_t);
                sign = 1;
                بشکن;

            حالت 'T':
                i64 = (int64_t) va_arg(args, time_t);
                sign = 1;
                بشکن;

            حالت 'M':
                ms = (ngx_msec_t) va_arg(args, ngx_msec_t);
                اگر ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } وگرنه {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                بشکن;

            حالت 'z':
                اگر (sign) {
                    i64 = (int64_t) va_arg(args, ssize_t);
                } وگرنه {
                    ui64 = (uint64_t) va_arg(args, size_t);
                }
                بشکن;

            حالت 'i':
                اگر (sign) {
                    i64 = (int64_t) va_arg(args, ngx_int_t);
                } وگرنه {
                    ui64 = (uint64_t) va_arg(args, ngx_uint_t);
                }

                اگر (max_width) {
                    width = NGX_INT_T_LEN;
                }

                بشکن;

            حالت 'd':
                اگر (sign) {
                    i64 = (int64_t) va_arg(args, صحیح);
                } وگرنه {
                    ui64 = (uint64_t) va_arg(args, u_int);
                }
                بشکن;

            حالت 'l':
                اگر (sign) {
                    i64 = (int64_t) va_arg(args, long);
                } وگرنه {
                    ui64 = (uint64_t) va_arg(args, u_long);
                }
                بشکن;

            حالت 'D':
                اگر (sign) {
                    i64 = (int64_t) va_arg(args, int32_t);
                } وگرنه {
                    ui64 = (uint64_t) va_arg(args, uint32_t);
                }
                بشکن;

            حالت 'L':
                اگر (sign) {
                    i64 = va_arg(args, int64_t);
                } وگرنه {
                    ui64 = va_arg(args, uint64_t);
                }
                بشکن;

            حالت 'A':
                اگر (sign) {
                    i64 = (int64_t) va_arg(args, ngx_atomic_int_t);
                } وگرنه {
                    ui64 = (uint64_t) va_arg(args, ngx_atomic_uint_t);
                }

                اگر (max_width) {
                    width = NGX_ATOMIC_T_LEN;
                }

                بشکن;

            حالت 'f':
                f = va_arg(args, دوگانه);

                اگر (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                اگر (frac_width) {

                    scale = 1;
                    برای (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (دوگانه) ui64) * scale + 0.5);

                    اگر (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_رقم(buf, last, ui64, zero, 0, width);

                اگر (frac_width) {
                    اگر (buf < last) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_رقم(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                ادامه;

#اگر !(NGX_WIN32)
            حالت 'r':
                i64 = (int64_t) va_arg(args, rlim_t);
                sign = 1;
                بشکن;
#endif

            حالت 'p':
                ui64 = (uintptr_t) va_arg(args, تهی *);
                hex = 2;
                sign = 0;
                zero = '0';
                width = 2 * اندازه(تهی *);
                بشکن;

            حالت 'c':
                d = va_arg(args, صحیح);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                ادامه;

            حالت 'Z':
                *buf++ = '\0';
                fmt++;

                ادامه;

            حالت 'N':
#اگر (NGX_WIN32)
                *buf++ = CR;
                اگر (buf < last) {
                    *buf++ = LF;
                }
#وگرنه
                *buf++ = LF;
#endif
                fmt++;

                ادامه;

            حالت '%':
                *buf++ = '%';
                fmt++;

                ادامه;

            پیشفرض:
                *buf++ = *fmt++;

                ادامه;
            }

            اگر (sign) {
                اگر (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } وگرنه {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_رقم(buf, last, ui64, zero, hex, width);

            fmt++;

        } وگرنه {
            *buf++ = *fmt++;
        }
    }

    بازگشت buf;
}


ایستا u_char *
ngx_sprintf_رقم(u_char *buf, u_char *last, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t width)
{
    u_char         *p, temp[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    size_t          len;
    uint32_t        ui32;
    ایستا u_char   hex[] = "0123456789abcdef";
    ایستا u_char   HEX[] = "0123456789ABCDEF";

    p = temp + NGX_INT64_LEN;

    اگر (hexadecimal == 0) {

        اگر (ui64 <= (uint64_t) NGX_MAX_UINT32_مقدار) {

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

            انجامبده {
                *--p = (u_char) (ui32 % 10 + '0');
            } وقتی (ui32 /= 10);

        } وگرنه {
            انجامبده {
                *--p = (u_char) (ui64 % 10 + '0');
            } وقتی (ui64 /= 10);
        }

    } وگرنه اگر (hexadecimal == 1) {

        انجامبده {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } وقتی (ui64 >>= 4);

    } وگرنه { /* hexadecimal == 2 */

        انجامبده {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } وقتی (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    وقتی (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    اگر (buf + len > last) {
        len = last - buf;
    }

    بازگشت ngx_cpymem(buf, p, len);
}


ایستا u_char *
ngx_sprintf_str(u_char *buf, u_char *last, u_char *src, size_t len,
    ngx_uint_t hexadecimal)
{
    ایستا u_char   hex[] = "0123456789abcdef";
    ایستا u_char   HEX[] = "0123456789ABCDEF";

    اگر (hexadecimal == 0) {

        اگر (len == (size_t) -1) {
            وقتی (*src && buf < last) {
                *buf++ = *src++;
            }

        } وگرنه {
            len = ngx_min((size_t) (last - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } وگرنه اگر (hexadecimal == 1) {

        اگر (len == (size_t) -1) {

            وقتی (*src && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } وگرنه {

            وقتی (len-- && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } وگرنه { /* hexadecimal == 2 */

        اگر (len == (size_t) -1) {

            وقتی (*src && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } وگرنه {

            وقتی (len-- && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    بازگشت buf;
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

    برای ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        اگر (c1 == c2) {

            اگر (c1) {
                ادامه;
            }

            بازگشت 0;
        }

        بازگشت c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    وقتی (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        اگر (c1 == c2) {

            اگر (c1) {
                n--;
                ادامه;
            }

            بازگشت 0;
        }

        بازگشت c1 - c2;
    }

    بازگشت 0;
}


u_char *
ngx_strnstr(u_char *s1, نویسه *s2, size_t len)
{
    u_char  c1, c2;
    size_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    انجامبده {
        انجامبده {
            اگر (len-- == 0) {
                بازگشت پوچ;
            }

            c1 = *s1++;

            اگر (c1 == 0) {
                بازگشت پوچ;
            }

        } وقتی (c1 != c2);

        اگر (n > len) {
            بازگشت پوچ;
        }

    } وقتی (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    بازگشت --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, نویسه *s2, size_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    انجامبده {
        انجامبده {
            c1 = *s1++;

            اگر (c1 == 0) {
                بازگشت پوچ;
            }

        } وقتی (c1 != c2);

    } وقتی (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    بازگشت --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, نویسه *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    انجامبده {
        انجامبده {
            c1 = (ngx_uint_t) *s1++;

            اگر (c1 == 0) {
                بازگشت پوچ;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } وقتی (c1 != c2);

    } وقتی (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    بازگشت --s1;
}


/*
 * ngx_strlcasestrn() is intended to search for static substring
 * with known length in string until the argument last. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strlcasestrn(u_char *s1, u_char *last, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;
    last -= n;

    انجامبده {
        انجامبده {
            اگر (s1 >= last) {
                بازگشت پوچ;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } وقتی (c1 != c2);

    } وقتی (ngx_strncasecmp(s1, s2, n) != 0);

    بازگشت --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, size_t n)
{
    اگر (n == 0) {
        بازگشت 0;
    }

    n--;

    برای ( ;; ) {
        اگر (s1[n] != s2[n]) {
            بازگشت s1[n] - s2[n];
        }

        اگر (n == 0) {
            بازگشت 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n)
{
    u_char  c1, c2;

    اگر (n == 0) {
        بازگشت 0;
    }

    n--;

    برای ( ;; ) {
        c1 = s1[n];
        اگر (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        اگر (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        اگر (c1 != c2) {
            بازگشت c1 - c2;
        }

        اگر (n == 0) {
            بازگشت 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2)
{
    size_t     n;
    ngx_int_t  m, z;

    اگر (n1 <= n2) {
        n = n1;
        z = -1;

    } وگرنه {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    اگر (m || n1 == n2) {
        بازگشت m;
    }

    بازگشت z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    برای ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        اگر (c1 == c2) {

            اگر (c1) {
                ادامه;
            }

            بازگشت 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        بازگشت c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    وقتی (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#اگر (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#endif

        اگر (c1 == c2) {

            اگر (c1) {
                n--;
                ادامه;
            }

            بازگشت 0;
        }

        /* we need '/' to be the lowest character */

        اگر (c1 == 0 || c2 == 0) {
            بازگشت c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        بازگشت c1 - c2;
    }

    بازگشت 0;
}


ngx_int_t
ngx_atoi(u_char *سطر, size_t n)
{
    ngx_int_t  مقدار, cutoff, cutlim;

    اگر (n == 0) {
        بازگشت NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_مقدار / 10;
    cutlim = NGX_MAX_INT_T_مقدار % 10;

    برای (مقدار = 0; n--; سطر++) {
        اگر (*سطر < '0' || *سطر > '9') {
            بازگشت NGX_ERROR;
        }

        اگر (مقدار >= cutoff && (مقدار > cutoff || *سطر - '0' > cutlim)) {
            بازگشت NGX_ERROR;
        }

        مقدار = مقدار * 10 + (*سطر - '0');
    }

    بازگشت مقدار;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *سطر, size_t n, size_t point)
{
    ngx_int_t   مقدار, cutoff, cutlim;
    ngx_uint_t  dot;

    اگر (n == 0) {
        بازگشت NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_مقدار / 10;
    cutlim = NGX_MAX_INT_T_مقدار % 10;

    dot = 0;

    برای (مقدار = 0; n--; سطر++) {

        اگر (point == 0) {
            بازگشت NGX_ERROR;
        }

        اگر (*سطر == '.') {
            اگر (dot) {
                بازگشت NGX_ERROR;
            }

            dot = 1;
            ادامه;
        }

        اگر (*سطر < '0' || *سطر > '9') {
            بازگشت NGX_ERROR;
        }

        اگر (مقدار >= cutoff && (مقدار > cutoff || *سطر - '0' > cutlim)) {
            بازگشت NGX_ERROR;
        }

        مقدار = مقدار * 10 + (*سطر - '0');
        point -= dot;
    }

    وقتی (point--) {
        اگر (مقدار > cutoff) {
            بازگشت NGX_ERROR;
        }

        مقدار = مقدار * 10;
    }

    بازگشت مقدار;
}


ssize_t
ngx_atosz(u_char *سطر, size_t n)
{
    ssize_t  مقدار, cutoff, cutlim;

    اگر (n == 0) {
        بازگشت NGX_ERROR;
    }

    cutoff = NGX_MAX_SIZE_T_مقدار / 10;
    cutlim = NGX_MAX_SIZE_T_مقدار % 10;

    برای (مقدار = 0; n--; سطر++) {
        اگر (*سطر < '0' || *سطر > '9') {
            بازگشت NGX_ERROR;
        }

        اگر (مقدار >= cutoff && (مقدار > cutoff || *سطر - '0' > cutlim)) {
            بازگشت NGX_ERROR;
        }

        مقدار = مقدار * 10 + (*سطر - '0');
    }

    بازگشت مقدار;
}


off_t
ngx_atoof(u_char *سطر, size_t n)
{
    off_t  مقدار, cutoff, cutlim;

    اگر (n == 0) {
        بازگشت NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_مقدار / 10;
    cutlim = NGX_MAX_OFF_T_مقدار % 10;

    برای (مقدار = 0; n--; سطر++) {
        اگر (*سطر < '0' || *سطر > '9') {
            بازگشت NGX_ERROR;
        }

        اگر (مقدار >= cutoff && (مقدار > cutoff || *سطر - '0' > cutlim)) {
            بازگشت NGX_ERROR;
        }

        مقدار = مقدار * 10 + (*سطر - '0');
    }

    بازگشت مقدار;
}


time_t
ngx_atotm(u_char *سطر, size_t n)
{
    time_t  مقدار, cutoff, cutlim;

    اگر (n == 0) {
        بازگشت NGX_ERROR;
    }

    cutoff = NGX_MAX_TIME_T_مقدار / 10;
    cutlim = NGX_MAX_TIME_T_مقدار % 10;

    برای (مقدار = 0; n--; سطر++) {
        اگر (*سطر < '0' || *سطر > '9') {
            بازگشت NGX_ERROR;
        }

        اگر (مقدار >= cutoff && (مقدار > cutoff || *سطر - '0' > cutlim)) {
            بازگشت NGX_ERROR;
        }

        مقدار = مقدار * 10 + (*سطر - '0');
    }

    بازگشت مقدار;
}


ngx_int_t
ngx_hextoi(u_char *سطر, size_t n)
{
    u_char     c, ch;
    ngx_int_t  مقدار, cutoff;

    اگر (n == 0) {
        بازگشت NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_مقدار / 16;

    برای (مقدار = 0; n--; سطر++) {
        اگر (مقدار > cutoff) {
            بازگشت NGX_ERROR;
        }

        ch = *سطر;

        اگر (ch >= '0' && ch <= '9') {
            مقدار = مقدار * 16 + (ch - '0');
            ادامه;
        }

        c = (u_char) (ch | 0x20);

        اگر (c >= 'a' && c <= 'f') {
            مقدار = مقدار * 16 + (c - 'a' + 10);
            ادامه;
        }

        بازگشت NGX_ERROR;
    }

    بازگشت مقدار;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, size_t len)
{
    ایستا u_char  hex[] = "0123456789abcdef";

    وقتی (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    بازگشت dst;
}


تهی
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    ایستا u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_internal(dst, src, basis64, 1);
}


تهی
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    ایستا u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_internal(dst, src, basis64, 0);
}


ایستا تهی
ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src, ثابت u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    size_t          len;

    len = src->len;
    s = src->data;
    d = dst->data;

    وقتی (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    اگر (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        اگر (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            اگر (padding) {
                *d++ = '=';
            }

        } وگرنه {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        اگر (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->data;
}


ngx_int_t
ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    ایستا u_char   basis64[] = {
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

    بازگشت ngx_decode_base64_internal(dst, src, basis64);
}


ngx_int_t
ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    ایستا u_char   basis64[] = {
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

    بازگشت ngx_decode_base64_internal(dst, src, basis64);
}


ایستا ngx_int_t
ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src, ثابت u_char *basis)
{
    size_t          len;
    u_char         *d, *s;

    برای (len = 0; len < src->len; len++) {
        اگر (src->data[len] == '=') {
            بشکن;
        }

        اگر (basis[src->data[len]] == 77) {
            بازگشت NGX_ERROR;
        }
    }

    اگر (len % 4 == 1) {
        بازگشت NGX_ERROR;
    }

    s = src->data;
    d = dst->data;

    وقتی (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    اگر (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    اگر (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->data;

    بازگشت NGX_OK;
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
ngx_utf8_decode(u_char **p, size_t n)
{
    size_t    len;
    uint32_t  u, i, valid;

    u = **p;

    اگر (u >= 0xf8) {

        (*p)++;
        بازگشت 0xffffffff;

    } وگرنه اگر (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } وگرنه اگر (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } وگرنه اگر (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } وگرنه {
        (*p)++;
        بازگشت 0xffffffff;
    }

    اگر (n - 1 < len) {
        بازگشت 0xfffffffe;
    }

    (*p)++;

    وقتی (len) {
        i = *(*p)++;

        اگر (i < 0x80) {
            بازگشت 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    اگر (u > valid) {
        بازگشت u;
    }

    بازگشت 0xffffffff;
}


size_t
ngx_utf8_length(u_char *p, size_t n)
{
    u_char  c, *last;
    size_t  len;

    last = p + n;

    برای (len = 0; p < last; len++) {

        c = *p;

        اگر (c < 0x80) {
            p++;
            ادامه;
        }

        اگر (ngx_utf8_decode(&p, last - p) > 0x10ffff) {
            /* invalid UTF-8 */
            بازگشت n;
        }
    }

    بازگشت len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len)
{
    u_char  c, *next;

    اگر (n == 0) {
        بازگشت dst;
    }

    وقتی (--n) {

        c = *src;
        *dst = c;

        اگر (c < 0x80) {

            اگر (c != '\0') {
                dst++;
                src++;
                len--;

                ادامه;
            }

            بازگشت dst;
        }

        next = src;

        اگر (ngx_utf8_decode(&next, len) > 0x10ffff) {
            /* invalid UTF-8 */
            بشکن;
        }

        وقتی (src < next) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    بازگشت dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, size_t size, ngx_uint_t type)
{
    u_char          prefix;
    uint32_t       *escape;
    ngx_uint_t      n;
    ایستا u_char   hex[] = "0123456789ABCDEF";

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

    ایستا uint32_t   uri[] = {
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

    ایستا uint32_t   args[] = {
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

    ایستا uint32_t   uri_component[] = {
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

    ایستا uint32_t   html[] = {
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

    ایستا uint32_t   refresh[] = {
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

    ایستا uint32_t   memcached[] = {
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

    ایستا uint32_t   mail_xtext[] = {
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

    ایستا uint32_t  *map[] =
        { uri, args, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    ایستا u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    prefix = map_char[type];

    اگر (dst == پوچ) {

        /* find the number of the characters to be escaped */

        n = 0;

        وقتی (size) {
            اگر (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            size--;
        }

        بازگشت (uintptr_t) n;
    }

    وقتی (size) {
        اگر (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = prefix;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } وگرنه {
            *dst++ = *src++;
        }
        size--;
    }

    بازگشت (uintptr_t) dst;
}


تهی
ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    شمارش {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_second
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    وقتی (size--) {

        ch = *s++;

        انتخاب (state) {
        حالت sw_usual:
            اگر (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                برو done;
            }

            اگر (ch == '%') {
                state = sw_quoted;
                بشکن;
            }

            *d++ = ch;
            بشکن;

        حالت sw_quoted:

            اگر (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_second;
                بشکن;
            }

            c = (u_char) (ch | 0x20);
            اگر (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_second;
                بشکن;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            بشکن;

        حالت sw_quoted_second:

            state = sw_usual;

            اگر (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                اگر (type & NGX_UNESCAPE_REDIRECT) {
                    اگر (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        بشکن;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    بشکن;
                }

                *d++ = ch;

                بشکن;
            }

            c = (u_char) (ch | 0x20);
            اگر (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                اگر (type & NGX_UNESCAPE_URI) {
                    اگر (ch == '?') {
                        *d++ = ch;
                        برو done;
                    }

                    *d++ = ch;
                    بشکن;
                }

                اگر (type & NGX_UNESCAPE_REDIRECT) {
                    اگر (ch == '?') {
                        *d++ = ch;
                        برو done;
                    }

                    اگر (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        بشکن;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    بشکن;
                }

                *d++ = ch;

                بشکن;
            }

            /* the invalid quoted character */

            بشکن;
        }
    }

done:

    *dst = d;
    *src = s;
}


uintptr_t
ngx_escape_html(u_char *dst, u_char *src, size_t size)
{
    u_char      ch;
    ngx_uint_t  len;

    اگر (dst == پوچ) {

        len = 0;

        وقتی (size) {
            انتخاب (*src++) {

            حالت '<':
                len += اندازه("&lt;") - 2;
                بشکن;

            حالت '>':
                len += اندازه("&gt;") - 2;
                بشکن;

            حالت '&':
                len += اندازه("&amp;") - 2;
                بشکن;

            حالت '"':
                len += اندازه("&quot;") - 2;
                بشکن;

            پیشفرض:
                بشکن;
            }
            size--;
        }

        بازگشت (uintptr_t) len;
    }

    وقتی (size) {
        ch = *src++;

        انتخاب (ch) {

        حالت '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            بشکن;

        حالت '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            بشکن;

        حالت '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            بشکن;

        حالت '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            بشکن;

        پیشفرض:
            *dst++ = ch;
            بشکن;
        }
        size--;
    }

    بازگشت (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, size_t size)
{
    u_char      ch;
    ngx_uint_t  len;

    اگر (dst == پوچ) {
        len = 0;

        وقتی (size) {
            ch = *src++;

            اگر (ch == '\\' || ch == '"') {
                len++;

            } وگرنه اگر (ch <= 0x1f) {

                انتخاب (ch) {
                حالت '\n':
                حالت '\r':
                حالت '\t':
                حالت '\b':
                حالت '\f':
                    len++;
                    بشکن;

                پیشفرض:
                    len += اندازه("\\u001F") - 2;
                }
            }

            size--;
        }

        بازگشت (uintptr_t) len;
    }

    وقتی (size) {
        ch = *src++;

        اگر (ch > 0x1f) {

            اگر (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } وگرنه {
            *dst++ = '\\';

            انتخاب (ch) {
            حالت '\n':
                *dst++ = 'n';
                بشکن;

            حالت '\r':
                *dst++ = 'r';
                بشکن;

            حالت '\t':
                *dst++ = 't';
                بشکن;

            حالت '\b':
                *dst++ = 'b';
                بشکن;

            حالت '\f':
                *dst++ = 'f';
                بشکن;

            پیشفرض:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        size--;
    }

    بازگشت (uintptr_t) dst;
}


تهی
ngx_str_rbtree_درج_مقدار(ngx_rbtree_گره_t *temp,
    ngx_rbtree_گره_t *گره, ngx_rbtree_گره_t *sentinel)
{
    ngx_str_گره_t      *n, *t;
    ngx_rbtree_گره_t  **p;

    برای ( ;; ) {

        n = (ngx_str_گره_t *) گره;
        t = (ngx_str_گره_t *) temp;

        اگر (گره->کلید != temp->کلید) {

            p = (گره->کلید < temp->کلید) ? &temp->چپ : &temp->راست;

        } وگرنه اگر (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &temp->چپ : &temp->راست;

        } وگرنه {
            p = (ngx_memcmp(n->str.data, t->str.data, n->str.len) < 0)
                 ? &temp->چپ : &temp->راست;
        }

        اگر (*p == sentinel) {
            بشکن;
        }

        temp = *p;
    }

    *p = گره;
    گره->parent = temp;
    گره->چپ = sentinel;
    گره->راست = sentinel;
    ngx_rbt_red(گره);
}


ngx_str_گره_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_گره_t     *n;
    ngx_rbtree_گره_t  *گره, *sentinel;

    گره = rbtree->ریشه;
    sentinel = rbtree->sentinel;

    وقتی (گره != sentinel) {

        n = (ngx_str_گره_t *) گره;

        اگر (hash != گره->کلید) {
            گره = (hash < گره->کلید) ? گره->چپ : گره->راست;
            ادامه;
        }

        اگر (val->len != n->str.len) {
            گره = (val->len < n->str.len) ? گره->چپ : گره->راست;
            ادامه;
        }

        rc = ngx_memcmp(val->data, n->str.data, val->len);

        اگر (rc < 0) {
            گره = گره->چپ;
            ادامه;
        }

        اگر (rc > 0) {
            گره = گره->راست;
            ادامه;
        }

        بازگشت n;
    }

    بازگشت پوچ;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

تهی
ngx_sort(تهی *base, size_t n, size_t size,
    ngx_int_t (*cmp)(ثابت تهی *, ثابت تهی *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(size, ngx_cycle->log);
    اگر (p == پوچ) {
        بازگشت;
    }

    برای (p1 = (u_char *) base + size;
         p1 < (u_char *) base + n * size;
         p1 += size)
    {
        ngx_memcpy(p, p1, size);

        برای (p2 = p1;
             p2 > (u_char *) base && cmp(p2 - size, p) > 0;
             p2 -= size)
        {
            ngx_memcpy(p2, p2 - size, size);
        }

        ngx_memcpy(p2, p, size);
    }

    ngx_free(p);
}


تهی
ngx_explicit_memzero(تهی *buf, size_t n)
{
    ngx_memzero(buf, n);
    ngx_memory_barrier();
}


#اگر (NGX_MEMCPY_LIMIT)

تهی *
ngx_memcpy(تهی *dst, ثابت تهی *src, size_t n)
{
    اگر (n > NGX_MEMCPY_LIMIT) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "memcpy %uz bytes", n);
        ngx_debug_point();
    }

    بازگشت memcpy(dst, src, n);
}

#endif
