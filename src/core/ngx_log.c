
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ ವರ್ಣ *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf);
ಸ್ಥಿರ ವರ್ಣ *ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_log_ಸೇರಿಸು(ngx_log_t *log, ngx_log_t *new_log);


#ಯದಿ (NGX_DEBUG)

ಸ್ಥಿರ ಶೂನ್ಯ ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level,
    u_char *buf, size_t len);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_log_memory_cleanup(ಶೂನ್ಯ *data);


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    u_char        *start;
    u_char        *end;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_memory_buf_t;

#endif


ಸ್ಥಿರ ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1MORE,
      ngx_error_log,
      0,
      0,
      ರಿಕ್ತ },

      ngx_null_command
};


ಸ್ಥಿರ ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    ರಿಕ್ತ,
    ರಿಕ್ತ
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ರಿಕ್ತ,                                  /* init master */
    ರಿಕ್ತ,                                  /* init module */
    ರಿಕ್ತ,                                  /* init process */
    ರಿಕ್ತ,                                  /* init thread */
    ರಿಕ್ತ,                                  /* exit thread */
    ರಿಕ್ತ,                                  /* exit process */
    ರಿಕ್ತ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ಸ್ಥಿರ ngx_log_t        ngx_log;
ಸ್ಥಿರ ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


ಸ್ಥಿರ ngx_str_t err_levels[] = {
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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *debug_levels[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#ಯದಿ (NGX_HAVE_VARIADIC_MACROS)

ಶೂನ್ಯ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ಧ್ರುವ ವರ್ಣ *fmt, ...)

#ಅನ್ಯಥಾ

ಶೂನ್ಯ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ಧ್ರುವ ವರ್ಣ *fmt, va_list args)

#endif
{
#ಯದಿ (NGX_HAVE_VARIADIC_MACROS)
    va_list      args;
#endif
    u_char      *p, *last, *ಸಂದೇಶ;
    ssize_t      n;
    ngx_uint_t   wrote_stderr, debug_connection;
    u_char       errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, ngx_cached_err_log_time.data,
                   ngx_cached_err_log_time.len);

    p = ngx_slprintf(p, last, " [%V] ", &err_levels[level]);

    /* pid#tid */
    p = ngx_slprintf(p, last, "%P#" NGX_TID_T_FMT ": ",
                    ngx_log_pid, ngx_log_tid);

    ಯದಿ (log->connection) {
        p = ngx_slprintf(p, last, "*%uA ", log->connection);
    }

    ಸಂದೇಶ = p;

#ಯದಿ (NGX_HAVE_VARIADIC_MACROS)

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

#ಅನ್ಯಥಾ

    p = ngx_vslprintf(p, last, fmt, args);

#endif

    ಯದಿ (err) {
        p = ngx_log_errno(p, last, err);
    }

    ಯದಿ (level != NGX_LOG_DEBUG && log->handler) {
        p = log->handler(log, p, last - p);
    }

    ಯದಿ (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (log->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    ಯಾವತ್ (log) {

        ಯದಿ (log->log_level < level && !debug_connection) {
            ಭಙ್ಗ;
        }

        ಯದಿ (log->writer) {
            log->writer(log, level, errstr, p - errstr);
            ಗಚ್ಛ next;
        }

        ಯದಿ (ngx_time() == log->disk_full_time) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            ಗಚ್ಛ next;
        }

        n = ngx_write_fd(log->file->fd, errstr, p - errstr);

        ಯದಿ (n == -1 && ngx_errno == NGX_ENOSPC) {
            log->disk_full_time = ngx_time();
        }

        ಯದಿ (log->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    next:

        log = log->next;
    }

    ಯದಿ (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಸಂದೇಶ -= (7 + err_levels[level].len + 3);

    (ಶೂನ್ಯ) ngx_sprintf(ಸಂದೇಶ, "nginx: [%V] ", &err_levels[level]);

    (ಶೂನ್ಯ) ngx_write_console(ngx_stderr, ಸಂದೇಶ, p - ಸಂದೇಶ);
}


#ಯದಿ !(NGX_HAVE_VARIADIC_MACROS)

ಶೂನ್ಯ ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
    va_list  args;

    ಯದಿ (log->log_level >= level) {
        va_start(args, fmt);
        ngx_log_error_core(level, log, err, fmt, args);
        va_end(args);
    }
}


ಶೂನ್ಯ ngx_cdecl
ngx_log_debug_core(ngx_log_t *log, ngx_err_t err, ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
    va_list  args;

    va_start(args, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, log, err, fmt, args);
    va_end(args);
}

#endif


ಶೂನ್ಯ ngx_cdecl
ngx_log_abort(ngx_err_t err, ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
    u_char   *p;
    va_list   args;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_start(args, fmt);
    p = ngx_vsnprintf(errstr, ಪರಿಮಾಣ(errstr) - 1, fmt, args);
    va_end(args);

    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                  "%*s", p - errstr, errstr);
}


