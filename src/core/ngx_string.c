
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर u_char *ngx_sprintf_अङ्क(u_char *buf, u_char *last, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
स्थिर u_char *ngx_sprintf_str(u_char *buf, u_char *last, u_char *src,
    माप_प्रकार len, ngx_uint_t hexadecimal);
स्थिर शून्य ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    अचर u_char *basis, ngx_uint_t padding);
स्थिर ngx_int_t ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    अचर u_char *basis);


शून्य
ngx_strlow(u_char *dst, u_char *src, माप_प्रकार n)
{
    जबसम्म (n) {
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

    लागि (i = 0; i < n; i++) {

        यदि (p[i] == '\0') {
            फर्काऊ i;
        }
    }

    फर्काऊ n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, माप_प्रकार n)
{
    यदि (n == 0) {
        फर्काऊ dst;
    }

    जबसम्म (--n) {
        *dst = *src;

        यदि (*dst == '\0') {
            फर्काऊ dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    फर्काऊ dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    यदि (dst == रिक्त) {
        फर्काऊ रिक्त;
    }

    ngx_memcpy(dst, src->data, src->len);

    फर्काऊ dst;
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
ngx_sprintf(u_char *buf, अचर वर्ण *fmt, ...)
{
    u_char   *p;
    बहु_सूची   args;

    बहु_शुरू(args, fmt);
    p = ngx_vslprintf(buf, (शून्य *) -1, fmt, args);
    बहु_पूर्ण(args);

    फर्काऊ p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, माप_प्रकार max, अचर वर्ण *fmt, ...)
{
    u_char   *p;
    बहु_सूची   args;

    बहु_शुरू(args, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, args);
    बहु_पूर्ण(args);

    फर्काऊ p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *last, अचर वर्ण *fmt, ...)
{
    u_char   *p;
    बहु_सूची   args;

    बहु_शुरू(args, fmt);
    p = ngx_vslprintf(buf, last, fmt, args);
    बहु_पूर्ण(args);

    फर्काऊ p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *last, अचर वर्ण *fmt, बहु_सूची args)
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
    ngx_variable_मान_t  *vv;

    जबसम्म (*fmt && buf < last) {

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

            जबसम्म (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }


            लागि ( ;; ) {
                विकल्प (*fmt) {

                पक्ष 'u':
                    sign = 0;
                    fmt++;
                    जारी;

                पक्ष 'm':
                    max_width = 1;
                    fmt++;
                    जारी;

                पक्ष 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    जारी;

                पक्ष 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    जारी;

                पक्ष '.':
                    fmt++;

                    जबसम्म (*fmt >= '0' && *fmt <= '9') {
                        frac_width = frac_width * 10 + (*fmt++ - '0');
                    }

                    रोक;

                पक्ष '*':
                    slen = बहु_तर्क(args, माप_प्रकार);
                    fmt++;
                    जारी;

                स्वभाव:
                    रोक;
                }

                रोक;
            }


            विकल्प (*fmt) {

            पक्ष 'V':
                v = बहु_तर्क(args, ngx_str_t *);

                buf = ngx_sprintf_str(buf, last, v->data, v->len, hex);
                fmt++;

                जारी;

            पक्ष 'v':
                vv = बहु_तर्क(args, ngx_variable_मान_t *);

                buf = ngx_sprintf_str(buf, last, vv->data, vv->len, hex);
                fmt++;

                जारी;

            पक्ष 's':
                p = बहु_तर्क(args, u_char *);

                buf = ngx_sprintf_str(buf, last, p, slen, hex);
                fmt++;

                जारी;

            पक्ष 'O':
                i64 = (int64_t) बहु_तर्क(args, off_t);
                sign = 1;
                रोक;

            पक्ष 'P':
                i64 = (int64_t) बहु_तर्क(args, ngx_pid_t);
                sign = 1;
                रोक;

            पक्ष 'T':
                i64 = (int64_t) बहु_तर्क(args, समय_प्रकार);
                sign = 1;
                रोक;

            पक्ष 'M':
                ms = (ngx_msec_t) बहु_तर्क(args, ngx_msec_t);
                यदि ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } अन्यथा {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                रोक;

            पक्ष 'z':
                यदि (sign) {
                    i64 = (int64_t) बहु_तर्क(args, ssize_t);
                } अन्यथा {
                    ui64 = (uint64_t) बहु_तर्क(args, माप_प्रकार);
                }
                रोक;

            पक्ष 'i':
                यदि (sign) {
                    i64 = (int64_t) बहु_तर्क(args, ngx_int_t);
                } अन्यथा {
                    ui64 = (uint64_t) बहु_तर्क(args, ngx_uint_t);
                }

                यदि (max_width) {
                    width = NGX_INT_T_LEN;
                }

                रोक;

            पक्ष 'd':
                यदि (sign) {
                    i64 = (int64_t) बहु_तर्क(args, पूर्णाङ्क);
                } अन्यथा {
                    ui64 = (uint64_t) बहु_तर्क(args, u_int);
                }
                रोक;

            पक्ष 'l':
                यदि (sign) {
                    i64 = (int64_t) बहु_तर्क(args, दीर्घ);
                } अन्यथा {
                    ui64 = (uint64_t) बहु_तर्क(args, u_long);
                }
                रोक;

            पक्ष 'D':
                यदि (sign) {
                    i64 = (int64_t) बहु_तर्क(args, int32_t);
                } अन्यथा {
                    ui64 = (uint64_t) बहु_तर्क(args, uint32_t);
                }
                रोक;

            पक्ष 'L':
                यदि (sign) {
                    i64 = बहु_तर्क(args, int64_t);
                } अन्यथा {
                    ui64 = बहु_तर्क(args, uint64_t);
                }
                रोक;

            पक्ष 'A':
                यदि (sign) {
                    i64 = (int64_t) बहु_तर्क(args, ngx_atomic_int_t);
                } अन्यथा {
                    ui64 = (uint64_t) बहु_तर्क(args, ngx_atomic_uint_t);
                }

                यदि (max_width) {
                    width = NGX_ATOMIC_T_LEN;
                }

                रोक;

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
                    लागि (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (द्विगुण) ui64) * scale + 0.5);

                    यदि (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_अङ्क(buf, last, ui64, zero, 0, width);

                यदि (frac_width) {
                    यदि (buf < last) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_अङ्क(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                जारी;

#यदि !(NGX_WIN32)
            पक्ष 'r':
                i64 = (int64_t) बहु_तर्क(args, rlim_t);
                sign = 1;
                रोक;
#endif

            पक्ष 'p':
                ui64 = (uintptr_t) बहु_तर्क(args, शून्य *);
                hex = 2;
                sign = 0;
                zero = '0';
                width = 2 * परिमाण(शून्य *);
                रोक;

            पक्ष 'c':
                d = बहु_तर्क(args, पूर्णाङ्क);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                जारी;

            पक्ष 'Z':
                *buf++ = '\0';
                fmt++;

                जारी;

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

                जारी;

            पक्ष '%':
                *buf++ = '%';
                fmt++;

                जारी;

            स्वभाव:
                *buf++ = *fmt++;

                जारी;
            }

            यदि (sign) {
                यदि (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } अन्यथा {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_अङ्क(buf, last, ui64, zero, hex, width);

            fmt++;

        } अन्यथा {
            *buf++ = *fmt++;
        }
    }

    फर्काऊ buf;
}


स्थिर u_char *
ngx_sprintf_अङ्क(u_char *buf, u_char *last, uint64_t ui64, u_char zero,
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

        यदि (ui64 <= (uint64_t) NGX_MAX_UINT32_मान) {

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

            गर {
                *--p = (u_char) (ui32 % 10 + '0');
            } जबसम्म (ui32 /= 10);

        } अन्यथा {
            गर {
                *--p = (u_char) (ui64 % 10 + '0');
            } जबसम्म (ui64 /= 10);
        }

    } अन्यथा यदि (hexadecimal == 1) {

        गर {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } जबसम्म (ui64 >>= 4);

    } अन्यथा { /* hexadecimal == 2 */

        गर {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } जबसम्म (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    जबसम्म (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    यदि (buf + len > last) {
        len = last - buf;
    }

    फर्काऊ ngx_cpymem(buf, p, len);
}


स्थिर u_char *
ngx_sprintf_str(u_char *buf, u_char *last, u_char *src, माप_प्रकार len,
    ngx_uint_t hexadecimal)
{
    स्थिर u_char   hex[] = "0123456789abcdef";
    स्थिर u_char   HEX[] = "0123456789ABCDEF";

    यदि (hexadecimal == 0) {

        यदि (len == (माप_प्रकार) -1) {
            जबसम्म (*src && buf < last) {
                *buf++ = *src++;
            }

        } अन्यथा {
            len = ngx_min((माप_प्रकार) (last - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } अन्यथा यदि (hexadecimal == 1) {

        यदि (len == (माप_प्रकार) -1) {

            जबसम्म (*src && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } अन्यथा {

            जबसम्म (len-- && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } अन्यथा { /* hexadecimal == 2 */

        यदि (len == (माप_प्रकार) -1) {

            जबसम्म (*src && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } अन्यथा {

            जबसम्म (len-- && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    फर्काऊ buf;
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

    लागि ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        यदि (c1 == c2) {

            यदि (c1) {
                जारी;
            }

            फर्काऊ 0;
        }

        फर्काऊ c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, माप_प्रकार n)
{
    ngx_uint_t  c1, c2;

    जबसम्म (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        यदि (c1 == c2) {

            यदि (c1) {
                n--;
                जारी;
            }

            फर्काऊ 0;
        }

        फर्काऊ c1 - c2;
    }

    फर्काऊ 0;
}


u_char *
ngx_strnstr(u_char *s1, वर्ण *s2, माप_प्रकार len)
{
    u_char  c1, c2;
    माप_प्रकार  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    गर {
        गर {
            यदि (len-- == 0) {
                फर्काऊ रिक्त;
            }

            c1 = *s1++;

            यदि (c1 == 0) {
                फर्काऊ रिक्त;
            }

        } जबसम्म (c1 != c2);

        यदि (n > len) {
            फर्काऊ रिक्त;
        }

    } जबसम्म (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    फर्काऊ --s1;
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

    गर {
        गर {
            c1 = *s1++;

            यदि (c1 == 0) {
                फर्काऊ रिक्त;
            }

        } जबसम्म (c1 != c2);

    } जबसम्म (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    फर्काऊ --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, वर्ण *s2, माप_प्रकार n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    गर {
        गर {
            c1 = (ngx_uint_t) *s1++;

            यदि (c1 == 0) {
                फर्काऊ रिक्त;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } जबसम्म (c1 != c2);

    } जबसम्म (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    फर्काऊ --s1;
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

    गर {
        गर {
            यदि (s1 >= last) {
                फर्काऊ रिक्त;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } जबसम्म (c1 != c2);

    } जबसम्म (ngx_strncasecmp(s1, s2, n) != 0);

    फर्काऊ --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, माप_प्रकार n)
{
    यदि (n == 0) {
        फर्काऊ 0;
    }

    n--;

    लागि ( ;; ) {
        यदि (s1[n] != s2[n]) {
            फर्काऊ s1[n] - s2[n];
        }

        यदि (n == 0) {
            फर्काऊ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, माप_प्रकार n)
{
    u_char  c1, c2;

    यदि (n == 0) {
        फर्काऊ 0;
    }

    n--;

    लागि ( ;; ) {
        c1 = s1[n];
        यदि (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        यदि (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        यदि (c1 != c2) {
            फर्काऊ c1 - c2;
        }

        यदि (n == 0) {
            फर्काऊ 0;
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
        फर्काऊ m;
    }

    फर्काऊ z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    लागि ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        यदि (c1 == c2) {

            यदि (c1) {
                जारी;
            }

            फर्काऊ 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        फर्काऊ c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, माप_प्रकार n)
{
    ngx_uint_t  c1, c2;

    जबसम्म (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#यदि (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = छोटे(c1);
        c2 = छोटे(c2);
#endif

        यदि (c1 == c2) {

            यदि (c1) {
                n--;
                जारी;
            }

            फर्काऊ 0;
        }

        /* we need '/' to be the lowest character */

        यदि (c1 == 0 || c2 == 0) {
            फर्काऊ c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        फर्काऊ c1 - c2;
    }

    फर्काऊ 0;
}


ngx_int_t
ngx_atoi(u_char *पङ्क्ति, माप_प्रकार n)
{
    ngx_int_t  मान, cutoff, cutlim;

    यदि (n == 0) {
        फर्काऊ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_मान / 10;
    cutlim = NGX_MAX_INT_T_मान % 10;

    लागि (मान = 0; n--; पङ्क्ति++) {
        यदि (*पङ्क्ति < '0' || *पङ्क्ति > '9') {
            फर्काऊ NGX_ERROR;
        }

        यदि (मान >= cutoff && (मान > cutoff || *पङ्क्ति - '0' > cutlim)) {
            फर्काऊ NGX_ERROR;
        }

        मान = मान * 10 + (*पङ्क्ति - '0');
    }

    फर्काऊ मान;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *पङ्क्ति, माप_प्रकार n, माप_प्रकार point)
{
    ngx_int_t   मान, cutoff, cutlim;
    ngx_uint_t  dot;

    यदि (n == 0) {
        फर्काऊ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_मान / 10;
    cutlim = NGX_MAX_INT_T_मान % 10;

    dot = 0;

    लागि (मान = 0; n--; पङ्क्ति++) {

        यदि (point == 0) {
            फर्काऊ NGX_ERROR;
        }

        यदि (*पङ्क्ति == '.') {
            यदि (dot) {
                फर्काऊ NGX_ERROR;
            }

            dot = 1;
            जारी;
        }

        यदि (*पङ्क्ति < '0' || *पङ्क्ति > '9') {
            फर्काऊ NGX_ERROR;
        }

        यदि (मान >= cutoff && (मान > cutoff || *पङ्क्ति - '0' > cutlim)) {
            फर्काऊ NGX_ERROR;
        }

        मान = मान * 10 + (*पङ्क्ति - '0');
        point -= dot;
    }

    जबसम्म (point--) {
        यदि (मान > cutoff) {
            फर्काऊ NGX_ERROR;
        }

        मान = मान * 10;
    }

    फर्काऊ मान;
}


ssize_t
ngx_atosz(u_char *पङ्क्ति, माप_प्रकार n)
{
    ssize_t  मान, cutoff, cutlim;

    यदि (n == 0) {
        फर्काऊ NGX_ERROR;
    }

    cutoff = NGX_MAX_SIZE_T_मान / 10;
    cutlim = NGX_MAX_SIZE_T_मान % 10;

    लागि (मान = 0; n--; पङ्क्ति++) {
        यदि (*पङ्क्ति < '0' || *पङ्क्ति > '9') {
            फर्काऊ NGX_ERROR;
        }

        यदि (मान >= cutoff && (मान > cutoff || *पङ्क्ति - '0' > cutlim)) {
            फर्काऊ NGX_ERROR;
        }

        मान = मान * 10 + (*पङ्क्ति - '0');
    }

    फर्काऊ मान;
}


off_t
ngx_atoof(u_char *पङ्क्ति, माप_प्रकार n)
{
    off_t  मान, cutoff, cutlim;

    यदि (n == 0) {
        फर्काऊ NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_मान / 10;
    cutlim = NGX_MAX_OFF_T_मान % 10;

    लागि (मान = 0; n--; पङ्क्ति++) {
        यदि (*पङ्क्ति < '0' || *पङ्क्ति > '9') {
            फर्काऊ NGX_ERROR;
        }

        यदि (मान >= cutoff && (मान > cutoff || *पङ्क्ति - '0' > cutlim)) {
            फर्काऊ NGX_ERROR;
        }

        मान = मान * 10 + (*पङ्क्ति - '0');
    }

    फर्काऊ मान;
}


समय_प्रकार
ngx_atotm(u_char *पङ्क्ति, माप_प्रकार n)
{
    समय_प्रकार  मान, cutoff, cutlim;

    यदि (n == 0) {
        फर्काऊ NGX_ERROR;
    }

    cutoff = NGX_MAX_TIME_T_मान / 10;
    cutlim = NGX_MAX_TIME_T_मान % 10;

    लागि (मान = 0; n--; पङ्क्ति++) {
        यदि (*पङ्क्ति < '0' || *पङ्क्ति > '9') {
            फर्काऊ NGX_ERROR;
        }

        यदि (मान >= cutoff && (मान > cutoff || *पङ्क्ति - '0' > cutlim)) {
            फर्काऊ NGX_ERROR;
        }

        मान = मान * 10 + (*पङ्क्ति - '0');
    }

    फर्काऊ मान;
}


ngx_int_t
ngx_hextoi(u_char *पङ्क्ति, माप_प्रकार n)
{
    u_char     c, ch;
    ngx_int_t  मान, cutoff;

    यदि (n == 0) {
        फर्काऊ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_मान / 16;

    लागि (मान = 0; n--; पङ्क्ति++) {
        यदि (मान > cutoff) {
            फर्काऊ NGX_ERROR;
        }

        ch = *पङ्क्ति;

        यदि (ch >= '0' && ch <= '9') {
            मान = मान * 16 + (ch - '0');
            जारी;
        }

        c = (u_char) (ch | 0x20);

        यदि (c >= 'a' && c <= 'f') {
            मान = मान * 16 + (c - 'a' + 10);
            जारी;
        }

        फर्काऊ NGX_ERROR;
    }

    फर्काऊ मान;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, माप_प्रकार len)
{
    स्थिर u_char  hex[] = "0123456789abcdef";

    जबसम्म (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    फर्काऊ dst;
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
ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src, अचर u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    माप_प्रकार          len;

    len = src->len;
    s = src->data;
    d = dst->data;

    जबसम्म (len > 2) {
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

    फर्काऊ ngx_decode_base64_internal(dst, src, basis64);
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

    फर्काऊ ngx_decode_base64_internal(dst, src, basis64);
}


स्थिर ngx_int_t
ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src, अचर u_char *basis)
{
    माप_प्रकार          len;
    u_char         *d, *s;

    लागि (len = 0; len < src->len; len++) {
        यदि (src->data[len] == '=') {
            रोक;
        }

        यदि (basis[src->data[len]] == 77) {
            फर्काऊ NGX_ERROR;
        }
    }

    यदि (len % 4 == 1) {
        फर्काऊ NGX_ERROR;
    }

    s = src->data;
    d = dst->data;

    जबसम्म (len > 3) {
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

    फर्काऊ NGX_OK;
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
        फर्काऊ 0xffffffff;

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
        फर्काऊ 0xffffffff;
    }

    यदि (n - 1 < len) {
        फर्काऊ 0xfffffffe;
    }

    (*p)++;

    जबसम्म (len) {
        i = *(*p)++;

        यदि (i < 0x80) {
            फर्काऊ 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    यदि (u > valid) {
        फर्काऊ u;
    }

    फर्काऊ 0xffffffff;
}


माप_प्रकार
ngx_utf8_length(u_char *p, माप_प्रकार n)
{
    u_char  c, *last;
    माप_प्रकार  len;

    last = p + n;

    लागि (len = 0; p < last; len++) {

        c = *p;

        यदि (c < 0x80) {
            p++;
            जारी;
        }

        यदि (ngx_utf8_decode(&p, last - p) > 0x10ffff) {
            /* invalid UTF-8 */
            फर्काऊ n;
        }
    }

    फर्काऊ len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, माप_प्रकार n, माप_प्रकार len)
{
    u_char  c, *next;

    यदि (n == 0) {
        फर्काऊ dst;
    }

    जबसम्म (--n) {

        c = *src;
        *dst = c;

        यदि (c < 0x80) {

            यदि (c != '\0') {
                dst++;
                src++;
                len--;

                जारी;
            }

            फर्काऊ dst;
        }

        next = src;

        यदि (ngx_utf8_decode(&next, len) > 0x10ffff) {
            /* invalid UTF-8 */
            रोक;
        }

        जबसम्म (src < next) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    फर्काऊ dst;
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

        जबसम्म (size) {
            यदि (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            size--;
        }

        फर्काऊ (uintptr_t) n;
    }

    जबसम्म (size) {
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

    फर्काऊ (uintptr_t) dst;
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

    जबसम्म (size--) {

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
                रोक;
            }

            *d++ = ch;
            रोक;

        पक्ष sw_quoted:

            यदि (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_second;
                रोक;
            }

            c = (u_char) (ch | 0x20);
            यदि (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_second;
                रोक;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            रोक;

        पक्ष sw_quoted_second:

            state = sw_usual;

            यदि (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                यदि (type & NGX_UNESCAPE_REDIRECT) {
                    यदि (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        रोक;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    रोक;
                }

                *d++ = ch;

                रोक;
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
                    रोक;
                }

                यदि (type & NGX_UNESCAPE_REDIRECT) {
                    यदि (ch == '?') {
                        *d++ = ch;
                        गच्छ done;
                    }

                    यदि (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        रोक;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    रोक;
                }

                *d++ = ch;

                रोक;
            }

            /* the invalid quoted character */

            रोक;
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

        जबसम्म (size) {
            विकल्प (*src++) {

            पक्ष '<':
                len += परिमाण("&lt;") - 2;
                रोक;

            पक्ष '>':
                len += परिमाण("&gt;") - 2;
                रोक;

            पक्ष '&':
                len += परिमाण("&amp;") - 2;
                रोक;

            पक्ष '"':
                len += परिमाण("&quot;") - 2;
                रोक;

            स्वभाव:
                रोक;
            }
            size--;
        }

        फर्काऊ (uintptr_t) len;
    }

    जबसम्म (size) {
        ch = *src++;

        विकल्प (ch) {

        पक्ष '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            रोक;

        पक्ष '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            रोक;

        पक्ष '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            रोक;

        पक्ष '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            रोक;

        स्वभाव:
            *dst++ = ch;
            रोक;
        }
        size--;
    }

    फर्काऊ (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, माप_प्रकार size)
{
    u_char      ch;
    ngx_uint_t  len;

    यदि (dst == रिक्त) {
        len = 0;

        जबसम्म (size) {
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
                    रोक;

                स्वभाव:
                    len += परिमाण("\\u001F") - 2;
                }
            }

            size--;
        }

        फर्काऊ (uintptr_t) len;
    }

    जबसम्म (size) {
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
                रोक;

            पक्ष '\r':
                *dst++ = 'r';
                रोक;

            पक्ष '\t':
                *dst++ = 't';
                रोक;

            पक्ष '\b':
                *dst++ = 'b';
                रोक;

            पक्ष '\f':
                *dst++ = 'f';
                रोक;

            स्वभाव:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        size--;
    }

    फर्काऊ (uintptr_t) dst;
}


शून्य
ngx_str_rbtree_घुसाउ_मान(ngx_rbtree_गाँठो_t *temp,
    ngx_rbtree_गाँठो_t *गाँठो, ngx_rbtree_गाँठो_t *sentinel)
{
    ngx_str_गाँठो_t      *n, *t;
    ngx_rbtree_गाँठो_t  **p;

    लागि ( ;; ) {

        n = (ngx_str_गाँठो_t *) गाँठो;
        t = (ngx_str_गाँठो_t *) temp;

        यदि (गाँठो->साँचो != temp->साँचो) {

            p = (गाँठो->साँचो < temp->साँचो) ? &temp->बायाँ : &temp->दायाँ;

        } अन्यथा यदि (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &temp->बायाँ : &temp->दायाँ;

        } अन्यथा {
            p = (ngx_memcmp(n->str.data, t->str.data, n->str.len) < 0)
                 ? &temp->बायाँ : &temp->दायाँ;
        }

        यदि (*p == sentinel) {
            रोक;
        }

        temp = *p;
    }

    *p = गाँठो;
    गाँठो->parent = temp;
    गाँठो->बायाँ = sentinel;
    गाँठो->दायाँ = sentinel;
    ngx_rbt_red(गाँठो);
}


ngx_str_गाँठो_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_गाँठो_t     *n;
    ngx_rbtree_गाँठो_t  *गाँठो, *sentinel;

    गाँठो = rbtree->जरा;
    sentinel = rbtree->sentinel;

    जबसम्म (गाँठो != sentinel) {

        n = (ngx_str_गाँठो_t *) गाँठो;

        यदि (hash != गाँठो->साँचो) {
            गाँठो = (hash < गाँठो->साँचो) ? गाँठो->बायाँ : गाँठो->दायाँ;
            जारी;
        }

        यदि (val->len != n->str.len) {
            गाँठो = (val->len < n->str.len) ? गाँठो->बायाँ : गाँठो->दायाँ;
            जारी;
        }

        rc = ngx_memcmp(val->data, n->str.data, val->len);

        यदि (rc < 0) {
            गाँठो = गाँठो->बायाँ;
            जारी;
        }

        यदि (rc > 0) {
            गाँठो = गाँठो->दायाँ;
            जारी;
        }

        फर्काऊ n;
    }

    फर्काऊ रिक्त;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

शून्य
ngx_sort(शून्य *base, माप_प्रकार n, माप_प्रकार size,
    ngx_int_t (*cmp)(अचर शून्य *, अचर शून्य *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(size, ngx_cycle->log);
    यदि (p == रिक्त) {
        फर्काऊ;
    }

    लागि (p1 = (u_char *) base + size;
         p1 < (u_char *) base + n * size;
         p1 += size)
    {
        ngx_memcpy(p, p1, size);

        लागि (p2 = p1;
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
ngx_memcpy(शून्य *dst, अचर शून्य *src, माप_प्रकार n)
{
    यदि (n > NGX_MEMCPY_LIMIT) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "memcpy %uz bytes", n);
        ngx_debug_point();
    }

    फर्काऊ स_नकल(dst, src, n);
}

#endif
