
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
    ໃນຂະນະທີ່ (n) {
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

    ສຳລັບ (i = 0; i < n; i++) {

        ຖ້າ (p[i] == '\0') {
            ສົ່ງຄືນ i;
        }
    }

    ສົ່ງຄືນ n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, size_t n)
{
    ຖ້າ (n == 0) {
        ສົ່ງຄືນ dst;
    }

    ໃນຂະນະທີ່ (--n) {
        *dst = *src;

        ຖ້າ (*dst == '\0') {
            ສົ່ງຄືນ dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    ສົ່ງຄືນ dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    ຖ້າ (dst == NULL) {
        ສົ່ງຄືນ NULL;
    }

    ngx_memcpy(dst, src->data, src->len);

    ສົ່ງຄືນ dst;
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

    ສົ່ງຄືນ p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, size_t max, const var_na *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, args);
    va_end(args);

    ສົ່ງຄືນ p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *last, const var_na *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, last, fmt, args);
    va_end(args);

    ສົ່ງຄືນ p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *last, const var_na *fmt, va_list args)
{
    u_char                *p, zero;
    ຈຳນວນເຕັມ                    d;
    dviguu_na                 f;
    size_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             width, sign, hex, max_width, frac_width, scale, n;
    ngx_str_t             *v;
    ngx_variable_value_t  *vv;

    ໃນຂະນະທີ່ (*fmt && buf < last) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        ຖ້າ (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (size_t) -1;

            ໃນຂະນະທີ່ (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }


            ສຳລັບ ( ;; ) {
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

                    ໃນຂະນະທີ່ (*fmt >= '0' && *fmt <= '9') {
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
                ຖ້າ ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } ບໍ່ດັ່ງນັ້ນ {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                bha_nga;

            pak_sa 'z':
                ຖ້າ (sign) {
                    i64 = (int64_t) va_arg(args, ssize_t);
                } ບໍ່ດັ່ງນັ້ນ {
                    ui64 = (uint64_t) va_arg(args, size_t);
                }
                bha_nga;

            pak_sa 'i':
                ຖ້າ (sign) {
                    i64 = (int64_t) va_arg(args, ngx_int_t);
                } ບໍ່ດັ່ງນັ້ນ {
                    ui64 = (uint64_t) va_arg(args, ngx_uint_t);
                }

                ຖ້າ (max_width) {
                    width = NGX_INT_T_LEN;
                }

                bha_nga;

            pak_sa 'd':
                ຖ້າ (sign) {
                    i64 = (int64_t) va_arg(args, ຈຳນວນເຕັມ);
                } ບໍ່ດັ່ງນັ້ນ {
                    ui64 = (uint64_t) va_arg(args, u_int);
                }
                bha_nga;

            pak_sa 'l':
                ຖ້າ (sign) {
                    i64 = (int64_t) va_arg(args, long);
                } ບໍ່ດັ່ງນັ້ນ {
                    ui64 = (uint64_t) va_arg(args, u_long);
                }
                bha_nga;

            pak_sa 'D':
                ຖ້າ (sign) {
                    i64 = (int64_t) va_arg(args, int32_t);
                } ບໍ່ດັ່ງນັ້ນ {
                    ui64 = (uint64_t) va_arg(args, uint32_t);
                }
                bha_nga;

            pak_sa 'L':
                ຖ້າ (sign) {
                    i64 = va_arg(args, int64_t);
                } ບໍ່ດັ່ງນັ້ນ {
                    ui64 = va_arg(args, uint64_t);
                }
                bha_nga;

            pak_sa 'A':
                ຖ້າ (sign) {
                    i64 = (int64_t) va_arg(args, ngx_atomic_int_t);
                } ບໍ່ດັ່ງນັ້ນ {
                    ui64 = (uint64_t) va_arg(args, ngx_atomic_uint_t);
                }

                ຖ້າ (max_width) {
                    width = NGX_ATOMIC_T_LEN;
                }

                bha_nga;

            pak_sa 'f':
                f = va_arg(args, dviguu_na);

                ຖ້າ (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                ຖ້າ (frac_width) {

                    scale = 1;
                    ສຳລັບ (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (dviguu_na) ui64) * scale + 0.5);

                    ຖ້າ (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_num(buf, last, ui64, zero, 0, width);

                ຖ້າ (frac_width) {
                    ຖ້າ (buf < last) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_num(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                continue;

#ຖ້າ !(NGX_WIN32)
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
                d = va_arg(args, ຈຳນວນເຕັມ);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                continue;

            pak_sa 'Z':
                *buf++ = '\0';
                fmt++;

                continue;

            pak_sa 'N':
#ຖ້າ (NGX_WIN32)
                *buf++ = CR;
                ຖ້າ (buf < last) {
                    *buf++ = LF;
                }
#ບໍ່ດັ່ງນັ້ນ
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

            ຖ້າ (sign) {
                ຖ້າ (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } ບໍ່ດັ່ງນັ້ນ {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_num(buf, last, ui64, zero, hex, width);

            fmt++;

        } ບໍ່ດັ່ງນັ້ນ {
            *buf++ = *fmt++;
        }
    }

    ສົ່ງຄືນ buf;
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

    ຖ້າ (hexadecimal == 0) {

        ຖ້າ (ui64 <= (uint64_t) NGX_MAX_UINT32_VALUE) {

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
            } ໃນຂະນະທີ່ (ui32 /= 10);

        } ບໍ່ດັ່ງນັ້ນ {
            do {
                *--p = (u_char) (ui64 % 10 + '0');
            } ໃນຂະນະທີ່ (ui64 /= 10);
        }

    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (hexadecimal == 1) {

        do {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } ໃນຂະນະທີ່ (ui64 >>= 4);

    } ບໍ່ດັ່ງນັ້ນ { /* hexadecimal == 2 */

        do {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } ໃນຂະນະທີ່ (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    ໃນຂະນະທີ່ (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    ຖ້າ (buf + len > last) {
        len = last - buf;
    }

    ສົ່ງຄືນ ngx_cpymem(buf, p, len);
}


static u_char *
ngx_sprintf_str(u_char *buf, u_char *last, u_char *src, size_t len,
    ngx_uint_t hexadecimal)
{
    static u_char   hex[] = "0123456789abcdef";
    static u_char   HEX[] = "0123456789ABCDEF";

    ຖ້າ (hexadecimal == 0) {

        ຖ້າ (len == (size_t) -1) {
            ໃນຂະນະທີ່ (*src && buf < last) {
                *buf++ = *src++;
            }

        } ບໍ່ດັ່ງນັ້ນ {
            len = ngx_min((size_t) (last - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (hexadecimal == 1) {

        ຖ້າ (len == (size_t) -1) {

            ໃນຂະນະທີ່ (*src && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } ບໍ່ດັ່ງນັ້ນ {

            ໃນຂະນະທີ່ (len-- && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } ບໍ່ດັ່ງນັ້ນ { /* hexadecimal == 2 */

        ຖ້າ (len == (size_t) -1) {

            ໃນຂະນະທີ່ (*src && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } ບໍ່ດັ່ງນັ້ນ {

            ໃນຂະນະທີ່ (len-- && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    ສົ່ງຄືນ buf;
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

    ສຳລັບ ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ຖ້າ (c1 == c2) {

            ຖ້າ (c1) {
                continue;
            }

            ສົ່ງຄືນ 0;
        }

        ສົ່ງຄືນ c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    ໃນຂະນະທີ່ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ຖ້າ (c1 == c2) {

            ຖ້າ (c1) {
                n--;
                continue;
            }

            ສົ່ງຄືນ 0;
        }

        ສົ່ງຄືນ c1 - c2;
    }

    ສົ່ງຄືນ 0;
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
            ຖ້າ (len-- == 0) {
                ສົ່ງຄືນ NULL;
            }

            c1 = *s1++;

            ຖ້າ (c1 == 0) {
                ສົ່ງຄືນ NULL;
            }

        } ໃນຂະນະທີ່ (c1 != c2);

        ຖ້າ (n > len) {
            ສົ່ງຄືນ NULL;
        }

    } ໃນຂະນະທີ່ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ສົ່ງຄືນ --s1;
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

            ຖ້າ (c1 == 0) {
                ສົ່ງຄືນ NULL;
            }

        } ໃນຂະນະທີ່ (c1 != c2);

    } ໃນຂະນະທີ່ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ສົ່ງຄືນ --s1;
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

            ຖ້າ (c1 == 0) {
                ສົ່ງຄືນ NULL;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ໃນຂະນະທີ່ (c1 != c2);

    } ໃນຂະນະທີ່ (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    ສົ່ງຄືນ --s1;
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
            ຖ້າ (s1 >= last) {
                ສົ່ງຄືນ NULL;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ໃນຂະນະທີ່ (c1 != c2);

    } ໃນຂະນະທີ່ (ngx_strncasecmp(s1, s2, n) != 0);

    ສົ່ງຄືນ --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, size_t n)
{
    ຖ້າ (n == 0) {
        ສົ່ງຄືນ 0;
    }

    n--;

    ສຳລັບ ( ;; ) {
        ຖ້າ (s1[n] != s2[n]) {
            ສົ່ງຄືນ s1[n] - s2[n];
        }

        ຖ້າ (n == 0) {
            ສົ່ງຄືນ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n)
{
    u_char  c1, c2;

    ຖ້າ (n == 0) {
        ສົ່ງຄືນ 0;
    }

    n--;

    ສຳລັບ ( ;; ) {
        c1 = s1[n];
        ຖ້າ (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        ຖ້າ (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        ຖ້າ (c1 != c2) {
            ສົ່ງຄືນ c1 - c2;
        }

        ຖ້າ (n == 0) {
            ສົ່ງຄືນ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2)
{
    size_t     n;
    ngx_int_t  m, z;

    ຖ້າ (n1 <= n2) {
        n = n1;
        z = -1;

    } ບໍ່ດັ່ງນັ້ນ {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    ຖ້າ (m || n1 == n2) {
        ສົ່ງຄືນ m;
    }

    ສົ່ງຄືນ z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    ສຳລັບ ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ຖ້າ (c1 == c2) {

            ຖ້າ (c1) {
                continue;
            }

            ສົ່ງຄືນ 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        ສົ່ງຄືນ c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    ໃນຂະນະທີ່ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#ຖ້າ (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#endif

        ຖ້າ (c1 == c2) {

            ຖ້າ (c1) {
                n--;
                continue;
            }

            ສົ່ງຄືນ 0;
        }

        /* we need '/' to be the lowest character */

        ຖ້າ (c1 == 0 || c2 == 0) {
            ສົ່ງຄືນ c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        ສົ່ງຄືນ c1 - c2;
    }

    ສົ່ງຄືນ 0;
}


ngx_int_t
ngx_atoi(u_char *line, size_t n)
{
    ngx_int_t  value, cutoff, cutlim;

    ຖ້າ (n == 0) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_VALUE / 10;
    cutlim = NGX_MAX_INT_T_VALUE % 10;

    ສຳລັບ (value = 0; n--; line++) {
        ຖ້າ (*line < '0' || *line > '9') {
            ສົ່ງຄືນ NGX_ERROR;
        }

        ຖ້າ (value >= cutoff && (value > cutoff || *line - '0' > cutlim)) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        value = value * 10 + (*line - '0');
    }

    ສົ່ງຄືນ value;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *line, size_t n, size_t point)
{
    ngx_int_t   value, cutoff, cutlim;
    ngx_uint_t  dot;

    ຖ້າ (n == 0) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_VALUE / 10;
    cutlim = NGX_MAX_INT_T_VALUE % 10;

    dot = 0;

    ສຳລັບ (value = 0; n--; line++) {

        ຖ້າ (point == 0) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        ຖ້າ (*line == '.') {
            ຖ້າ (dot) {
                ສົ່ງຄືນ NGX_ERROR;
            }

            dot = 1;
            continue;
        }

        ຖ້າ (*line < '0' || *line > '9') {
            ສົ່ງຄືນ NGX_ERROR;
        }

        ຖ້າ (value >= cutoff && (value > cutoff || *line - '0' > cutlim)) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        value = value * 10 + (*line - '0');
        point -= dot;
    }

    ໃນຂະນະທີ່ (point--) {
        ຖ້າ (value > cutoff) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        value = value * 10;
    }

    ສົ່ງຄືນ value;
}


ssize_t
ngx_atosz(u_char *line, size_t n)
{
    ssize_t  value, cutoff, cutlim;

    ຖ້າ (n == 0) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    cutoff = NGX_MAX_SIZE_T_VALUE / 10;
    cutlim = NGX_MAX_SIZE_T_VALUE % 10;

    ສຳລັບ (value = 0; n--; line++) {
        ຖ້າ (*line < '0' || *line > '9') {
            ສົ່ງຄືນ NGX_ERROR;
        }

        ຖ້າ (value >= cutoff && (value > cutoff || *line - '0' > cutlim)) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        value = value * 10 + (*line - '0');
    }

    ສົ່ງຄືນ value;
}


off_t
ngx_atoof(u_char *line, size_t n)
{
    off_t  value, cutoff, cutlim;

    ຖ້າ (n == 0) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_VALUE / 10;
    cutlim = NGX_MAX_OFF_T_VALUE % 10;

    ສຳລັບ (value = 0; n--; line++) {
        ຖ້າ (*line < '0' || *line > '9') {
            ສົ່ງຄືນ NGX_ERROR;
        }

        ຖ້າ (value >= cutoff && (value > cutoff || *line - '0' > cutlim)) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        value = value * 10 + (*line - '0');
    }

    ສົ່ງຄືນ value;
}


time_t
ngx_atotm(u_char *line, size_t n)
{
    time_t  value, cutoff, cutlim;

    ຖ້າ (n == 0) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    cutoff = NGX_MAX_TIME_T_VALUE / 10;
    cutlim = NGX_MAX_TIME_T_VALUE % 10;

    ສຳລັບ (value = 0; n--; line++) {
        ຖ້າ (*line < '0' || *line > '9') {
            ສົ່ງຄືນ NGX_ERROR;
        }

        ຖ້າ (value >= cutoff && (value > cutoff || *line - '0' > cutlim)) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        value = value * 10 + (*line - '0');
    }

    ສົ່ງຄືນ value;
}


ngx_int_t
ngx_hextoi(u_char *line, size_t n)
{
    u_char     c, ch;
    ngx_int_t  value, cutoff;

    ຖ້າ (n == 0) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_VALUE / 16;

    ສຳລັບ (value = 0; n--; line++) {
        ຖ້າ (value > cutoff) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        ch = *line;

        ຖ້າ (ch >= '0' && ch <= '9') {
            value = value * 16 + (ch - '0');
            continue;
        }

        c = (u_char) (ch | 0x20);

        ຖ້າ (c >= 'a' && c <= 'f') {
            value = value * 16 + (c - 'a' + 10);
            continue;
        }

        ສົ່ງຄືນ NGX_ERROR;
    }

    ສົ່ງຄືນ value;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, size_t len)
{
    static u_char  hex[] = "0123456789abcdef";

    ໃນຂະນະທີ່ (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    ສົ່ງຄືນ dst;
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

    ໃນຂະນະທີ່ (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    ຖ້າ (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        ຖ້າ (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            ຖ້າ (padding) {
                *d++ = '=';
            }

        } ບໍ່ດັ່ງນັ້ນ {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        ຖ້າ (padding) {
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

    ສົ່ງຄືນ ngx_decode_base64_internal(dst, src, basis64);
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

    ສົ່ງຄືນ ngx_decode_base64_internal(dst, src, basis64);
}


static ngx_int_t
ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src, const u_char *basis)
{
    size_t          len;
    u_char         *d, *s;

    ສຳລັບ (len = 0; len < src->len; len++) {
        ຖ້າ (src->data[len] == '=') {
            bha_nga;
        }

        ຖ້າ (basis[src->data[len]] == 77) {
            ສົ່ງຄືນ NGX_ERROR;
        }
    }

    ຖ້າ (len % 4 == 1) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    s = src->data;
    d = dst->data;

    ໃນຂະນະທີ່ (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    ຖ້າ (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    ຖ້າ (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->data;

    ສົ່ງຄືນ NGX_OK;
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

    ຖ້າ (u >= 0xf8) {

        (*p)++;
        ສົ່ງຄືນ 0xffffffff;

    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } ບໍ່ດັ່ງນັ້ນ {
        (*p)++;
        ສົ່ງຄືນ 0xffffffff;
    }

    ຖ້າ (n - 1 < len) {
        ສົ່ງຄືນ 0xfffffffe;
    }

    (*p)++;

    ໃນຂະນະທີ່ (len) {
        i = *(*p)++;

        ຖ້າ (i < 0x80) {
            ສົ່ງຄືນ 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    ຖ້າ (u > valid) {
        ສົ່ງຄືນ u;
    }

    ສົ່ງຄືນ 0xffffffff;
}


size_t
ngx_utf8_length(u_char *p, size_t n)
{
    u_char  c, *last;
    size_t  len;

    last = p + n;

    ສຳລັບ (len = 0; p < last; len++) {

        c = *p;

        ຖ້າ (c < 0x80) {
            p++;
            continue;
        }

        ຖ້າ (ngx_utf8_decode(&p, last - p) > 0x10ffff) {
            /* invalid UTF-8 */
            ສົ່ງຄືນ n;
        }
    }

    ສົ່ງຄືນ len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len)
{
    u_char  c, *next;

    ຖ້າ (n == 0) {
        ສົ່ງຄືນ dst;
    }

    ໃນຂະນະທີ່ (--n) {

        c = *src;
        *dst = c;

        ຖ້າ (c < 0x80) {

            ຖ້າ (c != '\0') {
                dst++;
                src++;
                len--;

                continue;
            }

            ສົ່ງຄືນ dst;
        }

        next = src;

        ຖ້າ (ngx_utf8_decode(&next, len) > 0x10ffff) {
            /* invalid UTF-8 */
            bha_nga;
        }

        ໃນຂະນະທີ່ (src < next) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    ສົ່ງຄືນ dst;
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

    ຖ້າ (dst == NULL) {

        /* find the number of the characters to be escaped */

        n = 0;

        ໃນຂະນະທີ່ (size) {
            ຖ້າ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            size--;
        }

        ສົ່ງຄືນ (uintptr_t) n;
    }

    ໃນຂະນະທີ່ (size) {
        ຖ້າ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = prefix;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } ບໍ່ດັ່ງນັ້ນ {
            *dst++ = *src++;
        }
        size--;
    }

    ສົ່ງຄືນ (uintptr_t) dst;
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

    ໃນຂະນະທີ່ (size--) {

        ch = *s++;

        switch (state) {
        pak_sa sw_usual:
            ຖ້າ (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                goto done;
            }

            ຖ້າ (ch == '%') {
                state = sw_quoted;
                bha_nga;
            }

            *d++ = ch;
            bha_nga;

        pak_sa sw_quoted:

            ຖ້າ (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_second;
                bha_nga;
            }

            c = (u_char) (ch | 0x20);
            ຖ້າ (c >= 'a' && c <= 'f') {
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

            ຖ້າ (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                ຖ້າ (type & NGX_UNESCAPE_REDIRECT) {
                    ຖ້າ (ch > '%' && ch < 0x7f) {
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
            ຖ້າ (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                ຖ້າ (type & NGX_UNESCAPE_URI) {
                    ຖ້າ (ch == '?') {
                        *d++ = ch;
                        goto done;
                    }

                    *d++ = ch;
                    bha_nga;
                }

                ຖ້າ (type & NGX_UNESCAPE_REDIRECT) {
                    ຖ້າ (ch == '?') {
                        *d++ = ch;
                        goto done;
                    }

                    ຖ້າ (ch > '%' && ch < 0x7f) {
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

    ຖ້າ (dst == NULL) {

        len = 0;

        ໃນຂະນະທີ່ (size) {
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

        ສົ່ງຄືນ (uintptr_t) len;
    }

    ໃນຂະນະທີ່ (size) {
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

    ສົ່ງຄືນ (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, size_t size)
{
    u_char      ch;
    ngx_uint_t  len;

    ຖ້າ (dst == NULL) {
        len = 0;

        ໃນຂະນະທີ່ (size) {
            ch = *src++;

            ຖ້າ (ch == '\\' || ch == '"') {
                len++;

            } ບໍ່ດັ່ງນັ້ນ ຖ້າ (ch <= 0x1f) {

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

        ສົ່ງຄືນ (uintptr_t) len;
    }

    ໃນຂະນະທີ່ (size) {
        ch = *src++;

        ຖ້າ (ch > 0x1f) {

            ຖ້າ (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } ບໍ່ດັ່ງນັ້ນ {
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

    ສົ່ງຄືນ (uintptr_t) dst;
}


void
ngx_str_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    ngx_str_node_t      *n, *t;
    ngx_rbtree_node_t  **p;

    ສຳລັບ ( ;; ) {

        n = (ngx_str_node_t *) node;
        t = (ngx_str_node_t *) temp;

        ຖ້າ (node->key != temp->key) {

            p = (node->key < temp->key) ? &temp->left : &temp->right;

        } ບໍ່ດັ່ງນັ້ນ ຖ້າ (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &temp->left : &temp->right;

        } ບໍ່ດັ່ງນັ້ນ {
            p = (ngx_memcmp(n->str.data, t->str.data, n->str.len) < 0)
                 ? &temp->left : &temp->right;
        }

        ຖ້າ (*p == sentinel) {
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

    ໃນຂະນະທີ່ (node != sentinel) {

        n = (ngx_str_node_t *) node;

        ຖ້າ (hash != node->key) {
            node = (hash < node->key) ? node->left : node->right;
            continue;
        }

        ຖ້າ (val->len != n->str.len) {
            node = (val->len < n->str.len) ? node->left : node->right;
            continue;
        }

        rc = ngx_memcmp(val->data, n->str.data, val->len);

        ຖ້າ (rc < 0) {
            node = node->left;
            continue;
        }

        ຖ້າ (rc > 0) {
            node = node->right;
            continue;
        }

        ສົ່ງຄືນ n;
    }

    ສົ່ງຄືນ NULL;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

void
ngx_sort(void *base, size_t n, size_t size,
    ngx_int_t (*cmp)(const void *, const void *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(size, ngx_cycle->log);
    ຖ້າ (p == NULL) {
        ສົ່ງຄືນ;
    }

    ສຳລັບ (p1 = (u_char *) base + size;
         p1 < (u_char *) base + n * size;
         p1 += size)
    {
        ngx_memcpy(p, p1, size);

        ສຳລັບ (p2 = p1;
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


#ຖ້າ (NGX_MEMCPY_LIMIT)

void *
ngx_memcpy(void *dst, const void *src, size_t n)
{
    ຖ້າ (n > NGX_MEMCPY_LIMIT) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "memcpy %uz bytes", n);
        ngx_debug_point();
    }

    ສົ່ງຄືນ memcpy(dst, src, n);
}

#endif
