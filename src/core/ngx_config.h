
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONFIG_H_INCLUDED_
#define _NGX_CONFIG_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_auto_headers.h>


#ଯଦି defined __DragonFly__ && !defined __FreeBSD__
#define __FreeBSD__        4
#define __FreeBSD_version  480101
#endif


#ଯଦି (NGX_FREEBSD)
#ଅନ୍ତର୍ଭାଵ <ngx_freebsd_config.h>


#ନଚେତଯଦି (NGX_LINUX)
#ଅନ୍ତର୍ଭାଵ <ngx_linux_config.h>


#ନଚେତଯଦି (NGX_SOLARIS)
#ଅନ୍ତର୍ଭାଵ <ngx_solaris_config.h>


#ନଚେତଯଦି (NGX_DARWIN)
#ଅନ୍ତର୍ଭାଵ <ngx_darwin_config.h>


#ନଚେତଯଦି (NGX_WIN32)
#ଅନ୍ତର୍ଭାଵ <ngx_win32_config.h>


#ଅନ୍ୟଥା /* POSIX */
#ଅନ୍ତର୍ଭାଵ <ngx_posix_config.h>

#endif


#ifndef NGX_HAVE_SO_SNDLOWAT
#define NGX_HAVE_SO_SNDLOWAT     1
#endif


#ଯଦି !(NGX_WIN32)

#define ngx_signal_helper(n)     SIG##n
#define ngx_signal_ମୂଲ୍ୟ(n)      ngx_signal_helper(n)

#define ngx_random               random

/* TODO: #ifndef */
#define NGX_SHUTDOWN_SIGNAL      QUIT
#define NGX_TERMINATE_SIGNAL     TERM
#define NGX_NOACCEPT_SIGNAL      WINCH
#define NGX_RECONFIGURE_SIGNAL   HUP

#ଯଦି (NGX_LINUXTHREADS)
#define NGX_REOPEN_SIGNAL        INFO
#define NGX_CHANGEBIN_SIGNAL     XCPU
#ଅନ୍ୟଥା
#define NGX_REOPEN_SIGNAL        USR1
#define NGX_CHANGEBIN_SIGNAL     USR2
#endif

#define ngx_cdecl
#define ngx_libc_cdecl

#endif

ପ୍ରକାରବ୍ୟାଖ୍ୟା intptr_t        ngx_int_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା uintptr_t       ngx_uint_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା intptr_t        ngx_flag_t;


#define NGX_INT32_LEN   (ଆକାର("-2147483648") - 1)
#define NGX_INT64_LEN   (ଆକାର("-9223372036854775808") - 1)

#ଯଦି (NGX_PTR_SIZE == 4)
#define NGX_INT_T_LEN   NGX_INT32_LEN
#define NGX_MAX_INT_T_ମୂଲ୍ୟ  2147483647

#ଅନ୍ୟଥା
#define NGX_INT_T_LEN   NGX_INT64_LEN
#define NGX_MAX_INT_T_ମୂଲ୍ୟ  9223372036854775807
#endif


#ifndef NGX_ALIGNMENT
#define NGX_ALIGNMENT   ଆକାର(uintptr_t)    /* platform word */
#endif

#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#define ngx_abort       abort


/* TODO: platform specific: array[NGX_INVALID_ARRAY_INDEX] must cause SIGSEGV */
#define NGX_INVALID_ARRAY_INDEX 0x80000000


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#ifndef ngx_inline
#define ngx_inline      ଧାଡ଼ିଭିତରେ
#endif

#ifndef INADDR_NONE  /* Solaris */
#define INADDR_NONE  ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) -1)
#endif

#ifdef MAXHOSTNAMELEN
#define NGX_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#ଅନ୍ୟଥା
#define NGX_MAXHOSTNAMELEN  256
#endif


#define NGX_MAX_UINT32_ମୂଲ୍ୟ  (uint32_t) 0xffffffff
#define NGX_MAX_INT32_ମୂଲ୍ୟ   (uint32_t) 0x7fffffff


#ଯଦି (NGX_COMPAT)

#define NGX_COMPAT_BEGIN(slots)  uint64_t spare[slots];
#define NGX_COMPAT_END

#ଅନ୍ୟଥା

#define NGX_COMPAT_BEGIN(slots)
#define NGX_COMPAT_END

#endif


#endif /* _NGX_CONFIG_H_INCLUDED_ */
