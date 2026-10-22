
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


സ്ഥിതം അക്ഷരം *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
സ്ഥിതം അക്ഷരം *ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log);
സ്ഥിതം ശൂന്യം ngx_log_തിരുകുക(ngx_log_t *log, ngx_log_t *new_log);


#എങ്കിൽ (NGX_DEBUG)

സ്ഥിതം ശൂന്യം ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level,
    u_char *buf, size_t len);
സ്ഥിതം ശൂന്യം ngx_log_memory_cleanup(ശൂന്യം *data);


തരനിർവചനം ഘടന {
    u_char        *start;
    u_char        *end;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_memory_buf_t;

#endif


സ്ഥിതം ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1MORE,
      ngx_error_log,
      0,
      0,
      ശൂന്യ },

      ngx_null_command
};


സ്ഥിതം ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    ശൂന്യ,
    ശൂന്യ
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ശൂന്യ,                                  /* init master */
    ശൂന്യ,                                  /* init module */
    ശൂന്യ,                                  /* init process */
    ശൂന്യ,                                  /* init thread */
    ശൂന്യ,                                  /* exit thread */
    ശൂന്യ,                                  /* exit process */
    ശൂന്യ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


സ്ഥിതം ngx_log_t        ngx_log;
സ്ഥിതം ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


സ്ഥിതം ngx_str_t err_levels[] = {
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

സ്ഥിതം സ്ഥിരം അക്ഷരം *debug_levels[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#എങ്കിൽ (NGX_HAVE_VARIADIC_MACROS)

ശൂന്യം
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    സ്ഥിരം അക്ഷരം *fmt, ...)

#അല്ലെങ്കിൽ

ശൂന്യം
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    സ്ഥിരം അക്ഷരം *fmt, va_list args)

#endif
{
#എങ്കിൽ (NGX_HAVE_VARIADIC_MACROS)
    va_list      args;
#endif
    u_char      *p, *last, *സന്ദേശം;
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

    എങ്കിൽ (log->connection) {
        p = ngx_slprintf(p, last, "*%uA ", log->connection);
    }

    സന്ദേശം = p;

#എങ്കിൽ (NGX_HAVE_VARIADIC_MACROS)

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

#അല്ലെങ്കിൽ

    p = ngx_vslprintf(p, last, fmt, args);

#endif

    എങ്കിൽ (err) {
        p = ngx_log_errno(p, last, err);
    }

    എങ്കിൽ (level != NGX_LOG_DEBUG && log->handler) {
        p = log->handler(log, p, last - p);
    }

    എങ്കിൽ (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (log->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    ആകുമ്പോൾ (log) {

        എങ്കിൽ (log->log_level < level && !debug_connection) {
            നിർത്തുക;
        }

        എങ്കിൽ (log->writer) {
            log->writer(log, level, errstr, p - errstr);
            പോകുക next;
        }

        എങ്കിൽ (ngx_time() == log->disk_full_time) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            പോകുക next;
        }

        n = ngx_write_fd(log->file->fd, errstr, p - errstr);

        എങ്കിൽ (n == -1 && ngx_errno == NGX_ENOSPC) {
            log->disk_full_time = ngx_time();
        }

        എങ്കിൽ (log->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    next:

        log = log->next;
    }

    എങ്കിൽ (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        തിരികെ;
    }

    സന്ദേശം -= (7 + err_levels[level].len + 3);

    (ശൂന്യം) ngx_sprintf(സന്ദേശം, "nginx: [%V] ", &err_levels[level]);

    (ശൂന്യം) ngx_write_console(ngx_stderr, സന്ദേശം, p - സന്ദേശം);
}


#എങ്കിൽ !(NGX_HAVE_VARIADIC_MACROS)

ശൂന്യം ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    സ്ഥിരം അക്ഷരം *fmt, ...)
{
    va_list  args;

    എങ്കിൽ (log->log_level >= level) {
        va_start(args, fmt);
        ngx_log_error_core(level, log, err, fmt, args);
        va_end(args);
    }
}


ശൂന്യം ngx_cdecl
ngx_log_debug_core(ngx_log_t *log, ngx_err_t err, സ്ഥിരം അക്ഷരം *fmt, ...)
{
    va_list  args;

    va_start(args, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, log, err, fmt, args);
    va_end(args);
}

#endif


ശൂന്യം ngx_cdecl
ngx_log_abort(ngx_err_t err, സ്ഥിരം അക്ഷരം *fmt, ...)
{
    u_char   *p;
    va_list   args;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_start(args, fmt);
    p = ngx_vsnprintf(errstr, വലുപ്പം(errstr) - 1, fmt, args);
    va_end(args);

    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                  "%*s", p - errstr, errstr);
}


