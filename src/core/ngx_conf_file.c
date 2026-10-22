
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

#define NGX_CONF_BUFFER  4096

static ngx_int_t ngx_conf_add_dump(ngx_conf_t *cf, ngx_str_t *filename);
static ngx_int_t ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last);
static ngx_int_t ngx_conf_read_token(ngx_conf_t *cf);
static void ngx_conf_flush_files(ngx_cycle_t *cycle);


static ngx_command_t  ngx_conf_commands[] = {

    { ngx_string("include"),
      NGX_ANY_CONF|NGX_CONF_TAKE1,
      ngx_conf_include,
      0,
      0,
      NULL },

      ngx_null_command
};


ngx_module_t  ngx_conf_module = {
    NGX_MODULE_V1,
    NULL,                                  /* module context */
    ngx_conf_commands,                     /* module directives */
    NGX_CONF_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    ngx_conf_flush_files,                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/* The eight fixed arguments */

static ngx_uint_t argument_number[] = {
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


var_na *
ngx_conf_param(ngx_conf_t *cf)
{
    var_na             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    ถ้า (param->len == 0) {
        คืน NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, parimaa_na(ngx_conf_file_t));

    ngx_memzero(&b, parimaa_na(ngx_buf_t));

    b.start = param->data;
    b.pos = param->data;
    b.last = param->data + param->len;
    b.end = b.last;
    b.temporary = 1;

    conf_file.file.fd = NGX_INVALID_FILE;
    conf_file.file.name.data = NULL;
    conf_file.line = 0;

    cf->conf_file = &conf_file;
    cf->conf_file->buffer = &b;

    rv = ngx_conf_parse(cf, NULL);

    cf->conf_file = NULL;

    คืน rv;
}


static ngx_int_t
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

    ถ้า (sn) {
        cf->conf_file->dump = NULL;
        คืน NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    ถ้า (p == NULL) {
        คืน NGX_ERROR;
    }

    cd = ngx_array_push(&cf->cycle->config_dump);
    ถ้า (cd == NULL) {
        คืน NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    ถ้า (buf == NULL) {
        คืน NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, parimaa_na(ngx_str_node_t));
    ถ้า (sn == NULL) {
        คืน NGX_ERROR;
    }

    sn->node.key = hash;
    sn->str = cd->name;

    ngx_rbtree_insert(&cf->cycle->config_dump_rbtree, &sn->node);

    คืน NGX_OK;
}


var_na *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    var_na             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    ga_nanaa {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#ถ้า (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = NULL;
#endif

    ถ้า (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        ถ้า (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            คืน NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        ถ้า (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        ถ้า (buf.start == NULL) {
            goto failed;
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

        ถ้า (ngx_dump_config
#ถ้า (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#ถ้า (NGX_DEBUG)
            || 1
#endif
           )
        {
            ถ้า (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                goto failed;
            }

        } มิฉะนั้น {
            cf->conf_file->dump = NULL;
        }

    } มิฉะนั้น ถ้า (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } มิฉะนั้น {
        type = parse_param;
    }


    สำหรับ ( ;; ) {
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

        ถ้า (rc == NGX_ERROR) {
            goto done;
        }

        ถ้า (rc == NGX_CONF_BLOCK_DONE) {

            ถ้า (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                goto failed;
            }

            goto done;
        }

        ถ้า (rc == NGX_CONF_FILE_DONE) {

            ถ้า (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                goto failed;
            }

            goto done;
        }

        ถ้า (rc == NGX_CONF_BLOCK_START) {

            ถ้า (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                goto failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        ถ้า (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            ถ้า (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                goto failed;
            }

            rv = (*cf->handler)(cf, NULL, cf->handler_conf);
            ถ้า (rv == NGX_CONF_OK) {
                continue;
            }

            ถ้า (rv == NGX_CONF_ERROR) {
                goto failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            goto failed;
        }


        rc = ngx_conf_handler(cf, rc);

        ถ้า (rc == NGX_ERROR) {
            goto failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    ถ้า (filename) {
        ถ้า (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        ถ้า (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    ถ้า (rc == NGX_ERROR) {
        คืน NGX_CONF_ERROR;
    }

    คืน NGX_CONF_OK;
}


static ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    var_na           *rv;
    void           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    สำหรับ (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        ถ้า (cmd == NULL) {
            continue;
        }

        สำหรับ ( /* void */ ; cmd->name.len; cmd++) {

            ถ้า (name->len != cmd->name.len) {
                continue;
            }

            ถ้า (ngx_strcmp(name->data, cmd->name.data) != 0) {
                continue;
            }

            found = 1;

            ถ้า (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                continue;
            }

            /* is the directive's location right ? */

            ถ้า (!(cmd->type & cf->cmd_type)) {
                continue;
            }

            ถ้า (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                คืน NGX_ERROR;
            }

            ถ้า ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                คืน NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            ถ้า (!(cmd->type & NGX_CONF_ANY)) {

                ถ้า (cmd->type & NGX_CONF_FLAG) {

                    ถ้า (cf->args->nelts != 2) {
                        goto invalid;
                    }

                } มิฉะนั้น ถ้า (cmd->type & NGX_CONF_1MORE) {

                    ถ้า (cf->args->nelts < 2) {
                        goto invalid;
                    }

                } มิฉะนั้น ถ้า (cmd->type & NGX_CONF_2MORE) {

                    ถ้า (cf->args->nelts < 3) {
                        goto invalid;
                    }

                } มิฉะนั้น ถ้า (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    goto invalid;

                } มิฉะนั้น ถ้า (!(cmd->type & argument_number[cf->args->nelts - 1]))
                {
                    goto invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = NULL;

            ถ้า (cmd->type & NGX_DIRECT_CONF) {
                conf = ((void **) cf->ctx)[cf->cycle->modules[i]->index];

            } มิฉะนั้น ถ้า (cmd->type & NGX_MAIN_CONF) {
                conf = &(((void **) cf->ctx)[cf->cycle->modules[i]->index]);

            } มิฉะนั้น ถ้า (cf->ctx) {
                confp = *(void **) ((var_na *) cf->ctx + cmd->conf);

                ถ้า (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            ถ้า (rv == NGX_CONF_OK) {
                คืน NGX_OK;
            }

            ถ้า (rv == NGX_CONF_ERROR) {
                คืน NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            คืน NGX_ERROR;
        }
    }

    ถ้า (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        คืน NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    คืน NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    คืน NGX_ERROR;
}


static ngx_int_t
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

    สำหรับ ( ;; ) {

        ถ้า (b->pos >= b->last) {

            ถ้า (cf->conf_file->file.offset >= file_size) {

                ถ้า (cf->args->nelts > 0 || !last_space) {

                    ถ้า (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        คืน NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    คืน NGX_ERROR;
                }

                คืน NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            ถ้า (len == NGX_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                ถ้า (d_quoted) {
                    ch = '"';

                } มิฉะนั้น ถ้า (s_quoted) {
                    ch = '\'';

                } มิฉะนั้น {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    คืน NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                คืน NGX_ERROR;
            }

            ถ้า (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            ถ้า (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            ถ้า (n == NGX_ERROR) {
                คืน NGX_ERROR;
            }

            ถ้า (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                คืน NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            ถ้า (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        ถ้า (ch == LF) {
            cf->conf_file->line++;

            ถ้า (sharp_comment) {
                sharp_comment = 0;
            }
        }

        ถ้า (sharp_comment) {
            continue;
        }

        ถ้า (quoted) {
            quoted = 0;
            continue;
        }

        ถ้า (need_space) {
            ถ้า (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                continue;
            }

            ถ้า (ch == ';') {
                คืน NGX_OK;
            }

            ถ้า (ch == '{') {
                คืน NGX_CONF_BLOCK_START;
            }

            ถ้า (ch == ')') {
                last_space = 1;
                need_space = 0;

            } มิฉะนั้น {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                คืน NGX_ERROR;
            }
        }

        ถ้า (last_space) {

            start = b->pos - 1;
            start_line = cf->conf_file->line;

            ถ้า (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                continue;
            }

            switch (ch) {

            pak_sa ';':
            pak_sa '{':
                ถ้า (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    คืน NGX_ERROR;
                }

                ถ้า (ch == '{') {
                    คืน NGX_CONF_BLOCK_START;
                }

                คืน NGX_OK;

            pak_sa '}':
                ถ้า (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    คืน NGX_ERROR;
                }

                คืน NGX_CONF_BLOCK_DONE;

            pak_sa '#':
                sharp_comment = 1;
                continue;

            pak_sa '\\':
                quoted = 1;
                last_space = 0;
                continue;

            pak_sa '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                continue;

            pak_sa '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                continue;

            pak_sa '$':
                variable = 1;
                last_space = 0;
                continue;

            default:
                last_space = 0;
            }

        } มิฉะนั้น {
            ถ้า (ch == '{' && variable) {
                continue;
            }

            variable = 0;

            ถ้า (ch == '\\') {
                quoted = 1;
                continue;
            }

            ถ้า (ch == '$') {
                variable = 1;
                continue;
            }

            ถ้า (d_quoted) {
                ถ้า (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } มิฉะนั้น ถ้า (s_quoted) {
                ถ้า (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } มิฉะนั้น ถ้า (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            ถ้า (found) {
                word = ngx_array_push(cf->args);
                ถ้า (word == NULL) {
                    คืน NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                ถ้า (word->data == NULL) {
                    คืน NGX_ERROR;
                }

                สำหรับ (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    ถ้า (*src == '\\') {
                        switch (src[1]) {
                        pak_sa '"':
                        pak_sa '\'':
                        pak_sa '\\':
                            src++;
                            bha_nga;

                        pak_sa 't':
                            *dst++ = '\t';
                            src += 2;
                            continue;

                        pak_sa 'r':
                            *dst++ = '\r';
                            src += 2;
                            continue;

                        pak_sa 'n':
                            *dst++ = '\n';
                            src += 2;
                            continue;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                ถ้า (ch == ';') {
                    คืน NGX_OK;
                }

                ถ้า (ch == '{') {
                    คืน NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


var_na *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na        *rv;
    ngx_int_t    n;
    ngx_str_t   *value, file, name;
    ngx_glob_t   gl;

    value = cf->args->elts;
    file = value[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    ถ้า (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        คืน NGX_CONF_ERROR;
    }

    ถ้า (strpbrk((var_na *) file.data, "*?[") == NULL) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        คืน ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, parimaa_na(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    ถ้า (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        คืน NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    สำหรับ ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        ถ้า (n != NGX_OK) {
            bha_nga;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        ถ้า (file.data == NULL) {
            คืน NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        ถ้า (rv != NGX_CONF_OK) {
            bha_nga;
        }
    }

    ngx_close_glob(&gl);

    คืน rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    คืน ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#ถ้า (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    ถ้า (name->len) {
        full = *name;

        ถ้า (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            คืน NULL;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        สำหรับ (i = 0; /* void */ ; i++) {

            ถ้า (i >= part->nelts) {
                ถ้า (part->next == NULL) {
                    bha_nga;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            ถ้า (full.len != file[i].name.len) {
                continue;
            }

            ถ้า (ngx_strcmp(full.data, file[i].name.data) == 0) {
                คืน &file[i];
            }
        }
    }

    file = ngx_list_push(&cycle->open_files);
    ถ้า (file == NULL) {
        คืน NULL;
    }

    ถ้า (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } มิฉะนั้น {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = NULL;
    file->data = NULL;

    คืน file;
}


static void
ngx_conf_flush_files(ngx_cycle_t *cycle)
{
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, cycle->log, 0, "flush files");

    part = &cycle->open_files.part;
    file = part->elts;

    สำหรับ (i = 0; /* void */ ; i++) {

        ถ้า (i >= part->nelts) {
            ถ้า (part->next == NULL) {
                bha_nga;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ถ้า (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


void ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    const var_na *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    va_list  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    va_end(args);

    ถ้า (err) {
        p = ngx_log_errno(p, last, err);
    }

    ถ้า (cf->conf_file == NULL) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        คืน;
    }

    ถ้า (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        คืน;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->line);
}


var_na *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t        *value;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    ถ้า (*fp != NGX_CONF_UNSET) {
        คืน "is duplicate";
    }

    value = cf->args->elts;

    ถ้า (ngx_strcasecmp(value[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } มิฉะนั้น ถ้า (ngx_strcasecmp(value[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } มิฉะนั้น {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     value[1].data, cmd->name.data);
        คืน NGX_CONF_ERROR;
    }

    ถ้า (cmd->post) {
        post = cmd->post;
        คืน post->post_handler(cf, post, fp);
    }

    คืน NGX_CONF_OK;
}


var_na *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t        *field, *value;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    ถ้า (field->data) {
        คืน "is duplicate";
    }

    value = cf->args->elts;

    *field = value[1];

    ถ้า (cmd->post) {
        post = cmd->post;
        คืน post->post_handler(cf, post, field);
    }

    คืน NGX_CONF_OK;
}


var_na *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t         *value, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    ถ้า (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, parimaa_na(ngx_str_t));
        ถ้า (*a == NULL) {
            คืน NGX_CONF_ERROR;
        }
    }

    s = ngx_array_push(*a);
    ถ้า (s == NULL) {
        คืน NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    *s = value[1];

    ถ้า (cmd->post) {
        post = cmd->post;
        คืน post->post_handler(cf, post, s);
    }

    คืน NGX_CONF_OK;
}


var_na *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_str_t         *value;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    ถ้า (*a == NGX_CONF_UNSET_PTR || *a == NULL) {
        *a = ngx_array_create(cf->pool, 4, parimaa_na(ngx_keyval_t));
        ถ้า (*a == NULL) {
            คืน NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_push(*a);
    ถ้า (kv == NULL) {
        คืน NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    kv->key = value[1];
    kv->value = value[2];

    ถ้า (cmd->post) {
        post = cmd->post;
        คืน post->post_handler(cf, post, kv);
    }

    คืน NGX_CONF_OK;
}


var_na *
ngx_conf_set_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    ถ้า (*np != NGX_CONF_UNSET) {
        คืน "is duplicate";
    }

    value = cf->args->elts;
    *np = ngx_atoi(value[1].data, value[1].len);
    ถ้า (*np == NGX_ERROR) {
        คืน "invalid number";
    }

    ถ้า (cmd->post) {
        post = cmd->post;
        คืน post->post_handler(cf, post, np);
    }

    คืน NGX_CONF_OK;
}


var_na *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    size_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    ถ้า (*sp != NGX_CONF_UNSET_SIZE) {
        คืน "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_size(&value[1]);
    ถ้า (*sp == (size_t) NGX_ERROR) {
        คืน "invalid value";
    }

    ถ้า (cmd->post) {
        post = cmd->post;
        คืน post->post_handler(cf, post, sp);
    }

    คืน NGX_CONF_OK;
}


var_na *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    off_t            *op;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    ถ้า (*op != NGX_CONF_UNSET) {
        คืน "is duplicate";
    }

    value = cf->args->elts;

    *op = ngx_parse_offset(&value[1]);
    ถ้า (*op == (off_t) NGX_ERROR) {
        คืน "invalid value";
    }

    ถ้า (cmd->post) {
        post = cmd->post;
        คืน post->post_handler(cf, post, op);
    }

    คืน NGX_CONF_OK;
}


var_na *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    ถ้า (*msp != NGX_CONF_UNSET_MSEC) {
        คืน "is duplicate";
    }

    value = cf->args->elts;

    *msp = ngx_parse_time(&value[1], 0);
    ถ้า (*msp == (ngx_msec_t) NGX_ERROR) {
        คืน "invalid value";
    }

    ถ้า (cmd->post) {
        post = cmd->post;
        คืน post->post_handler(cf, post, msp);
    }

    คืน NGX_CONF_OK;
}


var_na *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    time_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    ถ้า (*sp != NGX_CONF_UNSET) {
        คืน "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_time(&value[1], 1);
    ถ้า (*sp == (time_t) NGX_ERROR) {
        คืน "invalid value";
    }

    ถ้า (cmd->post) {
        post = cmd->post;
        คืน post->post_handler(cf, post, sp);
    }

    คืน NGX_CONF_OK;
}


var_na *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na *p = conf;

    ngx_str_t   *value;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    ถ้า (bufs->num) {
        คืน "is duplicate";
    }

    value = cf->args->elts;

    bufs->num = ngx_atoi(value[1].data, value[1].len);
    ถ้า (bufs->num == NGX_ERROR || bufs->num == 0) {
        คืน "invalid value";
    }

    bufs->size = ngx_parse_size(&value[2]);
    ถ้า (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        คืน "invalid value";
    }

    คืน NGX_CONF_OK;
}


var_na *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *value;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    ถ้า (*np != NGX_CONF_UNSET_UINT) {
        คืน "is duplicate";
    }

    value = cf->args->elts;
    e = cmd->post;

    สำหรับ (i = 0; e[i].name.len != 0; i++) {
        ถ้า (e[i].name.len != value[1].len
            || ngx_strcasecmp(e[i].name.data, value[1].data) != 0)
        {
            continue;
        }

        *np = e[i].value;

        คืน NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", value[1].data);

    คืน NGX_CONF_ERROR;
}


var_na *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *value;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    value = cf->args->elts;
    mask = cmd->post;

    สำหรับ (i = 1; i < cf->args->nelts; i++) {
        สำหรับ (m = 0; mask[m].name.len != 0; m++) {

            ถ้า (mask[m].name.len != value[i].len
                || ngx_strcasecmp(mask[m].name.data, value[i].data) != 0)
            {
                continue;
            }

            ถ้า (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", value[i].data);

            } มิฉะนั้น {
                *np |= mask[m].mask;
            }

            bha_nga;
        }

        ถ้า (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", value[i].data);

            คืน NGX_CONF_ERROR;
        }
    }

    คืน NGX_CONF_OK;
}


#ถ้า 0

var_na *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    คืน "unsupported on this platform";
}

#endif


var_na *
ngx_conf_deprecated(ngx_conf_t *cf, void *post, void *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    คืน NGX_CONF_OK;
}


var_na *
ngx_conf_check_num_bounds(ngx_conf_t *cf, void *post, void *data)
{
    ngx_conf_num_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    ถ้า (bounds->high == -1) {
        ถ้า (*np >= bounds->low) {
            คืน NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->low);

        คืน NGX_CONF_ERROR;
    }

    ถ้า (*np >= bounds->low && *np <= bounds->high) {
        คืน NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->low, bounds->high);

    คืน NGX_CONF_ERROR;
}


var_na *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, void *post, void *data)
{
    size_t  *sp = data;

    ถ้า (*sp == 0) {
        คืน "value must be nonzero";
    }

    คืน NGX_CONF_OK;
}
