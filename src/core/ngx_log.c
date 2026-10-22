
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


static var_na *ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static var_na *ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log);
static void ngx_log_insert(ngx_log_t *log, ngx_log_t *new_log);


#ប្រសិនបើ (NGX_DEBUG)

static void ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level,
    u_char *buf, size_t len);
static void ngx_log_memory_cleanup(void *data);


typedef sa_nracanaa {
    u_char        *start;
    u_char        *end;
    u_char        *pos;
    ngx_atomic_t   written;
} ngx_log_memory_buf_t;

#endif


static ngx_command_t  ngx_errlog_commands[] = {

    { ngx_string("error_log"),
      NGX_MAIN_CONF|NGX_CONF_1MORE,
      ngx_error_log,
      0,
      0,
      NULL },

      ngx_null_command
};


static ngx_core_module_t  ngx_errlog_module_ctx = {
    ngx_string("errlog"),
    NULL,
    NULL
};


ngx_module_t  ngx_errlog_module = {
    NGX_MODULE_V1,
    &ngx_errlog_module_ctx,                /* module context */
    ngx_errlog_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_log_t        ngx_log;
static ngx_open_file_t  ngx_log_file;
ngx_uint_t              ngx_use_stderr = 1;


static ngx_str_t err_levels[] = {
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

static const var_na *debug_levels[] = {
    "debug_core", "debug_alloc", "debug_mutex", "debug_event",
    "debug_http", "debug_mail", "debug_stream"
};


#ប្រសិនបើ (NGX_HAVE_VARIADIC_MACROS)

void
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    const var_na *fmt, ...)

#បើមិនដូច្នេះ

void
ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    const var_na *fmt, va_list args)

#endif
{
#ប្រសិនបើ (NGX_HAVE_VARIADIC_MACROS)
    va_list      args;
#endif
    u_char      *p, *last, *msg;
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

    ប្រសិនបើ (log->connection) {
        p = ngx_slprintf(p, last, "*%uA ", log->connection);
    }

    msg = p;

#ប្រសិនបើ (NGX_HAVE_VARIADIC_MACROS)

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

#បើមិនដូច្នេះ

    p = ngx_vslprintf(p, last, fmt, args);

#endif

    ប្រសិនបើ (err) {
        p = ngx_log_errno(p, last, err);
    }

    ប្រសិនបើ (level != NGX_LOG_DEBUG && log->handler) {
        p = log->handler(log, p, last - p);
    }

    ប្រសិនបើ (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    wrote_stderr = 0;
    debug_connection = (log->log_level & NGX_LOG_DEBUG_CONNECTION) != 0;

    ខណៈពេល (log) {

        ប្រសិនបើ (log->log_level < level && !debug_connection) {
            bha_nga;
        }

        ប្រសិនបើ (log->writer) {
            log->writer(log, level, errstr, p - errstr);
            goto next;
        }

        ប្រសិនបើ (ngx_time() == log->disk_full_time) {

            /*
             * on FreeBSD writing to a full filesystem with enabled softupdates
             * may block process for much longer time than writing to non-full
             * filesystem, so we skip writing to a log for one second
             */

            goto next;
        }

        n = ngx_write_fd(log->file->fd, errstr, p - errstr);

        ប្រសិនបើ (n == -1 && ngx_errno == NGX_ENOSPC) {
            log->disk_full_time = ngx_time();
        }

        ប្រសិនបើ (log->file->fd == ngx_stderr) {
            wrote_stderr = 1;
        }

    next:

        log = log->next;
    }

    ប្រសិនបើ (!ngx_use_stderr
        || level > NGX_LOG_WARN
        || wrote_stderr)
    {
        ត្រឡប់;
    }

    msg -= (7 + err_levels[level].len + 3);

    (void) ngx_sprintf(msg, "nginx: [%V] ", &err_levels[level]);

    (void) ngx_write_console(ngx_stderr, msg, p - msg);
}


#ប្រសិនបើ !(NGX_HAVE_VARIADIC_MACROS)

void ngx_cdecl
ngx_log_error(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
    const var_na *fmt, ...)
{
    va_list  args;

    ប្រសិនបើ (log->log_level >= level) {
        va_start(args, fmt);
        ngx_log_error_core(level, log, err, fmt, args);
        va_end(args);
    }
}


void ngx_cdecl
ngx_log_debug_core(ngx_log_t *log, ngx_err_t err, const var_na *fmt, ...)
{
    va_list  args;

    va_start(args, fmt);
    ngx_log_error_core(NGX_LOG_DEBUG, log, err, fmt, args);
    va_end(args);
}

#endif


void ngx_cdecl
ngx_log_abort(ngx_err_t err, const var_na *fmt, ...)
{
    u_char   *p;
    va_list   args;
    u_char    errstr[NGX_MAX_CONF_ERRSTR];

    va_start(args, fmt);
    p = ngx_vsnprintf(errstr, parimaa_na(errstr) - 1, fmt, args);
    va_end(args);

    ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, err,
                  "%*s", p - errstr, errstr);
}


