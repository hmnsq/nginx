
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


static ngx_int_t ngx_test_full_name(ngx_str_t *name);


static ngx_atomic_t   temp_number = 0;
ngx_atomic_t         *ngx_temp_number = &temp_number;
ngx_atomic_int_t      ngx_random_number = 123456;


ngx_int_t
ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix, ngx_str_t *name)
{
    size_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_full_name(name);

    ຖ້າ (rc == NGX_OK) {
        ສົ່ງຄືນ rc;
    }

    len = prefix->len;

#ຖ້າ (NGX_WIN32)

    ຖ້າ (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    ຖ້າ (n == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->data, len);
    ngx_cpystrn(p, name->data, name->len + 1);

    name->len += len;
    name->data = n;

    ສົ່ງຄືນ NGX_OK;
}


static ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#ຖ້າ (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    ຖ້າ (name->len < 2) {
        ຖ້າ (c0 == '/') {
            ສົ່ງຄືນ 2;
        }

        ສົ່ງຄືນ NGX_DECLINED;
    }

    c1 = name->data[1];

    ຖ້າ (c1 == ':') {
        c0 |= 0x20;

        ຖ້າ ((c0 >= 'a' && c0 <= 'z')) {
            ສົ່ງຄືນ NGX_OK;
        }

        ສົ່ງຄືນ NGX_DECLINED;
    }

    ຖ້າ (c1 == '/') {
        ສົ່ງຄືນ NGX_OK;
    }

    ຖ້າ (c0 == '/') {
        ສົ່ງຄືນ 2;
    }

    ສົ່ງຄືນ NGX_DECLINED;

