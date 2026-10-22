
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर वर्ण *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
स्थिर वर्ण *ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log);
स्थिर शून्य ngx_log_निवेशय(ngx_log_t *log, ngx_log_t *new_log);


#जइ (NGX_DEBUG)

स्थिर शून्य ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level,
    u_char *buf, size_t len);
स्थिर शून्य ngx_log_memory_cleanup(शून्य *data);


प्रकारनिर्देश संरचना {
    u_char        *start;
    u_char        *end;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_memory_buf_t;

#endif


स्थिर ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1MORE,
      ngx_error_log,
      0,
      0,
      रिक्त },

      ngx_null_command
};


स्थिर ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    रिक्त,
    रिक्त
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    रिक्त,                                  /* init master */
    रिक्त,                                  /* init module */
    रिक्त,                                  /* init process */
    रिक्त,                                  /* init thread */
    रिक्त,                                  /* exit thread */
    रिक्त,                                  /* exit process */
    रिक्त,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


स्थिर ngx_log_t        ngx_log;
स्थिर ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


स्थिर ngx_str_t err_levels[] = {
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

स्थिर ध्रुव वर्ण *debug_levels[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#जइ (NGX_HAVE_VARIADIC_MACROS)

शून्य
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ध्रुव वर्ण *fmt, ...)

#अण्णहा

शून्य
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ध्रुव वर्ण *fmt, va_list args)

#endif
{
#जइ (NGX_HAVE_VARIADIC_MACROS)
    va_list      args;
#endif
    u_char      *p, *last, *सन्देशः;
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

    जइ (log->connection) {
        p = ngx_slprintf(p, last, "*%uA ", log->connection);
    }

    सन्देशः = p;

#जइ (NGX_HAVE_VARIADIC_MACROS)

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

#अण्णहा

    p = ngx_vslprintf(p, last, fmt, args);

#endif

    जइ (err) {
        p = ngx_log_errno(p, last, err);
    }

    जइ (level != NGX_LOG_DEBUG && log->handler) {
        p = log->handler(log, p, last - p);
    }

    जइ (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (log->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    जाव (log) {

        जइ (log->log_level < level && !debug_connection) {
            भङ्ग;
        }

        जइ (log->writer) {
            log->writer(log, level, errstr, p - errstr);
            गच्छ next;
        }

        जइ (ngx_time() == log->disk_full_time) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            गच्छ next;
        }

        n = ngx_write_fd(log->file->fd, errstr, p - errstr);

        जइ (n == -1 && ngx_errno == NGX_ENOSPC) {
            log->disk_full_time = ngx_time();
        }

        जइ (log->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    next:

        log = log->next;
    }

    जइ (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        प्रत्यागम;
    }

    सन्देशः -= (7 + err_levels[level].len + 3);

    (शून्य) ngx_sprintf(सन्देशः, "nginx: [%V] ", &err_levels[level]);

    (शून्य) ngx_write_console(ngx_stderr, सन्देशः, p - सन्देशः);
}


#जइ !(NGX_HAVE_VARIADIC_MACROS)

शून्य ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ध्रुव वर्ण *fmt, ...)
{
    va_list  args;

    जइ (log->log_level >= level) {
        va_start(args, fmt);
        ngx_log_error_core(level, log, err, fmt, args);
        va_end(args);
    }
}


शून्य ngx_cdecl
ngx_log_debug_core(ngx_log_t *log, ngx_err_t err, ध्रुव वर्ण *fmt, ...)
{
    va_list  args;

    va_start(args, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, log, err, fmt, args);
    va_end(args);
}

#endif


शून्य ngx_cdecl
ngx_log_abort(ngx_err_t err, ध्रुव वर्ण *fmt, ...)
{
    u_char   *p;
    va_list   args;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_start(args, fmt);
    p = ngx_vsnprintf(errstr, परिमाण(errstr) - 1, fmt, args);
    va_end(args);

    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                  "%*s", p - errstr, errstr);
}


