
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_CONFIG_H_INCLUDED_
#وصف_ڪريو _NGX_CONFIG_H_INCLUDED_


#ضمن <ngx_auto_headers.h>


#إذا defined __DragonFly__ && !defined __FreeBSD__
#وصف_ڪريو __FreeBSD__        4
#وصف_ڪريو __FreeBSD_version  480101
#جيڪڏهن_پورو


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

#جيڪڏهن_پورو


#جيڪڏهن_وصف_نه_ٿيل NGX_HAVE_SO_SNDLOWAT
#وصف_ڪريو NGX_HAVE_SO_SNDLOWAT     1
#جيڪڏهن_پورو


#إذا !(NGX_WIN32)

#وصف_ڪريو ngx_signal_helper(n)     SIG##n
#وصف_ڪريو ngx_signal_قدر(n)      ngx_signal_helper(n)

#وصف_ڪريو ngx_random               random

/* TODO: #ifndef */
#وصف_ڪريو NGX_SHUTDOWN_SIGNAL      QUIT
#وصف_ڪريو NGX_TERMINATE_SIGNAL     رڪن
#وصف_ڪريو NGX_NOACCEPT_SIGNAL      WINCH
#وصف_ڪريو NGX_RECONFIGURE_SIGNAL   HUP

#إذا (NGX_LINUXTHREADS)
#وصف_ڪريو NGX_REOPEN_SIGNAL        INFO
#وصف_ڪريو NGX_CHANGEBIN_SIGNAL     XCPU
#وإلا
#وصف_ڪريو NGX_REOPEN_SIGNAL        USR1
#وصف_ڪريو NGX_CHANGEBIN_SIGNAL     USR2
#جيڪڏهن_پورو

#وصف_ڪريو ngx_cdecl
#وصف_ڪريو ngx_libc_cdecl

#جيڪڏهن_پورو

تعريفنوع intptr_t        ngx_int_t;
تعريفنوع uintptr_t       ngx_uint_t;
تعريفنوع intptr_t        ngx_flag_t;


#وصف_ڪريو NGX_INT32_LEN   (حجم("-2147483648") - 1)
#وصف_ڪريو NGX_INT64_LEN   (حجم("-9223372036854775808") - 1)

#إذا (NGX_PTR_سائيز == 4)
#وصف_ڪريو NGX_INT_T_LEN   NGX_INT32_LEN
#وصف_ڪريو NGX_MAX_INT_T_قدر  2147483647

#وإلا
#وصف_ڪريو NGX_INT_T_LEN   NGX_INT64_LEN
#وصف_ڪريو NGX_MAX_INT_T_قدر  9223372036854775807
#جيڪڏهن_پورو


#جيڪڏهن_وصف_نه_ٿيل NGX_ALIGNMENT
#وصف_ڪريو NGX_ALIGNMENT   حجم(uintptr_t)    /* platform word */
#جيڪڏهن_پورو

#وصف_ڪريو ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#وصف_ڪريو ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#وصف_ڪريو ngx_abort       abort


/* TODO: platform specific: array[NGX_INVALID_ARRAY_INDEX] must cause SIGSEGV */
#وصف_ڪريو NGX_INVALID_ARRAY_اشاريو 0x80000000


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#جيڪڏهن_وصف_نه_ٿيل ngx_inline
#وصف_ڪريو ngx_inline      مضمن
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INADDR_NONE  /* Solaris */
#وصف_ڪريو INADDR_NONE  ((بنانشان صحيح) -1)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل MAXHOSTNAMELEN
#وصف_ڪريو NGX_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#وإلا
#وصف_ڪريو NGX_MAXHOSTNAMELEN  256
#جيڪڏهن_پورو


#وصف_ڪريو NGX_MAX_UINT32_قدر  (uint32_t) 0xffffffff
#وصف_ڪريو NGX_MAX_INT32_قدر   (uint32_t) 0x7fffffff


#إذا (NGX_COMPAT)

#وصف_ڪريو NGX_COMPAT_BEGIN(slots)  uint64_t اضافي[slots];
#وصف_ڪريو NGX_COMPAT_پڇاڙي

#وإلا

#وصف_ڪريو NGX_COMPAT_BEGIN(slots)
#وصف_ڪريو NGX_COMPAT_پڇاڙي

#جيڪڏهن_پورو


#جيڪڏهن_پورو /* _NGX_CONFIG_H_INCLUDED_ */
