
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ସ୍ଥିର ngx_int_t ngx_test_full_name(ngx_str_t *name);


ସ୍ଥିର ngx_atomic_t   temp_ସଂଖ୍ୟା = 0;
ngx_atomic_t         *ngx_temp_ସଂଖ୍ୟା = &temp_ସଂଖ୍ୟା;
ngx_atomic_int_t      ngx_random_ସଂଖ୍ୟା = 123456;


ngx_int_t
ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix, ngx_str_t *name)
{
    size_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_full_name(name);

    ଯଦି (rc == NGX_OK) {
        ଫେରନ୍ତୁ rc;
    }

    len = prefix->len;

#ଯଦି (NGX_WIN32)

    ଯଦି (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    ଯଦି (n == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->data, len);
    ngx_cpystrn(p, name->data, name->len + 1);

    name->len += len;
    name->data = n;

    ଫେରନ୍ତୁ NGX_OK;
}


ସ୍ଥିର ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#ଯଦି (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    ଯଦି (name->len < 2) {
        ଯଦି (c0 == '/') {
            ଫେରନ୍ତୁ 2;
        }

        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    c1 = name->data[1];

    ଯଦି (c1 == ':') {
        c0 |= 0x20;

        ଯଦି ((c0 >= 'a' && c0 <= 'z')) {
            ଫେରନ୍ତୁ NGX_OK;
        }

        ଫେରନ୍ତୁ NGX_DECLINED;
    }

    ଯଦି (c1 == '/') {
        ଫେରନ୍ତୁ NGX_OK;
    }

    ଯଦି (c0 == '/') {
        ଫେରନ୍ତୁ 2;
    }

    ଫେରନ୍ତୁ NGX_DECLINED;

#ଅନ୍ୟଥା

    ଯଦି (name->data[0] == '/') {
        ଫେରନ୍ତୁ NGX_OK;
    }

    ଫେରନ୍ତୁ NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    ଯଦି (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        ଯଦି (rc != NGX_OK) {
            ଫେରନ୍ତୁ rc;
        }

        ଯଦି (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#ଯଦି (NGX_THREADS && NGX_HAVE_PWRITEV)

    ଯଦି (tf->thread_write) {
        ଫେରନ୍ତୁ ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    ଫେରନ୍ତୁ ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
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

    ଯଦି (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } ଅନ୍ୟଥା {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.data = ngx_pnalloc(pool, file->name.len + 1);
    ଯଦି (file->name.data == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

#ଯଦି 0
    ପାଇଁ (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    ଯଦି (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_ସଂଖ୍ୟା(0);

    cln = ngx_pool_cleanup_add(pool, ଆକାର(ngx_pool_cleanup_file_t));
    ଯଦି (cln == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ପାଇଁ ( ;; ) {
        (ଶୂନ୍ୟ) ngx_sprintf(p, "%010uD%Z", n);

        ଯଦି (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        ଯଦି (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            ଫେରନ୍ତୁ NGX_OK;
        }

        err = ngx_errno;

        ଯଦି (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_ସଂଖ୍ୟା(1);
            ଜାରି;
        }

        ଯଦି ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (ngx_create_path(file, path) == NGX_ERROR) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }
    }
}


ଶୂନ୍ୟ
ngx_create_hashed_filename(ngx_path_t *path, u_char *file, size_t len)
{
    size_t      i, level;
    ngx_uint_t  n;

    i = path->name.len + 1;

    file[path->name.len + path->len]  = '/';

    ପାଇଁ (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        ଯଦି (level == 0) {
            ଭାଙ୍ଗ;
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

    ପାଇଁ (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        ଯଦି (path->level[i] == 0) {
            ଭାଙ୍ଗ;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        ଯଦି (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ଯଦି (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.data);
                ଫେରନ୍ତୁ NGX_ERROR;
            }
        }

        file->name.data[pos] = '/';
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#ଯଦି (NGX_WIN32)
    p = dir + 3;
#ଅନ୍ୟଥା
    p = dir + 1;
#endif

    ପାଇଁ ( /* void */ ; *p; p++) {
        ch = *p;

        ଯଦି (ch != '/') {
            ଜାରି;
        }

        *p = '\0';

        ଯଦି (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            ବାଛ (err) {
            ସ୍ଥିତି NGX_EEXIST:
                err = 0;
            ସ୍ଥିତି NGX_EACCES:
                ଭାଙ୍ଗ;

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                ଫେରନ୍ତୁ err;
            }
        }

        *p = '/';
    }

    ଫେରନ୍ତୁ err;
}


ngx_atomic_uint_t
ngx_next_temp_ସଂଖ୍ୟା(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_ସଂଖ୍ୟା : 1;

    n = ngx_atomic_fetch_add(ngx_temp_ସଂଖ୍ୟା, add);

    ଫେରନ୍ତୁ n + add;
}


ଅକ୍ଷର *
ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର  *p = conf;

    ssize_t      level;
    ngx_str_t   *ମୂଲ୍ୟ;
    ngx_uint_t   i, n;
    ngx_path_t  *path, **slot;

    slot = (ngx_path_t **) (p + cmd->offset);

    ଯଦି (*slot) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, ଆକାର(ngx_path_t));
    ଯଦି (path == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ମୂଲ୍ୟ = cf->args->elts;

    path->name = ମୂଲ୍ୟ[1];

    ଯଦି (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    ଯଦି (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->ଧାଡ଼ି = cf->conf_file->ଧାଡ଼ି;

    ପାଇଁ (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(ମୂଲ୍ୟ[n].data, ମୂଲ୍ୟ[n].len);
        ଯଦି (level == NGX_ERROR || level == 0) {
            ଫେରନ୍ତୁ "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    ଯଦି (path->len > 10 + i) {
        ଫେରନ୍ତୁ "invalid value";
    }

    *slot = path;

    ଯଦି (ngx_add_path(cf, slot) == NGX_ERROR) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ଅକ୍ଷର *
ngx_conf_merge_path_ମୂଲ୍ୟ(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    ଯଦି (*path) {
        ଫେରନ୍ତୁ NGX_CONF_OK;
    }

    ଯଦି (prev) {
        *path = prev;
        ଫେରନ୍ତୁ NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, ଆକାର(ngx_path_t));
    ଯଦି (*path == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    ଯଦି (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ପାଇଁ (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    ଯଦି (ngx_add_path(cf, path) != NGX_OK) {
        ଫେରନ୍ତୁ NGX_CONF_ERROR;
    }

    ଫେରନ୍ତୁ NGX_CONF_OK;
}


ଅକ୍ଷର *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, ଶୂନ୍ୟ *conf)
{
    ଅକ୍ଷର  *confp = conf;

    u_char      *p;
    ngx_str_t   *ମୂଲ୍ୟ;
    ngx_uint_t   i, ଡାହାଣ, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    ଯଦି (*access != NGX_CONF_UNSET_UINT) {
        ଫେରନ୍ତୁ "is duplicate";
    }

    ମୂଲ୍ୟ = cf->args->elts;

    *access = 0;
    user = 0600;

    ପାଇଁ (i = 1; i < cf->args->nelts; i++) {

        p = ମୂଲ୍ୟ[i].data;

        ଯଦି (ngx_strncmp(p, "user:", ଆକାର("user:") - 1) == 0) {
            shift = 6;
            p += ଆକାର("user:") - 1;
            user = 0;

        } ଅନ୍ୟଥା ଯଦି (ngx_strncmp(p, "group:", ଆକାର("group:") - 1) == 0) {
            shift = 3;
            p += ଆକାର("group:") - 1;

        } ଅନ୍ୟଥା ଯଦି (ngx_strncmp(p, "all:", ଆକାର("all:") - 1) == 0) {
            shift = 0;
            p += ଆକାର("all:") - 1;

        } ଅନ୍ୟଥା {
            ଯାଅ invalid;
        }

        ଯଦି (ngx_strcmp(p, "rw") == 0) {
            ଡାହାଣ = 6;

        } ଅନ୍ୟଥା ଯଦି (ngx_strcmp(p, "r") == 0) {
            ଡାହାଣ = 4;

        } ଅନ୍ୟଥା {
            ଯାଅ invalid;
        }

        *access |= ଡାହାଣ << shift;
    }

    *access |= user;

    ଫେରନ୍ତୁ NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &ମୂଲ୍ୟ[i]);

    ଫେରନ୍ତୁ NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    ପାଇଁ (i = 0; i < cf->cycle->paths.nelts; i++) {
        ଯଦି (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            ଯଦି (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->ଧାଡ଼ି);
                ଫେରନ୍ତୁ NGX_ERROR;
            }

            ପାଇଁ (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                ଯଦି (p[i]->level[n] != path->level[n]) {
                    ଯଦି (path->conf_file == ଖାଲି) {
                        ଯଦି (p[i]->conf_file == ଖାଲି) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            ଫେରନ୍ତୁ NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->ଧାଡ଼ି);
                        ଫେରନ୍ତୁ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->ଧାଡ଼ି);
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                ଯଦି (p[i]->level[n] == 0) {
                    ଭାଙ୍ଗ;
                }
            }

            *slot = p[i];

            ଫେରନ୍ତୁ NGX_OK;
        }
    }

    p = ngx_array_ଠେଲ(&cf->cycle->paths);
    ଯଦି (p == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    *p = path;

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    ପାଇଁ (i = 0; i < cycle->paths.nelts; i++) {

        ଯଦି (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ଯଦି (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                ଫେରନ୍ତୁ NGX_ERROR;
            }
        }

        ଯଦି (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ଜାରି;
        }

#ଯଦି !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        ଯଦି (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        ଯଦି (fi.st_uid != user) {
            ଯଦି (chown((ଅଚଳ ଅକ୍ଷର *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                ଫେରନ୍ତୁ NGX_ERROR;
            }
        }

        ଯଦି ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            ଯଦି (chmod((ଅଚଳ ଅକ୍ଷର *) path[i]->name.data, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.data);
                ଫେରନ୍ତୁ NGX_ERROR;
            }
        }
        }
#endif
    }

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#ଯଦି !(NGX_WIN32)

    ଯଦି (ext->access) {
        ଯଦି (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            ଯାଅ failed;
        }
    }

#endif

    ଯଦି (ext->time != -1) {
        ଯଦି (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            ଯାଅ failed;
        }
    }

    ଯଦି (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    err = ngx_errno;

    ଯଦି (err == NGX_ENOPATH) {

        ଯଦି (!ext->create_path) {
            ଯାଅ failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        ଯଦି (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            ଯାଅ failed;
        }

        ଯଦି (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            ଫେରନ୍ତୁ NGX_OK;
        }

        err = ngx_errno;
    }

#ଯଦି (NGX_WIN32)

    ଯଦି (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        ଯଦି (err == 0) {
            ଫେରନ୍ତୁ NGX_OK;
        }
    }

#endif

    ଯଦି (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        ଯଦି (name == ଖାଲି) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        (ଶୂନ୍ୟ) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_ସଂଖ୍ୟା(0));

        ଯଦି (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            ଯଦି (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                ଯଦି (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->data);
                    ଫେରନ୍ତୁ NGX_ERROR;
                }

                ଫେରନ୍ତୁ NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->data);

            ଯଦି (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    ଯଦି (ext->delete_file) {
        ଯଦି (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    ଯଦି (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->data, to->data);
    }

    ଫେରନ୍ତୁ NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf)
{
    ଅକ୍ଷର             *buf;
    off_t             size;
    time_t            time;
    size_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = ଖାଲି;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    ଯଦି (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        ଯାଅ failed;
    }

    ଯଦି (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } ଅନ୍ୟଥା {
        ଯଦି (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            ଯାଅ failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    ଯଦି ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    ଯଦି (buf == ଖାଲି) {
        ଯାଅ failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    ଯଦି (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        ଯାଅ failed;
    }

    ଯେତେବେଳେ (size > 0) {

        ଯଦି ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        ଯଦି (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            ଯାଅ failed;
        }

        ଯଦି ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            ଯାଅ failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        ଯଦି (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            ଯାଅ failed;
        }

        ଯଦି ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            ଯାଅ failed;
        }

        size -= n;
    }

    ଯଦି (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        ଯାଅ failed;
    }

    rc = NGX_OK;

failed:

    ଯଦି (nfd != NGX_INVALID_FILE) {
        ଯଦି (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    ଯଦି (fd != NGX_INVALID_FILE) {
        ଯଦି (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    ଯଦି (buf) {
        ngx_free(buf);
    }

    ଫେରନ୍ତୁ rc;
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
ngx_walk_ବୃକ୍ଷ(ngx_ବୃକ୍ଷ_ctx_t *ctx, ngx_str_t *ବୃକ୍ଷ)
{
    ଶୂନ୍ୟ       *data, *prev;
    u_char     *p, *name;
    size_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                   "walk tree \"%V\"", ବୃକ୍ଷ);

    ଯଦି (ngx_open_dir(ବୃକ୍ଷ, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", ବୃକ୍ଷ->data);
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    prev = ctx->data;

    ଯଦି (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        ଯଦି (data == ଖାଲି) {
            ଯାଅ failed;
        }

        ଯଦି (ctx->init_handler(data, prev) == NGX_ABORT) {
            ଯାଅ failed;
        }

        ctx->data = data;

    } ଅନ୍ୟଥା {
        data = ଖାଲି;
    }

    ପାଇଁ ( ;; ) {

        ngx_set_errno(0);

        ଯଦି (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            ଯଦି (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } ଅନ୍ୟଥା {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, err,
                              ngx_read_dir_n " \"%s\" failed", ବୃକ୍ଷ->data);
                rc = NGX_ERROR;
            }

            ଯାଅ done;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                      "tree name %uz:\"%s\"", len, name);

        ଯଦି (len == 1 && name[0] == '.') {
            ଜାରି;
        }

        ଯଦି (len == 2 && name[0] == '.' && name[1] == '.') {
            ଜାରି;
        }

        file.len = ବୃକ୍ଷ->len + 1 + len;

        ଯଦି (file.len > buf.len) {

            ଯଦି (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = ବୃକ୍ଷ->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            ଯଦି (buf.data == ଖାଲି) {
                ଯାଅ failed;
            }
        }

        p = ngx_cpymem(buf.data, ବୃକ୍ଷ->data, ବୃକ୍ଷ->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        ଯଦି (!dir.valid_info) {
            ଯଦି (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                ଜାରି;
            }
        }

        ଯଦି (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ଯଦି (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                ଯାଅ failed;
            }

        } ଅନ୍ୟଥା ଯଦି (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_ବୃକ୍ଷ_handler(ctx, &file);

            ଯଦି (rc == NGX_ABORT) {
                ଯାଅ failed;
            }

            ଯଦି (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                ଜାରି;
            }

            ଯଦି (ngx_walk_ବୃକ୍ଷ(ctx, &file) == NGX_ABORT) {
                ଯାଅ failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ଯଦି (ctx->post_ବୃକ୍ଷ_handler(ctx, &file) == NGX_ABORT) {
                ଯାଅ failed;
            }

        } ଅନ୍ୟଥା {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            ଯଦି (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                ଯାଅ failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    ଯଦି (buf.len) {
        ngx_free(buf.data);
    }

    ଯଦି (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    ଯଦି (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", ବୃକ୍ଷ->data);
    }

    ଫେରନ୍ତୁ rc;
}