void ngx_cdecl
ngx_log_stderr(ngx_err_t err, const var_na *fmt, ...)
{
    u_char   *p, *last;
    va_list   args;
    u_char    errstr[NGX_MAX_ERROR_STR];

    last = errstr + NGX_MAX_ERROR_STR;

    p = ngx_cpymem(errstr, "nginx: ", 7);

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

    ប្រសិនបើ (err) {
        p = ngx_log_errno(p, last, err);
    }

    ប្រសិនបើ (p > last - NGX_LINEFEED_SIZE) {
        p = last - NGX_LINEFEED_SIZE;
    }

    ngx_linefeed(p);

    (void) ngx_write_console(ngx_stderr, errstr, p - errstr);
}


u_char *
ngx_log_errno(u_char *buf, u_char *last, ngx_err_t err)
{
    ប្រសិនបើ (buf > last - 50) {

        /* leave a space for an error code */

        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

#ប្រសិនបើ (NGX_WIN32)
    buf = ngx_slprintf(buf, last, ((unsigned) err < 0x80000000)
                                       ? " (%d: " : " (%Xd: ", err);
#បើមិនដូច្នេះ
    buf = ngx_slprintf(buf, last, " (%d: ", err);
#endif

    buf = ngx_strerror(err, buf, last - buf);

    ប្រសិនបើ (buf < last) {
        *buf++ = ')';
    }

    ត្រឡប់ buf;
}


ngx_log_t *
ngx_log_init(u_char *prefix, u_char *error_log)
{
    u_char  *p, *name;
    size_t   nlen, plen;

    ngx_log.file = &ngx_log_file;
    ngx_log.log_level = NGX_LOG_NOTICE;

    ប្រសិនបើ (error_log == NULL) {
        error_log = (u_char *) NGX_ERROR_LOG_PATH;
    }

    name = error_log;
    nlen = ngx_strlen(name);

    ប្រសិនបើ (nlen == 0) {
        ngx_log_file.fd = ngx_stderr;
        ត្រឡប់ &ngx_log;
    }

    p = NULL;

#ប្រសិនបើ (NGX_WIN32)
    ប្រសិនបើ (name[1] != ':') {
#បើមិនដូច្នេះ
    ប្រសិនបើ (name[0] != '/') {
#endif

        ប្រសិនបើ (prefix) {
            plen = ngx_strlen(prefix);

        } បើមិនដូច្នេះ {
#ifdef NGX_PREFIX
            prefix = (u_char *) NGX_PREFIX;
            plen = ngx_strlen(prefix);
#បើមិនដូច្នេះ
            plen = 0;
#endif
        }

        ប្រសិនបើ (plen) {
            name = malloc(plen + nlen + 2);
            ប្រសិនបើ (name == NULL) {
                ត្រឡប់ NULL;
            }

            p = ngx_cpymem(name, prefix, plen);

            ប្រសិនបើ (!ngx_path_separator(*(p - 1))) {
                *p++ = '/';
            }

            ngx_cpystrn(p, error_log, nlen + 1);

            p = name;
        }
    }

    ngx_log_file.fd = ngx_open_file(name, NGX_FILE_APPEND,
                                    NGX_FILE_CREATE_OR_OPEN,
                                    NGX_FILE_DEFAULT_ACCESS);

    ប្រសិនបើ (ngx_log_file.fd == NGX_INVALID_FILE) {
        ngx_log_stderr(ngx_errno,
                       "[alert] could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#ប្រសិនបើ (NGX_WIN32)
        ngx_event_log(ngx_errno,
                       "could not open error log file: "
                       ngx_open_file_n " \"%s\" failed", name);
#endif

        ngx_log_file.fd = ngx_stderr;
    }

    ប្រសិនបើ (p) {
        ngx_free(p);
    }

    ត្រឡប់ &ngx_log;
}


ngx_int_t
ngx_log_open_default(ngx_cycle_t *cycle)
{
    ngx_log_t  *log;

    ប្រសិនបើ (ngx_log_get_file_log(&cycle->new_log) != NULL) {
        ត្រឡប់ NGX_OK;
    }

    ប្រសិនបើ (cycle->new_log.log_level != 0) {
        /* there are some error logs, but no files */

        log = ngx_pcalloc(cycle->pool, parimaa_na(ngx_log_t));
        ប្រសិនបើ (log == NULL) {
            ត្រឡប់ NGX_ERROR;
        }

    } បើមិនដូច្នេះ {
        /* no error logs at all */
        log = &cycle->new_log;
    }

    log->log_level = NGX_LOG_ERR;

    log->file = ngx_conf_open_file(cycle, &cycle->error_log);
    ប្រសិនបើ (log->file == NULL) {
        ត្រឡប់ NGX_ERROR;
    }

    ប្រសិនបើ (log != &cycle->new_log) {
        ngx_log_insert(&cycle->new_log, log);
    }

    ត្រឡប់ NGX_OK;
}


ngx_int_t
ngx_log_redirect_stderr(ngx_cycle_t *cycle)
{
    ngx_fd_t  fd;

    ប្រសិនបើ (cycle->log_use_stderr) {
        ត្រឡប់ NGX_OK;
    }

    /* file log always exists when we are called */
    fd = ngx_log_get_file_log(cycle->log)->file->fd;

    ប្រសិនបើ (fd != ngx_stderr) {
        ប្រសិនបើ (ngx_set_stderr(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_set_stderr_n " failed");

            ត្រឡប់ NGX_ERROR;
        }
    }

    ត្រឡប់ NGX_OK;
}


ngx_log_t *
ngx_log_get_file_log(ngx_log_t *head)
{
    ngx_log_t  *log;

    សម្រាប់ (log = head; log; log = log->next) {
        ប្រសិនបើ (log->file != NULL) {
            ត្រឡប់ log;
        }
    }

    ត្រឡប់ NULL;
}


static var_na *
ngx_log_set_levels(ngx_conf_t *cf, ngx_log_t *log)
{
    ngx_uint_t   i, n, d, found;
    ngx_str_t   *value;

    ប្រសិនបើ (cf->args->nelts == 2) {
        log->log_level = NGX_LOG_ERR;
        ត្រឡប់ NGX_CONF_OK;
    }

    value = cf->args->elts;

    សម្រាប់ (i = 2; i < cf->args->nelts; i++) {
        found = 0;

        សម្រាប់ (n = 1; n <= NGX_LOG_DEBUG; n++) {
            ប្រសិនបើ (ngx_strcmp(value[i].data, err_levels[n].data) == 0) {

                ប្រសិនបើ (log->log_level != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "duplicate log level \"%V\"",
                                       &value[i]);
                    ត្រឡប់ NGX_CONF_ERROR;
                }

                log->log_level = n;
                found = 1;
                bha_nga;
            }
        }

        សម្រាប់ (n = 0, d = NGX_LOG_DEBUG_FIRST; d <= NGX_LOG_DEBUG_LAST; d <<= 1) {
            ប្រសិនបើ (ngx_strcmp(value[i].data, debug_levels[n++]) == 0) {
                ប្រសិនបើ (log->log_level & ~NGX_LOG_DEBUG_ALL) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "invalid log level \"%V\"",
                                       &value[i]);
                    ត្រឡប់ NGX_CONF_ERROR;
                }

                log->log_level |= d;
                found = 1;
                bha_nga;
            }
        }


        ប្រសិនបើ (!found) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid log level \"%V\"", &value[i]);
            ត្រឡប់ NGX_CONF_ERROR;
        }
    }

    ប្រសិនបើ (log->log_level == NGX_LOG_DEBUG) {
        log->log_level = NGX_LOG_DEBUG_ALL;
    }

    ត្រឡប់ NGX_CONF_OK;
}


