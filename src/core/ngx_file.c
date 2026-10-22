
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


நிலையான ngx_int_t ngx_test_full_name(ngx_str_t *name);


நிலையான ngx_atomic_t   temp_எண் = 0;
ngx_atomic_t         *ngx_temp_எண் = &temp_எண்;
ngx_atomic_int_t      ngx_random_எண் = 123456;


ngx_int_t
ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix, ngx_str_t *name)
{
    size_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_full_name(name);

    எனில் (rc == NGX_OK) {
        திரும்பு rc;
    }

    len = prefix->len;

#எனில் (NGX_WIN32)

    எனில் (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    எனில் (n == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->data, len);
    ngx_cpystrn(p, name->data, name->len + 1);

    name->len += len;
    name->data = n;

    திரும்பு NGX_OK;
}


நிலையான ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#எனில் (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    எனில் (name->len < 2) {
        எனில் (c0 == '/') {
            திரும்பு 2;
        }

        திரும்பு NGX_DECLINED;
    }

    c1 = name->data[1];

    எனில் (c1 == ':') {
        c0 |= 0x20;

        எனில் ((c0 >= 'a' && c0 <= 'z')) {
            திரும்பு NGX_OK;
        }

        திரும்பு NGX_DECLINED;
    }

    எனில் (c1 == '/') {
        திரும்பு NGX_OK;
    }

    எனில் (c0 == '/') {
        திரும்பு 2;
    }

    திரும்பு NGX_DECLINED;

