
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LOG_H_INCLUDED_
#define _NGX_LOG_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


#define NGX_LOG_STDERR            0
#define NGX_LOG_EMERG             1
#define NGX_LOG_ALERT             2
#define NGX_LOG_CRIT              3
#define NGX_LOG_ERR               4
#define NGX_LOG_WARN              5
#define NGX_LOG_NOTICE            6
#define NGX_LOG_INFO              7
#define NGX_LOG_DEBUG             8

#define NGX_LOG_DEBUG_CORE        0x010
#define NGX_LOG_DEBUG_ALLOC       0x020
#define NGX_LOG_DEBUG_MUTEX       0x040
#define NGX_LOG_DEBUG_EVENT       0x080
#define NGX_LOG_DEBUG_HTTP        0x100
#define NGX_LOG_DEBUG_MAIL        0x200
#define NGX_LOG_DEBUG_STREAM      0x400

/*
 * do not forget to update debug_levels[] in src/core/ngx_log.c
 * after the adding a new debug level
 */

#define NGX_LOG_DEBUG_FIRST       NGX_LOG_DEBUG_CORE
#define NGX_LOG_DEBUG_LAST        NGX_LOG_DEBUG_STREAM
#define NGX_LOG_DEBUG_CONNECTION  0x80000000
#define NGX_LOG_DEBUG_ALL         0x7ffffff0


ପ୍ରକାରବ୍ୟାଖ୍ୟା u_char *(*ngx_log_handler_pt) (ngx_log_t *log, u_char *buf, size_t len);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*ngx_log_writer_pt) (ngx_log_t *log, ngx_uint_t level,
    u_char *buf, size_t len);


ସଂରଚନା ngx_log_s {
    ngx_uint_t           log_level;
    ngx_open_file_t     *file;

    ngx_atomic_uint_t    connection;

    time_t               disk_full_time;

    ngx_log_handler_pt   handler;
    ଶୂନ୍ୟ                *data;

    ngx_log_writer_pt    writer;
    ଶୂନ୍ୟ                *wdata;

    /*
     * we declare "action" as "char *" because the actions are usually
     * the static strings and in the "u_char *" case we have to override
     * their types all the time
     */

    ଅକ୍ଷର                *action;

    ngx_log_t           *next;

    NGX_COMPAT_BEGIN(5)
    NGX_COMPAT_END
};


#define NGX_MAX_ERROR_STR   2048


/*********************************/

#ଯଦି (NGX_HAVE_C99_VARIADIC_MACROS)

#define NGX_HAVE_VARIADIC_MACROS  1

#define ngx_log_error(level, log, ...)                                        \
    ଯଦି ((log)->log_level >= level) ngx_log_error_core(level, log, __VA_ARGS__)

ଶୂନ୍ୟ ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ଅଚଳ ଅକ୍ଷର *fmt, ...);

#define ngx_log_debug(level, log, ...)                                        \
    ଯଦି ((log)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, log, __VA_ARGS__)

/*********************************/

#ନଚେତଯଦି (NGX_HAVE_GCC_VARIADIC_MACROS)

#define NGX_HAVE_VARIADIC_MACROS  1

#define ngx_log_error(level, log, args...)                                    \
    ଯଦି ((log)->log_level >= level) ngx_log_error_core(level, log, args)

ଶୂନ୍ୟ ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ଅଚଳ ଅକ୍ଷର *fmt, ...);

#define ngx_log_debug(level, log, args...)                                    \
    ଯଦି ((log)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, log, args)

/*********************************/

#ଅନ୍ୟଥା /* no variadic macros */

#define NGX_HAVE_VARIADIC_MACROS  0

ଶୂନ୍ୟ ngx_cdecl ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ଅଚଳ ଅକ୍ଷର *fmt, ...);
ଶୂନ୍ୟ ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ଅଚଳ ଅକ୍ଷର *fmt, va_list args);
ଶୂନ୍ୟ ngx_cdecl ngx_log_debug_core(ngx_log_t *log, ngx_err_t err,
    ଅଚଳ ଅକ୍ଷର *fmt, ...);


#endif /* variadic macros */


/*********************************/

#ଯଦି (NGX_DEBUG)

#ଯଦି (NGX_HAVE_VARIADIC_MACROS)

#define ngx_log_debug0(level, log, err, fmt)                                  \
        ngx_log_debug(level, log, err, fmt)

#define ngx_log_debug1(level, log, err, fmt, arg1)                            \
        ngx_log_debug(level, log, err, fmt, arg1)

