
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#تعریف_کن NGX_CONF_میانگیر  4096

ساكن ngx_int_t ngx_conf_جمع_کن_dump(ngx_conf_t *cf, ngx_str_t *filename);
ساكن ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t آخرین);
ساكن ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
ساكن فراغ ngx_conf_flush_files(ngx_چرخه_t *چرخه);


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

ساكن ngx_uint_t آرگومان_عدد[] = {
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

    param = &cf->چرخه->conf_param;

    إذا (param->len == 0) {
        أرجع NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, حجم(ngx_conf_file_t));

    ngx_memzero(&b, حجم(ngx_buf_t));

    b.شروع = param->داده;
    b.pos = param->داده;
    b.آخرین = param->داده + param->len;
    b.انتها = b.آخرین;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.داده = عدم;
    conf_file.سطر = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->میانگیر = &b;

    rv = ngx_conf_parse(cf, عدم);

    cf->conf_file = عدم;

    أرجع rv;
}


ساكن ngx_int_t
ngx_conf_جمع_کن_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             اندازه;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_گره_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_بررسی_چرخهای_32_long(filename->داده, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->چرخه->config_dump_rbtree, filename, hash);

    إذا (sn) {
        cf->conf_file->dump = عدم;
        أرجع NGX_OK;
    }

    p = ngx_pstrdup(cf->چرخه->pool, filename);
    إذا (p == عدم) {
        أرجع NGX_ERROR;
    }

    cd = ngx_array_افزودن(&cf->چرخه->config_dump);
    إذا (cd == عدم) {
        أرجع NGX_ERROR;
    }

    اندازه = ngx_file_اندازه(&cf->conf_file->file.info);

    buf = ngx_create_موقت_buf(cf->چرخه->pool, (اندازه_t) اندازه);
    إذا (buf == عدم) {
        أرجع NGX_ERROR;
    }

    cd->name.داده = p;
    cd->name.len = filename->len;
    cd->میانگیر = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->موقت_pool, حجم(ngx_str_گره_t));
    إذا (sn == عدم) {
        أرجع NGX_ERROR;
    }

    sn->گره.کلید = hash;
    sn->str = cd->name;

    ngx_rbtree_درج(&cf->چرخه->config_dump_rbtree, &sn->گره);

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
        parse_بلوک,
        parse_param
    } type;

