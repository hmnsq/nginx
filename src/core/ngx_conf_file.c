
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>

#నిర్వచించు NGX_CONF_బఫరు  4096

స్థిర ngx_int_t ngx_conf_కూడు_dump(ngx_conf_t *cf, ngx_str_t *filename);
స్థిర ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t చివరి);
స్థిర ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
స్థిర శూన్య ngx_conf_flush_files(ngx_చక్రం_t *చక్రం);


స్థిర ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      ఖాళీ },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    ఖాళీ,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    ఖాళీ,                                  /* init master */
    ఖాళీ,                                  /* init module */
    ఖాళీ,                                  /* init process */
    ఖాళీ,                                  /* init thread */
    ఖాళీ,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    ఖాళీ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

స్థిర ngx_uint_t వాదం_సంఖ్య[] = {
    NGX_CONF_NOARGS,
    NGX_CONF_TAKE1,
    NGX_CONF_TAKE2,
    NGX_CONF_TAKE3,
    NGX_CONF_TAKE4,
    NGX_CONF_TAKE5,
    NGX_CONF_TAKE6,
    NGX_CONF_TAKE7
};


ngx_conf_post_t  ngx_conf_కొలత_nonzero_post = { ngx_conf_check_కొలత_nonzero };


అక్షరం *
ngx_conf_param(ngx_conf_t *cf)
{
    అక్షరం             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->చక్రం->conf_param;

    ఒకవేళ (param->len == 0) {
        తిరిగి NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, పరిమాణం(ngx_conf_file_t));

    ngx_memzero(&b, పరిమాణం(ngx_buf_t));

    b.ప్రారంభం = param->దత్తాంశం;
    b.pos = param->దత్తాంశం;
    b.చివరి = param->దత్తాంశం + param->len;
    b.చివర = b.చివరి;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.దత్తాంశం = ఖాళీ;
    conf_file.పంక్తి = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->బఫరు = &b;

    rv = ngx_conf_parse(cf, ఖాళీ);

    cf->conf_file = ఖాళీ;

    తిరిగి rv;
}


