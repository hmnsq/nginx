
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ ngx_int_t ngx_test_full_name(ngx_str_t *name);


ಸ್ಥಿರ ngx_atomic_t   temp_ಸಂಖ್ಯೆ = 0;
ngx_atomic_t         *ngx_temp_ಸಂಖ್ಯೆ = &temp_ಸಂಖ್ಯೆ;
ngx_atomic_int_t      ngx_random_ಸಂಖ್ಯೆ = 123456;


ngx_int_t
ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix, ngx_str_t *name)
{
    size_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_full_name(name);

    ಯದಿ (rc == NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ rc;
    }

    len = prefix->len;

#ಯದಿ (NGX_WIN32)

    ಯದಿ (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    ಯದಿ (n == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->data, len);
    ngx_cpystrn(p, name->data, name->len + 1);

    name->len += len;
    name->data = n;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#ಯದಿ (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    ಯದಿ (name->len < 2) {
        ಯದಿ (c0 == '/') {
            ಪ್ರತ್ಯಾಗಮ 2;
        }

        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    c1 = name->data[1];

    ಯದಿ (c1 == ':') {
        c0 |= 0x20;

        ಯದಿ ((c0 >= 'a' && c0 <= 'z')) {
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;
    }

    ಯದಿ (c1 == '/') {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಯದಿ (c0 == '/') {
        ಪ್ರತ್ಯಾಗಮ 2;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

#ಅನ್ಯಥಾ

    ಯದಿ (name->data[0] == '/') {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    ಯದಿ (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        ಯದಿ (rc != NGX_OK) {
            ಪ್ರತ್ಯಾಗಮ rc;
        }

        ಯದಿ (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#ಯದಿ (NGX_THREADS && NGX_HAVE_PWRITEV)

    ಯದಿ (tf->thread_write) {
        ಪ್ರತ್ಯಾಗಮ ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    ಪ್ರತ್ಯಾಗಮ ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
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

    ಯದಿ (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } ಅನ್ಯಥಾ {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.data = ngx_pnalloc(pool, file->name.len + 1);
    ಯದಿ (file->name.data == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

#ಯದಿ 0
    ಪ್ರತಿ (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    ಯದಿ (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_ಸಂಖ್ಯೆ(0);

    cln = ngx_pool_cleanup_add(pool, ಪರಿಮಾಣ(ngx_pool_cleanup_file_t));
    ಯದಿ (cln == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಪ್ರತಿ ( ;; ) {
        (ಶೂನ್ಯ) ngx_sprintf(p, "%010uD%Z", n);

        ಯದಿ (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        ಯದಿ (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        err = ngx_errno;

        ಯದಿ (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_ಸಂಖ್ಯೆ(1);
            ಅನುವರ್ತ;
        }

        ಯದಿ ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (ngx_create_path(file, path) == NGX_ERROR) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }
    }
}


ಶೂನ್ಯ
ngx_create_hashed_filename(ngx_path_t *path, u_char *file, size_t len)
{
    size_t      i, level;
    ngx_uint_t  n;

    i = path->name.len + 1;

    file[path->name.len + path->len]  = '/';

    ಪ್ರತಿ (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        ಯದಿ (level == 0) {
            ಭಙ್ಗ;
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

    ಪ್ರತಿ (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        ಯದಿ (path->level[i] == 0) {
            ಭಙ್ಗ;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        ಯದಿ (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ಯದಿ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.data);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
        }

        file->name.data[pos] = '/';
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#ಯದಿ (NGX_WIN32)
    p = dir + 3;
#ಅನ್ಯಥಾ
    p = dir + 1;
#endif

    ಪ್ರತಿ ( /* void */ ; *p; p++) {
        ch = *p;

        ಯದಿ (ch != '/') {
            ಅನುವರ್ತ;
        }

        *p = '\0';

        ಯದಿ (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            ವಿಕಲ್ಪ (err) {
            ಪಕ್ಷ NGX_EEXIST:
                err = 0;
            ಪಕ್ಷ NGX_EACCES:
                ಭಙ್ಗ;

            ಸ್ವಭಾವ:
                ಪ್ರತ್ಯಾಗಮ err;
            }
        }

        *p = '/';
    }

    ಪ್ರತ್ಯಾಗಮ err;
}


ngx_atomic_uint_t
ngx_next_temp_ಸಂಖ್ಯೆ(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_ಸಂಖ್ಯೆ : 1;

    n = ngx_atomic_fetch_add(ngx_temp_ಸಂಖ್ಯೆ, add);

    ಪ್ರತ್ಯಾಗಮ n + add;
}


ವರ್ಣ *
ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ  *p = conf;

    ssize_t      level;
    ngx_str_t   *ಮೌಲ್ಯ;
    ngx_uint_t   i, n;
    ngx_path_t  *path, **slot;

    slot = (ngx_path_t **) (p + cmd->offset);

    ಯದಿ (*slot) {
        ಪ್ರತ್ಯಾಗಮ "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, ಪರಿಮಾಣ(ngx_path_t));
    ಯದಿ (path == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಮೌಲ್ಯ = cf->args->elts;

    path->name = ಮೌಲ್ಯ[1];

    ಯದಿ (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    ಯದಿ (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->ಸಾಲು = cf->conf_file->ಸಾಲು;

    ಪ್ರತಿ (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(ಮೌಲ್ಯ[n].data, ಮೌಲ್ಯ[n].len);
        ಯದಿ (level == NGX_ERROR || level == 0) {
            ಪ್ರತ್ಯಾಗಮ "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    ಯದಿ (path->len > 10 + i) {
        ಪ್ರತ್ಯಾಗಮ "invalid value";
    }

    *slot = path;

    ಯದಿ (ngx_add_path(cf, slot) == NGX_ERROR) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ವರ್ಣ *
ngx_conf_merge_path_ಮೌಲ್ಯ(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    ಯದಿ (*path) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
    }

    ಯದಿ (prev) {
        *path = prev;
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, ಪರಿಮಾಣ(ngx_path_t));
    ಯದಿ (*path == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    ಯದಿ (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಪ್ರತಿ (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    ಯದಿ (ngx_add_path(cf, path) != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
    }

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;
}


ವರ್ಣ *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ವರ್ಣ  *confp = conf;

    u_char      *p;
    ngx_str_t   *ಮೌಲ್ಯ;
    ngx_uint_t   i, ಬಲ, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    ಯದಿ (*access != NGX_CONF_UNSET_UINT) {
        ಪ್ರತ್ಯಾಗಮ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->args->elts;

    *access = 0;
    user = 0600;

    ಪ್ರತಿ (i = 1; i < cf->args->nelts; i++) {

        p = ಮೌಲ್ಯ[i].data;

        ಯದಿ (ngx_strncmp(p, "user:", ಪರಿಮಾಣ("user:") - 1) == 0) {
            shift = 6;
            p += ಪರಿಮಾಣ("user:") - 1;
            user = 0;

        } ಅನ್ಯಥಾ ಯದಿ (ngx_strncmp(p, "group:", ಪರಿಮಾಣ("group:") - 1) == 0) {
            shift = 3;
            p += ಪರಿಮಾಣ("group:") - 1;

        } ಅನ್ಯಥಾ ಯದಿ (ngx_strncmp(p, "all:", ಪರಿಮಾಣ("all:") - 1) == 0) {
            shift = 0;
            p += ಪರಿಮಾಣ("all:") - 1;

        } ಅನ್ಯಥಾ {
            ಗಚ್ಛ invalid;
        }

        ಯದಿ (ngx_strcmp(p, "rw") == 0) {
            ಬಲ = 6;

        } ಅನ್ಯಥಾ ಯದಿ (ngx_strcmp(p, "r") == 0) {
            ಬಲ = 4;

        } ಅನ್ಯಥಾ {
            ಗಚ್ಛ invalid;
        }

        *access |= ಬಲ << shift;
    }

    *access |= user;

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &ಮೌಲ್ಯ[i]);

    ಪ್ರತ್ಯಾಗಮ NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    ಪ್ರತಿ (i = 0; i < cf->cycle->paths.nelts; i++) {
        ಯದಿ (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            ಯದಿ (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->ಸಾಲು);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }

            ಪ್ರತಿ (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                ಯದಿ (p[i]->level[n] != path->level[n]) {
                    ಯದಿ (path->conf_file == ರಿಕ್ತ) {
                        ಯದಿ (p[i]->conf_file == ರಿಕ್ತ) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->ಸಾಲು);
                        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->ಸಾಲು);
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

                ಯದಿ (p[i]->level[n] == 0) {
                    ಭಙ್ಗ;
                }
            }

            *slot = p[i];

            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }
    }

    p = ngx_array_ತಳ್ಳು(&cf->cycle->paths);
    ಯದಿ (p == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    *p = path;

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    ಪ್ರತಿ (i = 0; i < cycle->paths.nelts; i++) {

        ಯದಿ (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ಯದಿ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
        }

        ಯದಿ (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ಅನುವರ್ತ;
        }

#ಯದಿ !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        ಯದಿ (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        ಯದಿ (fi.st_uid != user) {
            ಯದಿ (chown((ಧ್ರುವ ವರ್ಣ *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
        }

        ಯದಿ ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            ಯದಿ (chmod((ಧ್ರುವ ವರ್ಣ *) path[i]->name.data, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.data);
                ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
            }
        }
        }
#endif
    }

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#ಯದಿ !(NGX_WIN32)

    ಯದಿ (ext->access) {
        ಯದಿ (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            ಗಚ್ಛ failed;
        }
    }

#endif

    ಯದಿ (ext->time != -1) {
        ಯದಿ (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            ಗಚ್ಛ failed;
        }
    }

    ಯದಿ (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    err = ngx_errno;

    ಯದಿ (err == NGX_ENOPATH) {

        ಯದಿ (!ext->create_path) {
            ಗಚ್ಛ failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        ಯದಿ (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            ಗಚ್ಛ failed;
        }

        ಯದಿ (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

        err = ngx_errno;
    }

#ಯದಿ (NGX_WIN32)

    ಯದಿ (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        ಯದಿ (err == 0) {
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }
    }

#endif

    ಯದಿ (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        ಯದಿ (name == ರಿಕ್ತ) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        (ಶೂನ್ಯ) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_ಸಂಖ್ಯೆ(0));

        ಯದಿ (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            ಯದಿ (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                ಯದಿ (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->data);
                    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
                }

                ಪ್ರತ್ಯಾಗಮ NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->data);

            ಯದಿ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    ಯದಿ (ext->delete_file) {
        ಯದಿ (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    ಯದಿ (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->data, to->data);
    }

    ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf)
{
    ವರ್ಣ             *buf;
    off_t             size;
    time_t            time;
    size_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = ರಿಕ್ತ;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    ಯದಿ (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        ಗಚ್ಛ failed;
    }

    ಯದಿ (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } ಅನ್ಯಥಾ {
        ಯದಿ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            ಗಚ್ಛ failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    ಯದಿ ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    ಯದಿ (buf == ರಿಕ್ತ) {
        ಗಚ್ಛ failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    ಯದಿ (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        ಗಚ್ಛ failed;
    }

    ಯಾವತ್ (size > 0) {

        ಯದಿ ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        ಯದಿ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            ಗಚ್ಛ failed;
        }

        ಯದಿ ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            ಗಚ್ಛ failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        ಯದಿ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            ಗಚ್ಛ failed;
        }

        ಯದಿ ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            ಗಚ್ಛ failed;
        }

        size -= n;
    }

    ಯದಿ (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        ಗಚ್ಛ failed;
    }

    rc = NGX_OK;

failed:

    ಯದಿ (nfd != NGX_INVALID_FILE) {
        ಯದಿ (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    ಯದಿ (fd != NGX_INVALID_FILE) {
        ಯದಿ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    ಯದಿ (buf) {
        ngx_free(buf);
    }

    ಪ್ರತ್ಯಾಗಮ rc;
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
ngx_walk_ವೃಕ್ಷ(ngx_ವೃಕ್ಷ_ctx_t *ctx, ngx_str_t *ವೃಕ್ಷ)
{
    ಶೂನ್ಯ       *data, *prev;
    u_char     *p, *name;
    size_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                   "walk tree \"%V\"", ವೃಕ್ಷ);

    ಯದಿ (ngx_open_dir(ವೃಕ್ಷ, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", ವೃಕ್ಷ->data);
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    prev = ctx->data;

    ಯದಿ (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        ಯದಿ (data == ರಿಕ್ತ) {
            ಗಚ್ಛ failed;
        }

        ಯದಿ (ctx->init_handler(data, prev) == NGX_ABORT) {
            ಗಚ್ಛ failed;
        }

        ctx->data = data;

    } ಅನ್ಯಥಾ {
        data = ರಿಕ್ತ;
    }

    ಪ್ರತಿ ( ;; ) {

        ngx_set_errno(0);

        ಯದಿ (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            ಯದಿ (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } ಅನ್ಯಥಾ {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, err,
                              ngx_read_dir_n " \"%s\" failed", ವೃಕ್ಷ->data);
                rc = NGX_ERROR;
            }

            ಗಚ್ಛ done;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                      "tree name %uz:\"%s\"", len, name);

        ಯದಿ (len == 1 && name[0] == '.') {
            ಅನುವರ್ತ;
        }

        ಯದಿ (len == 2 && name[0] == '.' && name[1] == '.') {
            ಅನುವರ್ತ;
        }

        file.len = ವೃಕ್ಷ->len + 1 + len;

        ಯದಿ (file.len > buf.len) {

            ಯದಿ (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = ವೃಕ್ಷ->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            ಯದಿ (buf.data == ರಿಕ್ತ) {
                ಗಚ್ಛ failed;
            }
        }

        p = ngx_cpymem(buf.data, ವೃಕ್ಷ->data, ವೃಕ್ಷ->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        ಯದಿ (!dir.valid_info) {
            ಯದಿ (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                ಅನುವರ್ತ;
            }
        }

        ಯದಿ (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ಯದಿ (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                ಗಚ್ಛ failed;
            }

        } ಅನ್ಯಥಾ ಯದಿ (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_ವೃಕ್ಷ_handler(ctx, &file);

            ಯದಿ (rc == NGX_ABORT) {
                ಗಚ್ಛ failed;
            }

            ಯದಿ (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                ಅನುವರ್ತ;
            }

            ಯದಿ (ngx_walk_ವೃಕ್ಷ(ctx, &file) == NGX_ABORT) {
                ಗಚ್ಛ failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ಯದಿ (ctx->post_ವೃಕ್ಷ_handler(ctx, &file) == NGX_ABORT) {
                ಗಚ್ಛ failed;
            }

        } ಅನ್ಯಥಾ {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            ಯದಿ (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                ಗಚ್ಛ failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    ಯದಿ (buf.len) {
        ngx_free(buf.data);
    }

    ಯದಿ (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    ಯದಿ (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", ವೃಕ್ಷ->data);
    }

    ಪ್ರತ್ಯಾಗಮ rc;
}
