
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ساکن ngx_int_t ngx_test_بھرا_name(ngx_str_t *name);


ساکن ngx_atomic_t   عارضی_نمبر = 0;
ngx_atomic_t         *ngx_عارضی_نمبر = &عارضی_نمبر;
ngx_atomic_int_t      ngx_random_نمبر = 123456;


ngx_int_t
ngx_get_بھرا_name(ngx_pool_t *pool, ngx_str_t *سابقہ, ngx_str_t *name)
{
    سائز_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_بھرا_name(name);

    اگر (rc == NGX_OK) {
        واپس rc;
    }

    len = سابقہ->len;

#اگر (NGX_WIN32)

    اگر (rc == 2) {
        len = rc;
    }

#اگر_ختم

    n = ngx_pnalloc(pool, len + name->len + 1);
    اگر (n == تہی) {
        واپس NGX_ERROR;
    }

    p = ngx_cpymem(n, سابقہ->ڈیٹا, len);
    ngx_cpystrn(p, name->ڈیٹا, name->len + 1);

    name->len += len;
    name->ڈیٹا = n;

    واپس NGX_OK;
}


ساکن ngx_int_t
ngx_test_بھرا_name(ngx_str_t *name)
{
#اگر (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->ڈیٹا[0];

    اگر (name->len < 2) {
        اگر (c0 == '/') {
            واپس 2;
        }

        واپس NGX_DECLINED;
    }

    c1 = name->ڈیٹا[1];

    اگر (c1 == ':') {
        c0 |= 0x20;

        اگر ((c0 >= 'a' && c0 <= 'z')) {
            واپس NGX_OK;
        }

        واپس NGX_DECLINED;
    }

    اگر (c1 == '/') {
        واپس NGX_OK;
    }

    اگر (c0 == '/') {
        واپس 2;
    }

    واپس NGX_DECLINED;

#ورنہ

    اگر (name->ڈیٹا[0] == '/') {
        واپس NGX_OK;
    }

    واپس NGX_DECLINED;

#اگر_ختم
}


