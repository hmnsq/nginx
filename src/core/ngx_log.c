
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਸਥਿਰ ਅੱਖਰ *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf);
ਸਥਿਰ ਅੱਖਰ *ngx_log_ਸੈੱਟ_ਪੱਧਰ(ngx_conf_t *cf, ngx_log_t *ਲਘੂਗਣਕ);
ਸਥਿਰ ਖਾਲੀ ngx_log_ਪਾਓ(ngx_log_t *ਲਘੂਗਣਕ, ngx_log_t *new_log);


#ਜੇ (NGX_DEBUG)

ਸਥਿਰ ਖਾਲੀ ngx_log_ਸਮ੍ਰਿਤੀ_writer(ngx_log_t *ਲਘੂਗਣਕ, ngx_uint_t level,
    u_char *buf, ਮਾਪ_t len);
ਸਥਿਰ ਖਾਲੀ ngx_log_ਸਮ੍ਰਿਤੀ_cleanup(ਖਾਲੀ *ਅੰਕੜੇ);


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    u_char        *ਸ਼ੁਰੂ;
    u_char        *ਸਿਰਾ;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_ਸਮ੍ਰਿਤੀ_buf_t;

#ਜੇ_ਅੰਤ


ਸਥਿਰ ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1_ਹੋਰ,
      ngx_error_log,
      0,
      0,
      ਨਿਰਾ },

      ngx_null_command
};


ਸਥਿਰ ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    ਨਿਰਾ,
    ਨਿਰਾ
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ਨਿਰਾ,                                  /* init master */
    ਨਿਰਾ,                                  /* init module */
    ਨਿਰਾ,                                  /* init process */
    ਨਿਰਾ,                                  /* init thread */
    ਨਿਰਾ,                                  /* exit thread */
    ਨਿਰਾ,                                  /* exit process */
    ਨਿਰਾ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ਸਥਿਰ ngx_log_t        ngx_log;
ਸਥਿਰ ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


ਸਥਿਰ ngx_str_t err_ਪੱਧਰ[] = {
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

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *debug_ਪੱਧਰ[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#ਜੇ (NGX_HAVE_VARIADIC_MACROS)

ਖਾਲੀ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *ਲਘੂਗਣਕ, ngx_err_t err,
    ਅਚਲ ਅੱਖਰ *fmt, ...)

#ਨਹੀਂਤਾਂ

ਖਾਲੀ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *ਲਘੂਗਣਕ, ngx_err_t err,
    ਅਚਲ ਅੱਖਰ *fmt, va_list ਦਲੀਲਾਂ)

#ਜੇ_ਅੰਤ
{
#ਜੇ (NGX_HAVE_VARIADIC_MACROS)
    va_list      ਦਲੀਲਾਂ;
#ਜੇ_ਅੰਤ
    u_char      *p, *ਆਖਰੀ, *ਸੁਨੇਹਾ;
    ssize_t      n;
    ngx_uint_t   wrote_stderr, debug_connection;
    u_char       errstr[NGX_MAX_ERROR_STR];

    ਆਖਰੀ = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, ngx_cached_err_log_ਸਮਾਂ.ਅੰਕੜੇ,
                   ngx_cached_err_log_ਸਮਾਂ.len);

    p = ngx_slprintf(p, ਆਖਰੀ, " [%V] ", &err_ਪੱਧਰ[level]);

    /* pid#tid */
    p = ngx_slprintf(p, ਆਖਰੀ, "%P#" NGX_TID_T_FMT ": ",
                    ngx_log_pid, ngx_log_tid);

    ਜੇ (ਲਘੂਗਣਕ->connection) {
        p = ngx_slprintf(p, ਆਖਰੀ, "*%uA ", ਲਘੂਗਣਕ->connection);
    }

    ਸੁਨੇਹਾ = p;

#ਜੇ (NGX_HAVE_VARIADIC_MACROS)

    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
    p = ngx_vslprintf(p, ਆਖਰੀ, fmt, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);

#ਨਹੀਂਤਾਂ

    p = ngx_vslprintf(p, ਆਖਰੀ, fmt, ਦਲੀਲਾਂ);

