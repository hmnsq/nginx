
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਸਥਿਰ ngx_int_t ngx_test_ਭਰਿਆ_name(ngx_str_t *name);


ਸਥਿਰ ngx_atomic_t   ਆਰਜ਼ੀ_ਸੰਖਿਆ = 0;
ngx_atomic_t         *ngx_ਆਰਜ਼ੀ_ਸੰਖਿਆ = &ਆਰਜ਼ੀ_ਸੰਖਿਆ;
ngx_atomic_int_t      ngx_random_ਸੰਖਿਆ = 123456;


ngx_int_t
ngx_get_ਭਰਿਆ_name(ngx_pool_t *pool, ngx_str_t *ਅਗੇਤਰ, ngx_str_t *name)
{
    ਮਾਪ_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_ਭਰਿਆ_name(name);

    ਜੇ (rc == NGX_OK) {
        ਵਾਪਸ rc;
    }

    len = ਅਗੇਤਰ->len;

#ਜੇ (NGX_WIN32)

    ਜੇ (rc == 2) {
        len = rc;
    }

#ਜੇ_ਅੰਤ

    n = ngx_pnalloc(pool, len + name->len + 1);
    ਜੇ (n == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    p = ngx_cpymem(n, ਅਗੇਤਰ->ਅੰਕੜੇ, len);
    ngx_cpystrn(p, name->ਅੰਕੜੇ, name->len + 1);

    name->len += len;
    name->ਅੰਕੜੇ = n;

    ਵਾਪਸ NGX_OK;
}


ਸਥਿਰ ngx_int_t
ngx_test_ਭਰਿਆ_name(ngx_str_t *name)
{
#ਜੇ (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->ਅੰਕੜੇ[0];

    ਜੇ (name->len < 2) {
        ਜੇ (c0 == '/') {
            ਵਾਪਸ 2;
        }

        ਵਾਪਸ NGX_DECLINED;
    }

    c1 = name->ਅੰਕੜੇ[1];

    ਜੇ (c1 == ':') {
        c0 |= 0x20;

        ਜੇ ((c0 >= 'a' && c0 <= 'z')) {
            ਵਾਪਸ NGX_OK;
        }

        ਵਾਪਸ NGX_DECLINED;
    }

    ਜੇ (c1 == '/') {
        ਵਾਪਸ NGX_OK;
    }

    ਜੇ (c0 == '/') {
        ਵਾਪਸ 2;
    }

    ਵਾਪਸ NGX_DECLINED;

#ਨਹੀਂਤਾਂ

    ਜੇ (name->ਅੰਕੜੇ[0] == '/') {
        ਵਾਪਸ NGX_OK;
    }

    ਵਾਪਸ NGX_DECLINED;

#ਜੇ_ਅੰਤ
}


ssize_t
ngx_write_chain_ਮੰਜ਼ਿਲ_ਆਰਜ਼ੀ_file(ngx_ਆਰਜ਼ੀ_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    ਜੇ (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_ਆਰਜ਼ੀ_file(&tf->file, tf->ਰਸਤਾ, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        ਜੇ (rc != NGX_OK) {
            ਵਾਪਸ rc;
        }

        ਜੇ (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.ਲਘੂਗਣਕ, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#ਜੇ (NGX_ਧਾਗੇ && NGX_HAVE_PWRITEV)

    ਜੇ (tf->thread_write) {
        ਵਾਪਸ ngx_thread_write_chain_ਮੰਜ਼ਿਲ_file(&tf->file, chain, tf->ਵਿੱਥ,
                                              tf->pool);
    }

#ਜੇ_ਅੰਤ

    ਵਾਪਸ ngx_write_chain_ਮੰਜ਼ਿਲ_file(&tf->file, chain, tf->ਵਿੱਥ, tf->pool);
}


ngx_int_t
ngx_create_ਆਰਜ਼ੀ_file(ngx_file_t *file, ngx_ਰਸਤਾ_t *ਰਸਤਾ, ngx_pool_t *pool,
    ngx_uint_t persistent, ngx_uint_t clean, ngx_uint_t access)
{
    ਮਾਪ_t                    ਪੱਧਰ;
    u_char                   *p;
    uint32_t                  n;
    ngx_err_t                 err;
    ngx_str_t                 name;
    ngx_uint_t                ਅਗੇਤਰ;
    ngx_pool_cleanup_t       *cln;
    ngx_pool_cleanup_file_t  *clnf;

    ਜੇ (file->name.len) {
        name = file->name;
        ਪੱਧਰ = 0;
        ਅਗੇਤਰ = 1;

    } ਨਹੀਂਤਾਂ {
        name = ਰਸਤਾ->name;
        ਪੱਧਰ = ਰਸਤਾ->len;
        ਅਗੇਤਰ = 0;
    }

    file->name.len = name.len + 1 + ਪੱਧਰ + 10;

    file->name.ਅੰਕੜੇ = ngx_pnalloc(pool, file->name.len + 1);
    ਜੇ (file->name.ਅੰਕੜੇ == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

#ਜੇ 0
    ਲਈ (i = 0; i < file->name.len; i++) {
        file->name.ਅੰਕੜੇ[i] = 'X';
    }
#ਜੇ_ਅੰਤ

    p = ngx_cpymem(file->name.ਅੰਕੜੇ, name.ਅੰਕੜੇ, name.len);

    ਜੇ (ਅਗੇਤਰ) {
        *p = '.';
    }

    p += 1 + ਪੱਧਰ;

    n = (uint32_t) ngx_ਅਗਲਾ_ਆਰਜ਼ੀ_ਸੰਖਿਆ(0);

    cln = ngx_pool_cleanup_ਜੋੜੋ(pool, ਆਕਾਰ(ngx_pool_cleanup_file_t));
    ਜੇ (cln == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    ਲਈ ( ;; ) {
        (ਖਾਲੀ) ngx_sprintf(p, "%010uD%Z", n);

        ਜੇ (!ਅਗੇਤਰ) {
            ngx_create_hashed_filename(ਰਸਤਾ, file->name.ਅੰਕੜੇ, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->ਲਘੂਗਣਕ, 0,
                       "hashed path: %s", file->name.ਅੰਕੜੇ);

        file->fd = ngx_open_tempfile(file->name.ਅੰਕੜੇ, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->ਲਘੂਗਣਕ, 0,
                       "temp fd:%d", file->fd);

        ਜੇ (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->ਅੰਕੜੇ;

            clnf->fd = file->fd;
            clnf->name = file->name.ਅੰਕੜੇ;
            clnf->ਲਘੂਗਣਕ = pool->ਲਘੂਗਣਕ;

            ਵਾਪਸ NGX_OK;
        }

        err = ngx_errno;

        ਜੇ (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_ਅਗਲਾ_ਆਰਜ਼ੀ_ਸੰਖਿਆ(1);
            ਜਾਰੀ;
        }

        ਜੇ ((ਰਸਤਾ->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->ਲਘੂਗਣਕ, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.ਅੰਕੜੇ);
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (ngx_create_ਰਸਤਾ(file, ਰਸਤਾ) == NGX_ERROR) {
            ਵਾਪਸ NGX_ERROR;
        }
    }
}


ਖਾਲੀ
ngx_create_hashed_filename(ngx_ਰਸਤਾ_t *ਰਸਤਾ, u_char *file, ਮਾਪ_t len)
{
    ਮਾਪ_t      i, level;
    ngx_uint_t  n;

    i = ਰਸਤਾ->name.len + 1;

    file[ਰਸਤਾ->name.len + ਰਸਤਾ->len]  = '/';

    ਲਈ (n = 0; n < NGX_MAX_ਰਸਤਾ_LEVEL; n++) {
        level = ਰਸਤਾ->level[n];

        ਜੇ (level == 0) {
            ਰੋਕੋ;
        }

        len -= level;
        file[i - 1] = '/';
        ngx_memcpy(&file[i], &file[len], level);
        i += level + 1;
    }
}


ngx_int_t
ngx_create_ਰਸਤਾ(ngx_file_t *file, ngx_ਰਸਤਾ_t *ਰਸਤਾ)
{
    ਮਾਪ_t      pos;
    ngx_err_t   err;
    ngx_uint_t  i;

    pos = ਰਸਤਾ->name.len;

    ਲਈ (i = 0; i < NGX_MAX_ਰਸਤਾ_LEVEL; i++) {
        ਜੇ (ਰਸਤਾ->level[i] == 0) {
            ਰੋਕੋ;
        }

        pos += ਰਸਤਾ->level[i] + 1;

        file->name.ਅੰਕੜੇ[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->ਲਘੂਗਣਕ, 0,
                       "temp file: \"%s\"", file->name.ਅੰਕੜੇ);

        ਜੇ (ngx_create_dir(file->name.ਅੰਕੜੇ, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ਜੇ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->ਲਘੂਗਣਕ, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.ਅੰਕੜੇ);
                ਵਾਪਸ NGX_ERROR;
            }
        }

        file->name.ਅੰਕੜੇ[pos] = '/';
    }

    ਵਾਪਸ NGX_OK;
}


ngx_err_t
ngx_create_ਭਰਿਆ_ਰਸਤਾ(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#ਜੇ (NGX_WIN32)
    p = dir + 3;
#ਨਹੀਂਤਾਂ
    p = dir + 1;
#ਜੇ_ਅੰਤ

    ਲਈ ( /* void */ ; *p; p++) {
        ch = *p;

        ਜੇ (ch != '/') {
            ਜਾਰੀ;
        }

        *p = '\0';

        ਜੇ (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            ਚੋਣ (err) {
            ਹਾਲਤ NGX_EEXIST:
                err = 0;
            ਹਾਲਤ NGX_EACCES:
                ਰੋਕੋ;

            ਮੂਲ:
                ਵਾਪਸ err;
            }
        }

        *p = '/';
    }

    ਵਾਪਸ err;
}


ngx_atomic_uint_t
ngx_ਅਗਲਾ_ਆਰਜ਼ੀ_ਸੰਖਿਆ(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, ਜੋੜੋ;

    ਜੋੜੋ = collision ? ngx_random_ਸੰਖਿਆ : 1;

    n = ngx_atomic_fetch_ਜੋੜੋ(ngx_ਆਰਜ਼ੀ_ਸੰਖਿਆ, ਜੋੜੋ);

    ਵਾਪਸ n + ਜੋੜੋ;
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_ਰਸਤਾ_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ  *p = conf;

    ssize_t      level;
    ngx_str_t   *ਮੁੱਲ;
    ngx_uint_t   i, n;
    ngx_ਰਸਤਾ_t  *ਰਸਤਾ, **ਥਾਂ;

    ਥਾਂ = (ngx_ਰਸਤਾ_t **) (p + cmd->ਵਿੱਥ);

    ਜੇ (*ਥਾਂ) {
        ਵਾਪਸ "is duplicate";
    }

    ਰਸਤਾ = ngx_pcalloc(cf->pool, ਆਕਾਰ(ngx_ਰਸਤਾ_t));
    ਜੇ (ਰਸਤਾ == ਨਿਰਾ) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    ਰਸਤਾ->name = ਮੁੱਲ[1];

    ਜੇ (ਰਸਤਾ->name.ਅੰਕੜੇ[ਰਸਤਾ->name.len - 1] == '/') {
        ਰਸਤਾ->name.len--;
    }

    ਜੇ (ngx_conf_ਭਰਿਆ_name(cf->ਚੱਕਰ, &ਰਸਤਾ->name, 0) != NGX_OK) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਰਸਤਾ->conf_file = cf->conf_file->file.name.ਅੰਕੜੇ;
    ਰਸਤਾ->ਸਤਰ = cf->conf_file->ਸਤਰ;

    ਲਈ (i = 0, n = 2; n < cf->ਦਲੀਲਾਂ->nelts; i++, n++) {
        level = ngx_atoi(ਮੁੱਲ[n].ਅੰਕੜੇ, ਮੁੱਲ[n].len);
        ਜੇ (level == NGX_ERROR || level == 0) {
            ਵਾਪਸ "invalid value";
        }

        ਰਸਤਾ->level[i] = level;
        ਰਸਤਾ->len += level + 1;
    }

    ਜੇ (ਰਸਤਾ->len > 10 + i) {
        ਵਾਪਸ "invalid value";
    }

    *ਥਾਂ = ਰਸਤਾ;

    ਜੇ (ngx_ਜੋੜੋ_ਰਸਤਾ(cf, ਥਾਂ) == NGX_ERROR) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਅੱਖਰ *
ngx_conf_ਮਿਲਾਓ_ਰਸਤਾ_ਮੁੱਲ(ngx_conf_t *cf, ngx_ਰਸਤਾ_t **ਰਸਤਾ, ngx_ਰਸਤਾ_t *prev,
    ngx_ਰਸਤਾ_init_t *init)
{
    ngx_uint_t  i;

    ਜੇ (*ਰਸਤਾ) {
        ਵਾਪਸ NGX_CONF_OK;
    }

    ਜੇ (prev) {
        *ਰਸਤਾ = prev;
        ਵਾਪਸ NGX_CONF_OK;
    }

    *ਰਸਤਾ = ngx_pcalloc(cf->pool, ਆਕਾਰ(ngx_ਰਸਤਾ_t));
    ਜੇ (*ਰਸਤਾ == ਨਿਰਾ) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    (*ਰਸਤਾ)->name = init->name;

    ਜੇ (ngx_conf_ਭਰਿਆ_name(cf->ਚੱਕਰ, &(*ਰਸਤਾ)->name, 0) != NGX_OK) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਲਈ (i = 0; i < NGX_MAX_ਰਸਤਾ_LEVEL; i++) {
        (*ਰਸਤਾ)->level[i] = init->level[i];
        (*ਰਸਤਾ)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    ਜੇ (ngx_ਜੋੜੋ_ਰਸਤਾ(cf, ਰਸਤਾ) != NGX_OK) {
        ਵਾਪਸ NGX_CONF_ERROR;
    }

    ਵਾਪਸ NGX_CONF_OK;
}


ਅੱਖਰ *
ngx_conf_ਸੈੱਟ_access_ਥਾਂ(ngx_conf_t *cf, ngx_command_t *cmd, ਖਾਲੀ *conf)
{
    ਅੱਖਰ  *confp = conf;

    u_char      *p;
    ngx_str_t   *ਮੁੱਲ;
    ngx_uint_t   i, ਸੱਜਾ, ਖਿਸਕਾਓ, *access, user;

    access = (ngx_uint_t *) (confp + cmd->ਵਿੱਥ);

    ਜੇ (*access != NGX_CONF_UNSET_UINT) {
        ਵਾਪਸ "is duplicate";
    }

    ਮੁੱਲ = cf->ਦਲੀਲਾਂ->elts;

    *access = 0;
    user = 0600;

    ਲਈ (i = 1; i < cf->ਦਲੀਲਾਂ->nelts; i++) {

        p = ਮੁੱਲ[i].ਅੰਕੜੇ;

        ਜੇ (ngx_strncmp(p, "user:", ਆਕਾਰ("user:") - 1) == 0) {
            ਖਿਸਕਾਓ = 6;
            p += ਆਕਾਰ("user:") - 1;
            user = 0;

        } ਨਹੀਂਤਾਂ ਜੇ (ngx_strncmp(p, "group:", ਆਕਾਰ("group:") - 1) == 0) {
            ਖਿਸਕਾਓ = 3;
            p += ਆਕਾਰ("group:") - 1;

        } ਨਹੀਂਤਾਂ ਜੇ (ngx_strncmp(p, "all:", ਆਕਾਰ("all:") - 1) == 0) {
            ਖਿਸਕਾਓ = 0;
            p += ਆਕਾਰ("all:") - 1;

        } ਨਹੀਂਤਾਂ {
            ਜਾਓ invalid;
        }

        ਜੇ (ngx_strcmp(p, "rw") == 0) {
            ਸੱਜਾ = 6;

        } ਨਹੀਂਤਾਂ ਜੇ (ngx_strcmp(p, "r") == 0) {
            ਸੱਜਾ = 4;

        } ਨਹੀਂਤਾਂ {
            ਜਾਓ invalid;
        }

        *access |= ਸੱਜਾ << ਖਿਸਕਾਓ;
    }

    *access |= user;

    ਵਾਪਸ NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &ਮੁੱਲ[i]);

    ਵਾਪਸ NGX_CONF_ERROR;
}


ngx_int_t
ngx_ਜੋੜੋ_ਰਸਤਾ(ngx_conf_t *cf, ngx_ਰਸਤਾ_t **ਥਾਂ)
{
    ngx_uint_t   i, n;
    ngx_ਰਸਤਾ_t  *ਰਸਤਾ, **p;

    ਰਸਤਾ = *ਥਾਂ;

    p = cf->ਚੱਕਰ->paths.elts;
    ਲਈ (i = 0; i < cf->ਚੱਕਰ->paths.nelts; i++) {
        ਜੇ (p[i]->name.len == ਰਸਤਾ->name.len
            && ngx_strcmp(p[i]->name.ਅੰਕੜੇ, ਰਸਤਾ->name.ਅੰਕੜੇ) == 0)
        {
            ਜੇ (p[i]->ਅੰਕੜੇ != ਰਸਤਾ->ਅੰਕੜੇ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->ਸਤਰ);
                ਵਾਪਸ NGX_ERROR;
            }

            ਲਈ (n = 0; n < NGX_MAX_ਰਸਤਾ_LEVEL; n++) {
                ਜੇ (p[i]->level[n] != ਰਸਤਾ->level[n]) {
                    ਜੇ (ਰਸਤਾ->conf_file == ਨਿਰਾ) {
                        ਜੇ (p[i]->conf_file == ਨਿਰਾ) {
                            ngx_log_error(NGX_LOG_EMERG, cf->ਲਘੂਗਣਕ, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            ਵਾਪਸ NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->ਲਘੂਗਣਕ, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->ਸਤਰ);
                        ਵਾਪਸ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->ਸਤਰ);
                    ਵਾਪਸ NGX_ERROR;
                }

                ਜੇ (p[i]->level[n] == 0) {
                    ਰੋਕੋ;
                }
            }

            *ਥਾਂ = p[i];

            ਵਾਪਸ NGX_OK;
        }
    }

    p = ngx_array_ਧੱਕੋ(&cf->ਚੱਕਰ->paths);
    ਜੇ (p == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    *p = ਰਸਤਾ;

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_ਰਸਤਾ_t      **ਰਸਤਾ;

    ਰਸਤਾ = ਚੱਕਰ->paths.elts;
    ਲਈ (i = 0; i < ਚੱਕਰ->paths.nelts; i++) {

        ਜੇ (ngx_create_dir(ਰਸਤਾ[i]->name.ਅੰਕੜੇ, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ਜੇ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, err,
                              ngx_create_dir_n " \"%s\" failed",
                              ਰਸਤਾ[i]->name.ਅੰਕੜੇ);
                ਵਾਪਸ NGX_ERROR;
            }
        }

        ਜੇ (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ਜਾਰੀ;
        }

#ਜੇ !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        ਜੇ (ngx_file_info(ਰਸਤਾ[i]->name.ਅੰਕੜੇ, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", ਰਸਤਾ[i]->name.ਅੰਕੜੇ);
            ਵਾਪਸ NGX_ERROR;
        }

        ਜੇ (fi.st_uid != user) {
            ਜੇ (chown((ਅਚਲ ਅੱਖਰ *) ਰਸਤਾ[i]->name.ਅੰਕੜੇ, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              ਰਸਤਾ[i]->name.ਅੰਕੜੇ, user);
                ਵਾਪਸ NGX_ERROR;
            }
        }

        ਜੇ ((fi.st_ਬਹੁਲਕ & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_ਬਹੁਲਕ |= (S_IRUSR|S_IWUSR|S_IXUSR);

            ਜੇ (chmod((ਅਚਲ ਅੱਖਰ *) ਰਸਤਾ[i]->name.ਅੰਕੜੇ, fi.st_ਬਹੁਲਕ) == -1) {
                ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                              "chmod() \"%s\" failed", ਰਸਤਾ[i]->name.ਅੰਕੜੇ);
                ਵਾਪਸ NGX_ERROR;
            }
        }
        }
#ਜੇ_ਅੰਤ
    }

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *ਮੰਜ਼ਿਲ, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#ਜੇ !(NGX_WIN32)

    ਜੇ (ext->access) {
        ਜੇ (ngx_change_file_access(src->ਅੰਕੜੇ, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->ਅੰਕੜੇ);
            err = 0;
            ਜਾਓ failed;
        }
    }

#ਜੇ_ਅੰਤ

    ਜੇ (ext->ਸਮਾਂ != -1) {
        ਜੇ (ngx_ਸੈੱਟ_file_ਸਮਾਂ(src->ਅੰਕੜੇ, ext->fd, ext->ਸਮਾਂ) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_ਸੈੱਟ_file_ਸਮਾਂ_n " \"%s\" failed", src->ਅੰਕੜੇ);
            err = 0;
            ਜਾਓ failed;
        }
    }

    ਜੇ (ngx_rename_file(src->ਅੰਕੜੇ, ਮੰਜ਼ਿਲ->ਅੰਕੜੇ) != NGX_FILE_ERROR) {
        ਵਾਪਸ NGX_OK;
    }

    err = ngx_errno;

    ਜੇ (err == NGX_ENOPATH) {

        ਜੇ (!ext->create_ਰਸਤਾ) {
            ਜਾਓ failed;
        }

        err = ngx_create_ਭਰਿਆ_ਰਸਤਾ(ਮੰਜ਼ਿਲ->ਅੰਕੜੇ, ngx_dir_access(ext->ਰਸਤਾ_access));

        ਜੇ (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->ਲਘੂਗਣਕ, err,
                          ngx_create_dir_n " \"%s\" failed", ਮੰਜ਼ਿਲ->ਅੰਕੜੇ);
            err = 0;
            ਜਾਓ failed;
        }

        ਜੇ (ngx_rename_file(src->ਅੰਕੜੇ, ਮੰਜ਼ਿਲ->ਅੰਕੜੇ) != NGX_FILE_ERROR) {
            ਵਾਪਸ NGX_OK;
        }

        err = ngx_errno;
    }

#ਜੇ (NGX_WIN32)

    ਜੇ (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, ਮੰਜ਼ਿਲ, ext->ਲਘੂਗਣਕ);

        ਜੇ (err == 0) {
            ਵਾਪਸ NGX_OK;
        }
    }

#ਜੇ_ਅੰਤ

    ਜੇ (err == NGX_EXDEV) {

        cf.ਮਾਪ = -1;
        cf.buf_ਮਾਪ = 0;
        cf.access = ext->access;
        cf.ਸਮਾਂ = ext->ਸਮਾਂ;
        cf.ਲਘੂਗਣਕ = ext->ਲਘੂਗਣਕ;

        name = ngx_alloc(ਮੰਜ਼ਿਲ->len + 1 + 10 + 1, ext->ਲਘੂਗਣਕ);
        ਜੇ (name == ਨਿਰਾ) {
            ਵਾਪਸ NGX_ERROR;
        }

        (ਖਾਲੀ) ngx_sprintf(name, "%*s.%010uD%Z", ਮੰਜ਼ਿਲ->len, ਮੰਜ਼ਿਲ->ਅੰਕੜੇ,
                           (uint32_t) ngx_ਅਗਲਾ_ਆਰਜ਼ੀ_ਸੰਖਿਆ(0));

        ਜੇ (ngx_copy_file(src->ਅੰਕੜੇ, name, &cf) == NGX_OK) {

            ਜੇ (ngx_rename_file(name, ਮੰਜ਼ਿਲ->ਅੰਕੜੇ) != NGX_FILE_ERROR) {
                ngx_free(name);

                ਜੇ (ngx_delete_file(src->ਅੰਕੜੇ) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->ਲਘੂਗਣਕ, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->ਅੰਕੜੇ);
                    ਵਾਪਸ NGX_ERROR;
                }

                ਵਾਪਸ NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, ਮੰਜ਼ਿਲ->ਅੰਕੜੇ);

            ਜੇ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->ਲਘੂਗਣਕ, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    ਜੇ (ext->delete_file) {
        ਜੇ (ngx_delete_file(src->ਅੰਕੜੇ) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->ਅੰਕੜੇ);
        }
    }

    ਜੇ (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->ਲਘੂਗਣਕ, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->ਅੰਕੜੇ, ਮੰਜ਼ਿਲ->ਅੰਕੜੇ);
    }

    ਵਾਪਸ NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *ਮੰਜ਼ਿਲ, ngx_copy_file_t *cf)
{
    ਅੱਖਰ             *buf;
    off_t             ਮਾਪ;
    ਸਮਾਂ_t            ਸਮਾਂ;
    ਮਾਪ_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = ਨਿਰਾ;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    ਜੇ (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        ਜਾਓ failed;
    }

    ਜੇ (cf->ਮਾਪ != -1 && cf->access != 0 && cf->ਸਮਾਂ != -1) {
        ਮਾਪ = cf->ਮਾਪ;
        access = cf->access;
        ਸਮਾਂ = cf->ਸਮਾਂ;

    } ਨਹੀਂਤਾਂ {
        ਜੇ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            ਜਾਓ failed;
        }

        ਮਾਪ = (cf->ਮਾਪ != -1) ? cf->ਮਾਪ : ngx_file_ਮਾਪ(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        ਸਮਾਂ = (cf->ਸਮਾਂ != -1) ? cf->ਸਮਾਂ : ngx_file_mtime(&fi);
    }

    len = cf->buf_ਮਾਪ ? cf->buf_ਮਾਪ : 65536;

    ਜੇ ((off_t) len > ਮਾਪ) {
        len = (ਮਾਪ_t) ਮਾਪ;
    }

    buf = ngx_alloc(len, cf->ਲਘੂਗਣਕ);
    ਜੇ (buf == ਨਿਰਾ) {
        ਜਾਓ failed;
    }

    nfd = ngx_open_file(ਮੰਜ਼ਿਲ, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    ਜੇ (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", ਮੰਜ਼ਿਲ);
        ਜਾਓ failed;
    }

    ਜਦੋਂ (ਮਾਪ > 0) {

        ਜੇ ((off_t) len > ਮਾਪ) {
            len = (ਮਾਪ_t) ਮਾਪ;
        }

        n = ngx_read_fd(fd, buf, len);

        ਜੇ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            ਜਾਓ failed;
        }

        ਜੇ ((ਮਾਪ_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->ਲਘੂਗਣਕ, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, ਮਾਪ, from);
            ਜਾਓ failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        ਜੇ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", ਮੰਜ਼ਿਲ);
            ਜਾਓ failed;
        }

        ਜੇ ((ਮਾਪ_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->ਲਘੂਗਣਕ, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, ਮਾਪ, ਮੰਜ਼ਿਲ);
            ਜਾਓ failed;
        }

        ਮਾਪ -= n;
    }

    ਜੇ (ngx_ਸੈੱਟ_file_ਸਮਾਂ(ਮੰਜ਼ਿਲ, nfd, ਸਮਾਂ) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_ਸੈੱਟ_file_ਸਮਾਂ_n " \"%s\" failed", ਮੰਜ਼ਿਲ);
        ਜਾਓ failed;
    }

    rc = NGX_OK;

failed:

    ਜੇ (nfd != NGX_INVALID_FILE) {
        ਜੇ (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", ਮੰਜ਼ਿਲ);
        }
    }

    ਜੇ (fd != NGX_INVALID_FILE) {
        ਜੇ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    ਜੇ (buf) {
        ngx_free(buf);
    }

    ਵਾਪਸ rc;
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
ngx_walk_ਰੁੱਖ(ngx_ਰੁੱਖ_ctx_t *ctx, ngx_str_t *ਰੁੱਖ)
{
    ਖਾਲੀ       *ਅੰਕੜੇ, *prev;
    u_char     *p, *name;
    ਮਾਪ_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->ਲਘੂਗਣਕ, 0,
                   "walk tree \"%V\"", ਰੁੱਖ);

    ਜੇ (ngx_open_dir(ਰੁੱਖ, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", ਰੁੱਖ->ਅੰਕੜੇ);
        ਵਾਪਸ NGX_ERROR;
    }

    prev = ctx->ਅੰਕੜੇ;

    ਜੇ (ctx->alloc) {
        ਅੰਕੜੇ = ngx_alloc(ctx->alloc, ctx->ਲਘੂਗਣਕ);
        ਜੇ (ਅੰਕੜੇ == ਨਿਰਾ) {
            ਜਾਓ failed;
        }

        ਜੇ (ctx->init_handler(ਅੰਕੜੇ, prev) == NGX_ABORT) {
            ਜਾਓ failed;
        }

        ctx->ਅੰਕੜੇ = ਅੰਕੜੇ;

    } ਨਹੀਂਤਾਂ {
        ਅੰਕੜੇ = ਨਿਰਾ;
    }

    ਲਈ ( ;; ) {

        ngx_ਸੈੱਟ_errno(0);

        ਜੇ (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            ਜੇ (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } ਨਹੀਂਤਾਂ {
                ngx_log_error(NGX_LOG_CRIT, ctx->ਲਘੂਗਣਕ, err,
                              ngx_read_dir_n " \"%s\" failed", ਰੁੱਖ->ਅੰਕੜੇ);
                rc = NGX_ERROR;
            }

            ਜਾਓ ਪੂਰਾ;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->ਲਘੂਗਣਕ, 0,
                      "tree name %uz:\"%s\"", len, name);

        ਜੇ (len == 1 && name[0] == '.') {
            ਜਾਰੀ;
        }

        ਜੇ (len == 2 && name[0] == '.' && name[1] == '.') {
            ਜਾਰੀ;
        }

        file.len = ਰੁੱਖ->len + 1 + len;

        ਜੇ (file.len > buf.len) {

            ਜੇ (buf.len) {
                ngx_free(buf.ਅੰਕੜੇ);
            }

            buf.len = ਰੁੱਖ->len + 1 + len;

            buf.ਅੰਕੜੇ = ngx_alloc(buf.len + 1, ctx->ਲਘੂਗਣਕ);
            ਜੇ (buf.ਅੰਕੜੇ == ਨਿਰਾ) {
                ਜਾਓ failed;
            }
        }

        p = ngx_cpymem(buf.ਅੰਕੜੇ, ਰੁੱਖ->ਅੰਕੜੇ, ਰੁੱਖ->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.ਅੰਕੜੇ = buf.ਅੰਕੜੇ;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->ਲਘੂਗਣਕ, 0,
                       "tree path \"%s\"", file.ਅੰਕੜੇ);

        ਜੇ (!dir.valid_info) {
            ਜੇ (ngx_de_info(file.ਅੰਕੜੇ, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->ਲਘੂਗਣਕ, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.ਅੰਕੜੇ);
                ਜਾਰੀ;
            }
        }

        ਜੇ (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->ਲਘੂਗਣਕ, 0,
                           "tree file \"%s\"", file.ਅੰਕੜੇ);

            ctx->ਮਾਪ = ngx_de_ਮਾਪ(&dir);
            ctx->fs_ਮਾਪ = ngx_de_fs_ਮਾਪ(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ਜੇ (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                ਜਾਓ failed;
            }

        } ਨਹੀਂਤਾਂ ਜੇ (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->ਲਘੂਗਣਕ, 0,
                           "tree enter dir \"%s\"", file.ਅੰਕੜੇ);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_ਰੁੱਖ_handler(ctx, &file);

            ਜੇ (rc == NGX_ABORT) {
                ਜਾਓ failed;
            }

            ਜੇ (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->ਲਘੂਗਣਕ, 0,
                               "tree skip dir \"%s\"", file.ਅੰਕੜੇ);
                ਜਾਰੀ;
            }

            ਜੇ (ngx_walk_ਰੁੱਖ(ctx, &file) == NGX_ABORT) {
                ਜਾਓ failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ਜੇ (ctx->post_ਰੁੱਖ_handler(ctx, &file) == NGX_ABORT) {
                ਜਾਓ failed;
            }

        } ਨਹੀਂਤਾਂ {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->ਲਘੂਗਣਕ, 0,
                           "tree special \"%s\"", file.ਅੰਕੜੇ);

            ਜੇ (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                ਜਾਓ failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

ਪੂਰਾ:

    ਜੇ (buf.len) {
        ngx_free(buf.ਅੰਕੜੇ);
    }

    ਜੇ (ਅੰਕੜੇ) {
        ngx_free(ਅੰਕੜੇ);
        ctx->ਅੰਕੜੇ = prev;
    }

    ਜੇ (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", ਰੁੱਖ->ਅੰਕੜੇ);
    }

    ਵਾਪਸ rc;
}
