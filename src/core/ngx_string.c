
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर u_char *ngx_sprintf_अंक(u_char *buf, u_char *last, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
स्थिर u_char *ngx_sprintf_str(u_char *buf, u_char *last, u_char *src,
    माप_प्रकार len, ngx_uint_t hexadecimal);
स्थिर शून्य ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ध्रुव u_char *basis, ngx_uint_t padding);
स्थिर ngx_int_t ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    ध्रुव u_char *basis);


शून्य
ngx_strlow(u_char *dst, u_char *src, माप_प्रकार n)
{
    यावत् (n) {
        *dst = ngx_tolower(*src);
        dst++;
        src++;
        n--;
    }
}


माप_प्रकार
ngx_strnlen(u_char *p, माप_प्रकार n)
{
    माप_प्रकार  i;

    प्रति (i = 0; i < n; i++) {

        यदि (p[i] == '\0') {
            प्रत्यागम i;
        }
    }

    प्रत्यागम n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, माप_प्रकार n)
{
    यदि (n == 0) {
        प्रत्यागम dst;
    }

    यावत् (--n) {
        *dst = *src;

        यदि (*dst == '\0') {
            प्रत्यागम dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    प्रत्यागम dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    यदि (dst == रिक्त) {
        प्रत्यागम रिक्त;
    }

    ngx_memcpy(dst, src->data, src->len);

    प्रत्यागम dst;
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
    बहु_सूची   args;

    बहु_शुरू(args, fmt);
    p = ngx_vslprintf(buf, (शून्य *) -1, fmt, args);
    बहु_पूर्ण(args);

    प्रत्यागम p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, माप_प्रकार max, ध्रुव वर्ण *fmt, ...)
{
    u_char   *p;
    बहु_सूची   args;

    बहु_शुरू(args, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, args);
    बहु_पूर्ण(args);

    प्रत्यागम p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *last, ध्रुव वर्ण *fmt, ...)
{
    u_char   *p;
    बहु_सूची   args;

    बहु_शुरू(args, fmt);
    p = ngx_vslprintf(buf, last, fmt, args);
    बहु_पूर्ण(args);

    प्रत्यागम p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *last, ध्रुव वर्ण *fmt, बहु_सूची args)
{
    u_char                *p, zero;
    पूर्णाङ्क                    d;
    द्विगुण                 f;
    माप_प्रकार                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             width, sign, hex, max_width, frac_width, scale, n;
    ngx_str_t             *v;
    ngx_variable_मूल्य_t  *vv;

    यावत् (*fmt && buf < last) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        यदि (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (माप_प्रकार) -1;

            यावत् (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }


            प्रति ( ;; ) {
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

                    यावत् (*fmt >= '0' && *fmt <= '9') {
                        frac_width = frac_width * 10 + (*fmt++ - '0');
                    }

                    भङ्ग;

                पक्ष '*':
                    slen = बहु_तर्क(args, माप_प्रकार);
                    fmt++;
                    अनुवर्त;

                स्वभाव:
                    भङ्ग;
                }

                भङ्ग;
            }


            विकल्प (*fmt) {

            पक्ष 'V':
                v = बहु_तर्क(args, ngx_str_t *);

                buf = ngx_sprintf_str(buf, last, v->data, v->len, hex);
                fmt++;

                अनुवर्त;

            पक्ष 'v':
                vv = बहु_तर्क(args, ngx_variable_मूल्य_t *);

                buf = ngx_sprintf_str(buf, last, vv->data, vv->len, hex);
                fmt++;

                अनुवर्त;

            पक्ष 's':
                p = बहु_तर्क(args, u_char *);

                buf = ngx_sprintf_str(buf, last, p, slen, hex);
                fmt++;

                अनुवर्त;

            पक्ष 'O':
                i64 = (int64_t) बहु_तर्क(args, off_t);
                sign = 1;
                भङ्ग;

            पक्ष 'P':
                i64 = (int64_t) बहु_तर्क(args, ngx_pid_t);
                sign = 1;
                भङ्ग;

            पक्ष 'T':
                i64 = (int64_t) बहु_तर्क(args, समय_प्रकार);
                sign = 1;
                भङ्ग;

            पक्ष 'M':
                ms = (ngx_msec_t) बहु_तर्क(args, ngx_msec_t);
                यदि ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } अन्यथा {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                भङ्ग;

            पक्ष 'z':
                यदि (sign) {
                    i64 = (int64_t) बहु_तर्क(args, ssize_t);
                } अन्यथा {
                    ui64 = (uint64_t) बहु_तर्क(args, माप_प्रकार);
                }
                भङ्ग;

            पक्ष 'i':
                यदि (sign) {
                    i64 = (int64_t) बहु_तर्क(args, ngx_int_t);
                } अन्यथा {
                    ui64 = (uint64_t) बहु_तर्क(args, ngx_uint_t);
                }

                यदि (max_width) {
                    width = NGX_INT_T_LEN;
                }

                भङ्ग;

            पक्ष 'd':
                यदि (sign) {
                    i64 = (int64_t) बहु_तर्क(args, पूर्णाङ्क);
                } अन्यथा {
                    ui64 = (uint64_t) बहु_तर्क(args, u_int);
                }
                भङ्ग;

            पक्ष 'l':
                यदि (sign) {
                    i64 = (int64_t) बहु_तर्क(args, दीर्घ);
                } अन्यथा {
                    ui64 = (uint64_t) बहु_तर्क(args, u_long);
                }
                भङ्ग;

            पक्ष 'D':
                यदि (sign) {
                    i64 = (int64_t) बहु_तर्क(args, int32_t);
                } अन्यथा {
                    ui64 = (uint64_t) बहु_तर्क(args, uint32_t);
                }
                भङ्ग;

            पक्ष 'L':
                यदि (sign) {
                    i64 = बहु_तर्क(args, int64_t);
                } अन्यथा {
                    ui64 = बहु_तर्क(args, uint64_t);
                }
                भङ्ग;

            पक्ष 'A':
                यदि (sign) {
                    i64 = (int64_t) बहु_तर्क(args, ngx_atomic_int_t);
                } अन्यथा {
                    ui64 = (uint64_t) बहु_तर्क(args, ngx_atomic_uint_t);
                }

                यदि (max_width) {
                    width = NGX_ATOMIC_T_LEN;
                }

                भङ्ग;

            पक्ष 'f':
                f = बहु_तर्क(args, द्विगुण);

                यदि (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                यदि (frac_width) {

                    scale = 1;
                    प्रति (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (द्विगुण) ui64) * scale + 0.5);

                    यदि (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_अंक(buf, last, ui64, zero, 0, width);

                यदि (frac_width) {
                    यदि (buf < last) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_अंक(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                अनुवर्त;

#यदि !(NGX_WIN32)
            पक्ष 'r':
                i64 = (int64_t) बहु_तर्क(args, rlim_t);
                sign = 1;
                भङ्ग;
#endif

            पक्ष 'p':
                ui64 = (uintptr_t) बहु_तर्क(args, शून्य *);
                hex = 2;
                sign = 0;
                zero = '0';
                width = 2 * परिमाण(शून्य *);
                भङ्ग;

            पक्ष 'c':
                d = बहु_तर्क(args, पूर्णाङ्क);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                अनुवर्त;

            पक्ष 'Z':
                *buf++ = '\0';
                fmt++;

                अनुवर्त;

            पक्ष 'N':
#यदि (NGX_WIN32)
                *buf++ = CR;
                यदि (buf < last) {
                    *buf++ = LF;
                }
#अन्यथा
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

            यदि (sign) {
                यदि (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } अन्यथा {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_अंक(buf, last, ui64, zero, hex, width);

            fmt++;

        } अन्यथा {
            *buf++ = *fmt++;
        }
    }

    प्रत्यागम buf;
}


स्थिर u_char *
ngx_sprintf_अंक(u_char *buf, u_char *last, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t width)
{
    u_char         *p, temp[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    माप_प्रकार          len;
    uint32_t        ui32;
    स्थिर u_char   hex[] = "0123456789abcdef";
    स्थिर u_char   HEX[] = "0123456789ABCDEF";

    p = temp + NGX_INT64_LEN;

    यदि (hexadecimal == 0) {

        यदि (ui64 <= (uint64_t) NGX_MAX_UINT32_मूल्य) {

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

            कुरु {
                *--p = (u_char) (ui32 % 10 + '0');
            } यावत् (ui32 /= 10);

        } अन्यथा {
            कुरु {
                *--p = (u_char) (ui64 % 10 + '0');
            } यावत् (ui64 /= 10);
        }

    } अन्यथा यदि (hexadecimal == 1) {

        कुरु {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } यावत् (ui64 >>= 4);

    } अन्यथा { /* hexadecimal == 2 */

        कुरु {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } यावत् (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    यावत् (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    यदि (buf + len > last) {
        len = last - buf;
    }

    प्रत्यागम ngx_cpymem(buf, p, len);
}


स्थिर u_char *
ngx_sprintf_str(u_char *buf, u_char *last, u_char *src, माप_प्रकार len,
    ngx_uint_t hexadecimal)
{
    स्थिर u_char   hex[] = "0123456789abcdef";
    स्थिर u_char   HEX[] = "0123456789ABCDEF";

    यदि (hexadecimal == 0) {

        यदि (len == (माप_प्रकार) -1) {
            यावत् (*src && buf < last) {
                *buf++ = *src++;
            }

        } अन्यथा {
            len = ngx_min((माप_प्रकार) (last - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } अन्यथा यदि (hexadecimal == 1) {

        यदि (len == (माप_प्रकार) -1) {

            यावत् (*src && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } अन्यथा {

            यावत् (len-- && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } अन्यथा { /* hexadecimal == 2 */

        यदि (len == (माप_प्रकार) -1) {

            यावत् (*src && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } अन्यथा {

            यावत् (len-- && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    प्रत्यागम buf;
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

    प्रति ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        यदि (c1 == c2) {

            यदि (c1) {
                अनुवर्त;
            }

            प्रत्यागम 0;
        }

        प्रत्यागम c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, माप_प्रकार n)
{
    ngx_uint_t  c1, c2;

    यावत् (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        यदि (c1 == c2) {

            यदि (c1) {
                n--;
                अनुवर्त;
            }

            प्रत्यागम 0;
        }

        प्रत्यागम c1 - c2;
    }

    प्रत्यागम 0;
}


u_char *
ngx_strnstr(u_char *s1, वर्ण *s2, माप_प्रकार len)
{
    u_char  c1, c2;
    माप_प्रकार  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    कुरु {
        कुरु {
            यदि (len-- == 0) {
                प्रत्यागम रिक्त;
            }

            c1 = *s1++;

            यदि (c1 == 0) {
                प्रत्यागम रिक्त;
            }

        } यावत् (c1 != c2);

        यदि (n > len) {
            प्रत्यागम रिक्त;
        }

    } यावत् (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    प्रत्यागम --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, वर्ण *s2, माप_प्रकार n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    कुरु {
        कुरु {
            c1 = *s1++;

            यदि (c1 == 0) {
                प्रत्यागम रिक्त;
            }

        } यावत् (c1 != c2);

    } यावत् (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    प्रत्यागम --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, वर्ण *s2, माप_प्रकार n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    कुरु {
        कुरु {
            c1 = (ngx_uint_t) *s1++;

            यदि (c1 == 0) {
                प्रत्यागम रिक्त;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } यावत् (c1 != c2);

    } यावत् (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    प्रत्यागम --s1;
}


/*
 * ngx_strlcasestrn() is intended to search for static substring
 * with known length in string until the argument last. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strlcasestrn(u_char *s1, u_char *last, u_char *s2, माप_प्रकार n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;
    last -= n;

    कुरु {
        कुरु {
            यदि (s1 >= last) {
                प्रत्यागम रिक्त;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } यावत् (c1 != c2);

    } यावत् (ngx_strncasecmp(s1, s2, n) != 0);

    प्रत्यागम --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, माप_प्रकार n)
{
    यदि (n == 0) {
        प्रत्यागम 0;
    }

    n--;

    प्रति ( ;; ) {
        यदि (s1[n] != s2[n]) {
            प्रत्यागम s1[n] - s2[n];
        }

        यदि (n == 0) {
            प्रत्यागम 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, माप_प्रकार n)
{
    u_char  c1, c2;

    यदि (n == 0) {
        प्रत्यागम 0;
    }

    n--;

    प्रति ( ;; ) {
        c1 = s1[n];
        यदि (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        यदि (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        यदि (c1 != c2) {
            प्रत्यागम c1 - c2;
        }

        यदि (n == 0) {
            प्रत्यागम 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, माप_प्रकार n1, माप_प्रकार n2)
{
    माप_प्रकार     n;
    ngx_int_t  m, z;

    यदि (n1 <= n2) {
        n = n1;
        z = -1;

    } अन्यथा {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    यदि (m || n1 == n2) {
        प्रत्यागम m;
    }

    प्रत्यागम z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    प्रति ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        यदि (c1 == c2) {

            यदि (c1) {
                अनुवर्त;
            }

            प्रत्यागम 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        प्रत्यागम c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, माप_प्रकार n)
{
    ngx_uint_t  c1, c2;

    यावत् (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#यदि (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = छोटे(c1);
        c2 = छोटे(c2);
#endif

        यदि (c1 == c2) {

            यदि (c1) {
                n--;
                अनुवर्त;
            }

            प्रत्यागम 0;
        }

        /* we need '/' to be the lowest character */

        यदि (c1 == 0 || c2 == 0) {
            प्रत्यागम c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        प्रत्यागम c1 - c2;
    }

    प्रत्यागम 0;
}


ngx_int_t
ngx_atoi(u_char *पंक्ति, माप_प्रकार n)
{
    ngx_int_t  मूल्य, cutoff, cutlim;

    यदि (n == 0) {
        प्रत्यागम NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_मूल्य / 10;
    cutlim = NGX_MAX_INT_T_मूल्य % 10;

    प्रति (मूल्य = 0; n--; पंक्ति++) {
        यदि (*पंक्ति < '0' || *पंक्ति > '9') {
            प्रत्यागम NGX_ERROR;
        }

        यदि (मूल्य >= cutoff && (मूल्य > cutoff || *पंक्ति - '0' > cutlim)) {
            प्रत्यागम NGX_ERROR;
        }

        मूल्य = मूल्य * 10 + (*पंक्ति - '0');
    }

    प्रत्यागम मूल्य;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *पंक्ति, माप_प्रकार n, माप_प्रकार point)
{
    ngx_int_t   मूल्य, cutoff, cutlim;
    ngx_uint_t  dot;

    यदि (n == 0) {
        प्रत्यागम NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_मूल्य / 10;
    cutlim = NGX_MAX_INT_T_मूल्य % 10;

    dot = 0;

    प्रति (मूल्य = 0; n--; पंक्ति++) {

        यदि (point == 0) {
            प्रत्यागम NGX_ERROR;
        }

        यदि (*पंक्ति == '.') {
            यदि (dot) {
                प्रत्यागम NGX_ERROR;
            }

            dot = 1;
            अनुवर्त;
        }

        यदि (*पंक्ति < '0' || *पंक्ति > '9') {
            प्रत्यागम NGX_ERROR;
        }

        यदि (मूल्य >= cutoff && (मूल्य > cutoff || *पंक्ति - '0' > cutlim)) {
            प्रत्यागम NGX_ERROR;
        }

        मूल्य = मूल्य * 10 + (*पंक्ति - '0');
        point -= dot;
    }

    यावत् (point--) {
        यदि (मूल्य > cutoff) {
            प्रत्यागम NGX_ERROR;
        }

        मूल्य = मूल्य * 10;
    }

    प्रत्यागम मूल्य;
}


ssize_t
ngx_atosz(u_char *पंक्ति, माप_प्रकार n)
{
    ssize_t  मूल्य, cutoff, cutlim;

    यदि (n == 0) {
        प्रत्यागम NGX_ERROR;
    }

    cutoff = NGX_MAX_SIZE_T_मूल्य / 10;
    cutlim = NGX_MAX_SIZE_T_मूल्य % 10;

    प्रति (मूल्य = 0; n--; पंक्ति++) {
        यदि (*पंक्ति < '0' || *पंक्ति > '9') {
            प्रत्यागम NGX_ERROR;
        }

        यदि (मूल्य >= cutoff && (मूल्य > cutoff || *पंक्ति - '0' > cutlim)) {
            प्रत्यागम NGX_ERROR;
        }

        मूल्य = मूल्य * 10 + (*पंक्ति - '0');
    }

    प्रत्यागम मूल्य;
}


off_t
ngx_atoof(u_char *पंक्ति, माप_प्रकार n)
{
    off_t  मूल्य, cutoff, cutlim;

    यदि (n == 0) {
        प्रत्यागम NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_मूल्य / 10;
    cutlim = NGX_MAX_OFF_T_मूल्य % 10;

    प्रति (मूल्य = 0; n--; पंक्ति++) {
        यदि (*पंक्ति < '0' || *पंक्ति > '9') {
            प्रत्यागम NGX_ERROR;
        }

        यदि (मूल्य >= cutoff && (मूल्य > cutoff || *पंक्ति - '0' > cutlim)) {
            प्रत्यागम NGX_ERROR;
        }

        मूल्य = मूल्य * 10 + (*पंक्ति - '0');
    }

    प्रत्यागम मूल्य;
}


समय_प्रकार
ngx_atotm(u_char *पंक्ति, माप_प्रकार n)
{
    समय_प्रकार  मूल्य, cutoff, cutlim;

    यदि (n == 0) {
        प्रत्यागम NGX_ERROR;
    }

    cutoff = NGX_MAX_TIME_T_मूल्य / 10;
    cutlim = NGX_MAX_TIME_T_मूल्य % 10;

    प्रति (मूल्य = 0; n--; पंक्ति++) {
        यदि (*पंक्ति < '0' || *पंक्ति > '9') {
            प्रत्यागम NGX_ERROR;
        }

        यदि (मूल्य >= cutoff && (मूल्य > cutoff || *पंक्ति - '0' > cutlim)) {
            प्रत्यागम NGX_ERROR;
        }

        मूल्य = मूल्य * 10 + (*पंक्ति - '0');
    }

    प्रत्यागम मूल्य;
}


ngx_int_t
ngx_hextoi(u_char *पंक्ति, माप_प्रकार n)
{
    u_char     c, ch;
    ngx_int_t  मूल्य, cutoff;

    यदि (n == 0) {
        प्रत्यागम NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_मूल्य / 16;

    प्रति (मूल्य = 0; n--; पंक्ति++) {
        यदि (मूल्य > cutoff) {
            प्रत्यागम NGX_ERROR;
        }

        ch = *पंक्ति;

        यदि (ch >= '0' && ch <= '9') {
            मूल्य = मूल्य * 16 + (ch - '0');
            अनुवर्त;
        }

        c = (u_char) (ch | 0x20);

        यदि (c >= 'a' && c <= 'f') {
            मूल्य = मूल्य * 16 + (c - 'a' + 10);
            अनुवर्त;
        }

        प्रत्यागम NGX_ERROR;
    }

    प्रत्यागम मूल्य;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, माप_प्रकार len)
{
    स्थिर u_char  hex[] = "0123456789abcdef";

    यावत् (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    प्रत्यागम dst;
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
    माप_प्रकार          len;

    len = src->len;
    s = src->data;
    d = dst->data;

    यावत् (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    यदि (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        यदि (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            यदि (padding) {
                *d++ = '=';
            }

        } अन्यथा {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        यदि (padding) {
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

    प्रत्यागम ngx_decode_base64_internal(dst, src, basis64);
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

    प्रत्यागम ngx_decode_base64_internal(dst, src, basis64);
}


स्थिर ngx_int_t
ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src, ध्रुव u_char *basis)
{
    माप_प्रकार          len;
    u_char         *d, *s;

    प्रति (len = 0; len < src->len; len++) {
        यदि (src->data[len] == '=') {
            भङ्ग;
        }

        यदि (basis[src->data[len]] == 77) {
            प्रत्यागम NGX_ERROR;
        }
    }

    यदि (len % 4 == 1) {
        प्रत्यागम NGX_ERROR;
    }

    s = src->data;
    d = dst->data;

    यावत् (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    यदि (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    यदि (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->data;

    प्रत्यागम NGX_OK;
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
ngx_utf8_decode(u_char **p, माप_प्रकार n)
{
    माप_प्रकार    len;
    uint32_t  u, i, valid;

    u = **p;

    यदि (u >= 0xf8) {

        (*p)++;
        प्रत्यागम 0xffffffff;

    } अन्यथा यदि (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } अन्यथा यदि (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } अन्यथा यदि (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } अन्यथा {
        (*p)++;
        प्रत्यागम 0xffffffff;
    }

    यदि (n - 1 < len) {
        प्रत्यागम 0xfffffffe;
    }

    (*p)++;

    यावत् (len) {
        i = *(*p)++;

        यदि (i < 0x80) {
            प्रत्यागम 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    यदि (u > valid) {
        प्रत्यागम u;
    }

    प्रत्यागम 0xffffffff;
}


माप_प्रकार
ngx_utf8_length(u_char *p, माप_प्रकार n)
{
    u_char  c, *last;
    माप_प्रकार  len;

    last = p + n;

    प्रति (len = 0; p < last; len++) {

        c = *p;

        यदि (c < 0x80) {
            p++;
            अनुवर्त;
        }

        यदि (ngx_utf8_decode(&p, last - p) > 0x10ffff) {
            /* invalid UTF-8 */
            प्रत्यागम n;
        }
    }

    प्रत्यागम len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, माप_प्रकार n, माप_प्रकार len)
{
    u_char  c, *next;

    यदि (n == 0) {
        प्रत्यागम dst;
    }

    यावत् (--n) {

        c = *src;
        *dst = c;

        यदि (c < 0x80) {

            यदि (c != '\0') {
                dst++;
                src++;
                len--;

                अनुवर्त;
            }

            प्रत्यागम dst;
        }

        next = src;

        यदि (ngx_utf8_decode(&next, len) > 0x10ffff) {
            /* invalid UTF-8 */
            भङ्ग;
        }

        यावत् (src < next) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    प्रत्यागम dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, माप_प्रकार size, ngx_uint_t type)
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

    यदि (dst == रिक्त) {

        /* find the number of the characters to be escaped */

        n = 0;

        यावत् (size) {
            यदि (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            size--;
        }

        प्रत्यागम (uintptr_t) n;
    }

    यावत् (size) {
        यदि (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = prefix;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } अन्यथा {
            *dst++ = *src++;
        }
        size--;
    }

    प्रत्यागम (uintptr_t) dst;
}


शून्य
ngx_unescape_uri(u_char **dst, u_char **src, माप_प्रकार size, ngx_uint_t type)
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

    यावत् (size--) {

        ch = *s++;

        विकल्प (state) {
        पक्ष sw_usual:
            यदि (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                गच्छ done;
            }

            यदि (ch == '%') {
                state = sw_quoted;
                भङ्ग;
            }

            *d++ = ch;
            भङ्ग;

        पक्ष sw_quoted:

            यदि (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_second;
                भङ्ग;
            }

            c = (u_char) (ch | 0x20);
            यदि (c >= 'a' && c <= 'f') {
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

            यदि (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                यदि (type & NGX_UNESCAPE_REDIRECT) {
                    यदि (ch > '%' && ch < 0x7f) {
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
            यदि (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                यदि (type & NGX_UNESCAPE_URI) {
                    यदि (ch == '?') {
                        *d++ = ch;
                        गच्छ done;
                    }

                    *d++ = ch;
                    भङ्ग;
                }

                यदि (type & NGX_UNESCAPE_REDIRECT) {
                    यदि (ch == '?') {
                        *d++ = ch;
                        गच्छ done;
                    }

                    यदि (ch > '%' && ch < 0x7f) {
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
ngx_escape_html(u_char *dst, u_char *src, माप_प्रकार size)
{
    u_char      ch;
    ngx_uint_t  len;

    यदि (dst == रिक्त) {

        len = 0;

        यावत् (size) {
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

        प्रत्यागम (uintptr_t) len;
    }

    यावत् (size) {
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

    प्रत्यागम (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, माप_प्रकार size)
{
    u_char      ch;
    ngx_uint_t  len;

    यदि (dst == रिक्त) {
        len = 0;

        यावत् (size) {
            ch = *src++;

            यदि (ch == '\\' || ch == '"') {
                len++;

            } अन्यथा यदि (ch <= 0x1f) {

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

        प्रत्यागम (uintptr_t) len;
    }

    यावत् (size) {
        ch = *src++;

        यदि (ch > 0x1f) {

            यदि (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } अन्यथा {
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

    प्रत्यागम (uintptr_t) dst;
}


शून्य
ngx_str_rbtree_जोड़ो_मूल्य(ngx_rbtree_गाँठ_t *temp,
    ngx_rbtree_गाँठ_t *गाँठ, ngx_rbtree_गाँठ_t *sentinel)
{
    ngx_str_गाँठ_t      *n, *t;
    ngx_rbtree_गाँठ_t  **p;

    प्रति ( ;; ) {

        n = (ngx_str_गाँठ_t *) गाँठ;
        t = (ngx_str_गाँठ_t *) temp;

        यदि (गाँठ->कुंजी != temp->कुंजी) {

            p = (गाँठ->कुंजी < temp->कुंजी) ? &temp->बायाँ : &temp->दायाँ;

        } अन्यथा यदि (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &temp->बायाँ : &temp->दायाँ;

        } अन्यथा {
            p = (ngx_memcmp(n->str.data, t->str.data, n->str.len) < 0)
                 ? &temp->बायाँ : &temp->दायाँ;
        }

        यदि (*p == sentinel) {
            भङ्ग;
        }

        temp = *p;
    }

    *p = गाँठ;
    गाँठ->parent = temp;
    गाँठ->बायाँ = sentinel;
    गाँठ->दायाँ = sentinel;
    ngx_rbt_red(गाँठ);
}


ngx_str_गाँठ_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_गाँठ_t     *n;
    ngx_rbtree_गाँठ_t  *गाँठ, *sentinel;

    गाँठ = rbtree->मूल;
    sentinel = rbtree->sentinel;

    यावत् (गाँठ != sentinel) {

        n = (ngx_str_गाँठ_t *) गाँठ;

        यदि (hash != गाँठ->कुंजी) {
            गाँठ = (hash < गाँठ->कुंजी) ? गाँठ->बायाँ : गाँठ->दायाँ;
            अनुवर्त;
        }

        यदि (val->len != n->str.len) {
            गाँठ = (val->len < n->str.len) ? गाँठ->बायाँ : गाँठ->दायाँ;
            अनुवर्त;
        }

        rc = ngx_memcmp(val->data, n->str.data, val->len);

        यदि (rc < 0) {
            गाँठ = गाँठ->बायाँ;
            अनुवर्त;
        }

        यदि (rc > 0) {
            गाँठ = गाँठ->दायाँ;
            अनुवर्त;
        }

        प्रत्यागम n;
    }

    प्रत्यागम रिक्त;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

शून्य
ngx_sort(शून्य *base, माप_प्रकार n, माप_प्रकार size,
    ngx_int_t (*cmp)(ध्रुव शून्य *, ध्रुव शून्य *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(size, ngx_cycle->log);
    यदि (p == रिक्त) {
        प्रत्यागम;
    }

    प्रति (p1 = (u_char *) base + size;
         p1 < (u_char *) base + n * size;
         p1 += size)
    {
        ngx_memcpy(p, p1, size);

        प्रति (p2 = p1;
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
ngx_explicit_memzero(शून्य *buf, माप_प्रकार n)
{
    ngx_memzero(buf, n);
    ngx_memory_barrier();
}


#यदि (NGX_MEMCPY_LIMIT)

शून्य *
ngx_memcpy(शून्य *dst, ध्रुव शून्य *src, माप_प्रकार n)
{
    यदि (n > NGX_MEMCPY_LIMIT) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "memcpy %uz bytes", n);
        ngx_debug_point();
    }

    प्रत्यागम स_नकल(dst, src, n);
}

#endif
