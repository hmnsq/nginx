
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>

#घोषणा NGX_CONF_बफर  4096

अटल ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
अटल ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
अटल ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
अटल व्योम ngx_conf_flush_files(ngx_cycle_t *cycle);


अटल ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      शून्य },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    शून्य,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    शून्य,                                  /* init master */
    शून्य,                                  /* init module */
    शून्य,                                  /* init process */
    शून्य,                                  /* init thread */
    शून्य,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    शून्य,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

अटल ngx_uint_t argument_संख्या[] = {
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


अक्षर *
ngx_conf_param(ngx_conf_t *cf)
{
    अक्षर             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    अगर (param->len == 0) {
        वापस NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, माप(ngx_conf_file_t));

    ngx_memzero(&b, माप(ngx_buf_t));

    b.start = param->तथ्य;
    b.pos = param->तथ्य;
    b.last = param->तथ्य + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.तथ्य = शून्य;
    conf_file.पंक्ति = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->बफर = &b;

    rv = ngx_conf_parse(cf, शून्य);

    cf->conf_file = शून्य;

    वापस rv;
}


अटल ngx_int_t
ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             आकार;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_गाँठ_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_crc32_long(filename->तथ्य, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->cycle->config_dump_rbtree, filename, hash);

    अगर (sn) {
        cf->conf_file->dump = शून्य;
        वापस NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    अगर (p == शून्य) {
        वापस NGX_ERROR;
    }

    cd = ngx_array_धकेलो(&cf->cycle->config_dump);
    अगर (cd == शून्य) {
        वापस NGX_ERROR;
    }

    आकार = ngx_file_आकार(&cf->conf_file->file.info);

    buf = ngx_create_अस्थायी_buf(cf->cycle->pool, (माप_प्रकार) आकार);
    अगर (buf == शून्य) {
        वापस NGX_ERROR;
    }

    cd->name.तथ्य = p;
    cd->name.len = filename->len;
    cd->बफर = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->अस्थायी_pool, माप(ngx_str_गाँठ_t));
    अगर (sn == शून्य) {
        वापस NGX_ERROR;
    }

    sn->गाँठ.कुंजी = hash;
    sn->str = cd->name;

    ngx_rbtree_जोड़ो(&cf->cycle->config_dump_rbtree, &sn->गाँठ);

    वापस NGX_OK;
}


