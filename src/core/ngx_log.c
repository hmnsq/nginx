
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن حرف *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_log_سيٽ_سطحون(ngx_conf_t *cf, ngx_log_t *لاگرٿم);
ساكن فراغ ngx_log_وجهو(ngx_log_t *لاگرٿم, ngx_log_t *new_log);


#إذا (NGX_DEBUG)

ساكن فراغ ngx_log_يادگيري_writer(ngx_log_t *لاگرٿم, ngx_uint_t level,
    u_char *buf, سائيز_t len);
ساكن فراغ ngx_log_يادگيري_cleanup(فراغ *معلومات);


تعريفنوع بنية {
    u_char        *شروعات;
    u_char        *پڇاڙي;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_يادگيري_buf_t;

#جيڪڏهن_پورو


ساكن ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1_وڌيڪ,
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


ساكن ngx_str_t err_سطحون[] = {
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

ساكن ثابت حرف *debug_سطحون[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#إذا (NGX_HAVE_VARIADIC_MACROS)

فراغ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *لاگرٿم, ngx_err_t err,
    ثابت حرف *fmt, ...)

#وإلا

فراغ
ngx_log_error_core(ngx_uint_t level, ngx_log_t *لاگرٿم, ngx_err_t err,
    ثابت حرف *fmt, va_list دليلون)

#جيڪڏهن_پورو
{
#إذا (NGX_HAVE_VARIADIC_MACROS)
    va_list      دليلون;
#جيڪڏهن_پورو
    u_char      *p, *آخري, *نياپو;
    ssize_t      n;
    ngx_uint_t   wrote_stderr, debug_connection;
    u_char       errstr[NGX_MAX_ERROR_STR];

    آخري = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, ngx_cached_err_log_وقت.معلومات,
                   ngx_cached_err_log_وقت.len);

    p = ngx_slprintf(p, آخري, " [%V] ", &err_سطحون[level]);

    /* pid#tid */
    p = ngx_slprintf(p, آخري, "%P#" NGX_TID_T_FMT ": ",
                    ngx_log_pid, ngx_log_tid);

    إذا (لاگرٿم->connection) {
        p = ngx_slprintf(p, آخري, "*%uA ", لاگرٿم->connection);
    }

    نياپو = p;

#إذا (NGX_HAVE_VARIADIC_MACROS)

    va_شروعات(دليلون, fmt);
    p = ngx_vslprintf(p, آخري, fmt, دليلون);
    va_پڇاڙي(دليلون);

#وإلا

    p = ngx_vslprintf(p, آخري, fmt, دليلون);

#جيڪڏهن_پورو

    إذا (err) {
        p = ngx_log_errno(p, آخري, err);
    }

    إذا (level != NGX_LOG_DEBUG && لاگرٿم->handler) {
        p = لاگرٿم->handler(لاگرٿم, p, آخري - p);
    }

    إذا (p > آخري - NGX_LINEFEED_سائيز) {
        p = آخري - NGX_LINEFEED_سائيز;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (لاگرٿم->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    طالما (لاگرٿم) {

        إذا (لاگرٿم->log_level < level && !debug_connection) {
            اكسر;
        }

        إذا (لاگرٿم->writer) {
            لاگرٿم->writer(لاگرٿم, level, errstr, p - errstr);
            اذهب ايندڙ;
        }

        إذا (ngx_وقت() == لاگرٿم->disk_ڀريل_وقت) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            اذهب ايندڙ;
        }

        n = ngx_write_fd(لاگرٿم->file->fd, errstr, p - errstr);

        إذا (n == -1 && ngx_errno == NGX_ENOSPC) {
            لاگرٿم->disk_ڀريل_وقت = ngx_وقت();
        }

        إذا (لاگرٿم->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    ايندڙ:

        لاگرٿم = لاگرٿم->ايندڙ;
    }

    إذا (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        أرجع;
    }

    نياپو -= (7 + err_سطحون[level].len + 3);

    (فراغ) ngx_sprintf(نياپو, "nginx: [%V] ", &err_سطحون[level]);

    (فراغ) ngx_write_console(ngx_stderr, نياپو, p - نياپو);
}


#إذا !(NGX_HAVE_VARIADIC_MACROS)

فراغ ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *لاگرٿم, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    va_list  دليلون;

    إذا (لاگرٿم->log_level >= level) {
        va_شروعات(دليلون, fmt);
        ngx_log_error_core(level, لاگرٿم, err, fmt, دليلون);
        va_پڇاڙي(دليلون);
    }
}


فراغ ngx_cdecl
ngx_log_debug_core(ngx_log_t *لاگرٿم, ngx_err_t err, ثابت حرف *fmt, ...)
{
    va_list  دليلون;

    va_شروعات(دليلون, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, لاگرٿم, err, fmt, دليلون);
    va_پڇاڙي(دليلون);
}

#جيڪڏهن_پورو


فراغ ngx_cdecl
ngx_log_abort(ngx_err_t err, ثابت حرف *fmt, ...)
{
    u_char   *p;
    va_list   دليلون;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_شروعات(دليلون, fmt);
    p = ngx_vsnprintf(errstr, حجم(errstr) - 1, fmt, دليلون);
    va_پڇاڙي(دليلون);

    ngx_log_error(NGX_LOG_ALERT, ngx_چڪر->لاگرٿم, err,
                  "%*s", p - errstr, errstr);
}


فراغ ngx_cdecl
ngx_log_stderr(ngx_err_t err, ثابت حرف *fmt, ...)
{
    u_char   *p, *آخري;
    va_list   دليلون;
    u_char    errstr[NGX_MAX_ERROR_STR];

    آخري = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_شروعات(دليلون, fmt);
    p = ngx_vslprintf(p, آخري, fmt, دليلون);
    va_پڇاڙي(دليلون);

    إذا (err) {
        p = ngx_log_errno(p, آخري, err);
    }

    إذا (p > آخري - NGX_LINEFEED_سائيز) {
        p = آخري - NGX_LINEFEED_سائيز;
    }

    ngx_linefeed(p);

    (فراغ) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *آخري, ngx_err_t err)
{
    إذا (buf > آخري - 50) {

        /* leave a space for an error code */

        buf = آخري - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#إذا (NGX_WIN32)
    buf = ngx_slprintf(buf, آخري, ((بنانشان) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#وإلا
    buf = ngx_slprintf(buf, آخري, " (%d: ", err);
#جيڪڏهن_پورو

    buf = ngx_strerror(err, buf, آخري - buf);

    إذا (buf < آخري) {
        *buf++ = ')';
    }

    أرجع buf;
}


ngx_log_t *
ngx_log_init(u_char *اڳياڙي, u_char *error_log)
{
    u_char  *p, *name;
    سائيز_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    إذا (error_log == عدم) {
        error_log = (u_char *) NGX_ERROR_LOG_رستو;
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
#جيڪڏهن_پورو

        إذا (اڳياڙي) {
            plen = ngx_strlen(اڳياڙي);

        } وإلا {
#جيڪڏهن_وصف_ٿيل NGX_اڳياڙي
            اڳياڙي = (u_char *) NGX_اڳياڙي;
            plen = ngx_strlen(اڳياڙي);
#وإلا
            plen = 0;
#جيڪڏهن_پورو
        }

        إذا (plen) {
            name = يادگيري_ڏيو(plen + nlen + 2);
            إذا (name == عدم) {
                أرجع عدم;
            }

            p = ngx_cpymem(name, اڳياڙي, plen);

            إذا (!ngx_رستو_separator(*(p - 1))) {
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
#جيڪڏهن_پورو

        ngx_log_file.fd = ngx_stderr;
    }

    إذا (p) {
        ngx_free(p);
    }

    أرجع &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_چڪر_t *چڪر)
{
    ngx_log_t  *لاگرٿم;

    إذا (ngx_log_get_file_log(&چڪر->new_log) != عدم) {
        أرجع NGX_OK;
    }

    إذا (چڪر->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        لاگرٿم = ngx_pcalloc(چڪر->pool, حجم(ngx_log_t));
        إذا (لاگرٿم == عدم) {
            أرجع NGX_ERROR;
        }

    } وإلا {
        /* no error logs at all */
        لاگرٿم = &چڪر->new_log;
    }

    لاگرٿم->log_level = NGX_LOG_ERR;

    لاگرٿم->file = ngx_conf_open_file(چڪر, &چڪر->error_log);
    إذا (لاگرٿم->file == عدم) {
        أرجع NGX_ERROR;
    }

    إذا (لاگرٿم != &چڪر->new_log) {
        ngx_log_وجهو(&چڪر->new_log, لاگرٿم);
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_چڪر_t *چڪر)
{
    ngx_fd_t  fd;

    إذا (چڪر->log_use_stderr) {
        أرجع NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(چڪر->لاگرٿم)->file->fd;

    إذا (fd != ngx_stderr) {
        إذا (ngx_سيٽ_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, چڪر->لاگرٿم, ngx_errno,
                          ngx_سيٽ_stderr_n " failed");

            أرجع NGX_ERROR;
        }
    }

    أرجع NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *سر)
{
    ngx_log_t  *لاگرٿم;

    لكل (لاگرٿم = سر; لاگرٿم; لاگرٿم = لاگرٿم->ايندڙ) {
        إذا (لاگرٿم->file != عدم) {
            أرجع لاگرٿم;
        }
    }

    أرجع عدم;
}


ساكن حرف *
ngx_log_سيٽ_سطحون(ngx_conf_t *cf, ngx_log_t *لاگرٿم)
{
    ngx_uint_t   i, n, d, مليو;
    ngx_str_t   *قدر;

    إذا (cf->دليلون->nelts == 2) {
        لاگرٿم->log_level = NGX_LOG_ERR;
        أرجع NGX_CONF_OK;
    }

    قدر = cf->دليلون->elts;

    لكل (i = 2; i < cf->دليلون->nelts; i++) {
        مليو = 0;

        لكل (n = 1; n <= NGX_LOG_DEBUG; n++) {
            إذا (ngx_strcmp(قدر[i].معلومات, err_سطحون[n].معلومات) == 0) {

                إذا (لاگرٿم->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &قدر[i]);
                    أرجع NGX_CONF_ERROR;
                }

                لاگرٿم->log_level = n;
                مليو = 1;
                اكسر;
            }
        }

        لكل (n = 0, d = NGX_LOG_DEBUG_پهريون; d <= NGX_LOG_DEBUG_آخري; d <<= 1) {
            إذا (ngx_strcmp(قدر[i].معلومات, debug_سطحون[n++]) == 0) {
                إذا (لاگرٿم->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &قدر[i]);
                    أرجع NGX_CONF_ERROR;
                }

                لاگرٿم->log_level |= d;
                مليو = 1;
                اكسر;
            }
        }


        إذا (!مليو) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &قدر[i]);
            أرجع NGX_CONF_ERROR;
        }
    }

    إذا (لاگرٿم->log_level == NGX_LOG_DEBUG) {
        لاگرٿم->log_level = NGX_LOG_DEBUG_ALL;
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->چڪر->new_log;

    أرجع ngx_log_سيٽ_log(cf, &dummy);
}


حرف *
ngx_log_سيٽ_log(ngx_conf_t *cf, ngx_log_t **سر)
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

    قدر = cf->دليلون->elts;

    إذا (ngx_strcmp(قدر[1].معلومات, "stderr") == 0) {
        ngx_str_null(&name);
        cf->چڪر->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->چڪر, &name);
        إذا (new_log->file == عدم) {
            أرجع NGX_CONF_ERROR;
        }

    } وإلا إذا (ngx_strncmp(قدر[1].معلومات, "memory:", 7) == 0) {

#إذا (NGX_DEBUG)
        سائيز_t                 سائيز, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_يادگيري_buf_t  *buf;

        قدر[1].len -= 7;
        قدر[1].معلومات += 7;

        needed = حجم("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_سائيز_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        سائيز = ngx_parse_سائيز(&قدر[1]);

        إذا (سائيز == (سائيز_t) NGX_ERROR || سائيز < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &قدر[1]);
            أرجع NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, حجم(ngx_log_يادگيري_buf_t));
        إذا (buf == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        buf->شروعات = ngx_pnalloc(cf->pool, سائيز);
        إذا (buf->شروعات == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        buf->پڇاڙي = buf->شروعات + سائيز;

        buf->pos = ngx_slprintf(buf->شروعات, buf->پڇاڙي, "MEMLOG %uz %V:%ui%N",
                                سائيز, &cf->conf_file->file.name,
                                cf->conf_file->سٽ);

        ngx_memset(buf->pos, ' ', buf->پڇاڙي - buf->pos);

        cln = ngx_pool_cleanup_جوڙيو(cf->pool, 0);
        إذا (cln == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        cln->معلومات = new_log;
        cln->handler = ngx_log_يادگيري_cleanup;

        new_log->writer = ngx_log_يادگيري_writer;
        new_log->wdata = buf;

#وإلا
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        أرجع NGX_CONF_ERROR;
#جيڪڏهن_پورو

    } وإلا إذا (ngx_strncmp(قدر[1].معلومات, "syslog:", 7) == 0) {
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
        new_log->file = ngx_conf_open_file(cf->چڪر, &قدر[1]);
        إذا (new_log->file == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    إذا (ngx_log_سيٽ_سطحون(cf, new_log) != NGX_CONF_OK) {
        أرجع NGX_CONF_ERROR;
    }

    إذا (*سر != new_log) {
        ngx_log_وجهو(*سر, new_log);
    }

    أرجع NGX_CONF_OK;
}


ساكن فراغ
ngx_log_وجهو(ngx_log_t *لاگرٿم, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    إذا (new_log->log_level > لاگرٿم->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *لاگرٿم;
        *لاگرٿم = *new_log;
        *new_log = tmp;

        لاگرٿم->ايندڙ = new_log;
        أرجع;
    }

    طالما (لاگرٿم->ايندڙ) {
        إذا (new_log->log_level > لاگرٿم->ايندڙ->log_level) {
            new_log->ايندڙ = لاگرٿم->ايندڙ;
            لاگرٿم->ايندڙ = new_log;
            أرجع;
        }

        لاگرٿم = لاگرٿم->ايندڙ;
    }

    لاگرٿم->ايندڙ = new_log;
}


#إذا (NGX_DEBUG)

ساكن فراغ
ngx_log_يادگيري_writer(ngx_log_t *لاگرٿم, ngx_uint_t level, u_char *buf,
    سائيز_t len)
{
    u_char                *p;
    سائيز_t                 avail, written;
    ngx_log_يادگيري_buf_t  *mem;

    mem = لاگرٿم->wdata;

    إذا (mem == عدم) {
        أرجع;
    }

    written = ngx_atomic_fetch_جوڙيو(&mem->written, len);

    p = mem->pos + written % (mem->پڇاڙي - mem->pos);

    avail = mem->پڇاڙي - p;

    إذا (avail >= len) {
        ngx_memcpy(p, buf, len);

    } وإلا {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


ساكن فراغ
ngx_log_يادگيري_cleanup(فراغ *معلومات)
{
    ngx_log_t *لاگرٿم = معلومات;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, لاگرٿم, 0, "destroy memory log buffer");

    لاگرٿم->wdata = عدم;
}

#جيڪڏهن_پورو
