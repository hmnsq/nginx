
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ساکن u_char *ngx_sprintf_عدد(u_char *buf, u_char *آخری, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t چوڑائی);
ساکن u_char *ngx_sprintf_str(u_char *buf, u_char *آخری, u_char *src,
    سائز_t len, ngx_uint_t hexadecimal);
ساکن خالی ngx_encode_base64_اندرونی(ngx_str_t *dst, ngx_str_t *src,
    ثابت u_char *basis, ngx_uint_t padding);
ساکن ngx_int_t ngx_decode_base64_اندرونی(ngx_str_t *dst, ngx_str_t *src,
    ثابت u_char *basis);


خالی
ngx_strlow(u_char *dst, u_char *src, سائز_t n)
{
    جب (n) {
        *dst = ngx_tolower(*src);
        dst++;
        src++;
        n--;
    }
}


سائز_t
ngx_strnlen(u_char *p, سائز_t n)
{
    سائز_t  i;

    برائے (i = 0; i < n; i++) {

        اگر (p[i] == '\0') {
            واپس i;
        }
    }

    واپس n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, سائز_t n)
{
    اگر (n == 0) {
        واپس dst;
    }

    جب (--n) {
        *dst = *src;

        اگر (*dst == '\0') {
            واپس dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    واپس dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    اگر (dst == تہی) {
        واپس تہی;
    }

    ngx_memcpy(dst, src->ڈیٹا, src->len);

    واپس dst;
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
ngx_sprintf(u_char *buf, ثابت حرف *fmt, ...)
{
    u_char   *p;
    va_list   دلائل;

    va_آغاز(دلائل, fmt);
    p = ngx_vslprintf(buf, (خالی *) -1, fmt, دلائل);
    va_انتہا(دلائل);

    واپس p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, سائز_t max, ثابت حرف *fmt, ...)
{
    u_char   *p;
    va_list   دلائل;

    va_آغاز(دلائل, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, دلائل);
    va_انتہا(دلائل);

    واپس p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *آخری, ثابت حرف *fmt, ...)
{
    u_char   *p;
    va_list   دلائل;

    va_آغاز(دلائل, fmt);
    p = ngx_vslprintf(buf, آخری, fmt, دلائل);
    va_انتہا(دلائل);

    واپس p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *آخری, ثابت حرف *fmt, va_list دلائل)
{
    u_char                *p, zero;
    صحیح                    d;
    دوہرا                 f;
    سائز_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             چوڑائی, sign, hex, max_چوڑائی, frac_چوڑائی, scale, n;
    ngx_str_t             *v;
    ngx_variable_قدر_t  *vv;

    جب (*fmt && buf < آخری) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        اگر (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            چوڑائی = 0;
            sign = 1;
            hex = 0;
            max_چوڑائی = 0;
            frac_چوڑائی = 0;
            slen = (سائز_t) -1;

            جب (*fmt >= '0' && *fmt <= '9') {
                چوڑائی = چوڑائی * 10 + (*fmt++ - '0');
            }


            برائے ( ;; ) {
                انتخاب (*fmt) {

                حالت 'u':
                    sign = 0;
                    fmt++;
                    جاری;

                حالت 'm':
                    max_چوڑائی = 1;
                    fmt++;
                    جاری;

                حالت 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    جاری;

                حالت 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    جاری;

                حالت '.':
                    fmt++;

                    جب (*fmt >= '0' && *fmt <= '9') {
                        frac_چوڑائی = frac_چوڑائی * 10 + (*fmt++ - '0');
                    }

                    توڑو;

                حالت '*':
                    slen = va_arg(دلائل, سائز_t);
                    fmt++;
                    جاری;

                طےشدہ:
                    توڑو;
                }

                توڑو;
            }


            انتخاب (*fmt) {

            حالت 'V':
                v = va_arg(دلائل, ngx_str_t *);

                buf = ngx_sprintf_str(buf, آخری, v->ڈیٹا, v->len, hex);
                fmt++;

                جاری;

            حالت 'v':
                vv = va_arg(دلائل, ngx_variable_قدر_t *);

                buf = ngx_sprintf_str(buf, آخری, vv->ڈیٹا, vv->len, hex);
                fmt++;

                جاری;

            حالت 's':
                p = va_arg(دلائل, u_char *);

                buf = ngx_sprintf_str(buf, آخری, p, slen, hex);
                fmt++;

                جاری;

            حالت 'O':
                i64 = (int64_t) va_arg(دلائل, off_t);
                sign = 1;
                توڑو;

            حالت 'P':
                i64 = (int64_t) va_arg(دلائل, ngx_pid_t);
                sign = 1;
                توڑو;

            حالت 'T':
                i64 = (int64_t) va_arg(دلائل, وقت_t);
                sign = 1;
                توڑو;

            حالت 'M':
                ms = (ngx_msec_t) va_arg(دلائل, ngx_msec_t);
                اگر ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } ورنہ {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                توڑو;

            حالت 'z':
                اگر (sign) {
                    i64 = (int64_t) va_arg(دلائل, ssize_t);
                } ورنہ {
                    ui64 = (uint64_t) va_arg(دلائل, سائز_t);
                }
                توڑو;

            حالت 'i':
                اگر (sign) {
                    i64 = (int64_t) va_arg(دلائل, ngx_int_t);
                } ورنہ {
                    ui64 = (uint64_t) va_arg(دلائل, ngx_uint_t);
                }

                اگر (max_چوڑائی) {
                    چوڑائی = NGX_INT_T_LEN;
                }

                توڑو;

            حالت 'd':
                اگر (sign) {
                    i64 = (int64_t) va_arg(دلائل, صحیح);
                } ورنہ {
                    ui64 = (uint64_t) va_arg(دلائل, u_int);
                }
                توڑو;

            حالت 'l':
                اگر (sign) {
                    i64 = (int64_t) va_arg(دلائل, طویل);
                } ورنہ {
                    ui64 = (uint64_t) va_arg(دلائل, u_long);
                }
                توڑو;

            حالت 'D':
                اگر (sign) {
                    i64 = (int64_t) va_arg(دلائل, int32_t);
                } ورنہ {
                    ui64 = (uint64_t) va_arg(دلائل, uint32_t);
                }
                توڑو;

            حالت 'L':
                اگر (sign) {
                    i64 = va_arg(دلائل, int64_t);
                } ورنہ {
                    ui64 = va_arg(دلائل, uint64_t);
                }
                توڑو;

            حالت 'A':
                اگر (sign) {
                    i64 = (int64_t) va_arg(دلائل, ngx_atomic_int_t);
                } ورنہ {
                    ui64 = (uint64_t) va_arg(دلائل, ngx_atomic_uint_t);
                }

                اگر (max_چوڑائی) {
                    چوڑائی = NGX_ATOMIC_T_LEN;
                }

                توڑو;

            حالت 'f':
                f = va_arg(دلائل, دوہرا);

                اگر (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                اگر (frac_چوڑائی) {

                    scale = 1;
                    برائے (n = frac_چوڑائی; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (دوہرا) ui64) * scale + 0.5);

                    اگر (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_عدد(buf, آخری, ui64, zero, 0, چوڑائی);

                اگر (frac_چوڑائی) {
                    اگر (buf < آخری) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_عدد(buf, آخری, frac, '0', 0, frac_چوڑائی);
                }

                fmt++;

                جاری;

#اگر !(NGX_WIN32)
            حالت 'r':
                i64 = (int64_t) va_arg(دلائل, rlim_t);
                sign = 1;
                توڑو;
#اگر_ختم

            حالت 'p':
                ui64 = (uintptr_t) va_arg(دلائل, خالی *);
                hex = 2;
                sign = 0;
                zero = '0';
                چوڑائی = 2 * حجم(خالی *);
                توڑو;

            حالت 'c':
                d = va_arg(دلائل, صحیح);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                جاری;

            حالت 'Z':
                *buf++ = '\0';
                fmt++;

                جاری;

            حالت 'N':
#اگر (NGX_WIN32)
                *buf++ = CR;
                اگر (buf < آخری) {
                    *buf++ = LF;
                }
#ورنہ
                *buf++ = LF;
#اگر_ختم
                fmt++;

                جاری;

            حالت '%':
                *buf++ = '%';
                fmt++;

                جاری;

            طےشدہ:
                *buf++ = *fmt++;

                جاری;
            }

            اگر (sign) {
                اگر (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } ورنہ {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_عدد(buf, آخری, ui64, zero, hex, چوڑائی);

            fmt++;

        } ورنہ {
            *buf++ = *fmt++;
        }
    }

    واپس buf;
}


ساکن u_char *
ngx_sprintf_عدد(u_char *buf, u_char *آخری, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t چوڑائی)
{
    u_char         *p, عارضی[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    سائز_t          len;
    uint32_t        ui32;
    ساکن u_char   hex[] = "0123456789abcdef";
    ساکن u_char   HEX[] = "0123456789ABCDEF";

    p = عارضی + NGX_INT64_LEN;

    اگر (hexadecimal == 0) {

        اگر (ui64 <= (uint64_t) NGX_MAX_UINT32_قدر) {

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

            کرو {
                *--p = (u_char) (ui32 % 10 + '0');
            } جب (ui32 /= 10);

        } ورنہ {
            کرو {
                *--p = (u_char) (ui64 % 10 + '0');
            } جب (ui64 /= 10);
        }

    } ورنہ اگر (hexadecimal == 1) {

        کرو {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } جب (ui64 >>= 4);

    } ورنہ { /* hexadecimal == 2 */

        کرو {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } جب (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (عارضی + NGX_INT64_LEN) - p;

    جب (len++ < چوڑائی && buf < آخری) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (عارضی + NGX_INT64_LEN) - p;

    اگر (buf + len > آخری) {
        len = آخری - buf;
    }

    واپس ngx_cpymem(buf, p, len);
}


ساکن u_char *
ngx_sprintf_str(u_char *buf, u_char *آخری, u_char *src, سائز_t len,
    ngx_uint_t hexadecimal)
{
    ساکن u_char   hex[] = "0123456789abcdef";
    ساکن u_char   HEX[] = "0123456789ABCDEF";

    اگر (hexadecimal == 0) {

        اگر (len == (سائز_t) -1) {
            جب (*src && buf < آخری) {
                *buf++ = *src++;
            }

        } ورنہ {
            len = ngx_min((سائز_t) (آخری - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } ورنہ اگر (hexadecimal == 1) {

        اگر (len == (سائز_t) -1) {

            جب (*src && buf < آخری - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } ورنہ {

            جب (len-- && buf < آخری - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } ورنہ { /* hexadecimal == 2 */

        اگر (len == (سائز_t) -1) {

            جب (*src && buf < آخری - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } ورنہ {

            جب (len-- && buf < آخری - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    واپس buf;
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

    برائے ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        اگر (c1 == c2) {

            اگر (c1) {
                جاری;
            }

            واپس 0;
        }

        واپس c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, سائز_t n)
{
    ngx_uint_t  c1, c2;

    جب (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        اگر (c1 == c2) {

            اگر (c1) {
                n--;
                جاری;
            }

            واپس 0;
        }

        واپس c1 - c2;
    }

    واپس 0;
}


u_char *
ngx_strnstr(u_char *s1, حرف *s2, سائز_t len)
{
    u_char  c1, c2;
    سائز_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    کرو {
        کرو {
            اگر (len-- == 0) {
                واپس تہی;
            }

            c1 = *s1++;

            اگر (c1 == 0) {
                واپس تہی;
            }

        } جب (c1 != c2);

        اگر (n > len) {
            واپس تہی;
        }

    } جب (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    واپس --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, حرف *s2, سائز_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    کرو {
        کرو {
            c1 = *s1++;

            اگر (c1 == 0) {
                واپس تہی;
            }

        } جب (c1 != c2);

    } جب (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    واپس --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, حرف *s2, سائز_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    کرو {
        کرو {
            c1 = (ngx_uint_t) *s1++;

            اگر (c1 == 0) {
                واپس تہی;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } جب (c1 != c2);

    } جب (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    واپس --s1;
}


/*
 * ngx_strlcasestrn() is intended to search for static substring
 * with known length in string until the argument last. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strlcasestrn(u_char *s1, u_char *آخری, u_char *s2, سائز_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;
    آخری -= n;

    کرو {
        کرو {
            اگر (s1 >= آخری) {
                واپس تہی;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } جب (c1 != c2);

    } جب (ngx_strncasecmp(s1, s2, n) != 0);

    واپس --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, سائز_t n)
{
    اگر (n == 0) {
        واپس 0;
    }

    n--;

    برائے ( ;; ) {
        اگر (s1[n] != s2[n]) {
            واپس s1[n] - s2[n];
        }

        اگر (n == 0) {
            واپس 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, سائز_t n)
{
    u_char  c1, c2;

    اگر (n == 0) {
        واپس 0;
    }

    n--;

    برائے ( ;; ) {
        c1 = s1[n];
        اگر (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        اگر (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        اگر (c1 != c2) {
            واپس c1 - c2;
        }

        اگر (n == 0) {
            واپس 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, سائز_t n1, سائز_t n2)
{
    سائز_t     n;
    ngx_int_t  m, z;

    اگر (n1 <= n2) {
        n = n1;
        z = -1;

    } ورنہ {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    اگر (m || n1 == n2) {
        واپس m;
    }

    واپس z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    برائے ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        اگر (c1 == c2) {

            اگر (c1) {
                جاری;
            }

            واپس 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        واپس c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, سائز_t n)
{
    ngx_uint_t  c1, c2;

    جب (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#اگر (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#اگر_ختم

        اگر (c1 == c2) {

            اگر (c1) {
                n--;
                جاری;
            }

            واپس 0;
        }

        /* we need '/' to be the lowest character */

        اگر (c1 == 0 || c2 == 0) {
            واپس c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        واپس c1 - c2;
    }

    واپس 0;
}


ngx_int_t
ngx_atoi(u_char *سطر, سائز_t n)
{
    ngx_int_t  قدر, cutoff, cutlim;

    اگر (n == 0) {
        واپس NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_قدر / 10;
    cutlim = NGX_MAX_INT_T_قدر % 10;

    برائے (قدر = 0; n--; سطر++) {
        اگر (*سطر < '0' || *سطر > '9') {
            واپس NGX_ERROR;
        }

        اگر (قدر >= cutoff && (قدر > cutoff || *سطر - '0' > cutlim)) {
            واپس NGX_ERROR;
        }

        قدر = قدر * 10 + (*سطر - '0');
    }

    واپس قدر;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *سطر, سائز_t n, سائز_t نقطہ)
{
    ngx_int_t   قدر, cutoff, cutlim;
    ngx_uint_t  dot;

    اگر (n == 0) {
        واپس NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_قدر / 10;
    cutlim = NGX_MAX_INT_T_قدر % 10;

    dot = 0;

    برائے (قدر = 0; n--; سطر++) {

        اگر (نقطہ == 0) {
            واپس NGX_ERROR;
        }

        اگر (*سطر == '.') {
            اگر (dot) {
                واپس NGX_ERROR;
            }

            dot = 1;
            جاری;
        }

        اگر (*سطر < '0' || *سطر > '9') {
            واپس NGX_ERROR;
        }

        اگر (قدر >= cutoff && (قدر > cutoff || *سطر - '0' > cutlim)) {
            واپس NGX_ERROR;
        }

        قدر = قدر * 10 + (*سطر - '0');
        نقطہ -= dot;
    }

    جب (نقطہ--) {
        اگر (قدر > cutoff) {
            واپس NGX_ERROR;
        }

        قدر = قدر * 10;
    }

    واپس قدر;
}


ssize_t
ngx_atosz(u_char *سطر, سائز_t n)
{
    ssize_t  قدر, cutoff, cutlim;

    اگر (n == 0) {
        واپس NGX_ERROR;
    }

    cutoff = NGX_MAX_سائز_T_قدر / 10;
    cutlim = NGX_MAX_سائز_T_قدر % 10;

    برائے (قدر = 0; n--; سطر++) {
        اگر (*سطر < '0' || *سطر > '9') {
            واپس NGX_ERROR;
        }

        اگر (قدر >= cutoff && (قدر > cutoff || *سطر - '0' > cutlim)) {
            واپس NGX_ERROR;
        }

        قدر = قدر * 10 + (*سطر - '0');
    }

    واپس قدر;
}


off_t
ngx_atoof(u_char *سطر, سائز_t n)
{
    off_t  قدر, cutoff, cutlim;

    اگر (n == 0) {
        واپس NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_قدر / 10;
    cutlim = NGX_MAX_OFF_T_قدر % 10;

    برائے (قدر = 0; n--; سطر++) {
        اگر (*سطر < '0' || *سطر > '9') {
            واپس NGX_ERROR;
        }

        اگر (قدر >= cutoff && (قدر > cutoff || *سطر - '0' > cutlim)) {
            واپس NGX_ERROR;
        }

        قدر = قدر * 10 + (*سطر - '0');
    }

    واپس قدر;
}


وقت_t
ngx_atotm(u_char *سطر, سائز_t n)
{
    وقت_t  قدر, cutoff, cutlim;

    اگر (n == 0) {
        واپس NGX_ERROR;
    }

    cutoff = NGX_MAX_وقت_T_قدر / 10;
    cutlim = NGX_MAX_وقت_T_قدر % 10;

    برائے (قدر = 0; n--; سطر++) {
        اگر (*سطر < '0' || *سطر > '9') {
            واپس NGX_ERROR;
        }

        اگر (قدر >= cutoff && (قدر > cutoff || *سطر - '0' > cutlim)) {
            واپس NGX_ERROR;
        }

        قدر = قدر * 10 + (*سطر - '0');
    }

    واپس قدر;
}


ngx_int_t
ngx_hextoi(u_char *سطر, سائز_t n)
{
    u_char     c, ch;
    ngx_int_t  قدر, cutoff;

    اگر (n == 0) {
        واپس NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_قدر / 16;

    برائے (قدر = 0; n--; سطر++) {
        اگر (قدر > cutoff) {
            واپس NGX_ERROR;
        }

        ch = *سطر;

        اگر (ch >= '0' && ch <= '9') {
            قدر = قدر * 16 + (ch - '0');
            جاری;
        }

        c = (u_char) (ch | 0x20);

        اگر (c >= 'a' && c <= 'f') {
            قدر = قدر * 16 + (c - 'a' + 10);
            جاری;
        }

        واپس NGX_ERROR;
    }

    واپس قدر;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, سائز_t len)
{
    ساکن u_char  hex[] = "0123456789abcdef";

    جب (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    واپس dst;
}


خالی
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    ساکن u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_اندرونی(dst, src, basis64, 1);
}


خالی
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    ساکن u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_اندرونی(dst, src, basis64, 0);
}


ساکن خالی
ngx_encode_base64_اندرونی(ngx_str_t *dst, ngx_str_t *src, ثابت u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    سائز_t          len;

    len = src->len;
    s = src->ڈیٹا;
    d = dst->ڈیٹا;

    جب (len > 2) {
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

        } ورنہ {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        اگر (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->ڈیٹا;
}


ngx_int_t
ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    ساکن u_char   basis64[] = {
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

    واپس ngx_decode_base64_اندرونی(dst, src, basis64);
}


ngx_int_t
ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    ساکن u_char   basis64[] = {
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

    واپس ngx_decode_base64_اندرونی(dst, src, basis64);
}


ساکن ngx_int_t
ngx_decode_base64_اندرونی(ngx_str_t *dst, ngx_str_t *src, ثابت u_char *basis)
{
    سائز_t          len;
    u_char         *d, *s;

    برائے (len = 0; len < src->len; len++) {
        اگر (src->ڈیٹا[len] == '=') {
            توڑو;
        }

        اگر (basis[src->ڈیٹا[len]] == 77) {
            واپس NGX_ERROR;
        }
    }

    اگر (len % 4 == 1) {
        واپس NGX_ERROR;
    }

    s = src->ڈیٹا;
    d = dst->ڈیٹا;

    جب (len > 3) {
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

    dst->len = d - dst->ڈیٹا;

    واپس NGX_OK;
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
ngx_utf8_decode(u_char **p, سائز_t n)
{
    سائز_t    len;
    uint32_t  u, i, valid;

    u = **p;

    اگر (u >= 0xf8) {

        (*p)++;
        واپس 0xffffffff;

    } ورنہ اگر (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } ورنہ اگر (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } ورنہ اگر (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } ورنہ {
        (*p)++;
        واپس 0xffffffff;
    }

    اگر (n - 1 < len) {
        واپس 0xfffffffe;
    }

    (*p)++;

    جب (len) {
        i = *(*p)++;

        اگر (i < 0x80) {
            واپس 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    اگر (u > valid) {
        واپس u;
    }

    واپس 0xffffffff;
}


سائز_t
ngx_utf8_لمبائی(u_char *p, سائز_t n)
{
    u_char  c, *آخری;
    سائز_t  len;

    آخری = p + n;

    برائے (len = 0; p < آخری; len++) {

        c = *p;

        اگر (c < 0x80) {
            p++;
            جاری;
        }

        اگر (ngx_utf8_decode(&p, آخری - p) > 0x10ffff) {
            /* invalid UTF-8 */
            واپس n;
        }
    }

    واپس len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, سائز_t n, سائز_t len)
{
    u_char  c, *اگلا;

    اگر (n == 0) {
        واپس dst;
    }

    جب (--n) {

        c = *src;
        *dst = c;

        اگر (c < 0x80) {

            اگر (c != '\0') {
                dst++;
                src++;
                len--;

                جاری;
            }

            واپس dst;
        }

        اگلا = src;

        اگر (ngx_utf8_decode(&اگلا, len) > 0x10ffff) {
            /* invalid UTF-8 */
            توڑو;
        }

        جب (src < اگلا) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    واپس dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, سائز_t سائز, ngx_uint_t type)
{
    u_char          سابقہ;
    uint32_t       *escape;
    ngx_uint_t      n;
    ساکن u_char   hex[] = "0123456789ABCDEF";

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

    ساکن uint32_t   uri[] = {
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

    ساکن uint32_t   دلائل[] = {
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

    ساکن uint32_t   uri_component[] = {
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

    ساکن uint32_t   html[] = {
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

    ساکن uint32_t   refresh[] = {
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

    ساکن uint32_t   memcached[] = {
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

    ساکن uint32_t   mail_xtext[] = {
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

    ساکن uint32_t  *map[] =
        { uri, دلائل, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    ساکن u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    سابقہ = map_char[type];

    اگر (dst == تہی) {

        /* find the number of the characters to be escaped */

        n = 0;

        جب (سائز) {
            اگر (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            سائز--;
        }

        واپس (uintptr_t) n;
    }

    جب (سائز) {
        اگر (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = سابقہ;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } ورنہ {
            *dst++ = *src++;
        }
        سائز--;
    }

    واپس (uintptr_t) dst;
}


خالی
ngx_unescape_uri(u_char **dst, u_char **src, سائز_t سائز, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    شمار {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_دوسرا
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    جب (سائز--) {

        ch = *s++;

        انتخاب (state) {
        حالت sw_usual:
            اگر (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                جاؤ مکمل;
            }

            اگر (ch == '%') {
                state = sw_quoted;
                توڑو;
            }

            *d++ = ch;
            توڑو;

        حالت sw_quoted:

            اگر (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_دوسرا;
                توڑو;
            }

            c = (u_char) (ch | 0x20);
            اگر (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_دوسرا;
                توڑو;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            توڑو;

        حالت sw_quoted_دوسرا:

            state = sw_usual;

            اگر (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                اگر (type & NGX_UNESCAPE_REDIRECT) {
                    اگر (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        توڑو;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    توڑو;
                }

                *d++ = ch;

                توڑو;
            }

            c = (u_char) (ch | 0x20);
            اگر (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                اگر (type & NGX_UNESCAPE_URI) {
                    اگر (ch == '?') {
                        *d++ = ch;
                        جاؤ مکمل;
                    }

                    *d++ = ch;
                    توڑو;
                }

                اگر (type & NGX_UNESCAPE_REDIRECT) {
                    اگر (ch == '?') {
                        *d++ = ch;
                        جاؤ مکمل;
                    }

                    اگر (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        توڑو;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    توڑو;
                }

                *d++ = ch;

                توڑو;
            }

            /* the invalid quoted character */

            توڑو;
        }
    }

مکمل:

    *dst = d;
    *src = s;
}


uintptr_t
ngx_escape_html(u_char *dst, u_char *src, سائز_t سائز)
{
    u_char      ch;
    ngx_uint_t  len;

    اگر (dst == تہی) {

        len = 0;

        جب (سائز) {
            انتخاب (*src++) {

            حالت '<':
                len += حجم("&lt;") - 2;
                توڑو;

            حالت '>':
                len += حجم("&gt;") - 2;
                توڑو;

            حالت '&':
                len += حجم("&amp;") - 2;
                توڑو;

            حالت '"':
                len += حجم("&quot;") - 2;
                توڑو;

            طےشدہ:
                توڑو;
            }
            سائز--;
        }

        واپس (uintptr_t) len;
    }

    جب (سائز) {
        ch = *src++;

        انتخاب (ch) {

        حالت '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            توڑو;

        حالت '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            توڑو;

        حالت '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            توڑو;

        حالت '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            توڑو;

        طےشدہ:
            *dst++ = ch;
            توڑو;
        }
        سائز--;
    }

    واپس (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, سائز_t سائز)
{
    u_char      ch;
    ngx_uint_t  len;

    اگر (dst == تہی) {
        len = 0;

        جب (سائز) {
            ch = *src++;

            اگر (ch == '\\' || ch == '"') {
                len++;

            } ورنہ اگر (ch <= 0x1f) {

                انتخاب (ch) {
                حالت '\n':
                حالت '\r':
                حالت '\t':
                حالت '\b':
                حالت '\f':
                    len++;
                    توڑو;

                طےشدہ:
                    len += حجم("\\u001F") - 2;
                }
            }

            سائز--;
        }

        واپس (uintptr_t) len;
    }

    جب (سائز) {
        ch = *src++;

        اگر (ch > 0x1f) {

            اگر (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } ورنہ {
            *dst++ = '\\';

            انتخاب (ch) {
            حالت '\n':
                *dst++ = 'n';
                توڑو;

            حالت '\r':
                *dst++ = 'r';
                توڑو;

            حالت '\t':
                *dst++ = 't';
                توڑو;

            حالت '\b':
                *dst++ = 'b';
                توڑو;

            حالت '\f':
                *dst++ = 'f';
                توڑو;

            طےشدہ:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        سائز--;
    }

    واپس (uintptr_t) dst;
}


خالی
ngx_str_rbtree_داخل_کرو_قدر(ngx_rbtree_گرہ_t *عارضی,
    ngx_rbtree_گرہ_t *گرہ, ngx_rbtree_گرہ_t *sentinel)
{
    ngx_str_گرہ_t      *n, *t;
    ngx_rbtree_گرہ_t  **p;

    برائے ( ;; ) {

        n = (ngx_str_گرہ_t *) گرہ;
        t = (ngx_str_گرہ_t *) عارضی;

        اگر (گرہ->کلید != عارضی->کلید) {

            p = (گرہ->کلید < عارضی->کلید) ? &عارضی->بایاں : &عارضی->دایاں;

        } ورنہ اگر (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &عارضی->بایاں : &عارضی->دایاں;

        } ورنہ {
            p = (ngx_memcmp(n->str.ڈیٹا, t->str.ڈیٹا, n->str.len) < 0)
                 ? &عارضی->بایاں : &عارضی->دایاں;
        }

        اگر (*p == sentinel) {
            توڑو;
        }

        عارضی = *p;
    }

    *p = گرہ;
    گرہ->والد = عارضی;
    گرہ->بایاں = sentinel;
    گرہ->دایاں = sentinel;
    ngx_rbt_red(گرہ);
}


ngx_str_گرہ_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_گرہ_t     *n;
    ngx_rbtree_گرہ_t  *گرہ, *sentinel;

    گرہ = rbtree->جڑ;
    sentinel = rbtree->sentinel;

    جب (گرہ != sentinel) {

        n = (ngx_str_گرہ_t *) گرہ;

        اگر (hash != گرہ->کلید) {
            گرہ = (hash < گرہ->کلید) ? گرہ->بایاں : گرہ->دایاں;
            جاری;
        }

        اگر (val->len != n->str.len) {
            گرہ = (val->len < n->str.len) ? گرہ->بایاں : گرہ->دایاں;
            جاری;
        }

        rc = ngx_memcmp(val->ڈیٹا, n->str.ڈیٹا, val->len);

        اگر (rc < 0) {
            گرہ = گرہ->بایاں;
            جاری;
        }

        اگر (rc > 0) {
            گرہ = گرہ->دایاں;
            جاری;
        }

        واپس n;
    }

    واپس تہی;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

خالی
ngx_ترتیب_دو(خالی *بنیاد, سائز_t n, سائز_t سائز,
    ngx_int_t (*cmp)(ثابت خالی *, ثابت خالی *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(سائز, ngx_چکر->لوگارتھم);
    اگر (p == تہی) {
        واپس;
    }

    برائے (p1 = (u_char *) بنیاد + سائز;
         p1 < (u_char *) بنیاد + n * سائز;
         p1 += سائز)
    {
        ngx_memcpy(p, p1, سائز);

        برائے (p2 = p1;
             p2 > (u_char *) بنیاد && cmp(p2 - سائز, p) > 0;
             p2 -= سائز)
        {
            ngx_memcpy(p2, p2 - سائز, سائز);
        }

        ngx_memcpy(p2, p, سائز);
    }

    ngx_free(p);
}


خالی
ngx_explicit_memzero(خالی *buf, سائز_t n)
{
    ngx_memzero(buf, n);
    ngx_حافظہ_barrier();
}


#اگر (NGX_MEMCPY_حد_بندی)

خالی *
ngx_memcpy(خالی *dst, ثابت خالی *src, سائز_t n)
{
    اگر (n > NGX_MEMCPY_حد_بندی) {
        ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, 0, "memcpy %uz bytes", n);
        ngx_debug_نقطہ();
    }

    واپس حافظہ_نقل(dst, src, n);
}

#اگر_ختم
