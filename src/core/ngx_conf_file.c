
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>

#ವ್ಯಾಖ್ಯಾನಿಸು NGX_CONF_ಬಫರ್  4096

ಸ್ಥಿರ ngx_int_t ngx_conf_ಕೂಡಿಸು_dump(ngx_conf_t *cf, ngx_str_t *filename);
ಸ್ಥಿರ ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t ಕೊನೆಯ);
ಸ್ಥಿರ ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_conf_flush_files(ngx_ಚಕ್ರ_t *ಚಕ್ರ);


ಸ್ಥಿರ ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      ಖಾಲಿ },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    ಖಾಲಿ,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    ಖಾಲಿ,                                  /* init master */
    ಖಾಲಿ,                                  /* init module */
    ಖಾಲಿ,                                  /* init process */
    ಖಾಲಿ,                                  /* init thread */
    ಖಾಲಿ,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    ಖಾಲಿ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

ಸ್ಥಿರ ngx_uint_t ವಾದ_ಸಂಖ್ಯೆ[] = {
    NGX_CONF_NOARGS,
    NGX_CONF_TAKE1,
    NGX_CONF_TAKE2,
    NGX_CONF_TAKE3,
    NGX_CONF_TAKE4,
    NGX_CONF_TAKE5,
    NGX_CONF_TAKE6,
    NGX_CONF_TAKE7
};


ngx_conf_post_t  ngx_conf_ಗಾತ್ರ_nonzero_post = { ngx_conf_check_ಗಾತ್ರ_nonzero };


ಅಕ್ಷರ *
ngx_conf_param(ngx_conf_t *cf)
{
    ಅಕ್ಷರ             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->ಚಕ್ರ->conf_param;

    ಆದರೆ (param->len == 0) {
        ಹಿಂತಿರುಗಿ NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, ಗಾತ್ರ(ngx_conf_file_t));

    ngx_memzero(&b, ಗಾತ್ರ(ngx_buf_t));

    b.ಆರಂಭ = param->ದತ್ತಾಂಶ;
    b.pos = param->ದತ್ತಾಂಶ;
    b.ಕೊನೆಯ = param->ದತ್ತಾಂಶ + param->len;
    b.ಕೊನೆ = b.ಕೊನೆಯ;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.ದತ್ತಾಂಶ = ಖಾಲಿ;
    conf_file.ಸಾಲು = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->ಬಫರ್ = &b;

    rv = ngx_conf_parse(cf, ಖಾಲಿ);

    cf->conf_file = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ rv;
}


