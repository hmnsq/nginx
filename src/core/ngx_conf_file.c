
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


char *
ngx_conf_param(ngx_conf_t *cf)
{
    char             *rv;
    ngx_str_t        *param;
    ngx_buf_t         b;
    ngx_conf_file_t   conf_file;

    param = &cf->cycle->conf_param;

    如果 (param->len == 0) {
        返回 NGX_CONF_OK;
    }

    ngx_memzero(&conf_file, sizeof(ngx_conf_file_t));

    ngx_memzero(&b, sizeof(ngx_buf_t));

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

    返回 rv;
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

    如果 (sn) {
        cf->conf_file->dump = NULL;
        返回 NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    如果 (p == NULL) {
        返回 NGX_ERROR;
    }

    cd = ngx_array_push(&cf->cycle->config_dump);
    如果 (cd == NULL) {
        返回 NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    如果 (buf == NULL) {
        返回 NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, sizeof(ngx_str_node_t));
    如果 (sn == NULL) {
        返回 NGX_ERROR;
    }

    sn->node.key = hash;
    sn->str = cd->name;

    ngx_rbtree_insert(&cf->cycle->config_dump_rbtree, &sn->node);

    返回 NGX_OK;
}


char *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    char             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    enum {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#如果 (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = NULL;
#endif

    如果 (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        如果 (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            返回 NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        如果 (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        如果 (buf.start == NULL) {
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

        如果 (ngx_dump_config
#如果 (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#如果 (NGX_DEBUG)
            || 1
#endif
           )
        {
            如果 (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                goto failed;
            }

        } 否则 {
            cf->conf_file->dump = NULL;
        }

    } 否则 如果 (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } 否则 {
        type = parse_param;
    }


    为 ( ;; ) {
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

        如果 (rc == NGX_ERROR) {
            goto done;
        }

        如果 (rc == NGX_CONF_BLOCK_DONE) {

            如果 (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                goto failed;
            }

            goto done;
        }

        如果 (rc == NGX_CONF_FILE_DONE) {

            如果 (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                goto failed;
            }

            goto done;
        }

        如果 (rc == NGX_CONF_BLOCK_START) {

            如果 (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                goto failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        如果 (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            如果 (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                goto failed;
            }

            rv = (*cf->handler)(cf, NULL, cf->handler_conf);
            如果 (rv == NGX_CONF_OK) {
                continue;
            }

            如果 (rv == NGX_CONF_ERROR) {
                goto failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            goto failed;
        }


        rc = ngx_conf_handler(cf, rc);

        如果 (rc == NGX_ERROR) {
            goto failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    如果 (filename) {
        如果 (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        如果 (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    如果 (rc == NGX_ERROR) {
        返回 NGX_CONF_ERROR;
    }

    返回 NGX_CONF_OK;
}


static ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    char           *rv;
    void           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    为 (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        如果 (cmd == NULL) {
            continue;
        }

        为 ( /* void */ ; cmd->name.len; cmd++) {

            如果 (name->len != cmd->name.len) {
                continue;
            }

            如果 (ngx_strcmp(name->data, cmd->name.data) != 0) {
                continue;
            }

            found = 1;

            如果 (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                continue;
            }

            /* is the directive's location right ? */

            如果 (!(cmd->type & cf->cmd_type)) {
                continue;
            }

            如果 (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                返回 NGX_ERROR;
            }

            如果 ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                返回 NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            如果 (!(cmd->type & NGX_CONF_ANY)) {

                如果 (cmd->type & NGX_CONF_FLAG) {

                    如果 (cf->args->nelts != 2) {
                        goto invalid;
                    }

                } 否则 如果 (cmd->type & NGX_CONF_1MORE) {

                    如果 (cf->args->nelts < 2) {
                        goto invalid;
                    }

                } 否则 如果 (cmd->type & NGX_CONF_2MORE) {

                    如果 (cf->args->nelts < 3) {
                        goto invalid;
                    }

                } 否则 如果 (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    goto invalid;

                } 否则 如果 (!(cmd->type & argument_number[cf->args->nelts - 1]))
                {
                    goto invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = NULL;

            如果 (cmd->type & NGX_DIRECT_CONF) {
                conf = ((void **) cf->ctx)[cf->cycle->modules[i]->index];

            } 否则 如果 (cmd->type & NGX_MAIN_CONF) {
                conf = &(((void **) cf->ctx)[cf->cycle->modules[i]->index]);

            } 否则 如果 (cf->ctx) {
                confp = *(void **) ((char *) cf->ctx + cmd->conf);

                如果 (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            如果 (rv == NGX_CONF_OK) {
                返回 NGX_OK;
            }

            如果 (rv == NGX_CONF_ERROR) {
                返回 NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            返回 NGX_ERROR;
        }
    }

    如果 (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        返回 NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    返回 NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    返回 NGX_ERROR;
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

    为 ( ;; ) {

        如果 (b->pos >= b->last) {

            如果 (cf->conf_file->file.offset >= file_size) {

                如果 (cf->args->nelts > 0 || !last_space) {

                    如果 (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        返回 NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    返回 NGX_ERROR;
                }

                返回 NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            如果 (len == NGX_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                如果 (d_quoted) {
                    ch = '"';

                } 否则 如果 (s_quoted) {
                    ch = '\'';

                } 否则 {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    返回 NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                返回 NGX_ERROR;
            }

            如果 (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            如果 (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            如果 (n == NGX_ERROR) {
                返回 NGX_ERROR;
            }

            如果 (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                返回 NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            如果 (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        如果 (ch == LF) {
            cf->conf_file->line++;

            如果 (sharp_comment) {
                sharp_comment = 0;
            }
        }

        如果 (sharp_comment) {
            continue;
        }

        如果 (quoted) {
            quoted = 0;
            continue;
        }

        如果 (need_space) {
            如果 (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                continue;
            }

            如果 (ch == ';') {
                返回 NGX_OK;
            }

            如果 (ch == '{') {
                返回 NGX_CONF_BLOCK_START;
            }

            如果 (ch == ')') {
                last_space = 1;
                need_space = 0;

            } 否则 {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                返回 NGX_ERROR;
            }
        }

        如果 (last_space) {

            start = b->pos - 1;
            start_line = cf->conf_file->line;

            如果 (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                continue;
            }

            switch (ch) {

            case ';':
            case '{':
                如果 (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    返回 NGX_ERROR;
                }

                如果 (ch == '{') {
                    返回 NGX_CONF_BLOCK_START;
                }

                返回 NGX_OK;

            case '}':
                如果 (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    返回 NGX_ERROR;
                }

                返回 NGX_CONF_BLOCK_DONE;

            case '#':
                sharp_comment = 1;
                continue;

            case '\\':
                quoted = 1;
                last_space = 0;
                continue;

            case '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                continue;

            case '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                continue;

            case '$':
                variable = 1;
                last_space = 0;
                continue;

            default:
                last_space = 0;
            }

        } 否则 {
            如果 (ch == '{' && variable) {
                continue;
            }

            variable = 0;

            如果 (ch == '\\') {
                quoted = 1;
                continue;
            }

            如果 (ch == '$') {
                variable = 1;
                continue;
            }

            如果 (d_quoted) {
                如果 (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } 否则 如果 (s_quoted) {
                如果 (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } 否则 如果 (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            如果 (found) {
                word = ngx_array_push(cf->args);
                如果 (word == NULL) {
                    返回 NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                如果 (word->data == NULL) {
                    返回 NGX_ERROR;
                }

                为 (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    如果 (*src == '\\') {
                        switch (src[1]) {
                        case '"':
                        case '\'':
                        case '\\':
                            src++;
                            break;

                        case 't':
                            *dst++ = '\t';
                            src += 2;
                            continue;

                        case 'r':
                            *dst++ = '\r';
                            src += 2;
                            continue;

                        case 'n':
                            *dst++ = '\n';
                            src += 2;
                            continue;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                如果 (ch == ';') {
                    返回 NGX_OK;
                }

                如果 (ch == '{') {
                    返回 NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}


char *
ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char        *rv;
    ngx_int_t    n;
    ngx_str_t   *value, file, name;
    ngx_glob_t   gl;

    value = cf->args->elts;
    file = value[1];

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

    如果 (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        返回 NGX_CONF_ERROR;
    }

    如果 (strpbrk((char *) file.data, "*?[") == NULL) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        返回 ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, sizeof(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    如果 (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        返回 NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    为 ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        如果 (n != NGX_OK) {
            break;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        如果 (file.data == NULL) {
            返回 NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        如果 (rv != NGX_CONF_OK) {
            break;
        }
    }

    ngx_close_glob(&gl);

    返回 rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    返回 ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#如果 (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    如果 (name->len) {
        full = *name;

        如果 (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            返回 NULL;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        为 (i = 0; /* void */ ; i++) {

            如果 (i >= part->nelts) {
                如果 (part->next == NULL) {
                    break;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            如果 (full.len != file[i].name.len) {
                continue;
            }

            如果 (ngx_strcmp(full.data, file[i].name.data) == 0) {
                返回 &file[i];
            }
        }
    }

    file = ngx_list_push(&cycle->open_files);
    如果 (file == NULL) {
        返回 NULL;
    }

    如果 (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } 否则 {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = NULL;
    file->data = NULL;

    返回 file;
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

    为 (i = 0; /* void */ ; i++) {

        如果 (i >= part->nelts) {
            如果 (part->next == NULL) {
                break;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        如果 (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }
    }
}


void ngx_cdecl
ngx_conf_log_error(ngx_uint_t level, ngx_conf_t *cf, ngx_err_t err,
    const char *fmt, ...)
{
    u_char   errstr[NGX_MAX_CONF_ERRSTR], *p, *last;
    va_list  args;

    last = errstr + NGX_MAX_CONF_ERRSTR;

    va_start(args, fmt);
    p = ngx_vslprintf(errstr, last, fmt, args);
    va_end(args);

    如果 (err) {
        p = ngx_log_errno(p, last, err);
    }

    如果 (cf->conf_file == NULL) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        返回;
    }

    如果 (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        返回;
    }

    ngx_log_error(level, cf->log, 0, "%*s in %s:%ui",
                  p - errstr, errstr,
                  cf->conf_file->file.name.data, cf->conf_file->line);
}


char *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_str_t        *value;
    ngx_flag_t       *fp;
    ngx_conf_post_t  *post;

    fp = (ngx_flag_t *) (p + cmd->offset);

    如果 (*fp != NGX_CONF_UNSET) {
        返回 "is duplicate";
    }

    value = cf->args->elts;

    如果 (ngx_strcasecmp(value[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } 否则 如果 (ngx_strcasecmp(value[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } 否则 {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     value[1].data, cmd->name.data);
        返回 NGX_CONF_ERROR;
    }

    如果 (cmd->post) {
        post = cmd->post;
        返回 post->post_handler(cf, post, fp);
    }

    返回 NGX_CONF_OK;
}


char *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_str_t        *field, *value;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    如果 (field->data) {
        返回 "is duplicate";
    }

    value = cf->args->elts;

    *field = value[1];

    如果 (cmd->post) {
        post = cmd->post;
        返回 post->post_handler(cf, post, field);
    }

    返回 NGX_CONF_OK;
}


char *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_str_t         *value, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    如果 (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, sizeof(ngx_str_t));
        如果 (*a == NULL) {
            返回 NGX_CONF_ERROR;
        }
    }

    s = ngx_array_push(*a);
    如果 (s == NULL) {
        返回 NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    *s = value[1];

    如果 (cmd->post) {
        post = cmd->post;
        返回 post->post_handler(cf, post, s);
    }

    返回 NGX_CONF_OK;
}


char *
ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_str_t         *value;
    ngx_array_t      **a;
    ngx_keyval_t      *kv;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    如果 (*a == NGX_CONF_UNSET_PTR || *a == NULL) {
        *a = ngx_array_create(cf->pool, 4, sizeof(ngx_keyval_t));
        如果 (*a == NULL) {
            返回 NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_push(*a);
    如果 (kv == NULL) {
        返回 NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    kv->key = value[1];
    kv->value = value[2];

    如果 (cmd->post) {
        post = cmd->post;
        返回 post->post_handler(cf, post, kv);
    }

    返回 NGX_CONF_OK;
}


char *
ngx_conf_set_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    如果 (*np != NGX_CONF_UNSET) {
        返回 "is duplicate";
    }

    value = cf->args->elts;
    *np = ngx_atoi(value[1].data, value[1].len);
    如果 (*np == NGX_ERROR) {
        返回 "invalid number";
    }

    如果 (cmd->post) {
        post = cmd->post;
        返回 post->post_handler(cf, post, np);
    }

    返回 NGX_CONF_OK;
}


char *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    size_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    如果 (*sp != NGX_CONF_UNSET_SIZE) {
        返回 "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_size(&value[1]);
    如果 (*sp == (size_t) NGX_ERROR) {
        返回 "invalid value";
    }

    如果 (cmd->post) {
        post = cmd->post;
        返回 post->post_handler(cf, post, sp);
    }

    返回 NGX_CONF_OK;
}


char *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    off_t            *op;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    如果 (*op != NGX_CONF_UNSET) {
        返回 "is duplicate";
    }

    value = cf->args->elts;

    *op = ngx_parse_offset(&value[1]);
    如果 (*op == (off_t) NGX_ERROR) {
        返回 "invalid value";
    }

    如果 (cmd->post) {
        post = cmd->post;
        返回 post->post_handler(cf, post, op);
    }

    返回 NGX_CONF_OK;
}


char *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    如果 (*msp != NGX_CONF_UNSET_MSEC) {
        返回 "is duplicate";
    }

    value = cf->args->elts;

    *msp = ngx_parse_time(&value[1], 0);
    如果 (*msp == (ngx_msec_t) NGX_ERROR) {
        返回 "invalid value";
    }

    如果 (cmd->post) {
        post = cmd->post;
        返回 post->post_handler(cf, post, msp);
    }

    返回 NGX_CONF_OK;
}


char *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    time_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    如果 (*sp != NGX_CONF_UNSET) {
        返回 "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_time(&value[1], 1);
    如果 (*sp == (time_t) NGX_ERROR) {
        返回 "invalid value";
    }

    如果 (cmd->post) {
        post = cmd->post;
        返回 post->post_handler(cf, post, sp);
    }

    返回 NGX_CONF_OK;
}


char *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char *p = conf;

    ngx_str_t   *value;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    如果 (bufs->num) {
        返回 "is duplicate";
    }

    value = cf->args->elts;

    bufs->num = ngx_atoi(value[1].data, value[1].len);
    如果 (bufs->num == NGX_ERROR || bufs->num == 0) {
        返回 "invalid value";
    }

    bufs->size = ngx_parse_size(&value[2]);
    如果 (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        返回 "invalid value";
    }

    返回 NGX_CONF_OK;
}


char *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *value;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    如果 (*np != NGX_CONF_UNSET_UINT) {
        返回 "is duplicate";
    }

    value = cf->args->elts;
    e = cmd->post;

    为 (i = 0; e[i].name.len != 0; i++) {
        如果 (e[i].name.len != value[1].len
            || ngx_strcasecmp(e[i].name.data, value[1].data) != 0)
        {
            continue;
        }

        *np = e[i].value;

        返回 NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", value[1].data);

    返回 NGX_CONF_ERROR;
}


char *
ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_uint_t          *np, i, m;
    ngx_str_t           *value;
    ngx_conf_bitmask_t  *mask;


    np = (ngx_uint_t *) (p + cmd->offset);
    value = cf->args->elts;
    mask = cmd->post;

    为 (i = 1; i < cf->args->nelts; i++) {
        为 (m = 0; mask[m].name.len != 0; m++) {

            如果 (mask[m].name.len != value[i].len
                || ngx_strcasecmp(mask[m].name.data, value[i].data) != 0)
            {
                continue;
            }

            如果 (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", value[i].data);

            } 否则 {
                *np |= mask[m].mask;
            }

            break;
        }

        如果 (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", value[i].data);

            返回 NGX_CONF_ERROR;
        }
    }

    返回 NGX_CONF_OK;
}


#如果 0

char *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    返回 "unsupported on this platform";
}

#endif


char *
ngx_conf_deprecated(ngx_conf_t *cf, void *post, void *data)
{
    ngx_conf_deprecated_t  *d = post;

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "the \"%s\" directive is deprecated, "
                       "use the \"%s\" directive instead",
                       d->old_name, d->new_name);

    返回 NGX_CONF_OK;
}


char *
ngx_conf_check_num_bounds(ngx_conf_t *cf, void *post, void *data)
{
    ngx_conf_num_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    如果 (bounds->high == -1) {
        如果 (*np >= bounds->low) {
            返回 NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->low);

        返回 NGX_CONF_ERROR;
    }

    如果 (*np >= bounds->low && *np <= bounds->high) {
        返回 NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->low, bounds->high);

    返回 NGX_CONF_ERROR;
}


char *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, void *post, void *data)
{
    size_t  *sp = data;

    如果 (*sp == 0) {
        返回 "value must be nonzero";
    }

    返回 NGX_CONF_OK;
}
