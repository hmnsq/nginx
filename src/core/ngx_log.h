
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_LOG_H_INCLUDED_
#وصف_ڪريو _NGX_LOG_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#وصف_ڪريو NGX_LOG_STDERR            0
#وصف_ڪريو NGX_LOG_EMERG             1
#وصف_ڪريو NGX_LOG_ALERT             2
#وصف_ڪريو NGX_LOG_CRIT              3
#وصف_ڪريو NGX_LOG_ERR               4
#وصف_ڪريو NGX_LOG_WARN              5
#وصف_ڪريو NGX_LOG_NOTICE            6
#وصف_ڪريو NGX_LOG_INFO              7
#وصف_ڪريو NGX_LOG_DEBUG             8

#وصف_ڪريو NGX_LOG_DEBUG_CORE        0x010
#وصف_ڪريو NGX_LOG_DEBUG_ALLOC       0x020
#وصف_ڪريو NGX_LOG_DEBUG_MUTEX       0x040
#وصف_ڪريو NGX_LOG_DEBUG_EVENT       0x080
#وصف_ڪريو NGX_LOG_DEBUG_HTTP        0x100
#وصف_ڪريو NGX_LOG_DEBUG_MAIL        0x200
#وصف_ڪريو NGX_LOG_DEBUG_وهڪرو      0x400

/*
 * do not forget to update debug_levels[] in src/core/ngx_log.c
 * after the adding a new debug level
 */

#وصف_ڪريو NGX_LOG_DEBUG_پهريون       NGX_LOG_DEBUG_CORE
#وصف_ڪريو NGX_LOG_DEBUG_آخري        NGX_LOG_DEBUG_وهڪرو
#وصف_ڪريو NGX_LOG_DEBUG_CONNECTION  0x80000000
#وصف_ڪريو NGX_LOG_DEBUG_ALL         0x7ffffff0


تعريفنوع u_char *(*ngx_log_handler_pt) (ngx_log_t *لاگرٿم, u_char *buf, سائيز_t len);
تعريفنوع فراغ (*ngx_log_writer_pt) (ngx_log_t *لاگرٿم, ngx_uint_t level,
    u_char *buf, سائيز_t len);


بنية ngx_log_s {
    ngx_uint_t           log_level;
    ngx_open_file_t     *file;

    ngx_atomic_uint_t    connection;

    وقت_t               disk_ڀريل_وقت;

    ngx_log_handler_pt   handler;
    فراغ                *معلومات;

    ngx_log_writer_pt    writer;
    فراغ                *wdata;

    /*
     * we declare "action" as "char *" because the actions are usually
     * the static strings and in the "u_char *" case we have to override
     * their types all the time
     */

    حرف                *action;

    ngx_log_t           *ايندڙ;

    NGX_COMPAT_BEGIN(5)
    NGX_COMPAT_پڇاڙي
};


#وصف_ڪريو NGX_MAX_ERROR_STR   2048


/*********************************/

#إذا (NGX_HAVE_C99_VARIADIC_MACROS)

#وصف_ڪريو NGX_HAVE_VARIADIC_MACROS  1

#وصف_ڪريو ngx_log_error(level, لاگرٿم, ...)                                        \
    إذا ((لاگرٿم)->log_level >= level) ngx_log_error_core(level, لاگرٿم, __VA_ARGS__)

فراغ ngx_log_error_core(ngx_uint_t level, ngx_log_t *لاگرٿم, ngx_err_t err,
    ثابت حرف *fmt, ...);

#وصف_ڪريو ngx_log_debug(level, لاگرٿم, ...)                                        \
    إذا ((لاگرٿم)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, لاگرٿم, __VA_ARGS__)

/*********************************/

#وإلاإذا (NGX_HAVE_GCC_VARIADIC_MACROS)

#وصف_ڪريو NGX_HAVE_VARIADIC_MACROS  1

#وصف_ڪريو ngx_log_error(level, لاگرٿم, دليلون...)                                    \
    إذا ((لاگرٿم)->log_level >= level) ngx_log_error_core(level, لاگرٿم, دليلون)

