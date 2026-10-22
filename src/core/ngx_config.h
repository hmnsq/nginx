
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_CONFIG_H_INCLUDED_
#परिभाषय _NGX_CONFIG_H_INCLUDED_


#अन्तर्भाव <ngx_auto_headers.h>


#यदि defined __DragonFly__ && !defined __FreeBSD__
#परिभाषय __FreeBSD__        4
#परिभाषय __FreeBSD_version  480101
#यदि_अन्तः


#यदि (NGX_FREEBSD)
#अन्तर्भाव <ngx_freebsd_config.h>


#नोचेद्यदि (NGX_LINUX)
#अन्तर्भाव <ngx_linux_config.h>


#नोचेद्यदि (NGX_SOLARIS)
#अन्तर्भाव <ngx_solaris_config.h>


#नोचेद्यदि (NGX_DARWIN)
#अन्तर्भाव <ngx_darwin_config.h>


#नोचेद्यदि (NGX_WIN32)
#अन्तर्भाव <ngx_win32_config.h>


#अन्यथा /* POSIX */
#अन्तर्भाव <ngx_posix_config.h>

#यदि_अन्तः


#यदि_अपरिभाषितम् NGX_HAVE_SO_SNDLOWAT
#परिभाषय NGX_HAVE_SO_SNDLOWAT     1
#यदि_अन्तः


#यदि !(NGX_WIN32)

#परिभाषय ngx_signal_helper(n)     SIG##n
#परिभाषय ngx_signal_मानम्(n)      ngx_signal_helper(n)

#परिभाषय ngx_random               random

/* TODO: #ifndef */
#परिभाषय NGX_SHUTDOWN_SIGNAL      QUIT
#परिभाषय NGX_TERMINATE_SIGNAL     पदांशः
#परिभाषय NGX_NOACCEPT_SIGNAL      WINCH
#परिभाषय NGX_RECONFIGURE_SIGNAL   HUP

#यदि (NGX_LINUXTHREADS)
#परिभाषय NGX_REOPEN_SIGNAL        INFO
#परिभाषय NGX_CHANGEBIN_SIGNAL     XCPU
#अन्यथा
#परिभाषय NGX_REOPEN_SIGNAL        USR1
#परिभाषय NGX_CHANGEBIN_SIGNAL     USR2
#यदि_अन्तः

#परिभाषय ngx_cdecl
#परिभाषय ngx_libc_cdecl

#यदि_अन्तः

प्रकारनिर्देश intptr_t        ngx_int_t;
प्रकारनिर्देश uintptr_t       ngx_uint_t;
प्रकारनिर्देश intptr_t        ngx_flag_t;


#परिभाषय NGX_INT32_LEN   (परिमाण("-2147483648") - 1)
#परिभाषय NGX_INT64_LEN   (परिमाण("-9223372036854775808") - 1)

#यदि (NGX_PTR_आकार == 4)
#परिभाषय NGX_INT_T_LEN   NGX_INT32_LEN
#परिभाषय NGX_MAX_INT_T_मानम्  2147483647

#अन्यथा
#परिभाषय NGX_INT_T_LEN   NGX_INT64_LEN
#परिभाषय NGX_MAX_INT_T_मानम्  9223372036854775807
#यदि_अन्तः


#यदि_अपरिभाषितम् NGX_ALIGNMENT
#परिभाषय NGX_ALIGNMENT   परिमाण(uintptr_t)    /* platform word */
#यदि_अन्तः

#परिभाषय ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#परिभाषय ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#परिभाषय ngx_abort       abort


/* TODO: platform specific: array[NGX_INVALID_ARRAY_INDEX] must cause SIGSEGV */
#परिभाषय NGX_INVALID_ARRAY_सूचकांक 0x80000000


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#यदि_अपरिभाषितम् ngx_inline
#परिभाषय ngx_inline      अन्तःस्थ
#यदि_अन्तः

#यदि_अपरिभाषितम् INADDR_NONE  /* Solaris */
#परिभाषय INADDR_NONE  ((अचिह्नित पूर्णाङ्क) -1)
#यदि_अन्तः

#यदि_परिभाषितम् MAXHOSTNAMELEN
#परिभाषय NGX_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#अन्यथा
#परिभाषय NGX_MAXHOSTNAMELEN  256
#यदि_अन्तः


#परिभाषय NGX_MAX_UINT32_मानम्  (uint32_t) 0xffffffff
#परिभाषय NGX_MAX_INT32_मानम्   (uint32_t) 0x7fffffff


#यदि (NGX_COMPAT)

#परिभाषय NGX_COMPAT_BEGIN(slots)  uint64_t अतिरिक्तः[slots];
#परिभाषय NGX_COMPAT_अन्तः

#अन्यथा

#परिभाषय NGX_COMPAT_BEGIN(slots)
#परिभाषय NGX_COMPAT_अन्तः

#यदि_अन्तः


#यदि_अन्तः /* _NGX_CONFIG_H_INCLUDED_ */
