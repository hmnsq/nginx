
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>

#परिभाषय NGX_CONF_बफर  4096

स्थिर ngx_int_t ngx_conf_सङ्कलय_dump(ngx_conf_t *cf, ngx_str_t *filename);
स्थिर ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t चरमः);
स्थिर ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
स्थिर शून्य ngx_conf_flush_files(ngx_आवृत्तिः_t *आवृत्तिः);


स्थिर ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      रिक्त },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    रिक्त,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    रिक्त,                                  /* init master */
    रिक्त,                                  /* init module */
    रिक्त,                                  /* init process */
    रिक्त,                                  /* init thread */
    रिक्त,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    रिक्त,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

स्थिर ngx_uint_t तर्कः_सङ्ख्या[] = {
    NGX_CONF_NOARGS,
    NGX_CONF_TAKE1,
    NGX_CONF_TAKE2,
    NGX_CONF_TAKE3,
    NGX_CONF_TAKE4,
    NGX_CONF_TAKE5,
    NGX_CONF_TAKE6,
    NGX_CONF_TAKE7
};


ngx_conf_post_t  ngx_conf_आकार_nonzero_post = { ngx_conf_check_आकार_nonzero };


वर्ण *
ngx_conf_param(ngx_conf_t *cf)
{
    वर्ण             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->आवृत्तिः->conf_param;

    यदि (param->len == 0) {
        प्रत्यागम NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, परिमाण(ngx_conf_file_t));

    ngx_memzero(&b, परिमाण(ngx_buf_t));

    b.start = param->दत्त;
    b.pos = param->दत्त;
    b.चरमः = param->दत्त + param->len;
    b.अन्तः = b.चरमः;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.दत्त = रिक्त;
    conf_file.पङ्क्तिः = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->बफर = &b;

    rv = ngx_conf_parse(cf, रिक्त);

    cf->conf_file = रिक्त;

    प्रत्यागम rv;
}


