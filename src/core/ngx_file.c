
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

    जइ (rc == NGX_OK) {
        प्रत्यागम rc;
    }

    len = prefix->len;

#जइ (NGX_WIN32)

    जइ (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    जइ (n == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->data, len);
    ngx_cpystrn(p, name->data, name->len + 1);

    name->len += len;
    name->data = n;

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#जइ (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    जइ (name->len < 2) {
        जइ (c0 == '/') {
            प्रत्यागम 2;
        }

        प्रत्यागम NGX_DECLINED;
    }

    c1 = name->data[1];

    जइ (c1 == ':') {
        c0 |= 0x20;

        जइ ((c0 >= 'a' && c0 <= 'z')) {
            प्रत्यागम NGX_OK;
        }

        प्रत्यागम NGX_DECLINED;
    }

    जइ (c1 == '/') {
        प्रत्यागम NGX_OK;
    }

    जइ (c0 == '/') {
        प्रत्यागम 2;
    }

    प्रत्यागम NGX_DECLINED;

#अण्णहा

    जइ (name->data[0] == '/') {
        प्रत्यागम NGX_OK;
    }

    प्रत्यागम NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    जइ (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        जइ (rc != NGX_OK) {
            प्रत्यागम rc;
        }

        जइ (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#जइ (NGX_THREADS && NGX_HAVE_PWRITEV)

    जइ (tf->thread_write) {
        प्रत्यागम ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    प्रत्यागम ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
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

    जइ (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } अण्णहा {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.data = ngx_pnalloc(pool, file->name.len + 1);
    जइ (file->name.data == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

#जइ 0
    प्रति (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    जइ (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_सङ्ख्या(0);

    cln = ngx_pool_cleanup_add(pool, परिमाण(ngx_pool_cleanup_file_t));
    जइ (cln == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    प्रति ( ;; ) {
        (शून्य) ngx_sprintf(p, "%010uD%Z", n);

        जइ (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        जइ (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            प्रत्यागम NGX_OK;
        }

        err = ngx_errno;

        जइ (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_सङ्ख्या(1);
            अनुवर्त;
        }

        जइ ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            प्रत्यागम NGX_ERROR;
        }

        जइ (ngx_create_path(file, path) == NGX_ERROR) {
            प्रत्यागम NGX_ERROR;
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

    प्रति (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        जइ (level == 0) {
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

    प्रति (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        जइ (path->level[i] == 0) {
            भङ्ग;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        जइ (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            जइ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.data);
                प्रत्यागम NGX_ERROR;
            }
        }

        file->name.data[pos] = '/';
    }

    प्रत्यागम NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#जइ (NGX_WIN32)
    p = dir + 3;
#अण्णहा
    p = dir + 1;
#endif

    प्रति ( /* void */ ; *p; p++) {
        ch = *p;

        जइ (ch != '/') {
            अनुवर्त;
        }

        *p = '\0';

        जइ (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            विकल्प (err) {
            पक्ष NGX_EEXIST:
                err = 0;
            पक्ष NGX_EACCES:
                भङ्ग;

            स्वभाव:
                प्रत्यागम err;
            }
        }

        *p = '/';
    }

    प्रत्यागम err;
}


ngx_atomic_uint_t
ngx_next_temp_सङ्ख्या(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_सङ्ख्या : 1;

    n = ngx_atomic_fetch_add(ngx_temp_सङ्ख्या, add);

    प्रत्यागम n + add;
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

    जइ (*slot) {
        प्रत्यागम "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, परिमाण(ngx_path_t));
    जइ (path == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    मानम् = cf->args->elts;

    path->name = मानम्[1];

    जइ (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    जइ (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->पङ्क्तिः = cf->conf_file->पङ्क्तिः;

    प्रति (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(मानम्[n].data, मानम्[n].len);
        जइ (level == NGX_ERROR || level == 0) {
            प्रत्यागम "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    जइ (path->len > 10 + i) {
        प्रत्यागम "invalid value";
    }

    *slot = path;

    जइ (ngx_add_path(cf, slot) == NGX_ERROR) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_merge_path_मानम्(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    जइ (*path) {
        प्रत्यागम NGX_CONF_OK;
    }

    जइ (prev) {
        *path = prev;
        प्रत्यागम NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, परिमाण(ngx_path_t));
    जइ (*path == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    जइ (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    प्रति (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    जइ (ngx_add_path(cf, path) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *confp = conf;

    u_char      *p;
    ngx_str_t   *मानम्;
    ngx_uint_t   i, दक्षिणः, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    जइ (*access != NGX_CONF_UNSET_UINT) {
        प्रत्यागम "is duplicate";
    }

    मानम् = cf->args->elts;

    *access = 0;
    user = 0600;

    प्रति (i = 1; i < cf->args->nelts; i++) {

        p = मानम्[i].data;

        जइ (ngx_strncmp(p, "user:", परिमाण("user:") - 1) == 0) {
            shift = 6;
            p += परिमाण("user:") - 1;
            user = 0;

        } अण्णहा जइ (ngx_strncmp(p, "group:", परिमाण("group:") - 1) == 0) {
            shift = 3;
            p += परिमाण("group:") - 1;

        } अण्णहा जइ (ngx_strncmp(p, "all:", परिमाण("all:") - 1) == 0) {
            shift = 0;
            p += परिमाण("all:") - 1;

        } अण्णहा {
            गच्छ invalid;
        }

        जइ (ngx_strcmp(p, "rw") == 0) {
            दक्षिणः = 6;

        } अण्णहा जइ (ngx_strcmp(p, "r") == 0) {
            दक्षिणः = 4;

        } अण्णहा {
            गच्छ invalid;
        }

        *access |= दक्षिणः << shift;
    }

    *access |= user;

    प्रत्यागम NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &मानम्[i]);

    प्रत्यागम NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    प्रति (i = 0; i < cf->cycle->paths.nelts; i++) {
        जइ (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            जइ (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->पङ्क्तिः);
                प्रत्यागम NGX_ERROR;
            }

            प्रति (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                जइ (p[i]->level[n] != path->level[n]) {
                    जइ (path->conf_file == रिक्त) {
                        जइ (p[i]->conf_file == रिक्त) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            प्रत्यागम NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->पङ्क्तिः);
                        प्रत्यागम NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->पङ्क्तिः);
                    प्रत्यागम NGX_ERROR;
                }

                जइ (p[i]->level[n] == 0) {
                    भङ्ग;
                }
            }

            *slot = p[i];

            प्रत्यागम NGX_OK;
        }
    }

    p = ngx_array_प्रक्षिप(&cf->cycle->paths);
    जइ (p == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    *p = path;

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    प्रति (i = 0; i < cycle->paths.nelts; i++) {

        जइ (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            जइ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                प्रत्यागम NGX_ERROR;
            }
        }

        जइ (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            अनुवर्त;
        }

#जइ !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        जइ (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            प्रत्यागम NGX_ERROR;
        }

        जइ (fi.st_uid != user) {
            जइ (chown((ध्रुव वर्ण *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                प्रत्यागम NGX_ERROR;
            }
        }

        जइ ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            जइ (chmod((ध्रुव वर्ण *) path[i]->name.data, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.data);
                प्रत्यागम NGX_ERROR;
            }
        }
        }
#endif
    }

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#जइ !(NGX_WIN32)

    जइ (ext->access) {
        जइ (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            गच्छ failed;
        }
    }

#endif

    जइ (ext->time != -1) {
        जइ (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            गच्छ failed;
        }
    }

    जइ (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        प्रत्यागम NGX_OK;
    }

    err = ngx_errno;

    जइ (err == NGX_ENOPATH) {

        जइ (!ext->create_path) {
            गच्छ failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        जइ (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            गच्छ failed;
        }

        जइ (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            प्रत्यागम NGX_OK;
        }

        err = ngx_errno;
    }

#जइ (NGX_WIN32)

    जइ (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        जइ (err == 0) {
            प्रत्यागम NGX_OK;
        }
    }

#endif

    जइ (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        जइ (name == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        (शून्य) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_सङ्ख्या(0));

        जइ (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            जइ (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                जइ (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->data);
                    प्रत्यागम NGX_ERROR;
                }

                प्रत्यागम NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->data);

            जइ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    जइ (ext->delete_file) {
        जइ (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    जइ (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->data, to->data);
    }

    प्रत्यागम NGX_ERROR;
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

    जइ (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        गच्छ failed;
    }

    जइ (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } अण्णहा {
        जइ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            गच्छ failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    जइ ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    जइ (buf == रिक्त) {
        गच्छ failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    जइ (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        गच्छ failed;
    }

    जाव (size > 0) {

        जइ ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        जइ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            गच्छ failed;
        }

        जइ ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            गच्छ failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        जइ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            गच्छ failed;
        }

        जइ ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            गच्छ failed;
        }

        size -= n;
    }

    जइ (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        गच्छ failed;
    }

    rc = NGX_OK;

failed:

    जइ (nfd != NGX_INVALID_FILE) {
        जइ (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    जइ (fd != NGX_INVALID_FILE) {
        जइ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    जइ (buf) {
        ngx_free(buf);
    }

    प्रत्यागम rc;
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

    जइ (ngx_open_dir(वृक्षः, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", वृक्षः->data);
        प्रत्यागम NGX_ERROR;
    }

    prev = ctx->data;

    जइ (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        जइ (data == रिक्त) {
            गच्छ failed;
        }

        जइ (ctx->init_handler(data, prev) == NGX_ABORT) {
            गच्छ failed;
        }

        ctx->data = data;

    } अण्णहा {
        data = रिक्त;
    }

    प्रति ( ;; ) {

        ngx_set_errno(0);

        जइ (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            जइ (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } अण्णहा {
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

        जइ (len == 1 && name[0] == '.') {
            अनुवर्त;
        }

        जइ (len == 2 && name[0] == '.' && name[1] == '.') {
            अनुवर्त;
        }

        file.len = वृक्षः->len + 1 + len;

        जइ (file.len > buf.len) {

            जइ (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = वृक्षः->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            जइ (buf.data == रिक्त) {
                गच्छ failed;
            }
        }

        p = ngx_cpymem(buf.data, वृक्षः->data, वृक्षः->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        जइ (!dir.valid_info) {
            जइ (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                अनुवर्त;
            }
        }

        जइ (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            जइ (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                गच्छ failed;
            }

        } अण्णहा जइ (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_वृक्षः_handler(ctx, &file);

            जइ (rc == NGX_ABORT) {
                गच्छ failed;
            }

            जइ (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                अनुवर्त;
            }

            जइ (ngx_walk_वृक्षः(ctx, &file) == NGX_ABORT) {
                गच्छ failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            जइ (ctx->post_वृक्षः_handler(ctx, &file) == NGX_ABORT) {
                गच्छ failed;
            }

        } अण्णहा {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            जइ (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                गच्छ failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    जइ (buf.len) {
        ngx_free(buf.data);
    }

    जइ (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    जइ (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", वृक्षः->data);
    }

    प्रत्यागम rc;
}