ശൂന്യം ngx_cdecl
ngx_log_stderr(ngx_err_t err, സ്ഥിരം അക്ഷരം *fmt, ...)
{
    u_char   *p, *last;
    va_list   args;
    u_char    errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

    എങ്കിൽ (err) {
        p = ngx_log_errno(p, last, err);
    }

    എങ്കിൽ (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    (ശൂന്യം) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err)
{
    എങ്കിൽ (buf > last - 50) {

        /* leave a space for an error code */

        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#എങ്കിൽ (NGX_WIN32)
    buf = ngx_slprintf(buf, last, ((unsigned) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#അല്ലെങ്കിൽ
    buf = ngx_slprintf(buf, last, " (%d: ", err);
#endif

    buf = ngx_strerror(err, buf, last - buf);

    എങ്കിൽ (buf < last) {
        *buf++ = ')';
    }

    തിരികെ buf;
}


ngx_log_t *
ngx_log_init(u_char *prefix, u_char *error_log)
{
    u_char  *p, *name;
    size_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    എങ്കിൽ (error_log == ശൂന്യ) {
        error_log = (u_char *) NGX_ERROR_LOG_PATH;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    എങ്കിൽ (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        തിരികെ &ngx_log;
    }

    p = ശൂന്യ;

#എങ്കിൽ (NGX_WIN32)
    എങ്കിൽ (name[1] != ':') {
#അല്ലെങ്കിൽ
    എങ്കിൽ (name[0] != '/') {
#endif

        എങ്കിൽ (prefix) {
            plen = ngx_strlen(prefix);

        } അല്ലെങ്കിൽ {
#ifdef NGX_PREFIX
            prefix = (u_char *) NGX_PREFIX;
            plen = ngx_strlen(prefix);
#അല്ലെങ്കിൽ
            plen = 0;
#endif
        }

        എങ്കിൽ (plen) {
            name = malloc(plen + nlen + 2);
            എങ്കിൽ (name == ശൂന്യ) {
                തിരികെ ശൂന്യ;
            }

            p = ngx_cpymem(name, prefix, plen);

            എങ്കിൽ (!ngx_path_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    എങ്കിൽ (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#എങ്കിൽ (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#endif

        ngx_log_file.fd = ngx_stderr;
    }

    എങ്കിൽ (p) {
        ngx_free(p);
    }

    തിരികെ &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_cycle_t *cycle)
{
    ngx_log_t  *log;

    എങ്കിൽ (ngx_log_get_file_log(&cycle->new_log) != ശൂന്യ) {
        തിരികെ NGX_OK;
    }

    എങ്കിൽ (cycle->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        log = ngx_pcalloc(cycle->pool, വലുപ്പം(ngx_log_t));
        എങ്കിൽ (log == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

    } അല്ലെങ്കിൽ {
        /* no error logs at all */
        log = &cycle->new_log;
    }

    log->log_level = NGX_LOG_ERR;

    log->file = ngx_conf_open_file(cycle, &cycle->error_log);
    എങ്കിൽ (log->file == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    എങ്കിൽ (log != &cycle->new_log) {
        ngx_log_തിരുകുക(&cycle->new_log, log);
    }

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_cycle_t *cycle)
{
    ngx_fd_t  fd;

    എങ്കിൽ (cycle->log_use_stderr) {
        തിരികെ NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(cycle->log)->file->fd;

    എങ്കിൽ (fd != ngx_stderr) {
        എങ്കിൽ (ngx_set_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");

            തിരികെ NGX_ERROR;
        }
    }

    തിരികെ NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *head)
{
    ngx_log_t  *log;

    വേണ്ടി (log = head; log; log = log->next) {
        എങ്കിൽ (log->file != ശൂന്യ) {
            തിരികെ log;
        }
    }

    തിരികെ ശൂന്യ;
}


സ്ഥിതം അക്ഷരം *
ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log)
{
    ngx_uint_t   i, n, d, found;
    ngx_str_t   *മൂല്യം;

    എങ്കിൽ (cf->args->nelts == 2) {
        log->log_level = NGX_LOG_ERR;
        തിരികെ NGX_CONF_OK;
    }

    മൂല്യം = cf->args->elts;

    വേണ്ടി (i = 2; i < cf->args->nelts; i++) {
        found = 0;

        വേണ്ടി (n = 1; n <= NGX_LOG_DEBUG; n++) {
            എങ്കിൽ (ngx_strcmp(മൂല്യം[i].data, err_levels[n].data) == 0) {

                എങ്കിൽ (log->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &മൂല്യം[i]);
                    തിരികെ NGX_CONF_ERROR;
                }

                log->log_level = n;
                found = 1;
                നിർത്തുക;
            }
        }

        വേണ്ടി (n = 0, d = NGX_LOG_DEBUG_FIRST; d <= NGX_LOG_DEBUG_LAST; d <<= 1) {
            എങ്കിൽ (ngx_strcmp(മൂല്യം[i].data, debug_levels[n++]) == 0) {
                എങ്കിൽ (log->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &മൂല്യം[i]);
                    തിരികെ NGX_CONF_ERROR;
                }

                log->log_level |= d;
                found = 1;
                നിർത്തുക;
            }
        }


        എങ്കിൽ (!found) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &മൂല്യം[i]);
            തിരികെ NGX_CONF_ERROR;
        }
    }

    എങ്കിൽ (log->log_level == NGX_LOG_DEBUG) {
        log->log_level = NGX_LOG_DEBUG_ALL;
    }

    തിരികെ NGX_CONF_OK;
}


സ്ഥിതം അക്ഷരം *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->cycle->new_log;

    തിരികെ ngx_log_set_log(cf, &dummy);
}


അക്ഷരം *
ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **head)
{
    ngx_log_t          *new_log;
    ngx_str_t          *മൂല്യം, name;
    ngx_syslog_peer_t  *peer;

    എങ്കിൽ (*head != ശൂന്യ && (*head)->log_level == 0) {
        new_log = *head;

    } അല്ലെങ്കിൽ {

        new_log = ngx_pcalloc(cf->pool, വലുപ്പം(ngx_log_t));
        എങ്കിൽ (new_log == ശൂന്യ) {
            തിരികെ NGX_CONF_ERROR;
        }

        എങ്കിൽ (*head == ശൂന്യ) {
            *head = new_log;
        }
    }

    മൂല്യം = cf->args->elts;

    എങ്കിൽ (ngx_strcmp(മൂല്യം[1].data, "stderr") == 0) {
        ngx_str_null(&name);
        cf->cycle->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->cycle, &name);
        എങ്കിൽ (new_log->file == ശൂന്യ) {
            തിരികെ NGX_CONF_ERROR;
        }

    } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strncmp(മൂല്യം[1].data, "memory:", 7) == 0) {

#എങ്കിൽ (NGX_DEBUG)
        size_t                 size, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_memory_buf_t  *buf;

        മൂല്യം[1].len -= 7;
        മൂല്യം[1].data += 7;

        needed = വലുപ്പം("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_SIZE_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        size = ngx_parse_size(&മൂല്യം[1]);

        എങ്കിൽ (size == (size_t) NGX_ERROR || size < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &മൂല്യം[1]);
            തിരികെ NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, വലുപ്പം(ngx_log_memory_buf_t));
        എങ്കിൽ (buf == ശൂന്യ) {
            തിരികെ NGX_CONF_ERROR;
        }

        buf->start = ngx_pnalloc(cf->pool, size);
        എങ്കിൽ (buf->start == ശൂന്യ) {
            തിരികെ NGX_CONF_ERROR;
        }

        buf->end = buf->start + size;

        buf->pos = ngx_slprintf(buf->start, buf->end, "MEMLOG %uz %V:%ui%N",
                                size, &cf->conf_file->file.name,
                                cf->conf_file->വരി);

        ngx_memset(buf->pos, ' ', buf->end - buf->pos);

        cln = ngx_pool_cleanup_add(cf->pool, 0);
        എങ്കിൽ (cln == ശൂന്യ) {
            തിരികെ NGX_CONF_ERROR;
        }

        cln->data = new_log;
        cln->handler = ngx_log_memory_cleanup;

        new_log->writer = ngx_log_memory_writer;
        new_log->wdata = buf;

#അല്ലെങ്കിൽ
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        തിരികെ NGX_CONF_ERROR;
#endif

    } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strncmp(മൂല്യം[1].data, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, വലുപ്പം(ngx_syslog_peer_t));
        എങ്കിൽ (peer == ശൂന്യ) {
            തിരികെ NGX_CONF_ERROR;
        }

        എങ്കിൽ (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            തിരികെ NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } അല്ലെങ്കിൽ {
        new_log->file = ngx_conf_open_file(cf->cycle, &മൂല്യം[1]);
        എങ്കിൽ (new_log->file == ശൂന്യ) {
            തിരികെ NGX_CONF_ERROR;
        }
    }

    എങ്കിൽ (ngx_log_set_levels(cf, new_log) != NGX_CONF_OK) {
        തിരികെ NGX_CONF_ERROR;
    }

    എങ്കിൽ (*head != new_log) {
        ngx_log_തിരുകുക(*head, new_log);
    }

    തിരികെ NGX_CONF_OK;
}


സ്ഥിതം ശൂന്യം
ngx_log_തിരുകുക(ngx_log_t *log, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    എങ്കിൽ (new_log->log_level > log->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *log;
        *log = *new_log;
        *new_log = tmp;

        log->next = new_log;
        തിരികെ;
    }

    ആകുമ്പോൾ (log->next) {
        എങ്കിൽ (new_log->log_level > log->next->log_level) {
            new_log->next = log->next;
            log->next = new_log;
            തിരികെ;
        }

        log = log->next;
    }

    log->next = new_log;
}


#എങ്കിൽ (NGX_DEBUG)

സ്ഥിതം ശൂന്യം
ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char                *p;
    size_t                 avail, written;
    ngx_log_memory_buf_t  *mem;

    mem = log->wdata;

    എങ്കിൽ (mem == ശൂന്യ) {
        തിരികെ;
    }

    written = ngx_atomic_fetch_add(&mem->written, len);

    p = mem->pos + written % (mem->end - mem->pos);

    avail = mem->end - p;

    എങ്കിൽ (avail >= len) {
        ngx_memcpy(p, buf, len);

    } അല്ലെങ്കിൽ {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


സ്ഥിതം ശൂന്യം
ngx_log_memory_cleanup(ശൂന്യം *data)
{
    ngx_log_t *log = data;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, log, 0, "destroy memory log buffer");

    log->wdata = ശൂന്യ;
}

#endif
