
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن حرف *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_log_سېټ_پوړونه(ngx_conf_t *cf, ngx_log_t *لوګاریتم);
ساكن فراغ ngx_log_ورننباسه(ngx_log_t *لوګاریتم, ngx_log_t *new_log);


#إذا (NGX_DEBUG)

ساكن فراغ ngx_log_حافظه_writer(ngx_log_t *لوګاریتم, ngx_uint_t level,
    u_char *buf, اندازه_t len);
ساكن فراغ ngx_log_حافظه_cleanup(فراغ *معلومات);


تعريفنوع بنية {
    u_char        *پیل;
    u_char        *پای_ټکی;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_حافظه_buf_t;

#د_که_پای


ساكن ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1_نور,
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


ساكن ngx_str_t err_پوړونه[] = {
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

ساكن ثابت حرف *debug_پوړونه[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#إذا (NGX_HAVE_VARIADIC_MACROS)

فراغ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *لوګاریتم, ngx_err_t err,
    ثابت حرف *fmt, ...)

#وإلا

فراغ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *لوګاریتم, ngx_err_t err,
    ثابت حرف *fmt, va_list دلیلونه)

#د_که_پای
{
#إذا (NGX_HAVE_VARIADIC_MACROS)
    va_list      دلیلونه;
#د_که_پای
    u_char      *p, *وروستی, *پیغام;
    ssize_t      n;
    ngx_uint_t   wrote_stderr, debug_connection;
    u_char       errstr[NGX_MAX_ERROR_STR];

    وروستی = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, ngx_cached_err_log_وخت.معلومات,
                   ngx_cached_err_log_وخت.len);

    p = ngx_slprintf(p, وروستی, " [%V] ", &err_پوړونه[level]);

    /* pid#tid */
    p = ngx_slprintf(p, وروستی, "%P#" NGX_TID_T_FMT ": ",
                    ngx_log_pid, ngx_log_tid);

    إذا (لوګاریتم->connection) {
        p = ngx_slprintf(p, وروستی, "*%uA ", لوګاریتم->connection);
    }

    پیغام = p;

#إذا (NGX_HAVE_VARIADIC_MACROS)

    va_پیل(دلیلونه, fmt);
    p = ngx_vslprintf(p, وروستی, fmt, دلیلونه);
    va_پای_ټکی(دلیلونه);

#وإلا

    p = ngx_vslprintf(p, وروستی, fmt, دلیلونه);

#د_که_پای

    إذا (err) {
        p = ngx_log_errno(p, وروستی, err);
    }

    إذا (level != NGX_LOG_DEBUG && لوګاریتم->handler) {
        p = لوګاریتم->handler(لوګاریتم, p, وروستی - p);
    }

    إذا (p > وروستی - NGX_LINEFEED_اندازه) {
        p = وروستی - NGX_LINEFEED_اندازه;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (لوګاریتم->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    طالما (لوګاریتم) {

        إذا (لوګاریتم->log_level < level && !debug_connection) {
            اكسر;
        }

        إذا (لوګاریتم->writer) {
            لوګاریتم->writer(لوګاریتم, level, errstr, p - errstr);
            اذهب بل;
        }

        إذا (ngx_وخت() == لوګاریتم->disk_ډک_وخت) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            اذهب بل;
        }

        n = ngx_write_fd(لوګاریتم->file->fd, errstr, p - errstr);

        إذا (n == -1 && ngx_errno == NGX_ENOSPC) {
            لوګاریتم->disk_ډک_وخت = ngx_وخت();
        }

        إذا (لوګاریتم->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    بل:

        لوګاریتم = لوګاریتم->بل;
    }

    إذا (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        أرجع;
    }

    پیغام -= (7 + err_پوړونه[level].len + 3);

    (فراغ) ngx_sprintf(پیغام, "nginx: [%V] ", &err_پوړونه[level]);

    (فراغ) ngx_write_console(ngx_stderr, پیغام, p - پیغام);
}


#إذا !(NGX_HAVE_VARIADIC_MACROS)

فراغ ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *لوګاریتم, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    va_list  دلیلونه;

    إذا (لوګاریتم->log_level >= level) {
        va_پیل(دلیلونه, fmt);
        ngx_log_error_core(level, لوګاریتم, err, fmt, دلیلونه);
        va_پای_ټکی(دلیلونه);
    }
}


فراغ ngx_cdecl
ngx_log_debug_core(ngx_log_t *لوګاریتم, ngx_err_t err, ثابت حرف *fmt, ...)
{
    va_list  دلیلونه;

    va_پیل(دلیلونه, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, لوګاریتم, err, fmt, دلیلونه);
    va_پای_ټکی(دلیلونه);
}

#د_که_پای


فراغ ngx_cdecl
ngx_log_abort(ngx_err_t err, ثابت حرف *fmt, ...)
{
    u_char   *p;
    va_list   دلیلونه;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_پیل(دلیلونه, fmt);
    p = ngx_vsnprintf(errstr, حجم(errstr) - 1, fmt, دلیلونه);
    va_پای_ټکی(دلیلونه);

    ngx_log_error(NGX_LOG_ALERT, ngx_دوره->لوګاریتم, err,
                  "%*s", p - errstr, errstr);
}


فراغ ngx_cdecl
ngx_log_stderr(ngx_err_t err, ثابت حرف *fmt, ...)
{
    u_char   *p, *وروستی;
    va_list   دلیلونه;
    u_char    errstr[NGX_MAX_ERROR_STR];

    وروستی = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_پیل(دلیلونه, fmt);
    p = ngx_vslprintf(p, وروستی, fmt, دلیلونه);
    va_پای_ټکی(دلیلونه);

    إذا (err) {
        p = ngx_log_errno(p, وروستی, err);
    }

    إذا (p > وروستی - NGX_LINEFEED_اندازه) {
        p = وروستی - NGX_LINEFEED_اندازه;
    }

    ngx_linefeed(p);

    (فراغ) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *وروستی, ngx_err_t err)
{
    إذا (buf > وروستی - 50) {

        /* leave a space for an error code */

        buf = وروستی - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#إذا (NGX_WIN32)
    buf = ngx_slprintf(buf, وروستی, ((بینښه) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#وإلا
    buf = ngx_slprintf(buf, وروستی, " (%d: ", err);
#د_که_پای

    buf = ngx_strerror(err, buf, وروستی - buf);

    إذا (buf < وروستی) {
        *buf++ = ')';
    }

    أرجع buf;
}


ngx_log_t *
ngx_log_init(u_char *مختاړی, u_char *error_log)
{
    u_char  *p, *name;
    اندازه_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    إذا (error_log == عدم) {
        error_log = (u_char *) NGX_ERROR_LOG_لار;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    إذا (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        أرجع &ngx_log;
    }

    p = عدم;

#إذا (NGX_WIN32)
    إذا (name[1] != ':') {
#وإلا
    إذا (name[0] != '/') {
#د_که_پای

        إذا (مختاړی) {
            plen = ngx_strlen(مختاړی);

        } وإلا {
#که_تعریف_شوی NGX_مختاړی
            مختاړی = (u_char *) NGX_مختاړی;
            plen = ngx_strlen(مختاړی);
#وإلا
            plen = 0;
#د_که_پای
        }

        إذا (plen) {
            name = حافظه_ورکړه(plen + nlen + 2);
            إذا (name == عدم) {
                أرجع عدم;
            }

            p = ngx_cpymem(name, مختاړی, plen);

            إذا (!ngx_لار_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    إذا (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#إذا (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#د_که_پای

        ngx_log_file.fd = ngx_stderr;
    }

    إذا (p) {
        ngx_free(p);
    }

    أرجع &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_دوره_t *دوره)
{
    ngx_log_t  *لوګاریتم;

    إذا (ngx_log_get_file_log(&دوره->new_log) != عدم) {
        أرجع NGX_OK;
    }

    إذا (دوره->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        لوګاریتم = ngx_pcalloc(دوره->pool, حجم(ngx_log_t));
        إذا (لوګاریتم == عدم) {
            أرجع NGX_ERROR;
        }

    } وإلا {
        /* no error logs at all */
        لوګاریتم = &دوره->new_log;
    }

    لوګاریتم->log_level = NGX_LOG_ERR;

    لوګاریتم->file = ngx_conf_open_file(دوره, &دوره->error_log);
    إذا (لوګاریتم->file == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (لوګاریتم != &دوره->new_log) {
        ngx_log_ورننباسه(&دوره->new_log, لوګاریتم);
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_دوره_t *دوره)
{
    ngx_fd_t  fd;

    إذا (دوره->log_use_stderr) {
        أرجع NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(دوره->لوګاریتم)->file->fd;

    إذا (fd != ngx_stderr) {
        إذا (ngx_سېټ_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, دوره->لوګاریتم, ngx_errno,
                          ngx_سېټ_stderr_n " failed");

            أرجع NGX_ERROR;
        }
    }

    أرجع NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *سر)
{
    ngx_log_t  *لوګاریتم;

    لكل (لوګاریتم = سر; لوګاریتم; لوګاریتم = لوګاریتم->بل) {
        إذا (لوګاریتم->file != عدم) {
            أرجع لوګاریتم;
        }
    }

    أرجع عدم;
}


ساكن حرف *
ngx_log_سېټ_پوړونه(ngx_conf_t *cf, ngx_log_t *لوګاریتم)
{
    ngx_uint_t   i, n, d, وموندل;
    ngx_str_t   *ارزښت;

    إذا (cf->دلیلونه->nelts == 2) {
        لوګاریتم->log_level = NGX_LOG_ERR;
        أرجع NGX_CONF_OK;
    }

    ارزښت = cf->دلیلونه->elts;

    لكل (i = 2; i < cf->دلیلونه->nelts; i++) {
        وموندل = 0;

        لكل (n = 1; n <= NGX_LOG_DEBUG; n++) {
            إذا (ngx_strcmp(ارزښت[i].معلومات, err_پوړونه[n].معلومات) == 0) {

                إذا (لوګاریتم->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &ارزښت[i]);
                    أرجع NGX_CONF_ERROR;
                }

                لوګاریتم->log_level = n;
                وموندل = 1;
                اكسر;
            }
        }

        لكل (n = 0, d = NGX_LOG_DEBUG_لومړی; d <= NGX_LOG_DEBUG_وروستی; d <<= 1) {
            إذا (ngx_strcmp(ارزښت[i].معلومات, debug_پوړونه[n++]) == 0) {
                إذا (لوګاریتم->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &ارزښت[i]);
                    أرجع NGX_CONF_ERROR;
                }

                لوګاریتم->log_level |= d;
                وموندل = 1;
                اكسر;
            }
        }


        إذا (!وموندل) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &ارزښت[i]);
            أرجع NGX_CONF_ERROR;
        }
    }

    إذا (لوګاریتم->log_level == NGX_LOG_DEBUG) {
        لوګاریتم->log_level = NGX_LOG_DEBUG_ALL;
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->دوره->new_log;

    أرجع ngx_log_سېټ_log(cf, &dummy);
}


حرف *
ngx_log_سېټ_log(ngx_conf_t *cf, ngx_log_t **سر)
{
    ngx_log_t          *new_log;
    ngx_str_t          *ارزښت, name;
    ngx_syslog_peer_t  *peer;

    إذا (*سر != عدم && (*سر)->log_level == 0) {
        new_log = *سر;

    } وإلا {

        new_log = ngx_pcalloc(cf->pool, حجم(ngx_log_t));
        إذا (new_log == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        إذا (*سر == عدم) {
            *سر = new_log;
        }
    }

    ارزښت = cf->دلیلونه->elts;

    إذا (ngx_strcmp(ارزښت[1].معلومات, "stderr") == 0) {
        ngx_str_null(&name);
        cf->دوره->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->دوره, &name);
        إذا (new_log->file == عدم) {
            أرجع NGX_CONF_ERROR;
        }

    } وإلا إذا (ngx_strncmp(ارزښت[1].معلومات, "memory:", 7) == 0) {

#إذا (NGX_DEBUG)
        اندازه_t                 اندازه, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_حافظه_buf_t  *buf;

        ارزښت[1].len -= 7;
        ارزښت[1].معلومات += 7;

        needed = حجم("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_اندازه_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        اندازه = ngx_parse_اندازه(&ارزښت[1]);

        إذا (اندازه == (اندازه_t) NGX_ERROR || اندازه < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &ارزښت[1]);
            أرجع NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, حجم(ngx_log_حافظه_buf_t));
        إذا (buf == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        buf->پیل = ngx_pnalloc(cf->pool, اندازه);
        إذا (buf->پیل == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        buf->پای_ټکی = buf->پیل + اندازه;

        buf->pos = ngx_slprintf(buf->پیل, buf->پای_ټکی, "MEMLOG %uz %V:%ui%N",
                                اندازه, &cf->conf_file->file.name,
                                cf->conf_file->کرښه);

        ngx_memset(buf->pos, ' ', buf->پای_ټکی - buf->pos);

        cln = ngx_pool_cleanup_جمع_کړه(cf->pool, 0);
        إذا (cln == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        cln->معلومات = new_log;
        cln->handler = ngx_log_حافظه_cleanup;

        new_log->writer = ngx_log_حافظه_writer;
        new_log->wdata = buf;

#وإلا
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        أرجع NGX_CONF_ERROR;
#د_که_پای

    } وإلا إذا (ngx_strncmp(ارزښت[1].معلومات, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, حجم(ngx_syslog_peer_t));
        إذا (peer == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        إذا (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            أرجع NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } وإلا {
        new_log->file = ngx_conf_open_file(cf->دوره, &ارزښت[1]);
        إذا (new_log->file == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    إذا (ngx_log_سېټ_پوړونه(cf, new_log) != NGX_CONF_OK) {
        أرجع NGX_CONF_ERROR;
    }

    إذا (*سر != new_log) {
        ngx_log_ورننباسه(*سر, new_log);
    }

    أرجع NGX_CONF_OK;
}


ساكن فراغ
ngx_log_ورننباسه(ngx_log_t *لوګاریتم, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    إذا (new_log->log_level > لوګاریتم->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *لوګاریتم;
        *لوګاریتم = *new_log;
        *new_log = tmp;

        لوګاریتم->بل = new_log;
        أرجع;
    }

    طالما (لوګاریتم->بل) {
        إذا (new_log->log_level > لوګاریتم->بل->log_level) {
            new_log->بل = لوګاریتم->بل;
            لوګاریتم->بل = new_log;
            أرجع;
        }

        لوګاریتم = لوګاریتم->بل;
    }

    لوګاریتم->بل = new_log;
}


#إذا (NGX_DEBUG)

ساكن فراغ
ngx_log_حافظه_writer(ngx_log_t *لوګاریتم, ngx_uint_t level, u_char *buf,
    اندازه_t len)
{
    u_char                *p;
    اندازه_t                 avail, written;
    ngx_log_حافظه_buf_t  *mem;

    mem = لوګاریتم->wdata;

    إذا (mem == عدم) {
        أرجع;
    }

    written = ngx_atomic_fetch_جمع_کړه(&mem->written, len);

    p = mem->pos + written % (mem->پای_ټکی - mem->pos);

    avail = mem->پای_ټکی - p;

    إذا (avail >= len) {
        ngx_memcpy(p, buf, len);

    } وإلا {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


ساكن فراغ
ngx_log_حافظه_cleanup(فراغ *معلومات)
{
    ngx_log_t *لوګاریتم = معلومات;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, لوګاریتم, 0, "destroy memory log buffer");

    لوګاریتم->wdata = عدم;
}

#د_که_پای
