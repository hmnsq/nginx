
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#وصف_ڪريو NGX_CONF_بفر  4096

ساكن ngx_int_t ngx_conf_جوڙيو_dump(ngx_conf_t *cf, ngx_str_t *filename);
ساكن ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t آخري);
ساكن ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
ساكن فراغ ngx_conf_flush_files(ngx_چڪر_t *چڪر);


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

ساكن ngx_uint_t دليل_عدد[] = {
    NGX_CONF_NOARGS,
    NGX_CONF_TAKE1,
    NGX_CONF_TAKE2,
    NGX_CONF_TAKE3,
    NGX_CONF_TAKE4,
    NGX_CONF_TAKE5,
    NGX_CONF_TAKE6,
    NGX_CONF_TAKE7
};


ngx_conf_post_t  ngx_conf_سائيز_nonzero_post = { ngx_conf_check_سائيز_nonzero };


حرف *
ngx_conf_param(ngx_conf_t *cf)
{
    حرف             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->چڪر->conf_param;

    إذا (param->len == 0) {
        أرجع NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, حجم(ngx_conf_file_t));

    ngx_memzero(&b, حجم(ngx_buf_t));

    b.شروعات = param->معلومات;
    b.pos = param->معلومات;
    b.آخري = param->معلومات + param->len;
    b.پڇاڙي = b.آخري;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.معلومات = عدم;
    conf_file.سٽ = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->بفر = &b;

    rv = ngx_conf_parse(cf, عدم);

    cf->conf_file = عدم;

    أرجع rv;
}


ساكن ngx_int_t
ngx_conf_جوڙيو_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             سائيز;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_ڳنڍ_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_چڪري_جاچ_32_long(filename->معلومات, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->چڪر->config_dump_rbtree, filename, hash);

    إذا (sn) {
        cf->conf_file->dump = عدم;
        أرجع NGX_OK;
    }

    p = ngx_pstrdup(cf->چڪر->pool, filename);
    إذا (p == عدم) {
        أرجع NGX_ERROR;
    }

    cd = ngx_array_ڌڪو(&cf->چڪر->config_dump);
    إذا (cd == عدم) {
        أرجع NGX_ERROR;
    }

    سائيز = ngx_file_سائيز(&cf->conf_file->file.info);

    buf = ngx_create_عارضي_buf(cf->چڪر->pool, (سائيز_t) سائيز);
    إذا (buf == عدم) {
        أرجع NGX_ERROR;
    }

    cd->name.معلومات = p;
    cd->name.len = filename->len;
    cd->بفر = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->عارضي_pool, حجم(ngx_str_ڳنڍ_t));
    إذا (sn == عدم) {
        أرجع NGX_ERROR;
    }

    sn->ڳنڍ.ڪنجي = hash;
    sn->str = cd->name;

    ngx_rbtree_وجهو(&cf->چڪر->config_dump_rbtree, &sn->ڳنڍ);

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
        parse_بلاڪ,
        parse_param
    } type;