#إذا (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = عدم;
#پایان_اگر

    إذا (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->داده, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        إذا (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->داده);
            أرجع NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        إذا (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->لگاریتم, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->داده);
        }

        cf->conf_file->میانگیر = &buf;

        buf.شروع = ngx_alloc(NGX_CONF_میانگیر, cf->لگاریتم);
        إذا (buf.شروع == عدم) {
            اذهب failed;
        }

        buf.pos = buf.شروع;
        buf.آخرین = buf.شروع;
        buf.انتها = buf.آخرین + NGX_CONF_میانگیر;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.داده = filename->داده;
        cf->conf_file->file.جابه_جایی = 0;
        cf->conf_file->file.لگاریتم = cf->لگاریتم;
        cf->conf_file->سطر = 1;

        type = parse_file;

        إذا (ngx_dump_config
#إذا (NGX_CONTROL_API)
            || ngx_control_api_enabled
#پایان_اگر
#إذا (NGX_DEBUG)
            || 1
#پایان_اگر
           )
        {
            إذا (ngx_conf_جمع_کن_dump(cf, filename) != NGX_OK) {
                اذهب failed;
            }

        } وإلا {
            cf->conf_file->dump = عدم;
        }

    } وإلا إذا (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_بلوک;

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
            اذهب انجامشده;
        }

        إذا (rc == NGX_CONF_بلوک_انجامشده) {

            إذا (type != parse_بلوک) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                اذهب failed;
            }

            اذهب انجامشده;
        }

        إذا (rc == NGX_CONF_FILE_انجامشده) {

            إذا (type == parse_بلوک) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                اذهب failed;
            }

            اذهب انجامشده;
        }

        إذا (rc == NGX_CONF_بلوک_شروع) {

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

            إذا (rc == NGX_CONF_بلوک_شروع) {
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

انجامشده:

    إذا (filename) {
        إذا (cf->conf_file->میانگیر->شروع) {
            ngx_free(cf->conf_file->میانگیر->شروع);
        }

        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لگاریتم, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->داده);
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
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t آخرین)
{
    حرف           *rv;
    فراغ           *conf, **confp;
    ngx_uint_t      i, یافت;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->آرگومانها->elts;

    یافت = 0;

    لكل (i = 0; cf->چرخه->modules[i]; i++) {

        cmd = cf->چرخه->modules[i]->commands;
        إذا (cmd == عدم) {
            استمر;
        }

        لكل ( /* void */ ; cmd->name.len; cmd++) {

            إذا (name->len != cmd->name.len) {
                استمر;
            }

            إذا (ngx_strcmp(name->داده, cmd->name.داده) != 0) {
                استمر;
            }

            یافت = 1;

            إذا (cf->چرخه->modules[i]->type != NGX_CONF_MODULE
                && cf->چرخه->modules[i]->type != cf->module_type)
            {
                استمر;
            }

            /* is the directive's location right ? */

            إذا (!(cmd->type & cf->cmd_type)) {
                استمر;
            }

            إذا (!(cmd->type & NGX_CONF_بلوک) && آخرین != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->داده);
                أرجع NGX_ERROR;
            }

            إذا ((cmd->type & NGX_CONF_بلوک) && آخرین != NGX_CONF_بلوک_شروع) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->داده);
                أرجع NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            إذا (!(cmd->type & NGX_CONF_ANY)) {

                إذا (cmd->type & NGX_CONF_FLAG) {

                    إذا (cf->آرگومانها->nelts != 2) {
                        اذهب invalid;
                    }

                } وإلا إذا (cmd->type & NGX_CONF_1_بیشتر) {

                    إذا (cf->آرگومانها->nelts < 2) {
                        اذهب invalid;
                    }

                } وإلا إذا (cmd->type & NGX_CONF_2_بیشتر) {

                    إذا (cf->آرگومانها->nelts < 3) {
                        اذهب invalid;
                    }

                } وإلا إذا (cf->آرگومانها->nelts > NGX_CONF_MAX_آرگومانها) {

                    اذهب invalid;

                } وإلا إذا (!(cmd->type & آرگومان_عدد[cf->آرگومانها->nelts - 1]))
                {
                    اذهب invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = عدم;

            إذا (cmd->type & NGX_مستقیم_CONF) {
                conf = ((فراغ **) cf->ctx)[cf->چرخه->modules[i]->شاخص];

            } وإلا إذا (cmd->type & NGX_MAIN_CONF) {
                conf = &(((فراغ **) cf->ctx)[cf->چرخه->modules[i]->شاخص]);

            } وإلا إذا (cf->ctx) {
                confp = *(فراغ **) ((حرف *) cf->ctx + cmd->conf);

                إذا (confp) {
                    conf = confp[cf->چرخه->modules[i]->ctx_شاخص];
                }
            }

            rv = cmd->مجموعه(cf, cmd, conf);

            إذا (rv == NGX_CONF_OK) {
                أرجع NGX_OK;
            }

            إذا (rv == NGX_CONF_ERROR) {
                أرجع NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->داده, rv);

            أرجع NGX_ERROR;
        }
    }

    إذا (یافت) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->داده);

        أرجع NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->داده);

    أرجع NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->داده);

    أرجع NGX_ERROR;
}


