
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_CONFIG_H_INCLUDED_
#घोषणा _NGX_CONFIG_H_INCLUDED_


#समावेश <ngx_auto_headers.h>


#अगर defined __DragonFly__ && !defined __FreeBSD__
#घोषणा __FreeBSD__        4
#घोषणा __FreeBSD_version  480101
#पूर्ण_अगर


#अगर (NGX_FREEBSD)
#समावेश <ngx_freebsd_config.h>


#या_अगर (NGX_LINUX)
#समावेश <ngx_linux_config.h>


#या_अगर (NGX_SOLARIS)
#समावेश <ngx_solaris_config.h>


#या_अगर (NGX_DARWIN)
#समावेश <ngx_darwin_config.h>


#या_अगर (NGX_WIN32)
#समावेश <ngx_win32_config.h>


#अन्यथा /* POSIX */
#समावेश <ngx_posix_config.h>

#पूर्ण_अगर


#अगर_अघोषित NGX_HAVE_SO_SNDLOWAT
#घोषणा NGX_HAVE_SO_SNDLOWAT     1
#पूर्ण_अगर


#अगर !(NGX_WIN32)

#घोषणा ngx_signal_helper(n)     SIG##n
#घोषणा ngx_signal_मूल्य(n)      ngx_signal_helper(n)

#घोषणा ngx_random               random

/* TODO: #ifndef */
#घोषणा NGX_SHUTDOWN_SIGNAL      QUIT
#घोषणा NGX_TERMINATE_SIGNAL     TERM
#घोषणा NGX_NOACCEPT_SIGNAL      WINCH
#घोषणा NGX_RECONFIGURE_SIGNAL   HUP

#अगर (NGX_LINUXTHREADS)
#घोषणा NGX_REOPEN_SIGNAL        INFO
#घोषणा NGX_CHANGEBIN_SIGNAL     XCPU
#अन्यथा
#घोषणा NGX_REOPEN_SIGNAL        USR1
#घोषणा NGX_CHANGEBIN_SIGNAL     USR2
#पूर्ण_अगर

#घोषणा ngx_cdecl
#घोषणा ngx_libc_cdecl

#पूर्ण_अगर

प्रकार intptr_t        ngx_int_t;
प्रकार uintptr_t       ngx_uint_t;
प्रकार intptr_t        ngx_flag_t;


#घोषणा NGX_INT32_LEN   (माप("-2147483648") - 1)
#घोषणा NGX_INT64_LEN   (माप("-9223372036854775808") - 1)

#अगर (NGX_PTR_आकार == 4)
#घोषणा NGX_INT_T_LEN   NGX_INT32_LEN
#घोषणा NGX_MAX_INT_T_मूल्य  2147483647

#अन्यथा
#घोषणा NGX_INT_T_LEN   NGX_INT64_LEN
#घोषणा NGX_MAX_INT_T_मूल्य  9223372036854775807
#पूर्ण_अगर


#अगर_अघोषित NGX_ALIGNMENT
#घोषणा NGX_ALIGNMENT   माप(uintptr_t)    /* platform word */
#पूर्ण_अगर

#घोषणा ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#घोषणा ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#घोषणा ngx_abort       पात


/* TODO: platform specific: array[NGX_INVALID_ARRAY_INDEX] must cause SIGSEGV */
#घोषणा NGX_INVALID_ARRAY_सूचकांक 0x80000000


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#अगर_अघोषित ngx_inline
#घोषणा ngx_inline      अंतरभूत
#पूर्ण_अगर

#अगर_अघोषित INADDR_NONE  /* Solaris */
#घोषणा INADDR_NONE  ((अचिन्हित पूर्णांक) -1)
#पूर्ण_अगर

#अगर_घोषित MAXHOSTNAMELEN
#घोषणा NGX_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#अन्यथा
#घोषणा NGX_MAXHOSTNAMELEN  256
#पूर्ण_अगर


#घोषणा NGX_MAX_UINT32_मूल्य  (uint32_t) 0xffffffff
#घोषणा NGX_MAX_INT32_मूल्य   (uint32_t) 0x7fffffff


#अगर (NGX_COMPAT)

#घोषणा NGX_COMPAT_BEGIN(slots)  uint64_t spare[slots];
#घोषणा NGX_COMPAT_END

#अन्यथा

#घोषणा NGX_COMPAT_BEGIN(slots)
#घोषणा NGX_COMPAT_END

#पूर्ण_अगर


#पूर्ण_अगर /* _NGX_CONFIG_H_INCLUDED_ */
