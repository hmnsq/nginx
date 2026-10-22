
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_CONFIG_H_INCLUDED_
#تعریف_کن _NGX_CONFIG_H_INCLUDED_


#ضمن <ngx_auto_headers.h>


#إذا defined __DragonFly__ && !defined __FreeBSD__
#تعریف_کن __FreeBSD__        4
#تعریف_کن __FreeBSD_version  480101
#پایان_اگر


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

#پایان_اگر


#اگر_تعریف_نشده NGX_HAVE_SO_SNDLOWAT
#تعریف_کن NGX_HAVE_SO_SNDLOWAT     1
#پایان_اگر


#إذا !(NGX_WIN32)

#تعریف_کن ngx_signal_helper(n)     SIG##n
#تعریف_کن ngx_signal_مقدار(n)      ngx_signal_helper(n)

#تعریف_کن ngx_random               random

/* TODO: #ifndef */
#تعریف_کن NGX_SHUTDOWN_SIGNAL      QUIT
#تعریف_کن NGX_TERMINATE_SIGNAL     جمله
#تعریف_کن NGX_NOACCEPT_SIGNAL      WINCH
#تعریف_کن NGX_RECONFIGURE_SIGNAL   HUP

#إذا (NGX_LINUXTHREADS)
#تعریف_کن NGX_REOPEN_SIGNAL        INFO
#تعریف_کن NGX_CHANGEBIN_SIGNAL     XCPU
#وإلا
#تعریف_کن NGX_REOPEN_SIGNAL        USR1
#تعریف_کن NGX_CHANGEBIN_SIGNAL     USR2
#پایان_اگر

#تعریف_کن ngx_cdecl
#تعریف_کن ngx_libc_cdecl

#پایان_اگر

تعريفنوع intptr_t        ngx_int_t;
تعريفنوع uintptr_t       ngx_uint_t;
تعريفنوع intptr_t        ngx_flag_t;


#تعریف_کن NGX_INT32_LEN   (حجم("-2147483648") - 1)
#تعریف_کن NGX_INT64_LEN   (حجم("-9223372036854775808") - 1)

#إذا (NGX_PTR_اندازه == 4)
#تعریف_کن NGX_INT_T_LEN   NGX_INT32_LEN
#تعریف_کن NGX_MAX_INT_T_مقدار  2147483647

#وإلا
#تعریف_کن NGX_INT_T_LEN   NGX_INT64_LEN
#تعریف_کن NGX_MAX_INT_T_مقدار  9223372036854775807
#پایان_اگر


#اگر_تعریف_نشده NGX_ALIGNMENT
#تعریف_کن NGX_ALIGNMENT   حجم(uintptr_t)    /* platform word */
#پایان_اگر

#تعریف_کن ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#تعریف_کن ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#تعریف_کن ngx_abort       abort


/* TODO: platform specific: array[NGX_INVALID_ARRAY_INDEX] must cause SIGSEGV */
#تعریف_کن NGX_INVALID_ARRAY_شاخص 0x80000000


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#اگر_تعریف_نشده ngx_inline
#تعریف_کن ngx_inline      مضمن
#پایان_اگر

#اگر_تعریف_نشده INADDR_NONE  /* Solaris */
#تعریف_کن INADDR_NONE  ((بیعلامت صحيح) -1)
#پایان_اگر

#اگر_تعریف_شده MAXHOSTNAMELEN
#تعریف_کن NGX_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#وإلا
#تعریف_کن NGX_MAXHOSTNAMELEN  256
#پایان_اگر


#تعریف_کن NGX_MAX_UINT32_مقدار  (uint32_t) 0xffffffff
#تعریف_کن NGX_MAX_INT32_مقدار   (uint32_t) 0x7fffffff


#إذا (NGX_COMPAT)

#تعریف_کن NGX_COMPAT_BEGIN(slots)  uint64_t یدکی[slots];
#تعریف_کن NGX_COMPAT_انتها

#وإلا

#تعریف_کن NGX_COMPAT_BEGIN(slots)
#تعریف_کن NGX_COMPAT_انتها

#پایان_اگر


#پایان_اگر /* _NGX_CONFIG_H_INCLUDED_ */