#ບໍ່ດັ່ງນັ້ນ

    ຖ້າ (name->data[0] == '/') {
        ສົ່ງຄືນ NGX_OK;
    }

    ສົ່ງຄືນ NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    ຖ້າ (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        ຖ້າ (rc != NGX_OK) {
            ສົ່ງຄືນ rc;
        }

        ຖ້າ (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#ຖ້າ (NGX_THREADS && NGX_HAVE_PWRITEV)

    ຖ້າ (tf->thread_write) {
        ສົ່ງຄືນ ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    ສົ່ງຄືນ ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
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

    ຖ້າ (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } ບໍ່ດັ່ງນັ້ນ {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.data = ngx_pnalloc(pool, file->name.len + 1);
    ຖ້າ (file->name.data == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

#ຖ້າ 0
    ສຳລັບ (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    ຖ້າ (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_number(0);

    cln = ngx_pool_cleanup_add(pool, parimaa_na(ngx_pool_cleanup_file_t));
    ຖ້າ (cln == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    ສຳລັບ ( ;; ) {
        (void) ngx_sprintf(p, "%010uD%Z", n);

        ຖ້າ (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        ຖ້າ (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            ສົ່ງຄືນ NGX_OK;
        }

        err = ngx_errno;

        ຖ້າ (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_number(1);
            continue;
        }

        ຖ້າ ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            ສົ່ງຄືນ NGX_ERROR;
        }

        ຖ້າ (ngx_create_path(file, path) == NGX_ERROR) {
            ສົ່ງຄືນ NGX_ERROR;
        }
    }
}


void
ngx_create_hashed_filename(ngx_path_t *path, u_char *file, size_t len)
{
    size_t      i, level;
    ngx_uint_t  n;

    i = path->name.len + 1;

    file[path->name.len + path->len]  = '/';

    ສຳລັບ (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        ຖ້າ (level == 0) {
            bha_nga;
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

    ສຳລັບ (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        ຖ້າ (path->level[i] == 0) {
            bha_nga;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        ຖ້າ (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ຖ້າ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.data);
                ສົ່ງຄືນ NGX_ERROR;
            }
        }

        file->name.data[pos] = '/';
    }

    ສົ່ງຄືນ NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#ຖ້າ (NGX_WIN32)
    p = dir + 3;
#ບໍ່ດັ່ງນັ້ນ
    p = dir + 1;
#endif

    ສຳລັບ ( /* void */ ; *p; p++) {
        ch = *p;

        ຖ້າ (ch != '/') {
            continue;
        }

        *p = '\0';

        ຖ້າ (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            switch (err) {
            pak_sa NGX_EEXIST:
                err = 0;
            pak_sa NGX_EACCES:
                bha_nga;

            default:
                ສົ່ງຄືນ err;
            }
        }

        *p = '/';
    }

    ສົ່ງຄືນ err;
}


ngx_atomic_uint_t
ngx_next_temp_number(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_number : 1;

    n = ngx_atomic_fetch_add(ngx_temp_number, add);

    ສົ່ງຄືນ n + add;
}


var_na *
ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *p = conf;

    ssize_t      level;
    ngx_str_t   *value;
    ngx_uint_t   i, n;
    ngx_path_t  *path, **slot;

    slot = (ngx_path_t **) (p + cmd->offset);

    ຖ້າ (*slot) {
        ສົ່ງຄືນ "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, parimaa_na(ngx_path_t));
    ຖ້າ (path == NULL) {
        ສົ່ງຄືນ NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    path->name = value[1];

    ຖ້າ (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    ຖ້າ (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        ສົ່ງຄືນ NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->line = cf->conf_file->line;

    ສຳລັບ (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(value[n].data, value[n].len);
        ຖ້າ (level == NGX_ERROR || level == 0) {
            ສົ່ງຄືນ "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    ຖ້າ (path->len > 10 + i) {
        ສົ່ງຄືນ "invalid value";
    }

    *slot = path;

    ຖ້າ (ngx_add_path(cf, slot) == NGX_ERROR) {
        ສົ່ງຄືນ NGX_CONF_ERROR;
    }

    ສົ່ງຄືນ NGX_CONF_OK;
}


var_na *
ngx_conf_merge_path_value(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    ຖ້າ (*path) {
        ສົ່ງຄືນ NGX_CONF_OK;
    }

    ຖ້າ (prev) {
        *path = prev;
        ສົ່ງຄືນ NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, parimaa_na(ngx_path_t));
    ຖ້າ (*path == NULL) {
        ສົ່ງຄືນ NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    ຖ້າ (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        ສົ່ງຄືນ NGX_CONF_ERROR;
    }

    ສຳລັບ (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    ຖ້າ (ngx_add_path(cf, path) != NGX_OK) {
        ສົ່ງຄືນ NGX_CONF_ERROR;
    }

    ສົ່ງຄືນ NGX_CONF_OK;
}


var_na *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *confp = conf;

    u_char      *p;
    ngx_str_t   *value;
    ngx_uint_t   i, right, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    ຖ້າ (*access != NGX_CONF_UNSET_UINT) {
        ສົ່ງຄືນ "is duplicate";
    }

    value = cf->args->elts;

    *access = 0;
    user = 0600;

    ສຳລັບ (i = 1; i < cf->args->nelts; i++) {

        p = value[i].data;

        ຖ້າ (ngx_strncmp(p, "user:", parimaa_na("user:") - 1) == 0) {
            shift = 6;
            p += parimaa_na("user:") - 1;
            user = 0;

        } ບໍ່ດັ່ງນັ້ນ ຖ້າ (ngx_strncmp(p, "group:", parimaa_na("group:") - 1) == 0) {
            shift = 3;
            p += parimaa_na("group:") - 1;

        } ບໍ່ດັ່ງນັ້ນ ຖ້າ (ngx_strncmp(p, "all:", parimaa_na("all:") - 1) == 0) {
            shift = 0;
            p += parimaa_na("all:") - 1;

        } ບໍ່ດັ່ງນັ້ນ {
            goto invalid;
        }

        ຖ້າ (ngx_strcmp(p, "rw") == 0) {
            right = 6;

        } ບໍ່ດັ່ງນັ້ນ ຖ້າ (ngx_strcmp(p, "r") == 0) {
            right = 4;

        } ບໍ່ດັ່ງນັ້ນ {
            goto invalid;
        }

        *access |= right << shift;
    }

    *access |= user;

    ສົ່ງຄືນ NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &value[i]);

    ສົ່ງຄືນ NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    ສຳລັບ (i = 0; i < cf->cycle->paths.nelts; i++) {
        ຖ້າ (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            ຖ້າ (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->line);
                ສົ່ງຄືນ NGX_ERROR;
            }

            ສຳລັບ (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                ຖ້າ (p[i]->level[n] != path->level[n]) {
                    ຖ້າ (path->conf_file == NULL) {
                        ຖ້າ (p[i]->conf_file == NULL) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            ສົ່ງຄືນ NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->line);
                        ສົ່ງຄືນ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->line);
                    ສົ່ງຄືນ NGX_ERROR;
                }

                ຖ້າ (p[i]->level[n] == 0) {
                    bha_nga;
                }
            }

            *slot = p[i];

            ສົ່ງຄືນ NGX_OK;
        }
    }

    p = ngx_array_push(&cf->cycle->paths);
    ຖ້າ (p == NULL) {
        ສົ່ງຄືນ NGX_ERROR;
    }

    *p = path;

    ສົ່ງຄືນ NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    ສຳລັບ (i = 0; i < cycle->paths.nelts; i++) {

        ຖ້າ (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ຖ້າ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                ສົ່ງຄືນ NGX_ERROR;
            }
        }

        ຖ້າ (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            continue;
        }

#ຖ້າ !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        ຖ້າ (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            ສົ່ງຄືນ NGX_ERROR;
        }

        ຖ້າ (fi.st_uid != user) {
            ຖ້າ (chown((const var_na *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                ສົ່ງຄືນ NGX_ERROR;
            }
        }

        ຖ້າ ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            ຖ້າ (chmod((const var_na *) path[i]->name.data, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.data);
                ສົ່ງຄືນ NGX_ERROR;
            }
        }
        }
#endif
    }

    ສົ່ງຄືນ NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#ຖ້າ !(NGX_WIN32)

    ຖ້າ (ext->access) {
        ຖ້າ (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            goto failed;
        }
    }

#endif

    ຖ້າ (ext->time != -1) {
        ຖ້າ (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            goto failed;
        }
    }

    ຖ້າ (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        ສົ່ງຄືນ NGX_OK;
    }

    err = ngx_errno;

    ຖ້າ (err == NGX_ENOPATH) {

        ຖ້າ (!ext->create_path) {
            goto failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        ຖ້າ (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            goto failed;
        }

        ຖ້າ (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            ສົ່ງຄືນ NGX_OK;
        }

        err = ngx_errno;
    }

#ຖ້າ (NGX_WIN32)

    ຖ້າ (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        ຖ້າ (err == 0) {
            ສົ່ງຄືນ NGX_OK;
        }
    }

#endif

    ຖ້າ (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        ຖ້າ (name == NULL) {
            ສົ່ງຄືນ NGX_ERROR;
        }

        (void) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_number(0));

        ຖ້າ (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            ຖ້າ (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                ຖ້າ (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->data);
                    ສົ່ງຄືນ NGX_ERROR;
                }

                ສົ່ງຄືນ NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->data);

            ຖ້າ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    ຖ້າ (ext->delete_file) {
        ຖ້າ (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    ຖ້າ (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->data, to->data);
    }

    ສົ່ງຄືນ NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf)
{
    var_na             *buf;
    off_t             size;
    time_t            time;
    size_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = NULL;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    ຖ້າ (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        goto failed;
    }

    ຖ້າ (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } ບໍ່ດັ່ງນັ້ນ {
        ຖ້າ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            goto failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    ຖ້າ ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    ຖ້າ (buf == NULL) {
        goto failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    ຖ້າ (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        goto failed;
    }

    ໃນຂະນະທີ່ (size > 0) {

        ຖ້າ ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        ຖ້າ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            goto failed;
        }

        ຖ້າ ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            goto failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        ຖ້າ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            goto failed;
        }

        ຖ້າ ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            goto failed;
        }

        size -= n;
    }

    ຖ້າ (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        goto failed;
    }

    rc = NGX_OK;

failed:

    ຖ້າ (nfd != NGX_INVALID_FILE) {
        ຖ້າ (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    ຖ້າ (fd != NGX_INVALID_FILE) {
        ຖ້າ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    ຖ້າ (buf) {
        ngx_free(buf);
    }

    ສົ່ງຄືນ rc;
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
    void       *data, *prev;
    u_char     *p, *name;
    size_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                   "walk tree \"%V\"", tree);

    ຖ້າ (ngx_open_dir(tree, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", tree->data);
        ສົ່ງຄືນ NGX_ERROR;
    }

    prev = ctx->data;

    ຖ້າ (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        ຖ້າ (data == NULL) {
            goto failed;
        }

        ຖ້າ (ctx->init_handler(data, prev) == NGX_ABORT) {
            goto failed;
        }

        ctx->data = data;

    } ບໍ່ດັ່ງນັ້ນ {
        data = NULL;
    }

    ສຳລັບ ( ;; ) {

        ngx_set_errno(0);

        ຖ້າ (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            ຖ້າ (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } ບໍ່ດັ່ງນັ້ນ {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, err,
                              ngx_read_dir_n " \"%s\" failed", tree->data);
                rc = NGX_ERROR;
            }

            goto done;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                      "tree name %uz:\"%s\"", len, name);

        ຖ້າ (len == 1 && name[0] == '.') {
            continue;
        }

        ຖ້າ (len == 2 && name[0] == '.' && name[1] == '.') {
            continue;
        }

        file.len = tree->len + 1 + len;

        ຖ້າ (file.len > buf.len) {

            ຖ້າ (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = tree->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            ຖ້າ (buf.data == NULL) {
                goto failed;
            }
        }

        p = ngx_cpymem(buf.data, tree->data, tree->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        ຖ້າ (!dir.valid_info) {
            ຖ້າ (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                continue;
            }
        }

        ຖ້າ (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ຖ້າ (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                goto failed;
            }

        } ບໍ່ດັ່ງນັ້ນ ຖ້າ (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_tree_handler(ctx, &file);

            ຖ້າ (rc == NGX_ABORT) {
                goto failed;
            }

            ຖ້າ (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                continue;
            }

            ຖ້າ (ngx_walk_tree(ctx, &file) == NGX_ABORT) {
                goto failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ຖ້າ (ctx->post_tree_handler(ctx, &file) == NGX_ABORT) {
                goto failed;
            }

        } ບໍ່ດັ່ງນັ້ນ {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            ຖ້າ (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                goto failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    ຖ້າ (buf.len) {
        ngx_free(buf.data);
    }

    ຖ້າ (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    ຖ້າ (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", tree->data);
    }

    ສົ່ງຄືນ rc;
}
