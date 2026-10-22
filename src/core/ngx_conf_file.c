
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>

#define NGX_CONF_BUFFER  4096

സ്ഥിതം ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
സ്ഥിതം ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
സ്ഥിതം ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
സ്ഥിതം ശൂന്യം ngx_conf_flush_files(ngx_cycle_t *cycle);


സ്ഥിതം ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      ശൂന്യ },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    ശൂന്യ,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    ശൂന്യ,                                  /* init master */
    ശൂന്യ,                                  /* init module */
    ശൂന്യ,                                  /* init process */
    ശൂന്യ,                                  /* init thread */
    ശൂന്യ,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    ശൂന്യ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

സ്ഥിതം ngx_uint_t argument_സംഖ്യ[] = {
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


അക്ഷരം *
ngx_conf_param(ngx_conf_t *cf)
{
    അക്ഷരം             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    എങ്കിൽ (param->len == 0) {
        തിരികെ NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, വലുപ്പം(ngx_conf_file_t));

    ngx_memzero(&b, വലുപ്പം(ngx_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.data = ശൂന്യ;
    conf_file.വരി = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    rv = ngx_conf_parse(cf, ശൂന്യ);

    cf->conf_file = ശൂന്യ;

    തിരികെ rv;
}


സ്ഥിതം ngx_int_t
ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             size;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_കണ്ണി_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_crc32_long(filename->data, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->cycle->config_dump_rbtree, filename, hash);

    എങ്കിൽ (sn) {
        cf->conf_file->dump = ശൂന്യ;
        തിരികെ NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    എങ്കിൽ (p == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    cd = ngx_array_തള്ളുക(&cf->cycle->config_dump);
    എങ്കിൽ (cd == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    എങ്കിൽ (buf == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, വലുപ്പം(ngx_str_കണ്ണി_t));
    എങ്കിൽ (sn == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    sn->കണ്ണി.താക്കോൽ = hash;
    sn->str = cd->name;

    ngx_rbtree_തിരുകുക(&cf->cycle->config_dump_rbtree, &sn->കണ്ണി);

    തിരികെ NGX_OK;
}


അക്ഷരം *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    അക്ഷരം             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    എണ്ണൽ {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#എങ്കിൽ (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = ശൂന്യ;
#endif

    എങ്കിൽ (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        എങ്കിൽ (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            തിരികെ NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        എങ്കിൽ (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        എങ്കിൽ (buf.start == ശൂന്യ) {
            പോകുക failed;
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
        cf->conf_file->വരി = 1;

        type = parse_file;

        എങ്കിൽ (ngx_dump_config
#എങ്കിൽ (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#എങ്കിൽ (NGX_DEBUG)
            || 1
#endif
           )
        {
            എങ്കിൽ (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                പോകുക failed;
            }

        } അല്ലെങ്കിൽ {
            cf->conf_file->dump = ശൂന്യ;
        }

    } അല്ലെങ്കിൽ എങ്കിൽ (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } അല്ലെങ്കിൽ {
        type = parse_param;
    }


    വേണ്ടി ( ;; ) {
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

        എങ്കിൽ (rc == NGX_ERROR) {
            പോകുക done;
        }

        എങ്കിൽ (rc == NGX_CONF_BLOCK_DONE) {

            എങ്കിൽ (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                പോകുക failed;
            }

            പോകുക done;
        }

        എങ്കിൽ (rc == NGX_CONF_FILE_DONE) {

            എങ്കിൽ (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                പോകുക failed;
            }

            പോകുക done;
        }

        എങ്കിൽ (rc == NGX_CONF_BLOCK_START) {

            എങ്കിൽ (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                പോകുക failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        എങ്കിൽ (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            എങ്കിൽ (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                പോകുക failed;
            }

            rv = (*cf->handler)(cf, ശൂന്യ, cf->handler_conf);
            എങ്കിൽ (rv == NGX_CONF_OK) {
                തുടരുക;
            }

            എങ്കിൽ (rv == NGX_CONF_ERROR) {
                പോകുക failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            പോകുക failed;
        }


        rc = ngx_conf_handler(cf, rc);

        എങ്കിൽ (rc == NGX_ERROR) {
            പോകുക failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    എങ്കിൽ (filename) {
        എങ്കിൽ (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        എങ്കിൽ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    എങ്കിൽ (rc == NGX_ERROR) {
        തിരികെ NGX_CONF_ERROR;
    }

    തിരികെ NGX_CONF_OK;
}


സ്ഥിതം ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    അക്ഷരം           *rv;
    ശൂന്യം           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    വേണ്ടി (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        എങ്കിൽ (cmd == ശൂന്യ) {
            തുടരുക;
        }

        വേണ്ടി ( /* void */ ; cmd->name.len; cmd++) {

            എങ്കിൽ (name->len != cmd->name.len) {
                തുടരുക;
            }

            എങ്കിൽ (ngx_strcmp(name->data, cmd->name.data) != 0) {
                തുടരുക;
            }

            found = 1;

            എങ്കിൽ (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                തുടരുക;
            }

            /* is the directive's location right ? */

            എങ്കിൽ (!(cmd->type & cf->cmd_type)) {
                തുടരുക;
            }

            എങ്കിൽ (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                തിരികെ NGX_ERROR;
            }

            എങ്കിൽ ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                തിരികെ NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            എങ്കിൽ (!(cmd->type & NGX_CONF_ANY)) {

                എങ്കിൽ (cmd->type & NGX_CONF_FLAG) {

                    എങ്കിൽ (cf->args->nelts != 2) {
                        പോകുക invalid;
                    }

                } അല്ലെങ്കിൽ എങ്കിൽ (cmd->type & NGX_CONF_1MORE) {

                    എങ്കിൽ (cf->args->nelts < 2) {
                        പോകുക invalid;
                    }

                } അല്ലെങ്കിൽ എങ്കിൽ (cmd->type & NGX_CONF_2MORE) {

                    എങ്കിൽ (cf->args->nelts < 3) {
                        പോകുക invalid;
                    }

                } അല്ലെങ്കിൽ എങ്കിൽ (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    പോകുക invalid;

                } അല്ലെങ്കിൽ എങ്കിൽ (!(cmd->type & argument_സംഖ്യ[cf->args->nelts - 1]))
                {
                    പോകുക invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = ശൂന്യ;

            എങ്കിൽ (cmd->type & NGX_DIRECT_CONF) {
                conf = ((ശൂന്യം **) cf->ctx)[cf->cycle->modules[i]->index];

            } അല്ലെങ്കിൽ എങ്കിൽ (cmd->type & NGX_MAIN_CONF) {
                conf = &(((ശൂന്യം **) cf->ctx)[cf->cycle->modules[i]->index]);

            } അല്ലെങ്കിൽ എങ്കിൽ (cf->ctx) {
                confp = *(ശൂന്യം **) ((അക്ഷരം *) cf->ctx + cmd->conf);

                എങ്കിൽ (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            എങ്കിൽ (rv == NGX_CONF_OK) {
                തിരികെ NGX_OK;
            }

            എങ്കിൽ (rv == NGX_CONF_ERROR) {
                തിരികെ NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            തിരികെ NGX_ERROR;
        }
    }

    എങ്കിൽ (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        തിരികെ NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    തിരികെ NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    തിരികെ NGX_ERROR;
}


സ്ഥിതം ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    size_t       len;
    ssize_t      n, size;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_വരി;
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
    start_വരി = cf->conf_file->വരി;

    file_size = ngx_file_size(&cf->conf_file->file.info);

    വേണ്ടി ( ;; ) {

        എങ്കിൽ (b->pos >= b->last) {

            എങ്കിൽ (cf->conf_file->file.offset >= file_size) {

                എങ്കിൽ (cf->args->nelts > 0 || !last_space) {

                    എങ്കിൽ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        തിരികെ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    തിരികെ NGX_ERROR;
                }

                തിരികെ NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            എങ്കിൽ (len == NGX_CONF_BUFFER) {
                cf->conf_file->വരി = start_വരി;

                എങ്കിൽ (d_quoted) {
                    ch = '"';

                } അല്ലെങ്കിൽ എങ്കിൽ (s_quoted) {
                    ch = '\'';

                } അല്ലെങ്കിൽ {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    തിരികെ NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                തിരികെ NGX_ERROR;
            }

            എങ്കിൽ (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            എങ്കിൽ (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            എങ്കിൽ (n == NGX_ERROR) {
                തിരികെ NGX_ERROR;
            }

            എങ്കിൽ (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                തിരികെ NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            എങ്കിൽ (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        എങ്കിൽ (ch == LF) {
            cf->conf_file->വരി++;

            എങ്കിൽ (sharp_comment) {
                sharp_comment = 0;
            }
        }

        എങ്കിൽ (sharp_comment) {
            തുടരുക;
        }

        എങ്കിൽ (quoted) {
            quoted = 0;
            തുടരുക;
        }

        എങ്കിൽ (need_space) {
            എങ്കിൽ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                തുടരുക;
            }

            എങ്കിൽ (ch == ';') {
                തിരികെ NGX_OK;
            }

            എങ്കിൽ (ch == '{') {
                തിരികെ NGX_CONF_BLOCK_START;
            }

            എങ്കിൽ (ch == ')') {
                last_space = 1;
                need_space = 0;

            } അല്ലെങ്കിൽ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                തിരികെ NGX_ERROR;
            }
        }

        എങ്കിൽ (last_space) {

            start = b->pos - 1;
            start_വരി = cf->conf_file->വരി;

            എങ്കിൽ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                തുടരുക;
            }

            തിരഞ്ഞെടുപ്പ് (ch) {

            അവസ്ഥ ';':
            അവസ്ഥ '{':
                എങ്കിൽ (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    തിരികെ NGX_ERROR;
                }

                എങ്കിൽ (ch == '{') {
                    തിരികെ NGX_CONF_BLOCK_START;
                }

                തിരികെ NGX_OK;

            അവസ്ഥ '}':
                എങ്കിൽ (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    തിരികെ NGX_ERROR;
                }

                തിരികെ NGX_CONF_BLOCK_DONE;

            അവസ്ഥ '#':
                sharp_comment = 1;
                തുടരുക;

            അവസ്ഥ '\\':
                quoted = 1;
                last_space = 0;
                തുടരുക;

            അവസ്ഥ '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                തുടരുക;

            അവസ്ഥ '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                തുടരുക;

            അവസ്ഥ '$':
                variable = 1;
                last_space = 0;
                തുടരുക;

            സ്ഥിരസ്ഥിതി:
                last_space = 0;
            }

        } അല്ലെങ്കിൽ {
            എങ്കിൽ (ch == '{' && variable) {
                തുടരുക;
            }

            variable = 0;

            എങ്കിൽ (ch == '\\') {
                quoted = 1;
                തുടരുക;
            }

            എങ്കിൽ (ch == '$') {
                variable = 1;
                തുടരുക;
            }

            എങ്കിൽ (d_quoted) {
                എങ്കിൽ (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } അല്ലെങ്കിൽ എങ്കിൽ (s_quoted) {
                എങ്കിൽ (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } അല്ലെങ്കിൽ എങ്കിൽ (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            എങ്കിൽ (found) {
                word = ngx_array_തള്ളുക(cf->args);
                എങ്കിൽ (word == ശൂന്യ) {
                    തിരികെ NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                എങ്കിൽ (word->data == ശൂന്യ) {
                    തിരികെ NGX_ERROR;
                }

                വേണ്ടി (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    എങ്കിൽ (*src == '\\') {
                        തിരഞ്ഞെടുപ്പ് (src[1]) {
                        അവസ്ഥ '"':
                        അവസ്ഥ '\'':
                        അവസ്ഥ '\\':
                            src++;
                            നിർത്തുക;

                        അവസ്ഥ 't':
                            *dst++ = '\t';
                            src += 2;
                            തുടരുക;

                        അവസ്ഥ 'r':
                            *dst++ = '\r';
                            src += 2;
                            തുടരുക;

                        അവസ്ഥ 'n':
                            *dst++ = '\n';
                            src += 2;
                            തുടരുക;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                എങ്കിൽ (ch == ';') {
                    തിരികെ NGX_OK;
                }

                എങ്കിൽ (ch == '{') {
                    തിരികെ NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


അക്ഷരം *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം        *rv;
    ngx_int_t    n;
    ngx_str_t   *മൂല്യം, file, name;
    ngx_glob_t   gl;

    മൂല്യം = cf->args->elts;
    file = മൂല്യം[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    എങ്കിൽ (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        തിരികെ NGX_CONF_ERROR;
    }

    എങ്കിൽ (strpbrk((അക്ഷരം *) file.data, "*?[") == ശൂന്യ) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        തിരികെ ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, വലുപ്പം(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    എങ്കിൽ (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        തിരികെ NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    വേണ്ടി ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        എങ്കിൽ (n != NGX_OK) {
            നിർത്തുക;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        എങ്കിൽ (file.data == ശൂന്യ) {
            തിരികെ NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        എങ്കിൽ (rv != NGX_CONF_OK) {
            നിർത്തുക;
        }
    }

    ngx_close_glob(&gl);

    തിരികെ rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    തിരികെ ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#എങ്കിൽ (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    എങ്കിൽ (name->len) {
        full = *name;

        എങ്കിൽ (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            തിരികെ ശൂന്യ;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        വേണ്ടി (i = 0; /* void */ ; i++) {

            എങ്കിൽ (i >= part->nelts) {
                എങ്കിൽ (part->next == ശൂന്യ) {
                    നിർത്തുക;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            എങ്കിൽ (full.len != file[i].name.len) {
                തുടരുക;
            }

            എങ്കിൽ (ngx_strcmp(full.data, file[i].name.data) == 0) {
                തിരികെ &file[i];
            }
        }
    }

    file = ngx_list_തള്ളുക(&cycle->open_files);
    എങ്കിൽ (file == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    എങ്കിൽ (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } അല്ലെങ്കിൽ {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = ശൂന്യ;
    file->data = ശൂന്യ;

    തിരികെ file;
}


സ്ഥിതം ശൂന്യം
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    വേണ്ടി (i = 0; /* void */ ; i++) {

        എങ്കിൽ (i >= part->nelts) {
            എങ്കിൽ (part->next == ശൂന്യ) {
                നിർത്തുക;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        എങ്കിൽ (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


ശൂന്യം ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    സ്ഥിരം അക്ഷരം *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    va_list  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    va_end(args);

    എങ്കിൽ (err) {
        p = ngx_log_errno(p, last, err);
    }

    എങ്കിൽ (cf->conf_file == ശൂന്യ) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        തിരികെ;
    }

    എങ്കിൽ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        തിരികെ;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->വരി);
}


അക്ഷരം *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം  *p = conf;

    ngx_str_t        *മൂല്യം;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    എങ്കിൽ (*fp != NGX_CONF_UNSET) {
        തിരികെ "is duplicate";
    }

    മൂല്യം = cf->args->elts;

    എങ്കിൽ (ngx_strcasecmp(മൂല്യം[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strcasecmp(മൂല്യം[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } അല്ലെങ്കിൽ {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     മൂല്യം[1].data, cmd->name.data);
        തിരികെ NGX_CONF_ERROR;
    }

    എങ്കിൽ (cmd->post) {
        post = cmd->post;
        തിരികെ post->post_handler(cf, post, fp);
    }

    തിരികെ NGX_CONF_OK;
}


അക്ഷരം *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം  *p = conf;

    ngx_str_t        *field, *മൂല്യം;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    എങ്കിൽ (field->data) {
        തിരികെ "is duplicate";
    }

    മൂല്യം = cf->args->elts;

    *field = മൂല്യം[1];

    എങ്കിൽ (cmd->post) {
        post = cmd->post;
        തിരികെ post->post_handler(cf, post, field);
    }

    തിരികെ NGX_CONF_OK;
}


അക്ഷരം *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം  *p = conf;

    ngx_str_t         *മൂല്യം, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    എങ്കിൽ (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, വലുപ്പം(ngx_str_t));
        എങ്കിൽ (*a == ശൂന്യ) {
            തിരികെ NGX_CONF_ERROR;
        }
    }

    s = ngx_array_തള്ളുക(*a);
    എങ്കിൽ (s == ശൂന്യ) {
        തിരികെ NGX_CONF_ERROR;
    }

    മൂല്യം = cf->args->elts;

    *s = മൂല്യം[1];

    എങ്കിൽ (cmd->post) {
        post = cmd->post;
        തിരികെ post->post_handler(cf, post, s);
    }

    തിരികെ NGX_CONF_OK;
}


അക്ഷരം *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം  *p = conf;

    ngx_str_t         *മൂല്യം;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    എങ്കിൽ (*a == NGX_CONF_UNSET_PTR || *a == ശൂന്യ) {
        *a = ngx_array_create(cf->pool, 4, വലുപ്പം(ngx_keyval_t));
        എങ്കിൽ (*a == ശൂന്യ) {
            തിരികെ NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_തള്ളുക(*a);
    എങ്കിൽ (kv == ശൂന്യ) {
        തിരികെ NGX_CONF_ERROR;
    }

    മൂല്യം = cf->args->elts;

    kv->താക്കോൽ = മൂല്യം[1];
    kv->മൂല്യം = മൂല്യം[2];

    എങ്കിൽ (cmd->post) {
        post = cmd->post;
        തിരികെ post->post_handler(cf, post, kv);
    }

    തിരികെ NGX_CONF_OK;
}


അക്ഷരം *
ngx_conf_set_അക്കം_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *മൂല്യം;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    എങ്കിൽ (*np != NGX_CONF_UNSET) {
        തിരികെ "is duplicate";
    }

    മൂല്യം = cf->args->elts;
    *np = ngx_atoi(മൂല്യം[1].data, മൂല്യം[1].len);
    എങ്കിൽ (*np == NGX_ERROR) {
        തിരികെ "invalid number";
    }

    എങ്കിൽ (cmd->post) {
        post = cmd->post;
        തിരികെ post->post_handler(cf, post, np);
    }

    തിരികെ NGX_CONF_OK;
}


അക്ഷരം *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം  *p = conf;

    size_t           *sp;
    ngx_str_t        *മൂല്യം;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    എങ്കിൽ (*sp != NGX_CONF_UNSET_SIZE) {
        തിരികെ "is duplicate";
    }

    മൂല്യം = cf->args->elts;

    *sp = ngx_parse_size(&മൂല്യം[1]);
    എങ്കിൽ (*sp == (size_t) NGX_ERROR) {
        തിരികെ "invalid value";
    }

    എങ്കിൽ (cmd->post) {
        post = cmd->post;
        തിരികെ post->post_handler(cf, post, sp);
    }

    തിരികെ NGX_CONF_OK;
}


അക്ഷരം *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം  *p = conf;

    off_t            *op;
    ngx_str_t        *മൂല്യം;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    എങ്കിൽ (*op != NGX_CONF_UNSET) {
        തിരികെ "is duplicate";
    }

    മൂല്യം = cf->args->elts;

    *op = ngx_parse_offset(&മൂല്യം[1]);
    എങ്കിൽ (*op == (off_t) NGX_ERROR) {
        തിരികെ "invalid value";
    }

    എങ്കിൽ (cmd->post) {
        post = cmd->post;
        തിരികെ post->post_handler(cf, post, op);
    }

    തിരികെ NGX_CONF_OK;
}


അക്ഷരം *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *മൂല്യം;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    എങ്കിൽ (*msp != NGX_CONF_UNSET_MSEC) {
        തിരികെ "is duplicate";
    }

    മൂല്യം = cf->args->elts;

    *msp = ngx_parse_time(&മൂല്യം[1], 0);
    എങ്കിൽ (*msp == (ngx_msec_t) NGX_ERROR) {
        തിരികെ "invalid value";
    }

    എങ്കിൽ (cmd->post) {
        post = cmd->post;
        തിരികെ post->post_handler(cf, post, msp);
    }

    തിരികെ NGX_CONF_OK;
}


അക്ഷരം *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം  *p = conf;

    time_t           *sp;
    ngx_str_t        *മൂല്യം;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    എങ്കിൽ (*sp != NGX_CONF_UNSET) {
        തിരികെ "is duplicate";
    }

    മൂല്യം = cf->args->elts;

    *sp = ngx_parse_time(&മൂല്യം[1], 1);
    എങ്കിൽ (*sp == (time_t) NGX_ERROR) {
        തിരികെ "invalid value";
    }

    എങ്കിൽ (cmd->post) {
        post = cmd->post;
        തിരികെ post->post_handler(cf, post, sp);
    }

    തിരികെ NGX_CONF_OK;
}


അക്ഷരം *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം *p = conf;

    ngx_str_t   *മൂല്യം;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    എങ്കിൽ (bufs->അക്കം) {
        തിരികെ "is duplicate";
    }

    മൂല്യം = cf->args->elts;

    bufs->അക്കം = ngx_atoi(മൂല്യം[1].data, മൂല്യം[1].len);
    എങ്കിൽ (bufs->അക്കം == NGX_ERROR || bufs->അക്കം == 0) {
        തിരികെ "invalid value";
    }

    bufs->size = ngx_parse_size(&മൂല്യം[2]);
    എങ്കിൽ (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        തിരികെ "invalid value";
    }

    തിരികെ NGX_CONF_OK;
}


അക്ഷരം *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *മൂല്യം;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    എങ്കിൽ (*np != NGX_CONF_UNSET_UINT) {
        തിരികെ "is duplicate";
    }

    മൂല്യം = cf->args->elts;
    e = cmd->post;

    വേണ്ടി (i = 0; e[i].name.len != 0; i++) {
        എങ്കിൽ (e[i].name.len != മൂല്യം[1].len
            || ngx_strcasecmp(e[i].name.data, മൂല്യം[1].data) != 0)
        {
            തുടരുക;
        }

        *np = e[i].മൂല്യം;

        തിരികെ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", മൂല്യം[1].data);

    തിരികെ NGX_CONF_ERROR;
}


അക്ഷരം *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *മൂല്യം;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    മൂല്യം = cf->args->elts;
    mask = cmd->post;

    വേണ്ടി (i = 1; i < cf->args->nelts; i++) {
        വേണ്ടി (m = 0; mask[m].name.len != 0; m++) {

            എങ്കിൽ (mask[m].name.len != മൂല്യം[i].len
                || ngx_strcasecmp(mask[m].name.data, മൂല്യം[i].data) != 0)
            {
                തുടരുക;
            }

            എങ്കിൽ (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", മൂല്യം[i].data);

            } അല്ലെങ്കിൽ {
                *np |= mask[m].mask;
            }

            നിർത്തുക;
        }

        എങ്കിൽ (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", മൂല്യം[i].data);

            തിരികെ NGX_CONF_ERROR;
        }
    }

    തിരികെ NGX_CONF_OK;
}


#എങ്കിൽ 0

അക്ഷരം *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    തിരികെ "unsupported on this platform";
}

#endif


അക്ഷരം *
ngx_conf_deprecated(ngx_conf_t *cf, ശൂന്യം *post, ശൂന്യം *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    തിരികെ NGX_CONF_OK;
}


അക്ഷരം *
ngx_conf_check_അക്കം_bounds(ngx_conf_t *cf, ശൂന്യം *post, ശൂന്യം *data)
{
    ngx_conf_അക്കം_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    എങ്കിൽ (bounds->മുകളിലെ == -1) {
        എങ്കിൽ (*np >= bounds->താഴെയുള്ള) {
            തിരികെ NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->താഴെയുള്ള);

        തിരികെ NGX_CONF_ERROR;
    }

    എങ്കിൽ (*np >= bounds->താഴെയുള്ള && *np <= bounds->മുകളിലെ) {
        തിരികെ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->താഴെയുള്ള, bounds->മുകളിലെ);

    തിരികെ NGX_CONF_ERROR;
}


അക്ഷരം *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, ശൂന്യം *post, ശൂന്യം *data)
{
    size_t  *sp = data;

    എങ്കിൽ (*sp == 0) {
        തിരികെ "value must be nonzero";
    }

    തിരികെ NGX_CONF_OK;
}