ssize_t
ngx_write_chain_مقصد_عارضی_file(ngx_عارضی_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    اگر (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_عارضی_file(&tf->file, tf->راستہ, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        اگر (rc != NGX_OK) {
            واپس rc;
        }

        اگر (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.لوگارتھم, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#اگر (NGX_دھاگے && NGX_HAVE_PWRITEV)

    اگر (tf->thread_write) {
        واپس ngx_thread_write_chain_مقصد_file(&tf->file, chain, tf->انحراف,
                                              tf->pool);
    }

#اگر_ختم

    واپس ngx_write_chain_مقصد_file(&tf->file, chain, tf->انحراف, tf->pool);
}


ngx_int_t
ngx_create_عارضی_file(ngx_file_t *file, ngx_راستہ_t *راستہ, ngx_pool_t *pool,
    ngx_uint_t persistent, ngx_uint_t clean, ngx_uint_t access)
{
    سائز_t                    سطحیں;
    u_char                   *p;
    uint32_t                  n;
    ngx_err_t                 err;
    ngx_str_t                 name;
    ngx_uint_t                سابقہ;
    ngx_pool_cleanup_t       *cln;
    ngx_pool_cleanup_file_t  *clnf;

    اگر (file->name.len) {
        name = file->name;
        سطحیں = 0;
        سابقہ = 1;

    } ورنہ {
        name = راستہ->name;
        سطحیں = راستہ->len;
        سابقہ = 0;
    }

    file->name.len = name.len + 1 + سطحیں + 10;

    file->name.ڈیٹا = ngx_pnalloc(pool, file->name.len + 1);
    اگر (file->name.ڈیٹا == تہی) {
        واپس NGX_ERROR;
    }

#اگر 0
    برائے (i = 0; i < file->name.len; i++) {
        file->name.ڈیٹا[i] = 'X';
    }
#اگر_ختم

    p = ngx_cpymem(file->name.ڈیٹا, name.ڈیٹا, name.len);

    اگر (سابقہ) {
        *p = '.';
    }

    p += 1 + سطحیں;

    n = (uint32_t) ngx_اگلا_عارضی_نمبر(0);

    cln = ngx_pool_cleanup_جوڑو(pool, حجم(ngx_pool_cleanup_file_t));
    اگر (cln == تہی) {
        واپس NGX_ERROR;
    }

    برائے ( ;; ) {
        (خالی) ngx_sprintf(p, "%010uD%Z", n);

        اگر (!سابقہ) {
            ngx_create_hashed_filename(راستہ, file->name.ڈیٹا, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لوگارتھم, 0,
                       "hashed path: %s", file->name.ڈیٹا);

        file->fd = ngx_open_tempfile(file->name.ڈیٹا, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لوگارتھم, 0,
                       "temp fd:%d", file->fd);

        اگر (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->ڈیٹا;

            clnf->fd = file->fd;
            clnf->name = file->name.ڈیٹا;
            clnf->لوگارتھم = pool->لوگارتھم;

            واپس NGX_OK;
        }

        err = ngx_errno;

        اگر (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_اگلا_عارضی_نمبر(1);
            جاری;
        }

        اگر ((راستہ->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->لوگارتھم, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.ڈیٹا);
            واپس NGX_ERROR;
        }

        اگر (ngx_create_راستہ(file, راستہ) == NGX_ERROR) {
            واپس NGX_ERROR;
        }
    }
}


خالی
ngx_create_hashed_filename(ngx_راستہ_t *راستہ, u_char *file, سائز_t len)
{
    سائز_t      i, level;
    ngx_uint_t  n;

    i = راستہ->name.len + 1;

    file[راستہ->name.len + راستہ->len]  = '/';

    برائے (n = 0; n < NGX_MAX_راستہ_LEVEL; n++) {
        level = راستہ->level[n];

        اگر (level == 0) {
            توڑو;
        }

        len -= level;
        file[i - 1] = '/';
        ngx_memcpy(&file[i], &file[len], level);
        i += level + 1;
    }
}


ngx_int_t
ngx_create_راستہ(ngx_file_t *file, ngx_راستہ_t *راستہ)
{
    سائز_t      pos;
    ngx_err_t   err;
    ngx_uint_t  i;

    pos = راستہ->name.len;

    برائے (i = 0; i < NGX_MAX_راستہ_LEVEL; i++) {
        اگر (راستہ->level[i] == 0) {
            توڑو;
        }

        pos += راستہ->level[i] + 1;

        file->name.ڈیٹا[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لوگارتھم, 0,
                       "temp file: \"%s\"", file->name.ڈیٹا);

        اگر (ngx_create_dir(file->name.ڈیٹا, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            اگر (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->لوگارتھم, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.ڈیٹا);
                واپس NGX_ERROR;
            }
        }

        file->name.ڈیٹا[pos] = '/';
    }

    واپس NGX_OK;
}


ngx_err_t
ngx_create_بھرا_راستہ(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#اگر (NGX_WIN32)
    p = dir + 3;
#ورنہ
    p = dir + 1;
#اگر_ختم

    برائے ( /* void */ ; *p; p++) {
        ch = *p;

        اگر (ch != '/') {
            جاری;
        }

        *p = '\0';

        اگر (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            انتخاب (err) {
            حالت NGX_EEXIST:
                err = 0;
            حالت NGX_EACCES:
                توڑو;

            طےشدہ:
                واپس err;
            }
        }

        *p = '/';
    }

    واپس err;
}


ngx_atomic_uint_t
ngx_اگلا_عارضی_نمبر(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, جوڑو;

    جوڑو = collision ? ngx_random_نمبر : 1;

    n = ngx_atomic_fetch_جوڑو(ngx_عارضی_نمبر, جوڑو);

    واپس n + جوڑو;
}


حرف *
ngx_conf_سیٹ_راستہ_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف  *p = conf;

    ssize_t      level;
    ngx_str_t   *قدر;
    ngx_uint_t   i, n;
    ngx_راستہ_t  *راستہ, **خانہ;

    خانہ = (ngx_راستہ_t **) (p + cmd->انحراف);

    اگر (*خانہ) {
        واپس "is duplicate";
    }

    راستہ = ngx_pcalloc(cf->pool, حجم(ngx_راستہ_t));
    اگر (راستہ == تہی) {
        واپس NGX_CONF_ERROR;
    }

    قدر = cf->دلائل->elts;

    راستہ->name = قدر[1];

    اگر (راستہ->name.ڈیٹا[راستہ->name.len - 1] == '/') {
        راستہ->name.len--;
    }

    اگر (ngx_conf_بھرا_name(cf->چکر, &راستہ->name, 0) != NGX_OK) {
        واپس NGX_CONF_ERROR;
    }

    راستہ->conf_file = cf->conf_file->file.name.ڈیٹا;
    راستہ->سطر = cf->conf_file->سطر;

    برائے (i = 0, n = 2; n < cf->دلائل->nelts; i++, n++) {
        level = ngx_atoi(قدر[n].ڈیٹا, قدر[n].len);
        اگر (level == NGX_ERROR || level == 0) {
            واپس "invalid value";
        }

        راستہ->level[i] = level;
        راستہ->len += level + 1;
    }

    اگر (راستہ->len > 10 + i) {
        واپس "invalid value";
    }

    *خانہ = راستہ;

    اگر (ngx_جوڑو_راستہ(cf, خانہ) == NGX_ERROR) {
        واپس NGX_CONF_ERROR;
    }

    واپس NGX_CONF_OK;
}


حرف *
ngx_conf_ملاؤ_راستہ_قدر(ngx_conf_t *cf, ngx_راستہ_t **راستہ, ngx_راستہ_t *پچھلا,
    ngx_راستہ_init_t *init)
{
    ngx_uint_t  i;

    اگر (*راستہ) {
        واپس NGX_CONF_OK;
    }

    اگر (پچھلا) {
        *راستہ = پچھلا;
        واپس NGX_CONF_OK;
    }

    *راستہ = ngx_pcalloc(cf->pool, حجم(ngx_راستہ_t));
    اگر (*راستہ == تہی) {
        واپس NGX_CONF_ERROR;
    }

    (*راستہ)->name = init->name;

    اگر (ngx_conf_بھرا_name(cf->چکر, &(*راستہ)->name, 0) != NGX_OK) {
        واپس NGX_CONF_ERROR;
    }

    برائے (i = 0; i < NGX_MAX_راستہ_LEVEL; i++) {
        (*راستہ)->level[i] = init->level[i];
        (*راستہ)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    اگر (ngx_جوڑو_راستہ(cf, راستہ) != NGX_OK) {
        واپس NGX_CONF_ERROR;
    }

    واپس NGX_CONF_OK;
}


حرف *
ngx_conf_سیٹ_access_خانہ(ngx_conf_t *cf, ngx_command_t *cmd, خالی *conf)
{
    حرف  *confp = conf;

    u_char      *p;
    ngx_str_t   *قدر;
    ngx_uint_t   i, دایاں, سرکاؤ, *access, user;

    access = (ngx_uint_t *) (confp + cmd->انحراف);

    اگر (*access != NGX_CONF_UNSET_UINT) {
        واپس "is duplicate";
    }

    قدر = cf->دلائل->elts;

    *access = 0;
    user = 0600;

    برائے (i = 1; i < cf->دلائل->nelts; i++) {

        p = قدر[i].ڈیٹا;

        اگر (ngx_strncmp(p, "user:", حجم("user:") - 1) == 0) {
            سرکاؤ = 6;
            p += حجم("user:") - 1;
            user = 0;

        } ورنہ اگر (ngx_strncmp(p, "group:", حجم("group:") - 1) == 0) {
            سرکاؤ = 3;
            p += حجم("group:") - 1;

        } ورنہ اگر (ngx_strncmp(p, "all:", حجم("all:") - 1) == 0) {
            سرکاؤ = 0;
            p += حجم("all:") - 1;

        } ورنہ {
            جاؤ invalid;
        }

        اگر (ngx_strcmp(p, "rw") == 0) {
            دایاں = 6;

        } ورنہ اگر (ngx_strcmp(p, "r") == 0) {
            دایاں = 4;

        } ورنہ {
            جاؤ invalid;
        }

        *access |= دایاں << سرکاؤ;
    }

    *access |= user;

    واپس NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &قدر[i]);

    واپس NGX_CONF_ERROR;
}


ngx_int_t
ngx_جوڑو_راستہ(ngx_conf_t *cf, ngx_راستہ_t **خانہ)
{
    ngx_uint_t   i, n;
    ngx_راستہ_t  *راستہ, **p;

    راستہ = *خانہ;

    p = cf->چکر->paths.elts;
    برائے (i = 0; i < cf->چکر->paths.nelts; i++) {
        اگر (p[i]->name.len == راستہ->name.len
            && ngx_strcmp(p[i]->name.ڈیٹا, راستہ->name.ڈیٹا) == 0)
        {
            اگر (p[i]->ڈیٹا != راستہ->ڈیٹا) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->سطر);
                واپس NGX_ERROR;
            }

            برائے (n = 0; n < NGX_MAX_راستہ_LEVEL; n++) {
                اگر (p[i]->level[n] != راستہ->level[n]) {
                    اگر (راستہ->conf_file == تہی) {
                        اگر (p[i]->conf_file == تہی) {
                            ngx_log_error(NGX_LOG_EMERG, cf->لوگارتھم, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            واپس NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->لوگارتھم, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->سطر);
                        واپس NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->سطر);
                    واپس NGX_ERROR;
                }

                اگر (p[i]->level[n] == 0) {
                    توڑو;
                }
            }

            *خانہ = p[i];

            واپس NGX_OK;
        }
    }

    p = ngx_array_ڈالو(&cf->چکر->paths);
    اگر (p == تہی) {
        واپس NGX_ERROR;
    }

    *p = راستہ;

    واپس NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_چکر_t *چکر, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_راستہ_t      **راستہ;

    راستہ = چکر->paths.elts;
    برائے (i = 0; i < چکر->paths.nelts; i++) {

        اگر (ngx_create_dir(راستہ[i]->name.ڈیٹا, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            اگر (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, err,
                              ngx_create_dir_n " \"%s\" failed",
                              راستہ[i]->name.ڈیٹا);
                واپس NGX_ERROR;
            }
        }

        اگر (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            جاری;
        }

#اگر !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        اگر (ngx_file_info(راستہ[i]->name.ڈیٹا, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", راستہ[i]->name.ڈیٹا);
            واپس NGX_ERROR;
        }

        اگر (fi.st_uid != user) {
            اگر (chown((ثابت حرف *) راستہ[i]->name.ڈیٹا, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              راستہ[i]->name.ڈیٹا, user);
                واپس NGX_ERROR;
            }
        }

        اگر ((fi.st_منوال & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_منوال |= (S_IRUSR|S_IWUSR|S_IXUSR);

            اگر (chmod((ثابت حرف *) راستہ[i]->name.ڈیٹا, fi.st_منوال) == -1) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                              "chmod() \"%s\" failed", راستہ[i]->name.ڈیٹا);
                واپس NGX_ERROR;
            }
        }
        }
#اگر_ختم
    }

    واپس NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *مقصد, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#اگر !(NGX_WIN32)

    اگر (ext->access) {
        اگر (ngx_change_file_access(src->ڈیٹا, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->ڈیٹا);
            err = 0;
            جاؤ failed;
        }
    }

