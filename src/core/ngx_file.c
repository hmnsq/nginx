
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

    אם (rc == NGX_OK) {
        החזר rc;
    }

    len = prefix->len;

#אם (NGX_WIN32)

    אם (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    אם (n == عدم) {
        החזר NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->data, len);
    ngx_cpystrn(p, name->data, name->len + 1);

    name->len += len;
    name->data = n;

    החזר NGX_OK;
}


ساكن ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#אם (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    אם (name->len < 2) {
        אם (c0 == '/') {
            החזר 2;
        }

        החזר NGX_DECLINED;
    }

    c1 = name->data[1];

    אם (c1 == ':') {
        c0 |= 0x20;

        אם ((c0 >= 'a' && c0 <= 'z')) {
            החזר NGX_OK;
        }

        החזר NGX_DECLINED;
    }

    אם (c1 == '/') {
        החזר NGX_OK;
    }

    אם (c0 == '/') {
        החזר 2;
    }

    החזר NGX_DECLINED;

#אחרת

    אם (name->data[0] == '/') {
        החזר NGX_OK;
    }

    החזר NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    אם (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        אם (rc != NGX_OK) {
            החזר rc;
        }

        אם (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#אם (NGX_THREADS && NGX_HAVE_PWRITEV)

    אם (tf->thread_write) {
        החזר ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    החזר ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
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

    אם (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } אחרת {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.data = ngx_pnalloc(pool, file->name.len + 1);
    אם (file->name.data == عدم) {
        החזר NGX_ERROR;
    }

#אם 0
    עבור (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    אם (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_number(0);

    cln = ngx_pool_cleanup_add(pool, حجم(ngx_pool_cleanup_file_t));
    אם (cln == عدم) {
        החזר NGX_ERROR;
    }

    עבור ( ;; ) {
        (فراغ) ngx_sprintf(p, "%010uD%Z", n);

        אם (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        אם (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            החזר NGX_OK;
        }

        err = ngx_errno;

        אם (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_number(1);
            استمر;
        }

        אם ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            החזר NGX_ERROR;
        }

        אם (ngx_create_path(file, path) == NGX_ERROR) {
            החזר NGX_ERROR;
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

    עבור (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        אם (level == 0) {
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

    עבור (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        אם (path->level[i] == 0) {
            اكسر;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        אם (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            אם (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.data);
                החזר NGX_ERROR;
            }
        }

        file->name.data[pos] = '/';
    }

    החזר NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#אם (NGX_WIN32)
    p = dir + 3;
#אחרת
    p = dir + 1;
#endif

    עבור ( /* void */ ; *p; p++) {
        ch = *p;

        אם (ch != '/') {
            استمر;
        }

        *p = '\0';

        אם (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            بدل (err) {
            حالة NGX_EEXIST:
                err = 0;
            حالة NGX_EACCES:
                اكسر;

            افتراضي:
                החזר err;
            }
        }

        *p = '/';
    }

    החזר err;
}


ngx_atomic_uint_t
ngx_next_temp_number(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_number : 1;

    n = ngx_atomic_fetch_add(ngx_temp_number, add);

    החזר n + add;
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

    אם (*slot) {
        החזר "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, حجم(ngx_path_t));
    אם (path == عدم) {
        החזר NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    path->name = value[1];

    אם (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    אם (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        החזר NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->line = cf->conf_file->line;

    עבור (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(value[n].data, value[n].len);
        אם (level == NGX_ERROR || level == 0) {
            החזר "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    אם (path->len > 10 + i) {
        החזר "invalid value";
    }

    *slot = path;

    אם (ngx_add_path(cf, slot) == NGX_ERROR) {
        החזר NGX_CONF_ERROR;
    }

    החזר NGX_CONF_OK;
}


حرف *
ngx_conf_merge_path_value(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    אם (*path) {
        החזר NGX_CONF_OK;
    }

    אם (prev) {
        *path = prev;
        החזר NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, حجم(ngx_path_t));
    אם (*path == عدم) {
        החזר NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    אם (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        החזר NGX_CONF_ERROR;
    }

    עבור (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    אם (ngx_add_path(cf, path) != NGX_OK) {
        החזר NGX_CONF_ERROR;
    }

    החזר NGX_CONF_OK;
}


حرف *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *confp = conf;

    u_char      *p;
    ngx_str_t   *value;
    ngx_uint_t   i, right, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    אם (*access != NGX_CONF_UNSET_UINT) {
        החזר "is duplicate";
    }

    value = cf->args->elts;

    *access = 0;
    user = 0600;

    עבור (i = 1; i < cf->args->nelts; i++) {

        p = value[i].data;

        אם (ngx_strncmp(p, "user:", حجم("user:") - 1) == 0) {
            shift = 6;
            p += حجم("user:") - 1;
            user = 0;

        } אחרת אם (ngx_strncmp(p, "group:", حجم("group:") - 1) == 0) {
            shift = 3;
            p += حجم("group:") - 1;

        } אחרת אם (ngx_strncmp(p, "all:", حجم("all:") - 1) == 0) {
            shift = 0;
            p += حجم("all:") - 1;

        } אחרת {
            اذهب invalid;
        }

        אם (ngx_strcmp(p, "rw") == 0) {
            right = 6;

        } אחרת אם (ngx_strcmp(p, "r") == 0) {
            right = 4;

        } אחרת {
            اذهب invalid;
        }

        *access |= right << shift;
    }

    *access |= user;

    החזר NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &value[i]);

    החזר NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    עבור (i = 0; i < cf->cycle->paths.nelts; i++) {
        אם (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            אם (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->line);
                החזר NGX_ERROR;
            }

            עבור (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                אם (p[i]->level[n] != path->level[n]) {
                    אם (path->conf_file == عدم) {
                        אם (p[i]->conf_file == عدم) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            החזר NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->line);
                        החזר NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->line);
                    החזר NGX_ERROR;
                }

                אם (p[i]->level[n] == 0) {
                    اكسر;
                }
            }

            *slot = p[i];

            החזר NGX_OK;
        }
    }

    p = ngx_array_push(&cf->cycle->paths);
    אם (p == عدم) {
        החזר NGX_ERROR;
    }

    *p = path;

    החזר NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    עבור (i = 0; i < cycle->paths.nelts; i++) {

        אם (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            אם (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                החזר NGX_ERROR;
            }
        }

        אם (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            استمر;
        }

#אם !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        אם (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            החזר NGX_ERROR;
        }

        אם (fi.st_uid != user) {
            אם (chown((ثابت حرف *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                החזר NGX_ERROR;
            }
        }

        אם ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            אם (chmod((ثابت حرف *) path[i]->name.data, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.data);
                החזר NGX_ERROR;
            }
        }
        }
#endif
    }

    החזר NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#אם !(NGX_WIN32)

    אם (ext->access) {
        אם (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            اذهب failed;
        }
    }

#endif

    אם (ext->time != -1) {
        אם (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            اذهب failed;
        }
    }

    אם (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        החזר NGX_OK;
    }

    err = ngx_errno;

    אם (err == NGX_ENOPATH) {

        אם (!ext->create_path) {
            اذهب failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        אם (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            اذهب failed;
        }

        אם (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            החזר NGX_OK;
        }

        err = ngx_errno;
    }

#אם (NGX_WIN32)

    אם (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        אם (err == 0) {
            החזר NGX_OK;
        }
    }

#endif

    אם (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        אם (name == عدم) {
            החזר NGX_ERROR;
        }

        (فراغ) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_number(0));

        אם (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            אם (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                אם (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->data);
                    החזר NGX_ERROR;
                }

                החזר NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->data);

            אם (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    אם (ext->delete_file) {
        אם (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    אם (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->data, to->data);
    }

    החזר NGX_ERROR;
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

    אם (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        اذهب failed;
    }

    אם (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } אחרת {
        אם (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            اذهب failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    אם ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    אם (buf == عدم) {
        اذهب failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    אם (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        اذهب failed;
    }

    כלעוד (size > 0) {

        אם ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        אם (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            اذهب failed;
        }

        אם ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            اذهب failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        אם (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            اذهب failed;
        }

        אם ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            اذهب failed;
        }

        size -= n;
    }

    אם (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        اذهب failed;
    }

    rc = NGX_OK;

failed:

    אם (nfd != NGX_INVALID_FILE) {
        אם (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    אם (fd != NGX_INVALID_FILE) {
        אם (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    אם (buf) {
        ngx_free(buf);
    }

    החזר rc;
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

    אם (ngx_open_dir(tree, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", tree->data);
        החזר NGX_ERROR;
    }

    prev = ctx->data;

    אם (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        אם (data == عدم) {
            اذهب failed;
        }

        אם (ctx->init_handler(data, prev) == NGX_ABORT) {
            اذهب failed;
        }

        ctx->data = data;

    } אחרת {
        data = عدم;
    }

    עבור ( ;; ) {

        ngx_set_errno(0);

        אם (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            אם (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } אחרת {
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

        אם (len == 1 && name[0] == '.') {
            استمر;
        }

        אם (len == 2 && name[0] == '.' && name[1] == '.') {
            استمر;
        }

        file.len = tree->len + 1 + len;

        אם (file.len > buf.len) {

            אם (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = tree->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            אם (buf.data == عدم) {
                اذهب failed;
            }
        }

        p = ngx_cpymem(buf.data, tree->data, tree->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        אם (!dir.valid_info) {
            אם (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                استمر;
            }
        }

        אם (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            אם (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

        } אחרת אם (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_tree_handler(ctx, &file);

            אם (rc == NGX_ABORT) {
                اذهب failed;
            }

            אם (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                استمر;
            }

            אם (ngx_walk_tree(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            אם (ctx->post_tree_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

        } אחרת {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            אם (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    אם (buf.len) {
        ngx_free(buf.data);
    }

    אם (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    אם (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", tree->data);
    }

    החזר rc;
}