static var_na *
ngx_error_log(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_log_t  *dummy;

    dummy = &cf->cycle->new_log;

    ត្រឡប់ ngx_log_set_log(cf, &dummy);
}


var_na *
ngx_log_set_log(ngx_conf_t *cf, ngx_log_t **head)
{
    ngx_log_t          *new_log;
    ngx_str_t          *value, name;
    ngx_syslog_peer_t  *peer;

    ប្រសិនបើ (*head != NULL && (*head)->log_level == 0) {
        new_log = *head;

    } បើមិនដូច្នេះ {

        new_log = ngx_pcalloc(cf->pool, parimaa_na(ngx_log_t));
        ប្រសិនបើ (new_log == NULL) {
            ត្រឡប់ NGX_CONF_ERROR;
        }

        ប្រសិនបើ (*head == NULL) {
            *head = new_log;
        }
    }

    value = cf->args->elts;

    ប្រសិនបើ (ngx_strcmp(value[1].data, "stderr") == 0) {
        ngx_str_null(&name);
        cf->cycle->log_use_stderr = 1;

        new_log->file = ngx_conf_open_file(cf->cycle, &name);
        ប្រសិនបើ (new_log->file == NULL) {
            ត្រឡប់ NGX_CONF_ERROR;
        }

    } បើមិនដូច្នេះ ប្រសិនបើ (ngx_strncmp(value[1].data, "memory:", 7) == 0) {

#ប្រសិនបើ (NGX_DEBUG)
        size_t                 size, needed;
        ngx_pool_cleanup_t    *cln;
        ngx_log_memory_buf_t  *buf;

        value[1].len -= 7;
        value[1].data += 7;

        needed = parimaa_na("MEMLOG  :" NGX_LINEFEED)
                 + cf->conf_file->file.name.len
                 + NGX_SIZE_T_LEN
                 + NGX_INT_T_LEN
                 + NGX_MAX_ERROR_STR;

        size = ngx_parse_size(&value[1]);

        ប្រសិនបើ (size == (size_t) NGX_ERROR || size < needed) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid buffer size \"%V\"", &value[1]);
            ត្រឡប់ NGX_CONF_ERROR;
        }

        buf = ngx_pcalloc(cf->pool, parimaa_na(ngx_log_memory_buf_t));
        ប្រសិនបើ (buf == NULL) {
            ត្រឡប់ NGX_CONF_ERROR;
        }

        buf->start = ngx_pnalloc(cf->pool, size);
        ប្រសិនបើ (buf->start == NULL) {
            ត្រឡប់ NGX_CONF_ERROR;
        }

        buf->end = buf->start + size;

        buf->pos = ngx_slprintf(buf->start, buf->end, "MEMLOG %uz %V:%ui%N",
                                size, &cf->conf_file->file.name,
                                cf->conf_file->line);

        ngx_memset(buf->pos, ' ', buf->end - buf->pos);

        cln = ngx_pool_cleanup_add(cf->pool, 0);
        ប្រសិនបើ (cln == NULL) {
            ត្រឡប់ NGX_CONF_ERROR;
        }

        cln->data = new_log;
        cln->handler = ngx_log_memory_cleanup;

        new_log->writer = ngx_log_memory_writer;
        new_log->wdata = buf;

