
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


സ്ഥിതം u_char *ngx_sprintf_അക്കം(u_char *buf, u_char *last, uint64_t ui64,
    u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
സ്ഥിതം u_char *ngx_sprintf_str(u_char *buf, u_char *last, u_char *src,
    size_t len, ngx_uint_t hexadecimal);
സ്ഥിതം ശൂന്യം ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    സ്ഥിരം u_char *basis, ngx_uint_t padding);
സ്ഥിതം ngx_int_t ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src,
    സ്ഥിരം u_char *basis);


ശൂന്യം
ngx_strlow(u_char *dst, u_char *src, size_t n)
{
    ആകുമ്പോൾ (n) {
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

    വേണ്ടി (i = 0; i < n; i++) {

        എങ്കിൽ (p[i] == '\0') {
            തിരികെ i;
        }
    }

    തിരികെ n;
}


u_char *
ngx_cpystrn(u_char *dst, u_char *src, size_t n)
{
    എങ്കിൽ (n == 0) {
        തിരികെ dst;
    }

    ആകുമ്പോൾ (--n) {
        *dst = *src;

        എങ്കിൽ (*dst == '\0') {
            തിരികെ dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    തിരികെ dst;
}


u_char *
ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len);
    എങ്കിൽ (dst == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    ngx_memcpy(dst, src->data, src->len);

    തിരികെ dst;
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
ngx_sprintf(u_char *buf, സ്ഥിരം അക്ഷരം *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, (ശൂന്യം *) -1, fmt, args);
    va_end(args);

    തിരികെ p;
}


u_char * ngx_cdecl
ngx_snprintf(u_char *buf, size_t max, സ്ഥിരം അക്ഷരം *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, buf + max, fmt, args);
    va_end(args);

    തിരികെ p;
}


u_char * ngx_cdecl
ngx_slprintf(u_char *buf, u_char *last, സ്ഥിരം അക്ഷരം *fmt, ...)
{
    u_char   *p;
    va_list   args;

    va_start(args, fmt);
    p = ngx_vslprintf(buf, last, fmt, args);
    va_end(args);

    തിരികെ p;
}


u_char *
ngx_vslprintf(u_char *buf, u_char *last, സ്ഥിരം അക്ഷരം *fmt, va_list args)
{
    u_char                *p, zero;
    പൂർണ്ണസംഖ്യ                    d;
    ഇരട്ട                 f;
    size_t                 slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    ngx_msec_t             ms;
    ngx_uint_t             width, sign, hex, max_width, frac_width, scale, n;
    ngx_str_t             *v;
    ngx_variable_മൂല്യം_t  *vv;

    ആകുമ്പോൾ (*fmt && buf < last) {

        /*
         * "buf < last" means that we could copy at least one character:
         * the plain character, "%%", "%c", and minus without the checking
         */

        എങ്കിൽ (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (u_char) ((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (size_t) -1;

            ആകുമ്പോൾ (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }


            വേണ്ടി ( ;; ) {
                തിരഞ്ഞെടുപ്പ് (*fmt) {

                അവസ്ഥ 'u':
                    sign = 0;
                    fmt++;
                    തുടരുക;

                അവസ്ഥ 'm':
                    max_width = 1;
                    fmt++;
                    തുടരുക;

                അവസ്ഥ 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    തുടരുക;

                അവസ്ഥ 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    തുടരുക;

                അവസ്ഥ '.':
                    fmt++;

                    ആകുമ്പോൾ (*fmt >= '0' && *fmt <= '9') {
                        frac_width = frac_width * 10 + (*fmt++ - '0');
                    }

                    നിർത്തുക;

                അവസ്ഥ '*':
                    slen = va_arg(args, size_t);
                    fmt++;
                    തുടരുക;

                സ്ഥിരസ്ഥിതി:
                    നിർത്തുക;
                }

                നിർത്തുക;
            }


            തിരഞ്ഞെടുപ്പ് (*fmt) {

            അവസ്ഥ 'V':
                v = va_arg(args, ngx_str_t *);

                buf = ngx_sprintf_str(buf, last, v->data, v->len, hex);
                fmt++;

                തുടരുക;

            അവസ്ഥ 'v':
                vv = va_arg(args, ngx_variable_മൂല്യം_t *);

                buf = ngx_sprintf_str(buf, last, vv->data, vv->len, hex);
                fmt++;

                തുടരുക;

            അവസ്ഥ 's':
                p = va_arg(args, u_char *);

                buf = ngx_sprintf_str(buf, last, p, slen, hex);
                fmt++;

                തുടരുക;

            അവസ്ഥ 'O':
                i64 = (int64_t) va_arg(args, off_t);
                sign = 1;
                നിർത്തുക;

            അവസ്ഥ 'P':
                i64 = (int64_t) va_arg(args, ngx_pid_t);
                sign = 1;
                നിർത്തുക;

            അവസ്ഥ 'T':
                i64 = (int64_t) va_arg(args, time_t);
                sign = 1;
                നിർത്തുക;

            അവസ്ഥ 'M':
                ms = (ngx_msec_t) va_arg(args, ngx_msec_t);
                എങ്കിൽ ((ngx_msec_int_t) ms == -1) {
                    sign = 1;
                    i64 = -1;
                } അല്ലെങ്കിൽ {
                    sign = 0;
                    ui64 = (uint64_t) ms;
                }
                നിർത്തുക;

            അവസ്ഥ 'z':
                എങ്കിൽ (sign) {
                    i64 = (int64_t) va_arg(args, ssize_t);
                } അല്ലെങ്കിൽ {
                    ui64 = (uint64_t) va_arg(args, size_t);
                }
                നിർത്തുക;

            അവസ്ഥ 'i':
                എങ്കിൽ (sign) {
                    i64 = (int64_t) va_arg(args, ngx_int_t);
                } അല്ലെങ്കിൽ {
                    ui64 = (uint64_t) va_arg(args, ngx_uint_t);
                }

                എങ്കിൽ (max_width) {
                    width = NGX_INT_T_LEN;
                }

                നിർത്തുക;

            അവസ്ഥ 'd':
                എങ്കിൽ (sign) {
                    i64 = (int64_t) va_arg(args, പൂർണ്ണസംഖ്യ);
                } അല്ലെങ്കിൽ {
                    ui64 = (uint64_t) va_arg(args, u_int);
                }
                നിർത്തുക;

            അവസ്ഥ 'l':
                എങ്കിൽ (sign) {
                    i64 = (int64_t) va_arg(args, long);
                } അല്ലെങ്കിൽ {
                    ui64 = (uint64_t) va_arg(args, u_long);
                }
                നിർത്തുക;

            അവസ്ഥ 'D':
                എങ്കിൽ (sign) {
                    i64 = (int64_t) va_arg(args, int32_t);
                } അല്ലെങ്കിൽ {
                    ui64 = (uint64_t) va_arg(args, uint32_t);
                }
                നിർത്തുക;

            അവസ്ഥ 'L':
                എങ്കിൽ (sign) {
                    i64 = va_arg(args, int64_t);
                } അല്ലെങ്കിൽ {
                    ui64 = va_arg(args, uint64_t);
                }
                നിർത്തുക;

            അവസ്ഥ 'A':
                എങ്കിൽ (sign) {
                    i64 = (int64_t) va_arg(args, ngx_atomic_int_t);
                } അല്ലെങ്കിൽ {
                    ui64 = (uint64_t) va_arg(args, ngx_atomic_uint_t);
                }

                എങ്കിൽ (max_width) {
                    width = NGX_ATOMIC_T_LEN;
                }

                നിർത്തുക;

            അവസ്ഥ 'f':
                f = va_arg(args, ഇരട്ട);

                എങ്കിൽ (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t) f;
                frac = 0;

                എങ്കിൽ (frac_width) {

                    scale = 1;
                    വേണ്ടി (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t) ((f - (ഇരട്ട) ui64) * scale + 0.5);

                    എങ്കിൽ (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = ngx_sprintf_അക്കം(buf, last, ui64, zero, 0, width);

                എങ്കിൽ (frac_width) {
                    എങ്കിൽ (buf < last) {
                        *buf++ = '.';
                    }

                    buf = ngx_sprintf_അക്കം(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                തുടരുക;

#എങ്കിൽ !(NGX_WIN32)
            അവസ്ഥ 'r':
                i64 = (int64_t) va_arg(args, rlim_t);
                sign = 1;
                നിർത്തുക;
#endif

            അവസ്ഥ 'p':
                ui64 = (uintptr_t) va_arg(args, ശൂന്യം *);
                hex = 2;
                sign = 0;
                zero = '0';
                width = 2 * വലുപ്പം(ശൂന്യം *);
                നിർത്തുക;

            അവസ്ഥ 'c':
                d = va_arg(args, പൂർണ്ണസംഖ്യ);
                *buf++ = (u_char) (d & 0xff);
                fmt++;

                തുടരുക;

            അവസ്ഥ 'Z':
                *buf++ = '\0';
                fmt++;

                തുടരുക;

            അവസ്ഥ 'N':
#എങ്കിൽ (NGX_WIN32)
                *buf++ = CR;
                എങ്കിൽ (buf < last) {
                    *buf++ = LF;
                }
#അല്ലെങ്കിൽ
                *buf++ = LF;
#endif
                fmt++;

                തുടരുക;

            അവസ്ഥ '%':
                *buf++ = '%';
                fmt++;

                തുടരുക;

            സ്ഥിരസ്ഥിതി:
                *buf++ = *fmt++;

                തുടരുക;
            }

            എങ്കിൽ (sign) {
                എങ്കിൽ (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t) -i64;

                } അല്ലെങ്കിൽ {
                    ui64 = (uint64_t) i64;
                }
            }

            buf = ngx_sprintf_അക്കം(buf, last, ui64, zero, hex, width);

            fmt++;

        } അല്ലെങ്കിൽ {
            *buf++ = *fmt++;
        }
    }

    തിരികെ buf;
}


സ്ഥിതം u_char *
ngx_sprintf_അക്കം(u_char *buf, u_char *last, uint64_t ui64, u_char zero,
    ngx_uint_t hexadecimal, ngx_uint_t width)
{
    u_char         *p, temp[NGX_INT64_LEN + 1];
                       /*
                        * we need temp[NGX_INT64_LEN] only,
                        * but icc issues the warning
                        */
    size_t          len;
    uint32_t        ui32;
    സ്ഥിതം u_char   hex[] = "0123456789abcdef";
    സ്ഥിതം u_char   HEX[] = "0123456789ABCDEF";

    p = temp + NGX_INT64_LEN;

    എങ്കിൽ (hexadecimal == 0) {

        എങ്കിൽ (ui64 <= (uint64_t) NGX_MAX_UINT32_മൂല്യം) {

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

            ചെയ്യുക {
                *--p = (u_char) (ui32 % 10 + '0');
            } ആകുമ്പോൾ (ui32 /= 10);

        } അല്ലെങ്കിൽ {
            ചെയ്യുക {
                *--p = (u_char) (ui64 % 10 + '0');
            } ആകുമ്പോൾ (ui64 /= 10);
        }

    } അല്ലെങ്കിൽ എങ്കിൽ (hexadecimal == 1) {

        ചെയ്യുക {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t) (ui64 & 0xf)];

        } ആകുമ്പോൾ (ui64 >>= 4);

    } അല്ലെങ്കിൽ { /* hexadecimal == 2 */

        ചെയ്യുക {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t) (ui64 & 0xf)];

        } ആകുമ്പോൾ (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    ആകുമ്പോൾ (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    എങ്കിൽ (buf + len > last) {
        len = last - buf;
    }

    തിരികെ ngx_cpymem(buf, p, len);
}


സ്ഥിതം u_char *
ngx_sprintf_str(u_char *buf, u_char *last, u_char *src, size_t len,
    ngx_uint_t hexadecimal)
{
    സ്ഥിതം u_char   hex[] = "0123456789abcdef";
    സ്ഥിതം u_char   HEX[] = "0123456789ABCDEF";

    എങ്കിൽ (hexadecimal == 0) {

        എങ്കിൽ (len == (size_t) -1) {
            ആകുമ്പോൾ (*src && buf < last) {
                *buf++ = *src++;
            }

        } അല്ലെങ്കിൽ {
            len = ngx_min((size_t) (last - buf), len);
            buf = ngx_cpymem(buf, src, len);
        }

    } അല്ലെങ്കിൽ എങ്കിൽ (hexadecimal == 1) {

        എങ്കിൽ (len == (size_t) -1) {

            ആകുമ്പോൾ (*src && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }

        } അല്ലെങ്കിൽ {

            ആകുമ്പോൾ (len-- && buf < last - 1) {
                *buf++ = hex[*src >> 4];
                *buf++ = hex[*src++ & 0xf];
            }
        }

    } അല്ലെങ്കിൽ { /* hexadecimal == 2 */

        എങ്കിൽ (len == (size_t) -1) {

            ആകുമ്പോൾ (*src && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }

        } അല്ലെങ്കിൽ {

            ആകുമ്പോൾ (len-- && buf < last - 1) {
                *buf++ = HEX[*src >> 4];
                *buf++ = HEX[*src++ & 0xf];
            }
        }
    }

    തിരികെ buf;
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

    വേണ്ടി ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        എങ്കിൽ (c1 == c2) {

            എങ്കിൽ (c1) {
                തുടരുക;
            }

            തിരികെ 0;
        }

        തിരികെ c1 - c2;
    }
}


ngx_int_t
ngx_strncasecmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    ആകുമ്പോൾ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        എങ്കിൽ (c1 == c2) {

            എങ്കിൽ (c1) {
                n--;
                തുടരുക;
            }

            തിരികെ 0;
        }

        തിരികെ c1 - c2;
    }

    തിരികെ 0;
}


u_char *
ngx_strnstr(u_char *s1, അക്ഷരം *s2, size_t len)
{
    u_char  c1, c2;
    size_t  n;

    c2 = *(u_char *) s2++;

    n = ngx_strlen(s2);

    ചെയ്യുക {
        ചെയ്യുക {
            എങ്കിൽ (len-- == 0) {
                തിരികെ ശൂന്യ;
            }

            c1 = *s1++;

            എങ്കിൽ (c1 == 0) {
                തിരികെ ശൂന്യ;
            }

        } ആകുമ്പോൾ (c1 != c2);

        എങ്കിൽ (n > len) {
            തിരികെ ശൂന്യ;
        }

    } ആകുമ്പോൾ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    തിരികെ --s1;
}


/*
 * ngx_strstrn() and ngx_strcasestrn() are intended to search for static
 * substring with known length in null-terminated string. The argument n
 * must be length of the second substring - 1.
 */

u_char *
ngx_strstrn(u_char *s1, അക്ഷരം *s2, size_t n)
{
    u_char  c1, c2;

    c2 = *(u_char *) s2++;

    ചെയ്യുക {
        ചെയ്യുക {
            c1 = *s1++;

            എങ്കിൽ (c1 == 0) {
                തിരികെ ശൂന്യ;
            }

        } ആകുമ്പോൾ (c1 != c2);

    } ആകുമ്പോൾ (ngx_strncmp(s1, (u_char *) s2, n) != 0);

    തിരികെ --s1;
}


u_char *
ngx_strcasestrn(u_char *s1, അക്ഷരം *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    c2 = (ngx_uint_t) *s2++;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

    ചെയ്യുക {
        ചെയ്യുക {
            c1 = (ngx_uint_t) *s1++;

            എങ്കിൽ (c1 == 0) {
                തിരികെ ശൂന്യ;
            }

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ആകുമ്പോൾ (c1 != c2);

    } ആകുമ്പോൾ (ngx_strncasecmp(s1, (u_char *) s2, n) != 0);

    തിരികെ --s1;
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

    ചെയ്യുക {
        ചെയ്യുക {
            എങ്കിൽ (s1 >= last) {
                തിരികെ ശൂന്യ;
            }

            c1 = (ngx_uint_t) *s1++;

            c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

        } ആകുമ്പോൾ (c1 != c2);

    } ആകുമ്പോൾ (ngx_strncasecmp(s1, s2, n) != 0);

    തിരികെ --s1;
}


ngx_int_t
ngx_rstrncmp(u_char *s1, u_char *s2, size_t n)
{
    എങ്കിൽ (n == 0) {
        തിരികെ 0;
    }

    n--;

    വേണ്ടി ( ;; ) {
        എങ്കിൽ (s1[n] != s2[n]) {
            തിരികെ s1[n] - s2[n];
        }

        എങ്കിൽ (n == 0) {
            തിരികെ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n)
{
    u_char  c1, c2;

    എങ്കിൽ (n == 0) {
        തിരികെ 0;
    }

    n--;

    വേണ്ടി ( ;; ) {
        c1 = s1[n];
        എങ്കിൽ (c1 >= 'a' && c1 <= 'z') {
            c1 -= 'a' - 'A';
        }

        c2 = s2[n];
        എങ്കിൽ (c2 >= 'a' && c2 <= 'z') {
            c2 -= 'a' - 'A';
        }

        എങ്കിൽ (c1 != c2) {
            തിരികെ c1 - c2;
        }

        എങ്കിൽ (n == 0) {
            തിരികെ 0;
        }

        n--;
    }
}


ngx_int_t
ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2)
{
    size_t     n;
    ngx_int_t  m, z;

    എങ്കിൽ (n1 <= n2) {
        n = n1;
        z = -1;

    } അല്ലെങ്കിൽ {
        n = n2;
        z = 1;
    }

    m = ngx_memcmp(s1, s2, n);

    എങ്കിൽ (m || n1 == n2) {
        തിരികെ m;
    }

    തിരികെ z;
}


ngx_int_t
ngx_dns_strcmp(u_char *s1, u_char *s2)
{
    ngx_uint_t  c1, c2;

    വേണ്ടി ( ;; ) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        എങ്കിൽ (c1 == c2) {

            എങ്കിൽ (c1) {
                തുടരുക;
            }

            തിരികെ 0;
        }

        /* in ASCII '.' > '-', but we need '.' to be the lowest character */

        c1 = (c1 == '.') ? ' ' : c1;
        c2 = (c2 == '.') ? ' ' : c2;

        തിരികെ c1 - c2;
    }
}


ngx_int_t
ngx_filename_cmp(u_char *s1, u_char *s2, size_t n)
{
    ngx_uint_t  c1, c2;

    ആകുമ്പോൾ (n) {
        c1 = (ngx_uint_t) *s1++;
        c2 = (ngx_uint_t) *s2++;

#എങ്കിൽ (NGX_HAVE_CASELESS_FILESYSTEM)
        c1 = tolower(c1);
        c2 = tolower(c2);
#endif

        എങ്കിൽ (c1 == c2) {

            എങ്കിൽ (c1) {
                n--;
                തുടരുക;
            }

            തിരികെ 0;
        }

        /* we need '/' to be the lowest character */

        എങ്കിൽ (c1 == 0 || c2 == 0) {
            തിരികെ c1 - c2;
        }

        c1 = (c1 == '/') ? 0 : c1;
        c2 = (c2 == '/') ? 0 : c2;

        തിരികെ c1 - c2;
    }

    തിരികെ 0;
}


ngx_int_t
ngx_atoi(u_char *വരി, size_t n)
{
    ngx_int_t  മൂല്യം, cutoff, cutlim;

    എങ്കിൽ (n == 0) {
        തിരികെ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_മൂല്യം / 10;
    cutlim = NGX_MAX_INT_T_മൂല്യം % 10;

    വേണ്ടി (മൂല്യം = 0; n--; വരി++) {
        എങ്കിൽ (*വരി < '0' || *വരി > '9') {
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (മൂല്യം >= cutoff && (മൂല്യം > cutoff || *വരി - '0' > cutlim)) {
            തിരികെ NGX_ERROR;
        }

        മൂല്യം = മൂല്യം * 10 + (*വരി - '0');
    }

    തിരികെ മൂല്യം;
}


/* parse a fixed point number, e.g., ngx_atofp("10.5", 4, 2) returns 1050 */

ngx_int_t
ngx_atofp(u_char *വരി, size_t n, size_t point)
{
    ngx_int_t   മൂല്യം, cutoff, cutlim;
    ngx_uint_t  dot;

    എങ്കിൽ (n == 0) {
        തിരികെ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_മൂല്യം / 10;
    cutlim = NGX_MAX_INT_T_മൂല്യം % 10;

    dot = 0;

    വേണ്ടി (മൂല്യം = 0; n--; വരി++) {

        എങ്കിൽ (point == 0) {
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (*വരി == '.') {
            എങ്കിൽ (dot) {
                തിരികെ NGX_ERROR;
            }

            dot = 1;
            തുടരുക;
        }

        എങ്കിൽ (*വരി < '0' || *വരി > '9') {
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (മൂല്യം >= cutoff && (മൂല്യം > cutoff || *വരി - '0' > cutlim)) {
            തിരികെ NGX_ERROR;
        }

        മൂല്യം = മൂല്യം * 10 + (*വരി - '0');
        point -= dot;
    }

    ആകുമ്പോൾ (point--) {
        എങ്കിൽ (മൂല്യം > cutoff) {
            തിരികെ NGX_ERROR;
        }

        മൂല്യം = മൂല്യം * 10;
    }

    തിരികെ മൂല്യം;
}


ssize_t
ngx_atosz(u_char *വരി, size_t n)
{
    ssize_t  മൂല്യം, cutoff, cutlim;

    എങ്കിൽ (n == 0) {
        തിരികെ NGX_ERROR;
    }

    cutoff = NGX_MAX_SIZE_T_മൂല്യം / 10;
    cutlim = NGX_MAX_SIZE_T_മൂല്യം % 10;

    വേണ്ടി (മൂല്യം = 0; n--; വരി++) {
        എങ്കിൽ (*വരി < '0' || *വരി > '9') {
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (മൂല്യം >= cutoff && (മൂല്യം > cutoff || *വരി - '0' > cutlim)) {
            തിരികെ NGX_ERROR;
        }

        മൂല്യം = മൂല്യം * 10 + (*വരി - '0');
    }

    തിരികെ മൂല്യം;
}


off_t
ngx_atoof(u_char *വരി, size_t n)
{
    off_t  മൂല്യം, cutoff, cutlim;

    എങ്കിൽ (n == 0) {
        തിരികെ NGX_ERROR;
    }

    cutoff = NGX_MAX_OFF_T_മൂല്യം / 10;
    cutlim = NGX_MAX_OFF_T_മൂല്യം % 10;

    വേണ്ടി (മൂല്യം = 0; n--; വരി++) {
        എങ്കിൽ (*വരി < '0' || *വരി > '9') {
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (മൂല്യം >= cutoff && (മൂല്യം > cutoff || *വരി - '0' > cutlim)) {
            തിരികെ NGX_ERROR;
        }

        മൂല്യം = മൂല്യം * 10 + (*വരി - '0');
    }

    തിരികെ മൂല്യം;
}


time_t
ngx_atotm(u_char *വരി, size_t n)
{
    time_t  മൂല്യം, cutoff, cutlim;

    എങ്കിൽ (n == 0) {
        തിരികെ NGX_ERROR;
    }

    cutoff = NGX_MAX_TIME_T_മൂല്യം / 10;
    cutlim = NGX_MAX_TIME_T_മൂല്യം % 10;

    വേണ്ടി (മൂല്യം = 0; n--; വരി++) {
        എങ്കിൽ (*വരി < '0' || *വരി > '9') {
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (മൂല്യം >= cutoff && (മൂല്യം > cutoff || *വരി - '0' > cutlim)) {
            തിരികെ NGX_ERROR;
        }

        മൂല്യം = മൂല്യം * 10 + (*വരി - '0');
    }

    തിരികെ മൂല്യം;
}


ngx_int_t
ngx_hextoi(u_char *വരി, size_t n)
{
    u_char     c, ch;
    ngx_int_t  മൂല്യം, cutoff;

    എങ്കിൽ (n == 0) {
        തിരികെ NGX_ERROR;
    }

    cutoff = NGX_MAX_INT_T_മൂല്യം / 16;

    വേണ്ടി (മൂല്യം = 0; n--; വരി++) {
        എങ്കിൽ (മൂല്യം > cutoff) {
            തിരികെ NGX_ERROR;
        }

        ch = *വരി;

        എങ്കിൽ (ch >= '0' && ch <= '9') {
            മൂല്യം = മൂല്യം * 16 + (ch - '0');
            തുടരുക;
        }

        c = (u_char) (ch | 0x20);

        എങ്കിൽ (c >= 'a' && c <= 'f') {
            മൂല്യം = മൂല്യം * 16 + (c - 'a' + 10);
            തുടരുക;
        }

        തിരികെ NGX_ERROR;
    }

    തിരികെ മൂല്യം;
}


u_char *
ngx_hex_dump(u_char *dst, u_char *src, size_t len)
{
    സ്ഥിതം u_char  hex[] = "0123456789abcdef";

    ആകുമ്പോൾ (len--) {
        *dst++ = hex[*src >> 4];
        *dst++ = hex[*src++ & 0xf];
    }

    തിരികെ dst;
}


ശൂന്യം
ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    സ്ഥിതം u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    ngx_encode_base64_internal(dst, src, basis64, 1);
}


ശൂന്യം
ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    സ്ഥിതം u_char   basis64[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    ngx_encode_base64_internal(dst, src, basis64, 0);
}


സ്ഥിതം ശൂന്യം
ngx_encode_base64_internal(ngx_str_t *dst, ngx_str_t *src, സ്ഥിരം u_char *basis,
    ngx_uint_t padding)
{
    u_char         *d, *s;
    size_t          len;

    len = src->len;
    s = src->data;
    d = dst->data;

    ആകുമ്പോൾ (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    എങ്കിൽ (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        എങ്കിൽ (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            എങ്കിൽ (padding) {
                *d++ = '=';
            }

        } അല്ലെങ്കിൽ {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        എങ്കിൽ (padding) {
            *d++ = '=';
        }
    }

    dst->len = d - dst->data;
}


ngx_int_t
ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)
{
    സ്ഥിതം u_char   basis64[] = {
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

    തിരികെ ngx_decode_base64_internal(dst, src, basis64);
}


ngx_int_t
ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src)
{
    സ്ഥിതം u_char   basis64[] = {
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

    തിരികെ ngx_decode_base64_internal(dst, src, basis64);
}


സ്ഥിതം ngx_int_t
ngx_decode_base64_internal(ngx_str_t *dst, ngx_str_t *src, സ്ഥിരം u_char *basis)
{
    size_t          len;
    u_char         *d, *s;

    വേണ്ടി (len = 0; len < src->len; len++) {
        എങ്കിൽ (src->data[len] == '=') {
            നിർത്തുക;
        }

        എങ്കിൽ (basis[src->data[len]] == 77) {
            തിരികെ NGX_ERROR;
        }
    }

    എങ്കിൽ (len % 4 == 1) {
        തിരികെ NGX_ERROR;
    }

    s = src->data;
    d = dst->data;

    ആകുമ്പോൾ (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    എങ്കിൽ (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    എങ്കിൽ (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst->len = d - dst->data;

    തിരികെ NGX_OK;
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

    എങ്കിൽ (u >= 0xf8) {

        (*p)++;
        തിരികെ 0xffffffff;

    } അല്ലെങ്കിൽ എങ്കിൽ (u >= 0xf0) {

        u &= 0x07;
        valid = 0xffff;
        len = 3;

    } അല്ലെങ്കിൽ എങ്കിൽ (u >= 0xe0) {

        u &= 0x0f;
        valid = 0x7ff;
        len = 2;

    } അല്ലെങ്കിൽ എങ്കിൽ (u >= 0xc2) {

        u &= 0x1f;
        valid = 0x7f;
        len = 1;

    } അല്ലെങ്കിൽ {
        (*p)++;
        തിരികെ 0xffffffff;
    }

    എങ്കിൽ (n - 1 < len) {
        തിരികെ 0xfffffffe;
    }

    (*p)++;

    ആകുമ്പോൾ (len) {
        i = *(*p)++;

        എങ്കിൽ (i < 0x80) {
            തിരികെ 0xffffffff;
        }

        u = (u << 6) | (i & 0x3f);

        len--;
    }

    എങ്കിൽ (u > valid) {
        തിരികെ u;
    }

    തിരികെ 0xffffffff;
}


size_t
ngx_utf8_length(u_char *p, size_t n)
{
    u_char  c, *last;
    size_t  len;

    last = p + n;

    വേണ്ടി (len = 0; p < last; len++) {

        c = *p;

        എങ്കിൽ (c < 0x80) {
            p++;
            തുടരുക;
        }

        എങ്കിൽ (ngx_utf8_decode(&p, last - p) > 0x10ffff) {
            /* invalid UTF-8 */
            തിരികെ n;
        }
    }

    തിരികെ len;
}


u_char *
ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len)
{
    u_char  c, *next;

    എങ്കിൽ (n == 0) {
        തിരികെ dst;
    }

    ആകുമ്പോൾ (--n) {

        c = *src;
        *dst = c;

        എങ്കിൽ (c < 0x80) {

            എങ്കിൽ (c != '\0') {
                dst++;
                src++;
                len--;

                തുടരുക;
            }

            തിരികെ dst;
        }

        next = src;

        എങ്കിൽ (ngx_utf8_decode(&next, len) > 0x10ffff) {
            /* invalid UTF-8 */
            നിർത്തുക;
        }

        ആകുമ്പോൾ (src < next) {
            *dst++ = *src++;
            len--;
        }
    }

    *dst = '\0';

    തിരികെ dst;
}


uintptr_t
ngx_escape_uri(u_char *dst, u_char *src, size_t size, ngx_uint_t type)
{
    u_char          prefix;
    uint32_t       *escape;
    ngx_uint_t      n;
    സ്ഥിതം u_char   hex[] = "0123456789ABCDEF";

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

    സ്ഥിതം uint32_t   uri[] = {
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

    സ്ഥിതം uint32_t   args[] = {
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

    സ്ഥിതം uint32_t   uri_component[] = {
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

    സ്ഥിതം uint32_t   html[] = {
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

    സ്ഥിതം uint32_t   refresh[] = {
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

    സ്ഥിതം uint32_t   memcached[] = {
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

    സ്ഥിതം uint32_t   mail_xtext[] = {
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

    സ്ഥിതം uint32_t  *map[] =
        { uri, args, uri_component, html, refresh, memcached, memcached,
          mail_xtext };

    സ്ഥിതം u_char  map_char[] =
        { '%', '%', '%', '%', '%', '%', '%', '+' };


    escape = map[type];
    prefix = map_char[type];

    എങ്കിൽ (dst == ശൂന്യ) {

        /* find the number of the characters to be escaped */

        n = 0;

        ആകുമ്പോൾ (size) {
            എങ്കിൽ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
                n++;
            }
            src++;
            size--;
        }

        തിരികെ (uintptr_t) n;
    }

    ആകുമ്പോൾ (size) {
        എങ്കിൽ (escape[*src >> 5] & (1U << (*src & 0x1f))) {
            *dst++ = prefix;
            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src & 0xf];
            src++;

        } അല്ലെങ്കിൽ {
            *dst++ = *src++;
        }
        size--;
    }

    തിരികെ (uintptr_t) dst;
}


ശൂന്യം
ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type)
{
    u_char  *d, *s, ch, c, decoded;
    എണ്ണൽ {
        sw_usual = 0,
        sw_quoted,
        sw_quoted_second
    } state;

    d = *dst;
    s = *src;

    state = 0;
    decoded = 0;

    ആകുമ്പോൾ (size--) {

        ch = *s++;

        തിരഞ്ഞെടുപ്പ് (state) {
        അവസ്ഥ sw_usual:
            എങ്കിൽ (ch == '?'
                && (type & (NGX_UNESCAPE_URI|NGX_UNESCAPE_REDIRECT)))
            {
                *d++ = ch;
                പോകുക done;
            }

            എങ്കിൽ (ch == '%') {
                state = sw_quoted;
                നിർത്തുക;
            }

            *d++ = ch;
            നിർത്തുക;

        അവസ്ഥ sw_quoted:

            എങ്കിൽ (ch >= '0' && ch <= '9') {
                decoded = (u_char) (ch - '0');
                state = sw_quoted_second;
                നിർത്തുക;
            }

            c = (u_char) (ch | 0x20);
            എങ്കിൽ (c >= 'a' && c <= 'f') {
                decoded = (u_char) (c - 'a' + 10);
                state = sw_quoted_second;
                നിർത്തുക;
            }

            /* the invalid quoted character */

            state = sw_usual;

            *d++ = ch;

            നിർത്തുക;

        അവസ്ഥ sw_quoted_second:

            state = sw_usual;

            എങ്കിൽ (ch >= '0' && ch <= '9') {
                ch = (u_char) ((decoded << 4) + (ch - '0'));

                എങ്കിൽ (type & NGX_UNESCAPE_REDIRECT) {
                    എങ്കിൽ (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        നിർത്തുക;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);

                    നിർത്തുക;
                }

                *d++ = ch;

                നിർത്തുക;
            }

            c = (u_char) (ch | 0x20);
            എങ്കിൽ (c >= 'a' && c <= 'f') {
                ch = (u_char) ((decoded << 4) + (c - 'a') + 10);

                എങ്കിൽ (type & NGX_UNESCAPE_URI) {
                    എങ്കിൽ (ch == '?') {
                        *d++ = ch;
                        പോകുക done;
                    }

                    *d++ = ch;
                    നിർത്തുക;
                }

                എങ്കിൽ (type & NGX_UNESCAPE_REDIRECT) {
                    എങ്കിൽ (ch == '?') {
                        *d++ = ch;
                        പോകുക done;
                    }

                    എങ്കിൽ (ch > '%' && ch < 0x7f) {
                        *d++ = ch;
                        നിർത്തുക;
                    }

                    *d++ = '%'; *d++ = *(s - 2); *d++ = *(s - 1);
                    നിർത്തുക;
                }

                *d++ = ch;

                നിർത്തുക;
            }

            /* the invalid quoted character */

            നിർത്തുക;
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

    എങ്കിൽ (dst == ശൂന്യ) {

        len = 0;

        ആകുമ്പോൾ (size) {
            തിരഞ്ഞെടുപ്പ് (*src++) {

            അവസ്ഥ '<':
                len += വലുപ്പം("&lt;") - 2;
                നിർത്തുക;

            അവസ്ഥ '>':
                len += വലുപ്പം("&gt;") - 2;
                നിർത്തുക;

            അവസ്ഥ '&':
                len += വലുപ്പം("&amp;") - 2;
                നിർത്തുക;

            അവസ്ഥ '"':
                len += വലുപ്പം("&quot;") - 2;
                നിർത്തുക;

            സ്ഥിരസ്ഥിതി:
                നിർത്തുക;
            }
            size--;
        }

        തിരികെ (uintptr_t) len;
    }

    ആകുമ്പോൾ (size) {
        ch = *src++;

        തിരഞ്ഞെടുപ്പ് (ch) {

        അവസ്ഥ '<':
            *dst++ = '&'; *dst++ = 'l'; *dst++ = 't'; *dst++ = ';';
            നിർത്തുക;

        അവസ്ഥ '>':
            *dst++ = '&'; *dst++ = 'g'; *dst++ = 't'; *dst++ = ';';
            നിർത്തുക;

        അവസ്ഥ '&':
            *dst++ = '&'; *dst++ = 'a'; *dst++ = 'm'; *dst++ = 'p';
            *dst++ = ';';
            നിർത്തുക;

        അവസ്ഥ '"':
            *dst++ = '&'; *dst++ = 'q'; *dst++ = 'u'; *dst++ = 'o';
            *dst++ = 't'; *dst++ = ';';
            നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
            *dst++ = ch;
            നിർത്തുക;
        }
        size--;
    }

    തിരികെ (uintptr_t) dst;
}


uintptr_t
ngx_escape_json(u_char *dst, u_char *src, size_t size)
{
    u_char      ch;
    ngx_uint_t  len;

    എങ്കിൽ (dst == ശൂന്യ) {
        len = 0;

        ആകുമ്പോൾ (size) {
            ch = *src++;

            എങ്കിൽ (ch == '\\' || ch == '"') {
                len++;

            } അല്ലെങ്കിൽ എങ്കിൽ (ch <= 0x1f) {

                തിരഞ്ഞെടുപ്പ് (ch) {
                അവസ്ഥ '\n':
                അവസ്ഥ '\r':
                അവസ്ഥ '\t':
                അവസ്ഥ '\b':
                അവസ്ഥ '\f':
                    len++;
                    നിർത്തുക;

                സ്ഥിരസ്ഥിതി:
                    len += വലുപ്പം("\\u001F") - 2;
                }
            }

            size--;
        }

        തിരികെ (uintptr_t) len;
    }

    ആകുമ്പോൾ (size) {
        ch = *src++;

        എങ്കിൽ (ch > 0x1f) {

            എങ്കിൽ (ch == '\\' || ch == '"') {
                *dst++ = '\\';
            }

            *dst++ = ch;

        } അല്ലെങ്കിൽ {
            *dst++ = '\\';

            തിരഞ്ഞെടുപ്പ് (ch) {
            അവസ്ഥ '\n':
                *dst++ = 'n';
                നിർത്തുക;

            അവസ്ഥ '\r':
                *dst++ = 'r';
                നിർത്തുക;

            അവസ്ഥ '\t':
                *dst++ = 't';
                നിർത്തുക;

            അവസ്ഥ '\b':
                *dst++ = 'b';
                നിർത്തുക;

            അവസ്ഥ '\f':
                *dst++ = 'f';
                നിർത്തുക;

            സ്ഥിരസ്ഥിതി:
                *dst++ = 'u'; *dst++ = '0'; *dst++ = '0';
                *dst++ = '0' + (ch >> 4);

                ch &= 0xf;

                *dst++ = (ch < 10) ? ('0' + ch) : ('A' + ch - 10);
            }
        }

        size--;
    }

    തിരികെ (uintptr_t) dst;
}


ശൂന്യം
ngx_str_rbtree_തിരുകുക_മൂല്യം(ngx_rbtree_കണ്ണി_t *temp,
    ngx_rbtree_കണ്ണി_t *കണ്ണി, ngx_rbtree_കണ്ണി_t *sentinel)
{
    ngx_str_കണ്ണി_t      *n, *t;
    ngx_rbtree_കണ്ണി_t  **p;

    വേണ്ടി ( ;; ) {

        n = (ngx_str_കണ്ണി_t *) കണ്ണി;
        t = (ngx_str_കണ്ണി_t *) temp;

        എങ്കിൽ (കണ്ണി->താക്കോൽ != temp->താക്കോൽ) {

            p = (കണ്ണി->താക്കോൽ < temp->താക്കോൽ) ? &temp->ഇടത് : &temp->വലത്;

        } അല്ലെങ്കിൽ എങ്കിൽ (n->str.len != t->str.len) {

            p = (n->str.len < t->str.len) ? &temp->ഇടത് : &temp->വലത്;

        } അല്ലെങ്കിൽ {
            p = (ngx_memcmp(n->str.data, t->str.data, n->str.len) < 0)
                 ? &temp->ഇടത് : &temp->വലത്;
        }

        എങ്കിൽ (*p == sentinel) {
            നിർത്തുക;
        }

        temp = *p;
    }

    *p = കണ്ണി;
    കണ്ണി->parent = temp;
    കണ്ണി->ഇടത് = sentinel;
    കണ്ണി->വലത് = sentinel;
    ngx_rbt_red(കണ്ണി);
}


ngx_str_കണ്ണി_t *
ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *val, uint32_t hash)
{
    ngx_int_t           rc;
    ngx_str_കണ്ണി_t     *n;
    ngx_rbtree_കണ്ണി_t  *കണ്ണി, *sentinel;

    കണ്ണി = rbtree->വേര്;
    sentinel = rbtree->sentinel;

    ആകുമ്പോൾ (കണ്ണി != sentinel) {

        n = (ngx_str_കണ്ണി_t *) കണ്ണി;

        എങ്കിൽ (hash != കണ്ണി->താക്കോൽ) {
            കണ്ണി = (hash < കണ്ണി->താക്കോൽ) ? കണ്ണി->ഇടത് : കണ്ണി->വലത്;
            തുടരുക;
        }

        എങ്കിൽ (val->len != n->str.len) {
            കണ്ണി = (val->len < n->str.len) ? കണ്ണി->ഇടത് : കണ്ണി->വലത്;
            തുടരുക;
        }

        rc = ngx_memcmp(val->data, n->str.data, val->len);

        എങ്കിൽ (rc < 0) {
            കണ്ണി = കണ്ണി->ഇടത്;
            തുടരുക;
        }

        എങ്കിൽ (rc > 0) {
            കണ്ണി = കണ്ണി->വലത്;
            തുടരുക;
        }

        തിരികെ n;
    }

    തിരികെ ശൂന്യ;
}


/* ngx_sort() is implemented as insertion sort because we need stable sort */

ശൂന്യം
ngx_sort(ശൂന്യം *base, size_t n, size_t size,
    ngx_int_t (*cmp)(സ്ഥിരം ശൂന്യം *, സ്ഥിരം ശൂന്യം *))
{
    u_char  *p1, *p2, *p;

    p = ngx_alloc(size, ngx_cycle->log);
    എങ്കിൽ (p == ശൂന്യ) {
        തിരികെ;
    }

    വേണ്ടി (p1 = (u_char *) base + size;
         p1 < (u_char *) base + n * size;
         p1 += size)
    {
        ngx_memcpy(p, p1, size);

        വേണ്ടി (p2 = p1;
             p2 > (u_char *) base && cmp(p2 - size, p) > 0;
             p2 -= size)
        {
            ngx_memcpy(p2, p2 - size, size);
        }

        ngx_memcpy(p2, p, size);
    }

    ngx_free(p);
}


ശൂന്യം
ngx_explicit_memzero(ശൂന്യം *buf, size_t n)
{
    ngx_memzero(buf, n);
    ngx_memory_barrier();
}


#എങ്കിൽ (NGX_MEMCPY_LIMIT)

ശൂന്യം *
ngx_memcpy(ശൂന്യം *dst, സ്ഥിരം ശൂന്യം *src, size_t n)
{
    എങ്കിൽ (n > NGX_MEMCPY_LIMIT) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0, "memcpy %uz bytes", n);
        ngx_debug_point();
    }

    തിരികെ memcpy(dst, src, n);
}

#endif
