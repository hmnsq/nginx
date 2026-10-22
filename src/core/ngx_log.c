
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर वर्ण *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
स्थिर वर्ण *ngx_log_समुच्चयः_स्तराः(ngx_conf_t *cf, ngx_log_t *लघुगणकः);
स्थिर शून्य ngx_log_निवेशय(ngx_log_t *लघुगणकः, ngx_log_t *new_log);


#यदि (NGX_DEBUG)

स्थिर शून्य ngx_log_स्मृतिः_writer(ngx_log_t *लघुगणकः, ngx_uint_t level,
    u_char *buf, आकार_t len);
स्थिर शून्य ngx_log_स्मृतिः_cleanup(शून्य *दत्त);


प्रकारनिर्देश संरचना {
    u_char        *start;
    u_char        *अन्तः;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_स्मृतिः_buf_t;

#यदि_अन्तः


स्थिर ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1_अधिकम्,
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


स्थिर ngx_str_t err_स्तराः[] = {
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

स्थिर ध्रुव वर्ण *debug_स्तराः[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#यदि (NGX_HAVE_VARIADIC_MACROS)

शून्य
ngx_log_error_core(ngx_uint_t level, ngx_log_t *लघुगणकः, ngx_err_t err,
    ध्रुव वर्ण *fmt, ...)

#अन्यथा

शून्य
ngx_log_error_core(ngx_uint_t level, ngx_log_t *लघुगणकः, ngx_err_t err,
    ध्रुव वर्ण *fmt, va_list तर्काः)

#यदि_अन्तः
{
#यदि (NGX_HAVE_VARIADIC_MACROS)
    va_list      तर्काः;
#यदि_अन्तः
    u_char      *p, *चरमः, *सन्देशः;
    ssize_t      n;
    ngx_uint_t   wrote_stderr, debug_connection;
    u_char       errstr[NGX_MAX_ERROR_STR];

    चरमः = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, ngx_cached_err_log_time.दत्त,
                   ngx_cached_err_log_time.len);

    p = ngx_slprintf(p, चरमः, " [%V] ", &err_स्तराः[level]);

    /* pid#tid */
    p = ngx_slprintf(p, चरमः, "%P#" NGX_TID_T_FMT ": ",
                    ngx_log_pid, ngx_log_tid);

    यदि (लघुगणकः->connection) {
        p = ngx_slprintf(p, चरमः, "*%uA ", लघुगणकः->connection);
    }

    सन्देशः = p;

#यदि (NGX_HAVE_VARIADIC_MACROS)

    va_start(तर्काः, fmt);
    p = ngx_vslprintf(p, चरमः, fmt, तर्काः);
    va_अन्तः(तर्काः);

#अन्यथा

    p = ngx_vslprintf(p, चरमः, fmt, तर्काः);

#यदि_अन्तः

    यदि (err) {
        p = ngx_log_errno(p, चरमः, err);
    }

    यदि (level != NGX_LOG_DEBUG && लघुगणकः->handler) {
        p = लघुगणकः->handler(लघुगणकः, p, चरमः - p);
    }

    यदि (p > चरमः - NGX_LINEFEED_आकार) {
        p = चरमः - NGX_LINEFEED_आकार;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (लघुगणकः->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    यावत् (लघुगणकः) {

        यदि (लघुगणकः->log_level < level && !debug_connection) {
            भङ्ग;
        }

        यदि (लघुगणकः->writer) {
            लघुगणकः->writer(लघुगणकः, level, errstr, p - errstr);
            गच्छ अग्र;
        }

        यदि (ngx_time() == लघुगणकः->disk_पूर्णम्_time) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            गच्छ अग्र;
        }

        n = ngx_write_fd(लघुगणकः->file->fd, errstr, p - errstr);

        यदि (n == -1 && ngx_errno == NGX_ENOSPC) {
            लघुगणकः->disk_पूर्णम्_time = ngx_time();
        }

        यदि (लघुगणकः->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    अग्र:

        लघुगणकः = लघुगणकः->अग्र;
    }

    यदि (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        प्रत्यागम;
    }

    सन्देशः -= (7 + err_स्तराः[level].len + 3);

    (शून्य) ngx_sprintf(सन्देशः, "nginx: [%V] ", &err_स्तराः[level]);

    (शून्य) ngx_write_console(ngx_stderr, सन्देशः, p - सन्देशः);
}


#यदि !(NGX_HAVE_VARIADIC_MACROS)

शून्य ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *लघुगणकः, ngx_err_t err,
    ध्रुव वर्ण *fmt, ...)
{
    va_list  तर्काः;

    यदि (लघुगणकः->log_level >= level) {
        va_start(तर्काः, fmt);
        ngx_log_error_core(level, लघुगणकः, err, fmt, तर्काः);
        va_अन्तः(तर्काः);
    }
}


शून्य ngx_cdecl
ngx_log_debug_core(ngx_log_t *लघुगणकः, ngx_err_t err, ध्रुव वर्ण *fmt, ...)
{
    va_list  तर्काः;

    va_start(तर्काः, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, लघुगणकः, err, fmt, तर्काः);
    va_अन्तः(तर्काः);
}

#यदि_अन्तः


शून्य ngx_cdecl
ngx_log_abort(ngx_err_t err, ध्रुव वर्ण *fmt, ...)
{
    u_char   *p;
    va_list   तर्काः;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_start(तर्काः, fmt);
    p = ngx_vsnprintf(errstr, परिमाण(errstr) - 1, fmt, तर्काः);
    va_अन्तः(तर्काः);

    ngx_log_error(NGX_LOG_ALERT, ngx_आवृत्तिः->लघुगणकः, err,
                  "%*s", p - errstr, errstr);
}


शून्य ngx_cdecl
ngx_log_stderr(ngx_err_t err, ध्रुव वर्ण *fmt, ...)
{
    u_char   *p, *चरमः;
    va_list   तर्काः;
    u_char    errstr[NGX_MAX_ERROR_STR];

    चरमः = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_start(तर्काः, fmt);
    p = ngx_vslprintf(p, चरमः, fmt, तर्काः);
    va_अन्तः(तर्काः);

    यदि (err) {
        p = ngx_log_errno(p, चरमः, err);
    }

    यदि (p > चरमः - NGX_LINEFEED_आकार) {
        p = चरमः - NGX_LINEFEED_आकार;
    }

    ngx_linefeed(p);

    (शून्य) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *चरमः, ngx_err_t err)
{
    यदि (buf > चरमः - 50) {

        /* leave a space for an error code */

        buf = चरमः - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#यदि (NGX_WIN32)
    buf = ngx_slprintf(buf, चरमः, ((अचिह्नित) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#अन्यथा
    buf = ngx_slprintf(buf, चरमः, " (%d: ", err);
#यदि_अन्तः

    buf = ngx_strerror(err, buf, चरमः - buf);

    यदि (buf < चरमः) {
        *buf++ = ')';
    }

    प्रत्यागम buf;
}


ngx_log_t *
ngx_log_init(u_char *उपसर्गः, u_char *error_log)
{
    u_char  *p, *name;
    आकार_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    यदि (error_log == रिक्त) {
        error_log = (u_char *) NGX_ERROR_LOG_मार्गः;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    यदि (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        प्रत्यागम &ngx_log;
    }

    p = रिक्त;

#यदि (NGX_WIN32)
    यदि (name[1] != ':') {
#अन्यथा
    यदि (name[0] != '/') {
#यदि_अन्तः

        यदि (उपसर्गः) {
            plen = ngx_strlen(उपसर्गः);

        } अन्यथा {
#यदि_परिभाषितम् NGX_उपसर्गः
            उपसर्गः = (u_char *) NGX_उपसर्गः;
            plen = ngx_strlen(उपसर्गः);
#अन्यथा
            plen = 0;
#यदि_अन्तः
        }

        यदि (plen) {
            name = स्मृतिं_देहि(plen + nlen + 2);
            यदि (name == रिक्त) {
                प्रत्यागम रिक्त;
            }

            p = ngx_cpymem(name, उपसर्गः, plen);

            यदि (!ngx_मार्गः_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    यदि (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#यदि (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#यदि_अन्तः

        ngx_log_file.fd = ngx_stderr;
    }

    यदि (p) {
        ngx_free(p);
    }

    प्रत्यागम &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_आवृत्तिः_t *आवृत्तिः)
{
    ngx_log_t  *लघुगणकः;

    यदि (ngx_log_get_file_log(&आवृत्तिः->new_log) != रिक्त) {
        प्रत्यागम NGX_OK;
    }

    यदि (आवृत्तिः->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        लघुगणकः = ngx_pcalloc(आवृत्तिः->pool, परिमाण(ngx_log_t));
        यदि (लघुगणकः == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

    } अन्यथा {
        /* no error logs at all */
        लघुगणकः = &आवृत्तिः->new_log;
    }

    लघुगणकः->log_level = NGX_LOG_ERR;

    लघुगणकः->file = ngx_conf_open_file(आवृत्तिः, &आवृत्तिः->error_log);
    यदि (लघुगणकः->file == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    यदि (लघुगणकः != &आवृत्तिः->new_log) {
        ngx_log_निवेशय(&आवृत्तिः->new_log, लघुगणकः);
    }

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_आवृत्तिः_t *आवृत्तिः)
{
    ngx_fd_t  fd;

    यदि (आवृत्तिः->log_use_stderr) {
        प्रत्यागम NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(आवृत्तिः->लघुगणकः)->file->fd;

    यदि (fd != ngx_stderr) {
        यदि (ngx_समुच्चयः_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, आवृत्तिः->लघुगणकः, ngx_errno,
                          ngx_समुच्चयः_stderr_n " failed");

            प्रत्यागम NGX_ERROR;
        }
    }

    प्रत्यागम NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *शीर्ष)
{
    ngx_log_t  *लघुगणकः;

    प्रति (लघुगणकः = शीर्ष; लघुगणकः; लघुगणकः = लघुगणकः->अग्र) {
        यदि (लघुगणकः->file != रिक्त) {
            प्रत्यागम लघुगणकः;
        }
    }

    प्रत्यागम रिक्त;
}


स्थिर वर्ण *
ngx_log_समुच्चयः_स्तराः(ngx_conf_t *cf, ngx_log_t *लघुगणकः)
{
    ngx_uint_t   i, n, d, प्राप्तम्;
    ngx_str_t   *मानम्;

    यदि (cf->तर्काः->nelts == 2) {
        लघुगणकः->log_level = NGX_LOG_ERR;
        प्रत्यागम NGX_CONF_OK;
    }

    मानम् = cf->तर्काः->elts;

    प्रति (i = 2; i < cf->तर्काः->nelts; i++) {
        प्राप्तम् = 0;

        प्रति (n = 1; n <= NGX_LOG_DEBUG; n++) {
            यदि (ngx_strcmp(मानम्[i].दत्त, err_स्तराः[n].दत्त) == 0) {

                यदि (लघुगणकः->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &मानम्[i]);
                    प्रत्यागम NGX_CONF_ERROR;
                }

                लघुगणकः->log_level = n;
                प्राप्तम् = 1;
                भङ्ग;
            }
        }

        प्रति (n = 0, d = NGX_LOG_DEBUG_FIRST; d <= NGX_LOG_DEBUG_चरमः; d <<= 1) {
            यदि (ngx_strcmp(मानम्[i].दत्त, debug_स्तराः[n++]) == 0) {
                यदि (लघुगणकः->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &मानम्[i]);
                    प्रत्यागम NGX_CONF_ERROR;
                }

                लघुगणकः->log_level |= d;
                प्राप्तम् = 1;
                भङ्ग;
            }
        }


        यदि (!प्राप्तम्) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &मानम्[i]);
            प्रत्यागम NGX_CONF_ERROR;
        }
    }

    यदि (लघुगणकः->log_level == NGX_LOG_DEBUG) {
        लघुगणकः->log_level = NGX_LOG_DEBUG_ALL;
    }

    प्रत्यागम NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->आवृत्तिः->new_log;

    प्रत्यागम ngx_log_समुच्चयः_log(cf, &dummy);
}


वर्ण *
ngx_log_समुच्चयः_log(ngx_conf_t *cf, ngx_log_t **शीर्ष)
{
    ngx_log_t          *new_log;
    ngx_str_t          *मानम्, name;
    ngx_syslog_peer_t  *peer;

    यदि (*शीर्ष != रिक्त && (*शीर्ष)->log_level == 0) {
        new_log = *शीर्ष;

    } अन्यथा {

        new_log = ngx_pcalloc(cf->pool, परिमाण(ngx_log_t));
        यदि (new_log == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        यदि (*शीर्ष == रिक्त) {
            *शीर्ष = new_log;
        }
    }

    मानम् = cf->तर्काः->elts;

    यदि (ngx_strcmp(मानम्[1].दत्त, "stderr") == 0) {
        ngx_str_null(&name);
        cf->आवृत्तिः->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->आवृत्तिः, &name);
        यदि (new_log->file == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

    } अन्यथा यदि (ngx_strncmp(मानम्[1].दत्त, "memory:", 7) == 0) {

#यदि (NGX_DEBUG)
        आकार_t                 आकार, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_स्मृतिः_buf_t  *buf;

        मानम्[1].len -= 7;
        मानम्[1].दत्त += 7;

        needed = परिमाण("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_आकार_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        आकार = ngx_parse_आकार(&मानम्[1]);

        यदि (आकार == (आकार_t) NGX_ERROR || आकार < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &मानम्[1]);
            प्रत्यागम NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, परिमाण(ngx_log_स्मृतिः_buf_t));
        यदि (buf == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        buf->start = ngx_pnalloc(cf->pool, आकार);
        यदि (buf->start == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        buf->अन्तः = buf->start + आकार;

        buf->pos = ngx_slprintf(buf->start, buf->अन्तः, "MEMLOG %uz %V:%ui%N",
                                आकार, &cf->conf_file->file.name,
                                cf->conf_file->पङ्क्तिः);

        ngx_memset(buf->pos, ' ', buf->अन्तः - buf->pos);

        cln = ngx_pool_cleanup_सङ्कलय(cf->pool, 0);
        यदि (cln == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        cln->दत्त = new_log;
        cln->handler = ngx_log_स्मृतिः_cleanup;

        new_log->writer = ngx_log_स्मृतिः_writer;
        new_log->wdata = buf;

#अन्यथा
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        प्रत्यागम NGX_CONF_ERROR;
#यदि_अन्तः

    } अन्यथा यदि (ngx_strncmp(मानम्[1].दत्त, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, परिमाण(ngx_syslog_peer_t));
        यदि (peer == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        यदि (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } अन्यथा {
        new_log->file = ngx_conf_open_file(cf->आवृत्तिः, &मानम्[1]);
        यदि (new_log->file == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }
    }

    यदि (ngx_log_समुच्चयः_स्तराः(cf, new_log) != NGX_CONF_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    यदि (*शीर्ष != new_log) {
        ngx_log_निवेशय(*शीर्ष, new_log);
    }

    प्रत्यागम NGX_CONF_OK;
}


स्थिर शून्य
ngx_log_निवेशय(ngx_log_t *लघुगणकः, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    यदि (new_log->log_level > लघुगणकः->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *लघुगणकः;
        *लघुगणकः = *new_log;
        *new_log = tmp;

        लघुगणकः->अग्र = new_log;
        प्रत्यागम;
    }

    यावत् (लघुगणकः->अग्र) {
        यदि (new_log->log_level > लघुगणकः->अग्र->log_level) {
            new_log->अग्र = लघुगणकः->अग्र;
            लघुगणकः->अग्र = new_log;
            प्रत्यागम;
        }

        लघुगणकः = लघुगणकः->अग्र;
    }

    लघुगणकः->अग्र = new_log;
}


#यदि (NGX_DEBUG)

स्थिर शून्य
ngx_log_स्मृतिः_writer(ngx_log_t *लघुगणकः, ngx_uint_t level, u_char *buf,
    आकार_t len)
{
    u_char                *p;
    आकार_t                 avail, written;
    ngx_log_स्मृतिः_buf_t  *mem;

    mem = लघुगणकः->wdata;

    यदि (mem == रिक्त) {
        प्रत्यागम;
    }

    written = ngx_atomic_fetch_सङ्कलय(&mem->written, len);

    p = mem->pos + written % (mem->अन्तः - mem->pos);

    avail = mem->अन्तः - p;

    यदि (avail >= len) {
        ngx_memcpy(p, buf, len);

    } अन्यथा {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


स्थिर शून्य
ngx_log_स्मृतिः_cleanup(शून्य *दत्त)
{
    ngx_log_t *लघुगणकः = दत्त;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, लघुगणकः, 0, "destroy memory log buffer");

    लघुगणकः->wdata = रिक्त;
}

#यदि_अन्तः