స్థిర ngx_int_t
ngx_conf_కూడు_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             కొలత;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_కణుపు_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_చక్రీయ_తనిఖీ_32_long(filename->దత్తాంశం, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->చక్రం->config_dump_rbtree, filename, hash);

    ఒకవేళ (sn) {
        cf->conf_file->dump = ఖాళీ;
        తిరిగి NGX_OK;
    }

    p = ngx_pstrdup(cf->చక్రం->pool, filename);
    ఒకవేళ (p == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    cd = ngx_array_నెట్టు(&cf->చక్రం->config_dump);
    ఒకవేళ (cd == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    కొలత = ngx_file_కొలత(&cf->conf_file->file.info);

    buf = ngx_create_తాత్కాలిక_buf(cf->చక్రం->pool, (కొలత_t) కొలత);
    ఒకవేళ (buf == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    cd->name.దత్తాంశం = p;
    cd->name.len = filename->len;
    cd->బఫరు = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->తాత్కాలిక_pool, పరిమాణం(ngx_str_కణుపు_t));
    ఒకవేళ (sn == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    sn->కణుపు.కీలకం = hash;
    sn->str = cd->name;

    ngx_rbtree_చొప్పించు(&cf->చక్రం->config_dump_rbtree, &sn->కణుపు);

    తిరిగి NGX_OK;
}


అక్షరం *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    అక్షరం             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    గణన {
        parse_file = 0,
        parse_ఖండం,
        parse_param
    } type;

#ఒకవేళ (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = ఖాళీ;
#అయితే_ముగింపు

    ఒకవేళ (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->దత్తాంశం, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        ఒకవేళ (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->దత్తాంశం);
            తిరిగి NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        ఒకవేళ (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->సంవర్గమానం, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->దత్తాంశం);
        }

        cf->conf_file->బఫరు = &buf;

        buf.ప్రారంభం = ngx_alloc(NGX_CONF_బఫరు, cf->సంవర్గమానం);
        ఒకవేళ (buf.ప్రారంభం == ఖాళీ) {
            వెళ్ళు failed;
        }

        buf.pos = buf.ప్రారంభం;
        buf.చివరి = buf.ప్రారంభం;
        buf.చివర = buf.చివరి + NGX_CONF_బఫరు;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.దత్తాంశం = filename->దత్తాంశం;
        cf->conf_file->file.స్థానభ్రంశం = 0;
        cf->conf_file->file.సంవర్గమానం = cf->సంవర్గమానం;
        cf->conf_file->పంక్తి = 1;

        type = parse_file;

        ఒకవేళ (ngx_dump_config
#ఒకవేళ (NGX_CONTROL_API)
            || ngx_control_api_enabled
#అయితే_ముగింపు
#ఒకవేళ (NGX_DEBUG)
            || 1
#అయితే_ముగింపు
           )
        {
            ఒకవేళ (ngx_conf_కూడు_dump(cf, filename) != NGX_OK) {
                వెళ్ళు failed;
            }

        } లేకపోతే {
            cf->conf_file->dump = ఖాళీ;
        }

    } లేకపోతే ఒకవేళ (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_ఖండం;

    } లేకపోతే {
        type = parse_param;
    }


    కొరకు ( ;; ) {
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

        ఒకవేళ (rc == NGX_ERROR) {
            వెళ్ళు పూర్తి;
        }

        ఒకవేళ (rc == NGX_CONF_ఖండం_పూర్తి) {

            ఒకవేళ (type != parse_ఖండం) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                వెళ్ళు failed;
            }

            వెళ్ళు పూర్తి;
        }

        ఒకవేళ (rc == NGX_CONF_FILE_పూర్తి) {

            ఒకవేళ (type == parse_ఖండం) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                వెళ్ళు failed;
            }

            వెళ్ళు పూర్తి;
        }

        ఒకవేళ (rc == NGX_CONF_ఖండం_ప్రారంభం) {

            ఒకవేళ (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                వెళ్ళు failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        ఒకవేళ (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            ఒకవేళ (rc == NGX_CONF_ఖండం_ప్రారంభం) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                వెళ్ళు failed;
            }

            rv = (*cf->handler)(cf, ఖాళీ, cf->handler_conf);
            ఒకవేళ (rv == NGX_CONF_OK) {
                కొనసాగు;
            }

            ఒకవేళ (rv == NGX_CONF_ERROR) {
                వెళ్ళు failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            వెళ్ళు failed;
        }


        rc = ngx_conf_handler(cf, rc);

        ఒకవేళ (rc == NGX_ERROR) {
            వెళ్ళు failed;
        }
    }

failed:

    rc = NGX_ERROR;

పూర్తి:

    ఒకవేళ (filename) {
        ఒకవేళ (cf->conf_file->బఫరు->ప్రారంభం) {
            ngx_free(cf->conf_file->బఫరు->ప్రారంభం);
        }

        ఒకవేళ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->సంవర్గమానం, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->దత్తాంశం);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    ఒకవేళ (rc == NGX_ERROR) {
        తిరిగి NGX_CONF_ERROR;
    }

    తిరిగి NGX_CONF_OK;
}


