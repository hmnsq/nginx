
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_LOG_H_INCLUDED_
#تعریف_کن _NGX_LOG_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کن NGX_LOG_STDERR            0
#تعریف_کن NGX_LOG_EMERG             1
#تعریف_کن NGX_LOG_ALERT             2
#تعریف_کن NGX_LOG_CRIT              3
#تعریف_کن NGX_LOG_ERR               4
#تعریف_کن NGX_LOG_WARN              5
#تعریف_کن NGX_LOG_NOTICE            6
#تعریف_کن NGX_LOG_INFO              7
#تعریف_کن NGX_LOG_DEBUG             8

#تعریف_کن NGX_LOG_DEBUG_CORE        0x010
#تعریف_کن NGX_LOG_DEBUG_ALLOC       0x020
#تعریف_کن NGX_LOG_DEBUG_MUTEX       0x040
#تعریف_کن NGX_LOG_DEBUG_EVENT       0x080
#تعریف_کن NGX_LOG_DEBUG_HTTP        0x100
#تعریف_کن NGX_LOG_DEBUG_MAIL        0x200
#تعریف_کن NGX_LOG_DEBUG_جریان      0x400

/*
 * do not forget to update debug_levels[] in src/core/ngx_log.c
 * after the adding a new debug level
 */

#تعریف_کن NGX_LOG_DEBUG_اولی       NGX_LOG_DEBUG_CORE
#تعریف_کن NGX_LOG_DEBUG_آخرین        NGX_LOG_DEBUG_جریان
#تعریف_کن NGX_LOG_DEBUG_CONNECTION  0x80000000
#تعریف_کن NGX_LOG_DEBUG_ALL         0x7ffffff0


تعريفنوع u_char *(*ngx_log_handler_pt) (ngx_log_t *لگاریتم, u_char *buf, اندازه_t len);
تعريفنوع فراغ (*ngx_log_writer_pt) (ngx_log_t *لگاریتم, ngx_uint_t level,
    u_char *buf, اندازه_t len);


بنية ngx_log_s {
    ngx_uint_t           log_level;
    ngx_open_file_t     *file;

    ngx_atomic_uint_t    connection;

    زمان_t               disk_پر_زمان;

    ngx_log_handler_pt   handler;
    فراغ                *داده;

    ngx_log_writer_pt    writer;
    فراغ                *wdata;

    /*
     * we declare "action" as "char *" because the actions are usually
     * the static strings and in the "u_char *" case we have to override
     * their types all the time
     */

    حرف                *action;

    ngx_log_t           *بعدی;

    NGX_COMPAT_BEGIN(5)
    NGX_COMPAT_انتها
};


#تعریف_کن NGX_MAX_ERROR_STR   2048


/*********************************/

#إذا (NGX_HAVE_C99_VARIADIC_MACROS)

#تعریف_کن NGX_HAVE_VARIADIC_MACROS  1

#تعریف_کن ngx_log_error(level, لگاریتم, ...)                                        \
    إذا ((لگاریتم)->log_level >= level) ngx_log_error_core(level, لگاریتم, __VA_ARGS__)

فراغ ngx_log_error_core(ngx_uint_t level, ngx_log_t *لگاریتم, ngx_err_t err,
    ثابت حرف *fmt, ...);

#تعریف_کن ngx_log_debug(level, لگاریتم, ...)                                        \
    إذا ((لگاریتم)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, لگاریتم, __VA_ARGS__)

/*********************************/

#وإلاإذا (NGX_HAVE_GCC_VARIADIC_MACROS)

#تعریف_کن NGX_HAVE_VARIADIC_MACROS  1

#تعریف_کن ngx_log_error(level, لگاریتم, آرگومانها...)                                    \
    إذا ((لگاریتم)->log_level >= level) ngx_log_error_core(level, لگاریتم, آرگومانها)

فراغ ngx_log_error_core(ngx_uint_t level, ngx_log_t *لگاریتم, ngx_err_t err,
    ثابت حرف *fmt, ...);

#تعریف_کن ngx_log_debug(level, لگاریتم, آرگومانها...)                                    \
    إذا ((لگاریتم)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, لگاریتم, آرگومانها)

/*********************************/

#وإلا /* no variadic macros */

#تعریف_کن NGX_HAVE_VARIADIC_MACROS  0

فراغ ngx_cdecl ngx_log_error(ngx_uint_t level, ngx_log_t *لگاریتم, ngx_err_t err,
    ثابت حرف *fmt, ...);
فراغ ngx_log_error_core(ngx_uint_t level, ngx_log_t *لگاریتم, ngx_err_t err,
    ثابت حرف *fmt, va_list آرگومانها);
فراغ ngx_cdecl ngx_log_debug_core(ngx_log_t *لگاریتم, ngx_err_t err,
    ثابت حرف *fmt, ...);


#پایان_اگر /* variadic macros */


/*********************************/

#إذا (NGX_DEBUG)

#إذا (NGX_HAVE_VARIADIC_MACROS)

#تعریف_کن ngx_log_debug0(level, لگاریتم, err, fmt)                                  \
        ngx_log_debug(level, لگاریتم, err, fmt)

#تعریف_کن ngx_log_debug1(level, لگاریتم, err, fmt, arg1)                            \
        ngx_log_debug(level, لگاریتم, err, fmt, arg1)

