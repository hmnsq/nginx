
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_int_t ngx_test_بھریا_name(ngx_str_t *name);


ساكن ngx_atomic_t   عارضی_نمبر = 0;
ngx_atomic_t         *ngx_عارضی_نمبر = &عارضی_نمبر;
ngx_atomic_int_t      ngx_random_نمبر = 123456;


ngx_int_t
ngx_get_بھریا_name(ngx_pool_t *pool, ngx_str_t *اگیتر, ngx_str_t *name)
{
    ناپ_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_بھریا_name(name);

    إذا (rc == NGX_OK) {
        أرجع rc;
    }

    len = اگیتر->len;

#إذا (NGX_WIN32)

    إذا (rc == 2) {
        len = rc;
    }

#جے_انت

    n = ngx_pnalloc(pool, len + name->len + 1);
    إذا (n == عدم) {
        أرجع NGX_ERROR;
    }

    p = ngx_cpymem(n, اگیتر->انکڑے, len);
    ngx_cpystrn(p, name->انکڑے, name->len + 1);

    name->len += len;
    name->انکڑے = n;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_test_بھریا_name(ngx_str_t *name)
{
#إذا (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->انکڑے[0];

    إذا (name->len < 2) {
        إذا (c0 == '/') {
            أرجع 2;
        }

        أرجع NGX_DECLINED;
    }

    c1 = name->انکڑے[1];

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

    إذا (name->انکڑے[0] == '/') {
        أرجع NGX_OK;
    }

    أرجع NGX_DECLINED;

#جے_انت
}


ssize_t
ngx_write_chain_منزل_عارضی_file(ngx_عارضی_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    إذا (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_عارضی_file(&tf->file, tf->رستہ, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        إذا (rc != NGX_OK) {
            أرجع rc;
        }

        إذا (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.لوگارتھم, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#إذا (NGX_دھاگے && NGX_HAVE_PWRITEV)

    إذا (tf->thread_write) {
        أرجع ngx_thread_write_chain_منزل_file(&tf->file, chain, tf->وتھ,
                                              tf->pool);
    }

#جے_انت

    أرجع ngx_write_chain_منزل_file(&tf->file, chain, tf->وتھ, tf->pool);
}


ngx_int_t
ngx_create_عارضی_file(ngx_file_t *file, ngx_رستہ_t *رستہ, ngx_pool_t *pool,
    ngx_uint_t persistent, ngx_uint_t clean, ngx_uint_t access)
{
    ناپ_t                    پدھراں;
    u_char                   *p;
    uint32_t                  n;
    ngx_err_t                 err;
    ngx_str_t                 name;
    ngx_uint_t                اگیتر;
    ngx_pool_cleanup_t       *cln;
    ngx_pool_cleanup_file_t  *clnf;

    إذا (file->name.len) {
        name = file->name;
        پدھراں = 0;
        اگیتر = 1;

    } وإلا {
        name = رستہ->name;
        پدھراں = رستہ->len;
        اگیتر = 0;
    }

    file->name.len = name.len + 1 + پدھراں + 10;

    file->name.انکڑے = ngx_pnalloc(pool, file->name.len + 1);
    إذا (file->name.انکڑے == عدم) {
        أرجع NGX_ERROR;
    }

#إذا 0
    لكل (i = 0; i < file->name.len; i++) {
        file->name.انکڑے[i] = 'X';
    }
#جے_انت

    p = ngx_cpymem(file->name.انکڑے, name.انکڑے, name.len);

    إذا (اگیتر) {
        *p = '.';
    }

    p += 1 + پدھراں;

    n = (uint32_t) ngx_اگلا_عارضی_نمبر(0);

    cln = ngx_pool_cleanup_جوڑو(pool, حجم(ngx_pool_cleanup_file_t));
    إذا (cln == عدم) {
        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        (فراغ) ngx_sprintf(p, "%010uD%Z", n);

        إذا (!اگیتر) {
            ngx_create_hashed_filename(رستہ, file->name.انکڑے, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لوگارتھم, 0,
                       "hashed path: %s", file->name.انکڑے);

        file->fd = ngx_open_tempfile(file->name.انکڑے, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لوگارتھم, 0,
                       "temp fd:%d", file->fd);

        إذا (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->انکڑے;

            clnf->fd = file->fd;
            clnf->name = file->name.انکڑے;
            clnf->لوگارتھم = pool->لوگارتھم;

            أرجع NGX_OK;
        }

        err = ngx_errno;

        إذا (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_اگلا_عارضی_نمبر(1);
            استمر;
        }

        إذا ((رستہ->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->لوگارتھم, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.انکڑے);
            أرجع NGX_ERROR;
        }

        إذا (ngx_create_رستہ(file, رستہ) == NGX_ERROR) {
            أرجع NGX_ERROR;
        }
    }
}


فراغ
ngx_create_hashed_filename(ngx_رستہ_t *رستہ, u_char *file, ناپ_t len)
{
    ناپ_t      i, level;
    ngx_uint_t  n;

    i = رستہ->name.len + 1;

    file[رستہ->name.len + رستہ->len]  = '/';

    لكل (n = 0; n < NGX_MAX_رستہ_LEVEL; n++) {
        level = رستہ->level[n];

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
ngx_create_رستہ(ngx_file_t *file, ngx_رستہ_t *رستہ)
{
    ناپ_t      pos;
    ngx_err_t   err;
    ngx_uint_t  i;

    pos = رستہ->name.len;

    لكل (i = 0; i < NGX_MAX_رستہ_LEVEL; i++) {
        إذا (رستہ->level[i] == 0) {
            اكسر;
        }

        pos += رستہ->level[i] + 1;

        file->name.انکڑے[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لوگارتھم, 0,
                       "temp file: \"%s\"", file->name.انکڑے);

        إذا (ngx_create_dir(file->name.انکڑے, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            إذا (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->لوگارتھم, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.انکڑے);
                أرجع NGX_ERROR;
            }
        }

        file->name.انکڑے[pos] = '/';
    }

    أرجع NGX_OK;
}


ngx_err_t
ngx_create_بھریا_رستہ(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#إذا (NGX_WIN32)
    p = dir + 3;
#وإلا
    p = dir + 1;
#جے_انت

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
ngx_اگلا_عارضی_نمبر(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, جوڑو;

    جوڑو = collision ? ngx_random_نمبر : 1;

    n = ngx_atomic_fetch_جوڑو(ngx_عارضی_نمبر, جوڑو);

    أرجع n + جوڑو;
}


حرف *
ngx_conf_سیٹ_رستہ_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ssize_t      level;
    ngx_str_t   *قدر;
    ngx_uint_t   i, n;
    ngx_رستہ_t  *رستہ, **تھاں;

    تھاں = (ngx_رستہ_t **) (p + cmd->وتھ);

    إذا (*تھاں) {
        أرجع "is duplicate";
    }

    رستہ = ngx_pcalloc(cf->pool, حجم(ngx_رستہ_t));
    إذا (رستہ == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    قدر = cf->دلیلاں->elts;

    رستہ->name = قدر[1];

    إذا (رستہ->name.انکڑے[رستہ->name.len - 1] == '/') {
        رستہ->name.len--;
    }

    إذا (ngx_conf_بھریا_name(cf->چکر, &رستہ->name, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    رستہ->conf_file = cf->conf_file->file.name.انکڑے;
    رستہ->سطر = cf->conf_file->سطر;

    لكل (i = 0, n = 2; n < cf->دلیلاں->nelts; i++, n++) {
        level = ngx_atoi(قدر[n].انکڑے, قدر[n].len);
        إذا (level == NGX_ERROR || level == 0) {
            أرجع "invalid value";
        }

        رستہ->level[i] = level;
        رستہ->len += level + 1;
    }

    إذا (رستہ->len > 10 + i) {
        أرجع "invalid value";
    }

    *تھاں = رستہ;

    إذا (ngx_جوڑو_رستہ(cf, تھاں) == NGX_ERROR) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_ملاؤ_رستہ_قدر(ngx_conf_t *cf, ngx_رستہ_t **رستہ, ngx_رستہ_t *prev,
    ngx_رستہ_init_t *init)
{
    ngx_uint_t  i;

    إذا (*رستہ) {
        أرجع NGX_CONF_OK;
    }

    إذا (prev) {
        *رستہ = prev;
        أرجع NGX_CONF_OK;
    }

    *رستہ = ngx_pcalloc(cf->pool, حجم(ngx_رستہ_t));
    إذا (*رستہ == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    (*رستہ)->name = init->name;

    إذا (ngx_conf_بھریا_name(cf->چکر, &(*رستہ)->name, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    لكل (i = 0; i < NGX_MAX_رستہ_LEVEL; i++) {
        (*رستہ)->level[i] = init->level[i];
        (*رستہ)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    إذا (ngx_جوڑو_رستہ(cf, رستہ) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_access_تھاں(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *confp = conf;

    u_char      *p;
    ngx_str_t   *قدر;
    ngx_uint_t   i, دایاں, کھسکاؤ, *access, user;

    access = (ngx_uint_t *) (confp + cmd->وتھ);

    إذا (*access != NGX_CONF_UNSET_UINT) {
        أرجع "is duplicate";
    }

    قدر = cf->دلیلاں->elts;

    *access = 0;
    user = 0600;

    لكل (i = 1; i < cf->دلیلاں->nelts; i++) {

        p = قدر[i].انکڑے;

        إذا (ngx_strncmp(p, "user:", حجم("user:") - 1) == 0) {
            کھسکاؤ = 6;
            p += حجم("user:") - 1;
            user = 0;

        } وإلا إذا (ngx_strncmp(p, "group:", حجم("group:") - 1) == 0) {
            کھسکاؤ = 3;
            p += حجم("group:") - 1;

        } وإلا إذا (ngx_strncmp(p, "all:", حجم("all:") - 1) == 0) {
            کھسکاؤ = 0;
            p += حجم("all:") - 1;

        } وإلا {
            اذهب invalid;
        }

        إذا (ngx_strcmp(p, "rw") == 0) {
            دایاں = 6;

        } وإلا إذا (ngx_strcmp(p, "r") == 0) {
            دایاں = 4;

        } وإلا {
            اذهب invalid;
        }

        *access |= دایاں << کھسکاؤ;
    }

    *access |= user;

    أرجع NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &قدر[i]);

    أرجع NGX_CONF_ERROR;
}


ngx_int_t
ngx_جوڑو_رستہ(ngx_conf_t *cf, ngx_رستہ_t **تھاں)
{
    ngx_uint_t   i, n;
    ngx_رستہ_t  *رستہ, **p;

    رستہ = *تھاں;

    p = cf->چکر->paths.elts;
    لكل (i = 0; i < cf->چکر->paths.nelts; i++) {
        إذا (p[i]->name.len == رستہ->name.len
            && ngx_strcmp(p[i]->name.انکڑے, رستہ->name.انکڑے) == 0)
        {
            إذا (p[i]->انکڑے != رستہ->انکڑے) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->سطر);
                أرجع NGX_ERROR;
            }

            لكل (n = 0; n < NGX_MAX_رستہ_LEVEL; n++) {
                إذا (p[i]->level[n] != رستہ->level[n]) {
                    إذا (رستہ->conf_file == عدم) {
                        إذا (p[i]->conf_file == عدم) {
                            ngx_log_error(NGX_LOG_EMERG, cf->لوگارتھم, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            أرجع NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->لوگارتھم, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->سطر);
                        أرجع NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->سطر);
                    أرجع NGX_ERROR;
                }

                إذا (p[i]->level[n] == 0) {
                    اكسر;
                }
            }

            *تھاں = p[i];

            أرجع NGX_OK;
        }
    }

    p = ngx_array_ڈالو(&cf->چکر->paths);
    إذا (p == عدم) {
        أرجع NGX_ERROR;
    }

    *p = رستہ;

    أرجع NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_چکر_t *چکر, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_رستہ_t      **رستہ;

    رستہ = چکر->paths.elts;
    لكل (i = 0; i < چکر->paths.nelts; i++) {

        إذا (ngx_create_dir(رستہ[i]->name.انکڑے, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            إذا (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, err,
                              ngx_create_dir_n " \"%s\" failed",
                              رستہ[i]->name.انکڑے);
                أرجع NGX_ERROR;
            }
        }

        إذا (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            استمر;
        }

#إذا !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        إذا (ngx_file_info(رستہ[i]->name.انکڑے, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", رستہ[i]->name.انکڑے);
            أرجع NGX_ERROR;
        }

        إذا (fi.st_uid != user) {
            إذا (chown((ثابت حرف *) رستہ[i]->name.انکڑے, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              رستہ[i]->name.انکڑے, user);
                أرجع NGX_ERROR;
            }
        }

        إذا ((fi.st_منوال & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_منوال |= (S_IRUSR|S_IWUSR|S_IXUSR);

            إذا (chmod((ثابت حرف *) رستہ[i]->name.انکڑے, fi.st_منوال) == -1) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                              "chmod() \"%s\" failed", رستہ[i]->name.انکڑے);
                أرجع NGX_ERROR;
            }
        }
        }
#جے_انت
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
        إذا (ngx_change_file_access(src->انکڑے, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->انکڑے);
            err = 0;
            اذهب failed;
        }
    }

#جے_انت

    إذا (ext->ویلا != -1) {
        إذا (ngx_سیٹ_file_ویلا(src->انکڑے, ext->fd, ext->ویلا) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, ngx_errno,
                          ngx_سیٹ_file_ویلا_n " \"%s\" failed", src->انکڑے);
            err = 0;
            اذهب failed;
        }
    }

    إذا (ngx_rename_file(src->انکڑے, منزل->انکڑے) != NGX_FILE_ERROR) {
        أرجع NGX_OK;
    }

    err = ngx_errno;

    إذا (err == NGX_ENOPATH) {

        إذا (!ext->create_رستہ) {
            اذهب failed;
        }

        err = ngx_create_بھریا_رستہ(منزل->انکڑے, ngx_dir_access(ext->رستہ_access));

        إذا (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, err,
                          ngx_create_dir_n " \"%s\" failed", منزل->انکڑے);
            err = 0;
            اذهب failed;
        }

        إذا (ngx_rename_file(src->انکڑے, منزل->انکڑے) != NGX_FILE_ERROR) {
            أرجع NGX_OK;
        }

        err = ngx_errno;
    }

#إذا (NGX_WIN32)

    إذا (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, منزل, ext->لوگارتھم);

        إذا (err == 0) {
            أرجع NGX_OK;
        }
    }

#جے_انت

    إذا (err == NGX_EXDEV) {

        cf.ناپ = -1;
        cf.buf_ناپ = 0;
        cf.access = ext->access;
        cf.ویلا = ext->ویلا;
        cf.لوگارتھم = ext->لوگارتھم;

        name = ngx_alloc(منزل->len + 1 + 10 + 1, ext->لوگارتھم);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_sprintf(name, "%*s.%010uD%Z", منزل->len, منزل->انکڑے,
                           (uint32_t) ngx_اگلا_عارضی_نمبر(0));

        إذا (ngx_copy_file(src->انکڑے, name, &cf) == NGX_OK) {

            إذا (ngx_rename_file(name, منزل->انکڑے) != NGX_FILE_ERROR) {
                ngx_free(name);

                إذا (ngx_delete_file(src->انکڑے) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->انکڑے);
                    أرجع NGX_ERROR;
                }

                أرجع NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, منزل->انکڑے);

            إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    إذا (ext->delete_file) {
        إذا (ngx_delete_file(src->انکڑے) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->انکڑے);
        }
    }

    إذا (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->انکڑے, منزل->انکڑے);
    }

    أرجع NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *منزل, ngx_copy_file_t *cf)
{
    حرف             *buf;
    off_t             ناپ;
    ویلا_t            ویلا;
    ناپ_t            len;
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
        ngx_log_error(NGX_LOG_CRIT, cf->لوگارتھم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        اذهب failed;
    }

    إذا (cf->ناپ != -1 && cf->access != 0 && cf->ویلا != -1) {
        ناپ = cf->ناپ;
        access = cf->access;
        ویلا = cf->ویلا;

    } وإلا {
        إذا (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            اذهب failed;
        }

        ناپ = (cf->ناپ != -1) ? cf->ناپ : ngx_file_ناپ(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        ویلا = (cf->ویلا != -1) ? cf->ویلا : ngx_file_mtime(&fi);
    }

    len = cf->buf_ناپ ? cf->buf_ناپ : 65536;

    إذا ((off_t) len > ناپ) {
        len = (ناپ_t) ناپ;
    }

    buf = ngx_alloc(len, cf->لوگارتھم);
    إذا (buf == عدم) {
        اذهب failed;
    }

    nfd = ngx_open_file(منزل, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    إذا (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->لوگارتھم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", منزل);
        اذهب failed;
    }

    طالما (ناپ > 0) {

        إذا ((off_t) len > ناپ) {
            len = (ناپ_t) ناپ;
        }

        n = ngx_read_fd(fd, buf, len);

        إذا (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            اذهب failed;
        }

        إذا ((ناپ_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, ناپ, from);
            اذهب failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        إذا (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", منزل);
            اذهب failed;
        }

        إذا ((ناپ_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, ناپ, منزل);
            اذهب failed;
        }

        ناپ -= n;
    }

    إذا (ngx_سیٹ_file_ویلا(منزل, nfd, ویلا) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
                      ngx_سیٹ_file_ویلا_n " \"%s\" failed", منزل);
        اذهب failed;
    }

    rc = NGX_OK;

failed:

    إذا (nfd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", منزل);
        }
    }

    إذا (fd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
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
ngx_walk_درخت(ngx_درخت_ctx_t *ctx, ngx_str_t *درخت)
{
    فراغ       *انکڑے, *prev;
    u_char     *p, *name;
    ناپ_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                   "walk tree \"%V\"", درخت);

    إذا (ngx_open_dir(درخت, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->لوگارتھم, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", درخت->انکڑے);
        أرجع NGX_ERROR;
    }

    prev = ctx->انکڑے;

    إذا (ctx->alloc) {
        انکڑے = ngx_alloc(ctx->alloc, ctx->لوگارتھم);
        إذا (انکڑے == عدم) {
            اذهب failed;
        }

        إذا (ctx->init_handler(انکڑے, prev) == NGX_ABORT) {
            اذهب failed;
        }

        ctx->انکڑے = انکڑے;

    } وإلا {
        انکڑے = عدم;
    }

    لكل ( ;; ) {

        ngx_سیٹ_errno(0);

        إذا (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            إذا (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } وإلا {
                ngx_log_error(NGX_LOG_CRIT, ctx->لوگارتھم, err,
                              ngx_read_dir_n " \"%s\" failed", درخت->انکڑے);
                rc = NGX_ERROR;
            }

            اذهب پورا;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                      "tree name %uz:\"%s\"", len, name);

        إذا (len == 1 && name[0] == '.') {
            استمر;
        }

        إذا (len == 2 && name[0] == '.' && name[1] == '.') {
            استمر;
        }

        file.len = درخت->len + 1 + len;

        إذا (file.len > buf.len) {

            إذا (buf.len) {
                ngx_free(buf.انکڑے);
            }

            buf.len = درخت->len + 1 + len;

            buf.انکڑے = ngx_alloc(buf.len + 1, ctx->لوگارتھم);
            إذا (buf.انکڑے == عدم) {
                اذهب failed;
            }
        }

        p = ngx_cpymem(buf.انکڑے, درخت->انکڑے, درخت->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.انکڑے = buf.انکڑے;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                       "tree path \"%s\"", file.انکڑے);

        إذا (!dir.valid_info) {
            إذا (ngx_de_info(file.انکڑے, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->لوگارتھم, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.انکڑے);
                استمر;
            }
        }

        إذا (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                           "tree file \"%s\"", file.انکڑے);

            ctx->ناپ = ngx_de_ناپ(&dir);
            ctx->fs_ناپ = ngx_de_fs_ناپ(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            إذا (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

        } وإلا إذا (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                           "tree enter dir \"%s\"", file.انکڑے);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_درخت_handler(ctx, &file);

            إذا (rc == NGX_ABORT) {
                اذهب failed;
            }

            إذا (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                               "tree skip dir \"%s\"", file.انکڑے);
                استمر;
            }

            إذا (ngx_walk_درخت(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            إذا (ctx->post_درخت_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

        } وإلا {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                           "tree special \"%s\"", file.انکڑے);

            إذا (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

پورا:

    إذا (buf.len) {
        ngx_free(buf.انکڑے);
    }

    إذا (انکڑے) {
        ngx_free(انکڑے);
        ctx->انکڑے = prev;
    }

    إذا (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->لوگارتھم, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", درخت->انکڑے);
    }

    أرجع rc;
}
