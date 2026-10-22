
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


अटल अक्षर *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf);
अटल अक्षर *ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log);
अटल व्योम ngx_log_जोड़ो(ngx_log_t *log, ngx_log_t *new_log);


#अगर (NGX_DEBUG)

अटल व्योम ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level,
    u_char *buf, माप_प्रकार len);
अटल व्योम ngx_log_memory_cleanup(व्योम *तथ्य);


प्रकार काष्ठा {
    u_char        *start;
    u_char        *end;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_memory_buf_t;

#पूर्ण_अगर


अटल ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1MORE,
      ngx_error_log,
      0,
      0,
      शून्य },

      ngx_null_command
};


अटल ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    शून्य,
    शून्य
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    शून्य,                                  /* init master */
    शून्य,                                  /* init module */
    शून्य,                                  /* init process */
    शून्य,                                  /* init thread */
    शून्य,                                  /* exit thread */
    शून्य,                                  /* exit process */
    शून्य,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


अटल ngx_log_t        ngx_log;
अटल ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


अटल ngx_str_t err_levels[] = {
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

अटल स्थिर अक्षर *debug_levels[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#अगर (NGX_HAVE_VARIADIC_MACROS)

व्योम
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    स्थिर अक्षर *fmt, ...)

#अन्यथा

व्योम
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    स्थिर अक्षर *fmt, बहु_सूची args)

#पूर्ण_अगर
{
#अगर (NGX_HAVE_VARIADIC_MACROS)
    बहु_सूची      args;
#पूर्ण_अगर
    u_char      *p, *last, *संदेश;
    ssize_t      n;
    ngx_uint_t   wrote_stderr, debug_connection;
    u_char       errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, ngx_cached_err_log_time.तथ्य,
                   ngx_cached_err_log_time.len);

    p = ngx_slprintf(p, last, " [%V] ", &err_levels[level]);

    /* pid#tid */
    p = ngx_slprintf(p, last, "%P#" NGX_TID_T_FMT ": ",
                    ngx_log_pid, ngx_log_tid);

    अगर (log->connection) {
        p = ngx_slprintf(p, last, "*%uA ", log->connection);
    }

    संदेश = p;

#अगर (NGX_HAVE_VARIADIC_MACROS)

    बहु_शुरू(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    बहु_पूर्ण(args);

#अन्यथा

    p = ngx_vslprintf(p, last, fmt, args);

#पूर्ण_अगर

    अगर (err) {
        p = ngx_log_errno(p, last, err);
    }

    अगर (level != NGX_LOG_DEBUG && log->handler) {
        p = log->handler(log, p, last - p);
    }

    अगर (p > last - NGX_LINEFEED_आकार) {
        p = last - NGX_LINEFEED_आकार;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (log->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    जबतक (log) {

        अगर (log->log_level < level && !debug_connection) {
            अवरोध;
        }

        अगर (log->writer) {
            log->writer(log, level, errstr, p - errstr);
            जाओ अग्र;
        }

        अगर (ngx_time() == log->disk_full_time) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            जाओ अग्र;
        }

        n = ngx_write_fd(log->file->fd, errstr, p - errstr);

        अगर (n == -1 && ngx_errno == NGX_ENOSPC) {
            log->disk_full_time = ngx_time();
        }

        अगर (log->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    अग्र:

        log = log->अग्र;
    }

    अगर (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        वापस;
    }

    संदेश -= (7 + err_levels[level].len + 3);

    (व्योम) ngx_sprintf(संदेश, "nginx: [%V] ", &err_levels[level]);

    (व्योम) ngx_write_console(ngx_stderr, संदेश, p - संदेश);
}


#अगर !(NGX_HAVE_VARIADIC_MACROS)

व्योम ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    स्थिर अक्षर *fmt, ...)
{
    बहु_सूची  args;

    अगर (log->log_level >= level) {
        बहु_शुरू(args, fmt);
        ngx_log_error_core(level, log, err, fmt, args);
        बहु_पूर्ण(args);
    }
}


व्योम ngx_cdecl
ngx_log_debug_core(ngx_log_t *log, ngx_err_t err, स्थिर अक्षर *fmt, ...)
{
    बहु_सूची  args;

    बहु_शुरू(args, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, log, err, fmt, args);
    बहु_पूर्ण(args);
}

#पूर्ण_अगर


व्योम ngx_cdecl
ngx_log_abort(ngx_err_t err, स्थिर अक्षर *fmt, ...)
{
    u_char   *p;
    बहु_सूची   args;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    बहु_शुरू(args, fmt);
    p = ngx_vsnprintf(errstr, माप(errstr) - 1, fmt, args);
    बहु_पूर्ण(args);

    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                  "%*s", p - errstr, errstr);
}