#تعریف_کن ngx_log_debug2(level, لگاریتم, err, fmt, arg1, arg2)                      \
        ngx_log_debug(level, لگاریتم, err, fmt, arg1, arg2)

#تعریف_کن ngx_log_debug3(level, لگاریتم, err, fmt, arg1, arg2, arg3)                \
        ngx_log_debug(level, لگاریتم, err, fmt, arg1, arg2, arg3)

#تعریف_کن ngx_log_debug4(level, لگاریتم, err, fmt, arg1, arg2, arg3, arg4)          \
        ngx_log_debug(level, لگاریتم, err, fmt, arg1, arg2, arg3, arg4)

#تعریف_کن ngx_log_debug5(level, لگاریتم, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
        ngx_log_debug(level, لگاریتم, err, fmt, arg1, arg2, arg3, arg4, arg5)

#تعریف_کن ngx_log_debug6(level, لگاریتم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
        ngx_log_debug(level, لگاریتم, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6)

#تعریف_کن ngx_log_debug7(level, لگاریتم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
        ngx_log_debug(level, لگاریتم, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#تعریف_کن ngx_log_debug8(level, لگاریتم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
        ngx_log_debug(level, لگاریتم, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)


#وإلا /* no variadic macros */

#تعریف_کن ngx_log_debug0(level, لگاریتم, err, fmt)                                  \
    إذا ((لگاریتم)->log_level & level)                                             \
        ngx_log_debug_core(لگاریتم, err, fmt)

#تعریف_کن ngx_log_debug1(level, لگاریتم, err, fmt, arg1)                            \
    إذا ((لگاریتم)->log_level & level)                                             \
        ngx_log_debug_core(لگاریتم, err, fmt, arg1)

#تعریف_کن ngx_log_debug2(level, لگاریتم, err, fmt, arg1, arg2)                      \
    إذا ((لگاریتم)->log_level & level)                                             \
        ngx_log_debug_core(لگاریتم, err, fmt, arg1, arg2)

#تعریف_کن ngx_log_debug3(level, لگاریتم, err, fmt, arg1, arg2, arg3)                \
    إذا ((لگاریتم)->log_level & level)                                             \
        ngx_log_debug_core(لگاریتم, err, fmt, arg1, arg2, arg3)

#تعریف_کن ngx_log_debug4(level, لگاریتم, err, fmt, arg1, arg2, arg3, arg4)          \
    إذا ((لگاریتم)->log_level & level)                                             \
        ngx_log_debug_core(لگاریتم, err, fmt, arg1, arg2, arg3, arg4)

#تعریف_کن ngx_log_debug5(level, لگاریتم, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
    إذا ((لگاریتم)->log_level & level)                                             \
        ngx_log_debug_core(لگاریتم, err, fmt, arg1, arg2, arg3, arg4, arg5)

#تعریف_کن ngx_log_debug6(level, لگاریتم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
    إذا ((لگاریتم)->log_level & level)                                             \
        ngx_log_debug_core(لگاریتم, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#تعریف_کن ngx_log_debug7(level, لگاریتم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
    إذا ((لگاریتم)->log_level & level)                                             \
        ngx_log_debug_core(لگاریتم, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#تعریف_کن ngx_log_debug8(level, لگاریتم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
    إذا ((لگاریتم)->log_level & level)                                             \
        ngx_log_debug_core(لگاریتم, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#پایان_اگر

#وإلا /* !NGX_DEBUG */

#تعریف_کن ngx_log_debug0(level, لگاریتم, err, fmt)
#تعریف_کن ngx_log_debug1(level, لگاریتم, err, fmt, arg1)
#تعریف_کن ngx_log_debug2(level, لگاریتم, err, fmt, arg1, arg2)
#تعریف_کن ngx_log_debug3(level, لگاریتم, err, fmt, arg1, arg2, arg3)
#تعریف_کن ngx_log_debug4(level, لگاریتم, err, fmt, arg1, arg2, arg3, arg4)
#تعریف_کن ngx_log_debug5(level, لگاریتم, err, fmt, arg1, arg2, arg3, arg4, arg5)
#تعریف_کن ngx_log_debug6(level, لگاریتم, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#تعریف_کن ngx_log_debug7(level, لگاریتم, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7)
#تعریف_کن ngx_log_debug8(level, لگاریتم, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7, arg8)

#پایان_اگر

/*********************************/

ngx_log_t *ngx_log_init(u_char *پیشوند, u_char *error_log);
فراغ ngx_cdecl ngx_log_abort(ngx_err_t err, ثابت حرف *fmt, ...);
فراغ ngx_cdecl ngx_log_stderr(ngx_err_t err, ثابت حرف *fmt, ...);
u_char *ngx_log_errno(u_char *buf, u_char *آخرین, ngx_err_t err);
ngx_int_t ngx_log_open_default(ngx_چرخه_t *چرخه);
ngx_int_t ngx_log_redirect_stderr(ngx_چرخه_t *چرخه);
ngx_log_t *ngx_log_get_file_log(ngx_log_t *سر);
حرف *ngx_log_مجموعه_log(ngx_conf_t *cf, ngx_log_t **سر);


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


#پایان_اگر /* _NGX_LOG_H_INCLUDED_ */
