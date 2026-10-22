
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>

#define NGX_CONF_বাফার  4096

স্থির ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
স্থির ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
স্থির ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
স্থির শূন্য ngx_conf_flush_files(ngx_cycle_t *cycle);


স্থির ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      ফাঁকা },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    ফাঁকা,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    ফাঁকা,                                  /* init master */
    ফাঁকা,                                  /* init module */
    ফাঁকা,                                  /* init process */
    ফাঁকা,                                  /* init thread */
    ফাঁকা,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    ফাঁকা,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

স্থির ngx_uint_t argument_সংখ্যা[] = {
    NGX_CONF_NOARGS,
    NGX_CONF_TAKE1,
    NGX_CONF_TAKE2,
    NGX_CONF_TAKE3,
    NGX_CONF_TAKE4,
    NGX_CONF_TAKE5,
    NGX_CONF_TAKE6,
    NGX_CONF_TAKE7
};


ngx_conf_post_t  ngx_conf_আকার_nonzero_post = { ngx_conf_check_আকার_nonzero };


অক্ষর *
ngx_conf_param(ngx_conf_t *cf)
{
    অক্ষর             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    যদি (param->len == 0) {
        ফেরত NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, আকার(ngx_conf_file_t));

    ngx_memzero(&b, আকার(ngx_buf_t));

    b.start = param->তথ্য;
    b.pos = param->তথ্য;
    b.last = param->তথ্য + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.তথ্য = ফাঁকা;
    conf_file.সারি = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->বাফার = &b;

    rv = ngx_conf_parse(cf, ফাঁকা);

    cf->conf_file = ফাঁকা;

    ফেরত rv;
}


