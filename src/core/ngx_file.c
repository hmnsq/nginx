
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ساكن ngx_int_t ngx_test_پر_name(ngx_str_t *name);


ساكن ngx_atomic_t   موقت_عدد = 0;
ngx_atomic_t         *ngx_موقت_عدد = &موقت_عدد;
ngx_atomic_int_t      ngx_random_عدد = 123456;


ngx_int_t
ngx_get_پر_name(ngx_pool_t *pool, ngx_str_t *پیشوند, ngx_str_t *name)
{
    اندازه_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_پر_name(name);

    إذا (rc == NGX_OK) {
        أرجع rc;
    }

    len = پیشوند->len;

#إذا (NGX_WIN32)

    إذا (rc == 2) {
        len = rc;
    }

#پایان_اگر

    n = ngx_pnalloc(pool, len + name->len + 1);
    إذا (n == عدم) {
        أرجع NGX_ERROR;
    }

    p = ngx_cpymem(n, پیشوند->داده, len);
    ngx_cpystrn(p, name->داده, name->len + 1);

    name->len += len;
    name->داده = n;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_test_پر_name(ngx_str_t *name)
{
#إذا (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->داده[0];

    إذا (name->len < 2) {
        إذا (c0 == '/') {
            أرجع 2;
        }

        أرجع NGX_DECLINED;
    }

    c1 = name->داده[1];

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

    إذا (name->داده[0] == '/') {
        أرجع NGX_OK;
    }

    أرجع NGX_DECLINED;

#پایان_اگر
}


ssize_t
ngx_write_chain_مقصد_موقت_file(ngx_موقت_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    إذا (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_موقت_file(&tf->file, tf->مسیر, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        إذا (rc != NGX_OK) {
            أرجع rc;
        }

        إذا (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.لگاریتم, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#إذا (NGX_نخها && NGX_HAVE_PWRITEV)

    إذا (tf->thread_write) {
        أرجع ngx_thread_write_chain_مقصد_file(&tf->file, chain, tf->جابه_جایی,
                                              tf->pool);
    }

#پایان_اگر

    أرجع ngx_write_chain_مقصد_file(&tf->file, chain, tf->جابه_جایی, tf->pool);
}


ngx_int_t
ngx_create_موقت_file(ngx_file_t *file, ngx_مسیر_t *مسیر, ngx_pool_t *pool,
    ngx_uint_t persistent, ngx_uint_t clean, ngx_uint_t access)
{
    اندازه_t                    سطوح;
    u_char                   *p;
    uint32_t                  n;
    ngx_err_t                 err;
    ngx_str_t                 name;
    ngx_uint_t                پیشوند;
    ngx_pool_cleanup_t       *cln;
    ngx_pool_cleanup_file_t  *clnf;

    إذا (file->name.len) {
        name = file->name;
        سطوح = 0;
        پیشوند = 1;

    } وإلا {
        name = مسیر->name;
        سطوح = مسیر->len;
        پیشوند = 0;
    }

    file->name.len = name.len + 1 + سطوح + 10;

    file->name.داده = ngx_pnalloc(pool, file->name.len + 1);
    إذا (file->name.داده == عدم) {
        أرجع NGX_ERROR;
    }

#إذا 0
    لكل (i = 0; i < file->name.len; i++) {
        file->name.داده[i] = 'X';
    }
#پایان_اگر

    p = ngx_cpymem(file->name.داده, name.داده, name.len);

    إذا (پیشوند) {
        *p = '.';
    }

    p += 1 + سطوح;

    n = (uint32_t) ngx_بعدی_موقت_عدد(0);

    cln = ngx_pool_cleanup_جمع_کن(pool, حجم(ngx_pool_cleanup_file_t));
    إذا (cln == عدم) {
        أرجع NGX_ERROR;
    }

    لكل ( ;; ) {
        (فراغ) ngx_sprintf(p, "%010uD%Z", n);

        إذا (!پیشوند) {
            ngx_create_hashed_filename(مسیر, file->name.داده, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لگاریتم, 0,
                       "hashed path: %s", file->name.داده);

        file->fd = ngx_open_tempfile(file->name.داده, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لگاریتم, 0,
                       "temp fd:%d", file->fd);

        إذا (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->داده;

            clnf->fd = file->fd;
            clnf->name = file->name.داده;
            clnf->لگاریتم = pool->لگاریتم;

            أرجع NGX_OK;
        }

        err = ngx_errno;

        إذا (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_بعدی_موقت_عدد(1);
            استمر;
        }

        إذا ((مسیر->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->لگاریتم, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.داده);
            أرجع NGX_ERROR;
        }

        إذا (ngx_create_مسیر(file, مسیر) == NGX_ERROR) {
            أرجع NGX_ERROR;
        }
    }
}


فراغ
ngx_create_hashed_filename(ngx_مسیر_t *مسیر, u_char *file, اندازه_t len)
{
    اندازه_t      i, level;
    ngx_uint_t  n;

    i = مسیر->name.len + 1;

    file[مسیر->name.len + مسیر->len]  = '/';

    لكل (n = 0; n < NGX_MAX_مسیر_LEVEL; n++) {
        level = مسیر->level[n];

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
ngx_create_مسیر(ngx_file_t *file, ngx_مسیر_t *مسیر)
{
    اندازه_t      pos;
    ngx_err_t   err;
    ngx_uint_t  i;

    pos = مسیر->name.len;

    لكل (i = 0; i < NGX_MAX_مسیر_LEVEL; i++) {
        إذا (مسیر->level[i] == 0) {
            اكسر;
        }

        pos += مسیر->level[i] + 1;

        file->name.داده[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->لگاریتم, 0,
                       "temp file: \"%s\"", file->name.داده);

        إذا (ngx_create_dir(file->name.داده, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            إذا (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->لگاریتم, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.داده);
                أرجع NGX_ERROR;
            }
        }

        file->name.داده[pos] = '/';
    }

    أرجع NGX_OK;
}


ngx_err_t
ngx_create_پر_مسیر(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#إذا (NGX_WIN32)
    p = dir + 3;
#وإلا
    p = dir + 1;
#پایان_اگر

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
ngx_بعدی_موقت_عدد(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, جمع_کن;

    جمع_کن = collision ? ngx_random_عدد : 1;

    n = ngx_atomic_fetch_جمع_کن(ngx_موقت_عدد, جمع_کن);

    أرجع n + جمع_کن;
}


حرف *
ngx_conf_مجموعه_مسیر_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *p = conf;

    ssize_t      level;
    ngx_str_t   *مقدار;
    ngx_uint_t   i, n;
    ngx_مسیر_t  *مسیر, **جایگاه;

    جایگاه = (ngx_مسیر_t **) (p + cmd->جابه_جایی);

    إذا (*جایگاه) {
        أرجع "is duplicate";
    }

    مسیر = ngx_pcalloc(cf->pool, حجم(ngx_مسیر_t));
    إذا (مسیر == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    مقدار = cf->آرگومانها->elts;

    مسیر->name = مقدار[1];

    إذا (مسیر->name.داده[مسیر->name.len - 1] == '/') {
        مسیر->name.len--;
    }

    إذا (ngx_conf_پر_name(cf->چرخه, &مسیر->name, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    مسیر->conf_file = cf->conf_file->file.name.داده;
    مسیر->سطر = cf->conf_file->سطر;

    لكل (i = 0, n = 2; n < cf->آرگومانها->nelts; i++, n++) {
        level = ngx_atoi(مقدار[n].داده, مقدار[n].len);
        إذا (level == NGX_ERROR || level == 0) {
            أرجع "invalid value";
        }

        مسیر->level[i] = level;
        مسیر->len += level + 1;
    }

    إذا (مسیر->len > 10 + i) {
        أرجع "invalid value";
    }

    *جایگاه = مسیر;

    إذا (ngx_جمع_کن_مسیر(cf, جایگاه) == NGX_ERROR) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_ادغام_مسیر_مقدار(ngx_conf_t *cf, ngx_مسیر_t **مسیر, ngx_مسیر_t *prev,
    ngx_مسیر_init_t *init)
{
    ngx_uint_t  i;

    إذا (*مسیر) {
        أرجع NGX_CONF_OK;
    }

    إذا (prev) {
        *مسیر = prev;
        أرجع NGX_CONF_OK;
    }

    *مسیر = ngx_pcalloc(cf->pool, حجم(ngx_مسیر_t));
    إذا (*مسیر == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    (*مسیر)->name = init->name;

    إذا (ngx_conf_پر_name(cf->چرخه, &(*مسیر)->name, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    لكل (i = 0; i < NGX_MAX_مسیر_LEVEL; i++) {
        (*مسیر)->level[i] = init->level[i];
        (*مسیر)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    إذا (ngx_جمع_کن_مسیر(cf, مسیر) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    أرجع NGX_CONF_OK;
}


حرف *
ngx_conf_مجموعه_access_جایگاه(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    حرف  *confp = conf;

    u_char      *p;
    ngx_str_t   *مقدار;
    ngx_uint_t   i, راست, لغزش, *access, user;

    access = (ngx_uint_t *) (confp + cmd->جابه_جایی);

    إذا (*access != NGX_CONF_UNSET_UINT) {
        أرجع "is duplicate";
    }

    مقدار = cf->آرگومانها->elts;

    *access = 0;
    user = 0600;

    لكل (i = 1; i < cf->آرگومانها->nelts; i++) {

        p = مقدار[i].داده;

        إذا (ngx_strncmp(p, "user:", حجم("user:") - 1) == 0) {
            لغزش = 6;
            p += حجم("user:") - 1;
            user = 0;

        } وإلا إذا (ngx_strncmp(p, "group:", حجم("group:") - 1) == 0) {
            لغزش = 3;
            p += حجم("group:") - 1;

        } وإلا إذا (ngx_strncmp(p, "all:", حجم("all:") - 1) == 0) {
            لغزش = 0;
            p += حجم("all:") - 1;

        } وإلا {
            اذهب invalid;
        }

        إذا (ngx_strcmp(p, "rw") == 0) {
            راست = 6;

        } وإلا إذا (ngx_strcmp(p, "r") == 0) {
            راست = 4;

        } وإلا {
            اذهب invalid;
        }

        *access |= راست << لغزش;
    }

    *access |= user;

    أرجع NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &مقدار[i]);

    أرجع NGX_CONF_ERROR;
}


ngx_int_t
ngx_جمع_کن_مسیر(ngx_conf_t *cf, ngx_مسیر_t **جایگاه)
{
    ngx_uint_t   i, n;
    ngx_مسیر_t  *مسیر, **p;

    مسیر = *جایگاه;

    p = cf->چرخه->paths.elts;
    لكل (i = 0; i < cf->چرخه->paths.nelts; i++) {
        إذا (p[i]->name.len == مسیر->name.len
            && ngx_strcmp(p[i]->name.داده, مسیر->name.داده) == 0)
        {
            إذا (p[i]->داده != مسیر->داده) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->سطر);
                أرجع NGX_ERROR;
            }

            لكل (n = 0; n < NGX_MAX_مسیر_LEVEL; n++) {
                إذا (p[i]->level[n] != مسیر->level[n]) {
                    إذا (مسیر->conf_file == عدم) {
                        إذا (p[i]->conf_file == عدم) {
                            ngx_log_error(NGX_LOG_EMERG, cf->لگاریتم, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            أرجع NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->لگاریتم, 0,
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

            *جایگاه = p[i];

            أرجع NGX_OK;
        }
    }

    p = ngx_array_افزودن(&cf->چرخه->paths);
    إذا (p == عدم) {
        أرجع NGX_ERROR;
    }

    *p = مسیر;

    أرجع NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_چرخه_t *چرخه, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_مسیر_t      **مسیر;

    مسیر = چرخه->paths.elts;
    لكل (i = 0; i < چرخه->paths.nelts; i++) {

        إذا (ngx_create_dir(مسیر[i]->name.داده, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            إذا (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, err,
                              ngx_create_dir_n " \"%s\" failed",
                              مسیر[i]->name.داده);
                أرجع NGX_ERROR;
            }
        }

        إذا (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            استمر;
        }

#إذا !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        إذا (ngx_file_info(مسیر[i]->name.داده, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", مسیر[i]->name.داده);
            أرجع NGX_ERROR;
        }

        إذا (fi.st_uid != user) {
            إذا (chown((ثابت حرف *) مسیر[i]->name.داده, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              مسیر[i]->name.داده, user);
                أرجع NGX_ERROR;
            }
        }

        إذا ((fi.st_نما & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_نما |= (S_IRUSR|S_IWUSR|S_IXUSR);

            إذا (chmod((ثابت حرف *) مسیر[i]->name.داده, fi.st_نما) == -1) {
                ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                              "chmod() \"%s\" failed", مسیر[i]->name.داده);
                أرجع NGX_ERROR;
            }
        }
        }
#پایان_اگر
    }

    أرجع NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *مقصد, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#إذا !(NGX_WIN32)

    إذا (ext->access) {
        إذا (ngx_change_file_access(src->داده, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->لگاریتم, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->داده);
            err = 0;
            اذهب failed;
        }
    }

#پایان_اگر

    إذا (ext->زمان != -1) {
        إذا (ngx_مجموعه_file_زمان(src->داده, ext->fd, ext->زمان) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->لگاریتم, ngx_errno,
                          ngx_مجموعه_file_زمان_n " \"%s\" failed", src->داده);
            err = 0;
            اذهب failed;
        }
    }

    إذا (ngx_rename_file(src->داده, مقصد->داده) != NGX_FILE_ERROR) {
        أرجع NGX_OK;
    }

    err = ngx_errno;

    إذا (err == NGX_ENOPATH) {

        إذا (!ext->create_مسیر) {
            اذهب failed;
        }

        err = ngx_create_پر_مسیر(مقصد->داده, ngx_dir_access(ext->مسیر_access));

        إذا (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->لگاریتم, err,
                          ngx_create_dir_n " \"%s\" failed", مقصد->داده);
            err = 0;
            اذهب failed;
        }

        إذا (ngx_rename_file(src->داده, مقصد->داده) != NGX_FILE_ERROR) {
            أرجع NGX_OK;
        }

        err = ngx_errno;
    }

#إذا (NGX_WIN32)

    إذا (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, مقصد, ext->لگاریتم);

        إذا (err == 0) {
            أرجع NGX_OK;
        }
    }

#پایان_اگر

    إذا (err == NGX_EXDEV) {

        cf.اندازه = -1;
        cf.buf_اندازه = 0;
        cf.access = ext->access;
        cf.زمان = ext->زمان;
        cf.لگاریتم = ext->لگاریتم;

        name = ngx_alloc(مقصد->len + 1 + 10 + 1, ext->لگاریتم);
        إذا (name == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_sprintf(name, "%*s.%010uD%Z", مقصد->len, مقصد->داده,
                           (uint32_t) ngx_بعدی_موقت_عدد(0));

        إذا (ngx_copy_file(src->داده, name, &cf) == NGX_OK) {

            إذا (ngx_rename_file(name, مقصد->داده) != NGX_FILE_ERROR) {
                ngx_free(name);

                إذا (ngx_delete_file(src->داده) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->لگاریتم, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->داده);
                    أرجع NGX_ERROR;
                }

                أرجع NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->لگاریتم, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, مقصد->داده);

            إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->لگاریتم, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    إذا (ext->delete_file) {
        إذا (ngx_delete_file(src->داده) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->لگاریتم, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->داده);
        }
    }

    إذا (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->لگاریتم, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->داده, مقصد->داده);
    }

    أرجع NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *مقصد, ngx_copy_file_t *cf)
{
    حرف             *buf;
    off_t             اندازه;
    زمان_t            زمان;
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
        ngx_log_error(NGX_LOG_CRIT, cf->لگاریتم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        اذهب failed;
    }

    إذا (cf->اندازه != -1 && cf->access != 0 && cf->زمان != -1) {
        اندازه = cf->اندازه;
        access = cf->access;
        زمان = cf->زمان;

    } وإلا {
        إذا (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لگاریتم, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            اذهب failed;
        }

        اندازه = (cf->اندازه != -1) ? cf->اندازه : ngx_file_اندازه(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        زمان = (cf->زمان != -1) ? cf->زمان : ngx_file_mtime(&fi);
    }

    len = cf->buf_اندازه ? cf->buf_اندازه : 65536;

    إذا ((off_t) len > اندازه) {
        len = (اندازه_t) اندازه;
    }

    buf = ngx_alloc(len, cf->لگاریتم);
    إذا (buf == عدم) {
        اذهب failed;
    }

    nfd = ngx_open_file(مقصد, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    إذا (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->لگاریتم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", مقصد);
        اذهب failed;
    }

    طالما (اندازه > 0) {

        إذا ((off_t) len > اندازه) {
            len = (اندازه_t) اندازه;
        }

        n = ngx_read_fd(fd, buf, len);

        إذا (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->لگاریتم, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            اذهب failed;
        }

        إذا ((اندازه_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->لگاریتم, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, اندازه, from);
            اذهب failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        إذا (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->لگاریتم, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", مقصد);
            اذهب failed;
        }

        إذا ((اندازه_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->لگاریتم, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, اندازه, مقصد);
            اذهب failed;
        }

        اندازه -= n;
    }

    إذا (ngx_مجموعه_file_زمان(مقصد, nfd, زمان) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->لگاریتم, ngx_errno,
                      ngx_مجموعه_file_زمان_n " \"%s\" failed", مقصد);
        اذهب failed;
    }

    rc = NGX_OK;

failed:

    إذا (nfd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لگاریتم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", مقصد);
        }
    }

    إذا (fd != NGX_INVALID_FILE) {
        إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->لگاریتم, ngx_errno,
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
    فراغ       *داده, *prev;
    u_char     *p, *name;
    اندازه_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لگاریتم, 0,
                   "walk tree \"%V\"", درخت);

    إذا (ngx_open_dir(درخت, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->لگاریتم, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", درخت->داده);
        أرجع NGX_ERROR;
    }

    prev = ctx->داده;

    إذا (ctx->alloc) {
        داده = ngx_alloc(ctx->alloc, ctx->لگاریتم);
        إذا (داده == عدم) {
            اذهب failed;
        }

        إذا (ctx->init_handler(داده, prev) == NGX_ABORT) {
            اذهب failed;
        }

        ctx->داده = داده;

    } وإلا {
        داده = عدم;
    }

    لكل ( ;; ) {

        ngx_مجموعه_errno(0);

        إذا (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            إذا (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } وإلا {
                ngx_log_error(NGX_LOG_CRIT, ctx->لگاریتم, err,
                              ngx_read_dir_n " \"%s\" failed", درخت->داده);
                rc = NGX_ERROR;
            }

            اذهب انجامشده;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->لگاریتم, 0,
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
                ngx_free(buf.داده);
            }

            buf.len = درخت->len + 1 + len;

            buf.داده = ngx_alloc(buf.len + 1, ctx->لگاریتم);
            إذا (buf.داده == عدم) {
                اذهب failed;
            }
        }

        p = ngx_cpymem(buf.داده, درخت->داده, درخت->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.داده = buf.داده;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لگاریتم, 0,
                       "tree path \"%s\"", file.داده);

        إذا (!dir.valid_info) {
            إذا (ngx_de_info(file.داده, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->لگاریتم, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.داده);
                استمر;
            }
        }

        إذا (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لگاریتم, 0,
                           "tree file \"%s\"", file.داده);

            ctx->اندازه = ngx_de_اندازه(&dir);
            ctx->fs_اندازه = ngx_de_fs_اندازه(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            إذا (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }

        } وإلا إذا (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لگاریتم, 0,
                           "tree enter dir \"%s\"", file.داده);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_درخت_handler(ctx, &file);

            إذا (rc == NGX_ABORT) {
                اذهب failed;
            }

            إذا (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لگاریتم, 0,
                               "tree skip dir \"%s\"", file.داده);
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

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->لگاریتم, 0,
                           "tree special \"%s\"", file.داده);

            إذا (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                اذهب failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

انجامشده:

    إذا (buf.len) {
        ngx_free(buf.داده);
    }

    إذا (داده) {
        ngx_free(داده);
        ctx->داده = prev;
    }

    إذا (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->لگاریتم, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", درخت->داده);
    }

    أرجع rc;
}
