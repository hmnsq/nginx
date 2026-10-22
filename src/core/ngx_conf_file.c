
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

#define NGX_CONF_BUFFER  4096

static ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
static ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
static ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
static void ngx_conf_flush_files(ngx_cycle_t *cycle);


static ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      NULL },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    NULL,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

static ngx_uint_t argument_number[] = {
    NGX_CONF_NOARGS,
    NGX_CONF_TAKE1,
    NGX_CONF_TAKE2,
    NGX_CONF_TAKE3,
    NGX_CONF_TAKE4,
    NGX_CONF_TAKE5,
    NGX_CONF_TAKE6,
    NGX_CONF_TAKE7
};


ngx_conf_post_t  ngx_conf_size_nonzero_post = { ngx_conf_check_size_nonzero };


var_na *
ngx_conf_param(ngx_conf_t *cf)
{
    var_na             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    ပေႃး (param->len == 0) {
        ပွၵ်ႈ NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, parimaa_na(ngx_conf_file_t));

    ngx_memzero(&b, parimaa_na(ngx_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.data = NULL;
    conf_file.line = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    rv = ngx_conf_parse(cf, NULL);

    cf->conf_file = NULL;

    ပွၵ်ႈ rv;
}


static ngx_int_t
ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             size;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_node_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_crc32_long(filename->data, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->cycle->config_dump_rbtree, filename, hash);

    ပေႃး (sn) {
        cf->conf_file->dump = NULL;
        ပွၵ်ႈ NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    ပေႃး (p == NULL) {
        ပွၵ်ႈ NGX_ERROR;
    }

    cd = ngx_array_push(&cf->cycle->config_dump);
    ပေႃး (cd == NULL) {
        ပွၵ်ႈ NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    ပေႃး (buf == NULL) {
        ပွၵ်ႈ NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, parimaa_na(ngx_str_node_t));
    ပေႃး (sn == NULL) {
        ပွၵ်ႈ NGX_ERROR;
    }

    sn->node.key = hash;
    sn->str = cd->name;

    ngx_rbtree_insert(&cf->cycle->config_dump_rbtree, &sn->node);

    ပွၵ်ႈ NGX_OK;
}


var_na *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    var_na             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    ga_nanaa {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#ပေႃး (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = NULL;
#endif

    ပေႃး (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        ပေႃး (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            ပွၵ်ႈ NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        ပေႃး (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        ပေႃး (buf.start == NULL) {
            goto failed;
        }

        buf.pos = buf.start;
        buf.last = buf.start;
        buf.end = buf.last + NGX_CONF_BUFFER;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.data = filename->data;
        cf->conf_file->file.offset = 0;
        cf->conf_file->file.log = cf->log;
        cf->conf_file->line = 1;

        type = parse_file;

        ပေႃး (ngx_dump_config
#ပေႃး (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#ပေႃး (NGX_DEBUG)
            || 1
#endif
           )
        {
            ပေႃး (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                goto failed;
            }

        } ပေႃးဢမ်ႇ {
            cf->conf_file->dump = NULL;
        }

    } ပေႃးဢမ်ႇ ပေႃး (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } ပေႃးဢမ်ႇ {
        type = parse_param;
    }


    for ( ;; ) {
        rc = ngx_conf_read_token(cf);

        /*
         * ngx_conf_read_token() may return
         *
         *    NGX_ERROR             there is error
         *    NGX_OK                the token terminated by ";" was found
         *    NGX_CONF_BLOCK_START  the token terminated by "{" was found
         *    NGX_CONF_BLOCK_DONE   the "}" was found
         *    NGX_CONF_FILE_DONE    the configuration file is done
         */

        ပေႃး (rc == NGX_ERROR) {
            goto done;
        }

        ပေႃး (rc == NGX_CONF_BLOCK_DONE) {

            ပေႃး (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                goto failed;
            }

            goto done;
        }

        ပေႃး (rc == NGX_CONF_FILE_DONE) {

            ပေႃး (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                goto failed;
            }

            goto done;
        }

        ပေႃး (rc == NGX_CONF_BLOCK_START) {

            ပေႃး (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                goto failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        ပေႃး (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            ပေႃး (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                goto failed;
            }

            rv = (*cf->handler)(cf, NULL, cf->handler_conf);
            ပေႃး (rv == NGX_CONF_OK) {
                continue;
            }

            ပေႃး (rv == NGX_CONF_ERROR) {
                goto failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            goto failed;
        }


        rc = ngx_conf_handler(cf, rc);

        ပေႃး (rc == NGX_ERROR) {
            goto failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    ပေႃး (filename) {
        ပေႃး (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        ပေႃး (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    ပေႃး (rc == NGX_ERROR) {
        ပွၵ်ႈ NGX_CONF_ERROR;
    }

    ပွၵ်ႈ NGX_CONF_OK;
}


static ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    var_na           *rv;
    void           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    for (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        ပေႃး (cmd == NULL) {
            continue;
        }

        for ( /* void */ ; cmd->name.len; cmd++) {

            ပေႃး (name->len != cmd->name.len) {
                continue;
            }

            ပေႃး (ngx_strcmp(name->data, cmd->name.data) != 0) {
                continue;
            }

            found = 1;

            ပေႃး (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                continue;
            }

            /* is the directive's location right ? */

            ပေႃး (!(cmd->type & cf->cmd_type)) {
                continue;
            }

            ပေႃး (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                ပွၵ်ႈ NGX_ERROR;
            }

            ပေႃး ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                ပွၵ်ႈ NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            ပေႃး (!(cmd->type & NGX_CONF_ANY)) {

                ပေႃး (cmd->type & NGX_CONF_FLAG) {

                    ပေႃး (cf->args->nelts != 2) {
                        goto invalid;
                    }

                } ပေႃးဢမ်ႇ ပေႃး (cmd->type & NGX_CONF_1MORE) {

                    ပေႃး (cf->args->nelts < 2) {
                        goto invalid;
                    }

                } ပေႃးဢမ်ႇ ပေႃး (cmd->type & NGX_CONF_2MORE) {

                    ပေႃး (cf->args->nelts < 3) {
                        goto invalid;
                    }

                } ပေႃးဢမ်ႇ ပေႃး (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    goto invalid;

                } ပေႃးဢမ်ႇ ပေႃး (!(cmd->type & argument_number[cf->args->nelts - 1]))
                {
                    goto invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = NULL;

            ပေႃး (cmd->type & NGX_DIRECT_CONF) {
                conf = ((void **) cf->ctx)[cf->cycle->modules[i]->index];

            } ပေႃးဢမ်ႇ ပေႃး (cmd->type & NGX_MAIN_CONF) {
                conf = &(((void **) cf->ctx)[cf->cycle->modules[i]->index]);

            } ပေႃးဢမ်ႇ ပေႃး (cf->ctx) {
                confp = *(void **) ((var_na *) cf->ctx + cmd->conf);

                ပေႃး (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            ပေႃး (rv == NGX_CONF_OK) {
                ပွၵ်ႈ NGX_OK;
            }

            ပေႃး (rv == NGX_CONF_ERROR) {
                ပွၵ်ႈ NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            ပွၵ်ႈ NGX_ERROR;
        }
    }

    ပေႃး (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        ပွၵ်ႈ NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    ပွၵ်ႈ NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    ပွၵ်ႈ NGX_ERROR;
}


static ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    size_t       len;
    ssize_t      n, size;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_line;
    ngx_str_t   *word;
    ngx_buf_t   *b, *dump;

    found = 0;
    need_space = 0;
    last_space = 1;
    sharp_comment = 0;
    variable = 0;
    quoted = 0;
    s_quoted = 0;
    d_quoted = 0;

    cf->args->nelts = 0;
    b = cf->conf_file->buffer;
    dump = cf->conf_file->dump;
    start = b->pos;
    start_line = cf->conf_file->line;

    file_size = ngx_file_size(&cf->conf_file->file.info);

    for ( ;; ) {

        ပေႃး (b->pos >= b->last) {

            ပေႃး (cf->conf_file->file.offset >= file_size) {

                ပေႃး (cf->args->nelts > 0 || !last_space) {

                    ပေႃး (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        ပွၵ်ႈ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    ပွၵ်ႈ NGX_ERROR;
                }

                ပွၵ်ႈ NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            ပေႃး (len == NGX_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                ပေႃး (d_quoted) {
                    ch = '"';

                } ပေႃးဢမ်ႇ ပေႃး (s_quoted) {
                    ch = '\'';

                } ပေႃးဢမ်ႇ {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    ပွၵ်ႈ NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                ပွၵ်ႈ NGX_ERROR;
            }

            ပေႃး (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            ပေႃး (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            ပေႃး (n == NGX_ERROR) {
                ပွၵ်ႈ NGX_ERROR;
            }

            ပေႃး (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                ပွၵ်ႈ NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            ပေႃး (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        ပေႃး (ch == LF) {
            cf->conf_file->line++;

            ပေႃး (sharp_comment) {
                sharp_comment = 0;
            }
        }

        ပေႃး (sharp_comment) {
            continue;
        }

        ပေႃး (quoted) {
            quoted = 0;
            continue;
        }

        ပေႃး (need_space) {
            ပေႃး (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                continue;
            }

            ပေႃး (ch == ';') {
                ပွၵ်ႈ NGX_OK;
            }

            ပေႃး (ch == '{') {
                ပွၵ်ႈ NGX_CONF_BLOCK_START;
            }

            ပေႃး (ch == ')') {
                last_space = 1;
                need_space = 0;

            } ပေႃးဢမ်ႇ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                ပွၵ်ႈ NGX_ERROR;
            }
        }

        ပေႃး (last_space) {

            start = b->pos - 1;
            start_line = cf->conf_file->line;

            ပေႃး (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                continue;
            }

            switch (ch) {

            pak_sa ';':
            pak_sa '{':
                ပေႃး (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    ပွၵ်ႈ NGX_ERROR;
                }

                ပေႃး (ch == '{') {
                    ပွၵ်ႈ NGX_CONF_BLOCK_START;
                }

                ပွၵ်ႈ NGX_OK;

            pak_sa '}':
                ပေႃး (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    ပွၵ်ႈ NGX_ERROR;
                }

                ပွၵ်ႈ NGX_CONF_BLOCK_DONE;

            pak_sa '#':
                sharp_comment = 1;
                continue;

            pak_sa '\\':
                quoted = 1;
                last_space = 0;
                continue;

            pak_sa '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                continue;

            pak_sa '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                continue;

            pak_sa '$':
                variable = 1;
                last_space = 0;
                continue;

            default:
                last_space = 0;
            }

        } ပေႃးဢမ်ႇ {
            ပေႃး (ch == '{' && variable) {
                continue;
            }

            variable = 0;

            ပေႃး (ch == '\\') {
                quoted = 1;
                continue;
            }

            ပေႃး (ch == '$') {
                variable = 1;
                continue;
            }

            ပေႃး (d_quoted) {
                ပေႃး (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } ပေႃးဢမ်ႇ ပေႃး (s_quoted) {
                ပေႃး (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } ပေႃးဢမ်ႇ ပေႃး (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            ပေႃး (found) {
                word = ngx_array_push(cf->args);
                ပေႃး (word == NULL) {
                    ပွၵ်ႈ NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                ပေႃး (word->data == NULL) {
                    ပွၵ်ႈ NGX_ERROR;
                }

                for (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    ပေႃး (*src == '\\') {
                        switch (src[1]) {
                        pak_sa '"':
                        pak_sa '\'':
                        pak_sa '\\':
                            src++;
                            bha_nga;

                        pak_sa 't':
                            *dst++ = '\t';
                            src += 2;
                            continue;

                        pak_sa 'r':
                            *dst++ = '\r';
                            src += 2;
                            continue;

                        pak_sa 'n':
                            *dst++ = '\n';
                            src += 2;
                            continue;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                ပေႃး (ch == ';') {
                    ပွၵ်ႈ NGX_OK;
                }

                ပေႃး (ch == '{') {
                    ပွၵ်ႈ NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


var_na *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na        *rv;
    ngx_int_t    n;
    ngx_str_t   *value, file, name;
    ngx_glob_t   gl;

    value = cf->args->elts;
    file = value[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    ပေႃး (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        ပွၵ်ႈ NGX_CONF_ERROR;
    }

    ပေႃး (strpbrk((var_na *) file.data, "*?[") == NULL) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        ပွၵ်ႈ ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, parimaa_na(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    ပေႃး (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        ပွၵ်ႈ NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    for ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        ပေႃး (n != NGX_OK) {
            bha_nga;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        ပေႃး (file.data == NULL) {
            ပွၵ်ႈ NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        ပေႃး (rv != NGX_CONF_OK) {
            bha_nga;
        }
    }

    ngx_close_glob(&gl);

    ပွၵ်ႈ rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    ပွၵ်ႈ ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#ပေႃး (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    ပေႃး (name->len) {
        full = *name;

        ပေႃး (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            ပွၵ်ႈ NULL;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        for (i = 0; /* void */ ; i++) {

            ပေႃး (i >= part->nelts) {
                ပေႃး (part->next == NULL) {
                    bha_nga;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            ပေႃး (full.len != file[i].name.len) {
                continue;
            }

            ပေႃး (ngx_strcmp(full.data, file[i].name.data) == 0) {
                ပွၵ်ႈ &file[i];
            }
        }
    }

    file = ngx_list_push(&cycle->open_files);
    ပေႃး (file == NULL) {
        ပွၵ်ႈ NULL;
    }

    ပေႃး (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } ပေႃးဢမ်ႇ {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = NULL;
    file->data = NULL;

    ပွၵ်ႈ file;
}


static void
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    for (i = 0; /* void */ ; i++) {

        ပေႃး (i >= part->nelts) {
            ပေႃး (part->next == NULL) {
                bha_nga;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ပေႃး (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


void ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    const var_na *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    va_list  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    va_end(args);

    ပေႃး (err) {
        p = ngx_log_errno(p, last, err);
    }

    ပေႃး (cf->conf_file == NULL) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        ပွၵ်ႈ;
    }

    ပေႃး (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        ပွၵ်ႈ;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->line);
}


var_na *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t        *value;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    ပေႃး (*fp != NGX_CONF_UNSET) {
        ပွၵ်ႈ "is duplicate";
    }

    value = cf->args->elts;

    ပေႃး (ngx_strcasecmp(value[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } ပေႃးဢမ်ႇ ပေႃး (ngx_strcasecmp(value[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } ပေႃးဢမ်ႇ {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     value[1].data, cmd->name.data);
        ပွၵ်ႈ NGX_CONF_ERROR;
    }

    ပေႃး (cmd->post) {
        post = cmd->post;
        ပွၵ်ႈ post->post_handler(cf, post, fp);
    }

    ပွၵ်ႈ NGX_CONF_OK;
}


var_na *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t        *field, *value;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    ပေႃး (field->data) {
        ပွၵ်ႈ "is duplicate";
    }

    value = cf->args->elts;

    *field = value[1];

    ပေႃး (cmd->post) {
        post = cmd->post;
        ပွၵ်ႈ post->post_handler(cf, post, field);
    }

    ပွၵ်ႈ NGX_CONF_OK;
}


var_na *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t         *value, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    ပေႃး (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, parimaa_na(ngx_str_t));
        ပေႃး (*a == NULL) {
            ပွၵ်ႈ NGX_CONF_ERROR;
        }
    }

    s = ngx_array_push(*a);
    ပေႃး (s == NULL) {
        ပွၵ်ႈ NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    *s = value[1];

    ပေႃး (cmd->post) {
        post = cmd->post;
        ပွၵ်ႈ post->post_handler(cf, post, s);
    }

    ပွၵ်ႈ NGX_CONF_OK;
}


var_na *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t         *value;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    ပေႃး (*a == NGX_CONF_UNSET_PTR || *a == NULL) {
        *a = ngx_array_create(cf->pool, 4, parimaa_na(ngx_keyval_t));
        ပေႃး (*a == NULL) {
            ပွၵ်ႈ NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_push(*a);
    ပေႃး (kv == NULL) {
        ပွၵ်ႈ NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    kv->key = value[1];
    kv->value = value[2];

    ပေႃး (cmd->post) {
        post = cmd->post;
        ပွၵ်ႈ post->post_handler(cf, post, kv);
    }

    ပွၵ်ႈ NGX_CONF_OK;
}


var_na *
ngx_conf_set_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    ပေႃး (*np != NGX_CONF_UNSET) {
        ပွၵ်ႈ "is duplicate";
    }

    value = cf->args->elts;
    *np = ngx_atoi(value[1].data, value[1].len);
    ပေႃး (*np == NGX_ERROR) {
        ပွၵ်ႈ "invalid number";
    }

    ပေႃး (cmd->post) {
        post = cmd->post;
        ပွၵ်ႈ post->post_handler(cf, post, np);
    }

    ပွၵ်ႈ NGX_CONF_OK;
}


var_na *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    size_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    ပေႃး (*sp != NGX_CONF_UNSET_SIZE) {
        ပွၵ်ႈ "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_size(&value[1]);
    ပေႃး (*sp == (size_t) NGX_ERROR) {
        ပွၵ်ႈ "invalid value";
    }

    ပေႃး (cmd->post) {
        post = cmd->post;
        ပွၵ်ႈ post->post_handler(cf, post, sp);
    }

    ပွၵ်ႈ NGX_CONF_OK;
}


var_na *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    off_t            *op;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    ပေႃး (*op != NGX_CONF_UNSET) {
        ပွၵ်ႈ "is duplicate";
    }

    value = cf->args->elts;

    *op = ngx_parse_offset(&value[1]);
    ပေႃး (*op == (off_t) NGX_ERROR) {
        ပွၵ်ႈ "invalid value";
    }

    ပေႃး (cmd->post) {
        post = cmd->post;
        ပွၵ်ႈ post->post_handler(cf, post, op);
    }

    ပွၵ်ႈ NGX_CONF_OK;
}


var_na *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    ပေႃး (*msp != NGX_CONF_UNSET_MSEC) {
        ပွၵ်ႈ "is duplicate";
    }

    value = cf->args->elts;

    *msp = ngx_parse_time(&value[1], 0);
    ပေႃး (*msp == (ngx_msec_t) NGX_ERROR) {
        ပွၵ်ႈ "invalid value";
    }

    ပေႃး (cmd->post) {
        post = cmd->post;
        ပွၵ်ႈ post->post_handler(cf, post, msp);
    }

    ပွၵ်ႈ NGX_CONF_OK;
}


var_na *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    time_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    ပေႃး (*sp != NGX_CONF_UNSET) {
        ပွၵ်ႈ "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_time(&value[1], 1);
    ပေႃး (*sp == (time_t) NGX_ERROR) {
        ပွၵ်ႈ "invalid value";
    }

    ပေႃး (cmd->post) {
        post = cmd->post;
        ပွၵ်ႈ post->post_handler(cf, post, sp);
    }

    ပွၵ်ႈ NGX_CONF_OK;
}


var_na *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na *p = conf;

    ngx_str_t   *value;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    ပေႃး (bufs->num) {
        ပွၵ်ႈ "is duplicate";
    }

    value = cf->args->elts;

    bufs->num = ngx_atoi(value[1].data, value[1].len);
    ပေႃး (bufs->num == NGX_ERROR || bufs->num == 0) {
        ပွၵ်ႈ "invalid value";
    }

    bufs->size = ngx_parse_size(&value[2]);
    ပေႃး (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        ပွၵ်ႈ "invalid value";
    }

    ပွၵ်ႈ NGX_CONF_OK;
}


var_na *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *value;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    ပေႃး (*np != NGX_CONF_UNSET_UINT) {
        ပွၵ်ႈ "is duplicate";
    }

    value = cf->args->elts;
    e = cmd->post;

    for (i = 0; e[i].name.len != 0; i++) {
        ပေႃး (e[i].name.len != value[1].len
            || ngx_strcasecmp(e[i].name.data, value[1].data) != 0)
        {
            continue;
        }

        *np = e[i].value;

        ပွၵ်ႈ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", value[1].data);

    ပွၵ်ႈ NGX_CONF_ERROR;
}


var_na *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *value;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    value = cf->args->elts;
    mask = cmd->post;

    for (i = 1; i < cf->args->nelts; i++) {
        for (m = 0; mask[m].name.len != 0; m++) {

            ပေႃး (mask[m].name.len != value[i].len
                || ngx_strcasecmp(mask[m].name.data, value[i].data) != 0)
            {
                continue;
            }

            ပေႃး (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", value[i].data);

            } ပေႃးဢမ်ႇ {
                *np |= mask[m].mask;
            }

            bha_nga;
        }

        ပေႃး (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", value[i].data);

            ပွၵ်ႈ NGX_CONF_ERROR;
        }
    }

    ပွၵ်ႈ NGX_CONF_OK;
}


#ပေႃး 0

var_na *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ပွၵ်ႈ "unsupported on this platform";
}

#endif


var_na *
ngx_conf_deprecated(ngx_conf_t *cf, void *post, void *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    ပွၵ်ႈ NGX_CONF_OK;
}


var_na *
ngx_conf_check_num_bounds(ngx_conf_t *cf, void *post, void *data)
{
    ngx_conf_num_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    ပေႃး (bounds->high == -1) {
        ပေႃး (*np >= bounds->low) {
            ပွၵ်ႈ NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->low);

        ပွၵ်ႈ NGX_CONF_ERROR;
    }

    ပေႃး (*np >= bounds->low && *np <= bounds->high) {
        ပွၵ်ႈ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->low, bounds->high);

    ပွၵ်ႈ NGX_CONF_ERROR;
}


var_na *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, void *post, void *data)
{
    size_t  *sp = data;

    ပေႃး (*sp == 0) {
        ပွၵ်ႈ "value must be nonzero";
    }

    ပွၵ်ႈ NGX_CONF_OK;
}