স্থির ngx_int_t
ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             আকার_2;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_গ্রন্থি_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_crc32_long(filename->তথ্য, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->cycle->config_dump_rbtree, filename, hash);

    যদি (sn) {
        cf->conf_file->dump = ফাঁকা;
        ফেরত NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    যদি (p == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    cd = ngx_array_ঠেলো(&cf->cycle->config_dump);
    যদি (cd == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    আকার_2 = ngx_file_আকার(&cf->conf_file->file.info);

    buf = ngx_create_অস্থায়ী_buf(cf->cycle->pool, (আকার_t) আকার_2);
    যদি (buf == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    cd->name.তথ্য = p;
    cd->name.len = filename->len;
    cd->বাফার = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->অস্থায়ী_pool, আকার(ngx_str_গ্রন্থি_t));
    যদি (sn == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    sn->গ্রন্থি.চাবি = hash;
    sn->str = cd->name;

    ngx_rbtree_ঢোকাও(&cf->cycle->config_dump_rbtree, &sn->গ্রন্থি);

    ফেরত NGX_OK;
}


অক্ষর *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    অক্ষর             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *পূর্ব, conf_file;
    গণনা {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#যদি (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    পূর্ব = ফাঁকা;
#endif

    যদি (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->তথ্য, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        যদি (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->তথ্য);
            ফেরত NGX_CONF_ERROR;
        }

        পূর্ব = cf->conf_file;

        cf->conf_file = &conf_file;

        যদি (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->তথ্য);
        }

        cf->conf_file->বাফার = &buf;

        buf.start = ngx_alloc(NGX_CONF_বাফার, cf->log);
        যদি (buf.start == ফাঁকা) {
            যাও failed;
        }

        buf.pos = buf.start;
        buf.last = buf.start;
        buf.end = buf.last + NGX_CONF_বাফার;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.তথ্য = filename->তথ্য;
        cf->conf_file->file.offset = 0;
        cf->conf_file->file.log = cf->log;
        cf->conf_file->সারি = 1;

        type = parse_file;

        যদি (ngx_dump_config
#যদি (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#যদি (NGX_DEBUG)
            || 1
#endif
           )
        {
            যদি (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                যাও failed;
            }

        } অন্যথা {
            cf->conf_file->dump = ফাঁকা;
        }

    } অন্যথা যদি (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } অন্যথা {
        type = parse_param;
    }


    জন্য ( ;; ) {
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

        যদি (rc == NGX_ERROR) {
            যাও done;
        }

        যদি (rc == NGX_CONF_BLOCK_DONE) {

            যদি (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                যাও failed;
            }

            যাও done;
        }

        যদি (rc == NGX_CONF_FILE_DONE) {

            যদি (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                যাও failed;
            }

            যাও done;
        }

        যদি (rc == NGX_CONF_BLOCK_START) {

            যদি (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                যাও failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        যদি (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            যদি (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                যাও failed;
            }

            rv = (*cf->handler)(cf, ফাঁকা, cf->handler_conf);
            যদি (rv == NGX_CONF_OK) {
                চালু;
            }

            যদি (rv == NGX_CONF_ERROR) {
                যাও failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            যাও failed;
        }


        rc = ngx_conf_handler(cf, rc);

        যদি (rc == NGX_ERROR) {
            যাও failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    যদি (filename) {
        যদি (cf->conf_file->বাফার->start) {
            ngx_free(cf->conf_file->বাফার->start);
        }

        যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->তথ্য);
            rc = NGX_ERROR;
        }

        cf->conf_file = পূর্ব;
    }

    যদি (rc == NGX_ERROR) {
        ফেরত NGX_CONF_ERROR;
    }

    ফেরত NGX_CONF_OK;
}


স্থির ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    অক্ষর           *rv;
    শূন্য           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    জন্য (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        যদি (cmd == ফাঁকা) {
            চালু;
        }

        জন্য ( /* void */ ; cmd->name.len; cmd++) {

            যদি (name->len != cmd->name.len) {
                চালু;
            }

            যদি (ngx_strcmp(name->তথ্য, cmd->name.তথ্য) != 0) {
                চালু;
            }

            found = 1;

            যদি (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                চালু;
            }

            /* is the directive's location right ? */

            যদি (!(cmd->type & cf->cmd_type)) {
                চালু;
            }

            যদি (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->তথ্য);
                ফেরত NGX_ERROR;
            }

            যদি ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->তথ্য);
                ফেরত NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            যদি (!(cmd->type & NGX_CONF_ANY)) {

                যদি (cmd->type & NGX_CONF_FLAG) {

                    যদি (cf->args->nelts != 2) {
                        যাও invalid;
                    }

                } অন্যথা যদি (cmd->type & NGX_CONF_1MORE) {

                    যদি (cf->args->nelts < 2) {
                        যাও invalid;
                    }

                } অন্যথা যদি (cmd->type & NGX_CONF_2MORE) {

                    যদি (cf->args->nelts < 3) {
                        যাও invalid;
                    }

                } অন্যথা যদি (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    যাও invalid;

                } অন্যথা যদি (!(cmd->type & argument_সংখ্যা[cf->args->nelts - 1]))
                {
                    যাও invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = ফাঁকা;

            যদি (cmd->type & NGX_DIRECT_CONF) {
                conf = ((শূন্য **) cf->ctx)[cf->cycle->modules[i]->সূচক];

            } অন্যথা যদি (cmd->type & NGX_MAIN_CONF) {
                conf = &(((শূন্য **) cf->ctx)[cf->cycle->modules[i]->সূচক]);

            } অন্যথা যদি (cf->ctx) {
                confp = *(শূন্য **) ((অক্ষর *) cf->ctx + cmd->conf);

                যদি (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_সূচক];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            যদি (rv == NGX_CONF_OK) {
                ফেরত NGX_OK;
            }

            যদি (rv == NGX_CONF_ERROR) {
                ফেরত NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->তথ্য, rv);

            ফেরত NGX_ERROR;
        }
    }

    যদি (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->তথ্য);

        ফেরত NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->তথ্য);

    ফেরত NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->তথ্য);

    ফেরত NGX_ERROR;
}


স্থির ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_আকার;
    আকার_t       len;
    ssize_t      n, আকার_2;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_সারি;
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
    b = cf->conf_file->বাফার;
    dump = cf->conf_file->dump;
    start = b->pos;
    start_সারি = cf->conf_file->সারি;

    file_আকার = ngx_file_আকার(&cf->conf_file->file.info);

    জন্য ( ;; ) {

        যদি (b->pos >= b->last) {

            যদি (cf->conf_file->file.offset >= file_আকার) {

                যদি (cf->args->nelts > 0 || !last_space) {

                    যদি (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        ফেরত NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    ফেরত NGX_ERROR;
                }

                ফেরত NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            যদি (len == NGX_CONF_বাফার) {
                cf->conf_file->সারি = start_সারি;

                যদি (d_quoted) {
                    ch = '"';

                } অন্যথা যদি (s_quoted) {
                    ch = '\'';

                } অন্যথা {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (আকার_t) 10, start);
                    ফেরত NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                ফেরত NGX_ERROR;
            }

            যদি (len) {
                ngx_memmove(b->start, start, len);
            }

            আকার_2 = (ssize_t) (file_আকার - cf->conf_file->file.offset);

            যদি (আকার_2 > b->end - (b->start + len)) {
                আকার_2 = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, আকার_2,
                              cf->conf_file->file.offset);

            যদি (n == NGX_ERROR) {
                ফেরত NGX_ERROR;
            }

            যদি (n != আকার_2) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, আকার_2);
                ফেরত NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            যদি (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, আকার_2);
            }
        }

        ch = *b->pos++;

        যদি (ch == LF) {
            cf->conf_file->সারি++;

            যদি (sharp_comment) {
                sharp_comment = 0;
            }
        }

        যদি (sharp_comment) {
            চালু;
        }

        যদি (quoted) {
            quoted = 0;
            চালু;
        }

        যদি (need_space) {
            যদি (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                চালু;
            }

            যদি (ch == ';') {
                ফেরত NGX_OK;
            }

            যদি (ch == '{') {
                ফেরত NGX_CONF_BLOCK_START;
            }

            যদি (ch == ')') {
                last_space = 1;
                need_space = 0;

            } অন্যথা {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                ফেরত NGX_ERROR;
            }
        }

        যদি (last_space) {

            start = b->pos - 1;
            start_সারি = cf->conf_file->সারি;

            যদি (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                চালু;
            }

            নির্বাচন (ch) {

            ক্ষেত্র ';':
            ক্ষেত্র '{':
                যদি (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    ফেরত NGX_ERROR;
                }

                যদি (ch == '{') {
                    ফেরত NGX_CONF_BLOCK_START;
                }

                ফেরত NGX_OK;

            ক্ষেত্র '}':
                যদি (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    ফেরত NGX_ERROR;
                }

                ফেরত NGX_CONF_BLOCK_DONE;

            ক্ষেত্র '#':
                sharp_comment = 1;
                চালু;

            ক্ষেত্র '\\':
                quoted = 1;
                last_space = 0;
                চালু;

            ক্ষেত্র '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                চালু;

            ক্ষেত্র '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                চালু;

            ক্ষেত্র '$':
                variable = 1;
                last_space = 0;
                চালু;

            পূর্বনির্ধারিত:
                last_space = 0;
            }

        } অন্যথা {
            যদি (ch == '{' && variable) {
                চালু;
            }

            variable = 0;

            যদি (ch == '\\') {
                quoted = 1;
                চালু;
            }

            যদি (ch == '$') {
                variable = 1;
                চালু;
            }

            যদি (d_quoted) {
                যদি (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } অন্যথা যদি (s_quoted) {
                যদি (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } অন্যথা যদি (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            যদি (found) {
                word = ngx_array_ঠেলো(cf->args);
                যদি (word == ফাঁকা) {
                    ফেরত NGX_ERROR;
                }

                word->তথ্য = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                যদি (word->তথ্য == ফাঁকা) {
                    ফেরত NGX_ERROR;
                }

                জন্য (dst = word->তথ্য, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    যদি (*src == '\\') {
                        নির্বাচন (src[1]) {
                        ক্ষেত্র '"':
                        ক্ষেত্র '\'':
                        ক্ষেত্র '\\':
                            src++;
                            বিরতি;

                        ক্ষেত্র 't':
                            *dst++ = '\t';
                            src += 2;
                            চালু;

                        ক্ষেত্র 'r':
                            *dst++ = '\r';
                            src += 2;
                            চালু;

                        ক্ষেত্র 'n':
                            *dst++ = '\n';
                            src += 2;
                            চালু;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                যদি (ch == ';') {
                    ফেরত NGX_OK;
                }

                যদি (ch == '{') {
                    ফেরত NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


অক্ষর *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর        *rv;
    ngx_int_t    n;
    ngx_str_t   *মান, file, name;
    ngx_glob_t   gl;

    মান = cf->args->elts;
    file = মান[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.তথ্য);

    যদি (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        ফেরত NGX_CONF_ERROR;
    }

    যদি (strpbrk((অক্ষর *) file.তথ্য, "*?[") == ফাঁকা) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.তথ্য);

        ফেরত ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, আকার(ngx_glob_t));

    gl.pattern = file.তথ্য;
    gl.log = cf->log;
    gl.test = 1;

    যদি (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.তথ্য);
        ফেরত NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    জন্য ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        যদি (n != NGX_OK) {
            বিরতি;
        }

        file.len = name.len++;
        file.তথ্য = ngx_pstrdup(cf->pool, &name);
        যদি (file.তথ্য == ফাঁকা) {
            ফেরত NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.তথ্য);

        rv = ngx_conf_parse(cf, &file);

        যদি (rv != NGX_CONF_OK) {
            বিরতি;
        }
    }

    ngx_close_glob(&gl);

    ফেরত rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    ফেরত ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#যদি (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    যদি (name->len) {
        full = *name;

        যদি (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            ফেরত ফাঁকা;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        জন্য (i = 0; /* void */ ; i++) {

            যদি (i >= part->nelts) {
                যদি (part->পরবর্তী == ফাঁকা) {
                    বিরতি;
                }
                part = part->পরবর্তী;
                file = part->elts;
                i = 0;
            }

            যদি (full.len != file[i].name.len) {
                চালু;
            }

            যদি (ngx_strcmp(full.তথ্য, file[i].name.তথ্য) == 0) {
                ফেরত &file[i];
            }
        }
    }

    file = ngx_list_ঠেলো(&cycle->open_files);
    যদি (file == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    যদি (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } অন্যথা {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = ফাঁকা;
    file->তথ্য = ফাঁকা;

    ফেরত file;
}


স্থির শূন্য
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    জন্য (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->পরবর্তী == ফাঁকা) {
                বিরতি;
            }
            part = part->পরবর্তী;
            file = part->elts;
            i = 0;
        }

        যদি (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


শূন্য ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ধ্রুবক অক্ষর *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    va_list  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    va_end(args);

    যদি (err) {
        p = ngx_log_errno(p, last, err);
    }

    যদি (cf->conf_file == ফাঁকা) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        ফেরত;
    }

    যদি (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        ফেরত;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.তথ্য, cf->conf_file->সারি);
}


অক্ষর *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর  *p = conf;

    ngx_str_t        *মান;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    যদি (*fp != NGX_CONF_UNSET) {
        ফেরত "is duplicate";
    }

    মান = cf->args->elts;

    যদি (ngx_strcasecmp(মান[1].তথ্য, (u_char *) "on") == 0) {
        *fp = 1;

    } অন্যথা যদি (ngx_strcasecmp(মান[1].তথ্য, (u_char *) "off") == 0) {
        *fp = 0;

    } অন্যথা {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     মান[1].তথ্য, cmd->name.তথ্য);
        ফেরত NGX_CONF_ERROR;
    }

    যদি (cmd->post) {
        post = cmd->post;
        ফেরত post->post_handler(cf, post, fp);
    }

    ফেরত NGX_CONF_OK;
}


অক্ষর *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর  *p = conf;

    ngx_str_t        *field, *মান;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    যদি (field->তথ্য) {
        ফেরত "is duplicate";
    }

    মান = cf->args->elts;

    *field = মান[1];

    যদি (cmd->post) {
        post = cmd->post;
        ফেরত post->post_handler(cf, post, field);
    }

    ফেরত NGX_CONF_OK;
}


অক্ষর *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর  *p = conf;

    ngx_str_t         *মান, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    যদি (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, আকার(ngx_str_t));
        যদি (*a == ফাঁকা) {
            ফেরত NGX_CONF_ERROR;
        }
    }

    s = ngx_array_ঠেলো(*a);
    যদি (s == ফাঁকা) {
        ফেরত NGX_CONF_ERROR;
    }

    মান = cf->args->elts;

    *s = মান[1];

    যদি (cmd->post) {
        post = cmd->post;
        ফেরত post->post_handler(cf, post, s);
    }

    ফেরত NGX_CONF_OK;
}


অক্ষর *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর  *p = conf;

    ngx_str_t         *মান;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    যদি (*a == NGX_CONF_UNSET_PTR || *a == ফাঁকা) {
        *a = ngx_array_create(cf->pool, 4, আকার(ngx_keyval_t));
        যদি (*a == ফাঁকা) {
            ফেরত NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_ঠেলো(*a);
    যদি (kv == ফাঁকা) {
        ফেরত NGX_CONF_ERROR;
    }

    মান = cf->args->elts;

    kv->চাবি = মান[1];
    kv->মান = মান[2];

    যদি (cmd->post) {
        post = cmd->post;
        ফেরত post->post_handler(cf, post, kv);
    }

    ফেরত NGX_CONF_OK;
}


অক্ষর *
ngx_conf_set_অঙ্ক_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *মান;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    যদি (*np != NGX_CONF_UNSET) {
        ফেরত "is duplicate";
    }

    মান = cf->args->elts;
    *np = ngx_atoi(মান[1].তথ্য, মান[1].len);
    যদি (*np == NGX_ERROR) {
        ফেরত "invalid number";
    }

    যদি (cmd->post) {
        post = cmd->post;
        ফেরত post->post_handler(cf, post, np);
    }

    ফেরত NGX_CONF_OK;
}


অক্ষর *
ngx_conf_set_আকার_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর  *p = conf;

    আকার_t           *sp;
    ngx_str_t        *মান;
    ngx_conf_post_t  *post;


    sp = (আকার_t *) (p + cmd->offset);
    যদি (*sp != NGX_CONF_UNSET_আকার) {
        ফেরত "is duplicate";
    }

    মান = cf->args->elts;

    *sp = ngx_parse_আকার(&মান[1]);
    যদি (*sp == (আকার_t) NGX_ERROR) {
        ফেরত "invalid value";
    }

    যদি (cmd->post) {
        post = cmd->post;
        ফেরত post->post_handler(cf, post, sp);
    }

    ফেরত NGX_CONF_OK;
}


অক্ষর *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর  *p = conf;

    off_t            *op;
    ngx_str_t        *মান;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    যদি (*op != NGX_CONF_UNSET) {
        ফেরত "is duplicate";
    }

    মান = cf->args->elts;

    *op = ngx_parse_offset(&মান[1]);
    যদি (*op == (off_t) NGX_ERROR) {
        ফেরত "invalid value";
    }

    যদি (cmd->post) {
        post = cmd->post;
        ফেরত post->post_handler(cf, post, op);
    }

    ফেরত NGX_CONF_OK;
}


অক্ষর *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *মান;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    যদি (*msp != NGX_CONF_UNSET_MSEC) {
        ফেরত "is duplicate";
    }

    মান = cf->args->elts;

    *msp = ngx_parse_time(&মান[1], 0);
    যদি (*msp == (ngx_msec_t) NGX_ERROR) {
        ফেরত "invalid value";
    }

    যদি (cmd->post) {
        post = cmd->post;
        ফেরত post->post_handler(cf, post, msp);
    }

    ফেরত NGX_CONF_OK;
}


