
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_LOG_H_INCLUDED_
#నిర్వచించు _NGX_LOG_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


#నిర్వచించు NGX_LOG_STDERR            0
#నిర్వచించు NGX_LOG_EMERG             1
#నిర్వచించు NGX_LOG_ALERT             2
#నిర్వచించు NGX_LOG_CRIT              3
#నిర్వచించు NGX_LOG_ERR               4
#నిర్వచించు NGX_LOG_WARN              5
#నిర్వచించు NGX_LOG_NOTICE            6
#నిర్వచించు NGX_LOG_INFO              7
#నిర్వచించు NGX_LOG_DEBUG             8

#నిర్వచించు NGX_LOG_DEBUG_CORE        0x010
#నిర్వచించు NGX_LOG_DEBUG_ALLOC       0x020
#నిర్వచించు NGX_LOG_DEBUG_MUTEX       0x040
#నిర్వచించు NGX_LOG_DEBUG_EVENT       0x080
#నిర్వచించు NGX_LOG_DEBUG_HTTP        0x100
#నిర్వచించు NGX_LOG_DEBUG_MAIL        0x200
#నిర్వచించు NGX_LOG_DEBUG_ప్రవాహం      0x400

/*
 * do not forget to update debug_levels[] in src/core/ngx_log.c
 * after the adding a new debug level
 */

#నిర్వచించు NGX_LOG_DEBUG_మొదటి       NGX_LOG_DEBUG_CORE
#నిర్వచించు NGX_LOG_DEBUG_చివరి        NGX_LOG_DEBUG_ప్రవాహం
#నిర్వచించు NGX_LOG_DEBUG_CONNECTION  0x80000000
#నిర్వచించు NGX_LOG_DEBUG_ALL         0x7ffffff0


రకనిర్వచనం u_char *(*ngx_log_handler_pt) (ngx_log_t *సంవర్గమానం, u_char *buf, కొలత_t len);
రకనిర్వచనం శూన్య (*ngx_log_writer_pt) (ngx_log_t *సంవర్గమానం, ngx_uint_t level,
    u_char *buf, కొలత_t len);


నిర్మాణం ngx_log_s {
    ngx_uint_t           log_level;
    ngx_open_file_t     *file;

    ngx_atomic_uint_t    connection;

    సమయం_t               disk_నిండిన_సమయం;

    ngx_log_handler_pt   handler;
    శూన్య                *దత్తాంశం;

    ngx_log_writer_pt    writer;
    శూన్య                *wdata;

    /*
     * we declare "action" as "char *" because the actions are usually
     * the static strings and in the "u_char *" case we have to override
     * their types all the time
     */

    అక్షరం                *action;

    ngx_log_t           *తదుపరి;

    NGX_COMPAT_BEGIN(5)
    NGX_COMPAT_చివర
};


#నిర్వచించు NGX_MAX_ERROR_STR   2048


/*********************************/

#ఒకవేళ (NGX_HAVE_C99_VARIADIC_MACROS)

#నిర్వచించు NGX_HAVE_VARIADIC_MACROS  1

#నిర్వచించు ngx_log_error(level, సంవర్గమానం, ...)                                        \
    ఒకవేళ ((సంవర్గమానం)->log_level >= level) ngx_log_error_core(level, సంవర్గమానం, __VA_ARGS__)

శూన్య ngx_log_error_core(ngx_uint_t level, ngx_log_t *సంవర్గమానం, ngx_err_t err,
    స్థిరరాశి అక్షరం *fmt, ...);

#నిర్వచించు ngx_log_debug(level, సంవర్గమానం, ...)                                        \
    ఒకవేళ ((సంవర్గమానం)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, సంవర్గమానం, __VA_ARGS__)

/*********************************/

#లేదావేళ (NGX_HAVE_GCC_VARIADIC_MACROS)

#నిర్వచించు NGX_HAVE_VARIADIC_MACROS  1