व्योम ngx_cdecl
ngx_log_stderr(ngx_err_t err, स्थिर अक्षर *fmt, ...)
{
    u_char   *p, *last;
    बहु_सूची   args;
    u_char    errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    बहु_शुरू(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    बहु_पूर्ण(args);

    अगर (err) {
        p = ngx_log_errno(p, last, err);
    }

    अगर (p > last - NGX_LINEFEED_आकार) {
        p = last - NGX_LINEFEED_आकार;
    }

    ngx_linefeed(p);

    (व्योम) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err)
{
    अगर (buf > last - 50) {

        /* leave a space for an error code */

        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#अगर (NGX_WIN32)
    buf = ngx_slprintf(buf, last, ((अचिन्हित) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#अन्यथा
    buf = ngx_slprintf(buf, last, " (%d: ", err);
#पूर्ण_अगर

    buf = ngx_strerror(err, buf, last - buf);

    अगर (buf < last) {
        *buf++ = ')';
    }

    वापस buf;
}


ngx_log_t *
ngx_log_init(u_char *prefix, u_char *error_log)
{
    u_char  *p, *name;
    माप_प्रकार   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    अगर (error_log == शून्य) {
        error_log = (u_char *) NGX_ERROR_LOG_PATH;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    अगर (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        वापस &ngx_log;
    }

    p = शून्य;

#अगर (NGX_WIN32)
    अगर (name[1] != ':') {
#अन्यथा
    अगर (name[0] != '/') {
#पूर्ण_अगर

        अगर (prefix) {
            plen = ngx_strlen(prefix);

        } अन्यथा {
#अगर_घोषित NGX_PREFIX
            prefix = (u_char *) NGX_PREFIX;
            plen = ngx_strlen(prefix);
#अन्यथा
            plen = 0;
#पूर्ण_अगर
        }

        अगर (plen) {
            name = दो_स्मृति(plen + nlen + 2);
            अगर (name == शून्य) {
                वापस शून्य;
            }

            p = ngx_cpymem(name, prefix, plen);

            अगर (!ngx_path_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    अगर (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#अगर (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#पूर्ण_अगर

        ngx_log_file.fd = ngx_stderr;
    }

    अगर (p) {
        ngx_free(p);
    }

    वापस &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_cycle_t *cycle)
{
    ngx_log_t  *log;

    अगर (ngx_log_get_file_log(&cycle->new_log) != शून्य) {
        वापस NGX_OK;
    }

    अगर (cycle->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        log = ngx_pcalloc(cycle->pool, माप(ngx_log_t));
        अगर (log == शून्य) {
            वापस NGX_ERROR;
        }

    } अन्यथा {
        /* no error logs at all */
        log = &cycle->new_log;
    }

    log->log_level = NGX_LOG_ERR;

    log->file = ngx_conf_open_file(cycle, &cycle->error_log);
    अगर (log->file == शून्य) {
        वापस NGX_ERROR;
    }

    अगर (log != &cycle->new_log) {
        ngx_log_जोड़ो(&cycle->new_log, log);
    }

    वापस NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_cycle_t *cycle)
{
    ngx_fd_t  fd;

    अगर (cycle->log_use_stderr) {
        वापस NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(cycle->log)->file->fd;

    अगर (fd != ngx_stderr) {
        अगर (ngx_set_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");

            वापस NGX_ERROR;
        }
    }

    वापस NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *शीर्ष)
{
    ngx_log_t  *log;

    क्रम (log = शीर्ष; log; log = log->अग्र) {
        अगर (log->file != शून्य) {
            वापस log;
        }
    }

    वापस शून्य;
}


अटल अक्षर *
ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log)
{
    ngx_uint_t   i, n, d, found;
    ngx_str_t   *मूल्य;

    अगर (cf->args->nelts == 2) {
        log->log_level = NGX_LOG_ERR;
        वापस NGX_CONF_OK;
    }

    मूल्य = cf->args->elts;

    क्रम (i = 2; i < cf->args->nelts; i++) {
        found = 0;

        क्रम (n = 1; n <= NGX_LOG_DEBUG; n++) {
            अगर (ngx_strcmp(मूल्य[i].तथ्य, err_levels[n].तथ्य) == 0) {

                अगर (log->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &मूल्य[i]);
                    वापस NGX_CONF_ERROR;
                }

                log->log_level = n;
                found = 1;
                अवरोध;
            }
        }

        क्रम (n = 0, d = NGX_LOG_DEBUG_FIRST; d <= NGX_LOG_DEBUG_LAST; d <<= 1) {
            अगर (ngx_strcmp(मूल्य[i].तथ्य, debug_levels[n++]) == 0) {
                अगर (log->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &मूल्य[i]);
                    वापस NGX_CONF_ERROR;
                }

                log->log_level |= d;
                found = 1;
                अवरोध;
            }
        }


        अगर (!found) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &मूल्य[i]);
            वापस NGX_CONF_ERROR;
        }
    }

    अगर (log->log_level == NGX_LOG_DEBUG) {
        log->log_level = NGX_LOG_DEBUG_ALL;
    }

    वापस NGX_CONF_OK;
}


अटल अक्षर *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->cycle->new_log;

    वापस ngx_log_set_log(cf, &dummy);
}


अक्षर *
ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **शीर्ष)
{
    ngx_log_t          *new_log;
    ngx_str_t          *मूल्य, name;
    ngx_syslog_peer_t  *peer;

    अगर (*शीर्ष != शून्य && (*शीर्ष)->log_level == 0) {
        new_log = *शीर्ष;

    } अन्यथा {

        new_log = ngx_pcalloc(cf->pool, माप(ngx_log_t));
        अगर (new_log == शून्य) {
            वापस NGX_CONF_ERROR;
        }

        अगर (*शीर्ष == शून्य) {
            *शीर्ष = new_log;
        }
    }

    मूल्य = cf->args->elts;

    अगर (ngx_strcmp(मूल्य[1].तथ्य, "stderr") == 0) {
        ngx_str_null(&name);
        cf->cycle->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->cycle, &name);
        अगर (new_log->file == शून्य) {
            वापस NGX_CONF_ERROR;
        }

    } अन्यथा अगर (ngx_strncmp(मूल्य[1].तथ्य, "memory:", 7) == 0) {

#अगर (NGX_DEBUG)
        माप_प्रकार                 आकार, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_memory_buf_t  *buf;

        मूल्य[1].len -= 7;
        मूल्य[1].तथ्य += 7;

        needed = माप("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_आकार_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        आकार = ngx_parse_आकार(&मूल्य[1]);

        अगर (आकार == (माप_प्रकार) NGX_ERROR || आकार < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &मूल्य[1]);
            वापस NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, माप(ngx_log_memory_buf_t));
        अगर (buf == शून्य) {
            वापस NGX_CONF_ERROR;
        }

        buf->start = ngx_pnalloc(cf->pool, आकार);
        अगर (buf->start == शून्य) {
            वापस NGX_CONF_ERROR;
        }

        buf->end = buf->start + आकार;

        buf->pos = ngx_slprintf(buf->start, buf->end, "MEMLOG %uz %V:%ui%N",
                                आकार, &cf->conf_file->file.name,
                                cf->conf_file->पंक्ति);

        ngx_memset(buf->pos, ' ', buf->end - buf->pos);

        cln = ngx_pool_cleanup_add(cf->pool, 0);
        अगर (cln == शून्य) {
            वापस NGX_CONF_ERROR;
        }

        cln->तथ्य = new_log;
        cln->handler = ngx_log_memory_cleanup;

        new_log->writer = ngx_log_memory_writer;
        new_log->wdata = buf;

#अन्यथा
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        वापस NGX_CONF_ERROR;
#पूर्ण_अगर

    } अन्यथा अगर (ngx_strncmp(मूल्य[1].तथ्य, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, माप(ngx_syslog_peer_t));
        अगर (peer == शून्य) {
            वापस NGX_CONF_ERROR;
        }

        अगर (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            वापस NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } अन्यथा {
        new_log->file = ngx_conf_open_file(cf->cycle, &मूल्य[1]);
        अगर (new_log->file == शून्य) {
            वापस NGX_CONF_ERROR;
        }
    }

    अगर (ngx_log_set_levels(cf, new_log) != NGX_CONF_OK) {
        वापस NGX_CONF_ERROR;
    }

    अगर (*शीर्ष != new_log) {
        ngx_log_जोड़ो(*शीर्ष, new_log);
    }

    वापस NGX_CONF_OK;
}


अटल व्योम
ngx_log_जोड़ो(ngx_log_t *log, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    अगर (new_log->log_level > log->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *log;
        *log = *new_log;
        *new_log = tmp;

        log->अग्र = new_log;
        वापस;
    }

    जबतक (log->अग्र) {
        अगर (new_log->log_level > log->अग्र->log_level) {
            new_log->अग्र = log->अग्र;
            log->अग्र = new_log;
            वापस;
        }

        log = log->अग्र;
    }

    log->अग्र = new_log;
}


#अगर (NGX_DEBUG)

अटल व्योम
ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    माप_प्रकार len)
{
    u_char                *p;
    माप_प्रकार                 avail, written;
    ngx_log_memory_buf_t  *mem;

    mem = log->wdata;

    अगर (mem == शून्य) {
        वापस;
    }

    written = ngx_atomic_fetch_add(&mem->written, len);

    p = mem->pos + written % (mem->end - mem->pos);

    avail = mem->end - p;

    अगर (avail >= len) {
        ngx_memcpy(p, buf, len);

    } अन्यथा {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


अटल व्योम
ngx_log_memory_cleanup(व्योम *तथ्य)
{
    ngx_log_t *log = तथ्य;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, log, 0, "destroy memory log buffer");

    log->wdata = शून्य;
}

#पूर्ण_अगर