शून्य ngx_cdecl
ngx_log_stderr(ngx_err_t err, ध्रुव वर्ण *fmt, ...)
{
    u_char   *p, *last;
    va_list   args;
    u_char    errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

    जइ (err) {
        p = ngx_log_errno(p, last, err);
    }

    जइ (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    (शून्य) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err)
{
    जइ (buf > last - 50) {

        /* leave a space for an error code */

        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#जइ (NGX_WIN32)
    buf = ngx_slprintf(buf, last, ((unsigned) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#अण्णहा
    buf = ngx_slprintf(buf, last, " (%d: ", err);
#endif

    buf = ngx_strerror(err, buf, last - buf);

    जइ (buf < last) {
        *buf++ = ')';
    }

    प्रत्यागम buf;
}


ngx_log_t *
ngx_log_init(u_char *prefix, u_char *error_log)
{
    u_char  *p, *name;
    size_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    जइ (error_log == रिक्त) {
        error_log = (u_char *) NGX_ERROR_LOG_PATH;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    जइ (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        प्रत्यागम &ngx_log;
    }

    p = रिक्त;

#जइ (NGX_WIN32)
    जइ (name[1] != ':') {
#अण्णहा
    जइ (name[0] != '/') {
#endif

        जइ (prefix) {
            plen = ngx_strlen(prefix);

        } अण्णहा {
#ifdef NGX_PREFIX
            prefix = (u_char *) NGX_PREFIX;
            plen = ngx_strlen(prefix);
#अण्णहा
            plen = 0;
#endif
        }

        जइ (plen) {
            name = malloc(plen + nlen + 2);
            जइ (name == रिक्त) {
                प्रत्यागम रिक्त;
            }

            p = ngx_cpymem(name, prefix, plen);

            जइ (!ngx_path_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    जइ (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#जइ (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#endif

        ngx_log_file.fd = ngx_stderr;
    }

    जइ (p) {
        ngx_free(p);
    }

    प्रत्यागम &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_cycle_t *cycle)
{
    ngx_log_t  *log;

    जइ (ngx_log_get_file_log(&cycle->new_log) != रिक्त) {
        प्रत्यागम NGX_OK;
    }

    जइ (cycle->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        log = ngx_pcalloc(cycle->pool, परिमाण(ngx_log_t));
        जइ (log == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

    } अण्णहा {
        /* no error logs at all */
        log = &cycle->new_log;
    }

    log->log_level = NGX_LOG_ERR;

    log->file = ngx_conf_open_file(cycle, &cycle->error_log);
    जइ (log->file == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    जइ (log != &cycle->new_log) {
        ngx_log_निवेशय(&cycle->new_log, log);
    }

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_cycle_t *cycle)
{
    ngx_fd_t  fd;

    जइ (cycle->log_use_stderr) {
        प्रत्यागम NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(cycle->log)->file->fd;

    जइ (fd != ngx_stderr) {
        जइ (ngx_set_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");

            प्रत्यागम NGX_ERROR;
        }
    }

    प्रत्यागम NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *head)
{
    ngx_log_t  *log;

    प्रति (log = head; log; log = log->next) {
        जइ (log->file != रिक्त) {
            प्रत्यागम log;
        }
    }

    प्रत्यागम रिक्त;
}


स्थिर वर्ण *
ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log)
{
    ngx_uint_t   i, n, d, found;
    ngx_str_t   *मानम्;

    जइ (cf->args->nelts == 2) {
        log->log_level = NGX_LOG_ERR;
        प्रत्यागम NGX_CONF_OK;
    }

    मानम् = cf->args->elts;

    प्रति (i = 2; i < cf->args->nelts; i++) {
        found = 0;

        प्रति (n = 1; n <= NGX_LOG_DEBUG; n++) {
            जइ (ngx_strcmp(मानम्[i].data, err_levels[n].data) == 0) {

                जइ (log->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &मानम्[i]);
                    प्रत्यागम NGX_CONF_ERROR;
                }

                log->log_level = n;
                found = 1;
                भङ्ग;
            }
        }

        प्रति (n = 0, d = NGX_LOG_DEBUG_FIRST; d <= NGX_LOG_DEBUG_LAST; d <<= 1) {
            जइ (ngx_strcmp(मानम्[i].data, debug_levels[n++]) == 0) {
                जइ (log->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &मानम्[i]);
                    प्रत्यागम NGX_CONF_ERROR;
                }

                log->log_level |= d;
                found = 1;
                भङ्ग;
            }
        }


        जइ (!found) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &मानम्[i]);
            प्रत्यागम NGX_CONF_ERROR;
        }
    }

    जइ (log->log_level == NGX_LOG_DEBUG) {
        log->log_level = NGX_LOG_DEBUG_ALL;
    }

    प्रत्यागम NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->cycle->new_log;

    प्रत्यागम ngx_log_set_log(cf, &dummy);
}


वर्ण *
ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **head)
{
    ngx_log_t          *new_log;
    ngx_str_t          *मानम्, name;
    ngx_syslog_peer_t  *peer;

    जइ (*head != रिक्त && (*head)->log_level == 0) {
        new_log = *head;

    } अण्णहा {

        new_log = ngx_pcalloc(cf->pool, परिमाण(ngx_log_t));
        जइ (new_log == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        जइ (*head == रिक्त) {
            *head = new_log;
        }
    }

    मानम् = cf->args->elts;

    जइ (ngx_strcmp(मानम्[1].data, "stderr") == 0) {
        ngx_str_null(&name);
        cf->cycle->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->cycle, &name);
        जइ (new_log->file == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

    } अण्णहा जइ (ngx_strncmp(मानम्[1].data, "memory:", 7) == 0) {

#जइ (NGX_DEBUG)
        size_t                 size, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_memory_buf_t  *buf;

        मानम्[1].len -= 7;
        मानम्[1].data += 7;

        needed = परिमाण("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_SIZE_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        size = ngx_parse_size(&मानम्[1]);

        जइ (size == (size_t) NGX_ERROR || size < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &मानम्[1]);
            प्रत्यागम NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, परिमाण(ngx_log_memory_buf_t));
        जइ (buf == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        buf->start = ngx_pnalloc(cf->pool, size);
        जइ (buf->start == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        buf->end = buf->start + size;

        buf->pos = ngx_slprintf(buf->start, buf->end, "MEMLOG %uz %V:%ui%N",
                                size, &cf->conf_file->file.name,
                                cf->conf_file->पङ्क्तिः);

        ngx_memset(buf->pos, ' ', buf->end - buf->pos);

        cln = ngx_pool_cleanup_add(cf->pool, 0);
        जइ (cln == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        cln->data = new_log;
        cln->handler = ngx_log_memory_cleanup;

        new_log->writer = ngx_log_memory_writer;
        new_log->wdata = buf;

#अण्णहा
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        प्रत्यागम NGX_CONF_ERROR;
#endif

    } अण्णहा जइ (ngx_strncmp(मानम्[1].data, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, परिमाण(ngx_syslog_peer_t));
        जइ (peer == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        जइ (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } अण्णहा {
        new_log->file = ngx_conf_open_file(cf->cycle, &मानम्[1]);
        जइ (new_log->file == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }
    }

    जइ (ngx_log_set_levels(cf, new_log) != NGX_CONF_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    जइ (*head != new_log) {
        ngx_log_निवेशय(*head, new_log);
    }

    प्रत्यागम NGX_CONF_OK;
}


स्थिर शून्य
ngx_log_निवेशय(ngx_log_t *log, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    जइ (new_log->log_level > log->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *log;
        *log = *new_log;
        *new_log = tmp;

        log->next = new_log;
        प्रत्यागम;
    }

    जाव (log->next) {
        जइ (new_log->log_level > log->next->log_level) {
            new_log->next = log->next;
            log->next = new_log;
            प्रत्यागम;
        }

        log = log->next;
    }

    log->next = new_log;
}


#जइ (NGX_DEBUG)

स्थिर शून्य
ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char                *p;
    size_t                 avail, written;
    ngx_log_memory_buf_t  *mem;

    mem = log->wdata;

    जइ (mem == रिक्त) {
        प्रत्यागम;
    }

    written = ngx_atomic_fetch_add(&mem->written, len);

    p = mem->pos + written % (mem->end - mem->pos);

    avail = mem->end - p;

    जइ (avail >= len) {
        ngx_memcpy(p, buf, len);

    } अण्णहा {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


स्थिर शून्य
ngx_log_memory_cleanup(शून्य *data)
{
    ngx_log_t *log = data;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, log, 0, "destroy memory log buffer");

    log->wdata = रिक्त;
}

#endif