स्थिर ngx_int_t
ngx_conf_सङ्कलय_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             आकार;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_ग्रन्थिः_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_चक्रीयपरीक्षा_32_long(filename->दत्त, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->आवृत्तिः->config_dump_rbtree, filename, hash);

    यदि (sn) {
        cf->conf_file->dump = रिक्त;
        प्रत्यागम NGX_OK;
    }

    p = ngx_pstrdup(cf->आवृत्तिः->pool, filename);
    यदि (p == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    cd = ngx_array_प्रक्षिप(&cf->आवृत्तिः->config_dump);
    यदि (cd == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    आकार = ngx_file_आकार(&cf->conf_file->file.info);

    buf = ngx_create_अस्थायी_buf(cf->आवृत्तिः->pool, (आकार_t) आकार);
    यदि (buf == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    cd->name.दत्त = p;
    cd->name.len = filename->len;
    cd->बफर = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->अस्थायी_pool, परिमाण(ngx_str_ग्रन्थिः_t));
    यदि (sn == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    sn->ग्रन्थिः.कुञ्चिका = hash;
    sn->str = cd->name;

    ngx_rbtree_निवेशय(&cf->आवृत्तिः->config_dump_rbtree, &sn->ग्रन्थिः);

    प्रत्यागम NGX_OK;
}


वर्ण *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    वर्ण             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *पूर्व, conf_file;
    गणना {
        parse_file = 0,
        parse_खण्डम्,
        parse_param
    } type;

#यदि (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    पूर्व = रिक्त;
#यदि_अन्तः

    यदि (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->दत्त, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        यदि (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->दत्त);
            प्रत्यागम NGX_CONF_ERROR;
        }

        पूर्व = cf->conf_file;

        cf->conf_file = &conf_file;

        यदि (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->लघुगणकः, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->दत्त);
        }

        cf->conf_file->बफर = &buf;

        buf.start = ngx_alloc(NGX_CONF_बफर, cf->लघुगणकः);
        यदि (buf.start == रिक्त) {
            गच्छ failed;
        }

        buf.pos = buf.start;
        buf.चरमः = buf.start;
        buf.अन्तः = buf.चरमः + NGX_CONF_बफर;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.दत्त = filename->दत्त;
        cf->conf_file->file.विस्थापनम् = 0;
        cf->conf_file->file.लघुगणकः = cf->लघुगणकः;
        cf->conf_file->पङ्क्तिः = 1;

        type = parse_file;

        यदि (ngx_dump_config
#यदि (NGX_CONTROL_API)
            || ngx_control_api_enabled
#यदि_अन्तः
#यदि (NGX_DEBUG)
            || 1
#यदि_अन्तः
           )
        {
            यदि (ngx_conf_सङ्कलय_dump(cf, filename) != NGX_OK) {
                गच्छ failed;
            }

        } अन्यथा {
            cf->conf_file->dump = रिक्त;
        }

    } अन्यथा यदि (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_खण्डम्;

    } अन्यथा {
        type = parse_param;
    }


    प्रति ( ;; ) {
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

        यदि (rc == NGX_ERROR) {
            गच्छ समाप्तम्;
        }

        यदि (rc == NGX_CONF_खण्डम्_समाप्तम्) {

            यदि (type != parse_खण्डम्) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                गच्छ failed;
            }

            गच्छ समाप्तम्;
        }

        यदि (rc == NGX_CONF_FILE_समाप्तम्) {

            यदि (type == parse_खण्डम्) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                गच्छ failed;
            }

            गच्छ समाप्तम्;
        }

        यदि (rc == NGX_CONF_खण्डम्_START) {

            यदि (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                गच्छ failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        यदि (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            यदि (rc == NGX_CONF_खण्डम्_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                गच्छ failed;
            }

            rv = (*cf->handler)(cf, रिक्त, cf->handler_conf);
            यदि (rv == NGX_CONF_OK) {
                अनुवर्त;
            }

            यदि (rv == NGX_CONF_ERROR) {
                गच्छ failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            गच्छ failed;
        }


        rc = ngx_conf_handler(cf, rc);

        यदि (rc == NGX_ERROR) {
            गच्छ failed;
        }
    }

failed:

    rc = NGX_ERROR;

समाप्तम्:

    यदि (filename) {
        यदि (cf->conf_file->बफर->start) {
            ngx_free(cf->conf_file->बफर->start);
        }

        यदि (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->लघुगणकः, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->दत्त);
            rc = NGX_ERROR;
        }

        cf->conf_file = पूर्व;
    }

    यदि (rc == NGX_ERROR) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    प्रत्यागम NGX_CONF_OK;
}


स्थिर ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t चरमः)
{
    वर्ण           *rv;
    शून्य           *conf, **confp;
    ngx_uint_t      i, प्राप्तम्;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->तर्काः->elts;

    प्राप्तम् = 0;

    प्रति (i = 0; cf->आवृत्तिः->modules[i]; i++) {

        cmd = cf->आवृत्तिः->modules[i]->commands;
        यदि (cmd == रिक्त) {
            अनुवर्त;
        }

        प्रति ( /* void */ ; cmd->name.len; cmd++) {

            यदि (name->len != cmd->name.len) {
                अनुवर्त;
            }

            यदि (ngx_strcmp(name->दत्त, cmd->name.दत्त) != 0) {
                अनुवर्त;
            }

            प्राप्तम् = 1;

            यदि (cf->आवृत्तिः->modules[i]->type != NGX_CONF_MODULE
                && cf->आवृत्तिः->modules[i]->type != cf->module_type)
            {
                अनुवर्त;
            }

            /* is the directive's location right ? */

            यदि (!(cmd->type & cf->cmd_type)) {
                अनुवर्त;
            }

            यदि (!(cmd->type & NGX_CONF_खण्डम्) && चरमः != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->दत्त);
                प्रत्यागम NGX_ERROR;
            }

            यदि ((cmd->type & NGX_CONF_खण्डम्) && चरमः != NGX_CONF_खण्डम्_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->दत्त);
                प्रत्यागम NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            यदि (!(cmd->type & NGX_CONF_ANY)) {

                यदि (cmd->type & NGX_CONF_FLAG) {

                    यदि (cf->तर्काः->nelts != 2) {
                        गच्छ invalid;
                    }

                } अन्यथा यदि (cmd->type & NGX_CONF_1_अधिकम्) {

                    यदि (cf->तर्काः->nelts < 2) {
                        गच्छ invalid;
                    }

                } अन्यथा यदि (cmd->type & NGX_CONF_2_अधिकम्) {

                    यदि (cf->तर्काः->nelts < 3) {
                        गच्छ invalid;
                    }

                } अन्यथा यदि (cf->तर्काः->nelts > NGX_CONF_MAX_तर्काः) {

                    गच्छ invalid;

                } अन्यथा यदि (!(cmd->type & तर्कः_सङ्ख्या[cf->तर्काः->nelts - 1]))
                {
                    गच्छ invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = रिक्त;

            यदि (cmd->type & NGX_प्रत्यक्षम्_CONF) {
                conf = ((शून्य **) cf->ctx)[cf->आवृत्तिः->modules[i]->सूचकांक];

            } अन्यथा यदि (cmd->type & NGX_MAIN_CONF) {
                conf = &(((शून्य **) cf->ctx)[cf->आवृत्तिः->modules[i]->सूचकांक]);

            } अन्यथा यदि (cf->ctx) {
                confp = *(शून्य **) ((वर्ण *) cf->ctx + cmd->conf);

                यदि (confp) {
                    conf = confp[cf->आवृत्तिः->modules[i]->ctx_सूचकांक];
                }
            }

            rv = cmd->समुच्चयः(cf, cmd, conf);

            यदि (rv == NGX_CONF_OK) {
                प्रत्यागम NGX_OK;
            }

            यदि (rv == NGX_CONF_ERROR) {
                प्रत्यागम NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->दत्त, rv);

            प्रत्यागम NGX_ERROR;
        }
    }

    यदि (प्राप्तम्) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->दत्त);

        प्रत्यागम NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->दत्त);

    प्रत्यागम NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->दत्त);

    प्रत्यागम NGX_ERROR;
}