অক্ষর *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর  *p = conf;

    time_t           *sp;
    ngx_str_t        *মান;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    যদি (*sp != NGX_CONF_UNSET) {
        ফেরত "is duplicate";
    }

    মান = cf->args->elts;

    *sp = ngx_parse_time(&মান[1], 1);
    যদি (*sp == (time_t) NGX_ERROR) {
        ফেরত "invalid value";
    }

    যদি (cmd->post) {
        post = cmd->post;
        ফেরত post->post_handler(cf, post, sp);
    }

    ফেরত NGX_CONF_OK;
}


অক্ষর *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর *p = conf;

    ngx_str_t   *মান;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    যদি (bufs->অঙ্ক) {
        ফেরত "is duplicate";
    }

    মান = cf->args->elts;

    bufs->অঙ্ক = ngx_atoi(মান[1].তথ্য, মান[1].len);
    যদি (bufs->অঙ্ক == NGX_ERROR || bufs->অঙ্ক == 0) {
        ফেরত "invalid value";
    }

    bufs->আকার_2 = ngx_parse_আকার(&মান[2]);
    যদি (bufs->আকার_2 == (আকার_t) NGX_ERROR || bufs->আকার_2 == 0) {
        ফেরত "invalid value";
    }

    ফেরত NGX_CONF_OK;
}