ساكن ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *شروع, ch, *src, *dst;
    off_t        file_اندازه;
    اندازه_t       len;
    ssize_t      n, اندازه;
    ngx_uint_t   یافت, need_space, آخرین_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, شروع_سطر;
    ngx_str_t   *کلمه;
    ngx_buf_t   *b, *dump;

    یافت = 0;
    need_space = 0;
    آخرین_space = 1;
    sharp_comment = 0;
    variable = 0;
    quoted = 0;
    s_quoted = 0;
    d_quoted = 0;

    cf->آرگومانها->nelts = 0;
    b = cf->conf_file->میانگیر;
    dump = cf->conf_file->dump;
    شروع = b->pos;
    شروع_سطر = cf->conf_file->سطر;

    file_اندازه = ngx_file_اندازه(&cf->conf_file->file.info);

    لكل ( ;; ) {

        إذا (b->pos >= b->آخرین) {

            إذا (cf->conf_file->file.جابه_جایی >= file_اندازه) {

                إذا (cf->آرگومانها->nelts > 0 || !آخرین_space) {

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

                أرجع NGX_CONF_FILE_انجامشده;
            }

            len = b->pos - شروع;

            إذا (len == NGX_CONF_میانگیر) {
                cf->conf_file->سطر = شروع_سطر;

                إذا (d_quoted) {
                    ch = '"';

                } وإلا إذا (s_quoted) {
                    ch = '\'';

                } وإلا {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (اندازه_t) 10, شروع);
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

            اندازه = (ssize_t) (file_اندازه - cf->conf_file->file.جابه_جایی);

            إذا (اندازه > b->انتها - (b->شروع + len)) {
                اندازه = b->انتها - (b->شروع + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->شروع + len, اندازه,
                              cf->conf_file->file.جابه_جایی);

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

            b->pos = b->شروع + len;
            b->آخرین = b->pos + n;
            شروع = b->شروع;

            إذا (dump) {
                dump->آخرین = ngx_cpymem(dump->آخرین, b->pos, اندازه);
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
                آخرین_space = 1;
                need_space = 0;
                استمر;
            }

            إذا (ch == ';') {
                أرجع NGX_OK;
            }

            إذا (ch == '{') {
                أرجع NGX_CONF_بلوک_شروع;
            }

            إذا (ch == ')') {
                آخرین_space = 1;
                need_space = 0;

            } وإلا {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                أرجع NGX_ERROR;
            }
        }

        إذا (آخرین_space) {

            شروع = b->pos - 1;
            شروع_سطر = cf->conf_file->سطر;

            إذا (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                استمر;
            }

            بدل (ch) {

            حالة ';':
            حالة '{':
                إذا (cf->آرگومانها->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    أرجع NGX_ERROR;
                }

                إذا (ch == '{') {
                    أرجع NGX_CONF_بلوک_شروع;
                }

                أرجع NGX_OK;

            حالة '}':
                إذا (cf->آرگومانها->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    أرجع NGX_ERROR;
                }

                أرجع NGX_CONF_بلوک_انجامشده;

            حالة '#':
                sharp_comment = 1;
                استمر;

            حالة '\\':
                quoted = 1;
                آخرین_space = 0;
                استمر;

            حالة '"':
                شروع++;
                d_quoted = 1;
                آخرین_space = 0;
                استمر;

            حالة '\'':
                شروع++;
                s_quoted = 1;
                آخرین_space = 0;
                استمر;

            حالة '$':
                variable = 1;
                آخرین_space = 0;
                استمر;

            افتراضي:
                آخرین_space = 0;
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
                    یافت = 1;
                }

            } وإلا إذا (s_quoted) {
                إذا (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    یافت = 1;
                }

            } وإلا إذا (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                آخرین_space = 1;
                یافت = 1;
            }

            إذا (یافت) {
                کلمه = ngx_array_افزودن(cf->آرگومانها);
                إذا (کلمه == عدم) {
                    أرجع NGX_ERROR;
                }

                کلمه->داده = ngx_pnalloc(cf->pool, b->pos - 1 - شروع + 1);
                إذا (کلمه->داده == عدم) {
                    أرجع NGX_ERROR;
                }

                لكل (dst = کلمه->داده, src = شروع, len = 0;
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
                    أرجع NGX_CONF_بلوک_شروع;
                }

                یافت = 0;
            }
        }
    }
}


حرف *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف        *rv;
    ngx_int_t    n;
    ngx_str_t   *مقدار, file, name;
    ngx_glob_t   gl;

    مقدار = cf->آرگومانها->elts;
    file = مقدار[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لگاریتم, 0, "include %s", file.داده);

    إذا (ngx_conf_پر_name(cf->چرخه, &file, 1) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    إذا (strpbrk((حرف *) file.داده, "*?[") == عدم) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لگاریتم, 0, "include %s", file.داده);

        أرجع ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, حجم(ngx_glob_t));

    gl.الگو = file.داده;
    gl.لگاریتم = cf->لگاریتم;
    gl.test = 1;

    إذا (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.داده);
        أرجع NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    لكل ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        إذا (n != NGX_OK) {
            اكسر;
        }

        file.len = name.len++;
        file.داده = ngx_pstrdup(cf->pool, &name);
        إذا (file.داده == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لگاریتم, 0, "include %s", file.داده);

        rv = ngx_conf_parse(cf, &file);

        إذا (rv != NGX_CONF_OK) {
            اكسر;
        }
    }

    ngx_close_glob(&gl);

    أرجع rv;
}