فراغ ngx_log_error_core(ngx_uint_t level, ngx_log_t *لاگرٿم, ngx_err_t err,
    ثابت حرف *fmt, ...);

#وصف_ڪريو ngx_log_debug(level, لاگرٿم, دليلون...)                                    \
    إذا ((لاگرٿم)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, لاگرٿم, دليلون)

/*********************************/

#وإلا /* no variadic macros */

#وصف_ڪريو NGX_HAVE_VARIADIC_MACROS  0

فراغ ngx_cdecl ngx_log_error(ngx_uint_t level, ngx_log_t *لاگرٿم, ngx_err_t err,
    ثابت حرف *fmt, ...);
فراغ ngx_log_error_core(ngx_uint_t level, ngx_log_t *لاگرٿم, ngx_err_t err,
    ثابت حرف *fmt, va_list دليلون);
فراغ ngx_cdecl ngx_log_debug_core(ngx_log_t *لاگرٿم, ngx_err_t err,
    ثابت حرف *fmt, ...);


#جيڪڏهن_پورو /* variadic macros */


/*********************************/

#إذا (NGX_DEBUG)

#إذا (NGX_HAVE_VARIADIC_MACROS)

#وصف_ڪريو ngx_log_debug0(level, لاگرٿم, err, fmt)                                  \
        ngx_log_debug(level, لاگرٿم, err, fmt)

#وصف_ڪريو ngx_log_debug1(level, لاگرٿم, err, fmt, arg1)                            \
        ngx_log_debug(level, لاگرٿم, err, fmt, arg1)

#وصف_ڪريو ngx_log_debug2(level, لاگرٿم, err, fmt, arg1, arg2)                      \
        ngx_log_debug(level, لاگرٿم, err, fmt, arg1, arg2)

#وصف_ڪريو ngx_log_debug3(level, لاگرٿم, err, fmt, arg1, arg2, arg3)                \
        ngx_log_debug(level, لاگرٿم, err, fmt, arg1, arg2, arg3)

#وصف_ڪريو ngx_log_debug4(level, لاگرٿم, err, fmt, arg1, arg2, arg3, arg4)          \
        ngx_log_debug(level, لاگرٿم, err, fmt, arg1, arg2, arg3, arg4)

#وصف_ڪريو ngx_log_debug5(level, لاگرٿم, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
        ngx_log_debug(level, لاگرٿم, err, fmt, arg1, arg2, arg3, arg4, arg5)

#وصف_ڪريو ngx_log_debug6(level, لاگرٿم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
        ngx_log_debug(level, لاگرٿم, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6)

