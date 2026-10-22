
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

    ᨀᨘ (rc == NGX_OK) {
        ᨀᨘᨒ rc;
    }

    len = prefix->len;

#ᨀᨘ (NGX_WIN32)

    ᨀᨘ (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    ᨀᨘ (n == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->data, len);
    ngx_cpystrn(p, name->data, name->len + 1);

    name->len += len;
    name->data = n;

    ᨀᨘᨒ NGX_OK;
}


static ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#ᨀᨘ (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    ᨀᨘ (name->len < 2) {
        ᨀᨘ (c0 == '/') {
            ᨀᨘᨒ 2;
        }

        ᨀᨘᨒ NGX_DECLINED;
    }

    c1 = name->data[1];

    ᨀᨘ (c1 == ':') {
        c0 |= 0x20;

        ᨀᨘ ((c0 >= 'a' && c0 <= 'z')) {
            ᨀᨘᨒ NGX_OK;
        }

        ᨀᨘᨒ NGX_DECLINED;
    }

    ᨀᨘ (c1 == '/') {
        ᨀᨘᨒ NGX_OK;
    }

    ᨀᨘ (c0 == '/') {
        ᨀᨘᨒ 2;
    }

    ᨀᨘᨒ NGX_DECLINED;

#ᨀᨘᨁ

    ᨀᨘ (name->data[0] == '/') {
        ᨀᨘᨒ NGX_OK;
    }

    ᨀᨘᨒ NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    ᨀᨘ (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        ᨀᨘ (rc != NGX_OK) {
            ᨀᨘᨒ rc;
        }

        ᨀᨘ (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#ᨀᨘ (NGX_THREADS && NGX_HAVE_PWRITEV)

    ᨀᨘ (tf->thread_write) {
        ᨀᨘᨒ ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    ᨀᨘᨒ ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
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

    ᨀᨘ (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } ᨀᨘᨁ {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.data = ngx_pnalloc(pool, file->name.len + 1);
    ᨀᨘ (file->name.data == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

#ᨀᨘ 0
    for (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    ᨀᨘ (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_number(0);

    cln = ngx_pool_cleanup_add(pool, parimaa_na(ngx_pool_cleanup_file_t));
    ᨀᨘ (cln == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    for ( ;; ) {
        (void) ngx_sprintf(p, "%010uD%Z", n);

        ᨀᨘ (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        ᨀᨘ (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            ᨀᨘᨒ NGX_OK;
        }

        err = ngx_errno;

        ᨀᨘ (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_number(1);
            continue;
        }

        ᨀᨘ ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            ᨀᨘᨒ NGX_ERROR;
        }

        ᨀᨘ (ngx_create_path(file, path) == NGX_ERROR) {
            ᨀᨘᨒ NGX_ERROR;
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

    for (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        ᨀᨘ (level == 0) {
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

    for (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        ᨀᨘ (path->level[i] == 0) {
            bha_nga;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        ᨀᨘ (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ᨀᨘ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.data);
                ᨀᨘᨒ NGX_ERROR;
            }
        }

        file->name.data[pos] = '/';
    }

    ᨀᨘᨒ NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#ᨀᨘ (NGX_WIN32)
    p = dir + 3;
#ᨀᨘᨁ
    p = dir + 1;
#endif

    for ( /* void */ ; *p; p++) {
        ch = *p;

        ᨀᨘ (ch != '/') {
            continue;
        }

        *p = '\0';

        ᨀᨘ (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            switch (err) {
            pak_sa NGX_EEXIST:
                err = 0;
            pak_sa NGX_EACCES:
                bha_nga;

            default:
                ᨀᨘᨒ err;
            }
        }

        *p = '/';
    }

    ᨀᨘᨒ err;
}


ngx_atomic_uint_t
ngx_next_temp_number(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_number : 1;

    n = ngx_atomic_fetch_add(ngx_temp_number, add);

    ᨀᨘᨒ n + add;
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

    ᨀᨘ (*slot) {
        ᨀᨘᨒ "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, parimaa_na(ngx_path_t));
    ᨀᨘ (path == NULL) {
        ᨀᨘᨒ NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    path->name = value[1];

    ᨀᨘ (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    ᨀᨘ (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        ᨀᨘᨒ NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->line = cf->conf_file->line;

    for (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(value[n].data, value[n].len);
        ᨀᨘ (level == NGX_ERROR || level == 0) {
            ᨀᨘᨒ "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    ᨀᨘ (path->len > 10 + i) {
        ᨀᨘᨒ "invalid value";
    }

    *slot = path;

    ᨀᨘ (ngx_add_path(cf, slot) == NGX_ERROR) {
        ᨀᨘᨒ NGX_CONF_ERROR;
    }

    ᨀᨘᨒ NGX_CONF_OK;
}


var_na *
ngx_conf_merge_path_value(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    ᨀᨘ (*path) {
        ᨀᨘᨒ NGX_CONF_OK;
    }

    ᨀᨘ (prev) {
        *path = prev;
        ᨀᨘᨒ NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, parimaa_na(ngx_path_t));
    ᨀᨘ (*path == NULL) {
        ᨀᨘᨒ NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    ᨀᨘ (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        ᨀᨘᨒ NGX_CONF_ERROR;
    }

    for (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    ᨀᨘ (ngx_add_path(cf, path) != NGX_OK) {
        ᨀᨘᨒ NGX_CONF_ERROR;
    }

    ᨀᨘᨒ NGX_CONF_OK;
}


var_na *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *confp = conf;

    u_char      *p;
    ngx_str_t   *value;
    ngx_uint_t   i, right, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    ᨀᨘ (*access != NGX_CONF_UNSET_UINT) {
        ᨀᨘᨒ "is duplicate";
    }

    value = cf->args->elts;

    *access = 0;
    user = 0600;

    for (i = 1; i < cf->args->nelts; i++) {

        p = value[i].data;

        ᨀᨘ (ngx_strncmp(p, "user:", parimaa_na("user:") - 1) == 0) {
            shift = 6;
            p += parimaa_na("user:") - 1;
            user = 0;

        } ᨀᨘᨁ ᨀᨘ (ngx_strncmp(p, "group:", parimaa_na("group:") - 1) == 0) {
            shift = 3;
            p += parimaa_na("group:") - 1;

        } ᨀᨘᨁ ᨀᨘ (ngx_strncmp(p, "all:", parimaa_na("all:") - 1) == 0) {
            shift = 0;
            p += parimaa_na("all:") - 1;

        } ᨀᨘᨁ {
            goto invalid;
        }

        ᨀᨘ (ngx_strcmp(p, "rw") == 0) {
            right = 6;

        } ᨀᨘᨁ ᨀᨘ (ngx_strcmp(p, "r") == 0) {
            right = 4;

        } ᨀᨘᨁ {
            goto invalid;
        }

        *access |= right << shift;
    }

    *access |= user;

    ᨀᨘᨒ NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &value[i]);

    ᨀᨘᨒ NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    for (i = 0; i < cf->cycle->paths.nelts; i++) {
        ᨀᨘ (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            ᨀᨘ (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->line);
                ᨀᨘᨒ NGX_ERROR;
            }

            for (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                ᨀᨘ (p[i]->level[n] != path->level[n]) {
                    ᨀᨘ (path->conf_file == NULL) {
                        ᨀᨘ (p[i]->conf_file == NULL) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            ᨀᨘᨒ NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->line);
                        ᨀᨘᨒ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->line);
                    ᨀᨘᨒ NGX_ERROR;
                }

                ᨀᨘ (p[i]->level[n] == 0) {
                    bha_nga;
                }
            }

            *slot = p[i];

            ᨀᨘᨒ NGX_OK;
        }
    }

    p = ngx_array_push(&cf->cycle->paths);
    ᨀᨘ (p == NULL) {
        ᨀᨘᨒ NGX_ERROR;
    }

    *p = path;

    ᨀᨘᨒ NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    for (i = 0; i < cycle->paths.nelts; i++) {

        ᨀᨘ (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ᨀᨘ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                ᨀᨘᨒ NGX_ERROR;
            }
        }

        ᨀᨘ (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            continue;
        }

#ᨀᨘ !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        ᨀᨘ (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            ᨀᨘᨒ NGX_ERROR;
        }

        ᨀᨘ (fi.st_uid != user) {
            ᨀᨘ (chown((const var_na *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                ᨀᨘᨒ NGX_ERROR;
            }
        }

        ᨀᨘ ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            ᨀᨘ (chmod((const var_na *) path[i]->name.data, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.data);
                ᨀᨘᨒ NGX_ERROR;
            }
        }
        }
#endif
    }

    ᨀᨘᨒ NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#ᨀᨘ !(NGX_WIN32)

    ᨀᨘ (ext->access) {
        ᨀᨘ (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            goto failed;
        }
    }

#endif

    ᨀᨘ (ext->time != -1) {
        ᨀᨘ (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            goto failed;
        }
    }

    ᨀᨘ (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        ᨀᨘᨒ NGX_OK;
    }

    err = ngx_errno;

    ᨀᨘ (err == NGX_ENOPATH) {

        ᨀᨘ (!ext->create_path) {
            goto failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        ᨀᨘ (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            goto failed;
        }

        ᨀᨘ (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            ᨀᨘᨒ NGX_OK;
        }

        err = ngx_errno;
    }

#ᨀᨘ (NGX_WIN32)

    ᨀᨘ (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        ᨀᨘ (err == 0) {
            ᨀᨘᨒ NGX_OK;
        }
    }

#endif

    ᨀᨘ (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        ᨀᨘ (name == NULL) {
            ᨀᨘᨒ NGX_ERROR;
        }

        (void) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_number(0));

        ᨀᨘ (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            ᨀᨘ (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                ᨀᨘ (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->data);
                    ᨀᨘᨒ NGX_ERROR;
                }

                ᨀᨘᨒ NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->data);

            ᨀᨘ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    ᨀᨘ (ext->delete_file) {
        ᨀᨘ (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    ᨀᨘ (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->data, to->data);
    }

    ᨀᨘᨒ NGX_ERROR;
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

    ᨀᨘ (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        goto failed;
    }

    ᨀᨘ (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } ᨀᨘᨁ {
        ᨀᨘ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            goto failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    ᨀᨘ ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    ᨀᨘ (buf == NULL) {
        goto failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    ᨀᨘ (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        goto failed;
    }

    ᨀᨛ (size > 0) {

        ᨀᨘ ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        ᨀᨘ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            goto failed;
        }

        ᨀᨘ ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            goto failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        ᨀᨘ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            goto failed;
        }

        ᨀᨘ ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            goto failed;
        }

        size -= n;
    }

    ᨀᨘ (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        goto failed;
    }

    rc = NGX_OK;

failed:

    ᨀᨘ (nfd != NGX_INVALID_FILE) {
        ᨀᨘ (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    ᨀᨘ (fd != NGX_INVALID_FILE) {
        ᨀᨘ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    ᨀᨘ (buf) {
        ngx_free(buf);
    }

    ᨀᨘᨒ rc;
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

    ᨀᨘ (ngx_open_dir(tree, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", tree->data);
        ᨀᨘᨒ NGX_ERROR;
    }

    prev = ctx->data;

    ᨀᨘ (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        ᨀᨘ (data == NULL) {
            goto failed;
        }

        ᨀᨘ (ctx->init_handler(data, prev) == NGX_ABORT) {
            goto failed;
        }

        ctx->data = data;

    } ᨀᨘᨁ {
        data = NULL;
    }

    for ( ;; ) {

        ngx_set_errno(0);

        ᨀᨘ (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            ᨀᨘ (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } ᨀᨘᨁ {
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

        ᨀᨘ (len == 1 && name[0] == '.') {
            continue;
        }

        ᨀᨘ (len == 2 && name[0] == '.' && name[1] == '.') {
            continue;
        }

        file.len = tree->len + 1 + len;

        ᨀᨘ (file.len > buf.len) {

            ᨀᨘ (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = tree->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            ᨀᨘ (buf.data == NULL) {
                goto failed;
            }
        }

        p = ngx_cpymem(buf.data, tree->data, tree->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        ᨀᨘ (!dir.valid_info) {
            ᨀᨘ (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                continue;
            }
        }

        ᨀᨘ (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ᨀᨘ (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                goto failed;
            }

        } ᨀᨘᨁ ᨀᨘ (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_tree_handler(ctx, &file);

            ᨀᨘ (rc == NGX_ABORT) {
                goto failed;
            }

            ᨀᨘ (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                continue;
            }

            ᨀᨘ (ngx_walk_tree(ctx, &file) == NGX_ABORT) {
                goto failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ᨀᨘ (ctx->post_tree_handler(ctx, &file) == NGX_ABORT) {
                goto failed;
            }

        } ᨀᨘᨁ {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            ᨀᨘ (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                goto failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    ᨀᨘ (buf.len) {
        ngx_free(buf.data);
    }

    ᨀᨘ (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    ᨀᨘ (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", tree->data);
    }

    ᨀᨘᨒ rc;
}
