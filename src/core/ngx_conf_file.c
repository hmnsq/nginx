
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NGX_CONF_ਬਫਰ  4096

ਸਥਿਰ ngx_int_t ngx_conf_ਜੋੜੋ_dump(ngx_conf_t *cf, ngx_str_t *filename);
ਸਥਿਰ ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t ਆਖਰੀ);
ਸਥਿਰ ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
ਸਥਿਰ ਖਾਲੀ ngx_conf_flush_files(ngx_ਚੱਕਰ_t *ਚੱਕਰ);


ਸਥਿਰ ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      ਨਿਰਾ },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    ਨਿਰਾ,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    ਨਿਰਾ,                                  /* init master */
    ਨਿਰਾ,                                  /* init module */
    ਨਿਰਾ,                                  /* init process */
    ਨਿਰਾ,                                  /* init thread */
    ਨਿਰਾ,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    ਨਿਰਾ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

ਸਥਿਰ ngx_uint_t ਦਲੀਲ_ਸੰਖਿਆ[] = {
    NGX_CONF_NOARGS,
    NGX_CONF_TAKE1,
    NGX_CONF_TAKE2,
    NGX_CONF_TAKE3,
    NGX_CONF_TAKE4,
    NGX_CONF_TAKE5,
    NGX_CONF_TAKE6,
    NGX_CONF_TAKE7
};


ngx_conf_post_t  ngx_conf_ਮਾਪ_nonzero_post = { ngx_conf_check_ਮਾਪ_nonzero };


ਅੱਖਰ *
ngx_conf_param(ngx_conf_t *cf)
{
    ਅੱਖਰ             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->ਚੱਕਰ->conf_param;

    ਜੇ (param->len == 0) {
        ਵਾਪਸ NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, ਆਕਾਰ(ngx_conf_file_t));

    ngx_memzero(&b, ਆਕਾਰ(ngx_buf_t));

    b.ਸ਼ੁਰੂ = param->ਅੰਕੜੇ;
    b.pos = param->ਅੰਕੜੇ;
    b.ਆਖਰੀ = param->ਅੰਕੜੇ + param->len;
    b.ਸਿਰਾ = b.ਆਖਰੀ;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.ਅੰਕੜੇ = ਨਿਰਾ;
    conf_file.ਸਤਰ = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->ਬਫਰ = &b;

    rv = ngx_conf_parse(cf, ਨਿਰਾ);

    cf->conf_file = ਨਿਰਾ;

    ਵਾਪਸ rv;
}


