
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ایستا نویسه *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
ایستا نویسه *ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log);
ایستا تهی ngx_log_درج(ngx_log_t *log, ngx_log_t *new_log);


#اگر (NGX_DEBUG)

ایستا تهی ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level,
    u_char *buf, size_t len);
ایستا تهی ngx_log_memory_cleanup(تهی *data);


تعریفنوع ساختار {
    u_char        *start;
    u_char        *end;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_memory_buf_t;

#endif


ایستا ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1MORE,
      ngx_error_log,
      0,
      0,
      پوچ },

      ngx_null_command
};


ایستا ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    پوچ,
    پوچ
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    پوچ,                                  /* init master */
    پوچ,                                  /* init module */
    پوچ,                                  /* init process */
    پوچ,                                  /* init thread */
    پوچ,                                  /* exit thread */
    پوچ,                                  /* exit process */
    پوچ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ایستا ngx_log_t        ngx_log;
ایستا ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


ایستا ngx_str_t err_levels[] = {
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

ایستا ثابت نویسه *debug_levels[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#اگر (NGX_HAVE_VARIADIC_MACROS)

تهی
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ثابت نویسه *fmt, ...)

#وگرنه

تهی
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ثابت نویسه *fmt, va_list args)

#endif
{
#اگر (NGX_HAVE_VARIADIC_MACROS)
    va_list      args;
#endif
    u_char      *p, *last, *پیام;
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

    اگر (log->connection) {
        p = ngx_slprintf(p, last, "*%uA ", log->connection);
    }

    پیام = p;

#اگر (NGX_HAVE_VARIADIC_MACROS)

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

#وگرنه

    p = ngx_vslprintf(p, last, fmt, args);

#endif

    اگر (err) {
        p = ngx_log_errno(p, last, err);
    }

    اگر (level != NGX_LOG_DEBUG && log->handler) {
        p = log->handler(log, p, last - p);
    }

    اگر (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (log->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    وقتی (log) {

        اگر (log->log_level < level && !debug_connection) {
            بشکن;
        }

        اگر (log->writer) {
            log->writer(log, level, errstr, p - errstr);
            برو next;
        }

        اگر (ngx_time() == log->disk_full_time) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            برو next;
        }

        n = ngx_write_fd(log->file->fd, errstr, p - errstr);

        اگر (n == -1 && ngx_errno == NGX_ENOSPC) {
            log->disk_full_time = ngx_time();
        }

        اگر (log->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    next:

        log = log->next;
    }

    اگر (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        بازگشت;
    }

    پیام -= (7 + err_levels[level].len + 3);

    (تهی) ngx_sprintf(پیام, "nginx: [%V] ", &err_levels[level]);

    (تهی) ngx_write_console(ngx_stderr, پیام, p - پیام);
}


#اگر !(NGX_HAVE_VARIADIC_MACROS)

تهی ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ثابت نویسه *fmt, ...)
{
    va_list  args;

    اگر (log->log_level >= level) {
        va_start(args, fmt);
        ngx_log_error_core(level, log, err, fmt, args);
        va_end(args);
    }
}


تهی ngx_cdecl
ngx_log_debug_core(ngx_log_t *log, ngx_err_t err, ثابت نویسه *fmt, ...)
{
    va_list  args;

    va_start(args, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, log, err, fmt, args);
    va_end(args);
}

#endif


تهی ngx_cdecl
ngx_log_abort(ngx_err_t err, ثابت نویسه *fmt, ...)
{
    u_char   *p;
    va_list   args;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_start(args, fmt);
    p = ngx_vsnprintf(errstr, اندازه(errstr) - 1, fmt, args);
    va_end(args);

    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                  "%*s", p - errstr, errstr);
}


