
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن حرف *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_log_مجموعه_سطوح(ngx_conf_t *cf, ngx_log_t *لگاریتم);
ساكن فراغ ngx_log_درج(ngx_log_t *لگاریتم, ngx_log_t *new_log);


#إذا (NGX_DEBUG)

ساكن فراغ ngx_log_حافظه_writer(ngx_log_t *لگاریتم, ngx_uint_t level,
    u_char *buf, اندازه_t len);
ساكن فراغ ngx_log_حافظه_cleanup(فراغ *داده);


تعريفنوع بنية {
    u_char        *شروع;
    u_char        *انتها;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_حافظه_buf_t;

#پایان_اگر


ساكن ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1_بیشتر,
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


ساكن ngx_str_t err_سطوح[] = {
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

ساكن ثابت حرف *debug_سطوح[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#إذا (NGX_HAVE_VARIADIC_MACROS)

فراغ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *لگاریتم, ngx_err_t err,
    ثابت حرف *fmt, ...)

#وإلا

فراغ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *لگاریتم, ngx_err_t err,
    ثابت حرف *fmt, va_list آرگومانها)

#پایان_اگر
{
#إذا (NGX_HAVE_VARIADIC_MACROS)
    va_list      آرگومانها;
#پایان_اگر
    u_char      *p, *آخرین, *پیام;
    ssize_t      n;
    ngx_uint_t   wrote_stderr, debug_connection;
    u_char       errstr[NGX_MAX_ERROR_STR];

    آخرین = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, ngx_cached_err_log_زمان.داده,
                   ngx_cached_err_log_زمان.len);

    p = ngx_slprintf(p, آخرین, " [%V] ", &err_سطوح[level]);

    /* pid#tid */
    p = ngx_slprintf(p, آخرین, "%P#" NGX_TID_T_FMT ": ",
                    ngx_log_pid, ngx_log_tid);

    إذا (لگاریتم->connection) {
        p = ngx_slprintf(p, آخرین, "*%uA ", لگاریتم->connection);
    }

    پیام = p;

#إذا (NGX_HAVE_VARIADIC_MACROS)

    va_شروع(آرگومانها, fmt);
    p = ngx_vslprintf(p, آخرین, fmt, آرگومانها);
    va_انتها(آرگومانها);

#وإلا

    p = ngx_vslprintf(p, آخرین, fmt, آرگومانها);

#پایان_اگر

    إذا (err) {
        p = ngx_log_errno(p, آخرین, err);
    }

    إذا (level != NGX_LOG_DEBUG && لگاریتم->handler) {
        p = لگاریتم->handler(لگاریتم, p, آخرین - p);
    }

    إذا (p > آخرین - NGX_LINEFEED_اندازه) {
        p = آخرین - NGX_LINEFEED_اندازه;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (لگاریتم->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    طالما (لگاریتم) {

        إذا (لگاریتم->log_level < level && !debug_connection) {
            اكسر;
        }

        إذا (لگاریتم->writer) {
            لگاریتم->writer(لگاریتم, level, errstr, p - errstr);
            اذهب بعدی;
        }

        إذا (ngx_زمان() == لگاریتم->disk_پر_زمان) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            اذهب بعدی;
        }

        n = ngx_write_fd(لگاریتم->file->fd, errstr, p - errstr);

        إذا (n == -1 && ngx_errno == NGX_ENOSPC) {
            لگاریتم->disk_پر_زمان = ngx_زمان();
        }

        إذا (لگاریتم->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    بعدی:

        لگاریتم = لگاریتم->بعدی;
    }

    إذا (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        أرجع;
    }

    پیام -= (7 + err_سطوح[level].len + 3);

    (فراغ) ngx_sprintf(پیام, "nginx: [%V] ", &err_سطوح[level]);

    (فراغ) ngx_write_console(ngx_stderr, پیام, p - پیام);
}


#إذا !(NGX_HAVE_VARIADIC_MACROS)

فراغ ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *لگاریتم, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    va_list  آرگومانها;

    إذا (لگاریتم->log_level >= level) {
        va_شروع(آرگومانها, fmt);
        ngx_log_error_core(level, لگاریتم, err, fmt, آرگومانها);
        va_انتها(آرگومانها);
    }
}


فراغ ngx_cdecl
ngx_log_debug_core(ngx_log_t *لگاریتم, ngx_err_t err, ثابت حرف *fmt, ...)
{
    va_list  آرگومانها;

    va_شروع(آرگومانها, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, لگاریتم, err, fmt, آرگومانها);
    va_انتها(آرگومانها);
}

#پایان_اگر


فراغ ngx_cdecl
ngx_log_abort(ngx_err_t err, ثابت حرف *fmt, ...)
{
    u_char   *p;
    va_list   آرگومانها;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_شروع(آرگومانها, fmt);
    p = ngx_vsnprintf(errstr, حجم(errstr) - 1, fmt, آرگومانها);
    va_انتها(آرگومانها);

    ngx_log_error(NGX_LOG_ALERT, ngx_چرخه->لگاریتم, err,
                  "%*s", p - errstr, errstr);
}


فراغ ngx_cdecl
ngx_log_stderr(ngx_err_t err, ثابت حرف *fmt, ...)
{
    u_char   *p, *آخرین;
    va_list   آرگومانها;
    u_char    errstr[NGX_MAX_ERROR_STR];

    آخرین = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_شروع(آرگومانها, fmt);
    p = ngx_vslprintf(p, آخرین, fmt, آرگومانها);
    va_انتها(آرگومانها);

    إذا (err) {
        p = ngx_log_errno(p, آخرین, err);
    }

    إذا (p > آخرین - NGX_LINEFEED_اندازه) {
        p = آخرین - NGX_LINEFEED_اندازه;
    }

    ngx_linefeed(p);

    (فراغ) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *آخرین, ngx_err_t err)
{
    إذا (buf > آخرین - 50) {

        /* leave a space for an error code */

        buf = آخرین - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#إذا (NGX_WIN32)
    buf = ngx_slprintf(buf, آخرین, ((بیعلامت) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#وإلا
    buf = ngx_slprintf(buf, آخرین, " (%d: ", err);
#پایان_اگر

    buf = ngx_strerror(err, buf, آخرین - buf);

    إذا (buf < آخرین) {
        *buf++ = ')';
    }

    أرجع buf;
}


ngx_log_t *
ngx_log_init(u_char *پیشوند, u_char *error_log)
{
    u_char  *p, *name;
    اندازه_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    إذا (error_log == عدم) {
        error_log = (u_char *) NGX_ERROR_LOG_مسیر;
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
#پایان_اگر

        إذا (پیشوند) {
            plen = ngx_strlen(پیشوند);

        } وإلا {
#اگر_تعریف_شده NGX_پیشوند
            پیشوند = (u_char *) NGX_پیشوند;
            plen = ngx_strlen(پیشوند);
#وإلا
            plen = 0;
#پایان_اگر
        }

        إذا (plen) {
            name = حافظه_بده(plen + nlen + 2);
            إذا (name == عدم) {
                أرجع عدم;
            }

            p = ngx_cpymem(name, پیشوند, plen);

            إذا (!ngx_مسیر_separator(*(p - 1))) {
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
#پایان_اگر

        ngx_log_file.fd = ngx_stderr;
    }

    إذا (p) {
        ngx_free(p);
    }

    أرجع &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_چرخه_t *چرخه)
{
    ngx_log_t  *لگاریتم;

    إذا (ngx_log_get_file_log(&چرخه->new_log) != عدم) {
        أرجع NGX_OK;
    }

    إذا (چرخه->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        لگاریتم = ngx_pcalloc(چرخه->pool, حجم(ngx_log_t));
        إذا (لگاریتم == عدم) {
            أرجع NGX_ERROR;
        }

    } وإلا {
        /* no error logs at all */
        لگاریتم = &چرخه->new_log;
    }

    لگاریتم->log_level = NGX_LOG_ERR;

    لگاریتم->file = ngx_conf_open_file(چرخه, &چرخه->error_log);
    إذا (لگاریتم->file == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (لگاریتم != &چرخه->new_log) {
        ngx_log_درج(&چرخه->new_log, لگاریتم);
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_چرخه_t *چرخه)
{
    ngx_fd_t  fd;

    إذا (چرخه->log_use_stderr) {
        أرجع NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(چرخه->لگاریتم)->file->fd;

    إذا (fd != ngx_stderr) {
        إذا (ngx_مجموعه_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, چرخه->لگاریتم, ngx_errno,
                          ngx_مجموعه_stderr_n " failed");

            أرجع NGX_ERROR;
        }
    }

    أرجع NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *سر)
{
    ngx_log_t  *لگاریتم;

    لكل (لگاریتم = سر; لگاریتم; لگاریتم = لگاریتم->بعدی) {
        إذا (لگاریتم->file != عدم) {
            أرجع لگاریتم;
        }
    }

    أرجع عدم;
}


ساكن حرف *
ngx_log_مجموعه_سطوح(ngx_conf_t *cf, ngx_log_t *لگاریتم)
{
    ngx_uint_t   i, n, d, یافت;
    ngx_str_t   *مقدار;

    إذا (cf->آرگومانها->nelts == 2) {
        لگاریتم->log_level = NGX_LOG_ERR;
        أرجع NGX_CONF_OK;
    }

    مقدار = cf->آرگومانها->elts;

    لكل (i = 2; i < cf->آرگومانها->nelts; i++) {
        یافت = 0;

        لكل (n = 1; n <= NGX_LOG_DEBUG; n++) {
            إذا (ngx_strcmp(مقدار[i].داده, err_سطوح[n].داده) == 0) {

                إذا (لگاریتم->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &مقدار[i]);
                    أرجع NGX_CONF_ERROR;
                }

                لگاریتم->log_level = n;
                یافت = 1;
                اكسر;
            }
        }

        لكل (n = 0, d = NGX_LOG_DEBUG_اولی; d <= NGX_LOG_DEBUG_آخرین; d <<= 1) {
            إذا (ngx_strcmp(مقدار[i].داده, debug_سطوح[n++]) == 0) {
                إذا (لگاریتم->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &مقدار[i]);
                    أرجع NGX_CONF_ERROR;
                }

                لگاریتم->log_level |= d;
                یافت = 1;
                اكسر;
            }
        }


        إذا (!یافت) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &مقدار[i]);
            أرجع NGX_CONF_ERROR;
        }
    }

    إذا (لگاریتم->log_level == NGX_LOG_DEBUG) {
        لگاریتم->log_level = NGX_LOG_DEBUG_ALL;
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->چرخه->new_log;

    أرجع ngx_log_مجموعه_log(cf, &dummy);
}


حرف *
ngx_log_مجموعه_log(ngx_conf_t *cf, ngx_log_t **سر)
{
    ngx_log_t          *new_log;
    ngx_str_t          *مقدار, name;
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

    مقدار = cf->آرگومانها->elts;

    إذا (ngx_strcmp(مقدار[1].داده, "stderr") == 0) {
        ngx_str_null(&name);
        cf->چرخه->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->چرخه, &name);
        إذا (new_log->file == عدم) {
            أرجع NGX_CONF_ERROR;
        }

    } وإلا إذا (ngx_strncmp(مقدار[1].داده, "memory:", 7) == 0) {

#إذا (NGX_DEBUG)
        اندازه_t                 اندازه, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_حافظه_buf_t  *buf;

        مقدار[1].len -= 7;
        مقدار[1].داده += 7;

        needed = حجم("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_اندازه_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        اندازه = ngx_parse_اندازه(&مقدار[1]);

        إذا (اندازه == (اندازه_t) NGX_ERROR || اندازه < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &مقدار[1]);
            أرجع NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, حجم(ngx_log_حافظه_buf_t));
        إذا (buf == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        buf->شروع = ngx_pnalloc(cf->pool, اندازه);
        إذا (buf->شروع == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        buf->انتها = buf->شروع + اندازه;

        buf->pos = ngx_slprintf(buf->شروع, buf->انتها, "MEMLOG %uz %V:%ui%N",
                                اندازه, &cf->conf_file->file.name,
                                cf->conf_file->سطر);

        ngx_memset(buf->pos, ' ', buf->انتها - buf->pos);

        cln = ngx_pool_cleanup_جمع_کن(cf->pool, 0);
        إذا (cln == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        cln->داده = new_log;
        cln->handler = ngx_log_حافظه_cleanup;

        new_log->writer = ngx_log_حافظه_writer;
        new_log->wdata = buf;

#وإلا
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        أرجع NGX_CONF_ERROR;
#پایان_اگر

    } وإلا إذا (ngx_strncmp(مقدار[1].داده, "syslog:", 7) == 0) {
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
        new_log->file = ngx_conf_open_file(cf->چرخه, &مقدار[1]);
        إذا (new_log->file == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    إذا (ngx_log_مجموعه_سطوح(cf, new_log) != NGX_CONF_OK) {
        أرجع NGX_CONF_ERROR;
    }

    إذا (*سر != new_log) {
        ngx_log_درج(*سر, new_log);
    }

    أرجع NGX_CONF_OK;
}


ساكن فراغ
ngx_log_درج(ngx_log_t *لگاریتم, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    إذا (new_log->log_level > لگاریتم->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *لگاریتم;
        *لگاریتم = *new_log;
        *new_log = tmp;

        لگاریتم->بعدی = new_log;
        أرجع;
    }

    طالما (لگاریتم->بعدی) {
        إذا (new_log->log_level > لگاریتم->بعدی->log_level) {
            new_log->بعدی = لگاریتم->بعدی;
            لگاریتم->بعدی = new_log;
            أرجع;
        }

        لگاریتم = لگاریتم->بعدی;
    }

    لگاریتم->بعدی = new_log;
}


#إذا (NGX_DEBUG)

ساكن فراغ
ngx_log_حافظه_writer(ngx_log_t *لگاریتم, ngx_uint_t level, u_char *buf,
    اندازه_t len)
{
    u_char                *p;
    اندازه_t                 avail, written;
    ngx_log_حافظه_buf_t  *mem;

    mem = لگاریتم->wdata;

    إذا (mem == عدم) {
        أرجع;
    }

    written = ngx_atomic_fetch_جمع_کن(&mem->written, len);

    p = mem->pos + written % (mem->انتها - mem->pos);

    avail = mem->انتها - p;

    إذا (avail >= len) {
        ngx_memcpy(p, buf, len);

    } وإلا {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


ساكن فراغ
ngx_log_حافظه_cleanup(فراغ *داده)
{
    ngx_log_t *لگاریتم = داده;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, لگاریتم, 0, "destroy memory log buffer");

    لگاریتم->wdata = عدم;
}

#پایان_اگر
