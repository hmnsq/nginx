
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>

#define NGX_CONF_BUFFER  4096

ساكن ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
ساكن ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
ساكن ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
ساكن فراغ ngx_conf_flush_files(ngx_cycle_t *cycle);


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

ساكن ngx_uint_t argument_number[] = {
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


حرف *
ngx_conf_param(ngx_conf_t *cf)
{
    حرف             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    אם (param->len == 0) {
        החזר NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, حجم(ngx_conf_file_t));

    ngx_memzero(&b, حجم(ngx_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.data = عدم;
    conf_file.line = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    rv = ngx_conf_parse(cf, عدم);

    cf->conf_file = عدم;

    החזר rv;
}


ساكن ngx_int_t
ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             size;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_node_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_crc32_long(filename->data, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->cycle->config_dump_rbtree, filename, hash);

    אם (sn) {
        cf->conf_file->dump = عدم;
        החזר NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    אם (p == عدم) {
        החזר NGX_ERROR;
    }

    cd = ngx_array_push(&cf->cycle->config_dump);
    אם (cd == عدم) {
        החזר NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    אם (buf == عدم) {
        החזר NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, حجم(ngx_str_node_t));
    אם (sn == عدم) {
        החזר NGX_ERROR;
    }

    sn->node.key = hash;
    sn->str = cd->name;

    ngx_rbtree_insert(&cf->cycle->config_dump_rbtree, &sn->node);

    החזר NGX_OK;
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
        parse_block,
        parse_param
    } type;

#אם (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = عدم;
#endif

    אם (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        אם (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            החזר NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        אם (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        אם (buf.start == عدم) {
            اذهب failed;
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
        cf->conf_file->line = 1;

        type = parse_file;

        אם (ngx_dump_config
#אם (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#אם (NGX_DEBUG)
            || 1
#endif
           )
        {
            אם (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                اذهب failed;
            }

        } אחרת {
            cf->conf_file->dump = عدم;
        }

    } אחרת אם (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } אחרת {
        type = parse_param;
    }


    עבור ( ;; ) {
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

        אם (rc == NGX_ERROR) {
            اذهب done;
        }

        אם (rc == NGX_CONF_BLOCK_DONE) {

            אם (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                اذهب failed;
            }

            اذهب done;
        }

        אם (rc == NGX_CONF_FILE_DONE) {

            אם (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                اذهب failed;
            }

            اذهب done;
        }

        אם (rc == NGX_CONF_BLOCK_START) {

            אם (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                اذهب failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        אם (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            אם (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                اذهب failed;
            }

            rv = (*cf->handler)(cf, عدم, cf->handler_conf);
            אם (rv == NGX_CONF_OK) {
                استمر;
            }

            אם (rv == NGX_CONF_ERROR) {
                اذهب failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            اذهب failed;
        }


        rc = ngx_conf_handler(cf, rc);

        אם (rc == NGX_ERROR) {
            اذهب failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    אם (filename) {
        אם (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        אם (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    אם (rc == NGX_ERROR) {
        החזר NGX_CONF_ERROR;
    }

    החזר NGX_CONF_OK;
}


ساكن ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    حرف           *rv;
    فراغ           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    עבור (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        אם (cmd == عدم) {
            استمر;
        }

        עבור ( /* void */ ; cmd->name.len; cmd++) {

            אם (name->len != cmd->name.len) {
                استمر;
            }

            אם (ngx_strcmp(name->data, cmd->name.data) != 0) {
                استمر;
            }

            found = 1;

            אם (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                استمر;
            }

            /* is the directive's location right ? */

            אם (!(cmd->type & cf->cmd_type)) {
                استمر;
            }

            אם (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                החזר NGX_ERROR;
            }

            אם ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                החזר NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            אם (!(cmd->type & NGX_CONF_ANY)) {

                אם (cmd->type & NGX_CONF_FLAG) {

                    אם (cf->args->nelts != 2) {
                        اذهب invalid;
                    }

                } אחרת אם (cmd->type & NGX_CONF_1MORE) {

                    אם (cf->args->nelts < 2) {
                        اذهب invalid;
                    }

                } אחרת אם (cmd->type & NGX_CONF_2MORE) {

                    אם (cf->args->nelts < 3) {
                        اذهب invalid;
                    }

                } אחרת אם (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    اذهب invalid;

                } אחרת אם (!(cmd->type & argument_number[cf->args->nelts - 1]))
                {
                    اذهب invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = عدم;

            אם (cmd->type & NGX_DIRECT_CONF) {
                conf = ((فراغ **) cf->ctx)[cf->cycle->modules[i]->index];

            } אחרת אם (cmd->type & NGX_MAIN_CONF) {
                conf = &(((فراغ **) cf->ctx)[cf->cycle->modules[i]->index]);

            } אחרת אם (cf->ctx) {
                confp = *(فراغ **) ((حرف *) cf->ctx + cmd->conf);

                אם (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            אם (rv == NGX_CONF_OK) {
                החזר NGX_OK;
            }

            אם (rv == NGX_CONF_ERROR) {
                החזר NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            החזר NGX_ERROR;
        }
    }

    אם (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        החזר NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    החזר NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    החזר NGX_ERROR;
}


ساكن ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    size_t       len;
    ssize_t      n, size;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_line;
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
    start_line = cf->conf_file->line;

    file_size = ngx_file_size(&cf->conf_file->file.info);

    עבור ( ;; ) {

        אם (b->pos >= b->last) {

            אם (cf->conf_file->file.offset >= file_size) {

                אם (cf->args->nelts > 0 || !last_space) {

                    אם (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        החזר NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    החזר NGX_ERROR;
                }

                החזר NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            אם (len == NGX_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                אם (d_quoted) {
                    ch = '"';

                } אחרת אם (s_quoted) {
                    ch = '\'';

                } אחרת {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    החזר NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                החזר NGX_ERROR;
            }

            אם (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            אם (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            אם (n == NGX_ERROR) {
                החזר NGX_ERROR;
            }

            אם (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                החזר NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            אם (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        אם (ch == LF) {
            cf->conf_file->line++;

            אם (sharp_comment) {
                sharp_comment = 0;
            }
        }

        אם (sharp_comment) {
            استمر;
        }

        אם (quoted) {
            quoted = 0;
            استمر;
        }

        אם (need_space) {
            אם (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                استمر;
            }

            אם (ch == ';') {
                החזר NGX_OK;
            }

            אם (ch == '{') {
                החזר NGX_CONF_BLOCK_START;
            }

            אם (ch == ')') {
                last_space = 1;
                need_space = 0;

            } אחרת {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                החזר NGX_ERROR;
            }
        }

        אם (last_space) {

            start = b->pos - 1;
            start_line = cf->conf_file->line;

            אם (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                استمر;
            }

            بدل (ch) {

            حالة ';':
            حالة '{':
                אם (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    החזר NGX_ERROR;
                }

                אם (ch == '{') {
                    החזר NGX_CONF_BLOCK_START;
                }

                החזר NGX_OK;

            حالة '}':
                אם (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    החזר NGX_ERROR;
                }

                החזר NGX_CONF_BLOCK_DONE;

            حالة '#':
                sharp_comment = 1;
                استمر;

            حالة '\\':
                quoted = 1;
                last_space = 0;
                استمر;

            حالة '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                استمر;

            حالة '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                استمر;

            حالة '$':
                variable = 1;
                last_space = 0;
                استمر;

            افتراضي:
                last_space = 0;
            }

        } אחרת {
            אם (ch == '{' && variable) {
                استمر;
            }

            variable = 0;

            אם (ch == '\\') {
                quoted = 1;
                استمر;
            }

            אם (ch == '$') {
                variable = 1;
                استمر;
            }

            אם (d_quoted) {
                אם (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } אחרת אם (s_quoted) {
                אם (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } אחרת אם (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            אם (found) {
                word = ngx_array_push(cf->args);
                אם (word == عدم) {
                    החזר NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                אם (word->data == عدم) {
                    החזר NGX_ERROR;
                }

                עבור (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    אם (*src == '\\') {
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
                word->len = len;

                אם (ch == ';') {
                    החזר NGX_OK;
                }

                אם (ch == '{') {
                    החזר NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


حرف *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف        *rv;
    ngx_int_t    n;
    ngx_str_t   *value, file, name;
    ngx_glob_t   gl;

    value = cf->args->elts;
    file = value[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    אם (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        החזר NGX_CONF_ERROR;
    }

    אם (strpbrk((حرف *) file.data, "*?[") == عدم) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        החזר ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, حجم(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    אם (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        החזר NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    עבור ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        אם (n != NGX_OK) {
            اكسر;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        אם (file.data == عدم) {
            החזר NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        אם (rv != NGX_CONF_OK) {
            اكسر;
        }
    }

    ngx_close_glob(&gl);

    החזר rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    החזר ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#אם (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    אם (name->len) {
        full = *name;

        אם (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            החזר عدم;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        עבור (i = 0; /* void */ ; i++) {

            אם (i >= part->nelts) {
                אם (part->next == عدم) {
                    اكسر;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            אם (full.len != file[i].name.len) {
                استمر;
            }

            אם (ngx_strcmp(full.data, file[i].name.data) == 0) {
                החזר &file[i];
            }
        }
    }

    file = ngx_list_push(&cycle->open_files);
    אם (file == عدم) {
        החזר عدم;
    }

    אם (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } אחרת {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = عدم;
    file->data = عدم;

    החזר file;
}


ساكن فراغ
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    עבור (i = 0; /* void */ ; i++) {

        אם (i >= part->nelts) {
            אם (part->next == عدم) {
                اكسر;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        אם (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


فراغ ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    ثابت حرف *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    va_list  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    va_end(args);

    אם (err) {
        p = ngx_log_errno(p, last, err);
    }

    אם (cf->conf_file == عدم) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        החזר;
    }

    אם (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        החזר;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->line);
}


حرف *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t        *value;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    אם (*fp != NGX_CONF_UNSET) {
        החזר "is duplicate";
    }

    value = cf->args->elts;

    אם (ngx_strcasecmp(value[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } אחרת אם (ngx_strcasecmp(value[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } אחרת {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     value[1].data, cmd->name.data);
        החזר NGX_CONF_ERROR;
    }

    אם (cmd->post) {
        post = cmd->post;
        החזר post->post_handler(cf, post, fp);
    }

    החזר NGX_CONF_OK;
}


حرف *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t        *field, *value;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    אם (field->data) {
        החזר "is duplicate";
    }

    value = cf->args->elts;

    *field = value[1];

    אם (cmd->post) {
        post = cmd->post;
        החזר post->post_handler(cf, post, field);
    }

    החזר NGX_CONF_OK;
}


حرف *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t         *value, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    אם (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_str_t));
        אם (*a == عدم) {
            החזר NGX_CONF_ERROR;
        }
    }

    s = ngx_array_push(*a);
    אם (s == عدم) {
        החזר NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    *s = value[1];

    אם (cmd->post) {
        post = cmd->post;
        החזר post->post_handler(cf, post, s);
    }

    החזר NGX_CONF_OK;
}


حرف *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_str_t         *value;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    אם (*a == NGX_CONF_UNSET_PTR || *a == عدم) {
        *a = ngx_array_create(cf->pool, 4, حجم(ngx_keyval_t));
        אם (*a == عدم) {
            החזר NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_push(*a);
    אם (kv == عدم) {
        החזר NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    kv->key = value[1];
    kv->value = value[2];

    אם (cmd->post) {
        post = cmd->post;
        החזר post->post_handler(cf, post, kv);
    }

    החזר NGX_CONF_OK;
}


حرف *
ngx_conf_set_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    אם (*np != NGX_CONF_UNSET) {
        החזר "is duplicate";
    }

    value = cf->args->elts;
    *np = ngx_atoi(value[1].data, value[1].len);
    אם (*np == NGX_ERROR) {
        החזר "invalid number";
    }

    אם (cmd->post) {
        post = cmd->post;
        החזר post->post_handler(cf, post, np);
    }

    החזר NGX_CONF_OK;
}


حرف *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    size_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    אם (*sp != NGX_CONF_UNSET_SIZE) {
        החזר "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_size(&value[1]);
    אם (*sp == (size_t) NGX_ERROR) {
        החזר "invalid value";
    }

    אם (cmd->post) {
        post = cmd->post;
        החזר post->post_handler(cf, post, sp);
    }

    החזר NGX_CONF_OK;
}


حرف *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    off_t            *op;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    אם (*op != NGX_CONF_UNSET) {
        החזר "is duplicate";
    }

    value = cf->args->elts;

    *op = ngx_parse_offset(&value[1]);
    אם (*op == (off_t) NGX_ERROR) {
        החזר "invalid value";
    }

    אם (cmd->post) {
        post = cmd->post;
        החזר post->post_handler(cf, post, op);
    }

    החזר NGX_CONF_OK;
}


حرف *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    אם (*msp != NGX_CONF_UNSET_MSEC) {
        החזר "is duplicate";
    }

    value = cf->args->elts;

    *msp = ngx_parse_time(&value[1], 0);
    אם (*msp == (ngx_msec_t) NGX_ERROR) {
        החזר "invalid value";
    }

    אם (cmd->post) {
        post = cmd->post;
        החזר post->post_handler(cf, post, msp);
    }

    החזר NGX_CONF_OK;
}


حرف *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    time_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    אם (*sp != NGX_CONF_UNSET) {
        החזר "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_time(&value[1], 1);
    אם (*sp == (time_t) NGX_ERROR) {
        החזר "invalid value";
    }

    אם (cmd->post) {
        post = cmd->post;
        החזר post->post_handler(cf, post, sp);
    }

    החזר NGX_CONF_OK;
}


حرف *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف *p = conf;

    ngx_str_t   *value;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    אם (bufs->num) {
        החזר "is duplicate";
    }

    value = cf->args->elts;

    bufs->num = ngx_atoi(value[1].data, value[1].len);
    אם (bufs->num == NGX_ERROR || bufs->num == 0) {
        החזר "invalid value";
    }

    bufs->size = ngx_parse_size(&value[2]);
    אם (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        החזר "invalid value";
    }

    החזר NGX_CONF_OK;
}


حرف *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *value;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    אם (*np != NGX_CONF_UNSET_UINT) {
        החזר "is duplicate";
    }

    value = cf->args->elts;
    e = cmd->post;

    עבור (i = 0; e[i].name.len != 0; i++) {
        אם (e[i].name.len != value[1].len
            || ngx_strcasecmp(e[i].name.data, value[1].data) != 0)
        {
            استمر;
        }

        *np = e[i].value;

        החזר NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", value[1].data);

    החזר NGX_CONF_ERROR;
}


حرف *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *value;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    value = cf->args->elts;
    mask = cmd->post;

    עבור (i = 1; i < cf->args->nelts; i++) {
        עבור (m = 0; mask[m].name.len != 0; m++) {

            אם (mask[m].name.len != value[i].len
                || ngx_strcasecmp(mask[m].name.data, value[i].data) != 0)
            {
                استمر;
            }

            אם (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", value[i].data);

            } אחרת {
                *np |= mask[m].mask;
            }

            اكسر;
        }

        אם (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", value[i].data);

            החזר NGX_CONF_ERROR;
        }
    }

    החזר NGX_CONF_OK;
}


#אם 0

حرف *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    החזר "unsupported on this platform";
}

#endif


حرف *
ngx_conf_deprecated(ngx_conf_t *cf, فراغ *post, فراغ *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    החזר NGX_CONF_OK;
}


حرف *
ngx_conf_check_num_bounds(ngx_conf_t *cf, فراغ *post, فراغ *data)
{
    ngx_conf_num_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    אם (bounds->high == -1) {
        אם (*np >= bounds->low) {
            החזר NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->low);

        החזר NGX_CONF_ERROR;
    }

    אם (*np >= bounds->low && *np <= bounds->high) {
        החזר NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->low, bounds->high);

    החזר NGX_CONF_ERROR;
}


حرف *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, فراغ *post, فراغ *data)
{
    size_t  *sp = data;

    אם (*sp == 0) {
        החזר "value must be nonzero";
    }

    החזר NGX_CONF_OK;
}
