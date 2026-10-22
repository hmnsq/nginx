
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_LOG_H_INCLUDED_
#تعریف_کرو _NGX_LOG_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


#تعریف_کرو NGX_LOG_STDERR            0
#تعریف_کرو NGX_LOG_EMERG             1
#تعریف_کرو NGX_LOG_ALERT             2
#تعریف_کرو NGX_LOG_CRIT              3
#تعریف_کرو NGX_LOG_ERR               4
#تعریف_کرو NGX_LOG_WARN              5
#تعریف_کرو NGX_LOG_NOTICE            6
#تعریف_کرو NGX_LOG_INFO              7
#تعریف_کرو NGX_LOG_DEBUG             8

#تعریف_کرو NGX_LOG_DEBUG_CORE        0x010
#تعریف_کرو NGX_LOG_DEBUG_ALLOC       0x020
#تعریف_کرو NGX_LOG_DEBUG_MUTEX       0x040
#تعریف_کرو NGX_LOG_DEBUG_EVENT       0x080
#تعریف_کرو NGX_LOG_DEBUG_HTTP        0x100
#تعریف_کرو NGX_LOG_DEBUG_MAIL        0x200
#تعریف_کرو NGX_LOG_DEBUG_دھارا      0x400

/*
 * do not forget to update debug_levels[] in src/core/ngx_log.c
 * after the adding a new debug level
 */

#تعریف_کرو NGX_LOG_DEBUG_پہلا       NGX_LOG_DEBUG_CORE
#تعریف_کرو NGX_LOG_DEBUG_آخری        NGX_LOG_DEBUG_دھارا
#تعریف_کرو NGX_LOG_DEBUG_CONNECTION  0x80000000
#تعریف_کرو NGX_LOG_DEBUG_ALL         0x7ffffff0


تعريفنوع u_char *(*ngx_log_handler_pt) (ngx_log_t *لوگارتھم, u_char *buf, ناپ_t len);
تعريفنوع فراغ (*ngx_log_writer_pt) (ngx_log_t *لوگارتھم, ngx_uint_t level,
    u_char *buf, ناپ_t len);


بنية ngx_log_s {
    ngx_uint_t           log_level;
    ngx_open_file_t     *file;

    ngx_atomic_uint_t    connection;

    ویلا_t               disk_بھریا_ویلا;

    ngx_log_handler_pt   handler;
    فراغ                *انکڑے;

    ngx_log_writer_pt    writer;
    فراغ                *wdata;

    /*
     * we declare "action" as "char *" because the actions are usually
     * the static strings and in the "u_char *" case we have to override
     * their types all the time
     */

    حرف                *action;

    ngx_log_t           *اگلا;

    NGX_COMPAT_BEGIN(5)
    NGX_COMPAT_سرا
};


#تعریف_کرو NGX_MAX_ERROR_STR   2048


/*********************************/

#إذا (NGX_HAVE_C99_VARIADIC_MACROS)

#تعریف_کرو NGX_HAVE_VARIADIC_MACROS  1

#تعریف_کرو ngx_log_error(level, لوگارتھم, ...)                                        \
    إذا ((لوگارتھم)->log_level >= level) ngx_log_error_core(level, لوگارتھم, __VA_ARGS__)

فراغ ngx_log_error_core(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, ...);

#تعریف_کرو ngx_log_debug(level, لوگارتھم, ...)                                        \
    إذا ((لوگارتھم)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, لوگارتھم, __VA_ARGS__)

/*********************************/

#وإلاإذا (NGX_HAVE_GCC_VARIADIC_MACROS)

#تعریف_کرو NGX_HAVE_VARIADIC_MACROS  1

#تعریف_کرو ngx_log_error(level, لوگارتھم, دلیلاں...)                                    \
    إذا ((لوگارتھم)->log_level >= level) ngx_log_error_core(level, لوگارتھم, دلیلاں)

فراغ ngx_log_error_core(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, ...);

#تعریف_کرو ngx_log_debug(level, لوگارتھم, دلیلاں...)                                    \
    إذا ((لوگارتھم)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, لوگارتھم, دلیلاں)

/*********************************/

#وإلا /* no variadic macros */

#تعریف_کرو NGX_HAVE_VARIADIC_MACROS  0

فراغ ngx_cdecl ngx_log_error(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, ...);
فراغ ngx_log_error_core(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, va_list دلیلاں);
فراغ ngx_cdecl ngx_log_debug_core(ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, ...);


#جے_انت /* variadic macros */


/*********************************/

#إذا (NGX_DEBUG)

#إذا (NGX_HAVE_VARIADIC_MACROS)

#تعریف_کرو ngx_log_debug0(level, لوگارتھم, err, fmt)                                  \
        ngx_log_debug(level, لوگارتھم, err, fmt)

