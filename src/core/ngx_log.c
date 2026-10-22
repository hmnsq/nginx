
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


સ્થિર અક્ષર *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf);
સ્થિર અક્ષર *ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log);
સ્થિર શૂન્ય ngx_log_ઉમેરો(ngx_log_t *log, ngx_log_t *new_log);


#જો (NGX_DEBUG)

સ્થિર શૂન્ય ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level,
    u_char *buf, size_t len);
સ્થિર શૂન્ય ngx_log_memory_cleanup(શૂન્ય *data);


પ્રકારવ્યાખ્યા સંરચના {
    u_char        *start;
    u_char        *end;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_memory_buf_t;

#endif


સ્થિર ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1MORE,
      ngx_error_log,
      0,
      0,
      ખાલી },

      ngx_null_command
};


સ્થિર ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    ખાલી,
    ખાલી
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ખાલી,                                  /* init master */
    ખાલી,                                  /* init module */
    ખાલી,                                  /* init process */
    ખાલી,                                  /* init thread */
    ખાલી,                                  /* exit thread */
    ખાલી,                                  /* exit process */
    ખાલી,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


સ્થિર ngx_log_t        ngx_log;
સ્થિર ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


સ્થિર ngx_str_t err_levels[] = {
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

સ્થિર અચલ અક્ષર *debug_levels[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#જો (NGX_HAVE_VARIADIC_MACROS)

શૂન્ય
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    અચલ અક્ષર *fmt, ...)

#નહીંતર

શૂન્ય
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    અચલ અક્ષર *fmt, va_list args)

#endif
{
#જો (NGX_HAVE_VARIADIC_MACROS)
    va_list      args;
#endif
    u_char      *p, *last, *સંદેશ;
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

    જો (log->connection) {
        p = ngx_slprintf(p, last, "*%uA ", log->connection);
    }

    સંદેશ = p;

#જો (NGX_HAVE_VARIADIC_MACROS)

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

#નહીંતર

    p = ngx_vslprintf(p, last, fmt, args);

#endif

    જો (err) {
        p = ngx_log_errno(p, last, err);
    }

    જો (level != NGX_LOG_DEBUG && log->handler) {
        p = log->handler(log, p, last - p);
    }

    જો (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (log->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    જ્યારે (log) {

        જો (log->log_level < level && !debug_connection) {
            તોડો;
        }

        જો (log->writer) {
            log->writer(log, level, errstr, p - errstr);
            જાઓ next;
        }

        જો (ngx_time() == log->disk_full_time) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            જાઓ next;
        }

        n = ngx_write_fd(log->file->fd, errstr, p - errstr);

        જો (n == -1 && ngx_errno == NGX_ENOSPC) {
            log->disk_full_time = ngx_time();
        }

        જો (log->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    next:

        log = log->next;
    }

    જો (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        પાછા;
    }

    સંદેશ -= (7 + err_levels[level].len + 3);

    (શૂન્ય) ngx_sprintf(સંદેશ, "nginx: [%V] ", &err_levels[level]);

    (શૂન્ય) ngx_write_console(ngx_stderr, સંદેશ, p - સંદેશ);
}


#જો !(NGX_HAVE_VARIADIC_MACROS)

શૂન્ય ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    અચલ અક્ષર *fmt, ...)
{
    va_list  args;

    જો (log->log_level >= level) {
        va_start(args, fmt);
        ngx_log_error_core(level, log, err, fmt, args);
        va_end(args);
    }
}


શૂન્ય ngx_cdecl
ngx_log_debug_core(ngx_log_t *log, ngx_err_t err, અચલ અક્ષર *fmt, ...)
{
    va_list  args;

    va_start(args, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, log, err, fmt, args);
    va_end(args);
}

#endif


શૂન્ય ngx_cdecl
ngx_log_abort(ngx_err_t err, અચલ અક્ષર *fmt, ...)
{
    u_char   *p;
    va_list   args;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_start(args, fmt);
    p = ngx_vsnprintf(errstr, કદ(errstr) - 1, fmt, args);
    va_end(args);

    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                  "%*s", p - errstr, errstr);
}


