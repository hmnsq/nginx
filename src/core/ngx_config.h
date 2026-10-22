
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_CONFIG_H_INCLUDED_
#تعریف_کرو _NGX_CONFIG_H_INCLUDED_


#شامل <ngx_auto_headers.h>


#اگر defined __DragonFly__ && !defined __FreeBSD__
#تعریف_کرو __FreeBSD__        4
#تعریف_کرو __FreeBSD_version  480101
#اگر_ختم


#اگر (NGX_FREEBSD)
#شامل <ngx_freebsd_config.h>


#وگرنہ_اگر (NGX_LINUX)
#شامل <ngx_linux_config.h>


#وگرنہ_اگر (NGX_SOLARIS)
#شامل <ngx_solaris_config.h>


#وگرنہ_اگر (NGX_DARWIN)
#شامل <ngx_darwin_config.h>


#وگرنہ_اگر (NGX_WIN32)
#شامل <ngx_win32_config.h>


#ورنہ /* POSIX */
#شامل <ngx_posix_config.h>

#اگر_ختم


#اگر_غیر_تعریف_شدہ NGX_HAVE_SO_SNDLOWAT
#تعریف_کرو NGX_HAVE_SO_SNDLOWAT     1
#اگر_ختم


#اگر !(NGX_WIN32)

#تعریف_کرو ngx_signal_helper(n)     SIG##n
#تعریف_کرو ngx_signal_قدر(n)      ngx_signal_helper(n)

#تعریف_کرو ngx_random               random

/* TODO: #ifndef */
#تعریف_کرو NGX_SHUTDOWN_SIGNAL      QUIT
#تعریف_کرو NGX_TERMINATE_SIGNAL     رکن
#تعریف_کرو NGX_NOACCEPT_SIGNAL      WINCH
#تعریف_کرو NGX_RECONFIGURE_SIGNAL   HUP

#اگر (NGX_LINUXTHREADS)
#تعریف_کرو NGX_REOPEN_SIGNAL        INFO
#تعریف_کرو NGX_CHANGEBIN_SIGNAL     XCPU
#ورنہ
#تعریف_کرو NGX_REOPEN_SIGNAL        USR1
#تعریف_کرو NGX_CHANGEBIN_SIGNAL     USR2
#اگر_ختم

#تعریف_کرو ngx_cdecl
#تعریف_کرو ngx_libc_cdecl

#اگر_ختم

قسمتعریف intptr_t        ngx_int_t;
قسمتعریف uintptr_t       ngx_uint_t;
قسمتعریف intptr_t        ngx_flag_t;


#تعریف_کرو NGX_INT32_LEN   (حجم("-2147483648") - 1)
#تعریف_کرو NGX_INT64_LEN   (حجم("-9223372036854775808") - 1)

#اگر (NGX_PTR_سائز == 4)
#تعریف_کرو NGX_INT_T_LEN   NGX_INT32_LEN
#تعریف_کرو NGX_MAX_INT_T_قدر  2147483647

#ورنہ
#تعریف_کرو NGX_INT_T_LEN   NGX_INT64_LEN
#تعریف_کرو NGX_MAX_INT_T_قدر  9223372036854775807
#اگر_ختم


#اگر_غیر_تعریف_شدہ NGX_ALIGNMENT
#تعریف_کرو NGX_ALIGNMENT   حجم(uintptr_t)    /* platform word */
#اگر_ختم

#تعریف_کرو ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#تعریف_کرو ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#تعریف_کرو ngx_abort       abort


/* TODO: platform specific: array[NGX_INVALID_ARRAY_INDEX] must cause SIGSEGV */
#تعریف_کرو NGX_INVALID_ARRAY_اشاریہ 0x80000000


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#اگر_غیر_تعریف_شدہ ngx_inline
#تعریف_کرو ngx_inline      اندرونی
#اگر_ختم

#اگر_غیر_تعریف_شدہ INADDR_NONE  /* Solaris */
#تعریف_کرو INADDR_NONE  ((غیرعلامتی صحیح) -1)
#اگر_ختم

#اگر_تعریف_شدہ MAXHOSTNAMELEN
#تعریف_کرو NGX_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#ورنہ
#تعریف_کرو NGX_MAXHOSTNAMELEN  256
#اگر_ختم


#تعریف_کرو NGX_MAX_UINT32_قدر  (uint32_t) 0xffffffff
#تعریف_کرو NGX_MAX_INT32_قدر   (uint32_t) 0x7fffffff


#اگر (NGX_COMPAT)

#تعریف_کرو NGX_COMPAT_BEGIN(slots)  uint64_t فالتو[slots];
#تعریف_کرو NGX_COMPAT_انتہا

#ورنہ

#تعریف_کرو NGX_COMPAT_BEGIN(slots)
#تعریف_کرو NGX_COMPAT_انتہا

#اگر_ختم


#اگر_ختم /* _NGX_CONFIG_H_INCLUDED_ */