స్థిర ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t చివరి)
{
    అక్షరం           *rv;
    శూన్య           *conf, **confp;
    ngx_uint_t      i, దొరికింది;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->వాదనలు->elts;

    దొరికింది = 0;

    కొరకు (i = 0; cf->చక్రం->modules[i]; i++) {

        cmd = cf->చక్రం->modules[i]->commands;
        ఒకవేళ (cmd == ఖాళీ) {
            కొనసాగు;
        }

        కొరకు ( /* void */ ; cmd->name.len; cmd++) {

            ఒకవేళ (name->len != cmd->name.len) {
                కొనసాగు;
            }

            ఒకవేళ (ngx_strcmp(name->దత్తాంశం, cmd->name.దత్తాంశం) != 0) {
                కొనసాగు;
            }

            దొరికింది = 1;

            ఒకవేళ (cf->చక్రం->modules[i]->type != NGX_CONF_MODULE
                && cf->చక్రం->modules[i]->type != cf->module_type)
            {
                కొనసాగు;
            }

            /* is the directive's location right ? */

            ఒకవేళ (!(cmd->type & cf->cmd_type)) {
                కొనసాగు;
            }

            ఒకవేళ (!(cmd->type & NGX_CONF_ఖండం) && చివరి != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->దత్తాంశం);
                తిరిగి NGX_ERROR;
            }

            ఒకవేళ ((cmd->type & NGX_CONF_ఖండం) && చివరి != NGX_CONF_ఖండం_ప్రారంభం) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->దత్తాంశం);
                తిరిగి NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            ఒకవేళ (!(cmd->type & NGX_CONF_ANY)) {

                ఒకవేళ (cmd->type & NGX_CONF_FLAG) {

                    ఒకవేళ (cf->వాదనలు->nelts != 2) {
                        వెళ్ళు invalid;
                    }

                } లేకపోతే ఒకవేళ (cmd->type & NGX_CONF_1_మరిన్ని) {

                    ఒకవేళ (cf->వాదనలు->nelts < 2) {
                        వెళ్ళు invalid;
                    }

                } లేకపోతే ఒకవేళ (cmd->type & NGX_CONF_2_మరిన్ని) {

                    ఒకవేళ (cf->వాదనలు->nelts < 3) {
                        వెళ్ళు invalid;
                    }

                } లేకపోతే ఒకవేళ (cf->వాదనలు->nelts > NGX_CONF_MAX_వాదనలు) {

                    వెళ్ళు invalid;

                } లేకపోతే ఒకవేళ (!(cmd->type & వాదం_సంఖ్య[cf->వాదనలు->nelts - 1]))
                {
                    వెళ్ళు invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = ఖాళీ;

            ఒకవేళ (cmd->type & NGX_ప్రత్యక్ష_CONF) {
                conf = ((శూన్య **) cf->ctx)[cf->చక్రం->modules[i]->సూచీ];

            } లేకపోతే ఒకవేళ (cmd->type & NGX_MAIN_CONF) {
                conf = &(((శూన్య **) cf->ctx)[cf->చక్రం->modules[i]->సూచీ]);

            } లేకపోతే ఒకవేళ (cf->ctx) {
                confp = *(శూన్య **) ((అక్షరం *) cf->ctx + cmd->conf);

                ఒకవేళ (confp) {
                    conf = confp[cf->చక్రం->modules[i]->ctx_సూచీ];
                }
            }

            rv = cmd->సమితి(cf, cmd, conf);

            ఒకవేళ (rv == NGX_CONF_OK) {
                తిరిగి NGX_OK;
            }

            ఒకవేళ (rv == NGX_CONF_ERROR) {
                తిరిగి NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->దత్తాంశం, rv);

            తిరిగి NGX_ERROR;
        }
    }

    ఒకవేళ (దొరికింది) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->దత్తాంశం);

        తిరిగి NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->దత్తాంశం);

    తిరిగి NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->దత్తాంశం);

    తిరిగి NGX_ERROR;
}


