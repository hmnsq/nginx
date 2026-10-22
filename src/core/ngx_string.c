
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ u_char *ngx_sprintf_ಅಂಕೆ(u_char *buf, u_char *ಕೊನೆಯ, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t ಅಗಲ);
ಸ್ಥಿರ u_char *ngx_sprintf_str(u_char *buf, u_char *ಕೊನೆಯ, u_char *src,
    ಗಾತ್ರ_t len, ngx_uint_t hexadecimal);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_encode_base64_ಆಂತರಿಕ(ngx_str_t *dst, ngx_str_t *src,
    ಸ್ಥಿರಾಂಕ u_char *basis, ngx_uint_t padding);
ಸ್ಥಿರ ngx_int_t ngx_decode_base64_ಆಂತರಿಕ(ngx_str_t *dst, ngx_str_t *src,
    ಸ್ಥಿರಾಂಕ u_char *basis);


ಶೂನ್ಯ
ngx_strlow(u_char *dst, u_char *src, ಗಾತ್ರ_t n)
{
    ಆಗ (n) {
        *dst = ngx_tolower(*src);
        dst++;
        src++;
        n--;
    }
}


ಗಾತ್ರ_t
ngx_strnlen(u_char *p, ಗಾತ್ರ_t n)
{
    ಗಾತ್ರ_t  i;

    ಗಾಗಿ (i = 0; i < n; i++) {

        ಆದರೆ (p[i] == '\0') {
            ಹಿಂತಿರುಗಿ i;
        }
    }

    ಹಿಂತಿರುಗಿ n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, ಗಾತ್ರ_t n)
{
    ಆದರೆ (n == 0) {
        ಹಿಂತಿರುಗಿ dst;
    }

    ಆಗ (--n) {
        *dst = *src;

        ಆದರೆ (*dst == '\0') {
            ಹಿಂತಿರುಗಿ dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    ಹಿಂತಿರುಗಿ dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    ಆದರೆ (dst == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ngx_memcpy(dst, src->ದತ್ತಾಂಶ, src->len);

    ಹಿಂತಿರುಗಿ dst;
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
ngx_sprintf(u_char *buf, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
    u_char   *p;
    va_list   ವಾದಗಳು;

    va_ಆರಂಭ(ವಾದಗಳು, fmt);
    p = ngx_vslprintf(buf, (ಶೂನ್ಯ *) -1, fmt, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);

    ಹಿಂತಿರುಗಿ p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, ಗಾತ್ರ_t max, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
    u_char   *p;
    va_list   ವಾದಗಳು;

    va_ಆರಂಭ(ವಾದಗಳು, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);

    ಹಿಂತಿರುಗಿ p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *ಕೊನೆಯ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
    u_char   *p;
    va_list   ವಾದಗಳು;

    va_ಆರಂಭ(ವಾದಗಳು, fmt);
    p = ngx_vslprintf(buf, ಕೊನೆಯ, fmt, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);

    ಹಿಂತಿರುಗಿ p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *ಕೊನೆಯ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ವಾದಗಳು)
{
    u_char                *p, zero;
    ಪೂರ್ಣಾಂಕ                    d;
    ದ್ವಿಗುಣ                 f;
    ಗಾತ್ರ_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             ಅಗಲ, sign, hex, max_ಅಗಲ, frac_ಅಗಲ, scale, n;
    ngx_str_t             *v;
    ngx_variable_ಮೌಲ್ಯ_t  *vv;

    ಆಗ (*fmt && buf < ಕೊನೆಯ) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        ಆದರೆ (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            ಅಗಲ = 0;
            sign = 1;
            hex = 0;
            max_ಅಗಲ = 0;
            frac_ಅಗಲ = 0;
            slen = (ಗಾತ್ರ_t) -1;

            ಆಗ (*fmt >= '0' && *fmt <= '9') {
                ಅಗಲ = ಅಗಲ * 10 + (*fmt++ - '0');
            }


            ಗಾಗಿ ( ;; ) {
                ಆಯ್ಕೆ (*fmt) {

                ಸ್ಥಿತಿ 'u':
                    sign = 0;
                    fmt++;
                    ಮುಂದುವರಿ;

                ಸ್ಥಿತಿ 'm':
                    max_ಅಗಲ = 1;
                    fmt++;
                    ಮುಂದುವರಿ;

                ಸ್ಥಿತಿ 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    ಮುಂದುವರಿ;

                ಸ್ಥಿತಿ 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    ಮುಂದುವರಿ;

                ಸ್ಥಿತಿ '.':
                    fmt++;

                    ಆಗ (*fmt >= '0' && *fmt <= '9') {
                        frac_ಅಗಲ = frac_ಅಗಲ * 10 + (*fmt++ - '0');
                    }

                    ನಿಲ್ಲಿಸು;

                ಸ್ಥಿತಿ '*':
                    slen = va_arg(ವಾದಗಳು, ಗಾತ್ರ_t);
                    fmt++;
                    ಮುಂದುವರಿ;

                ಪೂರ್ವನಿಯೋಜಿತ:
                    ನಿಲ್ಲಿಸು;
                }

                ನಿಲ್ಲಿಸು;
            }


            ಆಯ್ಕೆ (*fmt) {

            ಸ್ಥಿತಿ 'V':
                v = va_arg(ವಾದಗಳು, ngx_str_t *);

                buf = ngx_sprintf_str(buf, ಕೊನೆಯ, v->ದತ್ತಾಂಶ, v->len, hex);
                fmt++;

                ಮುಂದುವರಿ;

            ಸ್ಥಿತಿ 'v':
                vv = va_arg(ವಾದಗಳು, ngx_variable_ಮೌಲ್ಯ_t *);

                buf = ngx_sprintf_str(buf, ಕೊನೆಯ, vv->ದತ್ತಾಂಶ, vv->len, hex);
                fmt++;

                ಮುಂದುವರಿ;

            ಸ್ಥಿತಿ 's':
                p = va_arg(ವಾದಗಳು, u_char *);

                buf = ngx_sprintf_str(buf, ಕೊನೆಯ, p, slen, hex);
                fmt++;

                ಮುಂದುವರಿ;

            ಸ್ಥಿತಿ 'O':
                i64 = (int64_t) va_arg(ವಾದಗಳು, off_t);
                sign = 1;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'P':
                i64 = (int64_t) va_arg(ವಾದಗಳು, ngx_pid_t);
                sign = 1;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'T':
                i64 = (int64_t) va_arg(ವಾದಗಳು, ಸಮಯ_t);
                sign = 1;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'M':
                ms = (ngx_msec_t) va_arg(ವಾದಗಳು, ngx_msec_t);
                ಆದರೆ ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } ಇಲ್ಲದಿದ್ದರೆ {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'z':
                ಆದರೆ (sign) {
                    i64 = (int64_t) va_arg(ವಾದಗಳು, ssize_t);
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ui64 = (uint64_t) va_arg(ವಾದಗಳು, ಗಾತ್ರ_t);
                }
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'i':
                ಆದರೆ (sign) {
                    i64 = (int64_t) va_arg(ವಾದಗಳು, ngx_int_t);
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ui64 = (uint64_t) va_arg(ವಾದಗಳು, ngx_uint_t);
                }

                ಆದರೆ (max_ಅಗಲ) {
                    ಅಗಲ = NGX_INT_T_LEN;
                }

                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'd':
                ಆದರೆ (sign) {
                    i64 = (int64_t) va_arg(ವಾದಗಳು, ಪೂರ್ಣಾಂಕ);
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ui64 = (uint64_t) va_arg(ವಾದಗಳು, u_int);
                }
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'l':
                ಆದರೆ (sign) {
                    i64 = (int64_t) va_arg(ವಾದಗಳು, ದೀರ್ಘ);
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ui64 = (uint64_t) va_arg(ವಾದಗಳು, u_long);
                }
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'D':
                ಆದರೆ (sign) {
                    i64 = (int64_t) va_arg(ವಾದಗಳು, int32_t);
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ui64 = (uint64_t) va_arg(ವಾದಗಳು, uint32_t);
                }
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'L':
                ಆದರೆ (sign) {
                    i64 = va_arg(ವಾದಗಳು, int64_t);
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ui64 = va_arg(ವಾದಗಳು, uint64_t);
                }
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'A':
                ಆದರೆ (sign) {
                    i64 = (int64_t) va_arg(ವಾದಗಳು, ngx_atomic_int_t);
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ui64 = (uint64_t) va_arg(ವಾದಗಳು, ngx_atomic_uint_t);
                }

                ಆದರೆ (max_ಅಗಲ) {
                    ಅಗಲ = NGX_ATOMIC_T_LEN;
                }

                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'f':
                f = va_arg(ವಾದಗಳು, ದ್ವಿಗುಣ);

                ಆದರೆ (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                ಆದರೆ (frac_ಅಗಲ) {

                    scale = 1;
                    ಗಾಗಿ (n = frac_ಅಗಲ; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (ದ್ವಿಗುಣ) ui64) * scale + 0.5);

                    ಆದರೆ (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_ಅಂಕೆ(buf, ಕೊನೆಯ, ui64, zero, 0, ಅಗಲ);

                ಆದರೆ (frac_ಅಗಲ) {
                    ಆದರೆ (buf < ಕೊನೆಯ) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_ಅಂಕೆ(buf, ಕೊನೆಯ, frac, '0', 0, frac_ಅಗಲ);
                }

                fmt++;

                ಮುಂದುವರಿ;

#ಆದರೆ !(NGX_WIN32)
            ಸ್ಥಿತಿ 'r':
                i64 = (int64_t) va_arg(ವಾದಗಳು, rlim_t);
                sign = 1;
                ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

            ಸ್ಥಿತಿ 'p':
                ui64 = (uintptr_t) va_arg(ವಾದಗಳು, ಶೂನ್ಯ *);
                hex = 2;
                sign = 0;
                zero = '0';
                ಅಗಲ = 2 * ಗಾತ್ರ(ಶೂನ್ಯ *);
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'c':
                d = va_arg(ವಾದಗಳು, ಪೂರ್ಣಾಂಕ);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                ಮುಂದುವರಿ;

            ಸ್ಥಿತಿ 'Z':
                *buf++ = '\0';
                fmt++;

                ಮುಂದುವರಿ;

            ಸ್ಥಿತಿ 'N':
#ಆದರೆ (NGX_WIN32)
                *buf++ = CR;
                ಆದರೆ (buf < ಕೊನೆಯ) {
                    *buf++ = LF;
                }
#ಇಲ್ಲದಿದ್ದರೆ
                *buf++ = LF;
#ಆದರೆ_ಅಂತ್ಯ
                fmt++;

                ಮುಂದುವರಿ;

            ಸ್ಥಿತಿ '%':
                *buf++ = '%';
                fmt++;

                ಮುಂದುವರಿ;

            ಪೂರ್ವನಿಯೋಜಿತ:
                *buf++ = *fmt++;

                ಮುಂದುವರಿ;
            }

            ಆದರೆ (sign) {
                ಆದರೆ (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } ಇಲ್ಲದಿದ್ದರೆ {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_ಅಂಕೆ(buf, ಕೊನೆಯ, ui64, zero, hex, ಅಗಲ);

            fmt++;

        } ಇಲ್ಲದಿದ್ದರೆ {
            *buf++ = *fmt++;
        }
    }

    ಹಿಂತಿರುಗಿ buf;
}


ಸ್ಥಿರ u_char *
ngx_sprintf_ಅಂಕೆ(u_char *buf, u_char *ಕೊನೆಯ, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t ಅಗಲ)
{
    u_char         *p, ತಾತ್ಕಾಲಿಕ[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    ಗಾತ್ರ_t          len;
    uint32_t        ui32;
    ಸ್ಥಿರ u_char   hex[] = "0123456789abcdef";
    ಸ್ಥಿರ u_char   HEX[] = "0123456789ABCDEF";

    p = ತಾತ್ಕಾಲಿಕ + NGX_INT64_LEN;

    ಆದರೆ (hexadecimal == 0) {

        ಆದರೆ (ui64 <= (uint64_t) NGX_MAX_UINT32_ಮೌಲ್ಯ) {

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

            ಮಾಡು {
                *--p = (u_char) (ui32 % 10 + '0');
            } ಆಗ (ui32 /= 10);

        } ಇಲ್ಲದಿದ್ದರೆ {
            ಮಾಡು {
                *--p = (u_char) (ui64 % 10 + '0');
            } ಆಗ (ui64 /= 10);
        }

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (hexadecimal == 1) {

        ಮಾಡು {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } ಆಗ (ui64 >>= 4);

    } ಇಲ್ಲದಿದ್ದರೆ { /* hexadecimal == 2 */

        ಮಾಡು {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } ಆಗ (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (ತಾತ್ಕಾಲಿಕ + NGX_INT64_LEN) - p;

    ಆಗ (len++ < ಅಗಲ && buf < ಕೊನೆಯ) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (ತಾತ್ಕಾಲಿಕ + NGX_INT64_LEN) - p;

    ಆದರೆ (buf + len > ಕೊನೆಯ) {
        len = ಕೊನೆಯ - buf;
    }

    ಹಿಂತಿರುಗಿ ngx_cpymem(buf, p, len);
}


ಸ್ಥಿರ u_char *
ngx_sprintf_str(u_char *buf, u_char *ಕೊನೆಯ, u_char *src, ಗಾತ್ರ_t len,
    ngx_uint_t hexadecimal)
{
    ಸ್ಥಿರ u_char   hex[] = "0123456789abcdef";
    ಸ್ಥಿರ u_char   HEX[] = "0123456789ABCDEF";

    ಆದರೆ (hexadecimal == 0) {

        ಆದರೆ (len == (ಗಾತ್ರ_t) -1) {
            ಆಗ (*src && buf < ಕೊನೆಯ) {
                *buf++ = *src++;
            }

        } ಇಲ್ಲದಿದ್ದರೆ {
            len = ngx_min((ಗಾತ್ರ_t) (ಕೊನೆಯ - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (hexadecimal == 1) {

        ಆದರೆ (len == (ಗಾತ್ರ_t) -1) {

            ಆಗ (*src && buf < ಕೊನೆಯ - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } ಇಲ್ಲದಿದ್ದರೆ {

            ಆಗ (len-- && buf < ಕೊನೆಯ - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } ಇಲ್ಲದಿದ್ದರೆ { /* hexadecimal == 2 */

        ಆದರೆ (len == (ಗಾತ್ರ_t) -1) {

            ಆಗ (*src && buf < ಕೊನೆಯ - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } ಇಲ್ಲದಿದ್ದರೆ {

            ಆಗ (len-- && buf < ಕೊನೆಯ - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    ಹಿಂತಿರುಗಿ buf;
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

    ಗಾಗಿ ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ಆದರೆ (c1 == c2) {

            ಆದರೆ (c1) {
                ಮುಂದುವರಿ;
            }

            ಹಿಂತಿರುಗಿ 0;
        }

        ಹಿಂತಿರುಗಿ c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, ಗಾತ್ರ_t n)
{
    ngx_uint_t  c1, c2;

    ಆಗ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ಆದರೆ (c1 == c2) {

            ಆದರೆ (c1) {
                n--;
                ಮುಂದುವರಿ;
            }

            ಹಿಂತಿರುಗಿ 0;
        }

        ಹಿಂತಿರುಗಿ c1 - c2;
    }

    ಹಿಂತಿರುಗಿ 0;
}


u_char *
ngx_strnstr(u_char *s1, ಅಕ್ಷರ *s2, ಗಾತ್ರ_t len)
{
    u_char  c1, c2;
    ಗಾತ್ರ_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    ಮಾಡು {
        ಮಾಡು {
            ಆದರೆ (len-- == 0) {
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

            c1 = *s1++;

            ಆದರೆ (c1 == 0) {
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

        } ಆಗ (c1 != c2);

        ಆದರೆ (n > len) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

    } ಆಗ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ಹಿಂತಿರುಗಿ --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, ಅಕ್ಷರ *s2, ಗಾತ್ರ_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    ಮಾಡು {
        ಮಾಡು {
            c1 = *s1++;

            ಆದರೆ (c1 == 0) {
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

        } ಆಗ (c1 != c2);

    } ಆಗ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ಹಿಂತಿರುಗಿ --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, ಅಕ್ಷರ *s2, ಗಾತ್ರ_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    ಮಾಡು {
        ಮಾಡು {
            c1 = (ngx_uint_t) *s1++;

            ಆದರೆ (c1 == 0) {
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ಆಗ (c1 != c2);

    } ಆಗ (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    ಹಿಂತಿರುಗಿ --s1;
}


/*
 * ngx_strlcasestrn() is intended to search for static substring
 * with known length in string until the argument last. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strlcasestrn(u_char *s1, u_char *ಕೊನೆಯ, u_char *s2, ಗಾತ್ರ_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;
    ಕೊನೆಯ -= n;

    ಮಾಡು {
        ಮಾಡು {
            ಆದರೆ (s1 >= ಕೊನೆಯ) {
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ಆಗ (c1 != c2);

    } ಆಗ (ngx_strncasecmp(s1, s2, n) != 0);

    ಹಿಂತಿರುಗಿ --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, ಗಾತ್ರ_t n)
{
    ಆದರೆ (n == 0) {
        ಹಿಂತಿರುಗಿ 0;
    }

    n--;

    ಗಾಗಿ ( ;; ) {
        ಆದರೆ (s1[n] != s2[n]) {
            ಹಿಂತಿರುಗಿ s1[n] - s2[n];
        }

        ಆದರೆ (n == 0) {
            ಹಿಂತಿರುಗಿ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, ಗಾತ್ರ_t n)
{
    u_char  c1, c2;

    ಆದರೆ (n == 0) {
        ಹಿಂತಿರುಗಿ 0;
    }

    n--;

    ಗಾಗಿ ( ;; ) {
        c1 = s1[n];
        ಆದರೆ (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        ಆದರೆ (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        ಆದರೆ (c1 != c2) {
            ಹಿಂತಿರುಗಿ c1 - c2;
        }

        ಆದರೆ (n == 0) {
            ಹಿಂತಿರುಗಿ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, ಗಾತ್ರ_t n1, ಗಾತ್ರ_t n2)
{
    ಗಾತ್ರ_t     n;
    ngx_int_t  m, z;

    ಆದರೆ (n1 <= n2) {
        n = n1;
        z = -1;

    } ಇಲ್ಲದಿದ್ದರೆ {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    ಆದರೆ (m || n1 == n2) {
        ಹಿಂತಿರುಗಿ m;
    }

    ಹಿಂತಿರುಗಿ z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    ಗಾಗಿ ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        ಆದರೆ (c1 == c2) {

            ಆದರೆ (c1) {
                ಮುಂದುವರಿ;
            }

            ಹಿಂತಿರುಗಿ 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        ಹಿಂತಿರುಗಿ c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, ಗಾತ್ರ_t n)
{
    ngx_uint_t  c1, c2;

    ಆಗ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#ಆದರೆ (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#ಆದರೆ_ಅಂತ್ಯ

        ಆದರೆ (c1 == c2) {

            ಆದರೆ (c1) {
                n--;
                ಮುಂದುವರಿ;
            }

            ಹಿಂತಿರುಗಿ 0;
        }

        /* we need '/' to be the lowest character */

        ಆದರೆ (c1 == 0 || c2 == 0) {
            ಹಿಂತಿರುಗಿ c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        ಹಿಂತಿರುಗಿ c1 - c2;
    }

    ಹಿಂತಿರುಗಿ 0;
}


ngx_int_t
ngx_atoi(u_char *ಸಾಲು, ಗಾತ್ರ_t n)
{
    ngx_int_t  ಮೌಲ್ಯ, cutoff, cutlim;

    ಆದರೆ (n == 0) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_ಮೌಲ್ಯ / 10;
    cutlim = NGX_MAX_INT_T_ಮೌಲ್ಯ % 10;

    ಗಾಗಿ (ಮೌಲ್ಯ = 0; n--; ಸಾಲು++) {
        ಆದರೆ (*ಸಾಲು < '0' || *ಸಾಲು > '9') {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (ಮೌಲ್ಯ >= cutoff && (ಮೌಲ್ಯ > cutoff || *ಸಾಲು - '0' > cutlim)) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10 + (*ಸಾಲು - '0');
    }

    ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *ಸಾಲು, ಗಾತ್ರ_t n, ಗಾತ್ರ_t ಬಿಂದು)
{
    ngx_int_t   ಮೌಲ್ಯ, cutoff, cutlim;
    ngx_uint_t  dot;

    ಆದರೆ (n == 0) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_ಮೌಲ್ಯ / 10;
    cutlim = NGX_MAX_INT_T_ಮೌಲ್ಯ % 10;

    dot = 0;

    ಗಾಗಿ (ಮೌಲ್ಯ = 0; n--; ಸಾಲು++) {

        ಆದರೆ (ಬಿಂದು == 0) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (*ಸಾಲು == '.') {
            ಆದರೆ (dot) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            dot = 1;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (*ಸಾಲು < '0' || *ಸಾಲು > '9') {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (ಮೌಲ್ಯ >= cutoff && (ಮೌಲ್ಯ > cutoff || *ಸಾಲು - '0' > cutlim)) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10 + (*ಸಾಲು - '0');
        ಬಿಂದು -= dot;
    }

    ಆಗ (ಬಿಂದು--) {
        ಆದರೆ (ಮೌಲ್ಯ > cutoff) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10;
    }

    ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
}


ssize_t
ngx_atosz(u_char *ಸಾಲು, ಗಾತ್ರ_t n)
{
    ssize_t  ಮೌಲ್ಯ, cutoff, cutlim;

    ಆದರೆ (n == 0) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    cutoff = NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ / 10;
    cutlim = NGX_MAX_ಗಾತ್ರ_T_ಮೌಲ್ಯ % 10;

    ಗಾಗಿ (ಮೌಲ್ಯ = 0; n--; ಸಾಲು++) {
        ಆದರೆ (*ಸಾಲು < '0' || *ಸಾಲು > '9') {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (ಮೌಲ್ಯ >= cutoff && (ಮೌಲ್ಯ > cutoff || *ಸಾಲು - '0' > cutlim)) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10 + (*ಸಾಲು - '0');
    }

    ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
}


off_t
ngx_atoof(u_char *ಸಾಲು, ಗಾತ್ರ_t n)
{
    off_t  ಮೌಲ್ಯ, cutoff, cutlim;

    ಆದರೆ (n == 0) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_ಮೌಲ್ಯ / 10;
    cutlim = NGX_MAX_OFF_T_ಮೌಲ್ಯ % 10;

    ಗಾಗಿ (ಮೌಲ್ಯ = 0; n--; ಸಾಲು++) {
        ಆದರೆ (*ಸಾಲು < '0' || *ಸಾಲು > '9') {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (ಮೌಲ್ಯ >= cutoff && (ಮೌಲ್ಯ > cutoff || *ಸಾಲು - '0' > cutlim)) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10 + (*ಸಾಲು - '0');
    }

    ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
}


ಸಮಯ_t
ngx_atotm(u_char *ಸಾಲು, ಗಾತ್ರ_t n)
{
    ಸಮಯ_t  ಮೌಲ್ಯ, cutoff, cutlim;

    ಆದರೆ (n == 0) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    cutoff = NGX_MAX_ಸಮಯ_T_ಮೌಲ್ಯ / 10;
    cutlim = NGX_MAX_ಸಮಯ_T_ಮೌಲ್ಯ % 10;

    ಗಾಗಿ (ಮೌಲ್ಯ = 0; n--; ಸಾಲು++) {
        ಆದರೆ (*ಸಾಲು < '0' || *ಸಾಲು > '9') {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (ಮೌಲ್ಯ >= cutoff && (ಮೌಲ್ಯ > cutoff || *ಸಾಲು - '0' > cutlim)) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಮೌಲ್ಯ = ಮೌಲ್ಯ * 10 + (*ಸಾಲು - '0');
    }

    ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
}


ngx_int_t
ngx_hextoi(u_char *ಸಾಲು, ಗಾತ್ರ_t n)
{
    u_char     c, ch;
    ngx_int_t  ಮೌಲ್ಯ, cutoff;

    ಆದರೆ (n == 0) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_ಮೌಲ್ಯ / 16;

    ಗಾಗಿ (ಮೌಲ್ಯ = 0; n--; ಸಾಲು++) {
        ಆದರೆ (ಮೌಲ್ಯ > cutoff) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ch = *ಸಾಲು;

        ಆದರೆ (ch >= '0' && ch <= '9') {
            ಮೌಲ್ಯ = ಮೌಲ್ಯ * 16 + (ch - '0');
            ಮುಂದುವರಿ;
        }

        c = (u_char) (ch | 0x20);

        ಆದರೆ (c >= 'a' && c <= 'f') {
            ಮೌಲ್ಯ = ಮೌಲ್ಯ * 16 + (c - 'a' + 10);
            ಮುಂದುವರಿ;
        }

        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, ಗಾತ್ರ_t len)
{
    ಸ್ಥಿರ u_char  hex[] = "0123456789abcdef";

    ಆಗ (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    ಹಿಂತಿರುಗಿ dst;
}


ಶೂನ್ಯ
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    ಸ್ಥಿರ u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_ಆಂತರಿಕ(dst, src, basis64, 1);
}


ಶೂನ್ಯ
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    ಸ್ಥಿರ u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_ಆಂತರಿಕ(dst, src, basis64, 0);
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_encode_base64_ಆಂತರಿಕ(ngx_str_t *dst, ngx_str_t *src, ಸ್ಥಿರಾಂಕ u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    ಗಾತ್ರ_t          len;

    len = src->len;
    s = src->ದತ್ತಾಂಶ;
    d = dst->ದತ್ತಾಂಶ;

    ಆಗ (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    ಆದರೆ (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        ಆದರೆ (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            ಆದರೆ (padding) {
                *d++ = '=';
            }

        } ಇಲ್ಲದಿದ್ದರೆ {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        ಆದರೆ (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->ದತ್ತಾಂಶ;
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

    ಹಿಂತಿರುಗಿ ngx_decode_base64_ಆಂತರಿಕ(dst, src, basis64);
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

    ಹಿಂತಿರುಗಿ ngx_decode_base64_ಆಂತರಿಕ(dst, src, basis64);
}


ಸ್ಥಿರ ngx_int_t
ngx_decode_base64_ಆಂತರಿಕ(ngx_str_t *dst, ngx_str_t *src, ಸ್ಥಿರಾಂಕ u_char *basis)
{
    ಗಾತ್ರ_t          len;
    u_char         *d, *s;

    ಗಾಗಿ (len = 0; len < src->len; len++) {
        ಆದರೆ (src->ದತ್ತಾಂಶ[len] == '=') {
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (basis[src->ದತ್ತಾಂಶ[len]] == 77) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    ಆದರೆ (len % 4 == 1) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    s = src->ದತ್ತಾಂಶ;
    d = dst->ದತ್ತಾಂಶ;

    ಆಗ (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    ಆದರೆ (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    ಆದರೆ (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->ದತ್ತಾಂಶ;

    ಹಿಂತಿರುಗಿ NGX_OK;
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
ngx_utf8_decode(u_char **p, ಗಾತ್ರ_t n)
{
    ಗಾತ್ರ_t    len;
    uint32_t  u, i, valid;

    u = **p;

    ಆದರೆ (u >= 0xf8) {

        (*p)++;
        ಹಿಂತಿರುಗಿ 0xffffffff;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } ಇಲ್ಲದಿದ್ದರೆ {
        (*p)++;
        ಹಿಂತಿರುಗಿ 0xffffffff;
    }

    ಆದರೆ (n - 1 < len) {
        ಹಿಂತಿರುಗಿ 0xfffffffe;
    }

    (*p)++;

    ಆಗ (len) {
        i = *(*p)++;

        ಆದರೆ (i < 0x80) {
            ಹಿಂತಿರುಗಿ 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    ಆದರೆ (u > valid) {
        ಹಿಂತಿರುಗಿ u;
    }

    ಹಿಂತಿರುಗಿ 0xffffffff;
}


ಗಾತ್ರ_t
ngx_utf8_ಉದ್ದಳತೆ(u_char *p, ಗಾತ್ರ_t n)
{
    u_char  c, *ಕೊನೆಯ;
    ಗಾತ್ರ_t  len;

    ಕೊನೆಯ = p + n;

    ಗಾಗಿ (len = 0; p < ಕೊನೆಯ; len++) {

        c = *p;

        ಆದರೆ (c < 0x80) {
            p++;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ngx_utf8_decode(&p, ಕೊನೆಯ - p) > 0x10ffff) {
            /* invalid UTF-8 */
            ಹಿಂತಿರುಗಿ n;
        }
    }

    ಹಿಂತಿರುಗಿ len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, ಗಾತ್ರ_t n, ಗಾತ್ರ_t len)
{
    u_char  c, *ಮುಂದಿನ;

    ಆದರೆ (n == 0) {
        ಹಿಂತಿರುಗಿ dst;
    }

    ಆಗ (--n) {

        c = *src;
        *dst = c;

        ಆದರೆ (c < 0x80) {

            ಆದರೆ (c != '\0') {
                dst++;
                src++;
                len--;

                ಮುಂದುವರಿ;
            }

            ಹಿಂತಿರುಗಿ dst;
        }

        ಮುಂದಿನ = src;

        ಆದರೆ (ngx_utf8_decode(&ಮುಂದಿನ, len) > 0x10ffff) {
            /* invalid UTF-8 */
            ನಿಲ್ಲಿಸು;
        }

        ಆಗ (src < ಮುಂದಿನ) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    ಹಿಂತಿರುಗಿ dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, ಗಾತ್ರ_t ಗಾತ್ರ_2, ngx_uint_t type)
{
    u_char          ಪೂರ್ವಪ್ರತ್ಯಯ;
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

    ಸ್ಥಿರ uint32_t   ವಾದಗಳು[] = {
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
        { uri, ವಾದಗಳು, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    ಸ್ಥಿರ u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    ಪೂರ್ವಪ್ರತ್ಯಯ = map_char[type];

    ಆದರೆ (dst == ಖಾಲಿ) {

        /* find the number of the characters to be escaped */

        n = 0;

        ಆಗ (ಗಾತ್ರ_2) {
            ಆದರೆ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            ಗಾತ್ರ_2--;
        }

        ಹಿಂತಿರುಗಿ (uintptr_t) n;
    }

    ಆಗ (ಗಾತ್ರ_2) {
        ಆದರೆ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = ಪೂರ್ವಪ್ರತ್ಯಯ;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } ಇಲ್ಲದಿದ್ದರೆ {
            *dst++ = *src++;
        }
        ಗಾತ್ರ_2--;
    }

    ಹಿಂತಿರುಗಿ (uintptr_t) dst;
}


ಶೂನ್ಯ
ngx_unescape_uri(u_char **dst, u_char **src, ಗಾತ್ರ_t ಗಾತ್ರ_2, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    ಎಣಿಕೆ {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_ಎರಡನೇ
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    ಆಗ (ಗಾತ್ರ_2--) {

        ch = *s++;

        ಆಯ್ಕೆ (state) {
        ಸ್ಥಿತಿ sw_usual:
            ಆದರೆ (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                ಹೋಗು ಮುಗಿದ;
            }

            ಆದರೆ (ch == '%') {
                state = sw_quoted;
                ನಿಲ್ಲಿಸು;
            }

            *d++ = ch;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ sw_quoted:

            ಆದರೆ (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_ಎರಡನೇ;
                ನಿಲ್ಲಿಸು;
            }

            c = (u_char) (ch | 0x20);
            ಆದರೆ (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_ಎರಡನೇ;
                ನಿಲ್ಲಿಸು;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ sw_quoted_ಎರಡನೇ:

            state = sw_usual;

            ಆದರೆ (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                ಆದರೆ (type & NGX_UNESCAPE_REDIRECT) {
                    ಆದರೆ (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        ನಿಲ್ಲಿಸು;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    ನಿಲ್ಲಿಸು;
                }

                *d++ = ch;

                ನಿಲ್ಲಿಸು;
            }

            c = (u_char) (ch | 0x20);
            ಆದರೆ (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                ಆದರೆ (type & NGX_UNESCAPE_URI) {
                    ಆದರೆ (ch == '?') {
                        *d++ = ch;
                        ಹೋಗು ಮುಗಿದ;
                    }

                    *d++ = ch;
                    ನಿಲ್ಲಿಸು;
                }

                ಆದರೆ (type & NGX_UNESCAPE_REDIRECT) {
                    ಆದರೆ (ch == '?') {
                        *d++ = ch;
                        ಹೋಗು ಮುಗಿದ;
                    }

                    ಆದರೆ (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        ನಿಲ್ಲಿಸು;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    ನಿಲ್ಲಿಸು;
                }

                *d++ = ch;

                ನಿಲ್ಲಿಸು;
            }

            /* the invalid quoted character */

            ನಿಲ್ಲಿಸು;
        }
    }

ಮುಗಿದ:

    *dst = d;
    *src = s;
}


uintptr_t
ngx_escape_html(u_char *dst, u_char *src, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    u_char      ch;
    ngx_uint_t  len;

    ಆದರೆ (dst == ಖಾಲಿ) {

        len = 0;

        ಆಗ (ಗಾತ್ರ_2) {
            ಆಯ್ಕೆ (*src++) {

            ಸ್ಥಿತಿ '<':
                len += ಗಾತ್ರ("&lt;") - 2;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '>':
                len += ಗಾತ್ರ("&gt;") - 2;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '&':
                len += ಗಾತ್ರ("&amp;") - 2;
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '"':
                len += ಗಾತ್ರ("&quot;") - 2;
                ನಿಲ್ಲಿಸು;

            ಪೂರ್ವನಿಯೋಜಿತ:
                ನಿಲ್ಲಿಸು;
            }
            ಗಾತ್ರ_2--;
        }

        ಹಿಂತಿರುಗಿ (uintptr_t) len;
    }

    ಆಗ (ಗಾತ್ರ_2) {
        ch = *src++;

        ಆಯ್ಕೆ (ch) {

        ಸ್ಥಿತಿ '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
            *dst++ = ch;
            ನಿಲ್ಲಿಸು;
        }
        ಗಾತ್ರ_2--;
    }

    ಹಿಂತಿರುಗಿ (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    u_char      ch;
    ngx_uint_t  len;

    ಆದರೆ (dst == ಖಾಲಿ) {
        len = 0;

        ಆಗ (ಗಾತ್ರ_2) {
            ch = *src++;

            ಆದರೆ (ch == '\\' || ch == '"') {
                len++;

            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ch <= 0x1f) {

                ಆಯ್ಕೆ (ch) {
                ಸ್ಥಿತಿ '\n':
                ಸ್ಥಿತಿ '\r':
                ಸ್ಥಿತಿ '\t':
                ಸ್ಥಿತಿ '\b':
                ಸ್ಥಿತಿ '\f':
                    len++;
                    ನಿಲ್ಲಿಸು;

                ಪೂರ್ವನಿಯೋಜಿತ:
                    len += ಗಾತ್ರ("\\u001F") - 2;
                }
            }

            ಗಾತ್ರ_2--;
        }

        ಹಿಂತಿರುಗಿ (uintptr_t) len;
    }

    ಆಗ (ಗಾತ್ರ_2) {
        ch = *src++;

        ಆದರೆ (ch > 0x1f) {

            ಆದರೆ (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } ಇಲ್ಲದಿದ್ದರೆ {
            *dst++ = '\\';

            ಆಯ್ಕೆ (ch) {
            ಸ್ಥಿತಿ '\n':
                *dst++ = 'n';
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\r':
                *dst++ = 'r';
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\t':
                *dst++ = 't';
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\b':
                *dst++ = 'b';
                ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\f':
                *dst++ = 'f';
                ನಿಲ್ಲಿಸು;

            ಪೂರ್ವನಿಯೋಜಿತ:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        ಗಾತ್ರ_2--;
    }

    ಹಿಂತಿರುಗಿ (uintptr_t) dst;
}


ಶೂನ್ಯ
ngx_str_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ(ngx_rbtree_ಗಂಟು_t *ತಾತ್ಕಾಲಿಕ,
    ngx_rbtree_ಗಂಟು_t *ಗಂಟು, ngx_rbtree_ಗಂಟು_t *sentinel)
{
    ngx_str_ಗಂಟು_t      *n, *t;
    ngx_rbtree_ಗಂಟು_t  **p;

    ಗಾಗಿ ( ;; ) {

        n = (ngx_str_ಗಂಟು_t *) ಗಂಟು;
        t = (ngx_str_ಗಂಟು_t *) ತಾತ್ಕಾಲಿಕ;

        ಆದರೆ (ಗಂಟು->ಕೀಲಿ != ತಾತ್ಕಾಲಿಕ->ಕೀಲಿ) {

            p = (ಗಂಟು->ಕೀಲಿ < ತಾತ್ಕಾಲಿಕ->ಕೀಲಿ) ? &ತಾತ್ಕಾಲಿಕ->ಎಡ : &ತಾತ್ಕಾಲಿಕ->ಬಲ;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &ತಾತ್ಕಾಲಿಕ->ಎಡ : &ತಾತ್ಕಾಲಿಕ->ಬಲ;

        } ಇಲ್ಲದಿದ್ದರೆ {
            p = (ngx_memcmp(n->str.ದತ್ತಾಂಶ, t->str.ದತ್ತಾಂಶ, n->str.len) < 0)
                 ? &ತಾತ್ಕಾಲಿಕ->ಎಡ : &ತಾತ್ಕಾಲಿಕ->ಬಲ;
        }

        ಆದರೆ (*p == sentinel) {
            ನಿಲ್ಲಿಸು;
        }

        ತಾತ್ಕಾಲಿಕ = *p;
    }

    *p = ಗಂಟು;
    ಗಂಟು->ಪೋಷಕ = ತಾತ್ಕಾಲಿಕ;
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

    ಆಗ (ಗಂಟು != sentinel) {

        n = (ngx_str_ಗಂಟು_t *) ಗಂಟು;

        ಆದರೆ (hash != ಗಂಟು->ಕೀಲಿ) {
            ಗಂಟು = (hash < ಗಂಟು->ಕೀಲಿ) ? ಗಂಟು->ಎಡ : ಗಂಟು->ಬಲ;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (val->len != n->str.len) {
            ಗಂಟು = (val->len < n->str.len) ? ಗಂಟು->ಎಡ : ಗಂಟು->ಬಲ;
            ಮುಂದುವರಿ;
        }

        rc = ngx_memcmp(val->ದತ್ತಾಂಶ, n->str.ದತ್ತಾಂಶ, val->len);

        ಆದರೆ (rc < 0) {
            ಗಂಟು = ಗಂಟು->ಎಡ;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (rc > 0) {
            ಗಂಟು = ಗಂಟು->ಬಲ;
            ಮುಂದುವರಿ;
        }

        ಹಿಂತಿರುಗಿ n;
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

ಶೂನ್ಯ
ngx_ವಿಂಗಡಿಸು(ಶೂನ್ಯ *ತಳ, ಗಾತ್ರ_t n, ಗಾತ್ರ_t ಗಾತ್ರ_2,
    ngx_int_t (*cmp)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(ಗಾತ್ರ_2, ngx_ಚಕ್ರ->ಲಘುಗಣಕ);
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ;
    }

    ಗಾಗಿ (p1 = (u_char *) ತಳ + ಗಾತ್ರ_2;
         p1 < (u_char *) ತಳ + n * ಗಾತ್ರ_2;
         p1 += ಗಾತ್ರ_2)
    {
        ngx_memcpy(p, p1, ಗಾತ್ರ_2);

        ಗಾಗಿ (p2 = p1;
             p2 > (u_char *) ತಳ && cmp(p2 - ಗಾತ್ರ_2, p) > 0;
             p2 -= ಗಾತ್ರ_2)
        {
            ngx_memcpy(p2, p2 - ಗಾತ್ರ_2, ಗಾತ್ರ_2);
        }

        ngx_memcpy(p2, p, ಗಾತ್ರ_2);
    }

    ngx_free(p);
}


ಶೂನ್ಯ
ngx_explicit_memzero(ಶೂನ್ಯ *buf, ಗಾತ್ರ_t n)
{
    ngx_memzero(buf, n);
    ngx_ಸ್ಮರಣೆ_barrier();
}


#ಆದರೆ (NGX_MEMCPY_ಎಲ್ಲೆ)

ಶೂನ್ಯ *
ngx_memcpy(ಶೂನ್ಯ *dst, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *src, ಗಾತ್ರ_t n)
{
    ಆದರೆ (n > NGX_MEMCPY_ಎಲ್ಲೆ) {
        ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, 0, "memcpy %uz bytes", n);
        ngx_debug_ಬಿಂದು();
    }

    ಹಿಂತಿರುಗಿ ಸ್ಮರಣೆ_ನಕಲು(dst, src, n);
}

#ಆದರೆ_ಅಂತ್ಯ