অক্ষর *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *মান;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    যদি (*np != NGX_CONF_UNSET_UINT) {
        ফেরত "is duplicate";
    }

    মান = cf->args->elts;
    e = cmd->post;

    জন্য (i = 0; e[i].name.len != 0; i++) {
        যদি (e[i].name.len != মান[1].len
            || ngx_strcasecmp(e[i].name.তথ্য, মান[1].তথ্য) != 0)
        {
            চালু;
        }

        *np = e[i].মান;

        ফেরত NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", মান[1].তথ্য);

    ফেরত NGX_CONF_ERROR;
}


অক্ষর *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *মান;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    মান = cf->args->elts;
    mask = cmd->post;

    জন্য (i = 1; i < cf->args->nelts; i++) {
        জন্য (m = 0; mask[m].name.len != 0; m++) {

            যদি (mask[m].name.len != মান[i].len
                || ngx_strcasecmp(mask[m].name.তথ্য, মান[i].তথ্য) != 0)
            {
                চালু;
            }

            যদি (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", মান[i].তথ্য);

            } অন্যথা {
                *np |= mask[m].mask;
            }

            বিরতি;
        }

        যদি (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", মান[i].তথ্য);

            ফেরত NGX_CONF_ERROR;
        }
    }

    ফেরত NGX_CONF_OK;
}


