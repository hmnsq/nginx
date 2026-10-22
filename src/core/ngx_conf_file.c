
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

    nếu (param->len == 0) {
        trả_về NGX_CONF_OK;
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

    trả_về rv;
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

    nếu (sn) {
        cf->conf_file->dump = NULL;
        trả_về NGX_OK;
    }

    p = ngx_pstrdup(cf->cycle->pool, filename);
    nếu (p == NULL) {
        trả_về NGX_ERROR;
    }

    cd = ngx_array_push(&cf->cycle->config_dump);
    nếu (cd == NULL) {
        trả_về NGX_ERROR;
    }

    size = ngx_file_size(&cf->conf_file->file.info);

    buf = ngx_create_temp_buf(cf->cycle->pool, (size_t) size);
    nếu (buf == NULL) {
        trả_về NGX_ERROR;
    }

    cd->name.data = p;
    cd->name.len = filename->len;
    cd->buffer = buf;

    cf->conf_file->dump = buf;

    sn = ngx_palloc(cf->temp_pool, sizeof(ngx_str_node_t));
    nếu (sn == NULL) {
        trả_về NGX_ERROR;
    }

    sn->node.key = hash;
    sn->str = cd->name;

    ngx_rbtree_insert(&cf->cycle->config_dump_rbtree, &sn->node);

    trả_về NGX_OK;
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

#nếu (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = NULL;
#endif

    nếu (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

        nếu (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            trả_về NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        nếu (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        nếu (buf.start == NULL) {
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

        nếu (ngx_dump_config
#nếu (NGX_CONTROL_API)
            || ngx_control_api_enabled
#endif
#nếu (NGX_DEBUG)
            || 1
#endif
           )
        {
            nếu (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                goto failed;
            }

        } không_thì {
            cf->conf_file->dump = NULL;
        }

    } không_thì nếu (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    } không_thì {
        type = parse_param;
    }


    cho ( ;; ) {
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

        nếu (rc == NGX_ERROR) {
            goto done;
        }

        nếu (rc == NGX_CONF_BLOCK_DONE) {

            nếu (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                goto failed;
            }

            goto done;
        }

        nếu (rc == NGX_CONF_FILE_DONE) {

            nếu (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                goto failed;
            }

            goto done;
        }

        nếu (rc == NGX_CONF_BLOCK_START) {

            nếu (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                goto failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        nếu (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            nếu (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                goto failed;
            }

            rv = (*cf->handler)(cf, NULL, cf->handler_conf);
            nếu (rv == NGX_CONF_OK) {
                continue;
            }

            nếu (rv == NGX_CONF_ERROR) {
                goto failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "%s", rv);

            goto failed;
        }


        rc = ngx_conf_handler(cf, rc);

        nếu (rc == NGX_ERROR) {
            goto failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    nếu (filename) {
        nếu (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        nếu (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    nếu (rc == NGX_ERROR) {
        trả_về NGX_CONF_ERROR;
    }

    trả_về NGX_CONF_OK;
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

    cho (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        nếu (cmd == NULL) {
            continue;
        }

        cho ( /* void */ ; cmd->name.len; cmd++) {

            nếu (name->len != cmd->name.len) {
                continue;
            }

            nếu (ngx_strcmp(name->data, cmd->name.data) != 0) {
                continue;
            }

            found = 1;

            nếu (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                continue;
            }

            /* is the directive's location right ? */

            nếu (!(cmd->type & cf->cmd_type)) {
                continue;
            }

            nếu (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "directive \"%s\" is not terminated by \";\"",
                                  name->data);
                trả_về NGX_ERROR;
            }

            nếu ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                trả_về NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            nếu (!(cmd->type & NGX_CONF_ANY)) {

                nếu (cmd->type & NGX_CONF_FLAG) {

                    nếu (cf->args->nelts != 2) {
                        goto invalid;
                    }

                } không_thì nếu (cmd->type & NGX_CONF_1MORE) {

                    nếu (cf->args->nelts < 2) {
                        goto invalid;
                    }

                } không_thì nếu (cmd->type & NGX_CONF_2MORE) {

                    nếu (cf->args->nelts < 3) {
                        goto invalid;
                    }

                } không_thì nếu (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    goto invalid;

                } không_thì nếu (!(cmd->type & argument_number[cf->args->nelts - 1]))
                {
                    goto invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = NULL;

            nếu (cmd->type & NGX_DIRECT_CONF) {
                conf = ((void **) cf->ctx)[cf->cycle->modules[i]->index];

            } không_thì nếu (cmd->type & NGX_MAIN_CONF) {
                conf = &(((void **) cf->ctx)[cf->cycle->modules[i]->index]);

            } không_thì nếu (cf->ctx) {
                confp = *(void **) ((char *) cf->ctx + cmd->conf);

                nếu (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            rv = cmd->set(cf, cmd, conf);

            nếu (rv == NGX_CONF_OK) {
                trả_về NGX_OK;
            }

            nếu (rv == NGX_CONF_ERROR) {
                trả_về NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            trả_về NGX_ERROR;
        }
    }

    nếu (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        trả_về NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    trả_về NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    trả_về NGX_ERROR;
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

    cho ( ;; ) {

        nếu (b->pos >= b->last) {

            nếu (cf->conf_file->file.offset >= file_size) {

                nếu (cf->args->nelts > 0 || !last_space) {

                    nếu (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        trả_về NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected end of file, "
                                       "expecting \";\" or \"}\"");
                    trả_về NGX_ERROR;
                }

                trả_về NGX_CONF_FILE_DONE;
            }

            len = b->pos - start;

            nếu (len == NGX_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                nếu (d_quoted) {
                    ch = '"';

                } không_thì nếu (s_quoted) {
                    ch = '\'';

                } không_thì {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       (size_t) 10, start);
                    trả_về NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                trả_về NGX_ERROR;
            }

            nếu (len) {
                ngx_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            nếu (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            nếu (n == NGX_ERROR) {
                trả_về NGX_ERROR;
            }

            nếu (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                trả_về NGX_ERROR;
            }

            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            nếu (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        ch = *b->pos++;

        nếu (ch == LF) {
            cf->conf_file->line++;

            nếu (sharp_comment) {
                sharp_comment = 0;
            }
        }

        nếu (sharp_comment) {
            continue;
        }

        nếu (quoted) {
            quoted = 0;
            continue;
        }

        nếu (need_space) {
            nếu (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                continue;
            }

            nếu (ch == ';') {
                trả_về NGX_OK;
            }

            nếu (ch == '{') {
                trả_về NGX_CONF_BLOCK_START;
            }

            nếu (ch == ')') {
                last_space = 1;
                need_space = 0;

            } không_thì {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                trả_về NGX_ERROR;
            }
        }

        nếu (last_space) {

            start = b->pos - 1;
            start_line = cf->conf_file->line;

            nếu (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                continue;
            }

            switch (ch) {

            case ';':
            case '{':
                nếu (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    trả_về NGX_ERROR;
                }

                nếu (ch == '{') {
                    trả_về NGX_CONF_BLOCK_START;
                }

                trả_về NGX_OK;

            case '}':
                nếu (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    trả_về NGX_ERROR;
                }

                trả_về NGX_CONF_BLOCK_DONE;

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

        } không_thì {
            nếu (ch == '{' && variable) {
                continue;
            }

            variable = 0;

            nếu (ch == '\\') {
                quoted = 1;
                continue;
            }

            nếu (ch == '$') {
                variable = 1;
                continue;
            }

            nếu (d_quoted) {
                nếu (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } không_thì nếu (s_quoted) {
                nếu (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } không_thì nếu (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            nếu (found) {
                word = ngx_array_push(cf->args);
                nếu (word == NULL) {
                    trả_về NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                nếu (word->data == NULL) {
                    trả_về NGX_ERROR;
                }

                cho (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    nếu (*src == '\\') {
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

                nếu (ch == ';') {
                    trả_về NGX_OK;
                }

                nếu (ch == '{') {
                    trả_về NGX_CONF_BLOCK_START;
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

    nếu (ngx_conf_full_name(cf->cycle, &file, 1) != NGX_OK) {
        trả_về NGX_CONF_ERROR;
    }

    nếu (strpbrk((char *) file.data, "*?[") == NULL) {

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        trả_về ngx_conf_parse(cf, &file);
    }

    ngx_memzero(&gl, sizeof(ngx_glob_t));

    gl.pattern = file.data;
    gl.log = cf->log;
    gl.test = 1;

    nếu (ngx_open_glob(&gl) != NGX_OK) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           ngx_open_glob_n " \"%s\" failed", file.data);
        trả_về NGX_CONF_ERROR;
    }

    rv = NGX_CONF_OK;

    cho ( ;; ) {
        n = ngx_read_glob(&gl, &name);

        nếu (n != NGX_OK) {
            break;
        }

        file.len = name.len++;
        file.data = ngx_pstrdup(cf->pool, &name);
        nếu (file.data == NULL) {
            trả_về NGX_CONF_ERROR;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cf->log, 0, "include %s", file.data);

        rv = ngx_conf_parse(cf, &file);

        nếu (rv != NGX_CONF_OK) {
            break;
        }
    }

    ngx_close_glob(&gl);

    trả_về rv;
}


ngx_int_t
ngx_conf_full_name(ngx_cycle_t *cycle, ngx_str_t *name, ngx_uint_t conf_prefix)
{
    ngx_str_t  *prefix;

    prefix = conf_prefix ? &cycle->conf_prefix : &cycle->prefix;

    trả_về ngx_get_full_name(cycle->pool, prefix, name);
}


ngx_open_file_t *
ngx_conf_open_file(ngx_cycle_t *cycle, ngx_str_t *name)
{
    ngx_str_t         full;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

#nếu (NGX_SUPPRESS_WARN)
    ngx_str_null(&full);
#endif

    nếu (name->len) {
        full = *name;

        nếu (ngx_conf_full_name(cycle, &full, 0) != NGX_OK) {
            trả_về NULL;
        }

        part = &cycle->open_files.part;
        file = part->elts;

        cho (i = 0; /* void */ ; i++) {

            nếu (i >= part->nelts) {
                nếu (part->next == NULL) {
                    break;
                }
                part = part->next;
                file = part->elts;
                i = 0;
            }

            nếu (full.len != file[i].name.len) {
                continue;
            }

            nếu (ngx_strcmp(full.data, file[i].name.data) == 0) {
                trả_về &file[i];
            }
        }
    }

    file = ngx_list_push(&cycle->open_files);
    nếu (file == NULL) {
        trả_về NULL;
    }

    nếu (name->len) {
        file->fd = NGX_INVALID_FILE;
        file->name = full;

    } không_thì {
        file->fd = ngx_stderr;
        file->name = *name;
    }

    file->flush = NULL;
    file->data = NULL;

    trả_về file;
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

    cho (i = 0; /* void */ ; i++) {

        nếu (i >= part->nelts) {
            nếu (part->next == NULL) {
                break;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        nếu (file[i].flush) {
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

    nếu (err) {
        p = ngx_log_errno(p, last, err);
    }

    nếu (cf->conf_file == NULL) {
        ngx_log_error(level, cf->log, 0, "%*s", p - errstr, errstr);
        trả_về;
    }

    nếu (cf->conf_file->file.fd == NGX_INVALID_FILE) {
        ngx_log_error(level, cf->log, 0, "%*s in command line",
                      p - errstr, errstr);
        trả_về;
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

    nếu (*fp != NGX_CONF_UNSET) {
        trả_về "is duplicate";
    }

    value = cf->args->elts;

    nếu (ngx_strcasecmp(value[1].data, (u_char *) "on") == 0) {
        *fp = 1;

    } không_thì nếu (ngx_strcasecmp(value[1].data, (u_char *) "off") == 0) {
        *fp = 0;

    } không_thì {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                     "invalid value \"%s\" in \"%s\" directive, "
                     "it must be \"on\" or \"off\"",
                     value[1].data, cmd->name.data);
        trả_về NGX_CONF_ERROR;
    }

    nếu (cmd->post) {
        post = cmd->post;
        trả_về post->post_handler(cf, post, fp);
    }

    trả_về NGX_CONF_OK;
}


char *
ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_str_t        *field, *value;
    ngx_conf_post_t  *post;

    field = (ngx_str_t *) (p + cmd->offset);

    nếu (field->data) {
        trả_về "is duplicate";
    }

    value = cf->args->elts;

    *field = value[1];

    nếu (cmd->post) {
        post = cmd->post;
        trả_về post->post_handler(cf, post, field);
    }

    trả_về NGX_CONF_OK;
}


char *
ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_str_t         *value, *s;
    ngx_array_t      **a;
    ngx_conf_post_t   *post;

    a = (ngx_array_t **) (p + cmd->offset);

    nếu (*a == NGX_CONF_UNSET_PTR) {
        *a = ngx_array_create(cf->pool, 4, sizeof(ngx_str_t));
        nếu (*a == NULL) {
            trả_về NGX_CONF_ERROR;
        }
    }

    s = ngx_array_push(*a);
    nếu (s == NULL) {
        trả_về NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    *s = value[1];

    nếu (cmd->post) {
        post = cmd->post;
        trả_về post->post_handler(cf, post, s);
    }

    trả_về NGX_CONF_OK;
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

    nếu (*a == NGX_CONF_UNSET_PTR || *a == NULL) {
        *a = ngx_array_create(cf->pool, 4, sizeof(ngx_keyval_t));
        nếu (*a == NULL) {
            trả_về NGX_CONF_ERROR;
        }
    }

    kv = ngx_array_push(*a);
    nếu (kv == NULL) {
        trả_về NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    kv->key = value[1];
    kv->value = value[2];

    nếu (cmd->post) {
        post = cmd->post;
        trả_về post->post_handler(cf, post, kv);
    }

    trả_về NGX_CONF_OK;
}


char *
ngx_conf_set_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_int_t        *np;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    np = (ngx_int_t *) (p + cmd->offset);

    nếu (*np != NGX_CONF_UNSET) {
        trả_về "is duplicate";
    }

    value = cf->args->elts;
    *np = ngx_atoi(value[1].data, value[1].len);
    nếu (*np == NGX_ERROR) {
        trả_về "invalid number";
    }

    nếu (cmd->post) {
        post = cmd->post;
        trả_về post->post_handler(cf, post, np);
    }

    trả_về NGX_CONF_OK;
}


char *
ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    size_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (size_t *) (p + cmd->offset);
    nếu (*sp != NGX_CONF_UNSET_SIZE) {
        trả_về "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_size(&value[1]);
    nếu (*sp == (size_t) NGX_ERROR) {
        trả_về "invalid value";
    }

    nếu (cmd->post) {
        post = cmd->post;
        trả_về post->post_handler(cf, post, sp);
    }

    trả_về NGX_CONF_OK;
}


char *
ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    off_t            *op;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    op = (off_t *) (p + cmd->offset);
    nếu (*op != NGX_CONF_UNSET) {
        trả_về "is duplicate";
    }

    value = cf->args->elts;

    *op = ngx_parse_offset(&value[1]);
    nếu (*op == (off_t) NGX_ERROR) {
        trả_về "invalid value";
    }

    nếu (cmd->post) {
        post = cmd->post;
        trả_về post->post_handler(cf, post, op);
    }

    trả_về NGX_CONF_OK;
}


char *
ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_msec_t       *msp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    msp = (ngx_msec_t *) (p + cmd->offset);
    nếu (*msp != NGX_CONF_UNSET_MSEC) {
        trả_về "is duplicate";
    }

    value = cf->args->elts;

    *msp = ngx_parse_time(&value[1], 0);
    nếu (*msp == (ngx_msec_t) NGX_ERROR) {
        trả_về "invalid value";
    }

    nếu (cmd->post) {
        post = cmd->post;
        trả_về post->post_handler(cf, post, msp);
    }

    trả_về NGX_CONF_OK;
}


char *
ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    time_t           *sp;
    ngx_str_t        *value;
    ngx_conf_post_t  *post;


    sp = (time_t *) (p + cmd->offset);
    nếu (*sp != NGX_CONF_UNSET) {
        trả_về "is duplicate";
    }

    value = cf->args->elts;

    *sp = ngx_parse_time(&value[1], 1);
    nếu (*sp == (time_t) NGX_ERROR) {
        trả_về "invalid value";
    }

    nếu (cmd->post) {
        post = cmd->post;
        trả_về post->post_handler(cf, post, sp);
    }

    trả_về NGX_CONF_OK;
}


char *
ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char *p = conf;

    ngx_str_t   *value;
    ngx_bufs_t  *bufs;


    bufs = (ngx_bufs_t *) (p + cmd->offset);
    nếu (bufs->num) {
        trả_về "is duplicate";
    }

    value = cf->args->elts;

    bufs->num = ngx_atoi(value[1].data, value[1].len);
    nếu (bufs->num == NGX_ERROR || bufs->num == 0) {
        trả_về "invalid value";
    }

    bufs->size = ngx_parse_size(&value[2]);
    nếu (bufs->size == (size_t) NGX_ERROR || bufs->size == 0) {
        trả_về "invalid value";
    }

    trả_về NGX_CONF_OK;
}


char *
ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char  *p = conf;

    ngx_uint_t       *np, i;
    ngx_str_t        *value;
    ngx_conf_enum_t  *e;

    np = (ngx_uint_t *) (p + cmd->offset);

    nếu (*np != NGX_CONF_UNSET_UINT) {
        trả_về "is duplicate";
    }

    value = cf->args->elts;
    e = cmd->post;

    cho (i = 0; e[i].name.len != 0; i++) {
        nếu (e[i].name.len != value[1].len
            || ngx_strcasecmp(e[i].name.data, value[1].data) != 0)
        {
            continue;
        }

        *np = e[i].value;

        trả_về NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid value \"%s\"", value[1].data);

    trả_về NGX_CONF_ERROR;
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

    cho (i = 1; i < cf->args->nelts; i++) {
        cho (m = 0; mask[m].name.len != 0; m++) {

            nếu (mask[m].name.len != value[i].len
                || ngx_strcasecmp(mask[m].name.data, value[i].data) != 0)
            {
                continue;
            }

            nếu (*np & mask[m].mask) {
                ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                                   "duplicate value \"%s\"", value[i].data);

            } không_thì {
                *np |= mask[m].mask;
            }

            break;
        }

        nếu (mask[m].name.len == 0) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid value \"%s\"", value[i].data);

            trả_về NGX_CONF_ERROR;
        }
    }

    trả_về NGX_CONF_OK;
}


#nếu 0

char *
ngx_conf_unsupported(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    trả_về "unsupported on this platform";
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

    trả_về NGX_CONF_OK;
}


char *
ngx_conf_check_num_bounds(ngx_conf_t *cf, void *post, void *data)
{
    ngx_conf_num_bounds_t  *bounds = post;
    ngx_int_t  *np = data;

    nếu (bounds->high == -1) {
        nếu (*np >= bounds->low) {
            trả_về NGX_CONF_OK;
        }

        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "value must be equal to or greater than %i",
                           bounds->low);

        trả_về NGX_CONF_ERROR;
    }

    nếu (*np >= bounds->low && *np <= bounds->high) {
        trả_về NGX_CONF_OK;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "value must be between %i and %i",
                       bounds->low, bounds->high);

    trả_về NGX_CONF_ERROR;
}


char *
ngx_conf_check_size_nonzero(ngx_conf_t *cf, void *post, void *data)
{
    size_t  *sp = data;

    nếu (*sp == 0) {
        trả_về "value must be nonzero";
    }

    trả_về NGX_CONF_OK;
}
