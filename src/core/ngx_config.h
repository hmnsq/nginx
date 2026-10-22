
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_CONFIG_H_INCLUDED_
#నిర్వచించు _NGX_CONFIG_H_INCLUDED_


#అన్తర్భావ <ngx_auto_headers.h>


#ఒకవేళ defined __DragonFly__ && !defined __FreeBSD__
#నిర్వచించు __FreeBSD__        4
#నిర్వచించు __FreeBSD_version  480101
#అయితే_ముగింపు


#ఒకవేళ (NGX_FREEBSD)
#అన్తర్భావ <ngx_freebsd_config.h>


#లేదావేళ (NGX_LINUX)
#అన్తర్భావ <ngx_linux_config.h>


#లేదావేళ (NGX_SOLARIS)
#అన్తర్భావ <ngx_solaris_config.h>


#లేదావేళ (NGX_DARWIN)
#అన్తర్భావ <ngx_darwin_config.h>


#లేదావేళ (NGX_WIN32)
#అన్తర్భావ <ngx_win32_config.h>


#లేకపోతే /* POSIX */
#అన్తర్భావ <ngx_posix_config.h>

#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే NGX_HAVE_SO_SNDLOWAT
#నిర్వచించు NGX_HAVE_SO_SNDLOWAT     1
#అయితే_ముగింపు


#ఒకవేళ !(NGX_WIN32)

#నిర్వచించు ngx_signal_helper(n)     SIG##n
#నిర్వచించు ngx_signal_విలువ(n)      ngx_signal_helper(n)

#నిర్వచించు ngx_random               random

/* TODO: #ifndef */
#నిర్వచించు NGX_SHUTDOWN_SIGNAL      QUIT
#నిర్వచించు NGX_TERMINATE_SIGNAL     పదం
#నిర్వచించు NGX_NOACCEPT_SIGNAL      WINCH
#నిర్వచించు NGX_RECONFIGURE_SIGNAL   HUP

#ఒకవేళ (NGX_LINUXTHREADS)
#నిర్వచించు NGX_REOPEN_SIGNAL        INFO
#నిర్వచించు NGX_CHANGEBIN_SIGNAL     XCPU
#లేకపోతే
#నిర్వచించు NGX_REOPEN_SIGNAL        USR1
#నిర్వచించు NGX_CHANGEBIN_SIGNAL     USR2
#అయితే_ముగింపు

#నిర్వచించు ngx_cdecl
#నిర్వచించు ngx_libc_cdecl

#అయితే_ముగింపు

రకనిర్వచనం intptr_t        ngx_int_t;
రకనిర్వచనం uintptr_t       ngx_uint_t;
రకనిర్వచనం intptr_t        ngx_flag_t;


#నిర్వచించు NGX_INT32_LEN   (పరిమాణం("-2147483648") - 1)
#నిర్వచించు NGX_INT64_LEN   (పరిమాణం("-9223372036854775808") - 1)

#ఒకవేళ (NGX_PTR_కొలత == 4)
#నిర్వచించు NGX_INT_T_LEN   NGX_INT32_LEN
#నిర్వచించు NGX_MAX_INT_T_విలువ  2147483647

#లేకపోతే
#నిర్వచించు NGX_INT_T_LEN   NGX_INT64_LEN
#నిర్వచించు NGX_MAX_INT_T_విలువ  9223372036854775807
#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే NGX_ALIGNMENT
#నిర్వచించు NGX_ALIGNMENT   పరిమాణం(uintptr_t)    /* platform word */
#అయితే_ముగింపు

#నిర్వచించు ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#నిర్వచించు ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#నిర్వచించు ngx_abort       abort


/* TODO: platform specific: array[NGX_INVALID_ARRAY_INDEX] must cause SIGSEGV */
#నిర్వచించు NGX_INVALID_ARRAY_సూచీ 0x80000000


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#నిర్వచితం_కాకపోతే ngx_inline
#నిర్వచించు ngx_inline      అంతఃవరుస
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INADDR_NONE  /* Solaris */
#నిర్వచించు INADDR_NONE  ((గుర్తులేని పూర్ణాంకం) -1)
#అయితే_ముగింపు

#నిర్వచితమైతే MAXHOSTNAMELEN
#నిర్వచించు NGX_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#లేకపోతే
#నిర్వచించు NGX_MAXHOSTNAMELEN  256
#అయితే_ముగింపు


#నిర్వచించు NGX_MAX_UINT32_విలువ  (uint32_t) 0xffffffff
#నిర్వచించు NGX_MAX_INT32_విలువ   (uint32_t) 0x7fffffff


#ఒకవేళ (NGX_COMPAT)

#నిర్వచించు NGX_COMPAT_BEGIN(slots)  uint64_t అదనపు[slots];
#నిర్వచించు NGX_COMPAT_చివర

#లేకపోతే

#నిర్వచించు NGX_COMPAT_BEGIN(slots)
#నిర్వచించు NGX_COMPAT_చివర

#అయితే_ముగింపు


#అయితే_ముగింపు /* _NGX_CONFIG_H_INCLUDED_ */
