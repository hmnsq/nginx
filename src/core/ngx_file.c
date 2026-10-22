
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


স্থির ngx_int_t ngx_test_full_name(ngx_str_t *name);


স্থির ngx_atomic_t   অস্থায়ী_সংখ্যা = 0;
ngx_atomic_t         *ngx_অস্থায়ী_সংখ্যা = &অস্থায়ী_সংখ্যা;
ngx_atomic_int_t      ngx_random_সংখ্যা = 123456;


ngx_int_t
ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix, ngx_str_t *name)
{
    আকার_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_full_name(name);

    যদি (rc == NGX_OK) {
        ফেরত rc;
    }

    len = prefix->len;

#যদি (NGX_WIN32)

    যদি (rc == 2) {
        len = rc;
    }

#endif

    n = ngx_pnalloc(pool, len + name->len + 1);
    যদি (n == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->তথ্য, len);
    ngx_cpystrn(p, name->তথ্য, name->len + 1);

    name->len += len;
    name->তথ্য = n;

    ফেরত NGX_OK;
}


স্থির ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#যদি (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->তথ্য[0];

    যদি (name->len < 2) {
        যদি (c0 == '/') {
            ফেরত 2;
        }

        ফেরত NGX_DECLINED;
    }

    c1 = name->তথ্য[1];

    যদি (c1 == ':') {
        c0 |= 0x20;

        যদি ((c0 >= 'a' && c0 <= 'z')) {
            ফেরত NGX_OK;
        }

        ফেরত NGX_DECLINED;
    }

    যদি (c1 == '/') {
        ফেরত NGX_OK;
    }

    যদি (c0 == '/') {
        ফেরত 2;
    }

    ফেরত NGX_DECLINED;

#অন্যথা

    যদি (name->তথ্য[0] == '/') {
        ফেরত NGX_OK;
    }

    ফেরত NGX_DECLINED;

#endif
}


