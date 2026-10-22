
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাৱ <ngx_config.h>
#অন্তর্ভাৱ <ngx_core.h>

#define NGX_CONF_BUFFER  4096

স্থির ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
স্থির ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
স্থির ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
স্থির শূন্য ngx_conf_flush_files(ngx_cycle_t *cycle);


স্থির ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      রিক্ত },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    রিক্ত,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    রিক্ত,                                  /* init master */
    রিক্ত,                                  /* init module */
    রিক্ত,                                  /* init process */
    রিক্ত,                                  /* init thread */
    রিক্ত,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    রিক্ত,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

স্থির ngx_uint_t argument_সংখ্যা[] = {
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


ৱর্ণ *
ngx_conf_param(ngx_conf_t *cf)
{
    ৱর্ণ             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    যদি (param->len == 0) {
        ঘূৰাই NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, পরিমাণ(ngx_conf_file_t));

    ngx_memzero(&b, পরিমাণ(ngx_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.data = রিক্ত;
    conf_file.সাৰি = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    rv = ngx_conf_parse(cf, রিক্ত);

    cf->conf_file = রিক্ত;

    ঘূৰাই rv;
}


স্থির ngx_int_t
ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             size;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_গ্ৰন্থি_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_crc32_long(filename->data, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->cycle->config_dump_rbtree, filename, hash);

    যদি (sn) {
        cf->conf_file->dump = রিক্ত;
        ঘূৰাই NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    যদি (p == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    cd = ngx_array_ঠেলো(&cf->cycle->config_dump);
    যদি (cd == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    যদি (buf == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, পরিমাণ(ngx_str_গ্ৰন্থি_t));
    যদি (sn == রিক্ত) {
        ঘূৰাই NGX_ERROR;
    }

    sn->গ্ৰন্থি.চাবি = hash;
    sn->str = cd->name;

    ngx_rbtree_ঢোকাও(&cf->cycle->config_dump_rbtree, &sn->গ্ৰন্থি);

    ঘূৰাই NGX_OK;
}


ৱর্ণ *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    ৱর্ণ             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    গণনা {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#যদি (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = রিক্ত;
#endif

    যদি (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        যদি (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            ঘূৰাই NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        যদি (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        যদি (buf.start == রিক্ত) {
            গচ্ছ failed;
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
        cf->conf_file->সাৰি = 1;

        type = parse_file;

        যদি (ngx_dump_config
#যদি (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#যদি (NGX_DEBUG)
            || 1
#endif
           )
        {
            যদি (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                গচ্ছ failed;
            }

        } অন্যথা {
            cf->conf_file->dump = রিক্ত;
        }

    } অন্যথা যদি (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } অন্যথা {
        type = parse_param;
    }


    বাবে ( ;; ) {
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

        যদি (rc == NGX_ERROR) {
            গচ্ছ done;
        }

        যদি (rc == NGX_CONF_BLOCK_DONE) {

            যদি (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                গচ্ছ failed;
            }

            গচ্ছ done;
        }

        যদি (rc == NGX_CONF_FILE_DONE) {

            যদি (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                গচ্ছ failed;
            }

            গচ্ছ done;
        }

        যদি (rc == NGX_CONF_BLOCK_START) {

            যদি (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                গচ্ছ failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        যদি (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            যদি (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                গচ্ছ failed;
            }

            rv = (*cf->handler)(cf, রিক্ত, cf->handler_conf);
            যদি (rv == NGX_CONF_OK) {
                অনুৱর্ত;
            }

            যদি (rv == NGX_CONF_ERROR) {
                গচ্ছ failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            গচ্ছ failed;
        }


        rc = ngx_conf_handler(cf, rc);

        যদি (rc == NGX_ERROR) {
            গচ্ছ failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    যদি (filename) {
        যদি (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    যদি (rc == NGX_ERROR) {
        ঘূৰাই NGX_CONF_ERROR;
    }

    ঘূৰাই NGX_CONF_OK;
}


স্থির ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    ৱর্ণ           *rv;
    শূন্য           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    বাবে (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        যদি (cmd == রিক্ত) {
            অনুৱর্ত;
        }

        বাবে ( /* void */ ; cmd->name.len; cmd++) {

            যদি (name->len != cmd->name.len) {
                অনুৱর্ত;
            }

            যদি (ngx_strcmp(name->data, cmd->name.data) != 0) {
                অনুৱর্ত;
            }

            found = 1;

            যদি (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                অনুৱর্ত;
            }

            /* is the directive's location right ? */

            যদি (!(cmd->type & cf->cmd_type)) {
                অনুৱর্ত;
            }

            যদি (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                ঘূৰাই NGX_ERROR;
            }

            যদি ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                ঘূৰাই NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            যদি (!(cmd->type & NGX_CONF_ANY)) {

                যদি (cmd->type & NGX_CONF_FLAG) {

                    যদি (cf->args->nelts != 2) {
                        গচ্ছ invalid;
                    }

                } অন্যথা যদি (cmd->type & NGX_CONF_1MORE) {

                    যদি (cf->args->nelts < 2) {
                        গচ্ছ invalid;
                    }

                } অন্যথা যদি (cmd->type & NGX_CONF_2MORE) {

                    যদি (cf->args->nelts < 3) {
                        গচ্ছ invalid;
                    }

                } অন্যথা যদি (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    গচ্ছ invalid;

                } অন্যথা যদি (!(cmd->type & argument_সংখ্যা[cf->args->nelts - 1]))
                {
                    গচ্ছ invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = রিক্ত;

            যদি (cmd->type & NGX_DIRECT_CONF) {
                conf = ((শূন্য **) cf->ctx)[cf->cycle->modules[i]->index];

            } অন্যথা যদি (cmd->type & NGX_MAIN_CONF) {
                conf = &(((শূন্য **) cf->ctx)[cf->cycle->modules[i]->index]);

            } অন্যথা যদি (cf->ctx) {
                confp = *(শূন্য **) ((ৱর্ণ *) cf->ctx + cmd->conf);

                যদি (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            যদি (rv == NGX_CONF_OK) {
                ঘূৰাই NGX_OK;
            }

            যদি (rv == NGX_CONF_ERROR) {
                ঘূৰাই NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            ঘূৰাই NGX_ERROR;
        }
    }

    যদি (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        ঘূৰাই NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    ঘূৰাই NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    ঘূৰাই NGX_ERROR;
}


স্থির ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    size_t       len;
    ssize_t      n, size;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_সাৰি;
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
    start_সাৰি = cf->conf_file->সাৰি;

    file_size = ngx_file_size(&cf->conf_file->file.info);

    বাবে ( ;; ) {

        যদি (b->pos >= b->last) {

            যদি (cf->conf_file->file.offset >= file_size) {

                যদি (cf->args->nelts > 0 || !last_space) {

                    যদি (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        ঘূৰাই NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    ঘূৰাই NGX_ERROR;
                }

                ঘূৰাই NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            যদি (len == NGX_CONF_BUFFER) {
                cf->conf_file->সাৰি = start_সাৰি;

                যদি (d_quoted) {
                    ch = '"';

                } অন্যথা যদি (s_quoted) {
                    ch = '\'';

                } অন্যথা {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    ঘূৰাই NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                ঘূৰাই NGX_ERROR;
            }

            যদি (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            যদি (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            যদি (n == NGX_ERROR) {
                ঘূৰাই NGX_ERROR;
            }

            যদি (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                ঘূৰাই NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            যদি (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        যদি (ch == LF) {
            cf->conf_file->সাৰি++;

            যদি (sharp_comment) {
                sharp_comment = 0;
            }
        }

        যদি (sharp_comment) {
            অনুৱর্ত;
        }

        যদি (quoted) {
            quoted = 0;
            অনুৱর্ত;
        }

        যদি (need_space) {
            যদি (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                অনুৱর্ত;
            }

            যদি (ch == ';') {
                ঘূৰাই NGX_OK;
            }

            যদি (ch == '{') {
                ঘূৰাই NGX_CONF_BLOCK_START;
            }

            যদি (ch == ')') {
                last_space = 1;
                need_space = 0;

            } অন্যথা {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                ঘূৰাই NGX_ERROR;
            }
        }

        যদি (last_space) {

            start = b->pos - 1;
            start_সাৰি = cf->conf_file->সাৰি;

            যদি (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                অনুৱর্ত;
            }

            ৱিকল্প (ch) {

            পক্ষ ';':
            পক্ষ '{':
                যদি (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    ঘূৰাই NGX_ERROR;
                }

                যদি (ch == '{') {
                    ঘূৰাই NGX_CONF_BLOCK_START;
                }

                ঘূৰাই NGX_OK;

            পক্ষ '}':
                যদি (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    ঘূৰাই NGX_ERROR;
                }

                ঘূৰাই NGX_CONF_BLOCK_DONE;

            পক্ষ '#':
                sharp_comment = 1;
                অনুৱর্ত;

            পক্ষ '\\':
                quoted = 1;
                last_space = 0;
                অনুৱর্ত;

            পক্ষ '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                অনুৱর্ত;

            পক্ষ '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                অনুৱর্ত;

            পক্ষ '$':
                variable = 1;
                last_space = 0;
                অনুৱর্ত;

            স্ৱভাৱ:
                last_space = 0;
            }

        } অন্যথা {
            যদি (ch == '{' && variable) {
                অনুৱর্ত;
            }

            variable = 0;

            যদি (ch == '\\') {
                quoted = 1;
                অনুৱর্ত;
            }

            যদি (ch == '$') {
                variable = 1;
                অনুৱর্ত;
            }

            যদি (d_quoted) {
                যদি (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } অন্যথা যদি (s_quoted) {
                যদি (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } অন্যথা যদি (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            যদি (found) {
                word = ngx_array_ঠেলো(cf->args);
                যদি (word == রিক্ত) {
                    ঘূৰাই NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                যদি (word->data == রিক্ত) {
                    ঘূৰাই NGX_ERROR;
                }

                বাবে (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    যদি (*src == '\\') {
                        ৱিকল্প (src[1]) {
                        পক্ষ '"':
                        পক্ষ '\'':
                        পক্ষ '\\':
                            src++;
                            ভঙ্গ;

                        পক্ষ 't':
                            *dst++ = '\t';
                            src += 2;
                            অনুৱর্ত;

                        পক্ষ 'r':
                            *dst++ = '\r';
                            src += 2;
                            অনুৱর্ত;

                        পক্ষ 'n':
                            *dst++ = '\n';
                            src += 2;
                            অনুৱর্ত;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                যদি (ch == ';') {
                    ঘূৰাই NGX_OK;
                }

                যদি (ch == '{') {
                    ঘূৰাই NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


ৱর্ণ *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ৱর্ণ        *rv;
    ngx_int_t    n;
    ngx_str_t   *মান, file, name;
    ngx_glob_t   gl;

    মান = cf->args->elts;
    file = মান[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    যদি (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        ঘূৰাই NGX_CONF_ERROR;
    }

    যদি (strpbrk((ৱর্ণ *) file.data, "*?[") == রিক্ত) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        ঘূৰাই ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, পরিমাণ(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    যদি (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        ঘূৰাই NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    বাবে ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        যদি (n != NGX_OK) {
            ভঙ্গ;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        যদি (file.data == রিক্ত) {
            ঘূৰাই NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        যদি (rv != NGX_CONF_OK) {
            ভঙ্গ;
        }
    }

    ngx_close_glob(&gl);

    ঘূৰাই rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    ঘূৰাই ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#যদি (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    যদি (name->len) {
        full = *name;

        যদি (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            ঘূৰাই রিক্ত;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        বাবে (i = 0; /* void */ ; i++) {

            যদি (i >= part->nelts) {
                যদি (part->next == রিক্ত) {
                    ভঙ্গ;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            যদি (full.len != file[i].name.len) {
                অনুৱর্ত;
            }

            যদি (ngx_strcmp(full.data, file[i].name.data) == 0) {
                ঘূৰাই &file[i];
            }
        }
    }

    file = ngx_list_ঠেলো(&cycle->open_files);
    যদি (file == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    যদি (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } অন্যথা {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = রিক্ত;
    file->data = রিক্ত;

    ঘূৰাই file;
}


স্থির শূন্য
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    বাবে (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->next == রিক্ত) {
                ভঙ্গ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        যদি (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


শূন্য ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ধ্রুৱ ৱর্ণ *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    va_list  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    va_end(args);

    যদি (err) {
        p = ngx_log_errno(p, last, err);
    }

    যদি (cf->conf_file == রিক্ত) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        ঘূৰাই;
    }

    যদি (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        ঘূৰাই;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->সাৰি);
}


ৱর্ণ *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ৱর্ণ  *p = conf;

    ngx_str_t        *মান;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    যদি (*fp != NGX_CONF_UNSET) {
        ঘূৰাই "is duplicate";
    }

    মান = cf->args->elts;

    যদি (ngx_strcasecmp(মান[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } অন্যথা যদি (ngx_strcasecmp(মান[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } অন্যথা {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     মান[1].data, cmd->name.data);
        ঘূৰাই NGX_CONF_ERROR;
    }

    যদি (cmd->post) {
        post = cmd->post;
        ঘূৰাই post->post_handler(cf, post, fp);
    }

    ঘূৰাই NGX_CONF_OK;
}


ৱর্ণ *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ৱর্ণ  *p = conf;

    ngx_str_t        *field, *মান;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    যদি (field->data) {
        ঘূৰাই "is duplicate";
    }

    মান = cf->args->elts;

    *field = মান[1];

    যদি (cmd->post) {
        post = cmd->post;
        ঘূৰাই post->post_handler(cf, post, field);
    }

    ঘূৰাই NGX_CONF_OK;
}


ৱর্ণ *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ৱর্ণ  *p = conf;

    ngx_str_t         *মান, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    যদি (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, পরিমাণ(ngx_str_t));
        যদি (*a == রিক্ত) {
            ঘূৰাই NGX_CONF_ERROR;
        }
    }

    s = ngx_array_ঠেলো(*a);
    যদি (s == রিক্ত) {
        ঘূৰাই NGX_CONF_ERROR;
    }

    মান = cf->args->elts;

    *s = মান[1];

    যদি (cmd->post) {
        post = cmd->post;
        ঘূৰাই post->post_handler(cf, post, s);
    }

    ঘূৰাই NGX_CONF_OK;
}


ৱর্ণ *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ৱর্ণ  *p = conf;

    ngx_str_t         *মান;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    যদি (*a == NGX_CONF_UNSET_PTR || *a == রিক্ত) {
        *a = ngx_array_create(cf->pool, 4, পরিমাণ(ngx_keyval_t));
        যদি (*a == রিক্ত) {
            ঘূৰাই NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_ঠেলো(*a);
    যদি (kv == রিক্ত) {
        ঘূৰাই NGX_CONF_ERROR;
    }

    মান = cf->args->elts;

    kv->চাবি = মান[1];
    kv->মান = মান[2];

    যদি (cmd->post) {
        post = cmd->post;
        ঘূৰাই post->post_handler(cf, post, kv);
    }

    ঘূৰাই NGX_CONF_OK;
}


ৱর্ণ *
ngx_conf_set_অঙ্ক_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ৱর্ণ  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *মান;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    যদি (*np != NGX_CONF_UNSET) {
        ঘূৰাই "is duplicate";
    }

    মান = cf->args->elts;
    *np = ngx_atoi(মান[1].data, মান[1].len);
    যদি (*np == NGX_ERROR) {
        ঘূৰাই "invalid number";
    }

    যদি (cmd->post) {
        post = cmd->post;
        ঘূৰাই post->post_handler(cf, post, np);
    }

    ঘূৰাই NGX_CONF_OK;
}


ৱর্ণ *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ৱর্ণ  *p = conf;

    size_t           *sp;
    ngx_str_t        *মান;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    যদি (*sp != NGX_CONF_UNSET_SIZE) {
        ঘূৰাই "is duplicate";
    }

    মান = cf->args->elts;

    *sp = ngx_parse_size(&মান[1]);
    যদি (*sp == (size_t) NGX_ERROR) {
        ঘূৰাই "invalid value";
    }

    যদি (cmd->post) {
        post = cmd->post;
        ঘূৰাই post->post_handler(cf, post, sp);
    }

    ঘূৰাই NGX_CONF_OK;
}


ৱর্ণ *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ৱর্ণ  *p = conf;

    off_t            *op;
    ngx_str_t        *মান;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    যদি (*op != NGX_CONF_UNSET) {
        ঘূৰাই "is duplicate";
    }

    মান = cf->args->elts;

    *op = ngx_parse_offset(&মান[1]);
    যদি (*op == (off_t) NGX_ERROR) {
        ঘূৰাই "invalid value";
    }

    যদি (cmd->post) {
        post = cmd->post;
        ঘূৰাই post->post_handler(cf, post, op);
    }

    ঘূৰাই NGX_CONF_OK;
}


ৱর্ণ *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ৱর্ণ  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *মান;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    যদি (*msp != NGX_CONF_UNSET_MSEC) {
        ঘূৰাই "is duplicate";
    }

    মান = cf->args->elts;

    *msp = ngx_parse_time(&মান[1], 0);
    যদি (*msp == (ngx_msec_t) NGX_ERROR) {
        ঘূৰাই "invalid value";
    }

    যদি (cmd->post) {
        post = cmd->post;
        ঘূৰাই post->post_handler(cf, post, msp);
    }

    ঘূৰাই NGX_CONF_OK;
}


ৱর্ণ *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ৱর্ণ  *p = conf;

    time_t           *sp;
    ngx_str_t        *মান;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    যদি (*sp != NGX_CONF_UNSET) {
        ঘূৰাই "is duplicate";
    }

    মান = cf->args->elts;

    *sp = ngx_parse_time(&মান[1], 1);
    যদি (*sp == (time_t) NGX_ERROR) {
        ঘূৰাই "invalid value";
    }

    যদি (cmd->post) {
        post = cmd->post;
        ঘূৰাই post->post_handler(cf, post, sp);
    }

    ঘূৰাই NGX_CONF_OK;
}


ৱর্ণ *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ৱর্ণ *p = conf;

    ngx_str_t   *মান;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    যদি (bufs->অঙ্ক) {
        ঘূৰাই "is duplicate";
    }

    মান = cf->args->elts;

    bufs->অঙ্ক = ngx_atoi(মান[1].data, মান[1].len);
    যদি (bufs->অঙ্ক == NGX_ERROR || bufs->অঙ্ক == 0) {
        ঘূৰাই "invalid value";
    }

    bufs->size = ngx_parse_size(&মান[2]);
    যদি (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        ঘূৰাই "invalid value";
    }

    ঘূৰাই NGX_CONF_OK;
}


ৱর্ণ *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ৱর্ণ  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *মান;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    যদি (*np != NGX_CONF_UNSET_UINT) {
        ঘূৰাই "is duplicate";
    }

    মান = cf->args->elts;
    e = cmd->post;

    বাবে (i = 0; e[i].name.len != 0; i++) {
        যদি (e[i].name.len != মান[1].len
            || ngx_strcasecmp(e[i].name.data, মান[1].data) != 0)
        {
            অনুৱর্ত;
        }

        *np = e[i].মান;

        ঘূৰাই NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", মান[1].data);

    ঘূৰাই NGX_CONF_ERROR;
}


ৱর্ণ *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ৱর্ণ  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *মান;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    মান = cf->args->elts;
    mask = cmd->post;

    বাবে (i = 1; i < cf->args->nelts; i++) {
        বাবে (m = 0; mask[m].name.len != 0; m++) {

            যদি (mask[m].name.len != মান[i].len
                || ngx_strcasecmp(mask[m].name.data, মান[i].data) != 0)
            {
                অনুৱর্ত;
            }

            যদি (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", মান[i].data);

            } অন্যথা {
                *np |= mask[m].mask;
            }

            ভঙ্গ;
        }

        যদি (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", মান[i].data);

            ঘূৰাই NGX_CONF_ERROR;
        }
    }

    ঘূৰাই NGX_CONF_OK;
}


#যদি 0

ৱর্ণ *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ঘূৰাই "unsupported on this platform";
}

#endif


ৱর্ণ *
ngx_conf_deprecated(ngx_conf_t *cf, শূন্য *post, শূন্য *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    ঘূৰাই NGX_CONF_OK;
}


ৱর্ণ *
ngx_conf_check_অঙ্ক_bounds(ngx_conf_t *cf, শূন্য *post, শূন্য *data)
{
    ngx_conf_অঙ্ক_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    যদি (bounds->উপৰেৰ == -1) {
        যদি (*np >= bounds->নিচেৰ) {
            ঘূৰাই NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->নিচেৰ);

        ঘূৰাই NGX_CONF_ERROR;
    }

    যদি (*np >= bounds->নিচেৰ && *np <= bounds->উপৰেৰ) {
        ঘূৰাই NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->নিচেৰ, bounds->উপৰেৰ);

    ঘূৰাই NGX_CONF_ERROR;
}


ৱর্ণ *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, শূন্য *post, শূন্য *data)
{
    size_t  *sp = data;

    যদি (*sp == 0) {
        ঘূৰাই "value must be nonzero";
    }

    ঘূৰাই NGX_CONF_OK;
}
