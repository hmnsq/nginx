
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

    якщо (param->len == 0) {
        повернути NGX_CONF_OK;
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

    повернути rv;
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

    якщо (sn) {
        cf->conf_file->dump = NULL;
        повернути NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    якщо (p == NULL) {
        повернути NGX_ERROR;
    }

    cd = ngx_array_push(&cf->cycle->config_dump);
    якщо (cd == NULL) {
        повернути NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    якщо (buf == NULL) {
        повернути NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, sizeof(ngx_str_node_t));
    якщо (sn == NULL) {
        повернути NGX_ERROR;
    }

    sn->node.key = hash;
    sn->str = cd->name;

    ngx_rbtree_insert(&cf->cycle->config_dump_rbtree, &sn->node);

    повернути NGX_OK;
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

#якщо (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = NULL;
#endif

    якщо (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        якщо (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            повернути NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        якщо (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        якщо (buf.start == NULL) {
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

        якщо (ngx_dump_config
#якщо (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#якщо (NGX_DEBUG)
            || 1
#endif
           )
        {
            якщо (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                goto failed;
            }

        } інакше {
            cf->conf_file->dump = NULL;
        }

    } інакше якщо (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } інакше {
        type = parse_param;
    }


    для ( ;; ) {
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

        якщо (rc == NGX_ERROR) {
            goto done;
        }

        якщо (rc == NGX_CONF_BLOCK_DONE) {

            якщо (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                goto failed;
            }

            goto done;
        }

        якщо (rc == NGX_CONF_FILE_DONE) {

            якщо (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                goto failed;
            }

            goto done;
        }

        якщо (rc == NGX_CONF_BLOCK_START) {

            якщо (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                goto failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        якщо (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            якщо (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                goto failed;
            }

            rv = (*cf->handler)(cf, NULL, cf->handler_conf);
            якщо (rv == NGX_CONF_OK) {
                continue;
            }

            якщо (rv == NGX_CONF_ERROR) {
                goto failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            goto failed;
        }


        rc = ngx_conf_handler(cf, rc);

        якщо (rc == NGX_ERROR) {
            goto failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    якщо (filename) {
        якщо (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        якщо (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    якщо (rc == NGX_ERROR) {
        повернути NGX_CONF_ERROR;
    }

    повернути NGX_CONF_OK;
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

    для (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        якщо (cmd == NULL) {
            continue;
        }

        для ( /* void */ ; cmd->name.len; cmd++) {

            якщо (name->len != cmd->name.len) {
                continue;
            }

            якщо (ngx_strcmp(name->data, cmd->name.data) != 0) {
                continue;
            }

            found = 1;

            якщо (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                continue;
            }

            /* is the directive's location right ? */

            якщо (!(cmd->type & cf->cmd_type)) {
                continue;
            }

            якщо (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                повернути NGX_ERROR;
            }

            якщо ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                повернути NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            якщо (!(cmd->type & NGX_CONF_ANY)) {

                якщо (cmd->type & NGX_CONF_FLAG) {

                    якщо (cf->args->nelts != 2) {
                        goto invalid;
                    }

                } інакше якщо (cmd->type & NGX_CONF_1MORE) {

                    якщо (cf->args->nelts < 2) {
                        goto invalid;
                    }

                } інакше якщо (cmd->type & NGX_CONF_2MORE) {

                    якщо (cf->args->nelts < 3) {
                        goto invalid;
                    }

                } інакше якщо (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    goto invalid;

                } інакше якщо (!(cmd->type & argument_number[cf->args->nelts - 1]))
                {
                    goto invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = NULL;

            якщо (cmd->type & NGX_DIRECT_CONF) {
                conf = ((void **) cf->ctx)[cf->cycle->modules[i]->index];

            } інакше якщо (cmd->type & NGX_MAIN_CONF) {
                conf = &(((void **) cf->ctx)[cf->cycle->modules[i]->index]);

            } інакше якщо (cf->ctx) {
                confp = *(void **) ((char *) cf->ctx + cmd->conf);

                якщо (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            якщо (rv == NGX_CONF_OK) {
                повернути NGX_OK;
            }

            якщо (rv == NGX_CONF_ERROR) {
                повернути NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            повернути NGX_ERROR;
        }
    }

    якщо (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        повернути NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    повернути NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    повернути NGX_ERROR;
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

    для ( ;; ) {

        якщо (b->pos >= b->last) {

            якщо (cf->conf_file->file.offset >= file_size) {

                якщо (cf->args->nelts > 0 || !last_space) {

                    якщо (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        повернути NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    повернути NGX_ERROR;
                }

                повернути NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            якщо (len == NGX_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                якщо (d_quoted) {
                    ch = '"';

                } інакше якщо (s_quoted) {
                    ch = '\'';

                } інакше {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    повернути NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                повернути NGX_ERROR;
            }

            якщо (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            якщо (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            якщо (n == NGX_ERROR) {
                повернути NGX_ERROR;
            }

            якщо (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                повернути NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            якщо (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        якщо (ch == LF) {
            cf->conf_file->line++;

            якщо (sharp_comment) {
                sharp_comment = 0;
            }
        }

        якщо (sharp_comment) {
            continue;
        }

        якщо (quoted) {
            quoted = 0;
            continue;
        }

        якщо (need_space) {
            якщо (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                continue;
            }

            якщо (ch == ';') {
                повернути NGX_OK;
            }

            якщо (ch == '{') {
                повернути NGX_CONF_BLOCK_START;
            }

            якщо (ch == ')') {
                last_space = 1;
                need_space = 0;

            } інакше {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                повернути NGX_ERROR;
            }
        }

        якщо (last_space) {

            start = b->pos - 1;
            start_line = cf->conf_file->line;

            якщо (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                continue;
            }

            switch (ch) {

            case ';':
            case '{':
                якщо (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    повернути NGX_ERROR;
                }

                якщо (ch == '{') {
                    повернути NGX_CONF_BLOCK_START;
                }

                повернути NGX_OK;

            case '}':
                якщо (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    повернути NGX_ERROR;
                }

                повернути NGX_CONF_BLOCK_DONE;

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

        } інакше {
            якщо (ch == '{' && variable) {
                continue;
            }

            variable = 0;

            якщо (ch == '\\') {
                quoted = 1;
                continue;
            }

            якщо (ch == '$') {
                variable = 1;
                continue;
            }

            якщо (d_quoted) {
                якщо (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } інакше якщо (s_quoted) {
                якщо (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } інакше якщо (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            якщо (found) {
                word = ngx_array_push(cf->args);
                якщо (word == NULL) {
                    повернути NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                якщо (word->data == NULL) {
                    повернути NGX_ERROR;
                }

                для (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    якщо (*src == '\\') {
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

                якщо (ch == ';') {
                    повернути NGX_OK;
                }

                якщо (ch == '{') {
                    повернути NGX_CONF_BLOCK_START;
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

    якщо (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        повернути NGX_CONF_ERROR;
    }

    якщо (strpbrk((char *) file.data, "*?[") == NULL) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        повернути ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, sizeof(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    якщо (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        повернути NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    для ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        якщо (n != NGX_OK) {
            break;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        якщо (file.data == NULL) {
            повернути NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        якщо (rv != NGX_CONF_OK) {
            break;
        }
    }

    ngx_close_glob(&gl);

    повернути rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    повернути ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#якщо (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    якщо (name->len) {
        full = *name;

        якщо (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            повернути NULL;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        для (i = 0; /* void */ ; i++) {

            якщо (i >= part->nelts) {
                якщо (part->next == NULL) {
                    break;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            якщо (full.len != file[i].name.len) {
                continue;
            }

            якщо (ngx_strcmp(full.data, file[i].name.data) == 0) {
                повернути &file[i];
            }
        }
    }

    file = ngx_list_push(&cycle->open_files);
    якщо (file == NULL) {
        повернути NULL;
    }

    якщо (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } інакше {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = NULL;
    file->data = NULL;

    повернути file;
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

    для (i = 0; /* void */ ; i++) {

        якщо (i >= part->nelts) {
            якщо (part->next == NULL) {
                break;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        якщо (file[i].flush) {
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

    якщо (err) {
        p = ngx_log_errno(p, last, err);
    }

    якщо (cf->conf_file == NULL) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        повернути;
    }

    якщо (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        повернути;
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

    якщо (*fp != NGX_CONF_UNSET) {
        повернути "is duplicate";
    }

    value = cf->args->elts;

    якщо (ngx_strcasecmp(value[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } інакше якщо (ngx_strcasecmp(value[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } інакше {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     value[1].data, cmd->name.data);
        повернути NGX_CONF_ERROR;
    }

    якщо (cmd->post) {
        post = cmd->post;
        повернути post->post_handler(cf, post, fp);
    }

    повернути NGX_CONF_OK;
}


char *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_str_t        *field, *value;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    якщо (field->data) {
        повернути "is duplicate";
    }

    value = cf->args->elts;

    *field = value[1];

    якщо (cmd->post) {
        post = cmd->post;
        повернути post->post_handler(cf, post, field);
    }

    повернути NGX_CONF_OK;
}


char *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_str_t         *value, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    якщо (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, sizeof(ngx_str_t));
        якщо (*a == NULL) {
            повернути NGX_CONF_ERROR;
        }
    }

    s = ngx_array_push(*a);
    якщо (s == NULL) {
        повернути NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    *s = value[1];

    якщо (cmd->post) {
        post = cmd->post;
        повернути post->post_handler(cf, post, s);
    }

    повернути NGX_CONF_OK;
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

    якщо (*a == NGX_CONF_UNSET_PTR || *a == NULL) {
        *a = ngx_array_create(cf->pool, 4, sizeof(ngx_keyval_t));
        якщо (*a == NULL) {
            повернути NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_push(*a);
    якщо (kv == NULL) {
        повернути NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    kv->key = value[1];
    kv->value = value[2];

    якщо (cmd->post) {
        post = cmd->post;
        повернути post->post_handler(cf, post, kv);
    }

    повернути NGX_CONF_OK;
}


char *
ngx_conf_set_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    якщо (*np != NGX_CONF_UNSET) {
        повернути "is duplicate";
    }

    value = cf->args->elts;
    *np = ngx_atoi(value[1].data, value[1].len);
    якщо (*np == NGX_ERROR) {
        повернути "invalid number";
    }

    якщо (cmd->post) {
        post = cmd->post;
        повернути post->post_handler(cf, post, np);
    }

    повернути NGX_CONF_OK;
}


char *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    size_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    якщо (*sp != NGX_CONF_UNSET_SIZE) {
        повернути "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_size(&value[1]);
    якщо (*sp == (size_t) NGX_ERROR) {
        повернути "invalid value";
    }

    якщо (cmd->post) {
        post = cmd->post;
        повернути post->post_handler(cf, post, sp);
    }

    повернути NGX_CONF_OK;
}


char *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    off_t            *op;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    якщо (*op != NGX_CONF_UNSET) {
        повернути "is duplicate";
    }

    value = cf->args->elts;

    *op = ngx_parse_offset(&value[1]);
    якщо (*op == (off_t) NGX_ERROR) {
        повернути "invalid value";
    }

    якщо (cmd->post) {
        post = cmd->post;
        повернути post->post_handler(cf, post, op);
    }

    повернути NGX_CONF_OK;
}


char *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    якщо (*msp != NGX_CONF_UNSET_MSEC) {
        повернути "is duplicate";
    }

    value = cf->args->elts;

    *msp = ngx_parse_time(&value[1], 0);
    якщо (*msp == (ngx_msec_t) NGX_ERROR) {
        повернути "invalid value";
    }

    якщо (cmd->post) {
        post = cmd->post;
        повернути post->post_handler(cf, post, msp);
    }

    повернути NGX_CONF_OK;
}


char *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    time_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    якщо (*sp != NGX_CONF_UNSET) {
        повернути "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_time(&value[1], 1);
    якщо (*sp == (time_t) NGX_ERROR) {
        повернути "invalid value";
    }

    якщо (cmd->post) {
        post = cmd->post;
        повернути post->post_handler(cf, post, sp);
    }

    повернути NGX_CONF_OK;
}


char *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char *p = conf;

    ngx_str_t   *value;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    якщо (bufs->num) {
        повернути "is duplicate";
    }

    value = cf->args->elts;

    bufs->num = ngx_atoi(value[1].data, value[1].len);
    якщо (bufs->num == NGX_ERROR || bufs->num == 0) {
        повернути "invalid value";
    }

    bufs->size = ngx_parse_size(&value[2]);
    якщо (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        повернути "invalid value";
    }

    повернути NGX_CONF_OK;
}


char *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *value;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    якщо (*np != NGX_CONF_UNSET_UINT) {
        повернути "is duplicate";
    }

    value = cf->args->elts;
    e = cmd->post;

    для (i = 0; e[i].name.len != 0; i++) {
        якщо (e[i].name.len != value[1].len
            || ngx_strcasecmp(e[i].name.data, value[1].data) != 0)
        {
            continue;
        }

        *np = e[i].value;

        повернути NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", value[1].data);

    повернути NGX_CONF_ERROR;
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

    для (i = 1; i < cf->args->nelts; i++) {
        для (m = 0; mask[m].name.len != 0; m++) {

            якщо (mask[m].name.len != value[i].len
                || ngx_strcasecmp(mask[m].name.data, value[i].data) != 0)
            {
                continue;
            }

            якщо (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", value[i].data);

            } інакше {
                *np |= mask[m].mask;
            }

            break;
        }

        якщо (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", value[i].data);

            повернути NGX_CONF_ERROR;
        }
    }

    повернути NGX_CONF_OK;
}


#якщо 0

char *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    повернути "unsupported on this platform";
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

    повернути NGX_CONF_OK;
}


char *
ngx_conf_check_num_bounds(ngx_conf_t *cf, void *post, void *data)
{
    ngx_conf_num_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    якщо (bounds->high == -1) {
        якщо (*np >= bounds->low) {
            повернути NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->low);

        повернути NGX_CONF_ERROR;
    }

    якщо (*np >= bounds->low && *np <= bounds->high) {
        повернути NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->low, bounds->high);

    повернути NGX_CONF_ERROR;
}


char *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, void *post, void *data)
{
    size_t  *sp = data;

    якщо (*sp == 0) {
        повернути "value must be nonzero";
    }

    повернути NGX_CONF_OK;
}