#ਜੇ_ਅੰਤ

    ਜੇ (err) {
        p = ngx_log_errno(p, ਆਖਰੀ, err);
    }

    ਜੇ (level != NGX_LOG_DEBUG && ਲਘੂਗਣਕ->handler) {
        p = ਲਘੂਗਣਕ->handler(ਲਘੂਗਣਕ, p, ਆਖਰੀ - p);
    }

    ਜੇ (p > ਆਖਰੀ - NGX_LINEFEED_ਮਾਪ) {
        p = ਆਖਰੀ - NGX_LINEFEED_ਮਾਪ;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (ਲਘੂਗਣਕ->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    ਜਦੋਂ (ਲਘੂਗਣਕ) {

        ਜੇ (ਲਘੂਗਣਕ->log_level < level && !debug_connection) {
            ਰੋਕੋ;
        }

        ਜੇ (ਲਘੂਗਣਕ->writer) {
            ਲਘੂਗਣਕ->writer(ਲਘੂਗਣਕ, level, errstr, p - errstr);
            ਜਾਓ ਅਗਲਾ;
        }

        ਜੇ (ngx_ਸਮਾਂ() == ਲਘੂਗਣਕ->disk_ਭਰਿਆ_ਸਮਾਂ) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            ਜਾਓ ਅਗਲਾ;
        }

        n = ngx_write_fd(ਲਘੂਗਣਕ->file->fd, errstr, p - errstr);

        ਜੇ (n == -1 && ngx_errno == NGX_ENOSPC) {
            ਲਘੂਗਣਕ->disk_ਭਰਿਆ_ਸਮਾਂ = ngx_ਸਮਾਂ();
        }

        ਜੇ (ਲਘੂਗਣਕ->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    ਅਗਲਾ:

        ਲਘੂਗਣਕ = ਲਘੂਗਣਕ->ਅਗਲਾ;
    }

    ਜੇ (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        ਵਾਪਸ;
    }

    ਸੁਨੇਹਾ -= (7 + err_ਪੱਧਰ[level].len + 3);

    (ਖਾਲੀ) ngx_sprintf(ਸੁਨੇਹਾ, "nginx: [%V] ", &err_ਪੱਧਰ[level]);

    (ਖਾਲੀ) ngx_write_console(ngx_stderr, ਸੁਨੇਹਾ, p - ਸੁਨੇਹਾ);
}


#ਜੇ !(NGX_HAVE_VARIADIC_MACROS)

ਖਾਲੀ ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *ਲਘੂਗਣਕ, ngx_err_t err,
    ਅਚਲ ਅੱਖਰ *fmt, ...)
{
    va_list  ਦਲੀਲਾਂ;

    ਜੇ (ਲਘੂਗਣਕ->log_level >= level) {
        va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
        ngx_log_error_core(level, ਲਘੂਗਣਕ, err, fmt, ਦਲੀਲਾਂ);
        va_ਸਿਰਾ(ਦਲੀਲਾਂ);
    }
}


ਖਾਲੀ ngx_cdecl
ngx_log_debug_core(ngx_log_t *ਲਘੂਗਣਕ, ngx_err_t err, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
    va_list  ਦਲੀਲਾਂ;

    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, ਲਘੂਗਣਕ, err, fmt, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);
}

#ਜੇ_ਅੰਤ


ਖਾਲੀ ngx_cdecl
ngx_log_abort(ngx_err_t err, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
    u_char   *p;
    va_list   ਦਲੀਲਾਂ;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
    p = ngx_vsnprintf(errstr, ਆਕਾਰ(errstr) - 1, fmt, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);

    ngx_log_error(NGX_LOG_ALERT, ngx_ਚੱਕਰ->ਲਘੂਗਣਕ, err,
                  "%*s", p - errstr, errstr);
}


