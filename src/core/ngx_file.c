
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_int_t ngx_test_full_name(ngx_str_t *name);


ساكن ngx_atomic_t   temp_number = 0;
ngx_atomic_t         *ngx_temp_number = &temp_number;
ngx_atomic_int_t      ngx_random_number = 123456;


ngx_int_t
ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix, ngx_str_t *name)
{
    size_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_full_name(name);

    𐤀𐤌 (rc == NGX_OK) {
        أرجع rc;
    }

    len = prefix->len;

#𐤀𐤌 (NGX_WIN32)

    𐤀𐤌 (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    𐤀𐤌 (n == عدم) {
        أرجع NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->data, len);
    ngx_cpystrn(p, name->data, name->len + 1);

    name->len += len;
    name->data = n;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#𐤀𐤌 (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    𐤀𐤌 (name->len < 2) {
        𐤀𐤌 (c0 == '/') {
            أرجع 2;
        }

        أرجع NGX_DECLINED;
    }

    c1 = name->data[1];

    𐤀𐤌 (c1 == ':') {
        c0 |= 0x20;

        𐤀𐤌 ((c0 >= 'a' && c0 <= 'z')) {
            أرجع NGX_OK;
        }

        أرجع NGX_DECLINED;
    }

    𐤀𐤌 (c1 == '/') {
        أرجع NGX_OK;
    }

    𐤀𐤌 (c0 == '/') {
        أرجع 2;
    }

    أرجع NGX_DECLINED;

#وإلا

    𐤀𐤌 (name->data[0] == '/') {
        أرجع NGX_OK;
    }

    أرجع NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    𐤀𐤌 (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        𐤀𐤌 (rc != NGX_OK) {
            أرجع rc;
        }

        𐤀𐤌 (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#𐤀𐤌 (NGX_THREADS && NGX_HAVE_PWRITEV)

    𐤀𐤌 (tf->thread_write) {
        أرجع ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    أرجع ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
}


ngx_int_t
ngx_create_temp_file(ngx_file_t *file, ngx_path_t *path, ngx_pool_t *pool,
    ngx_uint_t persistent, ngx_uint_t clean, ngx_uint_t access)
{
    size_t                    levels;
    u_char                   *p;
    uint32_t                  n;
    ngx_err_t                 err;
    ngx_str_t                 name;
    ngx_uint_t                prefix;
    ngx_pool_cleanup_t       *cln;
    ngx_pool_cleanup_file_t  *clnf;

    𐤀𐤌 (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } وإلا {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.data = ngx_pnalloc(pool, file->name.len + 1);
    𐤀𐤌 (file->name.data == عدم) {
        أرجع NGX_ERROR;
    }

#𐤀𐤌 0
    لكل (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    𐤀𐤌 (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_number(0);

    cln = ngx_pool_cleanup_add(pool, حجم(ngx_pool_cleanup_file_t));
    𐤀𐤌 (cln == عدم) {
        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        (فراغ) ngx_sprintf(p, "%010uD%Z", n);

        𐤀𐤌 (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        𐤀𐤌 (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            أرجع NGX_OK;
        }

        err = ngx_errno;

        𐤀𐤌 (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_number(1);
            استمر;
        }

        𐤀𐤌 ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            أرجع NGX_ERROR;
        }

        𐤀𐤌 (ngx_create_path(file, path) == NGX_ERROR) {
            أرجع NGX_ERROR;
        }
    }
}


فراغ
ngx_create_hashed_filename(ngx_path_t *path, u_char *file, size_t len)
{
    size_t      i, level;
    ngx_uint_t  n;

    i = path->name.len + 1;

    file[path->name.len + path->len]  = '/';

    لكل (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        𐤀𐤌 (level == 0) {
            اكسر;
        }

        len -= level;
        file[i - 1] = '/';
        ngx_memcpy(&file[i], &file[len], level);
        i += level + 1;
    }
}


ngx_int_t
ngx_create_path(ngx_file_t *file, ngx_path_t *path)
{
    size_t      pos;
    ngx_err_t   err;
    ngx_uint_t  i;

    pos = path->name.len;

    لكل (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        𐤀𐤌 (path->level[i] == 0) {
            اكسر;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        𐤀𐤌 (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            𐤀𐤌 (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.data);
                أرجع NGX_ERROR;
            }
        }

        file->name.data[pos] = '/';
    }

    أرجع NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#𐤀𐤌 (NGX_WIN32)
    p = dir + 3;
#وإلا
    p = dir + 1;
#endif

    لكل ( /* void */ ; *p; p++) {
        ch = *p;

        𐤀𐤌 (ch != '/') {
            استمر;
        }

        *p = '\0';

        𐤀𐤌 (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            بدل (err) {
            حالة NGX_EEXIST:
                err = 0;
            حالة NGX_EACCES:
                اكسر;

            افتراضي:
                أرجع err;
            }
        }

        *p = '/';
    }

    أرجع err;
}


ngx_atomic_uint_t
ngx_next_temp_number(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_number : 1;

    n = ngx_atomic_fetch_add(ngx_temp_number, add);

    أرجع n + add;
}


حرف *
ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ssize_t      level;
    ngx_str_t   *value;
    ngx_uint_t   i, n;
    ngx_path_t  *path, **slot;

    slot = (ngx_path_t **) (p + cmd->offset);

    𐤀𐤌 (*slot) {
        أرجع "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, حجم(ngx_path_t));
    𐤀𐤌 (path == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    path->name = value[1];

    𐤀𐤌 (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    𐤀𐤌 (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->line = cf->conf_file->line;

    لكل (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(value[n].data, value[n].len);
        𐤀𐤌 (level == NGX_ERROR || level == 0) {
            أرجع "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    𐤀𐤌 (path->len > 10 + i) {
        أرجع "invalid value";
    }

    *slot = path;

    𐤀𐤌 (ngx_add_path(cf, slot) == NGX_ERROR) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_merge_path_value(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    𐤀𐤌 (*path) {
        أرجع NGX_CONF_OK;
    }

    𐤀𐤌 (prev) {
        *path = prev;
        أرجع NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, حجم(ngx_path_t));
    𐤀𐤌 (*path == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    𐤀𐤌 (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    لكل (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    𐤀𐤌 (ngx_add_path(cf, path) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *confp = conf;

    u_char      *p;
    ngx_str_t   *value;
    ngx_uint_t   i, right, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    𐤀𐤌 (*access != NGX_CONF_UNSET_UINT) {
        أرجع "is duplicate";
    }

    value = cf->args->elts;

    *access = 0;
    user = 0600;

    لكل (i = 1; i < cf->args->nelts; i++) {

        p = value[i].data;

        𐤀𐤌 (ngx_strncmp(p, "user:", حجم("user:") - 1) == 0) {
            shift = 6;
            p += حجم("user:") - 1;
            user = 0;

        } وإلا 𐤀𐤌 (ngx_strncmp(p, "group:", حجم("group:") - 1) == 0) {
            shift = 3;
            p += حجم("group:") - 1;

        } وإلا 𐤀𐤌 (ngx_strncmp(p, "all:", حجم("all:") - 1) == 0) {
            shift = 0;
            p += حجم("all:") - 1;

        } وإلا {
            اذهب invalid;
        }

        𐤀𐤌 (ngx_strcmp(p, "rw") == 0) {
            right = 6;

        } وإلا 𐤀𐤌 (ngx_strcmp(p, "r") == 0) {
            right = 4;

        } وإلا {
            اذهب invalid;
        }

        *access |= right << shift;
    }

    *access |= user;

    أرجع NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &value[i]);

    أرجع NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    لكل (i = 0; i < cf->cycle->paths.nelts; i++) {
        𐤀𐤌 (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            𐤀𐤌 (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->line);
                أرجع NGX_ERROR;
            }

            لكل (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                𐤀𐤌 (p[i]->level[n] != path->level[n]) {
                    𐤀𐤌 (path->conf_file == عدم) {
                        𐤀𐤌 (p[i]->conf_file == عدم) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            أرجع NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->line);
                        أرجع NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->line);
                    أرجع NGX_ERROR;
                }

                𐤀𐤌 (p[i]->level[n] == 0) {
                    اكسر;
                }
            }

            *slot = p[i];

            أرجع NGX_OK;
        }
    }

    p = ngx_array_push(&cf->cycle->paths);
    𐤀𐤌 (p == عدم) {
        أرجع NGX_ERROR;
    }

    *p = path;

    أرجع NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    لكل (i = 0; i < cycle->paths.nelts; i++) {

        𐤀𐤌 (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            𐤀𐤌 (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                أرجع NGX_ERROR;
            }
        }

        𐤀𐤌 (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            استمر;
        }

#𐤀𐤌 !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        𐤀𐤌 (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            أرجع NGX_ERROR;
        }

        𐤀𐤌 (fi.st_uid != user) {
            𐤀𐤌 (chown((ثابت حرف *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                أرجع NGX_ERROR;
            }
        }

        𐤀𐤌 ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            𐤀𐤌 (chmod((ثابت حرف *) path[i]->name.data, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.data);
                أرجع NGX_ERROR;
            }
        }
        }
#endif
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#𐤀𐤌 !(NGX_WIN32)

    𐤀𐤌 (ext->access) {
        𐤀𐤌 (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            اذهب failed;
        }
    }

#endif

    𐤀𐤌 (ext->time != -1) {
        𐤀𐤌 (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            اذهب failed;
        }
    }

    𐤀𐤌 (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        أرجع NGX_OK;
    }

    err = ngx_errno;

    𐤀𐤌 (err == NGX_ENOPATH) {

        𐤀𐤌 (!ext->create_path) {
            اذهب failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        𐤀𐤌 (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            اذهب failed;
        }

        𐤀𐤌 (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            أرجع NGX_OK;
        }

        err = ngx_errno;
    }

#𐤀𐤌 (NGX_WIN32)

    𐤀𐤌 (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        𐤀𐤌 (err == 0) {
            أرجع NGX_OK;
        }
    }

#endif

    𐤀𐤌 (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        𐤀𐤌 (name == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_number(0));

        𐤀𐤌 (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            𐤀𐤌 (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                𐤀𐤌 (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->data);
                    أرجع NGX_ERROR;
                }

                أرجع NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->data);

            𐤀𐤌 (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    𐤀𐤌 (ext->delete_file) {
        𐤀𐤌 (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    𐤀𐤌 (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->data, to->data);
    }

    أرجع NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf)
{
    حرف             *buf;
    off_t             size;
    time_t            time;
    size_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = عدم;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    𐤀𐤌 (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        اذهب failed;
    }

    𐤀𐤌 (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } وإلا {
        𐤀𐤌 (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            اذهب failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    𐤀𐤌 ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    𐤀𐤌 (buf == عدم) {
        اذهب failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    𐤀𐤌 (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        اذهب failed;
    }

    طالما (size > 0) {

        𐤀𐤌 ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        𐤀𐤌 (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            اذهب failed;
        }

        𐤀𐤌 ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            اذهب failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        𐤀𐤌 (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            اذهب failed;
        }

        𐤀𐤌 ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            اذهب failed;
        }

        size -= n;
    }

    𐤀𐤌 (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        اذهب failed;
    }

    rc = NGX_OK;

failed:

    𐤀𐤌 (nfd != NGX_INVALID_FILE) {
        𐤀𐤌 (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    𐤀𐤌 (fd != NGX_INVALID_FILE) {
        𐤀𐤌 (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    𐤀𐤌 (buf) {
        ngx_free(buf);
    }

    أرجع rc;
}


/*
 * ctx->init_handler() - see ctx->alloc
 * ctx->file_handler() - file handler
 * ctx->pre_tree_handler() - handler is called before entering directory
 * ctx->post_tree_handler() - handler is called after leaving directory
 * ctx->spec_handler() - special (socket, FIFO, etc.) file handler
 *
 * ctx->data - some data structure, it may be the same on all levels, or
 *     reallocated if ctx->alloc is nonzero
 *
 * ctx->alloc - a size of data structure that is allocated at every level
 *     and is initialized by ctx->init_handler()
 *
 * ctx->log - a log
 *
 * on fatal (memory) error handler must return NGX_ABORT to stop walking tree
 */

ngx_int_t
ngx_walk_tree(ngx_tree_ctx_t *ctx, ngx_str_t *tree)
{
    فراغ       *data, *prev;
    u_char     *p, *name;
    size_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                   "walk tree \"%V\"", tree);

    𐤀𐤌 (ngx_open_dir(tree, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", tree->data);
        أرجع NGX_ERROR;
    }

    prev = ctx->data;

    𐤀𐤌 (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        𐤀𐤌 (data == عدم) {
            اذهب failed;
        }

        𐤀𐤌 (ctx->init_handler(data, prev) == NGX_ABORT) {
            اذهب failed;
        }

        ctx->data = data;

    } وإلا {
        data = عدم;
    }

    لكل ( ;; ) {

        ngx_set_errno(0);

        𐤀𐤌 (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            𐤀𐤌 (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } وإلا {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, err,
                              ngx_read_dir_n " \"%s\" failed", tree->data);
                rc = NGX_ERROR;
            }

            اذهب done;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                      "tree name %uz:\"%s\"", len, name);

        𐤀𐤌 (len == 1 && name[0] == '.') {
            استمر;
        }

        𐤀𐤌 (len == 2 && name[0] == '.' && name[1] == '.') {
            استمر;
        }

        file.len = tree->len + 1 + len;

        𐤀𐤌 (file.len > buf.len) {

            𐤀𐤌 (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = tree->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            𐤀𐤌 (buf.data == عدم) {
                اذهب failed;
            }
        }

        p = ngx_cpymem(buf.data, tree->data, tree->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        𐤀𐤌 (!dir.valid_info) {
            𐤀𐤌 (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                استمر;
            }
        }

        𐤀𐤌 (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            𐤀𐤌 (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

        } وإلا 𐤀𐤌 (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_tree_handler(ctx, &file);

            𐤀𐤌 (rc == NGX_ABORT) {
                اذهب failed;
            }

            𐤀𐤌 (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                استمر;
            }

            𐤀𐤌 (ngx_walk_tree(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            𐤀𐤌 (ctx->post_tree_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

        } وإلا {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            𐤀𐤌 (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    𐤀𐤌 (buf.len) {
        ngx_free(buf.data);
    }

    𐤀𐤌 (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    𐤀𐤌 (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", tree->data);
    }

    أرجع rc;
}
