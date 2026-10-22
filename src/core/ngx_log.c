
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ساکن حرف *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf);
ساکن حرف *ngx_log_سیٹ_سطحیں(ngx_conf_t *cf, ngx_log_t *لوگارتھم);
ساکن خالی ngx_log_داخل_کرو(ngx_log_t *لوگارتھم, ngx_log_t *new_log);


#اگر (NGX_DEBUG)

ساکن خالی ngx_log_حافظہ_writer(ngx_log_t *لوگارتھم, ngx_uint_t level,
    u_char *buf, سائز_t len);
ساکن خالی ngx_log_حافظہ_cleanup(خالی *ڈیٹا);


قسمتعریف ساخت {
    u_char        *آغاز;
    u_char        *انتہا;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_حافظہ_buf_t;

#اگر_ختم


ساکن ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1_مزید,
      ngx_error_log,
      0,
      0,
      تہی },

      ngx_null_command
};


ساکن ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    تہی,
    تہی
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    تہی,                                  /* init master */
    تہی,                                  /* init module */
    تہی,                                  /* init process */
    تہی,                                  /* init thread */
    تہی,                                  /* exit thread */
    تہی,                                  /* exit process */
    تہی,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ساکن ngx_log_t        ngx_log;
ساکن ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


ساکن ngx_str_t err_سطحیں[] = {
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

ساکن ثابت حرف *debug_سطحیں[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#اگر (NGX_HAVE_VARIADIC_MACROS)

خالی
ngx_log_error_core(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, ...)

#ورنہ

خالی
ngx_log_error_core(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, va_list دلائل)

#اگر_ختم
{
#اگر (NGX_HAVE_VARIADIC_MACROS)
    va_list      دلائل;
#اگر_ختم
    u_char      *p, *آخری, *پیغام;
    ssize_t      n;
    ngx_uint_t   wrote_stderr, debug_connection;
    u_char       errstr[NGX_MAX_ERROR_STR];

    آخری = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, ngx_cached_err_log_وقت.ڈیٹا,
                   ngx_cached_err_log_وقت.len);

    p = ngx_slprintf(p, آخری, " [%V] ", &err_سطحیں[level]);

    /* pid#tid */
    p = ngx_slprintf(p, آخری, "%P#" NGX_TID_T_FMT ": ",
                    ngx_log_pid, ngx_log_tid);

    اگر (لوگارتھم->connection) {
        p = ngx_slprintf(p, آخری, "*%uA ", لوگارتھم->connection);
    }

    پیغام = p;

#اگر (NGX_HAVE_VARIADIC_MACROS)

    va_آغاز(دلائل, fmt);
    p = ngx_vslprintf(p, آخری, fmt, دلائل);
    va_انتہا(دلائل);

#ورنہ

    p = ngx_vslprintf(p, آخری, fmt, دلائل);

#اگر_ختم

    اگر (err) {
        p = ngx_log_errno(p, آخری, err);
    }

    اگر (level != NGX_LOG_DEBUG && لوگارتھم->handler) {
        p = لوگارتھم->handler(لوگارتھم, p, آخری - p);
    }

    اگر (p > آخری - NGX_LINEFEED_سائز) {
        p = آخری - NGX_LINEFEED_سائز;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (لوگارتھم->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    جب (لوگارتھم) {

        اگر (لوگارتھم->log_level < level && !debug_connection) {
            توڑو;
        }

        اگر (لوگارتھم->writer) {
            لوگارتھم->writer(لوگارتھم, level, errstr, p - errstr);
            جاؤ اگلا;
        }

        اگر (ngx_وقت() == لوگارتھم->disk_بھرا_وقت) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            جاؤ اگلا;
        }

        n = ngx_write_fd(لوگارتھم->file->fd, errstr, p - errstr);

        اگر (n == -1 && ngx_errno == NGX_ENOSPC) {
            لوگارتھم->disk_بھرا_وقت = ngx_وقت();
        }

        اگر (لوگارتھم->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    اگلا:

        لوگارتھم = لوگارتھم->اگلا;
    }

    اگر (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        واپس;
    }

    پیغام -= (7 + err_سطحیں[level].len + 3);

    (خالی) ngx_sprintf(پیغام, "nginx: [%V] ", &err_سطحیں[level]);

    (خالی) ngx_write_console(ngx_stderr, پیغام, p - پیغام);
}


#اگر !(NGX_HAVE_VARIADIC_MACROS)

خالی ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *لوگارتھم, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    va_list  دلائل;

    اگر (لوگارتھم->log_level >= level) {
        va_آغاز(دلائل, fmt);
        ngx_log_error_core(level, لوگارتھم, err, fmt, دلائل);
        va_انتہا(دلائل);
    }
}


خالی ngx_cdecl
ngx_log_debug_core(ngx_log_t *لوگارتھم, ngx_err_t err, ثابت حرف *fmt, ...)
{
    va_list  دلائل;

    va_آغاز(دلائل, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, لوگارتھم, err, fmt, دلائل);
    va_انتہا(دلائل);
}

#اگر_ختم


خالی ngx_cdecl
ngx_log_abort(ngx_err_t err, ثابت حرف *fmt, ...)
{
    u_char   *p;
    va_list   دلائل;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_آغاز(دلائل, fmt);
    p = ngx_vsnprintf(errstr, حجم(errstr) - 1, fmt, دلائل);
    va_انتہا(دلائل);

    ngx_log_error(NGX_LOG_ALERT, ngx_چکر->لوگارتھم, err,
                  "%*s", p - errstr, errstr);
}


خالی ngx_cdecl
ngx_log_stderr(ngx_err_t err, ثابت حرف *fmt, ...)
{
    u_char   *p, *آخری;
    va_list   دلائل;
    u_char    errstr[NGX_MAX_ERROR_STR];

    آخری = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_آغاز(دلائل, fmt);
    p = ngx_vslprintf(p, آخری, fmt, دلائل);
    va_انتہا(دلائل);

    اگر (err) {
        p = ngx_log_errno(p, آخری, err);
    }

    اگر (p > آخری - NGX_LINEFEED_سائز) {
        p = آخری - NGX_LINEFEED_سائز;
    }

    ngx_linefeed(p);

    (خالی) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *آخری, ngx_err_t err)
{
    اگر (buf > آخری - 50) {

        /* leave a space for an error code */

        buf = آخری - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#اگر (NGX_WIN32)
    buf = ngx_slprintf(buf, آخری, ((غیرعلامتی) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#ورنہ
    buf = ngx_slprintf(buf, آخری, " (%d: ", err);
#اگر_ختم

    buf = ngx_strerror(err, buf, آخری - buf);

    اگر (buf < آخری) {
        *buf++ = ')';
    }

    واپس buf;
}


ngx_log_t *
ngx_log_init(u_char *سابقہ, u_char *error_log)
{
    u_char  *p, *name;
    سائز_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    اگر (error_log == تہی) {
        error_log = (u_char *) NGX_ERROR_LOG_راستہ;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    اگر (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        واپس &ngx_log;
    }

    p = تہی;

#اگر (NGX_WIN32)
    اگر (name[1] != ':') {
#ورنہ
    اگر (name[0] != '/') {
#اگر_ختم

        اگر (سابقہ) {
            plen = ngx_strlen(سابقہ);

        } ورنہ {
#اگر_تعریف_شدہ NGX_سابقہ
            سابقہ = (u_char *) NGX_سابقہ;
            plen = ngx_strlen(سابقہ);
#ورنہ
            plen = 0;
#اگر_ختم
        }

        اگر (plen) {
            name = حافظہ_دو(plen + nlen + 2);
            اگر (name == تہی) {
                واپس تہی;
            }

            p = ngx_cpymem(name, سابقہ, plen);

            اگر (!ngx_راستہ_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    اگر (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#اگر (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#اگر_ختم

        ngx_log_file.fd = ngx_stderr;
    }

    اگر (p) {
        ngx_free(p);
    }

    واپس &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_چکر_t *چکر)
{
    ngx_log_t  *لوگارتھم;

    اگر (ngx_log_get_file_log(&چکر->new_log) != تہی) {
        واپس NGX_OK;
    }

    اگر (چکر->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        لوگارتھم = ngx_pcalloc(چکر->pool, حجم(ngx_log_t));
        اگر (لوگارتھم == تہی) {
            واپس NGX_ERROR;
        }

    } ورنہ {
        /* no error logs at all */
        لوگارتھم = &چکر->new_log;
    }

    لوگارتھم->log_level = NGX_LOG_ERR;

    لوگارتھم->file = ngx_conf_open_file(چکر, &چکر->error_log);
    اگر (لوگارتھم->file == تہی) {
        واپس NGX_ERROR;
    }

    اگر (لوگارتھم != &چکر->new_log) {
        ngx_log_داخل_کرو(&چکر->new_log, لوگارتھم);
    }

    واپس NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_چکر_t *چکر)
{
    ngx_fd_t  fd;

    اگر (چکر->log_use_stderr) {
        واپس NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(چکر->لوگارتھم)->file->fd;

    اگر (fd != ngx_stderr) {
        اگر (ngx_سیٹ_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_errno,
                          ngx_سیٹ_stderr_n " failed");

            واپس NGX_ERROR;
        }
    }

    واپس NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *سر)
{
    ngx_log_t  *لوگارتھم;

    برائے (لوگارتھم = سر; لوگارتھم; لوگارتھم = لوگارتھم->اگلا) {
        اگر (لوگارتھم->file != تہی) {
            واپس لوگارتھم;
        }
    }

    واپس تہی;
}


ساکن حرف *
ngx_log_سیٹ_سطحیں(ngx_conf_t *cf, ngx_log_t *لوگارتھم)
{
    ngx_uint_t   i, n, d, ملا;
    ngx_str_t   *قدر;

    اگر (cf->دلائل->nelts == 2) {
        لوگارتھم->log_level = NGX_LOG_ERR;
        واپس NGX_CONF_OK;
    }

    قدر = cf->دلائل->elts;

    برائے (i = 2; i < cf->دلائل->nelts; i++) {
        ملا = 0;

        برائے (n = 1; n <= NGX_LOG_DEBUG; n++) {
            اگر (ngx_strcmp(قدر[i].ڈیٹا, err_سطحیں[n].ڈیٹا) == 0) {

                اگر (لوگارتھم->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &قدر[i]);
                    واپس NGX_CONF_ERROR;
                }

                لوگارتھم->log_level = n;
                ملا = 1;
                توڑو;
            }
        }

        برائے (n = 0, d = NGX_LOG_DEBUG_پہلا; d <= NGX_LOG_DEBUG_آخری; d <<= 1) {
            اگر (ngx_strcmp(قدر[i].ڈیٹا, debug_سطحیں[n++]) == 0) {
                اگر (لوگارتھم->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &قدر[i]);
                    واپس NGX_CONF_ERROR;
                }

                لوگارتھم->log_level |= d;
                ملا = 1;
                توڑو;
            }
        }


        اگر (!ملا) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &قدر[i]);
            واپس NGX_CONF_ERROR;
        }
    }

    اگر (لوگارتھم->log_level == NGX_LOG_DEBUG) {
        لوگارتھم->log_level = NGX_LOG_DEBUG_ALL;
    }

    واپس NGX_CONF_OK;
}


ساکن حرف *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->چکر->new_log;

    واپس ngx_log_سیٹ_log(cf, &dummy);
}


حرف *
ngx_log_سیٹ_log(ngx_conf_t *cf, ngx_log_t **سر)
{
    ngx_log_t          *new_log;
    ngx_str_t          *قدر, name;
    ngx_syslog_peer_t  *peer;

    اگر (*سر != تہی && (*سر)->log_level == 0) {
        new_log = *سر;

    } ورنہ {

        new_log = ngx_pcalloc(cf->pool, حجم(ngx_log_t));
        اگر (new_log == تہی) {
            واپس NGX_CONF_ERROR;
        }

        اگر (*سر == تہی) {
            *سر = new_log;
        }
    }

    قدر = cf->دلائل->elts;

    اگر (ngx_strcmp(قدر[1].ڈیٹا, "stderr") == 0) {
        ngx_str_null(&name);
        cf->چکر->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->چکر, &name);
        اگر (new_log->file == تہی) {
            واپس NGX_CONF_ERROR;
        }

    } ورنہ اگر (ngx_strncmp(قدر[1].ڈیٹا, "memory:", 7) == 0) {

#اگر (NGX_DEBUG)
        سائز_t                 سائز, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_حافظہ_buf_t  *buf;

        قدر[1].len -= 7;
        قدر[1].ڈیٹا += 7;

        needed = حجم("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_سائز_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        سائز = ngx_parse_سائز(&قدر[1]);

        اگر (سائز == (سائز_t) NGX_ERROR || سائز < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &قدر[1]);
            واپس NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, حجم(ngx_log_حافظہ_buf_t));
        اگر (buf == تہی) {
            واپس NGX_CONF_ERROR;
        }

        buf->آغاز = ngx_pnalloc(cf->pool, سائز);
        اگر (buf->آغاز == تہی) {
            واپس NGX_CONF_ERROR;
        }

        buf->انتہا = buf->آغاز + سائز;

        buf->pos = ngx_slprintf(buf->آغاز, buf->انتہا, "MEMLOG %uz %V:%ui%N",
                                سائز, &cf->conf_file->file.name,
                                cf->conf_file->سطر);

        ngx_memset(buf->pos, ' ', buf->انتہا - buf->pos);

        cln = ngx_pool_cleanup_جوڑو(cf->pool, 0);
        اگر (cln == تہی) {
            واپس NGX_CONF_ERROR;
        }

        cln->ڈیٹا = new_log;
        cln->handler = ngx_log_حافظہ_cleanup;

        new_log->writer = ngx_log_حافظہ_writer;
        new_log->wdata = buf;

#ورنہ
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        واپس NGX_CONF_ERROR;
#اگر_ختم

    } ورنہ اگر (ngx_strncmp(قدر[1].ڈیٹا, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, حجم(ngx_syslog_peer_t));
        اگر (peer == تہی) {
            واپس NGX_CONF_ERROR;
        }

        اگر (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            واپس NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } ورنہ {
        new_log->file = ngx_conf_open_file(cf->چکر, &قدر[1]);
        اگر (new_log->file == تہی) {
            واپس NGX_CONF_ERROR;
        }
    }

    اگر (ngx_log_سیٹ_سطحیں(cf, new_log) != NGX_CONF_OK) {
        واپس NGX_CONF_ERROR;
    }

    اگر (*سر != new_log) {
        ngx_log_داخل_کرو(*سر, new_log);
    }

    واپس NGX_CONF_OK;
}


ساکن خالی
ngx_log_داخل_کرو(ngx_log_t *لوگارتھم, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    اگر (new_log->log_level > لوگارتھم->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *لوگارتھم;
        *لوگارتھم = *new_log;
        *new_log = tmp;

        لوگارتھم->اگلا = new_log;
        واپس;
    }

    جب (لوگارتھم->اگلا) {
        اگر (new_log->log_level > لوگارتھم->اگلا->log_level) {
            new_log->اگلا = لوگارتھم->اگلا;
            لوگارتھم->اگلا = new_log;
            واپس;
        }

        لوگارتھم = لوگارتھم->اگلا;
    }

    لوگارتھم->اگلا = new_log;
}


#اگر (NGX_DEBUG)

ساکن خالی
ngx_log_حافظہ_writer(ngx_log_t *لوگارتھم, ngx_uint_t level, u_char *buf,
    سائز_t len)
{
    u_char                *p;
    سائز_t                 avail, written;
    ngx_log_حافظہ_buf_t  *mem;

    mem = لوگارتھم->wdata;

    اگر (mem == تہی) {
        واپس;
    }

    written = ngx_atomic_fetch_جوڑو(&mem->written, len);

    p = mem->pos + written % (mem->انتہا - mem->pos);

    avail = mem->انتہا - p;

    اگر (avail >= len) {
        ngx_memcpy(p, buf, len);

    } ورنہ {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


ساکن خالی
ngx_log_حافظہ_cleanup(خالی *ڈیٹا)
{
    ngx_log_t *لوگارتھم = ڈیٹا;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, لوگارتھم, 0, "destroy memory log buffer");

    لوگارتھم->wdata = تہی;
}

#اگر_ختم