స్థిర ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *ప్రారంభం, ch, *src, *dst;
    off_t        file_కొలత;
    కొలత_t       len;
    ssize_t      n, కొలత;
    ngx_uint_t   దొరికింది, need_space, చివరి_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, ప్రారంభం_పంక్తి;
    ngx_str_t   *మాట;
    ngx_buf_t   *b, *dump;

    దొరికింది = 0;
    need_space = 0;
    చివరి_space = 1;
    sharp_comment = 0;
    variable = 0;
    quoted = 0;
    s_quoted = 0;
    d_quoted = 0;

    cf->వాదనలు->nelts = 0;
    b = cf->conf_file->బఫరు;
    dump = cf->conf_file->dump;
    ప్రారంభం = b->pos;
    ప్రారంభం_పంక్తి = cf->conf_file->పంక్తి;

    file_కొలత = ngx_file_కొలత(&cf->conf_file->file.info);

    కొరకు ( ;; ) {

        ఒకవేళ (b->pos >= b->చివరి) {

            ఒకవేళ (cf->conf_file->file.స్థానభ్రంశం >= file_కొలత) {

                ఒకవేళ (cf->వాదనలు->nelts > 0 || !చివరి_space) {

                    ఒకవేళ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        తిరిగి NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    తిరిగి NGX_ERROR;
                }

                తిరిగి NGX_CONF_FILE_పూర్తి;
            }

            len = b->pos - ప్రారంభం;

            ఒకవేళ (len == NGX_CONF_బఫరు) {
                cf->conf_file->పంక్తి = ప్రారంభం_పంక్తి;

                ఒకవేళ (d_quoted) {
                    ch = '"';

                } లేకపోతే ఒకవేళ (s_quoted) {
                    ch = '\'';

                } లేకపోతే {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (కొలత_t) 10, ప్రారంభం);
                    తిరిగి NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                తిరిగి NGX_ERROR;
            }

            ఒకవేళ (len) {
                ngx_memmove(b->ప్రారంభం, ప్రారంభం, len);
            }

            కొలత = (ssize_t) (file_కొలత - cf->conf_file->file.స్థానభ్రంశం);

            ఒకవేళ (కొలత > b->చివర - (b->ప్రారంభం + len)) {
                కొలత = b->చివర - (b->ప్రారంభం + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->ప్రారంభం + len, కొలత,
                              cf->conf_file->file.స్థానభ్రంశం);

            ఒకవేళ (n == NGX_ERROR) {
                తిరిగి NGX_ERROR;
            }

            ఒకవేళ (n != కొలత) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, కొలత);
                తిరిగి NGX_ERROR;
            }

            b->pos = b->ప్రారంభం + len;
            b->చివరి = b->pos + n;
            ప్రారంభం = b->ప్రారంభం;

            ఒకవేళ (dump) {
                dump->చివరి = ngx_cpymem(dump->చివరి, b->pos, కొలత);
            }
        }

        ch = *b->pos++;

        ఒకవేళ (ch == LF) {
            cf->conf_file->పంక్తి++;

            ఒకవేళ (sharp_comment) {
                sharp_comment = 0;
            }
        }

        ఒకవేళ (sharp_comment) {
            కొనసాగు;
        }

        ఒకవేళ (quoted) {
            quoted = 0;
            కొనసాగు;
        }

        ఒకవేళ (need_space) {
            ఒకవేళ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                చివరి_space = 1;
                need_space = 0;
                కొనసాగు;
            }

            ఒకవేళ (ch == ';') {
                తిరిగి NGX_OK;
            }

            ఒకవేళ (ch == '{') {
                తిరిగి NGX_CONF_ఖండం_ప్రారంభం;
            }

            ఒకవేళ (ch == ')') {
                చివరి_space = 1;
                need_space = 0;

            } లేకపోతే {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                తిరిగి NGX_ERROR;
            }
        }

        ఒకవేళ (చివరి_space) {

            ప్రారంభం = b->pos - 1;
            ప్రారంభం_పంక్తి = cf->conf_file->పంక్తి;

            ఒకవేళ (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                కొనసాగు;
            }

            ఎంపిక (ch) {

            స్థితి ';':
            స్థితి '{':
                ఒకవేళ (cf->వాదనలు->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    తిరిగి NGX_ERROR;
                }

                ఒకవేళ (ch == '{') {
                    తిరిగి NGX_CONF_ఖండం_ప్రారంభం;
                }

                తిరిగి NGX_OK;

            స్థితి '}':
                ఒకవేళ (cf->వాదనలు->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    తిరిగి NGX_ERROR;
                }

                తిరిగి NGX_CONF_ఖండం_పూర్తి;

            స్థితి '#':
                sharp_comment = 1;
                కొనసాగు;

            స్థితి '\\':
                quoted = 1;
                చివరి_space = 0;
                కొనసాగు;

            స్థితి '"':
                ప్రారంభం++;
                d_quoted = 1;
                చివరి_space = 0;
                కొనసాగు;

            స్థితి '\'':
                ప్రారంభం++;
                s_quoted = 1;
                చివరి_space = 0;
                కొనసాగు;

            స్థితి '$':
                variable = 1;
                చివరి_space = 0;
                కొనసాగు;

            అప్రమేయ:
                చివరి_space = 0;
            }

        } లేకపోతే {
            ఒకవేళ (ch == '{' && variable) {
                కొనసాగు;
            }

            variable = 0;

            ఒకవేళ (ch == '\\') {
                quoted = 1;
                కొనసాగు;
            }

            ఒకవేళ (ch == '$') {
                variable = 1;
                కొనసాగు;
            }

            ఒకవేళ (d_quoted) {
                ఒకవేళ (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    దొరికింది = 1;
                }

            } లేకపోతే ఒకవేళ (s_quoted) {
                ఒకవేళ (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    దొరికింది = 1;
                }

            } లేకపోతే ఒకవేళ (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                చివరి_space = 1;
                దొరికింది = 1;
            }

            ఒకవేళ (దొరికింది) {
                మాట = ngx_array_నెట్టు(cf->వాదనలు);
                ఒకవేళ (మాట == ఖాళీ) {
                    తిరిగి NGX_ERROR;
                }

                మాట->దత్తాంశం = ngx_pnalloc(cf->pool, b->pos - 1 - ప్రారంభం + 1);
                ఒకవేళ (మాట->దత్తాంశం == ఖాళీ) {
                    తిరిగి NGX_ERROR;
                }

                కొరకు (dst = మాట->దత్తాంశం, src = ప్రారంభం, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    ఒకవేళ (*src == '\\') {
                        ఎంపిక (src[1]) {
                        స్థితి '"':
                        స్థితి '\'':
                        స్థితి '\\':
                            src++;
                            ఆపు;

                        స్థితి 't':
                            *dst++ = '\t';
                            src += 2;
                            కొనసాగు;

                        స్థితి 'r':
                            *dst++ = '\r';
                            src += 2;
                            కొనసాగు;

                        స్థితి 'n':
                            *dst++ = '\n';
                            src += 2;
                            కొనసాగు;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                మాట->len = len;

                ఒకవేళ (ch == ';') {
                    తిరిగి NGX_OK;
                }

                ఒకవేళ (ch == '{') {
                    తిరిగి NGX_CONF_ఖండం_ప్రారంభం;
                }

                దొరికింది = 0;
            }
        }
    }
}


అక్షరం *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం        *rv;
    ngx_int_t    n;
    ngx_str_t   *విలువ, file, name;
    ngx_glob_t   gl;

    విలువ = cf->వాదనలు->elts;
    file = విలువ[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->సంవర్గమానం, 0, "include %s", file.దత్తాంశం);

    ఒకవేళ (ngx_conf_నిండిన_name(cf->చక్రం, &file, 1) != NGX_OK) {
        తిరిగి NGX_CONF_ERROR;
    }

    ఒకవేళ (strpbrk((అక్షరం *) file.దత్తాంశం, "*?[") == ఖాళీ) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->సంవర్గమానం, 0, "include %s", file.దత్తాంశం);

        తిరిగి ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, పరిమాణం(ngx_glob_t));

    gl.నమూనా = file.దత్తాంశం;
    gl.సంవర్గమానం = cf->సంవర్గమానం;
    gl.test = 1;

    ఒకవేళ (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.దత్తాంశం);
        తిరిగి NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    కొరకు ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        ఒకవేళ (n != NGX_OK) {
            ఆపు;
        }

        file.len = name.len++;
        file.దత్తాంశం = ngx_pstrdup(cf->pool, &name);
        ఒకవేళ (file.దత్తాంశం == ఖాళీ) {
            తిరిగి NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->సంవర్గమానం, 0, "include %s", file.దత్తాంశం);

        rv = ngx_conf_parse(cf, &file);

        ఒకవేళ (rv != NGX_CONF_OK) {
            ఆపు;
        }
    }

    ngx_close_glob(&gl);

    తిరిగి rv;
}


