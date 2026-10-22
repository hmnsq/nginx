
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ସ୍ଥିର u_char *ngx_sprintf_ଅଙ୍କ(u_char *buf, u_char *last, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
ସ୍ଥିର u_char *ngx_sprintf_str(u_char *buf, u_char *last, u_char *src,
    size_t len, ngx_uint_t hexadecimal);
ସ୍ଥିର ଶୂନ୍ୟ ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ଅଚଳ u_char *basis, ngx_uint_t padding);
ସ୍ଥିର ngx_int_t ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ଅଚଳ u_char *basis);


ଶୂନ୍ୟ
ngx_strlow(u_char *dst, u_char *src, size_t n)
{
    ଯେତେବେଳେ (n) {
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

    ପାଇଁ (i = 0; i < n; i++) {

        ଯଦି (p[i] == '\0') {
            ଫେରନ୍ତୁ i;
        }
    }

    ଫେରନ୍ତୁ n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, size_t n)
{
    ଯଦି (n == 0) {
        ଫେରନ୍ତୁ dst;
    }

    ଯେତେବେଳେ (--n) {
        *dst = *src;

        ଯଦି (*dst == '\0') {
            ଫେରନ୍ତୁ dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    ଫେରନ୍ତୁ dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    ଯଦି (dst == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ngx_memcpy(dst, src->data, src->len);

    ଫେରନ୍ତୁ dst;
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
ngx_sprintf(u_char *buf, ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, (ଶୂନ୍ୟ *) -1, fmt, args);
    va_end(args);

    ଫେରନ୍ତୁ p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, size_t max, ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, args);
    va_end(args);

    ଫେରନ୍ତୁ p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *last, ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, last, fmt, args);
    va_end(args);

    ଫେରନ୍ତୁ p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *last, ଅଚଳ ଅକ୍ଷର *fmt, va_list args)
{
    u_char                *p, zero;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା                    d;
    ଦ୍ୱିଗୁଣ                 f;
    size_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             width, sign, hex, max_width, frac_width, scale, n;
    ngx_str_t             *v;
    ngx_variable_ମୂଲ୍ୟ_t  *vv;

    ଯେତେବେଳେ (*fmt && buf < last) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        ଯଦି (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (size_t) -1;

            ଯେତେବେଳେ (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }


            ପାଇଁ ( ;; ) {
                ବାଛ (*fmt) {

                ସ୍ଥିତି 'u':
                    sign = 0;
                    fmt++;
                    ଜାରି;

                ସ୍ଥିତି 'm':
                    max_width = 1;
                    fmt++;
                    ଜାରି;

                ସ୍ଥିତି 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    ଜାରି;

                ସ୍ଥିତି 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    ଜାରି;

                ସ୍ଥିତି '.':
                    fmt++;

                    ଯେତେବେଳେ (*fmt >= '0' && *fmt <= '9') {
                        frac_width = frac_width * 10 + (*fmt++ - '0');
                    }

                    ଭାଙ୍ଗ;

                ସ୍ଥିତି '*':
                    slen = va_arg(args, size_t);
                    fmt++;
                    ଜାରି;

                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                    ଭାଙ୍ଗ;
                }

                ଭାଙ୍ଗ;
            }


            ବାଛ (*fmt) {

            ସ୍ଥିତି 'V':
                v = va_arg(args, ngx_str_t *);

                buf = ngx_sprintf_str(buf, last, v->data, v->len, hex);
                fmt++;

                ଜାରି;

            ସ୍ଥିତି 'v':
                vv = va_arg(args, ngx_variable_ମୂଲ୍ୟ_t *);

                buf = ngx_sprintf_str(buf, last, vv->data, vv->len, hex);
                fmt++;

                ଜାରି;

            ସ୍ଥିତି 's':
                p = va_arg(args, u_char *);

                buf = ngx_sprintf_str(buf, last, p, slen, hex);
                fmt++;

                ଜାରି;

            ସ୍ଥିତି 'O':
                i64 = (int64_t) va_arg(args, off_t);
                sign = 1;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'P':
                i64 = (int64_t) va_arg(args, ngx_pid_t);
                sign = 1;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'T':
                i64 = (int64_t) va_arg(args, time_t);
                sign = 1;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'M':
                ms = (ngx_msec_t) va_arg(args, ngx_msec_t);
                ଯଦି ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } ଅନ୍ୟଥା {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'z':
                ଯଦି (sign) {
                    i64 = (int64_t) va_arg(args, ssize_t);
                } ଅନ୍ୟଥା {
                    ui64 = (uint64_t) va_arg(args, size_t);
                }
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'i':
                ଯଦି (sign) {
                    i64 = (int64_t) va_arg(args, ngx_int_t);
                } ଅନ୍ୟଥା {
                    ui64 = (uint64_t) va_arg(args, ngx_uint_t);
                }

                ଯଦି (max_width) {
                    width = NGX_INT_T_LEN;
                }

                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'd':
                ଯଦି (sign) {
                    i64 = (int64_t) va_arg(args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
                } ଅନ୍ୟଥା {
                    ui64 = (uint64_t) va_arg(args, u_int);
                }
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'l':
                ଯଦି (sign) {
                    i64 = (int64_t) va_arg(args, long);
                } ଅନ୍ୟଥା {
                    ui64 = (uint64_t) va_arg(args, u_long);
                }
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'D':
                ଯଦି (sign) {
                    i64 = (int64_t) va_arg(args, int32_t);
                } ଅନ୍ୟଥା {
                    ui64 = (uint64_t) va_arg(args, uint32_t);
                }
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'L':
                ଯଦି (sign) {
                    i64 = va_arg(args, int64_t);
                } ଅନ୍ୟଥା {
                    ui64 = va_arg(args, uint64_t);
                }
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'A':
                ଯଦି (sign) {
                    i64 = (int64_t) va_arg(args, ngx_atomic_int_t);
                } ଅନ୍ୟଥା {
                    ui64 = (uint64_t) va_arg(args, ngx_atomic_uint_t);
                }

                ଯଦି (max_width) {
                    width = NGX_ATOMIC_T_LEN;
                }

                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'f':
                f = va_arg(args, ଦ୍ୱିଗୁଣ);

                ଯଦି (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                ଯଦି (frac_width) {

                    scale = 1;
                    ପାଇଁ (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (ଦ୍ୱିଗୁଣ) ui64) * scale + 0.5);

                    ଯଦି (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_ଅଙ୍କ(buf, last, ui64, zero, 0, width);

                ଯଦି (frac_width) {
                    ଯଦି (buf < last) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_ଅଙ୍କ(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                ଜାରି;

#ଯଦି !(NGX_WIN32)
            ସ୍ଥିତି 'r':
                i64 = (int64_t) va_arg(args, rlim_t);
                sign = 1;
                ଭାଙ୍ଗ;
#endif

            ସ୍ଥିତି 'p':
                ui64 = (uintptr_t) va_arg(args, ଶୂନ୍ୟ *);
                hex = 2;
                sign = 0;
                zero = '0';
                width = 2 * ଆକାର(ଶୂନ୍ୟ *);
                ଭାଙ୍ଗ;

            ସ୍ଥିତି 'c':
                d = va_arg(args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                ଜାରି;

            ସ୍ଥିତି 'Z':
                *buf++ = '\0';
                fmt++;

                ଜାରି;

            ସ୍ଥିତି 'N':
#ଯଦି (NGX_WIN32)
                *buf++ = CR;
                ଯଦି (buf < last) {
                    *buf++ = LF;
                }
#ଅନ୍ୟଥା
                *buf++ = LF;
#endif
                fmt++;

                ଜାରି;

            ସ୍ଥିତି '%':
                *buf++ = '%';
                fmt++;

                ଜାରି;

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                *buf++ = *fmt++;

                ଜାରି;
            }

            ଯଦି (sign) {
                ଯଦି (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } ଅନ୍ୟଥା {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_ଅଙ୍କ(buf, last, ui64, zero, hex, width);

            fmt++;

        } ଅନ୍ୟଥା {
            *buf++ = *fmt++;
        }
    }

    ଫେରନ୍ତୁ buf;
}


ସ୍ଥିର u_char *
ngx_sprintf_ଅଙ୍କ(u_char *buf, u_char *last, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t width)
{
    u_char         *p, temp[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    size_t          len;
    uint32_t        ui32;
    ସ୍ଥିର u_char   hex[] = "0123456789abcdef";
    ସ୍ଥିର u_char   HEX[] = "0123456789ABCDEF";

    p = temp + NGX_INT64_LEN;

    ଯଦି (hexadecimal == 0) {

        ଯଦି (ui64 <= (uint64_t) NGX_MAX_UINT32_ମୂଲ୍ୟ) {

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

            କର {
                *--p = (u_char) (ui32 % 10 + '0');
            } ଯେତେବେଳେ (ui32 /= 10);

        } ଅନ୍ୟଥା {
            କର {
                *--p = (u_char) (ui64 % 10 + '0');
            } ଯେତେବେଳେ (ui64 /= 10);
        }

    } ଅନ୍ୟଥା ଯଦି (hexadecimal == 1) {

        କର {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } ଯେତେବେଳେ (ui64 >>= 4);

    } ଅନ୍ୟଥା { /* hexadecimal == 2 */

        କର {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } ଯେତେବେଳେ (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    ଯେତେବେଳେ (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    ଯଦି (buf + len > last) {
        len = last - buf;
    }

    ଫେରନ୍ତୁ ngx_cpymem(buf, p, len);
}


ସ୍ଥିର u_char *
ngx_sprintf_str(u_char *buf, u_char *last, u_char *src, size_t len,
    ngx_uint_t hexadecimal)
{
    ସ୍ଥିର u_char   hex[] = "0123456789abcdef";
    ସ୍ଥିର u_char   HEX[] = "0123456789ABCDEF";

    ଯଦି (hexadecimal == 0) {

        ଯଦି (len == (size_t) -1) {
            ଯେତେବେଳେ (*src && buf < last) {
                *buf++ = *src++;
            }

        } ଅନ୍ୟଥା {
            len = ngx_min((size_t) (last - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } ଅନ୍ୟଥା ଯଦି (hexadecimal == 1) {

        ଯଦି (len == (size_t) -1) {

            ଯେତେବେଳେ (*src && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } ଅନ୍ୟଥା {

            ଯେତେବେଳେ (len-- && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } ଅନ୍ୟଥା { /* hexadecimal == 2 */

        ଯଦି (len == (size_t) -1) {

            ଯେତେବେଳେ (*src && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } ଅନ୍ୟଥା {

            ଯେତେବେଳେ (len-- && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    ଫେରନ୍ତୁ buf;
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

    ପାଇଁ ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ଯଦି (c1 == c2) {

            ଯଦି (c1) {
                ଜାରି;
            }

            ଫେରନ୍ତୁ 0;
        }

        ଫେରନ୍ତୁ c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    ଯେତେବେଳେ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ଯଦି (c1 == c2) {

            ଯଦି (c1) {
                n--;
                ଜାରି;
            }

            ଫେରନ୍ତୁ 0;
        }

        ଫେରନ୍ତୁ c1 - c2;
    }

    ଫେରନ୍ତୁ 0;
}


u_char *
ngx_strnstr(u_char *s1, ଅକ୍ଷର *s2, size_t len)
{
    u_char  c1, c2;
    size_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    କର {
        କର {
            ଯଦି (len-- == 0) {
                ଫେରନ୍ତୁ ଖାଲି;
            }

            c1 = *s1++;

            ଯଦି (c1 == 0) {
                ଫେରନ୍ତୁ ଖାଲି;
            }

        } ଯେତେବେଳେ (c1 != c2);

        ଯଦି (n > len) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

    } ଯେତେବେଳେ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ଫେରନ୍ତୁ --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, ଅକ୍ଷର *s2, size_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    କର {
        କର {
            c1 = *s1++;

            ଯଦି (c1 == 0) {
                ଫେରନ୍ତୁ ଖାଲି;
            }

        } ଯେତେବେଳେ (c1 != c2);

    } ଯେତେବେଳେ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ଫେରନ୍ତୁ --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, ଅକ୍ଷର *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    କର {
        କର {
            c1 = (ngx_uint_t) *s1++;

            ଯଦି (c1 == 0) {
                ଫେରନ୍ତୁ ଖାଲି;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ଯେତେବେଳେ (c1 != c2);

    } ଯେତେବେଳେ (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    ଫେରନ୍ତୁ --s1;
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

    କର {
        କର {
            ଯଦି (s1 >= last) {
                ଫେରନ୍ତୁ ଖାଲି;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ଯେତେବେଳେ (c1 != c2);

    } ଯେତେବେଳେ (ngx_strncasecmp(s1, s2, n) != 0);

    ଫେରନ୍ତୁ --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, size_t n)
{
    ଯଦି (n == 0) {
        ଫେରନ୍ତୁ 0;
    }

    n--;

    ପାଇଁ ( ;; ) {
        ଯଦି (s1[n] != s2[n]) {
            ଫେରନ୍ତୁ s1[n] - s2[n];
        }

        ଯଦି (n == 0) {
            ଫେରନ୍ତୁ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n)
{
    u_char  c1, c2;

    ଯଦି (n == 0) {
        ଫେରନ୍ତୁ 0;
    }

    n--;

    ପାଇଁ ( ;; ) {
        c1 = s1[n];
        ଯଦି (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        ଯଦି (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        ଯଦି (c1 != c2) {
            ଫେରନ୍ତୁ c1 - c2;
        }

        ଯଦି (n == 0) {
            ଫେରନ୍ତୁ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2)
{
    size_t     n;
    ngx_int_t  m, z;

    ଯଦି (n1 <= n2) {
        n = n1;
        z = -1;

    } ଅନ୍ୟଥା {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    ଯଦି (m || n1 == n2) {
        ଫେରନ୍ତୁ m;
    }

    ଫେରନ୍ତୁ z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    ପାଇଁ ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ଯଦି (c1 == c2) {

            ଯଦି (c1) {
                ଜାରି;
            }

            ଫେରନ୍ତୁ 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        ଫେରନ୍ତୁ c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    ଯେତେବେଳେ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#ଯଦି (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#endif

        ଯଦି (c1 == c2) {

            ଯଦି (c1) {
                n--;
                ଜାରି;
            }

            ଫେରନ୍ତୁ 0;
        }

        /* we need '/' to be the lowest character */

        ଯଦି (c1 == 0 || c2 == 0) {
            ଫେରନ୍ତୁ c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        ଫେରନ୍ତୁ c1 - c2;
    }

    ଫେରନ୍ତୁ 0;
}


ngx_int_t
ngx_atoi(u_char *ଧାଡ଼ି, size_t n)
{
    ngx_int_t  ମୂଲ୍ୟ, cutoff, cutlim;

    ଯଦି (n == 0) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_ମୂଲ୍ୟ / 10;
    cutlim = NGX_MAX_INT_T_ମୂଲ୍ୟ % 10;

    ପାଇଁ (ମୂଲ୍ୟ = 0; n--; ଧାଡ଼ି++) {
        ଯଦି (*ଧାଡ଼ି < '0' || *ଧାଡ଼ି > '9') {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (ମୂଲ୍ୟ >= cutoff && (ମୂଲ୍ୟ > cutoff || *ଧାଡ଼ି - '0' > cutlim)) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ମୂଲ୍ୟ = ମୂଲ୍ୟ * 10 + (*ଧାଡ଼ି - '0');
    }

    ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *ଧାଡ଼ି, size_t n, size_t point)
{
    ngx_int_t   ମୂଲ୍ୟ, cutoff, cutlim;
    ngx_uint_t  dot;

    ଯଦି (n == 0) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_ମୂଲ୍ୟ / 10;
    cutlim = NGX_MAX_INT_T_ମୂଲ୍ୟ % 10;

    dot = 0;

    ପାଇଁ (ମୂଲ୍ୟ = 0; n--; ଧାଡ଼ି++) {

        ଯଦି (point == 0) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (*ଧାଡ଼ି == '.') {
            ଯଦି (dot) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            dot = 1;
            ଜାରି;
        }

        ଯଦି (*ଧାଡ଼ି < '0' || *ଧାଡ଼ି > '9') {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (ମୂଲ୍ୟ >= cutoff && (ମୂଲ୍ୟ > cutoff || *ଧାଡ଼ି - '0' > cutlim)) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ମୂଲ୍ୟ = ମୂଲ୍ୟ * 10 + (*ଧାଡ଼ି - '0');
        point -= dot;
    }

    ଯେତେବେଳେ (point--) {
        ଯଦି (ମୂଲ୍ୟ > cutoff) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ମୂଲ୍ୟ = ମୂଲ୍ୟ * 10;
    }

    ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}


ssize_t
ngx_atosz(u_char *ଧାଡ଼ି, size_t n)
{
    ssize_t  ମୂଲ୍ୟ, cutoff, cutlim;

    ଯଦି (n == 0) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    cutoff = NGX_MAX_SIZE_T_ମୂଲ୍ୟ / 10;
    cutlim = NGX_MAX_SIZE_T_ମୂଲ୍ୟ % 10;

    ପାଇଁ (ମୂଲ୍ୟ = 0; n--; ଧାଡ଼ି++) {
        ଯଦି (*ଧାଡ଼ି < '0' || *ଧାଡ଼ି > '9') {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (ମୂଲ୍ୟ >= cutoff && (ମୂଲ୍ୟ > cutoff || *ଧାଡ଼ି - '0' > cutlim)) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ମୂଲ୍ୟ = ମୂଲ୍ୟ * 10 + (*ଧାଡ଼ି - '0');
    }

    ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}


off_t
ngx_atoof(u_char *ଧାଡ଼ି, size_t n)
{
    off_t  ମୂଲ୍ୟ, cutoff, cutlim;

    ଯଦି (n == 0) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_ମୂଲ୍ୟ / 10;
    cutlim = NGX_MAX_OFF_T_ମୂଲ୍ୟ % 10;

    ପାଇଁ (ମୂଲ୍ୟ = 0; n--; ଧାଡ଼ି++) {
        ଯଦି (*ଧାଡ଼ି < '0' || *ଧାଡ଼ି > '9') {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (ମୂଲ୍ୟ >= cutoff && (ମୂଲ୍ୟ > cutoff || *ଧାଡ଼ି - '0' > cutlim)) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ମୂଲ୍ୟ = ମୂଲ୍ୟ * 10 + (*ଧାଡ଼ି - '0');
    }

    ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}


time_t
ngx_atotm(u_char *ଧାଡ଼ି, size_t n)
{
    time_t  ମୂଲ୍ୟ, cutoff, cutlim;

    ଯଦି (n == 0) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    cutoff = NGX_MAX_TIME_T_ମୂଲ୍ୟ / 10;
    cutlim = NGX_MAX_TIME_T_ମୂଲ୍ୟ % 10;

    ପାଇଁ (ମୂଲ୍ୟ = 0; n--; ଧାଡ଼ି++) {
        ଯଦି (*ଧାଡ଼ି < '0' || *ଧାଡ଼ି > '9') {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (ମୂଲ୍ୟ >= cutoff && (ମୂଲ୍ୟ > cutoff || *ଧାଡ଼ି - '0' > cutlim)) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ମୂଲ୍ୟ = ମୂଲ୍ୟ * 10 + (*ଧାଡ଼ି - '0');
    }

    ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}


ngx_int_t
ngx_hextoi(u_char *ଧାଡ଼ି, size_t n)
{
    u_char     c, ch;
    ngx_int_t  ମୂଲ୍ୟ, cutoff;

    ଯଦି (n == 0) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_ମୂଲ୍ୟ / 16;

    ପାଇଁ (ମୂଲ୍ୟ = 0; n--; ଧାଡ଼ି++) {
        ଯଦି (ମୂଲ୍ୟ > cutoff) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ch = *ଧାଡ଼ି;

        ଯଦି (ch >= '0' && ch <= '9') {
            ମୂଲ୍ୟ = ମୂଲ୍ୟ * 16 + (ch - '0');
            ଜାରି;
        }

        c = (u_char) (ch | 0x20);

        ଯଦି (c >= 'a' && c <= 'f') {
            ମୂଲ୍ୟ = ମୂଲ୍ୟ * 16 + (c - 'a' + 10);
            ଜାରି;
        }

        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, size_t len)
{
    ସ୍ଥିର u_char  hex[] = "0123456789abcdef";

    ଯେତେବେଳେ (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    ଫେରନ୍ତୁ dst;
}


ଶୂନ୍ୟ
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    ସ୍ଥିର u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_internal(dst, src, basis64, 1);
}


ଶୂନ୍ୟ
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    ସ୍ଥିର u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_internal(dst, src, basis64, 0);
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src, ଅଚଳ u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    size_t          len;

    len = src->len;
    s = src->data;
    d = dst->data;

    ଯେତେବେଳେ (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    ଯଦି (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        ଯଦି (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            ଯଦି (padding) {
                *d++ = '=';
            }

        } ଅନ୍ୟଥା {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        ଯଦି (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->data;
}


ngx_int_t
ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    ସ୍ଥିର u_char   basis64[] = {
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

    ଫେରନ୍ତୁ ngx_decode_base64_internal(dst, src, basis64);
}


ngx_int_t
ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    ସ୍ଥିର u_char   basis64[] = {
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

    ଫେରନ୍ତୁ ngx_decode_base64_internal(dst, src, basis64);
}


ସ୍ଥିର ngx_int_t
ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src, ଅଚଳ u_char *basis)
{
    size_t          len;
    u_char         *d, *s;

    ପାଇଁ (len = 0; len < src->len; len++) {
        ଯଦି (src->data[len] == '=') {
            ଭାଙ୍ଗ;
        }

        ଯଦି (basis[src->data[len]] == 77) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    ଯଦି (len % 4 == 1) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    s = src->data;
    d = dst->data;

    ଯେତେବେଳେ (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    ଯଦି (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    ଯଦି (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->data;

    ଫେରନ୍ତୁ NGX_OK;
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

    ଯଦି (u >= 0xf8) {

        (*p)++;
        ଫେରନ୍ତୁ 0xffffffff;

    } ଅନ୍ୟଥା ଯଦି (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } ଅନ୍ୟଥା ଯଦି (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } ଅନ୍ୟଥା ଯଦି (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } ଅନ୍ୟଥା {
        (*p)++;
        ଫେରନ୍ତୁ 0xffffffff;
    }

    ଯଦି (n - 1 < len) {
        ଫେରନ୍ତୁ 0xfffffffe;
    }

    (*p)++;

    ଯେତେବେଳେ (len) {
        i = *(*p)++;

        ଯଦି (i < 0x80) {
            ଫେରନ୍ତୁ 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    ଯଦି (u > valid) {
        ଫେରନ୍ତୁ u;
    }

    ଫେରନ୍ତୁ 0xffffffff;
}


size_t
ngx_utf8_length(u_char *p, size_t n)
{
    u_char  c, *last;
    size_t  len;

    last = p + n;

    ପାଇଁ (len = 0; p < last; len++) {

        c = *p;

        ଯଦି (c < 0x80) {
            p++;
            ଜାରି;
        }

        ଯଦି (ngx_utf8_decode(&p, last - p) > 0x10ffff) {
            /* invalid UTF-8 */
            ଫେରନ୍ତୁ n;
        }
    }

    ଫେରନ୍ତୁ len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len)
{
    u_char  c, *next;

    ଯଦି (n == 0) {
        ଫେରନ୍ତୁ dst;
    }

    ଯେତେବେଳେ (--n) {

        c = *src;
        *dst = c;

        ଯଦି (c < 0x80) {

            ଯଦି (c != '\0') {
                dst++;
                src++;
                len--;

                ଜାରି;
            }

            ଫେରନ୍ତୁ dst;
        }

        next = src;

        ଯଦି (ngx_utf8_decode(&next, len) > 0x10ffff) {
            /* invalid UTF-8 */
            ଭାଙ୍ଗ;
        }

        ଯେତେବେଳେ (src < next) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    ଫେରନ୍ତୁ dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, size_t size, ngx_uint_t type)
{
    u_char          prefix;
    uint32_t       *escape;
    ngx_uint_t      n;
    ସ୍ଥିର u_char   hex[] = "0123456789ABCDEF";

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

    ସ୍ଥିର uint32_t   uri[] = {
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

    ସ୍ଥିର uint32_t   args[] = {
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

    ସ୍ଥିର uint32_t   uri_component[] = {
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

    ସ୍ଥିର uint32_t   html[] = {
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

    ସ୍ଥିର uint32_t   refresh[] = {
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

    ସ୍ଥିର uint32_t   memcached[] = {
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

    ସ୍ଥିର uint32_t   mail_xtext[] = {
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

    ସ୍ଥିର uint32_t  *map[] =
        { uri, args, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    ସ୍ଥିର u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    prefix = map_char[type];

    ଯଦି (dst == ଖାଲି) {

        /* find the number of the characters to be escaped */

        n = 0;

        ଯେତେବେଳେ (size) {
            ଯଦି (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            size--;
        }

        ଫେରନ୍ତୁ (uintptr_t) n;
    }

    ଯେତେବେଳେ (size) {
        ଯଦି (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = prefix;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } ଅନ୍ୟଥା {
            *dst++ = *src++;
        }
        size--;
    }

    ଫେରନ୍ତୁ (uintptr_t) dst;
}


ଶୂନ୍ୟ
ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    ଗଣନା {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_second
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    ଯେତେବେଳେ (size--) {

        ch = *s++;

        ବାଛ (state) {
        ସ୍ଥିତି sw_usual:
            ଯଦି (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                ଯାଅ done;
            }

            ଯଦି (ch == '%') {
                state = sw_quoted;
                ଭାଙ୍ଗ;
            }

            *d++ = ch;
            ଭାଙ୍ଗ;

        ସ୍ଥିତି sw_quoted:

            ଯଦି (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_second;
                ଭାଙ୍ଗ;
            }

            c = (u_char) (ch | 0x20);
            ଯଦି (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_second;
                ଭାଙ୍ଗ;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            ଭାଙ୍ଗ;

        ସ୍ଥିତି sw_quoted_second:

            state = sw_usual;

            ଯଦି (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                ଯଦି (type & NGX_UNESCAPE_REDIRECT) {
                    ଯଦି (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        ଭାଙ୍ଗ;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    ଭାଙ୍ଗ;
                }

                *d++ = ch;

                ଭାଙ୍ଗ;
            }

            c = (u_char) (ch | 0x20);
            ଯଦି (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                ଯଦି (type & NGX_UNESCAPE_URI) {
                    ଯଦି (ch == '?') {
                        *d++ = ch;
                        ଯାଅ done;
                    }

                    *d++ = ch;
                    ଭାଙ୍ଗ;
                }

                ଯଦି (type & NGX_UNESCAPE_REDIRECT) {
                    ଯଦି (ch == '?') {
                        *d++ = ch;
                        ଯାଅ done;
                    }

                    ଯଦି (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        ଭାଙ୍ଗ;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    ଭାଙ୍ଗ;
                }

                *d++ = ch;

                ଭାଙ୍ଗ;
            }

            /* the invalid quoted character */

            ଭାଙ୍ଗ;
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

    ଯଦି (dst == ଖାଲି) {

        len = 0;

        ଯେତେବେଳେ (size) {
            ବାଛ (*src++) {

            ସ୍ଥିତି '<':
                len += ଆକାର("&lt;") - 2;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି '>':
                len += ଆକାର("&gt;") - 2;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି '&':
                len += ଆକାର("&amp;") - 2;
                ଭାଙ୍ଗ;

            ସ୍ଥିତି '"':
                len += ଆକାର("&quot;") - 2;
                ଭାଙ୍ଗ;

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                ଭାଙ୍ଗ;
            }
            size--;
        }

        ଫେରନ୍ତୁ (uintptr_t) len;
    }

    ଯେତେବେଳେ (size) {
        ch = *src++;

        ବାଛ (ch) {

        ସ୍ଥିତି '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            ଭାଙ୍ଗ;

        ସ୍ଥିତି '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            ଭାଙ୍ଗ;

        ସ୍ଥିତି '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            ଭାଙ୍ଗ;

        ସ୍ଥିତି '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            *dst++ = ch;
            ଭାଙ୍ଗ;
        }
        size--;
    }

    ଫେରନ୍ତୁ (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, size_t size)
{
    u_char      ch;
    ngx_uint_t  len;

    ଯଦି (dst == ଖାଲି) {
        len = 0;

        ଯେତେବେଳେ (size) {
            ch = *src++;

            ଯଦି (ch == '\\' || ch == '"') {
                len++;

            } ଅନ୍ୟଥା ଯଦି (ch <= 0x1f) {

                ବାଛ (ch) {
                ସ୍ଥିତି '\n':
                ସ୍ଥିତି '\r':
                ସ୍ଥିତି '\t':
                ସ୍ଥିତି '\b':
                ସ୍ଥିତି '\f':
                    len++;
                    ଭାଙ୍ଗ;

                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                    len += ଆକାର("\\u001F") - 2;
                }
            }

            size--;
        }

        ଫେରନ୍ତୁ (uintptr_t) len;
    }

    ଯେତେବେଳେ (size) {
        ch = *src++;

        ଯଦି (ch > 0x1f) {

            ଯଦି (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } ଅନ୍ୟଥା {
            *dst++ = '\\';

            ବାଛ (ch) {
            ସ୍ଥିତି '\n':
                *dst++ = 'n';
                ଭାଙ୍ଗ;

            ସ୍ଥିତି '\r':
                *dst++ = 'r';
                ଭାଙ୍ଗ;

            ସ୍ଥିତି '\t':
                *dst++ = 't';
                ଭାଙ୍ଗ;

            ସ୍ଥିତି '\b':
                *dst++ = 'b';
                ଭାଙ୍ଗ;

            ସ୍ଥିତି '\f':
                *dst++ = 'f';
                ଭାଙ୍ଗ;

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        size--;
    }

    ଫେରନ୍ତୁ (uintptr_t) dst;
}


ଶୂନ୍ୟ
ngx_str_rbtree_ଭର୍ତ୍ତି_ମୂଲ୍ୟ(ngx_rbtree_ଗଣ୍ଠି_t *temp,
    ngx_rbtree_ଗଣ୍ଠି_t *ଗଣ୍ଠି, ngx_rbtree_ଗଣ୍ଠି_t *sentinel)
{
    ngx_str_ଗଣ୍ଠି_t      *n, *t;
    ngx_rbtree_ଗଣ୍ଠି_t  **p;

    ପାଇଁ ( ;; ) {

        n = (ngx_str_ଗଣ୍ଠି_t *) ଗଣ୍ଠି;
        t = (ngx_str_ଗଣ୍ଠି_t *) temp;

        ଯଦି (ଗଣ୍ଠି->ଚାବି != temp->ଚାବି) {

            p = (ଗଣ୍ଠି->ଚାବି < temp->ଚାବି) ? &temp->ବାମ : &temp->ଡାହାଣ;

        } ଅନ୍ୟଥା ଯଦି (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &temp->ବାମ : &temp->ଡାହାଣ;

        } ଅନ୍ୟଥା {
            p = (ngx_memcmp(n->str.data, t->str.data, n->str.len) < 0)
                 ? &temp->ବାମ : &temp->ଡାହାଣ;
        }

        ଯଦି (*p == sentinel) {
            ଭାଙ୍ଗ;
        }

        temp = *p;
    }

    *p = ଗଣ୍ଠି;
    ଗଣ୍ଠି->parent = temp;
    ଗଣ୍ଠି->ବାମ = sentinel;
    ଗଣ୍ଠି->ଡାହାଣ = sentinel;
    ngx_rbt_red(ଗଣ୍ଠି);
}


ngx_str_ଗଣ୍ଠି_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_ଗଣ୍ଠି_t     *n;
    ngx_rbtree_ଗଣ୍ଠି_t  *ଗଣ୍ଠି, *sentinel;

    ଗଣ୍ଠି = rbtree->ମୂଳ;
    sentinel = rbtree->sentinel;

    ଯେତେବେଳେ (ଗଣ୍ଠି != sentinel) {

        n = (ngx_str_ଗଣ୍ଠି_t *) ଗଣ୍ଠି;

        ଯଦି (hash != ଗଣ୍ଠି->ଚାବି) {
            ଗଣ୍ଠି = (hash < ଗଣ୍ଠି->ଚାବି) ? ଗଣ୍ଠି->ବାମ : ଗଣ୍ଠି->ଡାହାଣ;
            ଜାରି;
        }

        ଯଦି (val->len != n->str.len) {
            ଗଣ୍ଠି = (val->len < n->str.len) ? ଗଣ୍ଠି->ବାମ : ଗଣ୍ଠି->ଡାହାଣ;
            ଜାରି;
        }

        rc = ngx_memcmp(val->data, n->str.data, val->len);

        ଯଦି (rc < 0) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ବାମ;
            ଜାରି;
        }

        ଯଦି (rc > 0) {
            ଗଣ୍ଠି = ଗଣ୍ଠି->ଡାହାଣ;
            ଜାରି;
        }

        ଫେରନ୍ତୁ n;
    }

    ଫେରନ୍ତୁ ଖାଲି;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

ଶୂନ୍ୟ
ngx_sort(ଶୂନ୍ୟ *base, size_t n, size_t size,
    ngx_int_t (*cmp)(ଅଚଳ ଶୂନ୍ୟ *, ଅଚଳ ଶୂନ୍ୟ *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(size, ngx_cycle->log);
    ଯଦି (p == ଖାଲି) {
        ଫେରନ୍ତୁ;
    }

    ପାଇଁ (p1 = (u_char *) base + size;
         p1 < (u_char *) base + n * size;
         p1 += size)
    {
        ngx_memcpy(p, p1, size);

        ପାଇଁ (p2 = p1;
             p2 > (u_char *) base && cmp(p2 - size, p) > 0;
             p2 -= size)
        {
            ngx_memcpy(p2, p2 - size, size);
        }

        ngx_memcpy(p2, p, size);
    }

    ngx_free(p);
}


ଶୂନ୍ୟ
ngx_explicit_memzero(ଶୂନ୍ୟ *buf, size_t n)
{
    ngx_memzero(buf, n);
    ngx_memory_barrier();
}


#ଯଦି (NGX_MEMCPY_LIMIT)

ଶୂନ୍ୟ *
ngx_memcpy(ଶୂନ୍ୟ *dst, ଅଚଳ ଶୂନ୍ୟ *src, size_t n)
{
    ଯଦି (n > NGX_MEMCPY_LIMIT) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "memcpy %uz bytes", n);
        ngx_debug_point();
    }

    ଫେରନ୍ତୁ memcpy(dst, src, n);
}

#endif
