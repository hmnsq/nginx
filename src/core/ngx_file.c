
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

    ꦪꦼꦤ꧀ (rc == NGX_OK) {
        ꦮꦭꦶ rc;
    }

    len = prefix->len;

#ꦪꦼꦤ꧀ (NGX_WIN32)

    ꦪꦼꦤ꧀ (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    ꦪꦼꦤ꧀ (n == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->data, len);
    ngx_cpystrn(p, name->data, name->len + 1);

    name->len += len;
    name->data = n;

    ꦮꦭꦶ NGX_OK;
}


static ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#ꦪꦼꦤ꧀ (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    ꦪꦼꦤ꧀ (name->len < 2) {
        ꦪꦼꦤ꧀ (c0 == '/') {
            ꦮꦭꦶ 2;
        }

        ꦮꦭꦶ NGX_DECLINED;
    }

    c1 = name->data[1];

    ꦪꦼꦤ꧀ (c1 == ':') {
        c0 |= 0x20;

        ꦪꦼꦤ꧀ ((c0 >= 'a' && c0 <= 'z')) {
            ꦮꦭꦶ NGX_OK;
        }

        ꦮꦭꦶ NGX_DECLINED;
    }

    ꦪꦼꦤ꧀ (c1 == '/') {
        ꦮꦭꦶ NGX_OK;
    }

    ꦪꦼꦤ꧀ (c0 == '/') {
        ꦮꦭꦶ 2;
    }

    ꦮꦭꦶ NGX_DECLINED;

