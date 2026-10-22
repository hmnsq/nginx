
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONFIG_H_INCLUDED_
#define _NGX_CONFIG_H_INCLUDED_


#அந்தர்பாவ <ngx_auto_headers.h>


#எனில் defined __DragonFly__ && !defined __FreeBSD__
#define __FreeBSD__        4
#define __FreeBSD_version  480101
#endif


#எனில் (NGX_FREEBSD)
#அந்தர்பாவ <ngx_freebsd_config.h>


#அல்லதெனில் (NGX_LINUX)
#அந்தர்பாவ <ngx_linux_config.h>


#அல்லதெனில் (NGX_SOLARIS)
#அந்தர்பாவ <ngx_solaris_config.h>


#அல்லதெனில் (NGX_DARWIN)
#அந்தர்பாவ <ngx_darwin_config.h>


#அல்லதெனில் (NGX_WIN32)
#அந்தர்பாவ <ngx_win32_config.h>


#இல்லையெனில் /* POSIX */
#அந்தர்பாவ <ngx_posix_config.h>

#endif


#ifndef NGX_HAVE_SO_SNDLOWAT
#define NGX_HAVE_SO_SNDLOWAT     1
#endif


#எனில் !(NGX_WIN32)

#define ngx_signal_helper(n)     SIG##n
#define ngx_signal_மதிப்பு(n)      ngx_signal_helper(n)

#define ngx_random               random

/* TODO: #ifndef */
#define NGX_SHUTDOWN_SIGNAL      QUIT
#define NGX_TERMINATE_SIGNAL     TERM
#define NGX_NOACCEPT_SIGNAL      WINCH
#define NGX_RECONFIGURE_SIGNAL   HUP

#எனில் (NGX_LINUXTHREADS)
#define NGX_REOPEN_SIGNAL        INFO
#define NGX_CHANGEBIN_SIGNAL     XCPU
#இல்லையெனில்
#define NGX_REOPEN_SIGNAL        USR1
#define NGX_CHANGEBIN_SIGNAL     USR2
#endif

#define ngx_cdecl
#define ngx_libc_cdecl

#endif

வகைவரை intptr_t        ngx_int_t;
வகைவரை uintptr_t       ngx_uint_t;
வகைவரை intptr_t        ngx_flag_t;


#define NGX_INT32_LEN   (அளவு("-2147483648") - 1)
#define NGX_INT64_LEN   (அளவு("-9223372036854775808") - 1)

#எனில் (NGX_PTR_SIZE == 4)
#define NGX_INT_T_LEN   NGX_INT32_LEN
#define NGX_MAX_INT_T_மதிப்பு  2147483647

#இல்லையெனில்
#define NGX_INT_T_LEN   NGX_INT64_LEN
#define NGX_MAX_INT_T_மதிப்பு  9223372036854775807
#endif


#ifndef NGX_ALIGNMENT
#define NGX_ALIGNMENT   அளவு(uintptr_t)    /* platform word */
#endif

#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#define ngx_abort       abort


/* TODO: platform specific: array[NGX_INVALID_ARRAY_INDEX] must cause SIGSEGV */
#define NGX_INVALID_ARRAY_INDEX 0x80000000


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#ifndef ngx_inline
#define ngx_inline      உள்வரி
#endif

#ifndef INADDR_NONE  /* Solaris */
#define INADDR_NONE  ((unsigned முழு) -1)
#endif

#ifdef MAXHOSTNAMELEN
#define NGX_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#இல்லையெனில்
#define NGX_MAXHOSTNAMELEN  256
#endif


#define NGX_MAX_UINT32_மதிப்பு  (uint32_t) 0xffffffff
#define NGX_MAX_INT32_மதிப்பு   (uint32_t) 0x7fffffff


#எனில் (NGX_COMPAT)

#define NGX_COMPAT_BEGIN(slots)  uint64_t spare[slots];
#define NGX_COMPAT_END

#இல்லையெனில்

#define NGX_COMPAT_BEGIN(slots)
#define NGX_COMPAT_END

#endif


#endif /* _NGX_CONFIG_H_INCLUDED_ */