स्थिर ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_आकार;
    आकार_t       len;
    ssize_t      n, आकार;
    ngx_uint_t   प्राप्तम्, need_space, चरमः_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_पङ्क्तिः;
    ngx_str_t   *word;
    ngx_buf_t   *b, *dump;

    प्राप्तम् = 0;
    need_space = 0;
    चरमः_space = 1;
    sharp_comment = 0;
    variable = 0;
    quoted = 0;
    s_quoted = 0;
    d_quoted = 0;

    cf->तर्काः->nelts = 0;
    b = cf->conf_file->बफर;
    dump = cf->conf_file->dump;
    start = b->pos;
    start_पङ्क्तिः = cf->conf_file->पङ्क्तिः;

    file_आकार = ngx_file_आकार(&cf->conf_file->file.info);

    प्रति ( ;; ) {

        यदि (b->pos >= b->चरमः) {

            यदि (cf->conf_file->file.विस्थापनम् >= file_आकार) {

                यदि (cf->तर्काः->nelts > 0 || !चरमः_space) {

                    यदि (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        प्रत्यागम NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    प्रत्यागम NGX_ERROR;
                }

                प्रत्यागम NGX_CONF_FILE_समाप्तम्;
            }

            len = b->pos - start;

            यदि (len == NGX_CONF_बफर) {
                cf->conf_file->पङ्क्तिः = start_पङ्क्तिः;

                यदि (d_quoted) {
                    ch = '"';

                } अन्यथा यदि (s_quoted) {
                    ch = '\'';

                } अन्यथा {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (आकार_t) 10, start);
                    प्रत्यागम NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                प्रत्यागम NGX_ERROR;
            }

            यदि (len) {
                ngx_memmove(b->start, start, len);
            }

            आकार = (ssize_t) (file_आकार - cf->conf_file->file.विस्थापनम्);

            यदि (आकार > b->अन्तः - (b->start + len)) {
                आकार = b->अन्तः - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, आकार,
                              cf->conf_file->file.विस्थापनम्);

            यदि (n == NGX_ERROR) {
                प्रत्यागम NGX_ERROR;
            }

            यदि (n != आकार) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, आकार);
                प्रत्यागम NGX_ERROR;
            }

            b->pos = b->start + len;
            b->चरमः = b->pos + n;
            start = b->start;

            यदि (dump) {
                dump->चरमः = ngx_cpymem(dump->चरमः, b->pos, आकार);
            }
        }

        ch = *b->pos++;

        यदि (ch == LF) {
            cf->conf_file->पङ्क्तिः++;

            यदि (sharp_comment) {
                sharp_comment = 0;
            }
        }

        यदि (sharp_comment) {
            अनुवर्त;
        }

        यदि (quoted) {
            quoted = 0;
            अनुवर्त;
        }

        यदि (need_space) {
            यदि (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                चरमः_space = 1;
                need_space = 0;
                अनुवर्त;
            }

            यदि (ch == ';') {
                प्रत्यागम NGX_OK;
            }

            यदि (ch == '{') {
                प्रत्यागम NGX_CONF_खण्डम्_START;
            }

            यदि (ch == ')') {
                चरमः_space = 1;
                need_space = 0;

            } अन्यथा {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                प्रत्यागम NGX_ERROR;
            }
        }

        यदि (चरमः_space) {

            start = b->pos - 1;
            start_पङ्क्तिः = cf->conf_file->पङ्क्तिः;

            यदि (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                अनुवर्त;
            }

            विकल्प (ch) {

            पक्ष ';':
            पक्ष '{':
                यदि (cf->तर्काः->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    प्रत्यागम NGX_ERROR;
                }

                यदि (ch == '{') {
                    प्रत्यागम NGX_CONF_खण्डम्_START;
                }

                प्रत्यागम NGX_OK;

            पक्ष '}':
                यदि (cf->तर्काः->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    प्रत्यागम NGX_ERROR;
                }

                प्रत्यागम NGX_CONF_खण्डम्_समाप्तम्;

            पक्ष '#':
                sharp_comment = 1;
                अनुवर्त;

            पक्ष '\\':
                quoted = 1;
                चरमः_space = 0;
                अनुवर्त;

            पक्ष '"':
                start++;
                d_quoted = 1;
                चरमः_space = 0;
                अनुवर्त;

            पक्ष '\'':
                start++;
                s_quoted = 1;
                चरमः_space = 0;
                अनुवर्त;

            पक्ष '$':
                variable = 1;
                चरमः_space = 0;
                अनुवर्त;

            स्वभाव:
                चरमः_space = 0;
            }

        } अन्यथा {
            यदि (ch == '{' && variable) {
                अनुवर्त;
            }

            variable = 0;

            यदि (ch == '\\') {
                quoted = 1;
                अनुवर्त;
            }

            यदि (ch == '$') {
                variable = 1;
                अनुवर्त;
            }

            यदि (d_quoted) {
                यदि (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    प्राप्तम् = 1;
                }

            } अन्यथा यदि (s_quoted) {
                यदि (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    प्राप्तम् = 1;
                }

            } अन्यथा यदि (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                चरमः_space = 1;
                प्राप्तम् = 1;
            }

            यदि (प्राप्तम्) {
                word = ngx_array_प्रक्षिप(cf->तर्काः);
                यदि (word == रिक्त) {
                    प्रत्यागम NGX_ERROR;
                }

                word->दत्त = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                यदि (word->दत्त == रिक्त) {
                    प्रत्यागम NGX_ERROR;
                }

                प्रति (dst = word->दत्त, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    यदि (*src == '\\') {
                        विकल्प (src[1]) {
                        पक्ष '"':
                        पक्ष '\'':
                        पक्ष '\\':
                            src++;
                            भङ्ग;

                        पक्ष 't':
                            *dst++ = '\t';
                            src += 2;
                            अनुवर्त;

                        पक्ष 'r':
                            *dst++ = '\r';
                            src += 2;
                            अनुवर्त;

                        पक्ष 'n':
                            *dst++ = '\n';
                            src += 2;
                            अनुवर्त;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                यदि (ch == ';') {
                    प्रत्यागम NGX_OK;
                }

                यदि (ch == '{') {
                    प्रत्यागम NGX_CONF_खण्डम्_START;
                }

                प्राप्तम् = 0;
            }
        }
    }
}


वर्ण *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण        *rv;
    ngx_int_t    n;
    ngx_str_t   *मानम्, file, name;
    ngx_glob_t   gl;

    मानम् = cf->तर्काः->elts;
    file = मानम्[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->लघुगणकः, 0, "include %s", file.दत्त);

    यदि (ngx_conf_पूर्णम्_name(cf->आवृत्तिः, &file, 1) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    यदि (strpbrk((वर्ण *) file.दत्त, "*?[") == रिक्त) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->लघुगणकः, 0, "include %s", file.दत्त);

        प्रत्यागम ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, परिमाण(ngx_glob_t));

    gl.आदर्शः = file.दत्त;
    gl.लघुगणकः = cf->लघुगणकः;
    gl.test = 1;

    यदि (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.दत्त);
        प्रत्यागम NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    प्रति ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        यदि (n != NGX_OK) {
            भङ्ग;
        }

        file.len = name.len++;
        file.दत्त = ngx_pstrdup(cf->pool, &name);
        यदि (file.दत्त == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->लघुगणकः, 0, "include %s", file.दत्त);

        rv = ngx_conf_parse(cf, &file);

        यदि (rv != NGX_CONF_OK) {
            भङ्ग;
        }
    }

    ngx_close_glob(&gl);

    प्रत्यागम rv;
}