تهی ngx_cdecl
ngx_log_stderr(ngx_err_t err, ثابت نویسه *fmt, ...)
{
    u_char   *p, *last;
    va_list   args;
    u_char    errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

    اگر (err) {
        p = ngx_log_errno(p, last, err);
    }

    اگر (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    (تهی) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err)
{
    اگر (buf > last - 50) {

        /* leave a space for an error code */

        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#اگر (NGX_WIN32)
    buf = ngx_slprintf(buf, last, ((unsigned) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#وگرنه
    buf = ngx_slprintf(buf, last, " (%d: ", err);
#endif

    buf = ngx_strerror(err, buf, last - buf);

    اگر (buf < last) {
        *buf++ = ')';
    }

    بازگشت buf;
}


ngx_log_t *
ngx_log_init(u_char *prefix, u_char *error_log)
{
    u_char  *p, *name;
    size_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    اگر (error_log == پوچ) {
        error_log = (u_char *) NGX_ERROR_LOG_PATH;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    اگر (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        بازگشت &ngx_log;
    }

    p = پوچ;

#اگر (NGX_WIN32)
    اگر (name[1] != ':') {
#وگرنه
    اگر (name[0] != '/') {
#endif

        اگر (prefix) {
            plen = ngx_strlen(prefix);

        } وگرنه {
#ifdef NGX_PREFIX
            prefix = (u_char *) NGX_PREFIX;
            plen = ngx_strlen(prefix);
#وگرنه
            plen = 0;
#endif
        }

        اگر (plen) {
            name = malloc(plen + nlen + 2);
            اگر (name == پوچ) {
                بازگشت پوچ;
            }

            p = ngx_cpymem(name, prefix, plen);

            اگر (!ngx_path_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    اگر (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#اگر (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#endif

        ngx_log_file.fd = ngx_stderr;
    }

    اگر (p) {
        ngx_free(p);
    }

    بازگشت &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_cycle_t *cycle)
{
    ngx_log_t  *log;

    اگر (ngx_log_get_file_log(&cycle->new_log) != پوچ) {
        بازگشت NGX_OK;
    }

    اگر (cycle->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        log = ngx_pcalloc(cycle->pool, اندازه(ngx_log_t));
        اگر (log == پوچ) {
            بازگشت NGX_ERROR;
        }

    } وگرنه {
        /* no error logs at all */
        log = &cycle->new_log;
    }

    log->log_level = NGX_LOG_ERR;

    log->file = ngx_conf_open_file(cycle, &cycle->error_log);
    اگر (log->file == پوچ) {
        بازگشت NGX_ERROR;
    }

    اگر (log != &cycle->new_log) {
        ngx_log_درج(&cycle->new_log, log);
    }

    بازگشت NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_cycle_t *cycle)
{
    ngx_fd_t  fd;

    اگر (cycle->log_use_stderr) {
        بازگشت NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(cycle->log)->file->fd;

    اگر (fd != ngx_stderr) {
        اگر (ngx_set_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");

            بازگشت NGX_ERROR;
        }
    }

    بازگشت NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *head)
{
    ngx_log_t  *log;

    برای (log = head; log; log = log->next) {
        اگر (log->file != پوچ) {
            بازگشت log;
        }
    }

    بازگشت پوچ;
}


ایستا نویسه *
ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log)
{
    ngx_uint_t   i, n, d, found;
    ngx_str_t   *مقدار;

    اگر (cf->args->nelts == 2) {
        log->log_level = NGX_LOG_ERR;
        بازگشت NGX_CONF_OK;
    }

    مقدار = cf->args->elts;

    برای (i = 2; i < cf->args->nelts; i++) {
        found = 0;

        برای (n = 1; n <= NGX_LOG_DEBUG; n++) {
            اگر (ngx_strcmp(مقدار[i].data, err_levels[n].data) == 0) {

                اگر (log->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &مقدار[i]);
                    بازگشت NGX_CONF_ERROR;
                }

                log->log_level = n;
                found = 1;
                بشکن;
            }
        }

        برای (n = 0, d = NGX_LOG_DEBUG_FIRST; d <= NGX_LOG_DEBUG_LAST; d <<= 1) {
            اگر (ngx_strcmp(مقدار[i].data, debug_levels[n++]) == 0) {
                اگر (log->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &مقدار[i]);
                    بازگشت NGX_CONF_ERROR;
                }

                log->log_level |= d;
                found = 1;
                بشکن;
            }
        }


        اگر (!found) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &مقدار[i]);
            بازگشت NGX_CONF_ERROR;
        }
    }

    اگر (log->log_level == NGX_LOG_DEBUG) {
        log->log_level = NGX_LOG_DEBUG_ALL;
    }

    بازگشت NGX_CONF_OK;
}


ایستا نویسه *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->cycle->new_log;

    بازگشت ngx_log_set_log(cf, &dummy);
}


نویسه *
ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **head)
{
    ngx_log_t          *new_log;
    ngx_str_t          *مقدار, name;
    ngx_syslog_peer_t  *peer;

    اگر (*head != پوچ && (*head)->log_level == 0) {
        new_log = *head;

    } وگرنه {

        new_log = ngx_pcalloc(cf->pool, اندازه(ngx_log_t));
        اگر (new_log == پوچ) {
            بازگشت NGX_CONF_ERROR;
        }

        اگر (*head == پوچ) {
            *head = new_log;
        }
    }

    مقدار = cf->args->elts;

    اگر (ngx_strcmp(مقدار[1].data, "stderr") == 0) {
        ngx_str_null(&name);
        cf->cycle->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->cycle, &name);
        اگر (new_log->file == پوچ) {
            بازگشت NGX_CONF_ERROR;
        }

    } وگرنه اگر (ngx_strncmp(مقدار[1].data, "memory:", 7) == 0) {

#اگر (NGX_DEBUG)
        size_t                 size, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_memory_buf_t  *buf;

        مقدار[1].len -= 7;
        مقدار[1].data += 7;

        needed = اندازه("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_SIZE_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        size = ngx_parse_size(&مقدار[1]);

        اگر (size == (size_t) NGX_ERROR || size < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &مقدار[1]);
            بازگشت NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, اندازه(ngx_log_memory_buf_t));
        اگر (buf == پوچ) {
            بازگشت NGX_CONF_ERROR;
        }

        buf->start = ngx_pnalloc(cf->pool, size);
        اگر (buf->start == پوچ) {
            بازگشت NGX_CONF_ERROR;
        }

        buf->end = buf->start + size;

        buf->pos = ngx_slprintf(buf->start, buf->end, "MEMLOG %uz %V:%ui%N",
                                size, &cf->conf_file->file.name,
                                cf->conf_file->سطر);

        ngx_memset(buf->pos, ' ', buf->end - buf->pos);

        cln = ngx_pool_cleanup_add(cf->pool, 0);
        اگر (cln == پوچ) {
            بازگشت NGX_CONF_ERROR;
        }

        cln->data = new_log;
        cln->handler = ngx_log_memory_cleanup;

        new_log->writer = ngx_log_memory_writer;
        new_log->wdata = buf;

#وگرنه
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        بازگشت NGX_CONF_ERROR;
#endif

    } وگرنه اگر (ngx_strncmp(مقدار[1].data, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, اندازه(ngx_syslog_peer_t));
        اگر (peer == پوچ) {
            بازگشت NGX_CONF_ERROR;
        }

        اگر (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            بازگشت NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } وگرنه {
        new_log->file = ngx_conf_open_file(cf->cycle, &مقدار[1]);
        اگر (new_log->file == پوچ) {
            بازگشت NGX_CONF_ERROR;
        }
    }

    اگر (ngx_log_set_levels(cf, new_log) != NGX_CONF_OK) {
        بازگشت NGX_CONF_ERROR;
    }

    اگر (*head != new_log) {
        ngx_log_درج(*head, new_log);
    }

    بازگشت NGX_CONF_OK;
}


ایستا تهی
ngx_log_درج(ngx_log_t *log, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    اگر (new_log->log_level > log->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *log;
        *log = *new_log;
        *new_log = tmp;

        log->next = new_log;
        بازگشت;
    }

    وقتی (log->next) {
        اگر (new_log->log_level > log->next->log_level) {
            new_log->next = log->next;
            log->next = new_log;
            بازگشت;
        }

        log = log->next;
    }

    log->next = new_log;
}


#اگر (NGX_DEBUG)

ایستا تهی
ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char                *p;
    size_t                 avail, written;
    ngx_log_memory_buf_t  *mem;

    mem = log->wdata;

    اگر (mem == پوچ) {
        بازگشت;
    }

    written = ngx_atomic_fetch_add(&mem->written, len);

    p = mem->pos + written % (mem->end - mem->pos);

    avail = mem->end - p;

    اگر (avail >= len) {
        ngx_memcpy(p, buf, len);

    } وگرنه {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


ایستا تهی
ngx_log_memory_cleanup(تهی *data)
{
    ngx_log_t *log = data;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, log, 0, "destroy memory log buffer");

    log->wdata = پوچ;
}

#endif