ਖਾਲੀ ngx_cdecl
ngx_log_stderr(ngx_err_t err, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
    u_char   *p, *ਆਖਰੀ;
    va_list   ਦਲੀਲਾਂ;
    u_char    errstr[NGX_MAX_ERROR_STR];

    ਆਖਰੀ = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
    p = ngx_vslprintf(p, ਆਖਰੀ, fmt, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);

    ਜੇ (err) {
        p = ngx_log_errno(p, ਆਖਰੀ, err);
    }

    ਜੇ (p > ਆਖਰੀ - NGX_LINEFEED_ਮਾਪ) {
        p = ਆਖਰੀ - NGX_LINEFEED_ਮਾਪ;
    }

    ngx_linefeed(p);

    (ਖਾਲੀ) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *ਆਖਰੀ, ngx_err_t err)
{
    ਜੇ (buf > ਆਖਰੀ - 50) {

        /* leave a space for an error code */

        buf = ਆਖਰੀ - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#ਜੇ (NGX_WIN32)
    buf = ngx_slprintf(buf, ਆਖਰੀ, ((ਬਿਨਾਨਿਸ਼ਾਨੀ) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#ਨਹੀਂਤਾਂ
    buf = ngx_slprintf(buf, ਆਖਰੀ, " (%d: ", err);
#ਜੇ_ਅੰਤ

    buf = ngx_strerror(err, buf, ਆਖਰੀ - buf);

    ਜੇ (buf < ਆਖਰੀ) {
        *buf++ = ')';
    }

    ਵਾਪਸ buf;
}


ngx_log_t *
ngx_log_init(u_char *ਅਗੇਤਰ, u_char *error_log)
{
    u_char  *p, *name;
    ਮਾਪ_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    ਜੇ (error_log == ਨਿਰਾ) {
        error_log = (u_char *) NGX_ERROR_LOG_ਰਸਤਾ;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    ਜੇ (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        ਵਾਪਸ &ngx_log;
    }

    p = ਨਿਰਾ;

#ਜੇ (NGX_WIN32)
    ਜੇ (name[1] != ':') {
#ਨਹੀਂਤਾਂ
    ਜੇ (name[0] != '/') {
#ਜੇ_ਅੰਤ

        ਜੇ (ਅਗੇਤਰ) {
            plen = ngx_strlen(ਅਗੇਤਰ);

        } ਨਹੀਂਤਾਂ {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NGX_ਅਗੇਤਰ
            ਅਗੇਤਰ = (u_char *) NGX_ਅਗੇਤਰ;
            plen = ngx_strlen(ਅਗੇਤਰ);
#ਨਹੀਂਤਾਂ
            plen = 0;
#ਜੇ_ਅੰਤ
        }

        ਜੇ (plen) {
            name = ਸਮ੍ਰਿਤੀ_ਦਿਓ(plen + nlen + 2);
            ਜੇ (name == ਨਿਰਾ) {
                ਵਾਪਸ ਨਿਰਾ;
            }

            p = ngx_cpymem(name, ਅਗੇਤਰ, plen);

            ਜੇ (!ngx_ਰਸਤਾ_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    ਜੇ (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#ਜੇ (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#ਜੇ_ਅੰਤ

        ngx_log_file.fd = ngx_stderr;
    }

    ਜੇ (p) {
        ngx_free(p);
    }

    ਵਾਪਸ &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_log_t  *ਲਘੂਗਣਕ;

    ਜੇ (ngx_log_get_file_log(&ਚੱਕਰ->new_log) != ਨਿਰਾ) {
        ਵਾਪਸ NGX_OK;
    }

    ਜੇ (ਚੱਕਰ->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        ਲਘੂਗਣਕ = ngx_pcalloc(ਚੱਕਰ->pool, ਆਕਾਰ(ngx_log_t));
        ਜੇ (ਲਘੂਗਣਕ == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

    } ਨਹੀਂਤਾਂ {
        /* no error logs at all */
        ਲਘੂਗਣਕ = &ਚੱਕਰ->new_log;
    }

    ਲਘੂਗਣਕ->log_level = NGX_LOG_ERR;

    ਲਘੂਗਣਕ->file = ngx_conf_open_file(ਚੱਕਰ, &ਚੱਕਰ->error_log);
    ਜੇ (ਲਘੂਗਣਕ->file == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ਲਘੂਗਣਕ != &ਚੱਕਰ->new_log) {
        ngx_log_ਪਾਓ(&ਚੱਕਰ->new_log, ਲਘੂਗਣਕ);
    }

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_fd_t  fd;

    ਜੇ (ਚੱਕਰ->log_use_stderr) {
        ਵਾਪਸ NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(ਚੱਕਰ->ਲਘੂਗਣਕ)->file->fd;

    ਜੇ (fd != ngx_stderr) {
        ਜੇ (ngx_ਸੈੱਟ_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_ਸੈੱਟ_stderr_n " failed");

            ਵਾਪਸ NGX_ERROR;
        }
    }

    ਵਾਪਸ NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *ਸਿਰ)
{
    ngx_log_t  *ਲਘੂਗਣਕ;

    ਲਈ (ਲਘੂਗਣਕ = ਸਿਰ; ਲਘੂਗਣਕ; ਲਘੂਗਣਕ = ਲਘੂਗਣਕ->ਅਗਲਾ) {
        ਜੇ (ਲਘੂਗਣਕ->file != ਨਿਰਾ) {
            ਵਾਪਸ ਲਘੂਗਣਕ;
        }
    }

    ਵਾਪਸ ਨਿਰਾ;
}


ਸਥਿਰ ਅੱਖਰ *
ngx_log_ਸੈੱਟ_ਪੱਧਰ(ngx_conf_t *cf, ngx_log_t *ਲਘੂਗਣਕ)
{
    ngx_uint_t   i, n, d, ਮਿਲਿਆ;
    ngx_str_t   *ਮੁੱਲ;

    ਜੇ (cf->ਦਲੀਲਾਂ->nelts == 2) {
        ਲਘੂਗਣਕ->log_level = NGX_LOG_ERR;
        ਵਾਪਸ NGX_CONF_OK;
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    ਲਈ (i = 2; i < cf->ਦਲੀਲਾਂ->nelts; i++) {
        ਮਿਲਿਆ = 0;

        ਲਈ (n = 1; n <= NGX_LOG_DEBUG; n++) {
            ਜੇ (ngx_strcmp(ਮੁੱਲ[i].ਅੰਕੜੇ, err_ਪੱਧਰ[n].ਅੰਕੜੇ) == 0) {

                ਜੇ (ਲਘੂਗਣਕ->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &ਮੁੱਲ[i]);
                    ਵਾਪਸ NGX_CONF_ERROR;
                }

                ਲਘੂਗਣਕ->log_level = n;
                ਮਿਲਿਆ = 1;
                ਰੋਕੋ;
            }
        }

        ਲਈ (n = 0, d = NGX_LOG_DEBUG_ਪਹਿਲਾ; d <= NGX_LOG_DEBUG_ਆਖਰੀ; d <<= 1) {
            ਜੇ (ngx_strcmp(ਮੁੱਲ[i].ਅੰਕੜੇ, debug_ਪੱਧਰ[n++]) == 0) {
                ਜੇ (ਲਘੂਗਣਕ->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &ਮੁੱਲ[i]);
                    ਵਾਪਸ NGX_CONF_ERROR;
                }

                ਲਘੂਗਣਕ->log_level |= d;
                ਮਿਲਿਆ = 1;
                ਰੋਕੋ;
            }
        }


        ਜੇ (!ਮਿਲਿਆ) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &ਮੁੱਲ[i]);
            ਵਾਪਸ NGX_CONF_ERROR;
        }
    }

    ਜੇ (ਲਘੂਗਣਕ->log_level == NGX_LOG_DEBUG) {
        ਲਘੂਗਣਕ->log_level = NGX_LOG_DEBUG_ALL;
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਸਥਿਰ ਅੱਖਰ *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->ਚੱਕਰ->new_log;

    ਵਾਪਸ ngx_log_ਸੈੱਟ_log(cf, &dummy);
}


ਅੱਖਰ *
ngx_log_ਸੈੱਟ_log(ngx_conf_t *cf, ngx_log_t **ਸਿਰ)
{
    ngx_log_t          *new_log;
    ngx_str_t          *ਮੁੱਲ, name;
    ngx_syslog_peer_t  *peer;

    ਜੇ (*ਸਿਰ != ਨਿਰਾ && (*ਸਿਰ)->log_level == 0) {
        new_log = *ਸਿਰ;

    } ਨਹੀਂਤਾਂ {

        new_log = ngx_pcalloc(cf->pool, ਆਕਾਰ(ngx_log_t));
        ਜੇ (new_log == ਨਿਰਾ) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        ਜੇ (*ਸਿਰ == ਨਿਰਾ) {
            *ਸਿਰ = new_log;
        }
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    ਜੇ (ngx_strcmp(ਮੁੱਲ[1].ਅੰਕੜੇ, "stderr") == 0) {
        ngx_str_null(&name);
        cf->ਚੱਕਰ->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->ਚੱਕਰ, &name);
        ਜੇ (new_log->file == ਨਿਰਾ) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }

    } ਨਹੀਂਤਾਂ ਜੇ (ngx_strncmp(ਮੁੱਲ[1].ਅੰਕੜੇ, "memory:", 7) == 0) {

#ਜੇ (NGX_DEBUG)
        ਮਾਪ_t                 ਮਾਪ, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_ਸਮ੍ਰਿਤੀ_buf_t  *buf;

        ਮੁੱਲ[1].len -= 7;
        ਮੁੱਲ[1].ਅੰਕੜੇ += 7;

        needed = ਆਕਾਰ("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_ਮਾਪ_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        ਮਾਪ = ngx_parse_ਮਾਪ(&ਮੁੱਲ[1]);

        ਜੇ (ਮਾਪ == (ਮਾਪ_t) NGX_ERROR || ਮਾਪ < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &ਮੁੱਲ[1]);
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, ਆਕਾਰ(ngx_log_ਸਮ੍ਰਿਤੀ_buf_t));
        ਜੇ (buf == ਨਿਰਾ) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        buf->ਸ਼ੁਰੂ = ngx_pnalloc(cf->pool, ਮਾਪ);
        ਜੇ (buf->ਸ਼ੁਰੂ == ਨਿਰਾ) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        buf->ਸਿਰਾ = buf->ਸ਼ੁਰੂ + ਮਾਪ;

        buf->pos = ngx_slprintf(buf->ਸ਼ੁਰੂ, buf->ਸਿਰਾ, "MEMLOG %uz %V:%ui%N",
                                ਮਾਪ, &cf->conf_file->file.name,
                                cf->conf_file->ਸਤਰ);

        ngx_memset(buf->pos, ' ', buf->ਸਿਰਾ - buf->pos);

        cln = ngx_pool_cleanup_ਜੋੜੋ(cf->pool, 0);
        ਜੇ (cln == ਨਿਰਾ) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        cln->ਅੰਕੜੇ = new_log;
        cln->handler = ngx_log_ਸਮ੍ਰਿਤੀ_cleanup;

        new_log->writer = ngx_log_ਸਮ੍ਰਿਤੀ_writer;
        new_log->wdata = buf;

#ਨਹੀਂਤਾਂ
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        ਵਾਪਸ NGX_CONF_ERROR;
#ਜੇ_ਅੰਤ

    } ਨਹੀਂਤਾਂ ਜੇ (ngx_strncmp(ਮੁੱਲ[1].ਅੰਕੜੇ, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, ਆਕਾਰ(ngx_syslog_peer_t));
        ਜੇ (peer == ਨਿਰਾ) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        ਜੇ (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } ਨਹੀਂਤਾਂ {
        new_log->file = ngx_conf_open_file(cf->ਚੱਕਰ, &ਮੁੱਲ[1]);
        ਜੇ (new_log->file == ਨਿਰਾ) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }
    }

    ਜੇ (ngx_log_ਸੈੱਟ_ਪੱਧਰ(cf, new_log) != NGX_CONF_OK) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਜੇ (*ਸਿਰ != new_log) {
        ngx_log_ਪਾਓ(*ਸਿਰ, new_log);
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਸਥਿਰ ਖਾਲੀ
ngx_log_ਪਾਓ(ngx_log_t *ਲਘੂਗਣਕ, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    ਜੇ (new_log->log_level > ਲਘੂਗਣਕ->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *ਲਘੂਗਣਕ;
        *ਲਘੂਗਣਕ = *new_log;
        *new_log = tmp;

        ਲਘੂਗਣਕ->ਅਗਲਾ = new_log;
        ਵਾਪਸ;
    }

    ਜਦੋਂ (ਲਘੂਗਣਕ->ਅਗਲਾ) {
        ਜੇ (new_log->log_level > ਲਘੂਗਣਕ->ਅਗਲਾ->log_level) {
            new_log->ਅਗਲਾ = ਲਘੂਗਣਕ->ਅਗਲਾ;
            ਲਘੂਗਣਕ->ਅਗਲਾ = new_log;
            ਵਾਪਸ;
        }

        ਲਘੂਗਣਕ = ਲਘੂਗਣਕ->ਅਗਲਾ;
    }

    ਲਘੂਗਣਕ->ਅਗਲਾ = new_log;
}


#ਜੇ (NGX_DEBUG)

ਸਥਿਰ ਖਾਲੀ
ngx_log_ਸਮ੍ਰਿਤੀ_writer(ngx_log_t *ਲਘੂਗਣਕ, ngx_uint_t level, u_char *buf,
    ਮਾਪ_t len)
{
    u_char                *p;
    ਮਾਪ_t                 avail, written;
    ngx_log_ਸਮ੍ਰਿਤੀ_buf_t  *mem;

    mem = ਲਘੂਗਣਕ->wdata;

    ਜੇ (mem == ਨਿਰਾ) {
        ਵਾਪਸ;
    }

    written = ngx_atomic_fetch_ਜੋੜੋ(&mem->written, len);

    p = mem->pos + written % (mem->ਸਿਰਾ - mem->pos);

    avail = mem->ਸਿਰਾ - p;

    ਜੇ (avail >= len) {
        ngx_memcpy(p, buf, len);

    } ਨਹੀਂਤਾਂ {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


ਸਥਿਰ ਖਾਲੀ
ngx_log_ਸਮ੍ਰਿਤੀ_cleanup(ਖਾਲੀ *ਅੰਕੜੇ)
{
    ngx_log_t *ਲਘੂਗਣਕ = ਅੰਕੜੇ;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ਲਘੂਗਣਕ, 0, "destroy memory log buffer");

    ਲਘੂਗਣਕ->wdata = ਨਿਰਾ;
}

#ਜੇ_ਅੰਤ
