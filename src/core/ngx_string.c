
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


நிலையான u_char *ngx_sprintf_இலக்கம்(u_char *buf, u_char *last, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
நிலையான u_char *ngx_sprintf_str(u_char *buf, u_char *last, u_char *src,
    size_t len, ngx_uint_t hexadecimal);
நிலையான வெறுமை ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    மாறிலி u_char *basis, ngx_uint_t padding);
நிலையான ngx_int_t ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    மாறிலி u_char *basis);


வெறுமை
ngx_strlow(u_char *dst, u_char *src, size_t n)
{
    போது (n) {
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

    க்கு (i = 0; i < n; i++) {

        எனில் (p[i] == '\0') {
            திரும்பு i;
        }
    }

    திரும்பு n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, size_t n)
{
    எனில் (n == 0) {
        திரும்பு dst;
    }

    போது (--n) {
        *dst = *src;

        எனில் (*dst == '\0') {
            திரும்பு dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    திரும்பு dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    எனில் (dst == வெற்று) {
        திரும்பு வெற்று;
    }

    ngx_memcpy(dst, src->data, src->len);

    திரும்பு dst;
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
ngx_sprintf(u_char *buf, மாறிலி எழுத்து *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, (வெறுமை *) -1, fmt, args);
    va_end(args);

    திரும்பு p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, size_t max, மாறிலி எழுத்து *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, args);
    va_end(args);

    திரும்பு p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *last, மாறிலி எழுத்து *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, last, fmt, args);
    va_end(args);

    திரும்பு p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *last, மாறிலி எழுத்து *fmt, va_list args)
{
    u_char                *p, zero;
    முழு                    d;
    இரட்டை                 f;
    size_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             width, sign, hex, max_width, frac_width, scale, n;
    ngx_str_t             *v;
    ngx_variable_மதிப்பு_t  *vv;

    போது (*fmt && buf < last) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        எனில் (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (size_t) -1;

            போது (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }


            க்கு ( ;; ) {
                தேர்வு (*fmt) {

                நிலை 'u':
                    sign = 0;
                    fmt++;
                    தொடர்;

                நிலை 'm':
                    max_width = 1;
                    fmt++;
                    தொடர்;

                நிலை 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    தொடர்;

                நிலை 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    தொடர்;

                நிலை '.':
                    fmt++;

                    போது (*fmt >= '0' && *fmt <= '9') {
                        frac_width = frac_width * 10 + (*fmt++ - '0');
                    }

                    நிறுத்து;

                நிலை '*':
                    slen = va_arg(args, size_t);
                    fmt++;
                    தொடர்;

                இயல்பு:
                    நிறுத்து;
                }

                நிறுத்து;
            }


            தேர்வு (*fmt) {

            நிலை 'V':
                v = va_arg(args, ngx_str_t *);

                buf = ngx_sprintf_str(buf, last, v->data, v->len, hex);
                fmt++;

                தொடர்;

            நிலை 'v':
                vv = va_arg(args, ngx_variable_மதிப்பு_t *);

                buf = ngx_sprintf_str(buf, last, vv->data, vv->len, hex);
                fmt++;

                தொடர்;

            நிலை 's':
                p = va_arg(args, u_char *);

                buf = ngx_sprintf_str(buf, last, p, slen, hex);
                fmt++;

                தொடர்;

            நிலை 'O':
                i64 = (int64_t) va_arg(args, off_t);
                sign = 1;
                நிறுத்து;

            நிலை 'P':
                i64 = (int64_t) va_arg(args, ngx_pid_t);
                sign = 1;
                நிறுத்து;

            நிலை 'T':
                i64 = (int64_t) va_arg(args, time_t);
                sign = 1;
                நிறுத்து;

            நிலை 'M':
                ms = (ngx_msec_t) va_arg(args, ngx_msec_t);
                எனில் ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } இல்லையெனில் {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                நிறுத்து;

            நிலை 'z':
                எனில் (sign) {
                    i64 = (int64_t) va_arg(args, ssize_t);
                } இல்லையெனில் {
                    ui64 = (uint64_t) va_arg(args, size_t);
                }
                நிறுத்து;

            நிலை 'i':
                எனில் (sign) {
                    i64 = (int64_t) va_arg(args, ngx_int_t);
                } இல்லையெனில் {
                    ui64 = (uint64_t) va_arg(args, ngx_uint_t);
                }

                எனில் (max_width) {
                    width = NGX_INT_T_LEN;
                }

                நிறுத்து;

            நிலை 'd':
                எனில் (sign) {
                    i64 = (int64_t) va_arg(args, முழு);
                } இல்லையெனில் {
                    ui64 = (uint64_t) va_arg(args, u_int);
                }
                நிறுத்து;

            நிலை 'l':
                எனில் (sign) {
                    i64 = (int64_t) va_arg(args, long);
                } இல்லையெனில் {
                    ui64 = (uint64_t) va_arg(args, u_long);
                }
                நிறுத்து;

            நிலை 'D':
                எனில் (sign) {
                    i64 = (int64_t) va_arg(args, int32_t);
                } இல்லையெனில் {
                    ui64 = (uint64_t) va_arg(args, uint32_t);
                }
                நிறுத்து;

            நிலை 'L':
                எனில் (sign) {
                    i64 = va_arg(args, int64_t);
                } இல்லையெனில் {
                    ui64 = va_arg(args, uint64_t);
                }
                நிறுத்து;

            நிலை 'A':
                எனில் (sign) {
                    i64 = (int64_t) va_arg(args, ngx_atomic_int_t);
                } இல்லையெனில் {
                    ui64 = (uint64_t) va_arg(args, ngx_atomic_uint_t);
                }

                எனில் (max_width) {
                    width = NGX_ATOMIC_T_LEN;
                }

                நிறுத்து;

            நிலை 'f':
                f = va_arg(args, இரட்டை);

                எனில் (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                எனில் (frac_width) {

                    scale = 1;
                    க்கு (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (இரட்டை) ui64) * scale + 0.5);

                    எனில் (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_இலக்கம்(buf, last, ui64, zero, 0, width);

                எனில் (frac_width) {
                    எனில் (buf < last) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_இலக்கம்(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                தொடர்;

#எனில் !(NGX_WIN32)
            நிலை 'r':
                i64 = (int64_t) va_arg(args, rlim_t);
                sign = 1;
                நிறுத்து;
#endif

            நிலை 'p':
                ui64 = (uintptr_t) va_arg(args, வெறுமை *);
                hex = 2;
                sign = 0;
                zero = '0';
                width = 2 * அளவு(வெறுமை *);
                நிறுத்து;

            நிலை 'c':
                d = va_arg(args, முழு);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                தொடர்;

            நிலை 'Z':
                *buf++ = '\0';
                fmt++;

                தொடர்;

            நிலை 'N':
#எனில் (NGX_WIN32)
                *buf++ = CR;
                எனில் (buf < last) {
                    *buf++ = LF;
                }
#இல்லையெனில்
                *buf++ = LF;
#endif
                fmt++;

                தொடர்;

            நிலை '%':
                *buf++ = '%';
                fmt++;

                தொடர்;

            இயல்பு:
                *buf++ = *fmt++;

                தொடர்;
            }

            எனில் (sign) {
                எனில் (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } இல்லையெனில் {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_இலக்கம்(buf, last, ui64, zero, hex, width);

            fmt++;

        } இல்லையெனில் {
            *buf++ = *fmt++;
        }
    }

    திரும்பு buf;
}


நிலையான u_char *
ngx_sprintf_இலக்கம்(u_char *buf, u_char *last, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t width)
{
    u_char         *p, temp[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    size_t          len;
    uint32_t        ui32;
    நிலையான u_char   hex[] = "0123456789abcdef";
    நிலையான u_char   HEX[] = "0123456789ABCDEF";

    p = temp + NGX_INT64_LEN;

    எனில் (hexadecimal == 0) {

        எனில் (ui64 <= (uint64_t) NGX_MAX_UINT32_மதிப்பு) {

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

            செய் {
                *--p = (u_char) (ui32 % 10 + '0');
            } போது (ui32 /= 10);

        } இல்லையெனில் {
            செய் {
                *--p = (u_char) (ui64 % 10 + '0');
            } போது (ui64 /= 10);
        }

    } இல்லையெனில் எனில் (hexadecimal == 1) {

        செய் {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } போது (ui64 >>= 4);

    } இல்லையெனில் { /* hexadecimal == 2 */

        செய் {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } போது (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    போது (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    எனில் (buf + len > last) {
        len = last - buf;
    }

    திரும்பு ngx_cpymem(buf, p, len);
}


நிலையான u_char *
ngx_sprintf_str(u_char *buf, u_char *last, u_char *src, size_t len,
    ngx_uint_t hexadecimal)
{
    நிலையான u_char   hex[] = "0123456789abcdef";
    நிலையான u_char   HEX[] = "0123456789ABCDEF";

    எனில் (hexadecimal == 0) {

        எனில் (len == (size_t) -1) {
            போது (*src && buf < last) {
                *buf++ = *src++;
            }

        } இல்லையெனில் {
            len = ngx_min((size_t) (last - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } இல்லையெனில் எனில் (hexadecimal == 1) {

        எனில் (len == (size_t) -1) {

            போது (*src && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } இல்லையெனில் {

            போது (len-- && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } இல்லையெனில் { /* hexadecimal == 2 */

        எனில் (len == (size_t) -1) {

            போது (*src && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } இல்லையெனில் {

            போது (len-- && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    திரும்பு buf;
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

    க்கு ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        எனில் (c1 == c2) {

            எனில் (c1) {
                தொடர்;
            }

            திரும்பு 0;
        }

        திரும்பு c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    போது (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        எனில் (c1 == c2) {

            எனில் (c1) {
                n--;
                தொடர்;
            }

            திரும்பு 0;
        }

        திரும்பு c1 - c2;
    }

    திரும்பு 0;
}


u_char *
ngx_strnstr(u_char *s1, எழுத்து *s2, size_t len)
{
    u_char  c1, c2;
    size_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    செய் {
        செய் {
            எனில் (len-- == 0) {
                திரும்பு வெற்று;
            }

            c1 = *s1++;

            எனில் (c1 == 0) {
                திரும்பு வெற்று;
            }

        } போது (c1 != c2);

        எனில் (n > len) {
            திரும்பு வெற்று;
        }

    } போது (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    திரும்பு --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, எழுத்து *s2, size_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    செய் {
        செய் {
            c1 = *s1++;

            எனில் (c1 == 0) {
                திரும்பு வெற்று;
            }

        } போது (c1 != c2);

    } போது (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    திரும்பு --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, எழுத்து *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    செய் {
        செய் {
            c1 = (ngx_uint_t) *s1++;

            எனில் (c1 == 0) {
                திரும்பு வெற்று;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } போது (c1 != c2);

    } போது (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    திரும்பு --s1;
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

    செய் {
        செய் {
            எனில் (s1 >= last) {
                திரும்பு வெற்று;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } போது (c1 != c2);

    } போது (ngx_strncasecmp(s1, s2, n) != 0);

    திரும்பு --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, size_t n)
{
    எனில் (n == 0) {
        திரும்பு 0;
    }

    n--;

    க்கு ( ;; ) {
        எனில் (s1[n] != s2[n]) {
            திரும்பு s1[n] - s2[n];
        }

        எனில் (n == 0) {
            திரும்பு 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n)
{
    u_char  c1, c2;

    எனில் (n == 0) {
        திரும்பு 0;
    }

    n--;

    க்கு ( ;; ) {
        c1 = s1[n];
        எனில் (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        எனில் (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        எனில் (c1 != c2) {
            திரும்பு c1 - c2;
        }

        எனில் (n == 0) {
            திரும்பு 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2)
{
    size_t     n;
    ngx_int_t  m, z;

    எனில் (n1 <= n2) {
        n = n1;
        z = -1;

    } இல்லையெனில் {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    எனில் (m || n1 == n2) {
        திரும்பு m;
    }

    திரும்பு z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    க்கு ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        எனில் (c1 == c2) {

            எனில் (c1) {
                தொடர்;
            }

            திரும்பு 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        திரும்பு c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    போது (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#எனில் (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#endif

        எனில் (c1 == c2) {

            எனில் (c1) {
                n--;
                தொடர்;
            }

            திரும்பு 0;
        }

        /* we need '/' to be the lowest character */

        எனில் (c1 == 0 || c2 == 0) {
            திரும்பு c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        திரும்பு c1 - c2;
    }

    திரும்பு 0;
}


ngx_int_t
ngx_atoi(u_char *வரி, size_t n)
{
    ngx_int_t  மதிப்பு, cutoff, cutlim;

    எனில் (n == 0) {
        திரும்பு NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_மதிப்பு / 10;
    cutlim = NGX_MAX_INT_T_மதிப்பு % 10;

    க்கு (மதிப்பு = 0; n--; வரி++) {
        எனில் (*வரி < '0' || *வரி > '9') {
            திரும்பு NGX_ERROR;
        }

        எனில் (மதிப்பு >= cutoff && (மதிப்பு > cutoff || *வரி - '0' > cutlim)) {
            திரும்பு NGX_ERROR;
        }

        மதிப்பு = மதிப்பு * 10 + (*வரி - '0');
    }

    திரும்பு மதிப்பு;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *வரி, size_t n, size_t point)
{
    ngx_int_t   மதிப்பு, cutoff, cutlim;
    ngx_uint_t  dot;

    எனில் (n == 0) {
        திரும்பு NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_மதிப்பு / 10;
    cutlim = NGX_MAX_INT_T_மதிப்பு % 10;

    dot = 0;

    க்கு (மதிப்பு = 0; n--; வரி++) {

        எனில் (point == 0) {
            திரும்பு NGX_ERROR;
        }

        எனில் (*வரி == '.') {
            எனில் (dot) {
                திரும்பு NGX_ERROR;
            }

            dot = 1;
            தொடர்;
        }

        எனில் (*வரி < '0' || *வரி > '9') {
            திரும்பு NGX_ERROR;
        }

        எனில் (மதிப்பு >= cutoff && (மதிப்பு > cutoff || *வரி - '0' > cutlim)) {
            திரும்பு NGX_ERROR;
        }

        மதிப்பு = மதிப்பு * 10 + (*வரி - '0');
        point -= dot;
    }

    போது (point--) {
        எனில் (மதிப்பு > cutoff) {
            திரும்பு NGX_ERROR;
        }

        மதிப்பு = மதிப்பு * 10;
    }

    திரும்பு மதிப்பு;
}


ssize_t
ngx_atosz(u_char *வரி, size_t n)
{
    ssize_t  மதிப்பு, cutoff, cutlim;

    எனில் (n == 0) {
        திரும்பு NGX_ERROR;
    }

    cutoff = NGX_MAX_SIZE_T_மதிப்பு / 10;
    cutlim = NGX_MAX_SIZE_T_மதிப்பு % 10;

    க்கு (மதிப்பு = 0; n--; வரி++) {
        எனில் (*வரி < '0' || *வரி > '9') {
            திரும்பு NGX_ERROR;
        }

        எனில் (மதிப்பு >= cutoff && (மதிப்பு > cutoff || *வரி - '0' > cutlim)) {
            திரும்பு NGX_ERROR;
        }

        மதிப்பு = மதிப்பு * 10 + (*வரி - '0');
    }

    திரும்பு மதிப்பு;
}


off_t
ngx_atoof(u_char *வரி, size_t n)
{
    off_t  மதிப்பு, cutoff, cutlim;

    எனில் (n == 0) {
        திரும்பு NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_மதிப்பு / 10;
    cutlim = NGX_MAX_OFF_T_மதிப்பு % 10;

    க்கு (மதிப்பு = 0; n--; வரி++) {
        எனில் (*வரி < '0' || *வரி > '9') {
            திரும்பு NGX_ERROR;
        }

        எனில் (மதிப்பு >= cutoff && (மதிப்பு > cutoff || *வரி - '0' > cutlim)) {
            திரும்பு NGX_ERROR;
        }

        மதிப்பு = மதிப்பு * 10 + (*வரி - '0');
    }

    திரும்பு மதிப்பு;
}


time_t
ngx_atotm(u_char *வரி, size_t n)
{
    time_t  மதிப்பு, cutoff, cutlim;

    எனில் (n == 0) {
        திரும்பு NGX_ERROR;
    }

    cutoff = NGX_MAX_TIME_T_மதிப்பு / 10;
    cutlim = NGX_MAX_TIME_T_மதிப்பு % 10;

    க்கு (மதிப்பு = 0; n--; வரி++) {
        எனில் (*வரி < '0' || *வரி > '9') {
            திரும்பு NGX_ERROR;
        }

        எனில் (மதிப்பு >= cutoff && (மதிப்பு > cutoff || *வரி - '0' > cutlim)) {
            திரும்பு NGX_ERROR;
        }

        மதிப்பு = மதிப்பு * 10 + (*வரி - '0');
    }

    திரும்பு மதிப்பு;
}


ngx_int_t
ngx_hextoi(u_char *வரி, size_t n)
{
    u_char     c, ch;
    ngx_int_t  மதிப்பு, cutoff;

    எனில் (n == 0) {
        திரும்பு NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_மதிப்பு / 16;

    க்கு (மதிப்பு = 0; n--; வரி++) {
        எனில் (மதிப்பு > cutoff) {
            திரும்பு NGX_ERROR;
        }

        ch = *வரி;

        எனில் (ch >= '0' && ch <= '9') {
            மதிப்பு = மதிப்பு * 16 + (ch - '0');
            தொடர்;
        }

        c = (u_char) (ch | 0x20);

        எனில் (c >= 'a' && c <= 'f') {
            மதிப்பு = மதிப்பு * 16 + (c - 'a' + 10);
            தொடர்;
        }

        திரும்பு NGX_ERROR;
    }

    திரும்பு மதிப்பு;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, size_t len)
{
    நிலையான u_char  hex[] = "0123456789abcdef";

    போது (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    திரும்பு dst;
}


வெறுமை
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    நிலையான u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_internal(dst, src, basis64, 1);
}


வெறுமை
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    நிலையான u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_internal(dst, src, basis64, 0);
}


நிலையான வெறுமை
ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src, மாறிலி u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    size_t          len;

    len = src->len;
    s = src->data;
    d = dst->data;

    போது (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    எனில் (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        எனில் (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            எனில் (padding) {
                *d++ = '=';
            }

        } இல்லையெனில் {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        எனில் (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->data;
}


ngx_int_t
ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    நிலையான u_char   basis64[] = {
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

    திரும்பு ngx_decode_base64_internal(dst, src, basis64);
}


ngx_int_t
ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    நிலையான u_char   basis64[] = {
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

    திரும்பு ngx_decode_base64_internal(dst, src, basis64);
}


நிலையான ngx_int_t
ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src, மாறிலி u_char *basis)
{
    size_t          len;
    u_char         *d, *s;

    க்கு (len = 0; len < src->len; len++) {
        எனில் (src->data[len] == '=') {
            நிறுத்து;
        }

        எனில் (basis[src->data[len]] == 77) {
            திரும்பு NGX_ERROR;
        }
    }

    எனில் (len % 4 == 1) {
        திரும்பு NGX_ERROR;
    }

    s = src->data;
    d = dst->data;

    போது (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    எனில் (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    எனில் (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->data;

    திரும்பு NGX_OK;
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

    எனில் (u >= 0xf8) {

        (*p)++;
        திரும்பு 0xffffffff;

    } இல்லையெனில் எனில் (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } இல்லையெனில் எனில் (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } இல்லையெனில் எனில் (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } இல்லையெனில் {
        (*p)++;
        திரும்பு 0xffffffff;
    }

    எனில் (n - 1 < len) {
        திரும்பு 0xfffffffe;
    }

    (*p)++;

    போது (len) {
        i = *(*p)++;

        எனில் (i < 0x80) {
            திரும்பு 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    எனில் (u > valid) {
        திரும்பு u;
    }

    திரும்பு 0xffffffff;
}


size_t
ngx_utf8_length(u_char *p, size_t n)
{
    u_char  c, *last;
    size_t  len;

    last = p + n;

    க்கு (len = 0; p < last; len++) {

        c = *p;

        எனில் (c < 0x80) {
            p++;
            தொடர்;
        }

        எனில் (ngx_utf8_decode(&p, last - p) > 0x10ffff) {
            /* invalid UTF-8 */
            திரும்பு n;
        }
    }

    திரும்பு len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len)
{
    u_char  c, *next;

    எனில் (n == 0) {
        திரும்பு dst;
    }

    போது (--n) {

        c = *src;
        *dst = c;

        எனில் (c < 0x80) {

            எனில் (c != '\0') {
                dst++;
                src++;
                len--;

                தொடர்;
            }

            திரும்பு dst;
        }

        next = src;

        எனில் (ngx_utf8_decode(&next, len) > 0x10ffff) {
            /* invalid UTF-8 */
            நிறுத்து;
        }

        போது (src < next) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    திரும்பு dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, size_t size, ngx_uint_t type)
{
    u_char          prefix;
    uint32_t       *escape;
    ngx_uint_t      n;
    நிலையான u_char   hex[] = "0123456789ABCDEF";

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

    நிலையான uint32_t   uri[] = {
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

    நிலையான uint32_t   args[] = {
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

    நிலையான uint32_t   uri_component[] = {
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

    நிலையான uint32_t   html[] = {
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

    நிலையான uint32_t   refresh[] = {
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

    நிலையான uint32_t   memcached[] = {
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

    நிலையான uint32_t   mail_xtext[] = {
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

    நிலையான uint32_t  *map[] =
        { uri, args, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    நிலையான u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    prefix = map_char[type];

    எனில் (dst == வெற்று) {

        /* find the number of the characters to be escaped */

        n = 0;

        போது (size) {
            எனில் (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            size--;
        }

        திரும்பு (uintptr_t) n;
    }

    போது (size) {
        எனில் (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = prefix;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } இல்லையெனில் {
            *dst++ = *src++;
        }
        size--;
    }

    திரும்பு (uintptr_t) dst;
}


வெறுமை
ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    எண்ணிக்கை {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_second
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    போது (size--) {

        ch = *s++;

        தேர்வு (state) {
        நிலை sw_usual:
            எனில் (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                செல் done;
            }

            எனில் (ch == '%') {
                state = sw_quoted;
                நிறுத்து;
            }

            *d++ = ch;
            நிறுத்து;

        நிலை sw_quoted:

            எனில் (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_second;
                நிறுத்து;
            }

            c = (u_char) (ch | 0x20);
            எனில் (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_second;
                நிறுத்து;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            நிறுத்து;

        நிலை sw_quoted_second:

            state = sw_usual;

            எனில் (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                எனில் (type & NGX_UNESCAPE_REDIRECT) {
                    எனில் (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        நிறுத்து;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    நிறுத்து;
                }

                *d++ = ch;

                நிறுத்து;
            }

            c = (u_char) (ch | 0x20);
            எனில் (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                எனில் (type & NGX_UNESCAPE_URI) {
                    எனில் (ch == '?') {
                        *d++ = ch;
                        செல் done;
                    }

                    *d++ = ch;
                    நிறுத்து;
                }

                எனில் (type & NGX_UNESCAPE_REDIRECT) {
                    எனில் (ch == '?') {
                        *d++ = ch;
                        செல் done;
                    }

                    எனில் (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        நிறுத்து;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    நிறுத்து;
                }

                *d++ = ch;

                நிறுத்து;
            }

            /* the invalid quoted character */

            நிறுத்து;
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

    எனில் (dst == வெற்று) {

        len = 0;

        போது (size) {
            தேர்வு (*src++) {

            நிலை '<':
                len += அளவு("&lt;") - 2;
                நிறுத்து;

            நிலை '>':
                len += அளவு("&gt;") - 2;
                நிறுத்து;

            நிலை '&':
                len += அளவு("&amp;") - 2;
                நிறுத்து;

            நிலை '"':
                len += அளவு("&quot;") - 2;
                நிறுத்து;

            இயல்பு:
                நிறுத்து;
            }
            size--;
        }

        திரும்பு (uintptr_t) len;
    }

    போது (size) {
        ch = *src++;

        தேர்வு (ch) {

        நிலை '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            நிறுத்து;

        நிலை '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            நிறுத்து;

        நிலை '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            நிறுத்து;

        நிலை '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            நிறுத்து;

        இயல்பு:
            *dst++ = ch;
            நிறுத்து;
        }
        size--;
    }

    திரும்பு (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, size_t size)
{
    u_char      ch;
    ngx_uint_t  len;

    எனில் (dst == வெற்று) {
        len = 0;

        போது (size) {
            ch = *src++;

            எனில் (ch == '\\' || ch == '"') {
                len++;

            } இல்லையெனில் எனில் (ch <= 0x1f) {

                தேர்வு (ch) {
                நிலை '\n':
                நிலை '\r':
                நிலை '\t':
                நிலை '\b':
                நிலை '\f':
                    len++;
                    நிறுத்து;

                இயல்பு:
                    len += அளவு("\\u001F") - 2;
                }
            }

            size--;
        }

        திரும்பு (uintptr_t) len;
    }

    போது (size) {
        ch = *src++;

        எனில் (ch > 0x1f) {

            எனில் (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } இல்லையெனில் {
            *dst++ = '\\';

            தேர்வு (ch) {
            நிலை '\n':
                *dst++ = 'n';
                நிறுத்து;

            நிலை '\r':
                *dst++ = 'r';
                நிறுத்து;

            நிலை '\t':
                *dst++ = 't';
                நிறுத்து;

            நிலை '\b':
                *dst++ = 'b';
                நிறுத்து;

            நிலை '\f':
                *dst++ = 'f';
                நிறுத்து;

            இயல்பு:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        size--;
    }

    திரும்பு (uintptr_t) dst;
}


வெறுமை
ngx_str_rbtree_செருகு_மதிப்பு(ngx_rbtree_முனை_t *temp,
    ngx_rbtree_முனை_t *முனை, ngx_rbtree_முனை_t *sentinel)
{
    ngx_str_முனை_t      *n, *t;
    ngx_rbtree_முனை_t  **p;

    க்கு ( ;; ) {

        n = (ngx_str_முனை_t *) முனை;
        t = (ngx_str_முனை_t *) temp;

        எனில் (முனை->சாவி != temp->சாவி) {

            p = (முனை->சாவி < temp->சாவி) ? &temp->இடது : &temp->வலது;

        } இல்லையெனில் எனில் (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &temp->இடது : &temp->வலது;

        } இல்லையெனில் {
            p = (ngx_memcmp(n->str.data, t->str.data, n->str.len) < 0)
                 ? &temp->இடது : &temp->வலது;
        }

        எனில் (*p == sentinel) {
            நிறுத்து;
        }

        temp = *p;
    }

    *p = முனை;
    முனை->parent = temp;
    முனை->இடது = sentinel;
    முனை->வலது = sentinel;
    ngx_rbt_red(முனை);
}


ngx_str_முனை_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_முனை_t     *n;
    ngx_rbtree_முனை_t  *முனை, *sentinel;

    முனை = rbtree->வேர்;
    sentinel = rbtree->sentinel;

    போது (முனை != sentinel) {

        n = (ngx_str_முனை_t *) முனை;

        எனில் (hash != முனை->சாவி) {
            முனை = (hash < முனை->சாவி) ? முனை->இடது : முனை->வலது;
            தொடர்;
        }

        எனில் (val->len != n->str.len) {
            முனை = (val->len < n->str.len) ? முனை->இடது : முனை->வலது;
            தொடர்;
        }

        rc = ngx_memcmp(val->data, n->str.data, val->len);

        எனில் (rc < 0) {
            முனை = முனை->இடது;
            தொடர்;
        }

        எனில் (rc > 0) {
            முனை = முனை->வலது;
            தொடர்;
        }

        திரும்பு n;
    }

    திரும்பு வெற்று;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

வெறுமை
ngx_sort(வெறுமை *base, size_t n, size_t size,
    ngx_int_t (*cmp)(மாறிலி வெறுமை *, மாறிலி வெறுமை *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(size, ngx_cycle->log);
    எனில் (p == வெற்று) {
        திரும்பு;
    }

    க்கு (p1 = (u_char *) base + size;
         p1 < (u_char *) base + n * size;
         p1 += size)
    {
        ngx_memcpy(p, p1, size);

        க்கு (p2 = p1;
             p2 > (u_char *) base && cmp(p2 - size, p) > 0;
             p2 -= size)
        {
            ngx_memcpy(p2, p2 - size, size);
        }

        ngx_memcpy(p2, p, size);
    }

    ngx_free(p);
}


வெறுமை
ngx_explicit_memzero(வெறுமை *buf, size_t n)
{
    ngx_memzero(buf, n);
    ngx_memory_barrier();
}


#எனில் (NGX_MEMCPY_LIMIT)

வெறுமை *
ngx_memcpy(வெறுமை *dst, மாறிலி வெறுமை *src, size_t n)
{
    எனில் (n > NGX_MEMCPY_LIMIT) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "memcpy %uz bytes", n);
        ngx_debug_point();
    }

    திரும்பு memcpy(dst, src, n);
}

#endif
