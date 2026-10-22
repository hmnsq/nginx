
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن حرف *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log);
ساكن فراغ ngx_log_insert(ngx_log_t *log, ngx_log_t *new_log);


#אם (NGX_DEBUG)

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


#אם (NGX_HAVE_VARIADIC_MACROS)

فراغ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ثابت حرف *fmt, ...)

#אחרת

فراغ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ثابت حرف *fmt, va_list args)

#endif
{
#אם (NGX_HAVE_VARIADIC_MACROS)
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

    אם (log->connection) {
        p = ngx_slprintf(p, last, "*%uA ", log->connection);
    }

    msg = p;

#אם (NGX_HAVE_VARIADIC_MACROS)

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

#אחרת

    p = ngx_vslprintf(p, last, fmt, args);

#endif

    אם (err) {
        p = ngx_log_errno(p, last, err);
    }

    אם (level != NGX_LOG_DEBUG && log->handler) {
        p = log->handler(log, p, last - p);
    }

    אם (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (log->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    כלעוד (log) {

        אם (log->log_level < level && !debug_connection) {
            اكسر;
        }

        אם (log->writer) {
            log->writer(log, level, errstr, p - errstr);
            اذهب next;
        }

        אם (ngx_time() == log->disk_full_time) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            اذهب next;
        }

        n = ngx_write_fd(log->file->fd, errstr, p - errstr);

        אם (n == -1 && ngx_errno == NGX_ENOSPC) {
            log->disk_full_time = ngx_time();
        }

        אם (log->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    next:

        log = log->next;
    }

    אם (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        החזר;
    }

    msg -= (7 + err_levels[level].len + 3);

    (فراغ) ngx_sprintf(msg, "nginx: [%V] ", &err_levels[level]);

    (فراغ) ngx_write_console(ngx_stderr, msg, p - msg);
}


#אם !(NGX_HAVE_VARIADIC_MACROS)

فراغ ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    va_list  args;

    אם (log->log_level >= level) {
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

    אם (err) {
        p = ngx_log_errno(p, last, err);
    }

    אם (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    (فراغ) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err)
{
    אם (buf > last - 50) {

        /* leave a space for an error code */

        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#אם (NGX_WIN32)
    buf = ngx_slprintf(buf, last, ((unsigned) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#אחרת
    buf = ngx_slprintf(buf, last, " (%d: ", err);
#endif

    buf = ngx_strerror(err, buf, last - buf);

    אם (buf < last) {
        *buf++ = ')';
    }

    החזר buf;
}


ngx_log_t *
ngx_log_init(u_char *prefix, u_char *error_log)
{
    u_char  *p, *name;
    size_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    אם (error_log == عدم) {
        error_log = (u_char *) NGX_ERROR_LOG_PATH;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    אם (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        החזר &ngx_log;
    }

    p = عدم;

#אם (NGX_WIN32)
    אם (name[1] != ':') {
#אחרת
    אם (name[0] != '/') {
#endif

        אם (prefix) {
            plen = ngx_strlen(prefix);

        } אחרת {
#ifdef NGX_PREFIX
            prefix = (u_char *) NGX_PREFIX;
            plen = ngx_strlen(prefix);
#אחרת
            plen = 0;
#endif
        }

        אם (plen) {
            name = malloc(plen + nlen + 2);
            אם (name == عدم) {
                החזר عدم;
            }

            p = ngx_cpymem(name, prefix, plen);

            אם (!ngx_path_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    אם (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#אם (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#endif

        ngx_log_file.fd = ngx_stderr;
    }

    אם (p) {
        ngx_free(p);
    }

    החזר &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_cycle_t *cycle)
{
    ngx_log_t  *log;

    אם (ngx_log_get_file_log(&cycle->new_log) != عدم) {
        החזר NGX_OK;
    }

    אם (cycle->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        log = ngx_pcalloc(cycle->pool, حجم(ngx_log_t));
        אם (log == عدم) {
            החזר NGX_ERROR;
        }

    } אחרת {
        /* no error logs at all */
        log = &cycle->new_log;
    }

    log->log_level = NGX_LOG_ERR;

    log->file = ngx_conf_open_file(cycle, &cycle->error_log);
    אם (log->file == عدم) {
        החזר NGX_ERROR;
    }

    אם (log != &cycle->new_log) {
        ngx_log_insert(&cycle->new_log, log);
    }

    החזר NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_cycle_t *cycle)
{
    ngx_fd_t  fd;

    אם (cycle->log_use_stderr) {
        החזר NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(cycle->log)->file->fd;

    אם (fd != ngx_stderr) {
        אם (ngx_set_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");

            החזר NGX_ERROR;
        }
    }

    החזר NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *head)
{
    ngx_log_t  *log;

    עבור (log = head; log; log = log->next) {
        אם (log->file != عدم) {
            החזר log;
        }
    }

    החזר عدم;
}


ساكن حرف *
ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log)
{
    ngx_uint_t   i, n, d, found;
    ngx_str_t   *value;

    אם (cf->args->nelts == 2) {
        log->log_level = NGX_LOG_ERR;
        החזר NGX_CONF_OK;
    }

    value = cf->args->elts;

    עבור (i = 2; i < cf->args->nelts; i++) {
        found = 0;

        עבור (n = 1; n <= NGX_LOG_DEBUG; n++) {
            אם (ngx_strcmp(value[i].data, err_levels[n].data) == 0) {

                אם (log->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &value[i]);
                    החזר NGX_CONF_ERROR;
                }

                log->log_level = n;
                found = 1;
                اكسر;
            }
        }

        עבור (n = 0, d = NGX_LOG_DEBUG_FIRST; d <= NGX_LOG_DEBUG_LAST; d <<= 1) {
            אם (ngx_strcmp(value[i].data, debug_levels[n++]) == 0) {
                אם (log->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &value[i]);
                    החזר NGX_CONF_ERROR;
                }

                log->log_level |= d;
                found = 1;
                اكسر;
            }
        }


        אם (!found) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &value[i]);
            החזר NGX_CONF_ERROR;
        }
    }

    אם (log->log_level == NGX_LOG_DEBUG) {
        log->log_level = NGX_LOG_DEBUG_ALL;
    }

    החזר NGX_CONF_OK;
}


ساكن حرف *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->cycle->new_log;

    החזר ngx_log_set_log(cf, &dummy);
}


حرف *
ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **head)
{
    ngx_log_t          *new_log;
    ngx_str_t          *value, name;
    ngx_syslog_peer_t  *peer;

    אם (*head != عدم && (*head)->log_level == 0) {
        new_log = *head;

    } אחרת {

        new_log = ngx_pcalloc(cf->pool, حجم(ngx_log_t));
        אם (new_log == عدم) {
            החזר NGX_CONF_ERROR;
        }

        אם (*head == عدم) {
            *head = new_log;
        }
    }

    value = cf->args->elts;

    אם (ngx_strcmp(value[1].data, "stderr") == 0) {
        ngx_str_null(&name);
        cf->cycle->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->cycle, &name);
        אם (new_log->file == عدم) {
            החזר NGX_CONF_ERROR;
        }

    } אחרת אם (ngx_strncmp(value[1].data, "memory:", 7) == 0) {

#אם (NGX_DEBUG)
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

        אם (size == (size_t) NGX_ERROR || size < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &value[1]);
            החזר NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, حجم(ngx_log_memory_buf_t));
        אם (buf == عدم) {
            החזר NGX_CONF_ERROR;
        }

        buf->start = ngx_pnalloc(cf->pool, size);
        אם (buf->start == عدم) {
            החזר NGX_CONF_ERROR;
        }

        buf->end = buf->start + size;

        buf->pos = ngx_slprintf(buf->start, buf->end, "MEMLOG %uz %V:%ui%N",
                                size, &cf->conf_file->file.name,
                                cf->conf_file->line);

        ngx_memset(buf->pos, ' ', buf->end - buf->pos);

        cln = ngx_pool_cleanup_add(cf->pool, 0);
        אם (cln == عدم) {
            החזר NGX_CONF_ERROR;
        }

        cln->data = new_log;
        cln->handler = ngx_log_memory_cleanup;

        new_log->writer = ngx_log_memory_writer;
        new_log->wdata = buf;

#אחרת
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        החזר NGX_CONF_ERROR;
#endif

    } אחרת אם (ngx_strncmp(value[1].data, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, حجم(ngx_syslog_peer_t));
        אם (peer == عدم) {
            החזר NGX_CONF_ERROR;
        }

        אם (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            החזר NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } אחרת {
        new_log->file = ngx_conf_open_file(cf->cycle, &value[1]);
        אם (new_log->file == عدم) {
            החזר NGX_CONF_ERROR;
        }
    }

    אם (ngx_log_set_levels(cf, new_log) != NGX_CONF_OK) {
        החזר NGX_CONF_ERROR;
    }

    אם (*head != new_log) {
        ngx_log_insert(*head, new_log);
    }

    החזר NGX_CONF_OK;
}


ساكن فراغ
ngx_log_insert(ngx_log_t *log, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    אם (new_log->log_level > log->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *log;
        *log = *new_log;
        *new_log = tmp;

        log->next = new_log;
        החזר;
    }

    כלעוד (log->next) {
        אם (new_log->log_level > log->next->log_level) {
            new_log->next = log->next;
            log->next = new_log;
            החזר;
        }

        log = log->next;
    }

    log->next = new_log;
}


#אם (NGX_DEBUG)

ساكن فراغ
ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char                *p;
    size_t                 avail, written;
    ngx_log_memory_buf_t  *mem;

    mem = log->wdata;

    אם (mem == عدم) {
        החזר;
    }

    written = ngx_atomic_fetch_add(&mem->written, len);

    p = mem->pos + written % (mem->end - mem->pos);

    avail = mem->end - p;

    אם (avail >= len) {
        ngx_memcpy(p, buf, len);

    } אחרת {
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
