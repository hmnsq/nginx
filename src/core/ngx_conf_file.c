
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#تعریف_کړه NGX_CONF_بفر  4096

ساكن ngx_int_t ngx_conf_جمع_کړه_dump(ngx_conf_t *cf, ngx_str_t *filename);
ساكن ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t وروستی);
ساكن ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
ساكن فراغ ngx_conf_flush_files(ngx_دوره_t *دوره);


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

ساكن ngx_uint_t دلیل_شمېره[] = {
    NGX_CONF_NOARGS,
    NGX_CONF_TAKE1,
    NGX_CONF_TAKE2,
    NGX_CONF_TAKE3,
    NGX_CONF_TAKE4,
    NGX_CONF_TAKE5,
    NGX_CONF_TAKE6,
    NGX_CONF_TAKE7
};


ngx_conf_post_t  ngx_conf_اندازه_nonzero_post = { ngx_conf_check_اندازه_nonzero };


حرف *
ngx_conf_param(ngx_conf_t *cf)
{
    حرف             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->دوره->conf_param;

    إذا (param->len == 0) {
        أرجع NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, حجم(ngx_conf_file_t));

    ngx_memzero(&b, حجم(ngx_buf_t));

    b.پیل = param->معلومات;
    b.pos = param->معلومات;
    b.وروستی = param->معلومات + param->len;
    b.پای_ټکی = b.وروستی;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.معلومات = عدم;
    conf_file.کرښه = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->بفر = &b;

    rv = ngx_conf_parse(cf, عدم);

    cf->conf_file = عدم;

    أرجع rv;
}


