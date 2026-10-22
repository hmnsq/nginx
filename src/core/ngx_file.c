
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


अटल ngx_int_t ngx_test_full_name(ngx_str_t *name);


अटल ngx_atomic_t   अस्थायी_संख्या = 0;
ngx_atomic_t         *ngx_अस्थायी_संख्या = &अस्थायी_संख्या;
ngx_atomic_int_t      ngx_random_संख्या = 123456;


ngx_int_t
ngx_get_full_name(ngx_pool_t *pool, ngx_str_t *prefix, ngx_str_t *name)
{
    माप_प्रकार      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_full_name(name);

    अगर (rc == NGX_OK) {
        वापस rc;
    }

    len = prefix->len;

#अगर (NGX_WIN32)

    अगर (rc == 2) {
        len = rc;
    }

#पूर्ण_अगर

    n = ngx_pnalloc(pool, len + name->len + 1);
    अगर (n == शून्य) {
        वापस NGX_ERROR;
    }

    p = ngx_cpymem(n, prefix->तथ्य, len);
    ngx_cpystrn(p, name->तथ्य, name->len + 1);

    name->len += len;
    name->तथ्य = n;

    वापस NGX_OK;
}


अटल ngx_int_t
ngx_test_full_name(ngx_str_t *name)
{
#अगर (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->तथ्य[0];

    अगर (name->len < 2) {
        अगर (c0 == '/') {
            वापस 2;
        }

        वापस NGX_DECLINED;
    }

    c1 = name->तथ्य[1];

    अगर (c1 == ':') {
        c0 |= 0x20;

        अगर ((c0 >= 'a' && c0 <= 'z')) {
            वापस NGX_OK;
        }

        वापस NGX_DECLINED;
    }

    अगर (c1 == '/') {
        वापस NGX_OK;
    }

    अगर (c0 == '/') {
        वापस 2;
    }

    वापस NGX_DECLINED;

#अन्यथा

    अगर (name->तथ्य[0] == '/') {
        वापस NGX_OK;
    }

    वापस NGX_DECLINED;

#पूर्ण_अगर
}


