
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_CONFIG_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_CONFIG_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_auto_headers.h>


#ಆದರೆ defined __DragonFly__ && !defined __FreeBSD__
#ವ್ಯಾಖ್ಯಾನಿಸು __FreeBSD__        4
#ವ್ಯಾಖ್ಯಾನಿಸು __FreeBSD_version  480101
#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ (NGX_FREEBSD)
#ಅನ್ತರ್ಭಾವ <ngx_freebsd_config.h>


#ಅಥವಾಆದರೆ (NGX_LINUX)
#ಅನ್ತರ್ಭಾವ <ngx_linux_config.h>


#ಅಥವಾಆದರೆ (NGX_SOLARIS)
#ಅನ್ತರ್ಭಾವ <ngx_solaris_config.h>


#ಅಥವಾಆದರೆ (NGX_DARWIN)
#ಅನ್ತರ್ಭಾವ <ngx_darwin_config.h>


#ಅಥವಾಆದರೆ (NGX_WIN32)
#ಅನ್ತರ್ಭಾವ <ngx_win32_config.h>


#ಇಲ್ಲದಿದ್ದರೆ /* POSIX */
#ಅನ್ತರ್ಭಾವ <ngx_posix_config.h>

#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NGX_HAVE_SO_SNDLOWAT
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_HAVE_SO_SNDLOWAT     1
#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ !(NGX_WIN32)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_signal_helper(n)     SIG##n
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_signal_ಮೌಲ್ಯ(n)      ngx_signal_helper(n)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_random               random

/* TODO: #ifndef */
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_SHUTDOWN_SIGNAL      QUIT
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_TERMINATE_SIGNAL     ಪದ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_NOACCEPT_SIGNAL      WINCH
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_RECONFIGURE_SIGNAL   HUP

#ಆದರೆ (NGX_LINUXTHREADS)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_REOPEN_SIGNAL        INFO
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CHANGEBIN_SIGNAL     XCPU
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_REOPEN_SIGNAL        USR1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CHANGEBIN_SIGNAL     USR2
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_cdecl
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_libc_cdecl

#ಆದರೆ_ಅಂತ್ಯ

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ intptr_t        ngx_int_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uintptr_t       ngx_uint_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ intptr_t        ngx_flag_t;


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_INT32_LEN   (ಗಾತ್ರ("-2147483648") - 1)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_INT64_LEN   (ಗಾತ್ರ("-9223372036854775808") - 1)

#ಆದರೆ (NGX_PTR_ಗಾತ್ರ == 4)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_INT_T_LEN   NGX_INT32_LEN
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MAX_INT_T_ಮೌಲ್ಯ  2147483647

#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_INT_T_LEN   NGX_INT64_LEN
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MAX_INT_T_ಮೌಲ್ಯ  9223372036854775807
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NGX_ALIGNMENT
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_ALIGNMENT   ಗಾತ್ರ(uintptr_t)    /* platform word */
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#ವ್ಯಾಖ್ಯಾನಿಸು ngx_abort       abort


/* TODO: platform specific: array[NGX_INVALID_ARRAY_INDEX] must cause SIGSEGV */
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_INVALID_ARRAY_ಸೂಚ್ಯಂಕ 0x80000000


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ngx_inline
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_inline      ಸಾಲೊಳಗೆ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INADDR_NONE  /* Solaris */
#ವ್ಯಾಖ್ಯಾನಿಸು INADDR_NONE  ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) -1)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ MAXHOSTNAMELEN
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MAXHOSTNAMELEN  256
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MAX_UINT32_ಮೌಲ್ಯ  (uint32_t) 0xffffffff
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MAX_INT32_ಮೌಲ್ಯ   (uint32_t) 0x7fffffff


#ಆದರೆ (NGX_COMPAT)

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_COMPAT_BEGIN(slots)  uint64_t ಹೆಚ್ಚುವರಿ[slots];
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_COMPAT_ಕೊನೆ

#ಇಲ್ಲದಿದ್ದರೆ

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_COMPAT_BEGIN(slots)
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_COMPAT_ಕೊನೆ

#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_CONFIG_H_INCLUDED_ */