ساكن ngx_int_t
ngx_conf_جمع_کړه_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             اندازه;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_غوټه_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_دوراني_ازموینه_32_long(filename->معلومات, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->دوره->config_dump_rbtree, filename, hash);

    إذا (sn) {
        cf->conf_file->dump = عدم;
        أرجع NGX_OK;
    }

    p = ngx_pstrdup(cf->دوره->pool, filename);
    إذا (p == عدم) {
        أرجع NGX_ERROR;
    }

    cd = ngx_array_ټېل(&cf->دوره->config_dump);
    إذا (cd == عدم) {
        أرجع NGX_ERROR;
    }

    اندازه = ngx_file_اندازه(&cf->conf_file->file.info);

    buf = ngx_create_لنډمهاله_buf(cf->دوره->pool, (اندازه_t) اندازه);
    إذا (buf == عدم) {
        أرجع NGX_ERROR;
    }

    cd->name.معلومات = p;
    cd->name.len = filename->len;
    cd->بفر = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->لنډمهاله_pool, حجم(ngx_str_غوټه_t));
    إذا (sn == عدم) {
        أرجع NGX_ERROR;
    }

    sn->غوټه.کلی = hash;
    sn->str = cd->name;

    ngx_rbtree_ورننباسه(&cf->دوره->config_dump_rbtree, &sn->غوټه);

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
#د_که_پای

    إذا (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->معلومات, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        إذا (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->معلومات);
            أرجع NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        إذا (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->لوګاریتم, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->معلومات);
        }

        cf->conf_file->بفر = &buf;

        buf.پیل = ngx_alloc(NGX_CONF_بفر, cf->لوګاریتم);
        إذا (buf.پیل == عدم) {
            اذهب failed;
        }

        buf.pos = buf.پیل;
        buf.وروستی = buf.پیل;
        buf.پای_ټکی = buf.وروستی + NGX_CONF_بفر;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.معلومات = filename->معلومات;
        cf->conf_file->file.دننه_واټن = 0;
        cf->conf_file->file.لوګاریتم = cf->لوګاریتم;
        cf->conf_file->کرښه = 1;

        type = parse_file;

        إذا (ngx_dump_config
#إذا (NGX_CONTROL_API)
            || ngx_control_api_enabled
#د_که_پای
#إذا (NGX_DEBUG)
            || 1
#د_که_پای
           )
        {
            إذا (ngx_conf_جمع_کړه_dump(cf, filename) != NGX_OK) {
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
            اذهب بشپړ;
        }

        إذا (rc == NGX_CONF_بلاک_بشپړ) {

            إذا (type != parse_بلاک) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                اذهب failed;
            }

            اذهب بشپړ;
        }

        إذا (rc == NGX_CONF_FILE_بشپړ) {

            إذا (type == parse_بلاک) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                اذهب failed;
            }

            اذهب بشپړ;
        }

        إذا (rc == NGX_CONF_بلاک_پیل) {

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

            إذا (rc == NGX_CONF_بلاک_پیل) {
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

بشپړ:

    إذا (filename) {
        إذا (cf->conf_file->بفر->پیل) {
            ngx_free(cf->conf_file->بفر->پیل);
        }

        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوګاریتم, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->معلومات);
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
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t وروستی)
{
    حرف           *rv;
    فراغ           *conf, **confp;
    ngx_uint_t      i, وموندل;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->دلیلونه->elts;

    وموندل = 0;

    لكل (i = 0; cf->دوره->modules[i]; i++) {

        cmd = cf->دوره->modules[i]->commands;
        إذا (cmd == عدم) {
            استمر;
        }

        لكل ( /* void */ ; cmd->name.len; cmd++) {

            إذا (name->len != cmd->name.len) {
                استمر;
            }

            إذا (ngx_strcmp(name->معلومات, cmd->name.معلومات) != 0) {
                استمر;
            }

            وموندل = 1;

            إذا (cf->دوره->modules[i]->type != NGX_CONF_MODULE
                && cf->دوره->modules[i]->type != cf->module_type)
            {
                استمر;
            }

            /* is the directive's location right ? */

            إذا (!(cmd->type & cf->cmd_type)) {
                استمر;
            }

            إذا (!(cmd->type & NGX_CONF_بلاک) && وروستی != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->معلومات);
                أرجع NGX_ERROR;
            }

            إذا ((cmd->type & NGX_CONF_بلاک) && وروستی != NGX_CONF_بلاک_پیل) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->معلومات);
                أرجع NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            إذا (!(cmd->type & NGX_CONF_ANY)) {

                إذا (cmd->type & NGX_CONF_FLAG) {

                    إذا (cf->دلیلونه->nelts != 2) {
                        اذهب invalid;
                    }

                } وإلا إذا (cmd->type & NGX_CONF_1_نور) {

                    إذا (cf->دلیلونه->nelts < 2) {
                        اذهب invalid;
                    }

                } وإلا إذا (cmd->type & NGX_CONF_2_نور) {

                    إذا (cf->دلیلونه->nelts < 3) {
                        اذهب invalid;
                    }

                } وإلا إذا (cf->دلیلونه->nelts > NGX_CONF_MAX_دلیلونه) {

                    اذهب invalid;

                } وإلا إذا (!(cmd->type & دلیل_شمېره[cf->دلیلونه->nelts - 1]))
                {
                    اذهب invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = عدم;

            إذا (cmd->type & NGX_مستقیم_CONF) {
                conf = ((فراغ **) cf->ctx)[cf->دوره->modules[i]->شاخص];

            } وإلا إذا (cmd->type & NGX_MAIN_CONF) {
                conf = &(((فراغ **) cf->ctx)[cf->دوره->modules[i]->شاخص]);

            } وإلا إذا (cf->ctx) {
                confp = *(فراغ **) ((حرف *) cf->ctx + cmd->conf);

                إذا (confp) {
                    conf = confp[cf->دوره->modules[i]->ctx_شاخص];
                }
            }

            rv = cmd->سېټ(cf, cmd, conf);

            إذا (rv == NGX_CONF_OK) {
                أرجع NGX_OK;
            }

            إذا (rv == NGX_CONF_ERROR) {
                أرجع NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->معلومات, rv);

            أرجع NGX_ERROR;
        }
    }

    إذا (وموندل) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->معلومات);

        أرجع NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->معلومات);

    أرجع NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->معلومات);

    أرجع NGX_ERROR;
}


