
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


স্থির অক্ষর *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf);
স্থির অক্ষর *ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log);
স্থির শূন্য ngx_log_ঢোকাও(ngx_log_t *log, ngx_log_t *new_log);


#যদি (NGX_DEBUG)

স্থির শূন্য ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level,
    u_char *buf, আকার_t len);
স্থির শূন্য ngx_log_memory_cleanup(শূন্য *তথ্য);


প্রকারলেখ কাঠামো {
    u_char        *start;
    u_char        *end;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_memory_buf_t;

#endif


স্থির ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1MORE,
      ngx_error_log,
      0,
      0,
      ফাঁকা },

      ngx_null_command
};


স্থির ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    ফাঁকা,
    ফাঁকা
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ফাঁকা,                                  /* init master */
    ফাঁকা,                                  /* init module */
    ফাঁকা,                                  /* init process */
    ফাঁকা,                                  /* init thread */
    ফাঁকা,                                  /* exit thread */
    ফাঁকা,                                  /* exit process */
    ফাঁকা,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


স্থির ngx_log_t        ngx_log;
স্থির ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


স্থির ngx_str_t err_levels[] = {
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

স্থির ধ্রুবক অক্ষর *debug_levels[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#যদি (NGX_HAVE_VARIADIC_MACROS)

শূন্য
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ধ্রুবক অক্ষর *fmt, ...)

#অন্যথা

শূন্য
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ধ্রুবক অক্ষর *fmt, va_list args)

#endif
{
#যদি (NGX_HAVE_VARIADIC_MACROS)
    va_list      args;
#endif
    u_char      *p, *last, *বার্তা;
    ssize_t      n;
    ngx_uint_t   wrote_stderr, debug_connection;
    u_char       errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, ngx_cached_err_log_time.তথ্য,
                   ngx_cached_err_log_time.len);

    p = ngx_slprintf(p, last, " [%V] ", &err_levels[level]);

    /* pid#tid */
    p = ngx_slprintf(p, last, "%P#" NGX_TID_T_FMT ": ",
                    ngx_log_pid, ngx_log_tid);

    যদি (log->connection) {
        p = ngx_slprintf(p, last, "*%uA ", log->connection);
    }

    বার্তা = p;

#যদি (NGX_HAVE_VARIADIC_MACROS)

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

#অন্যথা

    p = ngx_vslprintf(p, last, fmt, args);

#endif

    যদি (err) {
        p = ngx_log_errno(p, last, err);
    }

    যদি (level != NGX_LOG_DEBUG && log->handler) {
        p = log->handler(log, p, last - p);
    }

    যদি (p > last - NGX_LINEFEED_আকার) {
        p = last - NGX_LINEFEED_আকার;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (log->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    যখন (log) {

        যদি (log->log_level < level && !debug_connection) {
            বিরতি;
        }

        যদি (log->writer) {
            log->writer(log, level, errstr, p - errstr);
            যাও পরবর্তী;
        }

        যদি (ngx_time() == log->disk_full_time) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            যাও পরবর্তী;
        }

        n = ngx_write_fd(log->file->fd, errstr, p - errstr);

        যদি (n == -1 && ngx_errno == NGX_ENOSPC) {
            log->disk_full_time = ngx_time();
        }

        যদি (log->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    পরবর্তী:

        log = log->পরবর্তী;
    }

    যদি (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        ফেরত;
    }

    বার্তা -= (7 + err_levels[level].len + 3);

    (শূন্য) ngx_sprintf(বার্তা, "nginx: [%V] ", &err_levels[level]);

    (শূন্য) ngx_write_console(ngx_stderr, বার্তা, p - বার্তা);
}


#যদি !(NGX_HAVE_VARIADIC_MACROS)

শূন্য ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    ধ্রুবক অক্ষর *fmt, ...)
{
    va_list  args;

    যদি (log->log_level >= level) {
        va_start(args, fmt);
        ngx_log_error_core(level, log, err, fmt, args);
        va_end(args);
    }
}


শূন্য ngx_cdecl
ngx_log_debug_core(ngx_log_t *log, ngx_err_t err, ধ্রুবক অক্ষর *fmt, ...)
{
    va_list  args;

    va_start(args, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, log, err, fmt, args);
    va_end(args);
}

#endif


শূন্য ngx_cdecl
ngx_log_abort(ngx_err_t err, ধ্রুবক অক্ষর *fmt, ...)
{
    u_char   *p;
    va_list   args;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_start(args, fmt);
    p = ngx_vsnprintf(errstr, আকার(errstr) - 1, fmt, args);
    va_end(args);

    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                  "%*s", p - errstr, errstr);
}


