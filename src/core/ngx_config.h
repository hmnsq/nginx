
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_CONFIG_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_CONFIG_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_auto_headers.h>


#ਜੇ defined __DragonFly__ && !defined __FreeBSD__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __FreeBSD__        4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __FreeBSD_version  480101
#ਜੇ_ਅੰਤ


#ਜੇ (NGX_FREEBSD)
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_freebsd_config.h>


#ਜਾਂਜੇ (NGX_LINUX)
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_linux_config.h>


#ਜਾਂਜੇ (NGX_SOLARIS)
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_solaris_config.h>


#ਜਾਂਜੇ (NGX_DARWIN)
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_darwin_config.h>


#ਜਾਂਜੇ (NGX_WIN32)
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_win32_config.h>


#ਨਹੀਂਤਾਂ /* POSIX */
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_posix_config.h>

#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NGX_HAVE_SO_SNDLOWAT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_HAVE_SO_SNDLOWAT     1
#ਜੇ_ਅੰਤ


#ਜੇ !(NGX_WIN32)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_signal_helper(n)     SIG##n
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_signal_ਮੁੱਲ(n)      ngx_signal_helper(n)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_random               random

/* TODO: #ifndef */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_SHUTDOWN_SIGNAL      QUIT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_TERMINATE_SIGNAL     ਪਦ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_NOACCEPT_SIGNAL      WINCH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_RECONFIGURE_SIGNAL   HUP

#ਜੇ (NGX_LINUXTHREADS)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_REOPEN_SIGNAL        INFO
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CHANGEBIN_SIGNAL     XCPU
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_REOPEN_SIGNAL        USR1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CHANGEBIN_SIGNAL     USR2
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_cdecl
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_libc_cdecl

#ਜੇ_ਅੰਤ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ intptr_t        ngx_int_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uintptr_t       ngx_uint_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ intptr_t        ngx_flag_t;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_INT32_LEN   (ਆਕਾਰ("-2147483648") - 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_INT64_LEN   (ਆਕਾਰ("-9223372036854775808") - 1)

#ਜੇ (NGX_PTR_ਮਾਪ == 4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_INT_T_LEN   NGX_INT32_LEN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MAX_INT_T_ਮੁੱਲ  2147483647

#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_INT_T_LEN   NGX_INT64_LEN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MAX_INT_T_ਮੁੱਲ  9223372036854775807
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NGX_ALIGNMENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_ALIGNMENT   ਆਕਾਰ(uintptr_t)    /* platform word */
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_abort       abort


/* TODO: platform specific: array[NGX_INVALID_ARRAY_INDEX] must cause SIGSEGV */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_INVALID_ARRAY_ਸੂਚਕਾਂਕ 0x80000000


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ngx_inline
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_inline      ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INADDR_NONE  /* Solaris */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INADDR_NONE  ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1)
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ MAXHOSTNAMELEN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MAXHOSTNAMELEN  256
#ਜੇ_ਅੰਤ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MAX_UINT32_ਮੁੱਲ  (uint32_t) 0xffffffff
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MAX_INT32_ਮੁੱਲ   (uint32_t) 0x7fffffff


#ਜੇ (NGX_COMPAT)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_COMPAT_BEGIN(slots)  uint64_t ਵਾਧੂ[slots];
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_COMPAT_ਸਿਰਾ

#ਨਹੀਂਤਾਂ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_COMPAT_BEGIN(slots)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_COMPAT_ਸਿਰਾ

#ਜੇ_ਅੰਤ


#ਜੇ_ਅੰਤ /* _NGX_CONFIG_H_INCLUDED_ */
