
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਸਥਿਰ u_char *ngx_sprintf_ਅੰਕ(u_char *buf, u_char *ਆਖਰੀ, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t ਚੌੜਾਈ);
ਸਥਿਰ u_char *ngx_sprintf_str(u_char *buf, u_char *ਆਖਰੀ, u_char *src,
    ਮਾਪ_t len, ngx_uint_t hexadecimal);
ਸਥਿਰ ਖਾਲੀ ngx_encode_base64_ਅੰਦਰੂਨੀ(ngx_str_t *dst, ngx_str_t *src,
    ਅਚਲ u_char *basis, ngx_uint_t padding);
ਸਥਿਰ ngx_int_t ngx_decode_base64_ਅੰਦਰੂਨੀ(ngx_str_t *dst, ngx_str_t *src,
    ਅਚਲ u_char *basis);


ਖਾਲੀ
ngx_strlow(u_char *dst, u_char *src, ਮਾਪ_t n)
{
    ਜਦੋਂ (n) {
        *dst = ngx_tolower(*src);
        dst++;
        src++;
        n--;
    }
}


ਮਾਪ_t
ngx_strnlen(u_char *p, ਮਾਪ_t n)
{
    ਮਾਪ_t  i;

    ਲਈ (i = 0; i < n; i++) {

        ਜੇ (p[i] == '\0') {
            ਵਾਪਸ i;
        }
    }

    ਵਾਪਸ n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, ਮਾਪ_t n)
{
    ਜੇ (n == 0) {
        ਵਾਪਸ dst;
    }

    ਜਦੋਂ (--n) {
        *dst = *src;

        ਜੇ (*dst == '\0') {
            ਵਾਪਸ dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    ਵਾਪਸ dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    ਜੇ (dst == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ngx_memcpy(dst, src->ਅੰਕੜੇ, src->len);

    ਵਾਪਸ dst;
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
ngx_sprintf(u_char *buf, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
    u_char   *p;
    va_list   ਦਲੀਲਾਂ;

    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
    p = ngx_vslprintf(buf, (ਖਾਲੀ *) -1, fmt, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);

    ਵਾਪਸ p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, ਮਾਪ_t max, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
    u_char   *p;
    va_list   ਦਲੀਲਾਂ;

    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);

    ਵਾਪਸ p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *ਆਖਰੀ, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
    u_char   *p;
    va_list   ਦਲੀਲਾਂ;

    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
    p = ngx_vslprintf(buf, ਆਖਰੀ, fmt, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);

    ਵਾਪਸ p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *ਆਖਰੀ, ਅਚਲ ਅੱਖਰ *fmt, va_list ਦਲੀਲਾਂ)
{
    u_char                *p, zero;
    ਪੂਰਨ                    d;
    ਦੁੱਗਣਾ                 f;
    ਮਾਪ_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             ਚੌੜਾਈ, sign, hex, max_ਚੌੜਾਈ, frac_ਚੌੜਾਈ, scale, n;
    ngx_str_t             *v;
    ngx_variable_ਮੁੱਲ_t  *vv;

    ਜਦੋਂ (*fmt && buf < ਆਖਰੀ) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        ਜੇ (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            ਚੌੜਾਈ = 0;
            sign = 1;
            hex = 0;
            max_ਚੌੜਾਈ = 0;
            frac_ਚੌੜਾਈ = 0;
            slen = (ਮਾਪ_t) -1;

            ਜਦੋਂ (*fmt >= '0' && *fmt <= '9') {
                ਚੌੜਾਈ = ਚੌੜਾਈ * 10 + (*fmt++ - '0');
            }


            ਲਈ ( ;; ) {
                ਚੋਣ (*fmt) {

                ਹਾਲਤ 'u':
                    sign = 0;
                    fmt++;
                    ਜਾਰੀ;

                ਹਾਲਤ 'm':
                    max_ਚੌੜਾਈ = 1;
                    fmt++;
                    ਜਾਰੀ;

                ਹਾਲਤ 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    ਜਾਰੀ;

                ਹਾਲਤ 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    ਜਾਰੀ;

                ਹਾਲਤ '.':
                    fmt++;

                    ਜਦੋਂ (*fmt >= '0' && *fmt <= '9') {
                        frac_ਚੌੜਾਈ = frac_ਚੌੜਾਈ * 10 + (*fmt++ - '0');
                    }

                    ਰੋਕੋ;

                ਹਾਲਤ '*':
                    slen = va_arg(ਦਲੀਲਾਂ, ਮਾਪ_t);
                    fmt++;
                    ਜਾਰੀ;

                ਮੂਲ:
                    ਰੋਕੋ;
                }

                ਰੋਕੋ;
            }


            ਚੋਣ (*fmt) {

            ਹਾਲਤ 'V':
                v = va_arg(ਦਲੀਲਾਂ, ngx_str_t *);

                buf = ngx_sprintf_str(buf, ਆਖਰੀ, v->ਅੰਕੜੇ, v->len, hex);
                fmt++;

                ਜਾਰੀ;

            ਹਾਲਤ 'v':
                vv = va_arg(ਦਲੀਲਾਂ, ngx_variable_ਮੁੱਲ_t *);

                buf = ngx_sprintf_str(buf, ਆਖਰੀ, vv->ਅੰਕੜੇ, vv->len, hex);
                fmt++;

                ਜਾਰੀ;

            ਹਾਲਤ 's':
                p = va_arg(ਦਲੀਲਾਂ, u_char *);

                buf = ngx_sprintf_str(buf, ਆਖਰੀ, p, slen, hex);
                fmt++;

                ਜਾਰੀ;

            ਹਾਲਤ 'O':
                i64 = (int64_t) va_arg(ਦਲੀਲਾਂ, off_t);
                sign = 1;
                ਰੋਕੋ;

            ਹਾਲਤ 'P':
                i64 = (int64_t) va_arg(ਦਲੀਲਾਂ, ngx_pid_t);
                sign = 1;
                ਰੋਕੋ;

            ਹਾਲਤ 'T':
                i64 = (int64_t) va_arg(ਦਲੀਲਾਂ, ਸਮਾਂ_t);
                sign = 1;
                ਰੋਕੋ;

            ਹਾਲਤ 'M':
                ms = (ngx_msec_t) va_arg(ਦਲੀਲਾਂ, ngx_msec_t);
                ਜੇ ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } ਨਹੀਂਤਾਂ {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                ਰੋਕੋ;

            ਹਾਲਤ 'z':
                ਜੇ (sign) {
                    i64 = (int64_t) va_arg(ਦਲੀਲਾਂ, ssize_t);
                } ਨਹੀਂਤਾਂ {
                    ui64 = (uint64_t) va_arg(ਦਲੀਲਾਂ, ਮਾਪ_t);
                }
                ਰੋਕੋ;

            ਹਾਲਤ 'i':
                ਜੇ (sign) {
                    i64 = (int64_t) va_arg(ਦਲੀਲਾਂ, ngx_int_t);
                } ਨਹੀਂਤਾਂ {
                    ui64 = (uint64_t) va_arg(ਦਲੀਲਾਂ, ngx_uint_t);
                }

                ਜੇ (max_ਚੌੜਾਈ) {
                    ਚੌੜਾਈ = NGX_INT_T_LEN;
                }

                ਰੋਕੋ;

            ਹਾਲਤ 'd':
                ਜੇ (sign) {
                    i64 = (int64_t) va_arg(ਦਲੀਲਾਂ, ਪੂਰਨ);
                } ਨਹੀਂਤਾਂ {
                    ui64 = (uint64_t) va_arg(ਦਲੀਲਾਂ, u_int);
                }
                ਰੋਕੋ;

            ਹਾਲਤ 'l':
                ਜੇ (sign) {
                    i64 = (int64_t) va_arg(ਦਲੀਲਾਂ, ਲੰਮਾ);
                } ਨਹੀਂਤਾਂ {
                    ui64 = (uint64_t) va_arg(ਦਲੀਲਾਂ, u_long);
                }
                ਰੋਕੋ;

            ਹਾਲਤ 'D':
                ਜੇ (sign) {
                    i64 = (int64_t) va_arg(ਦਲੀਲਾਂ, int32_t);
                } ਨਹੀਂਤਾਂ {
                    ui64 = (uint64_t) va_arg(ਦਲੀਲਾਂ, uint32_t);
                }
                ਰੋਕੋ;

            ਹਾਲਤ 'L':
                ਜੇ (sign) {
                    i64 = va_arg(ਦਲੀਲਾਂ, int64_t);
                } ਨਹੀਂਤਾਂ {
                    ui64 = va_arg(ਦਲੀਲਾਂ, uint64_t);
                }
                ਰੋਕੋ;

            ਹਾਲਤ 'A':
                ਜੇ (sign) {
                    i64 = (int64_t) va_arg(ਦਲੀਲਾਂ, ngx_atomic_int_t);
                } ਨਹੀਂਤਾਂ {
                    ui64 = (uint64_t) va_arg(ਦਲੀਲਾਂ, ngx_atomic_uint_t);
                }

                ਜੇ (max_ਚੌੜਾਈ) {
                    ਚੌੜਾਈ = NGX_ATOMIC_T_LEN;
                }

                ਰੋਕੋ;

            ਹਾਲਤ 'f':
                f = va_arg(ਦਲੀਲਾਂ, ਦੁੱਗਣਾ);

                ਜੇ (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                ਜੇ (frac_ਚੌੜਾਈ) {

                    scale = 1;
                    ਲਈ (n = frac_ਚੌੜਾਈ; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (ਦੁੱਗਣਾ) ui64) * scale + 0.5);

                    ਜੇ (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_ਅੰਕ(buf, ਆਖਰੀ, ui64, zero, 0, ਚੌੜਾਈ);

                ਜੇ (frac_ਚੌੜਾਈ) {
                    ਜੇ (buf < ਆਖਰੀ) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_ਅੰਕ(buf, ਆਖਰੀ, frac, '0', 0, frac_ਚੌੜਾਈ);
                }

                fmt++;

                ਜਾਰੀ;

#ਜੇ !(NGX_WIN32)
            ਹਾਲਤ 'r':
                i64 = (int64_t) va_arg(ਦਲੀਲਾਂ, rlim_t);
                sign = 1;
                ਰੋਕੋ;
#ਜੇ_ਅੰਤ

            ਹਾਲਤ 'p':
                ui64 = (uintptr_t) va_arg(ਦਲੀਲਾਂ, ਖਾਲੀ *);
                hex = 2;
                sign = 0;
                zero = '0';
                ਚੌੜਾਈ = 2 * ਆਕਾਰ(ਖਾਲੀ *);
                ਰੋਕੋ;

            ਹਾਲਤ 'c':
                d = va_arg(ਦਲੀਲਾਂ, ਪੂਰਨ);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                ਜਾਰੀ;

            ਹਾਲਤ 'Z':
                *buf++ = '\0';
                fmt++;

                ਜਾਰੀ;

            ਹਾਲਤ 'N':
#ਜੇ (NGX_WIN32)
                *buf++ = CR;
                ਜੇ (buf < ਆਖਰੀ) {
                    *buf++ = LF;
                }
#ਨਹੀਂਤਾਂ
                *buf++ = LF;
#ਜੇ_ਅੰਤ
                fmt++;

                ਜਾਰੀ;

            ਹਾਲਤ '%':
                *buf++ = '%';
                fmt++;

                ਜਾਰੀ;

            ਮੂਲ:
                *buf++ = *fmt++;

                ਜਾਰੀ;
            }

            ਜੇ (sign) {
                ਜੇ (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } ਨਹੀਂਤਾਂ {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_ਅੰਕ(buf, ਆਖਰੀ, ui64, zero, hex, ਚੌੜਾਈ);

            fmt++;

        } ਨਹੀਂਤਾਂ {
            *buf++ = *fmt++;
        }
    }

    ਵਾਪਸ buf;
}


ਸਥਿਰ u_char *
ngx_sprintf_ਅੰਕ(u_char *buf, u_char *ਆਖਰੀ, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t ਚੌੜਾਈ)
{
    u_char         *p, ਆਰਜ਼ੀ[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    ਮਾਪ_t          len;
    uint32_t        ui32;
    ਸਥਿਰ u_char   hex[] = "0123456789abcdef";
    ਸਥਿਰ u_char   HEX[] = "0123456789ABCDEF";

    p = ਆਰਜ਼ੀ + NGX_INT64_LEN;

    ਜੇ (hexadecimal == 0) {

        ਜੇ (ui64 <= (uint64_t) NGX_MAX_UINT32_ਮੁੱਲ) {

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

            ਕਰੋ {
                *--p = (u_char) (ui32 % 10 + '0');
            } ਜਦੋਂ (ui32 /= 10);

        } ਨਹੀਂਤਾਂ {
            ਕਰੋ {
                *--p = (u_char) (ui64 % 10 + '0');
            } ਜਦੋਂ (ui64 /= 10);
        }

    } ਨਹੀਂਤਾਂ ਜੇ (hexadecimal == 1) {

        ਕਰੋ {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } ਜਦੋਂ (ui64 >>= 4);

    } ਨਹੀਂਤਾਂ { /* hexadecimal == 2 */

        ਕਰੋ {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } ਜਦੋਂ (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (ਆਰਜ਼ੀ + NGX_INT64_LEN) - p;

    ਜਦੋਂ (len++ < ਚੌੜਾਈ && buf < ਆਖਰੀ) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (ਆਰਜ਼ੀ + NGX_INT64_LEN) - p;

    ਜੇ (buf + len > ਆਖਰੀ) {
        len = ਆਖਰੀ - buf;
    }

    ਵਾਪਸ ngx_cpymem(buf, p, len);
}


ਸਥਿਰ u_char *
ngx_sprintf_str(u_char *buf, u_char *ਆਖਰੀ, u_char *src, ਮਾਪ_t len,
    ngx_uint_t hexadecimal)
{
    ਸਥਿਰ u_char   hex[] = "0123456789abcdef";
    ਸਥਿਰ u_char   HEX[] = "0123456789ABCDEF";

    ਜੇ (hexadecimal == 0) {

        ਜੇ (len == (ਮਾਪ_t) -1) {
            ਜਦੋਂ (*src && buf < ਆਖਰੀ) {
                *buf++ = *src++;
            }

        } ਨਹੀਂਤਾਂ {
            len = ngx_min((ਮਾਪ_t) (ਆਖਰੀ - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } ਨਹੀਂਤਾਂ ਜੇ (hexadecimal == 1) {

        ਜੇ (len == (ਮਾਪ_t) -1) {

            ਜਦੋਂ (*src && buf < ਆਖਰੀ - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } ਨਹੀਂਤਾਂ {

            ਜਦੋਂ (len-- && buf < ਆਖਰੀ - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } ਨਹੀਂਤਾਂ { /* hexadecimal == 2 */

        ਜੇ (len == (ਮਾਪ_t) -1) {

            ਜਦੋਂ (*src && buf < ਆਖਰੀ - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } ਨਹੀਂਤਾਂ {

            ਜਦੋਂ (len-- && buf < ਆਖਰੀ - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    ਵਾਪਸ buf;
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

    ਲਈ ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ਜੇ (c1 == c2) {

            ਜੇ (c1) {
                ਜਾਰੀ;
            }

            ਵਾਪਸ 0;
        }

        ਵਾਪਸ c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, ਮਾਪ_t n)
{
    ngx_uint_t  c1, c2;

    ਜਦੋਂ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ਜੇ (c1 == c2) {

            ਜੇ (c1) {
                n--;
                ਜਾਰੀ;
            }

            ਵਾਪਸ 0;
        }

        ਵਾਪਸ c1 - c2;
    }

    ਵਾਪਸ 0;
}


u_char *
ngx_strnstr(u_char *s1, ਅੱਖਰ *s2, ਮਾਪ_t len)
{
    u_char  c1, c2;
    ਮਾਪ_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    ਕਰੋ {
        ਕਰੋ {
            ਜੇ (len-- == 0) {
                ਵਾਪਸ ਨਿਰਾ;
            }

            c1 = *s1++;

            ਜੇ (c1 == 0) {
                ਵਾਪਸ ਨਿਰਾ;
            }

        } ਜਦੋਂ (c1 != c2);

        ਜੇ (n > len) {
            ਵਾਪਸ ਨਿਰਾ;
        }

    } ਜਦੋਂ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ਵਾਪਸ --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, ਅੱਖਰ *s2, ਮਾਪ_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    ਕਰੋ {
        ਕਰੋ {
            c1 = *s1++;

            ਜੇ (c1 == 0) {
                ਵਾਪਸ ਨਿਰਾ;
            }

        } ਜਦੋਂ (c1 != c2);

    } ਜਦੋਂ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ਵਾਪਸ --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, ਅੱਖਰ *s2, ਮਾਪ_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    ਕਰੋ {
        ਕਰੋ {
            c1 = (ngx_uint_t) *s1++;

            ਜੇ (c1 == 0) {
                ਵਾਪਸ ਨਿਰਾ;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ਜਦੋਂ (c1 != c2);

    } ਜਦੋਂ (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    ਵਾਪਸ --s1;
}


/*
 * ngx_strlcasestrn() is intended to search for static substring
 * with known length in string until the argument last. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strlcasestrn(u_char *s1, u_char *ਆਖਰੀ, u_char *s2, ਮਾਪ_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;
    ਆਖਰੀ -= n;

    ਕਰੋ {
        ਕਰੋ {
            ਜੇ (s1 >= ਆਖਰੀ) {
                ਵਾਪਸ ਨਿਰਾ;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ਜਦੋਂ (c1 != c2);

    } ਜਦੋਂ (ngx_strncasecmp(s1, s2, n) != 0);

    ਵਾਪਸ --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, ਮਾਪ_t n)
{
    ਜੇ (n == 0) {
        ਵਾਪਸ 0;
    }

    n--;

    ਲਈ ( ;; ) {
        ਜੇ (s1[n] != s2[n]) {
            ਵਾਪਸ s1[n] - s2[n];
        }

        ਜੇ (n == 0) {
            ਵਾਪਸ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, ਮਾਪ_t n)
{
    u_char  c1, c2;

    ਜੇ (n == 0) {
        ਵਾਪਸ 0;
    }

    n--;

    ਲਈ ( ;; ) {
        c1 = s1[n];
        ਜੇ (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        ਜੇ (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        ਜੇ (c1 != c2) {
            ਵਾਪਸ c1 - c2;
        }

        ਜੇ (n == 0) {
            ਵਾਪਸ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, ਮਾਪ_t n1, ਮਾਪ_t n2)
{
    ਮਾਪ_t     n;
    ngx_int_t  m, z;

    ਜੇ (n1 <= n2) {
        n = n1;
        z = -1;

    } ਨਹੀਂਤਾਂ {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    ਜੇ (m || n1 == n2) {
        ਵਾਪਸ m;
    }

    ਵਾਪਸ z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    ਲਈ ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ਜੇ (c1 == c2) {

            ਜੇ (c1) {
                ਜਾਰੀ;
            }

            ਵਾਪਸ 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        ਵਾਪਸ c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, ਮਾਪ_t n)
{
    ngx_uint_t  c1, c2;

    ਜਦੋਂ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#ਜੇ (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#ਜੇ_ਅੰਤ

        ਜੇ (c1 == c2) {

            ਜੇ (c1) {
                n--;
                ਜਾਰੀ;
            }

            ਵਾਪਸ 0;
        }

        /* we need '/' to be the lowest character */

        ਜੇ (c1 == 0 || c2 == 0) {
            ਵਾਪਸ c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        ਵਾਪਸ c1 - c2;
    }

    ਵਾਪਸ 0;
}


ngx_int_t
ngx_atoi(u_char *ਸਤਰ, ਮਾਪ_t n)
{
    ngx_int_t  ਮੁੱਲ, cutoff, cutlim;

    ਜੇ (n == 0) {
        ਵਾਪਸ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_ਮੁੱਲ / 10;
    cutlim = NGX_MAX_INT_T_ਮੁੱਲ % 10;

    ਲਈ (ਮੁੱਲ = 0; n--; ਸਤਰ++) {
        ਜੇ (*ਸਤਰ < '0' || *ਸਤਰ > '9') {
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (ਮੁੱਲ >= cutoff && (ਮੁੱਲ > cutoff || *ਸਤਰ - '0' > cutlim)) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਮੁੱਲ = ਮੁੱਲ * 10 + (*ਸਤਰ - '0');
    }

    ਵਾਪਸ ਮੁੱਲ;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *ਸਤਰ, ਮਾਪ_t n, ਮਾਪ_t ਬਿੰਦੂ)
{
    ngx_int_t   ਮੁੱਲ, cutoff, cutlim;
    ngx_uint_t  dot;

    ਜੇ (n == 0) {
        ਵਾਪਸ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_ਮੁੱਲ / 10;
    cutlim = NGX_MAX_INT_T_ਮੁੱਲ % 10;

    dot = 0;

    ਲਈ (ਮੁੱਲ = 0; n--; ਸਤਰ++) {

        ਜੇ (ਬਿੰਦੂ == 0) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (*ਸਤਰ == '.') {
            ਜੇ (dot) {
                ਵਾਪਸ NGX_ERROR;
            }

            dot = 1;
            ਜਾਰੀ;
        }

        ਜੇ (*ਸਤਰ < '0' || *ਸਤਰ > '9') {
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (ਮੁੱਲ >= cutoff && (ਮੁੱਲ > cutoff || *ਸਤਰ - '0' > cutlim)) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਮੁੱਲ = ਮੁੱਲ * 10 + (*ਸਤਰ - '0');
        ਬਿੰਦੂ -= dot;
    }

    ਜਦੋਂ (ਬਿੰਦੂ--) {
        ਜੇ (ਮੁੱਲ > cutoff) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਮੁੱਲ = ਮੁੱਲ * 10;
    }

    ਵਾਪਸ ਮੁੱਲ;
}


ssize_t
ngx_atosz(u_char *ਸਤਰ, ਮਾਪ_t n)
{
    ssize_t  ਮੁੱਲ, cutoff, cutlim;

    ਜੇ (n == 0) {
        ਵਾਪਸ NGX_ERROR;
    }

    cutoff = NGX_MAX_ਮਾਪ_T_ਮੁੱਲ / 10;
    cutlim = NGX_MAX_ਮਾਪ_T_ਮੁੱਲ % 10;

    ਲਈ (ਮੁੱਲ = 0; n--; ਸਤਰ++) {
        ਜੇ (*ਸਤਰ < '0' || *ਸਤਰ > '9') {
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (ਮੁੱਲ >= cutoff && (ਮੁੱਲ > cutoff || *ਸਤਰ - '0' > cutlim)) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਮੁੱਲ = ਮੁੱਲ * 10 + (*ਸਤਰ - '0');
    }

    ਵਾਪਸ ਮੁੱਲ;
}


off_t
ngx_atoof(u_char *ਸਤਰ, ਮਾਪ_t n)
{
    off_t  ਮੁੱਲ, cutoff, cutlim;

    ਜੇ (n == 0) {
        ਵਾਪਸ NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_ਮੁੱਲ / 10;
    cutlim = NGX_MAX_OFF_T_ਮੁੱਲ % 10;

    ਲਈ (ਮੁੱਲ = 0; n--; ਸਤਰ++) {
        ਜੇ (*ਸਤਰ < '0' || *ਸਤਰ > '9') {
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (ਮੁੱਲ >= cutoff && (ਮੁੱਲ > cutoff || *ਸਤਰ - '0' > cutlim)) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਮੁੱਲ = ਮੁੱਲ * 10 + (*ਸਤਰ - '0');
    }

    ਵਾਪਸ ਮੁੱਲ;
}


ਸਮਾਂ_t
ngx_atotm(u_char *ਸਤਰ, ਮਾਪ_t n)
{
    ਸਮਾਂ_t  ਮੁੱਲ, cutoff, cutlim;

    ਜੇ (n == 0) {
        ਵਾਪਸ NGX_ERROR;
    }

    cutoff = NGX_MAX_ਸਮਾਂ_T_ਮੁੱਲ / 10;
    cutlim = NGX_MAX_ਸਮਾਂ_T_ਮੁੱਲ % 10;

    ਲਈ (ਮੁੱਲ = 0; n--; ਸਤਰ++) {
        ਜੇ (*ਸਤਰ < '0' || *ਸਤਰ > '9') {
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (ਮੁੱਲ >= cutoff && (ਮੁੱਲ > cutoff || *ਸਤਰ - '0' > cutlim)) {
            ਵਾਪਸ NGX_ERROR;
        }

        ਮੁੱਲ = ਮੁੱਲ * 10 + (*ਸਤਰ - '0');
    }

    ਵਾਪਸ ਮੁੱਲ;
}


ngx_int_t
ngx_hextoi(u_char *ਸਤਰ, ਮਾਪ_t n)
{
    u_char     c, ch;
    ngx_int_t  ਮੁੱਲ, cutoff;

    ਜੇ (n == 0) {
        ਵਾਪਸ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_ਮੁੱਲ / 16;

    ਲਈ (ਮੁੱਲ = 0; n--; ਸਤਰ++) {
        ਜੇ (ਮੁੱਲ > cutoff) {
            ਵਾਪਸ NGX_ERROR;
        }

        ch = *ਸਤਰ;

        ਜੇ (ch >= '0' && ch <= '9') {
            ਮੁੱਲ = ਮੁੱਲ * 16 + (ch - '0');
            ਜਾਰੀ;
        }

        c = (u_char) (ch | 0x20);

        ਜੇ (c >= 'a' && c <= 'f') {
            ਮੁੱਲ = ਮੁੱਲ * 16 + (c - 'a' + 10);
            ਜਾਰੀ;
        }

        ਵਾਪਸ NGX_ERROR;
    }

    ਵਾਪਸ ਮੁੱਲ;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, ਮਾਪ_t len)
{
    ਸਥਿਰ u_char  hex[] = "0123456789abcdef";

    ਜਦੋਂ (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    ਵਾਪਸ dst;
}


ਖਾਲੀ
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    ਸਥਿਰ u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_ਅੰਦਰੂਨੀ(dst, src, basis64, 1);
}


ਖਾਲੀ
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    ਸਥਿਰ u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_ਅੰਦਰੂਨੀ(dst, src, basis64, 0);
}


ਸਥਿਰ ਖਾਲੀ
ngx_encode_base64_ਅੰਦਰੂਨੀ(ngx_str_t *dst, ngx_str_t *src, ਅਚਲ u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    ਮਾਪ_t          len;

    len = src->len;
    s = src->ਅੰਕੜੇ;
    d = dst->ਅੰਕੜੇ;

    ਜਦੋਂ (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    ਜੇ (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        ਜੇ (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            ਜੇ (padding) {
                *d++ = '=';
            }

        } ਨਹੀਂਤਾਂ {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        ਜੇ (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->ਅੰਕੜੇ;
}


ngx_int_t
ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    ਸਥਿਰ u_char   basis64[] = {
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

    ਵਾਪਸ ngx_decode_base64_ਅੰਦਰੂਨੀ(dst, src, basis64);
}


ngx_int_t
ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    ਸਥਿਰ u_char   basis64[] = {
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

    ਵਾਪਸ ngx_decode_base64_ਅੰਦਰੂਨੀ(dst, src, basis64);
}


ਸਥਿਰ ngx_int_t
ngx_decode_base64_ਅੰਦਰੂਨੀ(ngx_str_t *dst, ngx_str_t *src, ਅਚਲ u_char *basis)
{
    ਮਾਪ_t          len;
    u_char         *d, *s;

    ਲਈ (len = 0; len < src->len; len++) {
        ਜੇ (src->ਅੰਕੜੇ[len] == '=') {
            ਰੋਕੋ;
        }

        ਜੇ (basis[src->ਅੰਕੜੇ[len]] == 77) {
            ਵਾਪਸ NGX_ERROR;
        }
    }

    ਜੇ (len % 4 == 1) {
        ਵਾਪਸ NGX_ERROR;
    }

    s = src->ਅੰਕੜੇ;
    d = dst->ਅੰਕੜੇ;

    ਜਦੋਂ (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    ਜੇ (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    ਜੇ (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->ਅੰਕੜੇ;

    ਵਾਪਸ NGX_OK;
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
ngx_utf8_decode(u_char **p, ਮਾਪ_t n)
{
    ਮਾਪ_t    len;
    uint32_t  u, i, valid;

    u = **p;

    ਜੇ (u >= 0xf8) {

        (*p)++;
        ਵਾਪਸ 0xffffffff;

    } ਨਹੀਂਤਾਂ ਜੇ (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } ਨਹੀਂਤਾਂ ਜੇ (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } ਨਹੀਂਤਾਂ ਜੇ (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } ਨਹੀਂਤਾਂ {
        (*p)++;
        ਵਾਪਸ 0xffffffff;
    }

    ਜੇ (n - 1 < len) {
        ਵਾਪਸ 0xfffffffe;
    }

    (*p)++;

    ਜਦੋਂ (len) {
        i = *(*p)++;

        ਜੇ (i < 0x80) {
            ਵਾਪਸ 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    ਜੇ (u > valid) {
        ਵਾਪਸ u;
    }

    ਵਾਪਸ 0xffffffff;
}


ਮਾਪ_t
ngx_utf8_ਲੰਮਾਈ(u_char *p, ਮਾਪ_t n)
{
    u_char  c, *ਆਖਰੀ;
    ਮਾਪ_t  len;

    ਆਖਰੀ = p + n;

    ਲਈ (len = 0; p < ਆਖਰੀ; len++) {

        c = *p;

        ਜੇ (c < 0x80) {
            p++;
            ਜਾਰੀ;
        }

        ਜੇ (ngx_utf8_decode(&p, ਆਖਰੀ - p) > 0x10ffff) {
            /* invalid UTF-8 */
            ਵਾਪਸ n;
        }
    }

    ਵਾਪਸ len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, ਮਾਪ_t n, ਮਾਪ_t len)
{
    u_char  c, *ਅਗਲਾ;

    ਜੇ (n == 0) {
        ਵਾਪਸ dst;
    }

    ਜਦੋਂ (--n) {

        c = *src;
        *dst = c;

        ਜੇ (c < 0x80) {

            ਜੇ (c != '\0') {
                dst++;
                src++;
                len--;

                ਜਾਰੀ;
            }

            ਵਾਪਸ dst;
        }

        ਅਗਲਾ = src;

        ਜੇ (ngx_utf8_decode(&ਅਗਲਾ, len) > 0x10ffff) {
            /* invalid UTF-8 */
            ਰੋਕੋ;
        }

        ਜਦੋਂ (src < ਅਗਲਾ) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    ਵਾਪਸ dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, ਮਾਪ_t ਮਾਪ, ngx_uint_t type)
{
    u_char          ਅਗੇਤਰ;
    uint32_t       *escape;
    ngx_uint_t      n;
    ਸਥਿਰ u_char   hex[] = "0123456789ABCDEF";

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

    ਸਥਿਰ uint32_t   uri[] = {
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

    ਸਥਿਰ uint32_t   ਦਲੀਲਾਂ[] = {
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

    ਸਥਿਰ uint32_t   uri_component[] = {
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

    ਸਥਿਰ uint32_t   html[] = {
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

    ਸਥਿਰ uint32_t   refresh[] = {
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

    ਸਥਿਰ uint32_t   memcached[] = {
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

    ਸਥਿਰ uint32_t   mail_xtext[] = {
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

    ਸਥਿਰ uint32_t  *map[] =
        { uri, ਦਲੀਲਾਂ, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    ਸਥਿਰ u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    ਅਗੇਤਰ = map_char[type];

    ਜੇ (dst == ਨਿਰਾ) {

        /* find the number of the characters to be escaped */

        n = 0;

        ਜਦੋਂ (ਮਾਪ) {
            ਜੇ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            ਮਾਪ--;
        }

        ਵਾਪਸ (uintptr_t) n;
    }

    ਜਦੋਂ (ਮਾਪ) {
        ਜੇ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = ਅਗੇਤਰ;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } ਨਹੀਂਤਾਂ {
            *dst++ = *src++;
        }
        ਮਾਪ--;
    }

    ਵਾਪਸ (uintptr_t) dst;
}


ਖਾਲੀ
ngx_unescape_uri(u_char **dst, u_char **src, ਮਾਪ_t ਮਾਪ, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    ਗਿਣਤੀ {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_ਦੂਜਾ
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    ਜਦੋਂ (ਮਾਪ--) {

        ch = *s++;

        ਚੋਣ (state) {
        ਹਾਲਤ sw_usual:
            ਜੇ (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                ਜਾਓ ਪੂਰਾ;
            }

            ਜੇ (ch == '%') {
                state = sw_quoted;
                ਰੋਕੋ;
            }

            *d++ = ch;
            ਰੋਕੋ;

        ਹਾਲਤ sw_quoted:

            ਜੇ (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_ਦੂਜਾ;
                ਰੋਕੋ;
            }

            c = (u_char) (ch | 0x20);
            ਜੇ (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_ਦੂਜਾ;
                ਰੋਕੋ;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            ਰੋਕੋ;

        ਹਾਲਤ sw_quoted_ਦੂਜਾ:

            state = sw_usual;

            ਜੇ (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                ਜੇ (type & NGX_UNESCAPE_REDIRECT) {
                    ਜੇ (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        ਰੋਕੋ;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    ਰੋਕੋ;
                }

                *d++ = ch;

                ਰੋਕੋ;
            }

            c = (u_char) (ch | 0x20);
            ਜੇ (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                ਜੇ (type & NGX_UNESCAPE_URI) {
                    ਜੇ (ch == '?') {
                        *d++ = ch;
                        ਜਾਓ ਪੂਰਾ;
                    }

                    *d++ = ch;
                    ਰੋਕੋ;
                }

                ਜੇ (type & NGX_UNESCAPE_REDIRECT) {
                    ਜੇ (ch == '?') {
                        *d++ = ch;
                        ਜਾਓ ਪੂਰਾ;
                    }

                    ਜੇ (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        ਰੋਕੋ;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    ਰੋਕੋ;
                }

                *d++ = ch;

                ਰੋਕੋ;
            }

            /* the invalid quoted character */

            ਰੋਕੋ;
        }
    }

ਪੂਰਾ:

    *dst = d;
    *src = s;
}


uintptr_t
ngx_escape_html(u_char *dst, u_char *src, ਮਾਪ_t ਮਾਪ)
{
    u_char      ch;
    ngx_uint_t  len;

    ਜੇ (dst == ਨਿਰਾ) {

        len = 0;

        ਜਦੋਂ (ਮਾਪ) {
            ਚੋਣ (*src++) {

            ਹਾਲਤ '<':
                len += ਆਕਾਰ("&lt;") - 2;
                ਰੋਕੋ;

            ਹਾਲਤ '>':
                len += ਆਕਾਰ("&gt;") - 2;
                ਰੋਕੋ;

            ਹਾਲਤ '&':
                len += ਆਕਾਰ("&amp;") - 2;
                ਰੋਕੋ;

            ਹਾਲਤ '"':
                len += ਆਕਾਰ("&quot;") - 2;
                ਰੋਕੋ;

            ਮੂਲ:
                ਰੋਕੋ;
            }
            ਮਾਪ--;
        }

        ਵਾਪਸ (uintptr_t) len;
    }

    ਜਦੋਂ (ਮਾਪ) {
        ch = *src++;

        ਚੋਣ (ch) {

        ਹਾਲਤ '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            ਰੋਕੋ;

        ਹਾਲਤ '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            ਰੋਕੋ;

        ਹਾਲਤ '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            ਰੋਕੋ;

        ਹਾਲਤ '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            ਰੋਕੋ;

        ਮੂਲ:
            *dst++ = ch;
            ਰੋਕੋ;
        }
        ਮਾਪ--;
    }

    ਵਾਪਸ (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, ਮਾਪ_t ਮਾਪ)
{
    u_char      ch;
    ngx_uint_t  len;

    ਜੇ (dst == ਨਿਰਾ) {
        len = 0;

        ਜਦੋਂ (ਮਾਪ) {
            ch = *src++;

            ਜੇ (ch == '\\' || ch == '"') {
                len++;

            } ਨਹੀਂਤਾਂ ਜੇ (ch <= 0x1f) {

                ਚੋਣ (ch) {
                ਹਾਲਤ '\n':
                ਹਾਲਤ '\r':
                ਹਾਲਤ '\t':
                ਹਾਲਤ '\b':
                ਹਾਲਤ '\f':
                    len++;
                    ਰੋਕੋ;

                ਮੂਲ:
                    len += ਆਕਾਰ("\\u001F") - 2;
                }
            }

            ਮਾਪ--;
        }

        ਵਾਪਸ (uintptr_t) len;
    }

    ਜਦੋਂ (ਮਾਪ) {
        ch = *src++;

        ਜੇ (ch > 0x1f) {

            ਜੇ (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } ਨਹੀਂਤਾਂ {
            *dst++ = '\\';

            ਚੋਣ (ch) {
            ਹਾਲਤ '\n':
                *dst++ = 'n';
                ਰੋਕੋ;

            ਹਾਲਤ '\r':
                *dst++ = 'r';
                ਰੋਕੋ;

            ਹਾਲਤ '\t':
                *dst++ = 't';
                ਰੋਕੋ;

            ਹਾਲਤ '\b':
                *dst++ = 'b';
                ਰੋਕੋ;

            ਹਾਲਤ '\f':
                *dst++ = 'f';
                ਰੋਕੋ;

            ਮੂਲ:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        ਮਾਪ--;
    }

    ਵਾਪਸ (uintptr_t) dst;
}


ਖਾਲੀ
ngx_str_rbtree_ਪਾਓ_ਮੁੱਲ(ngx_rbtree_ਗੰਢ_t *ਆਰਜ਼ੀ,
    ngx_rbtree_ਗੰਢ_t *ਗੰਢ, ngx_rbtree_ਗੰਢ_t *sentinel)
{
    ngx_str_ਗੰਢ_t      *n, *t;
    ngx_rbtree_ਗੰਢ_t  **p;

    ਲਈ ( ;; ) {

        n = (ngx_str_ਗੰਢ_t *) ਗੰਢ;
        t = (ngx_str_ਗੰਢ_t *) ਆਰਜ਼ੀ;

        ਜੇ (ਗੰਢ->ਕੁੰਜੀ != ਆਰਜ਼ੀ->ਕੁੰਜੀ) {

            p = (ਗੰਢ->ਕੁੰਜੀ < ਆਰਜ਼ੀ->ਕੁੰਜੀ) ? &ਆਰਜ਼ੀ->ਖੱਬਾ : &ਆਰਜ਼ੀ->ਸੱਜਾ;

        } ਨਹੀਂਤਾਂ ਜੇ (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &ਆਰਜ਼ੀ->ਖੱਬਾ : &ਆਰਜ਼ੀ->ਸੱਜਾ;

        } ਨਹੀਂਤਾਂ {
            p = (ngx_memcmp(n->str.ਅੰਕੜੇ, t->str.ਅੰਕੜੇ, n->str.len) < 0)
                 ? &ਆਰਜ਼ੀ->ਖੱਬਾ : &ਆਰਜ਼ੀ->ਸੱਜਾ;
        }

        ਜੇ (*p == sentinel) {
            ਰੋਕੋ;
        }

        ਆਰਜ਼ੀ = *p;
    }

    *p = ਗੰਢ;
    ਗੰਢ->ਮਾਪਾ = ਆਰਜ਼ੀ;
    ਗੰਢ->ਖੱਬਾ = sentinel;
    ਗੰਢ->ਸੱਜਾ = sentinel;
    ngx_rbt_red(ਗੰਢ);
}


ngx_str_ਗੰਢ_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_ਗੰਢ_t     *n;
    ngx_rbtree_ਗੰਢ_t  *ਗੰਢ, *sentinel;

    ਗੰਢ = rbtree->ਜੜ੍ਹ;
    sentinel = rbtree->sentinel;

    ਜਦੋਂ (ਗੰਢ != sentinel) {

        n = (ngx_str_ਗੰਢ_t *) ਗੰਢ;

        ਜੇ (hash != ਗੰਢ->ਕੁੰਜੀ) {
            ਗੰਢ = (hash < ਗੰਢ->ਕੁੰਜੀ) ? ਗੰਢ->ਖੱਬਾ : ਗੰਢ->ਸੱਜਾ;
            ਜਾਰੀ;
        }

        ਜੇ (val->len != n->str.len) {
            ਗੰਢ = (val->len < n->str.len) ? ਗੰਢ->ਖੱਬਾ : ਗੰਢ->ਸੱਜਾ;
            ਜਾਰੀ;
        }

        rc = ngx_memcmp(val->ਅੰਕੜੇ, n->str.ਅੰਕੜੇ, val->len);

        ਜੇ (rc < 0) {
            ਗੰਢ = ਗੰਢ->ਖੱਬਾ;
            ਜਾਰੀ;
        }

        ਜੇ (rc > 0) {
            ਗੰਢ = ਗੰਢ->ਸੱਜਾ;
            ਜਾਰੀ;
        }

        ਵਾਪਸ n;
    }

    ਵਾਪਸ ਨਿਰਾ;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

ਖਾਲੀ
ngx_ਛਾਂਟੋ(ਖਾਲੀ *ਅਧਾਰ, ਮਾਪ_t n, ਮਾਪ_t ਮਾਪ,
    ngx_int_t (*cmp)(ਅਚਲ ਖਾਲੀ *, ਅਚਲ ਖਾਲੀ *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(ਮਾਪ, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ);
    ਜੇ (p == ਨਿਰਾ) {
        ਵਾਪਸ;
    }

    ਲਈ (p1 = (u_char *) ਅਧਾਰ + ਮਾਪ;
         p1 < (u_char *) ਅਧਾਰ + n * ਮਾਪ;
         p1 += ਮਾਪ)
    {
        ngx_memcpy(p, p1, ਮਾਪ);

        ਲਈ (p2 = p1;
             p2 > (u_char *) ਅਧਾਰ && cmp(p2 - ਮਾਪ, p) > 0;
             p2 -= ਮਾਪ)
        {
            ngx_memcpy(p2, p2 - ਮਾਪ, ਮਾਪ);
        }

        ngx_memcpy(p2, p, ਮਾਪ);
    }

    ngx_free(p);
}


ਖਾਲੀ
ngx_explicit_memzero(ਖਾਲੀ *buf, ਮਾਪ_t n)
{
    ngx_memzero(buf, n);
    ngx_ਸਮ੍ਰਿਤੀ_barrier();
}


#ਜੇ (NGX_MEMCPY_ਹੱਦਬੰਦੀ)

ਖਾਲੀ *
ngx_memcpy(ਖਾਲੀ *dst, ਅਚਲ ਖਾਲੀ *src, ਮਾਪ_t n)
{
    ਜੇ (n > NGX_MEMCPY_ਹੱਦਬੰਦੀ) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "memcpy %uz bytes", n);
        ngx_debug_ਬਿੰਦੂ();
    }

    ਵਾਪਸ ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst, src, n);
}

#ਜੇ_ਅੰਤ
