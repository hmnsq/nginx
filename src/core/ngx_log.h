
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_LOG_H_INCLUDED_
#घोषणा _NGX_LOG_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


#घोषणा NGX_LOG_STDERR            0
#घोषणा NGX_LOG_EMERG             1
#घोषणा NGX_LOG_ALERT             2
#घोषणा NGX_LOG_CRIT              3
#घोषणा NGX_LOG_ERR               4
#घोषणा NGX_LOG_WARN              5
#घोषणा NGX_LOG_NOTICE            6
#घोषणा NGX_LOG_INFO              7
#घोषणा NGX_LOG_DEBUG             8

#घोषणा NGX_LOG_DEBUG_CORE        0x010
#घोषणा NGX_LOG_DEBUG_ALLOC       0x020
#घोषणा NGX_LOG_DEBUG_MUTEX       0x040
#घोषणा NGX_LOG_DEBUG_EVENT       0x080
#घोषणा NGX_LOG_DEBUG_HTTP        0x100
#घोषणा NGX_LOG_DEBUG_MAIL        0x200
#घोषणा NGX_LOG_DEBUG_STREAM      0x400

/*
 * do not forget to update debug_levels[] in src/core/ngx_log.c
 * after the adding a new debug level
 */

#घोषणा NGX_LOG_DEBUG_FIRST       NGX_LOG_DEBUG_CORE
#घोषणा NGX_LOG_DEBUG_LAST        NGX_LOG_DEBUG_STREAM
#घोषणा NGX_LOG_DEBUG_CONNECTION  0x80000000
#घोषणा NGX_LOG_DEBUG_ALL         0x7ffffff0


प्रकार u_char *(*ngx_log_handler_pt) (ngx_log_t *log, u_char *buf, माप_प्रकार len);
प्रकार व्योम (*ngx_log_writer_pt) (ngx_log_t *log, ngx_uint_t level,
    u_char *buf, माप_प्रकार len);


काष्ठा ngx_log_s {
    ngx_uint_t           log_level;
    ngx_open_file_t     *file;

    ngx_atomic_uint_t    connection;

    समय_प्रकार               disk_full_time;

    ngx_log_handler_pt   handler;
    व्योम                *तथ्य;

    ngx_log_writer_pt    writer;
    व्योम                *wdata;

    /*
     * we declare "action" as "char *" because the actions are usually
     * the static strings and in the "u_char *" case we have to override
     * their types all the time
     */

    अक्षर                *action;

    ngx_log_t           *अग्र;

    NGX_COMPAT_BEGIN(5)
    NGX_COMPAT_END
};


#घोषणा NGX_MAX_ERROR_STR   2048


/*********************************/

#अगर (NGX_HAVE_C99_VARIADIC_MACROS)

#घोषणा NGX_HAVE_VARIADIC_MACROS  1

#घोषणा ngx_log_error(level, log, ...)                                        \
    अगर ((log)->log_level >= level) ngx_log_error_core(level, log, __VA_ARGS__)

व्योम ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    स्थिर अक्षर *fmt, ...);

#घोषणा ngx_log_debug(level, log, ...)                                        \
    अगर ((log)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, log, __VA_ARGS__)

/*********************************/

#या_अगर (NGX_HAVE_GCC_VARIADIC_MACROS)

#घोषणा NGX_HAVE_VARIADIC_MACROS  1

#घोषणा ngx_log_error(level, log, args...)                                    \
    अगर ((log)->log_level >= level) ngx_log_error_core(level, log, args)

व्योम ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    स्थिर अक्षर *fmt, ...);

#घोषणा ngx_log_debug(level, log, args...)                                    \
    अगर ((log)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, log, args)

/*********************************/

#अन्यथा /* no variadic macros */

#घोषणा NGX_HAVE_VARIADIC_MACROS  0

व्योम ngx_cdecl ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    स्थिर अक्षर *fmt, ...);
व्योम ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    स्थिर अक्षर *fmt, बहु_सूची args);
व्योम ngx_cdecl ngx_log_debug_core(ngx_log_t *log, ngx_err_t err,
    स्थिर अक्षर *fmt, ...);


#पूर्ण_अगर /* variadic macros */


/*********************************/

#अगर (NGX_DEBUG)

#अगर (NGX_HAVE_VARIADIC_MACROS)

#घोषणा ngx_log_debug0(level, log, err, fmt)                                  \
        ngx_log_debug(level, log, err, fmt)

#घोषणा ngx_log_debug1(level, log, err, fmt, arg1)                            \
        ngx_log_debug(level, log, err, fmt, arg1)