ਸਥਿਰ ngx_int_t
ngx_conf_ਜੋੜੋ_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             ਮਾਪ;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_ਗੰਢ_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_ਚੱਕਰੀ_ਜਾਂਚ_32_long(filename->ਅੰਕੜੇ, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->ਚੱਕਰ->config_dump_rbtree, filename, hash);

    ਜੇ (sn) {
        cf->conf_file->dump = ਨਿਰਾ;
        ਵਾਪਸ NGX_OK;
    }

    p = ngx_pstrdup(cf->ਚੱਕਰ->pool, filename);
    ਜੇ (p == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    cd = ngx_array_ਧੱਕੋ(&cf->ਚੱਕਰ->config_dump);
    ਜੇ (cd == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਮਾਪ = ngx_file_ਮਾਪ(&cf->conf_file->file.info);

    buf = ngx_create_ਆਰਜ਼ੀ_buf(cf->ਚੱਕਰ->pool, (ਮਾਪ_t) ਮਾਪ);
    ਜੇ (buf == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    cd->name.ਅੰਕੜੇ = p;
    cd->name.len = filename->len;
    cd->ਬਫਰ = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->ਆਰਜ਼ੀ_pool, ਆਕਾਰ(ngx_str_ਗੰਢ_t));
    ਜੇ (sn == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    sn->ਗੰਢ.ਕੁੰਜੀ = hash;
    sn->str = cd->name;

    ngx_rbtree_ਪਾਓ(&cf->ਚੱਕਰ->config_dump_rbtree, &sn->ਗੰਢ);

    ਵਾਪਸ NGX_OK;
}


ਅੱਖਰ *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    ਅੱਖਰ             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    ਗਿਣਤੀ {
        parse_file = 0,
        parse_ਖੰਡ,
        parse_param
    } type;

#ਜੇ (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = ਨਿਰਾ;
#ਜੇ_ਅੰਤ

    ਜੇ (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->ਅੰਕੜੇ, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        ਜੇ (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->ਅੰਕੜੇ);
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        ਜੇ (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->ਅੰਕੜੇ);
        }

        cf->conf_file->ਬਫਰ = &buf;

        buf.ਸ਼ੁਰੂ = ngx_alloc(NGX_CONF_ਬਫਰ, cf->ਲਘੂਗਣਕ);
        ਜੇ (buf.ਸ਼ੁਰੂ == ਨਿਰਾ) {
            ਜਾਓ failed;
        }

        buf.pos = buf.ਸ਼ੁਰੂ;
        buf.ਆਖਰੀ = buf.ਸ਼ੁਰੂ;
        buf.ਸਿਰਾ = buf.ਆਖਰੀ + NGX_CONF_ਬਫਰ;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.ਅੰਕੜੇ = filename->ਅੰਕੜੇ;
        cf->conf_file->file.ਵਿੱਥ = 0;
        cf->conf_file->file.ਲਘੂਗਣਕ = cf->ਲਘੂਗਣਕ;
        cf->conf_file->ਸਤਰ = 1;

        type = parse_file;

        ਜੇ (ngx_dump_config
#ਜੇ (NGX_CONTROL_API)
            || ngx_control_api_enabled
#ਜੇ_ਅੰਤ
#ਜੇ (NGX_DEBUG)
            || 1
#ਜੇ_ਅੰਤ
           )
        {
            ਜੇ (ngx_conf_ਜੋੜੋ_dump(cf, filename) != NGX_OK) {
                ਜਾਓ failed;
            }

        } ਨਹੀਂਤਾਂ {
            cf->conf_file->dump = ਨਿਰਾ;
        }

    } ਨਹੀਂਤਾਂ ਜੇ (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_ਖੰਡ;

    } ਨਹੀਂਤਾਂ {
        type = parse_param;
    }


    ਲਈ ( ;; ) {
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

        ਜੇ (rc == NGX_ERROR) {
            ਜਾਓ ਪੂਰਾ;
        }

        ਜੇ (rc == NGX_CONF_ਖੰਡ_ਪੂਰਾ) {

            ਜੇ (type != parse_ਖੰਡ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                ਜਾਓ failed;
            }

            ਜਾਓ ਪੂਰਾ;
        }

        ਜੇ (rc == NGX_CONF_FILE_ਪੂਰਾ) {

            ਜੇ (type == parse_ਖੰਡ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                ਜਾਓ failed;
            }

            ਜਾਓ ਪੂਰਾ;
        }

        ਜੇ (rc == NGX_CONF_ਖੰਡ_ਸ਼ੁਰੂ) {

            ਜੇ (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                ਜਾਓ failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        ਜੇ (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            ਜੇ (rc == NGX_CONF_ਖੰਡ_ਸ਼ੁਰੂ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                ਜਾਓ failed;
            }

            rv = (*cf->handler)(cf, ਨਿਰਾ, cf->handler_conf);
            ਜੇ (rv == NGX_CONF_OK) {
                ਜਾਰੀ;
            }

            ਜੇ (rv == NGX_CONF_ERROR) {
                ਜਾਓ failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            ਜਾਓ failed;
        }


        rc = ngx_conf_handler(cf, rc);

        ਜੇ (rc == NGX_ERROR) {
            ਜਾਓ failed;
        }
    }

failed:

    rc = NGX_ERROR;

ਪੂਰਾ:

    ਜੇ (filename) {
        ਜੇ (cf->conf_file->ਬਫਰ->ਸ਼ੁਰੂ) {
            ngx_free(cf->conf_file->ਬਫਰ->ਸ਼ੁਰੂ);
        }

        ਜੇ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->ਅੰਕੜੇ);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    ਜੇ (rc == NGX_ERROR) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਸਥਿਰ ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t ਆਖਰੀ)
{
    ਅੱਖਰ           *rv;
    ਖਾਲੀ           *conf, **confp;
    ngx_uint_t      i, ਮਿਲਿਆ;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->ਦਲੀਲਾਂ->elts;

    ਮਿਲਿਆ = 0;

    ਲਈ (i = 0; cf->ਚੱਕਰ->modules[i]; i++) {

        cmd = cf->ਚੱਕਰ->modules[i]->commands;
        ਜੇ (cmd == ਨਿਰਾ) {
            ਜਾਰੀ;
        }

        ਲਈ ( /* void */ ; cmd->name.len; cmd++) {

            ਜੇ (name->len != cmd->name.len) {
                ਜਾਰੀ;
            }

            ਜੇ (ngx_strcmp(name->ਅੰਕੜੇ, cmd->name.ਅੰਕੜੇ) != 0) {
                ਜਾਰੀ;
            }

            ਮਿਲਿਆ = 1;

            ਜੇ (cf->ਚੱਕਰ->modules[i]->type != NGX_CONF_MODULE
                && cf->ਚੱਕਰ->modules[i]->type != cf->module_type)
            {
                ਜਾਰੀ;
            }

            /* is the directive's location right ? */

            ਜੇ (!(cmd->type & cf->cmd_type)) {
                ਜਾਰੀ;
            }

            ਜੇ (!(cmd->type & NGX_CONF_ਖੰਡ) && ਆਖਰੀ != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->ਅੰਕੜੇ);
                ਵਾਪਸ NGX_ERROR;
            }

            ਜੇ ((cmd->type & NGX_CONF_ਖੰਡ) && ਆਖਰੀ != NGX_CONF_ਖੰਡ_ਸ਼ੁਰੂ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->ਅੰਕੜੇ);
                ਵਾਪਸ NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            ਜੇ (!(cmd->type & NGX_CONF_ANY)) {

                ਜੇ (cmd->type & NGX_CONF_FLAG) {

                    ਜੇ (cf->ਦਲੀਲਾਂ->nelts != 2) {
                        ਜਾਓ invalid;
                    }

                } ਨਹੀਂਤਾਂ ਜੇ (cmd->type & NGX_CONF_1_ਹੋਰ) {

                    ਜੇ (cf->ਦਲੀਲਾਂ->nelts < 2) {
                        ਜਾਓ invalid;
                    }

                } ਨਹੀਂਤਾਂ ਜੇ (cmd->type & NGX_CONF_2_ਹੋਰ) {

                    ਜੇ (cf->ਦਲੀਲਾਂ->nelts < 3) {
                        ਜਾਓ invalid;
                    }

                } ਨਹੀਂਤਾਂ ਜੇ (cf->ਦਲੀਲਾਂ->nelts > NGX_CONF_MAX_ਦਲੀਲਾਂ) {

                    ਜਾਓ invalid;

                } ਨਹੀਂਤਾਂ ਜੇ (!(cmd->type & ਦਲੀਲ_ਸੰਖਿਆ[cf->ਦਲੀਲਾਂ->nelts - 1]))
                {
                    ਜਾਓ invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = ਨਿਰਾ;

            ਜੇ (cmd->type & NGX_ਸਿੱਧਾ_CONF) {
                conf = ((ਖਾਲੀ **) cf->ctx)[cf->ਚੱਕਰ->modules[i]->ਸੂਚਕਾਂਕ];

            } ਨਹੀਂਤਾਂ ਜੇ (cmd->type & NGX_MAIN_CONF) {
                conf = &(((ਖਾਲੀ **) cf->ctx)[cf->ਚੱਕਰ->modules[i]->ਸੂਚਕਾਂਕ]);

            } ਨਹੀਂਤਾਂ ਜੇ (cf->ctx) {
                confp = *(ਖਾਲੀ **) ((ਅੱਖਰ *) cf->ctx + cmd->conf);

                ਜੇ (confp) {
                    conf = confp[cf->ਚੱਕਰ->modules[i]->ctx_ਸੂਚਕਾਂਕ];
                }
            }

            rv = cmd->ਸੈੱਟ(cf, cmd, conf);

            ਜੇ (rv == NGX_CONF_OK) {
                ਵਾਪਸ NGX_OK;
            }

            ਜੇ (rv == NGX_CONF_ERROR) {
                ਵਾਪਸ NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->ਅੰਕੜੇ, rv);

            ਵਾਪਸ NGX_ERROR;
        }
    }

    ਜੇ (ਮਿਲਿਆ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->ਅੰਕੜੇ);

        ਵਾਪਸ NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->ਅੰਕੜੇ);

    ਵਾਪਸ NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->ਅੰਕੜੇ);

    ਵਾਪਸ NGX_ERROR;
}


ਸਥਿਰ ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *ਸ਼ੁਰੂ, ch, *src, *dst;
    off_t        file_ਮਾਪ;
    ਮਾਪ_t       len;
    ssize_t      n, ਮਾਪ;
    ngx_uint_t   ਮਿਲਿਆ, need_space, ਆਖਰੀ_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, ਸ਼ੁਰੂ_ਸਤਰ;
    ngx_str_t   *ਸ਼ਬਦ;
    ngx_buf_t   *b, *dump;

    ਮਿਲਿਆ = 0;
    need_space = 0;
    ਆਖਰੀ_space = 1;
    sharp_comment = 0;
    variable = 0;
    quoted = 0;
    s_quoted = 0;
    d_quoted = 0;

    cf->ਦਲੀਲਾਂ->nelts = 0;
    b = cf->conf_file->ਬਫਰ;
    dump = cf->conf_file->dump;
    ਸ਼ੁਰੂ = b->pos;
    ਸ਼ੁਰੂ_ਸਤਰ = cf->conf_file->ਸਤਰ;

    file_ਮਾਪ = ngx_file_ਮਾਪ(&cf->conf_file->file.info);

    ਲਈ ( ;; ) {

        ਜੇ (b->pos >= b->ਆਖਰੀ) {

            ਜੇ (cf->conf_file->file.ਵਿੱਥ >= file_ਮਾਪ) {

                ਜੇ (cf->ਦਲੀਲਾਂ->nelts > 0 || !ਆਖਰੀ_space) {

                    ਜੇ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        ਵਾਪਸ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    ਵਾਪਸ NGX_ERROR;
                }

                ਵਾਪਸ NGX_CONF_FILE_ਪੂਰਾ;
            }

            len = b->pos - ਸ਼ੁਰੂ;

            ਜੇ (len == NGX_CONF_ਬਫਰ) {
                cf->conf_file->ਸਤਰ = ਸ਼ੁਰੂ_ਸਤਰ;

                ਜੇ (d_quoted) {
                    ch = '"';

                } ਨਹੀਂਤਾਂ ਜੇ (s_quoted) {
                    ch = '\'';

                } ਨਹੀਂਤਾਂ {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (ਮਾਪ_t) 10, ਸ਼ੁਰੂ);
                    ਵਾਪਸ NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                ਵਾਪਸ NGX_ERROR;
            }

            ਜੇ (len) {
                ngx_memmove(b->ਸ਼ੁਰੂ, ਸ਼ੁਰੂ, len);
            }

            ਮਾਪ = (ssize_t) (file_ਮਾਪ - cf->conf_file->file.ਵਿੱਥ);

            ਜੇ (ਮਾਪ > b->ਸਿਰਾ - (b->ਸ਼ੁਰੂ + len)) {
                ਮਾਪ = b->ਸਿਰਾ - (b->ਸ਼ੁਰੂ + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->ਸ਼ੁਰੂ + len, ਮਾਪ,
                              cf->conf_file->file.ਵਿੱਥ);

            ਜੇ (n == NGX_ERROR) {
                ਵਾਪਸ NGX_ERROR;
            }

            ਜੇ (n != ਮਾਪ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, ਮਾਪ);
                ਵਾਪਸ NGX_ERROR;
            }

            b->pos = b->ਸ਼ੁਰੂ + len;
            b->ਆਖਰੀ = b->pos + n;
            ਸ਼ੁਰੂ = b->ਸ਼ੁਰੂ;

            ਜੇ (dump) {
                dump->ਆਖਰੀ = ngx_cpymem(dump->ਆਖਰੀ, b->pos, ਮਾਪ);
            }
        }

        ch = *b->pos++;

        ਜੇ (ch == LF) {
            cf->conf_file->ਸਤਰ++;

            ਜੇ (sharp_comment) {
                sharp_comment = 0;
            }
        }

        ਜੇ (sharp_comment) {
            ਜਾਰੀ;
        }

        ਜੇ (quoted) {
            quoted = 0;
            ਜਾਰੀ;
        }

        ਜੇ (need_space) {
            ਜੇ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                ਆਖਰੀ_space = 1;
                need_space = 0;
                ਜਾਰੀ;
            }

            ਜੇ (ch == ';') {
                ਵਾਪਸ NGX_OK;
            }

            ਜੇ (ch == '{') {
                ਵਾਪਸ NGX_CONF_ਖੰਡ_ਸ਼ੁਰੂ;
            }

            ਜੇ (ch == ')') {
                ਆਖਰੀ_space = 1;
                need_space = 0;

            } ਨਹੀਂਤਾਂ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                ਵਾਪਸ NGX_ERROR;
            }
        }

        ਜੇ (ਆਖਰੀ_space) {

            ਸ਼ੁਰੂ = b->pos - 1;
            ਸ਼ੁਰੂ_ਸਤਰ = cf->conf_file->ਸਤਰ;

            ਜੇ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                ਜਾਰੀ;
            }

            ਚੋਣ (ch) {

            ਹਾਲਤ ';':
            ਹਾਲਤ '{':
                ਜੇ (cf->ਦਲੀਲਾਂ->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    ਵਾਪਸ NGX_ERROR;
                }

                ਜੇ (ch == '{') {
                    ਵਾਪਸ NGX_CONF_ਖੰਡ_ਸ਼ੁਰੂ;
                }

                ਵਾਪਸ NGX_OK;

            ਹਾਲਤ '}':
                ਜੇ (cf->ਦਲੀਲਾਂ->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    ਵਾਪਸ NGX_ERROR;
                }

                ਵਾਪਸ NGX_CONF_ਖੰਡ_ਪੂਰਾ;

            ਹਾਲਤ '#':
                sharp_comment = 1;
                ਜਾਰੀ;

            ਹਾਲਤ '\\':
                quoted = 1;
                ਆਖਰੀ_space = 0;
                ਜਾਰੀ;

            ਹਾਲਤ '"':
                ਸ਼ੁਰੂ++;
                d_quoted = 1;
                ਆਖਰੀ_space = 0;
                ਜਾਰੀ;

            ਹਾਲਤ '\'':
                ਸ਼ੁਰੂ++;
                s_quoted = 1;
                ਆਖਰੀ_space = 0;
                ਜਾਰੀ;

            ਹਾਲਤ '$':
                variable = 1;
                ਆਖਰੀ_space = 0;
                ਜਾਰੀ;

            ਮੂਲ:
                ਆਖਰੀ_space = 0;
            }

        } ਨਹੀਂਤਾਂ {
            ਜੇ (ch == '{' && variable) {
                ਜਾਰੀ;
            }

            variable = 0;

            ਜੇ (ch == '\\') {
                quoted = 1;
                ਜਾਰੀ;
            }

            ਜੇ (ch == '$') {
                variable = 1;
                ਜਾਰੀ;
            }

            ਜੇ (d_quoted) {
                ਜੇ (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    ਮਿਲਿਆ = 1;
                }

            } ਨਹੀਂਤਾਂ ਜੇ (s_quoted) {
                ਜੇ (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    ਮਿਲਿਆ = 1;
                }

            } ਨਹੀਂਤਾਂ ਜੇ (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                ਆਖਰੀ_space = 1;
                ਮਿਲਿਆ = 1;
            }

            ਜੇ (ਮਿਲਿਆ) {
                ਸ਼ਬਦ = ngx_array_ਧੱਕੋ(cf->ਦਲੀਲਾਂ);
                ਜੇ (ਸ਼ਬਦ == ਨਿਰਾ) {
                    ਵਾਪਸ NGX_ERROR;
                }

                ਸ਼ਬਦ->ਅੰਕੜੇ = ngx_pnalloc(cf->pool, b->pos - 1 - ਸ਼ੁਰੂ + 1);
                ਜੇ (ਸ਼ਬਦ->ਅੰਕੜੇ == ਨਿਰਾ) {
                    ਵਾਪਸ NGX_ERROR;
                }

                ਲਈ (dst = ਸ਼ਬਦ->ਅੰਕੜੇ, src = ਸ਼ੁਰੂ, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    ਜੇ (*src == '\\') {
                        ਚੋਣ (src[1]) {
                        ਹਾਲਤ '"':
                        ਹਾਲਤ '\'':
                        ਹਾਲਤ '\\':
                            src++;
                            ਰੋਕੋ;

                        ਹਾਲਤ 't':
                            *dst++ = '\t';
                            src += 2;
                            ਜਾਰੀ;

                        ਹਾਲਤ 'r':
                            *dst++ = '\r';
                            src += 2;
                            ਜਾਰੀ;

                        ਹਾਲਤ 'n':
                            *dst++ = '\n';
                            src += 2;
                            ਜਾਰੀ;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                ਸ਼ਬਦ->len = len;

                ਜੇ (ch == ';') {
                    ਵਾਪਸ NGX_OK;
                }

                ਜੇ (ch == '{') {
                    ਵਾਪਸ NGX_CONF_ਖੰਡ_ਸ਼ੁਰੂ;
                }

                ਮਿਲਿਆ = 0;
            }
        }
    }
}


ਅੱਖਰ *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ        *rv;
    ngx_int_t    n;
    ngx_str_t   *ਮੁੱਲ, file, name;
    ngx_glob_t   gl;

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;
    file = ਮੁੱਲ[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->ਲਘੂਗਣਕ, 0, "include %s", file.ਅੰਕੜੇ);

    ਜੇ (ngx_conf_ਭਰਿਆ_name(cf->ਚੱਕਰ, &file, 1) != NGX_OK) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਜੇ (strpbrk((ਅੱਖਰ *) file.ਅੰਕੜੇ, "*?[") == ਨਿਰਾ) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->ਲਘੂਗਣਕ, 0, "include %s", file.ਅੰਕੜੇ);

        ਵਾਪਸ ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, ਆਕਾਰ(ngx_glob_t));

    gl.ਨਮੂਨਾ = file.ਅੰਕੜੇ;
    gl.ਲਘੂਗਣਕ = cf->ਲਘੂਗਣਕ;
    gl.test = 1;

    ਜੇ (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.ਅੰਕੜੇ);
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    ਲਈ ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        ਜੇ (n != NGX_OK) {
            ਰੋਕੋ;
        }

        file.len = name.len++;
        file.ਅੰਕੜੇ = ngx_pstrdup(cf->pool, &name);
        ਜੇ (file.ਅੰਕੜੇ == ਨਿਰਾ) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->ਲਘੂਗਣਕ, 0, "include %s", file.ਅੰਕੜੇ);

        rv = ngx_conf_parse(cf, &file);

        ਜੇ (rv != NGX_CONF_OK) {
            ਰੋਕੋ;
        }
    }

    ngx_close_glob(&gl);

    ਵਾਪਸ rv;
}


