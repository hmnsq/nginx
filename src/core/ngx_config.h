
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONFIG_H_INCLUDED_
#define _NGX_CONFIG_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_auto_headers.h>


#ಯದಿ defined __DragonFly__ && !defined __FreeBSD__
#define __FreeBSD__        4
#define __FreeBSD_version  480101
#endif


#ಯದಿ (NGX_FREEBSD)
#ಅನ್ತರ್ಭಾವ <ngx_freebsd_config.h>


#ನೋಚೇದ್ಯದಿ (NGX_LINUX)
#ಅನ್ತರ್ಭಾವ <ngx_linux_config.h>


#ನೋಚೇದ್ಯದಿ (NGX_SOLARIS)
#ಅನ್ತರ್ಭಾವ <ngx_solaris_config.h>


#ನೋಚೇದ್ಯದಿ (NGX_DARWIN)
#ಅನ್ತರ್ಭಾವ <ngx_darwin_config.h>


#ನೋಚೇದ್ಯದಿ (NGX_WIN32)
#ಅನ್ತರ್ಭಾವ <ngx_win32_config.h>


#ಅನ್ಯಥಾ /* POSIX */
#ಅನ್ತರ್ಭಾವ <ngx_posix_config.h>

#endif


#ifndef NGX_HAVE_SO_SNDLOWAT
#define NGX_HAVE_SO_SNDLOWAT     1
#endif


#ಯದಿ !(NGX_WIN32)

#define ngx_signal_helper(n)     SIG##n
#define ngx_signal_ಮೌಲ್ಯ(n)      ngx_signal_helper(n)

#define ngx_random               random

/* TODO: #ifndef */
#define NGX_SHUTDOWN_SIGNAL      QUIT
#define NGX_TERMINATE_SIGNAL     TERM
#define NGX_NOACCEPT_SIGNAL      WINCH
#define NGX_RECONFIGURE_SIGNAL   HUP

#ಯದಿ (NGX_LINUXTHREADS)
#define NGX_REOPEN_SIGNAL        INFO
#define NGX_CHANGEBIN_SIGNAL     XCPU
#ಅನ್ಯಥಾ
#define NGX_REOPEN_SIGNAL        USR1
#define NGX_CHANGEBIN_SIGNAL     USR2
#endif

#define ngx_cdecl
#define ngx_libc_cdecl

#endif

ಪ್ರಕಾರನಿರ್ದೇಶ intptr_t        ngx_int_t;
ಪ್ರಕಾರನಿರ್ದೇಶ uintptr_t       ngx_uint_t;
ಪ್ರಕಾರನಿರ್ದೇಶ intptr_t        ngx_flag_t;


#define NGX_INT32_LEN   (ಪರಿಮಾಣ("-2147483648") - 1)
#define NGX_INT64_LEN   (ಪರಿಮಾಣ("-9223372036854775808") - 1)

#ಯದಿ (NGX_PTR_SIZE == 4)
#define NGX_INT_T_LEN   NGX_INT32_LEN
#define NGX_MAX_INT_T_ಮೌಲ್ಯ  2147483647

#ಅನ್ಯಥಾ
#define NGX_INT_T_LEN   NGX_INT64_LEN
#define NGX_MAX_INT_T_ಮೌಲ್ಯ  9223372036854775807
#endif


#ifndef NGX_ALIGNMENT
#define NGX_ALIGNMENT   ಪರಿಮಾಣ(uintptr_t)    /* platform word */
#endif

#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#define ngx_abort       abort


/* TODO: platform specific: array[NGX_INVALID_ARRAY_INDEX] must cause SIGSEGV */
#define NGX_INVALID_ARRAY_INDEX 0x80000000


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#ifndef ngx_inline
#define ngx_inline      ಅನ್ತಃಸ್ಥ
#endif

#ifndef INADDR_NONE  /* Solaris */
#define INADDR_NONE  ((unsigned ಪೂರ್ಣಾಙ್ಕ) -1)
#endif

#ifdef MAXHOSTNAMELEN
#define NGX_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#ಅನ್ಯಥಾ
#define NGX_MAXHOSTNAMELEN  256
#endif


#define NGX_MAX_UINT32_ಮೌಲ್ಯ  (uint32_t) 0xffffffff
#define NGX_MAX_INT32_ಮೌಲ್ಯ   (uint32_t) 0x7fffffff


#ಯದಿ (NGX_COMPAT)

#define NGX_COMPAT_BEGIN(slots)  uint64_t spare[slots];
#define NGX_COMPAT_END

#ಅನ್ಯಥಾ

#define NGX_COMPAT_BEGIN(slots)
#define NGX_COMPAT_END

#endif


#endif /* _NGX_CONFIG_H_INCLUDED_ */
