
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ u_char *ngx_sprintf_ಅಂಕೆ(u_char *buf, u_char *last, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
ಸ್ಥಿರ u_char *ngx_sprintf_str(u_char *buf, u_char *last, u_char *src,
    size_t len, ngx_uint_t hexadecimal);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ಧ್ರುವ u_char *basis, ngx_uint_t padding);
ಸ್ಥಿರ ngx_int_t ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ಧ್ರುವ u_char *basis);


ಶೂನ್ಯ
ngx_strlow(u_char *dst, u_char *src, size_t n)
{
    ಯಾವತ್ (n) {
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

    ಪ್ರತಿ (i = 0; i < n; i++) {

        ಯದಿ (p[i] == '\0') {
            ಪ್ರತ್ಯಾಗಮ i;
        }
    }

    ಪ್ರತ್ಯಾಗಮ n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, size_t n)
{
    ಯದಿ (n == 0) {
        ಪ್ರತ್ಯಾಗಮ dst;
    }

    ಯಾವತ್ (--n) {
        *dst = *src;

        ಯದಿ (*dst == '\0') {
            ಪ್ರತ್ಯಾಗಮ dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    ಪ್ರತ್ಯಾಗಮ dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    ಯದಿ (dst == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ngx_memcpy(dst, src->data, src->len);

    ಪ್ರತ್ಯಾಗಮ dst;
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
ngx_sprintf(u_char *buf, ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, (ಶೂನ್ಯ *) -1, fmt, args);
    va_end(args);

    ಪ್ರತ್ಯಾಗಮ p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, size_t max, ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, args);
    va_end(args);

    ಪ್ರತ್ಯಾಗಮ p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *last, ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, last, fmt, args);
    va_end(args);

    ಪ್ರತ್ಯಾಗಮ p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *last, ಧ್ರುವ ವರ್ಣ *fmt, va_list args)
{
    u_char                *p, zero;
    ಪೂರ್ಣಾಙ್ಕ                    d;
    ದ್ವಿಗುಣ                 f;
    size_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             width, sign, hex, max_width, frac_width, scale, n;
    ngx_str_t             *v;
    ngx_variable_ಮೌಲ್ಯ_t  *vv;

    ಯಾವತ್ (*fmt && buf < last) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        ಯದಿ (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (size_t) -1;

            ಯಾವತ್ (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }


            ಪ್ರತಿ ( ;; ) {
                ವಿಕಲ್ಪ (*fmt) {

                ಪಕ್ಷ 'u':
                    sign = 0;
                    fmt++;
                    ಅನುವರ್ತ;

                ಪಕ್ಷ 'm':
                    max_width = 1;
                    fmt++;
                    ಅನುವರ್ತ;

                ಪಕ್ಷ 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    ಅನುವರ್ತ;

                ಪಕ್ಷ 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    ಅನುವರ್ತ;

                ಪಕ್ಷ '.':
                    fmt++;

                    ಯಾವತ್ (*fmt >= '0' && *fmt <= '9') {
                        frac_width = frac_width * 10 + (*fmt++ - '0');
                    }

                    ಭಙ್ಗ;

                ಪಕ್ಷ '*':
                    slen = va_arg(args, size_t);
                    fmt++;
                    ಅನುವರ್ತ;

                ಸ್ವಭಾವ:
                    ಭಙ್ಗ;
                }

                ಭಙ್ಗ;
            }


            ವಿಕಲ್ಪ (*fmt) {

            ಪಕ್ಷ 'V':
                v = va_arg(args, ngx_str_t *);

                buf = ngx_sprintf_str(buf, last, v->data, v->len, hex);
                fmt++;

                ಅನುವರ್ತ;

            ಪಕ್ಷ 'v':
                vv = va_arg(args, ngx_variable_ಮೌಲ್ಯ_t *);

                buf = ngx_sprintf_str(buf, last, vv->data, vv->len, hex);
                fmt++;

                ಅನುವರ್ತ;

            ಪಕ್ಷ 's':
                p = va_arg(args, u_char *);

                buf = ngx_sprintf_str(buf, last, p, slen, hex);
                fmt++;

                ಅನುವರ್ತ;

            ಪಕ್ಷ 'O':
                i64 = (int64_t) va_arg(args, off_t);
                sign = 1;
                ಭಙ್ಗ;

            ಪಕ್ಷ 'P':
                i64 = (int64_t) va_arg(args, ngx_pid_t);
                sign = 1;
                ಭಙ್ಗ;

            ಪಕ್ಷ 'T':
                i64 = (int64_t) va_arg(args, time_t);
                sign = 1;
                ಭಙ್ಗ;

            ಪಕ್ಷ 'M':
                ms = (ngx_msec_t) va_arg(args, ngx_msec_t);
                ಯದಿ ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } ಅನ್ಯಥಾ {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                ಭಙ್ಗ;

            ಪಕ್ಷ 'z':
                ಯದಿ (sign) {
                    i64 = (int64_t) va_arg(args, ssize_t);
                } ಅನ್ಯಥಾ {
                    ui64 = (uint64_t) va_arg(args, size_t);
                }
                ಭಙ್ಗ;

            ಪಕ್ಷ 'i':
                ಯದಿ (sign) {
                    i64 = (int64_t) va_arg(args, ngx_int_t);
                } ಅನ್ಯಥಾ {
                    ui64 = (uint64_t) va_arg(args, ngx_uint_t);
                }

                ಯದಿ (max_width) {
                    width = NGX_INT_T_LEN;
                }

                ಭಙ್ಗ;

            ಪಕ್ಷ 'd':
                ಯದಿ (sign) {
                    i64 = (int64_t) va_arg(args, ಪೂರ್ಣಾಙ್ಕ);
                } ಅನ್ಯಥಾ {
                    ui64 = (uint64_t) va_arg(args, u_int);
                }
                ಭಙ್ಗ;

            ಪಕ್ಷ 'l':
                ಯದಿ (sign) {
                    i64 = (int64_t) va_arg(args, long);
                } ಅನ್ಯಥಾ {
                    ui64 = (uint64_t) va_arg(args, u_long);
                }
                ಭಙ್ಗ;

            ಪಕ್ಷ 'D':
                ಯದಿ (sign) {
                    i64 = (int64_t) va_arg(args, int32_t);
                } ಅನ್ಯಥಾ {
                    ui64 = (uint64_t) va_arg(args, uint32_t);
                }
                ಭಙ್ಗ;

            ಪಕ್ಷ 'L':
                ಯದಿ (sign) {
                    i64 = va_arg(args, int64_t);
                } ಅನ್ಯಥಾ {
                    ui64 = va_arg(args, uint64_t);
                }
                ಭಙ್ಗ;

            ಪಕ್ಷ 'A':
                ಯದಿ (sign) {
                    i64 = (int64_t) va_arg(args, ngx_atomic_int_t);
                } ಅನ್ಯಥಾ {
                    ui64 = (uint64_t) va_arg(args, ngx_atomic_uint_t);
                }

                ಯದಿ (max_width) {
                    width = NGX_ATOMIC_T_LEN;
                }

                ಭಙ್ಗ;

            ಪಕ್ಷ 'f':
                f = va_arg(args, ದ್ವಿಗುಣ);

                ಯದಿ (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                ಯದಿ (frac_width) {

                    scale = 1;
                    ಪ್ರತಿ (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (ದ್ವಿಗುಣ) ui64) * scale + 0.5);

                    ಯದಿ (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_ಅಂಕೆ(buf, last, ui64, zero, 0, width);

                ಯದಿ (frac_width) {
                    ಯದಿ (buf < last) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_ಅಂಕೆ(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                ಅನುವರ್ತ;

#ಯದಿ !(NGX_WIN32)
            ಪಕ್ಷ 'r':
                i64 = (int64_t) va_arg(args, rlim_t);
                sign = 1;
                ಭಙ್ಗ;
#endif

            ಪಕ್ಷ 'p':
                ui64 = (uintptr_t) va_arg(args, ಶೂನ್ಯ *);
                hex = 2;
                sign = 0;
                zero = '0';
                width = 2 * ಪರಿಮಾಣ(ಶೂನ್ಯ *);
                ಭಙ್ಗ;

            ಪಕ್ಷ 'c':
                d = va_arg(args, ಪೂರ್ಣಾಙ್ಕ);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                ಅನುವರ್ತ;

            ಪಕ್ಷ 'Z':
                *buf++ = '\0';
                fmt++;

                ಅನುವರ್ತ;

            ಪಕ್ಷ 'N':
#ಯದಿ (NGX_WIN32)
                *buf++ = CR;
                ಯದಿ (buf < last) {
                    *buf++ = LF;
                }
#ಅನ್ಯಥಾ
                *buf++ = LF;
#endif
                fmt++;

                ಅನುವರ್ತ;

            ಪಕ್ಷ '%':
                *buf++ = '%';
                fmt++;

                ಅನುವರ್ತ;

            ಸ್ವಭಾವ:
                *buf++ = *fmt++;

                ಅನುವರ್ತ;
            }

            ಯದಿ (sign) {
                ಯದಿ (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } ಅನ್ಯಥಾ {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_ಅಂಕೆ(buf, last, ui64, zero, hex, width);

            fmt++;

        } ಅನ್ಯಥಾ {
            *buf++ = *fmt++;
        }
    }

    ಪ್ರತ್ಯಾಗಮ buf;
}


ಸ್ಥಿರ u_char *
ngx_sprintf_ಅಂಕೆ(u_char *buf, u_char *last, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t width)
{
    u_char         *p, temp[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    size_t          len;
    uint32_t        ui32;
    ಸ್ಥಿರ u_char   hex[] = "0123456789abcdef";
    ಸ್ಥಿರ u_char   HEX[] = "0123456789ABCDEF";

    p = temp + NGX_INT64_LEN;

    ಯದಿ (hexadecimal == 0) {

        ಯದಿ (ui64 <= (uint64_t) NGX_MAX_UINT32_ಮೌಲ್ಯ) {

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

            ಕುರು {
                *--p = (u_char) (ui32 % 10 + '0');
            } ಯಾವತ್ (ui32 /= 10);

        } ಅನ್ಯಥಾ {
            ಕುರು {
                *--p = (u_char) (ui64 % 10 + '0');
            } ಯಾವತ್ (ui64 /= 10);
        }

    } ಅನ್ಯಥಾ ಯದಿ (hexadecimal == 1) {

        ಕುರು {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } ಯಾವತ್ (ui64 >>= 4);

    } ಅನ್ಯಥಾ { /* hexadecimal == 2 */

        ಕುರು {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } ಯಾವತ್ (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    ಯಾವತ್ (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    ಯದಿ (buf + len > last) {
        len = last - buf;
    }

    ಪ್ರತ್ಯಾಗಮ ngx_cpymem(buf, p, len);
}


ಸ್ಥಿರ u_char *
ngx_sprintf_str(u_char *buf, u_char *last, u_char *src, size_t len,
    ngx_uint_t hexadecimal)
{
    ಸ್ಥಿರ u_char   hex[] = "0123456789abcdef";
    ಸ್ಥಿರ u_char   HEX[] = "0123456789ABCDEF";

    ಯದಿ (hexadecimal == 0) {

        ಯದಿ (len == (size_t) -1) {
            ಯಾವತ್ (*src && buf < last) {
                *buf++ = *src++;
            }

        } ಅನ್ಯಥಾ {
            len = ngx_min((size_t) (last - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } ಅನ್ಯಥಾ ಯದಿ (hexadecimal == 1) {

        ಯದಿ (len == (size_t) -1) {

            ಯಾವತ್ (*src && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } ಅನ್ಯಥಾ {

            ಯಾವತ್ (len-- && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } ಅನ್ಯಥಾ { /* hexadecimal == 2 */

        ಯದಿ (len == (size_t) -1) {

            ಯಾವತ್ (*src && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } ಅನ್ಯಥಾ {

            ಯಾವತ್ (len-- && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    ಪ್ರತ್ಯಾಗಮ buf;
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

    ಪ್ರತಿ ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ಯದಿ (c1 == c2) {

            ಯದಿ (c1) {
                ಅನುವರ್ತ;
            }

            ಪ್ರತ್ಯಾಗಮ 0;
        }

        ಪ್ರತ್ಯಾಗಮ c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    ಯಾವತ್ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ಯದಿ (c1 == c2) {

            ಯದಿ (c1) {
                n--;
                ಅನುವರ್ತ;
            }

            ಪ್ರತ್ಯಾಗಮ 0;
        }

        ಪ್ರತ್ಯಾಗಮ c1 - c2;
    }

    ಪ್ರತ್ಯಾಗಮ 0;
}


u_char *
ngx_strnstr(u_char *s1, ವರ್ಣ *s2, size_t len)
{
    u_char  c1, c2;
    size_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    ಕುರು {
        ಕುರು {
            ಯದಿ (len-- == 0) {
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }

            c1 = *s1++;

            ಯದಿ (c1 == 0) {
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }

        } ಯಾವತ್ (c1 != c2);

        ಯದಿ (n > len) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

    } ಯಾವತ್ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ಪ್ರತ್ಯಾಗಮ --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, ವರ್ಣ *s2, size_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    ಕುರು {
        ಕುರು {
            c1 = *s1++;

            ಯದಿ (c1 == 0) {
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }

        } ಯಾವತ್ (c1 != c2);

    } ಯಾವತ್ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ಪ್ರತ್ಯಾಗಮ --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, ವರ್ಣ *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    ಕುರು {
        ಕುರು {
            c1 = (ngx_uint_t) *s1++;

            ಯದಿ (c1 == 0) {
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ಯಾವತ್ (c1 != c2);

    } ಯಾವತ್ (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    ಪ್ರತ್ಯಾಗಮ --s1;
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

    ಕುರು {
        ಕುರು {
            ಯದಿ (s1 >= last) {
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ಯಾವತ್ (c1 != c2);

    } ಯಾವತ್ (ngx_strncasecmp(s1, s2, n) != 0);

    ಪ್ರತ್ಯಾಗಮ --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, size_t n)
{
    ಯದಿ (n == 0) {
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    n--;

    ಪ್ರತಿ ( ;; ) {
        ಯದಿ (s1[n] != s2[n]) {
            ಪ್ರತ್ಯಾಗಮ s1[n] - s2[n];
        }

        ಯದಿ (n == 0) {
            ಪ್ರತ್ಯಾಗಮ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n)
{
    u_char  c1, c2;

    ಯದಿ (n == 0) {
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    n--;

    ಪ್ರತಿ ( ;; ) {
        c1 = s1[n];
        ಯದಿ (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        ಯದಿ (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        ಯದಿ (c1 != c2) {
            ಪ್ರತ್ಯಾಗಮ c1 - c2;
        }

        ಯದಿ (n == 0) {
            ಪ್ರತ್ಯಾಗಮ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2)
{
    size_t     n;
    ngx_int_t  m, z;

    ಯದಿ (n1 <= n2) {
        n = n1;
        z = -1;

    } ಅನ್ಯಥಾ {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    ಯದಿ (m || n1 == n2) {
        ಪ್ರತ್ಯಾಗಮ m;
    }

    ಪ್ರತ್ಯಾಗಮ z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    ಪ್ರತಿ ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ಯದಿ (c1 == c2) {

            ಯದಿ (c1) {
                ಅನುವರ್ತ;
            }

            ಪ್ರತ್ಯಾಗಮ 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        ಪ್ರತ್ಯಾಗಮ c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    ಯಾವತ್ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#ಯದಿ (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#endif

        ಯದಿ (c1 == c2) {

            ಯದಿ (c1) {
                n--;
                ಅನುವರ್ತ;
            }

            ಪ್ರತ್ಯಾಗಮ 0;
        }

        /* we need '/' to be the lowest character */

        ಯದಿ (c1 == 0 || c2 == 0) {
            ಪ್ರತ್ಯಾಗಮ c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        ಪ್ರತ್ಯಾಗಮ c1 - c2;
    }

    ಪ್ರತ್ಯಾಗಮ 0;
}


ngx_int_t
ngx_atoi(u_char *ಸಾಲು, size_t n)
{
    ngx_int_t  ಮೌಲ್ಯ, cutoff, cutlim;

    ಯದಿ (n == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_ಮೌಲ್ಯ / 10;
    cutlim = NGX_MAX_INT_T_ಮೌಲ್ಯ % 10;

    ಪ್ರತಿ (ಮೌಲ್ಯ = 0; n--; ಸಾಲು++) {
        ಯದಿ (*ಸಾಲು < '0' || *ಸಾಲು > '9') {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (ಮೌಲ್ಯ >= cutoff && (ಮೌಲ್ಯ > cutoff || *ಸಾಲು - '0' > cutlim)) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10 + (*ಸಾಲು - '0');
    }

    ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *ಸಾಲು, size_t n, size_t point)
{
    ngx_int_t   ಮೌಲ್ಯ, cutoff, cutlim;
    ngx_uint_t  dot;

    ಯದಿ (n == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_ಮೌಲ್ಯ / 10;
    cutlim = NGX_MAX_INT_T_ಮೌಲ್ಯ % 10;

    dot = 0;

    ಪ್ರತಿ (ಮೌಲ್ಯ = 0; n--; ಸಾಲು++) {

        ಯದಿ (point == 0) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (*ಸಾಲು == '.') {
            ಯದಿ (dot) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            dot = 1;
            ಅನುವರ್ತ;
        }

        ಯದಿ (*ಸಾಲು < '0' || *ಸಾಲು > '9') {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (ಮೌಲ್ಯ >= cutoff && (ಮೌಲ್ಯ > cutoff || *ಸಾಲು - '0' > cutlim)) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10 + (*ಸಾಲು - '0');
        point -= dot;
    }

    ಯಾವತ್ (point--) {
        ಯದಿ (ಮೌಲ್ಯ > cutoff) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10;
    }

    ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
}


ssize_t
ngx_atosz(u_char *ಸಾಲು, size_t n)
{
    ssize_t  ಮೌಲ್ಯ, cutoff, cutlim;

    ಯದಿ (n == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    cutoff = NGX_MAX_SIZE_T_ಮೌಲ್ಯ / 10;
    cutlim = NGX_MAX_SIZE_T_ಮೌಲ್ಯ % 10;

    ಪ್ರತಿ (ಮೌಲ್ಯ = 0; n--; ಸಾಲು++) {
        ಯದಿ (*ಸಾಲು < '0' || *ಸಾಲು > '9') {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (ಮೌಲ್ಯ >= cutoff && (ಮೌಲ್ಯ > cutoff || *ಸಾಲು - '0' > cutlim)) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10 + (*ಸಾಲು - '0');
    }

    ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
}


off_t
ngx_atoof(u_char *ಸಾಲು, size_t n)
{
    off_t  ಮೌಲ್ಯ, cutoff, cutlim;

    ಯದಿ (n == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_ಮೌಲ್ಯ / 10;
    cutlim = NGX_MAX_OFF_T_ಮೌಲ್ಯ % 10;

    ಪ್ರತಿ (ಮೌಲ್ಯ = 0; n--; ಸಾಲು++) {
        ಯದಿ (*ಸಾಲು < '0' || *ಸಾಲು > '9') {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (ಮೌಲ್ಯ >= cutoff && (ಮೌಲ್ಯ > cutoff || *ಸಾಲು - '0' > cutlim)) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10 + (*ಸಾಲು - '0');
    }

    ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
}


time_t
ngx_atotm(u_char *ಸಾಲು, size_t n)
{
    time_t  ಮೌಲ್ಯ, cutoff, cutlim;

    ಯದಿ (n == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    cutoff = NGX_MAX_TIME_T_ಮೌಲ್ಯ / 10;
    cutlim = NGX_MAX_TIME_T_ಮೌಲ್ಯ % 10;

    ಪ್ರತಿ (ಮೌಲ್ಯ = 0; n--; ಸಾಲು++) {
        ಯದಿ (*ಸಾಲು < '0' || *ಸಾಲು > '9') {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (ಮೌಲ್ಯ >= cutoff && (ಮೌಲ್ಯ > cutoff || *ಸಾಲು - '0' > cutlim)) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10 + (*ಸಾಲು - '0');
    }

    ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
}


ngx_int_t
ngx_hextoi(u_char *ಸಾಲು, size_t n)
{
    u_char     c, ch;
    ngx_int_t  ಮೌಲ್ಯ, cutoff;

    ಯದಿ (n == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_ಮೌಲ್ಯ / 16;

    ಪ್ರತಿ (ಮೌಲ್ಯ = 0; n--; ಸಾಲು++) {
        ಯದಿ (ಮೌಲ್ಯ > cutoff) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ch = *ಸಾಲು;

        ಯದಿ (ch >= '0' && ch <= '9') {
            ಮೌಲ್ಯ = ಮೌಲ್ಯ * 16 + (ch - '0');
            ಅನುವರ್ತ;
        }

        c = (u_char) (ch | 0x20);

        ಯದಿ (c >= 'a' && c <= 'f') {
            ಮೌಲ್ಯ = ಮೌಲ್ಯ * 16 + (c - 'a' + 10);
            ಅನುವರ್ತ;
        }

        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, size_t len)
{
    ಸ್ಥಿರ u_char  hex[] = "0123456789abcdef";

    ಯಾವತ್ (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    ಪ್ರತ್ಯಾಗಮ dst;
}


ಶೂನ್ಯ
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    ಸ್ಥಿರ u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_internal(dst, src, basis64, 1);
}


ಶೂನ್ಯ
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    ಸ್ಥಿರ u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_internal(dst, src, basis64, 0);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src, ಧ್ರುವ u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    size_t          len;

    len = src->len;
    s = src->data;
    d = dst->data;

    ಯಾವತ್ (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    ಯದಿ (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        ಯದಿ (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            ಯದಿ (padding) {
                *d++ = '=';
            }

        } ಅನ್ಯಥಾ {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        ಯದಿ (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->data;
}


ngx_int_t
ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    ಸ್ಥಿರ u_char   basis64[] = {
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

    ಪ್ರತ್ಯಾಗಮ ngx_decode_base64_internal(dst, src, basis64);
}


ngx_int_t
ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    ಸ್ಥಿರ u_char   basis64[] = {
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

    ಪ್ರತ್ಯಾಗಮ ngx_decode_base64_internal(dst, src, basis64);
}


ಸ್ಥಿರ ngx_int_t
ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src, ಧ್ರುವ u_char *basis)
{
    size_t          len;
    u_char         *d, *s;

    ಪ್ರತಿ (len = 0; len < src->len; len++) {
        ಯದಿ (src->data[len] == '=') {
            ಭಙ್ಗ;
        }

        ಯದಿ (basis[src->data[len]] == 77) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    ಯದಿ (len % 4 == 1) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    s = src->data;
    d = dst->data;

    ಯಾವತ್ (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    ಯದಿ (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    ಯದಿ (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->data;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
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

    ಯದಿ (u >= 0xf8) {

        (*p)++;
        ಪ್ರತ್ಯಾಗಮ 0xffffffff;

    } ಅನ್ಯಥಾ ಯದಿ (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } ಅನ್ಯಥಾ ಯದಿ (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } ಅನ್ಯಥಾ ಯದಿ (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } ಅನ್ಯಥಾ {
        (*p)++;
        ಪ್ರತ್ಯಾಗಮ 0xffffffff;
    }

    ಯದಿ (n - 1 < len) {
        ಪ್ರತ್ಯಾಗಮ 0xfffffffe;
    }

    (*p)++;

    ಯಾವತ್ (len) {
        i = *(*p)++;

        ಯದಿ (i < 0x80) {
            ಪ್ರತ್ಯಾಗಮ 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    ಯದಿ (u > valid) {
        ಪ್ರತ್ಯಾಗಮ u;
    }

    ಪ್ರತ್ಯಾಗಮ 0xffffffff;
}


size_t
ngx_utf8_length(u_char *p, size_t n)
{
    u_char  c, *last;
    size_t  len;

    last = p + n;

    ಪ್ರತಿ (len = 0; p < last; len++) {

        c = *p;

        ಯದಿ (c < 0x80) {
            p++;
            ಅನುವರ್ತ;
        }

        ಯದಿ (ngx_utf8_decode(&p, last - p) > 0x10ffff) {
            /* invalid UTF-8 */
            ಪ್ರತ್ಯಾಗಮ n;
        }
    }

    ಪ್ರತ್ಯಾಗಮ len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len)
{
    u_char  c, *next;

    ಯದಿ (n == 0) {
        ಪ್ರತ್ಯಾಗಮ dst;
    }

    ಯಾವತ್ (--n) {

        c = *src;
        *dst = c;

        ಯದಿ (c < 0x80) {

            ಯದಿ (c != '\0') {
                dst++;
                src++;
                len--;

                ಅನುವರ್ತ;
            }

            ಪ್ರತ್ಯಾಗಮ dst;
        }

        next = src;

        ಯದಿ (ngx_utf8_decode(&next, len) > 0x10ffff) {
            /* invalid UTF-8 */
            ಭಙ್ಗ;
        }

        ಯಾವತ್ (src < next) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    ಪ್ರತ್ಯಾಗಮ dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, size_t size, ngx_uint_t type)
{
    u_char          prefix;
    uint32_t       *escape;
    ngx_uint_t      n;
    ಸ್ಥಿರ u_char   hex[] = "0123456789ABCDEF";

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

    ಸ್ಥಿರ uint32_t   uri[] = {
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

    ಸ್ಥಿರ uint32_t   args[] = {
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

    ಸ್ಥಿರ uint32_t   uri_component[] = {
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

    ಸ್ಥಿರ uint32_t   html[] = {
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

    ಸ್ಥಿರ uint32_t   refresh[] = {
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

    ಸ್ಥಿರ uint32_t   memcached[] = {
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

    ಸ್ಥಿರ uint32_t   mail_xtext[] = {
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

    ಸ್ಥಿರ uint32_t  *map[] =
        { uri, args, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    ಸ್ಥಿರ u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    prefix = map_char[type];

    ಯದಿ (dst == ರಿಕ್ತ) {

        /* find the number of the characters to be escaped */

        n = 0;

        ಯಾವತ್ (size) {
            ಯದಿ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            size--;
        }

        ಪ್ರತ್ಯಾಗಮ (uintptr_t) n;
    }

    ಯಾವತ್ (size) {
        ಯದಿ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = prefix;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } ಅನ್ಯಥಾ {
            *dst++ = *src++;
        }
        size--;
    }

    ಪ್ರತ್ಯಾಗಮ (uintptr_t) dst;
}


ಶೂನ್ಯ
ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    ಗಣನಾ {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_second
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    ಯಾವತ್ (size--) {

        ch = *s++;

        ವಿಕಲ್ಪ (state) {
        ಪಕ್ಷ sw_usual:
            ಯದಿ (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                ಗಚ್ಛ done;
            }

            ಯದಿ (ch == '%') {
                state = sw_quoted;
                ಭಙ್ಗ;
            }

            *d++ = ch;
            ಭಙ್ಗ;

        ಪಕ್ಷ sw_quoted:

            ಯದಿ (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_second;
                ಭಙ್ಗ;
            }

            c = (u_char) (ch | 0x20);
            ಯದಿ (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_second;
                ಭಙ್ಗ;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            ಭಙ್ಗ;

        ಪಕ್ಷ sw_quoted_second:

            state = sw_usual;

            ಯದಿ (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                ಯದಿ (type & NGX_UNESCAPE_REDIRECT) {
                    ಯದಿ (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        ಭಙ್ಗ;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    ಭಙ್ಗ;
                }

                *d++ = ch;

                ಭಙ್ಗ;
            }

            c = (u_char) (ch | 0x20);
            ಯದಿ (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                ಯದಿ (type & NGX_UNESCAPE_URI) {
                    ಯದಿ (ch == '?') {
                        *d++ = ch;
                        ಗಚ್ಛ done;
                    }

                    *d++ = ch;
                    ಭಙ್ಗ;
                }

                ಯದಿ (type & NGX_UNESCAPE_REDIRECT) {
                    ಯದಿ (ch == '?') {
                        *d++ = ch;
                        ಗಚ್ಛ done;
                    }

                    ಯದಿ (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        ಭಙ್ಗ;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    ಭಙ್ಗ;
                }

                *d++ = ch;

                ಭಙ್ಗ;
            }

            /* the invalid quoted character */

            ಭಙ್ಗ;
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

    ಯದಿ (dst == ರಿಕ್ತ) {

        len = 0;

        ಯಾವತ್ (size) {
            ವಿಕಲ್ಪ (*src++) {

            ಪಕ್ಷ '<':
                len += ಪರಿಮಾಣ("&lt;") - 2;
                ಭಙ್ಗ;

            ಪಕ್ಷ '>':
                len += ಪರಿಮಾಣ("&gt;") - 2;
                ಭಙ್ಗ;

            ಪಕ್ಷ '&':
                len += ಪರಿಮಾಣ("&amp;") - 2;
                ಭಙ್ಗ;

            ಪಕ್ಷ '"':
                len += ಪರಿಮಾಣ("&quot;") - 2;
                ಭಙ್ಗ;

            ಸ್ವಭಾವ:
                ಭಙ್ಗ;
            }
            size--;
        }

        ಪ್ರತ್ಯಾಗಮ (uintptr_t) len;
    }

    ಯಾವತ್ (size) {
        ch = *src++;

        ವಿಕಲ್ಪ (ch) {

        ಪಕ್ಷ '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            ಭಙ್ಗ;

        ಪಕ್ಷ '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            ಭಙ್ಗ;

        ಪಕ್ಷ '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            ಭಙ್ಗ;

        ಪಕ್ಷ '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            ಭಙ್ಗ;

        ಸ್ವಭಾವ:
            *dst++ = ch;
            ಭಙ್ಗ;
        }
        size--;
    }

    ಪ್ರತ್ಯಾಗಮ (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, size_t size)
{
    u_char      ch;
    ngx_uint_t  len;

    ಯದಿ (dst == ರಿಕ್ತ) {
        len = 0;

        ಯಾವತ್ (size) {
            ch = *src++;

            ಯದಿ (ch == '\\' || ch == '"') {
                len++;

            } ಅನ್ಯಥಾ ಯದಿ (ch <= 0x1f) {

                ವಿಕಲ್ಪ (ch) {
                ಪಕ್ಷ '\n':
                ಪಕ್ಷ '\r':
                ಪಕ್ಷ '\t':
                ಪಕ್ಷ '\b':
                ಪಕ್ಷ '\f':
                    len++;
                    ಭಙ್ಗ;

                ಸ್ವಭಾವ:
                    len += ಪರಿಮಾಣ("\\u001F") - 2;
                }
            }

            size--;
        }

        ಪ್ರತ್ಯಾಗಮ (uintptr_t) len;
    }

    ಯಾವತ್ (size) {
        ch = *src++;

        ಯದಿ (ch > 0x1f) {

            ಯದಿ (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } ಅನ್ಯಥಾ {
            *dst++ = '\\';

            ವಿಕಲ್ಪ (ch) {
            ಪಕ್ಷ '\n':
                *dst++ = 'n';
                ಭಙ್ಗ;

            ಪಕ್ಷ '\r':
                *dst++ = 'r';
                ಭಙ್ಗ;

            ಪಕ್ಷ '\t':
                *dst++ = 't';
                ಭಙ್ಗ;

            ಪಕ್ಷ '\b':
                *dst++ = 'b';
                ಭಙ್ಗ;

            ಪಕ್ಷ '\f':
                *dst++ = 'f';
                ಭಙ್ಗ;

            ಸ್ವಭಾವ:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        size--;
    }

    ಪ್ರತ್ಯಾಗಮ (uintptr_t) dst;
}


ಶೂನ್ಯ
ngx_str_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *temp,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel)
{
    ngx_str_ಗಂಟು_t      *n, *t;
    ngx_rbtree_ಗಂಟು_t  **p;

    ಪ್ರತಿ ( ;; ) {

        n = (ngx_str_ಗಂಟು_t *) ಗಂಟು;
        t = (ngx_str_ಗಂಟು_t *) temp;

        ಯದಿ (ಗಂಟು->ಕೀಲಿ != temp->ಕೀಲಿ) {

            p = (ಗಂಟು->ಕೀಲಿ < temp->ಕೀಲಿ) ? &temp->ಎಡ : &temp->ಬಲ;

        } ಅನ್ಯಥಾ ಯದಿ (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &temp->ಎಡ : &temp->ಬಲ;

        } ಅನ್ಯಥಾ {
            p = (ngx_memcmp(n->str.data, t->str.data, n->str.len) < 0)
                 ? &temp->ಎಡ : &temp->ಬಲ;
        }

        ಯದಿ (*p == sentinel) {
            ಭಙ್ಗ;
        }

        temp = *p;
    }

    *p = ಗಂಟು;
    ಗಂಟು->parent = temp;
    ಗಂಟು->ಎಡ = sentinel;
    ಗಂಟು->ಬಲ = sentinel;
    ngx_rbt_red(ಗಂಟು);
}


ngx_str_ಗಂಟು_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_ಗಂಟು_t     *n;
    ngx_rbtree_ಗಂಟು_t  *ಗಂಟು, *sentinel;

    ಗಂಟು = rbtree->ಬೇರು;
    sentinel = rbtree->sentinel;

    ಯಾವತ್ (ಗಂಟು != sentinel) {

        n = (ngx_str_ಗಂಟು_t *) ಗಂಟು;

        ಯದಿ (hash != ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = (hash < ಗಂಟು->ಕೀಲಿ) ? ಗಂಟು->ಎಡ : ಗಂಟು->ಬಲ;
            ಅನುವರ್ತ;
        }

        ಯದಿ (val->len != n->str.len) {
            ಗಂಟು = (val->len < n->str.len) ? ಗಂಟು->ಎಡ : ಗಂಟು->ಬಲ;
            ಅನುವರ್ತ;
        }

        rc = ngx_memcmp(val->data, n->str.data, val->len);

        ಯದಿ (rc < 0) {
            ಗಂಟು = ಗಂಟು->ಎಡ;
            ಅನುವರ್ತ;
        }

        ಯದಿ (rc > 0) {
            ಗಂಟು = ಗಂಟು->ಬಲ;
            ಅನುವರ್ತ;
        }

        ಪ್ರತ್ಯಾಗಮ n;
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

ಶೂನ್ಯ
ngx_sort(ಶೂನ್ಯ *base, size_t n, size_t size,
    ngx_int_t (*cmp)(ಧ್ರುವ ಶೂನ್ಯ *, ಧ್ರುವ ಶೂನ್ಯ *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(size, ngx_cycle->log);
    ಯದಿ (p == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಪ್ರತಿ (p1 = (u_char *) base + size;
         p1 < (u_char *) base + n * size;
         p1 += size)
    {
        ngx_memcpy(p, p1, size);

        ಪ್ರತಿ (p2 = p1;
             p2 > (u_char *) base && cmp(p2 - size, p) > 0;
             p2 -= size)
        {
            ngx_memcpy(p2, p2 - size, size);
        }

        ngx_memcpy(p2, p, size);
    }

    ngx_free(p);
}


ಶೂನ್ಯ
ngx_explicit_memzero(ಶೂನ್ಯ *buf, size_t n)
{
    ngx_memzero(buf, n);
    ngx_memory_barrier();
}


#ಯದಿ (NGX_MEMCPY_LIMIT)

ಶೂನ್ಯ *
ngx_memcpy(ಶೂನ್ಯ *dst, ಧ್ರುವ ಶೂನ್ಯ *src, size_t n)
{
    ಯದಿ (n > NGX_MEMCPY_LIMIT) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "memcpy %uz bytes", n);
        ngx_debug_point();
    }

    ಪ್ರತ್ಯಾಗಮ memcpy(dst, src, n);
}

#endif