ssize_t
ngx_write_chain_to_অস্থায়ী_file(ngx_অস্থায়ী_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    যদি (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_অস্থায়ী_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        যদি (rc != NGX_OK) {
            ফেরত rc;
        }

        যদি (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#যদি (NGX_THREADS && NGX_HAVE_PWRITEV)

    যদি (tf->thread_write) {
        ফেরত ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#endif

    ফেরত ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
}


ngx_int_t
ngx_create_অস্থায়ী_file(ngx_file_t *file, ngx_path_t *path, ngx_pool_t *pool,
    ngx_uint_t persistent, ngx_uint_t clean, ngx_uint_t access)
{
    আকার_t                    levels;
    u_char                   *p;
    uint32_t                  n;
    ngx_err_t                 err;
    ngx_str_t                 name;
    ngx_uint_t                prefix;
    ngx_pool_cleanup_t       *cln;
    ngx_pool_cleanup_file_t  *clnf;

    যদি (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } অন্যথা {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.তথ্য = ngx_pnalloc(pool, file->name.len + 1);
    যদি (file->name.তথ্য == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

#যদি 0
    জন্য (i = 0; i < file->name.len; i++) {
        file->name.তথ্য[i] = 'X';
    }
#endif

    p = ngx_cpymem(file->name.তথ্য, name.তথ্য, name.len);

    যদি (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_পরবর্তী_অস্থায়ী_সংখ্যা(0);

    cln = ngx_pool_cleanup_add(pool, আকার(ngx_pool_cleanup_file_t));
    যদি (cln == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    জন্য ( ;; ) {
        (শূন্য) ngx_sprintf(p, "%010uD%Z", n);

        যদি (!prefix) {
            ngx_create_hashed_filename(path, file->name.তথ্য, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.তথ্য);

        file->fd = ngx_open_tempfile(file->name.তথ্য, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        যদি (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->তথ্য;

            clnf->fd = file->fd;
            clnf->name = file->name.তথ্য;
            clnf->log = pool->log;

            ফেরত NGX_OK;
        }

        err = ngx_errno;

        যদি (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_পরবর্তী_অস্থায়ী_সংখ্যা(1);
            চালু;
        }

        যদি ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.তথ্য);
            ফেরত NGX_ERROR;
        }

        যদি (ngx_create_path(file, path) == NGX_ERROR) {
            ফেরত NGX_ERROR;
        }
    }
}


শূন্য
ngx_create_hashed_filename(ngx_path_t *path, u_char *file, আকার_t len)
{
    আকার_t      i, level;
    ngx_uint_t  n;

    i = path->name.len + 1;

    file[path->name.len + path->len]  = '/';

    জন্য (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        যদি (level == 0) {
            বিরতি;
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
    আকার_t      pos;
    ngx_err_t   err;
    ngx_uint_t  i;

    pos = path->name.len;

    জন্য (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        যদি (path->level[i] == 0) {
            বিরতি;
        }

        pos += path->level[i] + 1;

        file->name.তথ্য[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.তথ্য);

        যদি (ngx_create_dir(file->name.তথ্য, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            যদি (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.তথ্য);
                ফেরত NGX_ERROR;
            }
        }

        file->name.তথ্য[pos] = '/';
    }

    ফেরত NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#যদি (NGX_WIN32)
    p = dir + 3;
#অন্যথা
    p = dir + 1;
#endif

    জন্য ( /* void */ ; *p; p++) {
        ch = *p;

        যদি (ch != '/') {
            চালু;
        }

        *p = '\0';

        যদি (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            নির্বাচন (err) {
            ক্ষেত্র NGX_EEXIST:
                err = 0;
            ক্ষেত্র NGX_EACCES:
                বিরতি;

            পূর্বনির্ধারিত:
                ফেরত err;
            }
        }

        *p = '/';
    }

    ফেরত err;
}


ngx_atomic_uint_t
ngx_পরবর্তী_অস্থায়ী_সংখ্যা(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_সংখ্যা : 1;

    n = ngx_atomic_fetch_add(ngx_অস্থায়ী_সংখ্যা, add);

    ফেরত n + add;
}


অক্ষর *
ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর  *p = conf;

    ssize_t      level;
    ngx_str_t   *মান;
    ngx_uint_t   i, n;
    ngx_path_t  *path, **slot;

    slot = (ngx_path_t **) (p + cmd->offset);

    যদি (*slot) {
        ফেরত "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, আকার(ngx_path_t));
    যদি (path == ফাঁকা) {
        ফেরত NGX_CONF_ERROR;
    }

    মান = cf->args->elts;

    path->name = মান[1];

    যদি (path->name.তথ্য[path->name.len - 1] == '/') {
        path->name.len--;
    }

    যদি (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        ফেরত NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.তথ্য;
    path->সারি = cf->conf_file->সারি;

    জন্য (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(মান[n].তথ্য, মান[n].len);
        যদি (level == NGX_ERROR || level == 0) {
            ফেরত "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    যদি (path->len > 10 + i) {
        ফেরত "invalid value";
    }

    *slot = path;

    যদি (ngx_add_path(cf, slot) == NGX_ERROR) {
        ফেরত NGX_CONF_ERROR;
    }

    ফেরত NGX_CONF_OK;
}


অক্ষর *
ngx_conf_merge_path_মান(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *পূর্ব,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    যদি (*path) {
        ফেরত NGX_CONF_OK;
    }

    যদি (পূর্ব) {
        *path = পূর্ব;
        ফেরত NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, আকার(ngx_path_t));
    যদি (*path == ফাঁকা) {
        ফেরত NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    যদি (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        ফেরত NGX_CONF_ERROR;
    }

    জন্য (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    যদি (ngx_add_path(cf, path) != NGX_OK) {
        ফেরত NGX_CONF_ERROR;
    }

    ফেরত NGX_CONF_OK;
}


অক্ষর *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, শূন্য *conf)
{
    অক্ষর  *confp = conf;

    u_char      *p;
    ngx_str_t   *মান;
    ngx_uint_t   i, ডান, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    যদি (*access != NGX_CONF_UNSET_UINT) {
        ফেরত "is duplicate";
    }

    মান = cf->args->elts;

    *access = 0;
    user = 0600;

    জন্য (i = 1; i < cf->args->nelts; i++) {

        p = মান[i].তথ্য;

        যদি (ngx_strncmp(p, "user:", আকার("user:") - 1) == 0) {
            shift = 6;
            p += আকার("user:") - 1;
            user = 0;

        } অন্যথা যদি (ngx_strncmp(p, "group:", আকার("group:") - 1) == 0) {
            shift = 3;
            p += আকার("group:") - 1;

        } অন্যথা যদি (ngx_strncmp(p, "all:", আকার("all:") - 1) == 0) {
            shift = 0;
            p += আকার("all:") - 1;

        } অন্যথা {
            যাও invalid;
        }

        যদি (ngx_strcmp(p, "rw") == 0) {
            ডান = 6;

        } অন্যথা যদি (ngx_strcmp(p, "r") == 0) {
            ডান = 4;

        } অন্যথা {
            যাও invalid;
        }

        *access |= ডান << shift;
    }

    *access |= user;

    ফেরত NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &মান[i]);

    ফেরত NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    জন্য (i = 0; i < cf->cycle->paths.nelts; i++) {
        যদি (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.তথ্য, path->name.তথ্য) == 0)
        {
            যদি (p[i]->তথ্য != path->তথ্য) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->সারি);
                ফেরত NGX_ERROR;
            }

            জন্য (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                যদি (p[i]->level[n] != path->level[n]) {
                    যদি (path->conf_file == ফাঁকা) {
                        যদি (p[i]->conf_file == ফাঁকা) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            ফেরত NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->সারি);
                        ফেরত NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->সারি);
                    ফেরত NGX_ERROR;
                }

                যদি (p[i]->level[n] == 0) {
                    বিরতি;
                }
            }

            *slot = p[i];

            ফেরত NGX_OK;
        }
    }

    p = ngx_array_ঠেলো(&cf->cycle->paths);
    যদি (p == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    *p = path;

    ফেরত NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    জন্য (i = 0; i < cycle->paths.nelts; i++) {

        যদি (ngx_create_dir(path[i]->name.তথ্য, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            যদি (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.তথ্য);
                ফেরত NGX_ERROR;
            }
        }

        যদি (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            চালু;
        }

#যদি !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        যদি (ngx_file_info(path[i]->name.তথ্য, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.তথ্য);
            ফেরত NGX_ERROR;
        }

        যদি (fi.st_uid != user) {
            যদি (chown((ধ্রুবক অক্ষর *) path[i]->name.তথ্য, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.তথ্য, user);
                ফেরত NGX_ERROR;
            }
        }

        যদি ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            যদি (chmod((ধ্রুবক অক্ষর *) path[i]->name.তথ্য, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.তথ্য);
                ফেরত NGX_ERROR;
            }
        }
        }
#endif
    }

    ফেরত NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#যদি !(NGX_WIN32)

    যদি (ext->access) {
        যদি (ngx_change_file_access(src->তথ্য, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->তথ্য);
            err = 0;
            যাও failed;
        }
    }

#endif

    যদি (ext->time != -1) {
        যদি (ngx_set_file_time(src->তথ্য, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->তথ্য);
            err = 0;
            যাও failed;
        }
    }

    যদি (ngx_rename_file(src->তথ্য, to->তথ্য) != NGX_FILE_ERROR) {
        ফেরত NGX_OK;
    }

    err = ngx_errno;

    যদি (err == NGX_ENOPATH) {

        যদি (!ext->create_path) {
            যাও failed;
        }

        err = ngx_create_full_path(to->তথ্য, ngx_dir_access(ext->path_access));

        যদি (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->তথ্য);
            err = 0;
            যাও failed;
        }

        যদি (ngx_rename_file(src->তথ্য, to->তথ্য) != NGX_FILE_ERROR) {
            ফেরত NGX_OK;
        }

        err = ngx_errno;
    }

#যদি (NGX_WIN32)

    যদি (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        যদি (err == 0) {
            ফেরত NGX_OK;
        }
    }

#endif

    যদি (err == NGX_EXDEV) {

        cf.আকার_2 = -1;
        cf.buf_আকার = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        যদি (name == ফাঁকা) {
            ফেরত NGX_ERROR;
        }

        (শূন্য) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->তথ্য,
                           (uint32_t) ngx_পরবর্তী_অস্থায়ী_সংখ্যা(0));

        যদি (ngx_copy_file(src->তথ্য, name, &cf) == NGX_OK) {

            যদি (ngx_rename_file(name, to->তথ্য) != NGX_FILE_ERROR) {
                ngx_free(name);

                যদি (ngx_delete_file(src->তথ্য) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->তথ্য);
                    ফেরত NGX_ERROR;
                }

                ফেরত NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->তথ্য);

            যদি (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    যদি (ext->delete_file) {
        যদি (ngx_delete_file(src->তথ্য) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->তথ্য);
        }
    }

    যদি (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->তথ্য, to->তথ্য);
    }

    ফেরত NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf)
{
    অক্ষর             *buf;
    off_t             আকার_2;
    time_t            time;
    আকার_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = ফাঁকা;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    যদি (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        যাও failed;
    }

    যদি (cf->আকার_2 != -1 && cf->access != 0 && cf->time != -1) {
        আকার_2 = cf->আকার_2;
        access = cf->access;
        time = cf->time;

    } অন্যথা {
        যদি (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            যাও failed;
        }

        আকার_2 = (cf->আকার_2 != -1) ? cf->আকার_2 : ngx_file_আকার(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_আকার ? cf->buf_আকার : 65536;

    যদি ((off_t) len > আকার_2) {
        len = (আকার_t) আকার_2;
    }

    buf = ngx_alloc(len, cf->log);
    যদি (buf == ফাঁকা) {
        যাও failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    যদি (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        যাও failed;
    }

    যখন (আকার_2 > 0) {

        যদি ((off_t) len > আকার_2) {
            len = (আকার_t) আকার_2;
        }

        n = ngx_read_fd(fd, buf, len);

        যদি (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            যাও failed;
        }

        যদি ((আকার_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, আকার_2, from);
            যাও failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        যদি (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            যাও failed;
        }

        যদি ((আকার_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, আকার_2, to);
            যাও failed;
        }

        আকার_2 -= n;
    }

    যদি (ngx_set_file_time(to, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        যাও failed;
    }

    rc = NGX_OK;

failed:

    যদি (nfd != NGX_INVALID_FILE) {
        যদি (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    যদি (fd != NGX_INVALID_FILE) {
        যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    যদি (buf) {
        ngx_free(buf);
    }

    ফেরত rc;
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
ngx_walk_বৃক্ষ(ngx_বৃক্ষ_ctx_t *ctx, ngx_str_t *বৃক্ষ)
{
    শূন্য       *তথ্য, *পূর্ব;
    u_char     *p, *name;
    আকার_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                   "walk tree \"%V\"", বৃক্ষ);

    যদি (ngx_open_dir(বৃক্ষ, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", বৃক্ষ->তথ্য);
        ফেরত NGX_ERROR;
    }

    পূর্ব = ctx->তথ্য;

    যদি (ctx->alloc) {
        তথ্য = ngx_alloc(ctx->alloc, ctx->log);
        যদি (তথ্য == ফাঁকা) {
            যাও failed;
        }

        যদি (ctx->init_handler(তথ্য, পূর্ব) == NGX_ABORT) {
            যাও failed;
        }

        ctx->তথ্য = তথ্য;

    } অন্যথা {
        তথ্য = ফাঁকা;
    }

    জন্য ( ;; ) {

        ngx_set_errno(0);

        যদি (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            যদি (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } অন্যথা {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, err,
                              ngx_read_dir_n " \"%s\" failed", বৃক্ষ->তথ্য);
                rc = NGX_ERROR;
            }

            যাও done;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                      "tree name %uz:\"%s\"", len, name);

        যদি (len == 1 && name[0] == '.') {
            চালু;
        }

        যদি (len == 2 && name[0] == '.' && name[1] == '.') {
            চালু;
        }

        file.len = বৃক্ষ->len + 1 + len;

        যদি (file.len > buf.len) {

            যদি (buf.len) {
                ngx_free(buf.তথ্য);
            }

            buf.len = বৃক্ষ->len + 1 + len;

            buf.তথ্য = ngx_alloc(buf.len + 1, ctx->log);
            যদি (buf.তথ্য == ফাঁকা) {
                যাও failed;
            }
        }

        p = ngx_cpymem(buf.তথ্য, বৃক্ষ->তথ্য, বৃক্ষ->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.তথ্য = buf.তথ্য;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.তথ্য);

        যদি (!dir.valid_info) {
            যদি (ngx_de_info(file.তথ্য, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.তথ্য);
                চালু;
            }
        }

        যদি (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.তথ্য);

            ctx->আকার_2 = ngx_de_আকার(&dir);
            ctx->fs_আকার = ngx_de_fs_আকার(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            যদি (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                যাও failed;
            }

        } অন্যথা যদি (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.তথ্য);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_বৃক্ষ_handler(ctx, &file);

            যদি (rc == NGX_ABORT) {
                যাও failed;
            }

            যদি (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.তথ্য);
                চালু;
            }

            যদি (ngx_walk_বৃক্ষ(ctx, &file) == NGX_ABORT) {
                যাও failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            যদি (ctx->post_বৃক্ষ_handler(ctx, &file) == NGX_ABORT) {
                যাও failed;
            }

        } অন্যথা {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.তথ্য);

            যদি (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                যাও failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    যদি (buf.len) {
        ngx_free(buf.তথ্য);
    }

    যদি (তথ্য) {
        ngx_free(তথ্য);
        ctx->তথ্য = পূর্ব;
    }

    যদি (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", বৃক্ষ->তথ্য);
    }

    ফেরত rc;
}