#যদি 0

অক্ষর *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    ফেরত "unsupported on this platform";
}

#endif


অক্ষর *
ngx_conf_deprecated(ngx_conf_t *cf, শূন্য *post, শূন্য *তথ্য)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    ফেরত NGX_CONF_OK;
}


অক্ষর *
ngx_conf_check_অঙ্ক_bounds(ngx_conf_t *cf, শূন্য *post, শূন্য *তথ্য)
{
    ngx_conf_অঙ্ক_bounds_t  *bounds = post;
    ngx_int_t  *np = তথ্য;

    যদি (bounds->উপরের == -1) {
        যদি (*np >= bounds->নিচের) {
            ফেরত NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->নিচের);

        ফেরত NGX_CONF_ERROR;
    }

    যদি (*np >= bounds->নিচের && *np <= bounds->উপরের) {
        ফেরত NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->নিচের, bounds->উপরের);

    ফেরত NGX_CONF_ERROR;
}


অক্ষর *
ngx_conf_check_আকার_nonzero(ngx_conf_t *cf, শূন্য *post, শূন্য *তথ্য)
{
    আকার_t  *sp = তথ্য;

    যদি (*sp == 0) {
        ফেরত "value must be nonzero";
    }

    ফেরত NGX_CONF_OK;
}
