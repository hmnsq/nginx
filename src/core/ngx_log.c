
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


స్థిర అక్షరం *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf);
స్థిర అక్షరం *ngx_log_సమితి_స్థాయిలు(ngx_conf_t *cf, ngx_log_t *సంవర్గమానం);
స్థిర శూన్య ngx_log_చొప్పించు(ngx_log_t *సంవర్గమానం, ngx_log_t *new_log);


#ఒకవేళ (NGX_DEBUG)

స్థిర శూన్య ngx_log_జ్ఞాపకం_writer(ngx_log_t *సంవర్గమానం, ngx_uint_t level,
    u_char *buf, కొలత_t len);
స్థిర శూన్య ngx_log_జ్ఞాపకం_cleanup(శూన్య *దత్తాంశం);


రకనిర్వచనం నిర్మాణం {
    u_char        *ప్రారంభం;
    u_char        *చివర;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_జ్ఞాపకం_buf_t;

#అయితే_ముగింపు


స్థిర ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1_మరిన్ని,
      ngx_error_log,
      0,
      0,
      ఖాళీ },

      ngx_null_command
};


స్థిర ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    ఖాళీ,
    ఖాళీ
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ఖాళీ,                                  /* init master */
    ఖాళీ,                                  /* init module */
    ఖాళీ,                                  /* init process */
    ఖాళీ,                                  /* init thread */
    ఖాళీ,                                  /* exit thread */
    ఖాళీ,                                  /* exit process */
    ఖాళీ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


స్థిర ngx_log_t        ngx_log;
స్థిర ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