ngx_int_t
ngx_conf_నిండిన_name(ngx_చక్రం_t *చక్రం, ngx_str_t *name, ngx_uint_t conf_ఉపసర్గ)
{
    ngx_str_t  *ఉపసర్గ;

    ఉపసర్గ = conf_ఉపసర్గ ? &చక్రం->conf_ఉపసర్గ : &చక్రం->ఉపసర్గ;

    తిరిగి ngx_get_నిండిన_name(చక్రం->pool, ఉపసర్గ, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_చక్రం_t *చక్రం, ngx_str_t *name)
{
    ngx_str_t         నిండిన;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#ఒకవేళ (NGX_SUPPRESS_WARN)
    ngx_str_null(&నిండిన);
#అయితే_ముగింపు

    ఒకవేళ (name->len) {
        నిండిన = *name;

        ఒకవేళ (ngx_conf_నిండిన_name(చక్రం, &నిండిన, 0) != NGX_OK) {
            తిరిగి ఖాళీ;
        }

        part = &చక్రం->open_files.part;
        file = part->elts;

        కొరకు (i = 0; /* void */ ; i++) {

            ఒకవేళ (i >= part->nelts) {
                ఒకవేళ (part->తదుపరి == ఖాళీ) {
                    ఆపు;
                }
                part = part->తదుపరి;
                file = part->elts;
                i = 0;
            }

            ఒకవేళ (నిండిన.len != file[i].name.len) {
                కొనసాగు;
            }

            ఒకవేళ (ngx_strcmp(నిండిన.దత్తాంశం, file[i].name.దత్తాంశం) == 0) {
                తిరిగి &file[i];
            }
        }
    }

    file = ngx_list_నెట్టు(&చక్రం->open_files);
    ఒకవేళ (file == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = నిండిన;

    } లేకపోతే {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = ఖాళీ;
    file->దత్తాంశం = ఖాళీ;

    తిరిగి file;
}


స్థిర శూన్య
ngx_conf_flush_files(ngx_చక్రం_t *చక్రం)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, చక్రం->సంవర్గమానం, 0, "flush files");

    part = &చక్రం->open_files.part;
    file = part->elts;

    కొరకు (i = 0; /* void */ ; i++) {

        ఒకవేళ (i >= part->nelts) {
            ఒకవేళ (part->తదుపరి == ఖాళీ) {
                ఆపు;
            }
            part = part->తదుపరి;
            file = part->elts;
            i = 0;
        }

        ఒకవేళ (file[i].flush) {
            file[i].flush(&file[i], చక్రం->సంవర్గమానం);
        }
    }
}


