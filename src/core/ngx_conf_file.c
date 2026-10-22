
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

    ຖ້າ (param->len == 0) {
        ສົ່ງຄືນ NGX_CONF_OK;
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

    ສົ່ງຄືນ rv;
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

    ຖ້າ (sn) {
        cf->conf_file->dump = NULL;
        ສົ່ງຄືນ NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    ຖ້າ (p == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    cd = ngx_array_push(&cf->cycle->config_dump);
    ຖ້າ (cd == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    ຖ້າ (buf == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, parimaa_na(ngx_str_node_t));
    ຖ້າ (sn == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    sn->node.key = hash;
    sn->str = cd->name;

    ngx_rbtree_insert(&cf->cycle->config_dump_rbtree, &sn->node);

    ສົ່ງຄືນ NGX_OK;
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

#ຖ້າ (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = NULL;
#endif

    ຖ້າ (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        ຖ້າ (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            ສົ່ງຄືນ NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        ຖ້າ (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        ຖ້າ (buf.start == NULL) {
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

        ຖ້າ (ngx_dump_config
#ຖ້າ (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#ຖ້າ (NGX_DEBUG)
            || 1
#endif
           )
        {
            ຖ້າ (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                goto failed;
            }

        } ບໍ່ດັ່ງນັ້ນ {
            cf->conf_file->dump = NULL;
        }

    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } ບໍ່ດັ່ງນັ້ນ {
        type = parse_param;
    }


    ສຳລັບ ( ;; ) {
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

        ຖ້າ (rc == NGX_ERROR) {
            goto done;
        }

        ຖ້າ (rc == NGX_CONF_BLOCK_DONE) {

            ຖ້າ (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                goto failed;
            }

            goto done;
        }

        ຖ້າ (rc == NGX_CONF_FILE_DONE) {

            ຖ້າ (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                goto failed;
            }

            goto done;
        }

        ຖ້າ (rc == NGX_CONF_BLOCK_START) {

            ຖ້າ (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                goto failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        ຖ້າ (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            ຖ້າ (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                goto failed;
            }

            rv = (*cf->handler)(cf, NULL, cf->handler_conf);
            ຖ້າ (rv == NGX_CONF_OK) {
                continue;
            }

            ຖ້າ (rv == NGX_CONF_ERROR) {
                goto failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            goto failed;
        }


        rc = ngx_conf_handler(cf, rc);

        ຖ້າ (rc == NGX_ERROR) {
            goto failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    ຖ້າ (filename) {
        ຖ້າ (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        ຖ້າ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    ຖ້າ (rc == NGX_ERROR) {
        ສົ່ງຄືນ NGX_CONF_ERROR;
    }

    ສົ່ງຄືນ NGX_CONF_OK;
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

    ສຳລັບ (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        ຖ້າ (cmd == NULL) {
            continue;
        }

        ສຳລັບ ( /* void */ ; cmd->name.len; cmd++) {

            ຖ້າ (name->len != cmd->name.len) {
                continue;
            }

            ຖ້າ (ngx_strcmp(name->data, cmd->name.data) != 0) {
                continue;
            }

            found = 1;

            ຖ້າ (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                continue;
            }

            /* is the directive's location right ? */

            ຖ້າ (!(cmd->type & cf->cmd_type)) {
                continue;
            }

            ຖ້າ (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                ສົ່ງຄືນ NGX_ERROR;
            }

            ຖ້າ ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                ສົ່ງຄືນ NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            ຖ້າ (!(cmd->type & NGX_CONF_ANY)) {

                ຖ້າ (cmd->type & NGX_CONF_FLAG) {

                    ຖ້າ (cf->args->nelts != 2) {
                        goto invalid;
                    }

                } ບໍ່ດັ່ງນັ້ນ ຖ້າ (cmd->type & NGX_CONF_1MORE) {

                    ຖ້າ (cf->args->nelts < 2) {
                        goto invalid;
                    }

                } ບໍ່ດັ່ງນັ້ນ ຖ້າ (cmd->type & NGX_CONF_2MORE) {

                    ຖ້າ (cf->args->nelts < 3) {
                        goto invalid;
                    }

                } ບໍ່ດັ່ງນັ້ນ ຖ້າ (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    goto invalid;

                } ບໍ່ດັ່ງນັ້ນ ຖ້າ (!(cmd->type & argument_number[cf->args->nelts - 1]))
                {
                    goto invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = NULL;

            ຖ້າ (cmd->type & NGX_DIRECT_CONF) {
                conf = ((void **) cf->ctx)[cf->cycle->modules[i]->index];

            } ບໍ່ດັ່ງນັ້ນ ຖ້າ (cmd->type & NGX_MAIN_CONF) {
                conf = &(((void **) cf->ctx)[cf->cycle->modules[i]->index]);

            } ບໍ່ດັ່ງນັ້ນ ຖ້າ (cf->ctx) {
                confp = *(void **) ((var_na *) cf->ctx + cmd->conf);

                ຖ້າ (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            ຖ້າ (rv == NGX_CONF_OK) {
                ສົ່ງຄືນ NGX_OK;
            }

            ຖ້າ (rv == NGX_CONF_ERROR) {
                ສົ່ງຄືນ NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            ສົ່ງຄືນ NGX_ERROR;
        }
    }

    ຖ້າ (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        ສົ່ງຄືນ NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    ສົ່ງຄືນ NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    ສົ່ງຄືນ NGX_ERROR;
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

    ສຳລັບ ( ;; ) {

        ຖ້າ (b->pos >= b->last) {

            ຖ້າ (cf->conf_file->file.offset >= file_size) {

                ຖ້າ (cf->args->nelts > 0 || !last_space) {

                    ຖ້າ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        ສົ່ງຄືນ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    ສົ່ງຄືນ NGX_ERROR;
                }

                ສົ່ງຄືນ NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            ຖ້າ (len == NGX_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                ຖ້າ (d_quoted) {
                    ch = '"';

                } ບໍ່ດັ່ງນັ້ນ ຖ້າ (s_quoted) {
                    ch = '\'';

                } ບໍ່ດັ່ງນັ້ນ {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    ສົ່ງຄືນ NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                ສົ່ງຄືນ NGX_ERROR;
            }

            ຖ້າ (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            ຖ້າ (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            ຖ້າ (n == NGX_ERROR) {
                ສົ່ງຄືນ NGX_ERROR;
            }

            ຖ້າ (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                ສົ່ງຄືນ NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            ຖ້າ (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        ຖ້າ (ch == LF) {
            cf->conf_file->line++;

            ຖ້າ (sharp_comment) {
                sharp_comment = 0;
            }
        }

        ຖ້າ (sharp_comment) {
            continue;
        }

        ຖ້າ (quoted) {
            quoted = 0;
            continue;
        }

        ຖ້າ (need_space) {
            ຖ້າ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                continue;
            }

            ຖ້າ (ch == ';') {
                ສົ່ງຄືນ NGX_OK;
            }

            ຖ້າ (ch == '{') {
                ສົ່ງຄືນ NGX_CONF_BLOCK_START;
            }

            ຖ້າ (ch == ')') {
                last_space = 1;
                need_space = 0;

            } ບໍ່ດັ່ງນັ້ນ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                ສົ່ງຄືນ NGX_ERROR;
            }
        }

        ຖ້າ (last_space) {

            start = b->pos - 1;
            start_line = cf->conf_file->line;

            ຖ້າ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                continue;
            }

            switch (ch) {

            pak_sa ';':
            pak_sa '{':
                ຖ້າ (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    ສົ່ງຄືນ NGX_ERROR;
                }

                ຖ້າ (ch == '{') {
                    ສົ່ງຄືນ NGX_CONF_BLOCK_START;
                }

                ສົ່ງຄືນ NGX_OK;

            pak_sa '}':
                ຖ້າ (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    ສົ່ງຄືນ NGX_ERROR;
                }

                ສົ່ງຄືນ NGX_CONF_BLOCK_DONE;

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

        } ບໍ່ດັ່ງນັ້ນ {
            ຖ້າ (ch == '{' && variable) {
                continue;
            }

            variable = 0;

            ຖ້າ (ch == '\\') {
                quoted = 1;
                continue;
            }

            ຖ້າ (ch == '$') {
                variable = 1;
                continue;
            }

            ຖ້າ (d_quoted) {
                ຖ້າ (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } ບໍ່ດັ່ງນັ້ນ ຖ້າ (s_quoted) {
                ຖ້າ (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } ບໍ່ດັ່ງນັ້ນ ຖ້າ (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            ຖ້າ (found) {
                word = ngx_array_push(cf->args);
                ຖ້າ (word == NULL) {
                    ສົ່ງຄືນ NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                ຖ້າ (word->data == NULL) {
                    ສົ່ງຄືນ NGX_ERROR;
                }

                ສຳລັບ (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    ຖ້າ (*src == '\\') {
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

                ຖ້າ (ch == ';') {
                    ສົ່ງຄືນ NGX_OK;
                }

                ຖ້າ (ch == '{') {
                    ສົ່ງຄືນ NGX_CONF_BLOCK_START;
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

    ຖ້າ (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        ສົ່ງຄືນ NGX_CONF_ERROR;
    }

    ຖ້າ (strpbrk((var_na *) file.data, "*?[") == NULL) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        ສົ່ງຄືນ ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, parimaa_na(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    ຖ້າ (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        ສົ່ງຄືນ NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    ສຳລັບ ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        ຖ້າ (n != NGX_OK) {
            bha_nga;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        ຖ້າ (file.data == NULL) {
            ສົ່ງຄືນ NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        ຖ້າ (rv != NGX_CONF_OK) {
            bha_nga;
        }
    }

    ngx_close_glob(&gl);

    ສົ່ງຄືນ rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    ສົ່ງຄືນ ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#ຖ້າ (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    ຖ້າ (name->len) {
        full = *name;

        ຖ້າ (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            ສົ່ງຄືນ NULL;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        ສຳລັບ (i = 0; /* void */ ; i++) {

            ຖ້າ (i >= part->nelts) {
                ຖ້າ (part->next == NULL) {
                    bha_nga;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            ຖ້າ (full.len != file[i].name.len) {
                continue;
            }

            ຖ້າ (ngx_strcmp(full.data, file[i].name.data) == 0) {
                ສົ່ງຄືນ &file[i];
            }
        }
    }

    file = ngx_list_push(&cycle->open_files);
    ຖ້າ (file == NULL) {
        ສົ່ງຄືນ NULL;
    }

    ຖ້າ (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } ບໍ່ດັ່ງນັ້ນ {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = NULL;
    file->data = NULL;

    ສົ່ງຄືນ file;
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

    ສຳລັບ (i = 0; /* void */ ; i++) {

        ຖ້າ (i >= part->nelts) {
            ຖ້າ (part->next == NULL) {
                bha_nga;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ຖ້າ (file[i].flush) {
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

    ຖ້າ (err) {
        p = ngx_log_errno(p, last, err);
    }

    ຖ້າ (cf->conf_file == NULL) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        ສົ່ງຄືນ;
    }

    ຖ້າ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        ສົ່ງຄືນ;
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

    ຖ້າ (*fp != NGX_CONF_UNSET) {
        ສົ່ງຄືນ "is duplicate";
    }

    value = cf->args->elts;

    ຖ້າ (ngx_strcasecmp(value[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (ngx_strcasecmp(value[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } ບໍ່ດັ່ງນັ້ນ {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     value[1].data, cmd->name.data);
        ສົ່ງຄືນ NGX_CONF_ERROR;
    }

    ຖ້າ (cmd->post) {
        post = cmd->post;
        ສົ່ງຄືນ post->post_handler(cf, post, fp);
    }

    ສົ່ງຄືນ NGX_CONF_OK;
}


var_na *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t        *field, *value;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    ຖ້າ (field->data) {
        ສົ່ງຄືນ "is duplicate";
    }

    value = cf->args->elts;

    *field = value[1];

    ຖ້າ (cmd->post) {
        post = cmd->post;
        ສົ່ງຄືນ post->post_handler(cf, post, field);
    }

    ສົ່ງຄືນ NGX_CONF_OK;
}


var_na *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t         *value, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    ຖ້າ (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, parimaa_na(ngx_str_t));
        ຖ້າ (*a == NULL) {
            ສົ່ງຄືນ NGX_CONF_ERROR;
        }
    }

    s = ngx_array_push(*a);
    ຖ້າ (s == NULL) {
        ສົ່ງຄືນ NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    *s = value[1];

    ຖ້າ (cmd->post) {
        post = cmd->post;
        ສົ່ງຄືນ post->post_handler(cf, post, s);
    }

    ສົ່ງຄືນ NGX_CONF_OK;
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

    ຖ້າ (*a == NGX_CONF_UNSET_PTR || *a == NULL) {
        *a = ngx_array_create(cf->pool, 4, parimaa_na(ngx_keyval_t));
        ຖ້າ (*a == NULL) {
            ສົ່ງຄືນ NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_push(*a);
    ຖ້າ (kv == NULL) {
        ສົ່ງຄືນ NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    kv->key = value[1];
    kv->value = value[2];

    ຖ້າ (cmd->post) {
        post = cmd->post;
        ສົ່ງຄືນ post->post_handler(cf, post, kv);
    }

    ສົ່ງຄືນ NGX_CONF_OK;
}


var_na *
ngx_conf_set_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    ຖ້າ (*np != NGX_CONF_UNSET) {
        ສົ່ງຄືນ "is duplicate";
    }

    value = cf->args->elts;
    *np = ngx_atoi(value[1].data, value[1].len);
    ຖ້າ (*np == NGX_ERROR) {
        ສົ່ງຄືນ "invalid number";
    }

    ຖ້າ (cmd->post) {
        post = cmd->post;
        ສົ່ງຄືນ post->post_handler(cf, post, np);
    }

    ສົ່ງຄືນ NGX_CONF_OK;
}


var_na *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    size_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    ຖ້າ (*sp != NGX_CONF_UNSET_SIZE) {
        ສົ່ງຄືນ "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_size(&value[1]);
    ຖ້າ (*sp == (size_t) NGX_ERROR) {
        ສົ່ງຄືນ "invalid value";
    }

    ຖ້າ (cmd->post) {
        post = cmd->post;
        ສົ່ງຄືນ post->post_handler(cf, post, sp);
    }

    ສົ່ງຄືນ NGX_CONF_OK;
}


var_na *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    off_t            *op;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    ຖ້າ (*op != NGX_CONF_UNSET) {
        ສົ່ງຄືນ "is duplicate";
    }

    value = cf->args->elts;

    *op = ngx_parse_offset(&value[1]);
    ຖ້າ (*op == (off_t) NGX_ERROR) {
        ສົ່ງຄືນ "invalid value";
    }

    ຖ້າ (cmd->post) {
        post = cmd->post;
        ສົ່ງຄືນ post->post_handler(cf, post, op);
    }

    ສົ່ງຄືນ NGX_CONF_OK;
}


var_na *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    ຖ້າ (*msp != NGX_CONF_UNSET_MSEC) {
        ສົ່ງຄືນ "is duplicate";
    }

    value = cf->args->elts;

    *msp = ngx_parse_time(&value[1], 0);
    ຖ້າ (*msp == (ngx_msec_t) NGX_ERROR) {
        ສົ່ງຄືນ "invalid value";
    }

    ຖ້າ (cmd->post) {
        post = cmd->post;
        ສົ່ງຄືນ post->post_handler(cf, post, msp);
    }

    ສົ່ງຄືນ NGX_CONF_OK;
}


var_na *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    time_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    ຖ້າ (*sp != NGX_CONF_UNSET) {
        ສົ່ງຄືນ "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_time(&value[1], 1);
    ຖ້າ (*sp == (time_t) NGX_ERROR) {
        ສົ່ງຄືນ "invalid value";
    }

    ຖ້າ (cmd->post) {
        post = cmd->post;
        ສົ່ງຄືນ post->post_handler(cf, post, sp);
    }

    ສົ່ງຄືນ NGX_CONF_OK;
}


var_na *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na *p = conf;

    ngx_str_t   *value;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    ຖ້າ (bufs->num) {
        ສົ່ງຄືນ "is duplicate";
    }

    value = cf->args->elts;

    bufs->num = ngx_atoi(value[1].data, value[1].len);
    ຖ້າ (bufs->num == NGX_ERROR || bufs->num == 0) {
        ສົ່ງຄືນ "invalid value";
    }

    bufs->size = ngx_parse_size(&value[2]);
    ຖ້າ (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        ສົ່ງຄືນ "invalid value";
    }

    ສົ່ງຄືນ NGX_CONF_OK;
}


var_na *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *value;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    ຖ້າ (*np != NGX_CONF_UNSET_UINT) {
        ສົ່ງຄືນ "is duplicate";
    }

    value = cf->args->elts;
    e = cmd->post;

    ສຳລັບ (i = 0; e[i].name.len != 0; i++) {
        ຖ້າ (e[i].name.len != value[1].len
            || ngx_strcasecmp(e[i].name.data, value[1].data) != 0)
        {
            continue;
        }

        *np = e[i].value;

        ສົ່ງຄືນ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", value[1].data);

    ສົ່ງຄືນ NGX_CONF_ERROR;
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

    ສຳລັບ (i = 1; i < cf->args->nelts; i++) {
        ສຳລັບ (m = 0; mask[m].name.len != 0; m++) {

            ຖ້າ (mask[m].name.len != value[i].len
                || ngx_strcasecmp(mask[m].name.data, value[i].data) != 0)
            {
                continue;
            }

            ຖ້າ (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", value[i].data);

            } ບໍ່ດັ່ງນັ້ນ {
                *np |= mask[m].mask;
            }

            bha_nga;
        }

        ຖ້າ (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", value[i].data);

            ສົ່ງຄືນ NGX_CONF_ERROR;
        }
    }

    ສົ່ງຄືນ NGX_CONF_OK;
}


#ຖ້າ 0

var_na *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ສົ່ງຄືນ "unsupported on this platform";
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

    ສົ່ງຄືນ NGX_CONF_OK;
}


var_na *
ngx_conf_check_num_bounds(ngx_conf_t *cf, void *post, void *data)
{
    ngx_conf_num_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    ຖ້າ (bounds->high == -1) {
        ຖ້າ (*np >= bounds->low) {
            ສົ່ງຄືນ NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->low);

        ສົ່ງຄືນ NGX_CONF_ERROR;
    }

    ຖ້າ (*np >= bounds->low && *np <= bounds->high) {
        ສົ່ງຄືນ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->low, bounds->high);

    ສົ່ງຄືນ NGX_CONF_ERROR;
}


var_na *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, void *post, void *data)
{
    size_t  *sp = data;

    ຖ້າ (*sp == 0) {
        ສົ່ງຄືນ "value must be nonzero";
    }

    ສົ່ງຄືນ NGX_CONF_OK;
}