ಸ್ಥಿರ ngx_int_t
ngx_conf_ಕೂಡಿಸು_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             ಗಾತ್ರ_2;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_ಗಂಟು_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_long(filename->ದತ್ತಾಂಶ, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->ಚಕ್ರ->config_dump_rbtree, filename, hash);

    ಆದರೆ (sn) {
        cf->conf_file->dump = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    p = ngx_pstrdup(cf->ಚಕ್ರ->pool, filename);
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    cd = ngx_array_ತಳ್ಳು(&cf->ಚಕ್ರ->config_dump);
    ಆದರೆ (cd == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಗಾತ್ರ_2 = ngx_file_ಗಾತ್ರ(&cf->conf_file->file.info);

    buf = ngx_create_ತಾತ್ಕಾಲಿಕ_buf(cf->ಚಕ್ರ->pool, (ಗಾತ್ರ_t) ಗಾತ್ರ_2);
    ಆದರೆ (buf == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    cd->name.ದತ್ತಾಂಶ = p;
    cd->name.len = filename->len;
    cd->ಬಫರ್ = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->ತಾತ್ಕಾಲಿಕ_pool, ಗಾತ್ರ(ngx_str_ಗಂಟು_t));
    ಆದರೆ (sn == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    sn->ಗಂಟು.ಕೀಲಿ = hash;
    sn->str = cd->name;

    ngx_rbtree_ಸೇರಿಸು(&cf->ಚಕ್ರ->config_dump_rbtree, &sn->ಗಂಟು);

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಅಕ್ಷರ *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    ಅಕ್ಷರ             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    ಎಣಿಕೆ {
        parse_file = 0,
        parse_ಖಂಡ,
        parse_param
    } type;

#ಆದರೆ (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->ದತ್ತಾಂಶ, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        ಆದರೆ (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->ದತ್ತಾಂಶ);
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        ಆದರೆ (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->ಲಘುಗಣಕ, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->ದತ್ತಾಂಶ);
        }

        cf->conf_file->ಬಫರ್ = &buf;

        buf.ಆರಂಭ = ngx_alloc(NGX_CONF_ಬಫರ್, cf->ಲಘುಗಣಕ);
        ಆದರೆ (buf.ಆರಂಭ == ಖಾಲಿ) {
            ಹೋಗು failed;
        }

        buf.pos = buf.ಆರಂಭ;
        buf.ಕೊನೆಯ = buf.ಆರಂಭ;
        buf.ಕೊನೆ = buf.ಕೊನೆಯ + NGX_CONF_ಬಫರ್;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.ದತ್ತಾಂಶ = filename->ದತ್ತಾಂಶ;
        cf->conf_file->file.ಸ್ಥಾನಾಂತರ = 0;
        cf->conf_file->file.ಲಘುಗಣಕ = cf->ಲಘುಗಣಕ;
        cf->conf_file->ಸಾಲು = 1;

        type = parse_file;

        ಆದರೆ (ngx_dump_config
#ಆದರೆ (NGX_CONTROL_API)
            || ngx_control_api_enabled
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ (NGX_DEBUG)
            || 1
#ಆದರೆ_ಅಂತ್ಯ
           )
        {
            ಆದರೆ (ngx_conf_ಕೂಡಿಸು_dump(cf, filename) != NGX_OK) {
                ಹೋಗು failed;
            }

        } ಇಲ್ಲದಿದ್ದರೆ {
            cf->conf_file->dump = ಖಾಲಿ;
        }

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_ಖಂಡ;

    } ಇಲ್ಲದಿದ್ದರೆ {
        type = parse_param;
    }


    ಗಾಗಿ ( ;; ) {
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

        ಆದರೆ (rc == NGX_ERROR) {
            ಹೋಗು ಮುಗಿದ;
        }

        ಆದರೆ (rc == NGX_CONF_ಖಂಡ_ಮುಗಿದ) {

            ಆದರೆ (type != parse_ಖಂಡ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                ಹೋಗು failed;
            }

            ಹೋಗು ಮುಗಿದ;
        }

        ಆದರೆ (rc == NGX_CONF_FILE_ಮುಗಿದ) {

            ಆದರೆ (type == parse_ಖಂಡ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                ಹೋಗು failed;
            }

            ಹೋಗು ಮುಗಿದ;
        }

        ಆದರೆ (rc == NGX_CONF_ಖಂಡ_ಆರಂಭ) {

            ಆದರೆ (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                ಹೋಗು failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        ಆದರೆ (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            ಆದರೆ (rc == NGX_CONF_ಖಂಡ_ಆರಂಭ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                ಹೋಗು failed;
            }

            rv = (*cf->handler)(cf, ಖಾಲಿ, cf->handler_conf);
            ಆದರೆ (rv == NGX_CONF_OK) {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (rv == NGX_CONF_ERROR) {
                ಹೋಗು failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            ಹೋಗು failed;
        }


        rc = ngx_conf_handler(cf, rc);

        ಆದರೆ (rc == NGX_ERROR) {
            ಹೋಗು failed;
        }
    }

failed:

    rc = NGX_ERROR;

ಮುಗಿದ:

    ಆದರೆ (filename) {
        ಆದರೆ (cf->conf_file->ಬಫರ್->ಆರಂಭ) {
            ngx_free(cf->conf_file->ಬಫರ್->ಆರಂಭ);
        }

        ಆದರೆ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->ಲಘುಗಣಕ, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->ದತ್ತಾಂಶ);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    ಆದರೆ (rc == NGX_ERROR) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t ಕೊನೆಯ)
{
    ಅಕ್ಷರ           *rv;
    ಶೂನ್ಯ           *conf, **confp;
    ngx_uint_t      i, ಸಿಕ್ಕಿತು;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->ವಾದಗಳು->elts;

    ಸಿಕ್ಕಿತು = 0;

    ಗಾಗಿ (i = 0; cf->ಚಕ್ರ->modules[i]; i++) {

        cmd = cf->ಚಕ್ರ->modules[i]->commands;
        ಆದರೆ (cmd == ಖಾಲಿ) {
            ಮುಂದುವರಿ;
        }

        ಗಾಗಿ ( /* void */ ; cmd->name.len; cmd++) {

            ಆದರೆ (name->len != cmd->name.len) {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (ngx_strcmp(name->ದತ್ತಾಂಶ, cmd->name.ದತ್ತಾಂಶ) != 0) {
                ಮುಂದುವರಿ;
            }

            ಸಿಕ್ಕಿತು = 1;

            ಆದರೆ (cf->ಚಕ್ರ->modules[i]->type != NGX_CONF_MODULE
                && cf->ಚಕ್ರ->modules[i]->type != cf->module_type)
            {
                ಮುಂದುವರಿ;
            }

            /* is the directive's location right ? */

            ಆದರೆ (!(cmd->type & cf->cmd_type)) {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (!(cmd->type & NGX_CONF_ಖಂಡ) && ಕೊನೆಯ != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->ದತ್ತಾಂಶ);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಆದರೆ ((cmd->type & NGX_CONF_ಖಂಡ) && ಕೊನೆಯ != NGX_CONF_ಖಂಡ_ಆರಂಭ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->ದತ್ತಾಂಶ);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            ಆದರೆ (!(cmd->type & NGX_CONF_ANY)) {

                ಆದರೆ (cmd->type & NGX_CONF_FLAG) {

                    ಆದರೆ (cf->ವಾದಗಳು->nelts != 2) {
                        ಹೋಗು invalid;
                    }

                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cmd->type & NGX_CONF_1_ಇನ್ನಷ್ಟು) {

                    ಆದರೆ (cf->ವಾದಗಳು->nelts < 2) {
                        ಹೋಗು invalid;
                    }

                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cmd->type & NGX_CONF_2_ಇನ್ನಷ್ಟು) {

                    ಆದರೆ (cf->ವಾದಗಳು->nelts < 3) {
                        ಹೋಗು invalid;
                    }

                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cf->ವಾದಗಳು->nelts > NGX_CONF_MAX_ವಾದಗಳು) {

                    ಹೋಗು invalid;

                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(cmd->type & ವಾದ_ಸಂಖ್ಯೆ[cf->ವಾದಗಳು->nelts - 1]))
                {
                    ಹೋಗು invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = ಖಾಲಿ;

            ಆದರೆ (cmd->type & NGX_ನೇರ_CONF) {
                conf = ((ಶೂನ್ಯ **) cf->ctx)[cf->ಚಕ್ರ->modules[i]->ಸೂಚ್ಯಂಕ];

            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cmd->type & NGX_MAIN_CONF) {
                conf = &(((ಶೂನ್ಯ **) cf->ctx)[cf->ಚಕ್ರ->modules[i]->ಸೂಚ್ಯಂಕ]);

            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cf->ctx) {
                confp = *(ಶೂನ್ಯ **) ((ಅಕ್ಷರ *) cf->ctx + cmd->conf);

                ಆದರೆ (confp) {
                    conf = confp[cf->ಚಕ್ರ->modules[i]->ctx_ಸೂಚ್ಯಂಕ];
                }
            }

            rv = cmd->ಗಣ(cf, cmd, conf);

            ಆದರೆ (rv == NGX_CONF_OK) {
                ಹಿಂತಿರುಗಿ NGX_OK;
            }

            ಆದರೆ (rv == NGX_CONF_ERROR) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->ದತ್ತಾಂಶ, rv);

            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }

    ಆದರೆ (ಸಿಕ್ಕಿತು) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->ದತ್ತಾಂಶ);

        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->ದತ್ತಾಂಶ);

    ಹಿಂತಿರುಗಿ NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->ದತ್ತಾಂಶ);

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}


ಸ್ಥಿರ ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *ಆರಂಭ, ch, *src, *dst;
    off_t        file_ಗಾತ್ರ;
    ಗಾತ್ರ_t       len;
    ssize_t      n, ಗಾತ್ರ_2;
    ngx_uint_t   ಸಿಕ್ಕಿತು, need_space, ಕೊನೆಯ_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, ಆರಂಭ_ಸಾಲು;
    ngx_str_t   *ಶಬ್ದ;
    ngx_buf_t   *b, *dump;

    ಸಿಕ್ಕಿತು = 0;
    need_space = 0;
    ಕೊನೆಯ_space = 1;
    sharp_comment = 0;
    variable = 0;
    quoted = 0;
    s_quoted = 0;
    d_quoted = 0;

    cf->ವಾದಗಳು->nelts = 0;
    b = cf->conf_file->ಬಫರ್;
    dump = cf->conf_file->dump;
    ಆರಂಭ = b->pos;
    ಆರಂಭ_ಸಾಲು = cf->conf_file->ಸಾಲು;

    file_ಗಾತ್ರ = ngx_file_ಗಾತ್ರ(&cf->conf_file->file.info);

    ಗಾಗಿ ( ;; ) {

        ಆದರೆ (b->pos >= b->ಕೊನೆಯ) {

            ಆದರೆ (cf->conf_file->file.ಸ್ಥಾನಾಂತರ >= file_ಗಾತ್ರ) {

                ಆದರೆ (cf->ವಾದಗಳು->nelts > 0 || !ಕೊನೆಯ_space) {

                    ಆದರೆ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        ಹಿಂತಿರುಗಿ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                ಹಿಂತಿರುಗಿ NGX_CONF_FILE_ಮುಗಿದ;
            }

            len = b->pos - ಆರಂಭ;

            ಆದರೆ (len == NGX_CONF_ಬಫರ್) {
                cf->conf_file->ಸಾಲು = ಆರಂಭ_ಸಾಲು;

                ಆದರೆ (d_quoted) {
                    ch = '"';

                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s_quoted) {
                    ch = '\'';

                } ಇಲ್ಲದಿದ್ದರೆ {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (ಗಾತ್ರ_t) 10, ಆರಂಭ);
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಆದರೆ (len) {
                ngx_memmove(b->ಆರಂಭ, ಆರಂಭ, len);
            }

            ಗಾತ್ರ_2 = (ssize_t) (file_ಗಾತ್ರ - cf->conf_file->file.ಸ್ಥಾನಾಂತರ);

            ಆದರೆ (ಗಾತ್ರ_2 > b->ಕೊನೆ - (b->ಆರಂಭ + len)) {
                ಗಾತ್ರ_2 = b->ಕೊನೆ - (b->ಆರಂಭ + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->ಆರಂಭ + len, ಗಾತ್ರ_2,
                              cf->conf_file->file.ಸ್ಥಾನಾಂತರ);

            ಆದರೆ (n == NGX_ERROR) {
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಆದರೆ (n != ಗಾತ್ರ_2) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, ಗಾತ್ರ_2);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            b->pos = b->ಆರಂಭ + len;
            b->ಕೊನೆಯ = b->pos + n;
            ಆರಂಭ = b->ಆರಂಭ;

            ಆದರೆ (dump) {
                dump->ಕೊನೆಯ = ngx_cpymem(dump->ಕೊನೆಯ, b->pos, ಗಾತ್ರ_2);
            }
        }

        ch = *b->pos++;

        ಆದರೆ (ch == LF) {
            cf->conf_file->ಸಾಲು++;

            ಆದರೆ (sharp_comment) {
                sharp_comment = 0;
            }
        }

        ಆದರೆ (sharp_comment) {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (quoted) {
            quoted = 0;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (need_space) {
            ಆದರೆ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                ಕೊನೆಯ_space = 1;
                need_space = 0;
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (ch == ';') {
                ಹಿಂತಿರುಗಿ NGX_OK;
            }

            ಆದರೆ (ch == '{') {
                ಹಿಂತಿರುಗಿ NGX_CONF_ಖಂಡ_ಆರಂಭ;
            }

            ಆದರೆ (ch == ')') {
                ಕೊನೆಯ_space = 1;
                need_space = 0;

            } ಇಲ್ಲದಿದ್ದರೆ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
        }

        ಆದರೆ (ಕೊನೆಯ_space) {

            ಆರಂಭ = b->pos - 1;
            ಆರಂಭ_ಸಾಲು = cf->conf_file->ಸಾಲು;

            ಆದರೆ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                ಮುಂದುವರಿ;
            }

            ಆಯ್ಕೆ (ch) {

            ಸ್ಥಿತಿ ';':
            ಸ್ಥಿತಿ '{':
                ಆದರೆ (cf->ವಾದಗಳು->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                ಆದರೆ (ch == '{') {
                    ಹಿಂತಿರುಗಿ NGX_CONF_ಖಂಡ_ಆರಂಭ;
                }

                ಹಿಂತಿರುಗಿ NGX_OK;

            ಸ್ಥಿತಿ '}':
                ಆದರೆ (cf->ವಾದಗಳು->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                ಹಿಂತಿರುಗಿ NGX_CONF_ಖಂಡ_ಮುಗಿದ;

            ಸ್ಥಿತಿ '#':
                sharp_comment = 1;
                ಮುಂದುವರಿ;

            ಸ್ಥಿತಿ '\\':
                quoted = 1;
                ಕೊನೆಯ_space = 0;
                ಮುಂದುವರಿ;

            ಸ್ಥಿತಿ '"':
                ಆರಂಭ++;
                d_quoted = 1;
                ಕೊನೆಯ_space = 0;
                ಮುಂದುವರಿ;

            ಸ್ಥಿತಿ '\'':
                ಆರಂಭ++;
                s_quoted = 1;
                ಕೊನೆಯ_space = 0;
                ಮುಂದುವರಿ;

            ಸ್ಥಿತಿ '$':
                variable = 1;
                ಕೊನೆಯ_space = 0;
                ಮುಂದುವರಿ;

            ಪೂರ್ವನಿಯೋಜಿತ:
                ಕೊನೆಯ_space = 0;
            }

        } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (ch == '{' && variable) {
                ಮುಂದುವರಿ;
            }

            variable = 0;

            ಆದರೆ (ch == '\\') {
                quoted = 1;
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (ch == '$') {
                variable = 1;
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (d_quoted) {
                ಆದರೆ (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    ಸಿಕ್ಕಿತು = 1;
                }

            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s_quoted) {
                ಆದರೆ (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    ಸಿಕ್ಕಿತು = 1;
                }

            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                ಕೊನೆಯ_space = 1;
                ಸಿಕ್ಕಿತು = 1;
            }

            ಆದರೆ (ಸಿಕ್ಕಿತು) {
                ಶಬ್ದ = ngx_array_ತಳ್ಳು(cf->ವಾದಗಳು);
                ಆದರೆ (ಶಬ್ದ == ಖಾಲಿ) {
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                ಶಬ್ದ->ದತ್ತಾಂಶ = ngx_pnalloc(cf->pool, b->pos - 1 - ಆರಂಭ + 1);
                ಆದರೆ (ಶಬ್ದ->ದತ್ತಾಂಶ == ಖಾಲಿ) {
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                ಗಾಗಿ (dst = ಶಬ್ದ->ದತ್ತಾಂಶ, src = ಆರಂಭ, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    ಆದರೆ (*src == '\\') {
                        ಆಯ್ಕೆ (src[1]) {
                        ಸ್ಥಿತಿ '"':
                        ಸ್ಥಿತಿ '\'':
                        ಸ್ಥಿತಿ '\\':
                            src++;
                            ನಿಲ್ಲಿಸು;

                        ಸ್ಥಿತಿ 't':
                            *dst++ = '\t';
                            src += 2;
                            ಮುಂದುವರಿ;

                        ಸ್ಥಿತಿ 'r':
                            *dst++ = '\r';
                            src += 2;
                            ಮುಂದುವರಿ;

                        ಸ್ಥಿತಿ 'n':
                            *dst++ = '\n';
                            src += 2;
                            ಮುಂದುವರಿ;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                ಶಬ್ದ->len = len;

                ಆದರೆ (ch == ';') {
                    ಹಿಂತಿರುಗಿ NGX_OK;
                }

                ಆದರೆ (ch == '{') {
                    ಹಿಂತಿರುಗಿ NGX_CONF_ಖಂಡ_ಆರಂಭ;
                }

                ಸಿಕ್ಕಿತು = 0;
            }
        }
    }
}


ಅಕ್ಷರ *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ        *rv;
    ngx_int_t    n;
    ngx_str_t   *ಮೌಲ್ಯ, file, name;
    ngx_glob_t   gl;

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;
    file = ಮೌಲ್ಯ[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->ಲಘುಗಣಕ, 0, "include %s", file.ದತ್ತಾಂಶ);

    ಆದರೆ (ngx_conf_ತುಂಬಿದ_name(cf->ಚಕ್ರ, &file, 1) != NGX_OK) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಆದರೆ (strpbrk((ಅಕ್ಷರ *) file.ದತ್ತಾಂಶ, "*?[") == ಖಾಲಿ) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->ಲಘುಗಣಕ, 0, "include %s", file.ದತ್ತಾಂಶ);

        ಹಿಂತಿರುಗಿ ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, ಗಾತ್ರ(ngx_glob_t));

    gl.ನಮೂನೆ = file.ದತ್ತಾಂಶ;
    gl.ಲಘುಗಣಕ = cf->ಲಘುಗಣಕ;
    gl.test = 1;

    ಆದರೆ (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.ದತ್ತಾಂಶ);
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    ಗಾಗಿ ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        ಆದರೆ (n != NGX_OK) {
            ನಿಲ್ಲಿಸು;
        }

        file.len = name.len++;
        file.ದತ್ತಾಂಶ = ngx_pstrdup(cf->pool, &name);
        ಆದರೆ (file.ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->ಲಘುಗಣಕ, 0, "include %s", file.ದತ್ತಾಂಶ);

        rv = ngx_conf_parse(cf, &file);

        ಆದರೆ (rv != NGX_CONF_OK) {
            ನಿಲ್ಲಿಸು;
        }
    }

    ngx_close_glob(&gl);

    ಹಿಂತಿರುಗಿ rv;
}


ngx_int_t
ngx_conf_ತುಂಬಿದ_name(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_str_t *name, ngx_uint_t conf_ಪೂರ್ವಪ್ರತ್ಯಯ)
{
    ngx_str_t  *ಪೂರ್ವಪ್ರತ್ಯಯ;

    ಪೂರ್ವಪ್ರತ್ಯಯ = conf_ಪೂರ್ವಪ್ರತ್ಯಯ ? &ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ : &ಚಕ್ರ->ಪೂರ್ವಪ್ರತ್ಯಯ;

    ಹಿಂತಿರುಗಿ ngx_get_ತುಂಬಿದ_name(ಚಕ್ರ->pool, ಪೂರ್ವಪ್ರತ್ಯಯ, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_str_t *name)
{
    ngx_str_t         ತುಂಬಿದ;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#ಆದರೆ (NGX_SUPPRESS_WARN)
    ngx_str_null(&ತುಂಬಿದ);
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (name->len) {
        ತುಂಬಿದ = *name;

        ಆದರೆ (ngx_conf_ತುಂಬಿದ_name(ಚಕ್ರ, &ತುಂಬಿದ, 0) != NGX_OK) {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        part = &ಚಕ್ರ->open_files.part;
        file = part->elts;

        ಗಾಗಿ (i = 0; /* void */ ; i++) {

            ಆದರೆ (i >= part->nelts) {
                ಆದರೆ (part->ಮುಂದಿನ == ಖಾಲಿ) {
                    ನಿಲ್ಲಿಸು;
                }
                part = part->ಮುಂದಿನ;
                file = part->elts;
                i = 0;
            }

            ಆದರೆ (ತುಂಬಿದ.len != file[i].name.len) {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (ngx_strcmp(ತುಂಬಿದ.ದತ್ತಾಂಶ, file[i].name.ದತ್ತಾಂಶ) == 0) {
                ಹಿಂತಿರುಗಿ &file[i];
            }
        }
    }

    file = ngx_list_ತಳ್ಳು(&ಚಕ್ರ->open_files);
    ಆದರೆ (file == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = ತುಂಬಿದ;

    } ಇಲ್ಲದಿದ್ದರೆ {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = ಖಾಲಿ;
    file->ದತ್ತಾಂಶ = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ file;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_conf_flush_files(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ಚಕ್ರ->ಲಘುಗಣಕ, 0, "flush files");

    part = &ಚಕ್ರ->open_files.part;
    file = part->elts;

    ಗಾಗಿ (i = 0; /* void */ ; i++) {

        ಆದರೆ (i >= part->nelts) {
            ಆದರೆ (part->ಮುಂದಿನ == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }
            part = part->ಮುಂದಿನ;
            file = part->elts;
            i = 0;
        }

        ಆದರೆ (file[i].flush) {
            file[i].flush(&file[i], ಚಕ್ರ->ಲಘುಗಣಕ);
        }
    }
}


ಶೂನ್ಯ ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *ಕೊನೆಯ;
    va_list  ವಾದಗಳು;

    ಕೊನೆಯ = errstr + NGX_MAX_CONF_ERRSTR;

    va_ಆರಂಭ(ವಾದಗಳು, fmt);
    p = ngx_vslprintf(errstr, ಕೊನೆಯ, fmt, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);

    ಆದರೆ (err) {
        p = ngx_log_errno(p, ಕೊನೆಯ, err);
    }

    ಆದರೆ (cf->conf_file == ಖಾಲಿ) {
        ngx_log_error(level, cf->ಲಘುಗಣಕ, 0, "%*s", p - errstr, errstr);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->ಲಘುಗಣಕ, 0, "%*s in command line",
                      p - errstr, errstr);
        ಹಿಂತಿರುಗಿ;
    }

    ngx_log_error(level, cf->ಲಘುಗಣಕ, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.ದತ್ತಾಂಶ, cf->conf_file->ಸಾಲು);
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_flag_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ  *p = conf;

    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->ಸ್ಥಾನಾಂತರ);

    ಆದರೆ (*fp != NGX_CONF_UNSET) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    ಆದರೆ (ngx_strcasecmp(ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ, (u_char *) "on") == 0) {
        *fp = 1;

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strcasecmp(ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ, (u_char *) "off") == 0) {
        *fp = 0;

    } ಇಲ್ಲದಿದ್ದರೆ {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ, cmd->name.ದತ್ತಾಂಶ);
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಆದರೆ (cmd->post) {
        post = cmd->post;
        ಹಿಂತಿರುಗಿ post->post_handler(cf, post, fp);
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_str_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ  *p = conf;

    ngx_str_t        *ಕ್ಷೇತ್ರ, *ಮೌಲ್ಯ;
    ngx_conf_post_t  *post;

    ಕ್ಷೇತ್ರ = (ngx_str_t *) (p + cmd->ಸ್ಥಾನಾಂತರ);

    ಆದರೆ (ಕ್ಷೇತ್ರ->ದತ್ತಾಂಶ) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    *ಕ್ಷೇತ್ರ = ಮೌಲ್ಯ[1];

    ಆದರೆ (cmd->post) {
        post = cmd->post;
        ಹಿಂತಿರುಗಿ post->post_handler(cf, post, ಕ್ಷೇತ್ರ);
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_str_array_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ  *p = conf;

    ngx_str_t         *ಮೌಲ್ಯ, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->ಸ್ಥಾನಾಂತರ);

    ಆದರೆ (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, ಗಾತ್ರ(ngx_str_t));
        ಆದರೆ (*a == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }
    }

    s = ngx_array_ತಳ್ಳು(*a);
    ಆದರೆ (s == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    *s = ಮೌಲ್ಯ[1];

    ಆದರೆ (cmd->post) {
        post = cmd->post;
        ಹಿಂತಿರುಗಿ post->post_handler(cf, post, s);
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_keyval_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ  *p = conf;

    ngx_str_t         *ಮೌಲ್ಯ;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->ಸ್ಥಾನಾಂತರ);

    ಆದರೆ (*a == NGX_CONF_UNSET_PTR || *a == ಖಾಲಿ) {
        *a = ngx_array_create(cf->pool, 4, ಗಾತ್ರ(ngx_keyval_t));
        ಆದರೆ (*a == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_ತಳ್ಳು(*a);
    ಆದರೆ (kv == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    kv->ಕೀಲಿ = ಮೌಲ್ಯ[1];
    kv->ಮೌಲ್ಯ = ಮೌಲ್ಯ[2];

    ಆದರೆ (cmd->post) {
        post = cmd->post;
        ಹಿಂತಿರುಗಿ post->post_handler(cf, post, kv);
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_ಅಂಕೆ_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->ಸ್ಥಾನಾಂತರ);

    ಆದರೆ (*np != NGX_CONF_UNSET) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;
    *np = ngx_atoi(ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ, ಮೌಲ್ಯ[1].len);
    ಆದರೆ (*np == NGX_ERROR) {
        ಹಿಂತಿರುಗಿ "invalid number";
    }

    ಆದರೆ (cmd->post) {
        post = cmd->post;
        ಹಿಂತಿರುಗಿ post->post_handler(cf, post, np);
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_ಗಾತ್ರ_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ  *p = conf;

    ಗಾತ್ರ_t           *sp;
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_conf_post_t  *post;


    sp = (ಗಾತ್ರ_t *) (p + cmd->ಸ್ಥಾನಾಂತರ);
    ಆದರೆ (*sp != NGX_CONF_UNSET_ಗಾತ್ರ) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    *sp = ngx_parse_ಗಾತ್ರ(&ಮೌಲ್ಯ[1]);
    ಆದರೆ (*sp == (ಗಾತ್ರ_t) NGX_ERROR) {
        ಹಿಂತಿರುಗಿ "invalid value";
    }

    ಆದರೆ (cmd->post) {
        post = cmd->post;
        ಹಿಂತಿರುಗಿ post->post_handler(cf, post, sp);
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_off_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ  *p = conf;

    off_t            *op;
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->ಸ್ಥಾನಾಂತರ);
    ಆದರೆ (*op != NGX_CONF_UNSET) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    *op = ngx_parse_ಸ್ಥಾನಾಂತರ(&ಮೌಲ್ಯ[1]);
    ಆದರೆ (*op == (off_t) NGX_ERROR) {
        ಹಿಂತಿರುಗಿ "invalid value";
    }

    ಆದರೆ (cmd->post) {
        post = cmd->post;
        ಹಿಂತಿರುಗಿ post->post_handler(cf, post, op);
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_msec_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->ಸ್ಥಾನಾಂತರ);
    ಆದರೆ (*msp != NGX_CONF_UNSET_MSEC) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    *msp = ngx_parse_ಸಮಯ(&ಮೌಲ್ಯ[1], 0);
    ಆದರೆ (*msp == (ngx_msec_t) NGX_ERROR) {
        ಹಿಂತಿರುಗಿ "invalid value";
    }

    ಆದರೆ (cmd->post) {
        post = cmd->post;
        ಹಿಂತಿರುಗಿ post->post_handler(cf, post, msp);
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_sec_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ  *p = conf;

    ಸಮಯ_t           *sp;
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_conf_post_t  *post;


    sp = (ಸಮಯ_t *) (p + cmd->ಸ್ಥಾನಾಂತರ);
    ಆದರೆ (*sp != NGX_CONF_UNSET) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    *sp = ngx_parse_ಸಮಯ(&ಮೌಲ್ಯ[1], 1);
    ಆದರೆ (*sp == (ಸಮಯ_t) NGX_ERROR) {
        ಹಿಂತಿರುಗಿ "invalid value";
    }

    ಆದರೆ (cmd->post) {
        post = cmd->post;
        ಹಿಂತಿರುಗಿ post->post_handler(cf, post, sp);
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_bufs_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ *p = conf;

    ngx_str_t   *ಮೌಲ್ಯ;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->ಸ್ಥಾನಾಂತರ);
    ಆದರೆ (bufs->ಅಂಕೆ) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    bufs->ಅಂಕೆ = ngx_atoi(ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ, ಮೌಲ್ಯ[1].len);
    ಆದರೆ (bufs->ಅಂಕೆ == NGX_ERROR || bufs->ಅಂಕೆ == 0) {
        ಹಿಂತಿರುಗಿ "invalid value";
    }

    bufs->ಗಾತ್ರ_2 = ngx_parse_ಗಾತ್ರ(&ಮೌಲ್ಯ[2]);
    ಆದರೆ (bufs->ಗಾತ್ರ_2 == (ಗಾತ್ರ_t) NGX_ERROR || bufs->ಗಾತ್ರ_2 == 0) {
        ಹಿಂತಿರುಗಿ "invalid value";
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_enum_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *ಮೌಲ್ಯ;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->ಸ್ಥಾನಾಂತರ);

    ಆದರೆ (*np != NGX_CONF_UNSET_UINT) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;
    e = cmd->post;

    ಗಾಗಿ (i = 0; e[i].name.len != 0; i++) {
        ಆದರೆ (e[i].name.len != ಮೌಲ್ಯ[1].len
            || ngx_strcasecmp(e[i].name.ದತ್ತಾಂಶ, ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ) != 0)
        {
            ಮುಂದುವರಿ;
        }

        *np = e[i].ಮೌಲ್ಯ;

        ಹಿಂತಿರುಗಿ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", ಮೌಲ್ಯ[1].ದತ್ತಾಂಶ);

    ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_bitmask_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *ಮೌಲ್ಯ;
    ngx_conf_bitmask_t  *ಮುಖವಾಡ;


    np = (ngx_uint_t *) (p + cmd->ಸ್ಥಾನಾಂತರ);
    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;
    ಮುಖವಾಡ = cmd->post;

    ಗಾಗಿ (i = 1; i < cf->ವಾದಗಳು->nelts; i++) {
        ಗಾಗಿ (m = 0; ಮುಖವಾಡ[m].name.len != 0; m++) {

            ಆದರೆ (ಮುಖವಾಡ[m].name.len != ಮೌಲ್ಯ[i].len
                || ngx_strcasecmp(ಮುಖವಾಡ[m].name.ದತ್ತಾಂಶ, ಮೌಲ್ಯ[i].ದತ್ತಾಂಶ) != 0)
            {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (*np & ಮುಖವಾಡ[m].ಮುಖವಾಡ) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", ಮೌಲ್ಯ[i].ದತ್ತಾಂಶ);

            } ಇಲ್ಲದಿದ್ದರೆ {
                *np |= ಮುಖವಾಡ[m].ಮುಖವಾಡ;
            }

            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (ಮುಖವಾಡ[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", ಮೌಲ್ಯ[i].ದತ್ತಾಂಶ);

            ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
        }
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


#ಆದರೆ 0

ಅಕ್ಷರ *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಹಿಂತಿರುಗಿ "unsupported on this platform";
}

#ಆದರೆ_ಅಂತ್ಯ


ಅಕ್ಷರ *
ngx_conf_deprecated(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಅಕ್ಷರ *
ngx_conf_check_ಅಂಕೆ_bounds(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ngx_conf_ಅಂಕೆ_bounds_t  *bounds = post;
    ngx_int_t  *np = ದತ್ತಾಂಶ;

    ಆದರೆ (bounds->ಮೇಲಿನ == -1) {
        ಆದರೆ (*np >= bounds->ಕೆಳಗಿನ) {
            ಹಿಂತಿರುಗಿ NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->ಕೆಳಗಿನ);

        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಆದರೆ (*np >= bounds->ಕೆಳಗಿನ && *np <= bounds->ಮೇಲಿನ) {
        ಹಿಂತಿರುಗಿ NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->ಕೆಳಗಿನ, bounds->ಮೇಲಿನ);

    ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
}


ಅಕ್ಷರ *
ngx_conf_check_ಗಾತ್ರ_nonzero(ngx_conf_t *cf, ಶೂನ್ಯ *post, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ಗಾತ್ರ_t  *sp = ದತ್ತಾಂಶ;

    ಆದರೆ (*sp == 0) {
        ಹಿಂತಿರುಗಿ "value must be nonzero";
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}