#define ngx_log_debug2(level, log, err, fmt, arg1, arg2)                      \
        ngx_log_debug(level, log, err, fmt, arg1, arg2)

#define ngx_log_debug3(level, log, err, fmt, arg1, arg2, arg3)                \
        ngx_log_debug(level, log, err, fmt, arg1, arg2, arg3)

#define ngx_log_debug4(level, log, err, fmt, arg1, arg2, arg3, arg4)          \
        ngx_log_debug(level, log, err, fmt, arg1, arg2, arg3, arg4)

#define ngx_log_debug5(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
        ngx_log_debug(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5)

#define ngx_log_debug6(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
        ngx_log_debug(level, log, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6)

#define ngx_log_debug7(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
        ngx_log_debug(level, log, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#define ngx_log_debug8(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
        ngx_log_debug(level, log, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)


#ଅନ୍ୟଥା /* no variadic macros */

#define ngx_log_debug0(level, log, err, fmt)                                  \
    ଯଦି ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt)

#define ngx_log_debug1(level, log, err, fmt, arg1)                            \
    ଯଦି ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt, arg1)

#define ngx_log_debug2(level, log, err, fmt, arg1, arg2)                      \
    ଯଦି ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt, arg1, arg2)

#define ngx_log_debug3(level, log, err, fmt, arg1, arg2, arg3)                \
    ଯଦି ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt, arg1, arg2, arg3)

#define ngx_log_debug4(level, log, err, fmt, arg1, arg2, arg3, arg4)          \
    ଯଦି ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt, arg1, arg2, arg3, arg4)

#define ngx_log_debug5(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
    ଯଦି ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt, arg1, arg2, arg3, arg4, arg5)

#define ngx_log_debug6(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
    ଯଦି ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#define ngx_log_debug7(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
    ଯଦି ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#define ngx_log_debug8(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
    ଯଦି ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#endif

#ଅନ୍ୟଥା /* !NGX_DEBUG */

#define ngx_log_debug0(level, log, err, fmt)
#define ngx_log_debug1(level, log, err, fmt, arg1)
#define ngx_log_debug2(level, log, err, fmt, arg1, arg2)
#define ngx_log_debug3(level, log, err, fmt, arg1, arg2, arg3)
#define ngx_log_debug4(level, log, err, fmt, arg1, arg2, arg3, arg4)
#define ngx_log_debug5(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5)
#define ngx_log_debug6(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#define ngx_log_debug7(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7)
#define ngx_log_debug8(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7, arg8)

#endif

/*********************************/

ngx_log_t *ngx_log_init(u_char *prefix, u_char *error_log);
ଶୂନ୍ୟ ngx_cdecl ngx_log_abort(ngx_err_t err, ଅଚଳ ଅକ୍ଷର *fmt, ...);
ଶୂନ୍ୟ ngx_cdecl ngx_log_stderr(ngx_err_t err, ଅଚଳ ଅକ୍ଷର *fmt, ...);
u_char *ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err);
ngx_int_t ngx_log_open_default(ngx_cycle_t *cycle);
ngx_int_t ngx_log_redirect_stderr(ngx_cycle_t *cycle);
ngx_log_t *ngx_log_get_file_log(ngx_log_t *head);
ଅକ୍ଷର *ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **head);


/*
 * ngx_write_stderr() cannot be implemented as macro, since
 * MSVC does not allow to use #ifdef inside macro parameters.
 *
 * ngx_write_fd() is used instead of ngx_write_console(), since
 * CharToOemBuff() inside ngx_write_console() cannot be used with
 * read only buffer as destination and CharToOemBuff() is not needed
 * for ngx_write_stderr() anyway.
 */
ସ୍ଥିର ngx_inline ଶୂନ୍ୟ
ngx_write_stderr(ଅକ୍ଷର *text)
{
    (ଶୂନ୍ୟ) ngx_write_fd(ngx_stderr, text, ngx_strlen(text));
}


ସ୍ଥିର ngx_inline ଶୂନ୍ୟ
ngx_write_stdout(ଅକ୍ଷର *text)
{
    (ଶୂନ୍ୟ) ngx_write_fd(ngx_stdout, text, ngx_strlen(text));
}


ବାହ୍ୟ ngx_module_t  ngx_errlog_module;
ବାହ୍ୟ ngx_uint_t    ngx_use_stderr;


#endif /* _NGX_LOG_H_INCLUDED_ */