#تعریف_کرو ngx_log_debug1(level, لوگارتھم, err, fmt, arg1)                            \
        ngx_log_debug(level, لوگارتھم, err, fmt, arg1)

#تعریف_کرو ngx_log_debug2(level, لوگارتھم, err, fmt, arg1, arg2)                      \
        ngx_log_debug(level, لوگارتھم, err, fmt, arg1, arg2)

#تعریف_کرو ngx_log_debug3(level, لوگارتھم, err, fmt, arg1, arg2, arg3)                \
        ngx_log_debug(level, لوگارتھم, err, fmt, arg1, arg2, arg3)

#تعریف_کرو ngx_log_debug4(level, لوگارتھم, err, fmt, arg1, arg2, arg3, arg4)          \
        ngx_log_debug(level, لوگارتھم, err, fmt, arg1, arg2, arg3, arg4)

#تعریف_کرو ngx_log_debug5(level, لوگارتھم, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
        ngx_log_debug(level, لوگارتھم, err, fmt, arg1, arg2, arg3, arg4, arg5)

#تعریف_کرو ngx_log_debug6(level, لوگارتھم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
        ngx_log_debug(level, لوگارتھم, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6)

#تعریف_کرو ngx_log_debug7(level, لوگارتھم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
        ngx_log_debug(level, لوگارتھم, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#تعریف_کرو ngx_log_debug8(level, لوگارتھم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
        ngx_log_debug(level, لوگارتھم, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)


#وإلا /* no variadic macros */

#تعریف_کرو ngx_log_debug0(level, لوگارتھم, err, fmt)                                  \
    إذا ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt)

#تعریف_کرو ngx_log_debug1(level, لوگارتھم, err, fmt, arg1)                            \
    إذا ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt, arg1)

#تعریف_کرو ngx_log_debug2(level, لوگارتھم, err, fmt, arg1, arg2)                      \
    إذا ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt, arg1, arg2)

#تعریف_کرو ngx_log_debug3(level, لوگارتھم, err, fmt, arg1, arg2, arg3)                \
    إذا ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt, arg1, arg2, arg3)

#تعریف_کرو ngx_log_debug4(level, لوگارتھم, err, fmt, arg1, arg2, arg3, arg4)          \
    إذا ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt, arg1, arg2, arg3, arg4)

#تعریف_کرو ngx_log_debug5(level, لوگارتھم, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
    إذا ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt, arg1, arg2, arg3, arg4, arg5)

#تعریف_کرو ngx_log_debug6(level, لوگارتھم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
    إذا ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#تعریف_کرو ngx_log_debug7(level, لوگارتھم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
    إذا ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#تعریف_کرو ngx_log_debug8(level, لوگارتھم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
    إذا ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#جے_انت

#وإلا /* !NGX_DEBUG */

#تعریف_کرو ngx_log_debug0(level, لوگارتھم, err, fmt)
#تعریف_کرو ngx_log_debug1(level, لوگارتھم, err, fmt, arg1)
#تعریف_کرو ngx_log_debug2(level, لوگارتھم, err, fmt, arg1, arg2)
#تعریف_کرو ngx_log_debug3(level, لوگارتھم, err, fmt, arg1, arg2, arg3)
#تعریف_کرو ngx_log_debug4(level, لوگارتھم, err, fmt, arg1, arg2, arg3, arg4)
#تعریف_کرو ngx_log_debug5(level, لوگارتھم, err, fmt, arg1, arg2, arg3, arg4, arg5)
#تعریف_کرو ngx_log_debug6(level, لوگارتھم, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#تعریف_کرو ngx_log_debug7(level, لوگارتھم, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7)
#تعریف_کرو ngx_log_debug8(level, لوگارتھم, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7, arg8)

#جے_انت

/*********************************/

ngx_log_t *ngx_log_init(u_char *اگیتر, u_char *error_log);
فراغ ngx_cdecl ngx_log_abort(ngx_err_t err, ثابت حرف *fmt, ...);
فراغ ngx_cdecl ngx_log_stderr(ngx_err_t err, ثابت حرف *fmt, ...);
u_char *ngx_log_errno(u_char *buf, u_char *آخری, ngx_err_t err);
ngx_int_t ngx_log_open_default(ngx_چکر_t *چکر);
ngx_int_t ngx_log_redirect_stderr(ngx_چکر_t *چکر);
ngx_log_t *ngx_log_get_file_log(ngx_log_t *سر);
حرف *ngx_log_سیٹ_log(ngx_conf_t *cf, ngx_log_t **سر);


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


#جے_انت /* _NGX_LOG_H_INCLUDED_ */
