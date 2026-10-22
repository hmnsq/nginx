
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#define NGX_CONF_BUFFER  4096

ساكن ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
ساكن ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
ساكن ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
ساكن فراغ ngx_conf_flush_files(ngx_cycle_t *cycle);


ساكن ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      عدم },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    عدم,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    عدم,                                  /* init master */
    عدم,                                  /* init module */
    عدم,                                  /* init process */
    عدم,                                  /* init thread */
    عدم,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    عدم,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

ساكن ngx_uint_t argument_number[] = {
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


حرف *
ngx_conf_param(ngx_conf_t *cf)
{
    حرف             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    𐡄𐡍 (param->len == 0) {
        أرجع NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, حجم(ngx_conf_file_t));

    ngx_memzero(&b, حجم(ngx_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.data = عدم;
    conf_file.line = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    rv = ngx_conf_parse(cf, عدم);

    cf->conf_file = عدم;

    أرجع rv;
}


ساكن ngx_int_t
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

    𐡄𐡍 (sn) {
        cf->conf_file->dump = عدم;
        أرجع NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    𐡄𐡍 (p == عدم) {
        أرجع NGX_ERROR;
    }

    cd = ngx_array_push(&cf->cycle->config_dump);
    𐡄𐡍 (cd == عدم) {
        أرجع NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    𐡄𐡍 (buf == عدم) {
        أرجع NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, حجم(ngx_str_node_t));
    𐡄𐡍 (sn == عدم) {
        أرجع NGX_ERROR;
    }

    sn->node.key = hash;
    sn->str = cd->name;

    ngx_rbtree_insert(&cf->cycle->config_dump_rbtree, &sn->node);

    أرجع NGX_OK;
}


حرف *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    حرف             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    تعداد {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#𐡄𐡍 (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = عدم;
#endif

    𐡄𐡍 (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        𐡄𐡍 (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            أرجع NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        𐡄𐡍 (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        𐡄𐡍 (buf.start == عدم) {
            اذهب failed;
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

        𐡄𐡍 (ngx_dump_config
#𐡄𐡍 (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#𐡄𐡍 (NGX_DEBUG)
            || 1
#endif
           )
        {
            𐡄𐡍 (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                اذهب failed;
            }

        } وإلا {
            cf->conf_file->dump = عدم;
        }

    } وإلا 𐡄𐡍 (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } وإلا {
        type = parse_param;
    }


    لكل ( ;; ) {
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

        𐡄𐡍 (rc == NGX_ERROR) {
            اذهب done;
        }

        𐡄𐡍 (rc == NGX_CONF_BLOCK_DONE) {

            𐡄𐡍 (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                اذهب failed;
            }

            اذهب done;
        }

        𐡄𐡍 (rc == NGX_CONF_FILE_DONE) {

            𐡄𐡍 (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                اذهب failed;
            }

            اذهب done;
        }

        𐡄𐡍 (rc == NGX_CONF_BLOCK_START) {

            𐡄𐡍 (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                اذهب failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        𐡄𐡍 (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            𐡄𐡍 (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                اذهب failed;
            }

            rv = (*cf->handler)(cf, عدم, cf->handler_conf);
            𐡄𐡍 (rv == NGX_CONF_OK) {
                استمر;
            }

            𐡄𐡍 (rv == NGX_CONF_ERROR) {
                اذهب failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            اذهب failed;
        }


        rc = ngx_conf_handler(cf, rc);

        𐡄𐡍 (rc == NGX_ERROR) {
            اذهب failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    𐡄𐡍 (filename) {
        𐡄𐡍 (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        𐡄𐡍 (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    𐡄𐡍 (rc == NGX_ERROR) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


ساكن ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    حرف           *rv;
    فراغ           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    لكل (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        𐡄𐡍 (cmd == عدم) {
            استمر;
        }

        لكل ( /* void */ ; cmd->name.len; cmd++) {

            𐡄𐡍 (name->len != cmd->name.len) {
                استمر;
            }

            𐡄𐡍 (ngx_strcmp(name->data, cmd->name.data) != 0) {
                استمر;
            }

            found = 1;

            𐡄𐡍 (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                استمر;
            }

            /* is the directive's location right ? */

            𐡄𐡍 (!(cmd->type & cf->cmd_type)) {
                استمر;
            }

            𐡄𐡍 (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                أرجع NGX_ERROR;
            }

            𐡄𐡍 ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                أرجع NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            𐡄𐡍 (!(cmd->type & NGX_CONF_ANY)) {

                𐡄𐡍 (cmd->type & NGX_CONF_FLAG) {

                    𐡄𐡍 (cf->args->nelts != 2) {
                        اذهب invalid;
                    }

                } وإلا 𐡄𐡍 (cmd->type & NGX_CONF_1MORE) {

                    𐡄𐡍 (cf->args->nelts < 2) {
                        اذهب invalid;
                    }

                } وإلا 𐡄𐡍 (cmd->type & NGX_CONF_2MORE) {

                    𐡄𐡍 (cf->args->nelts < 3) {
                        اذهب invalid;
                    }

                } وإلا 𐡄𐡍 (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    اذهب invalid;

                } وإلا 𐡄𐡍 (!(cmd->type & argument_number[cf->args->nelts - 1]))
                {
                    اذهب invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = عدم;

            𐡄𐡍 (cmd->type & NGX_DIRECT_CONF) {
                conf = ((فراغ **) cf->ctx)[cf->cycle->modules[i]->index];

            } وإلا 𐡄𐡍 (cmd->type & NGX_MAIN_CONF) {
                conf = &(((فراغ **) cf->ctx)[cf->cycle->modules[i]->index]);

            } وإلا 𐡄𐡍 (cf->ctx) {
                confp = *(فراغ **) ((حرف *) cf->ctx + cmd->conf);

                𐡄𐡍 (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            𐡄𐡍 (rv == NGX_CONF_OK) {
                أرجع NGX_OK;
            }

            𐡄𐡍 (rv == NGX_CONF_ERROR) {
                أرجع NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            أرجع NGX_ERROR;
        }
    }

    𐡄𐡍 (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        أرجع NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    أرجع NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    أرجع NGX_ERROR;
}


ساكن ngx_int_t
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

    لكل ( ;; ) {

        𐡄𐡍 (b->pos >= b->last) {

            𐡄𐡍 (cf->conf_file->file.offset >= file_size) {

                𐡄𐡍 (cf->args->nelts > 0 || !last_space) {

                    𐡄𐡍 (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        أرجع NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    أرجع NGX_ERROR;
                }

                أرجع NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            𐡄𐡍 (len == NGX_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                𐡄𐡍 (d_quoted) {
                    ch = '"';

                } وإلا 𐡄𐡍 (s_quoted) {
                    ch = '\'';

                } وإلا {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    أرجع NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                أرجع NGX_ERROR;
            }

            𐡄𐡍 (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            𐡄𐡍 (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            𐡄𐡍 (n == NGX_ERROR) {
                أرجع NGX_ERROR;
            }

            𐡄𐡍 (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                أرجع NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            𐡄𐡍 (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        𐡄𐡍 (ch == LF) {
            cf->conf_file->line++;

            𐡄𐡍 (sharp_comment) {
                sharp_comment = 0;
            }
        }

        𐡄𐡍 (sharp_comment) {
            استمر;
        }

        𐡄𐡍 (quoted) {
            quoted = 0;
            استمر;
        }

        𐡄𐡍 (need_space) {
            𐡄𐡍 (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                استمر;
            }

            𐡄𐡍 (ch == ';') {
                أرجع NGX_OK;
            }

            𐡄𐡍 (ch == '{') {
                أرجع NGX_CONF_BLOCK_START;
            }

            𐡄𐡍 (ch == ')') {
                last_space = 1;
                need_space = 0;

            } وإلا {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                أرجع NGX_ERROR;
            }
        }

        𐡄𐡍 (last_space) {

            start = b->pos - 1;
            start_line = cf->conf_file->line;

            𐡄𐡍 (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                استمر;
            }

            بدل (ch) {

            حالة ';':
            حالة '{':
                𐡄𐡍 (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    أرجع NGX_ERROR;
                }

                𐡄𐡍 (ch == '{') {
                    أرجع NGX_CONF_BLOCK_START;
                }

                أرجع NGX_OK;

            حالة '}':
                𐡄𐡍 (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    أرجع NGX_ERROR;
                }

                أرجع NGX_CONF_BLOCK_DONE;

            حالة '#':
                sharp_comment = 1;
                استمر;

            حالة '\\':
                quoted = 1;
                last_space = 0;
                استمر;

            حالة '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                استمر;

            حالة '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                استمر;

            حالة '$':
                variable = 1;
                last_space = 0;
                استمر;

            افتراضي:
                last_space = 0;
            }

        } وإلا {
            𐡄𐡍 (ch == '{' && variable) {
                استمر;
            }

            variable = 0;

            𐡄𐡍 (ch == '\\') {
                quoted = 1;
                استمر;
            }

            𐡄𐡍 (ch == '$') {
                variable = 1;
                استمر;
            }

            𐡄𐡍 (d_quoted) {
                𐡄𐡍 (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } وإلا 𐡄𐡍 (s_quoted) {
                𐡄𐡍 (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } وإلا 𐡄𐡍 (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            𐡄𐡍 (found) {
                word = ngx_array_push(cf->args);
                𐡄𐡍 (word == عدم) {
                    أرجع NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                𐡄𐡍 (word->data == عدم) {
                    أرجع NGX_ERROR;
                }

                لكل (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    𐡄𐡍 (*src == '\\') {
                        بدل (src[1]) {
                        حالة '"':
                        حالة '\'':
                        حالة '\\':
                            src++;
                            اكسر;

                        حالة 't':
                            *dst++ = '\t';
                            src += 2;
                            استمر;

                        حالة 'r':
                            *dst++ = '\r';
                            src += 2;
                            استمر;

                        حالة 'n':
                            *dst++ = '\n';
                            src += 2;
                            استمر;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                𐡄𐡍 (ch == ';') {
                    أرجع NGX_OK;
                }

                𐡄𐡍 (ch == '{') {
                    أرجع NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


حرف *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف        *rv;
    ngx_int_t    n;
    ngx_str_t   *value, file, name;
    ngx_glob_t   gl;

    value = cf->args->elts;
    file = value[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    𐡄𐡍 (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    𐡄𐡍 (strpbrk((حرف *) file.data, "*?[") == عدم) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        أرجع ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, حجم(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    𐡄𐡍 (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        أرجع NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    لكل ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        𐡄𐡍 (n != NGX_OK) {
            اكسر;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        𐡄𐡍 (file.data == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        𐡄𐡍 (rv != NGX_CONF_OK) {
            اكسر;
        }
    }

    ngx_close_glob(&gl);

    أرجع rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    أرجع ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#𐡄𐡍 (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    𐡄𐡍 (name->len) {
        full = *name;

        𐡄𐡍 (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            أرجع عدم;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        لكل (i = 0; /* void */ ; i++) {

            𐡄𐡍 (i >= part->nelts) {
                𐡄𐡍 (part->next == عدم) {
                    اكسر;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            𐡄𐡍 (full.len != file[i].name.len) {
                استمر;
            }

            𐡄𐡍 (ngx_strcmp(full.data, file[i].name.data) == 0) {
                أرجع &file[i];
            }
        }
    }

    file = ngx_list_push(&cycle->open_files);
    𐡄𐡍 (file == عدم) {
        أرجع عدم;
    }

    𐡄𐡍 (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } وإلا {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = عدم;
    file->data = عدم;

    أرجع file;
}


ساكن فراغ
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        𐡄𐡍 (i >= part->nelts) {
            𐡄𐡍 (part->next == عدم) {
                اكسر;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        𐡄𐡍 (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


فراغ ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    va_list  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    va_end(args);

    𐡄𐡍 (err) {
        p = ngx_log_errno(p, last, err);
    }

    𐡄𐡍 (cf->conf_file == عدم) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        أرجع;
    }

    𐡄𐡍 (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        أرجع;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->line);
}


حرف *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t        *value;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    𐡄𐡍 (*fp != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    value = cf->args->elts;

    𐡄𐡍 (ngx_strcasecmp(value[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } وإلا 𐡄𐡍 (ngx_strcasecmp(value[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } وإلا {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     value[1].data, cmd->name.data);
        أرجع NGX_CONF_ERROR;
    }

    𐡄𐡍 (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, fp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t        *field, *value;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    𐡄𐡍 (field->data) {
        أرجع "is duplicate";
    }

    value = cf->args->elts;

    *field = value[1];

    𐡄𐡍 (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, field);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t         *value, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    𐡄𐡍 (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_str_t));
        𐡄𐡍 (*a == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    s = ngx_array_push(*a);
    𐡄𐡍 (s == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    *s = value[1];

    𐡄𐡍 (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, s);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t         *value;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    𐡄𐡍 (*a == NGX_CONF_UNSET_PTR || *a == عدم) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_keyval_t));
        𐡄𐡍 (*a == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_push(*a);
    𐡄𐡍 (kv == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    kv->key = value[1];
    kv->value = value[2];

    𐡄𐡍 (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, kv);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_set_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    𐡄𐡍 (*np != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    value = cf->args->elts;
    *np = ngx_atoi(value[1].data, value[1].len);
    𐡄𐡍 (*np == NGX_ERROR) {
        أرجع "invalid number";
    }

    𐡄𐡍 (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, np);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    size_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    𐡄𐡍 (*sp != NGX_CONF_UNSET_SIZE) {
        أرجع "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_size(&value[1]);
    𐡄𐡍 (*sp == (size_t) NGX_ERROR) {
        أرجع "invalid value";
    }

    𐡄𐡍 (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, sp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    off_t            *op;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    𐡄𐡍 (*op != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    value = cf->args->elts;

    *op = ngx_parse_offset(&value[1]);
    𐡄𐡍 (*op == (off_t) NGX_ERROR) {
        أرجع "invalid value";
    }

    𐡄𐡍 (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, op);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    𐡄𐡍 (*msp != NGX_CONF_UNSET_MSEC) {
        أرجع "is duplicate";
    }

    value = cf->args->elts;

    *msp = ngx_parse_time(&value[1], 0);
    𐡄𐡍 (*msp == (ngx_msec_t) NGX_ERROR) {
        أرجع "invalid value";
    }

    𐡄𐡍 (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, msp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    time_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    𐡄𐡍 (*sp != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_time(&value[1], 1);
    𐡄𐡍 (*sp == (time_t) NGX_ERROR) {
        أرجع "invalid value";
    }

    𐡄𐡍 (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, sp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف *p = conf;

    ngx_str_t   *value;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    𐡄𐡍 (bufs->num) {
        أرجع "is duplicate";
    }

    value = cf->args->elts;

    bufs->num = ngx_atoi(value[1].data, value[1].len);
    𐡄𐡍 (bufs->num == NGX_ERROR || bufs->num == 0) {
        أرجع "invalid value";
    }

    bufs->size = ngx_parse_size(&value[2]);
    𐡄𐡍 (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        أرجع "invalid value";
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *value;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    𐡄𐡍 (*np != NGX_CONF_UNSET_UINT) {
        أرجع "is duplicate";
    }

    value = cf->args->elts;
    e = cmd->post;

    لكل (i = 0; e[i].name.len != 0; i++) {
        𐡄𐡍 (e[i].name.len != value[1].len
            || ngx_strcasecmp(e[i].name.data, value[1].data) != 0)
        {
            استمر;
        }

        *np = e[i].value;

        أرجع NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", value[1].data);

    أرجع NGX_CONF_ERROR;
}


حرف *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *value;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    value = cf->args->elts;
    mask = cmd->post;

    لكل (i = 1; i < cf->args->nelts; i++) {
        لكل (m = 0; mask[m].name.len != 0; m++) {

            𐡄𐡍 (mask[m].name.len != value[i].len
                || ngx_strcasecmp(mask[m].name.data, value[i].data) != 0)
            {
                استمر;
            }

            𐡄𐡍 (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", value[i].data);

            } وإلا {
                *np |= mask[m].mask;
            }

            اكسر;
        }

        𐡄𐡍 (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", value[i].data);

            أرجع NGX_CONF_ERROR;
        }
    }

    أرجع NGX_CONF_OK;
}


#𐡄𐡍 0

حرف *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    أرجع "unsupported on this platform";
}

#endif


حرف *
ngx_conf_deprecated(ngx_conf_t *cf, فراغ *post, فراغ *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_check_num_bounds(ngx_conf_t *cf, فراغ *post, فراغ *data)
{
    ngx_conf_num_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    𐡄𐡍 (bounds->high == -1) {
        𐡄𐡍 (*np >= bounds->low) {
            أرجع NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->low);

        أرجع NGX_CONF_ERROR;
    }

    𐡄𐡍 (*np >= bounds->low && *np <= bounds->high) {
        أرجع NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->low, bounds->high);

    أرجع NGX_CONF_ERROR;
}


حرف *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, فراغ *post, فراغ *data)
{
    size_t  *sp = data;

    𐡄𐡍 (*sp == 0) {
        أرجع "value must be nonzero";
    }

    أرجع NGX_CONF_OK;
}
