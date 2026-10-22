
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


સ્થિર u_char *ngx_sprintf_અંક(u_char *buf, u_char *last, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
સ્થિર u_char *ngx_sprintf_str(u_char *buf, u_char *last, u_char *src,
    size_t len, ngx_uint_t hexadecimal);
સ્થિર શૂન્ય ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    અચલ u_char *basis, ngx_uint_t padding);
સ્થિર ngx_int_t ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    અચલ u_char *basis);


શૂન્ય
ngx_strlow(u_char *dst, u_char *src, size_t n)
{
    જ્યારે (n) {
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

    માટે (i = 0; i < n; i++) {

        જો (p[i] == '\0') {
            પાછા i;
        }
    }

    પાછા n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, size_t n)
{
    જો (n == 0) {
        પાછા dst;
    }

    જ્યારે (--n) {
        *dst = *src;

        જો (*dst == '\0') {
            પાછા dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    પાછા dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    જો (dst == ખાલી) {
        પાછા ખાલી;
    }

    ngx_memcpy(dst, src->data, src->len);

    પાછા dst;
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
ngx_sprintf(u_char *buf, અચલ અક્ષર *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, (શૂન્ય *) -1, fmt, args);
    va_end(args);

    પાછા p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, size_t max, અચલ અક્ષર *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, args);
    va_end(args);

    પાછા p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *last, અચલ અક્ષર *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, last, fmt, args);
    va_end(args);

    પાછા p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *last, અચલ અક્ષર *fmt, va_list args)
{
    u_char                *p, zero;
    પૂર્ણાંક                    d;
    દ્વિગુણ                 f;
    size_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             width, sign, hex, max_width, frac_width, scale, n;
    ngx_str_t             *v;
    ngx_variable_કિંમત_t  *vv;

    જ્યારે (*fmt && buf < last) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        જો (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (size_t) -1;

            જ્યારે (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }


            માટે ( ;; ) {
                પસંદગી (*fmt) {

                સ્થિતિ 'u':
                    sign = 0;
                    fmt++;
                    ચાલુ;

                સ્થિતિ 'm':
                    max_width = 1;
                    fmt++;
                    ચાલુ;

                સ્થિતિ 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    ચાલુ;

                સ્થિતિ 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    ચાલુ;

                સ્થિતિ '.':
                    fmt++;

                    જ્યારે (*fmt >= '0' && *fmt <= '9') {
                        frac_width = frac_width * 10 + (*fmt++ - '0');
                    }

                    તોડો;

                સ્થિતિ '*':
                    slen = va_arg(args, size_t);
                    fmt++;
                    ચાલુ;

                મૂળભૂત:
                    તોડો;
                }

                તોડો;
            }


            પસંદગી (*fmt) {

            સ્થિતિ 'V':
                v = va_arg(args, ngx_str_t *);

                buf = ngx_sprintf_str(buf, last, v->data, v->len, hex);
                fmt++;

                ચાલુ;

            સ્થિતિ 'v':
                vv = va_arg(args, ngx_variable_કિંમત_t *);

                buf = ngx_sprintf_str(buf, last, vv->data, vv->len, hex);
                fmt++;

                ચાલુ;

            સ્થિતિ 's':
                p = va_arg(args, u_char *);

                buf = ngx_sprintf_str(buf, last, p, slen, hex);
                fmt++;

                ચાલુ;

            સ્થિતિ 'O':
                i64 = (int64_t) va_arg(args, off_t);
                sign = 1;
                તોડો;

            સ્થિતિ 'P':
                i64 = (int64_t) va_arg(args, ngx_pid_t);
                sign = 1;
                તોડો;

            સ્થિતિ 'T':
                i64 = (int64_t) va_arg(args, time_t);
                sign = 1;
                તોડો;

            સ્થિતિ 'M':
                ms = (ngx_msec_t) va_arg(args, ngx_msec_t);
                જો ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } નહીંતર {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                તોડો;

            સ્થિતિ 'z':
                જો (sign) {
                    i64 = (int64_t) va_arg(args, ssize_t);
                } નહીંતર {
                    ui64 = (uint64_t) va_arg(args, size_t);
                }
                તોડો;

            સ્થિતિ 'i':
                જો (sign) {
                    i64 = (int64_t) va_arg(args, ngx_int_t);
                } નહીંતર {
                    ui64 = (uint64_t) va_arg(args, ngx_uint_t);
                }

                જો (max_width) {
                    width = NGX_INT_T_LEN;
                }

                તોડો;

            સ્થિતિ 'd':
                જો (sign) {
                    i64 = (int64_t) va_arg(args, પૂર્ણાંક);
                } નહીંતર {
                    ui64 = (uint64_t) va_arg(args, u_int);
                }
                તોડો;

            સ્થિતિ 'l':
                જો (sign) {
                    i64 = (int64_t) va_arg(args, long);
                } નહીંતર {
                    ui64 = (uint64_t) va_arg(args, u_long);
                }
                તોડો;

            સ્થિતિ 'D':
                જો (sign) {
                    i64 = (int64_t) va_arg(args, int32_t);
                } નહીંતર {
                    ui64 = (uint64_t) va_arg(args, uint32_t);
                }
                તોડો;

            સ્થિતિ 'L':
                જો (sign) {
                    i64 = va_arg(args, int64_t);
                } નહીંતર {
                    ui64 = va_arg(args, uint64_t);
                }
                તોડો;

            સ્થિતિ 'A':
                જો (sign) {
                    i64 = (int64_t) va_arg(args, ngx_atomic_int_t);
                } નહીંતર {
                    ui64 = (uint64_t) va_arg(args, ngx_atomic_uint_t);
                }

                જો (max_width) {
                    width = NGX_ATOMIC_T_LEN;
                }

                તોડો;

            સ્થિતિ 'f':
                f = va_arg(args, દ્વિગુણ);

                જો (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                જો (frac_width) {

                    scale = 1;
                    માટે (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (દ્વિગુણ) ui64) * scale + 0.5);

                    જો (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_અંક(buf, last, ui64, zero, 0, width);

                જો (frac_width) {
                    જો (buf < last) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_અંક(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                ચાલુ;

#જો !(NGX_WIN32)
            સ્થિતિ 'r':
                i64 = (int64_t) va_arg(args, rlim_t);
                sign = 1;
                તોડો;
#endif

            સ્થિતિ 'p':
                ui64 = (uintptr_t) va_arg(args, શૂન્ય *);
                hex = 2;
                sign = 0;
                zero = '0';
                width = 2 * કદ(શૂન્ય *);
                તોડો;

            સ્થિતિ 'c':
                d = va_arg(args, પૂર્ણાંક);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                ચાલુ;

            સ્થિતિ 'Z':
                *buf++ = '\0';
                fmt++;

                ચાલુ;

            સ્થિતિ 'N':
#જો (NGX_WIN32)
                *buf++ = CR;
                જો (buf < last) {
                    *buf++ = LF;
                }
#નહીંતર
                *buf++ = LF;
#endif
                fmt++;

                ચાલુ;

            સ્થિતિ '%':
                *buf++ = '%';
                fmt++;

                ચાલુ;

            મૂળભૂત:
                *buf++ = *fmt++;

                ચાલુ;
            }

            જો (sign) {
                જો (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } નહીંતર {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_અંક(buf, last, ui64, zero, hex, width);

            fmt++;

        } નહીંતર {
            *buf++ = *fmt++;
        }
    }

    પાછા buf;
}


સ્થિર u_char *
ngx_sprintf_અંક(u_char *buf, u_char *last, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t width)
{
    u_char         *p, temp[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    size_t          len;
    uint32_t        ui32;
    સ્થિર u_char   hex[] = "0123456789abcdef";
    સ્થિર u_char   HEX[] = "0123456789ABCDEF";

    p = temp + NGX_INT64_LEN;

    જો (hexadecimal == 0) {

        જો (ui64 <= (uint64_t) NGX_MAX_UINT32_કિંમત) {

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

            કરો {
                *--p = (u_char) (ui32 % 10 + '0');
            } જ્યારે (ui32 /= 10);

        } નહીંતર {
            કરો {
                *--p = (u_char) (ui64 % 10 + '0');
            } જ્યારે (ui64 /= 10);
        }

    } નહીંતર જો (hexadecimal == 1) {

        કરો {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } જ્યારે (ui64 >>= 4);

    } નહીંતર { /* hexadecimal == 2 */

        કરો {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } જ્યારે (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    જ્યારે (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    જો (buf + len > last) {
        len = last - buf;
    }

    પાછા ngx_cpymem(buf, p, len);
}


સ્થિર u_char *
ngx_sprintf_str(u_char *buf, u_char *last, u_char *src, size_t len,
    ngx_uint_t hexadecimal)
{
    સ્થિર u_char   hex[] = "0123456789abcdef";
    સ્થિર u_char   HEX[] = "0123456789ABCDEF";

    જો (hexadecimal == 0) {

        જો (len == (size_t) -1) {
            જ્યારે (*src && buf < last) {
                *buf++ = *src++;
            }

        } નહીંતર {
            len = ngx_min((size_t) (last - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } નહીંતર જો (hexadecimal == 1) {

        જો (len == (size_t) -1) {

            જ્યારે (*src && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } નહીંતર {

            જ્યારે (len-- && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } નહીંતર { /* hexadecimal == 2 */

        જો (len == (size_t) -1) {

            જ્યારે (*src && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } નહીંતર {

            જ્યારે (len-- && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    પાછા buf;
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

    માટે ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        જો (c1 == c2) {

            જો (c1) {
                ચાલુ;
            }

            પાછા 0;
        }

        પાછા c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    જ્યારે (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        જો (c1 == c2) {

            જો (c1) {
                n--;
                ચાલુ;
            }

            પાછા 0;
        }

        પાછા c1 - c2;
    }

    પાછા 0;
}


u_char *
ngx_strnstr(u_char *s1, અક્ષર *s2, size_t len)
{
    u_char  c1, c2;
    size_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    કરો {
        કરો {
            જો (len-- == 0) {
                પાછા ખાલી;
            }

            c1 = *s1++;

            જો (c1 == 0) {
                પાછા ખાલી;
            }

        } જ્યારે (c1 != c2);

        જો (n > len) {
            પાછા ખાલી;
        }

    } જ્યારે (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    પાછા --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, અક્ષર *s2, size_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    કરો {
        કરો {
            c1 = *s1++;

            જો (c1 == 0) {
                પાછા ખાલી;
            }

        } જ્યારે (c1 != c2);

    } જ્યારે (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    પાછા --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, અક્ષર *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    કરો {
        કરો {
            c1 = (ngx_uint_t) *s1++;

            જો (c1 == 0) {
                પાછા ખાલી;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } જ્યારે (c1 != c2);

    } જ્યારે (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    પાછા --s1;
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

    કરો {
        કરો {
            જો (s1 >= last) {
                પાછા ખાલી;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } જ્યારે (c1 != c2);

    } જ્યારે (ngx_strncasecmp(s1, s2, n) != 0);

    પાછા --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, size_t n)
{
    જો (n == 0) {
        પાછા 0;
    }

    n--;

    માટે ( ;; ) {
        જો (s1[n] != s2[n]) {
            પાછા s1[n] - s2[n];
        }

        જો (n == 0) {
            પાછા 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n)
{
    u_char  c1, c2;

    જો (n == 0) {
        પાછા 0;
    }

    n--;

    માટે ( ;; ) {
        c1 = s1[n];
        જો (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        જો (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        જો (c1 != c2) {
            પાછા c1 - c2;
        }

        જો (n == 0) {
            પાછા 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2)
{
    size_t     n;
    ngx_int_t  m, z;

    જો (n1 <= n2) {
        n = n1;
        z = -1;

    } નહીંતર {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    જો (m || n1 == n2) {
        પાછા m;
    }

    પાછા z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    માટે ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        જો (c1 == c2) {

            જો (c1) {
                ચાલુ;
            }

            પાછા 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        પાછા c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    જ્યારે (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#જો (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#endif

        જો (c1 == c2) {

            જો (c1) {
                n--;
                ચાલુ;
            }

            પાછા 0;
        }

        /* we need '/' to be the lowest character */

        જો (c1 == 0 || c2 == 0) {
            પાછા c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        પાછા c1 - c2;
    }

    પાછા 0;
}


ngx_int_t
ngx_atoi(u_char *લીટી, size_t n)
{
    ngx_int_t  કિંમત, cutoff, cutlim;

    જો (n == 0) {
        પાછા NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_કિંમત / 10;
    cutlim = NGX_MAX_INT_T_કિંમત % 10;

    માટે (કિંમત = 0; n--; લીટી++) {
        જો (*લીટી < '0' || *લીટી > '9') {
            પાછા NGX_ERROR;
        }

        જો (કિંમત >= cutoff && (કિંમત > cutoff || *લીટી - '0' > cutlim)) {
            પાછા NGX_ERROR;
        }

        કિંમત = કિંમત * 10 + (*લીટી - '0');
    }

    પાછા કિંમત;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *લીટી, size_t n, size_t point)
{
    ngx_int_t   કિંમત, cutoff, cutlim;
    ngx_uint_t  dot;

    જો (n == 0) {
        પાછા NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_કિંમત / 10;
    cutlim = NGX_MAX_INT_T_કિંમત % 10;

    dot = 0;

    માટે (કિંમત = 0; n--; લીટી++) {

        જો (point == 0) {
            પાછા NGX_ERROR;
        }

        જો (*લીટી == '.') {
            જો (dot) {
                પાછા NGX_ERROR;
            }

            dot = 1;
            ચાલુ;
        }

        જો (*લીટી < '0' || *લીટી > '9') {
            પાછા NGX_ERROR;
        }

        જો (કિંમત >= cutoff && (કિંમત > cutoff || *લીટી - '0' > cutlim)) {
            પાછા NGX_ERROR;
        }

        કિંમત = કિંમત * 10 + (*લીટી - '0');
        point -= dot;
    }

    જ્યારે (point--) {
        જો (કિંમત > cutoff) {
            પાછા NGX_ERROR;
        }

        કિંમત = કિંમત * 10;
    }

    પાછા કિંમત;
}


ssize_t
ngx_atosz(u_char *લીટી, size_t n)
{
    ssize_t  કિંમત, cutoff, cutlim;

    જો (n == 0) {
        પાછા NGX_ERROR;
    }

    cutoff = NGX_MAX_SIZE_T_કિંમત / 10;
    cutlim = NGX_MAX_SIZE_T_કિંમત % 10;

    માટે (કિંમત = 0; n--; લીટી++) {
        જો (*લીટી < '0' || *લીટી > '9') {
            પાછા NGX_ERROR;
        }

        જો (કિંમત >= cutoff && (કિંમત > cutoff || *લીટી - '0' > cutlim)) {
            પાછા NGX_ERROR;
        }

        કિંમત = કિંમત * 10 + (*લીટી - '0');
    }

    પાછા કિંમત;
}


off_t
ngx_atoof(u_char *લીટી, size_t n)
{
    off_t  કિંમત, cutoff, cutlim;

    જો (n == 0) {
        પાછા NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_કિંમત / 10;
    cutlim = NGX_MAX_OFF_T_કિંમત % 10;

    માટે (કિંમત = 0; n--; લીટી++) {
        જો (*લીટી < '0' || *લીટી > '9') {
            પાછા NGX_ERROR;
        }

        જો (કિંમત >= cutoff && (કિંમત > cutoff || *લીટી - '0' > cutlim)) {
            પાછા NGX_ERROR;
        }

        કિંમત = કિંમત * 10 + (*લીટી - '0');
    }

    પાછા કિંમત;
}


time_t
ngx_atotm(u_char *લીટી, size_t n)
{
    time_t  કિંમત, cutoff, cutlim;

    જો (n == 0) {
        પાછા NGX_ERROR;
    }

    cutoff = NGX_MAX_TIME_T_કિંમત / 10;
    cutlim = NGX_MAX_TIME_T_કિંમત % 10;

    માટે (કિંમત = 0; n--; લીટી++) {
        જો (*લીટી < '0' || *લીટી > '9') {
            પાછા NGX_ERROR;
        }

        જો (કિંમત >= cutoff && (કિંમત > cutoff || *લીટી - '0' > cutlim)) {
            પાછા NGX_ERROR;
        }

        કિંમત = કિંમત * 10 + (*લીટી - '0');
    }

    પાછા કિંમત;
}


ngx_int_t
ngx_hextoi(u_char *લીટી, size_t n)
{
    u_char     c, ch;
    ngx_int_t  કિંમત, cutoff;

    જો (n == 0) {
        પાછા NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_કિંમત / 16;

    માટે (કિંમત = 0; n--; લીટી++) {
        જો (કિંમત > cutoff) {
            પાછા NGX_ERROR;
        }

        ch = *લીટી;

        જો (ch >= '0' && ch <= '9') {
            કિંમત = કિંમત * 16 + (ch - '0');
            ચાલુ;
        }

        c = (u_char) (ch | 0x20);

        જો (c >= 'a' && c <= 'f') {
            કિંમત = કિંમત * 16 + (c - 'a' + 10);
            ચાલુ;
        }

        પાછા NGX_ERROR;
    }

    પાછા કિંમત;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, size_t len)
{
    સ્થિર u_char  hex[] = "0123456789abcdef";

    જ્યારે (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    પાછા dst;
}


શૂન્ય
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    સ્થિર u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_internal(dst, src, basis64, 1);
}


શૂન્ય
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    સ્થિર u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_internal(dst, src, basis64, 0);
}


સ્થિર શૂન્ય
ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src, અચલ u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    size_t          len;

    len = src->len;
    s = src->data;
    d = dst->data;

    જ્યારે (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    જો (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        જો (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            જો (padding) {
                *d++ = '=';
            }

        } નહીંતર {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        જો (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->data;
}


ngx_int_t
ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    સ્થિર u_char   basis64[] = {
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

    પાછા ngx_decode_base64_internal(dst, src, basis64);
}


ngx_int_t
ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    સ્થિર u_char   basis64[] = {
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

    પાછા ngx_decode_base64_internal(dst, src, basis64);
}


સ્થિર ngx_int_t
ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src, અચલ u_char *basis)
{
    size_t          len;
    u_char         *d, *s;

    માટે (len = 0; len < src->len; len++) {
        જો (src->data[len] == '=') {
            તોડો;
        }

        જો (basis[src->data[len]] == 77) {
            પાછા NGX_ERROR;
        }
    }

    જો (len % 4 == 1) {
        પાછા NGX_ERROR;
    }

    s = src->data;
    d = dst->data;

    જ્યારે (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    જો (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    જો (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->data;

    પાછા NGX_OK;
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

    જો (u >= 0xf8) {

        (*p)++;
        પાછા 0xffffffff;

    } નહીંતર જો (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } નહીંતર જો (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } નહીંતર જો (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } નહીંતર {
        (*p)++;
        પાછા 0xffffffff;
    }

    જો (n - 1 < len) {
        પાછા 0xfffffffe;
    }

    (*p)++;

    જ્યારે (len) {
        i = *(*p)++;

        જો (i < 0x80) {
            પાછા 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    જો (u > valid) {
        પાછા u;
    }

    પાછા 0xffffffff;
}


size_t
ngx_utf8_length(u_char *p, size_t n)
{
    u_char  c, *last;
    size_t  len;

    last = p + n;

    માટે (len = 0; p < last; len++) {

        c = *p;

        જો (c < 0x80) {
            p++;
            ચાલુ;
        }

        જો (ngx_utf8_decode(&p, last - p) > 0x10ffff) {
            /* invalid UTF-8 */
            પાછા n;
        }
    }

    પાછા len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len)
{
    u_char  c, *next;

    જો (n == 0) {
        પાછા dst;
    }

    જ્યારે (--n) {

        c = *src;
        *dst = c;

        જો (c < 0x80) {

            જો (c != '\0') {
                dst++;
                src++;
                len--;

                ચાલુ;
            }

            પાછા dst;
        }

        next = src;

        જો (ngx_utf8_decode(&next, len) > 0x10ffff) {
            /* invalid UTF-8 */
            તોડો;
        }

        જ્યારે (src < next) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    પાછા dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, size_t size, ngx_uint_t type)
{
    u_char          prefix;
    uint32_t       *escape;
    ngx_uint_t      n;
    સ્થિર u_char   hex[] = "0123456789ABCDEF";

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

    સ્થિર uint32_t   uri[] = {
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

    સ્થિર uint32_t   args[] = {
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

    સ્થિર uint32_t   uri_component[] = {
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

    સ્થિર uint32_t   html[] = {
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

    સ્થિર uint32_t   refresh[] = {
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

    સ્થિર uint32_t   memcached[] = {
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

    સ્થિર uint32_t   mail_xtext[] = {
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

    સ્થિર uint32_t  *map[] =
        { uri, args, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    સ્થિર u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    prefix = map_char[type];

    જો (dst == ખાલી) {

        /* find the number of the characters to be escaped */

        n = 0;

        જ્યારે (size) {
            જો (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            size--;
        }

        પાછા (uintptr_t) n;
    }

    જ્યારે (size) {
        જો (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = prefix;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } નહીંતર {
            *dst++ = *src++;
        }
        size--;
    }

    પાછા (uintptr_t) dst;
}


શૂન્ય
ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    ગણના {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_second
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    જ્યારે (size--) {

        ch = *s++;

        પસંદગી (state) {
        સ્થિતિ sw_usual:
            જો (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                જાઓ done;
            }

            જો (ch == '%') {
                state = sw_quoted;
                તોડો;
            }

            *d++ = ch;
            તોડો;

        સ્થિતિ sw_quoted:

            જો (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_second;
                તોડો;
            }

            c = (u_char) (ch | 0x20);
            જો (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_second;
                તોડો;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            તોડો;

        સ્થિતિ sw_quoted_second:

            state = sw_usual;

            જો (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                જો (type & NGX_UNESCAPE_REDIRECT) {
                    જો (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        તોડો;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    તોડો;
                }

                *d++ = ch;

                તોડો;
            }

            c = (u_char) (ch | 0x20);
            જો (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                જો (type & NGX_UNESCAPE_URI) {
                    જો (ch == '?') {
                        *d++ = ch;
                        જાઓ done;
                    }

                    *d++ = ch;
                    તોડો;
                }

                જો (type & NGX_UNESCAPE_REDIRECT) {
                    જો (ch == '?') {
                        *d++ = ch;
                        જાઓ done;
                    }

                    જો (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        તોડો;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    તોડો;
                }

                *d++ = ch;

                તોડો;
            }

            /* the invalid quoted character */

            તોડો;
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

    જો (dst == ખાલી) {

        len = 0;

        જ્યારે (size) {
            પસંદગી (*src++) {

            સ્થિતિ '<':
                len += કદ("&lt;") - 2;
                તોડો;

            સ્થિતિ '>':
                len += કદ("&gt;") - 2;
                તોડો;

            સ્થિતિ '&':
                len += કદ("&amp;") - 2;
                તોડો;

            સ્થિતિ '"':
                len += કદ("&quot;") - 2;
                તોડો;

            મૂળભૂત:
                તોડો;
            }
            size--;
        }

        પાછા (uintptr_t) len;
    }

    જ્યારે (size) {
        ch = *src++;

        પસંદગી (ch) {

        સ્થિતિ '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            તોડો;

        સ્થિતિ '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            તોડો;

        સ્થિતિ '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            તોડો;

        સ્થિતિ '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            તોડો;

        મૂળભૂત:
            *dst++ = ch;
            તોડો;
        }
        size--;
    }

    પાછા (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, size_t size)
{
    u_char      ch;
    ngx_uint_t  len;

    જો (dst == ખાલી) {
        len = 0;

        જ્યારે (size) {
            ch = *src++;

            જો (ch == '\\' || ch == '"') {
                len++;

            } નહીંતર જો (ch <= 0x1f) {

                પસંદગી (ch) {
                સ્થિતિ '\n':
                સ્થિતિ '\r':
                સ્થિતિ '\t':
                સ્થિતિ '\b':
                સ્થિતિ '\f':
                    len++;
                    તોડો;

                મૂળભૂત:
                    len += કદ("\\u001F") - 2;
                }
            }

            size--;
        }

        પાછા (uintptr_t) len;
    }

    જ્યારે (size) {
        ch = *src++;

        જો (ch > 0x1f) {

            જો (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } નહીંતર {
            *dst++ = '\\';

            પસંદગી (ch) {
            સ્થિતિ '\n':
                *dst++ = 'n';
                તોડો;

            સ્થિતિ '\r':
                *dst++ = 'r';
                તોડો;

            સ્થિતિ '\t':
                *dst++ = 't';
                તોડો;

            સ્થિતિ '\b':
                *dst++ = 'b';
                તોડો;

            સ્થિતિ '\f':
                *dst++ = 'f';
                તોડો;

            મૂળભૂત:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        size--;
    }

    પાછા (uintptr_t) dst;
}


શૂન્ય
ngx_str_rbtree_ઉમેરો_કિંમત(ngx_rbtree_ગાંઠ_t *temp,
    ngx_rbtree_ગાંઠ_t *ગાંઠ, ngx_rbtree_ગાંઠ_t *sentinel)
{
    ngx_str_ગાંઠ_t      *n, *t;
    ngx_rbtree_ગાંઠ_t  **p;

    માટે ( ;; ) {

        n = (ngx_str_ગાંઠ_t *) ગાંઠ;
        t = (ngx_str_ગાંઠ_t *) temp;

        જો (ગાંઠ->ચાવી != temp->ચાવી) {

            p = (ગાંઠ->ચાવી < temp->ચાવી) ? &temp->ડાબું : &temp->જમણું;

        } નહીંતર જો (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &temp->ડાબું : &temp->જમણું;

        } નહીંતર {
            p = (ngx_memcmp(n->str.data, t->str.data, n->str.len) < 0)
                 ? &temp->ડાબું : &temp->જમણું;
        }

        જો (*p == sentinel) {
            તોડો;
        }

        temp = *p;
    }

    *p = ગાંઠ;
    ગાંઠ->parent = temp;
    ગાંઠ->ડાબું = sentinel;
    ગાંઠ->જમણું = sentinel;
    ngx_rbt_red(ગાંઠ);
}


ngx_str_ગાંઠ_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_ગાંઠ_t     *n;
    ngx_rbtree_ગાંઠ_t  *ગાંઠ, *sentinel;

    ગાંઠ = rbtree->મૂળ;
    sentinel = rbtree->sentinel;

    જ્યારે (ગાંઠ != sentinel) {

        n = (ngx_str_ગાંઠ_t *) ગાંઠ;

        જો (hash != ગાંઠ->ચાવી) {
            ગાંઠ = (hash < ગાંઠ->ચાવી) ? ગાંઠ->ડાબું : ગાંઠ->જમણું;
            ચાલુ;
        }

        જો (val->len != n->str.len) {
            ગાંઠ = (val->len < n->str.len) ? ગાંઠ->ડાબું : ગાંઠ->જમણું;
            ચાલુ;
        }

        rc = ngx_memcmp(val->data, n->str.data, val->len);

        જો (rc < 0) {
            ગાંઠ = ગાંઠ->ડાબું;
            ચાલુ;
        }

        જો (rc > 0) {
            ગાંઠ = ગાંઠ->જમણું;
            ચાલુ;
        }

        પાછા n;
    }

    પાછા ખાલી;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

શૂન્ય
ngx_sort(શૂન્ય *base, size_t n, size_t size,
    ngx_int_t (*cmp)(અચલ શૂન્ય *, અચલ શૂન્ય *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(size, ngx_cycle->log);
    જો (p == ખાલી) {
        પાછા;
    }

    માટે (p1 = (u_char *) base + size;
         p1 < (u_char *) base + n * size;
         p1 += size)
    {
        ngx_memcpy(p, p1, size);

        માટે (p2 = p1;
             p2 > (u_char *) base && cmp(p2 - size, p) > 0;
             p2 -= size)
        {
            ngx_memcpy(p2, p2 - size, size);
        }

        ngx_memcpy(p2, p, size);
    }

    ngx_free(p);
}


શૂન્ય
ngx_explicit_memzero(શૂન્ય *buf, size_t n)
{
    ngx_memzero(buf, n);
    ngx_memory_barrier();
}


#જો (NGX_MEMCPY_LIMIT)

શૂન્ય *
ngx_memcpy(શૂન્ય *dst, અચલ શૂન્ય *src, size_t n)
{
    જો (n > NGX_MEMCPY_LIMIT) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "memcpy %uz bytes", n);
        ngx_debug_point();
    }

    પાછા memcpy(dst, src, n);
}

#endif