ssize_t
ngx_write_chain_to_अस्थायी_file(ngx_अस्थायी_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    अगर (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_अस्थायी_file(&tf->file, tf->path, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        अगर (rc != NGX_OK) {
            वापस rc;
        }

        अगर (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.log, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#अगर (NGX_THREADS && NGX_HAVE_PWRITEV)

    अगर (tf->thread_write) {
        वापस ngx_thread_write_chain_to_file(&tf->file, chain, tf->offset,
                                              tf->pool);
    }

#पूर्ण_अगर

    वापस ngx_write_chain_to_file(&tf->file, chain, tf->offset, tf->pool);
}


ngx_int_t
ngx_create_अस्थायी_file(ngx_file_t *file, ngx_path_t *path, ngx_pool_t *pool,
    ngx_uint_t persistent, ngx_uint_t clean, ngx_uint_t access)
{
    माप_प्रकार                    levels;
    u_char                   *p;
    uint32_t                  n;
    ngx_err_t                 err;
    ngx_str_t                 name;
    ngx_uint_t                prefix;
    ngx_pool_cleanup_t       *cln;
    ngx_pool_cleanup_file_t  *clnf;

    अगर (file->name.len) {
        name = file->name;
        levels = 0;
        prefix = 1;

    } अन्यथा {
        name = path->name;
        levels = path->len;
        prefix = 0;
    }

    file->name.len = name.len + 1 + levels + 10;

    file->name.तथ्य = ngx_pnalloc(pool, file->name.len + 1);
    अगर (file->name.तथ्य == शून्य) {
        वापस NGX_ERROR;
    }

#अगर 0
    क्रम (i = 0; i < file->name.len; i++) {
        file->name.तथ्य[i] = 'X';
    }
#पूर्ण_अगर

    p = ngx_cpymem(file->name.तथ्य, name.तथ्य, name.len);

    अगर (prefix) {
        *p = '.';
    }

    p += 1 + levels;

    n = (uint32_t) ngx_अग्र_अस्थायी_संख्या(0);

    cln = ngx_pool_cleanup_add(pool, माप(ngx_pool_cleanup_file_t));
    अगर (cln == शून्य) {
        वापस NGX_ERROR;
    }

    क्रम ( ;; ) {
        (व्योम) ngx_sprintf(p, "%010uD%Z", n);

        अगर (!prefix) {
            ngx_create_hashed_filename(path, file->name.तथ्य, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "hashed path: %s", file->name.तथ्य);

        file->fd = ngx_open_tempfile(file->name.तथ्य, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp fd:%d", file->fd);

        अगर (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->तथ्य;

            clnf->fd = file->fd;
            clnf->name = file->name.तथ्य;
            clnf->log = pool->log;

            वापस NGX_OK;
        }

        err = ngx_errno;

        अगर (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_अग्र_अस्थायी_संख्या(1);
            जारी;
        }

        अगर ((path->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->log, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.तथ्य);
            वापस NGX_ERROR;
        }

        अगर (ngx_create_path(file, path) == NGX_ERROR) {
            वापस NGX_ERROR;
        }
    }
}


व्योम
ngx_create_hashed_filename(ngx_path_t *path, u_char *file, माप_प्रकार len)
{
    माप_प्रकार      i, level;
    ngx_uint_t  n;

    i = path->name.len + 1;

    file[path->name.len + path->len]  = '/';

    क्रम (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
        level = path->level[n];

        अगर (level == 0) {
            अवरोध;
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
    माप_प्रकार      pos;
    ngx_err_t   err;
    ngx_uint_t  i;

    pos = path->name.len;

    क्रम (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        अगर (path->level[i] == 0) {
            अवरोध;
        }

        pos += path->level[i] + 1;

        file->name.तथ्य[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->log, 0,
                       "temp file: \"%s\"", file->name.तथ्य);

        अगर (ngx_create_dir(file->name.तथ्य, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            अगर (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.तथ्य);
                वापस NGX_ERROR;
            }
        }

        file->name.तथ्य[pos] = '/';
    }

    वापस NGX_OK;
}


ngx_err_t
ngx_create_full_path(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#अगर (NGX_WIN32)
    p = dir + 3;
#अन्यथा
    p = dir + 1;
#पूर्ण_अगर

    क्रम ( /* void */ ; *p; p++) {
        ch = *p;

        अगर (ch != '/') {
            जारी;
        }

        *p = '\0';

        अगर (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            चयन (err) {
            हाल NGX_EEXIST:
                err = 0;
            हाल NGX_EACCES:
                अवरोध;

            शेष:
                वापस err;
            }
        }

        *p = '/';
    }

    वापस err;
}


ngx_atomic_uint_t
ngx_अग्र_अस्थायी_संख्या(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, add;

    add = collision ? ngx_random_संख्या : 1;

    n = ngx_atomic_fetch_add(ngx_अस्थायी_संख्या, add);

    वापस n + add;
}


अक्षर *
ngx_conf_set_path_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर  *p = conf;

    ssize_t      level;
    ngx_str_t   *मूल्य;
    ngx_uint_t   i, n;
    ngx_path_t  *path, **slot;

    slot = (ngx_path_t **) (p + cmd->offset);

    अगर (*slot) {
        वापस "is duplicate";
    }

    path = ngx_pcalloc(cf->pool, माप(ngx_path_t));
    अगर (path == शून्य) {
        वापस NGX_CONF_ERROR;
    }

    मूल्य = cf->args->elts;

    path->name = मूल्य[1];

    अगर (path->name.तथ्य[path->name.len - 1] == '/') {
        path->name.len--;
    }

    अगर (ngx_conf_full_name(cf->cycle, &path->name, 0) != NGX_OK) {
        वापस NGX_CONF_ERROR;
    }

    path->conf_file = cf->conf_file->file.name.तथ्य;
    path->पंक्ति = cf->conf_file->पंक्ति;

    क्रम (i = 0, n = 2; n < cf->args->nelts; i++, n++) {
        level = ngx_atoi(मूल्य[n].तथ्य, मूल्य[n].len);
        अगर (level == NGX_ERROR || level == 0) {
            वापस "invalid value";
        }

        path->level[i] = level;
        path->len += level + 1;
    }

    अगर (path->len > 10 + i) {
        वापस "invalid value";
    }

    *slot = path;

    अगर (ngx_add_path(cf, slot) == NGX_ERROR) {
        वापस NGX_CONF_ERROR;
    }

    वापस NGX_CONF_OK;
}


अक्षर *
ngx_conf_merge_path_मूल्य(ngx_conf_t *cf, ngx_path_t **path, ngx_path_t *पूर्व,
    ngx_path_init_t *init)
{
    ngx_uint_t  i;

    अगर (*path) {
        वापस NGX_CONF_OK;
    }

    अगर (पूर्व) {
        *path = पूर्व;
        वापस NGX_CONF_OK;
    }

    *path = ngx_pcalloc(cf->pool, माप(ngx_path_t));
    अगर (*path == शून्य) {
        वापस NGX_CONF_ERROR;
    }

    (*path)->name = init->name;

    अगर (ngx_conf_full_name(cf->cycle, &(*path)->name, 0) != NGX_OK) {
        वापस NGX_CONF_ERROR;
    }

    क्रम (i = 0; i < NGX_MAX_PATH_LEVEL; i++) {
        (*path)->level[i] = init->level[i];
        (*path)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    अगर (ngx_add_path(cf, path) != NGX_OK) {
        वापस NGX_CONF_ERROR;
    }

    वापस NGX_CONF_OK;
}


अक्षर *
ngx_conf_set_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, व्योम *conf)
{
    अक्षर  *confp = conf;

    u_char      *p;
    ngx_str_t   *मूल्य;
    ngx_uint_t   i, दायाँ, shift, *access, user;

    access = (ngx_uint_t *) (confp + cmd->offset);

    अगर (*access != NGX_CONF_UNSET_UINT) {
        वापस "is duplicate";
    }

    मूल्य = cf->args->elts;

    *access = 0;
    user = 0600;

    क्रम (i = 1; i < cf->args->nelts; i++) {

        p = मूल्य[i].तथ्य;

        अगर (ngx_strncmp(p, "user:", माप("user:") - 1) == 0) {
            shift = 6;
            p += माप("user:") - 1;
            user = 0;

        } अन्यथा अगर (ngx_strncmp(p, "group:", माप("group:") - 1) == 0) {
            shift = 3;
            p += माप("group:") - 1;

        } अन्यथा अगर (ngx_strncmp(p, "all:", माप("all:") - 1) == 0) {
            shift = 0;
            p += माप("all:") - 1;

        } अन्यथा {
            जाओ invalid;
        }

        अगर (ngx_strcmp(p, "rw") == 0) {
            दायाँ = 6;

        } अन्यथा अगर (ngx_strcmp(p, "r") == 0) {
            दायाँ = 4;

        } अन्यथा {
            जाओ invalid;
        }

        *access |= दायाँ << shift;
    }

    *access |= user;

    वापस NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &मूल्य[i]);

    वापस NGX_CONF_ERROR;
}


ngx_int_t
ngx_add_path(ngx_conf_t *cf, ngx_path_t **slot)
{
    ngx_uint_t   i, n;
    ngx_path_t  *path, **p;

    path = *slot;

    p = cf->cycle->paths.elts;
    क्रम (i = 0; i < cf->cycle->paths.nelts; i++) {
        अगर (p[i]->name.len == path->name.len
            && ngx_strcmp(p[i]->name.तथ्य, path->name.तथ्य) == 0)
        {
            अगर (p[i]->तथ्य != path->तथ्य) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->पंक्ति);
                वापस NGX_ERROR;
            }

            क्रम (n = 0; n < NGX_MAX_PATH_LEVEL; n++) {
                अगर (p[i]->level[n] != path->level[n]) {
                    अगर (path->conf_file == शून्य) {
                        अगर (p[i]->conf_file == शून्य) {
                            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            वापस NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->पंक्ति);
                        वापस NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->पंक्ति);
                    वापस NGX_ERROR;
                }

                अगर (p[i]->level[n] == 0) {
                    अवरोध;
                }
            }

            *slot = p[i];

            वापस NGX_OK;
        }
    }

    p = ngx_array_धकेलो(&cf->cycle->paths);
    अगर (p == शून्य) {
        वापस NGX_ERROR;
    }

    *p = path;

    वापस NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_path_t      **path;

    path = cycle->paths.elts;
    क्रम (i = 0; i < cycle->paths.nelts; i++) {

        अगर (ngx_create_dir(path[i]->name.तथ्य, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            अगर (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, err,
                              ngx_create_dir_n " \"%s\" failed",
                              path[i]->name.तथ्य);
                वापस NGX_ERROR;
            }
        }

        अगर (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            जारी;
        }

#अगर !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        अगर (ngx_file_info(path[i]->name.तथ्य, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", path[i]->name.तथ्य);
            वापस NGX_ERROR;
        }

        अगर (fi.st_uid != user) {
            अगर (chown((स्थिर अक्षर *) path[i]->name.तथ्य, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              path[i]->name.तथ्य, user);
                वापस NGX_ERROR;
            }
        }

        अगर ((fi.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_mode |= (S_IRUSR|S_IWUSR|S_IXUSR);

            अगर (chmod((स्थिर अक्षर *) path[i]->name.तथ्य, fi.st_mode) == -1) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              "chmod() \"%s\" failed", path[i]->name.तथ्य);
                वापस NGX_ERROR;
            }
        }
        }
#पूर्ण_अगर
    }

    वापस NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *to, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#अगर !(NGX_WIN32)

    अगर (ext->access) {
        अगर (ngx_change_file_access(src->तथ्य, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->तथ्य);
            err = 0;
            जाओ failed;
        }
    }

#पूर्ण_अगर

    अगर (ext->समय != -1) {
        अगर (ngx_set_file_time(src->तथ्य, ext->fd, ext->समय) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_set_file_time_n " \"%s\" failed", src->तथ्य);
            err = 0;
            जाओ failed;
        }
    }

    अगर (ngx_rename_file(src->तथ्य, to->तथ्य) != NGX_FILE_ERROR) {
        वापस NGX_OK;
    }

    err = ngx_errno;

    अगर (err == NGX_ENOPATH) {

        अगर (!ext->create_path) {
            जाओ failed;
        }

        err = ngx_create_full_path(to->तथ्य, ngx_dir_access(ext->path_access));

        अगर (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                          ngx_create_dir_n " \"%s\" failed", to->तथ्य);
            err = 0;
            जाओ failed;
        }

        अगर (ngx_rename_file(src->तथ्य, to->तथ्य) != NGX_FILE_ERROR) {
            वापस NGX_OK;
        }

        err = ngx_errno;
    }

