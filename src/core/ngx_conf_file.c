
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>

#تعریف_کرو NGX_CONF_بفر  4096

ساکن ngx_int_t ngx_conf_جوڑو_dump(ngx_conf_t *cf, ngx_str_t *filename);
ساکن ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t آخری);
ساکن ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
ساکن خالی ngx_conf_flush_files(ngx_چکر_t *چکر);


ساکن ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      تہی },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    تہی,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    تہی,                                  /* init master */
    تہی,                                  /* init module */
    تہی,                                  /* init process */
    تہی,                                  /* init thread */
    تہی,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    تہی,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

ساکن ngx_uint_t دلیل_نمبر[] = {
    NGX_CONF_NOARGS,
    NGX_CONF_TAKE1,
    NGX_CONF_TAKE2,
    NGX_CONF_TAKE3,
    NGX_CONF_TAKE4,
    NGX_CONF_TAKE5,
    NGX_CONF_TAKE6,
    NGX_CONF_TAKE7
};


ngx_conf_post_t  ngx_conf_سائز_nonzero_post = { ngx_conf_check_سائز_nonzero };


حرف *
ngx_conf_param(ngx_conf_t *cf)
{
    حرف             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->چکر->conf_param;

    اگر (param->len == 0) {
        واپس NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, حجم(ngx_conf_file_t));

    ngx_memzero(&b, حجم(ngx_buf_t));

    b.آغاز = param->ڈیٹا;
    b.pos = param->ڈیٹا;
    b.آخری = param->ڈیٹا + param->len;
    b.انتہا = b.آخری;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.ڈیٹا = تہی;
    conf_file.سطر = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->بفر = &b;

    rv = ngx_conf_parse(cf, تہی);

    cf->conf_file = تہی;

    واپس rv;
}


ساکن ngx_int_t
ngx_conf_جوڑو_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             سائز;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_گرہ_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_چکری_جانچ_32_long(filename->ڈیٹا, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->چکر->config_dump_rbtree, filename, hash);

    اگر (sn) {
        cf->conf_file->dump = تہی;
        واپس NGX_OK;
    }

    p = ngx_pstrdup(cf->چکر->pool, filename);
    اگر (p == تہی) {
        واپس NGX_ERROR;
    }

    cd = ngx_array_ڈالو(&cf->چکر->config_dump);
    اگر (cd == تہی) {
        واپس NGX_ERROR;
    }

    سائز = ngx_file_سائز(&cf->conf_file->file.info);

    buf = ngx_create_عارضی_buf(cf->چکر->pool, (سائز_t) سائز);
    اگر (buf == تہی) {
        واپس NGX_ERROR;
    }

    cd->name.ڈیٹا = p;
    cd->name.len = filename->len;
    cd->بفر = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->عارضی_pool, حجم(ngx_str_گرہ_t));
    اگر (sn == تہی) {
        واپس NGX_ERROR;
    }

    sn->گرہ.کلید = hash;
    sn->str = cd->name;

    ngx_rbtree_داخل_کرو(&cf->چکر->config_dump_rbtree, &sn->گرہ);

    واپس NGX_OK;
}


حرف *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    حرف             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *پچھلا, conf_file;
    شمار {
        parse_file = 0,
        parse_بلاک,
        parse_param
    } type;