ngx_int_t
ngx_conf_पूर्णम्_name(ngx_आवृत्तिः_t *आवृत्तिः, ngx_str_t *name, ngx_uint_t conf_उपसर्गः)
{
    ngx_str_t  *उपसर्गः;

    उपसर्गः = conf_उपसर्गः ? &आवृत्तिः->conf_उपसर्गः : &आवृत्तिः->उपसर्गः;

    प्रत्यागम ngx_get_पूर्णम्_name(आवृत्तिः->pool, उपसर्गः, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_आवृत्तिः_t *आवृत्तिः, ngx_str_t *name)
{
    ngx_str_t         पूर्णम्;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#यदि (NGX_SUPPRESS_WARN)
    ngx_str_null(&पूर्णम्);
#यदि_अन्तः

    यदि (name->len) {
        पूर्णम् = *name;

        यदि (ngx_conf_पूर्णम्_name(आवृत्तिः, &पूर्णम्, 0) != NGX_OK) {
            प्रत्यागम रिक्त;
        }

        part = &आवृत्तिः->open_files.part;
        file = part->elts;

        प्रति (i = 0; /* void */ ; i++) {

            यदि (i >= part->nelts) {
                यदि (part->अग्र == रिक्त) {
                    भङ्ग;
                }
                part = part->अग्र;
                file = part->elts;
                i = 0;
            }

            यदि (पूर्णम्.len != file[i].name.len) {
                अनुवर्त;
            }

            यदि (ngx_strcmp(पूर्णम्.दत्त, file[i].name.दत्त) == 0) {
                प्रत्यागम &file[i];
            }
        }
    }

    file = ngx_list_प्रक्षिप(&आवृत्तिः->open_files);
    यदि (file == रिक्त) {
        प्रत्यागम रिक्त;
    }

    यदि (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = पूर्णम्;

    } अन्यथा {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = रिक्त;
    file->दत्त = रिक्त;

    प्रत्यागम file;
}


स्थिर शून्य
ngx_conf_flush_files(ngx_आवृत्तिः_t *आवृत्तिः)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, आवृत्तिः->लघुगणकः, 0, "flush files");

    part = &आवृत्तिः->open_files.part;
    file = part->elts;

    प्रति (i = 0; /* void */ ; i++) {

        यदि (i >= part->nelts) {
            यदि (part->अग्र == रिक्त) {
                भङ्ग;
            }
            part = part->अग्र;
            file = part->elts;
            i = 0;
        }

        यदि (file[i].flush) {
            file[i].flush(&file[i], आवृत्तिः->लघुगणकः);
        }
    }
}


