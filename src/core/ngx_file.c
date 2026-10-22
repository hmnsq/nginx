
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_int_t ngx_test_ڀريل_name(ngx_str_t *name);


ساكن ngx_atomic_t   عارضي_عدد = 0;
ngx_atomic_t         *ngx_عارضي_عدد = &عارضي_عدد;
ngx_atomic_int_t      ngx_random_عدد = 123456;


ngx_int_t
ngx_get_ڀريل_name(ngx_pool_t *pool, ngx_str_t *اڳياڙي, ngx_str_t *name)
{
    سائيز_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_ڀريل_name(name);

    إذا (rc == NGX_OK) {
        أرجع rc;
    }

    len = اڳياڙي->len;

#إذا (NGX_WIN32)

    إذا (rc == 2) {
        len = rc;
    }

#جيڪڏهن_پورو

    n = ngx_pnalloc(pool, len + name->len + 1);
    إذا (n == عدم) {
        أرجع NGX_ERROR;
    }

    p = ngx_cpymem(n, اڳياڙي->معلومات, len);
    ngx_cpystrn(p, name->معلومات, name->len + 1);

    name->len += len;
    name->معلومات = n;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_test_ڀريل_name(ngx_str_t *name)
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

#جيڪڏهن_پورو
}


ssize_t
ngx_write_chain_منزل_عارضي_file(ngx_عارضي_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    إذا (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_عارضي_file(&tf->file, tf->رستو, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        إذا (rc != NGX_OK) {
            أرجع rc;
        }

        إذا (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.لاگرٿم, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#إذا (NGX_ڌاڳا && NGX_HAVE_PWRITEV)

    إذا (tf->thread_write) {
        أرجع ngx_thread_write_chain_منزل_file(&tf->file, chain, tf->وٿي,
                                              tf->pool);
    }

#جيڪڏهن_پورو

    أرجع ngx_write_chain_منزل_file(&tf->file, chain, tf->وٿي, tf->pool);
}


ngx_int_t
ngx_create_عارضي_file(ngx_file_t *file, ngx_رستو_t *رستو, ngx_pool_t *pool,
    ngx_uint_t persistent, ngx_uint_t clean, ngx_uint_t access)
{
    سائيز_t                    سطحون;
    u_char                   *p;
    uint32_t                  n;
    ngx_err_t                 err;
    ngx_str_t                 name;
    ngx_uint_t                اڳياڙي;
    ngx_pool_cleanup_t       *cln;
    ngx_pool_cleanup_file_t  *clnf;

    إذا (file->name.len) {
        name = file->name;
        سطحون = 0;
        اڳياڙي = 1;

    } وإلا {
        name = رستو->name;
        سطحون = رستو->len;
        اڳياڙي = 0;
    }

    file->name.len = name.len + 1 + سطحون + 10;

    file->name.معلومات = ngx_pnalloc(pool, file->name.len + 1);
    إذا (file->name.معلومات == عدم) {
        أرجع NGX_ERROR;
    }

#إذا 0
    لكل (i = 0; i < file->name.len; i++) {
        file->name.معلومات[i] = 'X';
    }
#جيڪڏهن_پورو

    p = ngx_cpymem(file->name.معلومات, name.معلومات, name.len);

    إذا (اڳياڙي) {
        *p = '.';
    }

    p += 1 + سطحون;

    n = (uint32_t) ngx_ايندڙ_عارضي_عدد(0);

    cln = ngx_pool_cleanup_جوڙيو(pool, حجم(ngx_pool_cleanup_file_t));
    إذا (cln == عدم) {
        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        (فراغ) ngx_sprintf(p, "%010uD%Z", n);

        إذا (!اڳياڙي) {
            ngx_create_hashed_filename(رستو, file->name.معلومات, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لاگرٿم, 0,
                       "hashed path: %s", file->name.معلومات);

        file->fd = ngx_open_tempfile(file->name.معلومات, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لاگرٿم, 0,
                       "temp fd:%d", file->fd);

        إذا (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->معلومات;

            clnf->fd = file->fd;
            clnf->name = file->name.معلومات;
            clnf->لاگرٿم = pool->لاگرٿم;

            أرجع NGX_OK;
        }

        err = ngx_errno;

        إذا (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_ايندڙ_عارضي_عدد(1);
            استمر;
        }

        إذا ((رستو->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->لاگرٿم, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.معلومات);
            أرجع NGX_ERROR;
        }

        إذا (ngx_create_رستو(file, رستو) == NGX_ERROR) {
            أرجع NGX_ERROR;
        }
    }
}


فراغ
ngx_create_hashed_filename(ngx_رستو_t *رستو, u_char *file, سائيز_t len)
{
    سائيز_t      i, level;
    ngx_uint_t  n;

    i = رستو->name.len + 1;

    file[رستو->name.len + رستو->len]  = '/';

    لكل (n = 0; n < NGX_MAX_رستو_LEVEL; n++) {
        level = رستو->level[n];

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
ngx_create_رستو(ngx_file_t *file, ngx_رستو_t *رستو)
{
    سائيز_t      pos;
    ngx_err_t   err;
    ngx_uint_t  i;

    pos = رستو->name.len;

    لكل (i = 0; i < NGX_MAX_رستو_LEVEL; i++) {
        إذا (رستو->level[i] == 0) {
            اكسر;
        }

        pos += رستو->level[i] + 1;

        file->name.معلومات[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لاگرٿم, 0,
                       "temp file: \"%s\"", file->name.معلومات);

        إذا (ngx_create_dir(file->name.معلومات, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            إذا (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->لاگرٿم, err,
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
ngx_create_ڀريل_رستو(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#إذا (NGX_WIN32)
    p = dir + 3;
#وإلا
    p = dir + 1;
#جيڪڏهن_پورو

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
ngx_ايندڙ_عارضي_عدد(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, جوڙيو;

    جوڙيو = collision ? ngx_random_عدد : 1;

    n = ngx_atomic_fetch_جوڙيو(ngx_عارضي_عدد, جوڙيو);

    أرجع n + جوڙيو;
}


حرف *
ngx_conf_سيٽ_رستو_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ssize_t      level;
    ngx_str_t   *قدر;
    ngx_uint_t   i, n;
    ngx_رستو_t  *رستو, **جڳهه;

    جڳهه = (ngx_رستو_t **) (p + cmd->وٿي);

    إذا (*جڳهه) {
        أرجع "is duplicate";
    }

    رستو = ngx_pcalloc(cf->pool, حجم(ngx_رستو_t));
    إذا (رستو == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    قدر = cf->دليلون->elts;

    رستو->name = قدر[1];

    إذا (رستو->name.معلومات[رستو->name.len - 1] == '/') {
        رستو->name.len--;
    }

    إذا (ngx_conf_ڀريل_name(cf->چڪر, &رستو->name, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    رستو->conf_file = cf->conf_file->file.name.معلومات;
    رستو->سٽ = cf->conf_file->سٽ;

    لكل (i = 0, n = 2; n < cf->دليلون->nelts; i++, n++) {
        level = ngx_atoi(قدر[n].معلومات, قدر[n].len);
        إذا (level == NGX_ERROR || level == 0) {
            أرجع "invalid value";
        }

        رستو->level[i] = level;
        رستو->len += level + 1;
    }

    إذا (رستو->len > 10 + i) {
        أرجع "invalid value";
    }

    *جڳهه = رستو;

    إذا (ngx_جوڙيو_رستو(cf, جڳهه) == NGX_ERROR) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_ملايو_رستو_قدر(ngx_conf_t *cf, ngx_رستو_t **رستو, ngx_رستو_t *prev,
    ngx_رستو_init_t *init)
{
    ngx_uint_t  i;

    إذا (*رستو) {
        أرجع NGX_CONF_OK;
    }

    إذا (prev) {
        *رستو = prev;
        أرجع NGX_CONF_OK;
    }

    *رستو = ngx_pcalloc(cf->pool, حجم(ngx_رستو_t));
    إذا (*رستو == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    (*رستو)->name = init->name;

    إذا (ngx_conf_ڀريل_name(cf->چڪر, &(*رستو)->name, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    لكل (i = 0; i < NGX_MAX_رستو_LEVEL; i++) {
        (*رستو)->level[i] = init->level[i];
        (*رستو)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    إذا (ngx_جوڙيو_رستو(cf, رستو) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_سيٽ_access_جڳهه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *confp = conf;

    u_char      *p;
    ngx_str_t   *قدر;
    ngx_uint_t   i, ساڄو, سرڪايو, *access, user;

    access = (ngx_uint_t *) (confp + cmd->وٿي);

    إذا (*access != NGX_CONF_UNSET_UINT) {
        أرجع "is duplicate";
    }

    قدر = cf->دليلون->elts;

    *access = 0;
    user = 0600;

    لكل (i = 1; i < cf->دليلون->nelts; i++) {

        p = قدر[i].معلومات;

        إذا (ngx_strncmp(p, "user:", حجم("user:") - 1) == 0) {
            سرڪايو = 6;
            p += حجم("user:") - 1;
            user = 0;

        } وإلا إذا (ngx_strncmp(p, "group:", حجم("group:") - 1) == 0) {
            سرڪايو = 3;
            p += حجم("group:") - 1;

        } وإلا إذا (ngx_strncmp(p, "all:", حجم("all:") - 1) == 0) {
            سرڪايو = 0;
            p += حجم("all:") - 1;

        } وإلا {
            اذهب invalid;
        }

        إذا (ngx_strcmp(p, "rw") == 0) {
            ساڄو = 6;

        } وإلا إذا (ngx_strcmp(p, "r") == 0) {
            ساڄو = 4;

        } وإلا {
            اذهب invalid;
        }

        *access |= ساڄو << سرڪايو;
    }

    *access |= user;

    أرجع NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &قدر[i]);

    أرجع NGX_CONF_ERROR;
}


ngx_int_t
ngx_جوڙيو_رستو(ngx_conf_t *cf, ngx_رستو_t **جڳهه)
{
    ngx_uint_t   i, n;
    ngx_رستو_t  *رستو, **p;

    رستو = *جڳهه;

    p = cf->چڪر->paths.elts;
    لكل (i = 0; i < cf->چڪر->paths.nelts; i++) {
        إذا (p[i]->name.len == رستو->name.len
            && ngx_strcmp(p[i]->name.معلومات, رستو->name.معلومات) == 0)
        {
            إذا (p[i]->معلومات != رستو->معلومات) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->سٽ);
                أرجع NGX_ERROR;
            }

            لكل (n = 0; n < NGX_MAX_رستو_LEVEL; n++) {
                إذا (p[i]->level[n] != رستو->level[n]) {
                    إذا (رستو->conf_file == عدم) {
                        إذا (p[i]->conf_file == عدم) {
                            ngx_log_error(NGX_LOG_EMERG, cf->لاگرٿم, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            أرجع NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->لاگرٿم, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->سٽ);
                        أرجع NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->سٽ);
                    أرجع NGX_ERROR;
                }

                إذا (p[i]->level[n] == 0) {
                    اكسر;
                }
            }

            *جڳهه = p[i];

            أرجع NGX_OK;
        }
    }

    p = ngx_array_ڌڪو(&cf->چڪر->paths);
    إذا (p == عدم) {
        أرجع NGX_ERROR;
    }

    *p = رستو;

    أرجع NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_چڪر_t *چڪر, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_رستو_t      **رستو;

    رستو = چڪر->paths.elts;
    لكل (i = 0; i < چڪر->paths.nelts; i++) {

        إذا (ngx_create_dir(رستو[i]->name.معلومات, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            إذا (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, err,
                              ngx_create_dir_n " \"%s\" failed",
                              رستو[i]->name.معلومات);
                أرجع NGX_ERROR;
            }
        }

        إذا (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            استمر;
        }

#إذا !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        إذا (ngx_file_info(رستو[i]->name.معلومات, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", رستو[i]->name.معلومات);
            أرجع NGX_ERROR;
        }

        إذا (fi.st_uid != user) {
            إذا (chown((ثابت حرف *) رستو[i]->name.معلومات, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              رستو[i]->name.معلومات, user);
                أرجع NGX_ERROR;
            }
        }

        إذا ((fi.st_گهڻائي & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_گهڻائي |= (S_IRUSR|S_IWUSR|S_IXUSR);

            إذا (chmod((ثابت حرف *) رستو[i]->name.معلومات, fi.st_گهڻائي) == -1) {
                ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                              "chmod() \"%s\" failed", رستو[i]->name.معلومات);
                أرجع NGX_ERROR;
            }
        }
        }
#جيڪڏهن_پورو
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
            ngx_log_error(NGX_LOG_CRIT, ext->لاگرٿم, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->معلومات);
            err = 0;
            اذهب failed;
        }
    }

#جيڪڏهن_پورو

    إذا (ext->وقت != -1) {
        إذا (ngx_سيٽ_file_وقت(src->معلومات, ext->fd, ext->وقت) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->لاگرٿم, ngx_errno,
                          ngx_سيٽ_file_وقت_n " \"%s\" failed", src->معلومات);
            err = 0;
            اذهب failed;
        }
    }

    إذا (ngx_rename_file(src->معلومات, منزل->معلومات) != NGX_FILE_ERROR) {
        أرجع NGX_OK;
    }

    err = ngx_errno;

    إذا (err == NGX_ENOPATH) {

        إذا (!ext->create_رستو) {
            اذهب failed;
        }

        err = ngx_create_ڀريل_رستو(منزل->معلومات, ngx_dir_access(ext->رستو_access));

        إذا (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->لاگرٿم, err,
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
        err = ngx_win32_rename_file(src, منزل, ext->لاگرٿم);

        إذا (err == 0) {
            أرجع NGX_OK;
        }
    }

#جيڪڏهن_پورو

    إذا (err == NGX_EXDEV) {

        cf.سائيز = -1;
        cf.buf_سائيز = 0;
        cf.access = ext->access;
        cf.وقت = ext->وقت;
        cf.لاگرٿم = ext->لاگرٿم;

        name = ngx_alloc(منزل->len + 1 + 10 + 1, ext->لاگرٿم);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_sprintf(name, "%*s.%010uD%Z", منزل->len, منزل->معلومات,
                           (uint32_t) ngx_ايندڙ_عارضي_عدد(0));

        إذا (ngx_copy_file(src->معلومات, name, &cf) == NGX_OK) {

            إذا (ngx_rename_file(name, منزل->معلومات) != NGX_FILE_ERROR) {
                ngx_free(name);

                إذا (ngx_delete_file(src->معلومات) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->لاگرٿم, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->معلومات);
                    أرجع NGX_ERROR;
                }

                أرجع NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->لاگرٿم, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, منزل->معلومات);

            إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->لاگرٿم, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    إذا (ext->delete_file) {
        إذا (ngx_delete_file(src->معلومات) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->لاگرٿم, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->معلومات);
        }
    }

    إذا (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->لاگرٿم, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->معلومات, منزل->معلومات);
    }

    أرجع NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *منزل, ngx_copy_file_t *cf)
{
    حرف             *buf;
    off_t             سائيز;
    وقت_t            وقت;
    سائيز_t            len;
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
        ngx_log_error(NGX_LOG_CRIT, cf->لاگرٿم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        اذهب failed;
    }

    إذا (cf->سائيز != -1 && cf->access != 0 && cf->وقت != -1) {
        سائيز = cf->سائيز;
        access = cf->access;
        وقت = cf->وقت;

    } وإلا {
        إذا (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لاگرٿم, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            اذهب failed;
        }

        سائيز = (cf->سائيز != -1) ? cf->سائيز : ngx_file_سائيز(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        وقت = (cf->وقت != -1) ? cf->وقت : ngx_file_mtime(&fi);
    }

    len = cf->buf_سائيز ? cf->buf_سائيز : 65536;

    إذا ((off_t) len > سائيز) {
        len = (سائيز_t) سائيز;
    }

    buf = ngx_alloc(len, cf->لاگرٿم);
    إذا (buf == عدم) {
        اذهب failed;
    }

    nfd = ngx_open_file(منزل, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    إذا (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->لاگرٿم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", منزل);
        اذهب failed;
    }

    طالما (سائيز > 0) {

        إذا ((off_t) len > سائيز) {
            len = (سائيز_t) سائيز;
        }

        n = ngx_read_fd(fd, buf, len);

        إذا (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->لاگرٿم, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            اذهب failed;
        }

        إذا ((سائيز_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->لاگرٿم, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, سائيز, from);
            اذهب failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        إذا (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->لاگرٿم, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", منزل);
            اذهب failed;
        }

        إذا ((سائيز_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->لاگرٿم, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, سائيز, منزل);
            اذهب failed;
        }

        سائيز -= n;
    }

    إذا (ngx_سيٽ_file_وقت(منزل, nfd, وقت) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->لاگرٿم, ngx_errno,
                      ngx_سيٽ_file_وقت_n " \"%s\" failed", منزل);
        اذهب failed;
    }

    rc = NGX_OK;

failed:

    إذا (nfd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لاگرٿم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", منزل);
        }
    }

    إذا (fd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لاگرٿم, ngx_errno,
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
ngx_walk_وڻ(ngx_وڻ_ctx_t *ctx, ngx_str_t *وڻ)
{
    فراغ       *معلومات, *prev;
    u_char     *p, *name;
    سائيز_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لاگرٿم, 0,
                   "walk tree \"%V\"", وڻ);

    إذا (ngx_open_dir(وڻ, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->لاگرٿم, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", وڻ->معلومات);
        أرجع NGX_ERROR;
    }

    prev = ctx->معلومات;

    إذا (ctx->alloc) {
        معلومات = ngx_alloc(ctx->alloc, ctx->لاگرٿم);
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

        ngx_سيٽ_errno(0);

        إذا (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            إذا (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } وإلا {
                ngx_log_error(NGX_LOG_CRIT, ctx->لاگرٿم, err,
                              ngx_read_dir_n " \"%s\" failed", وڻ->معلومات);
                rc = NGX_ERROR;
            }

            اذهب پورو;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->لاگرٿم, 0,
                      "tree name %uz:\"%s\"", len, name);

        إذا (len == 1 && name[0] == '.') {
            استمر;
        }

        إذا (len == 2 && name[0] == '.' && name[1] == '.') {
            استمر;
        }

        file.len = وڻ->len + 1 + len;

        إذا (file.len > buf.len) {

            إذا (buf.len) {
                ngx_free(buf.معلومات);
            }

            buf.len = وڻ->len + 1 + len;

            buf.معلومات = ngx_alloc(buf.len + 1, ctx->لاگرٿم);
            إذا (buf.معلومات == عدم) {
                اذهب failed;
            }
        }

        p = ngx_cpymem(buf.معلومات, وڻ->معلومات, وڻ->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.معلومات = buf.معلومات;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لاگرٿم, 0,
                       "tree path \"%s\"", file.معلومات);

        إذا (!dir.valid_info) {
            إذا (ngx_de_info(file.معلومات, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->لاگرٿم, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.معلومات);
                استمر;
            }
        }

        إذا (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لاگرٿم, 0,
                           "tree file \"%s\"", file.معلومات);

            ctx->سائيز = ngx_de_سائيز(&dir);
            ctx->fs_سائيز = ngx_de_fs_سائيز(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            إذا (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

        } وإلا إذا (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لاگرٿم, 0,
                           "tree enter dir \"%s\"", file.معلومات);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_وڻ_handler(ctx, &file);

            إذا (rc == NGX_ABORT) {
                اذهب failed;
            }

            إذا (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لاگرٿم, 0,
                               "tree skip dir \"%s\"", file.معلومات);
                استمر;
            }

            إذا (ngx_walk_وڻ(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            إذا (ctx->post_وڻ_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

        } وإلا {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لاگرٿم, 0,
                           "tree special \"%s\"", file.معلومات);

            إذا (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

پورو:

    إذا (buf.len) {
        ngx_free(buf.معلومات);
    }

    إذا (معلومات) {
        ngx_free(معلومات);
        ctx->معلومات = prev;
    }

    إذا (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->لاگرٿم, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", وڻ->معلومات);
    }

    أرجع rc;
}
