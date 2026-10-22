
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಸ್ಥಿರ ngx_int_t ngx_test_ತುಂಬಿದ_name(ngx_str_t *name);


ಸ್ಥಿರ ngx_atomic_t   ತಾತ್ಕಾಲಿಕ_ಸಂಖ್ಯೆ = 0;
ngx_atomic_t         *ngx_ತಾತ್ಕಾಲಿಕ_ಸಂಖ್ಯೆ = &ತಾತ್ಕಾಲಿಕ_ಸಂಖ್ಯೆ;
ngx_atomic_int_t      ngx_random_ಸಂಖ್ಯೆ = 123456;


ngx_int_t
ngx_get_ತುಂಬಿದ_name(ngx_pool_t *pool, ngx_str_t *ಪೂರ್ವಪ್ರತ್ಯಯ, ngx_str_t *name)
{
    ಗಾತ್ರ_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_ತುಂಬಿದ_name(name);

    ಆದರೆ (rc == NGX_OK) {
        ಹಿಂತಿರುಗಿ rc;
    }

    len = ಪೂರ್ವಪ್ರತ್ಯಯ->len;

#ಆದರೆ (NGX_WIN32)

    ಆದರೆ (rc == 2) {
        len = rc;
    }

#ಆದರೆ_ಅಂತ್ಯ

    n = ngx_pnalloc(pool, len + name->len + 1);
    ಆದರೆ (n == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    p = ngx_cpymem(n, ಪೂರ್ವಪ್ರತ್ಯಯ->ದತ್ತಾಂಶ, len);
    ngx_cpystrn(p, name->ದತ್ತಾಂಶ, name->len + 1);

    name->len += len;
    name->ದತ್ತಾಂಶ = n;

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಸ್ಥಿರ ngx_int_t
ngx_test_ತುಂಬಿದ_name(ngx_str_t *name)
{
#ಆದರೆ (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->ದತ್ತಾಂಶ[0];

    ಆದರೆ (name->len < 2) {
        ಆದರೆ (c0 == '/') {
            ಹಿಂತಿರುಗಿ 2;
        }

        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    c1 = name->ದತ್ತಾಂಶ[1];

    ಆದರೆ (c1 == ':') {
        c0 |= 0x20;

        ಆದರೆ ((c0 >= 'a' && c0 <= 'z')) {
            ಹಿಂತಿರುಗಿ NGX_OK;
        }

        ಹಿಂತಿರುಗಿ NGX_DECLINED;
    }

    ಆದರೆ (c1 == '/') {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಆದರೆ (c0 == '/') {
        ಹಿಂತಿರುಗಿ 2;
    }

    ಹಿಂತಿರುಗಿ NGX_DECLINED;

#ಇಲ್ಲದಿದ್ದರೆ

    ಆದರೆ (name->ದತ್ತಾಂಶ[0] == '/') {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ಹಿಂತಿರುಗಿ NGX_DECLINED;

#ಆದರೆ_ಅಂತ್ಯ
}


ssize_t
ngx_write_chain_ಗಮ್ಯ_ತಾತ್ಕಾಲಿಕ_file(ngx_ತಾತ್ಕಾಲಿಕ_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    ಆದರೆ (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_ತಾತ್ಕಾಲಿಕ_file(&tf->file, tf->ಮಾರ್ಗ, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        ಆದರೆ (rc != NGX_OK) {
            ಹಿಂತಿರುಗಿ rc;
        }

        ಆದರೆ (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.ಲಘುಗಣಕ, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#ಆದರೆ (NGX_ಎಳೆಗಳು && NGX_HAVE_PWRITEV)

    ಆದರೆ (tf->thread_write) {
        ಹಿಂತಿರುಗಿ ngx_thread_write_chain_ಗಮ್ಯ_file(&tf->file, chain, tf->ಸ್ಥಾನಾಂತರ,
                                              tf->pool);
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ ngx_write_chain_ಗಮ್ಯ_file(&tf->file, chain, tf->ಸ್ಥಾನಾಂತರ, tf->pool);
}


ngx_int_t
ngx_create_ತಾತ್ಕಾಲಿಕ_file(ngx_file_t *file, ngx_ಮಾರ್ಗ_t *ಮಾರ್ಗ, ngx_pool_t *pool,
    ngx_uint_t persistent, ngx_uint_t clean, ngx_uint_t access)
{
    ಗಾತ್ರ_t                    ಮಟ್ಟಗಳು;
    u_char                   *p;
    uint32_t                  n;
    ngx_err_t                 err;
    ngx_str_t                 name;
    ngx_uint_t                ಪೂರ್ವಪ್ರತ್ಯಯ;
    ngx_pool_cleanup_t       *cln;
    ngx_pool_cleanup_file_t  *clnf;

    ಆದರೆ (file->name.len) {
        name = file->name;
        ಮಟ್ಟಗಳು = 0;
        ಪೂರ್ವಪ್ರತ್ಯಯ = 1;

    } ಇಲ್ಲದಿದ್ದರೆ {
        name = ಮಾರ್ಗ->name;
        ಮಟ್ಟಗಳು = ಮಾರ್ಗ->len;
        ಪೂರ್ವಪ್ರತ್ಯಯ = 0;
    }

    file->name.len = name.len + 1 + ಮಟ್ಟಗಳು + 10;

    file->name.ದತ್ತಾಂಶ = ngx_pnalloc(pool, file->name.len + 1);
    ಆದರೆ (file->name.ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

#ಆದರೆ 0
    ಗಾಗಿ (i = 0; i < file->name.len; i++) {
        file->name.ದತ್ತಾಂಶ[i] = 'X';
    }
#ಆದರೆ_ಅಂತ್ಯ

    p = ngx_cpymem(file->name.ದತ್ತಾಂಶ, name.ದತ್ತಾಂಶ, name.len);

    ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ) {
        *p = '.';
    }

    p += 1 + ಮಟ್ಟಗಳು;

    n = (uint32_t) ngx_ಮುಂದಿನ_ತಾತ್ಕಾಲಿಕ_ಸಂಖ್ಯೆ(0);

    cln = ngx_pool_cleanup_ಕೂಡಿಸು(pool, ಗಾತ್ರ(ngx_pool_cleanup_file_t));
    ಆದರೆ (cln == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಗಾಗಿ ( ;; ) {
        (ಶೂನ್ಯ) ngx_sprintf(p, "%010uD%Z", n);

        ಆದರೆ (!ಪೂರ್ವಪ್ರತ್ಯಯ) {
            ngx_create_hashed_filename(ಮಾರ್ಗ, file->name.ದತ್ತಾಂಶ, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->ಲಘುಗಣಕ, 0,
                       "hashed path: %s", file->name.ದತ್ತಾಂಶ);

        file->fd = ngx_open_tempfile(file->name.ದತ್ತಾಂಶ, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->ಲಘುಗಣಕ, 0,
                       "temp fd:%d", file->fd);

        ಆದರೆ (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->ದತ್ತಾಂಶ;

            clnf->fd = file->fd;
            clnf->name = file->name.ದತ್ತಾಂಶ;
            clnf->ಲಘುಗಣಕ = pool->ಲಘುಗಣಕ;

            ಹಿಂತಿರುಗಿ NGX_OK;
        }

        err = ngx_errno;

        ಆದರೆ (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_ಮುಂದಿನ_ತಾತ್ಕಾಲಿಕ_ಸಂಖ್ಯೆ(1);
            ಮುಂದುವರಿ;
        }

        ಆದರೆ ((ಮಾರ್ಗ->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->ಲಘುಗಣಕ, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.ದತ್ತಾಂಶ);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (ngx_create_ಮಾರ್ಗ(file, ಮಾರ್ಗ) == NGX_ERROR) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }
    }
}


ಶೂನ್ಯ
ngx_create_hashed_filename(ngx_ಮಾರ್ಗ_t *ಮಾರ್ಗ, u_char *file, ಗಾತ್ರ_t len)
{
    ಗಾತ್ರ_t      i, level;
    ngx_uint_t  n;

    i = ಮಾರ್ಗ->name.len + 1;

    file[ಮಾರ್ಗ->name.len + ಮಾರ್ಗ->len]  = '/';

    ಗಾಗಿ (n = 0; n < NGX_MAX_ಮಾರ್ಗ_LEVEL; n++) {
        level = ಮಾರ್ಗ->level[n];

        ಆದರೆ (level == 0) {
            ನಿಲ್ಲಿಸು;
        }

        len -= level;
        file[i - 1] = '/';
        ngx_memcpy(&file[i], &file[len], level);
        i += level + 1;
    }
}


ngx_int_t
ngx_create_ಮಾರ್ಗ(ngx_file_t *file, ngx_ಮಾರ್ಗ_t *ಮಾರ್ಗ)
{
    ಗಾತ್ರ_t      pos;
    ngx_err_t   err;
    ngx_uint_t  i;

    pos = ಮಾರ್ಗ->name.len;

    ಗಾಗಿ (i = 0; i < NGX_MAX_ಮಾರ್ಗ_LEVEL; i++) {
        ಆದರೆ (ಮಾರ್ಗ->level[i] == 0) {
            ನಿಲ್ಲಿಸು;
        }

        pos += ಮಾರ್ಗ->level[i] + 1;

        file->name.ದತ್ತಾಂಶ[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->ಲಘುಗಣಕ, 0,
                       "temp file: \"%s\"", file->name.ದತ್ತಾಂಶ);

        ಆದರೆ (ngx_create_dir(file->name.ದತ್ತಾಂಶ, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ಆದರೆ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->ಲಘುಗಣಕ, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.ದತ್ತಾಂಶ);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
        }

        file->name.ದತ್ತಾಂಶ[pos] = '/';
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_err_t
ngx_create_ತುಂಬಿದ_ಮಾರ್ಗ(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#ಆದರೆ (NGX_WIN32)
    p = dir + 3;
#ಇಲ್ಲದಿದ್ದರೆ
    p = dir + 1;
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ ( /* void */ ; *p; p++) {
        ch = *p;

        ಆದರೆ (ch != '/') {
            ಮುಂದುವರಿ;
        }

        *p = '\0';

        ಆದರೆ (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            ಆಯ್ಕೆ (err) {
            ಸ್ಥಿತಿ NGX_EEXIST:
                err = 0;
            ಸ್ಥಿತಿ NGX_EACCES:
                ನಿಲ್ಲಿಸು;

            ಪೂರ್ವನಿಯೋಜಿತ:
                ಹಿಂತಿರುಗಿ err;
            }
        }

        *p = '/';
    }

    ಹಿಂತಿರುಗಿ err;
}


ngx_atomic_uint_t
ngx_ಮುಂದಿನ_ತಾತ್ಕಾಲಿಕ_ಸಂಖ್ಯೆ(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, ಕೂಡಿಸು;

    ಕೂಡಿಸು = collision ? ngx_random_ಸಂಖ್ಯೆ : 1;

    n = ngx_atomic_fetch_ಕೂಡಿಸು(ngx_ತಾತ್ಕಾಲಿಕ_ಸಂಖ್ಯೆ, ಕೂಡಿಸು);

    ಹಿಂತಿರುಗಿ n + ಕೂಡಿಸು;
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_ಮಾರ್ಗ_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ  *p = conf;

    ssize_t      level;
    ngx_str_t   *ಮೌಲ್ಯ;
    ngx_uint_t   i, n;
    ngx_ಮಾರ್ಗ_t  *ಮಾರ್ಗ, **ಸ್ಥಾನ;

    ಸ್ಥಾನ = (ngx_ಮಾರ್ಗ_t **) (p + cmd->ಸ್ಥಾನಾಂತರ);

    ಆದರೆ (*ಸ್ಥಾನ) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮಾರ್ಗ = ngx_pcalloc(cf->pool, ಗಾತ್ರ(ngx_ಮಾರ್ಗ_t));
    ಆದರೆ (ಮಾರ್ಗ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    ಮಾರ್ಗ->name = ಮೌಲ್ಯ[1];

    ಆದರೆ (ಮಾರ್ಗ->name.ದತ್ತಾಂಶ[ಮಾರ್ಗ->name.len - 1] == '/') {
        ಮಾರ್ಗ->name.len--;
    }

    ಆದರೆ (ngx_conf_ತುಂಬಿದ_name(cf->ಚಕ್ರ, &ಮಾರ್ಗ->name, 0) != NGX_OK) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಮಾರ್ಗ->conf_file = cf->conf_file->file.name.ದತ್ತಾಂಶ;
    ಮಾರ್ಗ->ಸಾಲು = cf->conf_file->ಸಾಲು;

    ಗಾಗಿ (i = 0, n = 2; n < cf->ವಾದಗಳು->nelts; i++, n++) {
        level = ngx_atoi(ಮೌಲ್ಯ[n].ದತ್ತಾಂಶ, ಮೌಲ್ಯ[n].len);
        ಆದರೆ (level == NGX_ERROR || level == 0) {
            ಹಿಂತಿರುಗಿ "invalid value";
        }

        ಮಾರ್ಗ->level[i] = level;
        ಮಾರ್ಗ->len += level + 1;
    }

    ಆದರೆ (ಮಾರ್ಗ->len > 10 + i) {
        ಹಿಂತಿರುಗಿ "invalid value";
    }

    *ಸ್ಥಾನ = ಮಾರ್ಗ;

    ಆದರೆ (ngx_ಕೂಡಿಸು_ಮಾರ್ಗ(cf, ಸ್ಥಾನ) == NGX_ERROR) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಅಕ್ಷರ *
ngx_conf_ವಿಲೀನ_ಮಾರ್ಗ_ಮೌಲ್ಯ(ngx_conf_t *cf, ngx_ಮಾರ್ಗ_t **ಮಾರ್ಗ, ngx_ಮಾರ್ಗ_t *prev,
    ngx_ಮಾರ್ಗ_init_t *init)
{
    ngx_uint_t  i;

    ಆದರೆ (*ಮಾರ್ಗ) {
        ಹಿಂತಿರುಗಿ NGX_CONF_OK;
    }

    ಆದರೆ (prev) {
        *ಮಾರ್ಗ = prev;
        ಹಿಂತಿರುಗಿ NGX_CONF_OK;
    }

    *ಮಾರ್ಗ = ngx_pcalloc(cf->pool, ಗಾತ್ರ(ngx_ಮಾರ್ಗ_t));
    ಆದರೆ (*ಮಾರ್ಗ == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    (*ಮಾರ್ಗ)->name = init->name;

    ಆದರೆ (ngx_conf_ತುಂಬಿದ_name(cf->ಚಕ್ರ, &(*ಮಾರ್ಗ)->name, 0) != NGX_OK) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಗಾಗಿ (i = 0; i < NGX_MAX_ಮಾರ್ಗ_LEVEL; i++) {
        (*ಮಾರ್ಗ)->level[i] = init->level[i];
        (*ಮಾರ್ಗ)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    ಆದರೆ (ngx_ಕೂಡಿಸು_ಮಾರ್ಗ(cf, ಮಾರ್ಗ) != NGX_OK) {
        ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
    }

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;
}


ಅಕ್ಷರ *
ngx_conf_ಗಣ_access_ಸ್ಥಾನ(ngx_conf_t *cf, ngx_command_t *cmd, ಶೂನ್ಯ *conf)
{
    ಅಕ್ಷರ  *confp = conf;

    u_char      *p;
    ngx_str_t   *ಮೌಲ್ಯ;
    ngx_uint_t   i, ಬಲ, ಸರಿಸು, *access, user;

    access = (ngx_uint_t *) (confp + cmd->ಸ್ಥಾನಾಂತರ);

    ಆದರೆ (*access != NGX_CONF_UNSET_UINT) {
        ಹಿಂತಿರುಗಿ "is duplicate";
    }

    ಮೌಲ್ಯ = cf->ವಾದಗಳು->elts;

    *access = 0;
    user = 0600;

    ಗಾಗಿ (i = 1; i < cf->ವಾದಗಳು->nelts; i++) {

        p = ಮೌಲ್ಯ[i].ದತ್ತಾಂಶ;

        ಆದರೆ (ngx_strncmp(p, "user:", ಗಾತ್ರ("user:") - 1) == 0) {
            ಸರಿಸು = 6;
            p += ಗಾತ್ರ("user:") - 1;
            user = 0;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strncmp(p, "group:", ಗಾತ್ರ("group:") - 1) == 0) {
            ಸರಿಸು = 3;
            p += ಗಾತ್ರ("group:") - 1;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strncmp(p, "all:", ಗಾತ್ರ("all:") - 1) == 0) {
            ಸರಿಸು = 0;
            p += ಗಾತ್ರ("all:") - 1;

        } ಇಲ್ಲದಿದ್ದರೆ {
            ಹೋಗು invalid;
        }

        ಆದರೆ (ngx_strcmp(p, "rw") == 0) {
            ಬಲ = 6;

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_strcmp(p, "r") == 0) {
            ಬಲ = 4;

        } ಇಲ್ಲದಿದ್ದರೆ {
            ಹೋಗು invalid;
        }

        *access |= ಬಲ << ಸರಿಸು;
    }

    *access |= user;

    ಹಿಂತಿರುಗಿ NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &ಮೌಲ್ಯ[i]);

    ಹಿಂತಿರುಗಿ NGX_CONF_ERROR;
}


ngx_int_t
ngx_ಕೂಡಿಸು_ಮಾರ್ಗ(ngx_conf_t *cf, ngx_ಮಾರ್ಗ_t **ಸ್ಥಾನ)
{
    ngx_uint_t   i, n;
    ngx_ಮಾರ್ಗ_t  *ಮಾರ್ಗ, **p;

    ಮಾರ್ಗ = *ಸ್ಥಾನ;

    p = cf->ಚಕ್ರ->paths.elts;
    ಗಾಗಿ (i = 0; i < cf->ಚಕ್ರ->paths.nelts; i++) {
        ಆದರೆ (p[i]->name.len == ಮಾರ್ಗ->name.len
            && ngx_strcmp(p[i]->name.ದತ್ತಾಂಶ, ಮಾರ್ಗ->name.ದತ್ತಾಂಶ) == 0)
        {
            ಆದರೆ (p[i]->ದತ್ತಾಂಶ != ಮಾರ್ಗ->ದತ್ತಾಂಶ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->ಸಾಲು);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }

            ಗಾಗಿ (n = 0; n < NGX_MAX_ಮಾರ್ಗ_LEVEL; n++) {
                ಆದರೆ (p[i]->level[n] != ಮಾರ್ಗ->level[n]) {
                    ಆದರೆ (ಮಾರ್ಗ->conf_file == ಖಾಲಿ) {
                        ಆದರೆ (p[i]->conf_file == ಖಾಲಿ) {
                            ngx_log_error(NGX_LOG_EMERG, cf->ಲಘುಗಣಕ, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            ಹಿಂತಿರುಗಿ NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->ಲಘುಗಣಕ, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->ಸಾಲು);
                        ಹಿಂತಿರುಗಿ NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->ಸಾಲು);
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                ಆದರೆ (p[i]->level[n] == 0) {
                    ನಿಲ್ಲಿಸು;
                }
            }

            *ಸ್ಥಾನ = p[i];

            ಹಿಂತಿರುಗಿ NGX_OK;
        }
    }

    p = ngx_array_ತಳ್ಳು(&cf->ಚಕ್ರ->paths);
    ಆದರೆ (p == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    *p = ಮಾರ್ಗ;

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_ಮಾರ್ಗ_t      **ಮಾರ್ಗ;

    ಮಾರ್ಗ = ಚಕ್ರ->paths.elts;
    ಗಾಗಿ (i = 0; i < ಚಕ್ರ->paths.nelts; i++) {

        ಆದರೆ (ngx_create_dir(ಮಾರ್ಗ[i]->name.ದತ್ತಾಂಶ, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ಆದರೆ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, err,
                              ngx_create_dir_n " \"%s\" failed",
                              ಮಾರ್ಗ[i]->name.ದತ್ತಾಂಶ);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
        }

        ಆದರೆ (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ಮುಂದುವರಿ;
        }

#ಆದರೆ !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        ಆದರೆ (ngx_file_info(ಮಾರ್ಗ[i]->name.ದತ್ತಾಂಶ, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", ಮಾರ್ಗ[i]->name.ದತ್ತಾಂಶ);
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        ಆದರೆ (fi.st_uid != user) {
            ಆದರೆ (chown((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) ಮಾರ್ಗ[i]->name.ದತ್ತಾಂಶ, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              ಮಾರ್ಗ[i]->name.ದತ್ತಾಂಶ, user);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
        }

        ಆದರೆ ((fi.st_ಬಹುಲಕ & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_ಬಹುಲಕ |= (S_IRUSR|S_IWUSR|S_IXUSR);

            ಆದರೆ (chmod((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) ಮಾರ್ಗ[i]->name.ದತ್ತಾಂಶ, fi.st_ಬಹುಲಕ) == -1) {
                ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                              "chmod() \"%s\" failed", ಮಾರ್ಗ[i]->name.ದತ್ತಾಂಶ);
                ಹಿಂತಿರುಗಿ NGX_ERROR;
            }
        }
        }
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *ಗಮ್ಯ, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#ಆದರೆ !(NGX_WIN32)

    ಆದರೆ (ext->access) {
        ಆದರೆ (ngx_change_file_access(src->ದತ್ತಾಂಶ, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->ಲಘುಗಣಕ, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->ದತ್ತಾಂಶ);
            err = 0;
            ಹೋಗು failed;
        }
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (ext->ಸಮಯ != -1) {
        ಆದರೆ (ngx_ಗಣ_file_ಸಮಯ(src->ದತ್ತಾಂಶ, ext->fd, ext->ಸಮಯ) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->ಲಘುಗಣಕ, ngx_errno,
                          ngx_ಗಣ_file_ಸಮಯ_n " \"%s\" failed", src->ದತ್ತಾಂಶ);
            err = 0;
            ಹೋಗು failed;
        }
    }

    ಆದರೆ (ngx_rename_file(src->ದತ್ತಾಂಶ, ಗಮ್ಯ->ದತ್ತಾಂಶ) != NGX_FILE_ERROR) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    err = ngx_errno;

    ಆದರೆ (err == NGX_ENOPATH) {

        ಆದರೆ (!ext->create_ಮಾರ್ಗ) {
            ಹೋಗು failed;
        }

        err = ngx_create_ತುಂಬಿದ_ಮಾರ್ಗ(ಗಮ್ಯ->ದತ್ತಾಂಶ, ngx_dir_access(ext->ಮಾರ್ಗ_access));

        ಆದರೆ (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->ಲಘುಗಣಕ, err,
                          ngx_create_dir_n " \"%s\" failed", ಗಮ್ಯ->ದತ್ತಾಂಶ);
            err = 0;
            ಹೋಗು failed;
        }

        ಆದರೆ (ngx_rename_file(src->ದತ್ತಾಂಶ, ಗಮ್ಯ->ದತ್ತಾಂಶ) != NGX_FILE_ERROR) {
            ಹಿಂತಿರುಗಿ NGX_OK;
        }

        err = ngx_errno;
    }

#ಆದರೆ (NGX_WIN32)

    ಆದರೆ (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, ಗಮ್ಯ, ext->ಲಘುಗಣಕ);

        ಆದರೆ (err == 0) {
            ಹಿಂತಿರುಗಿ NGX_OK;
        }
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (err == NGX_EXDEV) {

        cf.ಗಾತ್ರ_2 = -1;
        cf.buf_ಗಾತ್ರ = 0;
        cf.access = ext->access;
        cf.ಸಮಯ = ext->ಸಮಯ;
        cf.ಲಘುಗಣಕ = ext->ಲಘುಗಣಕ;

        name = ngx_alloc(ಗಮ್ಯ->len + 1 + 10 + 1, ext->ಲಘುಗಣಕ);
        ಆದರೆ (name == ಖಾಲಿ) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        (ಶೂನ್ಯ) ngx_sprintf(name, "%*s.%010uD%Z", ಗಮ್ಯ->len, ಗಮ್ಯ->ದತ್ತಾಂಶ,
                           (uint32_t) ngx_ಮುಂದಿನ_ತಾತ್ಕಾಲಿಕ_ಸಂಖ್ಯೆ(0));

        ಆದರೆ (ngx_copy_file(src->ದತ್ತಾಂಶ, name, &cf) == NGX_OK) {

            ಆದರೆ (ngx_rename_file(name, ಗಮ್ಯ->ದತ್ತಾಂಶ) != NGX_FILE_ERROR) {
                ngx_free(name);

                ಆದರೆ (ngx_delete_file(src->ದತ್ತಾಂಶ) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->ಲಘುಗಣಕ, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->ದತ್ತಾಂಶ);
                    ಹಿಂತಿರುಗಿ NGX_ERROR;
                }

                ಹಿಂತಿರುಗಿ NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->ಲಘುಗಣಕ, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, ಗಮ್ಯ->ದತ್ತಾಂಶ);

            ಆದರೆ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->ಲಘುಗಣಕ, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    ಆದರೆ (ext->delete_file) {
        ಆದರೆ (ngx_delete_file(src->ದತ್ತಾಂಶ) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->ಲಘುಗಣಕ, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->ದತ್ತಾಂಶ);
        }
    }

    ಆದರೆ (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->ಲಘುಗಣಕ, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->ದತ್ತಾಂಶ, ಗಮ್ಯ->ದತ್ತಾಂಶ);
    }

    ಹಿಂತಿರುಗಿ NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *ಗಮ್ಯ, ngx_copy_file_t *cf)
{
    ಅಕ್ಷರ             *buf;
    off_t             ಗಾತ್ರ_2;
    ಸಮಯ_t            ಸಮಯ;
    ಗಾತ್ರ_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = ಖಾಲಿ;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    ಆದರೆ (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->ಲಘುಗಣಕ, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        ಹೋಗು failed;
    }

    ಆದರೆ (cf->ಗಾತ್ರ_2 != -1 && cf->access != 0 && cf->ಸಮಯ != -1) {
        ಗಾತ್ರ_2 = cf->ಗಾತ್ರ_2;
        access = cf->access;
        ಸಮಯ = cf->ಸಮಯ;

    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->ಲಘುಗಣಕ, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            ಹೋಗು failed;
        }

        ಗಾತ್ರ_2 = (cf->ಗಾತ್ರ_2 != -1) ? cf->ಗಾತ್ರ_2 : ngx_file_ಗಾತ್ರ(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        ಸಮಯ = (cf->ಸಮಯ != -1) ? cf->ಸಮಯ : ngx_file_mtime(&fi);
    }

    len = cf->buf_ಗಾತ್ರ ? cf->buf_ಗಾತ್ರ : 65536;

    ಆದರೆ ((off_t) len > ಗಾತ್ರ_2) {
        len = (ಗಾತ್ರ_t) ಗಾತ್ರ_2;
    }

    buf = ngx_alloc(len, cf->ಲಘುಗಣಕ);
    ಆದರೆ (buf == ಖಾಲಿ) {
        ಹೋಗು failed;
    }

    nfd = ngx_open_file(ಗಮ್ಯ, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    ಆದರೆ (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->ಲಘುಗಣಕ, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", ಗಮ್ಯ);
        ಹೋಗು failed;
    }

    ಆಗ (ಗಾತ್ರ_2 > 0) {

        ಆದರೆ ((off_t) len > ಗಾತ್ರ_2) {
            len = (ಗಾತ್ರ_t) ಗಾತ್ರ_2;
        }

        n = ngx_read_fd(fd, buf, len);

        ಆದರೆ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->ಲಘುಗಣಕ, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            ಹೋಗು failed;
        }

        ಆದರೆ ((ಗಾತ್ರ_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->ಲಘುಗಣಕ, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, ಗಾತ್ರ_2, from);
            ಹೋಗು failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        ಆದರೆ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->ಲಘುಗಣಕ, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", ಗಮ್ಯ);
            ಹೋಗು failed;
        }

        ಆದರೆ ((ಗಾತ್ರ_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->ಲಘುಗಣಕ, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, ಗಾತ್ರ_2, ಗಮ್ಯ);
            ಹೋಗು failed;
        }

        ಗಾತ್ರ_2 -= n;
    }

    ಆದರೆ (ngx_ಗಣ_file_ಸಮಯ(ಗಮ್ಯ, nfd, ಸಮಯ) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->ಲಘುಗಣಕ, ngx_errno,
                      ngx_ಗಣ_file_ಸಮಯ_n " \"%s\" failed", ಗಮ್ಯ);
        ಹೋಗು failed;
    }

    rc = NGX_OK;

failed:

    ಆದರೆ (nfd != NGX_INVALID_FILE) {
        ಆದರೆ (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->ಲಘುಗಣಕ, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", ಗಮ್ಯ);
        }
    }

    ಆದರೆ (fd != NGX_INVALID_FILE) {
        ಆದರೆ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->ಲಘುಗಣಕ, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    ಆದರೆ (buf) {
        ngx_free(buf);
    }

    ಹಿಂತಿರುಗಿ rc;
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
ngx_walk_ವೃಕ್ಷ(ngx_ವೃಕ್ಷ_ctx_t *ctx, ngx_str_t *ವೃಕ್ಷ)
{
    ಶೂನ್ಯ       *ದತ್ತಾಂಶ, *prev;
    u_char     *p, *name;
    ಗಾತ್ರ_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->ಲಘುಗಣಕ, 0,
                   "walk tree \"%V\"", ವೃಕ್ಷ);

    ಆದರೆ (ngx_open_dir(ವೃಕ್ಷ, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->ಲಘುಗಣಕ, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", ವೃಕ್ಷ->ದತ್ತಾಂಶ);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    prev = ctx->ದತ್ತಾಂಶ;

    ಆದರೆ (ctx->alloc) {
        ದತ್ತಾಂಶ = ngx_alloc(ctx->alloc, ctx->ಲಘುಗಣಕ);
        ಆದರೆ (ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ಹೋಗು failed;
        }

        ಆದರೆ (ctx->init_handler(ದತ್ತಾಂಶ, prev) == NGX_ABORT) {
            ಹೋಗು failed;
        }

        ctx->ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;

    } ಇಲ್ಲದಿದ್ದರೆ {
        ದತ್ತಾಂಶ = ಖಾಲಿ;
    }

    ಗಾಗಿ ( ;; ) {

        ngx_ಗಣ_errno(0);

        ಆದರೆ (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            ಆದರೆ (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } ಇಲ್ಲದಿದ್ದರೆ {
                ngx_log_error(NGX_LOG_CRIT, ctx->ಲಘುಗಣಕ, err,
                              ngx_read_dir_n " \"%s\" failed", ವೃಕ್ಷ->ದತ್ತಾಂಶ);
                rc = NGX_ERROR;
            }

            ಹೋಗು ಮುಗಿದ;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->ಲಘುಗಣಕ, 0,
                      "tree name %uz:\"%s\"", len, name);

        ಆದರೆ (len == 1 && name[0] == '.') {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (len == 2 && name[0] == '.' && name[1] == '.') {
            ಮುಂದುವರಿ;
        }

        file.len = ವೃಕ್ಷ->len + 1 + len;

        ಆದರೆ (file.len > buf.len) {

            ಆದರೆ (buf.len) {
                ngx_free(buf.ದತ್ತಾಂಶ);
            }

            buf.len = ವೃಕ್ಷ->len + 1 + len;

            buf.ದತ್ತಾಂಶ = ngx_alloc(buf.len + 1, ctx->ಲಘುಗಣಕ);
            ಆದರೆ (buf.ದತ್ತಾಂಶ == ಖಾಲಿ) {
                ಹೋಗು failed;
            }
        }

        p = ngx_cpymem(buf.ದತ್ತಾಂಶ, ವೃಕ್ಷ->ದತ್ತಾಂಶ, ವೃಕ್ಷ->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.ದತ್ತಾಂಶ = buf.ದತ್ತಾಂಶ;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->ಲಘುಗಣಕ, 0,
                       "tree path \"%s\"", file.ದತ್ತಾಂಶ);

        ಆದರೆ (!dir.valid_info) {
            ಆದರೆ (ngx_de_info(file.ದತ್ತಾಂಶ, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->ಲಘುಗಣಕ, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.ದತ್ತಾಂಶ);
                ಮುಂದುವರಿ;
            }
        }

        ಆದರೆ (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->ಲಘುಗಣಕ, 0,
                           "tree file \"%s\"", file.ದತ್ತಾಂಶ);

            ctx->ಗಾತ್ರ_2 = ngx_de_ಗಾತ್ರ(&dir);
            ctx->fs_ಗಾತ್ರ = ngx_de_fs_ಗಾತ್ರ(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ಆದರೆ (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                ಹೋಗು failed;
            }

        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->ಲಘುಗಣಕ, 0,
                           "tree enter dir \"%s\"", file.ದತ್ತಾಂಶ);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_ವೃಕ್ಷ_handler(ctx, &file);

            ಆದರೆ (rc == NGX_ABORT) {
                ಹೋಗು failed;
            }

            ಆದರೆ (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->ಲಘುಗಣಕ, 0,
                               "tree skip dir \"%s\"", file.ದತ್ತಾಂಶ);
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (ngx_walk_ವೃಕ್ಷ(ctx, &file) == NGX_ABORT) {
                ಹೋಗು failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ಆದರೆ (ctx->post_ವೃಕ್ಷ_handler(ctx, &file) == NGX_ABORT) {
                ಹೋಗು failed;
            }

        } ಇಲ್ಲದಿದ್ದರೆ {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->ಲಘುಗಣಕ, 0,
                           "tree special \"%s\"", file.ದತ್ತಾಂಶ);

            ಆದರೆ (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                ಹೋಗು failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

ಮುಗಿದ:

    ಆದರೆ (buf.len) {
        ngx_free(buf.ದತ್ತಾಂಶ);
    }

    ಆದರೆ (ದತ್ತಾಂಶ) {
        ngx_free(ದತ್ತಾಂಶ);
        ctx->ದತ್ತಾಂಶ = prev;
    }

    ಆದರೆ (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->ಲಘುಗಣಕ, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", ವೃಕ್ಷ->ದತ್ತಾಂಶ);
    }

    ಹಿಂತಿರುಗಿ rc;
}