#నిర్వచించు ngx_log_error(level, సంవర్గమానం, వాదనలు...)                                    \
    ఒకవేళ ((సంవర్గమానం)->log_level >= level) ngx_log_error_core(level, సంవర్గమానం, వాదనలు)

శూన్య ngx_log_error_core(ngx_uint_t level, ngx_log_t *సంవర్గమానం, ngx_err_t err,
    స్థిరరాశి అక్షరం *fmt, ...);

#నిర్వచించు ngx_log_debug(level, సంవర్గమానం, వాదనలు...)                                    \
    ఒకవేళ ((సంవర్గమానం)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, సంవర్గమానం, వాదనలు)

/*********************************/

#లేకపోతే /* no variadic macros */

#నిర్వచించు NGX_HAVE_VARIADIC_MACROS  0

శూన్య ngx_cdecl ngx_log_error(ngx_uint_t level, ngx_log_t *సంవర్గమానం, ngx_err_t err,
    స్థిరరాశి అక్షరం *fmt, ...);
శూన్య ngx_log_error_core(ngx_uint_t level, ngx_log_t *సంవర్గమానం, ngx_err_t err,
    స్థిరరాశి అక్షరం *fmt, va_list వాదనలు);
శూన్య ngx_cdecl ngx_log_debug_core(ngx_log_t *సంవర్గమానం, ngx_err_t err,
    స్థిరరాశి అక్షరం *fmt, ...);


#అయితే_ముగింపు /* variadic macros */


/*********************************/

#ఒకవేళ (NGX_DEBUG)

#ఒకవేళ (NGX_HAVE_VARIADIC_MACROS)

#నిర్వచించు ngx_log_debug0(level, సంవర్గమానం, err, fmt)                                  \
        ngx_log_debug(level, సంవర్గమానం, err, fmt)

#నిర్వచించు ngx_log_debug1(level, సంవర్గమానం, err, fmt, arg1)                            \
        ngx_log_debug(level, సంవర్గమానం, err, fmt, arg1)

#నిర్వచించు ngx_log_debug2(level, సంవర్గమానం, err, fmt, arg1, arg2)                      \
        ngx_log_debug(level, సంవర్గమానం, err, fmt, arg1, arg2)

#నిర్వచించు ngx_log_debug3(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3)                \
        ngx_log_debug(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3)

#నిర్వచించు ngx_log_debug4(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4)          \
        ngx_log_debug(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4)

#నిర్వచించు ngx_log_debug5(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
        ngx_log_debug(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4, arg5)

#నిర్వచించు ngx_log_debug6(level, సంవర్గమానం, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
        ngx_log_debug(level, సంవర్గమానం, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6)