శూన్య ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    స్థిరరాశి అక్షరం *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *చివరి;
    va_list  వాదనలు;

    చివరి = errstr + NGX_MAX_CONF_ERRSTR;

    va_ప్రారంభం(వాదనలు, fmt);
    p = ngx_vslprintf(errstr, చివరి, fmt, వాదనలు);
    va_చివర(వాదనలు);

    ఒకవేళ (err) {
        p = ngx_log_errno(p, చివరి, err);
    }

    ఒకవేళ (cf->conf_file == ఖాళీ) {
        ngx_log_error(level, cf->సంవర్గమానం, 0, "%*s", p - errstr, errstr);
        తిరిగి;
    }

    ఒకవేళ (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->సంవర్గమానం, 0, "%*s in command line",
                      p - errstr, errstr);
        తిరిగి;
    }

    ngx_log_error(level, cf->సంవర్గమానం, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.దత్తాంశం, cf->conf_file->పంక్తి);
}


అక్షరం *
ngx_conf_సమితి_flag_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం  *p = conf;

    ngx_str_t        *విలువ;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->స్థానభ్రంశం);

    ఒకవేళ (*fp != NGX_CONF_UNSET) {
        తిరిగి "is duplicate";
    }

    విలువ = cf->వాదనలు->elts;

    ఒకవేళ (ngx_strcasecmp(విలువ[1].దత్తాంశం, (u_char *) "on") == 0) {
        *fp = 1;

    } లేకపోతే ఒకవేళ (ngx_strcasecmp(విలువ[1].దత్తాంశం, (u_char *) "off") == 0) {
        *fp = 0;

    } లేకపోతే {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     విలువ[1].దత్తాంశం, cmd->name.దత్తాంశం);
        తిరిగి NGX_CONF_ERROR;
    }

    ఒకవేళ (cmd->post) {
        post = cmd->post;
        తిరిగి post->post_handler(cf, post, fp);
    }

    తిరిగి NGX_CONF_OK;
}


అక్షరం *
ngx_conf_సమితి_str_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం  *p = conf;

    ngx_str_t        *క్షేత్రం, *విలువ;
    ngx_conf_post_t  *post;

    క్షేత్రం = (ngx_str_t *) (p + cmd->స్థానభ్రంశం);

    ఒకవేళ (క్షేత్రం->దత్తాంశం) {
        తిరిగి "is duplicate";
    }

    విలువ = cf->వాదనలు->elts;

    *క్షేత్రం = విలువ[1];

    ఒకవేళ (cmd->post) {
        post = cmd->post;
        తిరిగి post->post_handler(cf, post, క్షేత్రం);
    }

    తిరిగి NGX_CONF_OK;
}


అక్షరం *
ngx_conf_సమితి_str_array_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం  *p = conf;

    ngx_str_t         *విలువ, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->స్థానభ్రంశం);

    ఒకవేళ (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, పరిమాణం(ngx_str_t));
        ఒకవేళ (*a == ఖాళీ) {
            తిరిగి NGX_CONF_ERROR;
        }
    }

    s = ngx_array_నెట్టు(*a);
    ఒకవేళ (s == ఖాళీ) {
        తిరిగి NGX_CONF_ERROR;
    }

    విలువ = cf->వాదనలు->elts;

    *s = విలువ[1];

    ఒకవేళ (cmd->post) {
        post = cmd->post;
        తిరిగి post->post_handler(cf, post, s);
    }

    తిరిగి NGX_CONF_OK;
}