ngx_int_t
ngx_conf_پر_name(ngx_چرخه_t *چرخه, ngx_str_t *name, ngx_uint_t conf_پیشوند)
{
    ngx_str_t  *پیشوند;

    پیشوند = conf_پیشوند ? &چرخه->conf_پیشوند : &چرخه->پیشوند;

    أرجع ngx_get_پر_name(چرخه->pool, پیشوند, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_چرخه_t *چرخه, ngx_str_t *name)
{
    ngx_str_t         پر;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#إذا (NGX_SUPPRESS_WARN)
    ngx_str_null(&پر);
#پایان_اگر

    إذا (name->len) {
        پر = *name;

        إذا (ngx_conf_پر_name(چرخه, &پر, 0) != NGX_OK) {
            أرجع عدم;
        }

        part = &چرخه->open_files.part;
        file = part->elts;

        لكل (i = 0; /* void */ ; i++) {

            إذا (i >= part->nelts) {
                إذا (part->بعدی == عدم) {
                    اكسر;
                }
                part = part->بعدی;
                file = part->elts;
                i = 0;
            }

            إذا (پر.len != file[i].name.len) {
                استمر;
            }

            إذا (ngx_strcmp(پر.داده, file[i].name.داده) == 0) {
                أرجع &file[i];
            }
        }
    }

    file = ngx_list_افزودن(&چرخه->open_files);
    إذا (file == عدم) {
        أرجع عدم;
    }

    إذا (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = پر;

    } وإلا {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = عدم;
    file->داده = عدم;

    أرجع file;
}


ساكن فراغ
ngx_conf_flush_files(ngx_چرخه_t *چرخه)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, چرخه->لگاریتم, 0, "flush files");

    part = &چرخه->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بعدی == عدم) {
                اكسر;
            }
            part = part->بعدی;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].flush) {
            file[i].flush(&file[i], چرخه->لگاریتم);
        }
    }
}


فراغ ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *آخرین;
    va_list  آرگومانها;

    آخرین = errstr + NGX_MAX_CONF_ERRSTR;

    va_شروع(آرگومانها, fmt);
    p = ngx_vslprintf(errstr, آخرین, fmt, آرگومانها);
    va_انتها(آرگومانها);

    إذا (err) {
        p = ngx_log_errno(p, آخرین, err);
    }

    إذا (cf->conf_file == عدم) {
        ngx_log_error(level, cf->لگاریتم, 0, "%*s", p - errstr, errstr);
        أرجع;
    }

    إذا (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->لگاریتم, 0, "%*s in command line",
                      p - errstr, errstr);
        أرجع;
    }

    ngx_log_error(level, cf->لگاریتم, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.داده, cf->conf_file->سطر);
}


