
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


static u_char *ngx_sprintf_num(u_char *buf, u_char *last, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
static u_char *ngx_sprintf_str(u_char *buf, u_char *last, u_char *src,
    size_t len, ngx_uint_t hexadecimal);
static void ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    const u_char *basis, ngx_uint_t padding);
static ngx_int_t ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    const u_char *basis);


void
ngx_strlow(u_char *dst, u_char *src, size_t n)
{
    ꦱꦼꦢꦼꦭ (n) {
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

    ꦏꦁꦒꦺꦴ (i = 0; i < n; i++) {

        ꦪꦼꦤ꧀ (p[i] == '\0') {
            ꦮꦭꦶ i;
        }
    }

    ꦮꦭꦶ n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, size_t n)
{
    ꦪꦼꦤ꧀ (n == 0) {
        ꦮꦭꦶ dst;
    }

    ꦱꦼꦢꦼꦭ (--n) {
        *dst = *src;

        ꦪꦼꦤ꧀ (*dst == '\0') {
            ꦮꦭꦶ dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    ꦮꦭꦶ dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    ꦪꦼꦤ꧀ (dst == NULL) {
        ꦮꦭꦶ NULL;
    }

    ngx_memcpy(dst, src->data, src->len);

    ꦮꦭꦶ dst;
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
ngx_sprintf(u_char *buf, const var_na *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, (void *) -1, fmt, args);
    va_end(args);

    ꦮꦭꦶ p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, size_t max, const var_na *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, args);
    va_end(args);

    ꦮꦭꦶ p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *last, const var_na *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, last, fmt, args);
    va_end(args);

    ꦮꦭꦶ p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *last, const var_na *fmt, va_list args)
{
    u_char                *p, zero;
    puur_naa_nka                    d;
    dviguu_na                 f;
    size_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             width, sign, hex, max_width, frac_width, scale, n;
    ngx_str_t             *v;
    ngx_variable_value_t  *vv;

    ꦱꦼꦢꦼꦭ (*fmt && buf < last) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        ꦪꦼꦤ꧀ (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (size_t) -1;

            ꦱꦼꦢꦼꦭ (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }


            ꦏꦁꦒꦺꦴ ( ;; ) {
                switch (*fmt) {

                pak_sa 'u':
                    sign = 0;
                    fmt++;
                    continue;

                pak_sa 'm':
                    max_width = 1;
                    fmt++;
                    continue;

                pak_sa 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    continue;

                pak_sa 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    continue;

                pak_sa '.':
                    fmt++;

                    ꦱꦼꦢꦼꦭ (*fmt >= '0' && *fmt <= '9') {
                        frac_width = frac_width * 10 + (*fmt++ - '0');
                    }

                    bha_nga;

                pak_sa '*':
                    slen = va_arg(args, size_t);
                    fmt++;
                    continue;

                default:
                    bha_nga;
                }

                bha_nga;
            }


            switch (*fmt) {

            pak_sa 'V':
                v = va_arg(args, ngx_str_t *);

                buf = ngx_sprintf_str(buf, last, v->data, v->len, hex);
                fmt++;

                continue;

            pak_sa 'v':
                vv = va_arg(args, ngx_variable_value_t *);

                buf = ngx_sprintf_str(buf, last, vv->data, vv->len, hex);
                fmt++;

                continue;

            pak_sa 's':
                p = va_arg(args, u_char *);

                buf = ngx_sprintf_str(buf, last, p, slen, hex);
                fmt++;

                continue;

            pak_sa 'O':
                i64 = (int64_t) va_arg(args, off_t);
                sign = 1;
                bha_nga;

            pak_sa 'P':
                i64 = (int64_t) va_arg(args, ngx_pid_t);
                sign = 1;
                bha_nga;

            pak_sa 'T':
                i64 = (int64_t) va_arg(args, time_t);
                sign = 1;
                bha_nga;

            pak_sa 'M':
                ms = (ngx_msec_t) va_arg(args, ngx_msec_t);
                ꦪꦼꦤ꧀ ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } ꦭꦶꦪ {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                bha_nga;

            pak_sa 'z':
                ꦪꦼꦤ꧀ (sign) {
                    i64 = (int64_t) va_arg(args, ssize_t);
                } ꦭꦶꦪ {
                    ui64 = (uint64_t) va_arg(args, size_t);
                }
                bha_nga;

            pak_sa 'i':
                ꦪꦼꦤ꧀ (sign) {
                    i64 = (int64_t) va_arg(args, ngx_int_t);
                } ꦭꦶꦪ {
                    ui64 = (uint64_t) va_arg(args, ngx_uint_t);
                }

                ꦪꦼꦤ꧀ (max_width) {
                    width = NGX_INT_T_LEN;
                }

                bha_nga;

            pak_sa 'd':
                ꦪꦼꦤ꧀ (sign) {
                    i64 = (int64_t) va_arg(args, puur_naa_nka);
                } ꦭꦶꦪ {
                    ui64 = (uint64_t) va_arg(args, u_int);
                }
                bha_nga;

            pak_sa 'l':
                ꦪꦼꦤ꧀ (sign) {
                    i64 = (int64_t) va_arg(args, long);
                } ꦭꦶꦪ {
                    ui64 = (uint64_t) va_arg(args, u_long);
                }
                bha_nga;

            pak_sa 'D':
                ꦪꦼꦤ꧀ (sign) {
                    i64 = (int64_t) va_arg(args, int32_t);
                } ꦭꦶꦪ {
                    ui64 = (uint64_t) va_arg(args, uint32_t);
                }
                bha_nga;

            pak_sa 'L':
                ꦪꦼꦤ꧀ (sign) {
                    i64 = va_arg(args, int64_t);
                } ꦭꦶꦪ {
                    ui64 = va_arg(args, uint64_t);
                }
                bha_nga;

            pak_sa 'A':
                ꦪꦼꦤ꧀ (sign) {
                    i64 = (int64_t) va_arg(args, ngx_atomic_int_t);
                } ꦭꦶꦪ {
                    ui64 = (uint64_t) va_arg(args, ngx_atomic_uint_t);
                }

                ꦪꦼꦤ꧀ (max_width) {
                    width = NGX_ATOMIC_T_LEN;
                }

                bha_nga;

            pak_sa 'f':
                f = va_arg(args, dviguu_na);

                ꦪꦼꦤ꧀ (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                ꦪꦼꦤ꧀ (frac_width) {

                    scale = 1;
                    ꦏꦁꦒꦺꦴ (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (dviguu_na) ui64) * scale + 0.5);

                    ꦪꦼꦤ꧀ (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_num(buf, last, ui64, zero, 0, width);

                ꦪꦼꦤ꧀ (frac_width) {
                    ꦪꦼꦤ꧀ (buf < last) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_num(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                continue;

#ꦪꦼꦤ꧀ !(NGX_WIN32)
            pak_sa 'r':
                i64 = (int64_t) va_arg(args, rlim_t);
                sign = 1;
                bha_nga;
#endif

            pak_sa 'p':
                ui64 = (uintptr_t) va_arg(args, void *);
                hex = 2;
                sign = 0;
                zero = '0';
                width = 2 * parimaa_na(void *);
                bha_nga;

            pak_sa 'c':
                d = va_arg(args, puur_naa_nka);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                continue;

            pak_sa 'Z':
                *buf++ = '\0';
                fmt++;

                continue;

            pak_sa 'N':
#ꦪꦼꦤ꧀ (NGX_WIN32)
                *buf++ = CR;
                ꦪꦼꦤ꧀ (buf < last) {
                    *buf++ = LF;
                }
#ꦭꦶꦪ
                *buf++ = LF;
#endif
                fmt++;

                continue;

            pak_sa '%':
                *buf++ = '%';
                fmt++;

                continue;

            default:
                *buf++ = *fmt++;

                continue;
            }

            ꦪꦼꦤ꧀ (sign) {
                ꦪꦼꦤ꧀ (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } ꦭꦶꦪ {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_num(buf, last, ui64, zero, hex, width);

            fmt++;

        } ꦭꦶꦪ {
            *buf++ = *fmt++;
        }
    }

    ꦮꦭꦶ buf;
}


static u_char *
ngx_sprintf_num(u_char *buf, u_char *last, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t width)
{
    u_char         *p, temp[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    size_t          len;
    uint32_t        ui32;
    static u_char   hex[] = "0123456789abcdef";
    static u_char   HEX[] = "0123456789ABCDEF";

    p = temp + NGX_INT64_LEN;

    ꦪꦼꦤ꧀ (hexadecimal == 0) {

        ꦪꦼꦤ꧀ (ui64 <= (uint64_t) NGX_MAX_UINT32_VALUE) {

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

            do {
                *--p = (u_char) (ui32 % 10 + '0');
            } ꦱꦼꦢꦼꦭ (ui32 /= 10);

        } ꦭꦶꦪ {
            do {
                *--p = (u_char) (ui64 % 10 + '0');
            } ꦱꦼꦢꦼꦭ (ui64 /= 10);
        }

    } ꦭꦶꦪ ꦪꦼꦤ꧀ (hexadecimal == 1) {

        do {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } ꦱꦼꦢꦼꦭ (ui64 >>= 4);

    } ꦭꦶꦪ { /* hexadecimal == 2 */

        do {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } ꦱꦼꦢꦼꦭ (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    ꦱꦼꦢꦼꦭ (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    ꦪꦼꦤ꧀ (buf + len > last) {
        len = last - buf;
    }

    ꦮꦭꦶ ngx_cpymem(buf, p, len);
}


static u_char *
ngx_sprintf_str(u_char *buf, u_char *last, u_char *src, size_t len,
    ngx_uint_t hexadecimal)
{
    static u_char   hex[] = "0123456789abcdef";
    static u_char   HEX[] = "0123456789ABCDEF";

    ꦪꦼꦤ꧀ (hexadecimal == 0) {

        ꦪꦼꦤ꧀ (len == (size_t) -1) {
            ꦱꦼꦢꦼꦭ (*src && buf < last) {
                *buf++ = *src++;
            }

        } ꦭꦶꦪ {
            len = ngx_min((size_t) (last - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } ꦭꦶꦪ ꦪꦼꦤ꧀ (hexadecimal == 1) {

        ꦪꦼꦤ꧀ (len == (size_t) -1) {

            ꦱꦼꦢꦼꦭ (*src && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } ꦭꦶꦪ {

            ꦱꦼꦢꦼꦭ (len-- && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } ꦭꦶꦪ { /* hexadecimal == 2 */

        ꦪꦼꦤ꧀ (len == (size_t) -1) {

            ꦱꦼꦢꦼꦭ (*src && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } ꦭꦶꦪ {

            ꦱꦼꦢꦼꦭ (len-- && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    ꦮꦭꦶ buf;
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

    ꦏꦁꦒꦺꦴ ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ꦪꦼꦤ꧀ (c1 == c2) {

            ꦪꦼꦤ꧀ (c1) {
                continue;
            }

            ꦮꦭꦶ 0;
        }

        ꦮꦭꦶ c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    ꦱꦼꦢꦼꦭ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ꦪꦼꦤ꧀ (c1 == c2) {

            ꦪꦼꦤ꧀ (c1) {
                n--;
                continue;
            }

            ꦮꦭꦶ 0;
        }

        ꦮꦭꦶ c1 - c2;
    }

    ꦮꦭꦶ 0;
}


u_char *
ngx_strnstr(u_char *s1, var_na *s2, size_t len)
{
    u_char  c1, c2;
    size_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    do {
        do {
            ꦪꦼꦤ꧀ (len-- == 0) {
                ꦮꦭꦶ NULL;
            }

            c1 = *s1++;

            ꦪꦼꦤ꧀ (c1 == 0) {
                ꦮꦭꦶ NULL;
            }

        } ꦱꦼꦢꦼꦭ (c1 != c2);

        ꦪꦼꦤ꧀ (n > len) {
            ꦮꦭꦶ NULL;
        }

    } ꦱꦼꦢꦼꦭ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ꦮꦭꦶ --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, var_na *s2, size_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    do {
        do {
            c1 = *s1++;

            ꦪꦼꦤ꧀ (c1 == 0) {
                ꦮꦭꦶ NULL;
            }

        } ꦱꦼꦢꦼꦭ (c1 != c2);

    } ꦱꦼꦢꦼꦭ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ꦮꦭꦶ --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, var_na *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    do {
        do {
            c1 = (ngx_uint_t) *s1++;

            ꦪꦼꦤ꧀ (c1 == 0) {
                ꦮꦭꦶ NULL;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ꦱꦼꦢꦼꦭ (c1 != c2);

    } ꦱꦼꦢꦼꦭ (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    ꦮꦭꦶ --s1;
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

    do {
        do {
            ꦪꦼꦤ꧀ (s1 >= last) {
                ꦮꦭꦶ NULL;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ꦱꦼꦢꦼꦭ (c1 != c2);

    } ꦱꦼꦢꦼꦭ (ngx_strncasecmp(s1, s2, n) != 0);

    ꦮꦭꦶ --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, size_t n)
{
    ꦪꦼꦤ꧀ (n == 0) {
        ꦮꦭꦶ 0;
    }

    n--;

    ꦏꦁꦒꦺꦴ ( ;; ) {
        ꦪꦼꦤ꧀ (s1[n] != s2[n]) {
            ꦮꦭꦶ s1[n] - s2[n];
        }

        ꦪꦼꦤ꧀ (n == 0) {
            ꦮꦭꦶ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n)
{
    u_char  c1, c2;

    ꦪꦼꦤ꧀ (n == 0) {
        ꦮꦭꦶ 0;
    }

    n--;

    ꦏꦁꦒꦺꦴ ( ;; ) {
        c1 = s1[n];
        ꦪꦼꦤ꧀ (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        ꦪꦼꦤ꧀ (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        ꦪꦼꦤ꧀ (c1 != c2) {
            ꦮꦭꦶ c1 - c2;
        }

        ꦪꦼꦤ꧀ (n == 0) {
            ꦮꦭꦶ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2)
{
    size_t     n;
    ngx_int_t  m, z;

    ꦪꦼꦤ꧀ (n1 <= n2) {
        n = n1;
        z = -1;

    } ꦭꦶꦪ {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    ꦪꦼꦤ꧀ (m || n1 == n2) {
        ꦮꦭꦶ m;
    }

    ꦮꦭꦶ z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    ꦏꦁꦒꦺꦴ ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ꦪꦼꦤ꧀ (c1 == c2) {

            ꦪꦼꦤ꧀ (c1) {
                continue;
            }

            ꦮꦭꦶ 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        ꦮꦭꦶ c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    ꦱꦼꦢꦼꦭ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#ꦪꦼꦤ꧀ (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#endif

        ꦪꦼꦤ꧀ (c1 == c2) {

            ꦪꦼꦤ꧀ (c1) {
                n--;
                continue;
            }

            ꦮꦭꦶ 0;
        }

        /* we need '/' to be the lowest character */

        ꦪꦼꦤ꧀ (c1 == 0 || c2 == 0) {
            ꦮꦭꦶ c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        ꦮꦭꦶ c1 - c2;
    }

    ꦮꦭꦶ 0;
}


ngx_int_t
ngx_atoi(u_char *line, size_t n)
{
    ngx_int_t  value, cutoff, cutlim;

    ꦪꦼꦤ꧀ (n == 0) {
        ꦮꦭꦶ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_VALUE / 10;
    cutlim = NGX_MAX_INT_T_VALUE % 10;

    ꦏꦁꦒꦺꦴ (value = 0; n--; line++) {
        ꦪꦼꦤ꧀ (*line < '0' || *line > '9') {
            ꦮꦭꦶ NGX_ERROR;
        }

        ꦪꦼꦤ꧀ (value >= cutoff && (value > cutoff || *line - '0' > cutlim)) {
            ꦮꦭꦶ NGX_ERROR;
        }

        value = value * 10 + (*line - '0');
    }

    ꦮꦭꦶ value;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *line, size_t n, size_t point)
{
    ngx_int_t   value, cutoff, cutlim;
    ngx_uint_t  dot;

    ꦪꦼꦤ꧀ (n == 0) {
        ꦮꦭꦶ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_VALUE / 10;
    cutlim = NGX_MAX_INT_T_VALUE % 10;

    dot = 0;

    ꦏꦁꦒꦺꦴ (value = 0; n--; line++) {

        ꦪꦼꦤ꧀ (point == 0) {
            ꦮꦭꦶ NGX_ERROR;
        }

        ꦪꦼꦤ꧀ (*line == '.') {
            ꦪꦼꦤ꧀ (dot) {
                ꦮꦭꦶ NGX_ERROR;
            }

            dot = 1;
            continue;
        }

        ꦪꦼꦤ꧀ (*line < '0' || *line > '9') {
            ꦮꦭꦶ NGX_ERROR;
        }

        ꦪꦼꦤ꧀ (value >= cutoff && (value > cutoff || *line - '0' > cutlim)) {
            ꦮꦭꦶ NGX_ERROR;
        }

        value = value * 10 + (*line - '0');
        point -= dot;
    }

    ꦱꦼꦢꦼꦭ (point--) {
        ꦪꦼꦤ꧀ (value > cutoff) {
            ꦮꦭꦶ NGX_ERROR;
        }

        value = value * 10;
    }

    ꦮꦭꦶ value;
}


ssize_t
ngx_atosz(u_char *line, size_t n)
{
    ssize_t  value, cutoff, cutlim;

    ꦪꦼꦤ꧀ (n == 0) {
        ꦮꦭꦶ NGX_ERROR;
    }

    cutoff = NGX_MAX_SIZE_T_VALUE / 10;
    cutlim = NGX_MAX_SIZE_T_VALUE % 10;

    ꦏꦁꦒꦺꦴ (value = 0; n--; line++) {
        ꦪꦼꦤ꧀ (*line < '0' || *line > '9') {
            ꦮꦭꦶ NGX_ERROR;
        }

        ꦪꦼꦤ꧀ (value >= cutoff && (value > cutoff || *line - '0' > cutlim)) {
            ꦮꦭꦶ NGX_ERROR;
        }

        value = value * 10 + (*line - '0');
    }

    ꦮꦭꦶ value;
}


off_t
ngx_atoof(u_char *line, size_t n)
{
    off_t  value, cutoff, cutlim;

    ꦪꦼꦤ꧀ (n == 0) {
        ꦮꦭꦶ NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_VALUE / 10;
    cutlim = NGX_MAX_OFF_T_VALUE % 10;

    ꦏꦁꦒꦺꦴ (value = 0; n--; line++) {
        ꦪꦼꦤ꧀ (*line < '0' || *line > '9') {
            ꦮꦭꦶ NGX_ERROR;
        }

        ꦪꦼꦤ꧀ (value >= cutoff && (value > cutoff || *line - '0' > cutlim)) {
            ꦮꦭꦶ NGX_ERROR;
        }

        value = value * 10 + (*line - '0');
    }

    ꦮꦭꦶ value;
}


time_t
ngx_atotm(u_char *line, size_t n)
{
    time_t  value, cutoff, cutlim;

    ꦪꦼꦤ꧀ (n == 0) {
        ꦮꦭꦶ NGX_ERROR;
    }

    cutoff = NGX_MAX_TIME_T_VALUE / 10;
    cutlim = NGX_MAX_TIME_T_VALUE % 10;

    ꦏꦁꦒꦺꦴ (value = 0; n--; line++) {
        ꦪꦼꦤ꧀ (*line < '0' || *line > '9') {
            ꦮꦭꦶ NGX_ERROR;
        }

        ꦪꦼꦤ꧀ (value >= cutoff && (value > cutoff || *line - '0' > cutlim)) {
            ꦮꦭꦶ NGX_ERROR;
        }

        value = value * 10 + (*line - '0');
    }

    ꦮꦭꦶ value;
}


ngx_int_t
ngx_hextoi(u_char *line, size_t n)
{
    u_char     c, ch;
    ngx_int_t  value, cutoff;

    ꦪꦼꦤ꧀ (n == 0) {
        ꦮꦭꦶ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_VALUE / 16;

    ꦏꦁꦒꦺꦴ (value = 0; n--; line++) {
        ꦪꦼꦤ꧀ (value > cutoff) {
            ꦮꦭꦶ NGX_ERROR;
        }

        ch = *line;

        ꦪꦼꦤ꧀ (ch >= '0' && ch <= '9') {
            value = value * 16 + (ch - '0');
            continue;
        }

        c = (u_char) (ch | 0x20);

        ꦪꦼꦤ꧀ (c >= 'a' && c <= 'f') {
            value = value * 16 + (c - 'a' + 10);
            continue;
        }

        ꦮꦭꦶ NGX_ERROR;
    }

    ꦮꦭꦶ value;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, size_t len)
{
    static u_char  hex[] = "0123456789abcdef";

    ꦱꦼꦢꦼꦭ (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    ꦮꦭꦶ dst;
}


void
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    static u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_internal(dst, src, basis64, 1);
}


void
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    static u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_internal(dst, src, basis64, 0);
}


static void
ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src, const u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    size_t          len;

    len = src->len;
    s = src->data;
    d = dst->data;

    ꦱꦼꦢꦼꦭ (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    ꦪꦼꦤ꧀ (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        ꦪꦼꦤ꧀ (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            ꦪꦼꦤ꧀ (padding) {
                *d++ = '=';
            }

        } ꦭꦶꦪ {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        ꦪꦼꦤ꧀ (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->data;
}


ngx_int_t
ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    static u_char   basis64[] = {
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

    ꦮꦭꦶ ngx_decode_base64_internal(dst, src, basis64);
}


ngx_int_t
ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    static u_char   basis64[] = {
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

    ꦮꦭꦶ ngx_decode_base64_internal(dst, src, basis64);
}


static ngx_int_t
ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src, const u_char *basis)
{
    size_t          len;
    u_char         *d, *s;

    ꦏꦁꦒꦺꦴ (len = 0; len < src->len; len++) {
        ꦪꦼꦤ꧀ (src->data[len] == '=') {
            bha_nga;
        }

        ꦪꦼꦤ꧀ (basis[src->data[len]] == 77) {
            ꦮꦭꦶ NGX_ERROR;
        }
    }

    ꦪꦼꦤ꧀ (len % 4 == 1) {
        ꦮꦭꦶ NGX_ERROR;
    }

    s = src->data;
    d = dst->data;

    ꦱꦼꦢꦼꦭ (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    ꦪꦼꦤ꧀ (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    ꦪꦼꦤ꧀ (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->data;

    ꦮꦭꦶ NGX_OK;
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

    ꦪꦼꦤ꧀ (u >= 0xf8) {

        (*p)++;
        ꦮꦭꦶ 0xffffffff;

    } ꦭꦶꦪ ꦪꦼꦤ꧀ (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } ꦭꦶꦪ ꦪꦼꦤ꧀ (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } ꦭꦶꦪ ꦪꦼꦤ꧀ (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } ꦭꦶꦪ {
        (*p)++;
        ꦮꦭꦶ 0xffffffff;
    }

    ꦪꦼꦤ꧀ (n - 1 < len) {
        ꦮꦭꦶ 0xfffffffe;
    }

    (*p)++;

    ꦱꦼꦢꦼꦭ (len) {
        i = *(*p)++;

        ꦪꦼꦤ꧀ (i < 0x80) {
            ꦮꦭꦶ 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    ꦪꦼꦤ꧀ (u > valid) {
        ꦮꦭꦶ u;
    }

    ꦮꦭꦶ 0xffffffff;
}


size_t
ngx_utf8_length(u_char *p, size_t n)
{
    u_char  c, *last;
    size_t  len;

    last = p + n;

    ꦏꦁꦒꦺꦴ (len = 0; p < last; len++) {

        c = *p;

        ꦪꦼꦤ꧀ (c < 0x80) {
            p++;
            continue;
        }

        ꦪꦼꦤ꧀ (ngx_utf8_decode(&p, last - p) > 0x10ffff) {
            /* invalid UTF-8 */
            ꦮꦭꦶ n;
        }
    }

    ꦮꦭꦶ len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len)
{
    u_char  c, *next;

    ꦪꦼꦤ꧀ (n == 0) {
        ꦮꦭꦶ dst;
    }

    ꦱꦼꦢꦼꦭ (--n) {

        c = *src;
        *dst = c;

        ꦪꦼꦤ꧀ (c < 0x80) {

            ꦪꦼꦤ꧀ (c != '\0') {
                dst++;
                src++;
                len--;

                continue;
            }

            ꦮꦭꦶ dst;
        }

        next = src;

        ꦪꦼꦤ꧀ (ngx_utf8_decode(&next, len) > 0x10ffff) {
            /* invalid UTF-8 */
            bha_nga;
        }

        ꦱꦼꦢꦼꦭ (src < next) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    ꦮꦭꦶ dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, size_t size, ngx_uint_t type)
{
    u_char          prefix;
    uint32_t       *escape;
    ngx_uint_t      n;
    static u_char   hex[] = "0123456789ABCDEF";

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

    static uint32_t   uri[] = {
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

    static uint32_t   args[] = {
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

    static uint32_t   uri_component[] = {
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

    static uint32_t   html[] = {
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

    static uint32_t   refresh[] = {
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

    static uint32_t   memcached[] = {
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

    static uint32_t   mail_xtext[] = {
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

    static uint32_t  *map[] =
        { uri, args, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    static u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    prefix = map_char[type];

    ꦪꦼꦤ꧀ (dst == NULL) {

        /* find the number of the characters to be escaped */

        n = 0;

        ꦱꦼꦢꦼꦭ (size) {
            ꦪꦼꦤ꧀ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            size--;
        }

        ꦮꦭꦶ (uintptr_t) n;
    }

    ꦱꦼꦢꦼꦭ (size) {
        ꦪꦼꦤ꧀ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = prefix;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } ꦭꦶꦪ {
            *dst++ = *src++;
        }
        size--;
    }

    ꦮꦭꦶ (uintptr_t) dst;
}


void
ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    ga_nanaa {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_second
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    ꦱꦼꦢꦼꦭ (size--) {

        ch = *s++;

        switch (state) {
        pak_sa sw_usual:
            ꦪꦼꦤ꧀ (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                goto done;
            }

            ꦪꦼꦤ꧀ (ch == '%') {
                state = sw_quoted;
                bha_nga;
            }

            *d++ = ch;
            bha_nga;

        pak_sa sw_quoted:

            ꦪꦼꦤ꧀ (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_second;
                bha_nga;
            }

            c = (u_char) (ch | 0x20);
            ꦪꦼꦤ꧀ (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_second;
                bha_nga;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            bha_nga;

        pak_sa sw_quoted_second:

            state = sw_usual;

            ꦪꦼꦤ꧀ (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                ꦪꦼꦤ꧀ (type & NGX_UNESCAPE_REDIRECT) {
                    ꦪꦼꦤ꧀ (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        bha_nga;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    bha_nga;
                }

                *d++ = ch;

                bha_nga;
            }

            c = (u_char) (ch | 0x20);
            ꦪꦼꦤ꧀ (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                ꦪꦼꦤ꧀ (type & NGX_UNESCAPE_URI) {
                    ꦪꦼꦤ꧀ (ch == '?') {
                        *d++ = ch;
                        goto done;
                    }

                    *d++ = ch;
                    bha_nga;
                }

                ꦪꦼꦤ꧀ (type & NGX_UNESCAPE_REDIRECT) {
                    ꦪꦼꦤ꧀ (ch == '?') {
                        *d++ = ch;
                        goto done;
                    }

                    ꦪꦼꦤ꧀ (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        bha_nga;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    bha_nga;
                }

                *d++ = ch;

                bha_nga;
            }

            /* the invalid quoted character */

            bha_nga;
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

    ꦪꦼꦤ꧀ (dst == NULL) {

        len = 0;

        ꦱꦼꦢꦼꦭ (size) {
            switch (*src++) {

            pak_sa '<':
                len += parimaa_na("&lt;") - 2;
                bha_nga;

            pak_sa '>':
                len += parimaa_na("&gt;") - 2;
                bha_nga;

            pak_sa '&':
                len += parimaa_na("&amp;") - 2;
                bha_nga;

            pak_sa '"':
                len += parimaa_na("&quot;") - 2;
                bha_nga;

            default:
                bha_nga;
            }
            size--;
        }

        ꦮꦭꦶ (uintptr_t) len;
    }

    ꦱꦼꦢꦼꦭ (size) {
        ch = *src++;

        switch (ch) {

        pak_sa '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            bha_nga;

        pak_sa '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            bha_nga;

        pak_sa '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            bha_nga;

        pak_sa '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            bha_nga;

        default:
            *dst++ = ch;
            bha_nga;
        }
        size--;
    }

    ꦮꦭꦶ (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, size_t size)
{
    u_char      ch;
    ngx_uint_t  len;

    ꦪꦼꦤ꧀ (dst == NULL) {
        len = 0;

        ꦱꦼꦢꦼꦭ (size) {
            ch = *src++;

            ꦪꦼꦤ꧀ (ch == '\\' || ch == '"') {
                len++;

            } ꦭꦶꦪ ꦪꦼꦤ꧀ (ch <= 0x1f) {

                switch (ch) {
                pak_sa '\n':
                pak_sa '\r':
                pak_sa '\t':
                pak_sa '\b':
                pak_sa '\f':
                    len++;
                    bha_nga;

                default:
                    len += parimaa_na("\\u001F") - 2;
                }
            }

            size--;
        }

        ꦮꦭꦶ (uintptr_t) len;
    }

    ꦱꦼꦢꦼꦭ (size) {
        ch = *src++;

        ꦪꦼꦤ꧀ (ch > 0x1f) {

            ꦪꦼꦤ꧀ (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } ꦭꦶꦪ {
            *dst++ = '\\';

            switch (ch) {
            pak_sa '\n':
                *dst++ = 'n';
                bha_nga;

            pak_sa '\r':
                *dst++ = 'r';
                bha_nga;

            pak_sa '\t':
                *dst++ = 't';
                bha_nga;

            pak_sa '\b':
                *dst++ = 'b';
                bha_nga;

            pak_sa '\f':
                *dst++ = 'f';
                bha_nga;

            default:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        size--;
    }

    ꦮꦭꦶ (uintptr_t) dst;
}


void
ngx_str_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_str_node_t      *n, *t;
    ngx_rbtree_node_t  **p;

    ꦏꦁꦒꦺꦴ ( ;; ) {

        n = (ngx_str_node_t *) node;
        t = (ngx_str_node_t *) temp;

        ꦪꦼꦤ꧀ (node->key != temp->key) {

            p = (node->key < temp->key) ? &temp->left : &temp->right;

        } ꦭꦶꦪ ꦪꦼꦤ꧀ (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &temp->left : &temp->right;

        } ꦭꦶꦪ {
            p = (ngx_memcmp(n->str.data, t->str.data, n->str.len) < 0)
                 ? &temp->left : &temp->right;
        }

        ꦪꦼꦤ꧀ (*p == sentinel) {
            bha_nga;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    ngx_rbt_red(node);
}


ngx_str_node_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_node_t     *n;
    ngx_rbtree_node_t  *node, *sentinel;

    node = rbtree->root;
    sentinel = rbtree->sentinel;

    ꦱꦼꦢꦼꦭ (node != sentinel) {

        n = (ngx_str_node_t *) node;

        ꦪꦼꦤ꧀ (hash != node->key) {
            node = (hash < node->key) ? node->left : node->right;
            continue;
        }

        ꦪꦼꦤ꧀ (val->len != n->str.len) {
            node = (val->len < n->str.len) ? node->left : node->right;
            continue;
        }

        rc = ngx_memcmp(val->data, n->str.data, val->len);

        ꦪꦼꦤ꧀ (rc < 0) {
            node = node->left;
            continue;
        }

        ꦪꦼꦤ꧀ (rc > 0) {
            node = node->right;
            continue;
        }

        ꦮꦭꦶ n;
    }

    ꦮꦭꦶ NULL;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

void
ngx_sort(void *base, size_t n, size_t size,
    ngx_int_t (*cmp)(const void *, const void *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(size, ngx_cycle->log);
    ꦪꦼꦤ꧀ (p == NULL) {
        ꦮꦭꦶ;
    }

    ꦏꦁꦒꦺꦴ (p1 = (u_char *) base + size;
         p1 < (u_char *) base + n * size;
         p1 += size)
    {
        ngx_memcpy(p, p1, size);

        ꦏꦁꦒꦺꦴ (p2 = p1;
             p2 > (u_char *) base && cmp(p2 - size, p) > 0;
             p2 -= size)
        {
            ngx_memcpy(p2, p2 - size, size);
        }

        ngx_memcpy(p2, p, size);
    }

    ngx_free(p);
}


void
ngx_explicit_memzero(void *buf, size_t n)
{
    ngx_memzero(buf, n);
    ngx_memory_barrier();
}


#ꦪꦼꦤ꧀ (NGX_MEMCPY_LIMIT)

void *
ngx_memcpy(void *dst, const void *src, size_t n)
{
    ꦪꦼꦤ꧀ (n > NGX_MEMCPY_LIMIT) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "memcpy %uz bytes", n);
        ngx_debug_point();
    }

    ꦮꦭꦶ memcpy(dst, src, n);
}

#endif
