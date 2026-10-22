
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#تعریف_کرو NGX_CONF_بفر  4096

ساكن ngx_int_t ngx_conf_جوڑو_dump(ngx_conf_t *cf, ngx_str_t *filename);
ساكن ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t آخری);
ساكن ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
ساكن فراغ ngx_conf_flush_files(ngx_چکر_t *چکر);


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

ساكن ngx_uint_t دلیل_نمبر[] = {
    NGX_CONF_NOARGS,
    NGX_CONF_TAKE1,
    NGX_CONF_TAKE2,
    NGX_CONF_TAKE3,
    NGX_CONF_TAKE4,
    NGX_CONF_TAKE5,
    NGX_CONF_TAKE6,
    NGX_CONF_TAKE7
};


ngx_conf_post_t  ngx_conf_ناپ_nonzero_post = { ngx_conf_check_ناپ_nonzero };


حرف *
ngx_conf_param(ngx_conf_t *cf)
{
    حرف             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->چکر->conf_param;

    إذا (param->len == 0) {
        أرجع NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, حجم(ngx_conf_file_t));

    ngx_memzero(&b, حجم(ngx_buf_t));

    b.شروع = param->انکڑے;
    b.pos = param->انکڑے;
    b.آخری = param->انکڑے + param->len;
    b.سرا = b.آخری;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.انکڑے = عدم;
    conf_file.سطر = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->بفر = &b;

    rv = ngx_conf_parse(cf, عدم);

    cf->conf_file = عدم;

    أرجع rv;
}


ساكن ngx_int_t
ngx_conf_جوڑو_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             ناپ;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_گرہ_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_چکری_جانچ_32_long(filename->انکڑے, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->چکر->config_dump_rbtree, filename, hash);

    إذا (sn) {
        cf->conf_file->dump = عدم;
        أرجع NGX_OK;
    }

    p = ngx_pstrdup(cf->چکر->pool, filename);
    إذا (p == عدم) {
        أرجع NGX_ERROR;
    }

    cd = ngx_array_ڈالو(&cf->چکر->config_dump);
    إذا (cd == عدم) {
        أرجع NGX_ERROR;
    }

    ناپ = ngx_file_ناپ(&cf->conf_file->file.info);

    buf = ngx_create_عارضی_buf(cf->چکر->pool, (ناپ_t) ناپ);
    إذا (buf == عدم) {
        أرجع NGX_ERROR;
    }

    cd->name.انکڑے = p;
    cd->name.len = filename->len;
    cd->بفر = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->عارضی_pool, حجم(ngx_str_گرہ_t));
    إذا (sn == عدم) {
        أرجع NGX_ERROR;
    }

    sn->گرہ.کلید = hash;
    sn->str = cd->name;

    ngx_rbtree_داخل_کرو(&cf->چکر->config_dump_rbtree, &sn->گرہ);

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
        parse_بلاک,
        parse_param
    } type;