শূন্য ngx_cdecl
ngx_log_stderr(ngx_err_t err, ধ্রুবক অক্ষর *fmt, ...)
{
    u_char   *p, *last;
    va_list   args;
    u_char    errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

    যদি (err) {
        p = ngx_log_errno(p, last, err);
    }

    যদি (p > last - NGX_LINEFEED_আকার) {
        p = last - NGX_LINEFEED_আকার;
    }

    ngx_linefeed(p);

    (শূন্য) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err)
{
    যদি (buf > last - 50) {

        /* leave a space for an error code */

        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#যদি (NGX_WIN32)
    buf = ngx_slprintf(buf, last, ((unsigned) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#অন্যথা
    buf = ngx_slprintf(buf, last, " (%d: ", err);
#endif

    buf = ngx_strerror(err, buf, last - buf);

    যদি (buf < last) {
        *buf++ = ')';
    }

    ফেরত buf;
}


ngx_log_t *
ngx_log_init(u_char *prefix, u_char *error_log)
{
    u_char  *p, *name;
    আকার_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    যদি (error_log == ফাঁকা) {
        error_log = (u_char *) NGX_ERROR_LOG_PATH;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    যদি (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        ফেরত &ngx_log;
    }

    p = ফাঁকা;

#যদি (NGX_WIN32)
    যদি (name[1] != ':') {
#অন্যথা
    যদি (name[0] != '/') {
#endif

        যদি (prefix) {
            plen = ngx_strlen(prefix);

        } অন্যথা {
#ifdef NGX_PREFIX
            prefix = (u_char *) NGX_PREFIX;
            plen = ngx_strlen(prefix);
#অন্যথা
            plen = 0;
#endif
        }

        যদি (plen) {
            name = malloc(plen + nlen + 2);
            যদি (name == ফাঁকা) {
                ফেরত ফাঁকা;
            }

            p = ngx_cpymem(name, prefix, plen);

            যদি (!ngx_path_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    যদি (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#যদি (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#endif

        ngx_log_file.fd = ngx_stderr;
    }

    যদি (p) {
        ngx_free(p);
    }

    ফেরত &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_cycle_t *cycle)
{
    ngx_log_t  *log;

    যদি (ngx_log_get_file_log(&cycle->new_log) != ফাঁকা) {
        ফেরত NGX_OK;
    }

    যদি (cycle->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        log = ngx_pcalloc(cycle->pool, আকার(ngx_log_t));
        যদি (log == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

    } অন্যথা {
        /* no error logs at all */
        log = &cycle->new_log;
    }

    log->log_level = NGX_LOG_ERR;

    log->file = ngx_conf_open_file(cycle, &cycle->error_log);
    যদি (log->file == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    যদি (log != &cycle->new_log) {
        ngx_log_ঢোকাও(&cycle->new_log, log);
    }

    ফেরত NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_cycle_t *cycle)
{
    ngx_fd_t  fd;

    যদি (cycle->log_use_stderr) {
        ফেরত NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(cycle->log)->file->fd;

    যদি (fd != ngx_stderr) {
        যদি (ngx_set_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");

            ফেরত NGX_ERROR;
        }
    }

    ফেরত NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *শীর্ষ)
{
    ngx_log_t  *log;

    জন্য (log = শীর্ষ; log; log = log->পরবর্তী) {
        যদি (log->file != ফাঁকা) {
            ফেরত log;
        }
    }

    ফেরত ফাঁকা;
}


স্থির অক্ষর *
ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log)
{
    ngx_uint_t   i, n, d, found;
    ngx_str_t   *মান;

    যদি (cf->args->nelts == 2) {
        log->log_level = NGX_LOG_ERR;
        ফেরত NGX_CONF_OK;
    }

    মান = cf->args->elts;

    জন্য (i = 2; i < cf->args->nelts; i++) {
        found = 0;

        জন্য (n = 1; n <= NGX_LOG_DEBUG; n++) {
            যদি (ngx_strcmp(মান[i].তথ্য, err_levels[n].তথ্য) == 0) {

                যদি (log->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &মান[i]);
                    ফেরত NGX_CONF_ERROR;
                }

                log->log_level = n;
                found = 1;
                বিরতি;
            }
        }

        জন্য (n = 0, d = NGX_LOG_DEBUG_FIRST; d <= NGX_LOG_DEBUG_LAST; d <<= 1) {
            যদি (ngx_strcmp(মান[i].তথ্য, debug_levels[n++]) == 0) {
                যদি (log->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &মান[i]);
                    ফেরত NGX_CONF_ERROR;
                }

                log->log_level |= d;
                found = 1;
                বিরতি;
            }
        }


        যদি (!found) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &মান[i]);
            ফেরত NGX_CONF_ERROR;
        }
    }

    যদি (log->log_level == NGX_LOG_DEBUG) {
        log->log_level = NGX_LOG_DEBUG_ALL;
    }

    ফেরত NGX_CONF_OK;
}


স্থির অক্ষর *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->cycle->new_log;

    ফেরত ngx_log_set_log(cf, &dummy);
}


অক্ষর *
ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **শীর্ষ)
{
    ngx_log_t          *new_log;
    ngx_str_t          *মান, name;
    ngx_syslog_peer_t  *peer;

    যদি (*শীর্ষ != ফাঁকা && (*শীর্ষ)->log_level == 0) {
        new_log = *শীর্ষ;

    } অন্যথা {

        new_log = ngx_pcalloc(cf->pool, আকার(ngx_log_t));
        যদি (new_log == ফাঁকা) {
            ফেরত NGX_CONF_ERROR;
        }

        যদি (*শীর্ষ == ফাঁকা) {
            *শীর্ষ = new_log;
        }
    }

    মান = cf->args->elts;

    যদি (ngx_strcmp(মান[1].তথ্য, "stderr") == 0) {
        ngx_str_null(&name);
        cf->cycle->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->cycle, &name);
        যদি (new_log->file == ফাঁকা) {
            ফেরত NGX_CONF_ERROR;
        }

    } অন্যথা যদি (ngx_strncmp(মান[1].তথ্য, "memory:", 7) == 0) {

#যদি (NGX_DEBUG)
        আকার_t                 আকার_2, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_memory_buf_t  *buf;

        মান[1].len -= 7;
        মান[1].তথ্য += 7;

        needed = আকার("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_আকার_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        আকার_2 = ngx_parse_আকার(&মান[1]);

        যদি (আকার_2 == (আকার_t) NGX_ERROR || আকার_2 < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &মান[1]);
            ফেরত NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, আকার(ngx_log_memory_buf_t));
        যদি (buf == ফাঁকা) {
            ফেরত NGX_CONF_ERROR;
        }

        buf->start = ngx_pnalloc(cf->pool, আকার_2);
        যদি (buf->start == ফাঁকা) {
            ফেরত NGX_CONF_ERROR;
        }

        buf->end = buf->start + আকার_2;

        buf->pos = ngx_slprintf(buf->start, buf->end, "MEMLOG %uz %V:%ui%N",
                                আকার_2, &cf->conf_file->file.name,
                                cf->conf_file->সারি);

        ngx_memset(buf->pos, ' ', buf->end - buf->pos);

        cln = ngx_pool_cleanup_add(cf->pool, 0);
        যদি (cln == ফাঁকা) {
            ফেরত NGX_CONF_ERROR;
        }

        cln->তথ্য = new_log;
        cln->handler = ngx_log_memory_cleanup;

        new_log->writer = ngx_log_memory_writer;
        new_log->wdata = buf;

#অন্যথা
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        ফেরত NGX_CONF_ERROR;
#endif

    } অন্যথা যদি (ngx_strncmp(মান[1].তথ্য, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, আকার(ngx_syslog_peer_t));
        যদি (peer == ফাঁকা) {
            ফেরত NGX_CONF_ERROR;
        }

        যদি (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            ফেরত NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } অন্যথা {
        new_log->file = ngx_conf_open_file(cf->cycle, &মান[1]);
        যদি (new_log->file == ফাঁকা) {
            ফেরত NGX_CONF_ERROR;
        }
    }

    যদি (ngx_log_set_levels(cf, new_log) != NGX_CONF_OK) {
        ফেরত NGX_CONF_ERROR;
    }

    যদি (*শীর্ষ != new_log) {
        ngx_log_ঢোকাও(*শীর্ষ, new_log);
    }

    ফেরত NGX_CONF_OK;
}


স্থির শূন্য
ngx_log_ঢোকাও(ngx_log_t *log, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    যদি (new_log->log_level > log->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *log;
        *log = *new_log;
        *new_log = tmp;

        log->পরবর্তী = new_log;
        ফেরত;
    }

    যখন (log->পরবর্তী) {
        যদি (new_log->log_level > log->পরবর্তী->log_level) {
            new_log->পরবর্তী = log->পরবর্তী;
            log->পরবর্তী = new_log;
            ফেরত;
        }

        log = log->পরবর্তী;
    }

    log->পরবর্তী = new_log;
}


#যদি (NGX_DEBUG)

স্থির শূন্য
ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    আকার_t len)
{
    u_char                *p;
    আকার_t                 avail, written;
    ngx_log_memory_buf_t  *mem;

    mem = log->wdata;

    যদি (mem == ফাঁকা) {
        ফেরত;
    }

    written = ngx_atomic_fetch_add(&mem->written, len);

    p = mem->pos + written % (mem->end - mem->pos);

    avail = mem->end - p;

    যদি (avail >= len) {
        ngx_memcpy(p, buf, len);

    } অন্যথা {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


স্থির শূন্য
ngx_log_memory_cleanup(শূন্য *তথ্য)
{
    ngx_log_t *log = তথ্য;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, log, 0, "destroy memory log buffer");

    log->wdata = ফাঁকা;
}

#endif
