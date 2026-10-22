
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_LOG_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_LOG_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_STDERR            0
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_EMERG             1
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_ALERT             2
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_CRIT              3
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_ERR               4
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_WARN              5
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_NOTICE            6
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_INFO              7
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_DEBUG             8

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_DEBUG_CORE        0x010
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_DEBUG_ALLOC       0x020
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_DEBUG_MUTEX       0x040
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_DEBUG_EVENT       0x080
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_DEBUG_HTTP        0x100
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_DEBUG_MAIL        0x200
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_DEBUG_ಹರಿವು      0x400

/*
 * do not forget to update debug_levels[] in src/core/ngx_log.c
 * after the adding a new debug level
 */

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_DEBUG_ಮೊದಲ       NGX_LOG_DEBUG_CORE
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_DEBUG_ಕೊನೆಯ        NGX_LOG_DEBUG_ಹರಿವು
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_DEBUG_CONNECTION  0x80000000
#ವ್ಯಾಖ್ಯಾನಿಸು NGX_LOG_DEBUG_ALL         0x7ffffff0


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ u_char *(*ngx_log_handler_pt) (ngx_log_t *ಲಘುಗಣಕ, u_char *buf, ಗಾತ್ರ_t len);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ngx_log_writer_pt) (ngx_log_t *ಲಘುಗಣಕ, ngx_uint_t level,
    u_char *buf, ಗಾತ್ರ_t len);


ರಚನೆ ngx_log_s {
    ngx_uint_t           log_level;
    ngx_open_file_t     *file;

    ngx_atomic_uint_t    connection;

    ಸಮಯ_t               disk_ತುಂಬಿದ_ಸಮಯ;

    ngx_log_handler_pt   handler;
    ಶೂನ್ಯ                *ದತ್ತಾಂಶ;

    ngx_log_writer_pt    writer;
    ಶೂನ್ಯ                *wdata;

    /*
     * we declare "action" as "char *" because the actions are usually
     * the static strings and in the "u_char *" case we have to override
     * their types all the time
     */

    ಅಕ್ಷರ                *action;

    ngx_log_t           *ಮುಂದಿನ;

    NGX_COMPAT_BEGIN(5)
    NGX_COMPAT_ಕೊನೆ
};


#ವ್ಯಾಖ್ಯಾನಿಸು NGX_MAX_ERROR_STR   2048


/*********************************/

#ಆದರೆ (NGX_HAVE_C99_VARIADIC_MACROS)

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_HAVE_VARIADIC_MACROS  1

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_error(level, ಲಘುಗಣಕ, ...)                                        \
    ಆದರೆ ((ಲಘುಗಣಕ)->log_level >= level) ngx_log_error_core(level, ಲಘುಗಣಕ, __VA_ARGS__)

ಶೂನ್ಯ ngx_log_error_core(ngx_uint_t level, ngx_log_t *ಲಘುಗಣಕ, ngx_err_t err,
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...);

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug(level, ಲಘುಗಣಕ, ...)                                        \
    ಆದರೆ ((ಲಘುಗಣಕ)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, ಲಘುಗಣಕ, __VA_ARGS__)

/*********************************/

#ಅಥವಾಆದರೆ (NGX_HAVE_GCC_VARIADIC_MACROS)

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_HAVE_VARIADIC_MACROS  1

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_error(level, ಲಘುಗಣಕ, ವಾದಗಳು...)                                    \
    ಆದರೆ ((ಲಘುಗಣಕ)->log_level >= level) ngx_log_error_core(level, ಲಘುಗಣಕ, ವಾದಗಳು)

ಶೂನ್ಯ ngx_log_error_core(ngx_uint_t level, ngx_log_t *ಲಘುಗಣಕ, ngx_err_t err,
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...);

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug(level, ಲಘುಗಣಕ, ವಾದಗಳು...)                                    \
    ಆದರೆ ((ಲಘುಗಣಕ)->log_level & level)                                             \
        ngx_log_error_core(NGX_LOG_DEBUG, ಲಘುಗಣಕ, ವಾದಗಳು)

/*********************************/

#ಇಲ್ಲದಿದ್ದರೆ /* no variadic macros */

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_HAVE_VARIADIC_MACROS  0

ಶೂನ್ಯ ngx_cdecl ngx_log_error(ngx_uint_t level, ngx_log_t *ಲಘುಗಣಕ, ngx_err_t err,
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...);
ಶೂನ್ಯ ngx_log_error_core(ngx_uint_t level, ngx_log_t *ಲಘುಗಣಕ, ngx_err_t err,
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ವಾದಗಳು);
ಶೂನ್ಯ ngx_cdecl ngx_log_debug_core(ngx_log_t *ಲಘುಗಣಕ, ngx_err_t err,
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...);


#ಆದರೆ_ಅಂತ್ಯ /* variadic macros */


/*********************************/

#ಆದರೆ (NGX_DEBUG)