शून्य ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ध्रुव वर्ण *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *चरमः;
    va_list  तर्काः;

    चरमः = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(तर्काः, fmt);
    p = ngx_vslprintf(errstr, चरमः, fmt, तर्काः);
    va_अन्तः(तर्काः);

    यदि (err) {
        p = ngx_log_errno(p, चरमः, err);
    }

    यदि (cf->conf_file == रिक्त) {
        ngx_log_error(level, cf->लघुगणकः, 0, "%*s", p - errstr, errstr);
        प्रत्यागम;
    }

    यदि (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->लघुगणकः, 0, "%*s in command line",
                      p - errstr, errstr);
        प्रत्यागम;
    }

    ngx_log_error(level, cf->लघुगणकः, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.दत्त, cf->conf_file->पङ्क्तिः);
}


वर्ण *
ngx_conf_समुच्चयः_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_str_t        *मानम्;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->विस्थापनम्);

    यदि (*fp != NGX_CONF_UNSET) {
        प्रत्यागम "is duplicate";
    }

    मानम् = cf->तर्काः->elts;

    यदि (ngx_strcasecmp(मानम्[1].दत्त, (u_char *) "on") == 0) {
        *fp = 1;

    } अन्यथा यदि (ngx_strcasecmp(मानम्[1].दत्त, (u_char *) "off") == 0) {
        *fp = 0;

    } अन्यथा {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     मानम्[1].दत्त, cmd->name.दत्त);
        प्रत्यागम NGX_CONF_ERROR;
    }

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, fp);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_समुच्चयः_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_str_t        *क्षेत्रम्, *मानम्;
    ngx_conf_post_t  *post;

    क्षेत्रम् = (ngx_str_t *) (p + cmd->विस्थापनम्);

    यदि (क्षेत्रम्->दत्त) {
        प्रत्यागम "is duplicate";
    }

    मानम् = cf->तर्काः->elts;

    *क्षेत्रम् = मानम्[1];

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, क्षेत्रम्);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_समुच्चयः_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_str_t         *मानम्, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->विस्थापनम्);

    यदि (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, परिमाण(ngx_str_t));
        यदि (*a == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }
    }

    s = ngx_array_प्रक्षिप(*a);
    यदि (s == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    मानम् = cf->तर्काः->elts;

    *s = मानम्[1];

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, s);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_समुच्चयः_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_str_t         *मानम्;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->विस्थापनम्);

    यदि (*a == NGX_CONF_UNSET_PTR || *a == रिक्त) {
        *a = ngx_array_create(cf->pool, 4, परिमाण(ngx_keyval_t));
        यदि (*a == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_प्रक्षिप(*a);
    यदि (kv == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    मानम् = cf->तर्काः->elts;

    kv->कुञ्चिका = मानम्[1];
    kv->मानम् = मानम्[2];

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, kv);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_समुच्चयः_अङ्कः_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *मानम्;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->विस्थापनम्);

    यदि (*np != NGX_CONF_UNSET) {
        प्रत्यागम "is duplicate";
    }

    मानम् = cf->तर्काः->elts;
    *np = ngx_atoi(मानम्[1].दत्त, मानम्[1].len);
    यदि (*np == NGX_ERROR) {
        प्रत्यागम "invalid number";
    }

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, np);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_समुच्चयः_आकार_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    आकार_t           *sp;
    ngx_str_t        *मानम्;
    ngx_conf_post_t  *post;


    sp = (आकार_t *) (p + cmd->विस्थापनम्);
    यदि (*sp != NGX_CONF_UNSET_आकार) {
        प्रत्यागम "is duplicate";
    }

    मानम् = cf->तर्काः->elts;

    *sp = ngx_parse_आकार(&मानम्[1]);
    यदि (*sp == (आकार_t) NGX_ERROR) {
        प्रत्यागम "invalid value";
    }

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, sp);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_समुच्चयः_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    off_t            *op;
    ngx_str_t        *मानम्;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->विस्थापनम्);
    यदि (*op != NGX_CONF_UNSET) {
        प्रत्यागम "is duplicate";
    }

    मानम् = cf->तर्काः->elts;

    *op = ngx_parse_विस्थापनम्(&मानम्[1]);
    यदि (*op == (off_t) NGX_ERROR) {
        प्रत्यागम "invalid value";
    }

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, op);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_समुच्चयः_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *मानम्;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->विस्थापनम्);
    यदि (*msp != NGX_CONF_UNSET_MSEC) {
        प्रत्यागम "is duplicate";
    }

    मानम् = cf->तर्काः->elts;

    *msp = ngx_parse_time(&मानम्[1], 0);
    यदि (*msp == (ngx_msec_t) NGX_ERROR) {
        प्रत्यागम "invalid value";
    }

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, msp);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_समुच्चयः_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    time_t           *sp;
    ngx_str_t        *मानम्;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->विस्थापनम्);
    यदि (*sp != NGX_CONF_UNSET) {
        प्रत्यागम "is duplicate";
    }

    मानम् = cf->तर्काः->elts;

    *sp = ngx_parse_time(&मानम्[1], 1);
    यदि (*sp == (time_t) NGX_ERROR) {
        प्रत्यागम "invalid value";
    }

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, sp);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_समुच्चयः_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण *p = conf;

    ngx_str_t   *मानम्;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->विस्थापनम्);
    यदि (bufs->अङ्कः) {
        प्रत्यागम "is duplicate";
    }

    मानम् = cf->तर्काः->elts;

    bufs->अङ्कः = ngx_atoi(मानम्[1].दत्त, मानम्[1].len);
    यदि (bufs->अङ्कः == NGX_ERROR || bufs->अङ्कः == 0) {
        प्रत्यागम "invalid value";
    }

    bufs->आकार = ngx_parse_आकार(&मानम्[2]);
    यदि (bufs->आकार == (आकार_t) NGX_ERROR || bufs->आकार == 0) {
        प्रत्यागम "invalid value";
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_समुच्चयः_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *मानम्;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->विस्थापनम्);

    यदि (*np != NGX_CONF_UNSET_UINT) {
        प्रत्यागम "is duplicate";
    }

    मानम् = cf->तर्काः->elts;
    e = cmd->post;

    प्रति (i = 0; e[i].name.len != 0; i++) {
        यदि (e[i].name.len != मानम्[1].len
            || ngx_strcasecmp(e[i].name.दत्त, मानम्[1].दत्त) != 0)
        {
            अनुवर्त;
        }

        *np = e[i].मानम्;

        प्रत्यागम NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", मानम्[1].दत्त);

    प्रत्यागम NGX_CONF_ERROR;
}


