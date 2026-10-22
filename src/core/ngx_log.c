
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर अक्षर *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
स्थिर अक्षर *ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log);
स्थिर शून्य ngx_log_घाला(ngx_log_t *log, ngx_log_t *new_log);


#जर (NGX_DEBUG)

स्थिर शून्य ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level,
    u_char *buf, माप_प्रकार len);
स्थिर शून्य ngx_log_memory_cleanup(शून्य *data);


प्रकारव्याख्या संरचना {
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

स्थिर स्थिरांक अक्षर *debug_levels[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#जर (NGX_HAVE_VARIADIC_MACROS)

शून्य
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    स्थिरांक अक्षर *fmt, ...)

#अन्यथा

शून्य
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    स्थिरांक अक्षर *fmt, बहु_सूची args)

#endif
{
#जर (NGX_HAVE_VARIADIC_MACROS)
    बहु_सूची      args;
#endif
    u_char      *p, *last, *संदेश;
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

    जर (log->connection) {
        p = ngx_slprintf(p, last, "*%uA ", log->connection);
    }

    संदेश = p;

#जर (NGX_HAVE_VARIADIC_MACROS)

    बहु_शुरू(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    बहु_पूर्ण(args);

#अन्यथा

    p = ngx_vslprintf(p, last, fmt, args);

#endif

    जर (err) {
        p = ngx_log_errno(p, last, err);
    }

    जर (level != NGX_LOG_DEBUG && log->handler) {
        p = log->handler(log, p, last - p);
    }

    जर (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (log->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    पर्यंत (log) {

        जर (log->log_level < level && !debug_connection) {
            थांबा;
        }

        जर (log->writer) {
            log->writer(log, level, errstr, p - errstr);
            जा next;
        }

        जर (ngx_time() == log->disk_full_time) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            जा next;
        }

        n = ngx_write_fd(log->file->fd, errstr, p - errstr);

        जर (n == -1 && ngx_errno == NGX_ENOSPC) {
            log->disk_full_time = ngx_time();
        }

        जर (log->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    next:

        log = log->next;
    }

    जर (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        परत;
    }

    संदेश -= (7 + err_levels[level].len + 3);

    (शून्य) ngx_sprintf(संदेश, "nginx: [%V] ", &err_levels[level]);

    (शून्य) ngx_write_console(ngx_stderr, संदेश, p - संदेश);
}


#जर !(NGX_HAVE_VARIADIC_MACROS)

शून्य ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    स्थिरांक अक्षर *fmt, ...)
{
    बहु_सूची  args;

    जर (log->log_level >= level) {
        बहु_शुरू(args, fmt);
        ngx_log_error_core(level, log, err, fmt, args);
        बहु_पूर्ण(args);
    }
}


शून्य ngx_cdecl
ngx_log_debug_core(ngx_log_t *log, ngx_err_t err, स्थिरांक अक्षर *fmt, ...)
{
    बहु_सूची  args;

    बहु_शुरू(args, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, log, err, fmt, args);
    बहु_पूर्ण(args);
}

#endif


शून्य ngx_cdecl
ngx_log_abort(ngx_err_t err, स्थिरांक अक्षर *fmt, ...)
{
    u_char   *p;
    बहु_सूची   args;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    बहु_शुरू(args, fmt);
    p = ngx_vsnprintf(errstr, आकार(errstr) - 1, fmt, args);
    बहु_पूर्ण(args);

    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                  "%*s", p - errstr, errstr);
}


शून्य ngx_cdecl
ngx_log_stderr(ngx_err_t err, स्थिरांक अक्षर *fmt, ...)
{
    u_char   *p, *last;
    बहु_सूची   args;
    u_char    errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    बहु_शुरू(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    बहु_पूर्ण(args);

    जर (err) {
        p = ngx_log_errno(p, last, err);
    }

    जर (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    (शून्य) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err)
{
    जर (buf > last - 50) {

        /* leave a space for an error code */

        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#जर (NGX_WIN32)
    buf = ngx_slprintf(buf, last, ((अचिन्हित) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#अन्यथा
    buf = ngx_slprintf(buf, last, " (%d: ", err);
#endif

    buf = ngx_strerror(err, buf, last - buf);

    जर (buf < last) {
        *buf++ = ')';
    }

    परत buf;
}


ngx_log_t *
ngx_log_init(u_char *prefix, u_char *error_log)
{
    u_char  *p, *name;
    माप_प्रकार   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    जर (error_log == रिक्त) {
        error_log = (u_char *) NGX_ERROR_LOG_PATH;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    जर (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        परत &ngx_log;
    }

    p = रिक्त;

#जर (NGX_WIN32)
    जर (name[1] != ':') {
#अन्यथा
    जर (name[0] != '/') {
#endif

        जर (prefix) {
            plen = ngx_strlen(prefix);

        } अन्यथा {
#ifdef NGX_PREFIX
            prefix = (u_char *) NGX_PREFIX;
            plen = ngx_strlen(prefix);
#अन्यथा
            plen = 0;
#endif
        }

        जर (plen) {
            name = दो_स्मृति(plen + nlen + 2);
            जर (name == रिक्त) {
                परत रिक्त;
            }

            p = ngx_cpymem(name, prefix, plen);

            जर (!ngx_path_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    जर (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#जर (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#endif

        ngx_log_file.fd = ngx_stderr;
    }

    जर (p) {
        ngx_free(p);
    }

    परत &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_cycle_t *cycle)
{
    ngx_log_t  *log;

    जर (ngx_log_get_file_log(&cycle->new_log) != रिक्त) {
        परत NGX_OK;
    }

    जर (cycle->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        log = ngx_pcalloc(cycle->pool, आकार(ngx_log_t));
        जर (log == रिक्त) {
            परत NGX_ERROR;
        }

    } अन्यथा {
        /* no error logs at all */
        log = &cycle->new_log;
    }

    log->log_level = NGX_LOG_ERR;

    log->file = ngx_conf_open_file(cycle, &cycle->error_log);
    जर (log->file == रिक्त) {
        परत NGX_ERROR;
    }

    जर (log != &cycle->new_log) {
        ngx_log_घाला(&cycle->new_log, log);
    }

    परत NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_cycle_t *cycle)
{
    ngx_fd_t  fd;

    जर (cycle->log_use_stderr) {
        परत NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(cycle->log)->file->fd;

    जर (fd != ngx_stderr) {
        जर (ngx_set_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");

            परत NGX_ERROR;
        }
    }

    परत NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *head)
{
    ngx_log_t  *log;

    साठी (log = head; log; log = log->next) {
        जर (log->file != रिक्त) {
            परत log;
        }
    }

    परत रिक्त;
}


स्थिर अक्षर *
ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log)
{
    ngx_uint_t   i, n, d, found;
    ngx_str_t   *मूल्य;

    जर (cf->args->nelts == 2) {
        log->log_level = NGX_LOG_ERR;
        परत NGX_CONF_OK;
    }

    मूल्य = cf->args->elts;

    साठी (i = 2; i < cf->args->nelts; i++) {
        found = 0;

        साठी (n = 1; n <= NGX_LOG_DEBUG; n++) {
            जर (ngx_strcmp(मूल्य[i].data, err_levels[n].data) == 0) {

                जर (log->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &मूल्य[i]);
                    परत NGX_CONF_ERROR;
                }

                log->log_level = n;
                found = 1;
                थांबा;
            }
        }

        साठी (n = 0, d = NGX_LOG_DEBUG_FIRST; d <= NGX_LOG_DEBUG_LAST; d <<= 1) {
            जर (ngx_strcmp(मूल्य[i].data, debug_levels[n++]) == 0) {
                जर (log->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &मूल्य[i]);
                    परत NGX_CONF_ERROR;
                }

                log->log_level |= d;
                found = 1;
                थांबा;
            }
        }


        जर (!found) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &मूल्य[i]);
            परत NGX_CONF_ERROR;
        }
    }

    जर (log->log_level == NGX_LOG_DEBUG) {
        log->log_level = NGX_LOG_DEBUG_ALL;
    }

    परत NGX_CONF_OK;
}


स्थिर अक्षर *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->cycle->new_log;

    परत ngx_log_set_log(cf, &dummy);
}


अक्षर *
ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **head)
{
    ngx_log_t          *new_log;
    ngx_str_t          *मूल्य, name;
    ngx_syslog_peer_t  *peer;

    जर (*head != रिक्त && (*head)->log_level == 0) {
        new_log = *head;

    } अन्यथा {

        new_log = ngx_pcalloc(cf->pool, आकार(ngx_log_t));
        जर (new_log == रिक्त) {
            परत NGX_CONF_ERROR;
        }

        जर (*head == रिक्त) {
            *head = new_log;
        }
    }

    मूल्य = cf->args->elts;

    जर (ngx_strcmp(मूल्य[1].data, "stderr") == 0) {
        ngx_str_null(&name);
        cf->cycle->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->cycle, &name);
        जर (new_log->file == रिक्त) {
            परत NGX_CONF_ERROR;
        }

    } अन्यथा जर (ngx_strncmp(मूल्य[1].data, "memory:", 7) == 0) {

#जर (NGX_DEBUG)
        माप_प्रकार                 size, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_memory_buf_t  *buf;

        मूल्य[1].len -= 7;
        मूल्य[1].data += 7;

        needed = आकार("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_SIZE_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        size = ngx_parse_size(&मूल्य[1]);

        जर (size == (माप_प्रकार) NGX_ERROR || size < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &मूल्य[1]);
            परत NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, आकार(ngx_log_memory_buf_t));
        जर (buf == रिक्त) {
            परत NGX_CONF_ERROR;
        }

        buf->start = ngx_pnalloc(cf->pool, size);
        जर (buf->start == रिक्त) {
            परत NGX_CONF_ERROR;
        }

        buf->end = buf->start + size;

        buf->pos = ngx_slprintf(buf->start, buf->end, "MEMLOG %uz %V:%ui%N",
                                size, &cf->conf_file->file.name,
                                cf->conf_file->ओळ);

        ngx_memset(buf->pos, ' ', buf->end - buf->pos);

        cln = ngx_pool_cleanup_add(cf->pool, 0);
        जर (cln == रिक्त) {
            परत NGX_CONF_ERROR;
        }

        cln->data = new_log;
        cln->handler = ngx_log_memory_cleanup;

        new_log->writer = ngx_log_memory_writer;
        new_log->wdata = buf;

#अन्यथा
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        परत NGX_CONF_ERROR;
#endif

    } अन्यथा जर (ngx_strncmp(मूल्य[1].data, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, आकार(ngx_syslog_peer_t));
        जर (peer == रिक्त) {
            परत NGX_CONF_ERROR;
        }

        जर (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            परत NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } अन्यथा {
        new_log->file = ngx_conf_open_file(cf->cycle, &मूल्य[1]);
        जर (new_log->file == रिक्त) {
            परत NGX_CONF_ERROR;
        }
    }

    जर (ngx_log_set_levels(cf, new_log) != NGX_CONF_OK) {
        परत NGX_CONF_ERROR;
    }

    जर (*head != new_log) {
        ngx_log_घाला(*head, new_log);
    }

    परत NGX_CONF_OK;
}


स्थिर शून्य
ngx_log_घाला(ngx_log_t *log, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    जर (new_log->log_level > log->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *log;
        *log = *new_log;
        *new_log = tmp;

        log->next = new_log;
        परत;
    }

    पर्यंत (log->next) {
        जर (new_log->log_level > log->next->log_level) {
            new_log->next = log->next;
            log->next = new_log;
            परत;
        }

        log = log->next;
    }

    log->next = new_log;
}


#जर (NGX_DEBUG)

स्थिर शून्य
ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    माप_प्रकार len)
{
    u_char                *p;
    माप_प्रकार                 avail, written;
    ngx_log_memory_buf_t  *mem;

    mem = log->wdata;

    जर (mem == रिक्त) {
        परत;
    }

    written = ngx_atomic_fetch_add(&mem->written, len);

    p = mem->pos + written % (mem->end - mem->pos);

    avail = mem->end - p;

    जर (avail >= len) {
        ngx_memcpy(p, buf, len);

    } अन्यथा {
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
