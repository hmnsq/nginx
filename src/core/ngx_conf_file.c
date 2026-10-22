
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>

#define NGX_CONF_BUFFER  4096

સ્થિર ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
સ્થિર ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
સ્થિર ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
સ્થિર શૂન્ય ngx_conf_flush_files(ngx_cycle_t *cycle);


સ્થિર ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      ખાલી },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    ખાલી,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    ખાલી,                                  /* init master */
    ખાલી,                                  /* init module */
    ખાલી,                                  /* init process */
    ખાલી,                                  /* init thread */
    ખાલી,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    ખાલી,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

સ્થિર ngx_uint_t argument_સંખ્યા[] = {
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


અક્ષર *
ngx_conf_param(ngx_conf_t *cf)
{
    અક્ષર             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    જો (param->len == 0) {
        પાછા NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, કદ(ngx_conf_file_t));

    ngx_memzero(&b, કદ(ngx_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.data = ખાલી;
    conf_file.લીટી = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    rv = ngx_conf_parse(cf, ખાલી);

    cf->conf_file = ખાલી;

    પાછા rv;
}


સ્થિર ngx_int_t
ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename)
{
    off_t             size;
    u_char           *p;
    uint32_t          hash;
    ngx_buf_t        *buf;
    ngx_str_ગાંઠ_t   *sn;
    ngx_conf_dump_t  *cd;

    hash = ngx_crc32_long(filename->data, filename->len);

    sn = ngx_str_rbtree_lookup(&cf->cycle->config_dump_rbtree, filename, hash);

    જો (sn) {
        cf->conf_file->dump = ખાલી;
        પાછા NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    જો (p == ખાલી) {
        પાછા NGX_ERROR;
    }

    cd = ngx_array_ધકેલો(&cf->cycle->config_dump);
    જો (cd == ખાલી) {
        પાછા NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    જો (buf == ખાલી) {
        પાછા NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, કદ(ngx_str_ગાંઠ_t));
    જો (sn == ખાલી) {
        પાછા NGX_ERROR;
    }

    sn->ગાંઠ.ચાવી = hash;
    sn->str = cd->name;

    ngx_rbtree_ઉમેરો(&cf->cycle->config_dump_rbtree, &sn->ગાંઠ);

    પાછા NGX_OK;
}


અક્ષર *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    અક્ષર             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    ગણના {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#જો (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = ખાલી;
#endif

    જો (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        જો (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            પાછા NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        જો (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        જો (buf.start == ખાલી) {
            જાઓ failed;
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
        cf->conf_file->લીટી = 1;

        type = parse_file;

        જો (ngx_dump_config
#જો (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#જો (NGX_DEBUG)
            || 1
#endif
           )
        {
            જો (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                જાઓ failed;
            }

        } નહીંતર {
            cf->conf_file->dump = ખાલી;
        }

    } નહીંતર જો (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } નહીંતર {
        type = parse_param;
    }


    માટે ( ;; ) {
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

        જો (rc == NGX_ERROR) {
            જાઓ done;
        }

        જો (rc == NGX_CONF_BLOCK_DONE) {

            જો (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                જાઓ failed;
            }

            જાઓ done;
        }

        જો (rc == NGX_CONF_FILE_DONE) {

            જો (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                જાઓ failed;
            }

            જાઓ done;
        }

        જો (rc == NGX_CONF_BLOCK_START) {

            જો (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                જાઓ failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        જો (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            જો (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                જાઓ failed;
            }

            rv = (*cf->handler)(cf, ખાલી, cf->handler_conf);
            જો (rv == NGX_CONF_OK) {
                ચાલુ;
            }

            જો (rv == NGX_CONF_ERROR) {
                જાઓ failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            જાઓ failed;
        }


        rc = ngx_conf_handler(cf, rc);

        જો (rc == NGX_ERROR) {
            જાઓ failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    જો (filename) {
        જો (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        જો (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    જો (rc == NGX_ERROR) {
        પાછા NGX_CONF_ERROR;
    }

    પાછા NGX_CONF_OK;
}


સ્થિર ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    અક્ષર           *rv;
    શૂન્ય           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    માટે (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        જો (cmd == ખાલી) {
            ચાલુ;
        }

        માટે ( /* void */ ; cmd->name.len; cmd++) {

            જો (name->len != cmd->name.len) {
                ચાલુ;
            }

            જો (ngx_strcmp(name->data, cmd->name.data) != 0) {
                ચાલુ;
            }

            found = 1;

            જો (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                ચાલુ;
            }

            /* is the directive's location right ? */

            જો (!(cmd->type & cf->cmd_type)) {
                ચાલુ;
            }

            જો (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                પાછા NGX_ERROR;
            }

            જો ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                પાછા NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            જો (!(cmd->type & NGX_CONF_ANY)) {

                જો (cmd->type & NGX_CONF_FLAG) {

                    જો (cf->args->nelts != 2) {
                        જાઓ invalid;
                    }

                } નહીંતર જો (cmd->type & NGX_CONF_1MORE) {

                    જો (cf->args->nelts < 2) {
                        જાઓ invalid;
                    }

                } નહીંતર જો (cmd->type & NGX_CONF_2MORE) {

                    જો (cf->args->nelts < 3) {
                        જાઓ invalid;
                    }

                } નહીંતર જો (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    જાઓ invalid;

                } નહીંતર જો (!(cmd->type & argument_સંખ્યા[cf->args->nelts - 1]))
                {
                    જાઓ invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = ખાલી;

            જો (cmd->type & NGX_DIRECT_CONF) {
                conf = ((શૂન્ય **) cf->ctx)[cf->cycle->modules[i]->index];

            } નહીંતર જો (cmd->type & NGX_MAIN_CONF) {
                conf = &(((શૂન્ય **) cf->ctx)[cf->cycle->modules[i]->index]);

            } નહીંતર જો (cf->ctx) {
                confp = *(શૂન્ય **) ((અક્ષર *) cf->ctx + cmd->conf);

                જો (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            જો (rv == NGX_CONF_OK) {
                પાછા NGX_OK;
            }

            જો (rv == NGX_CONF_ERROR) {
                પાછા NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            પાછા NGX_ERROR;
        }
    }

    જો (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        પાછા NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    પાછા NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    પાછા NGX_ERROR;
}


સ્થિર ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    size_t       len;
    ssize_t      n, size;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_લીટી;
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
    start_લીટી = cf->conf_file->લીટી;

    file_size = ngx_file_size(&cf->conf_file->file.info);

    માટે ( ;; ) {

        જો (b->pos >= b->last) {

            જો (cf->conf_file->file.offset >= file_size) {

                જો (cf->args->nelts > 0 || !last_space) {

                    જો (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        પાછા NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    પાછા NGX_ERROR;
                }

                પાછા NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            જો (len == NGX_CONF_BUFFER) {
                cf->conf_file->લીટી = start_લીટી;

                જો (d_quoted) {
                    ch = '"';

                } નહીંતર જો (s_quoted) {
                    ch = '\'';

                } નહીંતર {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    પાછા NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                પાછા NGX_ERROR;
            }

            જો (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            જો (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            જો (n == NGX_ERROR) {
                પાછા NGX_ERROR;
            }

            જો (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                પાછા NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            જો (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        જો (ch == LF) {
            cf->conf_file->લીટી++;

            જો (sharp_comment) {
                sharp_comment = 0;
            }
        }

        જો (sharp_comment) {
            ચાલુ;
        }

        જો (quoted) {
            quoted = 0;
            ચાલુ;
        }

        જો (need_space) {
            જો (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                ચાલુ;
            }

            જો (ch == ';') {
                પાછા NGX_OK;
            }

            જો (ch == '{') {
                પાછા NGX_CONF_BLOCK_START;
            }

            જો (ch == ')') {
                last_space = 1;
                need_space = 0;

            } નહીંતર {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                પાછા NGX_ERROR;
            }
        }

        જો (last_space) {

            start = b->pos - 1;
            start_લીટી = cf->conf_file->લીટી;

            જો (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                ચાલુ;
            }

            પસંદગી (ch) {

            સ્થિતિ ';':
            સ્થિતિ '{':
                જો (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    પાછા NGX_ERROR;
                }

                જો (ch == '{') {
                    પાછા NGX_CONF_BLOCK_START;
                }

                પાછા NGX_OK;

            સ્થિતિ '}':
                જો (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    પાછા NGX_ERROR;
                }

                પાછા NGX_CONF_BLOCK_DONE;

            સ્થિતિ '#':
                sharp_comment = 1;
                ચાલુ;

            સ્થિતિ '\\':
                quoted = 1;
                last_space = 0;
                ચાલુ;

            સ્થિતિ '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                ચાલુ;

            સ્થિતિ '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                ચાલુ;

            સ્થિતિ '$':
                variable = 1;
                last_space = 0;
                ચાલુ;

            મૂળભૂત:
                last_space = 0;
            }

        } નહીંતર {
            જો (ch == '{' && variable) {
                ચાલુ;
            }

            variable = 0;

            જો (ch == '\\') {
                quoted = 1;
                ચાલુ;
            }

            જો (ch == '$') {
                variable = 1;
                ચાલુ;
            }

            જો (d_quoted) {
                જો (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } નહીંતર જો (s_quoted) {
                જો (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } નહીંતર જો (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            જો (found) {
                word = ngx_array_ધકેલો(cf->args);
                જો (word == ખાલી) {
                    પાછા NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                જો (word->data == ખાલી) {
                    પાછા NGX_ERROR;
                }

                માટે (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    જો (*src == '\\') {
                        પસંદગી (src[1]) {
                        સ્થિતિ '"':
                        સ્થિતિ '\'':
                        સ્થિતિ '\\':
                            src++;
                            તોડો;

                        સ્થિતિ 't':
                            *dst++ = '\t';
                            src += 2;
                            ચાલુ;

                        સ્થિતિ 'r':
                            *dst++ = '\r';
                            src += 2;
                            ચાલુ;

                        સ્થિતિ 'n':
                            *dst++ = '\n';
                            src += 2;
                            ચાલુ;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                જો (ch == ';') {
                    પાછા NGX_OK;
                }

                જો (ch == '{') {
                    પાછા NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


અક્ષર *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર        *rv;
    ngx_int_t    n;
    ngx_str_t   *કિંમત, file, name;
    ngx_glob_t   gl;

    કિંમત = cf->args->elts;
    file = કિંમત[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    જો (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        પાછા NGX_CONF_ERROR;
    }

    જો (strpbrk((અક્ષર *) file.data, "*?[") == ખાલી) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        પાછા ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, કદ(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    જો (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        પાછા NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    માટે ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        જો (n != NGX_OK) {
            તોડો;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        જો (file.data == ખાલી) {
            પાછા NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        જો (rv != NGX_CONF_OK) {
            તોડો;
        }
    }

    ngx_close_glob(&gl);

    પાછા rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    પાછા ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#જો (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    જો (name->len) {
        full = *name;

        જો (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            પાછા ખાલી;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        માટે (i = 0; /* void */ ; i++) {

            જો (i >= part->nelts) {
                જો (part->next == ખાલી) {
                    તોડો;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            જો (full.len != file[i].name.len) {
                ચાલુ;
            }

            જો (ngx_strcmp(full.data, file[i].name.data) == 0) {
                પાછા &file[i];
            }
        }
    }

    file = ngx_list_ધકેલો(&cycle->open_files);
    જો (file == ખાલી) {
        પાછા ખાલી;
    }

    જો (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } નહીંતર {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = ખાલી;
    file->data = ખાલી;

    પાછા file;
}


સ્થિર શૂન્ય
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    માટે (i = 0; /* void */ ; i++) {

        જો (i >= part->nelts) {
            જો (part->next == ખાલી) {
                તોડો;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        જો (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


શૂન્ય ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    અચલ અક્ષર *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    va_list  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    va_end(args);

    જો (err) {
        p = ngx_log_errno(p, last, err);
    }

    જો (cf->conf_file == ખાલી) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        પાછા;
    }

    જો (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        પાછા;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->લીટી);
}


અક્ષર *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર  *p = conf;

    ngx_str_t        *કિંમત;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    જો (*fp != NGX_CONF_UNSET) {
        પાછા "is duplicate";
    }

    કિંમત = cf->args->elts;

    જો (ngx_strcasecmp(કિંમત[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } નહીંતર જો (ngx_strcasecmp(કિંમત[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } નહીંતર {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     કિંમત[1].data, cmd->name.data);
        પાછા NGX_CONF_ERROR;
    }

    જો (cmd->post) {
        post = cmd->post;
        પાછા post->post_handler(cf, post, fp);
    }

    પાછા NGX_CONF_OK;
}


અક્ષર *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર  *p = conf;

    ngx_str_t        *field, *કિંમત;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    જો (field->data) {
        પાછા "is duplicate";
    }

    કિંમત = cf->args->elts;

    *field = કિંમત[1];

    જો (cmd->post) {
        post = cmd->post;
        પાછા post->post_handler(cf, post, field);
    }

    પાછા NGX_CONF_OK;
}


અક્ષર *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર  *p = conf;

    ngx_str_t         *કિંમત, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    જો (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, કદ(ngx_str_t));
        જો (*a == ખાલી) {
            પાછા NGX_CONF_ERROR;
        }
    }

    s = ngx_array_ધકેલો(*a);
    જો (s == ખાલી) {
        પાછા NGX_CONF_ERROR;
    }

    કિંમત = cf->args->elts;

    *s = કિંમત[1];

    જો (cmd->post) {
        post = cmd->post;
        પાછા post->post_handler(cf, post, s);
    }

    પાછા NGX_CONF_OK;
}


અક્ષર *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર  *p = conf;

    ngx_str_t         *કિંમત;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    જો (*a == NGX_CONF_UNSET_PTR || *a == ખાલી) {
        *a = ngx_array_create(cf->pool, 4, કદ(ngx_keyval_t));
        જો (*a == ખાલી) {
            પાછા NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_ધકેલો(*a);
    જો (kv == ખાલી) {
        પાછા NGX_CONF_ERROR;
    }

    કિંમત = cf->args->elts;

    kv->ચાવી = કિંમત[1];
    kv->કિંમત = કિંમત[2];

    જો (cmd->post) {
        post = cmd->post;
        પાછા post->post_handler(cf, post, kv);
    }

    પાછા NGX_CONF_OK;
}


અક્ષર *
ngx_conf_set_અંક_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *કિંમત;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    જો (*np != NGX_CONF_UNSET) {
        પાછા "is duplicate";
    }

    કિંમત = cf->args->elts;
    *np = ngx_atoi(કિંમત[1].data, કિંમત[1].len);
    જો (*np == NGX_ERROR) {
        પાછા "invalid number";
    }

    જો (cmd->post) {
        post = cmd->post;
        પાછા post->post_handler(cf, post, np);
    }

    પાછા NGX_CONF_OK;
}


અક્ષર *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર  *p = conf;

    size_t           *sp;
    ngx_str_t        *કિંમત;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    જો (*sp != NGX_CONF_UNSET_SIZE) {
        પાછા "is duplicate";
    }

    કિંમત = cf->args->elts;

    *sp = ngx_parse_size(&કિંમત[1]);
    જો (*sp == (size_t) NGX_ERROR) {
        પાછા "invalid value";
    }

    જો (cmd->post) {
        post = cmd->post;
        પાછા post->post_handler(cf, post, sp);
    }

    પાછા NGX_CONF_OK;
}


અક્ષર *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર  *p = conf;

    off_t            *op;
    ngx_str_t        *કિંમત;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    જો (*op != NGX_CONF_UNSET) {
        પાછા "is duplicate";
    }

    કિંમત = cf->args->elts;

    *op = ngx_parse_offset(&કિંમત[1]);
    જો (*op == (off_t) NGX_ERROR) {
        પાછા "invalid value";
    }

    જો (cmd->post) {
        post = cmd->post;
        પાછા post->post_handler(cf, post, op);
    }

    પાછા NGX_CONF_OK;
}


અક્ષર *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *કિંમત;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    જો (*msp != NGX_CONF_UNSET_MSEC) {
        પાછા "is duplicate";
    }

    કિંમત = cf->args->elts;

    *msp = ngx_parse_time(&કિંમત[1], 0);
    જો (*msp == (ngx_msec_t) NGX_ERROR) {
        પાછા "invalid value";
    }

    જો (cmd->post) {
        post = cmd->post;
        પાછા post->post_handler(cf, post, msp);
    }

    પાછા NGX_CONF_OK;
}


અક્ષર *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર  *p = conf;

    time_t           *sp;
    ngx_str_t        *કિંમત;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    જો (*sp != NGX_CONF_UNSET) {
        પાછા "is duplicate";
    }

    કિંમત = cf->args->elts;

    *sp = ngx_parse_time(&કિંમત[1], 1);
    જો (*sp == (time_t) NGX_ERROR) {
        પાછા "invalid value";
    }

    જો (cmd->post) {
        post = cmd->post;
        પાછા post->post_handler(cf, post, sp);
    }

    પાછા NGX_CONF_OK;
}


અક્ષર *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર *p = conf;

    ngx_str_t   *કિંમત;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    જો (bufs->અંક) {
        પાછા "is duplicate";
    }

    કિંમત = cf->args->elts;

    bufs->અંક = ngx_atoi(કિંમત[1].data, કિંમત[1].len);
    જો (bufs->અંક == NGX_ERROR || bufs->અંક == 0) {
        પાછા "invalid value";
    }

    bufs->size = ngx_parse_size(&કિંમત[2]);
    જો (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        પાછા "invalid value";
    }

    પાછા NGX_CONF_OK;
}


અક્ષર *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *કિંમત;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    જો (*np != NGX_CONF_UNSET_UINT) {
        પાછા "is duplicate";
    }

    કિંમત = cf->args->elts;
    e = cmd->post;

    માટે (i = 0; e[i].name.len != 0; i++) {
        જો (e[i].name.len != કિંમત[1].len
            || ngx_strcasecmp(e[i].name.data, કિંમત[1].data) != 0)
        {
            ચાલુ;
        }

        *np = e[i].કિંમત;

        પાછા NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", કિંમત[1].data);

    પાછા NGX_CONF_ERROR;
}


અક્ષર *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *કિંમત;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    કિંમત = cf->args->elts;
    mask = cmd->post;

    માટે (i = 1; i < cf->args->nelts; i++) {
        માટે (m = 0; mask[m].name.len != 0; m++) {

            જો (mask[m].name.len != કિંમત[i].len
                || ngx_strcasecmp(mask[m].name.data, કિંમત[i].data) != 0)
            {
                ચાલુ;
            }

            જો (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", કિંમત[i].data);

            } નહીંતર {
                *np |= mask[m].mask;
            }

            તોડો;
        }

        જો (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", કિંમત[i].data);

            પાછા NGX_CONF_ERROR;
        }
    }

    પાછા NGX_CONF_OK;
}


#જો 0

અક્ષર *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    પાછા "unsupported on this platform";
}

#endif


અક્ષર *
ngx_conf_deprecated(ngx_conf_t *cf, શૂન્ય *post, શૂન્ય *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    પાછા NGX_CONF_OK;
}


અક્ષર *
ngx_conf_check_અંક_bounds(ngx_conf_t *cf, શૂન્ય *post, શૂન્ય *data)
{
    ngx_conf_અંક_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    જો (bounds->ઉપલું == -1) {
        જો (*np >= bounds->નીચલું) {
            પાછા NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->નીચલું);

        પાછા NGX_CONF_ERROR;
    }

    જો (*np >= bounds->નીચલું && *np <= bounds->ઉપલું) {
        પાછા NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->નીચલું, bounds->ઉપલું);

    પાછા NGX_CONF_ERROR;
}


અક્ષર *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, શૂન્ય *post, શૂન્ય *data)
{
    size_t  *sp = data;

    જો (*sp == 0) {
        પાછા "value must be nonzero";
    }

    પાછા NGX_CONF_OK;
}