ngx_int_t
ngx_conf_ਭਰਿਆ_name(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_str_t *name, ngx_uint_t conf_ਅਗੇਤਰ)
{
    ngx_str_t  *ਅਗੇਤਰ;

    ਅਗੇਤਰ = conf_ਅਗੇਤਰ ? &ਚੱਕਰ->conf_ਅਗੇਤਰ : &ਚੱਕਰ->ਅਗੇਤਰ;

    ਵਾਪਸ ngx_get_ਭਰਿਆ_name(ਚੱਕਰ->pool, ਅਗੇਤਰ, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_str_t *name)
{
    ngx_str_t         ਭਰਿਆ;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#ਜੇ (NGX_SUPPRESS_WARN)
    ngx_str_null(&ਭਰਿਆ);
#ਜੇ_ਅੰਤ

    ਜੇ (name->len) {
        ਭਰਿਆ = *name;

        ਜੇ (ngx_conf_ਭਰਿਆ_name(ਚੱਕਰ, &ਭਰਿਆ, 0) != NGX_OK) {
            ਵਾਪਸ ਨਿਰਾ;
        }

        part = &ਚੱਕਰ->open_files.part;
        file = part->elts;

        ਲਈ (i = 0; /* void */ ; i++) {

            ਜੇ (i >= part->nelts) {
                ਜੇ (part->ਅਗਲਾ == ਨਿਰਾ) {
                    ਰੋਕੋ;
                }
                part = part->ਅਗਲਾ;
                file = part->elts;
                i = 0;
            }

            ਜੇ (ਭਰਿਆ.len != file[i].name.len) {
                ਜਾਰੀ;
            }

            ਜੇ (ngx_strcmp(ਭਰਿਆ.ਅੰਕੜੇ, file[i].name.ਅੰਕੜੇ) == 0) {
                ਵਾਪਸ &file[i];
            }
        }
    }

    file = ngx_list_ਧੱਕੋ(&ਚੱਕਰ->open_files);
    ਜੇ (file == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = ਭਰਿਆ;

    } ਨਹੀਂਤਾਂ {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = ਨਿਰਾ;
    file->ਅੰਕੜੇ = ਨਿਰਾ;

    ਵਾਪਸ file;
}


ਸਥਿਰ ਖਾਲੀ
ngx_conf_flush_files(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "flush files");

    part = &ਚੱਕਰ->open_files.part;
    file = part->elts;

    ਲਈ (i = 0; /* void */ ; i++) {

        ਜੇ (i >= part->nelts) {
            ਜੇ (part->ਅਗਲਾ == ਨਿਰਾ) {
                ਰੋਕੋ;
            }
            part = part->ਅਗਲਾ;
            file = part->elts;
            i = 0;
        }

        ਜੇ (file[i].flush) {
            file[i].flush(&file[i], ਚੱਕਰ->ਲਘੂਗਣਕ);
        }
    }
}


ਖਾਲੀ ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ਅਚਲ ਅੱਖਰ *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *ਆਖਰੀ;
    va_list  ਦਲੀਲਾਂ;

    ਆਖਰੀ = errstr + NGX_MAX_CONF_ERRSTR;

    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
    p = ngx_vslprintf(errstr, ਆਖਰੀ, fmt, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);

    ਜੇ (err) {
        p = ngx_log_errno(p, ਆਖਰੀ, err);
    }

    ਜੇ (cf->conf_file == ਨਿਰਾ) {
        ngx_log_error(level, cf->ਲਘੂਗਣਕ, 0, "%*s", p - errstr, errstr);
        ਵਾਪਸ;
    }

    ਜੇ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->ਲਘੂਗਣਕ, 0, "%*s in command line",
                      p - errstr, errstr);
        ਵਾਪਸ;
    }

    ngx_log_error(level, cf->ਲਘੂਗਣਕ, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.ਅੰਕੜੇ, cf->conf_file->ਸਤਰ);
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_flag_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ  *p = conf;

    ngx_str_t        *ਮੁੱਲ;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->ਵਿੱਥ);

    ਜੇ (*fp != NGX_CONF_UNSET) {
        ਵਾਪਸ "is duplicate";
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    ਜੇ (ngx_strcasecmp(ਮੁੱਲ[1].ਅੰਕੜੇ, (u_char *) "on") == 0) {
        *fp = 1;

    } ਨਹੀਂਤਾਂ ਜੇ (ngx_strcasecmp(ਮੁੱਲ[1].ਅੰਕੜੇ, (u_char *) "off") == 0) {
        *fp = 0;

    } ਨਹੀਂਤਾਂ {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     ਮੁੱਲ[1].ਅੰਕੜੇ, cmd->name.ਅੰਕੜੇ);
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਜੇ (cmd->post) {
        post = cmd->post;
        ਵਾਪਸ post->post_handler(cf, post, fp);
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_str_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ  *p = conf;

    ngx_str_t        *ਖੇਤਰ, *ਮੁੱਲ;
    ngx_conf_post_t  *post;

    ਖੇਤਰ = (ngx_str_t *) (p + cmd->ਵਿੱਥ);

    ਜੇ (ਖੇਤਰ->ਅੰਕੜੇ) {
        ਵਾਪਸ "is duplicate";
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    *ਖੇਤਰ = ਮੁੱਲ[1];

    ਜੇ (cmd->post) {
        post = cmd->post;
        ਵਾਪਸ post->post_handler(cf, post, ਖੇਤਰ);
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_str_array_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ  *p = conf;

    ngx_str_t         *ਮੁੱਲ, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->ਵਿੱਥ);

    ਜੇ (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, ਆਕਾਰ(ngx_str_t));
        ਜੇ (*a == ਨਿਰਾ) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }
    }

    s = ngx_array_ਧੱਕੋ(*a);
    ਜੇ (s == ਨਿਰਾ) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    *s = ਮੁੱਲ[1];

    ਜੇ (cmd->post) {
        post = cmd->post;
        ਵਾਪਸ post->post_handler(cf, post, s);
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_keyval_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ  *p = conf;

    ngx_str_t         *ਮੁੱਲ;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->ਵਿੱਥ);

    ਜੇ (*a == NGX_CONF_UNSET_PTR || *a == ਨਿਰਾ) {
        *a = ngx_array_create(cf->pool, 4, ਆਕਾਰ(ngx_keyval_t));
        ਜੇ (*a == ਨਿਰਾ) {
            ਵਾਪਸ NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_ਧੱਕੋ(*a);
    ਜੇ (kv == ਨਿਰਾ) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    kv->ਕੁੰਜੀ = ਮੁੱਲ[1];
    kv->ਮੁੱਲ = ਮੁੱਲ[2];

    ਜੇ (cmd->post) {
        post = cmd->post;
        ਵਾਪਸ post->post_handler(cf, post, kv);
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_ਅੰਕ_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *ਮੁੱਲ;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->ਵਿੱਥ);

    ਜੇ (*np != NGX_CONF_UNSET) {
        ਵਾਪਸ "is duplicate";
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;
    *np = ngx_atoi(ਮੁੱਲ[1].ਅੰਕੜੇ, ਮੁੱਲ[1].len);
    ਜੇ (*np == NGX_ERROR) {
        ਵਾਪਸ "invalid number";
    }

    ਜੇ (cmd->post) {
        post = cmd->post;
        ਵਾਪਸ post->post_handler(cf, post, np);
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_ਮਾਪ_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ  *p = conf;

    ਮਾਪ_t           *sp;
    ngx_str_t        *ਮੁੱਲ;
    ngx_conf_post_t  *post;


    sp = (ਮਾਪ_t *) (p + cmd->ਵਿੱਥ);
    ਜੇ (*sp != NGX_CONF_UNSET_ਮਾਪ) {
        ਵਾਪਸ "is duplicate";
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    *sp = ngx_parse_ਮਾਪ(&ਮੁੱਲ[1]);
    ਜੇ (*sp == (ਮਾਪ_t) NGX_ERROR) {
        ਵਾਪਸ "invalid value";
    }

    ਜੇ (cmd->post) {
        post = cmd->post;
        ਵਾਪਸ post->post_handler(cf, post, sp);
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_off_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ  *p = conf;

    off_t            *op;
    ngx_str_t        *ਮੁੱਲ;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->ਵਿੱਥ);
    ਜੇ (*op != NGX_CONF_UNSET) {
        ਵਾਪਸ "is duplicate";
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    *op = ngx_parse_ਵਿੱਥ(&ਮੁੱਲ[1]);
    ਜੇ (*op == (off_t) NGX_ERROR) {
        ਵਾਪਸ "invalid value";
    }

    ਜੇ (cmd->post) {
        post = cmd->post;
        ਵਾਪਸ post->post_handler(cf, post, op);
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_msec_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *ਮੁੱਲ;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->ਵਿੱਥ);
    ਜੇ (*msp != NGX_CONF_UNSET_MSEC) {
        ਵਾਪਸ "is duplicate";
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    *msp = ngx_parse_ਸਮਾਂ(&ਮੁੱਲ[1], 0);
    ਜੇ (*msp == (ngx_msec_t) NGX_ERROR) {
        ਵਾਪਸ "invalid value";
    }

    ਜੇ (cmd->post) {
        post = cmd->post;
        ਵਾਪਸ post->post_handler(cf, post, msp);
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_sec_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ  *p = conf;

    ਸਮਾਂ_t           *sp;
    ngx_str_t        *ਮੁੱਲ;
    ngx_conf_post_t  *post;


    sp = (ਸਮਾਂ_t *) (p + cmd->ਵਿੱਥ);
    ਜੇ (*sp != NGX_CONF_UNSET) {
        ਵਾਪਸ "is duplicate";
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    *sp = ngx_parse_ਸਮਾਂ(&ਮੁੱਲ[1], 1);
    ਜੇ (*sp == (ਸਮਾਂ_t) NGX_ERROR) {
        ਵਾਪਸ "invalid value";
    }

    ਜੇ (cmd->post) {
        post = cmd->post;
        ਵਾਪਸ post->post_handler(cf, post, sp);
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_bufs_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ *p = conf;

    ngx_str_t   *ਮੁੱਲ;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->ਵਿੱਥ);
    ਜੇ (bufs->ਅੰਕ) {
        ਵਾਪਸ "is duplicate";
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    bufs->ਅੰਕ = ngx_atoi(ਮੁੱਲ[1].ਅੰਕੜੇ, ਮੁੱਲ[1].len);
    ਜੇ (bufs->ਅੰਕ == NGX_ERROR || bufs->ਅੰਕ == 0) {
        ਵਾਪਸ "invalid value";
    }

    bufs->ਮਾਪ = ngx_parse_ਮਾਪ(&ਮੁੱਲ[2]);
    ਜੇ (bufs->ਮਾਪ == (ਮਾਪ_t) NGX_ERROR || bufs->ਮਾਪ == 0) {
        ਵਾਪਸ "invalid value";
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_enum_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *ਮੁੱਲ;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->ਵਿੱਥ);

    ਜੇ (*np != NGX_CONF_UNSET_UINT) {
        ਵਾਪਸ "is duplicate";
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;
    e = cmd->post;

    ਲਈ (i = 0; e[i].name.len != 0; i++) {
        ਜੇ (e[i].name.len != ਮੁੱਲ[1].len
            || ngx_strcasecmp(e[i].name.ਅੰਕੜੇ, ਮੁੱਲ[1].ਅੰਕੜੇ) != 0)
        {
            ਜਾਰੀ;
        }

        *np = e[i].ਮੁੱਲ;

        ਵਾਪਸ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", ਮੁੱਲ[1].ਅੰਕੜੇ);

    ਵਾਪਸ NGX_CONF_ERROR;
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_bitmask_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *ਮੁੱਲ;
    ngx_conf_bitmask_t  *ਨਕਾਬ;


    np = (ngx_uint_t *) (p + cmd->ਵਿੱਥ);
    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;
    ਨਕਾਬ = cmd->post;

    ਲਈ (i = 1; i < cf->ਦਲੀਲਾਂ->nelts; i++) {
        ਲਈ (m = 0; ਨਕਾਬ[m].name.len != 0; m++) {

            ਜੇ (ਨਕਾਬ[m].name.len != ਮੁੱਲ[i].len
                || ngx_strcasecmp(ਨਕਾਬ[m].name.ਅੰਕੜੇ, ਮੁੱਲ[i].ਅੰਕੜੇ) != 0)
            {
                ਜਾਰੀ;
            }

            ਜੇ (*np & ਨਕਾਬ[m].ਨਕਾਬ) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", ਮੁੱਲ[i].ਅੰਕੜੇ);

            } ਨਹੀਂਤਾਂ {
                *np |= ਨਕਾਬ[m].ਨਕਾਬ;
            }

            ਰੋਕੋ;
        }

        ਜੇ (ਨਕਾਬ[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", ਮੁੱਲ[i].ਅੰਕੜੇ);

            ਵਾਪਸ NGX_CONF_ERROR;
        }
    }

    ਵਾਪਸ NGX_CONF_OK;
}


#ਜੇ 0

ਅੱਖਰ *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਵਾਪਸ "unsupported on this platform";
}

#ਜੇ_ਅੰਤ


ਅੱਖਰ *
ngx_conf_deprecated(ngx_conf_t *cf, ਖਾਲੀ *post, ਖਾਲੀ *ਅੰਕੜੇ)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    ਵਾਪਸ NGX_CONF_OK;
}


ਅੱਖਰ *
ngx_conf_check_ਅੰਕ_bounds(ngx_conf_t *cf, ਖਾਲੀ *post, ਖਾਲੀ *ਅੰਕੜੇ)
{
    ngx_conf_ਅੰਕ_bounds_t  *bounds = post;
    ngx_int_t  *np = ਅੰਕੜੇ;

    ਜੇ (bounds->ਉੱਚਾ == -1) {
        ਜੇ (*np >= bounds->ਨੀਵਾਂ) {
            ਵਾਪਸ NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->ਨੀਵਾਂ);

        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਜੇ (*np >= bounds->ਨੀਵਾਂ && *np <= bounds->ਉੱਚਾ) {
        ਵਾਪਸ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->ਨੀਵਾਂ, bounds->ਉੱਚਾ);

    ਵਾਪਸ NGX_CONF_ERROR;
}


ਅੱਖਰ *
ngx_conf_check_ਮਾਪ_nonzero(ngx_conf_t *cf, ਖਾਲੀ *post, ਖਾਲੀ *ਅੰਕੜੇ)
{
    ਮਾਪ_t  *sp = ਅੰਕੜੇ;

    ਜੇ (*sp == 0) {
        ਵਾਪਸ "value must be nonzero";
    }

    ਵਾਪਸ NGX_CONF_OK;
}
