
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>

#define NGX_CONF_BUFFER  4096

ସ୍ଥିର ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
ସ୍ଥିର ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
ସ୍ଥିର ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
ସ୍ଥିର ଶୂନ୍ୟ ngx_conf_flush_files(ngx_cycle_t *cycle);


ସ୍ଥିର ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      ଖାଲି },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    ଖାଲି,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    ଖାଲି,                                  /* init master */
    ଖାଲି,                                  /* init module */
    ଖାଲି,                                  /* init process */
    ଖାଲି,                                  /* init thread */
    ଖାଲି,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    ଖାଲି,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

ସ୍ଥିର ngx_uint_t argument_ସଂଖ୍ୟା[] = {
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


ଅକ୍ଷର *
ngx_conf_param(ngx_conf_t *cf)
{
    ଅକ୍ଷର             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    ଯଦି (param->len == 0) {
        ଫେରନ୍ତୁ NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, ଆକାର(ngx_conf_file_t));

    ngx_memzero(&b, ଆକାର(ngx_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.data = ଖାଲି;
    conf_file.ଧାଡ଼ି = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    rv = ngx_conf_parse(cf, ଖାଲି);

    cf->conf_file = ଖାଲି;

    ଫେରନ୍ତୁ rv;
}


ସ୍ଥିର ngx_int_t
ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             size;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_ଗଣ୍ଠି_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_crc32_long(filename->data, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->cycle->config_dump_rbtree, filename, hash);

    ଯଦି (sn) {
        cf->conf_file->dump = ଖାଲି;
        ଫେରନ୍ତୁ NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    ଯଦି (p == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    cd = ngx_array_ଠେଲ(&cf->cycle->config_dump);
    ଯଦି (cd == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    ଯଦି (buf == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, ଆକାର(ngx_str_ଗଣ୍ଠି_t));
    ଯଦି (sn == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    sn->ଗଣ୍ଠି.ଚାବି = hash;
    sn->str = cd->name;

    ngx_rbtree_ଭର୍ତ୍ତି(&cf->cycle->config_dump_rbtree, &sn->ଗଣ୍ଠି);

    ଫେରନ୍ତୁ NGX_OK;
}


ଅକ୍ଷର *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    ଅକ୍ଷର             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    ଗଣନା {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#ଯଦି (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = ଖାଲି;
#endif

    ଯଦି (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        ଯଦି (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        ଯଦି (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        ଯଦି (buf.start == ଖାଲି) {
            ଯାଅ failed;
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
        cf->conf_file->ଧାଡ଼ି = 1;

        type = parse_file;

        ଯଦି (ngx_dump_config
#ଯଦି (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#ଯଦି (NGX_DEBUG)
            || 1
#endif
           )
        {
            ଯଦି (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                ଯାଅ failed;
            }

        } ଅନ୍ୟଥା {
            cf->conf_file->dump = ଖାଲି;
        }

    } ଅନ୍ୟଥା ଯଦି (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } ଅନ୍ୟଥା {
        type = parse_param;
    }


    ପାଇଁ ( ;; ) {
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

        ଯଦି (rc == NGX_ERROR) {
            ଯାଅ done;
        }

        ଯଦି (rc == NGX_CONF_BLOCK_DONE) {

            ଯଦି (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                ଯାଅ failed;
            }

            ଯାଅ done;
        }

        ଯଦି (rc == NGX_CONF_FILE_DONE) {

            ଯଦି (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                ଯାଅ failed;
            }

            ଯାଅ done;
        }

        ଯଦି (rc == NGX_CONF_BLOCK_START) {

            ଯଦି (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                ଯାଅ failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        ଯଦି (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            ଯଦି (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                ଯାଅ failed;
            }

            rv = (*cf->handler)(cf, ଖାଲି, cf->handler_conf);
            ଯଦି (rv == NGX_CONF_OK) {
                ଜାରି;
            }

            ଯଦି (rv == NGX_CONF_ERROR) {
                ଯାଅ failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            ଯାଅ failed;
        }


        rc = ngx_conf_handler(cf, rc);

        ଯଦି (rc == NGX_ERROR) {
            ଯାଅ failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    ଯଦି (filename) {
        ଯଦି (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        ଯଦି (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    ଯଦି (rc == NGX_ERROR) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ସ୍ଥିର ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    ଅକ୍ଷର           *rv;
    ଶୂନ୍ୟ           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    ପାଇଁ (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        ଯଦି (cmd == ଖାଲି) {
            ଜାରି;
        }

        ପାଇଁ ( /* void */ ; cmd->name.len; cmd++) {

            ଯଦି (name->len != cmd->name.len) {
                ଜାରି;
            }

            ଯଦି (ngx_strcmp(name->data, cmd->name.data) != 0) {
                ଜାରି;
            }

            found = 1;

            ଯଦି (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                ଜାରି;
            }

            /* is the directive's location right ? */

            ଯଦି (!(cmd->type & cf->cmd_type)) {
                ଜାରି;
            }

            ଯଦି (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ଯଦି ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            ଯଦି (!(cmd->type & NGX_CONF_ANY)) {

                ଯଦି (cmd->type & NGX_CONF_FLAG) {

                    ଯଦି (cf->args->nelts != 2) {
                        ଯାଅ invalid;
                    }

                } ଅନ୍ୟଥା ଯଦି (cmd->type & NGX_CONF_1MORE) {

                    ଯଦି (cf->args->nelts < 2) {
                        ଯାଅ invalid;
                    }

                } ଅନ୍ୟଥା ଯଦି (cmd->type & NGX_CONF_2MORE) {

                    ଯଦି (cf->args->nelts < 3) {
                        ଯାଅ invalid;
                    }

                } ଅନ୍ୟଥା ଯଦି (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    ଯାଅ invalid;

                } ଅନ୍ୟଥା ଯଦି (!(cmd->type & argument_ସଂଖ୍ୟା[cf->args->nelts - 1]))
                {
                    ଯାଅ invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = ଖାଲି;

            ଯଦି (cmd->type & NGX_DIRECT_CONF) {
                conf = ((ଶୂନ୍ୟ **) cf->ctx)[cf->cycle->modules[i]->index];

            } ଅନ୍ୟଥା ଯଦି (cmd->type & NGX_MAIN_CONF) {
                conf = &(((ଶୂନ୍ୟ **) cf->ctx)[cf->cycle->modules[i]->index]);

            } ଅନ୍ୟଥା ଯଦି (cf->ctx) {
                confp = *(ଶୂନ୍ୟ **) ((ଅକ୍ଷର *) cf->ctx + cmd->conf);

                ଯଦି (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            ଯଦି (rv == NGX_CONF_OK) {
                ଫେରନ୍ତୁ NGX_OK;
            }

            ଯଦି (rv == NGX_CONF_ERROR) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }

    ଯଦି (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    ଫେରନ୍ତୁ NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    ଫେରନ୍ତୁ NGX_ERROR;
}


ସ୍ଥିର ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    size_t       len;
    ssize_t      n, size;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_ଧାଡ଼ି;
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
    start_ଧାଡ଼ି = cf->conf_file->ଧାଡ଼ି;

    file_size = ngx_file_size(&cf->conf_file->file.info);

    ପାଇଁ ( ;; ) {

        ଯଦି (b->pos >= b->last) {

            ଯଦି (cf->conf_file->file.offset >= file_size) {

                ଯଦି (cf->args->nelts > 0 || !last_space) {

                    ଯଦି (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        ଫେରନ୍ତୁ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                ଫେରନ୍ତୁ NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            ଯଦି (len == NGX_CONF_BUFFER) {
                cf->conf_file->ଧାଡ଼ି = start_ଧାଡ଼ି;

                ଯଦି (d_quoted) {
                    ch = '"';

                } ଅନ୍ୟଥା ଯଦି (s_quoted) {
                    ch = '\'';

                } ଅନ୍ୟଥା {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ଯଦି (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            ଯଦି (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            ଯଦି (n == NGX_ERROR) {
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ଯଦି (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            ଯଦି (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        ଯଦି (ch == LF) {
            cf->conf_file->ଧାଡ଼ି++;

            ଯଦି (sharp_comment) {
                sharp_comment = 0;
            }
        }

        ଯଦି (sharp_comment) {
            ଜାରି;
        }

        ଯଦି (quoted) {
            quoted = 0;
            ଜାରି;
        }

        ଯଦି (need_space) {
            ଯଦି (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                ଜାରି;
            }

            ଯଦି (ch == ';') {
                ଫେରନ୍ତୁ NGX_OK;
            }

            ଯଦି (ch == '{') {
                ଫେରନ୍ତୁ NGX_CONF_BLOCK_START;
            }

            ଯଦି (ch == ')') {
                last_space = 1;
                need_space = 0;

            } ଅନ୍ୟଥା {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                ଫେରନ୍ତୁ NGX_ERROR;
            }
        }

        ଯଦି (last_space) {

            start = b->pos - 1;
            start_ଧାଡ଼ି = cf->conf_file->ଧାଡ଼ି;

            ଯଦି (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                ଜାରି;
            }

            ବାଛ (ch) {

            ସ୍ଥିତି ';':
            ସ୍ଥିତି '{':
                ଯଦି (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                ଯଦି (ch == '{') {
                    ଫେରନ୍ତୁ NGX_CONF_BLOCK_START;
                }

                ଫେରନ୍ତୁ NGX_OK;

            ସ୍ଥିତି '}':
                ଯଦି (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                ଫେରନ୍ତୁ NGX_CONF_BLOCK_DONE;

            ସ୍ଥିତି '#':
                sharp_comment = 1;
                ଜାରି;

            ସ୍ଥିତି '\\':
                quoted = 1;
                last_space = 0;
                ଜାରି;

            ସ୍ଥିତି '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                ଜାରି;

            ସ୍ଥିତି '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                ଜାରି;

            ସ୍ଥିତି '$':
                variable = 1;
                last_space = 0;
                ଜାରି;

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                last_space = 0;
            }

        } ଅନ୍ୟଥା {
            ଯଦି (ch == '{' && variable) {
                ଜାରି;
            }

            variable = 0;

            ଯଦି (ch == '\\') {
                quoted = 1;
                ଜାରି;
            }

            ଯଦି (ch == '$') {
                variable = 1;
                ଜାରି;
            }

            ଯଦି (d_quoted) {
                ଯଦି (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } ଅନ୍ୟଥା ଯଦି (s_quoted) {
                ଯଦି (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } ଅନ୍ୟଥା ଯଦି (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            ଯଦି (found) {
                word = ngx_array_ଠେଲ(cf->args);
                ଯଦି (word == ଖାଲି) {
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                ଯଦି (word->data == ଖାଲି) {
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                ପାଇଁ (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    ଯଦି (*src == '\\') {
                        ବାଛ (src[1]) {
                        ସ୍ଥିତି '"':
                        ସ୍ଥିତି '\'':
                        ସ୍ଥିତି '\\':
                            src++;
                            ଭାଙ୍ଗ;

                        ସ୍ଥିତି 't':
                            *dst++ = '\t';
                            src += 2;
                            ଜାରି;

                        ସ୍ଥିତି 'r':
                            *dst++ = '\r';
                            src += 2;
                            ଜାରି;

                        ସ୍ଥିତି 'n':
                            *dst++ = '\n';
                            src += 2;
                            ଜାରି;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                ଯଦି (ch == ';') {
                    ଫେରନ୍ତୁ NGX_OK;
                }

                ଯଦି (ch == '{') {
                    ଫେରନ୍ତୁ NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


ଅକ୍ଷର *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର        *rv;
    ngx_int_t    n;
    ngx_str_t   *ମୂଲ୍ୟ, file, name;
    ngx_glob_t   gl;

    ମୂଲ୍ୟ = cf->args->elts;
    file = ମୂଲ୍ୟ[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    ଯଦି (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ଯଦି (strpbrk((ଅକ୍ଷର *) file.data, "*?[") == ଖାଲି) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        ଫେରନ୍ତୁ ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, ଆକାର(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    ଯଦି (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    ପାଇଁ ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        ଯଦି (n != NGX_OK) {
            ଭାଙ୍ଗ;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        ଯଦି (file.data == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        ଯଦି (rv != NGX_CONF_OK) {
            ଭାଙ୍ଗ;
        }
    }

    ngx_close_glob(&gl);

    ଫେରନ୍ତୁ rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    ଫେରନ୍ତୁ ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#ଯଦି (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    ଯଦି (name->len) {
        full = *name;

        ଯଦି (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            ଫେରନ୍ତୁ ଖାଲି;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        ପାଇଁ (i = 0; /* void */ ; i++) {

            ଯଦି (i >= part->nelts) {
                ଯଦି (part->next == ଖାଲି) {
                    ଭାଙ୍ଗ;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            ଯଦି (full.len != file[i].name.len) {
                ଜାରି;
            }

            ଯଦି (ngx_strcmp(full.data, file[i].name.data) == 0) {
                ଫେରନ୍ତୁ &file[i];
            }
        }
    }

    file = ngx_list_ଠେଲ(&cycle->open_files);
    ଯଦି (file == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } ଅନ୍ୟଥା {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = ଖାଲି;
    file->data = ଖାଲି;

    ଫେରନ୍ତୁ file;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    ପାଇଁ (i = 0; /* void */ ; i++) {

        ଯଦି (i >= part->nelts) {
            ଯଦି (part->next == ଖାଲି) {
                ଭାଙ୍ଗ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ଯଦି (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


ଶୂନ୍ୟ ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    va_list  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    va_end(args);

    ଯଦି (err) {
        p = ngx_log_errno(p, last, err);
    }

    ଯଦି (cf->conf_file == ଖାଲି) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        ଫେରନ୍ତୁ;
    }

    ଯଦି (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        ଫେରନ୍ତୁ;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->ଧାଡ଼ି);
}


ଅକ୍ଷର *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର  *p = conf;

    ngx_str_t        *ମୂଲ୍ୟ;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    ଯଦି (*fp != NGX_CONF_UNSET) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    ମୂଲ୍ୟ = cf->args->elts;

    ଯଦି (ngx_strcasecmp(ମୂଲ୍ୟ[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } ଅନ୍ୟଥା ଯଦି (ngx_strcasecmp(ମୂଲ୍ୟ[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } ଅନ୍ୟଥା {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     ମୂଲ୍ୟ[1].data, cmd->name.data);
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ଯଦି (cmd->post) {
        post = cmd->post;
        ଫେରନ୍ତୁ post->post_handler(cf, post, fp);
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ଅକ୍ଷର *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର  *p = conf;

    ngx_str_t        *field, *ମୂଲ୍ୟ;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    ଯଦି (field->data) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    ମୂଲ୍ୟ = cf->args->elts;

    *field = ମୂଲ୍ୟ[1];

    ଯଦି (cmd->post) {
        post = cmd->post;
        ଫେରନ୍ତୁ post->post_handler(cf, post, field);
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ଅକ୍ଷର *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର  *p = conf;

    ngx_str_t         *ମୂଲ୍ୟ, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    ଯଦି (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, ଆକାର(ngx_str_t));
        ଯଦି (*a == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }
    }

    s = ngx_array_ଠେଲ(*a);
    ଯଦି (s == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ମୂଲ୍ୟ = cf->args->elts;

    *s = ମୂଲ୍ୟ[1];

    ଯଦି (cmd->post) {
        post = cmd->post;
        ଫେରନ୍ତୁ post->post_handler(cf, post, s);
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ଅକ୍ଷର *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର  *p = conf;

    ngx_str_t         *ମୂଲ୍ୟ;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    ଯଦି (*a == NGX_CONF_UNSET_PTR || *a == ଖାଲି) {
        *a = ngx_array_create(cf->pool, 4, ଆକାର(ngx_keyval_t));
        ଯଦି (*a == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_ଠେଲ(*a);
    ଯଦି (kv == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ମୂଲ୍ୟ = cf->args->elts;

    kv->ଚାବି = ମୂଲ୍ୟ[1];
    kv->ମୂଲ୍ୟ = ମୂଲ୍ୟ[2];

    ଯଦି (cmd->post) {
        post = cmd->post;
        ଫେରନ୍ତୁ post->post_handler(cf, post, kv);
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ଅକ୍ଷର *
ngx_conf_set_ଅଙ୍କ_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *ମୂଲ୍ୟ;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    ଯଦି (*np != NGX_CONF_UNSET) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    ମୂଲ୍ୟ = cf->args->elts;
    *np = ngx_atoi(ମୂଲ୍ୟ[1].data, ମୂଲ୍ୟ[1].len);
    ଯଦି (*np == NGX_ERROR) {
        ଫେରନ୍ତୁ "invalid number";
    }

    ଯଦି (cmd->post) {
        post = cmd->post;
        ଫେରନ୍ତୁ post->post_handler(cf, post, np);
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ଅକ୍ଷର *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର  *p = conf;

    size_t           *sp;
    ngx_str_t        *ମୂଲ୍ୟ;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    ଯଦି (*sp != NGX_CONF_UNSET_SIZE) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    ମୂଲ୍ୟ = cf->args->elts;

    *sp = ngx_parse_size(&ମୂଲ୍ୟ[1]);
    ଯଦି (*sp == (size_t) NGX_ERROR) {
        ଫେରନ୍ତୁ "invalid value";
    }

    ଯଦି (cmd->post) {
        post = cmd->post;
        ଫେରନ୍ତୁ post->post_handler(cf, post, sp);
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ଅକ୍ଷର *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର  *p = conf;

    off_t            *op;
    ngx_str_t        *ମୂଲ୍ୟ;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    ଯଦି (*op != NGX_CONF_UNSET) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    ମୂଲ୍ୟ = cf->args->elts;

    *op = ngx_parse_offset(&ମୂଲ୍ୟ[1]);
    ଯଦି (*op == (off_t) NGX_ERROR) {
        ଫେରନ୍ତୁ "invalid value";
    }

    ଯଦି (cmd->post) {
        post = cmd->post;
        ଫେରନ୍ତୁ post->post_handler(cf, post, op);
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ଅକ୍ଷର *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *ମୂଲ୍ୟ;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    ଯଦି (*msp != NGX_CONF_UNSET_MSEC) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    ମୂଲ୍ୟ = cf->args->elts;

    *msp = ngx_parse_time(&ମୂଲ୍ୟ[1], 0);
    ଯଦି (*msp == (ngx_msec_t) NGX_ERROR) {
        ଫେରନ୍ତୁ "invalid value";
    }

    ଯଦି (cmd->post) {
        post = cmd->post;
        ଫେରନ୍ତୁ post->post_handler(cf, post, msp);
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ଅକ୍ଷର *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର  *p = conf;

    time_t           *sp;
    ngx_str_t        *ମୂଲ୍ୟ;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    ଯଦି (*sp != NGX_CONF_UNSET) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    ମୂଲ୍ୟ = cf->args->elts;

    *sp = ngx_parse_time(&ମୂଲ୍ୟ[1], 1);
    ଯଦି (*sp == (time_t) NGX_ERROR) {
        ଫେରନ୍ତୁ "invalid value";
    }

    ଯଦି (cmd->post) {
        post = cmd->post;
        ଫେରନ୍ତୁ post->post_handler(cf, post, sp);
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ଅକ୍ଷର *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର *p = conf;

    ngx_str_t   *ମୂଲ୍ୟ;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    ଯଦି (bufs->ଅଙ୍କ) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    ମୂଲ୍ୟ = cf->args->elts;

    bufs->ଅଙ୍କ = ngx_atoi(ମୂଲ୍ୟ[1].data, ମୂଲ୍ୟ[1].len);
    ଯଦି (bufs->ଅଙ୍କ == NGX_ERROR || bufs->ଅଙ୍କ == 0) {
        ଫେରନ୍ତୁ "invalid value";
    }

    bufs->size = ngx_parse_size(&ମୂଲ୍ୟ[2]);
    ଯଦି (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        ଫେରନ୍ତୁ "invalid value";
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ଅକ୍ଷର *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *ମୂଲ୍ୟ;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    ଯଦି (*np != NGX_CONF_UNSET_UINT) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    ମୂଲ୍ୟ = cf->args->elts;
    e = cmd->post;

    ପାଇଁ (i = 0; e[i].name.len != 0; i++) {
        ଯଦି (e[i].name.len != ମୂଲ୍ୟ[1].len
            || ngx_strcasecmp(e[i].name.data, ମୂଲ୍ୟ[1].data) != 0)
        {
            ଜାରି;
        }

        *np = e[i].ମୂଲ୍ୟ;

        ଫେରନ୍ତୁ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", ମୂଲ୍ୟ[1].data);

    ଫେରନ୍ତୁ NGX_CONF_ERROR;
}


ଅକ୍ଷର *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *ମୂଲ୍ୟ;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    ମୂଲ୍ୟ = cf->args->elts;
    mask = cmd->post;

    ପାଇଁ (i = 1; i < cf->args->nelts; i++) {
        ପାଇଁ (m = 0; mask[m].name.len != 0; m++) {

            ଯଦି (mask[m].name.len != ମୂଲ୍ୟ[i].len
                || ngx_strcasecmp(mask[m].name.data, ମୂଲ୍ୟ[i].data) != 0)
            {
                ଜାରି;
            }

            ଯଦି (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", ମୂଲ୍ୟ[i].data);

            } ଅନ୍ୟଥା {
                *np |= mask[m].mask;
            }

            ଭାଙ୍ଗ;
        }

        ଯଦି (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", ମୂଲ୍ୟ[i].data);

            ଫେରନ୍ତୁ NGX_CONF_ERROR;
        }
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


#ଯଦି 0

ଅକ୍ଷର *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଫେରନ୍ତୁ "unsupported on this platform";
}

#endif


ଅକ୍ଷର *
ngx_conf_deprecated(ngx_conf_t *cf, ଶୂନ୍ୟ *post, ଶୂନ୍ୟ *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ଅକ୍ଷର *
ngx_conf_check_ଅଙ୍କ_bounds(ngx_conf_t *cf, ଶୂନ୍ୟ *post, ଶୂନ୍ୟ *data)
{
    ngx_conf_ଅଙ୍କ_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    ଯଦି (bounds->ଉପର == -1) {
        ଯଦି (*np >= bounds->ତଳ) {
            ଫେରନ୍ତୁ NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->ତଳ);

        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ଯଦି (*np >= bounds->ତଳ && *np <= bounds->ଉପର) {
        ଫେରନ୍ତୁ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->ତଳ, bounds->ଉପର);

    ଫେରନ୍ତୁ NGX_CONF_ERROR;
}


ଅକ୍ଷର *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, ଶୂନ୍ୟ *post, ଶୂନ୍ୟ *data)
{
    size_t  *sp = data;

    ଯଦି (*sp == 0) {
        ଫେରନ୍ତୁ "value must be nonzero";
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}