అక్షరం *
ngx_conf_సమితి_keyval_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం  *p = conf;

    ngx_str_t         *విలువ;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->స్థానభ్రంశం);

    ఒకవేళ (*a == NGX_CONF_UNSET_PTR || *a == ఖాళీ) {
        *a = ngx_array_create(cf->pool, 4, పరిమాణం(ngx_keyval_t));
        ఒకవేళ (*a == ఖాళీ) {
            తిరిగి NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_నెట్టు(*a);
    ఒకవేళ (kv == ఖాళీ) {
        తిరిగి NGX_CONF_ERROR;
    }

    విలువ = cf->వాదనలు->elts;

    kv->కీలకం = విలువ[1];
    kv->విలువ = విలువ[2];

    ఒకవేళ (cmd->post) {
        post = cmd->post;
        తిరిగి post->post_handler(cf, post, kv);
    }

    తిరిగి NGX_CONF_OK;
}


అక్షరం *
ngx_conf_సమితి_అంకె_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *విలువ;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->స్థానభ్రంశం);

    ఒకవేళ (*np != NGX_CONF_UNSET) {
        తిరిగి "is duplicate";
    }

    విలువ = cf->వాదనలు->elts;
    *np = ngx_atoi(విలువ[1].దత్తాంశం, విలువ[1].len);
    ఒకవేళ (*np == NGX_ERROR) {
        తిరిగి "invalid number";
    }

    ఒకవేళ (cmd->post) {
        post = cmd->post;
        తిరిగి post->post_handler(cf, post, np);
    }

    తిరిగి NGX_CONF_OK;
}


అక్షరం *
ngx_conf_సమితి_కొలత_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం  *p = conf;

    కొలత_t           *sp;
    ngx_str_t        *విలువ;
    ngx_conf_post_t  *post;


    sp = (కొలత_t *) (p + cmd->స్థానభ్రంశం);
    ఒకవేళ (*sp != NGX_CONF_UNSET_కొలత) {
        తిరిగి "is duplicate";
    }

    విలువ = cf->వాదనలు->elts;

    *sp = ngx_parse_కొలత(&విలువ[1]);
    ఒకవేళ (*sp == (కొలత_t) NGX_ERROR) {
        తిరిగి "invalid value";
    }

    ఒకవేళ (cmd->post) {
        post = cmd->post;
        తిరిగి post->post_handler(cf, post, sp);
    }

    తిరిగి NGX_CONF_OK;
}


అక్షరం *
ngx_conf_సమితి_off_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం  *p = conf;

    off_t            *op;
    ngx_str_t        *విలువ;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->స్థానభ్రంశం);
    ఒకవేళ (*op != NGX_CONF_UNSET) {
        తిరిగి "is duplicate";
    }

    విలువ = cf->వాదనలు->elts;

    *op = ngx_parse_స్థానభ్రంశం(&విలువ[1]);
    ఒకవేళ (*op == (off_t) NGX_ERROR) {
        తిరిగి "invalid value";
    }

    ఒకవేళ (cmd->post) {
        post = cmd->post;
        తిరిగి post->post_handler(cf, post, op);
    }

    తిరిగి NGX_CONF_OK;
}


అక్షరం *
ngx_conf_సమితి_msec_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *విలువ;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->స్థానభ్రంశం);
    ఒకవేళ (*msp != NGX_CONF_UNSET_MSEC) {
        తిరిగి "is duplicate";
    }

    విలువ = cf->వాదనలు->elts;

    *msp = ngx_parse_సమయం(&విలువ[1], 0);
    ఒకవేళ (*msp == (ngx_msec_t) NGX_ERROR) {
        తిరిగి "invalid value";
    }

    ఒకవేళ (cmd->post) {
        post = cmd->post;
        తిరిగి post->post_handler(cf, post, msp);
    }

    తిరిగి NGX_CONF_OK;
}


అక్షరం *
ngx_conf_సమితి_sec_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం  *p = conf;

    సమయం_t           *sp;
    ngx_str_t        *విలువ;
    ngx_conf_post_t  *post;


    sp = (సమయం_t *) (p + cmd->స్థానభ్రంశం);
    ఒకవేళ (*sp != NGX_CONF_UNSET) {
        తిరిగి "is duplicate";
    }

    విలువ = cf->వాదనలు->elts;

    *sp = ngx_parse_సమయం(&విలువ[1], 1);
    ఒకవేళ (*sp == (సమయం_t) NGX_ERROR) {
        తిరిగి "invalid value";
    }

    ఒకవేళ (cmd->post) {
        post = cmd->post;
        తిరిగి post->post_handler(cf, post, sp);
    }

    తిరిగి NGX_CONF_OK;
}


