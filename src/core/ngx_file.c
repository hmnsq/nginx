
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


സ്ഥിതം ngx_int_t ngx_test_full_name(ngx_str_t *name);


സ്ഥിതം ngx_atomic_t   temp_സംഖ്യ = 0;
ngx_atomic_t         *ngx_temp_സംഖ്യ = &temp_സംഖ്യ;
ngx_atomic_int_t      ngx_random_സംഖ്യ = 123456;


ngx_int_t
ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix, ngx_str_t *name)
{
    size_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_full_name(name);

    എങ്കിൽ (rc == NGX_OK) {
        തിരികെ rc;
    }

    len = prefix->len;

#എങ്കിൽ (NGX_WIN32)

    എങ്കിൽ (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    എങ്കിൽ (n == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->data, len);
    ngx_cpystrn(p, name->data, name->len + 1);

    name->len += len;
    name->data = n;

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#എങ്കിൽ (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    എങ്കിൽ (name->len < 2) {
        എങ്കിൽ (c0 == '/') {
            തിരികെ 2;
        }

        തിരികെ NGX_DECLINED;
    }

    c1 = name->data[1];

    എങ്കിൽ (c1 == ':') {
        c0 |= 0x20;

        എങ്കിൽ ((c0 >= 'a' && c0 <= 'z')) {
            തിരികെ NGX_OK;
        }

        തിരികെ NGX_DECLINED;
    }

    എങ്കിൽ (c1 == '/') {
        തിരികെ NGX_OK;
    }

    എങ്കിൽ (c0 == '/') {
        തിരികെ 2;
    }

    തിരികെ NGX_DECLINED;

#അല്ലെങ്കിൽ

    എങ്കിൽ (name->data[0] == '/') {
        തിരികെ NGX_OK;
    }

    തിരികെ NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    എങ്കിൽ (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        എങ്കിൽ (rc != NGX_OK) {
            തിരികെ rc;
        }

        എങ്കിൽ (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#എങ്കിൽ (NGX_THREADS && NGX_HAVE_PWRITEV)

    എങ്കിൽ (tf->thread_write) {
        തിരികെ ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    തിരികെ ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
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

    എങ്കിൽ (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } അല്ലെങ്കിൽ {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.data = ngx_pnalloc(pool, file->name.len + 1);
    എങ്കിൽ (file->name.data == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

#എങ്കിൽ 0
    വേണ്ടി (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    എങ്കിൽ (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_സംഖ്യ(0);

    cln = ngx_pool_cleanup_add(pool, വലുപ്പം(ngx_pool_cleanup_file_t));
    എങ്കിൽ (cln == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    വേണ്ടി ( ;; ) {
        (ശൂന്യം) ngx_sprintf(p, "%010uD%Z", n);

        എങ്കിൽ (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        എങ്കിൽ (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            തിരികെ NGX_OK;
        }

        err = ngx_errno;

        എങ്കിൽ (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_സംഖ്യ(1);
            തുടരുക;
        }

        എങ്കിൽ ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (ngx_create_path(file, path) == NGX_ERROR) {
            തിരികെ NGX_ERROR;
        }
    }
}


ശൂന്യം
ngx_create_hashed_filename(ngx_path_t *path, u_char *file, size_t len)
{
    size_t      i, level;
    ngx_uint_t  n;

    i = path->name.len + 1;

    file[path->name.len + path->len]  = '/';

    വേണ്ടി (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        എങ്കിൽ (level == 0) {
            നിർത്തുക;
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

    വേണ്ടി (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        എങ്കിൽ (path->level[i] == 0) {
            നിർത്തുക;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        എങ്കിൽ (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            എങ്കിൽ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.data);
                തിരികെ NGX_ERROR;
            }
        }

        file->name.data[pos] = '/';
    }

    തിരികെ NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#എങ്കിൽ (NGX_WIN32)
    p = dir + 3;
#അല്ലെങ്കിൽ
    p = dir + 1;
#endif

    വേണ്ടി ( /* void */ ; *p; p++) {
        ch = *p;

        എങ്കിൽ (ch != '/') {
            തുടരുക;
        }

        *p = '\0';

        എങ്കിൽ (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            തിരഞ്ഞെടുപ്പ് (err) {
            അവസ്ഥ NGX_EEXIST:
                err = 0;
            അവസ്ഥ NGX_EACCES:
                നിർത്തുക;

            സ്ഥിരസ്ഥിതി:
                തിരികെ err;
            }
        }

        *p = '/';
    }

    തിരികെ err;
}


ngx_atomic_uint_t
ngx_next_temp_സംഖ്യ(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_സംഖ്യ : 1;

    n = ngx_atomic_fetch_add(ngx_temp_സംഖ്യ, add);

    തിരികെ n + add;
}


അക്ഷരം *
ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം  *p = conf;

    ssize_t      level;
    ngx_str_t   *മൂല്യം;
    ngx_uint_t   i, n;
    ngx_path_t  *path, **slot;

    slot = (ngx_path_t **) (p + cmd->offset);

    എങ്കിൽ (*slot) {
        തിരികെ "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, വലുപ്പം(ngx_path_t));
    എങ്കിൽ (path == ശൂന്യ) {
        തിരികെ NGX_CONF_ERROR;
    }

    മൂല്യം = cf->args->elts;

    path->name = മൂല്യം[1];

    എങ്കിൽ (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    എങ്കിൽ (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        തിരികെ NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->വരി = cf->conf_file->വരി;

    വേണ്ടി (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(മൂല്യം[n].data, മൂല്യം[n].len);
        എങ്കിൽ (level == NGX_ERROR || level == 0) {
            തിരികെ "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    എങ്കിൽ (path->len > 10 + i) {
        തിരികെ "invalid value";
    }

    *slot = path;

    എങ്കിൽ (ngx_add_path(cf, slot) == NGX_ERROR) {
        തിരികെ NGX_CONF_ERROR;
    }

    തിരികെ NGX_CONF_OK;
}


അക്ഷരം *
ngx_conf_merge_path_മൂല്യം(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    എങ്കിൽ (*path) {
        തിരികെ NGX_CONF_OK;
    }

    എങ്കിൽ (prev) {
        *path = prev;
        തിരികെ NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, വലുപ്പം(ngx_path_t));
    എങ്കിൽ (*path == ശൂന്യ) {
        തിരികെ NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    എങ്കിൽ (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        തിരികെ NGX_CONF_ERROR;
    }

    വേണ്ടി (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    എങ്കിൽ (ngx_add_path(cf, path) != NGX_OK) {
        തിരികെ NGX_CONF_ERROR;
    }

    തിരികെ NGX_CONF_OK;
}


അക്ഷരം *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    അക്ഷരം  *confp = conf;

    u_char      *p;
    ngx_str_t   *മൂല്യം;
    ngx_uint_t   i, വലത്, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    എങ്കിൽ (*access != NGX_CONF_UNSET_UINT) {
        തിരികെ "is duplicate";
    }

    മൂല്യം = cf->args->elts;

    *access = 0;
    user = 0600;

    വേണ്ടി (i = 1; i < cf->args->nelts; i++) {

        p = മൂല്യം[i].data;

        എങ്കിൽ (ngx_strncmp(p, "user:", വലുപ്പം("user:") - 1) == 0) {
            shift = 6;
            p += വലുപ്പം("user:") - 1;
            user = 0;

        } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strncmp(p, "group:", വലുപ്പം("group:") - 1) == 0) {
            shift = 3;
            p += വലുപ്പം("group:") - 1;

        } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strncmp(p, "all:", വലുപ്പം("all:") - 1) == 0) {
            shift = 0;
            p += വലുപ്പം("all:") - 1;

        } അല്ലെങ്കിൽ {
            പോകുക invalid;
        }

        എങ്കിൽ (ngx_strcmp(p, "rw") == 0) {
            വലത് = 6;

        } അല്ലെങ്കിൽ എങ്കിൽ (ngx_strcmp(p, "r") == 0) {
            വലത് = 4;

        } അല്ലെങ്കിൽ {
            പോകുക invalid;
        }

        *access |= വലത് << shift;
    }

    *access |= user;

    തിരികെ NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &മൂല്യം[i]);

    തിരികെ NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    വേണ്ടി (i = 0; i < cf->cycle->paths.nelts; i++) {
        എങ്കിൽ (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            എങ്കിൽ (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->വരി);
                തിരികെ NGX_ERROR;
            }

            വേണ്ടി (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                എങ്കിൽ (p[i]->level[n] != path->level[n]) {
                    എങ്കിൽ (path->conf_file == ശൂന്യ) {
                        എങ്കിൽ (p[i]->conf_file == ശൂന്യ) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            തിരികെ NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->വരി);
                        തിരികെ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->വരി);
                    തിരികെ NGX_ERROR;
                }

                എങ്കിൽ (p[i]->level[n] == 0) {
                    നിർത്തുക;
                }
            }

            *slot = p[i];

            തിരികെ NGX_OK;
        }
    }

    p = ngx_array_തള്ളുക(&cf->cycle->paths);
    എങ്കിൽ (p == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    *p = path;

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    വേണ്ടി (i = 0; i < cycle->paths.nelts; i++) {

        എങ്കിൽ (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            എങ്കിൽ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                തിരികെ NGX_ERROR;
            }
        }

        എങ്കിൽ (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            തുടരുക;
        }

#എങ്കിൽ !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        എങ്കിൽ (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (fi.st_uid != user) {
            എങ്കിൽ (chown((സ്ഥിരം അക്ഷരം *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                തിരികെ NGX_ERROR;
            }
        }

        എങ്കിൽ ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            എങ്കിൽ (chmod((സ്ഥിരം അക്ഷരം *) path[i]->name.data, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.data);
                തിരികെ NGX_ERROR;
            }
        }
        }
#endif
    }

    തിരികെ NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#എങ്കിൽ !(NGX_WIN32)

    എങ്കിൽ (ext->access) {
        എങ്കിൽ (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            പോകുക failed;
        }
    }

#endif

    എങ്കിൽ (ext->time != -1) {
        എങ്കിൽ (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            പോകുക failed;
        }
    }

    എങ്കിൽ (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        തിരികെ NGX_OK;
    }

    err = ngx_errno;

    എങ്കിൽ (err == NGX_ENOPATH) {

        എങ്കിൽ (!ext->create_path) {
            പോകുക failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        എങ്കിൽ (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            പോകുക failed;
        }

        എങ്കിൽ (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            തിരികെ NGX_OK;
        }

        err = ngx_errno;
    }

#എങ്കിൽ (NGX_WIN32)

    എങ്കിൽ (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        എങ്കിൽ (err == 0) {
            തിരികെ NGX_OK;
        }
    }

#endif

    എങ്കിൽ (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        എങ്കിൽ (name == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        (ശൂന്യം) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_സംഖ്യ(0));

        എങ്കിൽ (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            എങ്കിൽ (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                എങ്കിൽ (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->data);
                    തിരികെ NGX_ERROR;
                }

                തിരികെ NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->data);

            എങ്കിൽ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    എങ്കിൽ (ext->delete_file) {
        എങ്കിൽ (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    എങ്കിൽ (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->data, to->data);
    }

    തിരികെ NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf)
{
    അക്ഷരം             *buf;
    off_t             size;
    time_t            time;
    size_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = ശൂന്യ;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    എങ്കിൽ (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        പോകുക failed;
    }

    എങ്കിൽ (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } അല്ലെങ്കിൽ {
        എങ്കിൽ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            പോകുക failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    എങ്കിൽ ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    എങ്കിൽ (buf == ശൂന്യ) {
        പോകുക failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    എങ്കിൽ (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        പോകുക failed;
    }

    ആകുമ്പോൾ (size > 0) {

        എങ്കിൽ ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        എങ്കിൽ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            പോകുക failed;
        }

        എങ്കിൽ ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            പോകുക failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        എങ്കിൽ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            പോകുക failed;
        }

        എങ്കിൽ ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            പോകുക failed;
        }

        size -= n;
    }

    എങ്കിൽ (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        പോകുക failed;
    }

    rc = NGX_OK;

failed:

    എങ്കിൽ (nfd != NGX_INVALID_FILE) {
        എങ്കിൽ (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    എങ്കിൽ (fd != NGX_INVALID_FILE) {
        എങ്കിൽ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    എങ്കിൽ (buf) {
        ngx_free(buf);
    }

    തിരികെ rc;
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
ngx_walk_വൃക്ഷം(ngx_വൃക്ഷം_ctx_t *ctx, ngx_str_t *വൃക്ഷം)
{
    ശൂന്യം       *data, *prev;
    u_char     *p, *name;
    size_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                   "walk tree \"%V\"", വൃക്ഷം);

    എങ്കിൽ (ngx_open_dir(വൃക്ഷം, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", വൃക്ഷം->data);
        തിരികെ NGX_ERROR;
    }

    prev = ctx->data;

    എങ്കിൽ (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        എങ്കിൽ (data == ശൂന്യ) {
            പോകുക failed;
        }

        എങ്കിൽ (ctx->init_handler(data, prev) == NGX_ABORT) {
            പോകുക failed;
        }

        ctx->data = data;

    } അല്ലെങ്കിൽ {
        data = ശൂന്യ;
    }

    വേണ്ടി ( ;; ) {

        ngx_set_errno(0);

        എങ്കിൽ (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            എങ്കിൽ (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } അല്ലെങ്കിൽ {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, err,
                              ngx_read_dir_n " \"%s\" failed", വൃക്ഷം->data);
                rc = NGX_ERROR;
            }

            പോകുക done;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                      "tree name %uz:\"%s\"", len, name);

        എങ്കിൽ (len == 1 && name[0] == '.') {
            തുടരുക;
        }

        എങ്കിൽ (len == 2 && name[0] == '.' && name[1] == '.') {
            തുടരുക;
        }

        file.len = വൃക്ഷം->len + 1 + len;

        എങ്കിൽ (file.len > buf.len) {

            എങ്കിൽ (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = വൃക്ഷം->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            എങ്കിൽ (buf.data == ശൂന്യ) {
                പോകുക failed;
            }
        }

        p = ngx_cpymem(buf.data, വൃക്ഷം->data, വൃക്ഷം->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        എങ്കിൽ (!dir.valid_info) {
            എങ്കിൽ (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                തുടരുക;
            }
        }

        എങ്കിൽ (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            എങ്കിൽ (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                പോകുക failed;
            }

        } അല്ലെങ്കിൽ എങ്കിൽ (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_വൃക്ഷം_handler(ctx, &file);

            എങ്കിൽ (rc == NGX_ABORT) {
                പോകുക failed;
            }

            എങ്കിൽ (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                തുടരുക;
            }

            എങ്കിൽ (ngx_walk_വൃക്ഷം(ctx, &file) == NGX_ABORT) {
                പോകുക failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            എങ്കിൽ (ctx->post_വൃക്ഷം_handler(ctx, &file) == NGX_ABORT) {
                പോകുക failed;
            }

        } അല്ലെങ്കിൽ {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            എങ്കിൽ (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                പോകുക failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    എങ്കിൽ (buf.len) {
        ngx_free(buf.data);
    }

    എങ്കിൽ (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    എങ്കിൽ (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", വൃക്ഷം->data);
    }

    തിരികെ rc;
}