#اگر (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    پچھلا = تہی;
#اگر_ختم

    اگر (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->ڈیٹا, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        اگر (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->ڈیٹا);
            واپس NGX_CONF_ERROR;
        }

        پچھلا = cf->conf_file;

        cf->conf_file = &conf_file;

        اگر (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->لوگارتھم, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->ڈیٹا);
        }

        cf->conf_file->بفر = &buf;

        buf.آغاز = ngx_alloc(NGX_CONF_بفر, cf->لوگارتھم);
        اگر (buf.آغاز == تہی) {
            جاؤ failed;
        }

        buf.pos = buf.آغاز;
        buf.آخری = buf.آغاز;
        buf.انتہا = buf.آخری + NGX_CONF_بفر;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.ڈیٹا = filename->ڈیٹا;
        cf->conf_file->file.انحراف = 0;
        cf->conf_file->file.لوگارتھم = cf->لوگارتھم;
        cf->conf_file->سطر = 1;

        type = parse_file;

        اگر (ngx_dump_config
#اگر (NGX_CONTROL_API)
            || ngx_control_api_enabled
#اگر_ختم
#اگر (NGX_DEBUG)
            || 1
#اگر_ختم
           )
        {
            اگر (ngx_conf_جوڑو_dump(cf, filename) != NGX_OK) {
                جاؤ failed;
            }

        } ورنہ {
            cf->conf_file->dump = تہی;
        }

    } ورنہ اگر (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_بلاک;

    } ورنہ {
        type = parse_param;
    }


    برائے ( ;; ) {
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
            جاؤ مکمل;
        }

        اگر (rc == NGX_CONF_بلاک_مکمل) {

            اگر (type != parse_بلاک) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                جاؤ failed;
            }

            جاؤ مکمل;
        }

        اگر (rc == NGX_CONF_FILE_مکمل) {

            اگر (type == parse_بلاک) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                جاؤ failed;
            }

            جاؤ مکمل;
        }

        اگر (rc == NGX_CONF_بلاک_آغاز) {

            اگر (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                جاؤ failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        اگر (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            اگر (rc == NGX_CONF_بلاک_آغاز) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                جاؤ failed;
            }

            rv = (*cf->handler)(cf, تہی, cf->handler_conf);
            اگر (rv == NGX_CONF_OK) {
                جاری;
            }

            اگر (rv == NGX_CONF_ERROR) {
                جاؤ failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            جاؤ failed;
        }


        rc = ngx_conf_handler(cf, rc);

        اگر (rc == NGX_ERROR) {
            جاؤ failed;
        }
    }

failed:

    rc = NGX_ERROR;

مکمل:

    اگر (filename) {
        اگر (cf->conf_file->بفر->آغاز) {
            ngx_free(cf->conf_file->بفر->آغاز);
        }

        اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->ڈیٹا);
            rc = NGX_ERROR;
        }

        cf->conf_file = پچھلا;
    }

    اگر (rc == NGX_ERROR) {
        واپس NGX_CONF_ERROR;
    }

    واپس NGX_CONF_OK;
}


ساکن ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t آخری)
{
    حرف           *rv;
    خالی           *conf, **confp;
    ngx_uint_t      i, ملا;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->دلائل->elts;

    ملا = 0;

    برائے (i = 0; cf->چکر->modules[i]; i++) {

        cmd = cf->چکر->modules[i]->commands;
        اگر (cmd == تہی) {
            جاری;
        }

        برائے ( /* void */ ; cmd->name.len; cmd++) {

            اگر (name->len != cmd->name.len) {
                جاری;
            }

            اگر (ngx_strcmp(name->ڈیٹا, cmd->name.ڈیٹا) != 0) {
                جاری;
            }

            ملا = 1;

            اگر (cf->چکر->modules[i]->type != NGX_CONF_MODULE
                && cf->چکر->modules[i]->type != cf->module_type)
            {
                جاری;
            }

            /* is the directive's location right ? */

            اگر (!(cmd->type & cf->cmd_type)) {
                جاری;
            }

            اگر (!(cmd->type & NGX_CONF_بلاک) && آخری != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->ڈیٹا);
                واپس NGX_ERROR;
            }

            اگر ((cmd->type & NGX_CONF_بلاک) && آخری != NGX_CONF_بلاک_آغاز) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->ڈیٹا);
                واپس NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            اگر (!(cmd->type & NGX_CONF_ANY)) {

                اگر (cmd->type & NGX_CONF_FLAG) {

                    اگر (cf->دلائل->nelts != 2) {
                        جاؤ invalid;
                    }

                } ورنہ اگر (cmd->type & NGX_CONF_1_مزید) {

                    اگر (cf->دلائل->nelts < 2) {
                        جاؤ invalid;
                    }

                } ورنہ اگر (cmd->type & NGX_CONF_2_مزید) {

                    اگر (cf->دلائل->nelts < 3) {
                        جاؤ invalid;
                    }

                } ورنہ اگر (cf->دلائل->nelts > NGX_CONF_MAX_دلائل) {

                    جاؤ invalid;

                } ورنہ اگر (!(cmd->type & دلیل_نمبر[cf->دلائل->nelts - 1]))
                {
                    جاؤ invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = تہی;

            اگر (cmd->type & NGX_براہ_راست_CONF) {
                conf = ((خالی **) cf->ctx)[cf->چکر->modules[i]->اشاریہ];

            } ورنہ اگر (cmd->type & NGX_MAIN_CONF) {
                conf = &(((خالی **) cf->ctx)[cf->چکر->modules[i]->اشاریہ]);

            } ورنہ اگر (cf->ctx) {
                confp = *(خالی **) ((حرف *) cf->ctx + cmd->conf);

                اگر (confp) {
                    conf = confp[cf->چکر->modules[i]->ctx_اشاریہ];
                }
            }

            rv = cmd->سیٹ(cf, cmd, conf);

            اگر (rv == NGX_CONF_OK) {
                واپس NGX_OK;
            }

            اگر (rv == NGX_CONF_ERROR) {
                واپس NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->ڈیٹا, rv);

            واپس NGX_ERROR;
        }
    }

    اگر (ملا) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->ڈیٹا);

        واپس NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->ڈیٹا);

    واپس NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->ڈیٹا);

    واپس NGX_ERROR;
}


