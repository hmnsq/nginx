
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ ಅಕ್ಷರ *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಸ್ಥಿರ ಅಕ್ಷರ *ngx_log_ಗಣ_ಮಟ್ಟಗಳು(ngx_conf_t *cf, ngx_log_t *ಲಘುಗಣಕ);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_log_ಸೇರಿಸು(ngx_log_t *ಲಘುಗಣಕ, ngx_log_t *new_log);


#ಆದರೆ (NGX_DEBUG)

ಸ್ಥಿರ ಶೂನ್ಯ ngx_log_ಸ್ಮರಣೆ_writer(ngx_log_t *ಲಘುಗಣಕ, ngx_uint_t level,
    u_char *buf, ಗಾತ್ರ_t len);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_log_ಸ್ಮರಣೆ_cleanup(ಶೂನ್ಯ *ದತ್ತಾಂಶ);


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    u_char        *ಆರಂಭ;
    u_char        *ಕೊನೆ;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_ಸ್ಮರಣೆ_buf_t;

#ಆದರೆ_ಅಂತ್ಯ


ಸ್ಥಿರ ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1_ಇನ್ನಷ್ಟು,
      ngx_error_log,
      0,
      0,
      ಖಾಲಿ },

      ngx_null_command
};


ಸ್ಥಿರ ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    ಖಾಲಿ,
    ಖಾಲಿ
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ಖಾಲಿ,                                  /* init master */
    ಖಾಲಿ,                                  /* init module */
    ಖಾಲಿ,                                  /* init process */
    ಖಾಲಿ,                                  /* init thread */
    ಖಾಲಿ,                                  /* exit thread */
    ಖಾಲಿ,                                  /* exit process */
    ಖಾಲಿ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ಸ್ಥಿರ ngx_log_t        ngx_log;
ಸ್ಥಿರ ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


ಸ್ಥಿರ ngx_str_t err_ಮಟ್ಟಗಳು[] = {
    ngx_null_string,
    ngx_string("emerg"),
    ngx_string("alert"),
    ngx_string("crit"),
    ngx_string("error"),
    ngx_string("warn"),
    ngx_string("notice"),
    ngx_string("info"),
    ngx_string("debug")
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *debug_ಮಟ್ಟಗಳು[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#ಆದರೆ (NGX_HAVE_VARIADIC_MACROS)

ಶೂನ್ಯ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *ಲಘುಗಣಕ, ngx_err_t err,
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)

#ಇಲ್ಲದಿದ್ದರೆ

ಶೂನ್ಯ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *ಲಘುಗಣಕ, ngx_err_t err,
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ವಾದಗಳು)

#ಆದರೆ_ಅಂತ್ಯ
{
#ಆದರೆ (NGX_HAVE_VARIADIC_MACROS)
    va_list      ವಾದಗಳು;
#ಆದರೆ_ಅಂತ್ಯ
    u_char      *p, *ಕೊನೆಯ, *ಸಂದೇಶ;
    ssize_t      n;
    ngx_uint_t   wrote_stderr, debug_connection;
    u_char       errstr[NGX_MAX_ERROR_STR];

    ಕೊನೆಯ = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, ngx_cached_err_log_ಸಮಯ.ದತ್ತಾಂಶ,
                   ngx_cached_err_log_ಸಮಯ.len);

    p = ngx_slprintf(p, ಕೊನೆಯ, " [%V] ", &err_ಮಟ್ಟಗಳು[level]);

    /* pid#tid */
    p = ngx_slprintf(p, ಕೊನೆಯ, "%P#" NGX_TID_T_FMT ": ",
                    ngx_log_pid, ngx_log_tid);

    ಆದರೆ (ಲಘುಗಣಕ->connection) {
        p = ngx_slprintf(p, ಕೊನೆಯ, "*%uA ", ಲಘುಗಣಕ->connection);
    }

    ಸಂದೇಶ = p;

#ಆದರೆ (NGX_HAVE_VARIADIC_MACROS)

    va_ಆರಂಭ(ವಾದಗಳು, fmt);
    p = ngx_vslprintf(p, ಕೊನೆಯ, fmt, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);

#ಇಲ್ಲದಿದ್ದರೆ

    p = ngx_vslprintf(p, ಕೊನೆಯ, fmt, ವಾದಗಳು);

