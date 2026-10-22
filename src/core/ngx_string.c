
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن u_char *ngx_sprintf_ژمارە(u_char *buf, u_char *last, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
ساكن u_char *ngx_sprintf_str(u_char *buf, u_char *last, u_char *src,
    size_t len, ngx_uint_t hexadecimal);
ساكن فراغ ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ثابت u_char *basis, ngx_uint_t padding);
ساكن ngx_int_t ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ثابت u_char *basis);


فراغ
ngx_strlow(u_char *dst, u_char *src, size_t n)
{
    طالما (n) {
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

    لكل (i = 0; i < n; i++) {

        إذا (p[i] == '\0') {
            أرجع i;
        }
    }

    أرجع n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, size_t n)
{
    إذا (n == 0) {
        أرجع dst;
    }

    طالما (--n) {
        *dst = *src;

        إذا (*dst == '\0') {
            أرجع dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    أرجع dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    إذا (dst == عدم) {
        أرجع عدم;
    }

    ngx_memcpy(dst, src->data, src->len);

    أرجع dst;
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
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, (فراغ *) -1, fmt, args);
    va_end(args);

    أرجع p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, size_t max, ثابت حرف *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, args);
    va_end(args);

    أرجع p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *last, ثابت حرف *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, last, fmt, args);
    va_end(args);

    أرجع p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *last, ثابت حرف *fmt, va_list args)
{
    u_char                *p, zero;
    صحيح                    d;
    مزدوج                 f;
    size_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             width, sign, hex, max_width, frac_width, scale, n;
    ngx_str_t             *v;
    ngx_variable_بەها_t  *vv;

    طالما (*fmt && buf < last) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        إذا (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (size_t) -1;

            طالما (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }


            لكل ( ;; ) {
                بدل (*fmt) {

                حالة 'u':
                    sign = 0;
                    fmt++;
                    استمر;

                حالة 'm':
                    max_width = 1;
                    fmt++;
                    استمر;

                حالة 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    استمر;

                حالة 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    استمر;

                حالة '.':
                    fmt++;

                    طالما (*fmt >= '0' && *fmt <= '9') {
                        frac_width = frac_width * 10 + (*fmt++ - '0');
                    }

                    اكسر;

                حالة '*':
                    slen = va_arg(args, size_t);
                    fmt++;
                    استمر;

                افتراضي:
                    اكسر;
                }

                اكسر;
            }


            بدل (*fmt) {

            حالة 'V':
                v = va_arg(args, ngx_str_t *);

                buf = ngx_sprintf_str(buf, last, v->data, v->len, hex);
                fmt++;

                استمر;

            حالة 'v':
                vv = va_arg(args, ngx_variable_بەها_t *);

                buf = ngx_sprintf_str(buf, last, vv->data, vv->len, hex);
                fmt++;

                استمر;

            حالة 's':
                p = va_arg(args, u_char *);

                buf = ngx_sprintf_str(buf, last, p, slen, hex);
                fmt++;

                استمر;

            حالة 'O':
                i64 = (int64_t) va_arg(args, off_t);
                sign = 1;
                اكسر;

            حالة 'P':
                i64 = (int64_t) va_arg(args, ngx_pid_t);
                sign = 1;
                اكسر;

            حالة 'T':
                i64 = (int64_t) va_arg(args, time_t);
                sign = 1;
                اكسر;

            حالة 'M':
                ms = (ngx_msec_t) va_arg(args, ngx_msec_t);
                إذا ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } وإلا {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                اكسر;

            حالة 'z':
                إذا (sign) {
                    i64 = (int64_t) va_arg(args, ssize_t);
                } وإلا {
                    ui64 = (uint64_t) va_arg(args, size_t);
                }
                اكسر;

            حالة 'i':
                إذا (sign) {
                    i64 = (int64_t) va_arg(args, ngx_int_t);
                } وإلا {
                    ui64 = (uint64_t) va_arg(args, ngx_uint_t);
                }

                إذا (max_width) {
                    width = NGX_INT_T_LEN;
                }

                اكسر;

            حالة 'd':
                إذا (sign) {
                    i64 = (int64_t) va_arg(args, صحيح);
                } وإلا {
                    ui64 = (uint64_t) va_arg(args, u_int);
                }
                اكسر;

            حالة 'l':
                إذا (sign) {
                    i64 = (int64_t) va_arg(args, long);
                } وإلا {
                    ui64 = (uint64_t) va_arg(args, u_long);
                }
                اكسر;

            حالة 'D':
                إذا (sign) {
                    i64 = (int64_t) va_arg(args, int32_t);
                } وإلا {
                    ui64 = (uint64_t) va_arg(args, uint32_t);
                }
                اكسر;

            حالة 'L':
                إذا (sign) {
                    i64 = va_arg(args, int64_t);
                } وإلا {
                    ui64 = va_arg(args, uint64_t);
                }
                اكسر;

            حالة 'A':
                إذا (sign) {
                    i64 = (int64_t) va_arg(args, ngx_atomic_int_t);
                } وإلا {
                    ui64 = (uint64_t) va_arg(args, ngx_atomic_uint_t);
                }

                إذا (max_width) {
                    width = NGX_ATOMIC_T_LEN;
                }

                اكسر;

            حالة 'f':
                f = va_arg(args, مزدوج);

                إذا (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                إذا (frac_width) {

                    scale = 1;
                    لكل (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (مزدوج) ui64) * scale + 0.5);

                    إذا (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_ژمارە(buf, last, ui64, zero, 0, width);

                إذا (frac_width) {
                    إذا (buf < last) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_ژمارە(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                استمر;

#إذا !(NGX_WIN32)
            حالة 'r':
                i64 = (int64_t) va_arg(args, rlim_t);
                sign = 1;
                اكسر;
#endif

            حالة 'p':
                ui64 = (uintptr_t) va_arg(args, فراغ *);
                hex = 2;
                sign = 0;
                zero = '0';
                width = 2 * حجم(فراغ *);
                اكسر;

            حالة 'c':
                d = va_arg(args, صحيح);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                استمر;

            حالة 'Z':
                *buf++ = '\0';
                fmt++;

                استمر;

            حالة 'N':
#إذا (NGX_WIN32)
                *buf++ = CR;
                إذا (buf < last) {
                    *buf++ = LF;
                }
#وإلا
                *buf++ = LF;
#endif
                fmt++;

                استمر;

            حالة '%':
                *buf++ = '%';
                fmt++;

                استمر;

            افتراضي:
                *buf++ = *fmt++;

                استمر;
            }

            إذا (sign) {
                إذا (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } وإلا {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_ژمارە(buf, last, ui64, zero, hex, width);

            fmt++;

        } وإلا {
            *buf++ = *fmt++;
        }
    }

    أرجع buf;
}


ساكن u_char *
ngx_sprintf_ژمارە(u_char *buf, u_char *last, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t width)
{
    u_char         *p, temp[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    size_t          len;
    uint32_t        ui32;
    ساكن u_char   hex[] = "0123456789abcdef";
    ساكن u_char   HEX[] = "0123456789ABCDEF";

    p = temp + NGX_INT64_LEN;

    إذا (hexadecimal == 0) {

        إذا (ui64 <= (uint64_t) NGX_MAX_UINT32_بەها) {

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

            افعل {
                *--p = (u_char) (ui32 % 10 + '0');
            } طالما (ui32 /= 10);

        } وإلا {
            افعل {
                *--p = (u_char) (ui64 % 10 + '0');
            } طالما (ui64 /= 10);
        }

    } وإلا إذا (hexadecimal == 1) {

        افعل {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } طالما (ui64 >>= 4);

    } وإلا { /* hexadecimal == 2 */

        افعل {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } طالما (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    طالما (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    إذا (buf + len > last) {
        len = last - buf;
    }

    أرجع ngx_cpymem(buf, p, len);
}


ساكن u_char *
ngx_sprintf_str(u_char *buf, u_char *last, u_char *src, size_t len,
    ngx_uint_t hexadecimal)
{
    ساكن u_char   hex[] = "0123456789abcdef";
    ساكن u_char   HEX[] = "0123456789ABCDEF";

    إذا (hexadecimal == 0) {

        إذا (len == (size_t) -1) {
            طالما (*src && buf < last) {
                *buf++ = *src++;
            }

        } وإلا {
            len = ngx_min((size_t) (last - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } وإلا إذا (hexadecimal == 1) {

        إذا (len == (size_t) -1) {

            طالما (*src && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } وإلا {

            طالما (len-- && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } وإلا { /* hexadecimal == 2 */

        إذا (len == (size_t) -1) {

            طالما (*src && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } وإلا {

            طالما (len-- && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    أرجع buf;
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

    لكل ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        إذا (c1 == c2) {

            إذا (c1) {
                استمر;
            }

            أرجع 0;
        }

        أرجع c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    طالما (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        إذا (c1 == c2) {

            إذا (c1) {
                n--;
                استمر;
            }

            أرجع 0;
        }

        أرجع c1 - c2;
    }

    أرجع 0;
}


u_char *
ngx_strnstr(u_char *s1, حرف *s2, size_t len)
{
    u_char  c1, c2;
    size_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    افعل {
        افعل {
            إذا (len-- == 0) {
                أرجع عدم;
            }

            c1 = *s1++;

            إذا (c1 == 0) {
                أرجع عدم;
            }

        } طالما (c1 != c2);

        إذا (n > len) {
            أرجع عدم;
        }

    } طالما (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    أرجع --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, حرف *s2, size_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    افعل {
        افعل {
            c1 = *s1++;

            إذا (c1 == 0) {
                أرجع عدم;
            }

        } طالما (c1 != c2);

    } طالما (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    أرجع --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, حرف *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    افعل {
        افعل {
            c1 = (ngx_uint_t) *s1++;

            إذا (c1 == 0) {
                أرجع عدم;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } طالما (c1 != c2);

    } طالما (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    أرجع --s1;
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

    افعل {
        افعل {
            إذا (s1 >= last) {
                أرجع عدم;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } طالما (c1 != c2);

    } طالما (ngx_strncasecmp(s1, s2, n) != 0);

    أرجع --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, size_t n)
{
    إذا (n == 0) {
        أرجع 0;
    }

    n--;

    لكل ( ;; ) {
        إذا (s1[n] != s2[n]) {
            أرجع s1[n] - s2[n];
        }

        إذا (n == 0) {
            أرجع 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n)
{
    u_char  c1, c2;

    إذا (n == 0) {
        أرجع 0;
    }

    n--;

    لكل ( ;; ) {
        c1 = s1[n];
        إذا (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        إذا (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        إذا (c1 != c2) {
            أرجع c1 - c2;
        }

        إذا (n == 0) {
            أرجع 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2)
{
    size_t     n;
    ngx_int_t  m, z;

    إذا (n1 <= n2) {
        n = n1;
        z = -1;

    } وإلا {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    إذا (m || n1 == n2) {
        أرجع m;
    }

    أرجع z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    لكل ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        إذا (c1 == c2) {

            إذا (c1) {
                استمر;
            }

            أرجع 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        أرجع c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    طالما (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#إذا (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#endif

        إذا (c1 == c2) {

            إذا (c1) {
                n--;
                استمر;
            }

            أرجع 0;
        }

        /* we need '/' to be the lowest character */

        إذا (c1 == 0 || c2 == 0) {
            أرجع c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        أرجع c1 - c2;
    }

    أرجع 0;
}


ngx_int_t
ngx_atoi(u_char *هێڵ, size_t n)
{
    ngx_int_t  بەها, cutoff, cutlim;

    إذا (n == 0) {
        أرجع NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_بەها / 10;
    cutlim = NGX_MAX_INT_T_بەها % 10;

    لكل (بەها = 0; n--; هێڵ++) {
        إذا (*هێڵ < '0' || *هێڵ > '9') {
            أرجع NGX_ERROR;
        }

        إذا (بەها >= cutoff && (بەها > cutoff || *هێڵ - '0' > cutlim)) {
            أرجع NGX_ERROR;
        }

        بەها = بەها * 10 + (*هێڵ - '0');
    }

    أرجع بەها;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *هێڵ, size_t n, size_t point)
{
    ngx_int_t   بەها, cutoff, cutlim;
    ngx_uint_t  dot;

    إذا (n == 0) {
        أرجع NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_بەها / 10;
    cutlim = NGX_MAX_INT_T_بەها % 10;

    dot = 0;

    لكل (بەها = 0; n--; هێڵ++) {

        إذا (point == 0) {
            أرجع NGX_ERROR;
        }

        إذا (*هێڵ == '.') {
            إذا (dot) {
                أرجع NGX_ERROR;
            }

            dot = 1;
            استمر;
        }

        إذا (*هێڵ < '0' || *هێڵ > '9') {
            أرجع NGX_ERROR;
        }

        إذا (بەها >= cutoff && (بەها > cutoff || *هێڵ - '0' > cutlim)) {
            أرجع NGX_ERROR;
        }

        بەها = بەها * 10 + (*هێڵ - '0');
        point -= dot;
    }

    طالما (point--) {
        إذا (بەها > cutoff) {
            أرجع NGX_ERROR;
        }

        بەها = بەها * 10;
    }

    أرجع بەها;
}


ssize_t
ngx_atosz(u_char *هێڵ, size_t n)
{
    ssize_t  بەها, cutoff, cutlim;

    إذا (n == 0) {
        أرجع NGX_ERROR;
    }

    cutoff = NGX_MAX_SIZE_T_بەها / 10;
    cutlim = NGX_MAX_SIZE_T_بەها % 10;

    لكل (بەها = 0; n--; هێڵ++) {
        إذا (*هێڵ < '0' || *هێڵ > '9') {
            أرجع NGX_ERROR;
        }

        إذا (بەها >= cutoff && (بەها > cutoff || *هێڵ - '0' > cutlim)) {
            أرجع NGX_ERROR;
        }

        بەها = بەها * 10 + (*هێڵ - '0');
    }

    أرجع بەها;
}


off_t
ngx_atoof(u_char *هێڵ, size_t n)
{
    off_t  بەها, cutoff, cutlim;

    إذا (n == 0) {
        أرجع NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_بەها / 10;
    cutlim = NGX_MAX_OFF_T_بەها % 10;

    لكل (بەها = 0; n--; هێڵ++) {
        إذا (*هێڵ < '0' || *هێڵ > '9') {
            أرجع NGX_ERROR;
        }

        إذا (بەها >= cutoff && (بەها > cutoff || *هێڵ - '0' > cutlim)) {
            أرجع NGX_ERROR;
        }

        بەها = بەها * 10 + (*هێڵ - '0');
    }

    أرجع بەها;
}


time_t
ngx_atotm(u_char *هێڵ, size_t n)
{
    time_t  بەها, cutoff, cutlim;

    إذا (n == 0) {
        أرجع NGX_ERROR;
    }

    cutoff = NGX_MAX_TIME_T_بەها / 10;
    cutlim = NGX_MAX_TIME_T_بەها % 10;

    لكل (بەها = 0; n--; هێڵ++) {
        إذا (*هێڵ < '0' || *هێڵ > '9') {
            أرجع NGX_ERROR;
        }

        إذا (بەها >= cutoff && (بەها > cutoff || *هێڵ - '0' > cutlim)) {
            أرجع NGX_ERROR;
        }

        بەها = بەها * 10 + (*هێڵ - '0');
    }

    أرجع بەها;
}


ngx_int_t
ngx_hextoi(u_char *هێڵ, size_t n)
{
    u_char     c, ch;
    ngx_int_t  بەها, cutoff;

    إذا (n == 0) {
        أرجع NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_بەها / 16;

    لكل (بەها = 0; n--; هێڵ++) {
        إذا (بەها > cutoff) {
            أرجع NGX_ERROR;
        }

        ch = *هێڵ;

        إذا (ch >= '0' && ch <= '9') {
            بەها = بەها * 16 + (ch - '0');
            استمر;
        }

        c = (u_char) (ch | 0x20);

        إذا (c >= 'a' && c <= 'f') {
            بەها = بەها * 16 + (c - 'a' + 10);
            استمر;
        }

        أرجع NGX_ERROR;
    }

    أرجع بەها;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, size_t len)
{
    ساكن u_char  hex[] = "0123456789abcdef";

    طالما (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    أرجع dst;
}


فراغ
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    ساكن u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_internal(dst, src, basis64, 1);
}


فراغ
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    ساكن u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_internal(dst, src, basis64, 0);
}


ساكن فراغ
ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src, ثابت u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    size_t          len;

    len = src->len;
    s = src->data;
    d = dst->data;

    طالما (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    إذا (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        إذا (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            إذا (padding) {
                *d++ = '=';
            }

        } وإلا {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        إذا (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->data;
}


ngx_int_t
ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    ساكن u_char   basis64[] = {
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

    أرجع ngx_decode_base64_internal(dst, src, basis64);
}


ngx_int_t
ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    ساكن u_char   basis64[] = {
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

    أرجع ngx_decode_base64_internal(dst, src, basis64);
}


ساكن ngx_int_t
ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src, ثابت u_char *basis)
{
    size_t          len;
    u_char         *d, *s;

    لكل (len = 0; len < src->len; len++) {
        إذا (src->data[len] == '=') {
            اكسر;
        }

        إذا (basis[src->data[len]] == 77) {
            أرجع NGX_ERROR;
        }
    }

    إذا (len % 4 == 1) {
        أرجع NGX_ERROR;
    }

    s = src->data;
    d = dst->data;

    طالما (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    إذا (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    إذا (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->data;

    أرجع NGX_OK;
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

    إذا (u >= 0xf8) {

        (*p)++;
        أرجع 0xffffffff;

    } وإلا إذا (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } وإلا إذا (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } وإلا إذا (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } وإلا {
        (*p)++;
        أرجع 0xffffffff;
    }

    إذا (n - 1 < len) {
        أرجع 0xfffffffe;
    }

    (*p)++;

    طالما (len) {
        i = *(*p)++;

        إذا (i < 0x80) {
            أرجع 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    إذا (u > valid) {
        أرجع u;
    }

    أرجع 0xffffffff;
}


size_t
ngx_utf8_length(u_char *p, size_t n)
{
    u_char  c, *last;
    size_t  len;

    last = p + n;

    لكل (len = 0; p < last; len++) {

        c = *p;

        إذا (c < 0x80) {
            p++;
            استمر;
        }

        إذا (ngx_utf8_decode(&p, last - p) > 0x10ffff) {
            /* invalid UTF-8 */
            أرجع n;
        }
    }

    أرجع len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len)
{
    u_char  c, *next;

    إذا (n == 0) {
        أرجع dst;
    }

    طالما (--n) {

        c = *src;
        *dst = c;

        إذا (c < 0x80) {

            إذا (c != '\0') {
                dst++;
                src++;
                len--;

                استمر;
            }

            أرجع dst;
        }

        next = src;

        إذا (ngx_utf8_decode(&next, len) > 0x10ffff) {
            /* invalid UTF-8 */
            اكسر;
        }

        طالما (src < next) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    أرجع dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, size_t size, ngx_uint_t type)
{
    u_char          prefix;
    uint32_t       *escape;
    ngx_uint_t      n;
    ساكن u_char   hex[] = "0123456789ABCDEF";

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

    ساكن uint32_t   uri[] = {
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

    ساكن uint32_t   args[] = {
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

    ساكن uint32_t   uri_component[] = {
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

    ساكن uint32_t   html[] = {
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

    ساكن uint32_t   refresh[] = {
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

    ساكن uint32_t   memcached[] = {
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

    ساكن uint32_t   mail_xtext[] = {
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

    ساكن uint32_t  *map[] =
        { uri, args, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    ساكن u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    prefix = map_char[type];

    إذا (dst == عدم) {

        /* find the number of the characters to be escaped */

        n = 0;

        طالما (size) {
            إذا (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            size--;
        }

        أرجع (uintptr_t) n;
    }

    طالما (size) {
        إذا (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = prefix;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } وإلا {
            *dst++ = *src++;
        }
        size--;
    }

    أرجع (uintptr_t) dst;
}


فراغ
ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    تعداد {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_second
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    طالما (size--) {

        ch = *s++;

        بدل (state) {
        حالة sw_usual:
            إذا (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                اذهب done;
            }

            إذا (ch == '%') {
                state = sw_quoted;
                اكسر;
            }

            *d++ = ch;
            اكسر;

        حالة sw_quoted:

            إذا (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_second;
                اكسر;
            }

            c = (u_char) (ch | 0x20);
            إذا (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_second;
                اكسر;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            اكسر;

        حالة sw_quoted_second:

            state = sw_usual;

            إذا (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                إذا (type & NGX_UNESCAPE_REDIRECT) {
                    إذا (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        اكسر;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    اكسر;
                }

                *d++ = ch;

                اكسر;
            }

            c = (u_char) (ch | 0x20);
            إذا (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                إذا (type & NGX_UNESCAPE_URI) {
                    إذا (ch == '?') {
                        *d++ = ch;
                        اذهب done;
                    }

                    *d++ = ch;
                    اكسر;
                }

                إذا (type & NGX_UNESCAPE_REDIRECT) {
                    إذا (ch == '?') {
                        *d++ = ch;
                        اذهب done;
                    }

                    إذا (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        اكسر;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    اكسر;
                }

                *d++ = ch;

                اكسر;
            }

            /* the invalid quoted character */

            اكسر;
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

    إذا (dst == عدم) {

        len = 0;

        طالما (size) {
            بدل (*src++) {

            حالة '<':
                len += حجم("&lt;") - 2;
                اكسر;

            حالة '>':
                len += حجم("&gt;") - 2;
                اكسر;

            حالة '&':
                len += حجم("&amp;") - 2;
                اكسر;

            حالة '"':
                len += حجم("&quot;") - 2;
                اكسر;

            افتراضي:
                اكسر;
            }
            size--;
        }

        أرجع (uintptr_t) len;
    }

    طالما (size) {
        ch = *src++;

        بدل (ch) {

        حالة '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            اكسر;

        حالة '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            اكسر;

        حالة '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            اكسر;

        حالة '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            اكسر;

        افتراضي:
            *dst++ = ch;
            اكسر;
        }
        size--;
    }

    أرجع (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, size_t size)
{
    u_char      ch;
    ngx_uint_t  len;

    إذا (dst == عدم) {
        len = 0;

        طالما (size) {
            ch = *src++;

            إذا (ch == '\\' || ch == '"') {
                len++;

            } وإلا إذا (ch <= 0x1f) {

                بدل (ch) {
                حالة '\n':
                حالة '\r':
                حالة '\t':
                حالة '\b':
                حالة '\f':
                    len++;
                    اكسر;

                افتراضي:
                    len += حجم("\\u001F") - 2;
                }
            }

            size--;
        }

        أرجع (uintptr_t) len;
    }

    طالما (size) {
        ch = *src++;

        إذا (ch > 0x1f) {

            إذا (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } وإلا {
            *dst++ = '\\';

            بدل (ch) {
            حالة '\n':
                *dst++ = 'n';
                اكسر;

            حالة '\r':
                *dst++ = 'r';
                اكسر;

            حالة '\t':
                *dst++ = 't';
                اكسر;

            حالة '\b':
                *dst++ = 'b';
                اكسر;

            حالة '\f':
                *dst++ = 'f';
                اكسر;

            افتراضي:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        size--;
    }

    أرجع (uintptr_t) dst;
}


فراغ
ngx_str_rbtree_زیادکردن_بەها(ngx_rbtree_گرێ_t *temp,
    ngx_rbtree_گرێ_t *گرێ, ngx_rbtree_گرێ_t *sentinel)
{
    ngx_str_گرێ_t      *n, *t;
    ngx_rbtree_گرێ_t  **p;

    لكل ( ;; ) {

        n = (ngx_str_گرێ_t *) گرێ;
        t = (ngx_str_گرێ_t *) temp;

        إذا (گرێ->کلیل != temp->کلیل) {

            p = (گرێ->کلیل < temp->کلیل) ? &temp->چەپ : &temp->ڕاست;

        } وإلا إذا (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &temp->چەپ : &temp->ڕاست;

        } وإلا {
            p = (ngx_memcmp(n->str.data, t->str.data, n->str.len) < 0)
                 ? &temp->چەپ : &temp->ڕاست;
        }

        إذا (*p == sentinel) {
            اكسر;
        }

        temp = *p;
    }

    *p = گرێ;
    گرێ->parent = temp;
    گرێ->چەپ = sentinel;
    گرێ->ڕاست = sentinel;
    ngx_rbt_red(گرێ);
}


ngx_str_گرێ_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_گرێ_t     *n;
    ngx_rbtree_گرێ_t  *گرێ, *sentinel;

    گرێ = rbtree->ڕەگ;
    sentinel = rbtree->sentinel;

    طالما (گرێ != sentinel) {

        n = (ngx_str_گرێ_t *) گرێ;

        إذا (hash != گرێ->کلیل) {
            گرێ = (hash < گرێ->کلیل) ? گرێ->چەپ : گرێ->ڕاست;
            استمر;
        }

        إذا (val->len != n->str.len) {
            گرێ = (val->len < n->str.len) ? گرێ->چەپ : گرێ->ڕاست;
            استمر;
        }

        rc = ngx_memcmp(val->data, n->str.data, val->len);

        إذا (rc < 0) {
            گرێ = گرێ->چەپ;
            استمر;
        }

        إذا (rc > 0) {
            گرێ = گرێ->ڕاست;
            استمر;
        }

        أرجع n;
    }

    أرجع عدم;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

فراغ
ngx_sort(فراغ *base, size_t n, size_t size,
    ngx_int_t (*cmp)(ثابت فراغ *, ثابت فراغ *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(size, ngx_cycle->log);
    إذا (p == عدم) {
        أرجع;
    }

    لكل (p1 = (u_char *) base + size;
         p1 < (u_char *) base + n * size;
         p1 += size)
    {
        ngx_memcpy(p, p1, size);

        لكل (p2 = p1;
             p2 > (u_char *) base && cmp(p2 - size, p) > 0;
             p2 -= size)
        {
            ngx_memcpy(p2, p2 - size, size);
        }

        ngx_memcpy(p2, p, size);
    }

    ngx_free(p);
}


فراغ
ngx_explicit_memzero(فراغ *buf, size_t n)
{
    ngx_memzero(buf, n);
    ngx_memory_barrier();
}


#إذا (NGX_MEMCPY_LIMIT)

فراغ *
ngx_memcpy(فراغ *dst, ثابت فراغ *src, size_t n)
{
    إذا (n > NGX_MEMCPY_LIMIT) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "memcpy %uz bytes", n);
        ngx_debug_point();
    }

    أرجع memcpy(dst, src, n);
}

#endif