ساکن ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *آغاز, ch, *src, *dst;
    off_t        file_سائز;
    سائز_t       len;
    ssize_t      n, سائز;
    ngx_uint_t   ملا, need_space, آخری_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, آغاز_سطر;
    ngx_str_t   *لفظ;
    ngx_buf_t   *b, *dump;

    ملا = 0;
    need_space = 0;
    آخری_space = 1;
    sharp_comment = 0;
    variable = 0;
    quoted = 0;
    s_quoted = 0;
    d_quoted = 0;

    cf->دلائل->nelts = 0;
    b = cf->conf_file->بفر;
    dump = cf->conf_file->dump;
    آغاز = b->pos;
    آغاز_سطر = cf->conf_file->سطر;

    file_سائز = ngx_file_سائز(&cf->conf_file->file.info);

    برائے ( ;; ) {

        اگر (b->pos >= b->آخری) {

            اگر (cf->conf_file->file.انحراف >= file_سائز) {

                اگر (cf->دلائل->nelts > 0 || !آخری_space) {

                    اگر (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        واپس NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    واپس NGX_ERROR;
                }

                واپس NGX_CONF_FILE_مکمل;
            }

            len = b->pos - آغاز;

            اگر (len == NGX_CONF_بفر) {
                cf->conf_file->سطر = آغاز_سطر;

                اگر (d_quoted) {
                    ch = '"';

                } ورنہ اگر (s_quoted) {
                    ch = '\'';

                } ورنہ {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (سائز_t) 10, آغاز);
                    واپس NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                واپس NGX_ERROR;
            }

            اگر (len) {
                ngx_memmove(b->آغاز, آغاز, len);
            }

            سائز = (ssize_t) (file_سائز - cf->conf_file->file.انحراف);

            اگر (سائز > b->انتہا - (b->آغاز + len)) {
                سائز = b->انتہا - (b->آغاز + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->آغاز + len, سائز,
                              cf->conf_file->file.انحراف);

            اگر (n == NGX_ERROR) {
                واپس NGX_ERROR;
            }

            اگر (n != سائز) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, سائز);
                واپس NGX_ERROR;
            }

            b->pos = b->آغاز + len;
            b->آخری = b->pos + n;
            آغاز = b->آغاز;

            اگر (dump) {
                dump->آخری = ngx_cpymem(dump->آخری, b->pos, سائز);
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
            جاری;
        }

        اگر (quoted) {
            quoted = 0;
            جاری;
        }

        اگر (need_space) {
            اگر (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                آخری_space = 1;
                need_space = 0;
                جاری;
            }

            اگر (ch == ';') {
                واپس NGX_OK;
            }

            اگر (ch == '{') {
                واپس NGX_CONF_بلاک_آغاز;
            }

            اگر (ch == ')') {
                آخری_space = 1;
                need_space = 0;

            } ورنہ {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                واپس NGX_ERROR;
            }
        }

        اگر (آخری_space) {

            آغاز = b->pos - 1;
            آغاز_سطر = cf->conf_file->سطر;

            اگر (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                جاری;
            }

            انتخاب (ch) {

            حالت ';':
            حالت '{':
                اگر (cf->دلائل->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    واپس NGX_ERROR;
                }

                اگر (ch == '{') {
                    واپس NGX_CONF_بلاک_آغاز;
                }

                واپس NGX_OK;

            حالت '}':
                اگر (cf->دلائل->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    واپس NGX_ERROR;
                }

                واپس NGX_CONF_بلاک_مکمل;

            حالت '#':
                sharp_comment = 1;
                جاری;

            حالت '\\':
                quoted = 1;
                آخری_space = 0;
                جاری;

            حالت '"':
                آغاز++;
                d_quoted = 1;
                آخری_space = 0;
                جاری;

            حالت '\'':
                آغاز++;
                s_quoted = 1;
                آخری_space = 0;
                جاری;

            حالت '$':
                variable = 1;
                آخری_space = 0;
                جاری;

            طےشدہ:
                آخری_space = 0;
            }

        } ورنہ {
            اگر (ch == '{' && variable) {
                جاری;
            }

            variable = 0;

            اگر (ch == '\\') {
                quoted = 1;
                جاری;
            }

            اگر (ch == '$') {
                variable = 1;
                جاری;
            }

            اگر (d_quoted) {
                اگر (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    ملا = 1;
                }

            } ورنہ اگر (s_quoted) {
                اگر (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    ملا = 1;
                }

            } ورنہ اگر (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                آخری_space = 1;
                ملا = 1;
            }

            اگر (ملا) {
                لفظ = ngx_array_ڈالو(cf->دلائل);
                اگر (لفظ == تہی) {
                    واپس NGX_ERROR;
                }

                لفظ->ڈیٹا = ngx_pnalloc(cf->pool, b->pos - 1 - آغاز + 1);
                اگر (لفظ->ڈیٹا == تہی) {
                    واپس NGX_ERROR;
                }

                برائے (dst = لفظ->ڈیٹا, src = آغاز, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    اگر (*src == '\\') {
                        انتخاب (src[1]) {
                        حالت '"':
                        حالت '\'':
                        حالت '\\':
                            src++;
                            توڑو;

                        حالت 't':
                            *dst++ = '\t';
                            src += 2;
                            جاری;

                        حالت 'r':
                            *dst++ = '\r';
                            src += 2;
                            جاری;

                        حالت 'n':
                            *dst++ = '\n';
                            src += 2;
                            جاری;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                لفظ->len = len;

                اگر (ch == ';') {
                    واپس NGX_OK;
                }

                اگر (ch == '{') {
                    واپس NGX_CONF_بلاک_آغاز;
                }

                ملا = 0;
            }
        }
    }
}


حرف *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف        *rv;
    ngx_int_t    n;
    ngx_str_t   *قدر, file, name;
    ngx_glob_t   gl;

    قدر = cf->دلائل->elts;
    file = قدر[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لوگارتھم, 0, "include %s", file.ڈیٹا);

    اگر (ngx_conf_بھرا_name(cf->چکر, &file, 1) != NGX_OK) {
        واپس NGX_CONF_ERROR;
    }

    اگر (strpbrk((حرف *) file.ڈیٹا, "*?[") == تہی) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لوگارتھم, 0, "include %s", file.ڈیٹا);

        واپس ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, حجم(ngx_glob_t));

    gl.نمونہ = file.ڈیٹا;
    gl.لوگارتھم = cf->لوگارتھم;
    gl.test = 1;

    اگر (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.ڈیٹا);
        واپس NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    برائے ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        اگر (n != NGX_OK) {
            توڑو;
        }

        file.len = name.len++;
        file.ڈیٹا = ngx_pstrdup(cf->pool, &name);
        اگر (file.ڈیٹا == تہی) {
            واپس NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لوگارتھم, 0, "include %s", file.ڈیٹا);

        rv = ngx_conf_parse(cf, &file);

        اگر (rv != NGX_CONF_OK) {
            توڑو;
        }
    }

    ngx_close_glob(&gl);

    واپس rv;
}


