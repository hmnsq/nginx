
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن حرف *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_log_سیٹ_پدھراں(ngx_conf_t *cf, ngx_log_t *لوگارتھم);
ساكن فراغ ngx_log_داخل_کرو(ngx_log_t *لوگارتھم, ngx_log_t *new_log);


#إذا (NGX_DEBUG)

ساكن فراغ ngx_log_حافظہ_writer(ngx_log_t *لوگارتھم, ngx_uint_t level,
    u_char *buf, ناپ_t len);
ساكن فراغ ngx_log_حافظہ_cleanup(فراغ *انکڑے);


تعريفنوع بنية {
    u_char        *شروع;
    u_char        *سرا;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_حافظہ_buf_t;

#جے_انت


ساكن ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1_ہور,
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


ساكن ngx_str_t err_پدھراں[] = {
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

ساكن ثابت حرف *debug_پدھراں[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#إذا (NGX_HAVE_VARIADIC_MACROS)

فراغ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, ...)

#وإلا

فراغ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, va_list دلیلاں)

#جے_انت
{
#إذا (NGX_HAVE_VARIADIC_MACROS)
    va_list      دلیلاں;
#جے_انت
    u_char      *p, *آخری, *پیغام;
    ssize_t      n;
    ngx_uint_t   wrote_stderr, debug_connection;
    u_char       errstr[NGX_MAX_ERROR_STR];

    آخری = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, ngx_cached_err_log_ویلا.انکڑے,
                   ngx_cached_err_log_ویلا.len);

    p = ngx_slprintf(p, آخری, " [%V] ", &err_پدھراں[level]);

    /* pid#tid */
    p = ngx_slprintf(p, آخری, "%P#" NGX_TID_T_FMT ": ",
                    ngx_log_pid, ngx_log_tid);

    إذا (لوگارتھم->connection) {
        p = ngx_slprintf(p, آخری, "*%uA ", لوگارتھم->connection);
    }

    پیغام = p;

#إذا (NGX_HAVE_VARIADIC_MACROS)

    va_شروع(دلیلاں, fmt);
    p = ngx_vslprintf(p, آخری, fmt, دلیلاں);
    va_سرا(دلیلاں);

#وإلا

    p = ngx_vslprintf(p, آخری, fmt, دلیلاں);

#جے_انت

    إذا (err) {
        p = ngx_log_errno(p, آخری, err);
    }

    إذا (level != NGX_LOG_DEBUG && لوگارتھم->handler) {
        p = لوگارتھم->handler(لوگارتھم, p, آخری - p);
    }

    إذا (p > آخری - NGX_LINEFEED_ناپ) {
        p = آخری - NGX_LINEFEED_ناپ;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (لوگارتھم->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    طالما (لوگارتھم) {

        إذا (لوگارتھم->log_level < level && !debug_connection) {
            اكسر;
        }

        إذا (لوگارتھم->writer) {
            لوگارتھم->writer(لوگارتھم, level, errstr, p - errstr);
            اذهب اگلا;
        }

        إذا (ngx_ویلا() == لوگارتھم->disk_بھریا_ویلا) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            اذهب اگلا;
        }

        n = ngx_write_fd(لوگارتھم->file->fd, errstr, p - errstr);

        إذا (n == -1 && ngx_errno == NGX_ENOSPC) {
            لوگارتھم->disk_بھریا_ویلا = ngx_ویلا();
        }

        إذا (لوگارتھم->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    اگلا:

        لوگارتھم = لوگارتھم->اگلا;
    }

    إذا (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        أرجع;
    }

    پیغام -= (7 + err_پدھراں[level].len + 3);

    (فراغ) ngx_sprintf(پیغام, "nginx: [%V] ", &err_پدھراں[level]);

    (فراغ) ngx_write_console(ngx_stderr, پیغام, p - پیغام);
}


#إذا !(NGX_HAVE_VARIADIC_MACROS)

فراغ ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    va_list  دلیلاں;

    إذا (لوگارتھم->log_level >= level) {
        va_شروع(دلیلاں, fmt);
        ngx_log_error_core(level, لوگارتھم, err, fmt, دلیلاں);
        va_سرا(دلیلاں);
    }
}


فراغ ngx_cdecl
ngx_log_debug_core(ngx_log_t *لوگارتھم, ngx_err_t err, ثابت حرف *fmt, ...)
{
    va_list  دلیلاں;

    va_شروع(دلیلاں, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, لوگارتھم, err, fmt, دلیلاں);
    va_سرا(دلیلاں);
}

#جے_انت


فراغ ngx_cdecl
ngx_log_abort(ngx_err_t err, ثابت حرف *fmt, ...)
{
    u_char   *p;
    va_list   دلیلاں;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_شروع(دلیلاں, fmt);
    p = ngx_vsnprintf(errstr, حجم(errstr) - 1, fmt, دلیلاں);
    va_سرا(دلیلاں);

    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, err,
                  "%*s", p - errstr, errstr);
}


