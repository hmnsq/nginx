
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


స్థిర ngx_int_t ngx_test_నిండిన_name(ngx_str_t *name);


స్థిర ngx_atomic_t   తాత్కాలిక_సంఖ్య = 0;
ngx_atomic_t         *ngx_తాత్కాలిక_సంఖ్య = &తాత్కాలిక_సంఖ్య;
ngx_atomic_int_t      ngx_random_సంఖ్య = 123456;


ngx_int_t
ngx_get_నిండిన_name(ngx_pool_t *pool, ngx_str_t *ఉపసర్గ, ngx_str_t *name)
{
    కొలత_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_నిండిన_name(name);

    ఒకవేళ (rc == NGX_OK) {
        తిరిగి rc;
    }

    len = ఉపసర్గ->len;

#ఒకవేళ (NGX_WIN32)

    ఒకవేళ (rc == 2) {
        len = rc;
    }

#అయితే_ముగింపు

    n = ngx_pnalloc(pool, len + name->len + 1);
    ఒకవేళ (n == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    p = ngx_cpymem(n, ఉపసర్గ->దత్తాంశం, len);
    ngx_cpystrn(p, name->దత్తాంశం, name->len + 1);

    name->len += len;
    name->దత్తాంశం = n;

    తిరిగి NGX_OK;
}


స్థిర ngx_int_t
ngx_test_నిండిన_name(ngx_str_t *name)
{
#ఒకవేళ (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->దత్తాంశం[0];

    ఒకవేళ (name->len < 2) {
        ఒకవేళ (c0 == '/') {
            తిరిగి 2;
        }

        తిరిగి NGX_DECLINED;
    }

    c1 = name->దత్తాంశం[1];

    ఒకవేళ (c1 == ':') {
        c0 |= 0x20;

        ఒకవేళ ((c0 >= 'a' && c0 <= 'z')) {
            తిరిగి NGX_OK;
        }

        తిరిగి NGX_DECLINED;
    }

    ఒకవేళ (c1 == '/') {
        తిరిగి NGX_OK;
    }

    ఒకవేళ (c0 == '/') {
        తిరిగి 2;
    }

    తిరిగి NGX_DECLINED;

#లేకపోతే

    ఒకవేళ (name->దత్తాంశం[0] == '/') {
        తిరిగి NGX_OK;
    }

    తిరిగి NGX_DECLINED;

#అయితే_ముగింపు
}


ssize_t
ngx_write_chain_గమ్యం_తాత్కాలిక_file(ngx_తాత్కాలిక_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    ఒకవేళ (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_తాత్కాలిక_file(&tf->file, tf->మార్గం, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        ఒకవేళ (rc != NGX_OK) {
            తిరిగి rc;
        }

        ఒకవేళ (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.సంవర్గమానం, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#ఒకవేళ (NGX_దారాలు && NGX_HAVE_PWRITEV)

    ఒకవేళ (tf->thread_write) {
        తిరిగి ngx_thread_write_chain_గమ్యం_file(&tf->file, chain, tf->స్థానభ్రంశం,
                                              tf->pool);
    }

#అయితే_ముగింపు

    తిరిగి ngx_write_chain_గమ్యం_file(&tf->file, chain, tf->స్థానభ్రంశం, tf->pool);
}


ngx_int_t
ngx_create_తాత్కాలిక_file(ngx_file_t *file, ngx_మార్గం_t *మార్గం, ngx_pool_t *pool,
    ngx_uint_t persistent, ngx_uint_t clean, ngx_uint_t access)
{
    కొలత_t                    స్థాయిలు;
    u_char                   *p;
    uint32_t                  n;
    ngx_err_t                 err;
    ngx_str_t                 name;
    ngx_uint_t                ఉపసర్గ;
    ngx_pool_cleanup_t       *cln;
    ngx_pool_cleanup_file_t  *clnf;

    ఒకవేళ (file->name.len) {
        name = file->name;
        స్థాయిలు = 0;
        ఉపసర్గ = 1;

    } లేకపోతే {
        name = మార్గం->name;
        స్థాయిలు = మార్గం->len;
        ఉపసర్గ = 0;
    }

    file->name.len = name.len + 1 + స్థాయిలు + 10;

    file->name.దత్తాంశం = ngx_pnalloc(pool, file->name.len + 1);
    ఒకవేళ (file->name.దత్తాంశం == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

#ఒకవేళ 0
    కొరకు (i = 0; i < file->name.len; i++) {
        file->name.దత్తాంశం[i] = 'X';
    }
#అయితే_ముగింపు

    p = ngx_cpymem(file->name.దత్తాంశం, name.దత్తాంశం, name.len);

    ఒకవేళ (ఉపసర్గ) {
        *p = '.';
    }

    p += 1 + స్థాయిలు;

    n = (uint32_t) ngx_తదుపరి_తాత్కాలిక_సంఖ్య(0);

    cln = ngx_pool_cleanup_కూడు(pool, పరిమాణం(ngx_pool_cleanup_file_t));
    ఒకవేళ (cln == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    కొరకు ( ;; ) {
        (శూన్య) ngx_sprintf(p, "%010uD%Z", n);

        ఒకవేళ (!ఉపసర్గ) {
            ngx_create_hashed_filename(మార్గం, file->name.దత్తాంశం, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->సంవర్గమానం, 0,
                       "hashed path: %s", file->name.దత్తాంశం);

        file->fd = ngx_open_tempfile(file->name.దత్తాంశం, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->సంవర్గమానం, 0,
                       "temp fd:%d", file->fd);

        ఒకవేళ (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->దత్తాంశం;

            clnf->fd = file->fd;
            clnf->name = file->name.దత్తాంశం;
            clnf->సంవర్గమానం = pool->సంవర్గమానం;

            తిరిగి NGX_OK;
        }

        err = ngx_errno;

        ఒకవేళ (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_తదుపరి_తాత్కాలిక_సంఖ్య(1);
            కొనసాగు;
        }

        ఒకవేళ ((మార్గం->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->సంవర్గమానం, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.దత్తాంశం);
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (ngx_create_మార్గం(file, మార్గం) == NGX_ERROR) {
            తిరిగి NGX_ERROR;
        }
    }
}


శూన్య
ngx_create_hashed_filename(ngx_మార్గం_t *మార్గం, u_char *file, కొలత_t len)
{
    కొలత_t      i, level;
    ngx_uint_t  n;

    i = మార్గం->name.len + 1;

    file[మార్గం->name.len + మార్గం->len]  = '/';

    కొరకు (n = 0; n < NGX_MAX_మార్గం_LEVEL; n++) {
        level = మార్గం->level[n];

        ఒకవేళ (level == 0) {
            ఆపు;
        }

        len -= level;
        file[i - 1] = '/';
        ngx_memcpy(&file[i], &file[len], level);
        i += level + 1;
    }
}


ngx_int_t
ngx_create_మార్గం(ngx_file_t *file, ngx_మార్గం_t *మార్గం)
{
    కొలత_t      pos;
    ngx_err_t   err;
    ngx_uint_t  i;

    pos = మార్గం->name.len;

    కొరకు (i = 0; i < NGX_MAX_మార్గం_LEVEL; i++) {
        ఒకవేళ (మార్గం->level[i] == 0) {
            ఆపు;
        }

        pos += మార్గం->level[i] + 1;

        file->name.దత్తాంశం[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->సంవర్గమానం, 0,
                       "temp file: \"%s\"", file->name.దత్తాంశం);

        ఒకవేళ (ngx_create_dir(file->name.దత్తాంశం, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ఒకవేళ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->సంవర్గమానం, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.దత్తాంశం);
                తిరిగి NGX_ERROR;
            }
        }

        file->name.దత్తాంశం[pos] = '/';
    }

    తిరిగి NGX_OK;
}


ngx_err_t
ngx_create_నిండిన_మార్గం(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#ఒకవేళ (NGX_WIN32)
    p = dir + 3;
#లేకపోతే
    p = dir + 1;
#అయితే_ముగింపు

    కొరకు ( /* void */ ; *p; p++) {
        ch = *p;

        ఒకవేళ (ch != '/') {
            కొనసాగు;
        }

        *p = '\0';

        ఒకవేళ (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            ఎంపిక (err) {
            స్థితి NGX_EEXIST:
                err = 0;
            స్థితి NGX_EACCES:
                ఆపు;

            అప్రమేయ:
                తిరిగి err;
            }
        }

        *p = '/';
    }

    తిరిగి err;
}


ngx_atomic_uint_t
ngx_తదుపరి_తాత్కాలిక_సంఖ్య(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, కూడు;

    కూడు = collision ? ngx_random_సంఖ్య : 1;

    n = ngx_atomic_fetch_కూడు(ngx_తాత్కాలిక_సంఖ్య, కూడు);

    తిరిగి n + కూడు;
}


అక్షరం *
ngx_conf_సమితి_మార్గం_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం  *p = conf;

    ssize_t      level;
    ngx_str_t   *విలువ;
    ngx_uint_t   i, n;
    ngx_మార్గం_t  *మార్గం, **స్థానం;

    స్థానం = (ngx_మార్గం_t **) (p + cmd->స్థానభ్రంశం);

    ఒకవేళ (*స్థానం) {
        తిరిగి "is duplicate";
    }

    మార్గం = ngx_pcalloc(cf->pool, పరిమాణం(ngx_మార్గం_t));
    ఒకవేళ (మార్గం == ఖాళీ) {
        తిరిగి NGX_CONF_ERROR;
    }

    విలువ = cf->వాదనలు->elts;

    మార్గం->name = విలువ[1];

    ఒకవేళ (మార్గం->name.దత్తాంశం[మార్గం->name.len - 1] == '/') {
        మార్గం->name.len--;
    }

    ఒకవేళ (ngx_conf_నిండిన_name(cf->చక్రం, &మార్గం->name, 0) != NGX_OK) {
        తిరిగి NGX_CONF_ERROR;
    }

    మార్గం->conf_file = cf->conf_file->file.name.దత్తాంశం;
    మార్గం->పంక్తి = cf->conf_file->పంక్తి;

    కొరకు (i = 0, n = 2; n < cf->వాదనలు->nelts; i++, n++) {
        level = ngx_atoi(విలువ[n].దత్తాంశం, విలువ[n].len);
        ఒకవేళ (level == NGX_ERROR || level == 0) {
            తిరిగి "invalid value";
        }

        మార్గం->level[i] = level;
        మార్గం->len += level + 1;
    }

    ఒకవేళ (మార్గం->len > 10 + i) {
        తిరిగి "invalid value";
    }

    *స్థానం = మార్గం;

    ఒకవేళ (ngx_కూడు_మార్గం(cf, స్థానం) == NGX_ERROR) {
        తిరిగి NGX_CONF_ERROR;
    }

    తిరిగి NGX_CONF_OK;
}


అక్షరం *
ngx_conf_విలీనం_మార్గం_విలువ(ngx_conf_t *cf, ngx_మార్గం_t **మార్గం, ngx_మార్గం_t *prev,
    ngx_మార్గం_init_t *init)
{
    ngx_uint_t  i;

    ఒకవేళ (*మార్గం) {
        తిరిగి NGX_CONF_OK;
    }

    ఒకవేళ (prev) {
        *మార్గం = prev;
        తిరిగి NGX_CONF_OK;
    }

    *మార్గం = ngx_pcalloc(cf->pool, పరిమాణం(ngx_మార్గం_t));
    ఒకవేళ (*మార్గం == ఖాళీ) {
        తిరిగి NGX_CONF_ERROR;
    }

    (*మార్గం)->name = init->name;

    ఒకవేళ (ngx_conf_నిండిన_name(cf->చక్రం, &(*మార్గం)->name, 0) != NGX_OK) {
        తిరిగి NGX_CONF_ERROR;
    }

    కొరకు (i = 0; i < NGX_MAX_మార్గం_LEVEL; i++) {
        (*మార్గం)->level[i] = init->level[i];
        (*మార్గం)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    ఒకవేళ (ngx_కూడు_మార్గం(cf, మార్గం) != NGX_OK) {
        తిరిగి NGX_CONF_ERROR;
    }

    తిరిగి NGX_CONF_OK;
}


అక్షరం *
ngx_conf_సమితి_access_స్థానం(ngx_conf_t *cf, ngx_command_t *cmd, శూన్య *conf)
{
    అక్షరం  *confp = conf;

    u_char      *p;
    ngx_str_t   *విలువ;
    ngx_uint_t   i, కుడి, జరుపు, *access, user;

    access = (ngx_uint_t *) (confp + cmd->స్థానభ్రంశం);

    ఒకవేళ (*access != NGX_CONF_UNSET_UINT) {
        తిరిగి "is duplicate";
    }

    విలువ = cf->వాదనలు->elts;

    *access = 0;
    user = 0600;

    కొరకు (i = 1; i < cf->వాదనలు->nelts; i++) {

        p = విలువ[i].దత్తాంశం;

        ఒకవేళ (ngx_strncmp(p, "user:", పరిమాణం("user:") - 1) == 0) {
            జరుపు = 6;
            p += పరిమాణం("user:") - 1;
            user = 0;

        } లేకపోతే ఒకవేళ (ngx_strncmp(p, "group:", పరిమాణం("group:") - 1) == 0) {
            జరుపు = 3;
            p += పరిమాణం("group:") - 1;

        } లేకపోతే ఒకవేళ (ngx_strncmp(p, "all:", పరిమాణం("all:") - 1) == 0) {
            జరుపు = 0;
            p += పరిమాణం("all:") - 1;

        } లేకపోతే {
            వెళ్ళు invalid;
        }

        ఒకవేళ (ngx_strcmp(p, "rw") == 0) {
            కుడి = 6;

        } లేకపోతే ఒకవేళ (ngx_strcmp(p, "r") == 0) {
            కుడి = 4;

        } లేకపోతే {
            వెళ్ళు invalid;
        }

        *access |= కుడి << జరుపు;
    }

    *access |= user;

    తిరిగి NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &విలువ[i]);

    తిరిగి NGX_CONF_ERROR;
}


ngx_int_t
ngx_కూడు_మార్గం(ngx_conf_t *cf, ngx_మార్గం_t **స్థానం)
{
    ngx_uint_t   i, n;
    ngx_మార్గం_t  *మార్గం, **p;

    మార్గం = *స్థానం;

    p = cf->చక్రం->paths.elts;
    కొరకు (i = 0; i < cf->చక్రం->paths.nelts; i++) {
        ఒకవేళ (p[i]->name.len == మార్గం->name.len
            && ngx_strcmp(p[i]->name.దత్తాంశం, మార్గం->name.దత్తాంశం) == 0)
        {
            ఒకవేళ (p[i]->దత్తాంశం != మార్గం->దత్తాంశం) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->పంక్తి);
                తిరిగి NGX_ERROR;
            }

            కొరకు (n = 0; n < NGX_MAX_మార్గం_LEVEL; n++) {
                ఒకవేళ (p[i]->level[n] != మార్గం->level[n]) {
                    ఒకవేళ (మార్గం->conf_file == ఖాళీ) {
                        ఒకవేళ (p[i]->conf_file == ఖాళీ) {
                            ngx_log_error(NGX_LOG_EMERG, cf->సంవర్గమానం, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            తిరిగి NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->సంవర్గమానం, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->పంక్తి);
                        తిరిగి NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->పంక్తి);
                    తిరిగి NGX_ERROR;
                }

                ఒకవేళ (p[i]->level[n] == 0) {
                    ఆపు;
                }
            }

            *స్థానం = p[i];

            తిరిగి NGX_OK;
        }
    }

    p = ngx_array_నెట్టు(&cf->చక్రం->paths);
    ఒకవేళ (p == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    *p = మార్గం;

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_చక్రం_t *చక్రం, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_మార్గం_t      **మార్గం;

    మార్గం = చక్రం->paths.elts;
    కొరకు (i = 0; i < చక్రం->paths.nelts; i++) {

        ఒకవేళ (ngx_create_dir(మార్గం[i]->name.దత్తాంశం, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            ఒకవేళ (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, err,
                              ngx_create_dir_n " \"%s\" failed",
                              మార్గం[i]->name.దత్తాంశం);
                తిరిగి NGX_ERROR;
            }
        }

        ఒకవేళ (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            కొనసాగు;
        }

#ఒకవేళ !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        ఒకవేళ (ngx_file_info(మార్గం[i]->name.దత్తాంశం, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", మార్గం[i]->name.దత్తాంశం);
            తిరిగి NGX_ERROR;
        }

        ఒకవేళ (fi.st_uid != user) {
            ఒకవేళ (chown((స్థిరరాశి అక్షరం *) మార్గం[i]->name.దత్తాంశం, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              మార్గం[i]->name.దత్తాంశం, user);
                తిరిగి NGX_ERROR;
            }
        }

        ఒకవేళ ((fi.st_బహుళకం & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_బహుళకం |= (S_IRUSR|S_IWUSR|S_IXUSR);

            ఒకవేళ (chmod((స్థిరరాశి అక్షరం *) మార్గం[i]->name.దత్తాంశం, fi.st_బహుళకం) == -1) {
                ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                              "chmod() \"%s\" failed", మార్గం[i]->name.దత్తాంశం);
                తిరిగి NGX_ERROR;
            }
        }
        }
#అయితే_ముగింపు
    }

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *గమ్యం, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#ఒకవేళ !(NGX_WIN32)

    ఒకవేళ (ext->access) {
        ఒకవేళ (ngx_change_file_access(src->దత్తాంశం, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->సంవర్గమానం, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->దత్తాంశం);
            err = 0;
            వెళ్ళు failed;
        }
    }

#అయితే_ముగింపు

    ఒకవేళ (ext->సమయం != -1) {
        ఒకవేళ (ngx_సమితి_file_సమయం(src->దత్తాంశం, ext->fd, ext->సమయం) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->సంవర్గమానం, ngx_errno,
                          ngx_సమితి_file_సమయం_n " \"%s\" failed", src->దత్తాంశం);
            err = 0;
            వెళ్ళు failed;
        }
    }

    ఒకవేళ (ngx_rename_file(src->దత్తాంశం, గమ్యం->దత్తాంశం) != NGX_FILE_ERROR) {
        తిరిగి NGX_OK;
    }

    err = ngx_errno;

    ఒకవేళ (err == NGX_ENOPATH) {

        ఒకవేళ (!ext->create_మార్గం) {
            వెళ్ళు failed;
        }

        err = ngx_create_నిండిన_మార్గం(గమ్యం->దత్తాంశం, ngx_dir_access(ext->మార్గం_access));

        ఒకవేళ (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->సంవర్గమానం, err,
                          ngx_create_dir_n " \"%s\" failed", గమ్యం->దత్తాంశం);
            err = 0;
            వెళ్ళు failed;
        }

        ఒకవేళ (ngx_rename_file(src->దత్తాంశం, గమ్యం->దత్తాంశం) != NGX_FILE_ERROR) {
            తిరిగి NGX_OK;
        }

        err = ngx_errno;
    }

#ఒకవేళ (NGX_WIN32)

    ఒకవేళ (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, గమ్యం, ext->సంవర్గమానం);

        ఒకవేళ (err == 0) {
            తిరిగి NGX_OK;
        }
    }

#అయితే_ముగింపు

    ఒకవేళ (err == NGX_EXDEV) {

        cf.కొలత = -1;
        cf.buf_కొలత = 0;
        cf.access = ext->access;
        cf.సమయం = ext->సమయం;
        cf.సంవర్గమానం = ext->సంవర్గమానం;

        name = ngx_alloc(గమ్యం->len + 1 + 10 + 1, ext->సంవర్గమానం);
        ఒకవేళ (name == ఖాళీ) {
            తిరిగి NGX_ERROR;
        }

        (శూన్య) ngx_sprintf(name, "%*s.%010uD%Z", గమ్యం->len, గమ్యం->దత్తాంశం,
                           (uint32_t) ngx_తదుపరి_తాత్కాలిక_సంఖ్య(0));

        ఒకవేళ (ngx_copy_file(src->దత్తాంశం, name, &cf) == NGX_OK) {

            ఒకవేళ (ngx_rename_file(name, గమ్యం->దత్తాంశం) != NGX_FILE_ERROR) {
                ngx_free(name);

                ఒకవేళ (ngx_delete_file(src->దత్తాంశం) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->సంవర్గమానం, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->దత్తాంశం);
                    తిరిగి NGX_ERROR;
                }

                తిరిగి NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->సంవర్గమానం, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, గమ్యం->దత్తాంశం);

            ఒకవేళ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->సంవర్గమానం, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    ఒకవేళ (ext->delete_file) {
        ఒకవేళ (ngx_delete_file(src->దత్తాంశం) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->సంవర్గమానం, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->దత్తాంశం);
        }
    }

    ఒకవేళ (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->సంవర్గమానం, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->దత్తాంశం, గమ్యం->దత్తాంశం);
    }

    తిరిగి NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *గమ్యం, ngx_copy_file_t *cf)
{
    అక్షరం             *buf;
    off_t             కొలత;
    సమయం_t            సమయం;
    కొలత_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = ఖాళీ;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    ఒకవేళ (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->సంవర్గమానం, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        వెళ్ళు failed;
    }

    ఒకవేళ (cf->కొలత != -1 && cf->access != 0 && cf->సమయం != -1) {
        కొలత = cf->కొలత;
        access = cf->access;
        సమయం = cf->సమయం;

    } లేకపోతే {
        ఒకవేళ (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->సంవర్గమానం, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            వెళ్ళు failed;
        }

        కొలత = (cf->కొలత != -1) ? cf->కొలత : ngx_file_కొలత(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        సమయం = (cf->సమయం != -1) ? cf->సమయం : ngx_file_mtime(&fi);
    }

    len = cf->buf_కొలత ? cf->buf_కొలత : 65536;

    ఒకవేళ ((off_t) len > కొలత) {
        len = (కొలత_t) కొలత;
    }

    buf = ngx_alloc(len, cf->సంవర్గమానం);
    ఒకవేళ (buf == ఖాళీ) {
        వెళ్ళు failed;
    }

    nfd = ngx_open_file(గమ్యం, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    ఒకవేళ (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->సంవర్గమానం, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", గమ్యం);
        వెళ్ళు failed;
    }

    అయితే (కొలత > 0) {

        ఒకవేళ ((off_t) len > కొలత) {
            len = (కొలత_t) కొలత;
        }

        n = ngx_read_fd(fd, buf, len);

        ఒకవేళ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->సంవర్గమానం, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            వెళ్ళు failed;
        }

        ఒకవేళ ((కొలత_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->సంవర్గమానం, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, కొలత, from);
            వెళ్ళు failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        ఒకవేళ (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->సంవర్గమానం, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", గమ్యం);
            వెళ్ళు failed;
        }

        ఒకవేళ ((కొలత_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->సంవర్గమానం, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, కొలత, గమ్యం);
            వెళ్ళు failed;
        }

        కొలత -= n;
    }

    ఒకవేళ (ngx_సమితి_file_సమయం(గమ్యం, nfd, సమయం) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->సంవర్గమానం, ngx_errno,
                      ngx_సమితి_file_సమయం_n " \"%s\" failed", గమ్యం);
        వెళ్ళు failed;
    }

    rc = NGX_OK;

failed:

    ఒకవేళ (nfd != NGX_INVALID_FILE) {
        ఒకవేళ (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->సంవర్గమానం, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", గమ్యం);
        }
    }

    ఒకవేళ (fd != NGX_INVALID_FILE) {
        ఒకవేళ (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->సంవర్గమానం, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    ఒకవేళ (buf) {
        ngx_free(buf);
    }

    తిరిగి rc;
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
ngx_walk_వృక్షం(ngx_వృక్షం_ctx_t *ctx, ngx_str_t *వృక్షం)
{
    శూన్య       *దత్తాంశం, *prev;
    u_char     *p, *name;
    కొలత_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->సంవర్గమానం, 0,
                   "walk tree \"%V\"", వృక్షం);

    ఒకవేళ (ngx_open_dir(వృక్షం, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->సంవర్గమానం, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", వృక్షం->దత్తాంశం);
        తిరిగి NGX_ERROR;
    }

    prev = ctx->దత్తాంశం;

    ఒకవేళ (ctx->alloc) {
        దత్తాంశం = ngx_alloc(ctx->alloc, ctx->సంవర్గమానం);
        ఒకవేళ (దత్తాంశం == ఖాళీ) {
            వెళ్ళు failed;
        }

        ఒకవేళ (ctx->init_handler(దత్తాంశం, prev) == NGX_ABORT) {
            వెళ్ళు failed;
        }

        ctx->దత్తాంశం = దత్తాంశం;

    } లేకపోతే {
        దత్తాంశం = ఖాళీ;
    }

    కొరకు ( ;; ) {

        ngx_సమితి_errno(0);

        ఒకవేళ (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            ఒకవేళ (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } లేకపోతే {
                ngx_log_error(NGX_LOG_CRIT, ctx->సంవర్గమానం, err,
                              ngx_read_dir_n " \"%s\" failed", వృక్షం->దత్తాంశం);
                rc = NGX_ERROR;
            }

            వెళ్ళు పూర్తి;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->సంవర్గమానం, 0,
                      "tree name %uz:\"%s\"", len, name);

        ఒకవేళ (len == 1 && name[0] == '.') {
            కొనసాగు;
        }

        ఒకవేళ (len == 2 && name[0] == '.' && name[1] == '.') {
            కొనసాగు;
        }

        file.len = వృక్షం->len + 1 + len;

        ఒకవేళ (file.len > buf.len) {

            ఒకవేళ (buf.len) {
                ngx_free(buf.దత్తాంశం);
            }

            buf.len = వృక్షం->len + 1 + len;

            buf.దత్తాంశం = ngx_alloc(buf.len + 1, ctx->సంవర్గమానం);
            ఒకవేళ (buf.దత్తాంశం == ఖాళీ) {
                వెళ్ళు failed;
            }
        }

        p = ngx_cpymem(buf.దత్తాంశం, వృక్షం->దత్తాంశం, వృక్షం->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.దత్తాంశం = buf.దత్తాంశం;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->సంవర్గమానం, 0,
                       "tree path \"%s\"", file.దత్తాంశం);

        ఒకవేళ (!dir.valid_info) {
            ఒకవేళ (ngx_de_info(file.దత్తాంశం, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->సంవర్గమానం, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.దత్తాంశం);
                కొనసాగు;
            }
        }

        ఒకవేళ (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->సంవర్గమానం, 0,
                           "tree file \"%s\"", file.దత్తాంశం);

            ctx->కొలత = ngx_de_కొలత(&dir);
            ctx->fs_కొలత = ngx_de_fs_కొలత(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ఒకవేళ (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                వెళ్ళు failed;
            }

        } లేకపోతే ఒకవేళ (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->సంవర్గమానం, 0,
                           "tree enter dir \"%s\"", file.దత్తాంశం);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_వృక్షం_handler(ctx, &file);

            ఒకవేళ (rc == NGX_ABORT) {
                వెళ్ళు failed;
            }

            ఒకవేళ (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->సంవర్గమానం, 0,
                               "tree skip dir \"%s\"", file.దత్తాంశం);
                కొనసాగు;
            }

            ఒకవేళ (ngx_walk_వృక్షం(ctx, &file) == NGX_ABORT) {
                వెళ్ళు failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            ఒకవేళ (ctx->post_వృక్షం_handler(ctx, &file) == NGX_ABORT) {
                వెళ్ళు failed;
            }

        } లేకపోతే {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->సంవర్గమానం, 0,
                           "tree special \"%s\"", file.దత్తాంశం);

            ఒకవేళ (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                వెళ్ళు failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

పూర్తి:

    ఒకవేళ (buf.len) {
        ngx_free(buf.దత్తాంశం);
    }

    ఒకవేళ (దత్తాంశం) {
        ngx_free(దత్తాంశం);
        ctx->దత్తాంశం = prev;
    }

    ఒకవేళ (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->సంవర్గమానం, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", వృక్షం->దత్తాంశం);
    }

    తిరిగి rc;
}