#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (err) {
        p = ngx_log_errno(p, ಕೊನೆಯ, err);
    }

    ಆದರೆ (level != NGX_LOG_DEBUG && ಲಘುಗಣಕ->handler) {
        p = ಲಘುಗಣಕ->handler(ಲಘುಗಣಕ, p, ಕೊನೆಯ - p);
    }

    ಆದರೆ (p > ಕೊನೆಯ - NGX_LINEFEED_ಗಾತ್ರ) {
        p = ಕೊನೆಯ - NGX_LINEFEED_ಗಾತ್ರ;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (ಲಘುಗಣಕ->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    ಆಗ (ಲಘುಗಣಕ) {

        ಆದರೆ (ಲಘುಗಣಕ->log_level < level && !debug_connection) {
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (ಲಘುಗಣಕ->writer) {
            ಲಘುಗಣಕ->writer(ಲಘುಗಣಕ, level, errstr, p - errstr);
            ಹೋಗು ಮುಂದಿನ;
        }

        ಆದರೆ (ngx_ಸಮಯ() == ಲಘುಗಣಕ->disk_ತುಂಬಿದ_ಸಮಯ) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            ಹೋಗು ಮುಂದಿನ;
        }

        n = ngx_write_fd(ಲಘುಗಣಕ->file->fd, errstr, p - errstr);

        ಆದರೆ (n == -1 && ngx_errno == NGX_ENOSPC) {
            ಲಘುಗಣಕ->disk_ತುಂಬಿದ_ಸಮಯ = ngx_ಸಮಯ();
        }

        ಆದರೆ (ಲಘುಗಣಕ->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    ಮುಂದಿನ:

        ಲಘುಗಣಕ = ಲಘುಗಣಕ->ಮುಂದಿನ;
    }

    ಆದರೆ (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        ಹಿಂತಿರುಗಿ;
    }

    ಸಂದೇಶ -= (7 + err_ಮಟ್ಟಗಳು[level].len + 3);

    (ಶೂನ್ಯ) ngx_sprintf(ಸಂದೇಶ, "nginx: [%V] ", &err_ಮಟ್ಟಗಳು[level]);

    (ಶೂನ್ಯ) ngx_write_console(ngx_stderr, ಸಂದೇಶ, p - ಸಂದೇಶ);
}


#ಆದರೆ !(NGX_HAVE_VARIADIC_MACROS)

ಶೂನ್ಯ ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *ಲಘುಗಣಕ, ngx_err_t err,
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
    va_list  ವಾದಗಳು;

    ಆದರೆ (ಲಘುಗಣಕ->log_level >= level) {
        va_ಆರಂಭ(ವಾದಗಳು, fmt);
        ngx_log_error_core(level, ಲಘುಗಣಕ, err, fmt, ವಾದಗಳು);
        va_ಕೊನೆ(ವಾದಗಳು);
    }
}


ಶೂನ್ಯ ngx_cdecl
ngx_log_debug_core(ngx_log_t *ಲಘುಗಣಕ, ngx_err_t err, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
    va_list  ವಾದಗಳು;

    va_ಆರಂಭ(ವಾದಗಳು, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, ಲಘುಗಣಕ, err, fmt, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);
}

#ಆದರೆ_ಅಂತ್ಯ


ಶೂನ್ಯ ngx_cdecl
ngx_log_abort(ngx_err_t err, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
    u_char   *p;
    va_list   ವಾದಗಳು;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_ಆರಂಭ(ವಾದಗಳು, fmt);
    p = ngx_vsnprintf(errstr, ಗಾತ್ರ(errstr) - 1, fmt, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);

    ngx_log_error(NGX_LOG_ALERT, ngx_ಚಕ್ರ->ಲಘುಗಣಕ, err,
                  "%*s", p - errstr, errstr);
}


ಶೂನ್ಯ ngx_cdecl
ngx_log_stderr(ngx_err_t err, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
    u_char   *p, *ಕೊನೆಯ;
    va_list   ವಾದಗಳು;
    u_char    errstr[NGX_MAX_ERROR_STR];

    ಕೊನೆಯ = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_ಆರಂಭ(ವಾದಗಳು, fmt);
    p = ngx_vslprintf(p, ಕೊನೆಯ, fmt, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);

    ಆದರೆ (err) {
        p = ngx_log_errno(p, ಕೊನೆಯ, err);
    }

    ಆದರೆ (p > ಕೊನೆಯ - NGX_LINEFEED_ಗಾತ್ರ) {
        p = ಕೊನೆಯ - NGX_LINEFEED_ಗಾತ್ರ;
    }

    ngx_linefeed(p);

    (ಶೂನ್ಯ) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *ಕೊನೆಯ, ngx_err_t err)
{
    ಆದರೆ (buf > ಕೊನೆಯ - 50) {

        /* leave a space for an error code */

        buf = ಕೊನೆಯ - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#ಆದರೆ (NGX_WIN32)
    buf = ngx_slprintf(buf, ಕೊನೆಯ, ((ಚಿಹ್ನೆರಹಿತ) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#ಇಲ್ಲದಿದ್ದರೆ
    buf = ngx_slprintf(buf, ಕೊನೆಯ, " (%d: ", err);
#ಆದರೆ_ಅಂತ್ಯ

    buf = ngx_strerror(err, buf, ಕೊನೆಯ - buf);

    ಆದರೆ (buf < ಕೊನೆಯ) {
        *buf++ = ')';
    }

    ಹಿಂತಿರುಗಿ buf;
}


ngx_log_t *
ngx_log_init(u_char *ಪೂರ್ವಪ್ರತ್ಯಯ, u_char *error_log)
{
    u_char  *p, *name;
    ಗಾತ್ರ_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    ಆದರೆ (error_log == ಖಾಲಿ) {
        error_log = (u_char *) NGX_ERROR_LOG_ಮಾರ್ಗ;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    ಆದರೆ (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        ಹಿಂತಿರುಗಿ &ngx_log;
    }

    p = ಖಾಲಿ;

#ಆದರೆ (NGX_WIN32)
    ಆದರೆ (name[1] != ':') {
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ (name[0] != '/') {
#ಆದರೆ_ಅಂತ್ಯ

        ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ) {
            plen = ngx_strlen(ಪೂರ್ವಪ್ರತ್ಯಯ);

        } ಇಲ್ಲದಿದ್ದರೆ {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NGX_ಪೂರ್ವಪ್ರತ್ಯಯ
            ಪೂರ್ವಪ್ರತ್ಯಯ = (u_char *) NGX_ಪೂರ್ವಪ್ರತ್ಯಯ;
            plen = ngx_strlen(ಪೂರ್ವಪ್ರತ್ಯಯ);
#ಇಲ್ಲದಿದ್ದರೆ
            plen = 0;
#ಆದರೆ_ಅಂತ್ಯ
        }

        ಆದರೆ (plen) {
            name = ಸ್ಮರಣೆ_ಕೊಡು(plen + nlen + 2);
            ಆದರೆ (name == ಖಾಲಿ) {
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }

            p = ngx_cpymem(name, ಪೂರ್ವಪ್ರತ್ಯಯ, plen);

            ಆದರೆ (!ngx_ಮಾರ್ಗ_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    ಆದರೆ (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#ಆದರೆ (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#ಆದರೆ_ಅಂತ್ಯ

        ngx_log_file.fd = ngx_stderr;
    }

    ಆದರೆ (p) {
        ngx_free(p);
    }

    ಹಿಂತಿರುಗಿ &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_log_t  *ಲಘುಗಣಕ;

    ಆದರೆ (ngx_log_get_file_log(&ಚಕ್ರ->new_log) != ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಆದರೆ (ಚಕ್ರ->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        ಲಘುಗಣಕ = ngx_pcalloc(ಚಕ್ರ->pool, ಗಾತ್ರ(ngx_log_t));
        ಆದರೆ (ಲಘುಗಣಕ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

    } ಇಲ್ಲದಿದ್ದರೆ {
        /* no error logs at all */
        ಲಘುಗಣಕ = &ಚಕ್ರ->new_log;
    }

    ಲಘುಗಣಕ->log_level = NGX_LOG_ERR;

    ಲಘುಗಣಕ->file = ngx_conf_open_file(ಚಕ್ರ, &ಚಕ್ರ->error_log);
    ಆದರೆ (ಲಘುಗಣಕ->file == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ಲಘುಗಣಕ != &ಚಕ್ರ->new_log) {
        ngx_log_ಸೇರಿಸು(&ಚಕ್ರ->new_log, ಲಘುಗಣಕ);
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_fd_t  fd;

    ಆದರೆ (ಚಕ್ರ->log_use_stderr) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(ಚಕ್ರ->ಲಘುಗಣಕ)->file->fd;

    ಆದರೆ (fd != ngx_stderr) {
        ಆದರೆ (ngx_ಗಣ_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                          ngx_ಗಣ_stderr_n " failed");

            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *ತಲೆ)
{
    ngx_log_t  *ಲಘುಗಣಕ;

    ಗಾಗಿ (ಲಘುಗಣಕ = ತಲೆ; ಲಘುಗಣಕ; ಲಘುಗಣಕ = ಲಘುಗಣಕ->ಮುಂದಿನ) {
        ಆದರೆ (ಲಘುಗಣಕ->file != ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ ಲಘುಗಣಕ;
        }
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_log_ಗಣ_ಮಟ್ಟಗಳು(ngx_conf_t *cf, ngx_log_t *ಲಘುಗಣಕ)
{
    ngx_uint_t   i, n, d, ಸಿಕ್ಕಿತು;
    ngx_str_t   *ಮೌಲ್ಯ;

    ಆದರೆ (cf->ವಾದಗಳು->nelts == 2) {
        ಲಘುಗಣಕ->log_level = NGX_LOG_ERR;
        ಹಿಂತಿರುಗಿ NGX_CONF_OK;
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    ಗಾಗಿ (i = 2; i < cf->ವಾದಗಳು->nelts; i++) {
        ಸಿಕ್ಕಿತು = 0;

        ಗಾಗಿ (n = 1; n <= NGX_LOG_DEBUG; n++) {
            ಆದರೆ (ngx_strcmp(ಮೌಲ್ಯ[i].ದತ್ತಾಂಶ, err_ಮಟ್ಟಗಳು[n].ದತ್ತಾಂಶ) == 0) {

                ಆದರೆ (ಲಘುಗಣಕ->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &ಮೌಲ್ಯ[i]);
                    ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
                }

                ಲಘುಗಣಕ->log_level = n;
                ಸಿಕ್ಕಿತು = 1;
                ನಿಲ್ಲಿಸು;
            }
        }

        ಗಾಗಿ (n = 0, d = NGX_LOG_DEBUG_ಮೊದಲ; d <= NGX_LOG_DEBUG_ಕೊನೆಯ; d <<= 1) {
            ಆದರೆ (ngx_strcmp(ಮೌಲ್ಯ[i].ದತ್ತಾಂಶ, debug_ಮಟ್ಟಗಳು[n++]) == 0) {
                ಆದರೆ (ಲಘುಗಣಕ->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &ಮೌಲ್ಯ[i]);
                    ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
                }

                ಲಘುಗಣಕ->log_level |= d;
                ಸಿಕ್ಕಿತು = 1;
                ನಿಲ್ಲಿಸು;
            }
        }


        ಆದರೆ (!ಸಿಕ್ಕಿತು) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &ಮೌಲ್ಯ[i]);
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }
    }

    ಆದರೆ (ಲಘುಗಣಕ->log_level == NGX_LOG_DEBUG) {
        ಲಘುಗಣಕ->log_level = NGX_LOG_DEBUG_ALL;
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಸ್ಥಿರ ಅಕ್ಷರ *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->ಚಕ್ರ->new_log;

    ಹಿಂತಿರುಗಿ ngx_log_ಗಣ_log(cf, &dummy);
}


ಅಕ್ಷರ *
ngx_log_ಗಣ_log(ngx_conf_t *cf, ngx_log_t **ತಲೆ)
{
    ngx_log_t          *new_log;
    ngx_str_t          *ಮೌಲ್ಯ, name;
    ngx_syslog_peer_t  *peer;

    ಆದರೆ (*ತಲೆ != ಖಾಲಿ && (*ತಲೆ)->log_level == 0) {
        new_log = *ತಲೆ;

    } ಇಲ್ಲದಿದ್ದರೆ {

        new_log = ngx_pcalloc(cf->pool, ಗಾತ್ರ(ngx_log_t));
        ಆದರೆ (new_log == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        ಆದರೆ (*ತಲೆ == ಖಾಲಿ) {
            *ತಲೆ = new_log;
        }
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    ಆದರೆ (ngx_strcmp(ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ, "stderr") == 0) {
        ngx_str_null(&name);
        cf->ಚಕ್ರ->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->ಚಕ್ರ, &name);
        ಆದರೆ (new_log->file == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strncmp(ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ, "memory:", 7) == 0) {

#ಆದರೆ (NGX_DEBUG)
        ಗಾತ್ರ_t                 ಗಾತ್ರ_2, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_ಸ್ಮರಣೆ_buf_t  *buf;

        ಮೌಲ್ಯ[1].len -= 7;
        ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ += 7;

        needed = ಗಾತ್ರ("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_ಗಾತ್ರ_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        ಗಾತ್ರ_2 = ngx_parse_ಗಾತ್ರ(&ಮೌಲ್ಯ[1]);

        ಆದರೆ (ಗಾತ್ರ_2 == (ಗಾತ್ರ_t) NGX_ERROR || ಗಾತ್ರ_2 < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &ಮೌಲ್ಯ[1]);
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, ಗಾತ್ರ(ngx_log_ಸ್ಮರಣೆ_buf_t));
        ಆದರೆ (buf == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        buf->ಆರಂಭ = ngx_pnalloc(cf->pool, ಗಾತ್ರ_2);
        ಆದರೆ (buf->ಆರಂಭ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        buf->ಕೊನೆ = buf->ಆರಂಭ + ಗಾತ್ರ_2;

        buf->pos = ngx_slprintf(buf->ಆರಂಭ, buf->ಕೊನೆ, "MEMLOG %uz %V:%ui%N",
                                ಗಾತ್ರ_2, &cf->conf_file->file.name,
                                cf->conf_file->ಸಾಲು);

        ngx_memset(buf->pos, ' ', buf->ಕೊನೆ - buf->pos);

        cln = ngx_pool_cleanup_ಕೂಡಿಸು(cf->pool, 0);
        ಆದರೆ (cln == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        cln->ದತ್ತಾಂಶ = new_log;
        cln->handler = ngx_log_ಸ್ಮರಣೆ_cleanup;

        new_log->writer = ngx_log_ಸ್ಮರಣೆ_writer;
        new_log->wdata = buf;

#ಇಲ್ಲದಿದ್ದರೆ
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
#ಆದರೆ_ಅಂತ್ಯ

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strncmp(ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, ಗಾತ್ರ(ngx_syslog_peer_t));
        ಆದರೆ (peer == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        ಆದರೆ (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } ಇಲ್ಲದಿದ್ದರೆ {
        new_log->file = ngx_conf_open_file(cf->ಚಕ್ರ, &ಮೌಲ್ಯ[1]);
        ಆದರೆ (new_log->file == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }
    }

    ಆದರೆ (ngx_log_ಗಣ_ಮಟ್ಟಗಳು(cf, new_log) != NGX_CONF_OK) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಆದರೆ (*ತಲೆ != new_log) {
        ngx_log_ಸೇರಿಸು(*ತಲೆ, new_log);
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_log_ಸೇರಿಸು(ngx_log_t *ಲಘುಗಣಕ, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    ಆದರೆ (new_log->log_level > ಲಘುಗಣಕ->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *ಲಘುಗಣಕ;
        *ಲಘುಗಣಕ = *new_log;
        *new_log = tmp;

        ಲಘುಗಣಕ->ಮುಂದಿನ = new_log;
        ಹಿಂತಿರುಗಿ;
    }

    ಆಗ (ಲಘುಗಣಕ->ಮುಂದಿನ) {
        ಆದರೆ (new_log->log_level > ಲಘುಗಣಕ->ಮುಂದಿನ->log_level) {
            new_log->ಮುಂದಿನ = ಲಘುಗಣಕ->ಮುಂದಿನ;
            ಲಘುಗಣಕ->ಮುಂದಿನ = new_log;
            ಹಿಂತಿರುಗಿ;
        }

        ಲಘುಗಣಕ = ಲಘುಗಣಕ->ಮುಂದಿನ;
    }

    ಲಘುಗಣಕ->ಮುಂದಿನ = new_log;
}


#ಆದರೆ (NGX_DEBUG)

ಸ್ಥಿರ ಶೂನ್ಯ
ngx_log_ಸ್ಮರಣೆ_writer(ngx_log_t *ಲಘುಗಣಕ, ngx_uint_t level, u_char *buf,
    ಗಾತ್ರ_t len)
{
    u_char                *p;
    ಗಾತ್ರ_t                 avail, written;
    ngx_log_ಸ್ಮರಣೆ_buf_t  *mem;

    mem = ಲಘುಗಣಕ->wdata;

    ಆದರೆ (mem == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ;
    }

    written = ngx_atomic_fetch_ಕೂಡಿಸು(&mem->written, len);

    p = mem->pos + written % (mem->ಕೊನೆ - mem->pos);

    avail = mem->ಕೊನೆ - p;

    ಆದರೆ (avail >= len) {
        ngx_memcpy(p, buf, len);

    } ಇಲ್ಲದಿದ್ದರೆ {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_log_ಸ್ಮರಣೆ_cleanup(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ngx_log_t *ಲಘುಗಣಕ = ದತ್ತಾಂಶ;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ಲಘುಗಣಕ, 0, "destroy memory log buffer");

    ಲಘುಗಣಕ->wdata = ಖಾಲಿ;
}

#ಆದರೆ_ಅಂತ್ಯ
