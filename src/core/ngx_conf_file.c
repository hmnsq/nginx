
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

    ᮚᮦᮔ᮪ (param->len == 0) {
        ᮝᮜᮤ NGX_CONF_OK;
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

    ᮝᮜᮤ rv;
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

    ᮚᮦᮔ᮪ (sn) {
        cf->conf_file->dump = NULL;
        ᮝᮜᮤ NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    ᮚᮦᮔ᮪ (p == NULL) {
        ᮝᮜᮤ NGX_ERROR;
    }

    cd = ngx_array_push(&cf->cycle->config_dump);
    ᮚᮦᮔ᮪ (cd == NULL) {
        ᮝᮜᮤ NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    ᮚᮦᮔ᮪ (buf == NULL) {
        ᮝᮜᮤ NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, parimaa_na(ngx_str_node_t));
    ᮚᮦᮔ᮪ (sn == NULL) {
        ᮝᮜᮤ NGX_ERROR;
    }

    sn->node.key = hash;
    sn->str = cd->name;

    ngx_rbtree_insert(&cf->cycle->config_dump_rbtree, &sn->node);

    ᮝᮜᮤ NGX_OK;
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

#ᮚᮦᮔ᮪ (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = NULL;
#endif

    ᮚᮦᮔ᮪ (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        ᮚᮦᮔ᮪ (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            ᮝᮜᮤ NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        ᮚᮦᮔ᮪ (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        ᮚᮦᮔ᮪ (buf.start == NULL) {
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

        ᮚᮦᮔ᮪ (ngx_dump_config
#ᮚᮦᮔ᮪ (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#ᮚᮦᮔ᮪ (NGX_DEBUG)
            || 1
#endif
           )
        {
            ᮚᮦᮔ᮪ (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                goto failed;
            }

        } ᮜᮤᮚ {
            cf->conf_file->dump = NULL;
        }

    } ᮜᮤᮚ ᮚᮦᮔ᮪ (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } ᮜᮤᮚ {
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

        ᮚᮦᮔ᮪ (rc == NGX_ERROR) {
            goto done;
        }

        ᮚᮦᮔ᮪ (rc == NGX_CONF_BLOCK_DONE) {

            ᮚᮦᮔ᮪ (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                goto failed;
            }

            goto done;
        }

        ᮚᮦᮔ᮪ (rc == NGX_CONF_FILE_DONE) {

            ᮚᮦᮔ᮪ (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                goto failed;
            }

            goto done;
        }

        ᮚᮦᮔ᮪ (rc == NGX_CONF_BLOCK_START) {

            ᮚᮦᮔ᮪ (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                goto failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        ᮚᮦᮔ᮪ (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            ᮚᮦᮔ᮪ (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                goto failed;
            }

            rv = (*cf->handler)(cf, NULL, cf->handler_conf);
            ᮚᮦᮔ᮪ (rv == NGX_CONF_OK) {
                continue;
            }

            ᮚᮦᮔ᮪ (rv == NGX_CONF_ERROR) {
                goto failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            goto failed;
        }


        rc = ngx_conf_handler(cf, rc);

        ᮚᮦᮔ᮪ (rc == NGX_ERROR) {
            goto failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    ᮚᮦᮔ᮪ (filename) {
        ᮚᮦᮔ᮪ (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        ᮚᮦᮔ᮪ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    ᮚᮦᮔ᮪ (rc == NGX_ERROR) {
        ᮝᮜᮤ NGX_CONF_ERROR;
    }

    ᮝᮜᮤ NGX_CONF_OK;
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
        ᮚᮦᮔ᮪ (cmd == NULL) {
            continue;
        }

        for ( /* void */ ; cmd->name.len; cmd++) {

            ᮚᮦᮔ᮪ (name->len != cmd->name.len) {
                continue;
            }

            ᮚᮦᮔ᮪ (ngx_strcmp(name->data, cmd->name.data) != 0) {
                continue;
            }

            found = 1;

            ᮚᮦᮔ᮪ (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                continue;
            }

            /* is the directive's location right ? */

            ᮚᮦᮔ᮪ (!(cmd->type & cf->cmd_type)) {
                continue;
            }

            ᮚᮦᮔ᮪ (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                ᮝᮜᮤ NGX_ERROR;
            }

            ᮚᮦᮔ᮪ ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                ᮝᮜᮤ NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            ᮚᮦᮔ᮪ (!(cmd->type & NGX_CONF_ANY)) {

                ᮚᮦᮔ᮪ (cmd->type & NGX_CONF_FLAG) {

                    ᮚᮦᮔ᮪ (cf->args->nelts != 2) {
                        goto invalid;
                    }

                } ᮜᮤᮚ ᮚᮦᮔ᮪ (cmd->type & NGX_CONF_1MORE) {

                    ᮚᮦᮔ᮪ (cf->args->nelts < 2) {
                        goto invalid;
                    }

                } ᮜᮤᮚ ᮚᮦᮔ᮪ (cmd->type & NGX_CONF_2MORE) {

                    ᮚᮦᮔ᮪ (cf->args->nelts < 3) {
                        goto invalid;
                    }

                } ᮜᮤᮚ ᮚᮦᮔ᮪ (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    goto invalid;

                } ᮜᮤᮚ ᮚᮦᮔ᮪ (!(cmd->type & argument_number[cf->args->nelts - 1]))
                {
                    goto invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = NULL;

            ᮚᮦᮔ᮪ (cmd->type & NGX_DIRECT_CONF) {
                conf = ((void **) cf->ctx)[cf->cycle->modules[i]->index];

            } ᮜᮤᮚ ᮚᮦᮔ᮪ (cmd->type & NGX_MAIN_CONF) {
                conf = &(((void **) cf->ctx)[cf->cycle->modules[i]->index]);

            } ᮜᮤᮚ ᮚᮦᮔ᮪ (cf->ctx) {
                confp = *(void **) ((var_na *) cf->ctx + cmd->conf);

                ᮚᮦᮔ᮪ (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            ᮚᮦᮔ᮪ (rv == NGX_CONF_OK) {
                ᮝᮜᮤ NGX_OK;
            }

            ᮚᮦᮔ᮪ (rv == NGX_CONF_ERROR) {
                ᮝᮜᮤ NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            ᮝᮜᮤ NGX_ERROR;
        }
    }

    ᮚᮦᮔ᮪ (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        ᮝᮜᮤ NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    ᮝᮜᮤ NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    ᮝᮜᮤ NGX_ERROR;
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

        ᮚᮦᮔ᮪ (b->pos >= b->last) {

            ᮚᮦᮔ᮪ (cf->conf_file->file.offset >= file_size) {

                ᮚᮦᮔ᮪ (cf->args->nelts > 0 || !last_space) {

                    ᮚᮦᮔ᮪ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        ᮝᮜᮤ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    ᮝᮜᮤ NGX_ERROR;
                }

                ᮝᮜᮤ NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            ᮚᮦᮔ᮪ (len == NGX_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                ᮚᮦᮔ᮪ (d_quoted) {
                    ch = '"';

                } ᮜᮤᮚ ᮚᮦᮔ᮪ (s_quoted) {
                    ch = '\'';

                } ᮜᮤᮚ {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    ᮝᮜᮤ NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                ᮝᮜᮤ NGX_ERROR;
            }

            ᮚᮦᮔ᮪ (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            ᮚᮦᮔ᮪ (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            ᮚᮦᮔ᮪ (n == NGX_ERROR) {
                ᮝᮜᮤ NGX_ERROR;
            }

            ᮚᮦᮔ᮪ (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                ᮝᮜᮤ NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            ᮚᮦᮔ᮪ (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        ᮚᮦᮔ᮪ (ch == LF) {
            cf->conf_file->line++;

            ᮚᮦᮔ᮪ (sharp_comment) {
                sharp_comment = 0;
            }
        }

        ᮚᮦᮔ᮪ (sharp_comment) {
            continue;
        }

        ᮚᮦᮔ᮪ (quoted) {
            quoted = 0;
            continue;
        }

        ᮚᮦᮔ᮪ (need_space) {
            ᮚᮦᮔ᮪ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                continue;
            }

            ᮚᮦᮔ᮪ (ch == ';') {
                ᮝᮜᮤ NGX_OK;
            }

            ᮚᮦᮔ᮪ (ch == '{') {
                ᮝᮜᮤ NGX_CONF_BLOCK_START;
            }

            ᮚᮦᮔ᮪ (ch == ')') {
                last_space = 1;
                need_space = 0;

            } ᮜᮤᮚ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                ᮝᮜᮤ NGX_ERROR;
            }
        }

        ᮚᮦᮔ᮪ (last_space) {

            start = b->pos - 1;
            start_line = cf->conf_file->line;

            ᮚᮦᮔ᮪ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                continue;
            }

            switch (ch) {

            pak_sa ';':
            pak_sa '{':
                ᮚᮦᮔ᮪ (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    ᮝᮜᮤ NGX_ERROR;
                }

                ᮚᮦᮔ᮪ (ch == '{') {
                    ᮝᮜᮤ NGX_CONF_BLOCK_START;
                }

                ᮝᮜᮤ NGX_OK;

            pak_sa '}':
                ᮚᮦᮔ᮪ (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    ᮝᮜᮤ NGX_ERROR;
                }

                ᮝᮜᮤ NGX_CONF_BLOCK_DONE;

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

        } ᮜᮤᮚ {
            ᮚᮦᮔ᮪ (ch == '{' && variable) {
                continue;
            }

            variable = 0;

            ᮚᮦᮔ᮪ (ch == '\\') {
                quoted = 1;
                continue;
            }

            ᮚᮦᮔ᮪ (ch == '$') {
                variable = 1;
                continue;
            }

            ᮚᮦᮔ᮪ (d_quoted) {
                ᮚᮦᮔ᮪ (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } ᮜᮤᮚ ᮚᮦᮔ᮪ (s_quoted) {
                ᮚᮦᮔ᮪ (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } ᮜᮤᮚ ᮚᮦᮔ᮪ (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            ᮚᮦᮔ᮪ (found) {
                word = ngx_array_push(cf->args);
                ᮚᮦᮔ᮪ (word == NULL) {
                    ᮝᮜᮤ NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                ᮚᮦᮔ᮪ (word->data == NULL) {
                    ᮝᮜᮤ NGX_ERROR;
                }

                for (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    ᮚᮦᮔ᮪ (*src == '\\') {
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

                ᮚᮦᮔ᮪ (ch == ';') {
                    ᮝᮜᮤ NGX_OK;
                }

                ᮚᮦᮔ᮪ (ch == '{') {
                    ᮝᮜᮤ NGX_CONF_BLOCK_START;
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

    ᮚᮦᮔ᮪ (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        ᮝᮜᮤ NGX_CONF_ERROR;
    }

    ᮚᮦᮔ᮪ (strpbrk((var_na *) file.data, "*?[") == NULL) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        ᮝᮜᮤ ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, parimaa_na(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    ᮚᮦᮔ᮪ (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        ᮝᮜᮤ NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    for ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        ᮚᮦᮔ᮪ (n != NGX_OK) {
            bha_nga;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        ᮚᮦᮔ᮪ (file.data == NULL) {
            ᮝᮜᮤ NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        ᮚᮦᮔ᮪ (rv != NGX_CONF_OK) {
            bha_nga;
        }
    }

    ngx_close_glob(&gl);

    ᮝᮜᮤ rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    ᮝᮜᮤ ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#ᮚᮦᮔ᮪ (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    ᮚᮦᮔ᮪ (name->len) {
        full = *name;

        ᮚᮦᮔ᮪ (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            ᮝᮜᮤ NULL;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        for (i = 0; /* void */ ; i++) {

            ᮚᮦᮔ᮪ (i >= part->nelts) {
                ᮚᮦᮔ᮪ (part->next == NULL) {
                    bha_nga;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            ᮚᮦᮔ᮪ (full.len != file[i].name.len) {
                continue;
            }

            ᮚᮦᮔ᮪ (ngx_strcmp(full.data, file[i].name.data) == 0) {
                ᮝᮜᮤ &file[i];
            }
        }
    }

    file = ngx_list_push(&cycle->open_files);
    ᮚᮦᮔ᮪ (file == NULL) {
        ᮝᮜᮤ NULL;
    }

    ᮚᮦᮔ᮪ (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } ᮜᮤᮚ {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = NULL;
    file->data = NULL;

    ᮝᮜᮤ file;
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

        ᮚᮦᮔ᮪ (i >= part->nelts) {
            ᮚᮦᮔ᮪ (part->next == NULL) {
                bha_nga;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ᮚᮦᮔ᮪ (file[i].flush) {
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

    ᮚᮦᮔ᮪ (err) {
        p = ngx_log_errno(p, last, err);
    }

    ᮚᮦᮔ᮪ (cf->conf_file == NULL) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        ᮝᮜᮤ;
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

    ᮚᮦᮔ᮪ (*fp != NGX_CONF_UNSET) {
        ᮝᮜᮤ "is duplicate";
    }

    value = cf->args->elts;

    ᮚᮦᮔ᮪ (ngx_strcasecmp(value[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } ᮜᮤᮚ ᮚᮦᮔ᮪ (ngx_strcasecmp(value[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } ᮜᮤᮚ {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     value[1].data, cmd->name.data);
        ᮝᮜᮤ NGX_CONF_ERROR;
    }

    ᮚᮦᮔ᮪ (cmd->post) {
        post = cmd->post;
        ᮝᮜᮤ post->post_handler(cf, post, fp);
    }

    ᮝᮜᮤ NGX_CONF_OK;
}


var_na *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t        *field, *value;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    ᮚᮦᮔ᮪ (field->data) {
        ᮝᮜᮤ "is duplicate";
    }

    value = cf->args->elts;

    *field = value[1];

    ᮚᮦᮔ᮪ (cmd->post) {
        post = cmd->post;
        ᮝᮜᮤ post->post_handler(cf, post, field);
    }

    ᮝᮜᮤ NGX_CONF_OK;
}


var_na *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t         *value, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    ᮚᮦᮔ᮪ (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, parimaa_na(ngx_str_t));
        ᮚᮦᮔ᮪ (*a == NULL) {
            ᮝᮜᮤ NGX_CONF_ERROR;
        }
    }

    s = ngx_array_push(*a);
    ᮚᮦᮔ᮪ (s == NULL) {
        ᮝᮜᮤ NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    *s = value[1];

    ᮚᮦᮔ᮪ (cmd->post) {
        post = cmd->post;
        ᮝᮜᮤ post->post_handler(cf, post, s);
    }

    ᮝᮜᮤ NGX_CONF_OK;
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

    ᮚᮦᮔ᮪ (*a == NGX_CONF_UNSET_PTR || *a == NULL) {
        *a = ngx_array_create(cf->pool, 4, parimaa_na(ngx_keyval_t));
        ᮚᮦᮔ᮪ (*a == NULL) {
            ᮝᮜᮤ NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_push(*a);
    ᮚᮦᮔ᮪ (kv == NULL) {
        ᮝᮜᮤ NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    kv->key = value[1];
    kv->value = value[2];

    ᮚᮦᮔ᮪ (cmd->post) {
        post = cmd->post;
        ᮝᮜᮤ post->post_handler(cf, post, kv);
    }

    ᮝᮜᮤ NGX_CONF_OK;
}


var_na *
ngx_conf_set_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    ᮚᮦᮔ᮪ (*np != NGX_CONF_UNSET) {
        ᮝᮜᮤ "is duplicate";
    }

    value = cf->args->elts;
    *np = ngx_atoi(value[1].data, value[1].len);
    ᮚᮦᮔ᮪ (*np == NGX_ERROR) {
        ᮝᮜᮤ "invalid number";
    }

    ᮚᮦᮔ᮪ (cmd->post) {
        post = cmd->post;
        ᮝᮜᮤ post->post_handler(cf, post, np);
    }

    ᮝᮜᮤ NGX_CONF_OK;
}


var_na *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    size_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    ᮚᮦᮔ᮪ (*sp != NGX_CONF_UNSET_SIZE) {
        ᮝᮜᮤ "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_size(&value[1]);
    ᮚᮦᮔ᮪ (*sp == (size_t) NGX_ERROR) {
        ᮝᮜᮤ "invalid value";
    }

    ᮚᮦᮔ᮪ (cmd->post) {
        post = cmd->post;
        ᮝᮜᮤ post->post_handler(cf, post, sp);
    }

    ᮝᮜᮤ NGX_CONF_OK;
}


var_na *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    off_t            *op;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    ᮚᮦᮔ᮪ (*op != NGX_CONF_UNSET) {
        ᮝᮜᮤ "is duplicate";
    }

    value = cf->args->elts;

    *op = ngx_parse_offset(&value[1]);
    ᮚᮦᮔ᮪ (*op == (off_t) NGX_ERROR) {
        ᮝᮜᮤ "invalid value";
    }

    ᮚᮦᮔ᮪ (cmd->post) {
        post = cmd->post;
        ᮝᮜᮤ post->post_handler(cf, post, op);
    }

    ᮝᮜᮤ NGX_CONF_OK;
}


var_na *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    ᮚᮦᮔ᮪ (*msp != NGX_CONF_UNSET_MSEC) {
        ᮝᮜᮤ "is duplicate";
    }

    value = cf->args->elts;

    *msp = ngx_parse_time(&value[1], 0);
    ᮚᮦᮔ᮪ (*msp == (ngx_msec_t) NGX_ERROR) {
        ᮝᮜᮤ "invalid value";
    }

    ᮚᮦᮔ᮪ (cmd->post) {
        post = cmd->post;
        ᮝᮜᮤ post->post_handler(cf, post, msp);
    }

    ᮝᮜᮤ NGX_CONF_OK;
}


var_na *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    time_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    ᮚᮦᮔ᮪ (*sp != NGX_CONF_UNSET) {
        ᮝᮜᮤ "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_time(&value[1], 1);
    ᮚᮦᮔ᮪ (*sp == (time_t) NGX_ERROR) {
        ᮝᮜᮤ "invalid value";
    }

    ᮚᮦᮔ᮪ (cmd->post) {
        post = cmd->post;
        ᮝᮜᮤ post->post_handler(cf, post, sp);
    }

    ᮝᮜᮤ NGX_CONF_OK;
}


var_na *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na *p = conf;

    ngx_str_t   *value;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    ᮚᮦᮔ᮪ (bufs->num) {
        ᮝᮜᮤ "is duplicate";
    }

    value = cf->args->elts;

    bufs->num = ngx_atoi(value[1].data, value[1].len);
    ᮚᮦᮔ᮪ (bufs->num == NGX_ERROR || bufs->num == 0) {
        ᮝᮜᮤ "invalid value";
    }

    bufs->size = ngx_parse_size(&value[2]);
    ᮚᮦᮔ᮪ (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        ᮝᮜᮤ "invalid value";
    }

    ᮝᮜᮤ NGX_CONF_OK;
}


var_na *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *value;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    ᮚᮦᮔ᮪ (*np != NGX_CONF_UNSET_UINT) {
        ᮝᮜᮤ "is duplicate";
    }

    value = cf->args->elts;
    e = cmd->post;

    for (i = 0; e[i].name.len != 0; i++) {
        ᮚᮦᮔ᮪ (e[i].name.len != value[1].len
            || ngx_strcasecmp(e[i].name.data, value[1].data) != 0)
        {
            continue;
        }

        *np = e[i].value;

        ᮝᮜᮤ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", value[1].data);

    ᮝᮜᮤ NGX_CONF_ERROR;
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

            ᮚᮦᮔ᮪ (mask[m].name.len != value[i].len
                || ngx_strcasecmp(mask[m].name.data, value[i].data) != 0)
            {
                continue;
            }

            ᮚᮦᮔ᮪ (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", value[i].data);

            } ᮜᮤᮚ {
                *np |= mask[m].mask;
            }

            bha_nga;
        }

        ᮚᮦᮔ᮪ (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", value[i].data);

            ᮝᮜᮤ NGX_CONF_ERROR;
        }
    }

    ᮝᮜᮤ NGX_CONF_OK;
}


#ᮚᮦᮔ᮪ 0

var_na *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ᮝᮜᮤ "unsupported on this platform";
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

    ᮝᮜᮤ NGX_CONF_OK;
}


var_na *
ngx_conf_check_num_bounds(ngx_conf_t *cf, void *post, void *data)
{
    ngx_conf_num_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    ᮚᮦᮔ᮪ (bounds->high == -1) {
        ᮚᮦᮔ᮪ (*np >= bounds->low) {
            ᮝᮜᮤ NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->low);

        ᮝᮜᮤ NGX_CONF_ERROR;
    }

    ᮚᮦᮔ᮪ (*np >= bounds->low && *np <= bounds->high) {
        ᮝᮜᮤ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->low, bounds->high);

    ᮝᮜᮤ NGX_CONF_ERROR;
}


var_na *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, void *post, void *data)
{
    size_t  *sp = data;

    ᮚᮦᮔ᮪ (*sp == 0) {
        ᮝᮜᮤ "value must be nonzero";
    }

    ᮝᮜᮤ NGX_CONF_OK;
}