ساكن ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *پیل, ch, *src, *dst;
    off_t        file_اندازه;
    اندازه_t       len;
    ssize_t      n, اندازه;
    ngx_uint_t   وموندل, need_space, وروستی_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, پیل_کرښه;
    ngx_str_t   *کلمه;
    ngx_buf_t   *b, *dump;

    وموندل = 0;
    need_space = 0;
    وروستی_space = 1;
    sharp_comment = 0;
    variable = 0;
    quoted = 0;
    s_quoted = 0;
    d_quoted = 0;

    cf->دلیلونه->nelts = 0;
    b = cf->conf_file->بفر;
    dump = cf->conf_file->dump;
    پیل = b->pos;
    پیل_کرښه = cf->conf_file->کرښه;

    file_اندازه = ngx_file_اندازه(&cf->conf_file->file.info);

    لكل ( ;; ) {

        إذا (b->pos >= b->وروستی) {

            إذا (cf->conf_file->file.دننه_واټن >= file_اندازه) {

                إذا (cf->دلیلونه->nelts > 0 || !وروستی_space) {

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

                أرجع NGX_CONF_FILE_بشپړ;
            }

            len = b->pos - پیل;

            إذا (len == NGX_CONF_بفر) {
                cf->conf_file->کرښه = پیل_کرښه;

                إذا (d_quoted) {
                    ch = '"';

                } وإلا إذا (s_quoted) {
                    ch = '\'';

                } وإلا {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (اندازه_t) 10, پیل);
                    أرجع NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                أرجع NGX_ERROR;
            }

            إذا (len) {
                ngx_memmove(b->پیل, پیل, len);
            }

            اندازه = (ssize_t) (file_اندازه - cf->conf_file->file.دننه_واټن);

            إذا (اندازه > b->پای_ټکی - (b->پیل + len)) {
                اندازه = b->پای_ټکی - (b->پیل + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->پیل + len, اندازه,
                              cf->conf_file->file.دننه_واټن);

            إذا (n == NGX_ERROR) {
                أرجع NGX_ERROR;
            }

            إذا (n != اندازه) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, اندازه);
                أرجع NGX_ERROR;
            }

            b->pos = b->پیل + len;
            b->وروستی = b->pos + n;
            پیل = b->پیل;

            إذا (dump) {
                dump->وروستی = ngx_cpymem(dump->وروستی, b->pos, اندازه);
            }
        }

        ch = *b->pos++;

        إذا (ch == LF) {
            cf->conf_file->کرښه++;

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
                وروستی_space = 1;
                need_space = 0;
                استمر;
            }

            إذا (ch == ';') {
                أرجع NGX_OK;
            }

            إذا (ch == '{') {
                أرجع NGX_CONF_بلاک_پیل;
            }

            إذا (ch == ')') {
                وروستی_space = 1;
                need_space = 0;

            } وإلا {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                أرجع NGX_ERROR;
            }
        }

        إذا (وروستی_space) {

            پیل = b->pos - 1;
            پیل_کرښه = cf->conf_file->کرښه;

            إذا (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                استمر;
            }

            بدل (ch) {

            حالة ';':
            حالة '{':
                إذا (cf->دلیلونه->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    أرجع NGX_ERROR;
                }

                إذا (ch == '{') {
                    أرجع NGX_CONF_بلاک_پیل;
                }

                أرجع NGX_OK;

            حالة '}':
                إذا (cf->دلیلونه->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    أرجع NGX_ERROR;
                }

                أرجع NGX_CONF_بلاک_بشپړ;

            حالة '#':
                sharp_comment = 1;
                استمر;

            حالة '\\':
                quoted = 1;
                وروستی_space = 0;
                استمر;

            حالة '"':
                پیل++;
                d_quoted = 1;
                وروستی_space = 0;
                استمر;

            حالة '\'':
                پیل++;
                s_quoted = 1;
                وروستی_space = 0;
                استمر;

            حالة '$':
                variable = 1;
                وروستی_space = 0;
                استمر;

            افتراضي:
                وروستی_space = 0;
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
                    وموندل = 1;
                }

            } وإلا إذا (s_quoted) {
                إذا (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    وموندل = 1;
                }

            } وإلا إذا (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                وروستی_space = 1;
                وموندل = 1;
            }

            إذا (وموندل) {
                کلمه = ngx_array_ټېل(cf->دلیلونه);
                إذا (کلمه == عدم) {
                    أرجع NGX_ERROR;
                }

                کلمه->معلومات = ngx_pnalloc(cf->pool, b->pos - 1 - پیل + 1);
                إذا (کلمه->معلومات == عدم) {
                    أرجع NGX_ERROR;
                }

                لكل (dst = کلمه->معلومات, src = پیل, len = 0;
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
                کلمه->len = len;

                إذا (ch == ';') {
                    أرجع NGX_OK;
                }

                إذا (ch == '{') {
                    أرجع NGX_CONF_بلاک_پیل;
                }

                وموندل = 0;
            }
        }
    }
}