అక్షరం *
ngx_conf_సమితి_bufs_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం *p = conf;

    ngx_str_t   *విలువ;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->స్థానభ్రంశం);
    ఒకవేళ (bufs->అంకె) {
        తిరిగి "is duplicate";
    }

    విలువ = cf->వాదనలు->elts;

    bufs->అంకె = ngx_atoi(విలువ[1].దత్తాంశం, విలువ[1].len);
    ఒకవేళ (bufs->అంకె == NGX_ERROR || bufs->అంకె == 0) {
        తిరిగి "invalid value";
    }

    bufs->కొలత = ngx_parse_కొలత(&విలువ[2]);
    ఒకవేళ (bufs->కొలత == (కొలత_t) NGX_ERROR || bufs->కొలత == 0) {
        తిరిగి "invalid value";
    }

    తిరిగి NGX_CONF_OK;
}


అక్షరం *
ngx_conf_సమితి_enum_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *విలువ;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->స్థానభ్రంశం);

    ఒకవేళ (*np != NGX_CONF_UNSET_UINT) {
        తిరిగి "is duplicate";
    }

    విలువ = cf->వాదనలు->elts;
    e = cmd->post;

    కొరకు (i = 0; e[i].name.len != 0; i++) {
        ఒకవేళ (e[i].name.len != విలువ[1].len
            || ngx_strcasecmp(e[i].name.దత్తాంశం, విలువ[1].దత్తాంశం) != 0)
        {
            కొనసాగు;
        }

        *np = e[i].విలువ;

        తిరిగి NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", విలువ[1].దత్తాంశం);

    తిరిగి NGX_CONF_ERROR;
}


అక్షరం *
ngx_conf_సమితి_bitmask_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *విలువ;
    ngx_conf_bitmask_t  *ముసుగు;


    np = (ngx_uint_t *) (p + cmd->స్థానభ్రంశం);
    విలువ = cf->వాదనలు->elts;
    ముసుగు = cmd->post;

    కొరకు (i = 1; i < cf->వాదనలు->nelts; i++) {
        కొరకు (m = 0; ముసుగు[m].name.len != 0; m++) {

            ఒకవేళ (ముసుగు[m].name.len != విలువ[i].len
                || ngx_strcasecmp(ముసుగు[m].name.దత్తాంశం, విలువ[i].దత్తాంశం) != 0)
            {
                కొనసాగు;
            }

            ఒకవేళ (*np & ముసుగు[m].ముసుగు) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", విలువ[i].దత్తాంశం);

            } లేకపోతే {
                *np |= ముసుగు[m].ముసుగు;
            }

            ఆపు;
        }

        ఒకవేళ (ముసుగు[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", విలువ[i].దత్తాంశం);

            తిరిగి NGX_CONF_ERROR;
        }
    }

    తిరిగి NGX_CONF_OK;
}


#ఒకవేళ 0

అక్షరం *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    తిరిగి "unsupported on this platform";
}

#అయితే_ముగింపు


అక్షరం *
ngx_conf_deprecated(ngx_conf_t *cf, శూన్య *post, శూన్య *దత్తాంశం)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    తిరిగి NGX_CONF_OK;
}


అక్షరం *
ngx_conf_check_అంకె_bounds(ngx_conf_t *cf, శూన్య *post, శూన్య *దత్తాంశం)
{
    ngx_conf_అంకె_bounds_t  *bounds = post;
    ngx_int_t  *np = దత్తాంశం;

    ఒకవేళ (bounds->ఎగువ == -1) {
        ఒకవేళ (*np >= bounds->దిగువ) {
            తిరిగి NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->దిగువ);

        తిరిగి NGX_CONF_ERROR;
    }

    ఒకవేళ (*np >= bounds->దిగువ && *np <= bounds->ఎగువ) {
        తిరిగి NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->దిగువ, bounds->ఎగువ);

    తిరిగి NGX_CONF_ERROR;
}


అక్షరం *
ngx_conf_check_కొలత_nonzero(ngx_conf_t *cf, శూన్య *post, శూన్య *దత్తాంశం)
{
    కొలత_t  *sp = దత్తాంశం;

    ఒకవేళ (*sp == 0) {
        తిరిగి "value must be nonzero";
    }

    తిరిగి NGX_CONF_OK;
}