#இல்லையெனில்

    எனில் (name->data[0] == '/') {
        திரும்பு NGX_OK;
    }

    திரும்பு NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    எனில் (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        எனில் (rc != NGX_OK) {
            திரும்பு rc;
        }

        எனில் (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#எனில் (NGX_THREADS && NGX_HAVE_PWRITEV)

    எனில் (tf->thread_write) {
        திரும்பு ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    திரும்பு ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
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

    எனில் (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } இல்லையெனில் {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.data = ngx_pnalloc(pool, file->name.len + 1);
    எனில் (file->name.data == வெற்று) {
        திரும்பு NGX_ERROR;
    }

#எனில் 0
    க்கு (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    எனில் (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_எண்(0);

    cln = ngx_pool_cleanup_add(pool, அளவு(ngx_pool_cleanup_file_t));
    எனில் (cln == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    க்கு ( ;; ) {
        (வெறுமை) ngx_sprintf(p, "%010uD%Z", n);

        எனில் (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        எனில் (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            திரும்பு NGX_OK;
        }

        err = ngx_errno;

        எனில் (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_எண்(1);
            தொடர்;
        }

        எனில் ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            திரும்பு NGX_ERROR;
        }

        எனில் (ngx_create_path(file, path) == NGX_ERROR) {
            திரும்பு NGX_ERROR;
        }
    }
}


வெறுமை
ngx_create_hashed_filename(ngx_path_t *path, u_char *file, size_t len)
{
    size_t      i, level;
    ngx_uint_t  n;

    i = path->name.len + 1;

    file[path->name.len + path->len]  = '/';

    க்கு (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        எனில் (level == 0) {
            நிறுத்து;
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

    க்கு (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        எனில் (path->level[i] == 0) {
            நிறுத்து;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        எனில் (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            எனில் (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.data);
                திரும்பு NGX_ERROR;
            }
        }

        file->name.data[pos] = '/';
    }

    திரும்பு NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#எனில் (NGX_WIN32)
    p = dir + 3;
#இல்லையெனில்
    p = dir + 1;
#endif

    க்கு ( /* void */ ; *p; p++) {
        ch = *p;

        எனில் (ch != '/') {
            தொடர்;
        }

        *p = '\0';

        எனில் (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            தேர்வு (err) {
            நிலை NGX_EEXIST:
                err = 0;
            நிலை NGX_EACCES:
                நிறுத்து;

            இயல்பு:
                திரும்பு err;
            }
        }

        *p = '/';
    }

    திரும்பு err;
}


ngx_atomic_uint_t
ngx_next_temp_எண்(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_எண் : 1;

    n = ngx_atomic_fetch_add(ngx_temp_எண், add);

    திரும்பு n + add;
}


எழுத்து *
ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து  *p = conf;

    ssize_t      level;
    ngx_str_t   *மதிப்பு;
    ngx_uint_t   i, n;
    ngx_path_t  *path, **slot;

    slot = (ngx_path_t **) (p + cmd->offset);

    எனில் (*slot) {
        திரும்பு "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, அளவு(ngx_path_t));
    எனில் (path == வெற்று) {
        திரும்பு NGX_CONF_ERROR;
    }

    மதிப்பு = cf->args->elts;

    path->name = மதிப்பு[1];

    எனில் (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    எனில் (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        திரும்பு NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->வரி = cf->conf_file->வரி;

    க்கு (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(மதிப்பு[n].data, மதிப்பு[n].len);
        எனில் (level == NGX_ERROR || level == 0) {
            திரும்பு "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    எனில் (path->len > 10 + i) {
        திரும்பு "invalid value";
    }

    *slot = path;

    எனில் (ngx_add_path(cf, slot) == NGX_ERROR) {
        திரும்பு NGX_CONF_ERROR;
    }

    திரும்பு NGX_CONF_OK;
}


எழுத்து *
ngx_conf_merge_path_மதிப்பு(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    எனில் (*path) {
        திரும்பு NGX_CONF_OK;
    }

    எனில் (prev) {
        *path = prev;
        திரும்பு NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, அளவு(ngx_path_t));
    எனில் (*path == வெற்று) {
        திரும்பு NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    எனில் (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        திரும்பு NGX_CONF_ERROR;
    }

    க்கு (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    எனில் (ngx_add_path(cf, path) != NGX_OK) {
        திரும்பு NGX_CONF_ERROR;
    }

    திரும்பு NGX_CONF_OK;
}


எழுத்து *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    எழுத்து  *confp = conf;

    u_char      *p;
    ngx_str_t   *மதிப்பு;
    ngx_uint_t   i, வலது, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    எனில் (*access != NGX_CONF_UNSET_UINT) {
        திரும்பு "is duplicate";
    }

    மதிப்பு = cf->args->elts;

    *access = 0;
    user = 0600;

    க்கு (i = 1; i < cf->args->nelts; i++) {

        p = மதிப்பு[i].data;

        எனில் (ngx_strncmp(p, "user:", அளவு("user:") - 1) == 0) {
            shift = 6;
            p += அளவு("user:") - 1;
            user = 0;

        } இல்லையெனில் எனில் (ngx_strncmp(p, "group:", அளவு("group:") - 1) == 0) {
            shift = 3;
            p += அளவு("group:") - 1;

        } இல்லையெனில் எனில் (ngx_strncmp(p, "all:", அளவு("all:") - 1) == 0) {
            shift = 0;
            p += அளவு("all:") - 1;

        } இல்லையெனில் {
            செல் invalid;
        }

        எனில் (ngx_strcmp(p, "rw") == 0) {
            வலது = 6;

        } இல்லையெனில் எனில் (ngx_strcmp(p, "r") == 0) {
            வலது = 4;

        } இல்லையெனில் {
            செல் invalid;
        }

        *access |= வலது << shift;
    }

    *access |= user;

    திரும்பு NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &மதிப்பு[i]);

    திரும்பு NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    க்கு (i = 0; i < cf->cycle->paths.nelts; i++) {
        எனில் (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            எனில் (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->வரி);
                திரும்பு NGX_ERROR;
            }

            க்கு (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                எனில் (p[i]->level[n] != path->level[n]) {
                    எனில் (path->conf_file == வெற்று) {
                        எனில் (p[i]->conf_file == வெற்று) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            திரும்பு NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->வரி);
                        திரும்பு NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->வரி);
                    திரும்பு NGX_ERROR;
                }

                எனில் (p[i]->level[n] == 0) {
                    நிறுத்து;
                }
            }

            *slot = p[i];

            திரும்பு NGX_OK;
        }
    }

    p = ngx_array_தள்ளு(&cf->cycle->paths);
    எனில் (p == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    *p = path;

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    க்கு (i = 0; i < cycle->paths.nelts; i++) {

        எனில் (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            எனில் (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                திரும்பு NGX_ERROR;
            }
        }

        எனில் (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            தொடர்;
        }

#எனில் !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        எனில் (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            திரும்பு NGX_ERROR;
        }

        எனில் (fi.st_uid != user) {
            எனில் (chown((மாறிலி எழுத்து *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                திரும்பு NGX_ERROR;
            }
        }

        எனில் ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            எனில் (chmod((மாறிலி எழுத்து *) path[i]->name.data, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.data);
                திரும்பு NGX_ERROR;
            }
        }
        }
#endif
    }

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#எனில் !(NGX_WIN32)

    எனில் (ext->access) {
        எனில் (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            செல் failed;
        }
    }

#endif

    எனில் (ext->time != -1) {
        எனில் (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            செல் failed;
        }
    }

    எனில் (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        திரும்பு NGX_OK;
    }

    err = ngx_errno;

    எனில் (err == NGX_ENOPATH) {

        எனில் (!ext->create_path) {
            செல் failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        எனில் (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            செல் failed;
        }

        எனில் (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            திரும்பு NGX_OK;
        }

        err = ngx_errno;
    }

#எனில் (NGX_WIN32)

    எனில் (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        எனில் (err == 0) {
            திரும்பு NGX_OK;
        }
    }

#endif

    எனில் (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        எனில் (name == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        (வெறுமை) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_எண்(0));

        எனில் (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            எனில் (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                எனில் (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->data);
                    திரும்பு NGX_ERROR;
                }

                திரும்பு NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->data);

            எனில் (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    எனில் (ext->delete_file) {
        எனில் (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    எனில் (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->data, to->data);
    }

    திரும்பு NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf)
{
    எழுத்து             *buf;
    off_t             size;
    time_t            time;
    size_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = வெற்று;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    எனில் (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        செல் failed;
    }

    எனில் (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } இல்லையெனில் {
        எனில் (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            செல் failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    எனில் ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    எனில் (buf == வெற்று) {
        செல் failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    எனில் (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        செல் failed;
    }

    போது (size > 0) {

        எனில் ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        எனில் (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            செல் failed;
        }

        எனில் ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            செல் failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        எனில் (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            செல் failed;
        }

        எனில் ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            செல் failed;
        }

        size -= n;
    }

    எனில் (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        செல் failed;
    }

    rc = NGX_OK;

failed:

    எனில் (nfd != NGX_INVALID_FILE) {
        எனில் (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    எனில் (fd != NGX_INVALID_FILE) {
        எனில் (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    எனில் (buf) {
        ngx_free(buf);
    }

    திரும்பு rc;
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
ngx_walk_மரம்(ngx_மரம்_ctx_t *ctx, ngx_str_t *மரம்)
{
    வெறுமை       *data, *prev;
    u_char     *p, *name;
    size_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                   "walk tree \"%V\"", மரம்);

    எனில் (ngx_open_dir(மரம், &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", மரம்->data);
        திரும்பு NGX_ERROR;
    }

    prev = ctx->data;

    எனில் (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        எனில் (data == வெற்று) {
            செல் failed;
        }

        எனில் (ctx->init_handler(data, prev) == NGX_ABORT) {
            செல் failed;
        }

        ctx->data = data;

    } இல்லையெனில் {
        data = வெற்று;
    }

    க்கு ( ;; ) {

        ngx_set_errno(0);

        எனில் (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            எனில் (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } இல்லையெனில் {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, err,
                              ngx_read_dir_n " \"%s\" failed", மரம்->data);
                rc = NGX_ERROR;
            }

            செல் done;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                      "tree name %uz:\"%s\"", len, name);

        எனில் (len == 1 && name[0] == '.') {
            தொடர்;
        }

        எனில் (len == 2 && name[0] == '.' && name[1] == '.') {
            தொடர்;
        }

        file.len = மரம்->len + 1 + len;

        எனில் (file.len > buf.len) {

            எனில் (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = மரம்->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            எனில் (buf.data == வெற்று) {
                செல் failed;
            }
        }

        p = ngx_cpymem(buf.data, மரம்->data, மரம்->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        எனில் (!dir.valid_info) {
            எனில் (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                தொடர்;
            }
        }

        எனில் (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            எனில் (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                செல் failed;
            }

        } இல்லையெனில் எனில் (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_மரம்_handler(ctx, &file);

            எனில் (rc == NGX_ABORT) {
                செல் failed;
            }

            எனில் (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                தொடர்;
            }

            எனில் (ngx_walk_மரம்(ctx, &file) == NGX_ABORT) {
                செல் failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            எனில் (ctx->post_மரம்_handler(ctx, &file) == NGX_ABORT) {
                செல் failed;
            }

        } இல்லையெனில் {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            எனில் (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                செல் failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    எனில் (buf.len) {
        ngx_free(buf.data);
    }

    எனில் (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    எனில் (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", மரம்->data);
    }

    திரும்பு rc;
}