શૂન્ય ngx_cdecl
ngx_log_stderr(ngx_err_t err, અચલ અક્ષર *fmt, ...)
{
    u_char   *p, *last;
    va_list   args;
    u_char    errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

    જો (err) {
        p = ngx_log_errno(p, last, err);
    }

    જો (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    (શૂન્ય) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err)
{
    જો (buf > last - 50) {

        /* leave a space for an error code */

        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#જો (NGX_WIN32)
    buf = ngx_slprintf(buf, last, ((unsigned) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#નહીંતર
    buf = ngx_slprintf(buf, last, " (%d: ", err);
#endif

    buf = ngx_strerror(err, buf, last - buf);

    જો (buf < last) {
        *buf++ = ')';
    }

    પાછા buf;
}


ngx_log_t *
ngx_log_init(u_char *prefix, u_char *error_log)
{
    u_char  *p, *name;
    size_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    જો (error_log == ખાલી) {
        error_log = (u_char *) NGX_ERROR_LOG_PATH;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    જો (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        પાછા &ngx_log;
    }

    p = ખાલી;

#જો (NGX_WIN32)
    જો (name[1] != ':') {
#નહીંતર
    જો (name[0] != '/') {
#endif

        જો (prefix) {
            plen = ngx_strlen(prefix);

        } નહીંતર {
#ifdef NGX_PREFIX
            prefix = (u_char *) NGX_PREFIX;
            plen = ngx_strlen(prefix);
#નહીંતર
            plen = 0;
#endif
        }

        જો (plen) {
            name = malloc(plen + nlen + 2);
            જો (name == ખાલી) {
                પાછા ખાલી;
            }

            p = ngx_cpymem(name, prefix, plen);

            જો (!ngx_path_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    જો (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#જો (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#endif

        ngx_log_file.fd = ngx_stderr;
    }

    જો (p) {
        ngx_free(p);
    }

    પાછા &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_cycle_t *cycle)
{
    ngx_log_t  *log;

    જો (ngx_log_get_file_log(&cycle->new_log) != ખાલી) {
        પાછા NGX_OK;
    }

    જો (cycle->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        log = ngx_pcalloc(cycle->pool, કદ(ngx_log_t));
        જો (log == ખાલી) {
            પાછા NGX_ERROR;
        }

    } નહીંતર {
        /* no error logs at all */
        log = &cycle->new_log;
    }

    log->log_level = NGX_LOG_ERR;

    log->file = ngx_conf_open_file(cycle, &cycle->error_log);
    જો (log->file == ખાલી) {
        પાછા NGX_ERROR;
    }

    જો (log != &cycle->new_log) {
        ngx_log_ઉમેરો(&cycle->new_log, log);
    }

    પાછા NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_cycle_t *cycle)
{
    ngx_fd_t  fd;

    જો (cycle->log_use_stderr) {
        પાછા NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(cycle->log)->file->fd;

    જો (fd != ngx_stderr) {
        જો (ngx_set_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");

            પાછા NGX_ERROR;
        }
    }

    પાછા NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *head)
{
    ngx_log_t  *log;

    માટે (log = head; log; log = log->next) {
        જો (log->file != ખાલી) {
            પાછા log;
        }
    }

    પાછા ખાલી;
}


સ્થિર અક્ષર *
ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log)
{
    ngx_uint_t   i, n, d, found;
    ngx_str_t   *કિંમત;

    જો (cf->args->nelts == 2) {
        log->log_level = NGX_LOG_ERR;
        પાછા NGX_CONF_OK;
    }

    કિંમત = cf->args->elts;

    માટે (i = 2; i < cf->args->nelts; i++) {
        found = 0;

        માટે (n = 1; n <= NGX_LOG_DEBUG; n++) {
            જો (ngx_strcmp(કિંમત[i].data, err_levels[n].data) == 0) {

                જો (log->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &કિંમત[i]);
                    પાછા NGX_CONF_ERROR;
                }

                log->log_level = n;
                found = 1;
                તોડો;
            }
        }

        માટે (n = 0, d = NGX_LOG_DEBUG_FIRST; d <= NGX_LOG_DEBUG_LAST; d <<= 1) {
            જો (ngx_strcmp(કિંમત[i].data, debug_levels[n++]) == 0) {
                જો (log->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &કિંમત[i]);
                    પાછા NGX_CONF_ERROR;
                }

                log->log_level |= d;
                found = 1;
                તોડો;
            }
        }


        જો (!found) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &કિંમત[i]);
            પાછા NGX_CONF_ERROR;
        }
    }

    જો (log->log_level == NGX_LOG_DEBUG) {
        log->log_level = NGX_LOG_DEBUG_ALL;
    }

    પાછા NGX_CONF_OK;
}


સ્થિર અક્ષર *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->cycle->new_log;

    પાછા ngx_log_set_log(cf, &dummy);
}


અક્ષર *
ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **head)
{
    ngx_log_t          *new_log;
    ngx_str_t          *કિંમત, name;
    ngx_syslog_peer_t  *peer;

    જો (*head != ખાલી && (*head)->log_level == 0) {
        new_log = *head;

    } નહીંતર {

        new_log = ngx_pcalloc(cf->pool, કદ(ngx_log_t));
        જો (new_log == ખાલી) {
            પાછા NGX_CONF_ERROR;
        }

        જો (*head == ખાલી) {
            *head = new_log;
        }
    }

    કિંમત = cf->args->elts;

    જો (ngx_strcmp(કિંમત[1].data, "stderr") == 0) {
        ngx_str_null(&name);
        cf->cycle->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->cycle, &name);
        જો (new_log->file == ખાલી) {
            પાછા NGX_CONF_ERROR;
        }

    } નહીંતર જો (ngx_strncmp(કિંમત[1].data, "memory:", 7) == 0) {

#જો (NGX_DEBUG)
        size_t                 size, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_memory_buf_t  *buf;

        કિંમત[1].len -= 7;
        કિંમત[1].data += 7;

        needed = કદ("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_SIZE_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        size = ngx_parse_size(&કિંમત[1]);

        જો (size == (size_t) NGX_ERROR || size < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &કિંમત[1]);
            પાછા NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, કદ(ngx_log_memory_buf_t));
        જો (buf == ખાલી) {
            પાછા NGX_CONF_ERROR;
        }

        buf->start = ngx_pnalloc(cf->pool, size);
        જો (buf->start == ખાલી) {
            પાછા NGX_CONF_ERROR;
        }

        buf->end = buf->start + size;

        buf->pos = ngx_slprintf(buf->start, buf->end, "MEMLOG %uz %V:%ui%N",
                                size, &cf->conf_file->file.name,
                                cf->conf_file->લીટી);

        ngx_memset(buf->pos, ' ', buf->end - buf->pos);

        cln = ngx_pool_cleanup_add(cf->pool, 0);
        જો (cln == ખાલી) {
            પાછા NGX_CONF_ERROR;
        }

        cln->data = new_log;
        cln->handler = ngx_log_memory_cleanup;

        new_log->writer = ngx_log_memory_writer;
        new_log->wdata = buf;

#નહીંતર
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        પાછા NGX_CONF_ERROR;
#endif

    } નહીંતર જો (ngx_strncmp(કિંમત[1].data, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, કદ(ngx_syslog_peer_t));
        જો (peer == ખાલી) {
            પાછા NGX_CONF_ERROR;
        }

        જો (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            પાછા NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } નહીંતર {
        new_log->file = ngx_conf_open_file(cf->cycle, &કિંમત[1]);
        જો (new_log->file == ખાલી) {
            પાછા NGX_CONF_ERROR;
        }
    }

    જો (ngx_log_set_levels(cf, new_log) != NGX_CONF_OK) {
        પાછા NGX_CONF_ERROR;
    }

    જો (*head != new_log) {
        ngx_log_ઉમેરો(*head, new_log);
    }

    પાછા NGX_CONF_OK;
}


સ્થિર શૂન્ય
ngx_log_ઉમેરો(ngx_log_t *log, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    જો (new_log->log_level > log->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *log;
        *log = *new_log;
        *new_log = tmp;

        log->next = new_log;
        પાછા;
    }

    જ્યારે (log->next) {
        જો (new_log->log_level > log->next->log_level) {
            new_log->next = log->next;
            log->next = new_log;
            પાછા;
        }

        log = log->next;
    }

    log->next = new_log;
}


#જો (NGX_DEBUG)

સ્થિર શૂન્ય
ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char                *p;
    size_t                 avail, written;
    ngx_log_memory_buf_t  *mem;

    mem = log->wdata;

    જો (mem == ખાલી) {
        પાછા;
    }

    written = ngx_atomic_fetch_add(&mem->written, len);

    p = mem->pos + written % (mem->end - mem->pos);

    avail = mem->end - p;

    જો (avail >= len) {
        ngx_memcpy(p, buf, len);

    } નહીંતર {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


સ્થિર શૂન્ય
ngx_log_memory_cleanup(શૂન્ય *data)
{
    ngx_log_t *log = data;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, log, 0, "destroy memory log buffer");

    log->wdata = ખાલી;
}

#endif