#अगर (NGX_WIN32)

    अगर (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, to, ext->log);

        अगर (err == 0) {
            वापस NGX_OK;
        }
    }

#पूर्ण_अगर

    अगर (err == NGX_EXDEV) {

        cf.आकार = -1;
        cf.buf_आकार = 0;
        cf.access = ext->access;
        cf.समय = ext->समय;
        cf.log = ext->log;

        name = ngx_alloc(to->len + 1 + 10 + 1, ext->log);
        अगर (name == शून्य) {
            वापस NGX_ERROR;
        }

        (व्योम) ngx_sprintf(name, "%*s.%010uD%Z", to->len, to->तथ्य,
                           (uint32_t) ngx_अग्र_अस्थायी_संख्या(0));

        अगर (ngx_copy_file(src->तथ्य, name, &cf) == NGX_OK) {

            अगर (ngx_rename_file(name, to->तथ्य) != NGX_FILE_ERROR) {
                ngx_free(name);

                अगर (ngx_delete_file(src->तथ्य) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->तथ्य);
                    वापस NGX_ERROR;
                }

                वापस NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, to->तथ्य);

            अगर (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    अगर (ext->delete_file) {
        अगर (ngx_delete_file(src->तथ्य) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->log, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->तथ्य);
        }
    }

    अगर (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->log, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->तथ्य, to->तथ्य);
    }

    वापस NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *to, ngx_copy_file_t *cf)
{
    अक्षर             *buf;
    off_t             आकार;
    समय_प्रकार            समय;
    माप_प्रकार            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = शून्य;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    अगर (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        जाओ failed;
    }

    अगर (cf->आकार != -1 && cf->access != 0 && cf->समय != -1) {
        आकार = cf->आकार;
        access = cf->access;
        समय = cf->समय;

    } अन्यथा {
        अगर (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            जाओ failed;
        }

        आकार = (cf->आकार != -1) ? cf->आकार : ngx_file_आकार(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        समय = (cf->समय != -1) ? cf->समय : ngx_file_mtime(&fi);
    }

    len = cf->buf_आकार ? cf->buf_आकार : 65536;

    अगर ((off_t) len > आकार) {
        len = (माप_प्रकार) आकार;
    }

    buf = ngx_alloc(len, cf->log);
    अगर (buf == शून्य) {
        जाओ failed;
    }

    nfd = ngx_open_file(to, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    अगर (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", to);
        जाओ failed;
    }

    जबतक (आकार > 0) {

        अगर ((off_t) len > आकार) {
            len = (माप_प्रकार) आकार;
        }

        n = ngx_read_fd(fd, buf, len);

        अगर (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            जाओ failed;
        }

        अगर ((माप_प्रकार) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, आकार, from);
            जाओ failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        अगर (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", to);
            जाओ failed;
        }

        अगर ((माप_प्रकार) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, आकार, to);
            जाओ failed;
        }

        आकार -= n;
    }

    अगर (ngx_set_file_time(to, nfd, समय) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                      ngx_set_file_time_n " \"%s\" failed", to);
        जाओ failed;
    }

    rc = NGX_OK;

failed:

    अगर (nfd != NGX_INVALID_FILE) {
        अगर (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", to);
        }
    }

    अगर (fd != NGX_INVALID_FILE) {
        अगर (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    अगर (buf) {
        ngx_free(buf);
    }

    वापस rc;
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
ngx_walk_वृक्ष(ngx_वृक्ष_ctx_t *ctx, ngx_str_t *वृक्ष)
{
    व्योम       *तथ्य, *पूर्व;
    u_char     *p, *name;
    माप_प्रकार      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                   "walk tree \"%V\"", वृक्ष);

    अगर (ngx_open_dir(वृक्ष, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", वृक्ष->तथ्य);
        वापस NGX_ERROR;
    }

    पूर्व = ctx->तथ्य;

    अगर (ctx->alloc) {
        तथ्य = ngx_alloc(ctx->alloc, ctx->log);
        अगर (तथ्य == शून्य) {
            जाओ failed;
        }

        अगर (ctx->init_handler(तथ्य, पूर्व) == NGX_ABORT) {
            जाओ failed;
        }

        ctx->तथ्य = तथ्य;

    } अन्यथा {
        तथ्य = शून्य;
    }

    क्रम ( ;; ) {

        ngx_set_errno(0);

        अगर (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            अगर (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } अन्यथा {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, err,
                              ngx_read_dir_n " \"%s\" failed", वृक्ष->तथ्य);
                rc = NGX_ERROR;
            }

            जाओ done;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                      "tree name %uz:\"%s\"", len, name);

        अगर (len == 1 && name[0] == '.') {
            जारी;
        }

        अगर (len == 2 && name[0] == '.' && name[1] == '.') {
            जारी;
        }

        file.len = वृक्ष->len + 1 + len;

        अगर (file.len > buf.len) {

            अगर (buf.len) {
                ngx_free(buf.तथ्य);
            }

            buf.len = वृक्ष->len + 1 + len;

            buf.तथ्य = ngx_alloc(buf.len + 1, ctx->log);
            अगर (buf.तथ्य == शून्य) {
                जाओ failed;
            }
        }

        p = ngx_cpymem(buf.तथ्य, वृक्ष->तथ्य, वृक्ष->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.तथ्य = buf.तथ्य;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                       "tree path \"%s\"", file.तथ्य);

        अगर (!dir.valid_info) {
            अगर (ngx_de_info(file.तथ्य, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.तथ्य);
                जारी;
            }
        }

        अगर (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree file \"%s\"", file.तथ्य);

            ctx->आकार = ngx_de_आकार(&dir);
            ctx->fs_आकार = ngx_de_fs_आकार(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            अगर (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                जाओ failed;
            }

        } अन्यथा अगर (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree enter dir \"%s\"", file.तथ्य);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_वृक्ष_handler(ctx, &file);

            अगर (rc == NGX_ABORT) {
                जाओ failed;
            }

            अगर (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                               "tree skip dir \"%s\"", file.तथ्य);
                जारी;
            }

            अगर (ngx_walk_वृक्ष(ctx, &file) == NGX_ABORT) {
                जाओ failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            अगर (ctx->post_वृक्ष_handler(ctx, &file) == NGX_ABORT) {
                जाओ failed;
            }

        } अन्यथा {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->log, 0,
                           "tree special \"%s\"", file.तथ्य);

            अगर (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                जाओ failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

done:

    अगर (buf.len) {
        ngx_free(buf.तथ्य);
    }

    अगर (तथ्य) {
        ngx_free(तथ्य);
        ctx->तथ्य = पूर्व;
    }

    अगर (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->log, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", वृक्ष->तथ्य);
    }

    वापस rc;
}
