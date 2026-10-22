
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>

#define NGX_CONF_BUFFER  4096

ಸ್ಥಿರ ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
ಸ್ಥಿರ ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
ಸ್ಥಿರ ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_conf_flush_files(ngx_cycle_t *cycle);


ಸ್ಥಿರ ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      ರಿಕ್ತ },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    ರಿಕ್ತ,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    ರಿಕ್ತ,                                  /* init master */
    ರಿಕ್ತ,                                  /* init module */
    ರಿಕ್ತ,                                  /* init process */
    ರಿಕ್ತ,                                  /* init thread */
    ರಿಕ್ತ,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    ರಿಕ್ತ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

ಸ್ಥಿರ ngx_uint_t argument_ಸಂಖ್ಯೆ[] = {
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


ವರ್ಣ *
ngx_conf_param(ngx_conf_t *cf)
{
    ವರ್ಣ             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    ಯದಿ (param->len == 0) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, ಪರಿಮಾಣ(ngx_conf_file_t));

    ngx_memzero(&b, ಪರಿಮಾಣ(ngx_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.data = ರಿಕ್ತ;
    conf_file.ಸಾಲು = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    rv = ngx_conf_parse(cf, ರಿಕ್ತ);

    cf->conf_file = ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ rv;
}


ಸ್ಥಿರ ngx_int_t
ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             size;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_ಗಂಟು_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_crc32_long(filename->data, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->cycle->config_dump_rbtree, filename, hash);

    ಯದಿ (sn) {
        cf->conf_file->dump = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    ಯದಿ (p == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    cd = ngx_array_ತಳ್ಳು(&cf->cycle->config_dump);
    ಯದಿ (cd == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    ಯದಿ (buf == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, ಪರಿಮಾಣ(ngx_str_ಗಂಟು_t));
    ಯದಿ (sn == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    sn->ಗಂಟು.ಕೀಲಿ = hash;
    sn->str = cd->name;

    ngx_rbtree_ಸೇರಿಸು(&cf->cycle->config_dump_rbtree, &sn->ಗಂಟು);

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ವರ್ಣ *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    ವರ್ಣ             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    ಗಣನಾ {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#ಯದಿ (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = ರಿಕ್ತ;
#endif

    ಯದಿ (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        ಯದಿ (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        ಯದಿ (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        ಯದಿ (buf.start == ರಿಕ್ತ) {
            ಗಚ್ಛ failed;
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
        cf->conf_file->ಸಾಲು = 1;

        type = parse_file;

        ಯದಿ (ngx_dump_config
#ಯದಿ (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#ಯದಿ (NGX_DEBUG)
            || 1
#endif
           )
        {
            ಯದಿ (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                ಗಚ್ಛ failed;
            }

        } ಅನ್ಯಥಾ {
            cf->conf_file->dump = ರಿಕ್ತ;
        }

    } ಅನ್ಯಥಾ ಯದಿ (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } ಅನ್ಯಥಾ {
        type = parse_param;
    }


    ಪ್ರತಿ ( ;; ) {
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

        ಯದಿ (rc == NGX_ERROR) {
            ಗಚ್ಛ done;
        }

        ಯದಿ (rc == NGX_CONF_BLOCK_DONE) {

            ಯದಿ (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                ಗಚ್ಛ failed;
            }

            ಗಚ್ಛ done;
        }

        ಯದಿ (rc == NGX_CONF_FILE_DONE) {

            ಯದಿ (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                ಗಚ್ಛ failed;
            }

            ಗಚ್ಛ done;
        }

        ಯದಿ (rc == NGX_CONF_BLOCK_START) {

            ಯದಿ (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                ಗಚ್ಛ failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        ಯದಿ (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            ಯದಿ (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                ಗಚ್ಛ failed;
            }

            rv = (*cf->handler)(cf, ರಿಕ್ತ, cf->handler_conf);
            ಯದಿ (rv == NGX_CONF_OK) {
                ಅನುವರ್ತ;
            }

            ಯದಿ (rv == NGX_CONF_ERROR) {
                ಗಚ್ಛ failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            ಗಚ್ಛ failed;
        }


        rc = ngx_conf_handler(cf, rc);

        ಯದಿ (rc == NGX_ERROR) {
            ಗಚ್ಛ failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    ಯದಿ (filename) {
        ಯದಿ (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        ಯದಿ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    ಯದಿ (rc == NGX_ERROR) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    ವರ್ಣ           *rv;
    ಶೂನ್ಯ           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    ಪ್ರತಿ (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        ಯದಿ (cmd == ರಿಕ್ತ) {
            ಅನುವರ್ತ;
        }

        ಪ್ರತಿ ( /* void */ ; cmd->name.len; cmd++) {

            ಯದಿ (name->len != cmd->name.len) {
                ಅನುವರ್ತ;
            }

            ಯದಿ (ngx_strcmp(name->data, cmd->name.data) != 0) {
                ಅನುವರ್ತ;
            }

            found = 1;

            ಯದಿ (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                ಅನುವರ್ತ;
            }

            /* is the directive's location right ? */

            ಯದಿ (!(cmd->type & cf->cmd_type)) {
                ಅನುವರ್ತ;
            }

            ಯದಿ (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ಯದಿ ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            ಯದಿ (!(cmd->type & NGX_CONF_ANY)) {

                ಯದಿ (cmd->type & NGX_CONF_FLAG) {

                    ಯದಿ (cf->args->nelts != 2) {
                        ಗಚ್ಛ invalid;
                    }

                } ಅನ್ಯಥಾ ಯದಿ (cmd->type & NGX_CONF_1MORE) {

                    ಯದಿ (cf->args->nelts < 2) {
                        ಗಚ್ಛ invalid;
                    }

                } ಅನ್ಯಥಾ ಯದಿ (cmd->type & NGX_CONF_2MORE) {

                    ಯದಿ (cf->args->nelts < 3) {
                        ಗಚ್ಛ invalid;
                    }

                } ಅನ್ಯಥಾ ಯದಿ (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    ಗಚ್ಛ invalid;

                } ಅನ್ಯಥಾ ಯದಿ (!(cmd->type & argument_ಸಂಖ್ಯೆ[cf->args->nelts - 1]))
                {
                    ಗಚ್ಛ invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = ರಿಕ್ತ;

            ಯದಿ (cmd->type & NGX_DIRECT_CONF) {
                conf = ((ಶೂನ್ಯ **) cf->ctx)[cf->cycle->modules[i]->index];

            } ಅನ್ಯಥಾ ಯದಿ (cmd->type & NGX_MAIN_CONF) {
                conf = &(((ಶೂನ್ಯ **) cf->ctx)[cf->cycle->modules[i]->index]);

            } ಅನ್ಯಥಾ ಯದಿ (cf->ctx) {
                confp = *(ಶೂನ್ಯ **) ((ವರ್ಣ *) cf->ctx + cmd->conf);

                ಯದಿ (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            ಯದಿ (rv == NGX_CONF_OK) {
                ಪ್ರತ್ಯಾಗಮ NGX_OK;
            }

            ಯದಿ (rv == NGX_CONF_ERROR) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }

    ಯದಿ (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}


ಸ್ಥಿರ ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    size_t       len;
    ssize_t      n, size;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_ಸಾಲು;
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
    start_ಸಾಲು = cf->conf_file->ಸಾಲು;

    file_size = ngx_file_size(&cf->conf_file->file.info);

    ಪ್ರತಿ ( ;; ) {

        ಯದಿ (b->pos >= b->last) {

            ಯದಿ (cf->conf_file->file.offset >= file_size) {

                ಯದಿ (cf->args->nelts > 0 || !last_space) {

                    ಯದಿ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

                ಪ್ರತ್ಯಾಗಮ NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            ಯದಿ (len == NGX_CONF_BUFFER) {
                cf->conf_file->ಸಾಲು = start_ಸಾಲು;

                ಯದಿ (d_quoted) {
                    ch = '"';

                } ಅನ್ಯಥಾ ಯದಿ (s_quoted) {
                    ch = '\'';

                } ಅನ್ಯಥಾ {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ಯದಿ (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            ಯದಿ (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            ಯದಿ (n == NGX_ERROR) {
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ಯದಿ (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            ಯದಿ (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        ಯದಿ (ch == LF) {
            cf->conf_file->ಸಾಲು++;

            ಯದಿ (sharp_comment) {
                sharp_comment = 0;
            }
        }

        ಯದಿ (sharp_comment) {
            ಅನುವರ್ತ;
        }

        ಯದಿ (quoted) {
            quoted = 0;
            ಅನುವರ್ತ;
        }

        ಯದಿ (need_space) {
            ಯದಿ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                ಅನುವರ್ತ;
            }

            ಯದಿ (ch == ';') {
                ಪ್ರತ್ಯಾಗಮ NGX_OK;
            }

            ಯದಿ (ch == '{') {
                ಪ್ರತ್ಯಾಗಮ NGX_CONF_BLOCK_START;
            }

            ಯದಿ (ch == ')') {
                last_space = 1;
                need_space = 0;

            } ಅನ್ಯಥಾ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
        }

        ಯದಿ (last_space) {

            start = b->pos - 1;
            start_ಸಾಲು = cf->conf_file->ಸಾಲು;

            ಯದಿ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                ಅನುವರ್ತ;
            }

            ವಿಕಲ್ಪ (ch) {

            ಪಕ್ಷ ';':
            ಪಕ್ಷ '{':
                ಯದಿ (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

                ಯದಿ (ch == '{') {
                    ಪ್ರತ್ಯಾಗಮ NGX_CONF_BLOCK_START;
                }

                ಪ್ರತ್ಯಾಗಮ NGX_OK;

            ಪಕ್ಷ '}':
                ಯದಿ (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

                ಪ್ರತ್ಯಾಗಮ NGX_CONF_BLOCK_DONE;

            ಪಕ್ಷ '#':
                sharp_comment = 1;
                ಅನುವರ್ತ;

            ಪಕ್ಷ '\\':
                quoted = 1;
                last_space = 0;
                ಅನುವರ್ತ;

            ಪಕ್ಷ '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                ಅನುವರ್ತ;

            ಪಕ್ಷ '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                ಅನುವರ್ತ;

            ಪಕ್ಷ '$':
                variable = 1;
                last_space = 0;
                ಅನುವರ್ತ;

            ಸ್ವಭಾವ:
                last_space = 0;
            }

        } ಅನ್ಯಥಾ {
            ಯದಿ (ch == '{' && variable) {
                ಅನುವರ್ತ;
            }

            variable = 0;

            ಯದಿ (ch == '\\') {
                quoted = 1;
                ಅನುವರ್ತ;
            }

            ಯದಿ (ch == '$') {
                variable = 1;
                ಅನುವರ್ತ;
            }

            ಯದಿ (d_quoted) {
                ಯದಿ (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } ಅನ್ಯಥಾ ಯದಿ (s_quoted) {
                ಯದಿ (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } ಅನ್ಯಥಾ ಯದಿ (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            ಯದಿ (found) {
                word = ngx_array_ತಳ್ಳು(cf->args);
                ಯದಿ (word == ರಿಕ್ತ) {
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                ಯದಿ (word->data == ರಿಕ್ತ) {
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

                ಪ್ರತಿ (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    ಯದಿ (*src == '\\') {
                        ವಿಕಲ್ಪ (src[1]) {
                        ಪಕ್ಷ '"':
                        ಪಕ್ಷ '\'':
                        ಪಕ್ಷ '\\':
                            src++;
                            ಭಙ್ಗ;

                        ಪಕ್ಷ 't':
                            *dst++ = '\t';
                            src += 2;
                            ಅನುವರ್ತ;

                        ಪಕ್ಷ 'r':
                            *dst++ = '\r';
                            src += 2;
                            ಅನುವರ್ತ;

                        ಪಕ್ಷ 'n':
                            *dst++ = '\n';
                            src += 2;
                            ಅನುವರ್ತ;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                ಯದಿ (ch == ';') {
                    ಪ್ರತ್ಯಾಗಮ NGX_OK;
                }

                ಯದಿ (ch == '{') {
                    ಪ್ರತ್ಯಾಗಮ NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


ವರ್ಣ *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ        *rv;
    ngx_int_t    n;
    ngx_str_t   *ಮೌಲ್ಯ, file, name;
    ngx_glob_t   gl;

    ಮೌಲ್ಯ = cf->args->elts;
    file = ಮೌಲ್ಯ[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    ಯದಿ (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಯದಿ (strpbrk((ವರ್ಣ *) file.data, "*?[") == ರಿಕ್ತ) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        ಪ್ರತ್ಯಾಗಮ ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, ಪರಿಮಾಣ(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    ಯದಿ (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    ಪ್ರತಿ ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        ಯದಿ (n != NGX_OK) {
            ಭಙ್ಗ;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        ಯದಿ (file.data == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        ಯದಿ (rv != NGX_CONF_OK) {
            ಭಙ್ಗ;
        }
    }

    ngx_close_glob(&gl);

    ಪ್ರತ್ಯಾಗಮ rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    ಪ್ರತ್ಯಾಗಮ ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#ಯದಿ (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    ಯದಿ (name->len) {
        full = *name;

        ಯದಿ (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        ಪ್ರತಿ (i = 0; /* void */ ; i++) {

            ಯದಿ (i >= part->nelts) {
                ಯದಿ (part->next == ರಿಕ್ತ) {
                    ಭಙ್ಗ;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            ಯದಿ (full.len != file[i].name.len) {
                ಅನುವರ್ತ;
            }

            ಯದಿ (ngx_strcmp(full.data, file[i].name.data) == 0) {
                ಪ್ರತ್ಯಾಗಮ &file[i];
            }
        }
    }

    file = ngx_list_ತಳ್ಳು(&cycle->open_files);
    ಯದಿ (file == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯದಿ (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } ಅನ್ಯಥಾ {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = ರಿಕ್ತ;
    file->data = ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ file;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    ಪ್ರತಿ (i = 0; /* void */ ; i++) {

        ಯದಿ (i >= part->nelts) {
            ಯದಿ (part->next == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ಯದಿ (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


ಶೂನ್ಯ ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    va_list  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    va_end(args);

    ಯದಿ (err) {
        p = ngx_log_errno(p, last, err);
    }

    ಯದಿ (cf->conf_file == ರಿಕ್ತ) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->ಸಾಲು);
}


ವರ್ಣ *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ  *p = conf;

    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    ಯದಿ (*fp != NGX_CONF_UNSET) {
        ಪ್ರತ್ಯಾಗಮ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->args->elts;

    ಯದಿ (ngx_strcasecmp(ಮೌಲ್ಯ[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } ಅನ್ಯಥಾ ಯದಿ (ngx_strcasecmp(ಮೌಲ್ಯ[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } ಅನ್ಯಥಾ {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     ಮೌಲ್ಯ[1].data, cmd->name.data);
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಯದಿ (cmd->post) {
        post = cmd->post;
        ಪ್ರತ್ಯಾಗಮ post->post_handler(cf, post, fp);
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ವರ್ಣ *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ  *p = conf;

    ngx_str_t        *field, *ಮೌಲ್ಯ;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    ಯದಿ (field->data) {
        ಪ್ರತ್ಯಾಗಮ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->args->elts;

    *field = ಮೌಲ್ಯ[1];

    ಯದಿ (cmd->post) {
        post = cmd->post;
        ಪ್ರತ್ಯಾಗಮ post->post_handler(cf, post, field);
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ವರ್ಣ *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ  *p = conf;

    ngx_str_t         *ಮೌಲ್ಯ, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    ಯದಿ (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, ಪರಿಮಾಣ(ngx_str_t));
        ಯದಿ (*a == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }
    }

    s = ngx_array_ತಳ್ಳು(*a);
    ಯದಿ (s == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಮೌಲ್ಯ = cf->args->elts;

    *s = ಮೌಲ್ಯ[1];

    ಯದಿ (cmd->post) {
        post = cmd->post;
        ಪ್ರತ್ಯಾಗಮ post->post_handler(cf, post, s);
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ವರ್ಣ *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ  *p = conf;

    ngx_str_t         *ಮೌಲ್ಯ;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    ಯದಿ (*a == NGX_CONF_UNSET_PTR || *a == ರಿಕ್ತ) {
        *a = ngx_array_create(cf->pool, 4, ಪರಿಮಾಣ(ngx_keyval_t));
        ಯದಿ (*a == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_ತಳ್ಳು(*a);
    ಯದಿ (kv == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಮೌಲ್ಯ = cf->args->elts;

    kv->ಕೀಲಿ = ಮೌಲ್ಯ[1];
    kv->ಮೌಲ್ಯ = ಮೌಲ್ಯ[2];

    ಯದಿ (cmd->post) {
        post = cmd->post;
        ಪ್ರತ್ಯಾಗಮ post->post_handler(cf, post, kv);
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ವರ್ಣ *
ngx_conf_set_ಅಂಕೆ_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    ಯದಿ (*np != NGX_CONF_UNSET) {
        ಪ್ರತ್ಯಾಗಮ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->args->elts;
    *np = ngx_atoi(ಮೌಲ್ಯ[1].data, ಮೌಲ್ಯ[1].len);
    ಯದಿ (*np == NGX_ERROR) {
        ಪ್ರತ್ಯಾಗಮ "invalid number";
    }

    ಯದಿ (cmd->post) {
        post = cmd->post;
        ಪ್ರತ್ಯಾಗಮ post->post_handler(cf, post, np);
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ವರ್ಣ *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ  *p = conf;

    size_t           *sp;
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    ಯದಿ (*sp != NGX_CONF_UNSET_SIZE) {
        ಪ್ರತ್ಯಾಗಮ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->args->elts;

    *sp = ngx_parse_size(&ಮೌಲ್ಯ[1]);
    ಯದಿ (*sp == (size_t) NGX_ERROR) {
        ಪ್ರತ್ಯಾಗಮ "invalid value";
    }

    ಯದಿ (cmd->post) {
        post = cmd->post;
        ಪ್ರತ್ಯಾಗಮ post->post_handler(cf, post, sp);
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ವರ್ಣ *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ  *p = conf;

    off_t            *op;
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    ಯದಿ (*op != NGX_CONF_UNSET) {
        ಪ್ರತ್ಯಾಗಮ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->args->elts;

    *op = ngx_parse_offset(&ಮೌಲ್ಯ[1]);
    ಯದಿ (*op == (off_t) NGX_ERROR) {
        ಪ್ರತ್ಯಾಗಮ "invalid value";
    }

    ಯದಿ (cmd->post) {
        post = cmd->post;
        ಪ್ರತ್ಯಾಗಮ post->post_handler(cf, post, op);
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ವರ್ಣ *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    ಯದಿ (*msp != NGX_CONF_UNSET_MSEC) {
        ಪ್ರತ್ಯಾಗಮ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->args->elts;

    *msp = ngx_parse_time(&ಮೌಲ್ಯ[1], 0);
    ಯದಿ (*msp == (ngx_msec_t) NGX_ERROR) {
        ಪ್ರತ್ಯಾಗಮ "invalid value";
    }

    ಯದಿ (cmd->post) {
        post = cmd->post;
        ಪ್ರತ್ಯಾಗಮ post->post_handler(cf, post, msp);
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ವರ್ಣ *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ  *p = conf;

    time_t           *sp;
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    ಯದಿ (*sp != NGX_CONF_UNSET) {
        ಪ್ರತ್ಯಾಗಮ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->args->elts;

    *sp = ngx_parse_time(&ಮೌಲ್ಯ[1], 1);
    ಯದಿ (*sp == (time_t) NGX_ERROR) {
        ಪ್ರತ್ಯಾಗಮ "invalid value";
    }

    ಯದಿ (cmd->post) {
        post = cmd->post;
        ಪ್ರತ್ಯಾಗಮ post->post_handler(cf, post, sp);
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ವರ್ಣ *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ *p = conf;

    ngx_str_t   *ಮೌಲ್ಯ;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    ಯದಿ (bufs->ಅಂಕೆ) {
        ಪ್ರತ್ಯಾಗಮ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->args->elts;

    bufs->ಅಂಕೆ = ngx_atoi(ಮೌಲ್ಯ[1].data, ಮೌಲ್ಯ[1].len);
    ಯದಿ (bufs->ಅಂಕೆ == NGX_ERROR || bufs->ಅಂಕೆ == 0) {
        ಪ್ರತ್ಯಾಗಮ "invalid value";
    }

    bufs->size = ngx_parse_size(&ಮೌಲ್ಯ[2]);
    ಯದಿ (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        ಪ್ರತ್ಯಾಗಮ "invalid value";
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ವರ್ಣ *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    ಯದಿ (*np != NGX_CONF_UNSET_UINT) {
        ಪ್ರತ್ಯಾಗಮ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->args->elts;
    e = cmd->post;

    ಪ್ರತಿ (i = 0; e[i].name.len != 0; i++) {
        ಯದಿ (e[i].name.len != ಮೌಲ್ಯ[1].len
            || ngx_strcasecmp(e[i].name.data, ಮೌಲ್ಯ[1].data) != 0)
        {
            ಅನುವರ್ತ;
        }

        *np = e[i].ಮೌಲ್ಯ;

        ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", ಮೌಲ್ಯ[1].data);

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
}


ವರ್ಣ *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *ಮೌಲ್ಯ;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    ಮೌಲ್ಯ = cf->args->elts;
    mask = cmd->post;

    ಪ್ರತಿ (i = 1; i < cf->args->nelts; i++) {
        ಪ್ರತಿ (m = 0; mask[m].name.len != 0; m++) {

            ಯದಿ (mask[m].name.len != ಮೌಲ್ಯ[i].len
                || ngx_strcasecmp(mask[m].name.data, ಮೌಲ್ಯ[i].data) != 0)
            {
                ಅನುವರ್ತ;
            }

            ಯದಿ (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", ಮೌಲ್ಯ[i].data);

            } ಅನ್ಯಥಾ {
                *np |= mask[m].mask;
            }

            ಭಙ್ಗ;
        }

        ಯದಿ (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", ಮೌಲ್ಯ[i].data);

            ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
        }
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


#ಯದಿ 0

ವರ್ಣ *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಪ್ರತ್ಯಾಗಮ "unsupported on this platform";
}

#endif


ವರ್ಣ *
ngx_conf_deprecated(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ವರ್ಣ *
ngx_conf_check_ಅಂಕೆ_bounds(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *data)
{
    ngx_conf_ಅಂಕೆ_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    ಯದಿ (bounds->ಮೇಲಿನ == -1) {
        ಯದಿ (*np >= bounds->ಕೆಳಗಿನ) {
            ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->ಕೆಳಗಿನ);

        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಯದಿ (*np >= bounds->ಕೆಳಗಿನ && *np <= bounds->ಮೇಲಿನ) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->ಕೆಳಗಿನ, bounds->ಮೇಲಿನ);

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
}


ವರ್ಣ *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *data)
{
    size_t  *sp = data;

    ಯದಿ (*sp == 0) {
        ಪ್ರತ್ಯಾಗಮ "value must be nonzero";
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}