حرف *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف        *rv;
    ngx_int_t    n;
    ngx_str_t   *ارزښت, file, name;
    ngx_glob_t   gl;

    ارزښت = cf->دلیلونه->elts;
    file = ارزښت[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لوګاریتم, 0, "include %s", file.معلومات);

    إذا (ngx_conf_ډک_name(cf->دوره, &file, 1) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    إذا (strpbrk((حرف *) file.معلومات, "*?[") == عدم) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لوګاریتم, 0, "include %s", file.معلومات);

        أرجع ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, حجم(ngx_glob_t));

    gl.نمونه = file.معلومات;
    gl.لوګاریتم = cf->لوګاریتم;
    gl.test = 1;

    إذا (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.معلومات);
        أرجع NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    لكل ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        إذا (n != NGX_OK) {
            اكسر;
        }

        file.len = name.len++;
        file.معلومات = ngx_pstrdup(cf->pool, &name);
        إذا (file.معلومات == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لوګاریتم, 0, "include %s", file.معلومات);

        rv = ngx_conf_parse(cf, &file);

        إذا (rv != NGX_CONF_OK) {
            اكسر;
        }
    }

    ngx_close_glob(&gl);

    أرجع rv;
}


ngx_int_t
ngx_conf_ډک_name(ngx_دوره_t *دوره, ngx_str_t *name, ngx_uint_t conf_مختاړی)
{
    ngx_str_t  *مختاړی;

    مختاړی = conf_مختاړی ? &دوره->conf_مختاړی : &دوره->مختاړی;

    أرجع ngx_get_ډک_name(دوره->pool, مختاړی, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_دوره_t *دوره, ngx_str_t *name)
{
    ngx_str_t         ډک;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#إذا (NGX_SUPPRESS_WARN)
    ngx_str_null(&ډک);
#د_که_پای

    إذا (name->len) {
        ډک = *name;

        إذا (ngx_conf_ډک_name(دوره, &ډک, 0) != NGX_OK) {
            أرجع عدم;
        }

        part = &دوره->open_files.part;
        file = part->elts;

        لكل (i = 0; /* void */ ; i++) {

            إذا (i >= part->nelts) {
                إذا (part->بل == عدم) {
                    اكسر;
                }
                part = part->بل;
                file = part->elts;
                i = 0;
            }

            إذا (ډک.len != file[i].name.len) {
                استمر;
            }

            إذا (ngx_strcmp(ډک.معلومات, file[i].name.معلومات) == 0) {
                أرجع &file[i];
            }
        }
    }

    file = ngx_list_ټېل(&دوره->open_files);
    إذا (file == عدم) {
        أرجع عدم;
    }

    إذا (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = ډک;

    } وإلا {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = عدم;
    file->معلومات = عدم;

    أرجع file;
}


ساكن فراغ
ngx_conf_flush_files(ngx_دوره_t *دوره)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, دوره->لوګاریتم, 0, "flush files");

    part = &دوره->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بل == عدم) {
                اكسر;
            }
            part = part->بل;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].flush) {
            file[i].flush(&file[i], دوره->لوګاریتم);
        }
    }
}