స్థిర ngx_str_t err_స్థాయిలు[] = {
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

స్థిర స్థిరరాశి అక్షరం *debug_స్థాయిలు[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#ఒకవేళ (NGX_HAVE_VARIADIC_MACROS)

శూన్య
ngx_log_error_core(ngx_uint_t level, ngx_log_t *సంవర్గమానం, ngx_err_t err,
    స్థిరరాశి అక్షరం *fmt, ...)

#లేకపోతే

శూన్య
ngx_log_error_core(ngx_uint_t level, ngx_log_t *సంవర్గమానం, ngx_err_t err,
    స్థిరరాశి అక్షరం *fmt, va_list వాదనలు)

#అయితే_ముగింపు
{
#ఒకవేళ (NGX_HAVE_VARIADIC_MACROS)
    va_list      వాదనలు;
#అయితే_ముగింపు
    u_char      *p, *చివరి, *సందేశం;
    ssize_t      n;
    ngx_uint_t   wrote_stderr, debug_connection;
    u_char       errstr[NGX_MAX_ERROR_STR];

    చివరి = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, ngx_cached_err_log_సమయం.దత్తాంశం,
                   ngx_cached_err_log_సమయం.len);

    p = ngx_slprintf(p, చివరి, " [%V] ", &err_స్థాయిలు[level]);

    /* pid#tid */
    p = ngx_slprintf(p, చివరి, "%P#" NGX_TID_T_FMT ": ",
                    ngx_log_pid, ngx_log_tid);

    ఒకవేళ (సంవర్గమానం->connection) {
        p = ngx_slprintf(p, చివరి, "*%uA ", సంవర్గమానం->connection);
    }

    సందేశం = p;

#ఒకవేళ (NGX_HAVE_VARIADIC_MACROS)

    va_ప్రారంభం(వాదనలు, fmt);
    p = ngx_vslprintf(p, చివరి, fmt, వాదనలు);
    va_చివర(వాదనలు);

#లేకపోతే

    p = ngx_vslprintf(p, చివరి, fmt, వాదనలు);

#అయితే_ముగింపు

    ఒకవేళ (err) {
        p = ngx_log_errno(p, చివరి, err);
    }

    ఒకవేళ (level != NGX_LOG_DEBUG && సంవర్గమానం->handler) {
        p = సంవర్గమానం->handler(సంవర్గమానం, p, చివరి - p);
    }

    ఒకవేళ (p > చివరి - NGX_LINEFEED_కొలత) {
        p = చివరి - NGX_LINEFEED_కొలత;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (సంవర్గమానం->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    అయితే (సంవర్గమానం) {

        ఒకవేళ (సంవర్గమానం->log_level < level && !debug_connection) {
            ఆపు;
        }

        ఒకవేళ (సంవర్గమానం->writer) {
            సంవర్గమానం->writer(సంవర్గమానం, level, errstr, p - errstr);
            వెళ్ళు తదుపరి;
        }

        ఒకవేళ (ngx_సమయం() == సంవర్గమానం->disk_నిండిన_సమయం) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            వెళ్ళు తదుపరి;
        }

        n = ngx_write_fd(సంవర్గమానం->file->fd, errstr, p - errstr);

        ఒకవేళ (n == -1 && ngx_errno == NGX_ENOSPC) {
            సంవర్గమానం->disk_నిండిన_సమయం = ngx_సమయం();
        }

        ఒకవేళ (సంవర్గమానం->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    తదుపరి:

        సంవర్గమానం = సంవర్గమానం->తదుపరి;
    }

    ఒకవేళ (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        తిరిగి;
    }

    సందేశం -= (7 + err_స్థాయిలు[level].len + 3);

    (శూన్య) ngx_sprintf(సందేశం, "nginx: [%V] ", &err_స్థాయిలు[level]);

    (శూన్య) ngx_write_console(ngx_stderr, సందేశం, p - సందేశం);
}


#ఒకవేళ !(NGX_HAVE_VARIADIC_MACROS)

శూన్య ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *సంవర్గమానం, ngx_err_t err,
    స్థిరరాశి అక్షరం *fmt, ...)
{
    va_list  వాదనలు;

    ఒకవేళ (సంవర్గమానం->log_level >= level) {
        va_ప్రారంభం(వాదనలు, fmt);
        ngx_log_error_core(level, సంవర్గమానం, err, fmt, వాదనలు);
        va_చివర(వాదనలు);
    }
}


శూన్య ngx_cdecl
ngx_log_debug_core(ngx_log_t *సంవర్గమానం, ngx_err_t err, స్థిరరాశి అక్షరం *fmt, ...)
{
    va_list  వాదనలు;

    va_ప్రారంభం(వాదనలు, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, సంవర్గమానం, err, fmt, వాదనలు);
    va_చివర(వాదనలు);
}

#అయితే_ముగింపు


శూన్య ngx_cdecl
ngx_log_abort(ngx_err_t err, స్థిరరాశి అక్షరం *fmt, ...)
{
    u_char   *p;
    va_list   వాదనలు;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_ప్రారంభం(వాదనలు, fmt);
    p = ngx_vsnprintf(errstr, పరిమాణం(errstr) - 1, fmt, వాదనలు);
    va_చివర(వాదనలు);

    ngx_log_error(NGX_LOG_ALERT, ngx_చక్రం->సంవర్గమానం, err,
                  "%*s", p - errstr, errstr);
}


శూన్య ngx_cdecl
ngx_log_stderr(ngx_err_t err, స్థిరరాశి అక్షరం *fmt, ...)
{
    u_char   *p, *చివరి;
    va_list   వాదనలు;
    u_char    errstr[NGX_MAX_ERROR_STR];

    చివరి = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_ప్రారంభం(వాదనలు, fmt);
    p = ngx_vslprintf(p, చివరి, fmt, వాదనలు);
    va_చివర(వాదనలు);

    ఒకవేళ (err) {
        p = ngx_log_errno(p, చివరి, err);
    }

    ఒకవేళ (p > చివరి - NGX_LINEFEED_కొలత) {
        p = చివరి - NGX_LINEFEED_కొలత;
    }

    ngx_linefeed(p);

    (శూన్య) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *చివరి, ngx_err_t err)
{
    ఒకవేళ (buf > చివరి - 50) {

        /* leave a space for an error code */

        buf = చివరి - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#ఒకవేళ (NGX_WIN32)
    buf = ngx_slprintf(buf, చివరి, ((గుర్తులేని) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#లేకపోతే
    buf = ngx_slprintf(buf, చివరి, " (%d: ", err);
#అయితే_ముగింపు

    buf = ngx_strerror(err, buf, చివరి - buf);

    ఒకవేళ (buf < చివరి) {
        *buf++ = ')';
    }

    తిరిగి buf;
}


ngx_log_t *
ngx_log_init(u_char *ఉపసర్గ, u_char *error_log)
{
    u_char  *p, *name;
    కొలత_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    ఒకవేళ (error_log == ఖాళీ) {
        error_log = (u_char *) NGX_ERROR_LOG_మార్గం;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    ఒకవేళ (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        తిరిగి &ngx_log;
    }

    p = ఖాళీ;

#ఒకవేళ (NGX_WIN32)
    ఒకవేళ (name[1] != ':') {
#లేకపోతే
    ఒకవేళ (name[0] != '/') {
#అయితే_ముగింపు

        ఒకవేళ (ఉపసర్గ) {
            plen = ngx_strlen(ఉపసర్గ);

        } లేకపోతే {
#నిర్వచితమైతే NGX_ఉపసర్గ
            ఉపసర్గ = (u_char *) NGX_ఉపసర్గ;
            plen = ngx_strlen(ఉపసర్గ);
#లేకపోతే
            plen = 0;
#అయితే_ముగింపు
        }

        ఒకవేళ (plen) {
            name = జ్ఞాపకం_ఇవ్వు(plen + nlen + 2);
            ఒకవేళ (name == ఖాళీ) {
                తిరిగి ఖాళీ;
            }

            p = ngx_cpymem(name, ఉపసర్గ, plen);

            ఒకవేళ (!ngx_మార్గం_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    ఒకవేళ (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#ఒకవేళ (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#అయితే_ముగింపు

        ngx_log_file.fd = ngx_stderr;
    }

    ఒకవేళ (p) {
        ngx_free(p);
    }

    తిరిగి &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_చక్రం_t *చక్రం)
{
    ngx_log_t  *సంవర్గమానం;

    ఒకవేళ (ngx_log_get_file_log(&చక్రం->new_log) != ఖాళీ) {
        తిరిగి NGX_OK;
    }

    ఒకవేళ (చక్రం->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        సంవర్గమానం = ngx_pcalloc(చక్రం->pool, పరిమాణం(ngx_log_t));
        ఒకవేళ (సంవర్గమానం == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

    } లేకపోతే {
        /* no error logs at all */
        సంవర్గమానం = &చక్రం->new_log;
    }

    సంవర్గమానం->log_level = NGX_LOG_ERR;

    సంవర్గమానం->file = ngx_conf_open_file(చక్రం, &చక్రం->error_log);
    ఒకవేళ (సంవర్గమానం->file == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (సంవర్గమానం != &చక్రం->new_log) {
        ngx_log_చొప్పించు(&చక్రం->new_log, సంవర్గమానం);
    }

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_చక్రం_t *చక్రం)
{
    ngx_fd_t  fd;

    ఒకవేళ (చక్రం->log_use_stderr) {
        తిరిగి NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(చక్రం->సంవర్గమానం)->file->fd;

    ఒకవేళ (fd != ngx_stderr) {
        ఒకవేళ (ngx_సమితి_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_errno,
                          ngx_సమితి_stderr_n " failed");

            తిరిగి NGX_ERROR;
        }
    }

    తిరిగి NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *తల)
{
    ngx_log_t  *సంవర్గమానం;

    కొరకు (సంవర్గమానం = తల; సంవర్గమానం; సంవర్గమానం = సంవర్గమానం->తదుపరి) {
        ఒకవేళ (సంవర్గమానం->file != ఖాళీ) {
            తిరిగి సంవర్గమానం;
        }
    }

    తిరిగి ఖాళీ;
}


స్థిర అక్షరం *
ngx_log_సమితి_స్థాయిలు(ngx_conf_t *cf, ngx_log_t *సంవర్గమానం)
{
    ngx_uint_t   i, n, d, దొరికింది;
    ngx_str_t   *విలువ;

    ఒకవేళ (cf->వాదనలు->nelts == 2) {
        సంవర్గమానం->log_level = NGX_LOG_ERR;
        తిరిగి NGX_CONF_OK;
    }

    విలువ = cf->వాదనలు->elts;

    కొరకు (i = 2; i < cf->వాదనలు->nelts; i++) {
        దొరికింది = 0;

        కొరకు (n = 1; n <= NGX_LOG_DEBUG; n++) {
            ఒకవేళ (ngx_strcmp(విలువ[i].దత్తాంశం, err_స్థాయిలు[n].దత్తాంశం) == 0) {

                ఒకవేళ (సంవర్గమానం->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &విలువ[i]);
                    తిరిగి NGX_CONF_ERROR;
                }

                సంవర్గమానం->log_level = n;
                దొరికింది = 1;
                ఆపు;
            }
        }

        కొరకు (n = 0, d = NGX_LOG_DEBUG_మొదటి; d <= NGX_LOG_DEBUG_చివరి; d <<= 1) {
            ఒకవేళ (ngx_strcmp(విలువ[i].దత్తాంశం, debug_స్థాయిలు[n++]) == 0) {
                ఒకవేళ (సంవర్గమానం->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &విలువ[i]);
                    తిరిగి NGX_CONF_ERROR;
                }

                సంవర్గమానం->log_level |= d;
                దొరికింది = 1;
                ఆపు;
            }
        }


        ఒకవేళ (!దొరికింది) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &విలువ[i]);
            తిరిగి NGX_CONF_ERROR;
        }
    }

    ఒకవేళ (సంవర్గమానం->log_level == NGX_LOG_DEBUG) {
        సంవర్గమానం->log_level = NGX_LOG_DEBUG_ALL;
    }

    తిరిగి NGX_CONF_OK;
}


స్థిర అక్షరం *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->చక్రం->new_log;

    తిరిగి ngx_log_సమితి_log(cf, &dummy);
}


అక్షరం *
ngx_log_సమితి_log(ngx_conf_t *cf, ngx_log_t **తల)
{
    ngx_log_t          *new_log;
    ngx_str_t          *విలువ, name;
    ngx_syslog_peer_t  *peer;

    ఒకవేళ (*తల != ఖాళీ && (*తల)->log_level == 0) {
        new_log = *తల;

    } లేకపోతే {

        new_log = ngx_pcalloc(cf->pool, పరిమాణం(ngx_log_t));
        ఒకవేళ (new_log == ఖాళీ) {
            తిరిగి NGX_CONF_ERROR;
        }

        ఒకవేళ (*తల == ఖాళీ) {
            *తల = new_log;
        }
    }

    విలువ = cf->వాదనలు->elts;

    ఒకవేళ (ngx_strcmp(విలువ[1].దత్తాంశం, "stderr") == 0) {
        ngx_str_null(&name);
        cf->చక్రం->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->చక్రం, &name);
        ఒకవేళ (new_log->file == ఖాళీ) {
            తిరిగి NGX_CONF_ERROR;
        }

    } లేకపోతే ఒకవేళ (ngx_strncmp(విలువ[1].దత్తాంశం, "memory:", 7) == 0) {

#ఒకవేళ (NGX_DEBUG)
        కొలత_t                 కొలత, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_జ్ఞాపకం_buf_t  *buf;

        విలువ[1].len -= 7;
        విలువ[1].దత్తాంశం += 7;

        needed = పరిమాణం("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_కొలత_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        కొలత = ngx_parse_కొలత(&విలువ[1]);

        ఒకవేళ (కొలత == (కొలత_t) NGX_ERROR || కొలత < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &విలువ[1]);
            తిరిగి NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, పరిమాణం(ngx_log_జ్ఞాపకం_buf_t));
        ఒకవేళ (buf == ఖాళీ) {
            తిరిగి NGX_CONF_ERROR;
        }

        buf->ప్రారంభం = ngx_pnalloc(cf->pool, కొలత);
        ఒకవేళ (buf->ప్రారంభం == ఖాళీ) {
            తిరిగి NGX_CONF_ERROR;
        }

        buf->చివర = buf->ప్రారంభం + కొలత;

        buf->pos = ngx_slprintf(buf->ప్రారంభం, buf->చివర, "MEMLOG %uz %V:%ui%N",
                                కొలత, &cf->conf_file->file.name,
                                cf->conf_file->పంక్తి);

        ngx_memset(buf->pos, ' ', buf->చివర - buf->pos);

        cln = ngx_pool_cleanup_కూడు(cf->pool, 0);
        ఒకవేళ (cln == ఖాళీ) {
            తిరిగి NGX_CONF_ERROR;
        }

        cln->దత్తాంశం = new_log;
        cln->handler = ngx_log_జ్ఞాపకం_cleanup;

        new_log->writer = ngx_log_జ్ఞాపకం_writer;
        new_log->wdata = buf;

#లేకపోతే
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        తిరిగి NGX_CONF_ERROR;
#అయితే_ముగింపు

    } లేకపోతే ఒకవేళ (ngx_strncmp(విలువ[1].దత్తాంశం, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, పరిమాణం(ngx_syslog_peer_t));
        ఒకవేళ (peer == ఖాళీ) {
            తిరిగి NGX_CONF_ERROR;
        }

        ఒకవేళ (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            తిరిగి NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } లేకపోతే {
        new_log->file = ngx_conf_open_file(cf->చక్రం, &విలువ[1]);
        ఒకవేళ (new_log->file == ఖాళీ) {
            తిరిగి NGX_CONF_ERROR;
        }
    }

    ఒకవేళ (ngx_log_సమితి_స్థాయిలు(cf, new_log) != NGX_CONF_OK) {
        తిరిగి NGX_CONF_ERROR;
    }

    ఒకవేళ (*తల != new_log) {
        ngx_log_చొప్పించు(*తల, new_log);
    }

    తిరిగి NGX_CONF_OK;
}


స్థిర శూన్య
ngx_log_చొప్పించు(ngx_log_t *సంవర్గమానం, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    ఒకవేళ (new_log->log_level > సంవర్గమానం->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *సంవర్గమానం;
        *సంవర్గమానం = *new_log;
        *new_log = tmp;

        సంవర్గమానం->తదుపరి = new_log;
        తిరిగి;
    }

    అయితే (సంవర్గమానం->తదుపరి) {
        ఒకవేళ (new_log->log_level > సంవర్గమానం->తదుపరి->log_level) {
            new_log->తదుపరి = సంవర్గమానం->తదుపరి;
            సంవర్గమానం->తదుపరి = new_log;
            తిరిగి;
        }

        సంవర్గమానం = సంవర్గమానం->తదుపరి;
    }

    సంవర్గమానం->తదుపరి = new_log;
}


#ఒకవేళ (NGX_DEBUG)

స్థిర శూన్య
ngx_log_జ్ఞాపకం_writer(ngx_log_t *సంవర్గమానం, ngx_uint_t level, u_char *buf,
    కొలత_t len)
{
    u_char                *p;
    కొలత_t                 avail, written;
    ngx_log_జ్ఞాపకం_buf_t  *mem;

    mem = సంవర్గమానం->wdata;

    ఒకవేళ (mem == ఖాళీ) {
        తిరిగి;
    }

    written = ngx_atomic_fetch_కూడు(&mem->written, len);

    p = mem->pos + written % (mem->చివర - mem->pos);

    avail = mem->చివర - p;

    ఒకవేళ (avail >= len) {
        ngx_memcpy(p, buf, len);

    } లేకపోతే {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


స్థిర శూన్య
ngx_log_జ్ఞాపకం_cleanup(శూన్య *దత్తాంశం)
{
    ngx_log_t *సంవర్గమానం = దత్తాంశం;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, సంవర్గమానం, 0, "destroy memory log buffer");

    సంవర్గమానం->wdata = ఖాళీ;
}

#అయితే_ముగింపు