#إذا (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = عدم;
#جے_انت

    إذا (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->انکڑے, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        إذا (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->انکڑے);
            أرجع NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        إذا (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->لوگارتھم, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->انکڑے);
        }

        cf->conf_file->بفر = &buf;

        buf.شروع = ngx_alloc(NGX_CONF_بفر, cf->لوگارتھم);
        إذا (buf.شروع == عدم) {
            اذهب failed;
        }

        buf.pos = buf.شروع;
        buf.آخری = buf.شروع;
        buf.سرا = buf.آخری + NGX_CONF_بفر;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.انکڑے = filename->انکڑے;
        cf->conf_file->file.وتھ = 0;
        cf->conf_file->file.لوگارتھم = cf->لوگارتھم;
        cf->conf_file->سطر = 1;

        type = parse_file;

        إذا (ngx_dump_config
#إذا (NGX_CONTROL_API)
            || ngx_control_api_enabled
#جے_انت
#إذا (NGX_DEBUG)
            || 1
#جے_انت
           )
        {
            إذا (ngx_conf_جوڑو_dump(cf, filename) != NGX_OK) {
                اذهب failed;
            }

        } وإلا {
            cf->conf_file->dump = عدم;
        }

    } وإلا إذا (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_بلاک;

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

        إذا (rc == NGX_ERROR) {
            اذهب پورا;
        }

        إذا (rc == NGX_CONF_بلاک_پورا) {

            إذا (type != parse_بلاک) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                اذهب failed;
            }

            اذهب پورا;
        }

        إذا (rc == NGX_CONF_FILE_پورا) {

            إذا (type == parse_بلاک) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                اذهب failed;
            }

            اذهب پورا;
        }

        إذا (rc == NGX_CONF_بلاک_شروع) {

            إذا (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                اذهب failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        إذا (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            إذا (rc == NGX_CONF_بلاک_شروع) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                اذهب failed;
            }

            rv = (*cf->handler)(cf, عدم, cf->handler_conf);
            إذا (rv == NGX_CONF_OK) {
                استمر;
            }

            إذا (rv == NGX_CONF_ERROR) {
                اذهب failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            اذهب failed;
        }


        rc = ngx_conf_handler(cf, rc);

        إذا (rc == NGX_ERROR) {
            اذهب failed;
        }
    }

failed:

    rc = NGX_ERROR;

پورا:

    إذا (filename) {
        إذا (cf->conf_file->بفر->شروع) {
            ngx_free(cf->conf_file->بفر->شروع);
        }

        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->انکڑے);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    إذا (rc == NGX_ERROR) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


ساكن ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t آخری)
{
    حرف           *rv;
    فراغ           *conf, **confp;
    ngx_uint_t      i, لبھیا;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->دلیلاں->elts;

    لبھیا = 0;

    لكل (i = 0; cf->چکر->modules[i]; i++) {

        cmd = cf->چکر->modules[i]->commands;
        إذا (cmd == عدم) {
            استمر;
        }

        لكل ( /* void */ ; cmd->name.len; cmd++) {

            إذا (name->len != cmd->name.len) {
                استمر;
            }

            إذا (ngx_strcmp(name->انکڑے, cmd->name.انکڑے) != 0) {
                استمر;
            }

            لبھیا = 1;

            إذا (cf->چکر->modules[i]->type != NGX_CONF_MODULE
                && cf->چکر->modules[i]->type != cf->module_type)
            {
                استمر;
            }

            /* is the directive's location right ? */

            إذا (!(cmd->type & cf->cmd_type)) {
                استمر;
            }

            إذا (!(cmd->type & NGX_CONF_بلاک) && آخری != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->انکڑے);
                أرجع NGX_ERROR;
            }

            إذا ((cmd->type & NGX_CONF_بلاک) && آخری != NGX_CONF_بلاک_شروع) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->انکڑے);
                أرجع NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            إذا (!(cmd->type & NGX_CONF_ANY)) {

                إذا (cmd->type & NGX_CONF_FLAG) {

                    إذا (cf->دلیلاں->nelts != 2) {
                        اذهب invalid;
                    }

                } وإلا إذا (cmd->type & NGX_CONF_1_ہور) {

                    إذا (cf->دلیلاں->nelts < 2) {
                        اذهب invalid;
                    }

                } وإلا إذا (cmd->type & NGX_CONF_2_ہور) {

                    إذا (cf->دلیلاں->nelts < 3) {
                        اذهب invalid;
                    }

                } وإلا إذا (cf->دلیلاں->nelts > NGX_CONF_MAX_دلیلاں) {

                    اذهب invalid;

                } وإلا إذا (!(cmd->type & دلیل_نمبر[cf->دلیلاں->nelts - 1]))
                {
                    اذهب invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = عدم;

            إذا (cmd->type & NGX_سدھا_CONF) {
                conf = ((فراغ **) cf->ctx)[cf->چکر->modules[i]->اشاریہ];

            } وإلا إذا (cmd->type & NGX_MAIN_CONF) {
                conf = &(((فراغ **) cf->ctx)[cf->چکر->modules[i]->اشاریہ]);

            } وإلا إذا (cf->ctx) {
                confp = *(فراغ **) ((حرف *) cf->ctx + cmd->conf);

                إذا (confp) {
                    conf = confp[cf->چکر->modules[i]->ctx_اشاریہ];
                }
            }

            rv = cmd->سیٹ(cf, cmd, conf);

            إذا (rv == NGX_CONF_OK) {
                أرجع NGX_OK;
            }

            إذا (rv == NGX_CONF_ERROR) {
                أرجع NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->انکڑے, rv);

            أرجع NGX_ERROR;
        }
    }

    إذا (لبھیا) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->انکڑے);

        أرجع NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->انکڑے);

    أرجع NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->انکڑے);

    أرجع NGX_ERROR;
}