ngx_int_t
ngx_conf_بھرا_name(ngx_چکر_t *چکر, ngx_str_t *name, ngx_uint_t conf_سابقہ)
{
    ngx_str_t  *سابقہ;

    سابقہ = conf_سابقہ ? &چکر->conf_سابقہ : &چکر->سابقہ;

    واپس ngx_get_بھرا_name(چکر->pool, سابقہ, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_چکر_t *چکر, ngx_str_t *name)
{
    ngx_str_t         بھرا;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#اگر (NGX_SUPPRESS_WARN)
    ngx_str_null(&بھرا);
#اگر_ختم

    اگر (name->len) {
        بھرا = *name;

        اگر (ngx_conf_بھرا_name(چکر, &بھرا, 0) != NGX_OK) {
            واپس تہی;
        }

        part = &چکر->open_files.part;
        file = part->elts;

        برائے (i = 0; /* void */ ; i++) {

            اگر (i >= part->nelts) {
                اگر (part->اگلا == تہی) {
                    توڑو;
                }
                part = part->اگلا;
                file = part->elts;
                i = 0;
            }

            اگر (بھرا.len != file[i].name.len) {
                جاری;
            }

            اگر (ngx_strcmp(بھرا.ڈیٹا, file[i].name.ڈیٹا) == 0) {
                واپس &file[i];
            }
        }
    }

    file = ngx_list_ڈالو(&چکر->open_files);
    اگر (file == تہی) {
        واپس تہی;
    }

    اگر (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = بھرا;

    } ورنہ {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = تہی;
    file->ڈیٹا = تہی;

    واپس file;
}


ساکن خالی
ngx_conf_flush_files(ngx_چکر_t *چکر)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, چکر->لوگارتھم, 0, "flush files");

    part = &چکر->open_files.part;
    file = part->elts;

    برائے (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->اگلا == تہی) {
                توڑو;
            }
            part = part->اگلا;
            file = part->elts;
            i = 0;
        }

        اگر (file[i].flush) {
            file[i].flush(&file[i], چکر->لوگارتھم);
        }
    }
}