अक्षर *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    अक्षर             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *पूर्व, conf_file;
    क्रमागत {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#अगर (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    पूर्व = शून्य;
#पूर्ण_अगर

    अगर (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->तथ्य, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        अगर (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->तथ्य);
            वापस NGX_CONF_ERROR;
        }

        पूर्व = cf->conf_file;

        cf->conf_file = &conf_file;

        अगर (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->तथ्य);
        }

        cf->conf_file->बफर = &buf;

        buf.start = ngx_alloc(NGX_CONF_बफर, cf->log);
        अगर (buf.start == शून्य) {
            जाओ failed;
        }

        buf.pos = buf.start;
        buf.last = buf.start;
        buf.end = buf.last + NGX_CONF_बफर;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.तथ्य = filename->तथ्य;
        cf->conf_file->file.offset = 0;
        cf->conf_file->file.log = cf->log;
        cf->conf_file->पंक्ति = 1;

        type = parse_file;

        अगर (ngx_dump_config
#अगर (NGX_CONTROL_API)
            || ngx_control_api_enabled
#पूर्ण_अगर
#अगर (NGX_DEBUG)
            || 1
#पूर्ण_अगर
           )
        {
            अगर (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                जाओ failed;
            }

        } अन्यथा {
            cf->conf_file->dump = शून्य;
        }

    } अन्यथा अगर (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } अन्यथा {
        type = parse_param;
    }


    क्रम ( ;; ) {
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

        अगर (rc == NGX_ERROR) {
            जाओ done;
        }

        अगर (rc == NGX_CONF_BLOCK_DONE) {

            अगर (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                जाओ failed;
            }

            जाओ done;
        }

        अगर (rc == NGX_CONF_FILE_DONE) {

            अगर (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                जाओ failed;
            }

            जाओ done;
        }

        अगर (rc == NGX_CONF_BLOCK_START) {

            अगर (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                जाओ failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        अगर (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            अगर (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                जाओ failed;
            }

            rv = (*cf->handler)(cf, शून्य, cf->handler_conf);
            अगर (rv == NGX_CONF_OK) {
                जारी;
            }

            अगर (rv == NGX_CONF_ERROR) {
                जाओ failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            जाओ failed;
        }


        rc = ngx_conf_handler(cf, rc);

        अगर (rc == NGX_ERROR) {
            जाओ failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    अगर (filename) {
        अगर (cf->conf_file->बफर->start) {
            ngx_free(cf->conf_file->बफर->start);
        }

        अगर (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->तथ्य);
            rc = NGX_ERROR;
        }

        cf->conf_file = पूर्व;
    }

    अगर (rc == NGX_ERROR) {
        वापस NGX_CONF_ERROR;
    }

    वापस NGX_CONF_OK;
}


अटल ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    अक्षर           *rv;
    व्योम           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    क्रम (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        अगर (cmd == शून्य) {
            जारी;
        }

        क्रम ( /* void */ ; cmd->name.len; cmd++) {

            अगर (name->len != cmd->name.len) {
                जारी;
            }

            अगर (ngx_strcmp(name->तथ्य, cmd->name.तथ्य) != 0) {
                जारी;
            }

            found = 1;

            अगर (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                जारी;
            }

            /* is the directive's location right ? */

            अगर (!(cmd->type & cf->cmd_type)) {
                जारी;
            }

            अगर (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->तथ्य);
                वापस NGX_ERROR;
            }

            अगर ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->तथ्य);
                वापस NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            अगर (!(cmd->type & NGX_CONF_ANY)) {

                अगर (cmd->type & NGX_CONF_FLAG) {

                    अगर (cf->args->nelts != 2) {
                        जाओ invalid;
                    }

                } अन्यथा अगर (cmd->type & NGX_CONF_1MORE) {

                    अगर (cf->args->nelts < 2) {
                        जाओ invalid;
                    }

                } अन्यथा अगर (cmd->type & NGX_CONF_2MORE) {

                    अगर (cf->args->nelts < 3) {
                        जाओ invalid;
                    }

                } अन्यथा अगर (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    जाओ invalid;

                } अन्यथा अगर (!(cmd->type & argument_संख्या[cf->args->nelts - 1]))
                {
                    जाओ invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = शून्य;

            अगर (cmd->type & NGX_DIRECT_CONF) {
                conf = ((व्योम **) cf->ctx)[cf->cycle->modules[i]->सूचकांक];

            } अन्यथा अगर (cmd->type & NGX_MAIN_CONF) {
                conf = &(((व्योम **) cf->ctx)[cf->cycle->modules[i]->सूचकांक]);

            } अन्यथा अगर (cf->ctx) {
                confp = *(व्योम **) ((अक्षर *) cf->ctx + cmd->conf);

                अगर (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_सूचकांक];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            अगर (rv == NGX_CONF_OK) {
                वापस NGX_OK;
            }

            अगर (rv == NGX_CONF_ERROR) {
                वापस NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->तथ्य, rv);

            वापस NGX_ERROR;
        }
    }

    अगर (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->तथ्य);

        वापस NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->तथ्य);

    वापस NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->तथ्य);

    वापस NGX_ERROR;
}


अटल ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_आकार;
    माप_प्रकार       len;
    ssize_t      n, आकार;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_पंक्ति;
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
    b = cf->conf_file->बफर;
    dump = cf->conf_file->dump;
    start = b->pos;
    start_पंक्ति = cf->conf_file->पंक्ति;

    file_आकार = ngx_file_आकार(&cf->conf_file->file.info);

    क्रम ( ;; ) {

        अगर (b->pos >= b->last) {

            अगर (cf->conf_file->file.offset >= file_आकार) {

                अगर (cf->args->nelts > 0 || !last_space) {

                    अगर (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        वापस NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    वापस NGX_ERROR;
                }

                वापस NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            अगर (len == NGX_CONF_बफर) {
                cf->conf_file->पंक्ति = start_पंक्ति;

                अगर (d_quoted) {
                    ch = '"';

                } अन्यथा अगर (s_quoted) {
                    ch = '\'';

                } अन्यथा {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (माप_प्रकार) 10, start);
                    वापस NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                वापस NGX_ERROR;
            }

            अगर (len) {
                ngx_memmove(b->start, start, len);
            }

            आकार = (ssize_t) (file_आकार - cf->conf_file->file.offset);

            अगर (आकार > b->end - (b->start + len)) {
                आकार = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, आकार,
                              cf->conf_file->file.offset);

            अगर (n == NGX_ERROR) {
                वापस NGX_ERROR;
            }

            अगर (n != आकार) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, आकार);
                वापस NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            अगर (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, आकार);
            }
        }

        ch = *b->pos++;

        अगर (ch == LF) {
            cf->conf_file->पंक्ति++;

            अगर (sharp_comment) {
                sharp_comment = 0;
            }
        }

        अगर (sharp_comment) {
            जारी;
        }

        अगर (quoted) {
            quoted = 0;
            जारी;
        }

        अगर (need_space) {
            अगर (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                जारी;
            }

            अगर (ch == ';') {
                वापस NGX_OK;
            }

            अगर (ch == '{') {
                वापस NGX_CONF_BLOCK_START;
            }

            अगर (ch == ')') {
                last_space = 1;
                need_space = 0;

            } अन्यथा {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                वापस NGX_ERROR;
            }
        }

        अगर (last_space) {

            start = b->pos - 1;
            start_पंक्ति = cf->conf_file->पंक्ति;

            अगर (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                जारी;
            }

            चयन (ch) {

            हाल ';':
            हाल '{':
                अगर (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    वापस NGX_ERROR;
                }

                अगर (ch == '{') {
                    वापस NGX_CONF_BLOCK_START;
                }

                वापस NGX_OK;

            हाल '}':
                अगर (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    वापस NGX_ERROR;
                }

                वापस NGX_CONF_BLOCK_DONE;

            हाल '#':
                sharp_comment = 1;
                जारी;

            हाल '\\':
                quoted = 1;
                last_space = 0;
                जारी;

            हाल '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                जारी;

            हाल '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                जारी;

            हाल '$':
                variable = 1;
                last_space = 0;
                जारी;

            शेष:
                last_space = 0;
            }

        } अन्यथा {
            अगर (ch == '{' && variable) {
                जारी;
            }

            variable = 0;

            अगर (ch == '\\') {
                quoted = 1;
                जारी;
            }

            अगर (ch == '$') {
                variable = 1;
                जारी;
            }

            अगर (d_quoted) {
                अगर (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } अन्यथा अगर (s_quoted) {
                अगर (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } अन्यथा अगर (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            अगर (found) {
                word = ngx_array_धकेलो(cf->args);
                अगर (word == शून्य) {
                    वापस NGX_ERROR;
                }

                word->तथ्य = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                अगर (word->तथ्य == शून्य) {
                    वापस NGX_ERROR;
                }

                क्रम (dst = word->तथ्य, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    अगर (*src == '\\') {
                        चयन (src[1]) {
                        हाल '"':
                        हाल '\'':
                        हाल '\\':
                            src++;
                            अवरोध;

                        हाल 't':
                            *dst++ = '\t';
                            src += 2;
                            जारी;

                        हाल 'r':
                            *dst++ = '\r';
                            src += 2;
                            जारी;

                        हाल 'n':
                            *dst++ = '\n';
                            src += 2;
                            जारी;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                अगर (ch == ';') {
                    वापस NGX_OK;
                }

                अगर (ch == '{') {
                    वापस NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


अक्षर *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर        *rv;
    ngx_int_t    n;
    ngx_str_t   *मूल्य, file, name;
    ngx_glob_t   gl;

    मूल्य = cf->args->elts;
    file = मूल्य[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.तथ्य);

    अगर (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        वापस NGX_CONF_ERROR;
    }

    अगर (strpbrk((अक्षर *) file.तथ्य, "*?[") == शून्य) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.तथ्य);

        वापस ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, माप(ngx_glob_t));

    gl.pattern = file.तथ्य;
    gl.log = cf->log;
    gl.test = 1;

    अगर (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.तथ्य);
        वापस NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    क्रम ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        अगर (n != NGX_OK) {
            अवरोध;
        }

        file.len = name.len++;
        file.तथ्य = ngx_pstrdup(cf->pool, &name);
        अगर (file.तथ्य == शून्य) {
            वापस NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.तथ्य);

        rv = ngx_conf_parse(cf, &file);

        अगर (rv != NGX_CONF_OK) {
            अवरोध;
        }
    }

    ngx_close_glob(&gl);

    वापस rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    वापस ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#अगर (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#पूर्ण_अगर

    अगर (name->len) {
        full = *name;

        अगर (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            वापस शून्य;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        क्रम (i = 0; /* void */ ; i++) {

            अगर (i >= part->nelts) {
                अगर (part->अग्र == शून्य) {
                    अवरोध;
                }
                part = part->अग्र;
                file = part->elts;
                i = 0;
            }

            अगर (full.len != file[i].name.len) {
                जारी;
            }

            अगर (ngx_strcmp(full.तथ्य, file[i].name.तथ्य) == 0) {
                वापस &file[i];
            }
        }
    }

    file = ngx_list_धकेलो(&cycle->open_files);
    अगर (file == शून्य) {
        वापस शून्य;
    }

    अगर (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } अन्यथा {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = शून्य;
    file->तथ्य = शून्य;

    वापस file;
}


अटल व्योम
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    क्रम (i = 0; /* void */ ; i++) {

        अगर (i >= part->nelts) {
            अगर (part->अग्र == शून्य) {
                अवरोध;
            }
            part = part->अग्र;
            file = part->elts;
            i = 0;
        }

        अगर (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


व्योम ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    स्थिर अक्षर *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    बहु_सूची  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    बहु_शुरू(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    बहु_पूर्ण(args);

    अगर (err) {
        p = ngx_log_errno(p, last, err);
    }

    अगर (cf->conf_file == शून्य) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        वापस;
    }

    अगर (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        वापस;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.तथ्य, cf->conf_file->पंक्ति);
}


अक्षर *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर  *p = conf;

    ngx_str_t        *मूल्य;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    अगर (*fp != NGX_CONF_UNSET) {
        वापस "is duplicate";
    }

    मूल्य = cf->args->elts;

    अगर (ngx_strcasecmp(मूल्य[1].तथ्य, (u_char *) "on") == 0) {
        *fp = 1;

    } अन्यथा अगर (ngx_strcasecmp(मूल्य[1].तथ्य, (u_char *) "off") == 0) {
        *fp = 0;

    } अन्यथा {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     मूल्य[1].तथ्य, cmd->name.तथ्य);
        वापस NGX_CONF_ERROR;
    }

    अगर (cmd->post) {
        post = cmd->post;
        वापस post->post_handler(cf, post, fp);
    }

    वापस NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर  *p = conf;

    ngx_str_t        *field, *मूल्य;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    अगर (field->तथ्य) {
        वापस "is duplicate";
    }

    मूल्य = cf->args->elts;

    *field = मूल्य[1];

    अगर (cmd->post) {
        post = cmd->post;
        वापस post->post_handler(cf, post, field);
    }

    वापस NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर  *p = conf;

    ngx_str_t         *मूल्य, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    अगर (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, माप(ngx_str_t));
        अगर (*a == शून्य) {
            वापस NGX_CONF_ERROR;
        }
    }

    s = ngx_array_धकेलो(*a);
    अगर (s == शून्य) {
        वापस NGX_CONF_ERROR;
    }

    मूल्य = cf->args->elts;

    *s = मूल्य[1];

    अगर (cmd->post) {
        post = cmd->post;
        वापस post->post_handler(cf, post, s);
    }

    वापस NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर  *p = conf;

    ngx_str_t         *मूल्य;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    अगर (*a == NGX_CONF_UNSET_PTR || *a == शून्य) {
        *a = ngx_array_create(cf->pool, 4, माप(ngx_keyval_t));
        अगर (*a == शून्य) {
            वापस NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_धकेलो(*a);
    अगर (kv == शून्य) {
        वापस NGX_CONF_ERROR;
    }

    मूल्य = cf->args->elts;

    kv->कुंजी = मूल्य[1];
    kv->मूल्य = मूल्य[2];

    अगर (cmd->post) {
        post = cmd->post;
        वापस post->post_handler(cf, post, kv);
    }

    वापस NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_अंक_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    अगर (*np != NGX_CONF_UNSET) {
        वापस "is duplicate";
    }

    मूल्य = cf->args->elts;
    *np = ngx_atoi(मूल्य[1].तथ्य, मूल्य[1].len);
    अगर (*np == NGX_ERROR) {
        वापस "invalid number";
    }

    अगर (cmd->post) {
        post = cmd->post;
        वापस post->post_handler(cf, post, np);
    }

    वापस NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_आकार_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर  *p = conf;

    माप_प्रकार           *sp;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    sp = (माप_प्रकार *) (p + cmd->offset);
    अगर (*sp != NGX_CONF_UNSET_आकार) {
        वापस "is duplicate";
    }

    मूल्य = cf->args->elts;

    *sp = ngx_parse_आकार(&मूल्य[1]);
    अगर (*sp == (माप_प्रकार) NGX_ERROR) {
        वापस "invalid value";
    }

    अगर (cmd->post) {
        post = cmd->post;
        वापस post->post_handler(cf, post, sp);
    }

    वापस NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर  *p = conf;

    off_t            *op;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    अगर (*op != NGX_CONF_UNSET) {
        वापस "is duplicate";
    }

    मूल्य = cf->args->elts;

    *op = ngx_parse_offset(&मूल्य[1]);
    अगर (*op == (off_t) NGX_ERROR) {
        वापस "invalid value";
    }

    अगर (cmd->post) {
        post = cmd->post;
        वापस post->post_handler(cf, post, op);
    }

    वापस NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    अगर (*msp != NGX_CONF_UNSET_MSEC) {
        वापस "is duplicate";
    }

    मूल्य = cf->args->elts;

    *msp = ngx_parse_time(&मूल्य[1], 0);
    अगर (*msp == (ngx_msec_t) NGX_ERROR) {
        वापस "invalid value";
    }

    अगर (cmd->post) {
        post = cmd->post;
        वापस post->post_handler(cf, post, msp);
    }

    वापस NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर  *p = conf;

    समय_प्रकार           *sp;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    sp = (समय_प्रकार *) (p + cmd->offset);
    अगर (*sp != NGX_CONF_UNSET) {
        वापस "is duplicate";
    }

    मूल्य = cf->args->elts;

    *sp = ngx_parse_time(&मूल्य[1], 1);
    अगर (*sp == (समय_प्रकार) NGX_ERROR) {
        वापस "invalid value";
    }

    अगर (cmd->post) {
        post = cmd->post;
        वापस post->post_handler(cf, post, sp);
    }

    वापस NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर *p = conf;

    ngx_str_t   *मूल्य;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    अगर (bufs->अंक) {
        वापस "is duplicate";
    }

    मूल्य = cf->args->elts;

    bufs->अंक = ngx_atoi(मूल्य[1].तथ्य, मूल्य[1].len);
    अगर (bufs->अंक == NGX_ERROR || bufs->अंक == 0) {
        वापस "invalid value";
    }

    bufs->आकार = ngx_parse_आकार(&मूल्य[2]);
    अगर (bufs->आकार == (माप_प्रकार) NGX_ERROR || bufs->आकार == 0) {
        वापस "invalid value";
    }

    वापस NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *मूल्य;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    अगर (*np != NGX_CONF_UNSET_UINT) {
        वापस "is duplicate";
    }

    मूल्य = cf->args->elts;
    e = cmd->post;

    क्रम (i = 0; e[i].name.len != 0; i++) {
        अगर (e[i].name.len != मूल्य[1].len
            || ngx_strcasecmp(e[i].name.तथ्य, मूल्य[1].तथ्य) != 0)
        {
            जारी;
        }

        *np = e[i].मूल्य;

        वापस NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", मूल्य[1].तथ्य);

    वापस NGX_CONF_ERROR;
}


अक्षर *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *मूल्य;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    मूल्य = cf->args->elts;
    mask = cmd->post;

    क्रम (i = 1; i < cf->args->nelts; i++) {
        क्रम (m = 0; mask[m].name.len != 0; m++) {

            अगर (mask[m].name.len != मूल्य[i].len
                || ngx_strcasecmp(mask[m].name.तथ्य, मूल्य[i].तथ्य) != 0)
            {
                जारी;
            }

            अगर (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", मूल्य[i].तथ्य);

            } अन्यथा {
                *np |= mask[m].mask;
            }

            अवरोध;
        }

        अगर (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", मूल्य[i].तथ्य);

            वापस NGX_CONF_ERROR;
        }
    }

    वापस NGX_CONF_OK;
}


#अगर 0

अक्षर *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    वापस "unsupported on this platform";
}

#पूर्ण_अगर


अक्षर *
ngx_conf_deprecated(ngx_conf_t *cf, व्योम *post, व्योम *तथ्य)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    वापस NGX_CONF_OK;
}


अक्षर *
ngx_conf_check_अंक_bounds(ngx_conf_t *cf, व्योम *post, व्योम *तथ्य)
{
    ngx_conf_अंक_bounds_t  *bounds = post;
    ngx_int_t  *np = तथ्य;

    अगर (bounds->ऊपरी == -1) {
        अगर (*np >= bounds->निचला) {
            वापस NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->निचला);

        वापस NGX_CONF_ERROR;
    }

    अगर (*np >= bounds->निचला && *np <= bounds->ऊपरी) {
        वापस NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->निचला, bounds->ऊपरी);

    वापस NGX_CONF_ERROR;
}


अक्षर *
ngx_conf_check_आकार_nonzero(ngx_conf_t *cf, व्योम *post, व्योम *तथ्य)
{
    माप_प्रकार  *sp = तथ्य;

    अगर (*sp == 0) {
        वापस "value must be nonzero";
    }

    वापस NGX_CONF_OK;
}
