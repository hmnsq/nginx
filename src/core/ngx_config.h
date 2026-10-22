
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_CONFIG_H_INCLUDED_
#تعریف_کرو _NGX_CONFIG_H_INCLUDED_


#ضمن <ngx_auto_headers.h>


#إذا defined __DragonFly__ && !defined __FreeBSD__
#تعریف_کرو __FreeBSD__        4
#تعریف_کرو __FreeBSD_version  480101
#جے_انت


#إذا (NGX_FREEBSD)
#ضمن <ngx_freebsd_config.h>


#وإلاإذا (NGX_LINUX)
#ضمن <ngx_linux_config.h>


#وإلاإذا (NGX_SOLARIS)
#ضمن <ngx_solaris_config.h>


#وإلاإذا (NGX_DARWIN)
#ضمن <ngx_darwin_config.h>


#وإلاإذا (NGX_WIN32)
#ضمن <ngx_win32_config.h>


#وإلا /* POSIX */
#ضمن <ngx_posix_config.h>

#جے_انت


#جے_بنا_تعریف NGX_HAVE_SO_SNDLOWAT
#تعریف_کرو NGX_HAVE_SO_SNDLOWAT     1
#جے_انت


#إذا !(NGX_WIN32)

#تعریف_کرو ngx_signal_helper(n)     SIG##n
#تعریف_کرو ngx_signal_قدر(n)      ngx_signal_helper(n)

#تعریف_کرو ngx_random               random

/* TODO: #ifndef */
#تعریف_کرو NGX_SHUTDOWN_SIGNAL      QUIT
#تعریف_کرو NGX_TERMINATE_SIGNAL     رکن
#تعریف_کرو NGX_NOACCEPT_SIGNAL      WINCH
#تعریف_کرو NGX_RECONFIGURE_SIGNAL   HUP

#إذا (NGX_LINUXTHREADS)
#تعریف_کرو NGX_REOPEN_SIGNAL        INFO
#تعریف_کرو NGX_CHANGEBIN_SIGNAL     XCPU
#وإلا
#تعریف_کرو NGX_REOPEN_SIGNAL        USR1
#تعریف_کرو NGX_CHANGEBIN_SIGNAL     USR2
#جے_انت

#تعریف_کرو ngx_cdecl
#تعریف_کرو ngx_libc_cdecl

#جے_انت

تعريفنوع intptr_t        ngx_int_t;
تعريفنوع uintptr_t       ngx_uint_t;
تعريفنوع intptr_t        ngx_flag_t;


#تعریف_کرو NGX_INT32_LEN   (حجم("-2147483648") - 1)
#تعریف_کرو NGX_INT64_LEN   (حجم("-9223372036854775808") - 1)

#إذا (NGX_PTR_ناپ == 4)
#تعریف_کرو NGX_INT_T_LEN   NGX_INT32_LEN
#تعریف_کرو NGX_MAX_INT_T_قدر  2147483647

#وإلا
#تعریف_کرو NGX_INT_T_LEN   NGX_INT64_LEN
#تعریف_کرو NGX_MAX_INT_T_قدر  9223372036854775807
#جے_انت


#جے_بنا_تعریف NGX_ALIGNMENT
#تعریف_کرو NGX_ALIGNMENT   حجم(uintptr_t)    /* platform word */
#جے_انت

#تعریف_کرو ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#تعریف_کرو ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#تعریف_کرو ngx_abort       abort


/* TODO: platform specific: array[NGX_INVALID_ARRAY_INDEX] must cause SIGSEGV */
#تعریف_کرو NGX_INVALID_ARRAY_اشاریہ 0x80000000


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#جے_بنا_تعریف ngx_inline
#تعریف_کرو ngx_inline      مضمن
#جے_انت

#جے_بنا_تعریف INADDR_NONE  /* Solaris */
#تعریف_کرو INADDR_NONE  ((بنانشانی صحيح) -1)
#جے_انت

#جے_تعریف_شدہ MAXHOSTNAMELEN
#تعریف_کرو NGX_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#وإلا
#تعریف_کرو NGX_MAXHOSTNAMELEN  256
#جے_انت


#تعریف_کرو NGX_MAX_UINT32_قدر  (uint32_t) 0xffffffff
#تعریف_کرو NGX_MAX_INT32_قدر   (uint32_t) 0x7fffffff


#إذا (NGX_COMPAT)

#تعریف_کرو NGX_COMPAT_BEGIN(slots)  uint64_t وادھو[slots];
#تعریف_کرو NGX_COMPAT_سرا

#وإلا

#تعریف_کرو NGX_COMPAT_BEGIN(slots)
#تعریف_کرو NGX_COMPAT_سرا

#جے_انت


#جے_انت /* _NGX_CONFIG_H_INCLUDED_ */