فراغ ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *وروستی;
    va_list  دلیلونه;

    وروستی = errstr + NGX_MAX_CONF_ERRSTR;

    va_پیل(دلیلونه, fmt);
    p = ngx_vslprintf(errstr, وروستی, fmt, دلیلونه);
    va_پای_ټکی(دلیلونه);

    إذا (err) {
        p = ngx_log_errno(p, وروستی, err);
    }

    إذا (cf->conf_file == عدم) {
        ngx_log_error(level, cf->لوګاریتم, 0, "%*s", p - errstr, errstr);
        أرجع;
    }

    إذا (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->لوګاریتم, 0, "%*s in command line",
                      p - errstr, errstr);
        أرجع;
    }

    ngx_log_error(level, cf->لوګاریتم, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.معلومات, cf->conf_file->کرښه);
}


حرف *
ngx_conf_سېټ_flag_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t        *ارزښت;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->دننه_واټن);

    إذا (*fp != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    ارزښت = cf->دلیلونه->elts;

    إذا (ngx_strcasecmp(ارزښت[1].معلومات, (u_char *) "on") == 0) {
        *fp = 1;

    } وإلا إذا (ngx_strcasecmp(ارزښت[1].معلومات, (u_char *) "off") == 0) {
        *fp = 0;

    } وإلا {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     ارزښت[1].معلومات, cmd->name.معلومات);
        أرجع NGX_CONF_ERROR;
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, fp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سېټ_str_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t        *ډګر, *ارزښت;
    ngx_conf_post_t  *post;

    ډګر = (ngx_str_t *) (p + cmd->دننه_واټن);

    إذا (ډګر->معلومات) {
        أرجع "is duplicate";
    }

    ارزښت = cf->دلیلونه->elts;

    *ډګر = ارزښت[1];

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, ډګر);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سېټ_str_array_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t         *ارزښت, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->دننه_واټن);

    إذا (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_str_t));
        إذا (*a == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    s = ngx_array_ټېل(*a);
    إذا (s == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ارزښت = cf->دلیلونه->elts;

    *s = ارزښت[1];

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, s);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سېټ_keyval_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t         *ارزښت;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->دننه_واټن);

    إذا (*a == NGX_CONF_UNSET_PTR || *a == عدم) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_keyval_t));
        إذا (*a == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_ټېل(*a);
    إذا (kv == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ارزښت = cf->دلیلونه->elts;

    kv->کلی = ارزښت[1];
    kv->ارزښت = ارزښت[2];

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, kv);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سېټ_عدد_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *ارزښت;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->دننه_واټن);

    إذا (*np != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    ارزښت = cf->دلیلونه->elts;
    *np = ngx_atoi(ارزښت[1].معلومات, ارزښت[1].len);
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
ngx_conf_سېټ_اندازه_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    اندازه_t           *sp;
    ngx_str_t        *ارزښت;
    ngx_conf_post_t  *post;


    sp = (اندازه_t *) (p + cmd->دننه_واټن);
    إذا (*sp != NGX_CONF_UNSET_اندازه) {
        أرجع "is duplicate";
    }

    ارزښت = cf->دلیلونه->elts;

    *sp = ngx_parse_اندازه(&ارزښت[1]);
    إذا (*sp == (اندازه_t) NGX_ERROR) {
        أرجع "invalid value";
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, sp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سېټ_off_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    off_t            *op;
    ngx_str_t        *ارزښت;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->دننه_واټن);
    إذا (*op != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    ارزښت = cf->دلیلونه->elts;

    *op = ngx_parse_دننه_واټن(&ارزښت[1]);
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
ngx_conf_سېټ_msec_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *ارزښت;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->دننه_واټن);
    إذا (*msp != NGX_CONF_UNSET_MSEC) {
        أرجع "is duplicate";
    }

    ارزښت = cf->دلیلونه->elts;

    *msp = ngx_parse_وخت(&ارزښت[1], 0);
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
ngx_conf_سېټ_sec_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    وخت_t           *sp;
    ngx_str_t        *ارزښت;
    ngx_conf_post_t  *post;


    sp = (وخت_t *) (p + cmd->دننه_واټن);
    إذا (*sp != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    ارزښت = cf->دلیلونه->elts;

    *sp = ngx_parse_وخت(&ارزښت[1], 1);
    إذا (*sp == (وخت_t) NGX_ERROR) {
        أرجع "invalid value";
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, sp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سېټ_bufs_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف *p = conf;

    ngx_str_t   *ارزښت;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->دننه_واټن);
    إذا (bufs->عدد) {
        أرجع "is duplicate";
    }

    ارزښت = cf->دلیلونه->elts;

    bufs->عدد = ngx_atoi(ارزښت[1].معلومات, ارزښت[1].len);
    إذا (bufs->عدد == NGX_ERROR || bufs->عدد == 0) {
        أرجع "invalid value";
    }

    bufs->اندازه = ngx_parse_اندازه(&ارزښت[2]);
    إذا (bufs->اندازه == (اندازه_t) NGX_ERROR || bufs->اندازه == 0) {
        أرجع "invalid value";
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سېټ_enum_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *ارزښت;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->دننه_واټن);

    إذا (*np != NGX_CONF_UNSET_UINT) {
        أرجع "is duplicate";
    }

    ارزښت = cf->دلیلونه->elts;
    e = cmd->post;

    لكل (i = 0; e[i].name.len != 0; i++) {
        إذا (e[i].name.len != ارزښت[1].len
            || ngx_strcasecmp(e[i].name.معلومات, ارزښت[1].معلومات) != 0)
        {
            استمر;
        }

        *np = e[i].ارزښت;

        أرجع NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", ارزښت[1].معلومات);

    أرجع NGX_CONF_ERROR;
}


حرف *
ngx_conf_سېټ_bitmask_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *ارزښت;
    ngx_conf_bitmask_t  *ماسک;


    np = (ngx_uint_t *) (p + cmd->دننه_واټن);
    ارزښت = cf->دلیلونه->elts;
    ماسک = cmd->post;

    لكل (i = 1; i < cf->دلیلونه->nelts; i++) {
        لكل (m = 0; ماسک[m].name.len != 0; m++) {

            إذا (ماسک[m].name.len != ارزښت[i].len
                || ngx_strcasecmp(ماسک[m].name.معلومات, ارزښت[i].معلومات) != 0)
            {
                استمر;
            }

            إذا (*np & ماسک[m].ماسک) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", ارزښت[i].معلومات);

            } وإلا {
                *np |= ماسک[m].ماسک;
            }

            اكسر;
        }

        إذا (ماسک[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", ارزښت[i].معلومات);

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

#د_که_پای


حرف *
ngx_conf_deprecated(ngx_conf_t *cf, فراغ *post, فراغ *معلومات)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_check_عدد_bounds(ngx_conf_t *cf, فراغ *post, فراغ *معلومات)
{
    ngx_conf_عدد_bounds_t  *bounds = post;
    ngx_int_t  *np = معلومات;

    إذا (bounds->لوړ == -1) {
        إذا (*np >= bounds->ټیټ) {
            أرجع NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->ټیټ);

        أرجع NGX_CONF_ERROR;
    }

    إذا (*np >= bounds->ټیټ && *np <= bounds->لوړ) {
        أرجع NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->ټیټ, bounds->لوړ);

    أرجع NGX_CONF_ERROR;
}


حرف *
ngx_conf_check_اندازه_nonzero(ngx_conf_t *cf, فراغ *post, فراغ *معلومات)
{
    اندازه_t  *sp = معلومات;

    إذا (*sp == 0) {
        أرجع "value must be nonzero";
    }

    أرجع NGX_CONF_OK;
}
