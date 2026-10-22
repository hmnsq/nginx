
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


अक्षर *
ngx_conf_param(ngx_conf_t *cf)
{
    अक्षर             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    जर (param->len == 0) {
        परत NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, आकार(ngx_conf_file_t));

    ngx_memzero(&b, आकार(ngx_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.data = रिक्त;
    conf_file.ओळ = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    rv = ngx_conf_parse(cf, रिक्त);

    cf->conf_file = रिक्त;

    परत rv;
}


स्थिर ngx_int_t
ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             size;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_गाठ_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_crc32_long(filename->data, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->cycle->config_dump_rbtree, filename, hash);

    जर (sn) {
        cf->conf_file->dump = रिक्त;
        परत NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    जर (p == रिक्त) {
        परत NGX_ERROR;
    }

    cd = ngx_array_ढकला(&cf->cycle->config_dump);
    जर (cd == रिक्त) {
        परत NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (माप_प्रकार) size);
    जर (buf == रिक्त) {
        परत NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, आकार(ngx_str_गाठ_t));
    जर (sn == रिक्त) {
        परत NGX_ERROR;
    }

    sn->गाठ.किल्ली = hash;
    sn->str = cd->name;

    ngx_rbtree_घाला(&cf->cycle->config_dump_rbtree, &sn->गाठ);

    परत NGX_OK;
}


अक्षर *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    अक्षर             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    गणना {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#जर (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = रिक्त;
#endif

    जर (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        जर (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            परत NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        जर (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        जर (buf.start == रिक्त) {
            जा failed;
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
        cf->conf_file->ओळ = 1;

        type = parse_file;

        जर (ngx_dump_config
#जर (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#जर (NGX_DEBUG)
            || 1
#endif
           )
        {
            जर (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                जा failed;
            }

        } अन्यथा {
            cf->conf_file->dump = रिक्त;
        }

    } अन्यथा जर (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } अन्यथा {
        type = parse_param;
    }


    साठी ( ;; ) {
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

        जर (rc == NGX_ERROR) {
            जा done;
        }

        जर (rc == NGX_CONF_BLOCK_DONE) {

            जर (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                जा failed;
            }

            जा done;
        }

        जर (rc == NGX_CONF_FILE_DONE) {

            जर (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                जा failed;
            }

            जा done;
        }

        जर (rc == NGX_CONF_BLOCK_START) {

            जर (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                जा failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        जर (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            जर (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                जा failed;
            }

            rv = (*cf->handler)(cf, रिक्त, cf->handler_conf);
            जर (rv == NGX_CONF_OK) {
                सुरूठेवा;
            }

            जर (rv == NGX_CONF_ERROR) {
                जा failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            जा failed;
        }


        rc = ngx_conf_handler(cf, rc);

        जर (rc == NGX_ERROR) {
            जा failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    जर (filename) {
        जर (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        जर (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    जर (rc == NGX_ERROR) {
        परत NGX_CONF_ERROR;
    }

    परत NGX_CONF_OK;
}


स्थिर ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    अक्षर           *rv;
    शून्य           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    साठी (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        जर (cmd == रिक्त) {
            सुरूठेवा;
        }

        साठी ( /* void */ ; cmd->name.len; cmd++) {

            जर (name->len != cmd->name.len) {
                सुरूठेवा;
            }

            जर (ngx_strcmp(name->data, cmd->name.data) != 0) {
                सुरूठेवा;
            }

            found = 1;

            जर (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                सुरूठेवा;
            }

            /* is the directive's location right ? */

            जर (!(cmd->type & cf->cmd_type)) {
                सुरूठेवा;
            }

            जर (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                परत NGX_ERROR;
            }

            जर ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                परत NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            जर (!(cmd->type & NGX_CONF_ANY)) {

                जर (cmd->type & NGX_CONF_FLAG) {

                    जर (cf->args->nelts != 2) {
                        जा invalid;
                    }

                } अन्यथा जर (cmd->type & NGX_CONF_1MORE) {

                    जर (cf->args->nelts < 2) {
                        जा invalid;
                    }

                } अन्यथा जर (cmd->type & NGX_CONF_2MORE) {

                    जर (cf->args->nelts < 3) {
                        जा invalid;
                    }

                } अन्यथा जर (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    जा invalid;

                } अन्यथा जर (!(cmd->type & argument_संख्या[cf->args->nelts - 1]))
                {
                    जा invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = रिक्त;

            जर (cmd->type & NGX_DIRECT_CONF) {
                conf = ((शून्य **) cf->ctx)[cf->cycle->modules[i]->index];

            } अन्यथा जर (cmd->type & NGX_MAIN_CONF) {
                conf = &(((शून्य **) cf->ctx)[cf->cycle->modules[i]->index]);

            } अन्यथा जर (cf->ctx) {
                confp = *(शून्य **) ((अक्षर *) cf->ctx + cmd->conf);

                जर (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            जर (rv == NGX_CONF_OK) {
                परत NGX_OK;
            }

            जर (rv == NGX_CONF_ERROR) {
                परत NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            परत NGX_ERROR;
        }
    }

    जर (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        परत NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    परत NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    परत NGX_ERROR;
}


स्थिर ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    माप_प्रकार       len;
    ssize_t      n, size;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_ओळ;
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
    start_ओळ = cf->conf_file->ओळ;

    file_size = ngx_file_size(&cf->conf_file->file.info);

    साठी ( ;; ) {

        जर (b->pos >= b->last) {

            जर (cf->conf_file->file.offset >= file_size) {

                जर (cf->args->nelts > 0 || !last_space) {

                    जर (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        परत NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    परत NGX_ERROR;
                }

                परत NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            जर (len == NGX_CONF_BUFFER) {
                cf->conf_file->ओळ = start_ओळ;

                जर (d_quoted) {
                    ch = '"';

                } अन्यथा जर (s_quoted) {
                    ch = '\'';

                } अन्यथा {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (माप_प्रकार) 10, start);
                    परत NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                परत NGX_ERROR;
            }

            जर (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            जर (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            जर (n == NGX_ERROR) {
                परत NGX_ERROR;
            }

            जर (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                परत NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            जर (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        जर (ch == LF) {
            cf->conf_file->ओळ++;

            जर (sharp_comment) {
                sharp_comment = 0;
            }
        }

        जर (sharp_comment) {
            सुरूठेवा;
        }

        जर (quoted) {
            quoted = 0;
            सुरूठेवा;
        }

        जर (need_space) {
            जर (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                सुरूठेवा;
            }

            जर (ch == ';') {
                परत NGX_OK;
            }

            जर (ch == '{') {
                परत NGX_CONF_BLOCK_START;
            }

            जर (ch == ')') {
                last_space = 1;
                need_space = 0;

            } अन्यथा {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                परत NGX_ERROR;
            }
        }

        जर (last_space) {

            start = b->pos - 1;
            start_ओळ = cf->conf_file->ओळ;

            जर (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                सुरूठेवा;
            }

            निवड (ch) {

            स्थिती ';':
            स्थिती '{':
                जर (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    परत NGX_ERROR;
                }

                जर (ch == '{') {
                    परत NGX_CONF_BLOCK_START;
                }

                परत NGX_OK;

            स्थिती '}':
                जर (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    परत NGX_ERROR;
                }

                परत NGX_CONF_BLOCK_DONE;

            स्थिती '#':
                sharp_comment = 1;
                सुरूठेवा;

            स्थिती '\\':
                quoted = 1;
                last_space = 0;
                सुरूठेवा;

            स्थिती '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                सुरूठेवा;

            स्थिती '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                सुरूठेवा;

            स्थिती '$':
                variable = 1;
                last_space = 0;
                सुरूठेवा;

            पूर्वनिर्धारित:
                last_space = 0;
            }

        } अन्यथा {
            जर (ch == '{' && variable) {
                सुरूठेवा;
            }

            variable = 0;

            जर (ch == '\\') {
                quoted = 1;
                सुरूठेवा;
            }

            जर (ch == '$') {
                variable = 1;
                सुरूठेवा;
            }

            जर (d_quoted) {
                जर (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } अन्यथा जर (s_quoted) {
                जर (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } अन्यथा जर (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            जर (found) {
                word = ngx_array_ढकला(cf->args);
                जर (word == रिक्त) {
                    परत NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                जर (word->data == रिक्त) {
                    परत NGX_ERROR;
                }

                साठी (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    जर (*src == '\\') {
                        निवड (src[1]) {
                        स्थिती '"':
                        स्थिती '\'':
                        स्थिती '\\':
                            src++;
                            थांबा;

                        स्थिती 't':
                            *dst++ = '\t';
                            src += 2;
                            सुरूठेवा;

                        स्थिती 'r':
                            *dst++ = '\r';
                            src += 2;
                            सुरूठेवा;

                        स्थिती 'n':
                            *dst++ = '\n';
                            src += 2;
                            सुरूठेवा;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                जर (ch == ';') {
                    परत NGX_OK;
                }

                जर (ch == '{') {
                    परत NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


अक्षर *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    अक्षर        *rv;
    ngx_int_t    n;
    ngx_str_t   *मूल्य, file, name;
    ngx_glob_t   gl;

    मूल्य = cf->args->elts;
    file = मूल्य[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    जर (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        परत NGX_CONF_ERROR;
    }

    जर (strpbrk((अक्षर *) file.data, "*?[") == रिक्त) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        परत ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, आकार(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    जर (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        परत NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    साठी ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        जर (n != NGX_OK) {
            थांबा;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        जर (file.data == रिक्त) {
            परत NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        जर (rv != NGX_CONF_OK) {
            थांबा;
        }
    }

    ngx_close_glob(&gl);

    परत rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    परत ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#जर (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    जर (name->len) {
        full = *name;

        जर (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            परत रिक्त;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        साठी (i = 0; /* void */ ; i++) {

            जर (i >= part->nelts) {
                जर (part->next == रिक्त) {
                    थांबा;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            जर (full.len != file[i].name.len) {
                सुरूठेवा;
            }

            जर (ngx_strcmp(full.data, file[i].name.data) == 0) {
                परत &file[i];
            }
        }
    }

    file = ngx_list_ढकला(&cycle->open_files);
    जर (file == रिक्त) {
        परत रिक्त;
    }

    जर (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } अन्यथा {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = रिक्त;
    file->data = रिक्त;

    परत file;
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

    साठी (i = 0; /* void */ ; i++) {

        जर (i >= part->nelts) {
            जर (part->next == रिक्त) {
                थांबा;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        जर (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


शून्य ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    स्थिरांक अक्षर *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    बहु_सूची  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    बहु_शुरू(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    बहु_पूर्ण(args);

    जर (err) {
        p = ngx_log_errno(p, last, err);
    }

    जर (cf->conf_file == रिक्त) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        परत;
    }

    जर (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        परत;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->ओळ);
}


अक्षर *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    अक्षर  *p = conf;

    ngx_str_t        *मूल्य;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    जर (*fp != NGX_CONF_UNSET) {
        परत "is duplicate";
    }

    मूल्य = cf->args->elts;

    जर (ngx_strcasecmp(मूल्य[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } अन्यथा जर (ngx_strcasecmp(मूल्य[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } अन्यथा {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     मूल्य[1].data, cmd->name.data);
        परत NGX_CONF_ERROR;
    }

    जर (cmd->post) {
        post = cmd->post;
        परत post->post_handler(cf, post, fp);
    }

    परत NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    अक्षर  *p = conf;

    ngx_str_t        *field, *मूल्य;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    जर (field->data) {
        परत "is duplicate";
    }

    मूल्य = cf->args->elts;

    *field = मूल्य[1];

    जर (cmd->post) {
        post = cmd->post;
        परत post->post_handler(cf, post, field);
    }

    परत NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    अक्षर  *p = conf;

    ngx_str_t         *मूल्य, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    जर (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, आकार(ngx_str_t));
        जर (*a == रिक्त) {
            परत NGX_CONF_ERROR;
        }
    }

    s = ngx_array_ढकला(*a);
    जर (s == रिक्त) {
        परत NGX_CONF_ERROR;
    }

    मूल्य = cf->args->elts;

    *s = मूल्य[1];

    जर (cmd->post) {
        post = cmd->post;
        परत post->post_handler(cf, post, s);
    }

    परत NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    अक्षर  *p = conf;

    ngx_str_t         *मूल्य;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    जर (*a == NGX_CONF_UNSET_PTR || *a == रिक्त) {
        *a = ngx_array_create(cf->pool, 4, आकार(ngx_keyval_t));
        जर (*a == रिक्त) {
            परत NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_ढकला(*a);
    जर (kv == रिक्त) {
        परत NGX_CONF_ERROR;
    }

    मूल्य = cf->args->elts;

    kv->किल्ली = मूल्य[1];
    kv->मूल्य = मूल्य[2];

    जर (cmd->post) {
        post = cmd->post;
        परत post->post_handler(cf, post, kv);
    }

    परत NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_अंक_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    अक्षर  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    जर (*np != NGX_CONF_UNSET) {
        परत "is duplicate";
    }

    मूल्य = cf->args->elts;
    *np = ngx_atoi(मूल्य[1].data, मूल्य[1].len);
    जर (*np == NGX_ERROR) {
        परत "invalid number";
    }

    जर (cmd->post) {
        post = cmd->post;
        परत post->post_handler(cf, post, np);
    }

    परत NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    अक्षर  *p = conf;

    माप_प्रकार           *sp;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    sp = (माप_प्रकार *) (p + cmd->offset);
    जर (*sp != NGX_CONF_UNSET_SIZE) {
        परत "is duplicate";
    }

    मूल्य = cf->args->elts;

    *sp = ngx_parse_size(&मूल्य[1]);
    जर (*sp == (माप_प्रकार) NGX_ERROR) {
        परत "invalid value";
    }

    जर (cmd->post) {
        post = cmd->post;
        परत post->post_handler(cf, post, sp);
    }

    परत NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    अक्षर  *p = conf;

    off_t            *op;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    जर (*op != NGX_CONF_UNSET) {
        परत "is duplicate";
    }

    मूल्य = cf->args->elts;

    *op = ngx_parse_offset(&मूल्य[1]);
    जर (*op == (off_t) NGX_ERROR) {
        परत "invalid value";
    }

    जर (cmd->post) {
        post = cmd->post;
        परत post->post_handler(cf, post, op);
    }

    परत NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    अक्षर  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    जर (*msp != NGX_CONF_UNSET_MSEC) {
        परत "is duplicate";
    }

    मूल्य = cf->args->elts;

    *msp = ngx_parse_time(&मूल्य[1], 0);
    जर (*msp == (ngx_msec_t) NGX_ERROR) {
        परत "invalid value";
    }

    जर (cmd->post) {
        post = cmd->post;
        परत post->post_handler(cf, post, msp);
    }

    परत NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    अक्षर  *p = conf;

    समय_प्रकार           *sp;
    ngx_str_t        *मूल्य;
    ngx_conf_post_t  *post;


    sp = (समय_प्रकार *) (p + cmd->offset);
    जर (*sp != NGX_CONF_UNSET) {
        परत "is duplicate";
    }

    मूल्य = cf->args->elts;

    *sp = ngx_parse_time(&मूल्य[1], 1);
    जर (*sp == (समय_प्रकार) NGX_ERROR) {
        परत "invalid value";
    }

    जर (cmd->post) {
        post = cmd->post;
        परत post->post_handler(cf, post, sp);
    }

    परत NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    अक्षर *p = conf;

    ngx_str_t   *मूल्य;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    जर (bufs->अंक) {
        परत "is duplicate";
    }

    मूल्य = cf->args->elts;

    bufs->अंक = ngx_atoi(मूल्य[1].data, मूल्य[1].len);
    जर (bufs->अंक == NGX_ERROR || bufs->अंक == 0) {
        परत "invalid value";
    }

    bufs->size = ngx_parse_size(&मूल्य[2]);
    जर (bufs->size == (माप_प्रकार) NGX_ERROR || bufs->size == 0) {
        परत "invalid value";
    }

    परत NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    अक्षर  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *मूल्य;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    जर (*np != NGX_CONF_UNSET_UINT) {
        परत "is duplicate";
    }

    मूल्य = cf->args->elts;
    e = cmd->post;

    साठी (i = 0; e[i].name.len != 0; i++) {
        जर (e[i].name.len != मूल्य[1].len
            || ngx_strcasecmp(e[i].name.data, मूल्य[1].data) != 0)
        {
            सुरूठेवा;
        }

        *np = e[i].मूल्य;

        परत NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", मूल्य[1].data);

    परत NGX_CONF_ERROR;
}


अक्षर *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    अक्षर  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *मूल्य;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    मूल्य = cf->args->elts;
    mask = cmd->post;

    साठी (i = 1; i < cf->args->nelts; i++) {
        साठी (m = 0; mask[m].name.len != 0; m++) {

            जर (mask[m].name.len != मूल्य[i].len
                || ngx_strcasecmp(mask[m].name.data, मूल्य[i].data) != 0)
            {
                सुरूठेवा;
            }

            जर (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", मूल्य[i].data);

            } अन्यथा {
                *np |= mask[m].mask;
            }

            थांबा;
        }

        जर (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", मूल्य[i].data);

            परत NGX_CONF_ERROR;
        }
    }

    परत NGX_CONF_OK;
}


#जर 0

अक्षर *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    परत "unsupported on this platform";
}

#endif


अक्षर *
ngx_conf_deprecated(ngx_conf_t *cf, शून्य *post, शून्य *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    परत NGX_CONF_OK;
}


अक्षर *
ngx_conf_check_अंक_bounds(ngx_conf_t *cf, शून्य *post, शून्य *data)
{
    ngx_conf_अंक_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    जर (bounds->वरचा == -1) {
        जर (*np >= bounds->खालचा) {
            परत NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->खालचा);

        परत NGX_CONF_ERROR;
    }

    जर (*np >= bounds->खालचा && *np <= bounds->वरचा) {
        परत NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->खालचा, bounds->वरचा);

    परत NGX_CONF_ERROR;
}


अक्षर *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, शून्य *post, शून्य *data)
{
    माप_प्रकार  *sp = data;

    जर (*sp == 0) {
        परत "value must be nonzero";
    }

    परत NGX_CONF_OK;
}
