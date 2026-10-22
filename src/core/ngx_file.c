
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ایستا ngx_int_t ngx_test_full_name(ngx_str_t *name);


ایستا ngx_atomic_t   temp_عدد = 0;
ngx_atomic_t         *ngx_temp_عدد = &temp_عدد;
ngx_atomic_int_t      ngx_random_عدد = 123456;


ngx_int_t
ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix, ngx_str_t *name)
{
    size_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_full_name(name);

    اگر (rc == NGX_OK) {
        بازگشت rc;
    }

    len = prefix->len;

#اگر (NGX_WIN32)

    اگر (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    اگر (n == پوچ) {
        بازگشت NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->data, len);
    ngx_cpystrn(p, name->data, name->len + 1);

    name->len += len;
    name->data = n;

    بازگشت NGX_OK;
}


ایستا ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#اگر (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    اگر (name->len < 2) {
        اگر (c0 == '/') {
            بازگشت 2;
        }

        بازگشت NGX_DECLINED;
    }

    c1 = name->data[1];

    اگر (c1 == ':') {
        c0 |= 0x20;

        اگر ((c0 >= 'a' && c0 <= 'z')) {
            بازگشت NGX_OK;
        }

        بازگشت NGX_DECLINED;
    }

    اگر (c1 == '/') {
        بازگشت NGX_OK;
    }

    اگر (c0 == '/') {
        بازگشت 2;
    }

    بازگشت NGX_DECLINED;