ساكن ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *شروع, ch, *src, *dst;
    off_t        file_ناپ;
    ناپ_t       len;
    ssize_t      n, ناپ;
    ngx_uint_t   لبھیا, need_space, آخری_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, شروع_سطر;
    ngx_str_t   *لفظ;
    ngx_buf_t   *b, *dump;

    لبھیا = 0;
    need_space = 0;
    آخری_space = 1;
    sharp_comment = 0;
    variable = 0;
    quoted = 0;
    s_quoted = 0;
    d_quoted = 0;

    cf->دلیلاں->nelts = 0;
    b = cf->conf_file->بفر;
    dump = cf->conf_file->dump;
    شروع = b->pos;
    شروع_سطر = cf->conf_file->سطر;

    file_ناپ = ngx_file_ناپ(&cf->conf_file->file.info);

    لكل ( ;; ) {

        إذا (b->pos >= b->آخری) {

            إذا (cf->conf_file->file.وتھ >= file_ناپ) {

                إذا (cf->دلیلاں->nelts > 0 || !آخری_space) {

                    إذا (cf->conf_file->file.fd == NGX_INVALID_FILE) {
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

                أرجع NGX_CONF_FILE_پورا;
            }

            len = b->pos - شروع;

            إذا (len == NGX_CONF_بفر) {
                cf->conf_file->سطر = شروع_سطر;

                إذا (d_quoted) {
                    ch = '"';

                } وإلا إذا (s_quoted) {
                    ch = '\'';

                } وإلا {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (ناپ_t) 10, شروع);
                    أرجع NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                أرجع NGX_ERROR;
            }

            إذا (len) {
                ngx_memmove(b->شروع, شروع, len);
            }

            ناپ = (ssize_t) (file_ناپ - cf->conf_file->file.وتھ);

            إذا (ناپ > b->سرا - (b->شروع + len)) {
                ناپ = b->سرا - (b->شروع + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->شروع + len, ناپ,
                              cf->conf_file->file.وتھ);

            إذا (n == NGX_ERROR) {
                أرجع NGX_ERROR;
            }

            إذا (n != ناپ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, ناپ);
                أرجع NGX_ERROR;
            }

            b->pos = b->شروع + len;
            b->آخری = b->pos + n;
            شروع = b->شروع;

            إذا (dump) {
                dump->آخری = ngx_cpymem(dump->آخری, b->pos, ناپ);
            }
        }

        ch = *b->pos++;

        إذا (ch == LF) {
            cf->conf_file->سطر++;

            إذا (sharp_comment) {
                sharp_comment = 0;
            }
        }

        إذا (sharp_comment) {
            استمر;
        }

        إذا (quoted) {
            quoted = 0;
            استمر;
        }

        إذا (need_space) {
            إذا (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                آخری_space = 1;
                need_space = 0;
                استمر;
            }

            إذا (ch == ';') {
                أرجع NGX_OK;
            }

            إذا (ch == '{') {
                أرجع NGX_CONF_بلاک_شروع;
            }

            إذا (ch == ')') {
                آخری_space = 1;
                need_space = 0;

            } وإلا {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                أرجع NGX_ERROR;
            }
        }

        إذا (آخری_space) {

            شروع = b->pos - 1;
            شروع_سطر = cf->conf_file->سطر;

            إذا (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                استمر;
            }

            بدل (ch) {

            حالة ';':
            حالة '{':
                إذا (cf->دلیلاں->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    أرجع NGX_ERROR;
                }

                إذا (ch == '{') {
                    أرجع NGX_CONF_بلاک_شروع;
                }

                أرجع NGX_OK;

            حالة '}':
                إذا (cf->دلیلاں->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    أرجع NGX_ERROR;
                }

                أرجع NGX_CONF_بلاک_پورا;

            حالة '#':
                sharp_comment = 1;
                استمر;

            حالة '\\':
                quoted = 1;
                آخری_space = 0;
                استمر;

            حالة '"':
                شروع++;
                d_quoted = 1;
                آخری_space = 0;
                استمر;

            حالة '\'':
                شروع++;
                s_quoted = 1;
                آخری_space = 0;
                استمر;

            حالة '$':
                variable = 1;
                آخری_space = 0;
                استمر;

            افتراضي:
                آخری_space = 0;
            }

        } وإلا {
            إذا (ch == '{' && variable) {
                استمر;
            }

            variable = 0;

            إذا (ch == '\\') {
                quoted = 1;
                استمر;
            }

            إذا (ch == '$') {
                variable = 1;
                استمر;
            }

            إذا (d_quoted) {
                إذا (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    لبھیا = 1;
                }

            } وإلا إذا (s_quoted) {
                إذا (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    لبھیا = 1;
                }

            } وإلا إذا (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                آخری_space = 1;
                لبھیا = 1;
            }

            إذا (لبھیا) {
                لفظ = ngx_array_ڈالو(cf->دلیلاں);
                إذا (لفظ == عدم) {
                    أرجع NGX_ERROR;
                }

                لفظ->انکڑے = ngx_pnalloc(cf->pool, b->pos - 1 - شروع + 1);
                إذا (لفظ->انکڑے == عدم) {
                    أرجع NGX_ERROR;
                }

                لكل (dst = لفظ->انکڑے, src = شروع, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    إذا (*src == '\\') {
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
                لفظ->len = len;

                إذا (ch == ';') {
                    أرجع NGX_OK;
                }

                إذا (ch == '{') {
                    أرجع NGX_CONF_بلاک_شروع;
                }

                لبھیا = 0;
            }
        }
    }
}


حرف *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف        *rv;
    ngx_int_t    n;
    ngx_str_t   *قدر, file, name;
    ngx_glob_t   gl;

    قدر = cf->دلیلاں->elts;
    file = قدر[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لوگارتھم, 0, "include %s", file.انکڑے);

    إذا (ngx_conf_بھریا_name(cf->چکر, &file, 1) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    إذا (strpbrk((حرف *) file.انکڑے, "*?[") == عدم) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لوگارتھم, 0, "include %s", file.انکڑے);

        أرجع ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, حجم(ngx_glob_t));

    gl.نمونہ = file.انکڑے;
    gl.لوگارتھم = cf->لوگارتھم;
    gl.test = 1;

    إذا (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.انکڑے);
        أرجع NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    لكل ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        إذا (n != NGX_OK) {
            اكسر;
        }

        file.len = name.len++;
        file.انکڑے = ngx_pstrdup(cf->pool, &name);
        إذا (file.انکڑے == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لوگارتھم, 0, "include %s", file.انکڑے);

        rv = ngx_conf_parse(cf, &file);

        إذا (rv != NGX_CONF_OK) {
            اكسر;
        }
    }

    ngx_close_glob(&gl);

    أرجع rv;
}


