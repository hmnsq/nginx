
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_LOG_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_LOG_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_STDERR            0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_EMERG             1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_ALERT             2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_CRIT              3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_ERR               4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_WARN              5
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_NOTICE            6
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_INFO              7
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_DEBUG             8

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_DEBUG_CORE        0x010
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_DEBUG_ALLOC       0x020
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_DEBUG_MUTEX       0x040
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_DEBUG_EVENT       0x080
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_DEBUG_HTTP        0x100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_DEBUG_MAIL        0x200
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_DEBUG_ਧਾਰਾ      0x400

/*
 * do not forget to update debug_levels[] in src/core/ngx_log.c
 * after the adding a new debug level
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_DEBUG_ਪਹਿਲਾ       NGX_LOG_DEBUG_CORE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_DEBUG_ਆਖਰੀ        NGX_LOG_DEBUG_ਧਾਰਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_DEBUG_CONNECTION  0x80000000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_LOG_DEBUG_ALL         0x7ffffff0


ਕਿਸਮਪਰਿਭਾਸ਼ਾ u_char *(*ngx_log_handler_pt) (ngx_log_t *ਲਘੂਗਣਕ, u_char *buf, ਮਾਪ_t len);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ngx_log_writer_pt) (ngx_log_t *ਲਘੂਗਣਕ, ngx_uint_t level,
    u_char *buf, ਮਾਪ_t len);


ਬਣਤਰ ngx_log_s {
    ngx_uint_t           log_level;
    ngx_open_file_t     *file;

    ngx_atomic_uint_t    connection;

    ਸਮਾਂ_t               disk_ਭਰਿਆ_ਸਮਾਂ;

    ngx_log_handler_pt   handler;
    ਖਾਲੀ                *ਅੰਕੜੇ;

    ngx_log_writer_pt    writer;
    ਖਾਲੀ                *wdata;

    /*
     * we declare "action" as "char *" because the actions are usually
     * the static strings and in the "u_char *" case we have to override
     * their types all the time
     */

    ਅੱਖਰ                *action;

    ngx_log_t           *ਅਗਲਾ;

    NGX_COMPAT_BEGIN(5)
    NGX_COMPAT_ਸਿਰਾ
};


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_MAX_ERROR_STR   2048


/*********************************/

#ਜੇ (NGX_HAVE_C99_VARIADIC_MACROS)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_HAVE_VARIADIC_MACROS  1

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_error(level, ਲਘੂਗਣਕ, ...)                                        \
    ਜੇ ((ਲਘੂਗਣਕ)->log_level >= level) ngx_log_error_core(level, ਲਘੂਗਣਕ, __VA_ARGS__)

ਖਾਲੀ ngx_log_error_core(ngx_uint_t level, ngx_log_t *ਲਘੂਗਣਕ, ngx_err_t err,
    ਅਚਲ ਅੱਖਰ *fmt, ...);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug(level, ਲਘੂਗਣਕ, ...)                                        \
    ਜੇ ((ਲਘੂਗਣਕ)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, ਲਘੂਗਣਕ, __VA_ARGS__)

/*********************************/

#ਜਾਂਜੇ (NGX_HAVE_GCC_VARIADIC_MACROS)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_HAVE_VARIADIC_MACROS  1

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_error(level, ਲਘੂਗਣਕ, ਦਲੀਲਾਂ...)                                    \
    ਜੇ ((ਲਘੂਗਣਕ)->log_level >= level) ngx_log_error_core(level, ਲਘੂਗਣਕ, ਦਲੀਲਾਂ)

ਖਾਲੀ ngx_log_error_core(ngx_uint_t level, ngx_log_t *ਲਘੂਗਣਕ, ngx_err_t err,
    ਅਚਲ ਅੱਖਰ *fmt, ...);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug(level, ਲਘੂਗਣਕ, ਦਲੀਲਾਂ...)                                    \
    ਜੇ ((ਲਘੂਗਣਕ)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, ਲਘੂਗਣਕ, ਦਲੀਲਾਂ)

/*********************************/

#ਨਹੀਂਤਾਂ /* no variadic macros */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_HAVE_VARIADIC_MACROS  0

ਖਾਲੀ ngx_cdecl ngx_log_error(ngx_uint_t level, ngx_log_t *ਲਘੂਗਣਕ, ngx_err_t err,
    ਅਚਲ ਅੱਖਰ *fmt, ...);
ਖਾਲੀ ngx_log_error_core(ngx_uint_t level, ngx_log_t *ਲਘੂਗਣਕ, ngx_err_t err,
    ਅਚਲ ਅੱਖਰ *fmt, va_list ਦਲੀਲਾਂ);
ਖਾਲੀ ngx_cdecl ngx_log_debug_core(ngx_log_t *ਲਘੂਗਣਕ, ngx_err_t err,
    ਅਚਲ ਅੱਖਰ *fmt, ...);


#ਜੇ_ਅੰਤ /* variadic macros */


/*********************************/

#ਜੇ (NGX_DEBUG)

