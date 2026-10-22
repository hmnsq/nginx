
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>

#define NGX_CONF_BUFFER  4096

நிலையான ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
நிலையான ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
நிலையான ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
நிலையான வெறுமை ngx_conf_flush_files(ngx_cycle_t *cycle);


நிலையான ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      வெற்று },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    வெற்று,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    வெற்று,                                  /* init master */
    வெற்று,                                  /* init module */
    வெற்று,                                  /* init process */
    வெற்று,                                  /* init thread */
    வெற்று,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    வெற்று,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

நிலையான ngx_uint_t argument_எண்[] = {
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


எழுத்து *
ngx_conf_param(ngx_conf_t *cf)
{
    எழுத்து             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    எனில் (param->len == 0) {
        திரும்பு NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, அளவு(ngx_conf_file_t));

    ngx_memzero(&b, அளவு(ngx_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.data = வெற்று;
    conf_file.வரி = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    rv = ngx_conf_parse(cf, வெற்று);

    cf->conf_file = வெற்று;

    திரும்பு rv;
}


நிலையான ngx_int_t
ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             size;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_முனை_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_crc32_long(filename->data, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->cycle->config_dump_rbtree, filename, hash);

    எனில் (sn) {
        cf->conf_file->dump = வெற்று;
        திரும்பு NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    எனில் (p == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    cd = ngx_array_தள்ளு(&cf->cycle->config_dump);
    எனில் (cd == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    எனில் (buf == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, அளவு(ngx_str_முனை_t));
    எனில் (sn == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    sn->முனை.சாவி = hash;
    sn->str = cd->name;

    ngx_rbtree_செருகு(&cf->cycle->config_dump_rbtree, &sn->முனை);

    திரும்பு NGX_OK;
}


எழுத்து *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    எழுத்து             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    எண்ணிக்கை {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#எனில் (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = வெற்று;
#endif

    எனில் (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        எனில் (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            திரும்பு NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        எனில் (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        எனில் (buf.start == வெற்று) {
            செல் failed;
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
        cf->conf_file->வரி = 1;

        type = parse_file;

        எனில் (ngx_dump_config
#எனில் (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#எனில் (NGX_DEBUG)
            || 1
#endif
           )
        {
            எனில் (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                செல் failed;
            }

        } இல்லையெனில் {
            cf->conf_file->dump = வெற்று;
        }

    } இல்லையெனில் எனில் (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } இல்லையெனில் {
        type = parse_param;
    }


    க்கு ( ;; ) {
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

        எனில் (rc == NGX_ERROR) {
            செல் done;
        }

        எனில் (rc == NGX_CONF_BLOCK_DONE) {

            எனில் (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                செல் failed;
            }

            செல் done;
        }

        எனில் (rc == NGX_CONF_FILE_DONE) {

            எனில் (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                செல் failed;
            }

            செல் done;
        }

        எனில் (rc == NGX_CONF_BLOCK_START) {

            எனில் (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                செல் failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        எனில் (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            எனில் (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                செல் failed;
            }

            rv = (*cf->handler)(cf, வெற்று, cf->handler_conf);
            எனில் (rv == NGX_CONF_OK) {
                தொடர்;
            }

            எனில் (rv == NGX_CONF_ERROR) {
                செல் failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            செல் failed;
        }


        rc = ngx_conf_handler(cf, rc);

        எனில் (rc == NGX_ERROR) {
            செல் failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    எனில் (filename) {
        எனில் (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        எனில் (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    எனில் (rc == NGX_ERROR) {
        திரும்பு NGX_CONF_ERROR;
    }

    திரும்பு NGX_CONF_OK;
}


நிலையான ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    எழுத்து           *rv;
    வெறுமை           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    க்கு (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        எனில் (cmd == வெற்று) {
            தொடர்;
        }

        க்கு ( /* void */ ; cmd->name.len; cmd++) {

            எனில் (name->len != cmd->name.len) {
                தொடர்;
            }

            எனில் (ngx_strcmp(name->data, cmd->name.data) != 0) {
                தொடர்;
            }

            found = 1;

            எனில் (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                தொடர்;
            }

            /* is the directive's location right ? */

            எனில் (!(cmd->type & cf->cmd_type)) {
                தொடர்;
            }

            எனில் (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                திரும்பு NGX_ERROR;
            }

            எனில் ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                திரும்பு NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            எனில் (!(cmd->type & NGX_CONF_ANY)) {

                எனில் (cmd->type & NGX_CONF_FLAG) {

                    எனில் (cf->args->nelts != 2) {
                        செல் invalid;
                    }

                } இல்லையெனில் எனில் (cmd->type & NGX_CONF_1MORE) {

                    எனில் (cf->args->nelts < 2) {
                        செல் invalid;
                    }

                } இல்லையெனில் எனில் (cmd->type & NGX_CONF_2MORE) {

                    எனில் (cf->args->nelts < 3) {
                        செல் invalid;
                    }

                } இல்லையெனில் எனில் (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    செல் invalid;

                } இல்லையெனில் எனில் (!(cmd->type & argument_எண்[cf->args->nelts - 1]))
                {
                    செல் invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = வெற்று;

            எனில் (cmd->type & NGX_DIRECT_CONF) {
                conf = ((வெறுமை **) cf->ctx)[cf->cycle->modules[i]->index];

            } இல்லையெனில் எனில் (cmd->type & NGX_MAIN_CONF) {
                conf = &(((வெறுமை **) cf->ctx)[cf->cycle->modules[i]->index]);

            } இல்லையெனில் எனில் (cf->ctx) {
                confp = *(வெறுமை **) ((எழுத்து *) cf->ctx + cmd->conf);

                எனில் (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            எனில் (rv == NGX_CONF_OK) {
                திரும்பு NGX_OK;
            }

            எனில் (rv == NGX_CONF_ERROR) {
                திரும்பு NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            திரும்பு NGX_ERROR;
        }
    }

    எனில் (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        திரும்பு NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    திரும்பு NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    திரும்பு NGX_ERROR;
}


நிலையான ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    size_t       len;
    ssize_t      n, size;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_வரி;
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
    start_வரி = cf->conf_file->வரி;

    file_size = ngx_file_size(&cf->conf_file->file.info);

    க்கு ( ;; ) {

        எனில் (b->pos >= b->last) {

            எனில் (cf->conf_file->file.offset >= file_size) {

                எனில் (cf->args->nelts > 0 || !last_space) {

                    எனில் (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        திரும்பு NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    திரும்பு NGX_ERROR;
                }

                திரும்பு NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            எனில் (len == NGX_CONF_BUFFER) {
                cf->conf_file->வரி = start_வரி;

                எனில் (d_quoted) {
                    ch = '"';

                } இல்லையெனில் எனில் (s_quoted) {
                    ch = '\'';

                } இல்லையெனில் {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    திரும்பு NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                திரும்பு NGX_ERROR;
            }

            எனில் (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            எனில் (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            எனில் (n == NGX_ERROR) {
                திரும்பு NGX_ERROR;
            }

            எனில் (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                திரும்பு NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            எனில் (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        எனில் (ch == LF) {
            cf->conf_file->வரி++;

            எனில் (sharp_comment) {
                sharp_comment = 0;
            }
        }

        எனில் (sharp_comment) {
            தொடர்;
        }

        எனில் (quoted) {
            quoted = 0;
            தொடர்;
        }

        எனில் (need_space) {
            எனில் (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                தொடர்;
            }

            எனில் (ch == ';') {
                திரும்பு NGX_OK;
            }

            எனில் (ch == '{') {
                திரும்பு NGX_CONF_BLOCK_START;
            }

            எனில் (ch == ')') {
                last_space = 1;
                need_space = 0;

            } இல்லையெனில் {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                திரும்பு NGX_ERROR;
            }
        }

        எனில் (last_space) {

            start = b->pos - 1;
            start_வரி = cf->conf_file->வரி;

            எனில் (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                தொடர்;
            }

            தேர்வு (ch) {

            நிலை ';':
            நிலை '{':
                எனில் (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    திரும்பு NGX_ERROR;
                }

                எனில் (ch == '{') {
                    திரும்பு NGX_CONF_BLOCK_START;
                }

                திரும்பு NGX_OK;

            நிலை '}':
                எனில் (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    திரும்பு NGX_ERROR;
                }

                திரும்பு NGX_CONF_BLOCK_DONE;

            நிலை '#':
                sharp_comment = 1;
                தொடர்;

            நிலை '\\':
                quoted = 1;
                last_space = 0;
                தொடர்;

            நிலை '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                தொடர்;

            நிலை '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                தொடர்;

            நிலை '$':
                variable = 1;
                last_space = 0;
                தொடர்;

            இயல்பு:
                last_space = 0;
            }

        } இல்லையெனில் {
            எனில் (ch == '{' && variable) {
                தொடர்;
            }

            variable = 0;

            எனில் (ch == '\\') {
                quoted = 1;
                தொடர்;
            }

            எனில் (ch == '$') {
                variable = 1;
                தொடர்;
            }

            எனில் (d_quoted) {
                எனில் (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } இல்லையெனில் எனில் (s_quoted) {
                எனில் (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } இல்லையெனில் எனில் (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            எனில் (found) {
                word = ngx_array_தள்ளு(cf->args);
                எனில் (word == வெற்று) {
                    திரும்பு NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                எனில் (word->data == வெற்று) {
                    திரும்பு NGX_ERROR;
                }

                க்கு (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    எனில் (*src == '\\') {
                        தேர்வு (src[1]) {
                        நிலை '"':
                        நிலை '\'':
                        நிலை '\\':
                            src++;
                            நிறுத்து;

                        நிலை 't':
                            *dst++ = '\t';
                            src += 2;
                            தொடர்;

                        நிலை 'r':
                            *dst++ = '\r';
                            src += 2;
                            தொடர்;

                        நிலை 'n':
                            *dst++ = '\n';
                            src += 2;
                            தொடர்;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                எனில் (ch == ';') {
                    திரும்பு NGX_OK;
                }

                எனில் (ch == '{') {
                    திரும்பு NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


எழுத்து *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து        *rv;
    ngx_int_t    n;
    ngx_str_t   *மதிப்பு, file, name;
    ngx_glob_t   gl;

    மதிப்பு = cf->args->elts;
    file = மதிப்பு[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    எனில் (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        திரும்பு NGX_CONF_ERROR;
    }

    எனில் (strpbrk((எழுத்து *) file.data, "*?[") == வெற்று) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        திரும்பு ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, அளவு(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    எனில் (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        திரும்பு NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    க்கு ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        எனில் (n != NGX_OK) {
            நிறுத்து;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        எனில் (file.data == வெற்று) {
            திரும்பு NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        எனில் (rv != NGX_CONF_OK) {
            நிறுத்து;
        }
    }

    ngx_close_glob(&gl);

    திரும்பு rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    திரும்பு ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#எனில் (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    எனில் (name->len) {
        full = *name;

        எனில் (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            திரும்பு வெற்று;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        க்கு (i = 0; /* void */ ; i++) {

            எனில் (i >= part->nelts) {
                எனில் (part->next == வெற்று) {
                    நிறுத்து;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            எனில் (full.len != file[i].name.len) {
                தொடர்;
            }

            எனில் (ngx_strcmp(full.data, file[i].name.data) == 0) {
                திரும்பு &file[i];
            }
        }
    }

    file = ngx_list_தள்ளு(&cycle->open_files);
    எனில் (file == வெற்று) {
        திரும்பு வெற்று;
    }

    எனில் (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } இல்லையெனில் {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = வெற்று;
    file->data = வெற்று;

    திரும்பு file;
}


நிலையான வெறுமை
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    க்கு (i = 0; /* void */ ; i++) {

        எனில் (i >= part->nelts) {
            எனில் (part->next == வெற்று) {
                நிறுத்து;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        எனில் (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


வெறுமை ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    மாறிலி எழுத்து *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    va_list  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    va_end(args);

    எனில் (err) {
        p = ngx_log_errno(p, last, err);
    }

    எனில் (cf->conf_file == வெற்று) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        திரும்பு;
    }

    எனில் (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        திரும்பு;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->வரி);
}


எழுத்து *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து  *p = conf;

    ngx_str_t        *மதிப்பு;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    எனில் (*fp != NGX_CONF_UNSET) {
        திரும்பு "is duplicate";
    }

    மதிப்பு = cf->args->elts;

    எனில் (ngx_strcasecmp(மதிப்பு[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } இல்லையெனில் எனில் (ngx_strcasecmp(மதிப்பு[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } இல்லையெனில் {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     மதிப்பு[1].data, cmd->name.data);
        திரும்பு NGX_CONF_ERROR;
    }

    எனில் (cmd->post) {
        post = cmd->post;
        திரும்பு post->post_handler(cf, post, fp);
    }

    திரும்பு NGX_CONF_OK;
}


எழுத்து *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து  *p = conf;

    ngx_str_t        *field, *மதிப்பு;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    எனில் (field->data) {
        திரும்பு "is duplicate";
    }

    மதிப்பு = cf->args->elts;

    *field = மதிப்பு[1];

    எனில் (cmd->post) {
        post = cmd->post;
        திரும்பு post->post_handler(cf, post, field);
    }

    திரும்பு NGX_CONF_OK;
}


எழுத்து *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து  *p = conf;

    ngx_str_t         *மதிப்பு, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    எனில் (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, அளவு(ngx_str_t));
        எனில் (*a == வெற்று) {
            திரும்பு NGX_CONF_ERROR;
        }
    }

    s = ngx_array_தள்ளு(*a);
    எனில் (s == வெற்று) {
        திரும்பு NGX_CONF_ERROR;
    }

    மதிப்பு = cf->args->elts;

    *s = மதிப்பு[1];

    எனில் (cmd->post) {
        post = cmd->post;
        திரும்பு post->post_handler(cf, post, s);
    }

    திரும்பு NGX_CONF_OK;
}


எழுத்து *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து  *p = conf;

    ngx_str_t         *மதிப்பு;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    எனில் (*a == NGX_CONF_UNSET_PTR || *a == வெற்று) {
        *a = ngx_array_create(cf->pool, 4, அளவு(ngx_keyval_t));
        எனில் (*a == வெற்று) {
            திரும்பு NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_தள்ளு(*a);
    எனில் (kv == வெற்று) {
        திரும்பு NGX_CONF_ERROR;
    }

    மதிப்பு = cf->args->elts;

    kv->சாவி = மதிப்பு[1];
    kv->மதிப்பு = மதிப்பு[2];

    எனில் (cmd->post) {
        post = cmd->post;
        திரும்பு post->post_handler(cf, post, kv);
    }

    திரும்பு NGX_CONF_OK;
}


எழுத்து *
ngx_conf_set_இலக்கம்_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *மதிப்பு;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    எனில் (*np != NGX_CONF_UNSET) {
        திரும்பு "is duplicate";
    }

    மதிப்பு = cf->args->elts;
    *np = ngx_atoi(மதிப்பு[1].data, மதிப்பு[1].len);
    எனில் (*np == NGX_ERROR) {
        திரும்பு "invalid number";
    }

    எனில் (cmd->post) {
        post = cmd->post;
        திரும்பு post->post_handler(cf, post, np);
    }

    திரும்பு NGX_CONF_OK;
}


எழுத்து *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து  *p = conf;

    size_t           *sp;
    ngx_str_t        *மதிப்பு;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    எனில் (*sp != NGX_CONF_UNSET_SIZE) {
        திரும்பு "is duplicate";
    }

    மதிப்பு = cf->args->elts;

    *sp = ngx_parse_size(&மதிப்பு[1]);
    எனில் (*sp == (size_t) NGX_ERROR) {
        திரும்பு "invalid value";
    }

    எனில் (cmd->post) {
        post = cmd->post;
        திரும்பு post->post_handler(cf, post, sp);
    }

    திரும்பு NGX_CONF_OK;
}


எழுத்து *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து  *p = conf;

    off_t            *op;
    ngx_str_t        *மதிப்பு;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    எனில் (*op != NGX_CONF_UNSET) {
        திரும்பு "is duplicate";
    }

    மதிப்பு = cf->args->elts;

    *op = ngx_parse_offset(&மதிப்பு[1]);
    எனில் (*op == (off_t) NGX_ERROR) {
        திரும்பு "invalid value";
    }

    எனில் (cmd->post) {
        post = cmd->post;
        திரும்பு post->post_handler(cf, post, op);
    }

    திரும்பு NGX_CONF_OK;
}


எழுத்து *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *மதிப்பு;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    எனில் (*msp != NGX_CONF_UNSET_MSEC) {
        திரும்பு "is duplicate";
    }

    மதிப்பு = cf->args->elts;

    *msp = ngx_parse_time(&மதிப்பு[1], 0);
    எனில் (*msp == (ngx_msec_t) NGX_ERROR) {
        திரும்பு "invalid value";
    }

    எனில் (cmd->post) {
        post = cmd->post;
        திரும்பு post->post_handler(cf, post, msp);
    }

    திரும்பு NGX_CONF_OK;
}


எழுத்து *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து  *p = conf;

    time_t           *sp;
    ngx_str_t        *மதிப்பு;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    எனில் (*sp != NGX_CONF_UNSET) {
        திரும்பு "is duplicate";
    }

    மதிப்பு = cf->args->elts;

    *sp = ngx_parse_time(&மதிப்பு[1], 1);
    எனில் (*sp == (time_t) NGX_ERROR) {
        திரும்பு "invalid value";
    }

    எனில் (cmd->post) {
        post = cmd->post;
        திரும்பு post->post_handler(cf, post, sp);
    }

    திரும்பு NGX_CONF_OK;
}


எழுத்து *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து *p = conf;

    ngx_str_t   *மதிப்பு;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    எனில் (bufs->இலக்கம்) {
        திரும்பு "is duplicate";
    }

    மதிப்பு = cf->args->elts;

    bufs->இலக்கம் = ngx_atoi(மதிப்பு[1].data, மதிப்பு[1].len);
    எனில் (bufs->இலக்கம் == NGX_ERROR || bufs->இலக்கம் == 0) {
        திரும்பு "invalid value";
    }

    bufs->size = ngx_parse_size(&மதிப்பு[2]);
    எனில் (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        திரும்பு "invalid value";
    }

    திரும்பு NGX_CONF_OK;
}


எழுத்து *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *மதிப்பு;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    எனில் (*np != NGX_CONF_UNSET_UINT) {
        திரும்பு "is duplicate";
    }

    மதிப்பு = cf->args->elts;
    e = cmd->post;

    க்கு (i = 0; e[i].name.len != 0; i++) {
        எனில் (e[i].name.len != மதிப்பு[1].len
            || ngx_strcasecmp(e[i].name.data, மதிப்பு[1].data) != 0)
        {
            தொடர்;
        }

        *np = e[i].மதிப்பு;

        திரும்பு NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", மதிப்பு[1].data);

    திரும்பு NGX_CONF_ERROR;
}


எழுத்து *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *மதிப்பு;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    மதிப்பு = cf->args->elts;
    mask = cmd->post;

    க்கு (i = 1; i < cf->args->nelts; i++) {
        க்கு (m = 0; mask[m].name.len != 0; m++) {

            எனில் (mask[m].name.len != மதிப்பு[i].len
                || ngx_strcasecmp(mask[m].name.data, மதிப்பு[i].data) != 0)
            {
                தொடர்;
            }

            எனில் (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", மதிப்பு[i].data);

            } இல்லையெனில் {
                *np |= mask[m].mask;
            }

            நிறுத்து;
        }

        எனில் (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", மதிப்பு[i].data);

            திரும்பு NGX_CONF_ERROR;
        }
    }

    திரும்பு NGX_CONF_OK;
}


#எனில் 0

எழுத்து *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    திரும்பு "unsupported on this platform";
}

#endif


எழுத்து *
ngx_conf_deprecated(ngx_conf_t *cf, வெறுமை *post, வெறுமை *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    திரும்பு NGX_CONF_OK;
}


எழுத்து *
ngx_conf_check_இலக்கம்_bounds(ngx_conf_t *cf, வெறுமை *post, வெறுமை *data)
{
    ngx_conf_இலக்கம்_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    எனில் (bounds->உயர்வு == -1) {
        எனில் (*np >= bounds->கீழ்) {
            திரும்பு NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->கீழ்);

        திரும்பு NGX_CONF_ERROR;
    }

    எனில் (*np >= bounds->கீழ் && *np <= bounds->உயர்வு) {
        திரும்பு NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->கீழ், bounds->உயர்வு);

    திரும்பு NGX_CONF_ERROR;
}


எழுத்து *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, வெறுமை *post, வெறுமை *data)
{
    size_t  *sp = data;

    எனில் (*sp == 0) {
        திரும்பு "value must be nonzero";
    }

    திரும்பு NGX_CONF_OK;
}
