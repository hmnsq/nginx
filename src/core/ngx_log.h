
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_LOG_H_INCLUDED_
#تعریف_کرو _NGX_LOG_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


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


قسمتعریف u_char *(*ngx_log_handler_pt) (ngx_log_t *لوگارتھم, u_char *buf, سائز_t len);
قسمتعریف خالی (*ngx_log_writer_pt) (ngx_log_t *لوگارتھم, ngx_uint_t level,
    u_char *buf, سائز_t len);


ساخت ngx_log_s {
    ngx_uint_t           log_level;
    ngx_open_file_t     *file;

    ngx_atomic_uint_t    connection;

    وقت_t               disk_بھرا_وقت;

    ngx_log_handler_pt   handler;
    خالی                *ڈیٹا;

    ngx_log_writer_pt    writer;
    خالی                *wdata;

    /*
     * we declare "action" as "char *" because the actions are usually
     * the static strings and in the "u_char *" case we have to override
     * their types all the time
     */

    حرف                *action;

    ngx_log_t           *اگلا;

    NGX_COMPAT_BEGIN(5)
    NGX_COMPAT_انتہا
};


#تعریف_کرو NGX_MAX_ERROR_STR   2048


/*********************************/

#اگر (NGX_HAVE_C99_VARIADIC_MACROS)

#تعریف_کرو NGX_HAVE_VARIADIC_MACROS  1

#تعریف_کرو ngx_log_error(level, لوگارتھم, ...)                                        \
    اگر ((لوگارتھم)->log_level >= level) ngx_log_error_core(level, لوگارتھم, __VA_ARGS__)

خالی ngx_log_error_core(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, ...);

#تعریف_کرو ngx_log_debug(level, لوگارتھم, ...)                                        \
    اگر ((لوگارتھم)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, لوگارتھم, __VA_ARGS__)

/*********************************/

#وگرنہ_اگر (NGX_HAVE_GCC_VARIADIC_MACROS)

#تعریف_کرو NGX_HAVE_VARIADIC_MACROS  1

#تعریف_کرو ngx_log_error(level, لوگارتھم, دلائل...)                                    \
    اگر ((لوگارتھم)->log_level >= level) ngx_log_error_core(level, لوگارتھم, دلائل)

خالی ngx_log_error_core(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, ...);

#تعریف_کرو ngx_log_debug(level, لوگارتھم, دلائل...)                                    \
    اگر ((لوگارتھم)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, لوگارتھم, دلائل)

/*********************************/

#ورنہ /* no variadic macros */

#تعریف_کرو NGX_HAVE_VARIADIC_MACROS  0

خالی ngx_cdecl ngx_log_error(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, ...);
خالی ngx_log_error_core(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, va_list دلائل);
خالی ngx_cdecl ngx_log_debug_core(ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, ...);


#اگر_ختم /* variadic macros */


/*********************************/

#اگر (NGX_DEBUG)

#اگر (NGX_HAVE_VARIADIC_MACROS)

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


#ورنہ /* no variadic macros */

#تعریف_کرو ngx_log_debug0(level, لوگارتھم, err, fmt)                                  \
    اگر ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt)

#تعریف_کرو ngx_log_debug1(level, لوگارتھم, err, fmt, arg1)                            \
    اگر ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt, arg1)

#تعریف_کرو ngx_log_debug2(level, لوگارتھم, err, fmt, arg1, arg2)                      \
    اگر ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt, arg1, arg2)

#تعریف_کرو ngx_log_debug3(level, لوگارتھم, err, fmt, arg1, arg2, arg3)                \
    اگر ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt, arg1, arg2, arg3)

#تعریف_کرو ngx_log_debug4(level, لوگارتھم, err, fmt, arg1, arg2, arg3, arg4)          \
    اگر ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt, arg1, arg2, arg3, arg4)

#تعریف_کرو ngx_log_debug5(level, لوگارتھم, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
    اگر ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt, arg1, arg2, arg3, arg4, arg5)

#تعریف_کرو ngx_log_debug6(level, لوگارتھم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
    اگر ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#تعریف_کرو ngx_log_debug7(level, لوگارتھم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
    اگر ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#تعریف_کرو ngx_log_debug8(level, لوگارتھم, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
    اگر ((لوگارتھم)->log_level & level)                                             \
        ngx_log_debug_core(لوگارتھم, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#اگر_ختم

#ورنہ /* !NGX_DEBUG */

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

#اگر_ختم

/*********************************/

ngx_log_t *ngx_log_init(u_char *سابقہ, u_char *error_log);
خالی ngx_cdecl ngx_log_abort(ngx_err_t err, ثابت حرف *fmt, ...);
خالی ngx_cdecl ngx_log_stderr(ngx_err_t err, ثابت حرف *fmt, ...);
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
ساکن ngx_inline خالی
ngx_write_stderr(حرف *متن)
{
    (خالی) ngx_write_fd(ngx_stderr, متن, ngx_strlen(متن));
}


ساکن ngx_inline خالی
ngx_write_stdout(حرف *متن)
{
    (خالی) ngx_write_fd(ngx_stdout, متن, ngx_strlen(متن));
}


خارجی ngx_module_t  ngx_errlog_module;
خارجی ngx_uint_t    ngx_use_stderr;


#اگر_ختم /* _NGX_LOG_H_INCLUDED_ */
