
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_int_t ngx_test_full_name(ngx_str_t *name);


ساكن ngx_atomic_t   temp_لمب = 0;
ngx_atomic_t         *ngx_temp_لمب = &temp_لمب;
ngx_atomic_int_t      ngx_random_لمب = 123456;


ngx_int_t
ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix, ngx_str_t *name)
{
    size_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_full_name(name);

    إذا (rc == NGX_OK) {
        أرجع rc;
    }

    len = prefix->len;

#إذا (NGX_WIN32)

    إذا (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    إذا (n == عدم) {
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
#إذا (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    إذا (name->len < 2) {
        إذا (c0 == '/') {
            أرجع 2;
        }

        أرجع NGX_DECLINED;
    }

    c1 = name->data[1];

    إذا (c1 == ':') {
        c0 |= 0x20;

        إذا ((c0 >= 'a' && c0 <= 'z')) {
            أرجع NGX_OK;
        }

        أرجع NGX_DECLINED;
    }

    إذا (c1 == '/') {
        أرجع NGX_OK;
    }

    إذا (c0 == '/') {
        أرجع 2;
    }

    أرجع NGX_DECLINED;

#وإلا

    إذا (name->data[0] == '/') {
        أرجع NGX_OK;
    }

    أرجع NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    إذا (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        إذا (rc != NGX_OK) {
            أرجع rc;
        }

        إذا (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#إذا (NGX_THREADS && NGX_HAVE_PWRITEV)

    إذا (tf->thread_write) {
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

    إذا (file->name.len) {
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
    إذا (file->name.data == عدم) {
        أرجع NGX_ERROR;
    }

#إذا 0
    لكل (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    إذا (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_لمب(0);

    cln = ngx_pool_cleanup_add(pool, حجم(ngx_pool_cleanup_file_t));
    إذا (cln == عدم) {
        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        (فراغ) ngx_sprintf(p, "%010uD%Z", n);

        إذا (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        إذا (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            أرجع NGX_OK;
        }

        err = ngx_errno;

        إذا (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_لمب(1);
            استمر;
        }

        إذا ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            أرجع NGX_ERROR;
        }

        إذا (ngx_create_path(file, path) == NGX_ERROR) {
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

        إذا (level == 0) {
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
        إذا (path->level[i] == 0) {
            اكسر;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        إذا (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            إذا (err != NGX_EEXIST) {
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

#إذا (NGX_WIN32)
    p = dir + 3;
#وإلا
    p = dir + 1;
#endif

    لكل ( /* void */ ; *p; p++) {
        ch = *p;

        إذا (ch != '/') {
            استمر;
        }

        *p = '\0';

        إذا (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
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
ngx_next_temp_لمب(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_لمب : 1;

    n = ngx_atomic_fetch_add(ngx_temp_لمب, add);

    أرجع n + add;
}


حرف *
ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ssize_t      level;
    ngx_str_t   *درج;
    ngx_uint_t   i, n;
    ngx_path_t  *path, **slot;

    slot = (ngx_path_t **) (p + cmd->offset);

    إذا (*slot) {
        أرجع "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, حجم(ngx_path_t));
    إذا (path == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    درج = cf->حجوجي->elts;

    path->name = درج[1];

    إذا (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    إذا (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->لي = cf->conf_file->لي;

    لكل (i = 0, n = 2; n < cf->حجوجي->nelts; i++, n++) {
        level = ngx_atoi(درج[n].data, درج[n].len);
        إذا (level == NGX_ERROR || level == 0) {
            أرجع "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    إذا (path->len > 10 + i) {
        أرجع "invalid value";
    }

    *slot = path;

    إذا (ngx_add_path(cf, slot) == NGX_ERROR) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_merge_path_درج(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    إذا (*path) {
        أرجع NGX_CONF_OK;
    }

    إذا (prev) {
        *path = prev;
        أرجع NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, حجم(ngx_path_t));
    إذا (*path == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    إذا (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    لكل (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    إذا (ngx_add_path(cf, path) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *confp = conf;

    u_char      *p;
    ngx_str_t   *درج;
    ngx_uint_t   i, دام, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    إذا (*access != NGX_CONF_UNSET_UINT) {
        أرجع "is duplicate";
    }

    درج = cf->حجوجي->elts;

    *access = 0;
    user = 0600;

    لكل (i = 1; i < cf->حجوجي->nelts; i++) {

        p = درج[i].data;

        إذا (ngx_strncmp(p, "user:", حجم("user:") - 1) == 0) {
            shift = 6;
            p += حجم("user:") - 1;
            user = 0;

        } وإلا إذا (ngx_strncmp(p, "group:", حجم("group:") - 1) == 0) {
            shift = 3;
            p += حجم("group:") - 1;

        } وإلا إذا (ngx_strncmp(p, "all:", حجم("all:") - 1) == 0) {
            shift = 0;
            p += حجم("all:") - 1;

        } وإلا {
            اذهب invalid;
        }

        إذا (ngx_strcmp(p, "rw") == 0) {
            دام = 6;

        } وإلا إذا (ngx_strcmp(p, "r") == 0) {
            دام = 4;

        } وإلا {
            اذهب invalid;
        }

        *access |= دام << shift;
    }

    *access |= user;

    أرجع NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &درج[i]);

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
        إذا (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            إذا (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->لي);
                أرجع NGX_ERROR;
            }

            لكل (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                إذا (p[i]->level[n] != path->level[n]) {
                    إذا (path->conf_file == عدم) {
                        إذا (p[i]->conf_file == عدم) {
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
                                      &p[i]->name, p[i]->conf_file, p[i]->لي);
                        أرجع NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->لي);
                    أرجع NGX_ERROR;
                }

                إذا (p[i]->level[n] == 0) {
                    اكسر;
                }
            }

            *slot = p[i];

            أرجع NGX_OK;
        }
    }

    p = ngx_array_تور(&cf->cycle->paths);
    إذا (p == عدم) {
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

        إذا (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            إذا (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                أرجع NGX_ERROR;
            }
        }

        إذا (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            استمر;
        }

#إذا !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        إذا (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            أرجع NGX_ERROR;
        }

        إذا (fi.st_uid != user) {
            إذا (chown((ثابت حرف *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                أرجع NGX_ERROR;
            }
        }

        إذا ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            إذا (chmod((ثابت حرف *) path[i]->name.data, fi.st_mode) == -1) {
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

#إذا !(NGX_WIN32)

    إذا (ext->access) {
        إذا (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            اذهب failed;
        }
    }

#endif

    إذا (ext->time != -1) {
        إذا (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            اذهب failed;
        }
    }

    إذا (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        أرجع NGX_OK;
    }

    err = ngx_errno;

    إذا (err == NGX_ENOPATH) {

        إذا (!ext->create_path) {
            اذهب failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        إذا (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            اذهب failed;
        }

        إذا (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            أرجع NGX_OK;
        }

        err = ngx_errno;
    }

#إذا (NGX_WIN32)

    إذا (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        إذا (err == 0) {
            أرجع NGX_OK;
        }
    }

#endif

    إذا (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_لمب(0));

        إذا (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            إذا (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                إذا (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
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

            إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    إذا (ext->delete_file) {
        إذا (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    إذا (err) {
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

    إذا (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        اذهب failed;
    }

    إذا (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } وإلا {
        إذا (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            اذهب failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    إذا ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    إذا (buf == عدم) {
        اذهب failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    إذا (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        اذهب failed;
    }

    طالما (size > 0) {

        إذا ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        إذا (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            اذهب failed;
        }

        إذا ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            اذهب failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        إذا (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            اذهب failed;
        }

        إذا ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            اذهب failed;
        }

        size -= n;
    }

    إذا (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        اذهب failed;
    }

    rc = NGX_OK;

failed:

    إذا (nfd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    إذا (fd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    إذا (buf) {
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
ngx_walk_بشيا(ngx_بشيا_ctx_t *ctx, ngx_str_t *بشيا)
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
                   "walk tree \"%V\"", بشيا);

    إذا (ngx_open_dir(بشيا, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", بشيا->data);
        أرجع NGX_ERROR;
    }

    prev = ctx->data;

    إذا (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        إذا (data == عدم) {
            اذهب failed;
        }

        إذا (ctx->init_handler(data, prev) == NGX_ABORT) {
            اذهب failed;
        }

        ctx->data = data;

    } وإلا {
        data = عدم;
    }

    لكل ( ;; ) {

        ngx_set_errno(0);

        إذا (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            إذا (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } وإلا {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, err,
                              ngx_read_dir_n " \"%s\" failed", بشيا->data);
                rc = NGX_ERROR;
            }

            اذهب done;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                      "tree name %uz:\"%s\"", len, name);

        إذا (len == 1 && name[0] == '.') {
            استمر;
        }

        إذا (len == 2 && name[0] == '.' && name[1] == '.') {
            استمر;
        }

        file.len = بشيا->len + 1 + len;

        إذا (file.len > buf.len) {

            إذا (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = بشيا->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            إذا (buf.data == عدم) {
                اذهب failed;
            }
        }

        p = ngx_cpymem(buf.data, بشيا->data, بشيا->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        إذا (!dir.valid_info) {
            إذا (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                استمر;
            }
        }

        إذا (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            إذا (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

        } وإلا إذا (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_بشيا_handler(ctx, &file);

            إذا (rc == NGX_ABORT) {
                اذهب failed;
            }

            إذا (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                استمر;
            }

            إذا (ngx_walk_بشيا(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            إذا (ctx->post_بشيا_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

        } وإلا {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            إذا (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    إذا (buf.len) {
        ngx_free(buf.data);
    }

    إذا (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    إذا (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", بشيا->data);
    }

    أرجع rc;
}
