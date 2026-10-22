
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>

#define NGX_CONF_BUFFER  4096

ایستا ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
ایستا ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
ایستا ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
ایستا تهی ngx_conf_flush_files(ngx_cycle_t *cycle);


ایستا ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      پوچ },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    پوچ,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    پوچ,                                  /* init master */
    پوچ,                                  /* init module */
    پوچ,                                  /* init process */
    پوچ,                                  /* init thread */
    پوچ,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    پوچ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

ایستا ngx_uint_t argument_عدد[] = {
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


نویسه *
ngx_conf_param(ngx_conf_t *cf)
{
    نویسه             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    اگر (param->len == 0) {
        بازگشت NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, اندازه(ngx_conf_file_t));

    ngx_memzero(&b, اندازه(ngx_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.data = پوچ;
    conf_file.سطر = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    rv = ngx_conf_parse(cf, پوچ);

    cf->conf_file = پوچ;

    بازگشت rv;
}


ایستا ngx_int_t
ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             size;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_گره_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_crc32_long(filename->data, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->cycle->config_dump_rbtree, filename, hash);

    اگر (sn) {
        cf->conf_file->dump = پوچ;
        بازگشت NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    اگر (p == پوچ) {
        بازگشت NGX_ERROR;
    }

    cd = ngx_array_افزودن(&cf->cycle->config_dump);
    اگر (cd == پوچ) {
        بازگشت NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    اگر (buf == پوچ) {
        بازگشت NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, اندازه(ngx_str_گره_t));
    اگر (sn == پوچ) {
        بازگشت NGX_ERROR;
    }

    sn->گره.کلید = hash;
    sn->str = cd->name;

    ngx_rbtree_درج(&cf->cycle->config_dump_rbtree, &sn->گره);

    بازگشت NGX_OK;
}


نویسه *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    نویسه             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    شمارش {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#اگر (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = پوچ;
#endif

    اگر (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        اگر (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            بازگشت NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        اگر (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        اگر (buf.start == پوچ) {
            برو failed;
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
        cf->conf_file->سطر = 1;

        type = parse_file;

        اگر (ngx_dump_config
#اگر (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#اگر (NGX_DEBUG)
            || 1
#endif
           )
        {
            اگر (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                برو failed;
            }

        } وگرنه {
            cf->conf_file->dump = پوچ;
        }

    } وگرنه اگر (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } وگرنه {
        type = parse_param;
    }


    برای ( ;; ) {
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

        اگر (rc == NGX_ERROR) {
            برو done;
        }

        اگر (rc == NGX_CONF_BLOCK_DONE) {

            اگر (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                برو failed;
            }

            برو done;
        }

        اگر (rc == NGX_CONF_FILE_DONE) {

            اگر (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                برو failed;
            }

            برو done;
        }

        اگر (rc == NGX_CONF_BLOCK_START) {

            اگر (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                برو failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        اگر (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            اگر (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                برو failed;
            }

            rv = (*cf->handler)(cf, پوچ, cf->handler_conf);
            اگر (rv == NGX_CONF_OK) {
                ادامه;
            }

            اگر (rv == NGX_CONF_ERROR) {
                برو failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            برو failed;
        }


        rc = ngx_conf_handler(cf, rc);

        اگر (rc == NGX_ERROR) {
            برو failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    اگر (filename) {
        اگر (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    اگر (rc == NGX_ERROR) {
        بازگشت NGX_CONF_ERROR;
    }

    بازگشت NGX_CONF_OK;
}


ایستا ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    نویسه           *rv;
    تهی           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    برای (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        اگر (cmd == پوچ) {
            ادامه;
        }

        برای ( /* void */ ; cmd->name.len; cmd++) {

            اگر (name->len != cmd->name.len) {
                ادامه;
            }

            اگر (ngx_strcmp(name->data, cmd->name.data) != 0) {
                ادامه;
            }

            found = 1;

            اگر (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                ادامه;
            }

            /* is the directive's location right ? */

            اگر (!(cmd->type & cf->cmd_type)) {
                ادامه;
            }

            اگر (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                بازگشت NGX_ERROR;
            }

            اگر ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                بازگشت NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            اگر (!(cmd->type & NGX_CONF_ANY)) {

                اگر (cmd->type & NGX_CONF_FLAG) {

                    اگر (cf->args->nelts != 2) {
                        برو invalid;
                    }

                } وگرنه اگر (cmd->type & NGX_CONF_1MORE) {

                    اگر (cf->args->nelts < 2) {
                        برو invalid;
                    }

                } وگرنه اگر (cmd->type & NGX_CONF_2MORE) {

                    اگر (cf->args->nelts < 3) {
                        برو invalid;
                    }

                } وگرنه اگر (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    برو invalid;

                } وگرنه اگر (!(cmd->type & argument_عدد[cf->args->nelts - 1]))
                {
                    برو invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = پوچ;

            اگر (cmd->type & NGX_DIRECT_CONF) {
                conf = ((تهی **) cf->ctx)[cf->cycle->modules[i]->index];

            } وگرنه اگر (cmd->type & NGX_MAIN_CONF) {
                conf = &(((تهی **) cf->ctx)[cf->cycle->modules[i]->index]);

            } وگرنه اگر (cf->ctx) {
                confp = *(تهی **) ((نویسه *) cf->ctx + cmd->conf);

                اگر (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            اگر (rv == NGX_CONF_OK) {
                بازگشت NGX_OK;
            }

            اگر (rv == NGX_CONF_ERROR) {
                بازگشت NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            بازگشت NGX_ERROR;
        }
    }

    اگر (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        بازگشت NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    بازگشت NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    بازگشت NGX_ERROR;
}


ایستا ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    size_t       len;
    ssize_t      n, size;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_سطر;
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
    start_سطر = cf->conf_file->سطر;

    file_size = ngx_file_size(&cf->conf_file->file.info);

    برای ( ;; ) {

        اگر (b->pos >= b->last) {

            اگر (cf->conf_file->file.offset >= file_size) {

                اگر (cf->args->nelts > 0 || !last_space) {

                    اگر (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        بازگشت NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    بازگشت NGX_ERROR;
                }

                بازگشت NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            اگر (len == NGX_CONF_BUFFER) {
                cf->conf_file->سطر = start_سطر;

                اگر (d_quoted) {
                    ch = '"';

                } وگرنه اگر (s_quoted) {
                    ch = '\'';

                } وگرنه {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    بازگشت NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                بازگشت NGX_ERROR;
            }

            اگر (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            اگر (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            اگر (n == NGX_ERROR) {
                بازگشت NGX_ERROR;
            }

            اگر (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                بازگشت NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            اگر (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        اگر (ch == LF) {
            cf->conf_file->سطر++;

            اگر (sharp_comment) {
                sharp_comment = 0;
            }
        }

        اگر (sharp_comment) {
            ادامه;
        }

        اگر (quoted) {
            quoted = 0;
            ادامه;
        }

        اگر (need_space) {
            اگر (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                ادامه;
            }

            اگر (ch == ';') {
                بازگشت NGX_OK;
            }

            اگر (ch == '{') {
                بازگشت NGX_CONF_BLOCK_START;
            }

            اگر (ch == ')') {
                last_space = 1;
                need_space = 0;

            } وگرنه {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                بازگشت NGX_ERROR;
            }
        }

        اگر (last_space) {

            start = b->pos - 1;
            start_سطر = cf->conf_file->سطر;

            اگر (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                ادامه;
            }

            انتخاب (ch) {

            حالت ';':
            حالت '{':
                اگر (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    بازگشت NGX_ERROR;
                }

                اگر (ch == '{') {
                    بازگشت NGX_CONF_BLOCK_START;
                }

                بازگشت NGX_OK;

            حالت '}':
                اگر (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    بازگشت NGX_ERROR;
                }

                بازگشت NGX_CONF_BLOCK_DONE;

            حالت '#':
                sharp_comment = 1;
                ادامه;

            حالت '\\':
                quoted = 1;
                last_space = 0;
                ادامه;

            حالت '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                ادامه;

            حالت '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                ادامه;

            حالت '$':
                variable = 1;
                last_space = 0;
                ادامه;

            پیشفرض:
                last_space = 0;
            }

        } وگرنه {
            اگر (ch == '{' && variable) {
                ادامه;
            }

            variable = 0;

            اگر (ch == '\\') {
                quoted = 1;
                ادامه;
            }

            اگر (ch == '$') {
                variable = 1;
                ادامه;
            }

            اگر (d_quoted) {
                اگر (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } وگرنه اگر (s_quoted) {
                اگر (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } وگرنه اگر (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            اگر (found) {
                word = ngx_array_افزودن(cf->args);
                اگر (word == پوچ) {
                    بازگشت NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                اگر (word->data == پوچ) {
                    بازگشت NGX_ERROR;
                }

                برای (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    اگر (*src == '\\') {
                        انتخاب (src[1]) {
                        حالت '"':
                        حالت '\'':
                        حالت '\\':
                            src++;
                            بشکن;

                        حالت 't':
                            *dst++ = '\t';
                            src += 2;
                            ادامه;

                        حالت 'r':
                            *dst++ = '\r';
                            src += 2;
                            ادامه;

                        حالت 'n':
                            *dst++ = '\n';
                            src += 2;
                            ادامه;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                اگر (ch == ';') {
                    بازگشت NGX_OK;
                }

                اگر (ch == '{') {
                    بازگشت NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


نویسه *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه        *rv;
    ngx_int_t    n;
    ngx_str_t   *مقدار, file, name;
    ngx_glob_t   gl;

    مقدار = cf->args->elts;
    file = مقدار[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    اگر (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        بازگشت NGX_CONF_ERROR;
    }

    اگر (strpbrk((نویسه *) file.data, "*?[") == پوچ) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        بازگشت ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, اندازه(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    اگر (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        بازگشت NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    برای ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        اگر (n != NGX_OK) {
            بشکن;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        اگر (file.data == پوچ) {
            بازگشت NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        اگر (rv != NGX_CONF_OK) {
            بشکن;
        }
    }

    ngx_close_glob(&gl);

    بازگشت rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    بازگشت ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#اگر (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    اگر (name->len) {
        full = *name;

        اگر (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            بازگشت پوچ;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        برای (i = 0; /* void */ ; i++) {

            اگر (i >= part->nelts) {
                اگر (part->next == پوچ) {
                    بشکن;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            اگر (full.len != file[i].name.len) {
                ادامه;
            }

            اگر (ngx_strcmp(full.data, file[i].name.data) == 0) {
                بازگشت &file[i];
            }
        }
    }

    file = ngx_list_افزودن(&cycle->open_files);
    اگر (file == پوچ) {
        بازگشت پوچ;
    }

    اگر (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } وگرنه {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = پوچ;
    file->data = پوچ;

    بازگشت file;
}


ایستا تهی
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    برای (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->next == پوچ) {
                بشکن;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        اگر (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


تهی ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ثابت نویسه *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    va_list  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    va_end(args);

    اگر (err) {
        p = ngx_log_errno(p, last, err);
    }

    اگر (cf->conf_file == پوچ) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        بازگشت;
    }

    اگر (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        بازگشت;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->سطر);
}


نویسه *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه  *p = conf;

    ngx_str_t        *مقدار;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    اگر (*fp != NGX_CONF_UNSET) {
        بازگشت "is duplicate";
    }

    مقدار = cf->args->elts;

    اگر (ngx_strcasecmp(مقدار[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } وگرنه اگر (ngx_strcasecmp(مقدار[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } وگرنه {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     مقدار[1].data, cmd->name.data);
        بازگشت NGX_CONF_ERROR;
    }

    اگر (cmd->post) {
        post = cmd->post;
        بازگشت post->post_handler(cf, post, fp);
    }

    بازگشت NGX_CONF_OK;
}


نویسه *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه  *p = conf;

    ngx_str_t        *field, *مقدار;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    اگر (field->data) {
        بازگشت "is duplicate";
    }

    مقدار = cf->args->elts;

    *field = مقدار[1];

    اگر (cmd->post) {
        post = cmd->post;
        بازگشت post->post_handler(cf, post, field);
    }

    بازگشت NGX_CONF_OK;
}


نویسه *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه  *p = conf;

    ngx_str_t         *مقدار, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    اگر (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, اندازه(ngx_str_t));
        اگر (*a == پوچ) {
            بازگشت NGX_CONF_ERROR;
        }
    }

    s = ngx_array_افزودن(*a);
    اگر (s == پوچ) {
        بازگشت NGX_CONF_ERROR;
    }

    مقدار = cf->args->elts;

    *s = مقدار[1];

    اگر (cmd->post) {
        post = cmd->post;
        بازگشت post->post_handler(cf, post, s);
    }

    بازگشت NGX_CONF_OK;
}


نویسه *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه  *p = conf;

    ngx_str_t         *مقدار;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    اگر (*a == NGX_CONF_UNSET_PTR || *a == پوچ) {
        *a = ngx_array_create(cf->pool, 4, اندازه(ngx_keyval_t));
        اگر (*a == پوچ) {
            بازگشت NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_افزودن(*a);
    اگر (kv == پوچ) {
        بازگشت NGX_CONF_ERROR;
    }

    مقدار = cf->args->elts;

    kv->کلید = مقدار[1];
    kv->مقدار = مقدار[2];

    اگر (cmd->post) {
        post = cmd->post;
        بازگشت post->post_handler(cf, post, kv);
    }

    بازگشت NGX_CONF_OK;
}


نویسه *
ngx_conf_set_رقم_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *مقدار;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    اگر (*np != NGX_CONF_UNSET) {
        بازگشت "is duplicate";
    }

    مقدار = cf->args->elts;
    *np = ngx_atoi(مقدار[1].data, مقدار[1].len);
    اگر (*np == NGX_ERROR) {
        بازگشت "invalid number";
    }

    اگر (cmd->post) {
        post = cmd->post;
        بازگشت post->post_handler(cf, post, np);
    }

    بازگشت NGX_CONF_OK;
}


نویسه *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه  *p = conf;

    size_t           *sp;
    ngx_str_t        *مقدار;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    اگر (*sp != NGX_CONF_UNSET_SIZE) {
        بازگشت "is duplicate";
    }

    مقدار = cf->args->elts;

    *sp = ngx_parse_size(&مقدار[1]);
    اگر (*sp == (size_t) NGX_ERROR) {
        بازگشت "invalid value";
    }

    اگر (cmd->post) {
        post = cmd->post;
        بازگشت post->post_handler(cf, post, sp);
    }

    بازگشت NGX_CONF_OK;
}


نویسه *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه  *p = conf;

    off_t            *op;
    ngx_str_t        *مقدار;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    اگر (*op != NGX_CONF_UNSET) {
        بازگشت "is duplicate";
    }

    مقدار = cf->args->elts;

    *op = ngx_parse_offset(&مقدار[1]);
    اگر (*op == (off_t) NGX_ERROR) {
        بازگشت "invalid value";
    }

    اگر (cmd->post) {
        post = cmd->post;
        بازگشت post->post_handler(cf, post, op);
    }

    بازگشت NGX_CONF_OK;
}


نویسه *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *مقدار;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    اگر (*msp != NGX_CONF_UNSET_MSEC) {
        بازگشت "is duplicate";
    }

    مقدار = cf->args->elts;

    *msp = ngx_parse_time(&مقدار[1], 0);
    اگر (*msp == (ngx_msec_t) NGX_ERROR) {
        بازگشت "invalid value";
    }

    اگر (cmd->post) {
        post = cmd->post;
        بازگشت post->post_handler(cf, post, msp);
    }

    بازگشت NGX_CONF_OK;
}


نویسه *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه  *p = conf;

    time_t           *sp;
    ngx_str_t        *مقدار;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    اگر (*sp != NGX_CONF_UNSET) {
        بازگشت "is duplicate";
    }

    مقدار = cf->args->elts;

    *sp = ngx_parse_time(&مقدار[1], 1);
    اگر (*sp == (time_t) NGX_ERROR) {
        بازگشت "invalid value";
    }

    اگر (cmd->post) {
        post = cmd->post;
        بازگشت post->post_handler(cf, post, sp);
    }

    بازگشت NGX_CONF_OK;
}


نویسه *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه *p = conf;

    ngx_str_t   *مقدار;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    اگر (bufs->رقم) {
        بازگشت "is duplicate";
    }

    مقدار = cf->args->elts;

    bufs->رقم = ngx_atoi(مقدار[1].data, مقدار[1].len);
    اگر (bufs->رقم == NGX_ERROR || bufs->رقم == 0) {
        بازگشت "invalid value";
    }

    bufs->size = ngx_parse_size(&مقدار[2]);
    اگر (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        بازگشت "invalid value";
    }

    بازگشت NGX_CONF_OK;
}


نویسه *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *مقدار;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    اگر (*np != NGX_CONF_UNSET_UINT) {
        بازگشت "is duplicate";
    }

    مقدار = cf->args->elts;
    e = cmd->post;

    برای (i = 0; e[i].name.len != 0; i++) {
        اگر (e[i].name.len != مقدار[1].len
            || ngx_strcasecmp(e[i].name.data, مقدار[1].data) != 0)
        {
            ادامه;
        }

        *np = e[i].مقدار;

        بازگشت NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", مقدار[1].data);

    بازگشت NGX_CONF_ERROR;
}


نویسه *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *مقدار;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    مقدار = cf->args->elts;
    mask = cmd->post;

    برای (i = 1; i < cf->args->nelts; i++) {
        برای (m = 0; mask[m].name.len != 0; m++) {

            اگر (mask[m].name.len != مقدار[i].len
                || ngx_strcasecmp(mask[m].name.data, مقدار[i].data) != 0)
            {
                ادامه;
            }

            اگر (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", مقدار[i].data);

            } وگرنه {
                *np |= mask[m].mask;
            }

            بشکن;
        }

        اگر (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", مقدار[i].data);

            بازگشت NGX_CONF_ERROR;
        }
    }

    بازگشت NGX_CONF_OK;
}


#اگر 0

نویسه *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    بازگشت "unsupported on this platform";
}

#endif


نویسه *
ngx_conf_deprecated(ngx_conf_t *cf, تهی *post, تهی *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    بازگشت NGX_CONF_OK;
}


نویسه *
ngx_conf_check_رقم_bounds(ngx_conf_t *cf, تهی *post, تهی *data)
{
    ngx_conf_رقم_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    اگر (bounds->بالا == -1) {
        اگر (*np >= bounds->کم) {
            بازگشت NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->کم);

        بازگشت NGX_CONF_ERROR;
    }

    اگر (*np >= bounds->کم && *np <= bounds->بالا) {
        بازگشت NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->کم, bounds->بالا);

    بازگشت NGX_CONF_ERROR;
}


نویسه *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, تهی *post, تهی *data)
{
    size_t  *sp = data;

    اگر (*sp == 0) {
        بازگشت "value must be nonzero";
    }

    بازگشت NGX_CONF_OK;
}
