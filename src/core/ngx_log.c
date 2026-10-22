
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ସ୍ଥିର ଅକ୍ଷର *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf);
ସ୍ଥିର ଅକ୍ଷର *ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log);
ସ୍ଥିର ଶୂନ୍ୟ ngx_log_ଭର୍ତ୍ତି(ngx_log_t *log, ngx_log_t *new_log);


#ଯଦି (NGX_DEBUG)

ସ୍ଥିର ଶୂନ୍ୟ ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level,
    u_char *buf, size_t len);
ସ୍ଥିର ଶୂନ୍ୟ ngx_log_memory_cleanup(ଶୂନ୍ୟ *data);


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    u_char        *start;
    u_char        *end;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_memory_buf_t;

#endif


ସ୍ଥିର ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1MORE,
      ngx_error_log,
      0,
      0,
      ଖାଲି },

      ngx_null_command
};


ସ୍ଥିର ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    ଖାଲି,
    ଖାଲି
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ଖାଲି,                                  /* init master */
    ଖାଲି,                                  /* init module */
    ଖାଲି,                                  /* init process */
    ଖାଲି,                                  /* init thread */
    ଖାଲି,                                  /* exit thread */
    ଖାଲି,                                  /* exit process */
    ଖାଲି,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ସ୍ଥିର ngx_log_t        ngx_log;
ସ୍ଥିର ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


ସ୍ଥିର ngx_str_t err_levels[] = {
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

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *debug_levels[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#ଯଦି (NGX_HAVE_VARIADIC_MACROS)

ଶୂନ୍ୟ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ଅଚଳ ଅକ୍ଷର *fmt, ...)

#ଅନ୍ୟଥା

ଶୂନ୍ୟ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ଅଚଳ ଅକ୍ଷର *fmt, va_list args)

#endif
{
#ଯଦି (NGX_HAVE_VARIADIC_MACROS)
    va_list      args;
#endif
    u_char      *p, *last, *ସନ୍ଦେଶ;
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

    ଯଦି (log->connection) {
        p = ngx_slprintf(p, last, "*%uA ", log->connection);
    }

    ସନ୍ଦେଶ = p;

#ଯଦି (NGX_HAVE_VARIADIC_MACROS)

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

#ଅନ୍ୟଥା

    p = ngx_vslprintf(p, last, fmt, args);

#endif

    ଯଦି (err) {
        p = ngx_log_errno(p, last, err);
    }

    ଯଦି (level != NGX_LOG_DEBUG && log->handler) {
        p = log->handler(log, p, last - p);
    }

    ଯଦି (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (log->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    ଯେତେବେଳେ (log) {

        ଯଦି (log->log_level < level && !debug_connection) {
            ଭାଙ୍ଗ;
        }

        ଯଦି (log->writer) {
            log->writer(log, level, errstr, p - errstr);
            ଯାଅ next;
        }

        ଯଦି (ngx_time() == log->disk_full_time) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            ଯାଅ next;
        }

        n = ngx_write_fd(log->file->fd, errstr, p - errstr);

        ଯଦି (n == -1 && ngx_errno == NGX_ENOSPC) {
            log->disk_full_time = ngx_time();
        }

        ଯଦି (log->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    next:

        log = log->next;
    }

    ଯଦି (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        ଫେରନ୍ତୁ;
    }

    ସନ୍ଦେଶ -= (7 + err_levels[level].len + 3);

    (ଶୂନ୍ୟ) ngx_sprintf(ସନ୍ଦେଶ, "nginx: [%V] ", &err_levels[level]);

    (ଶୂନ୍ୟ) ngx_write_console(ngx_stderr, ସନ୍ଦେଶ, p - ସନ୍ଦେଶ);
}


#ଯଦି !(NGX_HAVE_VARIADIC_MACROS)

ଶୂନ୍ୟ ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
    va_list  args;

    ଯଦି (log->log_level >= level) {
        va_start(args, fmt);
        ngx_log_error_core(level, log, err, fmt, args);
        va_end(args);
    }
}


ଶୂନ୍ୟ ngx_cdecl
ngx_log_debug_core(ngx_log_t *log, ngx_err_t err, ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
    va_list  args;

    va_start(args, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, log, err, fmt, args);
    va_end(args);
}

#endif


ଶୂନ୍ୟ ngx_cdecl
ngx_log_abort(ngx_err_t err, ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
    u_char   *p;
    va_list   args;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_start(args, fmt);
    p = ngx_vsnprintf(errstr, ଆକାର(errstr) - 1, fmt, args);
    va_end(args);

    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                  "%*s", p - errstr, errstr);
}