वर्ण *
ngx_conf_समुच्चयः_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *मानम्;
    ngx_conf_bitmask_t  *आवरणम्;


    np = (ngx_uint_t *) (p + cmd->विस्थापनम्);
    मानम् = cf->तर्काः->elts;
    आवरणम् = cmd->post;

    प्रति (i = 1; i < cf->तर्काः->nelts; i++) {
        प्रति (m = 0; आवरणम्[m].name.len != 0; m++) {

            यदि (आवरणम्[m].name.len != मानम्[i].len
                || ngx_strcasecmp(आवरणम्[m].name.दत्त, मानम्[i].दत्त) != 0)
            {
                अनुवर्त;
            }

            यदि (*np & आवरणम्[m].आवरणम्) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", मानम्[i].दत्त);

            } अन्यथा {
                *np |= आवरणम्[m].आवरणम्;
            }

            भङ्ग;
        }

        यदि (आवरणम्[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", मानम्[i].दत्त);

            प्रत्यागम NGX_CONF_ERROR;
        }
    }

    प्रत्यागम NGX_CONF_OK;
}


#यदि 0

वर्ण *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    प्रत्यागम "unsupported on this platform";
}

#यदि_अन्तः


वर्ण *
ngx_conf_deprecated(ngx_conf_t *cf, शून्य *post, शून्य *दत्त)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_check_अङ्कः_bounds(ngx_conf_t *cf, शून्य *post, शून्य *दत्त)
{
    ngx_conf_अङ्कः_bounds_t  *bounds = post;
    ngx_int_t  *np = दत्त;

    यदि (bounds->उच्चः == -1) {
        यदि (*np >= bounds->नीचः) {
            प्रत्यागम NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->नीचः);

        प्रत्यागम NGX_CONF_ERROR;
    }

    यदि (*np >= bounds->नीचः && *np <= bounds->उच्चः) {
        प्रत्यागम NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->नीचः, bounds->उच्चः);

    प्रत्यागम NGX_CONF_ERROR;
}


वर्ण *
ngx_conf_check_आकार_nonzero(ngx_conf_t *cf, शून्य *post, शून्य *दत्त)
{
    आकार_t  *sp = दत्त;

    यदि (*sp == 0) {
        प्रत्यागम "value must be nonzero";
    }

    प्रत्यागम NGX_CONF_OK;
}
