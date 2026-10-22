
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن حرف *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log);
ساكن فراغ ngx_log_insert(ngx_log_t *log, ngx_log_t *new_log);


#ܐܢ (NGX_DEBUG)

ساكن فراغ ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level,
    u_char *buf, size_t len);
ساكن فراغ ngx_log_memory_cleanup(فراغ *data);


تعريفنوع بنية {
    u_char        *start;
    u_char        *end;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_memory_buf_t;

#endif


ساكن ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1MORE,
      ngx_error_log,
      0,
      0,
      عدم },

      ngx_null_command
};


ساكن ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    عدم,
    عدم
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    عدم,                                  /* init master */
    عدم,                                  /* init module */
    عدم,                                  /* init process */
    عدم,                                  /* init thread */
    عدم,                                  /* exit thread */
    عدم,                                  /* exit process */
    عدم,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ساكن ngx_log_t        ngx_log;
ساكن ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


ساكن ngx_str_t err_levels[] = {
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

ساكن ثابت حرف *debug_levels[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#ܐܢ (NGX_HAVE_VARIADIC_MACROS)

فراغ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ثابت حرف *fmt, ...)

#ܐܚܪܢܐ

فراغ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ثابت حرف *fmt, va_list args)

#endif
{
#ܐܢ (NGX_HAVE_VARIADIC_MACROS)
    va_list      args;
#endif
    u_char      *p, *last, *msg;
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

    ܐܢ (log->connection) {
        p = ngx_slprintf(p, last, "*%uA ", log->connection);
    }

    msg = p;

#ܐܢ (NGX_HAVE_VARIADIC_MACROS)

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

#ܐܚܪܢܐ

    p = ngx_vslprintf(p, last, fmt, args);

#endif

    ܐܢ (err) {
        p = ngx_log_errno(p, last, err);
    }

    ܐܢ (level != NGX_LOG_DEBUG && log->handler) {
        p = log->handler(log, p, last - p);
    }

    ܐܢ (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (log->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    ܟܡܐ (log) {

        ܐܢ (log->log_level < level && !debug_connection) {
            اكسر;
        }

        ܐܢ (log->writer) {
            log->writer(log, level, errstr, p - errstr);
            اذهب next;
        }

        ܐܢ (ngx_time() == log->disk_full_time) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            اذهب next;
        }

        n = ngx_write_fd(log->file->fd, errstr, p - errstr);

        ܐܢ (n == -1 && ngx_errno == NGX_ENOSPC) {
            log->disk_full_time = ngx_time();
        }

        ܐܢ (log->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    next:

        log = log->next;
    }

    ܐܢ (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        ܦܢܐ;
    }

    msg -= (7 + err_levels[level].len + 3);

    (فراغ) ngx_sprintf(msg, "nginx: [%V] ", &err_levels[level]);

    (فراغ) ngx_write_console(ngx_stderr, msg, p - msg);
}


#ܐܢ !(NGX_HAVE_VARIADIC_MACROS)

فراغ ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    va_list  args;

    ܐܢ (log->log_level >= level) {
        va_start(args, fmt);
        ngx_log_error_core(level, log, err, fmt, args);
        va_end(args);
    }
}


فراغ ngx_cdecl
ngx_log_debug_core(ngx_log_t *log, ngx_err_t err, ثابت حرف *fmt, ...)
{
    va_list  args;

    va_start(args, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, log, err, fmt, args);
    va_end(args);
}

#endif


فراغ ngx_cdecl
ngx_log_abort(ngx_err_t err, ثابت حرف *fmt, ...)
{
    u_char   *p;
    va_list   args;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_start(args, fmt);
    p = ngx_vsnprintf(errstr, حجم(errstr) - 1, fmt, args);
    va_end(args);

    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                  "%*s", p - errstr, errstr);
}