#اگر_ختم

    اگر (ext->وقت != -1) {
        اگر (ngx_سیٹ_file_وقت(src->ڈیٹا, ext->fd, ext->وقت) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, ngx_errno,
                          ngx_سیٹ_file_وقت_n " \"%s\" failed", src->ڈیٹا);
            err = 0;
            جاؤ failed;
        }
    }

    اگر (ngx_rename_file(src->ڈیٹا, مقصد->ڈیٹا) != NGX_FILE_ERROR) {
        واپس NGX_OK;
    }

    err = ngx_errno;

    اگر (err == NGX_ENOPATH) {

        اگر (!ext->create_راستہ) {
            جاؤ failed;
        }

        err = ngx_create_بھرا_راستہ(مقصد->ڈیٹا, ngx_dir_access(ext->راستہ_access));

        اگر (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, err,
                          ngx_create_dir_n " \"%s\" failed", مقصد->ڈیٹا);
            err = 0;
            جاؤ failed;
        }

        اگر (ngx_rename_file(src->ڈیٹا, مقصد->ڈیٹا) != NGX_FILE_ERROR) {
            واپس NGX_OK;
        }

        err = ngx_errno;
    }

#اگر (NGX_WIN32)

    اگر (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, مقصد, ext->لوگارتھم);

        اگر (err == 0) {
            واپس NGX_OK;
        }
    }

