
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_LOG_H_INCLUDED_
#परिभाषय _NGX_LOG_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


#परिभाषय NGX_LOG_STDERR            0
#परिभाषय NGX_LOG_EMERG             1
#परिभाषय NGX_LOG_ALERT             2
#परिभाषय NGX_LOG_CRIT              3
#परिभाषय NGX_LOG_ERR               4
#परिभाषय NGX_LOG_WARN              5
#परिभाषय NGX_LOG_NOTICE            6
#परिभाषय NGX_LOG_INFO              7
#परिभाषय NGX_LOG_DEBUG             8

#परिभाषय NGX_LOG_DEBUG_CORE        0x010
#परिभाषय NGX_LOG_DEBUG_ALLOC       0x020
#परिभाषय NGX_LOG_DEBUG_MUTEX       0x040
#परिभाषय NGX_LOG_DEBUG_EVENT       0x080
#परिभाषय NGX_LOG_DEBUG_HTTP        0x100
#परिभाषय NGX_LOG_DEBUG_MAIL        0x200
#परिभाषय NGX_LOG_DEBUG_प्रवाहः      0x400

/*
 * do not forget to update debug_levels[] in src/core/ngx_log.c
 * after the adding a new debug level
 */

#परिभाषय NGX_LOG_DEBUG_FIRST       NGX_LOG_DEBUG_CORE
#परिभाषय NGX_LOG_DEBUG_चरमः        NGX_LOG_DEBUG_प्रवाहः
#परिभाषय NGX_LOG_DEBUG_CONNECTION  0x80000000
#परिभाषय NGX_LOG_DEBUG_ALL         0x7ffffff0


प्रकारनिर्देश u_char *(*ngx_log_handler_pt) (ngx_log_t *लघुगणकः, u_char *buf, आकार_t len);
प्रकारनिर्देश शून्य (*ngx_log_writer_pt) (ngx_log_t *लघुगणकः, ngx_uint_t level,
    u_char *buf, आकार_t len);


संरचना ngx_log_s {
    ngx_uint_t           log_level;
    ngx_open_file_t     *file;

    ngx_atomic_uint_t    connection;

    time_t               disk_पूर्णम्_time;

    ngx_log_handler_pt   handler;
    शून्य                *दत्त;

    ngx_log_writer_pt    writer;
    शून्य                *wdata;

    /*
     * we declare "action" as "char *" because the actions are usually
     * the static strings and in the "u_char *" case we have to override
     * their types all the time
     */

    वर्ण                *action;

    ngx_log_t           *अग्र;

    NGX_COMPAT_BEGIN(5)
    NGX_COMPAT_अन्तः
};


#परिभाषय NGX_MAX_ERROR_STR   2048


/*********************************/

#यदि (NGX_HAVE_C99_VARIADIC_MACROS)

#परिभाषय NGX_HAVE_VARIADIC_MACROS  1

#परिभाषय ngx_log_error(level, लघुगणकः, ...)                                        \
    यदि ((लघुगणकः)->log_level >= level) ngx_log_error_core(level, लघुगणकः, __VA_ARGS__)

शून्य ngx_log_error_core(ngx_uint_t level, ngx_log_t *लघुगणकः, ngx_err_t err,
    ध्रुव वर्ण *fmt, ...);

#परिभाषय ngx_log_debug(level, लघुगणकः, ...)                                        \
    यदि ((लघुगणकः)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, लघुगणकः, __VA_ARGS__)

/*********************************/

#नोचेद्यदि (NGX_HAVE_GCC_VARIADIC_MACROS)

#परिभाषय NGX_HAVE_VARIADIC_MACROS  1

#परिभाषय ngx_log_error(level, लघुगणकः, तर्काः...)                                    \
    यदि ((लघुगणकः)->log_level >= level) ngx_log_error_core(level, लघुगणकः, तर्काः)