#إذا (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = عدم;
#جيڪڏهن_پورو

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
            ngx_log_error(NGX_LOG_EMERG, cf->لاگرٿم, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->معلومات);
        }

        cf->conf_file->بفر = &buf;

        buf.شروعات = ngx_alloc(NGX_CONF_بفر, cf->لاگرٿم);
        إذا (buf.شروعات == عدم) {
            اذهب failed;
        }

        buf.pos = buf.شروعات;
        buf.آخري = buf.شروعات;
        buf.پڇاڙي = buf.آخري + NGX_CONF_بفر;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.معلومات = filename->معلومات;
        cf->conf_file->file.وٿي = 0;
        cf->conf_file->file.لاگرٿم = cf->لاگرٿم;
        cf->conf_file->سٽ = 1;

        type = parse_file;

        إذا (ngx_dump_config
#إذا (NGX_CONTROL_API)
            || ngx_control_api_enabled
#جيڪڏهن_پورو
#إذا (NGX_DEBUG)
            || 1
#جيڪڏهن_پورو
           )
        {
            إذا (ngx_conf_جوڙيو_dump(cf, filename) != NGX_OK) {
                اذهب failed;
            }

        } وإلا {
            cf->conf_file->dump = عدم;
        }

    } وإلا إذا (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_بلاڪ;

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
            اذهب پورو;
        }

        إذا (rc == NGX_CONF_بلاڪ_پورو) {

            إذا (type != parse_بلاڪ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                اذهب failed;
            }

            اذهب پورو;
        }

        إذا (rc == NGX_CONF_FILE_پورو) {

            إذا (type == parse_بلاڪ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                اذهب failed;
            }

            اذهب پورو;
        }

        إذا (rc == NGX_CONF_بلاڪ_شروعات) {

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

            إذا (rc == NGX_CONF_بلاڪ_شروعات) {
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

پورو:

    إذا (filename) {
        إذا (cf->conf_file->بفر->شروعات) {
            ngx_free(cf->conf_file->بفر->شروعات);
        }

        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لاگرٿم, ngx_errno,
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
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t آخري)
{
    حرف           *rv;
    فراغ           *conf, **confp;
    ngx_uint_t      i, مليو;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->دليلون->elts;

    مليو = 0;

    لكل (i = 0; cf->چڪر->modules[i]; i++) {

        cmd = cf->چڪر->modules[i]->commands;
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

            مليو = 1;

            إذا (cf->چڪر->modules[i]->type != NGX_CONF_MODULE
                && cf->چڪر->modules[i]->type != cf->module_type)
            {
                استمر;
            }

            /* is the directive's location right ? */

            إذا (!(cmd->type & cf->cmd_type)) {
                استمر;
            }

            إذا (!(cmd->type & NGX_CONF_بلاڪ) && آخري != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->معلومات);
                أرجع NGX_ERROR;
            }

            إذا ((cmd->type & NGX_CONF_بلاڪ) && آخري != NGX_CONF_بلاڪ_شروعات) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->معلومات);
                أرجع NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            إذا (!(cmd->type & NGX_CONF_ANY)) {

                إذا (cmd->type & NGX_CONF_FLAG) {

                    إذا (cf->دليلون->nelts != 2) {
                        اذهب invalid;
                    }

                } وإلا إذا (cmd->type & NGX_CONF_1_وڌيڪ) {

                    إذا (cf->دليلون->nelts < 2) {
                        اذهب invalid;
                    }

                } وإلا إذا (cmd->type & NGX_CONF_2_وڌيڪ) {

                    إذا (cf->دليلون->nelts < 3) {
                        اذهب invalid;
                    }

                } وإلا إذا (cf->دليلون->nelts > NGX_CONF_MAX_دليلون) {

                    اذهب invalid;

                } وإلا إذا (!(cmd->type & دليل_عدد[cf->دليلون->nelts - 1]))
                {
                    اذهب invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = عدم;

            إذا (cmd->type & NGX_سڌو_CONF) {
                conf = ((فراغ **) cf->ctx)[cf->چڪر->modules[i]->اشاريو];

            } وإلا إذا (cmd->type & NGX_MAIN_CONF) {
                conf = &(((فراغ **) cf->ctx)[cf->چڪر->modules[i]->اشاريو]);

            } وإلا إذا (cf->ctx) {
                confp = *(فراغ **) ((حرف *) cf->ctx + cmd->conf);

                إذا (confp) {
                    conf = confp[cf->چڪر->modules[i]->ctx_اشاريو];
                }
            }

            rv = cmd->سيٽ(cf, cmd, conf);

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

    إذا (مليو) {
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
    u_char      *شروعات, ch, *src, *dst;
    off_t        file_سائيز;
    سائيز_t       len;
    ssize_t      n, سائيز;
    ngx_uint_t   مليو, need_space, آخري_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, شروعات_سٽ;
    ngx_str_t   *لفظ;
    ngx_buf_t   *b, *dump;

    مليو = 0;
    need_space = 0;
    آخري_space = 1;
    sharp_comment = 0;
    variable = 0;
    quoted = 0;
    s_quoted = 0;
    d_quoted = 0;

    cf->دليلون->nelts = 0;
    b = cf->conf_file->بفر;
    dump = cf->conf_file->dump;
    شروعات = b->pos;
    شروعات_سٽ = cf->conf_file->سٽ;

    file_سائيز = ngx_file_سائيز(&cf->conf_file->file.info);

    لكل ( ;; ) {

        إذا (b->pos >= b->آخري) {

            إذا (cf->conf_file->file.وٿي >= file_سائيز) {

                إذا (cf->دليلون->nelts > 0 || !آخري_space) {

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

                أرجع NGX_CONF_FILE_پورو;
            }

            len = b->pos - شروعات;

            إذا (len == NGX_CONF_بفر) {
                cf->conf_file->سٽ = شروعات_سٽ;

                إذا (d_quoted) {
                    ch = '"';

                } وإلا إذا (s_quoted) {
                    ch = '\'';

                } وإلا {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (سائيز_t) 10, شروعات);
                    أرجع NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                أرجع NGX_ERROR;
            }

            إذا (len) {
                ngx_memmove(b->شروعات, شروعات, len);
            }

            سائيز = (ssize_t) (file_سائيز - cf->conf_file->file.وٿي);

            إذا (سائيز > b->پڇاڙي - (b->شروعات + len)) {
                سائيز = b->پڇاڙي - (b->شروعات + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->شروعات + len, سائيز,
                              cf->conf_file->file.وٿي);

            إذا (n == NGX_ERROR) {
                أرجع NGX_ERROR;
            }

            إذا (n != سائيز) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, سائيز);
                أرجع NGX_ERROR;
            }

            b->pos = b->شروعات + len;
            b->آخري = b->pos + n;
            شروعات = b->شروعات;

            إذا (dump) {
                dump->آخري = ngx_cpymem(dump->آخري, b->pos, سائيز);
            }
        }

        ch = *b->pos++;

        إذا (ch == LF) {
            cf->conf_file->سٽ++;

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
                آخري_space = 1;
                need_space = 0;
                استمر;
            }

            إذا (ch == ';') {
                أرجع NGX_OK;
            }

            إذا (ch == '{') {
                أرجع NGX_CONF_بلاڪ_شروعات;
            }

            إذا (ch == ')') {
                آخري_space = 1;
                need_space = 0;

            } وإلا {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                أرجع NGX_ERROR;
            }
        }

        إذا (آخري_space) {

            شروعات = b->pos - 1;
            شروعات_سٽ = cf->conf_file->سٽ;

            إذا (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                استمر;
            }

            بدل (ch) {

            حالة ';':
            حالة '{':
                إذا (cf->دليلون->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    أرجع NGX_ERROR;
                }

                إذا (ch == '{') {
                    أرجع NGX_CONF_بلاڪ_شروعات;
                }

                أرجع NGX_OK;

            حالة '}':
                إذا (cf->دليلون->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    أرجع NGX_ERROR;
                }

                أرجع NGX_CONF_بلاڪ_پورو;

            حالة '#':
                sharp_comment = 1;
                استمر;

            حالة '\\':
                quoted = 1;
                آخري_space = 0;
                استمر;

            حالة '"':
                شروعات++;
                d_quoted = 1;
                آخري_space = 0;
                استمر;

            حالة '\'':
                شروعات++;
                s_quoted = 1;
                آخري_space = 0;
                استمر;

            حالة '$':
                variable = 1;
                آخري_space = 0;
                استمر;

            افتراضي:
                آخري_space = 0;
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
                    مليو = 1;
                }

            } وإلا إذا (s_quoted) {
                إذا (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    مليو = 1;
                }

            } وإلا إذا (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                آخري_space = 1;
                مليو = 1;
            }

            إذا (مليو) {
                لفظ = ngx_array_ڌڪو(cf->دليلون);
                إذا (لفظ == عدم) {
                    أرجع NGX_ERROR;
                }

                لفظ->معلومات = ngx_pnalloc(cf->pool, b->pos - 1 - شروعات + 1);
                إذا (لفظ->معلومات == عدم) {
                    أرجع NGX_ERROR;
                }

                لكل (dst = لفظ->معلومات, src = شروعات, len = 0;
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
                    أرجع NGX_CONF_بلاڪ_شروعات;
                }

                مليو = 0;
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

    قدر = cf->دليلون->elts;
    file = قدر[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لاگرٿم, 0, "include %s", file.معلومات);

    إذا (ngx_conf_ڀريل_name(cf->چڪر, &file, 1) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    إذا (strpbrk((حرف *) file.معلومات, "*?[") == عدم) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لاگرٿم, 0, "include %s", file.معلومات);

        أرجع ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, حجم(ngx_glob_t));

    gl.نمونو = file.معلومات;
    gl.لاگرٿم = cf->لاگرٿم;
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

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->لاگرٿم, 0, "include %s", file.معلومات);

        rv = ngx_conf_parse(cf, &file);

        إذا (rv != NGX_CONF_OK) {
            اكسر;
        }
    }

    ngx_close_glob(&gl);

    أرجع rv;
}


ngx_int_t
ngx_conf_ڀريل_name(ngx_چڪر_t *چڪر, ngx_str_t *name, ngx_uint_t conf_اڳياڙي)
{
    ngx_str_t  *اڳياڙي;

    اڳياڙي = conf_اڳياڙي ? &چڪر->conf_اڳياڙي : &چڪر->اڳياڙي;

    أرجع ngx_get_ڀريل_name(چڪر->pool, اڳياڙي, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_چڪر_t *چڪر, ngx_str_t *name)
{
    ngx_str_t         ڀريل;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#إذا (NGX_SUPPRESS_WARN)
    ngx_str_null(&ڀريل);
#جيڪڏهن_پورو

    إذا (name->len) {
        ڀريل = *name;

        إذا (ngx_conf_ڀريل_name(چڪر, &ڀريل, 0) != NGX_OK) {
            أرجع عدم;
        }

        part = &چڪر->open_files.part;
        file = part->elts;

        لكل (i = 0; /* void */ ; i++) {

            إذا (i >= part->nelts) {
                إذا (part->ايندڙ == عدم) {
                    اكسر;
                }
                part = part->ايندڙ;
                file = part->elts;
                i = 0;
            }

            إذا (ڀريل.len != file[i].name.len) {
                استمر;
            }

            إذا (ngx_strcmp(ڀريل.معلومات, file[i].name.معلومات) == 0) {
                أرجع &file[i];
            }
        }
    }

    file = ngx_list_ڌڪو(&چڪر->open_files);
    إذا (file == عدم) {
        أرجع عدم;
    }

    إذا (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = ڀريل;

    } وإلا {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = عدم;
    file->معلومات = عدم;

    أرجع file;
}


ساكن فراغ
ngx_conf_flush_files(ngx_چڪر_t *چڪر)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, چڪر->لاگرٿم, 0, "flush files");

    part = &چڪر->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->ايندڙ == عدم) {
                اكسر;
            }
            part = part->ايندڙ;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].flush) {
            file[i].flush(&file[i], چڪر->لاگرٿم);
        }
    }
}