ଶୂନ୍ୟ ngx_cdecl
ngx_log_stderr(ngx_err_t err, ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
    u_char   *p, *last;
    va_list   args;
    u_char    errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

    ଯଦି (err) {
        p = ngx_log_errno(p, last, err);
    }

    ଯଦି (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    (ଶୂନ୍ୟ) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err)
{
    ଯଦି (buf > last - 50) {

        /* leave a space for an error code */

        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#ଯଦି (NGX_WIN32)
    buf = ngx_slprintf(buf, last, ((unsigned) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#ଅନ୍ୟଥା
    buf = ngx_slprintf(buf, last, " (%d: ", err);
#endif

    buf = ngx_strerror(err, buf, last - buf);

    ଯଦି (buf < last) {
        *buf++ = ')';
    }

    ଫେରନ୍ତୁ buf;
}


ngx_log_t *
ngx_log_init(u_char *prefix, u_char *error_log)
{
    u_char  *p, *name;
    size_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    ଯଦି (error_log == ଖାଲି) {
        error_log = (u_char *) NGX_ERROR_LOG_PATH;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    ଯଦି (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        ଫେରନ୍ତୁ &ngx_log;
    }

    p = ଖାଲି;

#ଯଦି (NGX_WIN32)
    ଯଦି (name[1] != ':') {
#ଅନ୍ୟଥା
    ଯଦି (name[0] != '/') {
#endif

        ଯଦି (prefix) {
            plen = ngx_strlen(prefix);

        } ଅନ୍ୟଥା {
#ifdef NGX_PREFIX
            prefix = (u_char *) NGX_PREFIX;
            plen = ngx_strlen(prefix);
#ଅନ୍ୟଥା
            plen = 0;
#endif
        }

        ଯଦି (plen) {
            name = malloc(plen + nlen + 2);
            ଯଦି (name == ଖାଲି) {
                ଫେରନ୍ତୁ ଖାଲି;
            }

            p = ngx_cpymem(name, prefix, plen);

            ଯଦି (!ngx_path_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    ଯଦି (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#ଯଦି (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#endif

        ngx_log_file.fd = ngx_stderr;
    }

    ଯଦି (p) {
        ngx_free(p);
    }

    ଫେରନ୍ତୁ &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_cycle_t *cycle)
{
    ngx_log_t  *log;

    ଯଦି (ngx_log_get_file_log(&cycle->new_log) != ଖାଲି) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    ଯଦି (cycle->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        log = ngx_pcalloc(cycle->pool, ଆକାର(ngx_log_t));
        ଯଦି (log == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

    } ଅନ୍ୟଥା {
        /* no error logs at all */
        log = &cycle->new_log;
    }

    log->log_level = NGX_LOG_ERR;

    log->file = ngx_conf_open_file(cycle, &cycle->error_log);
    ଯଦି (log->file == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (log != &cycle->new_log) {
        ngx_log_ଭର୍ତ୍ତି(&cycle->new_log, log);
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_cycle_t *cycle)
{
    ngx_fd_t  fd;

    ଯଦି (cycle->log_use_stderr) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(cycle->log)->file->fd;

    ଯଦି (fd != ngx_stderr) {
        ଯଦି (ngx_set_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");

            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *head)
{
    ngx_log_t  *log;

    ପାଇଁ (log = head; log; log = log->next) {
        ଯଦି (log->file != ଖାଲି) {
            ଫେରନ୍ତୁ log;
        }
    }

    ଫେରନ୍ତୁ ଖାଲି;
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log)
{
    ngx_uint_t   i, n, d, found;
    ngx_str_t   *ମୂଲ୍ୟ;

    ଯଦି (cf->args->nelts == 2) {
        log->log_level = NGX_LOG_ERR;
        ଫେରନ୍ତୁ NGX_CONF_OK;
    }

    ମୂଲ୍ୟ = cf->args->elts;

    ପାଇଁ (i = 2; i < cf->args->nelts; i++) {
        found = 0;

        ପାଇଁ (n = 1; n <= NGX_LOG_DEBUG; n++) {
            ଯଦି (ngx_strcmp(ମୂଲ୍ୟ[i].data, err_levels[n].data) == 0) {

                ଯଦି (log->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &ମୂଲ୍ୟ[i]);
                    ଫେରନ୍ତୁ NGX_CONF_ERROR;
                }

                log->log_level = n;
                found = 1;
                ଭାଙ୍ଗ;
            }
        }

        ପାଇଁ (n = 0, d = NGX_LOG_DEBUG_FIRST; d <= NGX_LOG_DEBUG_LAST; d <<= 1) {
            ଯଦି (ngx_strcmp(ମୂଲ୍ୟ[i].data, debug_levels[n++]) == 0) {
                ଯଦି (log->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &ମୂଲ୍ୟ[i]);
                    ଫେରନ୍ତୁ NGX_CONF_ERROR;
                }

                log->log_level |= d;
                found = 1;
                ଭାଙ୍ଗ;
            }
        }


        ଯଦି (!found) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &ମୂଲ୍ୟ[i]);
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }
    }

    ଯଦି (log->log_level == NGX_LOG_DEBUG) {
        log->log_level = NGX_LOG_DEBUG_ALL;
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ସ୍ଥିର ଅକ୍ଷର *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->cycle->new_log;

    ଫେରନ୍ତୁ ngx_log_set_log(cf, &dummy);
}


ଅକ୍ଷର *
ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **head)
{
    ngx_log_t          *new_log;
    ngx_str_t          *ମୂଲ୍ୟ, name;
    ngx_syslog_peer_t  *peer;

    ଯଦି (*head != ଖାଲି && (*head)->log_level == 0) {
        new_log = *head;

    } ଅନ୍ୟଥା {

        new_log = ngx_pcalloc(cf->pool, ଆକାର(ngx_log_t));
        ଯଦି (new_log == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        ଯଦି (*head == ଖାଲି) {
            *head = new_log;
        }
    }

    ମୂଲ୍ୟ = cf->args->elts;

    ଯଦି (ngx_strcmp(ମୂଲ୍ୟ[1].data, "stderr") == 0) {
        ngx_str_null(&name);
        cf->cycle->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->cycle, &name);
        ଯଦି (new_log->file == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

    } ଅନ୍ୟଥା ଯଦି (ngx_strncmp(ମୂଲ୍ୟ[1].data, "memory:", 7) == 0) {

#ଯଦି (NGX_DEBUG)
        size_t                 size, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_memory_buf_t  *buf;

        ମୂଲ୍ୟ[1].len -= 7;
        ମୂଲ୍ୟ[1].data += 7;

        needed = ଆକାର("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_SIZE_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        size = ngx_parse_size(&ମୂଲ୍ୟ[1]);

        ଯଦି (size == (size_t) NGX_ERROR || size < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &ମୂଲ୍ୟ[1]);
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, ଆକାର(ngx_log_memory_buf_t));
        ଯଦି (buf == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        buf->start = ngx_pnalloc(cf->pool, size);
        ଯଦି (buf->start == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        buf->end = buf->start + size;

        buf->pos = ngx_slprintf(buf->start, buf->end, "MEMLOG %uz %V:%ui%N",
                                size, &cf->conf_file->file.name,
                                cf->conf_file->ଧାଡ଼ି);

        ngx_memset(buf->pos, ' ', buf->end - buf->pos);

        cln = ngx_pool_cleanup_add(cf->pool, 0);
        ଯଦି (cln == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        cln->data = new_log;
        cln->handler = ngx_log_memory_cleanup;

        new_log->writer = ngx_log_memory_writer;
        new_log->wdata = buf;

#ଅନ୍ୟଥା
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
#endif

    } ଅନ୍ୟଥା ଯଦି (ngx_strncmp(ମୂଲ୍ୟ[1].data, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, ଆକାର(ngx_syslog_peer_t));
        ଯଦି (peer == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        ଯଦି (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } ଅନ୍ୟଥା {
        new_log->file = ngx_conf_open_file(cf->cycle, &ମୂଲ୍ୟ[1]);
        ଯଦି (new_log->file == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }
    }

    ଯଦି (ngx_log_set_levels(cf, new_log) != NGX_CONF_OK) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ଯଦି (*head != new_log) {
        ngx_log_ଭର୍ତ୍ତି(*head, new_log);
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_log_ଭର୍ତ୍ତି(ngx_log_t *log, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    ଯଦି (new_log->log_level > log->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *log;
        *log = *new_log;
        *new_log = tmp;

        log->next = new_log;
        ଫେରନ୍ତୁ;
    }

    ଯେତେବେଳେ (log->next) {
        ଯଦି (new_log->log_level > log->next->log_level) {
            new_log->next = log->next;
            log->next = new_log;
            ଫେରନ୍ତୁ;
        }

        log = log->next;
    }

    log->next = new_log;
}


#ଯଦି (NGX_DEBUG)

ସ୍ଥିର ଶୂନ୍ୟ
ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char                *p;
    size_t                 avail, written;
    ngx_log_memory_buf_t  *mem;

    mem = log->wdata;

    ଯଦି (mem == ଖାଲି) {
        ଫେରନ୍ତୁ;
    }

    written = ngx_atomic_fetch_add(&mem->written, len);

    p = mem->pos + written % (mem->end - mem->pos);

    avail = mem->end - p;

    ଯଦି (avail >= len) {
        ngx_memcpy(p, buf, len);

    } ଅନ୍ୟଥା {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_log_memory_cleanup(ଶୂନ୍ୟ *data)
{
    ngx_log_t *log = data;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, log, 0, "destroy memory log buffer");

    log->wdata = ଖାଲି;
}

#endif