فراغ ngx_cdecl
ngx_log_stderr(ngx_err_t err, ثابت حرف *fmt, ...)
{
    u_char   *p, *آخری;
    va_list   دلیلاں;
    u_char    errstr[NGX_MAX_ERROR_STR];

    آخری = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_شروع(دلیلاں, fmt);
    p = ngx_vslprintf(p, آخری, fmt, دلیلاں);
    va_سرا(دلیلاں);

    إذا (err) {
        p = ngx_log_errno(p, آخری, err);
    }

    إذا (p > آخری - NGX_LINEFEED_ناپ) {
        p = آخری - NGX_LINEFEED_ناپ;
    }

    ngx_linefeed(p);

    (فراغ) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *آخری, ngx_err_t err)
{
    إذا (buf > آخری - 50) {

        /* leave a space for an error code */

        buf = آخری - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#إذا (NGX_WIN32)
    buf = ngx_slprintf(buf, آخری, ((بنانشانی) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#وإلا
    buf = ngx_slprintf(buf, آخری, " (%d: ", err);
#جے_انت

    buf = ngx_strerror(err, buf, آخری - buf);

    إذا (buf < آخری) {
        *buf++ = ')';
    }

    أرجع buf;
}


ngx_log_t *
ngx_log_init(u_char *اگیتر, u_char *error_log)
{
    u_char  *p, *name;
    ناپ_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    إذا (error_log == عدم) {
        error_log = (u_char *) NGX_ERROR_LOG_رستہ;
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
#جے_انت

        إذا (اگیتر) {
            plen = ngx_strlen(اگیتر);

        } وإلا {
#جے_تعریف_شدہ NGX_اگیتر
            اگیتر = (u_char *) NGX_اگیتر;
            plen = ngx_strlen(اگیتر);
#وإلا
            plen = 0;
#جے_انت
        }

        إذا (plen) {
            name = حافظہ_دیو(plen + nlen + 2);
            إذا (name == عدم) {
                أرجع عدم;
            }

            p = ngx_cpymem(name, اگیتر, plen);

            إذا (!ngx_رستہ_separator(*(p - 1))) {
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
#جے_انت

        ngx_log_file.fd = ngx_stderr;
    }

    إذا (p) {
        ngx_free(p);
    }

    أرجع &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_چکر_t *چکر)
{
    ngx_log_t  *لوگارتھم;

    إذا (ngx_log_get_file_log(&چکر->new_log) != عدم) {
        أرجع NGX_OK;
    }

    إذا (چکر->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        لوگارتھم = ngx_pcalloc(چکر->pool, حجم(ngx_log_t));
        إذا (لوگارتھم == عدم) {
            أرجع NGX_ERROR;
        }

    } وإلا {
        /* no error logs at all */
        لوگارتھم = &چکر->new_log;
    }

    لوگارتھم->log_level = NGX_LOG_ERR;

    لوگارتھم->file = ngx_conf_open_file(چکر, &چکر->error_log);
    إذا (لوگارتھم->file == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (لوگارتھم != &چکر->new_log) {
        ngx_log_داخل_کرو(&چکر->new_log, لوگارتھم);
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_چکر_t *چکر)
{
    ngx_fd_t  fd;

    إذا (چکر->log_use_stderr) {
        أرجع NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(چکر->لوگارتھم)->file->fd;

    إذا (fd != ngx_stderr) {
        إذا (ngx_سیٹ_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_errno,
                          ngx_سیٹ_stderr_n " failed");

            أرجع NGX_ERROR;
        }
    }

    أرجع NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *سر)
{
    ngx_log_t  *لوگارتھم;

    لكل (لوگارتھم = سر; لوگارتھم; لوگارتھم = لوگارتھم->اگلا) {
        إذا (لوگارتھم->file != عدم) {
            أرجع لوگارتھم;
        }
    }

    أرجع عدم;
}


ساكن حرف *
ngx_log_سیٹ_پدھراں(ngx_conf_t *cf, ngx_log_t *لوگارتھم)
{
    ngx_uint_t   i, n, d, لبھیا;
    ngx_str_t   *قدر;

    إذا (cf->دلیلاں->nelts == 2) {
        لوگارتھم->log_level = NGX_LOG_ERR;
        أرجع NGX_CONF_OK;
    }

    قدر = cf->دلیلاں->elts;

    لكل (i = 2; i < cf->دلیلاں->nelts; i++) {
        لبھیا = 0;

        لكل (n = 1; n <= NGX_LOG_DEBUG; n++) {
            إذا (ngx_strcmp(قدر[i].انکڑے, err_پدھراں[n].انکڑے) == 0) {

                إذا (لوگارتھم->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &قدر[i]);
                    أرجع NGX_CONF_ERROR;
                }

                لوگارتھم->log_level = n;
                لبھیا = 1;
                اكسر;
            }
        }

        لكل (n = 0, d = NGX_LOG_DEBUG_پہلا; d <= NGX_LOG_DEBUG_آخری; d <<= 1) {
            إذا (ngx_strcmp(قدر[i].انکڑے, debug_پدھراں[n++]) == 0) {
                إذا (لوگارتھم->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &قدر[i]);
                    أرجع NGX_CONF_ERROR;
                }

                لوگارتھم->log_level |= d;
                لبھیا = 1;
                اكسر;
            }
        }


        إذا (!لبھیا) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &قدر[i]);
            أرجع NGX_CONF_ERROR;
        }
    }

    إذا (لوگارتھم->log_level == NGX_LOG_DEBUG) {
        لوگارتھم->log_level = NGX_LOG_DEBUG_ALL;
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->چکر->new_log;

    أرجع ngx_log_سیٹ_log(cf, &dummy);
}


حرف *
ngx_log_سیٹ_log(ngx_conf_t *cf, ngx_log_t **سر)
{
    ngx_log_t          *new_log;
    ngx_str_t          *قدر, name;
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

    قدر = cf->دلیلاں->elts;

    إذا (ngx_strcmp(قدر[1].انکڑے, "stderr") == 0) {
        ngx_str_null(&name);
        cf->چکر->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->چکر, &name);
        إذا (new_log->file == عدم) {
            أرجع NGX_CONF_ERROR;
        }

    } وإلا إذا (ngx_strncmp(قدر[1].انکڑے, "memory:", 7) == 0) {

#إذا (NGX_DEBUG)
        ناپ_t                 ناپ, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_حافظہ_buf_t  *buf;

        قدر[1].len -= 7;
        قدر[1].انکڑے += 7;

        needed = حجم("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_ناپ_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        ناپ = ngx_parse_ناپ(&قدر[1]);

        إذا (ناپ == (ناپ_t) NGX_ERROR || ناپ < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &قدر[1]);
            أرجع NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, حجم(ngx_log_حافظہ_buf_t));
        إذا (buf == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        buf->شروع = ngx_pnalloc(cf->pool, ناپ);
        إذا (buf->شروع == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        buf->سرا = buf->شروع + ناپ;

        buf->pos = ngx_slprintf(buf->شروع, buf->سرا, "MEMLOG %uz %V:%ui%N",
                                ناپ, &cf->conf_file->file.name,
                                cf->conf_file->سطر);

        ngx_memset(buf->pos, ' ', buf->سرا - buf->pos);

        cln = ngx_pool_cleanup_جوڑو(cf->pool, 0);
        إذا (cln == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        cln->انکڑے = new_log;
        cln->handler = ngx_log_حافظہ_cleanup;

        new_log->writer = ngx_log_حافظہ_writer;
        new_log->wdata = buf;

#وإلا
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        أرجع NGX_CONF_ERROR;
#جے_انت

    } وإلا إذا (ngx_strncmp(قدر[1].انکڑے, "syslog:", 7) == 0) {
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
        new_log->file = ngx_conf_open_file(cf->چکر, &قدر[1]);
        إذا (new_log->file == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    إذا (ngx_log_سیٹ_پدھراں(cf, new_log) != NGX_CONF_OK) {
        أرجع NGX_CONF_ERROR;
    }

    إذا (*سر != new_log) {
        ngx_log_داخل_کرو(*سر, new_log);
    }

    أرجع NGX_CONF_OK;
}


ساكن فراغ
ngx_log_داخل_کرو(ngx_log_t *لوگارتھم, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    إذا (new_log->log_level > لوگارتھم->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *لوگارتھم;
        *لوگارتھم = *new_log;
        *new_log = tmp;

        لوگارتھم->اگلا = new_log;
        أرجع;
    }

    طالما (لوگارتھم->اگلا) {
        إذا (new_log->log_level > لوگارتھم->اگلا->log_level) {
            new_log->اگلا = لوگارتھم->اگلا;
            لوگارتھم->اگلا = new_log;
            أرجع;
        }

        لوگارتھم = لوگارتھم->اگلا;
    }

    لوگارتھم->اگلا = new_log;
}


#إذا (NGX_DEBUG)

ساكن فراغ
ngx_log_حافظہ_writer(ngx_log_t *لوگارتھم, ngx_uint_t level, u_char *buf,
    ناپ_t len)
{
    u_char                *p;
    ناپ_t                 avail, written;
    ngx_log_حافظہ_buf_t  *mem;

    mem = لوگارتھم->wdata;

    إذا (mem == عدم) {
        أرجع;
    }

    written = ngx_atomic_fetch_جوڑو(&mem->written, len);

    p = mem->pos + written % (mem->سرا - mem->pos);

    avail = mem->سرا - p;

    إذا (avail >= len) {
        ngx_memcpy(p, buf, len);

    } وإلا {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


ساكن فراغ
ngx_log_حافظہ_cleanup(فراغ *انکڑے)
{
    ngx_log_t *لوگارتھم = انکڑے;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, لوگارتھم, 0, "destroy memory log buffer");

    لوگارتھم->wdata = عدم;
}

#جے_انت
