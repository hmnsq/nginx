
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>


স্থির u_char *ngx_sprintf_অঙ্ক(u_char *buf, u_char *last, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
স্থির u_char *ngx_sprintf_str(u_char *buf, u_char *last, u_char *src,
    size_t len, ngx_uint_t hexadecimal);
স্থির শূন্য ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ধ্রুৱ u_char *basis, ngx_uint_t padding);
স্থির ngx_int_t ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ধ্রুৱ u_char *basis);


শূন্য
ngx_strlow(u_char *dst, u_char *src, size_t n)
{
    যেতিয়ালৈ (n) {
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

    বাবে (i = 0; i < n; i++) {

        যদি (p[i] == '\0') {
            ঘূৰাই i;
        }
    }

    ঘূৰাই n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, size_t n)
{
    যদি (n == 0) {
        ঘূৰাই dst;
    }

    যেতিয়ালৈ (--n) {
        *dst = *src;

        যদি (*dst == '\0') {
            ঘূৰাই dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    ঘূৰাই dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    যদি (dst == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    ngx_memcpy(dst, src->data, src->len);

    ঘূৰাই dst;
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
ngx_sprintf(u_char *buf, ধ্রুৱ ৱর্ণ *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, (শূন্য *) -1, fmt, args);
    va_end(args);

    ঘূৰাই p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, size_t max, ধ্রুৱ ৱর্ণ *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, args);
    va_end(args);

    ঘূৰাই p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *last, ধ্রুৱ ৱর্ণ *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, last, fmt, args);
    va_end(args);

    ঘূৰাই p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *last, ধ্রুৱ ৱর্ণ *fmt, va_list args)
{
    u_char                *p, zero;
    পূর্ণাঙ্ক                    d;
    দ্ৱিগুণ                 f;
    size_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             width, sign, hex, max_width, frac_width, scale, n;
    ngx_str_t             *v;
    ngx_variable_মান_t  *vv;

    যেতিয়ালৈ (*fmt && buf < last) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        যদি (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (size_t) -1;

            যেতিয়ালৈ (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }


            বাবে ( ;; ) {
                ৱিকল্প (*fmt) {

                পক্ষ 'u':
                    sign = 0;
                    fmt++;
                    অনুৱর্ত;

                পক্ষ 'm':
                    max_width = 1;
                    fmt++;
                    অনুৱর্ত;

                পক্ষ 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    অনুৱর্ত;

                পক্ষ 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    অনুৱর্ত;

                পক্ষ '.':
                    fmt++;

                    যেতিয়ালৈ (*fmt >= '0' && *fmt <= '9') {
                        frac_width = frac_width * 10 + (*fmt++ - '0');
                    }

                    ভঙ্গ;

                পক্ষ '*':
                    slen = va_arg(args, size_t);
                    fmt++;
                    অনুৱর্ত;

                স্ৱভাৱ:
                    ভঙ্গ;
                }

                ভঙ্গ;
            }


            ৱিকল্প (*fmt) {

            পক্ষ 'V':
                v = va_arg(args, ngx_str_t *);

                buf = ngx_sprintf_str(buf, last, v->data, v->len, hex);
                fmt++;

                অনুৱর্ত;

            পক্ষ 'v':
                vv = va_arg(args, ngx_variable_মান_t *);

                buf = ngx_sprintf_str(buf, last, vv->data, vv->len, hex);
                fmt++;

                অনুৱর্ত;

            পক্ষ 's':
                p = va_arg(args, u_char *);

                buf = ngx_sprintf_str(buf, last, p, slen, hex);
                fmt++;

                অনুৱর্ত;

            পক্ষ 'O':
                i64 = (int64_t) va_arg(args, off_t);
                sign = 1;
                ভঙ্গ;

            পক্ষ 'P':
                i64 = (int64_t) va_arg(args, ngx_pid_t);
                sign = 1;
                ভঙ্গ;

            পক্ষ 'T':
                i64 = (int64_t) va_arg(args, time_t);
                sign = 1;
                ভঙ্গ;

            পক্ষ 'M':
                ms = (ngx_msec_t) va_arg(args, ngx_msec_t);
                যদি ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } অন্যথা {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                ভঙ্গ;

            পক্ষ 'z':
                যদি (sign) {
                    i64 = (int64_t) va_arg(args, ssize_t);
                } অন্যথা {
                    ui64 = (uint64_t) va_arg(args, size_t);
                }
                ভঙ্গ;

            পক্ষ 'i':
                যদি (sign) {
                    i64 = (int64_t) va_arg(args, ngx_int_t);
                } অন্যথা {
                    ui64 = (uint64_t) va_arg(args, ngx_uint_t);
                }

                যদি (max_width) {
                    width = NGX_INT_T_LEN;
                }

                ভঙ্গ;

            পক্ষ 'd':
                যদি (sign) {
                    i64 = (int64_t) va_arg(args, পূর্ণাঙ্ক);
                } অন্যথা {
                    ui64 = (uint64_t) va_arg(args, u_int);
                }
                ভঙ্গ;

            পক্ষ 'l':
                যদি (sign) {
                    i64 = (int64_t) va_arg(args, long);
                } অন্যথা {
                    ui64 = (uint64_t) va_arg(args, u_long);
                }
                ভঙ্গ;

            পক্ষ 'D':
                যদি (sign) {
                    i64 = (int64_t) va_arg(args, int32_t);
                } অন্যথা {
                    ui64 = (uint64_t) va_arg(args, uint32_t);
                }
                ভঙ্গ;

            পক্ষ 'L':
                যদি (sign) {
                    i64 = va_arg(args, int64_t);
                } অন্যথা {
                    ui64 = va_arg(args, uint64_t);
                }
                ভঙ্গ;

            পক্ষ 'A':
                যদি (sign) {
                    i64 = (int64_t) va_arg(args, ngx_atomic_int_t);
                } অন্যথা {
                    ui64 = (uint64_t) va_arg(args, ngx_atomic_uint_t);
                }

                যদি (max_width) {
                    width = NGX_ATOMIC_T_LEN;
                }

                ভঙ্গ;

            পক্ষ 'f':
                f = va_arg(args, দ্ৱিগুণ);

                যদি (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                যদি (frac_width) {

                    scale = 1;
                    বাবে (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (দ্ৱিগুণ) ui64) * scale + 0.5);

                    যদি (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_অঙ্ক(buf, last, ui64, zero, 0, width);

                যদি (frac_width) {
                    যদি (buf < last) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_অঙ্ক(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                অনুৱর্ত;

#যদি !(NGX_WIN32)
            পক্ষ 'r':
                i64 = (int64_t) va_arg(args, rlim_t);
                sign = 1;
                ভঙ্গ;
#endif

            পক্ষ 'p':
                ui64 = (uintptr_t) va_arg(args, শূন্য *);
                hex = 2;
                sign = 0;
                zero = '0';
                width = 2 * পরিমাণ(শূন্য *);
                ভঙ্গ;

            পক্ষ 'c':
                d = va_arg(args, পূর্ণাঙ্ক);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                অনুৱর্ত;

            পক্ষ 'Z':
                *buf++ = '\0';
                fmt++;

                অনুৱর্ত;

            পক্ষ 'N':
#যদি (NGX_WIN32)
                *buf++ = CR;
                যদি (buf < last) {
                    *buf++ = LF;
                }
#অন্যথা
                *buf++ = LF;
#endif
                fmt++;

                অনুৱর্ত;

            পক্ষ '%':
                *buf++ = '%';
                fmt++;

                অনুৱর্ত;

            স্ৱভাৱ:
                *buf++ = *fmt++;

                অনুৱর্ত;
            }

            যদি (sign) {
                যদি (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } অন্যথা {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_অঙ্ক(buf, last, ui64, zero, hex, width);

            fmt++;

        } অন্যথা {
            *buf++ = *fmt++;
        }
    }

    ঘূৰাই buf;
}


স্থির u_char *
ngx_sprintf_অঙ্ক(u_char *buf, u_char *last, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t width)
{
    u_char         *p, temp[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    size_t          len;
    uint32_t        ui32;
    স্থির u_char   hex[] = "0123456789abcdef";
    স্থির u_char   HEX[] = "0123456789ABCDEF";

    p = temp + NGX_INT64_LEN;

    যদি (hexadecimal == 0) {

        যদি (ui64 <= (uint64_t) NGX_MAX_UINT32_মান) {

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

            কৰা {
                *--p = (u_char) (ui32 % 10 + '0');
            } যেতিয়ালৈ (ui32 /= 10);

        } অন্যথা {
            কৰা {
                *--p = (u_char) (ui64 % 10 + '0');
            } যেতিয়ালৈ (ui64 /= 10);
        }

    } অন্যথা যদি (hexadecimal == 1) {

        কৰা {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } যেতিয়ালৈ (ui64 >>= 4);

    } অন্যথা { /* hexadecimal == 2 */

        কৰা {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } যেতিয়ালৈ (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    যেতিয়ালৈ (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    যদি (buf + len > last) {
        len = last - buf;
    }

    ঘূৰাই ngx_cpymem(buf, p, len);
}


স্থির u_char *
ngx_sprintf_str(u_char *buf, u_char *last, u_char *src, size_t len,
    ngx_uint_t hexadecimal)
{
    স্থির u_char   hex[] = "0123456789abcdef";
    স্থির u_char   HEX[] = "0123456789ABCDEF";

    যদি (hexadecimal == 0) {

        যদি (len == (size_t) -1) {
            যেতিয়ালৈ (*src && buf < last) {
                *buf++ = *src++;
            }

        } অন্যথা {
            len = ngx_min((size_t) (last - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } অন্যথা যদি (hexadecimal == 1) {

        যদি (len == (size_t) -1) {

            যেতিয়ালৈ (*src && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } অন্যথা {

            যেতিয়ালৈ (len-- && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } অন্যথা { /* hexadecimal == 2 */

        যদি (len == (size_t) -1) {

            যেতিয়ালৈ (*src && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } অন্যথা {

            যেতিয়ালৈ (len-- && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    ঘূৰাই buf;
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

    বাবে ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        যদি (c1 == c2) {

            যদি (c1) {
                অনুৱর্ত;
            }

            ঘূৰাই 0;
        }

        ঘূৰাই c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    যেতিয়ালৈ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        যদি (c1 == c2) {

            যদি (c1) {
                n--;
                অনুৱর্ত;
            }

            ঘূৰাই 0;
        }

        ঘূৰাই c1 - c2;
    }

    ঘূৰাই 0;
}


u_char *
ngx_strnstr(u_char *s1, ৱর্ণ *s2, size_t len)
{
    u_char  c1, c2;
    size_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    কৰা {
        কৰা {
            যদি (len-- == 0) {
                ঘূৰাই রিক্ত;
            }

            c1 = *s1++;

            যদি (c1 == 0) {
                ঘূৰাই রিক্ত;
            }

        } যেতিয়ালৈ (c1 != c2);

        যদি (n > len) {
            ঘূৰাই রিক্ত;
        }

    } যেতিয়ালৈ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ঘূৰাই --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, ৱর্ণ *s2, size_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    কৰা {
        কৰা {
            c1 = *s1++;

            যদি (c1 == 0) {
                ঘূৰাই রিক্ত;
            }

        } যেতিয়ালৈ (c1 != c2);

    } যেতিয়ালৈ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    ঘূৰাই --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, ৱর্ণ *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    কৰা {
        কৰা {
            c1 = (ngx_uint_t) *s1++;

            যদি (c1 == 0) {
                ঘূৰাই রিক্ত;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } যেতিয়ালৈ (c1 != c2);

    } যেতিয়ালৈ (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    ঘূৰাই --s1;
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

    কৰা {
        কৰা {
            যদি (s1 >= last) {
                ঘূৰাই রিক্ত;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } যেতিয়ালৈ (c1 != c2);

    } যেতিয়ালৈ (ngx_strncasecmp(s1, s2, n) != 0);

    ঘূৰাই --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, size_t n)
{
    যদি (n == 0) {
        ঘূৰাই 0;
    }

    n--;

    বাবে ( ;; ) {
        যদি (s1[n] != s2[n]) {
            ঘূৰাই s1[n] - s2[n];
        }

        যদি (n == 0) {
            ঘূৰাই 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n)
{
    u_char  c1, c2;

    যদি (n == 0) {
        ঘূৰাই 0;
    }

    n--;

    বাবে ( ;; ) {
        c1 = s1[n];
        যদি (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        যদি (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        যদি (c1 != c2) {
            ঘূৰাই c1 - c2;
        }

        যদি (n == 0) {
            ঘূৰাই 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2)
{
    size_t     n;
    ngx_int_t  m, z;

    যদি (n1 <= n2) {
        n = n1;
        z = -1;

    } অন্যথা {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    যদি (m || n1 == n2) {
        ঘূৰাই m;
    }

    ঘূৰাই z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    বাবে ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        যদি (c1 == c2) {

            যদি (c1) {
                অনুৱর্ত;
            }

            ঘূৰাই 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        ঘূৰাই c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    যেতিয়ালৈ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#যদি (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#endif

        যদি (c1 == c2) {

            যদি (c1) {
                n--;
                অনুৱর্ত;
            }

            ঘূৰাই 0;
        }

        /* we need '/' to be the lowest character */

        যদি (c1 == 0 || c2 == 0) {
            ঘূৰাই c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        ঘূৰাই c1 - c2;
    }

    ঘূৰাই 0;
}


ngx_int_t
ngx_atoi(u_char *সাৰি, size_t n)
{
    ngx_int_t  মান, cutoff, cutlim;

    যদি (n == 0) {
        ঘূৰাই NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_মান / 10;
    cutlim = NGX_MAX_INT_T_মান % 10;

    বাবে (মান = 0; n--; সাৰি++) {
        যদি (*সাৰি < '0' || *সাৰি > '9') {
            ঘূৰাই NGX_ERROR;
        }

        যদি (মান >= cutoff && (মান > cutoff || *সাৰি - '0' > cutlim)) {
            ঘূৰাই NGX_ERROR;
        }

        মান = মান * 10 + (*সাৰি - '0');
    }

    ঘূৰাই মান;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *সাৰি, size_t n, size_t point)
{
    ngx_int_t   মান, cutoff, cutlim;
    ngx_uint_t  dot;

    যদি (n == 0) {
        ঘূৰাই NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_মান / 10;
    cutlim = NGX_MAX_INT_T_মান % 10;

    dot = 0;

    বাবে (মান = 0; n--; সাৰি++) {

        যদি (point == 0) {
            ঘূৰাই NGX_ERROR;
        }

        যদি (*সাৰি == '.') {
            যদি (dot) {
                ঘূৰাই NGX_ERROR;
            }

            dot = 1;
            অনুৱর্ত;
        }

        যদি (*সাৰি < '0' || *সাৰি > '9') {
            ঘূৰাই NGX_ERROR;
        }

        যদি (মান >= cutoff && (মান > cutoff || *সাৰি - '0' > cutlim)) {
            ঘূৰাই NGX_ERROR;
        }

        মান = মান * 10 + (*সাৰি - '0');
        point -= dot;
    }

    যেতিয়ালৈ (point--) {
        যদি (মান > cutoff) {
            ঘূৰাই NGX_ERROR;
        }

        মান = মান * 10;
    }

    ঘূৰাই মান;
}


ssize_t
ngx_atosz(u_char *সাৰি, size_t n)
{
    ssize_t  মান, cutoff, cutlim;

    যদি (n == 0) {
        ঘূৰাই NGX_ERROR;
    }

    cutoff = NGX_MAX_SIZE_T_মান / 10;
    cutlim = NGX_MAX_SIZE_T_মান % 10;

    বাবে (মান = 0; n--; সাৰি++) {
        যদি (*সাৰি < '0' || *সাৰি > '9') {
            ঘূৰাই NGX_ERROR;
        }

        যদি (মান >= cutoff && (মান > cutoff || *সাৰি - '0' > cutlim)) {
            ঘূৰাই NGX_ERROR;
        }

        মান = মান * 10 + (*সাৰি - '0');
    }

    ঘূৰাই মান;
}


off_t
ngx_atoof(u_char *সাৰি, size_t n)
{
    off_t  মান, cutoff, cutlim;

    যদি (n == 0) {
        ঘূৰাই NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_মান / 10;
    cutlim = NGX_MAX_OFF_T_মান % 10;

    বাবে (মান = 0; n--; সাৰি++) {
        যদি (*সাৰি < '0' || *সাৰি > '9') {
            ঘূৰাই NGX_ERROR;
        }

        যদি (মান >= cutoff && (মান > cutoff || *সাৰি - '0' > cutlim)) {
            ঘূৰাই NGX_ERROR;
        }

        মান = মান * 10 + (*সাৰি - '0');
    }

    ঘূৰাই মান;
}


time_t
ngx_atotm(u_char *সাৰি, size_t n)
{
    time_t  মান, cutoff, cutlim;

    যদি (n == 0) {
        ঘূৰাই NGX_ERROR;
    }

    cutoff = NGX_MAX_TIME_T_মান / 10;
    cutlim = NGX_MAX_TIME_T_মান % 10;

    বাবে (মান = 0; n--; সাৰি++) {
        যদি (*সাৰি < '0' || *সাৰি > '9') {
            ঘূৰাই NGX_ERROR;
        }

        যদি (মান >= cutoff && (মান > cutoff || *সাৰি - '0' > cutlim)) {
            ঘূৰাই NGX_ERROR;
        }

        মান = মান * 10 + (*সাৰি - '0');
    }

    ঘূৰাই মান;
}


ngx_int_t
ngx_hextoi(u_char *সাৰি, size_t n)
{
    u_char     c, ch;
    ngx_int_t  মান, cutoff;

    যদি (n == 0) {
        ঘূৰাই NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_মান / 16;

    বাবে (মান = 0; n--; সাৰি++) {
        যদি (মান > cutoff) {
            ঘূৰাই NGX_ERROR;
        }

        ch = *সাৰি;

        যদি (ch >= '0' && ch <= '9') {
            মান = মান * 16 + (ch - '0');
            অনুৱর্ত;
        }

        c = (u_char) (ch | 0x20);

        যদি (c >= 'a' && c <= 'f') {
            মান = মান * 16 + (c - 'a' + 10);
            অনুৱর্ত;
        }

        ঘূৰাই NGX_ERROR;
    }

    ঘূৰাই মান;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, size_t len)
{
    স্থির u_char  hex[] = "0123456789abcdef";

    যেতিয়ালৈ (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    ঘূৰাই dst;
}


শূন্য
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    স্থির u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_internal(dst, src, basis64, 1);
}


শূন্য
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    স্থির u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_internal(dst, src, basis64, 0);
}


স্থির শূন্য
ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src, ধ্রুৱ u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    size_t          len;

    len = src->len;
    s = src->data;
    d = dst->data;

    যেতিয়ালৈ (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    যদি (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        যদি (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            যদি (padding) {
                *d++ = '=';
            }

        } অন্যথা {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        যদি (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->data;
}


ngx_int_t
ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    স্থির u_char   basis64[] = {
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

    ঘূৰাই ngx_decode_base64_internal(dst, src, basis64);
}


ngx_int_t
ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    স্থির u_char   basis64[] = {
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

    ঘূৰাই ngx_decode_base64_internal(dst, src, basis64);
}


স্থির ngx_int_t
ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src, ধ্রুৱ u_char *basis)
{
    size_t          len;
    u_char         *d, *s;

    বাবে (len = 0; len < src->len; len++) {
        যদি (src->data[len] == '=') {
            ভঙ্গ;
        }

        যদি (basis[src->data[len]] == 77) {
            ঘূৰাই NGX_ERROR;
        }
    }

    যদি (len % 4 == 1) {
        ঘূৰাই NGX_ERROR;
    }

    s = src->data;
    d = dst->data;

    যেতিয়ালৈ (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    যদি (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    যদি (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->data;

    ঘূৰাই NGX_OK;
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

    যদি (u >= 0xf8) {

        (*p)++;
        ঘূৰাই 0xffffffff;

    } অন্যথা যদি (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } অন্যথা যদি (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } অন্যথা যদি (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } অন্যথা {
        (*p)++;
        ঘূৰাই 0xffffffff;
    }

    যদি (n - 1 < len) {
        ঘূৰাই 0xfffffffe;
    }

    (*p)++;

    যেতিয়ালৈ (len) {
        i = *(*p)++;

        যদি (i < 0x80) {
            ঘূৰাই 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    যদি (u > valid) {
        ঘূৰাই u;
    }

    ঘূৰাই 0xffffffff;
}


size_t
ngx_utf8_length(u_char *p, size_t n)
{
    u_char  c, *last;
    size_t  len;

    last = p + n;

    বাবে (len = 0; p < last; len++) {

        c = *p;

        যদি (c < 0x80) {
            p++;
            অনুৱর্ত;
        }

        যদি (ngx_utf8_decode(&p, last - p) > 0x10ffff) {
            /* invalid UTF-8 */
            ঘূৰাই n;
        }
    }

    ঘূৰাই len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len)
{
    u_char  c, *next;

    যদি (n == 0) {
        ঘূৰাই dst;
    }

    যেতিয়ালৈ (--n) {

        c = *src;
        *dst = c;

        যদি (c < 0x80) {

            যদি (c != '\0') {
                dst++;
                src++;
                len--;

                অনুৱর্ত;
            }

            ঘূৰাই dst;
        }

        next = src;

        যদি (ngx_utf8_decode(&next, len) > 0x10ffff) {
            /* invalid UTF-8 */
            ভঙ্গ;
        }

        যেতিয়ালৈ (src < next) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    ঘূৰাই dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, size_t size, ngx_uint_t type)
{
    u_char          prefix;
    uint32_t       *escape;
    ngx_uint_t      n;
    স্থির u_char   hex[] = "0123456789ABCDEF";

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

    স্থির uint32_t   uri[] = {
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

    স্থির uint32_t   args[] = {
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

    স্থির uint32_t   uri_component[] = {
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

    স্থির uint32_t   html[] = {
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

    স্থির uint32_t   refresh[] = {
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

    স্থির uint32_t   memcached[] = {
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

    স্থির uint32_t   mail_xtext[] = {
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

    স্থির uint32_t  *map[] =
        { uri, args, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    স্থির u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    prefix = map_char[type];

    যদি (dst == রিক্ত) {

        /* find the number of the characters to be escaped */

        n = 0;

        যেতিয়ালৈ (size) {
            যদি (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            size--;
        }

        ঘূৰাই (uintptr_t) n;
    }

    যেতিয়ালৈ (size) {
        যদি (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = prefix;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } অন্যথা {
            *dst++ = *src++;
        }
        size--;
    }

    ঘূৰাই (uintptr_t) dst;
}


শূন্য
ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    গণনা {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_second
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    যেতিয়ালৈ (size--) {

        ch = *s++;

        ৱিকল্প (state) {
        পক্ষ sw_usual:
            যদি (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                গচ্ছ done;
            }

            যদি (ch == '%') {
                state = sw_quoted;
                ভঙ্গ;
            }

            *d++ = ch;
            ভঙ্গ;

        পক্ষ sw_quoted:

            যদি (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_second;
                ভঙ্গ;
            }

            c = (u_char) (ch | 0x20);
            যদি (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_second;
                ভঙ্গ;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            ভঙ্গ;

        পক্ষ sw_quoted_second:

            state = sw_usual;

            যদি (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                যদি (type & NGX_UNESCAPE_REDIRECT) {
                    যদি (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        ভঙ্গ;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    ভঙ্গ;
                }

                *d++ = ch;

                ভঙ্গ;
            }

            c = (u_char) (ch | 0x20);
            যদি (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                যদি (type & NGX_UNESCAPE_URI) {
                    যদি (ch == '?') {
                        *d++ = ch;
                        গচ্ছ done;
                    }

                    *d++ = ch;
                    ভঙ্গ;
                }

                যদি (type & NGX_UNESCAPE_REDIRECT) {
                    যদি (ch == '?') {
                        *d++ = ch;
                        গচ্ছ done;
                    }

                    যদি (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        ভঙ্গ;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    ভঙ্গ;
                }

                *d++ = ch;

                ভঙ্গ;
            }

            /* the invalid quoted character */

            ভঙ্গ;
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

    যদি (dst == রিক্ত) {

        len = 0;

        যেতিয়ালৈ (size) {
            ৱিকল্প (*src++) {

            পক্ষ '<':
                len += পরিমাণ("&lt;") - 2;
                ভঙ্গ;

            পক্ষ '>':
                len += পরিমাণ("&gt;") - 2;
                ভঙ্গ;

            পক্ষ '&':
                len += পরিমাণ("&amp;") - 2;
                ভঙ্গ;

            পক্ষ '"':
                len += পরিমাণ("&quot;") - 2;
                ভঙ্গ;

            স্ৱভাৱ:
                ভঙ্গ;
            }
            size--;
        }

        ঘূৰাই (uintptr_t) len;
    }

    যেতিয়ালৈ (size) {
        ch = *src++;

        ৱিকল্প (ch) {

        পক্ষ '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            ভঙ্গ;

        পক্ষ '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            ভঙ্গ;

        পক্ষ '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            ভঙ্গ;

        পক্ষ '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            ভঙ্গ;

        স্ৱভাৱ:
            *dst++ = ch;
            ভঙ্গ;
        }
        size--;
    }

    ঘূৰাই (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, size_t size)
{
    u_char      ch;
    ngx_uint_t  len;

    যদি (dst == রিক্ত) {
        len = 0;

        যেতিয়ালৈ (size) {
            ch = *src++;

            যদি (ch == '\\' || ch == '"') {
                len++;

            } অন্যথা যদি (ch <= 0x1f) {

                ৱিকল্প (ch) {
                পক্ষ '\n':
                পক্ষ '\r':
                পক্ষ '\t':
                পক্ষ '\b':
                পক্ষ '\f':
                    len++;
                    ভঙ্গ;

                স্ৱভাৱ:
                    len += পরিমাণ("\\u001F") - 2;
                }
            }

            size--;
        }

        ঘূৰাই (uintptr_t) len;
    }

    যেতিয়ালৈ (size) {
        ch = *src++;

        যদি (ch > 0x1f) {

            যদি (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } অন্যথা {
            *dst++ = '\\';

            ৱিকল্প (ch) {
            পক্ষ '\n':
                *dst++ = 'n';
                ভঙ্গ;

            পক্ষ '\r':
                *dst++ = 'r';
                ভঙ্গ;

            পক্ষ '\t':
                *dst++ = 't';
                ভঙ্গ;

            পক্ষ '\b':
                *dst++ = 'b';
                ভঙ্গ;

            পক্ষ '\f':
                *dst++ = 'f';
                ভঙ্গ;

            স্ৱভাৱ:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        size--;
    }

    ঘূৰাই (uintptr_t) dst;
}


শূন্য
ngx_str_rbtree_ঢোকাও_মান(ngx_rbtree_গ্ৰন্থি_t *temp,
    ngx_rbtree_গ্ৰন্থি_t *গ্ৰন্থি, ngx_rbtree_গ্ৰন্থি_t *sentinel)
{
    ngx_str_গ্ৰন্থি_t      *n, *t;
    ngx_rbtree_গ্ৰন্থি_t  **p;

    বাবে ( ;; ) {

        n = (ngx_str_গ্ৰন্থি_t *) গ্ৰন্থি;
        t = (ngx_str_গ্ৰন্থি_t *) temp;

        যদি (গ্ৰন্থি->চাবি != temp->চাবি) {

            p = (গ্ৰন্থি->চাবি < temp->চাবি) ? &temp->বাম : &temp->ডান;

        } অন্যথা যদি (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &temp->বাম : &temp->ডান;

        } অন্যথা {
            p = (ngx_memcmp(n->str.data, t->str.data, n->str.len) < 0)
                 ? &temp->বাম : &temp->ডান;
        }

        যদি (*p == sentinel) {
            ভঙ্গ;
        }

        temp = *p;
    }

    *p = গ্ৰন্থি;
    গ্ৰন্থি->parent = temp;
    গ্ৰন্থি->বাম = sentinel;
    গ্ৰন্থি->ডান = sentinel;
    ngx_rbt_red(গ্ৰন্থি);
}


ngx_str_গ্ৰন্থি_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_গ্ৰন্থি_t     *n;
    ngx_rbtree_গ্ৰন্থি_t  *গ্ৰন্থি, *sentinel;

    গ্ৰন্থি = rbtree->মূল;
    sentinel = rbtree->sentinel;

    যেতিয়ালৈ (গ্ৰন্থি != sentinel) {

        n = (ngx_str_গ্ৰন্থি_t *) গ্ৰন্থি;

        যদি (hash != গ্ৰন্থি->চাবি) {
            গ্ৰন্থি = (hash < গ্ৰন্থি->চাবি) ? গ্ৰন্থি->বাম : গ্ৰন্থি->ডান;
            অনুৱর্ত;
        }

        যদি (val->len != n->str.len) {
            গ্ৰন্থি = (val->len < n->str.len) ? গ্ৰন্থি->বাম : গ্ৰন্থি->ডান;
            অনুৱর্ত;
        }

        rc = ngx_memcmp(val->data, n->str.data, val->len);

        যদি (rc < 0) {
            গ্ৰন্থি = গ্ৰন্থি->বাম;
            অনুৱর্ত;
        }

        যদি (rc > 0) {
            গ্ৰন্থি = গ্ৰন্থি->ডান;
            অনুৱর্ত;
        }

        ঘূৰাই n;
    }

    ঘূৰাই রিক্ত;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

শূন্য
ngx_sort(শূন্য *base, size_t n, size_t size,
    ngx_int_t (*cmp)(ধ্রুৱ শূন্য *, ধ্রুৱ শূন্য *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(size, ngx_cycle->log);
    যদি (p == রিক্ত) {
        ঘূৰাই;
    }

    বাবে (p1 = (u_char *) base + size;
         p1 < (u_char *) base + n * size;
         p1 += size)
    {
        ngx_memcpy(p, p1, size);

        বাবে (p2 = p1;
             p2 > (u_char *) base && cmp(p2 - size, p) > 0;
             p2 -= size)
        {
            ngx_memcpy(p2, p2 - size, size);
        }

        ngx_memcpy(p2, p, size);
    }

    ngx_free(p);
}


শূন্য
ngx_explicit_memzero(শূন্য *buf, size_t n)
{
    ngx_memzero(buf, n);
    ngx_memory_barrier();
}


#যদি (NGX_MEMCPY_LIMIT)

শূন্য *
ngx_memcpy(শূন্য *dst, ধ্রুৱ শূন্য *src, size_t n)
{
    যদি (n > NGX_MEMCPY_LIMIT) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "memcpy %uz bytes", n);
        ngx_debug_point();
    }

    ঘূৰাই memcpy(dst, src, n);
}

#endif
