
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_CONFIG_H_INCLUDED_
#تعریف_کړه _NGX_CONFIG_H_INCLUDED_


#ضمن <ngx_auto_headers.h>


#إذا defined __DragonFly__ && !defined __FreeBSD__
#تعریف_کړه __FreeBSD__        4
#تعریف_کړه __FreeBSD_version  480101
#د_که_پای


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

#د_که_پای


#که_نه_تعریف_شوی NGX_HAVE_SO_SNDLOWAT
#تعریف_کړه NGX_HAVE_SO_SNDLOWAT     1
#د_که_پای


#إذا !(NGX_WIN32)

#تعریف_کړه ngx_signal_helper(n)     SIG##n
#تعریف_کړه ngx_signal_ارزښت(n)      ngx_signal_helper(n)

#تعریف_کړه ngx_random               random

/* TODO: #ifndef */
#تعریف_کړه NGX_SHUTDOWN_SIGNAL      QUIT
#تعریف_کړه NGX_TERMINATE_SIGNAL     غړی
#تعریف_کړه NGX_NOACCEPT_SIGNAL      WINCH
#تعریف_کړه NGX_RECONFIGURE_SIGNAL   HUP

#إذا (NGX_LINUXTHREADS)
#تعریف_کړه NGX_REOPEN_SIGNAL        INFO
#تعریف_کړه NGX_CHANGEBIN_SIGNAL     XCPU
#وإلا
#تعریف_کړه NGX_REOPEN_SIGNAL        USR1
#تعریف_کړه NGX_CHANGEBIN_SIGNAL     USR2
#د_که_پای

#تعریف_کړه ngx_cdecl
#تعریف_کړه ngx_libc_cdecl

#د_که_پای

تعريفنوع intptr_t        ngx_int_t;
تعريفنوع uintptr_t       ngx_uint_t;
تعريفنوع intptr_t        ngx_flag_t;


#تعریف_کړه NGX_INT32_LEN   (حجم("-2147483648") - 1)
#تعریف_کړه NGX_INT64_LEN   (حجم("-9223372036854775808") - 1)

#إذا (NGX_PTR_اندازه == 4)
#تعریف_کړه NGX_INT_T_LEN   NGX_INT32_LEN
#تعریف_کړه NGX_MAX_INT_T_ارزښت  2147483647

#وإلا
#تعریف_کړه NGX_INT_T_LEN   NGX_INT64_LEN
#تعریف_کړه NGX_MAX_INT_T_ارزښت  9223372036854775807
#د_که_پای


#که_نه_تعریف_شوی NGX_ALIGNMENT
#تعریف_کړه NGX_ALIGNMENT   حجم(uintptr_t)    /* platform word */
#د_که_پای

#تعریف_کړه ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#تعریف_کړه ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#تعریف_کړه ngx_abort       abort


/* TODO: platform specific: array[NGX_INVALID_ARRAY_INDEX] must cause SIGSEGV */
#تعریف_کړه NGX_INVALID_ARRAY_شاخص 0x80000000


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#که_نه_تعریف_شوی ngx_inline
#تعریف_کړه ngx_inline      مضمن
#د_که_پای

#که_نه_تعریف_شوی INADDR_NONE  /* Solaris */
#تعریف_کړه INADDR_NONE  ((بینښه صحيح) -1)
#د_که_پای

#که_تعریف_شوی MAXHOSTNAMELEN
#تعریف_کړه NGX_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#وإلا
#تعریف_کړه NGX_MAXHOSTNAMELEN  256
#د_که_پای


#تعریف_کړه NGX_MAX_UINT32_ارزښت  (uint32_t) 0xffffffff
#تعریف_کړه NGX_MAX_INT32_ارزښت   (uint32_t) 0x7fffffff


#إذا (NGX_COMPAT)

#تعریف_کړه NGX_COMPAT_BEGIN(slots)  uint64_t اضافي[slots];
#تعریف_کړه NGX_COMPAT_پای_ټکی

#وإلا

#تعریف_کړه NGX_COMPAT_BEGIN(slots)
#تعریف_کړه NGX_COMPAT_پای_ټکی

#د_که_پای


#د_که_پای /* _NGX_CONFIG_H_INCLUDED_ */