#ਜੇ (NGX_HAVE_VARIADIC_MACROS)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug0(level, ਲਘੂਗਣਕ, err, fmt)                                  \
        ngx_log_debug(level, ਲਘੂਗਣਕ, err, fmt)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug1(level, ਲਘੂਗਣਕ, err, fmt, arg1)                            \
        ngx_log_debug(level, ਲਘੂਗਣਕ, err, fmt, arg1)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug2(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2)                      \
        ngx_log_debug(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug3(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3)                \
        ngx_log_debug(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug4(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4)          \
        ngx_log_debug(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug5(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
        ngx_log_debug(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4, arg5)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug6(level, ਲਘੂਗਣਕ, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
        ngx_log_debug(level, ਲਘੂਗਣਕ, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug7(level, ਲਘੂਗਣਕ, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
        ngx_log_debug(level, ਲਘੂਗਣਕ, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug8(level, ਲਘੂਗਣਕ, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
        ngx_log_debug(level, ਲਘੂਗਣਕ, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)


#ਨਹੀਂਤਾਂ /* no variadic macros */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug0(level, ਲਘੂਗਣਕ, err, fmt)                                  \
    ਜੇ ((ਲਘੂਗਣਕ)->log_level & level)                                             \
        ngx_log_debug_core(ਲਘੂਗਣਕ, err, fmt)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug1(level, ਲਘੂਗਣਕ, err, fmt, arg1)                            \
    ਜੇ ((ਲਘੂਗਣਕ)->log_level & level)                                             \
        ngx_log_debug_core(ਲਘੂਗਣਕ, err, fmt, arg1)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug2(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2)                      \
    ਜੇ ((ਲਘੂਗਣਕ)->log_level & level)                                             \
        ngx_log_debug_core(ਲਘੂਗਣਕ, err, fmt, arg1, arg2)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug3(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3)                \
    ਜੇ ((ਲਘੂਗਣਕ)->log_level & level)                                             \
        ngx_log_debug_core(ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug4(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4)          \
    ਜੇ ((ਲਘੂਗਣਕ)->log_level & level)                                             \
        ngx_log_debug_core(ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug5(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
    ਜੇ ((ਲਘੂਗਣਕ)->log_level & level)                                             \
        ngx_log_debug_core(ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4, arg5)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug6(level, ਲਘੂਗਣਕ, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
    ਜੇ ((ਲਘੂਗਣਕ)->log_level & level)                                             \
        ngx_log_debug_core(ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug7(level, ਲਘੂਗਣਕ, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
    ਜੇ ((ਲਘੂਗਣਕ)->log_level & level)                                             \
        ngx_log_debug_core(ਲਘੂਗਣਕ, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug8(level, ਲਘੂਗਣਕ, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
    ਜੇ ((ਲਘੂਗਣਕ)->log_level & level)                                             \
        ngx_log_debug_core(ਲਘੂਗਣਕ, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#ਜੇ_ਅੰਤ

#ਨਹੀਂਤਾਂ /* !NGX_DEBUG */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug0(level, ਲਘੂਗਣਕ, err, fmt)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug1(level, ਲਘੂਗਣਕ, err, fmt, arg1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug2(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug3(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug4(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug5(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4, arg5)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug6(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug7(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_log_debug8(level, ਲਘੂਗਣਕ, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7, arg8)

#ਜੇ_ਅੰਤ

/*********************************/

ngx_log_t *ngx_log_init(u_char *ਅਗੇਤਰ, u_char *error_log);
ਖਾਲੀ ngx_cdecl ngx_log_abort(ngx_err_t err, ਅਚਲ ਅੱਖਰ *fmt, ...);
ਖਾਲੀ ngx_cdecl ngx_log_stderr(ngx_err_t err, ਅਚਲ ਅੱਖਰ *fmt, ...);
u_char *ngx_log_errno(u_char *buf, u_char *ਆਖਰੀ, ngx_err_t err);
ngx_int_t ngx_log_open_default(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ngx_int_t ngx_log_redirect_stderr(ngx_ਚੱਕਰ_t *ਚੱਕਰ);
ngx_log_t *ngx_log_get_file_log(ngx_log_t *ਸਿਰ);
ਅੱਖਰ *ngx_log_ਸੈੱਟ_log(ngx_conf_t *cf, ngx_log_t **ਸਿਰ);


/*
 * ngx_write_stderr() cannot be implemented as macro, since
 * MSVC does not allow to use #ifdef inside macro parameters.
 *
 * ngx_write_fd() is used instead of ngx_write_console(), since
 * CharToOemBuff() inside ngx_write_console() cannot be used with
 * read only buffer as destination and CharToOemBuff() is not needed
 * for ngx_write_stderr() anyway.
 */
ਸਥਿਰ ngx_inline ਖਾਲੀ
ngx_write_stderr(ਅੱਖਰ *ਪਾਠ)
{
    (ਖਾਲੀ) ngx_write_fd(ngx_stderr, ਪਾਠ, ngx_strlen(ਪਾਠ));
}


ਸਥਿਰ ngx_inline ਖਾਲੀ
ngx_write_stdout(ਅੱਖਰ *ਪਾਠ)
{
    (ਖਾਲੀ) ngx_write_fd(ngx_stdout, ਪਾਠ, ngx_strlen(ਪਾਠ));
}


ਬਾਹਰੀ ngx_module_t  ngx_errlog_module;
ਬਾਹਰੀ ngx_uint_t    ngx_use_stderr;


#ਜੇ_ਅੰਤ /* _NGX_LOG_H_INCLUDED_ */