خالی ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *آخری;
    va_list  دلائل;

    آخری = errstr + NGX_MAX_CONF_ERRSTR;

    va_آغاز(دلائل, fmt);
    p = ngx_vslprintf(errstr, آخری, fmt, دلائل);
    va_انتہا(دلائل);

    اگر (err) {
        p = ngx_log_errno(p, آخری, err);
    }

    اگر (cf->conf_file == تہی) {
        ngx_log_error(level, cf->لوگارتھم, 0, "%*s", p - errstr, errstr);
        واپس;
    }

    اگر (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->لوگارتھم, 0, "%*s in command line",
                      p - errstr, errstr);
        واپس;
    }

    ngx_log_error(level, cf->لوگارتھم, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.ڈیٹا, cf->conf_file->سطر);
}


حرف *
ngx_conf_سیٹ_flag_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف  *p = conf;

    ngx_str_t        *قدر;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->انحراف);

    اگر (*fp != NGX_CONF_UNSET) {
        واپس "is duplicate";
    }

    قدر = cf->دلائل->elts;

    اگر (ngx_strcasecmp(قدر[1].ڈیٹا, (u_char *) "on") == 0) {
        *fp = 1;

    } ورنہ اگر (ngx_strcasecmp(قدر[1].ڈیٹا, (u_char *) "off") == 0) {
        *fp = 0;

    } ورنہ {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     قدر[1].ڈیٹا, cmd->name.ڈیٹا);
        واپس NGX_CONF_ERROR;
    }

    اگر (cmd->post) {
        post = cmd->post;
        واپس post->post_handler(cf, post, fp);
    }

    واپس NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_str_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف  *p = conf;

    ngx_str_t        *میدان, *قدر;
    ngx_conf_post_t  *post;

    میدان = (ngx_str_t *) (p + cmd->انحراف);

    اگر (میدان->ڈیٹا) {
        واپس "is duplicate";
    }

    قدر = cf->دلائل->elts;

    *میدان = قدر[1];

    اگر (cmd->post) {
        post = cmd->post;
        واپس post->post_handler(cf, post, میدان);
    }

    واپس NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_str_array_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف  *p = conf;

    ngx_str_t         *قدر, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->انحراف);

    اگر (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_str_t));
        اگر (*a == تہی) {
            واپس NGX_CONF_ERROR;
        }
    }

    s = ngx_array_ڈالو(*a);
    اگر (s == تہی) {
        واپس NGX_CONF_ERROR;
    }

    قدر = cf->دلائل->elts;

    *s = قدر[1];

    اگر (cmd->post) {
        post = cmd->post;
        واپس post->post_handler(cf, post, s);
    }

    واپس NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_keyval_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف  *p = conf;

    ngx_str_t         *قدر;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->انحراف);

    اگر (*a == NGX_CONF_UNSET_PTR || *a == تہی) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_keyval_t));
        اگر (*a == تہی) {
            واپس NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_ڈالو(*a);
    اگر (kv == تہی) {
        واپس NGX_CONF_ERROR;
    }

    قدر = cf->دلائل->elts;

    kv->کلید = قدر[1];
    kv->قدر = قدر[2];

    اگر (cmd->post) {
        post = cmd->post;
        واپس post->post_handler(cf, post, kv);
    }

    واپس NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_عدد_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->انحراف);

    اگر (*np != NGX_CONF_UNSET) {
        واپس "is duplicate";
    }

    قدر = cf->دلائل->elts;
    *np = ngx_atoi(قدر[1].ڈیٹا, قدر[1].len);
    اگر (*np == NGX_ERROR) {
        واپس "invalid number";
    }

    اگر (cmd->post) {
        post = cmd->post;
        واپس post->post_handler(cf, post, np);
    }

    واپس NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_سائز_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف  *p = conf;

    سائز_t           *sp;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    sp = (سائز_t *) (p + cmd->انحراف);
    اگر (*sp != NGX_CONF_UNSET_سائز) {
        واپس "is duplicate";
    }

    قدر = cf->دلائل->elts;

    *sp = ngx_parse_سائز(&قدر[1]);
    اگر (*sp == (سائز_t) NGX_ERROR) {
        واپس "invalid value";
    }

    اگر (cmd->post) {
        post = cmd->post;
        واپس post->post_handler(cf, post, sp);
    }

    واپس NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_off_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف  *p = conf;

    off_t            *op;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->انحراف);
    اگر (*op != NGX_CONF_UNSET) {
        واپس "is duplicate";
    }

    قدر = cf->دلائل->elts;

    *op = ngx_parse_انحراف(&قدر[1]);
    اگر (*op == (off_t) NGX_ERROR) {
        واپس "invalid value";
    }

    اگر (cmd->post) {
        post = cmd->post;
        واپس post->post_handler(cf, post, op);
    }

    واپس NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_msec_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->انحراف);
    اگر (*msp != NGX_CONF_UNSET_MSEC) {
        واپس "is duplicate";
    }

    قدر = cf->دلائل->elts;

    *msp = ngx_parse_وقت(&قدر[1], 0);
    اگر (*msp == (ngx_msec_t) NGX_ERROR) {
        واپس "invalid value";
    }

    اگر (cmd->post) {
        post = cmd->post;
        واپس post->post_handler(cf, post, msp);
    }

    واپس NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_sec_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف  *p = conf;

    وقت_t           *sp;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    sp = (وقت_t *) (p + cmd->انحراف);
    اگر (*sp != NGX_CONF_UNSET) {
        واپس "is duplicate";
    }

    قدر = cf->دلائل->elts;

    *sp = ngx_parse_وقت(&قدر[1], 1);
    اگر (*sp == (وقت_t) NGX_ERROR) {
        واپس "invalid value";
    }

    اگر (cmd->post) {
        post = cmd->post;
        واپس post->post_handler(cf, post, sp);
    }

    واپس NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_bufs_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف *p = conf;

    ngx_str_t   *قدر;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->انحراف);
    اگر (bufs->عدد) {
        واپس "is duplicate";
    }

    قدر = cf->دلائل->elts;

    bufs->عدد = ngx_atoi(قدر[1].ڈیٹا, قدر[1].len);
    اگر (bufs->عدد == NGX_ERROR || bufs->عدد == 0) {
        واپس "invalid value";
    }

    bufs->سائز = ngx_parse_سائز(&قدر[2]);
    اگر (bufs->سائز == (سائز_t) NGX_ERROR || bufs->سائز == 0) {
        واپس "invalid value";
    }

    واپس NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_enum_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *قدر;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->انحراف);

    اگر (*np != NGX_CONF_UNSET_UINT) {
        واپس "is duplicate";
    }

    قدر = cf->دلائل->elts;
    e = cmd->post;

    برائے (i = 0; e[i].name.len != 0; i++) {
        اگر (e[i].name.len != قدر[1].len
            || ngx_strcasecmp(e[i].name.ڈیٹا, قدر[1].ڈیٹا) != 0)
        {
            جاری;
        }

        *np = e[i].قدر;

        واپس NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", قدر[1].ڈیٹا);

    واپس NGX_CONF_ERROR;
}