#اگر_ختم

    اگر (err == NGX_EXDEV) {

        cf.سائز = -1;
        cf.buf_سائز = 0;
        cf.access = ext->access;
        cf.وقت = ext->وقت;
        cf.لوگارتھم = ext->لوگارتھم;

        name = ngx_alloc(مقصد->len + 1 + 10 + 1, ext->لوگارتھم);
        اگر (name == تہی) {
            واپس NGX_ERROR;
        }

        (خالی) ngx_sprintf(name, "%*s.%010uD%Z", مقصد->len, مقصد->ڈیٹا,
                           (uint32_t) ngx_اگلا_عارضی_نمبر(0));

        اگر (ngx_copy_file(src->ڈیٹا, name, &cf) == NGX_OK) {

            اگر (ngx_rename_file(name, مقصد->ڈیٹا) != NGX_FILE_ERROR) {
                ngx_free(name);

                اگر (ngx_delete_file(src->ڈیٹا) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->ڈیٹا);
                    واپس NGX_ERROR;
                }

                واپس NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, مقصد->ڈیٹا);

            اگر (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    اگر (ext->delete_file) {
        اگر (ngx_delete_file(src->ڈیٹا) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->ڈیٹا);
        }
    }

    اگر (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->لوگارتھم, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->ڈیٹا, مقصد->ڈیٹا);
    }

    واپس NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *مقصد, ngx_copy_file_t *cf)
{
    حرف             *buf;
    off_t             سائز;
    وقت_t            وقت;
    سائز_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = تہی;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    اگر (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->لوگارتھم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        جاؤ failed;
    }

    اگر (cf->سائز != -1 && cf->access != 0 && cf->وقت != -1) {
        سائز = cf->سائز;
        access = cf->access;
        وقت = cf->وقت;

    } ورنہ {
        اگر (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            جاؤ failed;
        }

        سائز = (cf->سائز != -1) ? cf->سائز : ngx_file_سائز(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        وقت = (cf->وقت != -1) ? cf->وقت : ngx_file_mtime(&fi);
    }

    len = cf->buf_سائز ? cf->buf_سائز : 65536;

    اگر ((off_t) len > سائز) {
        len = (سائز_t) سائز;
    }

    buf = ngx_alloc(len, cf->لوگارتھم);
    اگر (buf == تہی) {
        جاؤ failed;
    }

    nfd = ngx_open_file(مقصد, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    اگر (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->لوگارتھم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", مقصد);
        جاؤ failed;
    }

    جب (سائز > 0) {

        اگر ((off_t) len > سائز) {
            len = (سائز_t) سائز;
        }

        n = ngx_read_fd(fd, buf, len);

        اگر (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            جاؤ failed;
        }

        اگر ((سائز_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, سائز, from);
            جاؤ failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        اگر (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", مقصد);
            جاؤ failed;
        }

        اگر ((سائز_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, سائز, مقصد);
            جاؤ failed;
        }

        سائز -= n;
    }

    اگر (ngx_سیٹ_file_وقت(مقصد, nfd, وقت) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
                      ngx_سیٹ_file_وقت_n " \"%s\" failed", مقصد);
        جاؤ failed;
    }

    rc = NGX_OK;

failed:

    اگر (nfd != NGX_INVALID_FILE) {
        اگر (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", مقصد);
        }
    }

    اگر (fd != NGX_INVALID_FILE) {
        اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    اگر (buf) {
        ngx_free(buf);
    }

    واپس rc;
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
    خالی       *ڈیٹا, *پچھلا;
    u_char     *p, *name;
    سائز_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                   "walk tree \"%V\"", درخت);

    اگر (ngx_open_dir(درخت, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->لوگارتھم, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", درخت->ڈیٹا);
        واپس NGX_ERROR;
    }

    پچھلا = ctx->ڈیٹا;

    اگر (ctx->alloc) {
        ڈیٹا = ngx_alloc(ctx->alloc, ctx->لوگارتھم);
        اگر (ڈیٹا == تہی) {
            جاؤ failed;
        }

        اگر (ctx->init_handler(ڈیٹا, پچھلا) == NGX_ABORT) {
            جاؤ failed;
        }

        ctx->ڈیٹا = ڈیٹا;

    } ورنہ {
        ڈیٹا = تہی;
    }

    برائے ( ;; ) {

        ngx_سیٹ_errno(0);

        اگر (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            اگر (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } ورنہ {
                ngx_log_error(NGX_LOG_CRIT, ctx->لوگارتھم, err,
                              ngx_read_dir_n " \"%s\" failed", درخت->ڈیٹا);
                rc = NGX_ERROR;
            }

            جاؤ مکمل;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                      "tree name %uz:\"%s\"", len, name);

        اگر (len == 1 && name[0] == '.') {
            جاری;
        }

        اگر (len == 2 && name[0] == '.' && name[1] == '.') {
            جاری;
        }

        file.len = درخت->len + 1 + len;

        اگر (file.len > buf.len) {

            اگر (buf.len) {
                ngx_free(buf.ڈیٹا);
            }

            buf.len = درخت->len + 1 + len;

            buf.ڈیٹا = ngx_alloc(buf.len + 1, ctx->لوگارتھم);
            اگر (buf.ڈیٹا == تہی) {
                جاؤ failed;
            }
        }

        p = ngx_cpymem(buf.ڈیٹا, درخت->ڈیٹا, درخت->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.ڈیٹا = buf.ڈیٹا;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                       "tree path \"%s\"", file.ڈیٹا);

        اگر (!dir.valid_info) {
            اگر (ngx_de_info(file.ڈیٹا, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->لوگارتھم, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.ڈیٹا);
                جاری;
            }
        }

        اگر (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                           "tree file \"%s\"", file.ڈیٹا);

            ctx->سائز = ngx_de_سائز(&dir);
            ctx->fs_سائز = ngx_de_fs_سائز(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            اگر (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                جاؤ failed;
            }

        } ورنہ اگر (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                           "tree enter dir \"%s\"", file.ڈیٹا);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_درخت_handler(ctx, &file);

            اگر (rc == NGX_ABORT) {
                جاؤ failed;
            }

            اگر (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                               "tree skip dir \"%s\"", file.ڈیٹا);
                جاری;
            }

            اگر (ngx_walk_درخت(ctx, &file) == NGX_ABORT) {
                جاؤ failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            اگر (ctx->post_درخت_handler(ctx, &file) == NGX_ABORT) {
                جاؤ failed;
            }

        } ورنہ {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لوگارتھم, 0,
                           "tree special \"%s\"", file.ڈیٹا);

            اگر (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                جاؤ failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

مکمل:

    اگر (buf.len) {
        ngx_free(buf.ڈیٹا);
    }

    اگر (ڈیٹا) {
        ngx_free(ڈیٹا);
        ctx->ڈیٹا = پچھلا;
    }

    اگر (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->لوگارتھم, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", درخت->ڈیٹا);
    }

    واپس rc;
}