#घोषणा ngx_log_debug2(level, log, err, fmt, arg1, arg2)                      \
        ngx_log_debug(level, log, err, fmt, arg1, arg2)

#घोषणा ngx_log_debug3(level, log, err, fmt, arg1, arg2, arg3)                \
        ngx_log_debug(level, log, err, fmt, arg1, arg2, arg3)

#घोषणा ngx_log_debug4(level, log, err, fmt, arg1, arg2, arg3, arg4)          \
        ngx_log_debug(level, log, err, fmt, arg1, arg2, arg3, arg4)

#घोषणा ngx_log_debug5(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
        ngx_log_debug(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5)

#घोषणा ngx_log_debug6(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
        ngx_log_debug(level, log, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6)

#घोषणा ngx_log_debug7(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
        ngx_log_debug(level, log, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#घोषणा ngx_log_debug8(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
        ngx_log_debug(level, log, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)


#अन्यथा /* no variadic macros */

#घोषणा ngx_log_debug0(level, log, err, fmt)                                  \
    अगर ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt)

#घोषणा ngx_log_debug1(level, log, err, fmt, arg1)                            \
    अगर ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt, arg1)

#घोषणा ngx_log_debug2(level, log, err, fmt, arg1, arg2)                      \
    अगर ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt, arg1, arg2)

#घोषणा ngx_log_debug3(level, log, err, fmt, arg1, arg2, arg3)                \
    अगर ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt, arg1, arg2, arg3)

#घोषणा ngx_log_debug4(level, log, err, fmt, arg1, arg2, arg3, arg4)          \
    अगर ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt, arg1, arg2, arg3, arg4)

#घोषणा ngx_log_debug5(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
    अगर ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt, arg1, arg2, arg3, arg4, arg5)

#घोषणा ngx_log_debug6(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
    अगर ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#घोषणा ngx_log_debug7(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
    अगर ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#घोषणा ngx_log_debug8(level, log, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
    अगर ((log)->log_level & level)                                             \
        ngx_log_debug_core(log, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#पूर्ण_अगर

#अन्यथा /* !NGX_DEBUG */

#घोषणा ngx_log_debug0(level, log, err, fmt)
#घोषणा ngx_log_debug1(level, log, err, fmt, arg1)
#घोषणा ngx_log_debug2(level, log, err, fmt, arg1, arg2)
#घोषणा ngx_log_debug3(level, log, err, fmt, arg1, arg2, arg3)
#घोषणा ngx_log_debug4(level, log, err, fmt, arg1, arg2, arg3, arg4)
#घोषणा ngx_log_debug5(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5)
#घोषणा ngx_log_debug6(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#घोषणा ngx_log_debug7(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7)
#घोषणा ngx_log_debug8(level, log, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7, arg8)

#पूर्ण_अगर

/*********************************/

ngx_log_t *ngx_log_init(u_char *prefix, u_char *error_log);
व्योम ngx_cdecl ngx_log_abort(ngx_err_t err, स्थिर अक्षर *fmt, ...);
व्योम ngx_cdecl ngx_log_stderr(ngx_err_t err, स्थिर अक्षर *fmt, ...);
u_char *ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err);
ngx_int_t ngx_log_open_default(ngx_cycle_t *cycle);
ngx_int_t ngx_log_redirect_stderr(ngx_cycle_t *cycle);
ngx_log_t *ngx_log_get_file_log(ngx_log_t *शीर्ष);
अक्षर *ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **शीर्ष);


/*
 * ngx_write_stderr() cannot be implemented as macro, since
 * MSVC does not allow to use #ifdef inside macro parameters.
 *
 * ngx_write_fd() is used instead of ngx_write_console(), since
 * CharToOemBuff() inside ngx_write_console() cannot be used with
 * read only buffer as destination and CharToOemBuff() is not needed
 * for ngx_write_stderr() anyway.
 */
अटल ngx_inline व्योम
ngx_write_stderr(अक्षर *text)
{
    (व्योम) ngx_write_fd(ngx_stderr, text, ngx_strlen(text));
}


अटल ngx_inline व्योम
ngx_write_stdout(अक्षर *text)
{
    (व्योम) ngx_write_fd(ngx_stdout, text, ngx_strlen(text));
}


बाह्य ngx_module_t  ngx_errlog_module;
बाह्य ngx_uint_t    ngx_use_stderr;


#पूर्ण_अगर /* _NGX_LOG_H_INCLUDED_ */