#وگرنه

    اگر (name->data[0] == '/') {
        بازگشت NGX_OK;
    }

    بازگشت NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    اگر (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        اگر (rc != NGX_OK) {
            بازگشت rc;
        }

        اگر (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#اگر (NGX_THREADS && NGX_HAVE_PWRITEV)

    اگر (tf->thread_write) {
        بازگشت ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    بازگشت ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
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

    اگر (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } وگرنه {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.data = ngx_pnalloc(pool, file->name.len + 1);
    اگر (file->name.data == پوچ) {
        بازگشت NGX_ERROR;
    }

#اگر 0
    برای (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    اگر (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_عدد(0);

    cln = ngx_pool_cleanup_add(pool, اندازه(ngx_pool_cleanup_file_t));
    اگر (cln == پوچ) {
        بازگشت NGX_ERROR;
    }

    برای ( ;; ) {
        (تهی) ngx_sprintf(p, "%010uD%Z", n);

        اگر (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        اگر (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            بازگشت NGX_OK;
        }

        err = ngx_errno;

        اگر (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_عدد(1);
            ادامه;
        }

        اگر ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            بازگشت NGX_ERROR;
        }

        اگر (ngx_create_path(file, path) == NGX_ERROR) {
            بازگشت NGX_ERROR;
        }
    }
}


تهی
ngx_create_hashed_filename(ngx_path_t *path, u_char *file, size_t len)
{
    size_t      i, level;
    ngx_uint_t  n;

    i = path->name.len + 1;

    file[path->name.len + path->len]  = '/';

    برای (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        اگر (level == 0) {
            بشکن;
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

    برای (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        اگر (path->level[i] == 0) {
            بشکن;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        اگر (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            اگر (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.data);
                بازگشت NGX_ERROR;
            }
        }

        file->name.data[pos] = '/';
    }

    بازگشت NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#اگر (NGX_WIN32)
    p = dir + 3;
#وگرنه
    p = dir + 1;
#endif

    برای ( /* void */ ; *p; p++) {
        ch = *p;

        اگر (ch != '/') {
            ادامه;
        }

        *p = '\0';

        اگر (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            انتخاب (err) {
            حالت NGX_EEXIST:
                err = 0;
            حالت NGX_EACCES:
                بشکن;

            پیشفرض:
                بازگشت err;
            }
        }

        *p = '/';
    }

    بازگشت err;
}


ngx_atomic_uint_t
ngx_next_temp_عدد(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_عدد : 1;

    n = ngx_atomic_fetch_add(ngx_temp_عدد, add);

    بازگشت n + add;
}


نویسه *
ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه  *p = conf;

    ssize_t      level;
    ngx_str_t   *مقدار;
    ngx_uint_t   i, n;
    ngx_path_t  *path, **slot;

    slot = (ngx_path_t **) (p + cmd->offset);

    اگر (*slot) {
        بازگشت "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, اندازه(ngx_path_t));
    اگر (path == پوچ) {
        بازگشت NGX_CONF_ERROR;
    }

    مقدار = cf->args->elts;

    path->name = مقدار[1];

    اگر (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    اگر (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        بازگشت NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->سطر = cf->conf_file->سطر;

    برای (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(مقدار[n].data, مقدار[n].len);
        اگر (level == NGX_ERROR || level == 0) {
            بازگشت "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    اگر (path->len > 10 + i) {
        بازگشت "invalid value";
    }

    *slot = path;

    اگر (ngx_add_path(cf, slot) == NGX_ERROR) {
        بازگشت NGX_CONF_ERROR;
    }

    بازگشت NGX_CONF_OK;
}


نویسه *
ngx_conf_merge_path_مقدار(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    اگر (*path) {
        بازگشت NGX_CONF_OK;
    }

    اگر (prev) {
        *path = prev;
        بازگشت NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, اندازه(ngx_path_t));
    اگر (*path == پوچ) {
        بازگشت NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    اگر (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        بازگشت NGX_CONF_ERROR;
    }

    برای (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    اگر (ngx_add_path(cf, path) != NGX_OK) {
        بازگشت NGX_CONF_ERROR;
    }

    بازگشت NGX_CONF_OK;
}


نویسه *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    نویسه  *confp = conf;

    u_char      *p;
    ngx_str_t   *مقدار;
    ngx_uint_t   i, راست, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    اگر (*access != NGX_CONF_UNSET_UINT) {
        بازگشت "is duplicate";
    }

    مقدار = cf->args->elts;

    *access = 0;
    user = 0600;

    برای (i = 1; i < cf->args->nelts; i++) {

        p = مقدار[i].data;

        اگر (ngx_strncmp(p, "user:", اندازه("user:") - 1) == 0) {
            shift = 6;
            p += اندازه("user:") - 1;
            user = 0;

        } وگرنه اگر (ngx_strncmp(p, "group:", اندازه("group:") - 1) == 0) {
            shift = 3;
            p += اندازه("group:") - 1;

        } وگرنه اگر (ngx_strncmp(p, "all:", اندازه("all:") - 1) == 0) {
            shift = 0;
            p += اندازه("all:") - 1;

        } وگرنه {
            برو invalid;
        }

        اگر (ngx_strcmp(p, "rw") == 0) {
            راست = 6;

        } وگرنه اگر (ngx_strcmp(p, "r") == 0) {
            راست = 4;

        } وگرنه {
            برو invalid;
        }

        *access |= راست << shift;
    }

    *access |= user;

    بازگشت NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &مقدار[i]);

    بازگشت NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    برای (i = 0; i < cf->cycle->paths.nelts; i++) {
        اگر (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            اگر (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->سطر);
                بازگشت NGX_ERROR;
            }

            برای (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                اگر (p[i]->level[n] != path->level[n]) {
                    اگر (path->conf_file == پوچ) {
                        اگر (p[i]->conf_file == پوچ) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            بازگشت NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->سطر);
                        بازگشت NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->سطر);
                    بازگشت NGX_ERROR;
                }

                اگر (p[i]->level[n] == 0) {
                    بشکن;
                }
            }

            *slot = p[i];

            بازگشت NGX_OK;
        }
    }

    p = ngx_array_افزودن(&cf->cycle->paths);
    اگر (p == پوچ) {
        بازگشت NGX_ERROR;
    }

    *p = path;

    بازگشت NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    برای (i = 0; i < cycle->paths.nelts; i++) {

        اگر (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            اگر (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                بازگشت NGX_ERROR;
            }
        }

        اگر (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ادامه;
        }

#اگر !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        اگر (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            بازگشت NGX_ERROR;
        }

        اگر (fi.st_uid != user) {
            اگر (chown((ثابت نویسه *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                بازگشت NGX_ERROR;
            }
        }

        اگر ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            اگر (chmod((ثابت نویسه *) path[i]->name.data, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.data);
                بازگشت NGX_ERROR;
            }
        }
        }
#endif
    }

    بازگشت NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#اگر !(NGX_WIN32)

    اگر (ext->access) {
        اگر (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            برو failed;
        }
    }

#endif

    اگر (ext->time != -1) {
        اگر (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            برو failed;
        }
    }

    اگر (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        بازگشت NGX_OK;
    }

    err = ngx_errno;

    اگر (err == NGX_ENOPATH) {

        اگر (!ext->create_path) {
            برو failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        اگر (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            برو failed;
        }

        اگر (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            بازگشت NGX_OK;
        }

        err = ngx_errno;
    }

#اگر (NGX_WIN32)

    اگر (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        اگر (err == 0) {
            بازگشت NGX_OK;
        }
    }

#endif

    اگر (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        اگر (name == پوچ) {
            بازگشت NGX_ERROR;
        }

        (تهی) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_عدد(0));

        اگر (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            اگر (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                اگر (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->data);
                    بازگشت NGX_ERROR;
                }

                بازگشت NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->data);

            اگر (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    اگر (ext->delete_file) {
        اگر (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    اگر (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->data, to->data);
    }

    بازگشت NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf)
{
    نویسه             *buf;
    off_t             size;
    time_t            time;
    size_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = پوچ;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    اگر (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        برو failed;
    }

    اگر (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } وگرنه {
        اگر (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            برو failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    اگر ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    اگر (buf == پوچ) {
        برو failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    اگر (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        برو failed;
    }

    وقتی (size > 0) {

        اگر ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        اگر (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            برو failed;
        }

        اگر ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            برو failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        اگر (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            برو failed;
        }

        اگر ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            برو failed;
        }

        size -= n;
    }

    اگر (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        برو failed;
    }

    rc = NGX_OK;

failed:

    اگر (nfd != NGX_INVALID_FILE) {
        اگر (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    اگر (fd != NGX_INVALID_FILE) {
        اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    اگر (buf) {
        ngx_free(buf);
    }

    بازگشت rc;
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
ngx_walk_درخت(ngx_درخت_ctx_t *ctx, ngx_str_t *درخت)
{
    تهی       *data, *prev;
    u_char     *p, *name;
    size_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                   "walk tree \"%V\"", درخت);

    اگر (ngx_open_dir(درخت, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", درخت->data);
        بازگشت NGX_ERROR;
    }

    prev = ctx->data;

    اگر (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        اگر (data == پوچ) {
            برو failed;
        }

        اگر (ctx->init_handler(data, prev) == NGX_ABORT) {
            برو failed;
        }

        ctx->data = data;

    } وگرنه {
        data = پوچ;
    }

    برای ( ;; ) {

        ngx_set_errno(0);

        اگر (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            اگر (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } وگرنه {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, err,
                              ngx_read_dir_n " \"%s\" failed", درخت->data);
                rc = NGX_ERROR;
            }

            برو done;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                      "tree name %uz:\"%s\"", len, name);

        اگر (len == 1 && name[0] == '.') {
            ادامه;
        }

        اگر (len == 2 && name[0] == '.' && name[1] == '.') {
            ادامه;
        }

        file.len = درخت->len + 1 + len;

        اگر (file.len > buf.len) {

            اگر (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = درخت->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            اگر (buf.data == پوچ) {
                برو failed;
            }
        }

        p = ngx_cpymem(buf.data, درخت->data, درخت->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        اگر (!dir.valid_info) {
            اگر (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                ادامه;
            }
        }

        اگر (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            اگر (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                برو failed;
            }

        } وگرنه اگر (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_درخت_handler(ctx, &file);

            اگر (rc == NGX_ABORT) {
                برو failed;
            }

            اگر (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                ادامه;
            }

            اگر (ngx_walk_درخت(ctx, &file) == NGX_ABORT) {
                برو failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            اگر (ctx->post_درخت_handler(ctx, &file) == NGX_ABORT) {
                برو failed;
            }

        } وگرنه {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            اگر (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                برو failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    اگر (buf.len) {
        ngx_free(buf.data);
    }

    اگر (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    اگر (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", درخت->data);
    }

    بازگشت rc;
}