शून्य ngx_log_error_core(ngx_uint_t level, ngx_log_t *लघुगणकः, ngx_err_t err,
    ध्रुव वर्ण *fmt, ...);

#परिभाषय ngx_log_debug(level, लघुगणकः, तर्काः...)                                    \
    यदि ((लघुगणकः)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, लघुगणकः, तर्काः)

/*********************************/

#अन्यथा /* no variadic macros */

#परिभाषय NGX_HAVE_VARIADIC_MACROS  0

शून्य ngx_cdecl ngx_log_error(ngx_uint_t level, ngx_log_t *लघुगणकः, ngx_err_t err,
    ध्रुव वर्ण *fmt, ...);
शून्य ngx_log_error_core(ngx_uint_t level, ngx_log_t *लघुगणकः, ngx_err_t err,
    ध्रुव वर्ण *fmt, va_list तर्काः);
शून्य ngx_cdecl ngx_log_debug_core(ngx_log_t *लघुगणकः, ngx_err_t err,
    ध्रुव वर्ण *fmt, ...);


#यदि_अन्तः /* variadic macros */


/*********************************/

#यदि (NGX_DEBUG)

#यदि (NGX_HAVE_VARIADIC_MACROS)

#परिभाषय ngx_log_debug0(level, लघुगणकः, err, fmt)                                  \
        ngx_log_debug(level, लघुगणकः, err, fmt)

#परिभाषय ngx_log_debug1(level, लघुगणकः, err, fmt, arg1)                            \
        ngx_log_debug(level, लघुगणकः, err, fmt, arg1)

#परिभाषय ngx_log_debug2(level, लघुगणकः, err, fmt, arg1, arg2)                      \
        ngx_log_debug(level, लघुगणकः, err, fmt, arg1, arg2)

#परिभाषय ngx_log_debug3(level, लघुगणकः, err, fmt, arg1, arg2, arg3)                \
        ngx_log_debug(level, लघुगणकः, err, fmt, arg1, arg2, arg3)

#परिभाषय ngx_log_debug4(level, लघुगणकः, err, fmt, arg1, arg2, arg3, arg4)          \
        ngx_log_debug(level, लघुगणकः, err, fmt, arg1, arg2, arg3, arg4)

#परिभाषय ngx_log_debug5(level, लघुगणकः, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
        ngx_log_debug(level, लघुगणकः, err, fmt, arg1, arg2, arg3, arg4, arg5)

#परिभाषय ngx_log_debug6(level, लघुगणकः, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
        ngx_log_debug(level, लघुगणकः, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6)