ngx_int_t
ngx_conf_بھریا_name(ngx_چکر_t *چکر, ngx_str_t *name, ngx_uint_t conf_اگیتر)
{
    ngx_str_t  *اگیتر;

    اگیتر = conf_اگیتر ? &چکر->conf_اگیتر : &چکر->اگیتر;

    أرجع ngx_get_بھریا_name(چکر->pool, اگیتر, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_چکر_t *چکر, ngx_str_t *name)
{
    ngx_str_t         بھریا;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#إذا (NGX_SUPPRESS_WARN)
    ngx_str_null(&بھریا);
#جے_انت

    إذا (name->len) {
        بھریا = *name;

        إذا (ngx_conf_بھریا_name(چکر, &بھریا, 0) != NGX_OK) {
            أرجع عدم;
        }

        part = &چکر->open_files.part;
        file = part->elts;

        لكل (i = 0; /* void */ ; i++) {

            إذا (i >= part->nelts) {
                إذا (part->اگلا == عدم) {
                    اكسر;
                }
                part = part->اگلا;
                file = part->elts;
                i = 0;
            }

            إذا (بھریا.len != file[i].name.len) {
                استمر;
            }

            إذا (ngx_strcmp(بھریا.انکڑے, file[i].name.انکڑے) == 0) {
                أرجع &file[i];
            }
        }
    }

    file = ngx_list_ڈالو(&چکر->open_files);
    إذا (file == عدم) {
        أرجع عدم;
    }

    إذا (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = بھریا;

    } وإلا {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = عدم;
    file->انکڑے = عدم;

    أرجع file;
}


ساكن فراغ
ngx_conf_flush_files(ngx_چکر_t *چکر)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, چکر->لوگارتھم, 0, "flush files");

    part = &چکر->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->اگلا == عدم) {
                اكسر;
            }
            part = part->اگلا;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].flush) {
            file[i].flush(&file[i], چکر->لوگارتھم);
        }
    }
}