#నిర్వచించు ngx_log_debug7(level, సంవర్గమానం, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
        ngx_log_debug(level, సంవర్గమానం, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#నిర్వచించు ngx_log_debug8(level, సంవర్గమానం, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
        ngx_log_debug(level, సంవర్గమానం, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)


#లేకపోతే /* no variadic macros */

#నిర్వచించు ngx_log_debug0(level, సంవర్గమానం, err, fmt)                                  \
    ఒకవేళ ((సంవర్గమానం)->log_level & level)                                             \
        ngx_log_debug_core(సంవర్గమానం, err, fmt)

#నిర్వచించు ngx_log_debug1(level, సంవర్గమానం, err, fmt, arg1)                            \
    ఒకవేళ ((సంవర్గమానం)->log_level & level)                                             \
        ngx_log_debug_core(సంవర్గమానం, err, fmt, arg1)

#నిర్వచించు ngx_log_debug2(level, సంవర్గమానం, err, fmt, arg1, arg2)                      \
    ఒకవేళ ((సంవర్గమానం)->log_level & level)                                             \
        ngx_log_debug_core(సంవర్గమానం, err, fmt, arg1, arg2)

#నిర్వచించు ngx_log_debug3(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3)                \
    ఒకవేళ ((సంవర్గమానం)->log_level & level)                                             \
        ngx_log_debug_core(సంవర్గమానం, err, fmt, arg1, arg2, arg3)

#నిర్వచించు ngx_log_debug4(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4)          \
    ఒకవేళ ((సంవర్గమానం)->log_level & level)                                             \
        ngx_log_debug_core(సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4)

#నిర్వచించు ngx_log_debug5(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
    ఒకవేళ ((సంవర్గమానం)->log_level & level)                                             \
        ngx_log_debug_core(సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4, arg5)

#నిర్వచించు ngx_log_debug6(level, సంవర్గమానం, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
    ఒకవేళ ((సంవర్గమానం)->log_level & level)                                             \
        ngx_log_debug_core(సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#నిర్వచించు ngx_log_debug7(level, సంవర్గమానం, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
    ఒకవేళ ((సంవర్గమానం)->log_level & level)                                             \
        ngx_log_debug_core(సంవర్గమానం, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#నిర్వచించు ngx_log_debug8(level, సంవర్గమానం, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
    ఒకవేళ ((సంవర్గమానం)->log_level & level)                                             \
        ngx_log_debug_core(సంవర్గమానం, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#అయితే_ముగింపు

#లేకపోతే /* !NGX_DEBUG */

#నిర్వచించు ngx_log_debug0(level, సంవర్గమానం, err, fmt)
#నిర్వచించు ngx_log_debug1(level, సంవర్గమానం, err, fmt, arg1)
#నిర్వచించు ngx_log_debug2(level, సంవర్గమానం, err, fmt, arg1, arg2)
#నిర్వచించు ngx_log_debug3(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3)
#నిర్వచించు ngx_log_debug4(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4)
#నిర్వచించు ngx_log_debug5(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4, arg5)
#నిర్వచించు ngx_log_debug6(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#నిర్వచించు ngx_log_debug7(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7)
#నిర్వచించు ngx_log_debug8(level, సంవర్గమానం, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7, arg8)

#అయితే_ముగింపు

/*********************************/

ngx_log_t *ngx_log_init(u_char *ఉపసర్గ, u_char *error_log);
శూన్య ngx_cdecl ngx_log_abort(ngx_err_t err, స్థిరరాశి అక్షరం *fmt, ...);
శూన్య ngx_cdecl ngx_log_stderr(ngx_err_t err, స్థిరరాశి అక్షరం *fmt, ...);
u_char *ngx_log_errno(u_char *buf, u_char *చివరి, ngx_err_t err);
ngx_int_t ngx_log_open_default(ngx_చక్రం_t *చక్రం);
ngx_int_t ngx_log_redirect_stderr(ngx_చక్రం_t *చక్రం);
ngx_log_t *ngx_log_get_file_log(ngx_log_t *తల);
అక్షరం *ngx_log_సమితి_log(ngx_conf_t *cf, ngx_log_t **తల);


/*
 * ngx_write_stderr() cannot be implemented as macro, since
 * MSVC does not allow to use #ifdef inside macro parameters.
 *
 * ngx_write_fd() is used instead of ngx_write_console(), since
 * CharToOemBuff() inside ngx_write_console() cannot be used with
 * read only buffer as destination and CharToOemBuff() is not needed
 * for ngx_write_stderr() anyway.
 */
స్థిర ngx_inline శూన్య
ngx_write_stderr(అక్షరం *పాఠ్యం)
{
    (శూన్య) ngx_write_fd(ngx_stderr, పాఠ్యం, ngx_strlen(పాఠ్యం));
}


స్థిర ngx_inline శూన్య
ngx_write_stdout(అక్షరం *పాఠ్యం)
{
    (శూన్య) ngx_write_fd(ngx_stdout, పాఠ్యం, ngx_strlen(పాఠ్యం));
}


బాహ్య ngx_module_t  ngx_errlog_module;
బాహ్య ngx_uint_t    ngx_use_stderr;


#అయితే_ముగింపు /* _NGX_LOG_H_INCLUDED_ */