#ಆದರೆ (NGX_HAVE_VARIADIC_MACROS)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug0(level, ಲಘುಗಣಕ, err, fmt)                                  \
        ngx_log_debug(level, ಲಘುಗಣಕ, err, fmt)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug1(level, ಲಘುಗಣಕ, err, fmt, arg1)                            \
        ngx_log_debug(level, ಲಘುಗಣಕ, err, fmt, arg1)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug2(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2)                      \
        ngx_log_debug(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug3(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3)                \
        ngx_log_debug(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug4(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4)          \
        ngx_log_debug(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug5(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
        ngx_log_debug(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4, arg5)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug6(level, ಲಘುಗಣಕ, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
        ngx_log_debug(level, ಲಘುಗಣಕ, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug7(level, ಲಘುಗಣಕ, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
        ngx_log_debug(level, ಲಘುಗಣಕ, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug8(level, ಲಘುಗಣಕ, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
        ngx_log_debug(level, ಲಘುಗಣಕ, err, fmt,                                   \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)


#ಇಲ್ಲದಿದ್ದರೆ /* no variadic macros */

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug0(level, ಲಘುಗಣಕ, err, fmt)                                  \
    ಆದರೆ ((ಲಘುಗಣಕ)->log_level & level)                                             \
        ngx_log_debug_core(ಲಘುಗಣಕ, err, fmt)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug1(level, ಲಘುಗಣಕ, err, fmt, arg1)                            \
    ಆದರೆ ((ಲಘುಗಣಕ)->log_level & level)                                             \
        ngx_log_debug_core(ಲಘುಗಣಕ, err, fmt, arg1)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug2(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2)                      \
    ಆದರೆ ((ಲಘುಗಣಕ)->log_level & level)                                             \
        ngx_log_debug_core(ಲಘುಗಣಕ, err, fmt, arg1, arg2)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug3(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3)                \
    ಆದರೆ ((ಲಘುಗಣಕ)->log_level & level)                                             \
        ngx_log_debug_core(ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug4(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4)          \
    ಆದರೆ ((ಲಘುಗಣಕ)->log_level & level)                                             \
        ngx_log_debug_core(ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug5(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4, arg5)    \
    ಆದರೆ ((ಲಘುಗಣಕ)->log_level & level)                                             \
        ngx_log_debug_core(ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4, arg5)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug6(level, ಲಘುಗಣಕ, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6)                    \
    ಆದರೆ ((ಲಘುಗಣಕ)->log_level & level)                                             \
        ngx_log_debug_core(ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug7(level, ಲಘುಗಣಕ, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)              \
    ಆದರೆ ((ಲಘುಗಣಕ)->log_level & level)                                             \
        ngx_log_debug_core(ಲಘುಗಣಕ, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug8(level, ಲಘುಗಣಕ, err, fmt,                                  \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)        \
    ಆದರೆ ((ಲಘುಗಣಕ)->log_level & level)                                             \
        ngx_log_debug_core(ಲಘುಗಣಕ, err, fmt,                                     \
                       arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#ಆದರೆ_ಅಂತ್ಯ

#ಇಲ್ಲದಿದ್ದರೆ /* !NGX_DEBUG */

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug0(level, ಲಘುಗಣಕ, err, fmt)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug1(level, ಲಘುಗಣಕ, err, fmt, arg1)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug2(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug3(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug4(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug5(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4, arg5)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug6(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4, arg5, arg6)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug7(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7)
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_log_debug8(level, ಲಘುಗಣಕ, err, fmt, arg1, arg2, arg3, arg4, arg5,    \
                       arg6, arg7, arg8)

#ಆದರೆ_ಅಂತ್ಯ

/*********************************/

ngx_log_t *ngx_log_init(u_char *ಪೂರ್ವಪ್ರತ್ಯಯ, u_char *error_log);
ಶೂನ್ಯ ngx_cdecl ngx_log_abort(ngx_err_t err, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...);
ಶೂನ್ಯ ngx_cdecl ngx_log_stderr(ngx_err_t err, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...);
u_char *ngx_log_errno(u_char *buf, u_char *ಕೊನೆಯ, ngx_err_t err);
ngx_int_t ngx_log_open_default(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ngx_int_t ngx_log_redirect_stderr(ngx_ಚಕ್ರ_t *ಚಕ್ರ);
ngx_log_t *ngx_log_get_file_log(ngx_log_t *ತಲೆ);
ಅಕ್ಷರ *ngx_log_ಗಣ_log(ngx_conf_t *cf, ngx_log_t **ತಲೆ);


/*
 * ngx_write_stderr() cannot be implemented as macro, since
 * MSVC does not allow to use #ifdef inside macro parameters.
 *
 * ngx_write_fd() is used instead of ngx_write_console(), since
 * CharToOemBuff() inside ngx_write_console() cannot be used with
 * read only buffer as destination and CharToOemBuff() is not needed
 * for ngx_write_stderr() anyway.
 */
ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ
ngx_write_stderr(ಅಕ್ಷರ *ಪಠ್ಯ)
{
    (ಶೂನ್ಯ) ngx_write_fd(ngx_stderr, ಪಠ್ಯ, ngx_strlen(ಪಠ್ಯ));
}


ಸ್ಥಿರ ngx_inline ಶೂನ್ಯ
ngx_write_stdout(ಅಕ್ಷರ *ಪಠ್ಯ)
{
    (ಶೂನ್ಯ) ngx_write_fd(ngx_stdout, ಪಠ್ಯ, ngx_strlen(ಪಠ್ಯ));
}


ಬಾಹ್ಯ ngx_module_t  ngx_errlog_module;
ಬಾಹ್ಯ ngx_uint_t    ngx_use_stderr;


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_LOG_H_INCLUDED_ */