فراغ ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *آخری;
    va_list  دلیلاں;

    آخری = errstr + NGX_MAX_CONF_ERRSTR;

    va_شروع(دلیلاں, fmt);
    p = ngx_vslprintf(errstr, آخری, fmt, دلیلاں);
    va_سرا(دلیلاں);

    إذا (err) {
        p = ngx_log_errno(p, آخری, err);
    }

    إذا (cf->conf_file == عدم) {
        ngx_log_error(level, cf->لوگارتھم, 0, "%*s", p - errstr, errstr);
        أرجع;
    }

    إذا (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->لوگارتھم, 0, "%*s in command line",
                      p - errstr, errstr);
        أرجع;
    }

    ngx_log_error(level, cf->لوگارتھم, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.انکڑے, cf->conf_file->سطر);
}


حرف *
ngx_conf_سیٹ_flag_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t        *قدر;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->وتھ);

    إذا (*fp != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    قدر = cf->دلیلاں->elts;

    إذا (ngx_strcasecmp(قدر[1].انکڑے, (u_char *) "on") == 0) {
        *fp = 1;

    } وإلا إذا (ngx_strcasecmp(قدر[1].انکڑے, (u_char *) "off") == 0) {
        *fp = 0;

    } وإلا {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     قدر[1].انکڑے, cmd->name.انکڑے);
        أرجع NGX_CONF_ERROR;
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, fp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_str_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t        *میدان, *قدر;
    ngx_conf_post_t  *post;

    میدان = (ngx_str_t *) (p + cmd->وتھ);

    إذا (میدان->انکڑے) {
        أرجع "is duplicate";
    }

    قدر = cf->دلیلاں->elts;

    *میدان = قدر[1];

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, میدان);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_str_array_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t         *قدر, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->وتھ);

    إذا (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_str_t));
        إذا (*a == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    s = ngx_array_ڈالو(*a);
    إذا (s == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    قدر = cf->دلیلاں->elts;

    *s = قدر[1];

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, s);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_keyval_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t         *قدر;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->وتھ);

    إذا (*a == NGX_CONF_UNSET_PTR || *a == عدم) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_keyval_t));
        إذا (*a == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_ڈالو(*a);
    إذا (kv == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    قدر = cf->دلیلاں->elts;

    kv->کلید = قدر[1];
    kv->قدر = قدر[2];

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, kv);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_عدد_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->وتھ);

    إذا (*np != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    قدر = cf->دلیلاں->elts;
    *np = ngx_atoi(قدر[1].انکڑے, قدر[1].len);
    إذا (*np == NGX_ERROR) {
        أرجع "invalid number";
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, np);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_ناپ_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ناپ_t           *sp;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    sp = (ناپ_t *) (p + cmd->وتھ);
    إذا (*sp != NGX_CONF_UNSET_ناپ) {
        أرجع "is duplicate";
    }

    قدر = cf->دلیلاں->elts;

    *sp = ngx_parse_ناپ(&قدر[1]);
    إذا (*sp == (ناپ_t) NGX_ERROR) {
        أرجع "invalid value";
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, sp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_off_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    off_t            *op;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->وتھ);
    إذا (*op != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    قدر = cf->دلیلاں->elts;

    *op = ngx_parse_وتھ(&قدر[1]);
    إذا (*op == (off_t) NGX_ERROR) {
        أرجع "invalid value";
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, op);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_msec_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->وتھ);
    إذا (*msp != NGX_CONF_UNSET_MSEC) {
        أرجع "is duplicate";
    }

    قدر = cf->دلیلاں->elts;

    *msp = ngx_parse_ویلا(&قدر[1], 0);
    إذا (*msp == (ngx_msec_t) NGX_ERROR) {
        أرجع "invalid value";
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, msp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_sec_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ویلا_t           *sp;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    sp = (ویلا_t *) (p + cmd->وتھ);
    إذا (*sp != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    قدر = cf->دلیلاں->elts;

    *sp = ngx_parse_ویلا(&قدر[1], 1);
    إذا (*sp == (ویلا_t) NGX_ERROR) {
        أرجع "invalid value";
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, sp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_bufs_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف *p = conf;

    ngx_str_t   *قدر;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->وتھ);
    إذا (bufs->عدد) {
        أرجع "is duplicate";
    }

    قدر = cf->دلیلاں->elts;

    bufs->عدد = ngx_atoi(قدر[1].انکڑے, قدر[1].len);
    إذا (bufs->عدد == NGX_ERROR || bufs->عدد == 0) {
        أرجع "invalid value";
    }

    bufs->ناپ = ngx_parse_ناپ(&قدر[2]);
    إذا (bufs->ناپ == (ناپ_t) NGX_ERROR || bufs->ناپ == 0) {
        أرجع "invalid value";
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_enum_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *قدر;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->وتھ);

    إذا (*np != NGX_CONF_UNSET_UINT) {
        أرجع "is duplicate";
    }

    قدر = cf->دلیلاں->elts;
    e = cmd->post;

    لكل (i = 0; e[i].name.len != 0; i++) {
        إذا (e[i].name.len != قدر[1].len
            || ngx_strcasecmp(e[i].name.انکڑے, قدر[1].انکڑے) != 0)
        {
            استمر;
        }

        *np = e[i].قدر;

        أرجع NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", قدر[1].انکڑے);

    أرجع NGX_CONF_ERROR;
}


حرف *
ngx_conf_سیٹ_bitmask_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *قدر;
    ngx_conf_bitmask_t  *نقاب;


    np = (ngx_uint_t *) (p + cmd->وتھ);
    قدر = cf->دلیلاں->elts;
    نقاب = cmd->post;

    لكل (i = 1; i < cf->دلیلاں->nelts; i++) {
        لكل (m = 0; نقاب[m].name.len != 0; m++) {

            إذا (نقاب[m].name.len != قدر[i].len
                || ngx_strcasecmp(نقاب[m].name.انکڑے, قدر[i].انکڑے) != 0)
            {
                استمر;
            }

            إذا (*np & نقاب[m].نقاب) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", قدر[i].انکڑے);

            } وإلا {
                *np |= نقاب[m].نقاب;
            }

            اكسر;
        }

        إذا (نقاب[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", قدر[i].انکڑے);

            أرجع NGX_CONF_ERROR;
        }
    }

    أرجع NGX_CONF_OK;
}


#إذا 0

حرف *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    أرجع "unsupported on this platform";
}

#جے_انت


حرف *
ngx_conf_deprecated(ngx_conf_t *cf, فراغ *post, فراغ *انکڑے)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_check_عدد_bounds(ngx_conf_t *cf, فراغ *post, فراغ *انکڑے)
{
    ngx_conf_عدد_bounds_t  *bounds = post;
    ngx_int_t  *np = انکڑے;

    إذا (bounds->بلند == -1) {
        إذا (*np >= bounds->کم) {
            أرجع NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->کم);

        أرجع NGX_CONF_ERROR;
    }

    إذا (*np >= bounds->کم && *np <= bounds->بلند) {
        أرجع NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->کم, bounds->بلند);

    أرجع NGX_CONF_ERROR;
}


حرف *
ngx_conf_check_ناپ_nonzero(ngx_conf_t *cf, فراغ *post, فراغ *انکڑے)
{
    ناپ_t  *sp = انکڑے;

    إذا (*sp == 0) {
        أرجع "value must be nonzero";
    }

    أرجع NGX_CONF_OK;
}