#وصف_ڪريو ngx_log_debug7(level, لاگرٿم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
        ngx_log_debug(level, لاگرٿم, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#وصف_ڪريو ngx_log_debug8(level, لاگرٿم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
        ngx_log_debug(level, لاگرٿم, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)


#وإلا /* no variadic macros */

#وصف_ڪريو ngx_log_debug0(level, لاگرٿم, err, fmt)                                  \
    إذا ((لاگرٿم)->log_level & level)                                             \
        ngx_log_debug_core(لاگرٿم, err, fmt)

#وصف_ڪريو ngx_log_debug1(level, لاگرٿم, err, fmt, arg1)                            \
    إذا ((لاگرٿم)->log_level & level)                                             \
        ngx_log_debug_core(لاگرٿم, err, fmt, arg1)

#وصف_ڪريو ngx_log_debug2(level, لاگرٿم, err, fmt, arg1, arg2)                      \
    إذا ((لاگرٿم)->log_level & level)                                             \
        ngx_log_debug_core(لاگرٿم, err, fmt, arg1, arg2)

#وصف_ڪريو ngx_log_debug3(level, لاگرٿم, err, fmt, arg1, arg2, arg3)                \
    إذا ((لاگرٿم)->log_level & level)                                             \
        ngx_log_debug_core(لاگرٿم, err, fmt, arg1, arg2, arg3)

#وصف_ڪريو ngx_log_debug4(level, لاگرٿم, err, fmt, arg1, arg2, arg3, arg4)          \
    إذا ((لاگرٿم)->log_level & level)                                             \
        ngx_log_debug_core(لاگرٿم, err, fmt, arg1, arg2, arg3, arg4)

#وصف_ڪريو ngx_log_debug5(level, لاگرٿم, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
    إذا ((لاگرٿم)->log_level & level)                                             \
        ngx_log_debug_core(لاگرٿم, err, fmt, arg1, arg2, arg3, arg4, arg5)

#وصف_ڪريو ngx_log_debug6(level, لاگرٿم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
    إذا ((لاگرٿم)->log_level & level)                                             \
        ngx_log_debug_core(لاگرٿم, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#وصف_ڪريو ngx_log_debug7(level, لاگرٿم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
    إذا ((لاگرٿم)->log_level & level)                                             \
        ngx_log_debug_core(لاگرٿم, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#وصف_ڪريو ngx_log_debug8(level, لاگرٿم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
    إذا ((لاگرٿم)->log_level & level)                                             \
        ngx_log_debug_core(لاگرٿم, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#جيڪڏهن_پورو

#وإلا /* !NGX_DEBUG */

#وصف_ڪريو ngx_log_debug0(level, لاگرٿم, err, fmt)
#وصف_ڪريو ngx_log_debug1(level, لاگرٿم, err, fmt, arg1)
#وصف_ڪريو ngx_log_debug2(level, لاگرٿم, err, fmt, arg1, arg2)
#وصف_ڪريو ngx_log_debug3(level, لاگرٿم, err, fmt, arg1, arg2, arg3)
#وصف_ڪريو ngx_log_debug4(level, لاگرٿم, err, fmt, arg1, arg2, arg3, arg4)
#وصف_ڪريو ngx_log_debug5(level, لاگرٿم, err, fmt, arg1, arg2, arg3, arg4, arg5)
#وصف_ڪريو ngx_log_debug6(level, لاگرٿم, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#وصف_ڪريو ngx_log_debug7(level, لاگرٿم, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7)
#وصف_ڪريو ngx_log_debug8(level, لاگرٿم, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7, arg8)

#جيڪڏهن_پورو

/*********************************/

ngx_log_t *ngx_log_init(u_char *اڳياڙي, u_char *error_log);
فراغ ngx_cdecl ngx_log_abort(ngx_err_t err, ثابت حرف *fmt, ...);
فراغ ngx_cdecl ngx_log_stderr(ngx_err_t err, ثابت حرف *fmt, ...);
u_char *ngx_log_errno(u_char *buf, u_char *آخري, ngx_err_t err);
ngx_int_t ngx_log_open_default(ngx_چڪر_t *چڪر);
ngx_int_t ngx_log_redirect_stderr(ngx_چڪر_t *چڪر);
ngx_log_t *ngx_log_get_file_log(ngx_log_t *سر);
حرف *ngx_log_سيٽ_log(ngx_conf_t *cf, ngx_log_t **سر);


/*
 * ngx_write_stderr() cannot be implemented as macro, since
 * MSVC does not allow to use #ifdef inside macro parameters.
 *
 * ngx_write_fd() is used instead of ngx_write_console(), since
 * CharToOemBuff() inside ngx_write_console() cannot be used with
 * read only buffer as destination and CharToOemBuff() is not needed
 * for ngx_write_stderr() anyway.
 */
ساكن ngx_inline فراغ
ngx_write_stderr(حرف *متن)
{
    (فراغ) ngx_write_fd(ngx_stderr, متن, ngx_strlen(متن));
}


ساكن ngx_inline فراغ
ngx_write_stdout(حرف *متن)
{
    (فراغ) ngx_write_fd(ngx_stdout, متن, ngx_strlen(متن));
}


خارجي ngx_module_t  ngx_errlog_module;
خارجي ngx_uint_t    ngx_use_stderr;


#جيڪڏهن_پورو /* _NGX_LOG_H_INCLUDED_ */
