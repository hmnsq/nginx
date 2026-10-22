
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर u_char *ngx_sprintf_अङ्कः(u_char *buf, u_char *last, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
स्थिर u_char *ngx_sprintf_str(u_char *buf, u_char *last, u_char *src,
    size_t len, ngx_uint_t hexadecimal);
स्थिर शून्य ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ध्रुव u_char *basis, ngx_uint_t padding);
स्थिर ngx_int_t ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ध्रुव u_char *basis);


शून्य
ngx_strlow(u_char *dst, u_char *src, size_t n)
{
    याव (n) {
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

    अत्थाय (i = 0; i < n; i++) {

        सचे (p[i] == '\0') {
            पटिनिवत्त i;
        }
    }

    पटिनिवत्त n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, size_t n)
{
    सचे (n == 0) {
        पटिनिवत्त dst;
    }

    याव (--n) {
        *dst = *src;

        सचे (*dst == '\0') {
            पटिनिवत्त dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    पटिनिवत्त dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    सचे (dst == रिक्त) {
        पटिनिवत्त रिक्त;
    }

    ngx_memcpy(dst, src->data, src->len);

    पटिनिवत्त dst;
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
ngx_sprintf(u_char *buf, ध्रुव वर्ण *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, (शून्य *) -1, fmt, args);
    va_end(args);

    पटिनिवत्त p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, size_t max, ध्रुव वर्ण *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, args);
    va_end(args);

    पटिनिवत्त p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *last, ध्रुव वर्ण *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, last, fmt, args);
    va_end(args);

    पटिनिवत्त p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *last, ध्रुव वर्ण *fmt, va_list args)
{
    u_char                *p, zero;
    पूर्णाङ्क                    d;
    द्विगुण                 f;
    size_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             width, sign, hex, max_width, frac_width, scale, n;
    ngx_str_t             *v;
    ngx_variable_मानम्_t  *vv;

    याव (*fmt && buf < last) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        सचे (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (size_t) -1;

            याव (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }


            अत्थाय ( ;; ) {
                विकल्प (*fmt) {

                पक्ष 'u':
                    sign = 0;
                    fmt++;
                    अनुवर्त;

                पक्ष 'm':
                    max_width = 1;
                    fmt++;
                    अनुवर्त;

                पक्ष 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    अनुवर्त;

                पक्ष 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    अनुवर्त;

                पक्ष '.':
                    fmt++;

                    याव (*fmt >= '0' && *fmt <= '9') {
                        frac_width = frac_width * 10 + (*fmt++ - '0');
                    }

                    भङ्ग;

                पक्ष '*':
                    slen = va_arg(args, size_t);
                    fmt++;
                    अनुवर्त;

                स्वभाव:
                    भङ्ग;
                }

                भङ्ग;
            }


            विकल्प (*fmt) {

            पक्ष 'V':
                v = va_arg(args, ngx_str_t *);

                buf = ngx_sprintf_str(buf, last, v->data, v->len, hex);
                fmt++;

                अनुवर्त;

            पक्ष 'v':
                vv = va_arg(args, ngx_variable_मानम्_t *);

                buf = ngx_sprintf_str(buf, last, vv->data, vv->len, hex);
                fmt++;

                अनुवर्त;

            पक्ष 's':
                p = va_arg(args, u_char *);

                buf = ngx_sprintf_str(buf, last, p, slen, hex);
                fmt++;

                अनुवर्त;

            पक्ष 'O':
                i64 = (int64_t) va_arg(args, off_t);
                sign = 1;
                भङ्ग;

            पक्ष 'P':
                i64 = (int64_t) va_arg(args, ngx_pid_t);
                sign = 1;
                भङ्ग;

            पक्ष 'T':
                i64 = (int64_t) va_arg(args, time_t);
                sign = 1;
                भङ्ग;

            पक्ष 'M':
                ms = (ngx_msec_t) va_arg(args, ngx_msec_t);
                सचे ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } अञ्ञथा {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                भङ्ग;

            पक्ष 'z':
                सचे (sign) {
                    i64 = (int64_t) va_arg(args, ssize_t);
                } अञ्ञथा {
                    ui64 = (uint64_t) va_arg(args, size_t);
                }
                भङ्ग;

            पक्ष 'i':
                सचे (sign) {
                    i64 = (int64_t) va_arg(args, ngx_int_t);
                } अञ्ञथा {
                    ui64 = (uint64_t) va_arg(args, ngx_uint_t);
                }

                सचे (max_width) {
                    width = NGX_INT_T_LEN;
                }

                भङ्ग;

            पक्ष 'd':
                सचे (sign) {
                    i64 = (int64_t) va_arg(args, पूर्णाङ्क);
                } अञ्ञथा {
                    ui64 = (uint64_t) va_arg(args, u_int);
                }
                भङ्ग;

            पक्ष 'l':
                सचे (sign) {
                    i64 = (int64_t) va_arg(args, long);
                } अञ्ञथा {
                    ui64 = (uint64_t) va_arg(args, u_long);
                }
                भङ्ग;

            पक्ष 'D':
                सचे (sign) {
                    i64 = (int64_t) va_arg(args, int32_t);
                } अञ्ञथा {
                    ui64 = (uint64_t) va_arg(args, uint32_t);
                }
                भङ्ग;

            पक्ष 'L':
                सचे (sign) {
                    i64 = va_arg(args, int64_t);
                } अञ्ञथा {
                    ui64 = va_arg(args, uint64_t);
                }
                भङ्ग;

            पक्ष 'A':
                सचे (sign) {
                    i64 = (int64_t) va_arg(args, ngx_atomic_int_t);
                } अञ्ञथा {
                    ui64 = (uint64_t) va_arg(args, ngx_atomic_uint_t);
                }

                सचे (max_width) {
                    width = NGX_ATOMIC_T_LEN;
                }

                भङ्ग;

            पक्ष 'f':
                f = va_arg(args, द्विगुण);

                सचे (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                सचे (frac_width) {

                    scale = 1;
                    अत्थाय (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (द्विगुण) ui64) * scale + 0.5);

                    सचे (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_अङ्कः(buf, last, ui64, zero, 0, width);

                सचे (frac_width) {
                    सचे (buf < last) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_अङ्कः(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                अनुवर्त;

#सचे !(NGX_WIN32)
            पक्ष 'r':
                i64 = (int64_t) va_arg(args, rlim_t);
                sign = 1;
                भङ्ग;
#endif

            पक्ष 'p':
                ui64 = (uintptr_t) va_arg(args, शून्य *);
                hex = 2;
                sign = 0;
                zero = '0';
                width = 2 * परिमाण(शून्य *);
                भङ्ग;

            पक्ष 'c':
                d = va_arg(args, पूर्णाङ्क);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                अनुवर्त;

            पक्ष 'Z':
                *buf++ = '\0';
                fmt++;

                अनुवर्त;

            पक्ष 'N':
#सचे (NGX_WIN32)
                *buf++ = CR;
                सचे (buf < last) {
                    *buf++ = LF;
                }
#अञ्ञथा
                *buf++ = LF;
#endif
                fmt++;

                अनुवर्त;

            पक्ष '%':
                *buf++ = '%';
                fmt++;

                अनुवर्त;

            स्वभाव:
                *buf++ = *fmt++;

                अनुवर्त;
            }

            सचे (sign) {
                सचे (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } अञ्ञथा {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_अङ्कः(buf, last, ui64, zero, hex, width);

            fmt++;

        } अञ्ञथा {
            *buf++ = *fmt++;
        }
    }

    पटिनिवत्त buf;
}


स्थिर u_char *
ngx_sprintf_अङ्कः(u_char *buf, u_char *last, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t width)
{
    u_char         *p, temp[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    size_t          len;
    uint32_t        ui32;
    स्थिर u_char   hex[] = "0123456789abcdef";
    स्थिर u_char   HEX[] = "0123456789ABCDEF";

    p = temp + NGX_INT64_LEN;

    सचे (hexadecimal == 0) {

        सचे (ui64 <= (uint64_t) NGX_MAX_UINT32_मानम्) {

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

            करोहि {
                *--p = (u_char) (ui32 % 10 + '0');
            } याव (ui32 /= 10);

        } अञ्ञथा {
            करोहि {
                *--p = (u_char) (ui64 % 10 + '0');
            } याव (ui64 /= 10);
        }

    } अञ्ञथा सचे (hexadecimal == 1) {

        करोहि {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } याव (ui64 >>= 4);

    } अञ्ञथा { /* hexadecimal == 2 */

        करोहि {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } याव (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    याव (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    सचे (buf + len > last) {
        len = last - buf;
    }

    पटिनिवत्त ngx_cpymem(buf, p, len);
}


स्थिर u_char *
ngx_sprintf_str(u_char *buf, u_char *last, u_char *src, size_t len,
    ngx_uint_t hexadecimal)
{
    स्थिर u_char   hex[] = "0123456789abcdef";
    स्थिर u_char   HEX[] = "0123456789ABCDEF";

    सचे (hexadecimal == 0) {

        सचे (len == (size_t) -1) {
            याव (*src && buf < last) {
                *buf++ = *src++;
            }

        } अञ्ञथा {
            len = ngx_min((size_t) (last - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } अञ्ञथा सचे (hexadecimal == 1) {

        सचे (len == (size_t) -1) {

            याव (*src && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } अञ्ञथा {

            याव (len-- && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } अञ्ञथा { /* hexadecimal == 2 */

        सचे (len == (size_t) -1) {

            याव (*src && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } अञ्ञथा {

            याव (len-- && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    पटिनिवत्त buf;
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

    अत्थाय ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        सचे (c1 == c2) {

            सचे (c1) {
                अनुवर्त;
            }

            पटिनिवत्त 0;
        }

        पटिनिवत्त c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    याव (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        सचे (c1 == c2) {

            सचे (c1) {
                n--;
                अनुवर्त;
            }

            पटिनिवत्त 0;
        }

        पटिनिवत्त c1 - c2;
    }

    पटिनिवत्त 0;
}


u_char *
ngx_strnstr(u_char *s1, वर्ण *s2, size_t len)
{
    u_char  c1, c2;
    size_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    करोहि {
        करोहि {
            सचे (len-- == 0) {
                पटिनिवत्त रिक्त;
            }

            c1 = *s1++;

            सचे (c1 == 0) {
                पटिनिवत्त रिक्त;
            }

        } याव (c1 != c2);

        सचे (n > len) {
            पटिनिवत्त रिक्त;
        }

    } याव (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    पटिनिवत्त --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, वर्ण *s2, size_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    करोहि {
        करोहि {
            c1 = *s1++;

            सचे (c1 == 0) {
                पटिनिवत्त रिक्त;
            }

        } याव (c1 != c2);

    } याव (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    पटिनिवत्त --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, वर्ण *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    करोहि {
        करोहि {
            c1 = (ngx_uint_t) *s1++;

            सचे (c1 == 0) {
                पटिनिवत्त रिक्त;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } याव (c1 != c2);

    } याव (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    पटिनिवत्त --s1;
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

    करोहि {
        करोहि {
            सचे (s1 >= last) {
                पटिनिवत्त रिक्त;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } याव (c1 != c2);

    } याव (ngx_strncasecmp(s1, s2, n) != 0);

    पटिनिवत्त --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, size_t n)
{
    सचे (n == 0) {
        पटिनिवत्त 0;
    }

    n--;

    अत्थाय ( ;; ) {
        सचे (s1[n] != s2[n]) {
            पटिनिवत्त s1[n] - s2[n];
        }

        सचे (n == 0) {
            पटिनिवत्त 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n)
{
    u_char  c1, c2;

    सचे (n == 0) {
        पटिनिवत्त 0;
    }

    n--;

    अत्थाय ( ;; ) {
        c1 = s1[n];
        सचे (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        सचे (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        सचे (c1 != c2) {
            पटिनिवत्त c1 - c2;
        }

        सचे (n == 0) {
            पटिनिवत्त 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2)
{
    size_t     n;
    ngx_int_t  m, z;

    सचे (n1 <= n2) {
        n = n1;
        z = -1;

    } अञ्ञथा {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    सचे (m || n1 == n2) {
        पटिनिवत्त m;
    }

    पटिनिवत्त z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    अत्थाय ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        सचे (c1 == c2) {

            सचे (c1) {
                अनुवर्त;
            }

            पटिनिवत्त 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        पटिनिवत्त c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    याव (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#सचे (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#endif

        सचे (c1 == c2) {

            सचे (c1) {
                n--;
                अनुवर्त;
            }

            पटिनिवत्त 0;
        }

        /* we need '/' to be the lowest character */

        सचे (c1 == 0 || c2 == 0) {
            पटिनिवत्त c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        पटिनिवत्त c1 - c2;
    }

    पटिनिवत्त 0;
}


ngx_int_t
ngx_atoi(u_char *पङ्क्तिः, size_t n)
{
    ngx_int_t  मानम्, cutoff, cutlim;

    सचे (n == 0) {
        पटिनिवत्त NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_मानम् / 10;
    cutlim = NGX_MAX_INT_T_मानम् % 10;

    अत्थाय (मानम् = 0; n--; पङ्क्तिः++) {
        सचे (*पङ्क्तिः < '0' || *पङ्क्तिः > '9') {
            पटिनिवत्त NGX_ERROR;
        }

        सचे (मानम् >= cutoff && (मानम् > cutoff || *पङ्क्तिः - '0' > cutlim)) {
            पटिनिवत्त NGX_ERROR;
        }

        मानम् = मानम् * 10 + (*पङ्क्तिः - '0');
    }

    पटिनिवत्त मानम्;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *पङ्क्तिः, size_t n, size_t point)
{
    ngx_int_t   मानम्, cutoff, cutlim;
    ngx_uint_t  dot;

    सचे (n == 0) {
        पटिनिवत्त NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_मानम् / 10;
    cutlim = NGX_MAX_INT_T_मानम् % 10;

    dot = 0;

    अत्थाय (मानम् = 0; n--; पङ्क्तिः++) {

        सचे (point == 0) {
            पटिनिवत्त NGX_ERROR;
        }

        सचे (*पङ्क्तिः == '.') {
            सचे (dot) {
                पटिनिवत्त NGX_ERROR;
            }

            dot = 1;
            अनुवर्त;
        }

        सचे (*पङ्क्तिः < '0' || *पङ्क्तिः > '9') {
            पटिनिवत्त NGX_ERROR;
        }

        सचे (मानम् >= cutoff && (मानम् > cutoff || *पङ्क्तिः - '0' > cutlim)) {
            पटिनिवत्त NGX_ERROR;
        }

        मानम् = मानम् * 10 + (*पङ्क्तिः - '0');
        point -= dot;
    }

    याव (point--) {
        सचे (मानम् > cutoff) {
            पटिनिवत्त NGX_ERROR;
        }

        मानम् = मानम् * 10;
    }

    पटिनिवत्त मानम्;
}


ssize_t
ngx_atosz(u_char *पङ्क्तिः, size_t n)
{
    ssize_t  मानम्, cutoff, cutlim;

    सचे (n == 0) {
        पटिनिवत्त NGX_ERROR;
    }

    cutoff = NGX_MAX_SIZE_T_मानम् / 10;
    cutlim = NGX_MAX_SIZE_T_मानम् % 10;

    अत्थाय (मानम् = 0; n--; पङ्क्तिः++) {
        सचे (*पङ्क्तिः < '0' || *पङ्क्तिः > '9') {
            पटिनिवत्त NGX_ERROR;
        }

        सचे (मानम् >= cutoff && (मानम् > cutoff || *पङ्क्तिः - '0' > cutlim)) {
            पटिनिवत्त NGX_ERROR;
        }

        मानम् = मानम् * 10 + (*पङ्क्तिः - '0');
    }

    पटिनिवत्त मानम्;
}


off_t
ngx_atoof(u_char *पङ्क्तिः, size_t n)
{
    off_t  मानम्, cutoff, cutlim;

    सचे (n == 0) {
        पटिनिवत्त NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_मानम् / 10;
    cutlim = NGX_MAX_OFF_T_मानम् % 10;

    अत्थाय (मानम् = 0; n--; पङ्क्तिः++) {
        सचे (*पङ्क्तिः < '0' || *पङ्क्तिः > '9') {
            पटिनिवत्त NGX_ERROR;
        }

        सचे (मानम् >= cutoff && (मानम् > cutoff || *पङ्क्तिः - '0' > cutlim)) {
            पटिनिवत्त NGX_ERROR;
        }

        मानम् = मानम् * 10 + (*पङ्क्तिः - '0');
    }

    पटिनिवत्त मानम्;
}


time_t
ngx_atotm(u_char *पङ्क्तिः, size_t n)
{
    time_t  मानम्, cutoff, cutlim;

    सचे (n == 0) {
        पटिनिवत्त NGX_ERROR;
    }

    cutoff = NGX_MAX_TIME_T_मानम् / 10;
    cutlim = NGX_MAX_TIME_T_मानम् % 10;

    अत्थाय (मानम् = 0; n--; पङ्क्तिः++) {
        सचे (*पङ्क्तिः < '0' || *पङ्क्तिः > '9') {
            पटिनिवत्त NGX_ERROR;
        }

        सचे (मानम् >= cutoff && (मानम् > cutoff || *पङ्क्तिः - '0' > cutlim)) {
            पटिनिवत्त NGX_ERROR;
        }

        मानम् = मानम् * 10 + (*पङ्क्तिः - '0');
    }

    पटिनिवत्त मानम्;
}


ngx_int_t
ngx_hextoi(u_char *पङ्क्तिः, size_t n)
{
    u_char     c, ch;
    ngx_int_t  मानम्, cutoff;

    सचे (n == 0) {
        पटिनिवत्त NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_मानम् / 16;

    अत्थाय (मानम् = 0; n--; पङ्क्तिः++) {
        सचे (मानम् > cutoff) {
            पटिनिवत्त NGX_ERROR;
        }

        ch = *पङ्क्तिः;

        सचे (ch >= '0' && ch <= '9') {
            मानम् = मानम् * 16 + (ch - '0');
            अनुवर्त;
        }

        c = (u_char) (ch | 0x20);

        सचे (c >= 'a' && c <= 'f') {
            मानम् = मानम् * 16 + (c - 'a' + 10);
            अनुवर्त;
        }

        पटिनिवत्त NGX_ERROR;
    }

    पटिनिवत्त मानम्;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, size_t len)
{
    स्थिर u_char  hex[] = "0123456789abcdef";

    याव (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    पटिनिवत्त dst;
}


शून्य
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    स्थिर u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_internal(dst, src, basis64, 1);
}


शून्य
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    स्थिर u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_internal(dst, src, basis64, 0);
}


स्थिर शून्य
ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src, ध्रुव u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    size_t          len;

    len = src->len;
    s = src->data;
    d = dst->data;

    याव (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    सचे (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        सचे (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            सचे (padding) {
                *d++ = '=';
            }

        } अञ्ञथा {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        सचे (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->data;
}


ngx_int_t
ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    स्थिर u_char   basis64[] = {
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

    पटिनिवत्त ngx_decode_base64_internal(dst, src, basis64);
}


ngx_int_t
ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    स्थिर u_char   basis64[] = {
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

    पटिनिवत्त ngx_decode_base64_internal(dst, src, basis64);
}


स्थिर ngx_int_t
ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src, ध्रुव u_char *basis)
{
    size_t          len;
    u_char         *d, *s;

    अत्थाय (len = 0; len < src->len; len++) {
        सचे (src->data[len] == '=') {
            भङ्ग;
        }

        सचे (basis[src->data[len]] == 77) {
            पटिनिवत्त NGX_ERROR;
        }
    }

    सचे (len % 4 == 1) {
        पटिनिवत्त NGX_ERROR;
    }

    s = src->data;
    d = dst->data;

    याव (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    सचे (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    सचे (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->data;

    पटिनिवत्त NGX_OK;
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

    सचे (u >= 0xf8) {

        (*p)++;
        पटिनिवत्त 0xffffffff;

    } अञ्ञथा सचे (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } अञ्ञथा सचे (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } अञ्ञथा सचे (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } अञ्ञथा {
        (*p)++;
        पटिनिवत्त 0xffffffff;
    }

    सचे (n - 1 < len) {
        पटिनिवत्त 0xfffffffe;
    }

    (*p)++;

    याव (len) {
        i = *(*p)++;

        सचे (i < 0x80) {
            पटिनिवत्त 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    सचे (u > valid) {
        पटिनिवत्त u;
    }

    पटिनिवत्त 0xffffffff;
}


size_t
ngx_utf8_length(u_char *p, size_t n)
{
    u_char  c, *last;
    size_t  len;

    last = p + n;

    अत्थाय (len = 0; p < last; len++) {

        c = *p;

        सचे (c < 0x80) {
            p++;
            अनुवर्त;
        }

        सचे (ngx_utf8_decode(&p, last - p) > 0x10ffff) {
            /* invalid UTF-8 */
            पटिनिवत्त n;
        }
    }

    पटिनिवत्त len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len)
{
    u_char  c, *next;

    सचे (n == 0) {
        पटिनिवत्त dst;
    }

    याव (--n) {

        c = *src;
        *dst = c;

        सचे (c < 0x80) {

            सचे (c != '\0') {
                dst++;
                src++;
                len--;

                अनुवर्त;
            }

            पटिनिवत्त dst;
        }

        next = src;

        सचे (ngx_utf8_decode(&next, len) > 0x10ffff) {
            /* invalid UTF-8 */
            भङ्ग;
        }

        याव (src < next) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    पटिनिवत्त dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, size_t size, ngx_uint_t type)
{
    u_char          prefix;
    uint32_t       *escape;
    ngx_uint_t      n;
    स्थिर u_char   hex[] = "0123456789ABCDEF";

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

    स्थिर uint32_t   uri[] = {
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

    स्थिर uint32_t   args[] = {
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

    स्थिर uint32_t   uri_component[] = {
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

    स्थिर uint32_t   html[] = {
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

    स्थिर uint32_t   refresh[] = {
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

    स्थिर uint32_t   memcached[] = {
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

    स्थिर uint32_t   mail_xtext[] = {
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

    स्थिर uint32_t  *map[] =
        { uri, args, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    स्थिर u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    prefix = map_char[type];

    सचे (dst == रिक्त) {

        /* find the number of the characters to be escaped */

        n = 0;

        याव (size) {
            सचे (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            size--;
        }

        पटिनिवत्त (uintptr_t) n;
    }

    याव (size) {
        सचे (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = prefix;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } अञ्ञथा {
            *dst++ = *src++;
        }
        size--;
    }

    पटिनिवत्त (uintptr_t) dst;
}


शून्य
ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    गणना {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_second
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    याव (size--) {

        ch = *s++;

        विकल्प (state) {
        पक्ष sw_usual:
            सचे (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                गच्छ done;
            }

            सचे (ch == '%') {
                state = sw_quoted;
                भङ्ग;
            }

            *d++ = ch;
            भङ्ग;

        पक्ष sw_quoted:

            सचे (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_second;
                भङ्ग;
            }

            c = (u_char) (ch | 0x20);
            सचे (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_second;
                भङ्ग;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            भङ्ग;

        पक्ष sw_quoted_second:

            state = sw_usual;

            सचे (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                सचे (type & NGX_UNESCAPE_REDIRECT) {
                    सचे (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        भङ्ग;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    भङ्ग;
                }

                *d++ = ch;

                भङ्ग;
            }

            c = (u_char) (ch | 0x20);
            सचे (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                सचे (type & NGX_UNESCAPE_URI) {
                    सचे (ch == '?') {
                        *d++ = ch;
                        गच्छ done;
                    }

                    *d++ = ch;
                    भङ्ग;
                }

                सचे (type & NGX_UNESCAPE_REDIRECT) {
                    सचे (ch == '?') {
                        *d++ = ch;
                        गच्छ done;
                    }

                    सचे (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        भङ्ग;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    भङ्ग;
                }

                *d++ = ch;

                भङ्ग;
            }

            /* the invalid quoted character */

            भङ्ग;
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

    सचे (dst == रिक्त) {

        len = 0;

        याव (size) {
            विकल्प (*src++) {

            पक्ष '<':
                len += परिमाण("&lt;") - 2;
                भङ्ग;

            पक्ष '>':
                len += परिमाण("&gt;") - 2;
                भङ्ग;

            पक्ष '&':
                len += परिमाण("&amp;") - 2;
                भङ्ग;

            पक्ष '"':
                len += परिमाण("&quot;") - 2;
                भङ्ग;

            स्वभाव:
                भङ्ग;
            }
            size--;
        }

        पटिनिवत्त (uintptr_t) len;
    }

    याव (size) {
        ch = *src++;

        विकल्प (ch) {

        पक्ष '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            भङ्ग;

        पक्ष '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            भङ्ग;

        पक्ष '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            भङ्ग;

        पक्ष '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            भङ्ग;

        स्वभाव:
            *dst++ = ch;
            भङ्ग;
        }
        size--;
    }

    पटिनिवत्त (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, size_t size)
{
    u_char      ch;
    ngx_uint_t  len;

    सचे (dst == रिक्त) {
        len = 0;

        याव (size) {
            ch = *src++;

            सचे (ch == '\\' || ch == '"') {
                len++;

            } अञ्ञथा सचे (ch <= 0x1f) {

                विकल्प (ch) {
                पक्ष '\n':
                पक्ष '\r':
                पक्ष '\t':
                पक्ष '\b':
                पक्ष '\f':
                    len++;
                    भङ्ग;

                स्वभाव:
                    len += परिमाण("\\u001F") - 2;
                }
            }

            size--;
        }

        पटिनिवत्त (uintptr_t) len;
    }

    याव (size) {
        ch = *src++;

        सचे (ch > 0x1f) {

            सचे (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } अञ्ञथा {
            *dst++ = '\\';

            विकल्प (ch) {
            पक्ष '\n':
                *dst++ = 'n';
                भङ्ग;

            पक्ष '\r':
                *dst++ = 'r';
                भङ्ग;

            पक्ष '\t':
                *dst++ = 't';
                भङ्ग;

            पक्ष '\b':
                *dst++ = 'b';
                भङ्ग;

            पक्ष '\f':
                *dst++ = 'f';
                भङ्ग;

            स्वभाव:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        size--;
    }

    पटिनिवत्त (uintptr_t) dst;
}


शून्य
ngx_str_rbtree_निवेशय_मानम्(ngx_rbtree_ग्रन्थिः_t *temp,
    ngx_rbtree_ग्रन्थिः_t *ग्रन्थिः, ngx_rbtree_ग्रन्थिः_t *sentinel)
{
    ngx_str_ग्रन्थिः_t      *n, *t;
    ngx_rbtree_ग्रन्थिः_t  **p;

    अत्थाय ( ;; ) {

        n = (ngx_str_ग्रन्थिः_t *) ग्रन्थिः;
        t = (ngx_str_ग्रन्थिः_t *) temp;

        सचे (ग्रन्थिः->कुञ्चिका != temp->कुञ्चिका) {

            p = (ग्रन्थिः->कुञ्चिका < temp->कुञ्चिका) ? &temp->वामः : &temp->दक्षिणः;

        } अञ्ञथा सचे (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &temp->वामः : &temp->दक्षिणः;

        } अञ्ञथा {
            p = (ngx_memcmp(n->str.data, t->str.data, n->str.len) < 0)
                 ? &temp->वामः : &temp->दक्षिणः;
        }

        सचे (*p == sentinel) {
            भङ्ग;
        }

        temp = *p;
    }

    *p = ग्रन्थिः;
    ग्रन्थिः->parent = temp;
    ग्रन्थिः->वामः = sentinel;
    ग्रन्थिः->दक्षिणः = sentinel;
    ngx_rbt_red(ग्रन्थिः);
}


ngx_str_ग्रन्थिः_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_ग्रन्थिः_t     *n;
    ngx_rbtree_ग्रन्थिः_t  *ग्रन्थिः, *sentinel;

    ग्रन्थिः = rbtree->मूलम्;
    sentinel = rbtree->sentinel;

    याव (ग्रन्थिः != sentinel) {

        n = (ngx_str_ग्रन्थिः_t *) ग्रन्थिः;

        सचे (hash != ग्रन्थिः->कुञ्चिका) {
            ग्रन्थिः = (hash < ग्रन्थिः->कुञ्चिका) ? ग्रन्थिः->वामः : ग्रन्थिः->दक्षिणः;
            अनुवर्त;
        }

        सचे (val->len != n->str.len) {
            ग्रन्थिः = (val->len < n->str.len) ? ग्रन्थिः->वामः : ग्रन्थिः->दक्षिणः;
            अनुवर्त;
        }

        rc = ngx_memcmp(val->data, n->str.data, val->len);

        सचे (rc < 0) {
            ग्रन्थिः = ग्रन्थिः->वामः;
            अनुवर्त;
        }

        सचे (rc > 0) {
            ग्रन्थिः = ग्रन्थिः->दक्षिणः;
            अनुवर्त;
        }

        पटिनिवत्त n;
    }

    पटिनिवत्त रिक्त;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

शून्य
ngx_sort(शून्य *base, size_t n, size_t size,
    ngx_int_t (*cmp)(ध्रुव शून्य *, ध्रुव शून्य *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(size, ngx_cycle->log);
    सचे (p == रिक्त) {
        पटिनिवत्त;
    }

    अत्थाय (p1 = (u_char *) base + size;
         p1 < (u_char *) base + n * size;
         p1 += size)
    {
        ngx_memcpy(p, p1, size);

        अत्थाय (p2 = p1;
             p2 > (u_char *) base && cmp(p2 - size, p) > 0;
             p2 -= size)
        {
            ngx_memcpy(p2, p2 - size, size);
        }

        ngx_memcpy(p2, p, size);
    }

    ngx_free(p);
}


शून्य
ngx_explicit_memzero(शून्य *buf, size_t n)
{
    ngx_memzero(buf, n);
    ngx_memory_barrier();
}


#सचे (NGX_MEMCPY_LIMIT)

शून्य *
ngx_memcpy(शून्य *dst, ध्रुव शून्य *src, size_t n)
{
    सचे (n > NGX_MEMCPY_LIMIT) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "memcpy %uz bytes", n);
        ngx_debug_point();
    }

    पटिनिवत्त memcpy(dst, src, n);
}

#endif
