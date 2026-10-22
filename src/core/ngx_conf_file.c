
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>

#define NGX_CONF_BUFFER  4096

स्थिर ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
स्थिर ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
स्थिर ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
स्थिर शून्य ngx_conf_flush_files(ngx_cycle_t *cycle);


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

स्थिर ngx_uint_t argument_संख्या[] = {
    NGX_CONF_NOARGS,
    NGX_CONF_TAKE1,
    NGX_CONF_TAKE2,
    NGX_CONF_TAKE3,
    NGX_CONF_TAKE4,
    NGX_CONF_TAKE5,
    NGX_CONF_TAKE6,
    NGX_CONF_TAKE7
};


ngx_conf_post_t  ngx_conf_size_nonzero_post = { ngx_conf_check_size_nonzero };


वर्ण *
ngx_conf_param(ngx_conf_t *cf)
{
    वर्ण             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    यदि (param->len == 0) {
        प्रत्यागम NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, परिमाण(ngx_conf_file_t));

    ngx_memzero(&b, परिमाण(ngx_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.data = रिक्त;
    conf_file.पंक्ति = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    rv = ngx_conf_parse(cf, रिक्त);

    cf->conf_file = रिक्त;

    प्रत्यागम rv;
}


स्थिर ngx_int_t
ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             size;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_गाँठ_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_crc32_long(filename->data, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->cycle->config_dump_rbtree, filename, hash);

    यदि (sn) {
        cf->conf_file->dump = रिक्त;
        प्रत्यागम NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    यदि (p == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    cd = ngx_array_धकेलो(&cf->cycle->config_dump);
    यदि (cd == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (माप_प्रकार) size);
    यदि (buf == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, परिमाण(ngx_str_गाँठ_t));
    यदि (sn == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    sn->गाँठ.कुंजी = hash;
    sn->str = cd->name;

    ngx_rbtree_जोड़ो(&cf->cycle->config_dump_rbtree, &sn->गाँठ);

    प्रत्यागम NGX_OK;
}


वर्ण *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    वर्ण             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    गणना {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#यदि (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = रिक्त;
#endif

    यदि (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        यदि (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            प्रत्यागम NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        यदि (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        यदि (buf.start == रिक्त) {
            गच्छ failed;
        }

        buf.pos = buf.start;
        buf.last = buf.start;
        buf.end = buf.last + NGX_CONF_BUFFER;
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.data = filename->data;
        cf->conf_file->file.offset = 0;
        cf->conf_file->file.log = cf->log;
        cf->conf_file->पंक्ति = 1;

        type = parse_file;

        यदि (ngx_dump_config
#यदि (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#यदि (NGX_DEBUG)
            || 1
#endif
           )
        {
            यदि (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                गच्छ failed;
            }

        } अन्यथा {
            cf->conf_file->dump = रिक्त;
        }

    } अन्यथा यदि (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

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
            गच्छ done;
        }

        यदि (rc == NGX_CONF_BLOCK_DONE) {

            यदि (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                गच्छ failed;
            }

            गच्छ done;
        }

        यदि (rc == NGX_CONF_FILE_DONE) {

            यदि (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                गच्छ failed;
            }

            गच्छ done;
        }

        यदि (rc == NGX_CONF_BLOCK_START) {

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

            यदि (rc == NGX_CONF_BLOCK_START) {
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

done:

    यदि (filename) {
        यदि (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        यदि (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    यदि (rc == NGX_ERROR) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    प्रत्यागम NGX_CONF_OK;
}


स्थिर ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    वर्ण           *rv;
    शून्य           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    प्रति (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        यदि (cmd == रिक्त) {
            अनुवर्त;
        }

        प्रति ( /* void */ ; cmd->name.len; cmd++) {

            यदि (name->len != cmd->name.len) {
                अनुवर्त;
            }

            यदि (ngx_strcmp(name->data, cmd->name.data) != 0) {
                अनुवर्त;
            }

            found = 1;

            यदि (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                अनुवर्त;
            }

            /* is the directive's location right ? */

            यदि (!(cmd->type & cf->cmd_type)) {
                अनुवर्त;
            }

            यदि (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                प्रत्यागम NGX_ERROR;
            }

            यदि ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                प्रत्यागम NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            यदि (!(cmd->type & NGX_CONF_ANY)) {

                यदि (cmd->type & NGX_CONF_FLAG) {

                    यदि (cf->args->nelts != 2) {
                        गच्छ invalid;
                    }

                } अन्यथा यदि (cmd->type & NGX_CONF_1MORE) {

                    यदि (cf->args->nelts < 2) {
                        गच्छ invalid;
                    }

                } अन्यथा यदि (cmd->type & NGX_CONF_2MORE) {

                    यदि (cf->args->nelts < 3) {
                        गच्छ invalid;
                    }

                } अन्यथा यदि (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    गच्छ invalid;

                } अन्यथा यदि (!(cmd->type & argument_संख्या[cf->args->nelts - 1]))
                {
                    गच्छ invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = रिक्त;

            यदि (cmd->type & NGX_DIRECT_CONF) {
                conf = ((शून्य **) cf->ctx)[cf->cycle->modules[i]->index];

            } अन्यथा यदि (cmd->type & NGX_MAIN_CONF) {
                conf = &(((शून्य **) cf->ctx)[cf->cycle->modules[i]->index]);

            } अन्यथा यदि (cf->ctx) {
                confp = *(शून्य **) ((वर्ण *) cf->ctx + cmd->conf);

                यदि (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            यदि (rv == NGX_CONF_OK) {
                प्रत्यागम NGX_OK;
            }

            यदि (rv == NGX_CONF_ERROR) {
                प्रत्यागम NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            प्रत्यागम NGX_ERROR;
        }
    }

    यदि (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        प्रत्यागम NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    प्रत्यागम NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    प्रत्यागम NGX_ERROR;
}


स्थिर ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    माप_प्रकार       len;
    ssize_t      n, size;
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
    b = cf->conf_file->buffer;
    dump = cf->conf_file->dump;
    start = b->pos;
    start_पंक्ति = cf->conf_file->पंक्ति;

    file_size = ngx_file_size(&cf->conf_file->file.info);

    प्रति ( ;; ) {

        यदि (b->pos >= b->last) {

            यदि (cf->conf_file->file.offset >= file_size) {

                यदि (cf->args->nelts > 0 || !last_space) {

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

                प्रत्यागम NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            यदि (len == NGX_CONF_BUFFER) {
                cf->conf_file->पंक्ति = start_पंक्ति;

                यदि (d_quoted) {
                    ch = '"';

                } अन्यथा यदि (s_quoted) {
                    ch = '\'';

                } अन्यथा {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (माप_प्रकार) 10, start);
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

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            यदि (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            यदि (n == NGX_ERROR) {
                प्रत्यागम NGX_ERROR;
            }

            यदि (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                प्रत्यागम NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            यदि (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        यदि (ch == LF) {
            cf->conf_file->पंक्ति++;

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
                last_space = 1;
                need_space = 0;
                अनुवर्त;
            }

            यदि (ch == ';') {
                प्रत्यागम NGX_OK;
            }

            यदि (ch == '{') {
                प्रत्यागम NGX_CONF_BLOCK_START;
            }

            यदि (ch == ')') {
                last_space = 1;
                need_space = 0;

            } अन्यथा {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                प्रत्यागम NGX_ERROR;
            }
        }

        यदि (last_space) {

            start = b->pos - 1;
            start_पंक्ति = cf->conf_file->पंक्ति;

            यदि (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                अनुवर्त;
            }

            विकल्प (ch) {

            पक्ष ';':
            पक्ष '{':
                यदि (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    प्रत्यागम NGX_ERROR;
                }

                यदि (ch == '{') {
                    प्रत्यागम NGX_CONF_BLOCK_START;
                }

                प्रत्यागम NGX_OK;

            पक्ष '}':
                यदि (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    प्रत्यागम NGX_ERROR;
                }

                प्रत्यागम NGX_CONF_BLOCK_DONE;

            पक्ष '#':
                sharp_comment = 1;
                अनुवर्त;

            पक्ष '\\':
                quoted = 1;
                last_space = 0;
                अनुवर्त;

            पक्ष '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                अनुवर्त;

            पक्ष '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                अनुवर्त;

            पक्ष '$':
                variable = 1;
                last_space = 0;
                अनुवर्त;

            स्वभाव:
                last_space = 0;
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
                    found = 1;
                }

            } अन्यथा यदि (s_quoted) {
                यदि (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } अन्यथा यदि (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            यदि (found) {
                word = ngx_array_धकेलो(cf->args);
                यदि (word == रिक्त) {
                    प्रत्यागम NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                यदि (word->data == रिक्त) {
                    प्रत्यागम NGX_ERROR;
                }

                प्रति (dst = word->data, src = start, len = 0;
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
                    प्रत्यागम NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


वर्ण *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण        *rv;
    ngx_int_t    n;
    ngx_str_t   *मूल्य, file, name;
    ngx_glob_t   gl;

    मूल्य = cf->args->elts;
    file = मूल्य[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    यदि (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    यदि (strpbrk((वर्ण *) file.data, "*?[") == रिक्त) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        प्रत्यागम ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, परिमाण(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    यदि (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        प्रत्यागम NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    प्रति ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        यदि (n != NGX_OK) {
            भङ्ग;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        यदि (file.data == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        यदि (rv != NGX_CONF_OK) {
            भङ्ग;
        }
    }

    ngx_close_glob(&gl);

    प्रत्यागम rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    प्रत्यागम ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#यदि (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    यदि (name->len) {
        full = *name;

        यदि (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            प्रत्यागम रिक्त;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        प्रति (i = 0; /* void */ ; i++) {

            यदि (i >= part->nelts) {
                यदि (part->next == रिक्त) {
                    भङ्ग;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            यदि (full.len != file[i].name.len) {
                अनुवर्त;
            }

            यदि (ngx_strcmp(full.data, file[i].name.data) == 0) {
                प्रत्यागम &file[i];
            }
        }
    }

    file = ngx_list_धकेलो(&cycle->open_files);
    यदि (file == रिक्त) {
        प्रत्यागम रिक्त;
    }

    यदि (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } अन्यथा {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = रिक्त;
    file->data = रिक्त;

    प्रत्यागम file;
}


स्थिर शून्य
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    प्रति (i = 0; /* void */ ; i++) {

        यदि (i >= part->nelts) {
            यदि (part->next == रिक्त) {
                भङ्ग;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        यदि (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


शून्य ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ध्रुव वर्ण *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    बहु_सूची  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    बहु_शुरू(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    बहु_पूर्ण(args);

    यदि (err) {
        p = ngx_log_errno(p, last, err);
    }

    यदि (cf->conf_file == रिक्त) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        प्रत्यागम;
    }

    यदि (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        प्रत्यागम;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->पंक्ति);
}


वर्ण *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_str_t        *मूल्य;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    यदि (*fp != NGX_CONF_UNSET) {
        प्रत्यागम "is duplicate";
    }

    मूल्य = cf->args->elts;

    यदि (ngx_strcasecmp(मूल्य[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } अन्यथा यदि (ngx_strcasecmp(मूल्य[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } अन्यथा {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     मूल्य[1].data, cmd->name.data);
        प्रत्यागम NGX_CONF_ERROR;
    }

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, fp);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_str_t        *field, *मूल्य;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    यदि (field->data) {
        प्रत्यागम "is duplicate";
    }

    मूल्य = cf->args->elts;

    *field = मूल्य[1];

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, field);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_str_t         *मूल्य, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    यदि (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, परिमाण(ngx_str_t));
        यदि (*a == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }
    }

    s = ngx_array_धकेलो(*a);
    यदि (s == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    मूल्य = cf->args->elts;

    *s = मूल्य[1];

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, s);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_str_t         *मूल्य;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    यदि (*a == NGX_CONF_UNSET_PTR || *a == रिक्त) {
        *a = ngx_array_create(cf->pool, 4, परिमाण(ngx_keyval_t));
        यदि (*a == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_धकेलो(*a);
    यदि (kv == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    मूल्य = cf->args->elts;

    kv->कुंजी = मूल्य[1];
    kv->मूल्य = मूल्य[2];

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, kv);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_set_अंक_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    यदि (*np != NGX_CONF_UNSET) {
        प्रत्यागम "is duplicate";
    }

    मूल्य = cf->args->elts;
    *np = ngx_atoi(मूल्य[1].data, मूल्य[1].len);
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
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    माप_प्रकार           *sp;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    sp = (माप_प्रकार *) (p + cmd->offset);
    यदि (*sp != NGX_CONF_UNSET_SIZE) {
        प्रत्यागम "is duplicate";
    }

    मूल्य = cf->args->elts;

    *sp = ngx_parse_size(&मूल्य[1]);
    यदि (*sp == (माप_प्रकार) NGX_ERROR) {
        प्रत्यागम "invalid value";
    }

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, sp);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    off_t            *op;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    यदि (*op != NGX_CONF_UNSET) {
        प्रत्यागम "is duplicate";
    }

    मूल्य = cf->args->elts;

    *op = ngx_parse_offset(&मूल्य[1]);
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
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    यदि (*msp != NGX_CONF_UNSET_MSEC) {
        प्रत्यागम "is duplicate";
    }

    मूल्य = cf->args->elts;

    *msp = ngx_parse_time(&मूल्य[1], 0);
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
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    समय_प्रकार           *sp;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    sp = (समय_प्रकार *) (p + cmd->offset);
    यदि (*sp != NGX_CONF_UNSET) {
        प्रत्यागम "is duplicate";
    }

    मूल्य = cf->args->elts;

    *sp = ngx_parse_time(&मूल्य[1], 1);
    यदि (*sp == (समय_प्रकार) NGX_ERROR) {
        प्रत्यागम "invalid value";
    }

    यदि (cmd->post) {
        post = cmd->post;
        प्रत्यागम post->post_handler(cf, post, sp);
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण *p = conf;

    ngx_str_t   *मूल्य;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    यदि (bufs->अंक) {
        प्रत्यागम "is duplicate";
    }

    मूल्य = cf->args->elts;

    bufs->अंक = ngx_atoi(मूल्य[1].data, मूल्य[1].len);
    यदि (bufs->अंक == NGX_ERROR || bufs->अंक == 0) {
        प्रत्यागम "invalid value";
    }

    bufs->size = ngx_parse_size(&मूल्य[2]);
    यदि (bufs->size == (माप_प्रकार) NGX_ERROR || bufs->size == 0) {
        प्रत्यागम "invalid value";
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *मूल्य;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    यदि (*np != NGX_CONF_UNSET_UINT) {
        प्रत्यागम "is duplicate";
    }

    मूल्य = cf->args->elts;
    e = cmd->post;

    प्रति (i = 0; e[i].name.len != 0; i++) {
        यदि (e[i].name.len != मूल्य[1].len
            || ngx_strcasecmp(e[i].name.data, मूल्य[1].data) != 0)
        {
            अनुवर्त;
        }

        *np = e[i].मूल्य;

        प्रत्यागम NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", मूल्य[1].data);

    प्रत्यागम NGX_CONF_ERROR;
}


वर्ण *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *मूल्य;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    मूल्य = cf->args->elts;
    mask = cmd->post;

    प्रति (i = 1; i < cf->args->nelts; i++) {
        प्रति (m = 0; mask[m].name.len != 0; m++) {

            यदि (mask[m].name.len != मूल्य[i].len
                || ngx_strcasecmp(mask[m].name.data, मूल्य[i].data) != 0)
            {
                अनुवर्त;
            }

            यदि (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", मूल्य[i].data);

            } अन्यथा {
                *np |= mask[m].mask;
            }

            भङ्ग;
        }

        यदि (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", मूल्य[i].data);

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

#endif


वर्ण *
ngx_conf_deprecated(ngx_conf_t *cf, शून्य *post, शून्य *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_check_अंक_bounds(ngx_conf_t *cf, शून्य *post, शून्य *data)
{
    ngx_conf_अंक_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    यदि (bounds->ऊपरी == -1) {
        यदि (*np >= bounds->निचला) {
            प्रत्यागम NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->निचला);

        प्रत्यागम NGX_CONF_ERROR;
    }

    यदि (*np >= bounds->निचला && *np <= bounds->ऊपरी) {
        प्रत्यागम NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->निचला, bounds->ऊपरी);

    प्रत्यागम NGX_CONF_ERROR;
}


वर्ण *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, शून्य *post, शून्य *data)
{
    माप_प्रकार  *sp = data;

    यदि (*sp == 0) {
        प्रत्यागम "value must be nonzero";
    }

    प्रत्यागम NGX_CONF_OK;
}