حرف *
ngx_conf_مجموعه_flag_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t        *مقدار;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->جابه_جایی);

    إذا (*fp != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    مقدار = cf->آرگومانها->elts;

    إذا (ngx_strcasecmp(مقدار[1].داده, (u_char *) "on") == 0) {
        *fp = 1;

    } وإلا إذا (ngx_strcasecmp(مقدار[1].داده, (u_char *) "off") == 0) {
        *fp = 0;

    } وإلا {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     مقدار[1].داده, cmd->name.داده);
        أرجع NGX_CONF_ERROR;
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, fp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_مجموعه_str_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t        *فیلد, *مقدار;
    ngx_conf_post_t  *post;

    فیلد = (ngx_str_t *) (p + cmd->جابه_جایی);

    إذا (فیلد->داده) {
        أرجع "is duplicate";
    }

    مقدار = cf->آرگومانها->elts;

    *فیلد = مقدار[1];

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, فیلد);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_مجموعه_str_array_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t         *مقدار, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->جابه_جایی);

    إذا (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_str_t));
        إذا (*a == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    s = ngx_array_افزودن(*a);
    إذا (s == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    مقدار = cf->آرگومانها->elts;

    *s = مقدار[1];

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, s);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_مجموعه_keyval_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t         *مقدار;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->جابه_جایی);

    إذا (*a == NGX_CONF_UNSET_PTR || *a == عدم) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_keyval_t));
        إذا (*a == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_افزودن(*a);
    إذا (kv == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    مقدار = cf->آرگومانها->elts;

    kv->کلید = مقدار[1];
    kv->مقدار = مقدار[2];

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, kv);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_مجموعه_رقم_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *مقدار;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->جابه_جایی);

    إذا (*np != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    مقدار = cf->آرگومانها->elts;
    *np = ngx_atoi(مقدار[1].داده, مقدار[1].len);
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
ngx_conf_مجموعه_اندازه_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    اندازه_t           *sp;
    ngx_str_t        *مقدار;
    ngx_conf_post_t  *post;


    sp = (اندازه_t *) (p + cmd->جابه_جایی);
    إذا (*sp != NGX_CONF_UNSET_اندازه) {
        أرجع "is duplicate";
    }

    مقدار = cf->آرگومانها->elts;

    *sp = ngx_parse_اندازه(&مقدار[1]);
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
ngx_conf_مجموعه_off_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    off_t            *op;
    ngx_str_t        *مقدار;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->جابه_جایی);
    إذا (*op != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    مقدار = cf->آرگومانها->elts;

    *op = ngx_parse_جابه_جایی(&مقدار[1]);
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
ngx_conf_مجموعه_msec_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *مقدار;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->جابه_جایی);
    إذا (*msp != NGX_CONF_UNSET_MSEC) {
        أرجع "is duplicate";
    }

    مقدار = cf->آرگومانها->elts;

    *msp = ngx_parse_زمان(&مقدار[1], 0);
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
ngx_conf_مجموعه_sec_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    زمان_t           *sp;
    ngx_str_t        *مقدار;
    ngx_conf_post_t  *post;


    sp = (زمان_t *) (p + cmd->جابه_جایی);
    إذا (*sp != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    مقدار = cf->آرگومانها->elts;

    *sp = ngx_parse_زمان(&مقدار[1], 1);
    إذا (*sp == (زمان_t) NGX_ERROR) {
        أرجع "invalid value";
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, sp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_مجموعه_bufs_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف *p = conf;

    ngx_str_t   *مقدار;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->جابه_جایی);
    إذا (bufs->رقم) {
        أرجع "is duplicate";
    }

    مقدار = cf->آرگومانها->elts;

    bufs->رقم = ngx_atoi(مقدار[1].داده, مقدار[1].len);
    إذا (bufs->رقم == NGX_ERROR || bufs->رقم == 0) {
        أرجع "invalid value";
    }

    bufs->اندازه = ngx_parse_اندازه(&مقدار[2]);
    إذا (bufs->اندازه == (اندازه_t) NGX_ERROR || bufs->اندازه == 0) {
        أرجع "invalid value";
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_مجموعه_enum_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *مقدار;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->جابه_جایی);

    إذا (*np != NGX_CONF_UNSET_UINT) {
        أرجع "is duplicate";
    }

    مقدار = cf->آرگومانها->elts;
    e = cmd->post;

    لكل (i = 0; e[i].name.len != 0; i++) {
        إذا (e[i].name.len != مقدار[1].len
            || ngx_strcasecmp(e[i].name.داده, مقدار[1].داده) != 0)
        {
            استمر;
        }

        *np = e[i].مقدار;

        أرجع NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", مقدار[1].داده);

    أرجع NGX_CONF_ERROR;
}


حرف *
ngx_conf_مجموعه_bitmask_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *مقدار;
    ngx_conf_bitmask_t  *ماسک;


    np = (ngx_uint_t *) (p + cmd->جابه_جایی);
    مقدار = cf->آرگومانها->elts;
    ماسک = cmd->post;

    لكل (i = 1; i < cf->آرگومانها->nelts; i++) {
        لكل (m = 0; ماسک[m].name.len != 0; m++) {

            إذا (ماسک[m].name.len != مقدار[i].len
                || ngx_strcasecmp(ماسک[m].name.داده, مقدار[i].داده) != 0)
            {
                استمر;
            }

            إذا (*np & ماسک[m].ماسک) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", مقدار[i].داده);

            } وإلا {
                *np |= ماسک[m].ماسک;
            }

            اكسر;
        }

        إذا (ماسک[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", مقدار[i].داده);

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

#پایان_اگر


حرف *
ngx_conf_deprecated(ngx_conf_t *cf, فراغ *post, فراغ *داده)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_check_رقم_bounds(ngx_conf_t *cf, فراغ *post, فراغ *داده)
{
    ngx_conf_رقم_bounds_t  *bounds = post;
    ngx_int_t  *np = داده;

    إذا (bounds->بالا == -1) {
        إذا (*np >= bounds->کم) {
            أرجع NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->کم);

        أرجع NGX_CONF_ERROR;
    }

    إذا (*np >= bounds->کم && *np <= bounds->بالا) {
        أرجع NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->کم, bounds->بالا);

    أرجع NGX_CONF_ERROR;
}


حرف *
ngx_conf_check_اندازه_nonzero(ngx_conf_t *cf, فراغ *post, فراغ *داده)
{
    اندازه_t  *sp = داده;

    إذا (*sp == 0) {
        أرجع "value must be nonzero";
    }

    أرجع NGX_CONF_OK;
}