فراغ ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *آخري;
    va_list  دليلون;

    آخري = errstr + NGX_MAX_CONF_ERRSTR;

    va_شروعات(دليلون, fmt);
    p = ngx_vslprintf(errstr, آخري, fmt, دليلون);
    va_پڇاڙي(دليلون);

    إذا (err) {
        p = ngx_log_errno(p, آخري, err);
    }

    إذا (cf->conf_file == عدم) {
        ngx_log_error(level, cf->لاگرٿم, 0, "%*s", p - errstr, errstr);
        أرجع;
    }

    إذا (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->لاگرٿم, 0, "%*s in command line",
                      p - errstr, errstr);
        أرجع;
    }

    ngx_log_error(level, cf->لاگرٿم, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.معلومات, cf->conf_file->سٽ);
}


حرف *
ngx_conf_سيٽ_flag_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t        *قدر;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->وٿي);

    إذا (*fp != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    قدر = cf->دليلون->elts;

    إذا (ngx_strcasecmp(قدر[1].معلومات, (u_char *) "on") == 0) {
        *fp = 1;

    } وإلا إذا (ngx_strcasecmp(قدر[1].معلومات, (u_char *) "off") == 0) {
        *fp = 0;

    } وإلا {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     قدر[1].معلومات, cmd->name.معلومات);
        أرجع NGX_CONF_ERROR;
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, fp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سيٽ_str_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t        *ميدان, *قدر;
    ngx_conf_post_t  *post;

    ميدان = (ngx_str_t *) (p + cmd->وٿي);

    إذا (ميدان->معلومات) {
        أرجع "is duplicate";
    }

    قدر = cf->دليلون->elts;

    *ميدان = قدر[1];

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, ميدان);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سيٽ_str_array_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t         *قدر, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->وٿي);

    إذا (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_str_t));
        إذا (*a == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    s = ngx_array_ڌڪو(*a);
    إذا (s == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    قدر = cf->دليلون->elts;

    *s = قدر[1];

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, s);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سيٽ_keyval_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t         *قدر;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->وٿي);

    إذا (*a == NGX_CONF_UNSET_PTR || *a == عدم) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_keyval_t));
        إذا (*a == عدم) {
            أرجع NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_ڌڪو(*a);
    إذا (kv == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    قدر = cf->دليلون->elts;

    kv->ڪنجي = قدر[1];
    kv->قدر = قدر[2];

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, kv);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سيٽ_انگ_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->وٿي);

    إذا (*np != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    قدر = cf->دليلون->elts;
    *np = ngx_atoi(قدر[1].معلومات, قدر[1].len);
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
ngx_conf_سيٽ_سائيز_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    سائيز_t           *sp;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    sp = (سائيز_t *) (p + cmd->وٿي);
    إذا (*sp != NGX_CONF_UNSET_سائيز) {
        أرجع "is duplicate";
    }

    قدر = cf->دليلون->elts;

    *sp = ngx_parse_سائيز(&قدر[1]);
    إذا (*sp == (سائيز_t) NGX_ERROR) {
        أرجع "invalid value";
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, sp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سيٽ_off_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    off_t            *op;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->وٿي);
    إذا (*op != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    قدر = cf->دليلون->elts;

    *op = ngx_parse_وٿي(&قدر[1]);
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
ngx_conf_سيٽ_msec_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->وٿي);
    إذا (*msp != NGX_CONF_UNSET_MSEC) {
        أرجع "is duplicate";
    }

    قدر = cf->دليلون->elts;

    *msp = ngx_parse_وقت(&قدر[1], 0);
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
ngx_conf_سيٽ_sec_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    وقت_t           *sp;
    ngx_str_t        *قدر;
    ngx_conf_post_t  *post;


    sp = (وقت_t *) (p + cmd->وٿي);
    إذا (*sp != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    قدر = cf->دليلون->elts;

    *sp = ngx_parse_وقت(&قدر[1], 1);
    إذا (*sp == (وقت_t) NGX_ERROR) {
        أرجع "invalid value";
    }

    إذا (cmd->post) {
        post = cmd->post;
        أرجع post->post_handler(cf, post, sp);
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سيٽ_bufs_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف *p = conf;

    ngx_str_t   *قدر;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->وٿي);
    إذا (bufs->انگ) {
        أرجع "is duplicate";
    }

    قدر = cf->دليلون->elts;

    bufs->انگ = ngx_atoi(قدر[1].معلومات, قدر[1].len);
    إذا (bufs->انگ == NGX_ERROR || bufs->انگ == 0) {
        أرجع "invalid value";
    }

    bufs->سائيز = ngx_parse_سائيز(&قدر[2]);
    إذا (bufs->سائيز == (سائيز_t) NGX_ERROR || bufs->سائيز == 0) {
        أرجع "invalid value";
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سيٽ_enum_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *قدر;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->وٿي);

    إذا (*np != NGX_CONF_UNSET_UINT) {
        أرجع "is duplicate";
    }

    قدر = cf->دليلون->elts;
    e = cmd->post;

    لكل (i = 0; e[i].name.len != 0; i++) {
        إذا (e[i].name.len != قدر[1].len
            || ngx_strcasecmp(e[i].name.معلومات, قدر[1].معلومات) != 0)
        {
            استمر;
        }

        *np = e[i].قدر;

        أرجع NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", قدر[1].معلومات);

    أرجع NGX_CONF_ERROR;
}


حرف *
ngx_conf_سيٽ_bitmask_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *قدر;
    ngx_conf_bitmask_t  *نقاب;


    np = (ngx_uint_t *) (p + cmd->وٿي);
    قدر = cf->دليلون->elts;
    نقاب = cmd->post;

    لكل (i = 1; i < cf->دليلون->nelts; i++) {
        لكل (m = 0; نقاب[m].name.len != 0; m++) {

            إذا (نقاب[m].name.len != قدر[i].len
                || ngx_strcasecmp(نقاب[m].name.معلومات, قدر[i].معلومات) != 0)
            {
                استمر;
            }

            إذا (*np & نقاب[m].نقاب) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", قدر[i].معلومات);

            } وإلا {
                *np |= نقاب[m].نقاب;
            }

            اكسر;
        }

        إذا (نقاب[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", قدر[i].معلومات);

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

#جيڪڏهن_پورو


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
ngx_conf_check_انگ_bounds(ngx_conf_t *cf, فراغ *post, فراغ *معلومات)
{
    ngx_conf_انگ_bounds_t  *bounds = post;
    ngx_int_t  *np = معلومات;

    إذا (bounds->مٿاهون == -1) {
        إذا (*np >= bounds->هيٺيون) {
            أرجع NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->هيٺيون);

        أرجع NGX_CONF_ERROR;
    }

    إذا (*np >= bounds->هيٺيون && *np <= bounds->مٿاهون) {
        أرجع NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->هيٺيون, bounds->مٿاهون);

    أرجع NGX_CONF_ERROR;
}


حرف *
ngx_conf_check_سائيز_nonzero(ngx_conf_t *cf, فراغ *post, فراغ *معلومات)
{
    سائيز_t  *sp = معلومات;

    إذا (*sp == 0) {
        أرجع "value must be nonzero";
    }

    أرجع NGX_CONF_OK;
}
