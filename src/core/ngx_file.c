
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


સ્થિર ngx_int_t ngx_test_full_name(ngx_str_t *name);


સ્થિર ngx_atomic_t   temp_સંખ્યા = 0;
ngx_atomic_t         *ngx_temp_સંખ્યા = &temp_સંખ્યા;
ngx_atomic_int_t      ngx_random_સંખ્યા = 123456;


ngx_int_t
ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix, ngx_str_t *name)
{
    size_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_full_name(name);

    જો (rc == NGX_OK) {
        પાછા rc;
    }

    len = prefix->len;

#જો (NGX_WIN32)

    જો (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    જો (n == ખાલી) {
        પાછા NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->data, len);
    ngx_cpystrn(p, name->data, name->len + 1);

    name->len += len;
    name->data = n;

    પાછા NGX_OK;
}


સ્થિર ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#જો (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->data[0];

    જો (name->len < 2) {
        જો (c0 == '/') {
            પાછા 2;
        }

        પાછા NGX_DECLINED;
    }

    c1 = name->data[1];

    જો (c1 == ':') {
        c0 |= 0x20;

        જો ((c0 >= 'a' && c0 <= 'z')) {
            પાછા NGX_OK;
        }

        પાછા NGX_DECLINED;
    }

    જો (c1 == '/') {
        પાછા NGX_OK;
    }

    જો (c0 == '/') {
        પાછા 2;
    }

    પાછા NGX_DECLINED;

#નહીંતર

    જો (name->data[0] == '/') {
        પાછા NGX_OK;
    }

    પાછા NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_temp_file(ngx_temp_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    જો (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_temp_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        જો (rc != NGX_OK) {
            પાછા rc;
        }

        જો (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#જો (NGX_THREADS && NGX_HAVE_PWRITEV)

    જો (tf->thread_write) {
        પાછા ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    પાછા ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
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

    જો (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } નહીંતર {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.data = ngx_pnalloc(pool, file->name.len + 1);
    જો (file->name.data == ખાલી) {
        પાછા NGX_ERROR;
    }

#જો 0
    માટે (i = 0; i < file->name.len; i++) {
        file->name.data[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.data, name.data, name.len);

    જો (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_next_temp_સંખ્યા(0);

    cln = ngx_pool_cleanup_add(pool, કદ(ngx_pool_cleanup_file_t));
    જો (cln == ખાલી) {
        પાછા NGX_ERROR;
    }

    માટે ( ;; ) {
        (શૂન્ય) ngx_sprintf(p, "%010uD%Z", n);

        જો (!prefix) {
            ngx_create_hashed_filename(path, file->name.data, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.data);

        file->fd = ngx_open_tempfile(file->name.data, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        જો (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->data;

            clnf->fd = file->fd;
            clnf->name = file->name.data;
            clnf->log = pool->log;

            પાછા NGX_OK;
        }

        err = ngx_errno;

        જો (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_next_temp_સંખ્યા(1);
            ચાલુ;
        }

        જો ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.data);
            પાછા NGX_ERROR;
        }

        જો (ngx_create_path(file, path) == NGX_ERROR) {
            પાછા NGX_ERROR;
        }
    }
}


શૂન્ય
ngx_create_hashed_filename(ngx_path_t *path, u_char *file, size_t len)
{
    size_t      i, level;
    ngx_uint_t  n;

    i = path->name.len + 1;

    file[path->name.len + path->len]  = '/';

    માટે (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        જો (level == 0) {
            તોડો;
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

    માટે (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        જો (path->level[i] == 0) {
            તોડો;
        }

        pos += path->level[i] + 1;

        file->name.data[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.data);

        જો (ngx_create_dir(file->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            જો (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.data);
                પાછા NGX_ERROR;
            }
        }

        file->name.data[pos] = '/';
    }

    પાછા NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#જો (NGX_WIN32)
    p = dir + 3;
#નહીંતર
    p = dir + 1;
#endif

    માટે ( /* void */ ; *p; p++) {
        ch = *p;

        જો (ch != '/') {
            ચાલુ;
        }

        *p = '\0';

        જો (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            પસંદગી (err) {
            સ્થિતિ NGX_EEXIST:
                err = 0;
            સ્થિતિ NGX_EACCES:
                તોડો;

            મૂળભૂત:
                પાછા err;
            }
        }

        *p = '/';
    }

    પાછા err;
}


ngx_atomic_uint_t
ngx_next_temp_સંખ્યા(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_સંખ્યા : 1;

    n = ngx_atomic_fetch_add(ngx_temp_સંખ્યા, add);

    પાછા n + add;
}


અક્ષર *
ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર  *p = conf;

    ssize_t      level;
    ngx_str_t   *કિંમત;
    ngx_uint_t   i, n;
    ngx_path_t  *path, **slot;

    slot = (ngx_path_t **) (p + cmd->offset);

    જો (*slot) {
        પાછા "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, કદ(ngx_path_t));
    જો (path == ખાલી) {
        પાછા NGX_CONF_ERROR;
    }

    કિંમત = cf->args->elts;

    path->name = કિંમત[1];

    જો (path->name.data[path->name.len - 1] == '/') {
        path->name.len--;
    }

    જો (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        પાછા NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.data;
    path->લીટી = cf->conf_file->લીટી;

    માટે (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(કિંમત[n].data, કિંમત[n].len);
        જો (level == NGX_ERROR || level == 0) {
            પાછા "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    જો (path->len > 10 + i) {
        પાછા "invalid value";
    }

    *slot = path;

    જો (ngx_add_path(cf, slot) == NGX_ERROR) {
        પાછા NGX_CONF_ERROR;
    }

    પાછા NGX_CONF_OK;
}


અક્ષર *
ngx_conf_merge_path_કિંમત(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *prev,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    જો (*path) {
        પાછા NGX_CONF_OK;
    }

    જો (prev) {
        *path = prev;
        પાછા NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, કદ(ngx_path_t));
    જો (*path == ખાલી) {
        પાછા NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    જો (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        પાછા NGX_CONF_ERROR;
    }

    માટે (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    જો (ngx_add_path(cf, path) != NGX_OK) {
        પાછા NGX_CONF_ERROR;
    }

    પાછા NGX_CONF_OK;
}


અક્ષર *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, શૂન્ય *conf)
{
    અક્ષર  *confp = conf;

    u_char      *p;
    ngx_str_t   *કિંમત;
    ngx_uint_t   i, જમણું, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    જો (*access != NGX_CONF_UNSET_UINT) {
        પાછા "is duplicate";
    }

    કિંમત = cf->args->elts;

    *access = 0;
    user = 0600;

    માટે (i = 1; i < cf->args->nelts; i++) {

        p = કિંમત[i].data;

        જો (ngx_strncmp(p, "user:", કદ("user:") - 1) == 0) {
            shift = 6;
            p += કદ("user:") - 1;
            user = 0;

        } નહીંતર જો (ngx_strncmp(p, "group:", કદ("group:") - 1) == 0) {
            shift = 3;
            p += કદ("group:") - 1;

        } નહીંતર જો (ngx_strncmp(p, "all:", કદ("all:") - 1) == 0) {
            shift = 0;
            p += કદ("all:") - 1;

        } નહીંતર {
            જાઓ invalid;
        }

        જો (ngx_strcmp(p, "rw") == 0) {
            જમણું = 6;

        } નહીંતર જો (ngx_strcmp(p, "r") == 0) {
            જમણું = 4;

        } નહીંતર {
            જાઓ invalid;
        }

        *access |= જમણું << shift;
    }

    *access |= user;

    પાછા NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &કિંમત[i]);

    પાછા NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    માટે (i = 0; i < cf->cycle->paths.nelts; i++) {
        જો (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.data, path->name.data) == 0)
        {
            જો (p[i]->data != path->data) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->લીટી);
                પાછા NGX_ERROR;
            }

            માટે (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                જો (p[i]->level[n] != path->level[n]) {
                    જો (path->conf_file == ખાલી) {
                        જો (p[i]->conf_file == ખાલી) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            પાછા NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->લીટી);
                        પાછા NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->લીટી);
                    પાછા NGX_ERROR;
                }

                જો (p[i]->level[n] == 0) {
                    તોડો;
                }
            }

            *slot = p[i];

            પાછા NGX_OK;
        }
    }

    p = ngx_array_ધકેલો(&cf->cycle->paths);
    જો (p == ખાલી) {
        પાછા NGX_ERROR;
    }

    *p = path;

    પાછા NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    માટે (i = 0; i < cycle->paths.nelts; i++) {

        જો (ngx_create_dir(path[i]->name.data, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            જો (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.data);
                પાછા NGX_ERROR;
            }
        }

        જો (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ચાલુ;
        }

#જો !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        જો (ngx_file_info(path[i]->name.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.data);
            પાછા NGX_ERROR;
        }

        જો (fi.st_uid != user) {
            જો (chown((અચલ અક્ષર *) path[i]->name.data, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.data, user);
                પાછા NGX_ERROR;
            }
        }

        જો ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            જો (chmod((અચલ અક્ષર *) path[i]->name.data, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.data);
                પાછા NGX_ERROR;
            }
        }
        }
#endif
    }

    પાછા NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#જો !(NGX_WIN32)

    જો (ext->access) {
        જો (ngx_change_file_access(src->data, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->data);
            err = 0;
            જાઓ failed;
        }
    }

#endif

    જો (ext->time != -1) {
        જો (ngx_set_file_time(src->data, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->data);
            err = 0;
            જાઓ failed;
        }
    }

    જો (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
        પાછા NGX_OK;
    }

    err = ngx_errno;

    જો (err == NGX_ENOPATH) {

        જો (!ext->create_path) {
            જાઓ failed;
        }

        err = ngx_create_full_path(to->data, ngx_dir_access(ext->path_access));

        જો (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->data);
            err = 0;
            જાઓ failed;
        }

        જો (ngx_rename_file(src->data, to->data) != NGX_FILE_ERROR) {
            પાછા NGX_OK;
        }

        err = ngx_errno;
    }

#જો (NGX_WIN32)

    જો (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        જો (err == 0) {
            પાછા NGX_OK;
        }
    }

#endif

    જો (err == NGX_EXDEV) {

        cf.size = -1;
        cf.buf_size = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        જો (name == ખાલી) {
            પાછા NGX_ERROR;
        }

        (શૂન્ય) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->data,
                           (uint32_t) ngx_next_temp_સંખ્યા(0));

        જો (ngx_copy_file(src->data, name, &cf) == NGX_OK) {

            જો (ngx_rename_file(name, to->data) != NGX_FILE_ERROR) {
                ngx_free(name);

                જો (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->data);
                    પાછા NGX_ERROR;
                }

                પાછા NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->data);

            જો (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    જો (ext->delete_file) {
        જો (ngx_delete_file(src->data) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->data);
        }
    }

    જો (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->data, to->data);
    }

    પાછા NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf)
{
    અક્ષર             *buf;
    off_t             size;
    time_t            time;
    size_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = ખાલી;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    જો (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        જાઓ failed;
    }

    જો (cf->size != -1 && cf->access != 0 && cf->time != -1) {
        size = cf->size;
        access = cf->access;
        time = cf->time;

    } નહીંતર {
        જો (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            જાઓ failed;
        }

        size = (cf->size != -1) ? cf->size : ngx_file_size(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_size ? cf->buf_size : 65536;

    જો ((off_t) len > size) {
        len = (size_t) size;
    }

    buf = ngx_alloc(len, cf->log);
    જો (buf == ખાલી) {
        જાઓ failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    જો (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        જાઓ failed;
    }

    જ્યારે (size > 0) {

        જો ((off_t) len > size) {
            len = (size_t) size;
        }

        n = ngx_read_fd(fd, buf, len);

        જો (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            જાઓ failed;
        }

        જો ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, size, from);
            જાઓ failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        જો (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            જાઓ failed;
        }

        જો ((size_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, size, to);
            જાઓ failed;
        }

        size -= n;
    }

    જો (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        જાઓ failed;
    }

    rc = NGX_OK;

failed:

    જો (nfd != NGX_INVALID_FILE) {
        જો (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    જો (fd != NGX_INVALID_FILE) {
        જો (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    જો (buf) {
        ngx_free(buf);
    }

    પાછા rc;
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
ngx_walk_વૃક્ષ(ngx_વૃક્ષ_ctx_t *ctx, ngx_str_t *વૃક્ષ)
{
    શૂન્ય       *data, *prev;
    u_char     *p, *name;
    size_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                   "walk tree \"%V\"", વૃક્ષ);

    જો (ngx_open_dir(વૃક્ષ, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", વૃક્ષ->data);
        પાછા NGX_ERROR;
    }

    prev = ctx->data;

    જો (ctx->alloc) {
        data = ngx_alloc(ctx->alloc, ctx->log);
        જો (data == ખાલી) {
            જાઓ failed;
        }

        જો (ctx->init_handler(data, prev) == NGX_ABORT) {
            જાઓ failed;
        }

        ctx->data = data;

    } નહીંતર {
        data = ખાલી;
    }

    માટે ( ;; ) {

        ngx_set_errno(0);

        જો (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            જો (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } નહીંતર {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, err,
                              ngx_read_dir_n " \"%s\" failed", વૃક્ષ->data);
                rc = NGX_ERROR;
            }

            જાઓ done;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                      "tree name %uz:\"%s\"", len, name);

        જો (len == 1 && name[0] == '.') {
            ચાલુ;
        }

        જો (len == 2 && name[0] == '.' && name[1] == '.') {
            ચાલુ;
        }

        file.len = વૃક્ષ->len + 1 + len;

        જો (file.len > buf.len) {

            જો (buf.len) {
                ngx_free(buf.data);
            }

            buf.len = વૃક્ષ->len + 1 + len;

            buf.data = ngx_alloc(buf.len + 1, ctx->log);
            જો (buf.data == ખાલી) {
                જાઓ failed;
            }
        }

        p = ngx_cpymem(buf.data, વૃક્ષ->data, વૃક્ષ->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.data = buf.data;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.data);

        જો (!dir.valid_info) {
            જો (ngx_de_info(file.data, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.data);
                ચાલુ;
            }
        }

        જો (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.data);

            ctx->size = ngx_de_size(&dir);
            ctx->fs_size = ngx_de_fs_size(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            જો (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                જાઓ failed;
            }

        } નહીંતર જો (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.data);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_વૃક્ષ_handler(ctx, &file);

            જો (rc == NGX_ABORT) {
                જાઓ failed;
            }

            જો (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.data);
                ચાલુ;
            }

            જો (ngx_walk_વૃક્ષ(ctx, &file) == NGX_ABORT) {
                જાઓ failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            જો (ctx->post_વૃક્ષ_handler(ctx, &file) == NGX_ABORT) {
                જાઓ failed;
            }

        } નહીંતર {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.data);

            જો (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                જાઓ failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    જો (buf.len) {
        ngx_free(buf.data);
    }

    જો (data) {
        ngx_free(data);
        ctx->data = prev;
    }

    જો (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", વૃક્ષ->data);
    }

    પાછા rc;
}