فراغ ngx_cdecl
ngx_log_stderr(ngx_err_t err, ثابت حرف *fmt, ...)
{
    u_char   *p, *last;
    va_list   args;
    u_char    errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

    ܐܢ (err) {
        p = ngx_log_errno(p, last, err);
    }

    ܐܢ (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    (فراغ) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err)
{
    ܐܢ (buf > last - 50) {

        /* leave a space for an error code */

        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#ܐܢ (NGX_WIN32)
    buf = ngx_slprintf(buf, last, ((unsigned) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#ܐܚܪܢܐ
    buf = ngx_slprintf(buf, last, " (%d: ", err);
#endif

    buf = ngx_strerror(err, buf, last - buf);

    ܐܢ (buf < last) {
        *buf++ = ')';
    }

    ܦܢܐ buf;
}


ngx_log_t *
ngx_log_init(u_char *prefix, u_char *error_log)
{
    u_char  *p, *name;
    size_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    ܐܢ (error_log == عدم) {
        error_log = (u_char *) NGX_ERROR_LOG_PATH;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    ܐܢ (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        ܦܢܐ &ngx_log;
    }

    p = عدم;

#ܐܢ (NGX_WIN32)
    ܐܢ (name[1] != ':') {
#ܐܚܪܢܐ
    ܐܢ (name[0] != '/') {
#endif

        ܐܢ (prefix) {
            plen = ngx_strlen(prefix);

        } ܐܚܪܢܐ {
#ifdef NGX_PREFIX
            prefix = (u_char *) NGX_PREFIX;
            plen = ngx_strlen(prefix);
#ܐܚܪܢܐ
            plen = 0;
#endif
        }

        ܐܢ (plen) {
            name = malloc(plen + nlen + 2);
            ܐܢ (name == عدم) {
                ܦܢܐ عدم;
            }

            p = ngx_cpymem(name, prefix, plen);

            ܐܢ (!ngx_path_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    ܐܢ (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#ܐܢ (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#endif

        ngx_log_file.fd = ngx_stderr;
    }

    ܐܢ (p) {
        ngx_free(p);
    }

    ܦܢܐ &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_cycle_t *cycle)
{
    ngx_log_t  *log;

    ܐܢ (ngx_log_get_file_log(&cycle->new_log) != عدم) {
        ܦܢܐ NGX_OK;
    }

    ܐܢ (cycle->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        log = ngx_pcalloc(cycle->pool, حجم(ngx_log_t));
        ܐܢ (log == عدم) {
            ܦܢܐ NGX_ERROR;
        }

    } ܐܚܪܢܐ {
        /* no error logs at all */
        log = &cycle->new_log;
    }

    log->log_level = NGX_LOG_ERR;

    log->file = ngx_conf_open_file(cycle, &cycle->error_log);
    ܐܢ (log->file == عدم) {
        ܦܢܐ NGX_ERROR;
    }

    ܐܢ (log != &cycle->new_log) {
        ngx_log_insert(&cycle->new_log, log);
    }

    ܦܢܐ NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_cycle_t *cycle)
{
    ngx_fd_t  fd;

    ܐܢ (cycle->log_use_stderr) {
        ܦܢܐ NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(cycle->log)->file->fd;

    ܐܢ (fd != ngx_stderr) {
        ܐܢ (ngx_set_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");

            ܦܢܐ NGX_ERROR;
        }
    }

    ܦܢܐ NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *head)
{
    ngx_log_t  *log;

    لكل (log = head; log; log = log->next) {
        ܐܢ (log->file != عدم) {
            ܦܢܐ log;
        }
    }

    ܦܢܐ عدم;
}


ساكن حرف *
ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log)
{
    ngx_uint_t   i, n, d, found;
    ngx_str_t   *value;

    ܐܢ (cf->args->nelts == 2) {
        log->log_level = NGX_LOG_ERR;
        ܦܢܐ NGX_CONF_OK;
    }

    value = cf->args->elts;

    لكل (i = 2; i < cf->args->nelts; i++) {
        found = 0;

        لكل (n = 1; n <= NGX_LOG_DEBUG; n++) {
            ܐܢ (ngx_strcmp(value[i].data, err_levels[n].data) == 0) {

                ܐܢ (log->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &value[i]);
                    ܦܢܐ NGX_CONF_ERROR;
                }

                log->log_level = n;
                found = 1;
                اكسر;
            }
        }

        لكل (n = 0, d = NGX_LOG_DEBUG_FIRST; d <= NGX_LOG_DEBUG_LAST; d <<= 1) {
            ܐܢ (ngx_strcmp(value[i].data, debug_levels[n++]) == 0) {
                ܐܢ (log->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &value[i]);
                    ܦܢܐ NGX_CONF_ERROR;
                }

                log->log_level |= d;
                found = 1;
                اكسر;
            }
        }


        ܐܢ (!found) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &value[i]);
            ܦܢܐ NGX_CONF_ERROR;
        }
    }

    ܐܢ (log->log_level == NGX_LOG_DEBUG) {
        log->log_level = NGX_LOG_DEBUG_ALL;
    }

    ܦܢܐ NGX_CONF_OK;
}


ساكن حرف *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->cycle->new_log;

    ܦܢܐ ngx_log_set_log(cf, &dummy);
}


حرف *
ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **head)
{
    ngx_log_t          *new_log;
    ngx_str_t          *value, name;
    ngx_syslog_peer_t  *peer;

    ܐܢ (*head != عدم && (*head)->log_level == 0) {
        new_log = *head;

    } ܐܚܪܢܐ {

        new_log = ngx_pcalloc(cf->pool, حجم(ngx_log_t));
        ܐܢ (new_log == عدم) {
            ܦܢܐ NGX_CONF_ERROR;
        }

        ܐܢ (*head == عدم) {
            *head = new_log;
        }
    }

    value = cf->args->elts;

    ܐܢ (ngx_strcmp(value[1].data, "stderr") == 0) {
        ngx_str_null(&name);
        cf->cycle->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->cycle, &name);
        ܐܢ (new_log->file == عدم) {
            ܦܢܐ NGX_CONF_ERROR;
        }

    } ܐܚܪܢܐ ܐܢ (ngx_strncmp(value[1].data, "memory:", 7) == 0) {

#ܐܢ (NGX_DEBUG)
        size_t                 size, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_memory_buf_t  *buf;

        value[1].len -= 7;
        value[1].data += 7;

        needed = حجم("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_SIZE_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        size = ngx_parse_size(&value[1]);

        ܐܢ (size == (size_t) NGX_ERROR || size < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &value[1]);
            ܦܢܐ NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, حجم(ngx_log_memory_buf_t));
        ܐܢ (buf == عدم) {
            ܦܢܐ NGX_CONF_ERROR;
        }

        buf->start = ngx_pnalloc(cf->pool, size);
        ܐܢ (buf->start == عدم) {
            ܦܢܐ NGX_CONF_ERROR;
        }

        buf->end = buf->start + size;

        buf->pos = ngx_slprintf(buf->start, buf->end, "MEMLOG %uz %V:%ui%N",
                                size, &cf->conf_file->file.name,
                                cf->conf_file->line);

        ngx_memset(buf->pos, ' ', buf->end - buf->pos);

        cln = ngx_pool_cleanup_add(cf->pool, 0);
        ܐܢ (cln == عدم) {
            ܦܢܐ NGX_CONF_ERROR;
        }

        cln->data = new_log;
        cln->handler = ngx_log_memory_cleanup;

        new_log->writer = ngx_log_memory_writer;
        new_log->wdata = buf;

#ܐܚܪܢܐ
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        ܦܢܐ NGX_CONF_ERROR;
#endif

    } ܐܚܪܢܐ ܐܢ (ngx_strncmp(value[1].data, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, حجم(ngx_syslog_peer_t));
        ܐܢ (peer == عدم) {
            ܦܢܐ NGX_CONF_ERROR;
        }

        ܐܢ (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            ܦܢܐ NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } ܐܚܪܢܐ {
        new_log->file = ngx_conf_open_file(cf->cycle, &value[1]);
        ܐܢ (new_log->file == عدم) {
            ܦܢܐ NGX_CONF_ERROR;
        }
    }

    ܐܢ (ngx_log_set_levels(cf, new_log) != NGX_CONF_OK) {
        ܦܢܐ NGX_CONF_ERROR;
    }

    ܐܢ (*head != new_log) {
        ngx_log_insert(*head, new_log);
    }

    ܦܢܐ NGX_CONF_OK;
}


ساكن فراغ
ngx_log_insert(ngx_log_t *log, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    ܐܢ (new_log->log_level > log->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *log;
        *log = *new_log;
        *new_log = tmp;

        log->next = new_log;
        ܦܢܐ;
    }

    ܟܡܐ (log->next) {
        ܐܢ (new_log->log_level > log->next->log_level) {
            new_log->next = log->next;
            log->next = new_log;
            ܦܢܐ;
        }

        log = log->next;
    }

    log->next = new_log;
}


#ܐܢ (NGX_DEBUG)

ساكن فراغ
ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char                *p;
    size_t                 avail, written;
    ngx_log_memory_buf_t  *mem;

    mem = log->wdata;

    ܐܢ (mem == عدم) {
        ܦܢܐ;
    }

    written = ngx_atomic_fetch_add(&mem->written, len);

    p = mem->pos + written % (mem->end - mem->pos);

    avail = mem->end - p;

    ܐܢ (avail >= len) {
        ngx_memcpy(p, buf, len);

    } ܐܚܪܢܐ {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


ساكن فراغ
ngx_log_memory_cleanup(فراغ *data)
{
    ngx_log_t *log = data;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, log, 0, "destroy memory log buffer");

    log->wdata = عدم;
}

#endif
