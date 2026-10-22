
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_int_t ngx_test_ډک_name(ngx_str_t *name);


ساكن ngx_atomic_t   لنډمهاله_شمېره = 0;
ngx_atomic_t         *ngx_لنډمهاله_شمېره = &لنډمهاله_شمېره;
ngx_atomic_int_t      ngx_random_شمېره = 123456;


ngx_int_t
ngx_get_ډک_name(ngx_pool_t *pool, ngx_str_t *مختاړی, ngx_str_t *name)
{
    اندازه_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_ډک_name(name);

    إذا (rc == NGX_OK) {
        أرجع rc;
    }

    len = مختاړی->len;

#إذا (NGX_WIN32)

    إذا (rc == 2) {
        len = rc;
    }

#د_که_پای

    n = ngx_pnalloc(pool, len + name->len + 1);
    إذا (n == عدم) {
        أرجع NGX_ERROR;
    }

    p = ngx_cpymem(n, مختاړی->معلومات, len);
    ngx_cpystrn(p, name->معلومات, name->len + 1);

    name->len += len;
    name->معلومات = n;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_test_ډک_name(ngx_str_t *name)
{
#إذا (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->معلومات[0];

    إذا (name->len < 2) {
        إذا (c0 == '/') {
            أرجع 2;
        }

        أرجع NGX_DECLINED;
    }

    c1 = name->معلومات[1];

    إذا (c1 == ':') {
        c0 |= 0x20;

        إذا ((c0 >= 'a' && c0 <= 'z')) {
            أرجع NGX_OK;
        }

        أرجع NGX_DECLINED;
    }

    إذا (c1 == '/') {
        أرجع NGX_OK;
    }

    إذا (c0 == '/') {
        أرجع 2;
    }

    أرجع NGX_DECLINED;

#وإلا

    إذا (name->معلومات[0] == '/') {
        أرجع NGX_OK;
    }

    أرجع NGX_DECLINED;

#د_که_پای
}


ssize_t
ngx_write_chain_منزل_لنډمهاله_file(ngx_لنډمهاله_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    إذا (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_لنډمهاله_file(&tf->file, tf->لار, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        إذا (rc != NGX_OK) {
            أرجع rc;
        }

        إذا (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.لوګاریتم, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#إذا (NGX_تارونه && NGX_HAVE_PWRITEV)

    إذا (tf->thread_write) {
        أرجع ngx_thread_write_chain_منزل_file(&tf->file, chain, tf->دننه_واټن,
                                              tf->pool);
    }

#د_که_پای

    أرجع ngx_write_chain_منزل_file(&tf->file, chain, tf->دننه_واټن, tf->pool);
}


ngx_int_t
ngx_create_لنډمهاله_file(ngx_file_t *file, ngx_لار_t *لار, ngx_pool_t *pool,
    ngx_uint_t persistent, ngx_uint_t clean, ngx_uint_t access)
{
    اندازه_t                    پوړونه;
    u_char                   *p;
    uint32_t                  n;
    ngx_err_t                 err;
    ngx_str_t                 name;
    ngx_uint_t                مختاړی;
    ngx_pool_cleanup_t       *cln;
    ngx_pool_cleanup_file_t  *clnf;

    إذا (file->name.len) {
        name = file->name;
        پوړونه = 0;
        مختاړی = 1;

    } وإلا {
        name = لار->name;
        پوړونه = لار->len;
        مختاړی = 0;
    }

    file->name.len = name.len + 1 + پوړونه + 10;

    file->name.معلومات = ngx_pnalloc(pool, file->name.len + 1);
    إذا (file->name.معلومات == عدم) {
        أرجع NGX_ERROR;
    }

#إذا 0
    لكل (i = 0; i < file->name.len; i++) {
        file->name.معلومات[i] = 'X';
    }
#د_که_پای

    p = ngx_cpymem(file->name.معلومات, name.معلومات, name.len);

    إذا (مختاړی) {
        *p = '.';
    }

    p += 1 + پوړونه;

    n = (uint32_t) ngx_بل_لنډمهاله_شمېره(0);

    cln = ngx_pool_cleanup_جمع_کړه(pool, حجم(ngx_pool_cleanup_file_t));
    إذا (cln == عدم) {
        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        (فراغ) ngx_sprintf(p, "%010uD%Z", n);

        إذا (!مختاړی) {
            ngx_create_hashed_filename(لار, file->name.معلومات, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لوګاریتم, 0,
                       "hashed path: %s", file->name.معلومات);

        file->fd = ngx_open_tempfile(file->name.معلومات, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لوګاریتم, 0,
                       "temp fd:%d", file->fd);

        إذا (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->معلومات;

            clnf->fd = file->fd;
            clnf->name = file->name.معلومات;
            clnf->لوګاریتم = pool->لوګاریتم;

            أرجع NGX_OK;
        }

        err = ngx_errno;

        إذا (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_بل_لنډمهاله_شمېره(1);
            استمر;
        }

        إذا ((لار->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->لوګاریتم, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.معلومات);
            أرجع NGX_ERROR;
        }

        إذا (ngx_create_لار(file, لار) == NGX_ERROR) {
            أرجع NGX_ERROR;
        }
    }
}


فراغ
ngx_create_hashed_filename(ngx_لار_t *لار, u_char *file, اندازه_t len)
{
    اندازه_t      i, level;
    ngx_uint_t  n;

    i = لار->name.len + 1;

    file[لار->name.len + لار->len]  = '/';

    لكل (n = 0; n < NGX_MAX_لار_LEVEL; n++) {
        level = لار->level[n];

        إذا (level == 0) {
            اكسر;
        }

        len -= level;
        file[i - 1] = '/';
        ngx_memcpy(&file[i], &file[len], level);
        i += level + 1;
    }
}


ngx_int_t
ngx_create_لار(ngx_file_t *file, ngx_لار_t *لار)
{
    اندازه_t      pos;
    ngx_err_t   err;
    ngx_uint_t  i;

    pos = لار->name.len;

    لكل (i = 0; i < NGX_MAX_لار_LEVEL; i++) {
        إذا (لار->level[i] == 0) {
            اكسر;
        }

        pos += لار->level[i] + 1;

        file->name.معلومات[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لوګاریتم, 0,
                       "temp file: \"%s\"", file->name.معلومات);

        إذا (ngx_create_dir(file->name.معلومات, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            إذا (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->لوګاریتم, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.معلومات);
                أرجع NGX_ERROR;
            }
        }

        file->name.معلومات[pos] = '/';
    }

    أرجع NGX_OK;
}


ngx_err_t
ngx_create_ډک_لار(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#إذا (NGX_WIN32)
    p = dir + 3;
#وإلا
    p = dir + 1;
#د_که_پای

    لكل ( /* void */ ; *p; p++) {
        ch = *p;

        إذا (ch != '/') {
            استمر;
        }

        *p = '\0';

        إذا (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            بدل (err) {
            حالة NGX_EEXIST:
                err = 0;
            حالة NGX_EACCES:
                اكسر;

            افتراضي:
                أرجع err;
            }
        }

        *p = '/';
    }

    أرجع err;
}


ngx_atomic_uint_t
ngx_بل_لنډمهاله_شمېره(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, جمع_کړه;

    جمع_کړه = collision ? ngx_random_شمېره : 1;

    n = ngx_atomic_fetch_جمع_کړه(ngx_لنډمهاله_شمېره, جمع_کړه);

    أرجع n + جمع_کړه;
}


حرف *
ngx_conf_سېټ_لار_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ssize_t      level;
    ngx_str_t   *ارزښت;
    ngx_uint_t   i, n;
    ngx_لار_t  *لار, **خونه;

    خونه = (ngx_لار_t **) (p + cmd->دننه_واټن);

    إذا (*خونه) {
        أرجع "is duplicate";
    }

    لار = ngx_pcalloc(cf->pool, حجم(ngx_لار_t));
    إذا (لار == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ارزښت = cf->دلیلونه->elts;

    لار->name = ارزښت[1];

    إذا (لار->name.معلومات[لار->name.len - 1] == '/') {
        لار->name.len--;
    }

    إذا (ngx_conf_ډک_name(cf->دوره, &لار->name, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    لار->conf_file = cf->conf_file->file.name.معلومات;
    لار->کرښه = cf->conf_file->کرښه;

    لكل (i = 0, n = 2; n < cf->دلیلونه->nelts; i++, n++) {
        level = ngx_atoi(ارزښت[n].معلومات, ارزښت[n].len);
        إذا (level == NGX_ERROR || level == 0) {
            أرجع "invalid value";
        }

        لار->level[i] = level;
        لار->len += level + 1;
    }

    إذا (لار->len > 10 + i) {
        أرجع "invalid value";
    }

    *خونه = لار;

    إذا (ngx_جمع_کړه_لار(cf, خونه) == NGX_ERROR) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_یوځای_لار_ارزښت(ngx_conf_t *cf, ngx_لار_t **لار, ngx_لار_t *prev,
    ngx_لار_init_t *init)
{
    ngx_uint_t  i;

    إذا (*لار) {
        أرجع NGX_CONF_OK;
    }

    إذا (prev) {
        *لار = prev;
        أرجع NGX_CONF_OK;
    }

    *لار = ngx_pcalloc(cf->pool, حجم(ngx_لار_t));
    إذا (*لار == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    (*لار)->name = init->name;

    إذا (ngx_conf_ډک_name(cf->دوره, &(*لار)->name, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    لكل (i = 0; i < NGX_MAX_لار_LEVEL; i++) {
        (*لار)->level[i] = init->level[i];
        (*لار)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    إذا (ngx_جمع_کړه_لار(cf, لار) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سېټ_access_خونه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *confp = conf;

    u_char      *p;
    ngx_str_t   *ارزښت;
    ngx_uint_t   i, ښی, ښويدنه, *access, user;

    access = (ngx_uint_t *) (confp + cmd->دننه_واټن);

    إذا (*access != NGX_CONF_UNSET_UINT) {
        أرجع "is duplicate";
    }

    ارزښت = cf->دلیلونه->elts;

    *access = 0;
    user = 0600;

    لكل (i = 1; i < cf->دلیلونه->nelts; i++) {

        p = ارزښت[i].معلومات;

        إذا (ngx_strncmp(p, "user:", حجم("user:") - 1) == 0) {
            ښويدنه = 6;
            p += حجم("user:") - 1;
            user = 0;

        } وإلا إذا (ngx_strncmp(p, "group:", حجم("group:") - 1) == 0) {
            ښويدنه = 3;
            p += حجم("group:") - 1;

        } وإلا إذا (ngx_strncmp(p, "all:", حجم("all:") - 1) == 0) {
            ښويدنه = 0;
            p += حجم("all:") - 1;

        } وإلا {
            اذهب invalid;
        }

        إذا (ngx_strcmp(p, "rw") == 0) {
            ښی = 6;

        } وإلا إذا (ngx_strcmp(p, "r") == 0) {
            ښی = 4;

        } وإلا {
            اذهب invalid;
        }

        *access |= ښی << ښويدنه;
    }

    *access |= user;

    أرجع NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &ارزښت[i]);

    أرجع NGX_CONF_ERROR;
}


ngx_int_t
ngx_جمع_کړه_لار(ngx_conf_t *cf, ngx_لار_t **خونه)
{
    ngx_uint_t   i, n;
    ngx_لار_t  *لار, **p;

    لار = *خونه;

    p = cf->دوره->paths.elts;
    لكل (i = 0; i < cf->دوره->paths.nelts; i++) {
        إذا (p[i]->name.len == لار->name.len
            && ngx_strcmp(p[i]->name.معلومات, لار->name.معلومات) == 0)
        {
            إذا (p[i]->معلومات != لار->معلومات) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->کرښه);
                أرجع NGX_ERROR;
            }

            لكل (n = 0; n < NGX_MAX_لار_LEVEL; n++) {
                إذا (p[i]->level[n] != لار->level[n]) {
                    إذا (لار->conf_file == عدم) {
                        إذا (p[i]->conf_file == عدم) {
                            ngx_log_error(NGX_LOG_EMERG, cf->لوګاریتم, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            أرجع NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->لوګاریتم, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->کرښه);
                        أرجع NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->کرښه);
                    أرجع NGX_ERROR;
                }

                إذا (p[i]->level[n] == 0) {
                    اكسر;
                }
            }

            *خونه = p[i];

            أرجع NGX_OK;
        }
    }

    p = ngx_array_ټېل(&cf->دوره->paths);
    إذا (p == عدم) {
        أرجع NGX_ERROR;
    }

    *p = لار;

    أرجع NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_دوره_t *دوره, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_لار_t      **لار;

    لار = دوره->paths.elts;
    لكل (i = 0; i < دوره->paths.nelts; i++) {

        إذا (ngx_create_dir(لار[i]->name.معلومات, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            إذا (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, err,
                              ngx_create_dir_n " \"%s\" failed",
                              لار[i]->name.معلومات);
                أرجع NGX_ERROR;
            }
        }

        إذا (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            استمر;
        }

#إذا !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        إذا (ngx_file_info(لار[i]->name.معلومات, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", لار[i]->name.معلومات);
            أرجع NGX_ERROR;
        }

        إذا (fi.st_uid != user) {
            إذا (chown((ثابت حرف *) لار[i]->name.معلومات, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              لار[i]->name.معلومات, user);
                أرجع NGX_ERROR;
            }
        }

        إذا ((fi.st_ډېرپېښ & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_ډېرپېښ |= (S_IRUSR|S_IWUSR|S_IXUSR);

            إذا (chmod((ثابت حرف *) لار[i]->name.معلومات, fi.st_ډېرپېښ) == -1) {
                ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                              "chmod() \"%s\" failed", لار[i]->name.معلومات);
                أرجع NGX_ERROR;
            }
        }
        }
#د_که_پای
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *منزل, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#إذا !(NGX_WIN32)

    إذا (ext->access) {
        إذا (ngx_change_file_access(src->معلومات, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->لوګاریتم, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->معلومات);
            err = 0;
            اذهب failed;
        }
    }

#د_که_پای

    إذا (ext->وخت != -1) {
        إذا (ngx_سېټ_file_وخت(src->معلومات, ext->fd, ext->وخت) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->لوګاریتم, ngx_errno,
                          ngx_سېټ_file_وخت_n " \"%s\" failed", src->معلومات);
            err = 0;
            اذهب failed;
        }
    }

    إذا (ngx_rename_file(src->معلومات, منزل->معلومات) != NGX_FILE_ERROR) {
        أرجع NGX_OK;
    }

    err = ngx_errno;

    إذا (err == NGX_ENOPATH) {

        إذا (!ext->create_لار) {
            اذهب failed;
        }

        err = ngx_create_ډک_لار(منزل->معلومات, ngx_dir_access(ext->لار_access));

        إذا (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->لوګاریتم, err,
                          ngx_create_dir_n " \"%s\" failed", منزل->معلومات);
            err = 0;
            اذهب failed;
        }

        إذا (ngx_rename_file(src->معلومات, منزل->معلومات) != NGX_FILE_ERROR) {
            أرجع NGX_OK;
        }

        err = ngx_errno;
    }

#إذا (NGX_WIN32)

    إذا (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, منزل, ext->لوګاریتم);

        إذا (err == 0) {
            أرجع NGX_OK;
        }
    }

#د_که_پای

    إذا (err == NGX_EXDEV) {

        cf.اندازه = -1;
        cf.buf_اندازه = 0;
        cf.access = ext->access;
        cf.وخت = ext->وخت;
        cf.لوګاریتم = ext->لوګاریتم;

        name = ngx_alloc(منزل->len + 1 + 10 + 1, ext->لوګاریتم);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_sprintf(name, "%*s.%010uD%Z", منزل->len, منزل->معلومات,
                           (uint32_t) ngx_بل_لنډمهاله_شمېره(0));

        إذا (ngx_copy_file(src->معلومات, name, &cf) == NGX_OK) {

            إذا (ngx_rename_file(name, منزل->معلومات) != NGX_FILE_ERROR) {
                ngx_free(name);

                إذا (ngx_delete_file(src->معلومات) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->لوګاریتم, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->معلومات);
                    أرجع NGX_ERROR;
                }

                أرجع NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->لوګاریتم, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, منزل->معلومات);

            إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->لوګاریتم, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    إذا (ext->delete_file) {
        إذا (ngx_delete_file(src->معلومات) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->لوګاریتم, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->معلومات);
        }
    }

    إذا (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->لوګاریتم, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->معلومات, منزل->معلومات);
    }

    أرجع NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *منزل, ngx_copy_file_t *cf)
{
    حرف             *buf;
    off_t             اندازه;
    وخت_t            وخت;
    اندازه_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = عدم;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    إذا (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->لوګاریتم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        اذهب failed;
    }

    إذا (cf->اندازه != -1 && cf->access != 0 && cf->وخت != -1) {
        اندازه = cf->اندازه;
        access = cf->access;
        وخت = cf->وخت;

    } وإلا {
        إذا (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوګاریتم, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            اذهب failed;
        }

        اندازه = (cf->اندازه != -1) ? cf->اندازه : ngx_file_اندازه(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        وخت = (cf->وخت != -1) ? cf->وخت : ngx_file_mtime(&fi);
    }

    len = cf->buf_اندازه ? cf->buf_اندازه : 65536;

    إذا ((off_t) len > اندازه) {
        len = (اندازه_t) اندازه;
    }

    buf = ngx_alloc(len, cf->لوګاریتم);
    إذا (buf == عدم) {
        اذهب failed;
    }

    nfd = ngx_open_file(منزل, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    إذا (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->لوګاریتم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", منزل);
        اذهب failed;
    }

    طالما (اندازه > 0) {

        إذا ((off_t) len > اندازه) {
            len = (اندازه_t) اندازه;
        }

        n = ngx_read_fd(fd, buf, len);

        إذا (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوګاریتم, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            اذهب failed;
        }

        إذا ((اندازه_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوګاریتم, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, اندازه, from);
            اذهب failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        إذا (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوګاریتم, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", منزل);
            اذهب failed;
        }

        إذا ((اندازه_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوګاریتم, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, اندازه, منزل);
            اذهب failed;
        }

        اندازه -= n;
    }

    إذا (ngx_سېټ_file_وخت(منزل, nfd, وخت) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->لوګاریتم, ngx_errno,
                      ngx_سېټ_file_وخت_n " \"%s\" failed", منزل);
        اذهب failed;
    }

    rc = NGX_OK;

failed:

    إذا (nfd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوګاریتم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", منزل);
        }
    }

    إذا (fd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوګاریتم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    إذا (buf) {
        ngx_free(buf);
    }

    أرجع rc;
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
ngx_walk_ونه(ngx_ونه_ctx_t *ctx, ngx_str_t *ونه)
{
    فراغ       *معلومات, *prev;
    u_char     *p, *name;
    اندازه_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوګاریتم, 0,
                   "walk tree \"%V\"", ونه);

    إذا (ngx_open_dir(ونه, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->لوګاریتم, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", ونه->معلومات);
        أرجع NGX_ERROR;
    }

    prev = ctx->معلومات;

    إذا (ctx->alloc) {
        معلومات = ngx_alloc(ctx->alloc, ctx->لوګاریتم);
        إذا (معلومات == عدم) {
            اذهب failed;
        }

        إذا (ctx->init_handler(معلومات, prev) == NGX_ABORT) {
            اذهب failed;
        }

        ctx->معلومات = معلومات;

    } وإلا {
        معلومات = عدم;
    }

    لكل ( ;; ) {

        ngx_سېټ_errno(0);

        إذا (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            إذا (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } وإلا {
                ngx_log_error(NGX_LOG_CRIT, ctx->لوګاریتم, err,
                              ngx_read_dir_n " \"%s\" failed", ونه->معلومات);
                rc = NGX_ERROR;
            }

            اذهب بشپړ;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->لوګاریتم, 0,
                      "tree name %uz:\"%s\"", len, name);

        إذا (len == 1 && name[0] == '.') {
            استمر;
        }

        إذا (len == 2 && name[0] == '.' && name[1] == '.') {
            استمر;
        }

        file.len = ونه->len + 1 + len;

        إذا (file.len > buf.len) {

            إذا (buf.len) {
                ngx_free(buf.معلومات);
            }

            buf.len = ونه->len + 1 + len;

            buf.معلومات = ngx_alloc(buf.len + 1, ctx->لوګاریتم);
            إذا (buf.معلومات == عدم) {
                اذهب failed;
            }
        }

        p = ngx_cpymem(buf.معلومات, ونه->معلومات, ونه->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.معلومات = buf.معلومات;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوګاریتم, 0,
                       "tree path \"%s\"", file.معلومات);

        إذا (!dir.valid_info) {
            إذا (ngx_de_info(file.معلومات, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->لوګاریتم, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.معلومات);
                استمر;
            }
        }

        إذا (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوګاریتم, 0,
                           "tree file \"%s\"", file.معلومات);

            ctx->اندازه = ngx_de_اندازه(&dir);
            ctx->fs_اندازه = ngx_de_fs_اندازه(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            إذا (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

        } وإلا إذا (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوګاریتم, 0,
                           "tree enter dir \"%s\"", file.معلومات);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_ونه_handler(ctx, &file);

            إذا (rc == NGX_ABORT) {
                اذهب failed;
            }

            إذا (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوګاریتم, 0,
                               "tree skip dir \"%s\"", file.معلومات);
                استمر;
            }

            إذا (ngx_walk_ونه(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            إذا (ctx->post_ونه_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

        } وإلا {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوګاریتم, 0,
                           "tree special \"%s\"", file.معلومات);

            إذا (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

بشپړ:

    إذا (buf.len) {
        ngx_free(buf.معلومات);
    }

    إذا (معلومات) {
        ngx_free(معلومات);
        ctx->معلومات = prev;
    }

    إذا (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->لوګاریتم, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", ونه->معلومات);
    }

    أرجع rc;
}