#परिभाषय ngx_log_debug7(level, लघुगणकः, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
        ngx_log_debug(level, लघुगणकः, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#परिभाषय ngx_log_debug8(level, लघुगणकः, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
        ngx_log_debug(level, लघुगणकः, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)


#अन्यथा /* no variadic macros */

#परिभाषय ngx_log_debug0(level, लघुगणकः, err, fmt)                                  \
    यदि ((लघुगणकः)->log_level & level)                                             \
        ngx_log_debug_core(लघुगणकः, err, fmt)

#परिभाषय ngx_log_debug1(level, लघुगणकः, err, fmt, arg1)                            \
    यदि ((लघुगणकः)->log_level & level)                                             \
        ngx_log_debug_core(लघुगणकः, err, fmt, arg1)

#परिभाषय ngx_log_debug2(level, लघुगणकः, err, fmt, arg1, arg2)                      \
    यदि ((लघुगणकः)->log_level & level)                                             \
        ngx_log_debug_core(लघुगणकः, err, fmt, arg1, arg2)

#परिभाषय ngx_log_debug3(level, लघुगणकः, err, fmt, arg1, arg2, arg3)                \
    यदि ((लघुगणकः)->log_level & level)                                             \
        ngx_log_debug_core(लघुगणकः, err, fmt, arg1, arg2, arg3)

#परिभाषय ngx_log_debug4(level, लघुगणकः, err, fmt, arg1, arg2, arg3, arg4)          \
    यदि ((लघुगणकः)->log_level & level)                                             \
        ngx_log_debug_core(लघुगणकः, err, fmt, arg1, arg2, arg3, arg4)

#परिभाषय ngx_log_debug5(level, लघुगणकः, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
    यदि ((लघुगणकः)->log_level & level)                                             \
        ngx_log_debug_core(लघुगणकः, err, fmt, arg1, arg2, arg3, arg4, arg5)

#परिभाषय ngx_log_debug6(level, लघुगणकः, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
    यदि ((लघुगणकः)->log_level & level)                                             \
        ngx_log_debug_core(लघुगणकः, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#परिभाषय ngx_log_debug7(level, लघुगणकः, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
    यदि ((लघुगणकः)->log_level & level)                                             \
        ngx_log_debug_core(लघुगणकः, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#परिभाषय ngx_log_debug8(level, लघुगणकः, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
    यदि ((लघुगणकः)->log_level & level)                                             \
        ngx_log_debug_core(लघुगणकः, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#यदि_अन्तः

#अन्यथा /* !NGX_DEBUG */

#परिभाषय ngx_log_debug0(level, लघुगणकः, err, fmt)
#परिभाषय ngx_log_debug1(level, लघुगणकः, err, fmt, arg1)
#परिभाषय ngx_log_debug2(level, लघुगणकः, err, fmt, arg1, arg2)
#परिभाषय ngx_log_debug3(level, लघुगणकः, err, fmt, arg1, arg2, arg3)
#परिभाषय ngx_log_debug4(level, लघुगणकः, err, fmt, arg1, arg2, arg3, arg4)
#परिभाषय ngx_log_debug5(level, लघुगणकः, err, fmt, arg1, arg2, arg3, arg4, arg5)
#परिभाषय ngx_log_debug6(level, लघुगणकः, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#परिभाषय ngx_log_debug7(level, लघुगणकः, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7)
#परिभाषय ngx_log_debug8(level, लघुगणकः, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7, arg8)

#यदि_अन्तः

/*********************************/

ngx_log_t *ngx_log_init(u_char *उपसर्गः, u_char *error_log);
शून्य ngx_cdecl ngx_log_abort(ngx_err_t err, ध्रुव वर्ण *fmt, ...);
शून्य ngx_cdecl ngx_log_stderr(ngx_err_t err, ध्रुव वर्ण *fmt, ...);
u_char *ngx_log_errno(u_char *buf, u_char *चरमः, ngx_err_t err);
ngx_int_t ngx_log_open_default(ngx_आवृत्तिः_t *आवृत्तिः);
ngx_int_t ngx_log_redirect_stderr(ngx_आवृत्तिः_t *आवृत्तिः);
ngx_log_t *ngx_log_get_file_log(ngx_log_t *शीर्ष);
वर्ण *ngx_log_समुच्चयः_log(ngx_conf_t *cf, ngx_log_t **शीर्ष);


/*
 * ngx_write_stderr() cannot be implemented as macro, since
 * MSVC does not allow to use #ifdef inside macro parameters.
 *
 * ngx_write_fd() is used instead of ngx_write_console(), since
 * CharToOemBuff() inside ngx_write_console() cannot be used with
 * read only buffer as destination and CharToOemBuff() is not needed
 * for ngx_write_stderr() anyway.
 */
स्थिर ngx_inline शून्य
ngx_write_stderr(वर्ण *text)
{
    (शून्य) ngx_write_fd(ngx_stderr, text, ngx_strlen(text));
}


स्थिर ngx_inline शून्य
ngx_write_stdout(वर्ण *text)
{
    (शून्य) ngx_write_fd(ngx_stdout, text, ngx_strlen(text));
}


बाह्य ngx_module_t  ngx_errlog_module;
बाह्य ngx_uint_t    ngx_use_stderr;


#यदि_अन्तः /* _NGX_LOG_H_INCLUDED_ */