#បើមិនដូច្នេះ
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "nginx was built without debug support");
        ត្រឡប់ NGX_CONF_ERROR;
#endif

    } បើមិនដូច្នេះ ប្រសិនបើ (ngx_strncmp(value[1].data, "syslog:", 7) == 0) {
        peer = ngx_pcalloc(cf->pool, parimaa_na(ngx_syslog_peer_t));
        ប្រសិនបើ (peer == NULL) {
            ត្រឡប់ NGX_CONF_ERROR;
        }

        ប្រសិនបើ (ngx_syslog_process_conf(cf, peer) != NGX_CONF_OK) {
            ត្រឡប់ NGX_CONF_ERROR;
        }

        new_log->writer = ngx_syslog_writer;
        new_log->wdata = peer;

    } បើមិនដូច្នេះ {
        new_log->file = ngx_conf_open_file(cf->cycle, &value[1]);
        ប្រសិនបើ (new_log->file == NULL) {
            ត្រឡប់ NGX_CONF_ERROR;
        }
    }

    ប្រសិនបើ (ngx_log_set_levels(cf, new_log) != NGX_CONF_OK) {
        ត្រឡប់ NGX_CONF_ERROR;
    }

    ប្រសិនបើ (*head != new_log) {
        ngx_log_insert(*head, new_log);
    }

    ត្រឡប់ NGX_CONF_OK;
}


static void
ngx_log_insert(ngx_log_t *log, ngx_log_t *new_log)
{
    ngx_log_t  tmp;

    ប្រសិនបើ (new_log->log_level > log->log_level) {

        /*
         * list head address is permanent, insert new log after
         * head and swap its contents with head
         */

        tmp = *log;
        *log = *new_log;
        *new_log = tmp;

        log->next = new_log;
        ត្រឡប់;
    }

    ខណៈពេល (log->next) {
        ប្រសិនបើ (new_log->log_level > log->next->log_level) {
            new_log->next = log->next;
            log->next = new_log;
            ត្រឡប់;
        }

        log = log->next;
    }

    log->next = new_log;
}


#ប្រសិនបើ (NGX_DEBUG)

static void
ngx_log_memory_writer(ngx_log_t *log, ngx_uint_t level, u_char *buf,
    size_t len)
{
    u_char                *p;
    size_t                 avail, written;
    ngx_log_memory_buf_t  *mem;

    mem = log->wdata;

    ប្រសិនបើ (mem == NULL) {
        ត្រឡប់;
    }

    written = ngx_atomic_fetch_add(&mem->written, len);

    p = mem->pos + written % (mem->end - mem->pos);

    avail = mem->end - p;

    ប្រសិនបើ (avail >= len) {
        ngx_memcpy(p, buf, len);

    } បើមិនដូច្នេះ {
        ngx_memcpy(p, buf, avail);
        ngx_memcpy(mem->pos, buf + avail, len - avail);
    }
}


static void
ngx_log_memory_cleanup(void *data)
{
    ngx_log_t *log = data;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, log, 0, "destroy memory log buffer");

    log->wdata = NULL;
}

#endif