حرف *
ngx_conf_سیٹ_bitmask_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *قدر;
    ngx_conf_bitmask_t  *نقاب;


    np = (ngx_uint_t *) (p + cmd->انحراف);
    قدر = cf->دلائل->elts;
    نقاب = cmd->post;

    برائے (i = 1; i < cf->دلائل->nelts; i++) {
        برائے (m = 0; نقاب[m].name.len != 0; m++) {

            اگر (نقاب[m].name.len != قدر[i].len
                || ngx_strcasecmp(نقاب[m].name.ڈیٹا, قدر[i].ڈیٹا) != 0)
            {
                جاری;
            }

            اگر (*np & نقاب[m].نقاب) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", قدر[i].ڈیٹا);

            } ورنہ {
                *np |= نقاب[m].نقاب;
            }

            توڑو;
        }

        اگر (نقاب[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", قدر[i].ڈیٹا);

            واپس NGX_CONF_ERROR;
        }
    }

    واپس NGX_CONF_OK;
}


#اگر 0

حرف *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    واپس "unsupported on this platform";
}

#اگر_ختم


حرف *
ngx_conf_deprecated(ngx_conf_t *cf, خالی *post, خالی *ڈیٹا)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    واپس NGX_CONF_OK;
}


حرف *
ngx_conf_check_عدد_bounds(ngx_conf_t *cf, خالی *post, خالی *ڈیٹا)
{
    ngx_conf_عدد_bounds_t  *bounds = post;
    ngx_int_t  *np = ڈیٹا;

    اگر (bounds->بلند == -1) {
        اگر (*np >= bounds->کم) {
            واپس NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->کم);

        واپس NGX_CONF_ERROR;
    }

    اگر (*np >= bounds->کم && *np <= bounds->بلند) {
        واپس NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->کم, bounds->بلند);

    واپس NGX_CONF_ERROR;
}


حرف *
ngx_conf_check_سائز_nonzero(ngx_conf_t *cf, خالی *post, خالی *ڈیٹا)
{
    سائز_t  *sp = ڈیٹا;

    اگر (*sp == 0) {
        واپس "value must be nonzero";
    }

    واپس NGX_CONF_OK;
}
