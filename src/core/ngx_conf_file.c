
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

    ᤀᤡ᤺ (param->len == 0) {
        ᤀᤡᤴ NGX_CONF_OK;
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

    ᤀᤡᤴ rv;
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

    ᤀᤡ᤺ (sn) {
        cf->conf_file->dump = NULL;
        ᤀᤡᤴ NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    ᤀᤡ᤺ (p == NULL) {
        ᤀᤡᤴ NGX_ERROR;
    }

    cd = ngx_array_push(&cf->cycle->config_dump);
    ᤀᤡ᤺ (cd == NULL) {
        ᤀᤡᤴ NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    ᤀᤡ᤺ (buf == NULL) {
        ᤀᤡᤴ NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, parimaa_na(ngx_str_node_t));
    ᤀᤡ᤺ (sn == NULL) {
        ᤀᤡᤴ NGX_ERROR;
    }

    sn->node.key = hash;
    sn->str = cd->name;

    ngx_rbtree_insert(&cf->cycle->config_dump_rbtree, &sn->node);

    ᤀᤡᤴ NGX_OK;
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

#ᤀᤡ᤺ (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = NULL;
#endif

    ᤀᤡ᤺ (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        ᤀᤡ᤺ (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            ᤀᤡᤴ NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        ᤀᤡ᤺ (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        ᤀᤡ᤺ (buf.start == NULL) {
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

        ᤀᤡ᤺ (ngx_dump_config
#ᤀᤡ᤺ (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#ᤀᤡ᤺ (NGX_DEBUG)
            || 1
#endif
           )
        {
            ᤀᤡ᤺ (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                goto failed;
            }

        } ᤀᤡ᤺ᤆᤡ {
            cf->conf_file->dump = NULL;
        }

    } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } ᤀᤡ᤺ᤆᤡ {
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

        ᤀᤡ᤺ (rc == NGX_ERROR) {
            goto done;
        }

        ᤀᤡ᤺ (rc == NGX_CONF_BLOCK_DONE) {

            ᤀᤡ᤺ (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                goto failed;
            }

            goto done;
        }

        ᤀᤡ᤺ (rc == NGX_CONF_FILE_DONE) {

            ᤀᤡ᤺ (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                goto failed;
            }

            goto done;
        }

        ᤀᤡ᤺ (rc == NGX_CONF_BLOCK_START) {

            ᤀᤡ᤺ (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                goto failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        ᤀᤡ᤺ (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            ᤀᤡ᤺ (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                goto failed;
            }

            rv = (*cf->handler)(cf, NULL, cf->handler_conf);
            ᤀᤡ᤺ (rv == NGX_CONF_OK) {
                continue;
            }

            ᤀᤡ᤺ (rv == NGX_CONF_ERROR) {
                goto failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            goto failed;
        }


        rc = ngx_conf_handler(cf, rc);

        ᤀᤡ᤺ (rc == NGX_ERROR) {
            goto failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    ᤀᤡ᤺ (filename) {
        ᤀᤡ᤺ (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        ᤀᤡ᤺ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    ᤀᤡ᤺ (rc == NGX_ERROR) {
        ᤀᤡᤴ NGX_CONF_ERROR;
    }

    ᤀᤡᤴ NGX_CONF_OK;
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
        ᤀᤡ᤺ (cmd == NULL) {
            continue;
        }

        for ( /* void */ ; cmd->name.len; cmd++) {

            ᤀᤡ᤺ (name->len != cmd->name.len) {
                continue;
            }

            ᤀᤡ᤺ (ngx_strcmp(name->data, cmd->name.data) != 0) {
                continue;
            }

            found = 1;

            ᤀᤡ᤺ (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                continue;
            }

            /* is the directive's location right ? */

            ᤀᤡ᤺ (!(cmd->type & cf->cmd_type)) {
                continue;
            }

            ᤀᤡ᤺ (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                ᤀᤡᤴ NGX_ERROR;
            }

            ᤀᤡ᤺ ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                ᤀᤡᤴ NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            ᤀᤡ᤺ (!(cmd->type & NGX_CONF_ANY)) {

                ᤀᤡ᤺ (cmd->type & NGX_CONF_FLAG) {

                    ᤀᤡ᤺ (cf->args->nelts != 2) {
                        goto invalid;
                    }

                } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (cmd->type & NGX_CONF_1MORE) {

                    ᤀᤡ᤺ (cf->args->nelts < 2) {
                        goto invalid;
                    }

                } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (cmd->type & NGX_CONF_2MORE) {

                    ᤀᤡ᤺ (cf->args->nelts < 3) {
                        goto invalid;
                    }

                } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    goto invalid;

                } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!(cmd->type & argument_number[cf->args->nelts - 1]))
                {
                    goto invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = NULL;

            ᤀᤡ᤺ (cmd->type & NGX_DIRECT_CONF) {
                conf = ((void **) cf->ctx)[cf->cycle->modules[i]->index];

            } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (cmd->type & NGX_MAIN_CONF) {
                conf = &(((void **) cf->ctx)[cf->cycle->modules[i]->index]);

            } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (cf->ctx) {
                confp = *(void **) ((var_na *) cf->ctx + cmd->conf);

                ᤀᤡ᤺ (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            ᤀᤡ᤺ (rv == NGX_CONF_OK) {
                ᤀᤡᤴ NGX_OK;
            }

            ᤀᤡ᤺ (rv == NGX_CONF_ERROR) {
                ᤀᤡᤴ NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            ᤀᤡᤴ NGX_ERROR;
        }
    }

    ᤀᤡ᤺ (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        ᤀᤡᤴ NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    ᤀᤡᤴ NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    ᤀᤡᤴ NGX_ERROR;
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

        ᤀᤡ᤺ (b->pos >= b->last) {

            ᤀᤡ᤺ (cf->conf_file->file.offset >= file_size) {

                ᤀᤡ᤺ (cf->args->nelts > 0 || !last_space) {

                    ᤀᤡ᤺ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        ᤀᤡᤴ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    ᤀᤡᤴ NGX_ERROR;
                }

                ᤀᤡᤴ NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            ᤀᤡ᤺ (len == NGX_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                ᤀᤡ᤺ (d_quoted) {
                    ch = '"';

                } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (s_quoted) {
                    ch = '\'';

                } ᤀᤡ᤺ᤆᤡ {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    ᤀᤡᤴ NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                ᤀᤡᤴ NGX_ERROR;
            }

            ᤀᤡ᤺ (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            ᤀᤡ᤺ (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            ᤀᤡ᤺ (n == NGX_ERROR) {
                ᤀᤡᤴ NGX_ERROR;
            }

            ᤀᤡ᤺ (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                ᤀᤡᤴ NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            ᤀᤡ᤺ (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        ᤀᤡ᤺ (ch == LF) {
            cf->conf_file->line++;

            ᤀᤡ᤺ (sharp_comment) {
                sharp_comment = 0;
            }
        }

        ᤀᤡ᤺ (sharp_comment) {
            continue;
        }

        ᤀᤡ᤺ (quoted) {
            quoted = 0;
            continue;
        }

        ᤀᤡ᤺ (need_space) {
            ᤀᤡ᤺ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                continue;
            }

            ᤀᤡ᤺ (ch == ';') {
                ᤀᤡᤴ NGX_OK;
            }

            ᤀᤡ᤺ (ch == '{') {
                ᤀᤡᤴ NGX_CONF_BLOCK_START;
            }

            ᤀᤡ᤺ (ch == ')') {
                last_space = 1;
                need_space = 0;

            } ᤀᤡ᤺ᤆᤡ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                ᤀᤡᤴ NGX_ERROR;
            }
        }

        ᤀᤡ᤺ (last_space) {

            start = b->pos - 1;
            start_line = cf->conf_file->line;

            ᤀᤡ᤺ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                continue;
            }

            switch (ch) {

            pak_sa ';':
            pak_sa '{':
                ᤀᤡ᤺ (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    ᤀᤡᤴ NGX_ERROR;
                }

                ᤀᤡ᤺ (ch == '{') {
                    ᤀᤡᤴ NGX_CONF_BLOCK_START;
                }

                ᤀᤡᤴ NGX_OK;

            pak_sa '}':
                ᤀᤡ᤺ (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    ᤀᤡᤴ NGX_ERROR;
                }

                ᤀᤡᤴ NGX_CONF_BLOCK_DONE;

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

        } ᤀᤡ᤺ᤆᤡ {
            ᤀᤡ᤺ (ch == '{' && variable) {
                continue;
            }

            variable = 0;

            ᤀᤡ᤺ (ch == '\\') {
                quoted = 1;
                continue;
            }

            ᤀᤡ᤺ (ch == '$') {
                variable = 1;
                continue;
            }

            ᤀᤡ᤺ (d_quoted) {
                ᤀᤡ᤺ (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (s_quoted) {
                ᤀᤡ᤺ (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            ᤀᤡ᤺ (found) {
                word = ngx_array_push(cf->args);
                ᤀᤡ᤺ (word == NULL) {
                    ᤀᤡᤴ NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                ᤀᤡ᤺ (word->data == NULL) {
                    ᤀᤡᤴ NGX_ERROR;
                }

                for (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    ᤀᤡ᤺ (*src == '\\') {
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

                ᤀᤡ᤺ (ch == ';') {
                    ᤀᤡᤴ NGX_OK;
                }

                ᤀᤡ᤺ (ch == '{') {
                    ᤀᤡᤴ NGX_CONF_BLOCK_START;
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

    ᤀᤡ᤺ (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        ᤀᤡᤴ NGX_CONF_ERROR;
    }

    ᤀᤡ᤺ (strpbrk((var_na *) file.data, "*?[") == NULL) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        ᤀᤡᤴ ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, parimaa_na(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    ᤀᤡ᤺ (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        ᤀᤡᤴ NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    for ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        ᤀᤡ᤺ (n != NGX_OK) {
            bha_nga;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        ᤀᤡ᤺ (file.data == NULL) {
            ᤀᤡᤴ NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        ᤀᤡ᤺ (rv != NGX_CONF_OK) {
            bha_nga;
        }
    }

    ngx_close_glob(&gl);

    ᤀᤡᤴ rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    ᤀᤡᤴ ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#ᤀᤡ᤺ (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    ᤀᤡ᤺ (name->len) {
        full = *name;

        ᤀᤡ᤺ (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            ᤀᤡᤴ NULL;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        for (i = 0; /* void */ ; i++) {

            ᤀᤡ᤺ (i >= part->nelts) {
                ᤀᤡ᤺ (part->next == NULL) {
                    bha_nga;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            ᤀᤡ᤺ (full.len != file[i].name.len) {
                continue;
            }

            ᤀᤡ᤺ (ngx_strcmp(full.data, file[i].name.data) == 0) {
                ᤀᤡᤴ &file[i];
            }
        }
    }

    file = ngx_list_push(&cycle->open_files);
    ᤀᤡ᤺ (file == NULL) {
        ᤀᤡᤴ NULL;
    }

    ᤀᤡ᤺ (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } ᤀᤡ᤺ᤆᤡ {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = NULL;
    file->data = NULL;

    ᤀᤡᤴ file;
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

        ᤀᤡ᤺ (i >= part->nelts) {
            ᤀᤡ᤺ (part->next == NULL) {
                bha_nga;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ᤀᤡ᤺ (file[i].flush) {
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

    ᤀᤡ᤺ (err) {
        p = ngx_log_errno(p, last, err);
    }

    ᤀᤡ᤺ (cf->conf_file == NULL) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        ᤀᤡᤴ;
    }

    ᤀᤡ᤺ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        ᤀᤡᤴ;
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

    ᤀᤡ᤺ (*fp != NGX_CONF_UNSET) {
        ᤀᤡᤴ "is duplicate";
    }

    value = cf->args->elts;

    ᤀᤡ᤺ (ngx_strcasecmp(value[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (ngx_strcasecmp(value[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } ᤀᤡ᤺ᤆᤡ {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     value[1].data, cmd->name.data);
        ᤀᤡᤴ NGX_CONF_ERROR;
    }

    ᤀᤡ᤺ (cmd->post) {
        post = cmd->post;
        ᤀᤡᤴ post->post_handler(cf, post, fp);
    }

    ᤀᤡᤴ NGX_CONF_OK;
}


var_na *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t        *field, *value;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    ᤀᤡ᤺ (field->data) {
        ᤀᤡᤴ "is duplicate";
    }

    value = cf->args->elts;

    *field = value[1];

    ᤀᤡ᤺ (cmd->post) {
        post = cmd->post;
        ᤀᤡᤴ post->post_handler(cf, post, field);
    }

    ᤀᤡᤴ NGX_CONF_OK;
}


var_na *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t         *value, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    ᤀᤡ᤺ (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, parimaa_na(ngx_str_t));
        ᤀᤡ᤺ (*a == NULL) {
            ᤀᤡᤴ NGX_CONF_ERROR;
        }
    }

    s = ngx_array_push(*a);
    ᤀᤡ᤺ (s == NULL) {
        ᤀᤡᤴ NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    *s = value[1];

    ᤀᤡ᤺ (cmd->post) {
        post = cmd->post;
        ᤀᤡᤴ post->post_handler(cf, post, s);
    }

    ᤀᤡᤴ NGX_CONF_OK;
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

    ᤀᤡ᤺ (*a == NGX_CONF_UNSET_PTR || *a == NULL) {
        *a = ngx_array_create(cf->pool, 4, parimaa_na(ngx_keyval_t));
        ᤀᤡ᤺ (*a == NULL) {
            ᤀᤡᤴ NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_push(*a);
    ᤀᤡ᤺ (kv == NULL) {
        ᤀᤡᤴ NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    kv->key = value[1];
    kv->value = value[2];

    ᤀᤡ᤺ (cmd->post) {
        post = cmd->post;
        ᤀᤡᤴ post->post_handler(cf, post, kv);
    }

    ᤀᤡᤴ NGX_CONF_OK;
}


var_na *
ngx_conf_set_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    ᤀᤡ᤺ (*np != NGX_CONF_UNSET) {
        ᤀᤡᤴ "is duplicate";
    }

    value = cf->args->elts;
    *np = ngx_atoi(value[1].data, value[1].len);
    ᤀᤡ᤺ (*np == NGX_ERROR) {
        ᤀᤡᤴ "invalid number";
    }

    ᤀᤡ᤺ (cmd->post) {
        post = cmd->post;
        ᤀᤡᤴ post->post_handler(cf, post, np);
    }

    ᤀᤡᤴ NGX_CONF_OK;
}


var_na *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    size_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    ᤀᤡ᤺ (*sp != NGX_CONF_UNSET_SIZE) {
        ᤀᤡᤴ "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_size(&value[1]);
    ᤀᤡ᤺ (*sp == (size_t) NGX_ERROR) {
        ᤀᤡᤴ "invalid value";
    }

    ᤀᤡ᤺ (cmd->post) {
        post = cmd->post;
        ᤀᤡᤴ post->post_handler(cf, post, sp);
    }

    ᤀᤡᤴ NGX_CONF_OK;
}


var_na *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    off_t            *op;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    ᤀᤡ᤺ (*op != NGX_CONF_UNSET) {
        ᤀᤡᤴ "is duplicate";
    }

    value = cf->args->elts;

    *op = ngx_parse_offset(&value[1]);
    ᤀᤡ᤺ (*op == (off_t) NGX_ERROR) {
        ᤀᤡᤴ "invalid value";
    }

    ᤀᤡ᤺ (cmd->post) {
        post = cmd->post;
        ᤀᤡᤴ post->post_handler(cf, post, op);
    }

    ᤀᤡᤴ NGX_CONF_OK;
}


var_na *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    ᤀᤡ᤺ (*msp != NGX_CONF_UNSET_MSEC) {
        ᤀᤡᤴ "is duplicate";
    }

    value = cf->args->elts;

    *msp = ngx_parse_time(&value[1], 0);
    ᤀᤡ᤺ (*msp == (ngx_msec_t) NGX_ERROR) {
        ᤀᤡᤴ "invalid value";
    }

    ᤀᤡ᤺ (cmd->post) {
        post = cmd->post;
        ᤀᤡᤴ post->post_handler(cf, post, msp);
    }

    ᤀᤡᤴ NGX_CONF_OK;
}


var_na *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    time_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    ᤀᤡ᤺ (*sp != NGX_CONF_UNSET) {
        ᤀᤡᤴ "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_time(&value[1], 1);
    ᤀᤡ᤺ (*sp == (time_t) NGX_ERROR) {
        ᤀᤡᤴ "invalid value";
    }

    ᤀᤡ᤺ (cmd->post) {
        post = cmd->post;
        ᤀᤡᤴ post->post_handler(cf, post, sp);
    }

    ᤀᤡᤴ NGX_CONF_OK;
}


var_na *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na *p = conf;

    ngx_str_t   *value;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    ᤀᤡ᤺ (bufs->num) {
        ᤀᤡᤴ "is duplicate";
    }

    value = cf->args->elts;

    bufs->num = ngx_atoi(value[1].data, value[1].len);
    ᤀᤡ᤺ (bufs->num == NGX_ERROR || bufs->num == 0) {
        ᤀᤡᤴ "invalid value";
    }

    bufs->size = ngx_parse_size(&value[2]);
    ᤀᤡ᤺ (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        ᤀᤡᤴ "invalid value";
    }

    ᤀᤡᤴ NGX_CONF_OK;
}


var_na *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *value;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    ᤀᤡ᤺ (*np != NGX_CONF_UNSET_UINT) {
        ᤀᤡᤴ "is duplicate";
    }

    value = cf->args->elts;
    e = cmd->post;

    for (i = 0; e[i].name.len != 0; i++) {
        ᤀᤡ᤺ (e[i].name.len != value[1].len
            || ngx_strcasecmp(e[i].name.data, value[1].data) != 0)
        {
            continue;
        }

        *np = e[i].value;

        ᤀᤡᤴ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", value[1].data);

    ᤀᤡᤴ NGX_CONF_ERROR;
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

            ᤀᤡ᤺ (mask[m].name.len != value[i].len
                || ngx_strcasecmp(mask[m].name.data, value[i].data) != 0)
            {
                continue;
            }

            ᤀᤡ᤺ (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", value[i].data);

            } ᤀᤡ᤺ᤆᤡ {
                *np |= mask[m].mask;
            }

            bha_nga;
        }

        ᤀᤡ᤺ (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", value[i].data);

            ᤀᤡᤴ NGX_CONF_ERROR;
        }
    }

    ᤀᤡᤴ NGX_CONF_OK;
}


#ᤀᤡ᤺ 0

var_na *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ᤀᤡᤴ "unsupported on this platform";
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

    ᤀᤡᤴ NGX_CONF_OK;
}


var_na *
ngx_conf_check_num_bounds(ngx_conf_t *cf, void *post, void *data)
{
    ngx_conf_num_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    ᤀᤡ᤺ (bounds->high == -1) {
        ᤀᤡ᤺ (*np >= bounds->low) {
            ᤀᤡᤴ NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->low);

        ᤀᤡᤴ NGX_CONF_ERROR;
    }

    ᤀᤡ᤺ (*np >= bounds->low && *np <= bounds->high) {
        ᤀᤡᤴ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->low, bounds->high);

    ᤀᤡᤴ NGX_CONF_ERROR;
}


var_na *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, void *post, void *data)
{
    size_t  *sp = data;

    ᤀᤡ᤺ (*sp == 0) {
        ᤀᤡᤴ "value must be nonzero";
    }

    ᤀᤡᤴ NGX_CONF_OK;
}
