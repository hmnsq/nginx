
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर ngx_int_t ngx_test_full_name(ngx_str_t *name);


स्थिर ngx_atomic_t   temp_सङ्ख्या = 0;
ngx_atomic_t         *ngx_temp_सङ्ख्या = &temp_सङ्ख्या;
ngx_atomic_int_t      ngx_random_सङ्ख्या = 123456;


ngx_int_t
ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix, ngx_str_t *name)
{
    size_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_full_name(name);

    सचे (rc == NGX_OK) {
        पटिनिवत्त rc;
    }

    len = prefix->len;

#सचे (NGX_WIN32)

    सचे (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    सचे (n == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->data, len);
    ngx_cpystrn(p, name->data, name->len + 1);

    name->len += len;
    name->data = n;

    पटिनिवत्त NGX_OK;
}


स्थिर ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#सचे (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    सचे (name->len < 2) {
        सचे (c0 == '/') {
            पटिनिवत्त 2;
        }

        पटिनिवत्त NGX_DECLINED;
    }

    c1 = name->data[1];

    सचे (c1 == ':') {
        c0 |= 0x20;

        सचे ((c0 >= 'a' && c0 <= 'z')) {
            पटिनिवत्त NGX_OK;
        }

        पटिनिवत्त NGX_DECLINED;
    }

    सचे (c1 == '/') {
        पटिनिवत्त NGX_OK;
    }

    सचे (c0 == '/') {
        पटिनिवत्त 2;
    }

    पटिनिवत्त NGX_DECLINED;