ಶೂನ್ಯ ngx_cdecl
ngx_log_stderr(ngx_err_t err, ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
    u_char   *p, *last;
    va_list   args;
    u_char    errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

    ಯದಿ (err) {
        p = ngx_log_errno(p, last, err);
    }

    ಯದಿ (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    (ಶೂನ್ಯ) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err)
{
    ಯದಿ (buf > last - 50) {

        /* leave a space for an error code */

        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#ಯದಿ (NGX_WIN32)
    buf = ngx_slprintf(buf, last, ((unsigned) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#ಅನ್ಯಥಾ
    buf = ngx_slprintf(buf, last, " (%d: ", err);
#endif

    buf = ngx_strerror(err, buf, last - buf);

    ಯದಿ (buf < last) {
        *buf++ = ')';
    }

    ಪ್ರತ್ಯಾಗಮ buf;
}


ngx_log_t *
ngx_log_init(u_char *prefix, u_char *error_log)
{
    u_char  *p, *name;
    size_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    ಯದಿ (error_log == ರಿಕ್ತ) {
        error_log = (u_char *) NGX_ERROR_LOG_PATH;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    ಯದಿ (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        ಪ್ರತ್ಯಾಗಮ &ngx_log;
    }

    p = ರಿಕ್ತ;

#ಯದಿ (NGX_WIN32)
    ಯದಿ (name[1] != ':') {
#ಅನ್ಯಥಾ
    ಯದಿ (name[0] != '/') {
#endif

        ಯದಿ (prefix) {
            plen = ngx_strlen(prefix);

        } ಅನ್ಯಥಾ {
#ifdef NGX_PREFIX
            prefix = (u_char *) NGX_PREFIX;
            plen = ngx_strlen(prefix);
#ಅನ್ಯಥಾ
            plen = 0;
#endif
        }

        ಯದಿ (plen) {
            name = malloc(plen + nlen + 2);
            ಯದಿ (name == ರಿಕ್ತ) {
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }

            p = ngx_cpymem(name, prefix, plen);

            ಯದಿ (!ngx_path_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    ಯದಿ (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#ಯದಿ (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#endif

        ngx_log_file.fd = ngx_stderr;
    }

    ಯದಿ (p) {
        ngx_free(p);
    }

    ಪ್ರತ್ಯಾಗಮ &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_cycle_t *cycle)
{
    ngx_log_t  *log;

    ಯದಿ (ngx_log_get_file_log(&cycle->new_log) != ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಯದಿ (cycle->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        log = ngx_pcalloc(cycle->pool, ಪರಿಮಾಣ(ngx_log_t));
        ಯದಿ (log == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

    } ಅನ್ಯಥಾ {
        /* no error logs at all */
        log = &cycle->new_log;
    }

    log->log_level = NGX_LOG_ERR;

    log->file = ngx_conf_open_file(cycle, &cycle->error_log);
    ಯದಿ (log->file == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (log != &cycle->new_log) {
        ngx_log_ಸೇರಿಸು(&cycle->new_log, log);
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_cycle_t *cycle)
{
    ngx_fd_t  fd;

    ಯದಿ (cycle->log_use_stderr) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(cycle->log)->file->fd;

    ಯದಿ (fd != ngx_stderr) {
        ಯದಿ (ngx_set_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");

            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *head)
{
    ngx_log_t  *log;

    ಪ್ರತಿ (log = head; log; log = log->next) {
        ಯದಿ (log->file != ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ log;
        }
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


ಸ್ಥಿರ ವರ್ಣ *
ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log)
{
    ngx_uint_t   i, n, d, found;
    ngx_str_t   *ಮೌಲ್ಯ;

    ಯದಿ (cf->args->nelts == 2) {
        log->log_level = NGX_LOG_ERR;
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
    }

    ಮೌಲ್ಯ = cf->args->elts;

    ಪ್ರತಿ (i = 2; i < cf->args->nelts; i++) {
        found = 0;

        ಪ್ರತಿ (n = 1; n <= NGX_LOG_DEBUG; n++) {
            ಯದಿ (ngx_strcmp(ಮೌಲ್ಯ[i].data, err_levels[n].data) == 0) {

                ಯದಿ (log->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &ಮೌಲ್ಯ[i]);
                    ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
                }

                log->log_level = n;
                found = 1;
                ಭಙ್ಗ;
            }
        }

        ಪ್ರತಿ (n = 0, d = NGX_LOG_DEBUG_FIRST; d <= NGX_LOG_DEBUG_LAST; d <<= 1) {
            ಯದಿ (ngx_strcmp(ಮೌಲ್ಯ[i].data, debug_levels[n++]) == 0) {
                ಯದಿ (log->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &ಮೌಲ್ಯ[i]);
                    ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
                }

                log->log_level |= d;
                found = 1;
                ಭಙ್ಗ;
            }
        }


        ಯದಿ (!found) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &ಮೌಲ್ಯ[i]);
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }
    }

    ಯದಿ (log->log_level == NGX_LOG_DEBUG) {
        log->log_level = NGX_LOG_DEBUG_ALL;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ಸ್ಥಿರ ವರ್ಣ *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->cycle->new_log;

    ಪ್ರತ್ಯಾಗಮ ngx_log_set_log(cf, &dummy);
}


ವರ್ಣ *
ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **head)
{
    ngx_log_t          *new_log;
    ngx_str_t          *ಮೌಲ್ಯ, name;
    ngx_syslog_peer_t  *peer;

    ಯದಿ (*head != ರಿಕ್ತ && (*head)->log_level == 0) {
        new_log = *head;

    } ಅನ್ಯಥಾ {

        new_log = ngx_pcalloc(cf->pool, ಪರಿಮಾಣ(ngx_log_t));
        ಯದಿ (new_log == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }

        ಯದಿ (*head == ರಿಕ್ತ) {
            *head = new_log;
        }
    }

    ಮೌಲ್ಯ = cf->args->elts;

    ಯದಿ (ngx_strcmp(ಮೌಲ್ಯ[1].data, "stderr") == 0) {
        ngx_str_null(&name);
        cf->cycle->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->cycle, &name);
        ಯದಿ (new_log->file == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }

    } ಅನ್ಯಥಾ ಯದಿ (ngx_strncmp(ಮೌಲ್ಯ[1].data, "memory:", 7) == 0) {

#ಯದಿ (NGX_DEBUG)
        size_t                 size, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_memory_buf_t  *buf;

        ಮೌಲ್ಯ[1].len -= 7;
        ಮೌಲ್ಯ[1].data += 7;

        needed = ಪರಿಮಾಣ("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_SIZE_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        size = ngx_parse_size(&ಮೌಲ್ಯ[1]);

        ಯದಿ (size == (size_t) NGX_ERROR || size < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &ಮೌಲ್ಯ[1]);
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, ಪರಿಮಾಣ(ngx_log_memory_buf_t));
        ಯದಿ (buf == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }

        buf->start = ngx_pnalloc(cf->pool, size);
        ಯದಿ (buf->start == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }

        buf->end = buf->start + size;

        buf->pos = ngx_slprintf(buf->start, buf->end, "MEMLOG %uz %V:%ui%N",
                                size, &cf->conf_file->file.name,
                                cf->conf_file->ಸಾಲು);

        ngx_memset(buf->pos, ' ', buf->end - buf->pos);

        cln = ngx_pool_cleanup_add(cf->pool, 0);
        ಯದಿ (cln == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }

        cln->data = new_log;
        cln->handler = ngx_log_memory_cleanup;

        new_log->writer = ngx_log_memory_writer;
        new_log->wdata = buf;

#ಅನ್ಯಥಾ
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
#endif

    } ಅನ್ಯಥಾ ಯದಿ (ngx_strncmp(ಮೌಲ್ಯ[1].data, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, ಪರಿಮಾಣ(ngx_syslog_peer_t));
        ಯದಿ (peer == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }

        ಯದಿ (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } ಅನ್ಯಥಾ {
        new_log->file = ngx_conf_open_file(cf->cycle, &ಮೌಲ್ಯ[1]);
        ಯದಿ (new_log->file == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }
    }

    ಯದಿ (ngx_log_set_levels(cf, new_log) != NGX_CONF_OK) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಯದಿ (*head != new_log) {
        ngx_log_ಸೇರಿಸು(*head, new_log);
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_log_ಸೇರಿಸು(ngx_log_t *log, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    ಯದಿ (new_log->log_level > log->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *log;
        *log = *new_log;
        *new_log = tmp;

        log->next = new_log;
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯಾವತ್ (log->next) {
        ಯದಿ (new_log->log_level > log->next->log_level) {
            new_log->next = log->next;
            log->next = new_log;
            ಪ್ರತ್ಯಾಗಮ;
        }

        log = log->next;
    }

    log->next = new_log;
}


#ಯದಿ (NGX_DEBUG)

ಸ್ಥಿರ ಶೂನ್ಯ
ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char                *p;
    size_t                 avail, written;
    ngx_log_memory_buf_t  *mem;

    mem = log->wdata;

    ಯದಿ (mem == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    written = ngx_atomic_fetch_add(&mem->written, len);

    p = mem->pos + written % (mem->end - mem->pos);

    avail = mem->end - p;

    ಯದಿ (avail >= len) {
        ngx_memcpy(p, buf, len);

    } ಅನ್ಯಥಾ {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_log_memory_cleanup(ಶೂನ್ಯ *data)
{
    ngx_log_t *log = data;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, log, 0, "destroy memory log buffer");

    log->wdata = ರಿಕ್ತ;
}

#endif