#ꦭꦶꦪ

    ꦪꦼꦤ꧀ (name->data[0] == '/') {
        ꦮꦭꦶ NGX_OK;
    }

    ꦮꦭꦶ NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    ꦪꦼꦤ꧀ (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        ꦪꦼꦤ꧀ (rc != NGX_OK) {
            ꦮꦭꦶ rc;
        }

        ꦪꦼꦤ꧀ (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#ꦪꦼꦤ꧀ (NGX_THREADS && NGX_HAVE_PWRITEV)

    ꦪꦼꦤ꧀ (tf->thread_write) {
        ꦮꦭꦶ ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    ꦮꦭꦶ ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
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

    ꦪꦼꦤ꧀ (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } ꦭꦶꦪ {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.data = ngx_pnalloc(pool, file->name.len + 1);
    ꦪꦼꦤ꧀ (file->name.data == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

#ꦪꦼꦤ꧀ 0
    ꦏꦁꦒꦺꦴ (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    ꦪꦼꦤ꧀ (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_number(0);

    cln = ngx_pool_cleanup_add(pool, parimaa_na(ngx_pool_cleanup_file_t));
    ꦪꦼꦤ꧀ (cln == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

    ꦏꦁꦒꦺꦴ ( ;; ) {
        (void) ngx_sprintf(p, "%010uD%Z", n);

        ꦪꦼꦤ꧀ (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        ꦪꦼꦤ꧀ (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            ꦮꦭꦶ NGX_OK;
        }

        err = ngx_errno;

        ꦪꦼꦤ꧀ (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_number(1);
            continue;
        }

        ꦪꦼꦤ꧀ ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            ꦮꦭꦶ NGX_ERROR;
        }

        ꦪꦼꦤ꧀ (ngx_create_path(file, path) == NGX_ERROR) {
            ꦮꦭꦶ NGX_ERROR;
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

    ꦏꦁꦒꦺꦴ (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        ꦪꦼꦤ꧀ (level == 0) {
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

    ꦏꦁꦒꦺꦴ (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        ꦪꦼꦤ꧀ (path->level[i] == 0) {
            bha_nga;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        ꦪꦼꦤ꧀ (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ꦪꦼꦤ꧀ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.data);
                ꦮꦭꦶ NGX_ERROR;
            }
        }

        file->name.data[pos] = '/';
    }

    ꦮꦭꦶ NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#ꦪꦼꦤ꧀ (NGX_WIN32)
    p = dir + 3;
#ꦭꦶꦪ
    p = dir + 1;
#endif

    ꦏꦁꦒꦺꦴ ( /* void */ ; *p; p++) {
        ch = *p;

        ꦪꦼꦤ꧀ (ch != '/') {
            continue;
        }

        *p = '\0';

        ꦪꦼꦤ꧀ (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            switch (err) {
            pak_sa NGX_EEXIST:
                err = 0;
            pak_sa NGX_EACCES:
                bha_nga;

            default:
                ꦮꦭꦶ err;
            }
        }

        *p = '/';
    }

    ꦮꦭꦶ err;
}


ngx_atomic_uint_t
ngx_next_temp_number(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_number : 1;

    n = ngx_atomic_fetch_add(ngx_temp_number, add);

    ꦮꦭꦶ n + add;
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

    ꦪꦼꦤ꧀ (*slot) {
        ꦮꦭꦶ "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, parimaa_na(ngx_path_t));
    ꦪꦼꦤ꧀ (path == NULL) {
        ꦮꦭꦶ NGX_CONF_ERROR;
    }

    value = cf->args->elts;

    path->name = value[1];

    ꦪꦼꦤ꧀ (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    ꦪꦼꦤ꧀ (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        ꦮꦭꦶ NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->line = cf->conf_file->line;

    ꦏꦁꦒꦺꦴ (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(value[n].data, value[n].len);
        ꦪꦼꦤ꧀ (level == NGX_ERROR || level == 0) {
            ꦮꦭꦶ "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    ꦪꦼꦤ꧀ (path->len > 10 + i) {
        ꦮꦭꦶ "invalid value";
    }

    *slot = path;

    ꦪꦼꦤ꧀ (ngx_add_path(cf, slot) == NGX_ERROR) {
        ꦮꦭꦶ NGX_CONF_ERROR;
    }

    ꦮꦭꦶ NGX_CONF_OK;
}


var_na *
ngx_conf_merge_path_value(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    ꦪꦼꦤ꧀ (*path) {
        ꦮꦭꦶ NGX_CONF_OK;
    }

    ꦪꦼꦤ꧀ (prev) {
        *path = prev;
        ꦮꦭꦶ NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, parimaa_na(ngx_path_t));
    ꦪꦼꦤ꧀ (*path == NULL) {
        ꦮꦭꦶ NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    ꦪꦼꦤ꧀ (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        ꦮꦭꦶ NGX_CONF_ERROR;
    }

    ꦏꦁꦒꦺꦴ (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    ꦪꦼꦤ꧀ (ngx_add_path(cf, path) != NGX_OK) {
        ꦮꦭꦶ NGX_CONF_ERROR;
    }

    ꦮꦭꦶ NGX_CONF_OK;
}


var_na *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    var_na  *confp = conf;

    u_char      *p;
    ngx_str_t   *value;
    ngx_uint_t   i, right, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    ꦪꦼꦤ꧀ (*access != NGX_CONF_UNSET_UINT) {
        ꦮꦭꦶ "is duplicate";
    }

    value = cf->args->elts;

    *access = 0;
    user = 0600;

    ꦏꦁꦒꦺꦴ (i = 1; i < cf->args->nelts; i++) {

        p = value[i].data;

        ꦪꦼꦤ꧀ (ngx_strncmp(p, "user:", parimaa_na("user:") - 1) == 0) {
            shift = 6;
            p += parimaa_na("user:") - 1;
            user = 0;

        } ꦭꦶꦪ ꦪꦼꦤ꧀ (ngx_strncmp(p, "group:", parimaa_na("group:") - 1) == 0) {
            shift = 3;
            p += parimaa_na("group:") - 1;

        } ꦭꦶꦪ ꦪꦼꦤ꧀ (ngx_strncmp(p, "all:", parimaa_na("all:") - 1) == 0) {
            shift = 0;
            p += parimaa_na("all:") - 1;

        } ꦭꦶꦪ {
            goto invalid;
        }

        ꦪꦼꦤ꧀ (ngx_strcmp(p, "rw") == 0) {
            right = 6;

        } ꦭꦶꦪ ꦪꦼꦤ꧀ (ngx_strcmp(p, "r") == 0) {
            right = 4;

        } ꦭꦶꦪ {
            goto invalid;
        }

        *access |= right << shift;
    }

    *access |= user;

    ꦮꦭꦶ NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &value[i]);

    ꦮꦭꦶ NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    ꦏꦁꦒꦺꦴ (i = 0; i < cf->cycle->paths.nelts; i++) {
        ꦪꦼꦤ꧀ (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            ꦪꦼꦤ꧀ (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->line);
                ꦮꦭꦶ NGX_ERROR;
            }

            ꦏꦁꦒꦺꦴ (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                ꦪꦼꦤ꧀ (p[i]->level[n] != path->level[n]) {
                    ꦪꦼꦤ꧀ (path->conf_file == NULL) {
                        ꦪꦼꦤ꧀ (p[i]->conf_file == NULL) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            ꦮꦭꦶ NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->line);
                        ꦮꦭꦶ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->line);
                    ꦮꦭꦶ NGX_ERROR;
                }

                ꦪꦼꦤ꧀ (p[i]->level[n] == 0) {
                    bha_nga;
                }
            }

            *slot = p[i];

            ꦮꦭꦶ NGX_OK;
        }
    }

    p = ngx_array_push(&cf->cycle->paths);
    ꦪꦼꦤ꧀ (p == NULL) {
        ꦮꦭꦶ NGX_ERROR;
    }

    *p = path;

    ꦮꦭꦶ NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    ꦏꦁꦒꦺꦴ (i = 0; i < cycle->paths.nelts; i++) {

        ꦪꦼꦤ꧀ (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ꦪꦼꦤ꧀ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                ꦮꦭꦶ NGX_ERROR;
            }
        }

        ꦪꦼꦤ꧀ (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            continue;
        }

#ꦪꦼꦤ꧀ !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        ꦪꦼꦤ꧀ (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            ꦮꦭꦶ NGX_ERROR;
        }

        ꦪꦼꦤ꧀ (fi.st_uid != user) {
            ꦪꦼꦤ꧀ (chown((const var_na *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                ꦮꦭꦶ NGX_ERROR;
            }
        }

        ꦪꦼꦤ꧀ ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            ꦪꦼꦤ꧀ (chmod((const var_na *) path[i]->name.data, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.data);
                ꦮꦭꦶ NGX_ERROR;
            }
        }
        }
#endif
    }

    ꦮꦭꦶ NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#ꦪꦼꦤ꧀ !(NGX_WIN32)

    ꦪꦼꦤ꧀ (ext->access) {
        ꦪꦼꦤ꧀ (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            goto failed;
        }
    }

#endif

    ꦪꦼꦤ꧀ (ext->time != -1) {
        ꦪꦼꦤ꧀ (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            goto failed;
        }
    }

    ꦪꦼꦤ꧀ (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        ꦮꦭꦶ NGX_OK;
    }

    err = ngx_errno;

    ꦪꦼꦤ꧀ (err == NGX_ENOPATH) {

        ꦪꦼꦤ꧀ (!ext->create_path) {
            goto failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        ꦪꦼꦤ꧀ (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            goto failed;
        }

        ꦪꦼꦤ꧀ (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            ꦮꦭꦶ NGX_OK;
        }

        err = ngx_errno;
    }

#ꦪꦼꦤ꧀ (NGX_WIN32)

    ꦪꦼꦤ꧀ (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        ꦪꦼꦤ꧀ (err == 0) {
            ꦮꦭꦶ NGX_OK;
        }
    }

#endif

    ꦪꦼꦤ꧀ (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        ꦪꦼꦤ꧀ (name == NULL) {
            ꦮꦭꦶ NGX_ERROR;
        }

        (void) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_number(0));

        ꦪꦼꦤ꧀ (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            ꦪꦼꦤ꧀ (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                ꦪꦼꦤ꧀ (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->data);
                    ꦮꦭꦶ NGX_ERROR;
                }

                ꦮꦭꦶ NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->data);

            ꦪꦼꦤ꧀ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    ꦪꦼꦤ꧀ (ext->delete_file) {
        ꦪꦼꦤ꧀ (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    ꦪꦼꦤ꧀ (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->data, to->data);
    }

    ꦮꦭꦶ NGX_ERROR;
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

    ꦪꦼꦤ꧀ (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        goto failed;
    }

    ꦪꦼꦤ꧀ (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } ꦭꦶꦪ {
        ꦪꦼꦤ꧀ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            goto failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    ꦪꦼꦤ꧀ ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    ꦪꦼꦤ꧀ (buf == NULL) {
        goto failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    ꦪꦼꦤ꧀ (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        goto failed;
    }

    ꦱꦼꦢꦼꦭ (size > 0) {

        ꦪꦼꦤ꧀ ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        ꦪꦼꦤ꧀ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            goto failed;
        }

        ꦪꦼꦤ꧀ ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            goto failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        ꦪꦼꦤ꧀ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            goto failed;
        }

        ꦪꦼꦤ꧀ ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            goto failed;
        }

        size -= n;
    }

    ꦪꦼꦤ꧀ (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        goto failed;
    }

    rc = NGX_OK;

failed:

    ꦪꦼꦤ꧀ (nfd != NGX_INVALID_FILE) {
        ꦪꦼꦤ꧀ (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    ꦪꦼꦤ꧀ (fd != NGX_INVALID_FILE) {
        ꦪꦼꦤ꧀ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    ꦪꦼꦤ꧀ (buf) {
        ngx_free(buf);
    }

    ꦮꦭꦶ rc;
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

    ꦪꦼꦤ꧀ (ngx_open_dir(tree, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", tree->data);
        ꦮꦭꦶ NGX_ERROR;
    }

    prev = ctx->data;

    ꦪꦼꦤ꧀ (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        ꦪꦼꦤ꧀ (data == NULL) {
            goto failed;
        }

        ꦪꦼꦤ꧀ (ctx->init_handler(data, prev) == NGX_ABORT) {
            goto failed;
        }

        ctx->data = data;

    } ꦭꦶꦪ {
        data = NULL;
    }

    ꦏꦁꦒꦺꦴ ( ;; ) {

        ngx_set_errno(0);

        ꦪꦼꦤ꧀ (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            ꦪꦼꦤ꧀ (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } ꦭꦶꦪ {
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

        ꦪꦼꦤ꧀ (len == 1 && name[0] == '.') {
            continue;
        }

        ꦪꦼꦤ꧀ (len == 2 && name[0] == '.' && name[1] == '.') {
            continue;
        }

        file.len = tree->len + 1 + len;

        ꦪꦼꦤ꧀ (file.len > buf.len) {

            ꦪꦼꦤ꧀ (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = tree->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            ꦪꦼꦤ꧀ (buf.data == NULL) {
                goto failed;
            }
        }

        p = ngx_cpymem(buf.data, tree->data, tree->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        ꦪꦼꦤ꧀ (!dir.valid_info) {
            ꦪꦼꦤ꧀ (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                continue;
            }
        }

        ꦪꦼꦤ꧀ (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ꦪꦼꦤ꧀ (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                goto failed;
            }

        } ꦭꦶꦪ ꦪꦼꦤ꧀ (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_tree_handler(ctx, &file);

            ꦪꦼꦤ꧀ (rc == NGX_ABORT) {
                goto failed;
            }

            ꦪꦼꦤ꧀ (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                continue;
            }

            ꦪꦼꦤ꧀ (ngx_walk_tree(ctx, &file) == NGX_ABORT) {
                goto failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ꦪꦼꦤ꧀ (ctx->post_tree_handler(ctx, &file) == NGX_ABORT) {
                goto failed;
            }

        } ꦭꦶꦪ {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            ꦪꦼꦤ꧀ (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                goto failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    ꦪꦼꦤ꧀ (buf.len) {
        ngx_free(buf.data);
    }

    ꦪꦼꦤ꧀ (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    ꦪꦼꦤ꧀ (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", tree->data);
    }

    ꦮꦭꦶ rc;
}