#अञ्ञथा

    सचे (name->data[0] == '/') {
        पटिनिवत्त NGX_OK;
    }

    पटिनिवत्त NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    सचे (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        सचे (rc != NGX_OK) {
            पटिनिवत्त rc;
        }

        सचे (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#सचे (NGX_THREADS && NGX_HAVE_PWRITEV)

    सचे (tf->thread_write) {
        पटिनिवत्त ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    पटिनिवत्त ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
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

    सचे (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } अञ्ञथा {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.data = ngx_pnalloc(pool, file->name.len + 1);
    सचे (file->name.data == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

#सचे 0
    अत्थाय (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    सचे (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_सङ्ख्या(0);

    cln = ngx_pool_cleanup_add(pool, परिमाण(ngx_pool_cleanup_file_t));
    सचे (cln == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    अत्थाय ( ;; ) {
        (शून्य) ngx_sprintf(p, "%010uD%Z", n);

        सचे (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        सचे (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            पटिनिवत्त NGX_OK;
        }

        err = ngx_errno;

        सचे (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_सङ्ख्या(1);
            अनुवर्त;
        }

        सचे ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            पटिनिवत्त NGX_ERROR;
        }

        सचे (ngx_create_path(file, path) == NGX_ERROR) {
            पटिनिवत्त NGX_ERROR;
        }
    }
}


शून्य
ngx_create_hashed_filename(ngx_path_t *path, u_char *file, size_t len)
{
    size_t      i, level;
    ngx_uint_t  n;

    i = path->name.len + 1;

    file[path->name.len + path->len]  = '/';

    अत्थाय (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        सचे (level == 0) {
            भङ्ग;
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

    अत्थाय (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        सचे (path->level[i] == 0) {
            भङ्ग;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        सचे (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            सचे (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.data);
                पटिनिवत्त NGX_ERROR;
            }
        }

        file->name.data[pos] = '/';
    }

    पटिनिवत्त NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#सचे (NGX_WIN32)
    p = dir + 3;
#अञ्ञथा
    p = dir + 1;
#endif

    अत्थाय ( /* void */ ; *p; p++) {
        ch = *p;

        सचे (ch != '/') {
            अनुवर्त;
        }

        *p = '\0';

        सचे (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            विकल्प (err) {
            पक्ष NGX_EEXIST:
                err = 0;
            पक्ष NGX_EACCES:
                भङ्ग;

            स्वभाव:
                पटिनिवत्त err;
            }
        }

        *p = '/';
    }

    पटिनिवत्त err;
}


ngx_atomic_uint_t
ngx_next_temp_सङ्ख्या(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_सङ्ख्या : 1;

    n = ngx_atomic_fetch_add(ngx_temp_सङ्ख्या, add);

    पटिनिवत्त n + add;
}


वर्ण *
ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ssize_t      level;
    ngx_str_t   *मानम्;
    ngx_uint_t   i, n;
    ngx_path_t  *path, **slot;

    slot = (ngx_path_t **) (p + cmd->offset);

    सचे (*slot) {
        पटिनिवत्त "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, परिमाण(ngx_path_t));
    सचे (path == रिक्त) {
        पटिनिवत्त NGX_CONF_ERROR;
    }

    मानम् = cf->args->elts;

    path->name = मानम्[1];

    सचे (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    सचे (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        पटिनिवत्त NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->पङ्क्तिः = cf->conf_file->पङ्क्तिः;

    अत्थाय (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(मानम्[n].data, मानम्[n].len);
        सचे (level == NGX_ERROR || level == 0) {
            पटिनिवत्त "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    सचे (path->len > 10 + i) {
        पटिनिवत्त "invalid value";
    }

    *slot = path;

    सचे (ngx_add_path(cf, slot) == NGX_ERROR) {
        पटिनिवत्त NGX_CONF_ERROR;
    }

    पटिनिवत्त NGX_CONF_OK;
}


वर्ण *
ngx_conf_merge_path_मानम्(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    सचे (*path) {
        पटिनिवत्त NGX_CONF_OK;
    }

    सचे (prev) {
        *path = prev;
        पटिनिवत्त NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, परिमाण(ngx_path_t));
    सचे (*path == रिक्त) {
        पटिनिवत्त NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    सचे (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        पटिनिवत्त NGX_CONF_ERROR;
    }

    अत्थाय (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    सचे (ngx_add_path(cf, path) != NGX_OK) {
        पटिनिवत्त NGX_CONF_ERROR;
    }

    पटिनिवत्त NGX_CONF_OK;
}


वर्ण *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *confp = conf;

    u_char      *p;
    ngx_str_t   *मानम्;
    ngx_uint_t   i, दक्षिणः, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    सचे (*access != NGX_CONF_UNSET_UINT) {
        पटिनिवत्त "is duplicate";
    }

    मानम् = cf->args->elts;

    *access = 0;
    user = 0600;

    अत्थाय (i = 1; i < cf->args->nelts; i++) {

        p = मानम्[i].data;

        सचे (ngx_strncmp(p, "user:", परिमाण("user:") - 1) == 0) {
            shift = 6;
            p += परिमाण("user:") - 1;
            user = 0;

        } अञ्ञथा सचे (ngx_strncmp(p, "group:", परिमाण("group:") - 1) == 0) {
            shift = 3;
            p += परिमाण("group:") - 1;

        } अञ्ञथा सचे (ngx_strncmp(p, "all:", परिमाण("all:") - 1) == 0) {
            shift = 0;
            p += परिमाण("all:") - 1;

        } अञ्ञथा {
            गच्छ invalid;
        }

        सचे (ngx_strcmp(p, "rw") == 0) {
            दक्षिणः = 6;

        } अञ्ञथा सचे (ngx_strcmp(p, "r") == 0) {
            दक्षिणः = 4;

        } अञ्ञथा {
            गच्छ invalid;
        }

        *access |= दक्षिणः << shift;
    }

    *access |= user;

    पटिनिवत्त NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &मानम्[i]);

    पटिनिवत्त NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    अत्थाय (i = 0; i < cf->cycle->paths.nelts; i++) {
        सचे (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            सचे (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->पङ्क्तिः);
                पटिनिवत्त NGX_ERROR;
            }

            अत्थाय (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                सचे (p[i]->level[n] != path->level[n]) {
                    सचे (path->conf_file == रिक्त) {
                        सचे (p[i]->conf_file == रिक्त) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            पटिनिवत्त NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->पङ्क्तिः);
                        पटिनिवत्त NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->पङ्क्तिः);
                    पटिनिवत्त NGX_ERROR;
                }

                सचे (p[i]->level[n] == 0) {
                    भङ्ग;
                }
            }

            *slot = p[i];

            पटिनिवत्त NGX_OK;
        }
    }

    p = ngx_array_प्रक्षिप(&cf->cycle->paths);
    सचे (p == रिक्त) {
        पटिनिवत्त NGX_ERROR;
    }

    *p = path;

    पटिनिवत्त NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    अत्थाय (i = 0; i < cycle->paths.nelts; i++) {

        सचे (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            सचे (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                पटिनिवत्त NGX_ERROR;
            }
        }

        सचे (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            अनुवर्त;
        }

#सचे !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        सचे (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            पटिनिवत्त NGX_ERROR;
        }

        सचे (fi.st_uid != user) {
            सचे (chown((ध्रुव वर्ण *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                पटिनिवत्त NGX_ERROR;
            }
        }

        सचे ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            सचे (chmod((ध्रुव वर्ण *) path[i]->name.data, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.data);
                पटिनिवत्त NGX_ERROR;
            }
        }
        }
#endif
    }

    पटिनिवत्त NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#सचे !(NGX_WIN32)

    सचे (ext->access) {
        सचे (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            गच्छ failed;
        }
    }

#endif

    सचे (ext->time != -1) {
        सचे (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            गच्छ failed;
        }
    }

    सचे (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        पटिनिवत्त NGX_OK;
    }

    err = ngx_errno;

    सचे (err == NGX_ENOPATH) {

        सचे (!ext->create_path) {
            गच्छ failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        सचे (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            गच्छ failed;
        }

        सचे (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            पटिनिवत्त NGX_OK;
        }

        err = ngx_errno;
    }

#सचे (NGX_WIN32)

    सचे (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        सचे (err == 0) {
            पटिनिवत्त NGX_OK;
        }
    }

#endif

    सचे (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        सचे (name == रिक्त) {
            पटिनिवत्त NGX_ERROR;
        }

        (शून्य) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_सङ्ख्या(0));

        सचे (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            सचे (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                सचे (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->data);
                    पटिनिवत्त NGX_ERROR;
                }

                पटिनिवत्त NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->data);

            सचे (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    सचे (ext->delete_file) {
        सचे (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    सचे (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->data, to->data);
    }

    पटिनिवत्त NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf)
{
    वर्ण             *buf;
    off_t             size;
    time_t            time;
    size_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = रिक्त;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    सचे (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        गच्छ failed;
    }

    सचे (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } अञ्ञथा {
        सचे (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            गच्छ failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    सचे ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    सचे (buf == रिक्त) {
        गच्छ failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    सचे (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        गच्छ failed;
    }

    याव (size > 0) {

        सचे ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        सचे (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            गच्छ failed;
        }

        सचे ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            गच्छ failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        सचे (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            गच्छ failed;
        }

        सचे ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            गच्छ failed;
        }

        size -= n;
    }

    सचे (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        गच्छ failed;
    }

    rc = NGX_OK;

failed:

    सचे (nfd != NGX_INVALID_FILE) {
        सचे (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    सचे (fd != NGX_INVALID_FILE) {
        सचे (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    सचे (buf) {
        ngx_free(buf);
    }

    पटिनिवत्त rc;
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
ngx_walk_वृक्षः(ngx_वृक्षः_ctx_t *ctx, ngx_str_t *वृक्षः)
{
    शून्य       *data, *prev;
    u_char     *p, *name;
    size_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                   "walk tree \"%V\"", वृक्षः);

    सचे (ngx_open_dir(वृक्षः, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", वृक्षः->data);
        पटिनिवत्त NGX_ERROR;
    }

    prev = ctx->data;

    सचे (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        सचे (data == रिक्त) {
            गच्छ failed;
        }

        सचे (ctx->init_handler(data, prev) == NGX_ABORT) {
            गच्छ failed;
        }

        ctx->data = data;

    } अञ्ञथा {
        data = रिक्त;
    }

    अत्थाय ( ;; ) {

        ngx_set_errno(0);

        सचे (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            सचे (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } अञ्ञथा {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, err,
                              ngx_read_dir_n " \"%s\" failed", वृक्षः->data);
                rc = NGX_ERROR;
            }

            गच्छ done;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                      "tree name %uz:\"%s\"", len, name);

        सचे (len == 1 && name[0] == '.') {
            अनुवर्त;
        }

        सचे (len == 2 && name[0] == '.' && name[1] == '.') {
            अनुवर्त;
        }

        file.len = वृक्षः->len + 1 + len;

        सचे (file.len > buf.len) {

            सचे (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = वृक्षः->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            सचे (buf.data == रिक्त) {
                गच्छ failed;
            }
        }

        p = ngx_cpymem(buf.data, वृक्षः->data, वृक्षः->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        सचे (!dir.valid_info) {
            सचे (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                अनुवर्त;
            }
        }

        सचे (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            सचे (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                गच्छ failed;
            }

        } अञ्ञथा सचे (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_वृक्षः_handler(ctx, &file);

            सचे (rc == NGX_ABORT) {
                गच्छ failed;
            }

            सचे (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                अनुवर्त;
            }

            सचे (ngx_walk_वृक्षः(ctx, &file) == NGX_ABORT) {
                गच्छ failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            सचे (ctx->post_वृक्षः_handler(ctx, &file) == NGX_ABORT) {
                गच्छ failed;
            }

        } अञ्ञथा {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            सचे (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                गच्छ failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    सचे (buf.len) {
        ngx_free(buf.data);
    }

    सचे (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    सचे (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", वृक्षः->data);
    }

    पटिनिवत्त rc;
}
