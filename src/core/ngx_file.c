
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


स्थिर ngx_int_t ngx_test_पूर्णम्_name(ngx_str_t *name);


स्थिर ngx_atomic_t   अस्थायी_सङ्ख्या = 0;
ngx_atomic_t         *ngx_अस्थायी_सङ्ख्या = &अस्थायी_सङ्ख्या;
ngx_atomic_int_t      ngx_random_सङ्ख्या = 123456;


ngx_int_t
ngx_get_पूर्णम्_name(ngx_pool_t *pool, ngx_str_t *उपसर्गः, ngx_str_t *name)
{
    आकार_t      len;
    u_char     *p, *n;
    ngx_int_t   rc;

    rc = ngx_test_पूर्णम्_name(name);

    यदि (rc == NGX_OK) {
        प्रत्यागम rc;
    }

    len = उपसर्गः->len;

#यदि (NGX_WIN32)

    यदि (rc == 2) {
        len = rc;
    }

#यदि_अन्तः

    n = ngx_pnalloc(pool, len + name->len + 1);
    यदि (n == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    p = ngx_cpymem(n, उपसर्गः->दत्त, len);
    ngx_cpystrn(p, name->दत्त, name->len + 1);

    name->len += len;
    name->दत्त = n;

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_test_पूर्णम्_name(ngx_str_t *name)
{
#यदि (NGX_WIN32)
    u_char  c0, c1;

    c0 = name->दत्त[0];

    यदि (name->len < 2) {
        यदि (c0 == '/') {
            प्रत्यागम 2;
        }

        प्रत्यागम NGX_DECLINED;
    }

    c1 = name->दत्त[1];

    यदि (c1 == ':') {
        c0 |= 0x20;

        यदि ((c0 >= 'a' && c0 <= 'z')) {
            प्रत्यागम NGX_OK;
        }

        प्रत्यागम NGX_DECLINED;
    }

    यदि (c1 == '/') {
        प्रत्यागम NGX_OK;
    }

    यदि (c0 == '/') {
        प्रत्यागम 2;
    }

    प्रत्यागम NGX_DECLINED;

#अन्यथा

    यदि (name->दत्त[0] == '/') {
        प्रत्यागम NGX_OK;
    }

    प्रत्यागम NGX_DECLINED;

#यदि_अन्तः
}


ssize_t
ngx_write_chain_गन्तव्यम्_अस्थायी_file(ngx_अस्थायी_file_t *tf, ngx_chain_t *chain)
{
    ngx_int_t  rc;

    यदि (tf->file.fd == NGX_INVALID_FILE) {
        rc = ngx_create_अस्थायी_file(&tf->file, tf->मार्गः, tf->pool,
                                  tf->persistent, tf->clean, tf->access);

        यदि (rc != NGX_OK) {
            प्रत्यागम rc;
        }

        यदि (tf->log_level) {
            ngx_log_error(tf->log_level, tf->file.लघुगणकः, 0, "%s %V",
                          tf->warn, &tf->file.name);
        }
    }

#यदि (NGX_तन्तवः && NGX_HAVE_PWRITEV)

    यदि (tf->thread_write) {
        प्रत्यागम ngx_thread_write_chain_गन्तव्यम्_file(&tf->file, chain, tf->विस्थापनम्,
                                              tf->pool);
    }

#यदि_अन्तः

    प्रत्यागम ngx_write_chain_गन्तव्यम्_file(&tf->file, chain, tf->विस्थापनम्, tf->pool);
}


ngx_int_t
ngx_create_अस्थायी_file(ngx_file_t *file, ngx_मार्गः_t *मार्गः, ngx_pool_t *pool,
    ngx_uint_t persistent, ngx_uint_t clean, ngx_uint_t access)
{
    आकार_t                    स्तराः;
    u_char                   *p;
    uint32_t                  n;
    ngx_err_t                 err;
    ngx_str_t                 name;
    ngx_uint_t                उपसर्गः;
    ngx_pool_cleanup_t       *cln;
    ngx_pool_cleanup_file_t  *clnf;

    यदि (file->name.len) {
        name = file->name;
        स्तराः = 0;
        उपसर्गः = 1;

    } अन्यथा {
        name = मार्गः->name;
        स्तराः = मार्गः->len;
        उपसर्गः = 0;
    }

    file->name.len = name.len + 1 + स्तराः + 10;

    file->name.दत्त = ngx_pnalloc(pool, file->name.len + 1);
    यदि (file->name.दत्त == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

#यदि 0
    प्रति (i = 0; i < file->name.len; i++) {
        file->name.दत्त[i] = 'X';
    }
#यदि_अन्तः

    p = ngx_cpymem(file->name.दत्त, name.दत्त, name.len);

    यदि (उपसर्गः) {
        *p = '.';
    }

    p += 1 + स्तराः;

    n = (uint32_t) ngx_अग्र_अस्थायी_सङ्ख्या(0);

    cln = ngx_pool_cleanup_सङ्कलय(pool, परिमाण(ngx_pool_cleanup_file_t));
    यदि (cln == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    प्रति ( ;; ) {
        (शून्य) ngx_sprintf(p, "%010uD%Z", n);

        यदि (!उपसर्गः) {
            ngx_create_hashed_filename(मार्गः, file->name.दत्त, file->name.len);
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->लघुगणकः, 0,
                       "hashed path: %s", file->name.दत्त);

        file->fd = ngx_open_tempfile(file->name.दत्त, persistent, access);

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->लघुगणकः, 0,
                       "temp fd:%d", file->fd);

        यदि (file->fd != NGX_INVALID_FILE) {

            cln->handler = clean ? ngx_pool_delete_file : ngx_pool_cleanup_file;
            clnf = cln->दत्त;

            clnf->fd = file->fd;
            clnf->name = file->name.दत्त;
            clnf->लघुगणकः = pool->लघुगणकः;

            प्रत्यागम NGX_OK;
        }

        err = ngx_errno;

        यदि (err == NGX_EEXIST_FILE) {
            n = (uint32_t) ngx_अग्र_अस्थायी_सङ्ख्या(1);
            अनुवर्त;
        }

        यदि ((मार्गः->level[0] == 0) || (err != NGX_ENOPATH)) {
            ngx_log_error(NGX_LOG_CRIT, file->लघुगणकः, err,
                          ngx_open_tempfile_n " \"%s\" failed",
                          file->name.दत्त);
            प्रत्यागम NGX_ERROR;
        }

        यदि (ngx_create_मार्गः(file, मार्गः) == NGX_ERROR) {
            प्रत्यागम NGX_ERROR;
        }
    }
}


शून्य
ngx_create_hashed_filename(ngx_मार्गः_t *मार्गः, u_char *file, आकार_t len)
{
    आकार_t      i, level;
    ngx_uint_t  n;

    i = मार्गः->name.len + 1;

    file[मार्गः->name.len + मार्गः->len]  = '/';

    प्रति (n = 0; n < NGX_MAX_मार्गः_LEVEL; n++) {
        level = मार्गः->level[n];

        यदि (level == 0) {
            भङ्ग;
        }

        len -= level;
        file[i - 1] = '/';
        ngx_memcpy(&file[i], &file[len], level);
        i += level + 1;
    }
}


ngx_int_t
ngx_create_मार्गः(ngx_file_t *file, ngx_मार्गः_t *मार्गः)
{
    आकार_t      pos;
    ngx_err_t   err;
    ngx_uint_t  i;

    pos = मार्गः->name.len;

    प्रति (i = 0; i < NGX_MAX_मार्गः_LEVEL; i++) {
        यदि (मार्गः->level[i] == 0) {
            भङ्ग;
        }

        pos += मार्गः->level[i] + 1;

        file->name.दत्त[pos] = '\0';

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, file->लघुगणकः, 0,
                       "temp file: \"%s\"", file->name.दत्त);

        यदि (ngx_create_dir(file->name.दत्त, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            यदि (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_CRIT, file->लघुगणकः, err,
                              ngx_create_dir_n " \"%s\" failed",
                              file->name.दत्त);
                प्रत्यागम NGX_ERROR;
            }
        }

        file->name.दत्त[pos] = '/';
    }

    प्रत्यागम NGX_OK;
}


ngx_err_t
ngx_create_पूर्णम्_मार्गः(u_char *dir, ngx_uint_t access)
{
    u_char     *p, ch;
    ngx_err_t   err;

    err = 0;

#यदि (NGX_WIN32)
    p = dir + 3;
#अन्यथा
    p = dir + 1;
#यदि_अन्तः

    प्रति ( /* void */ ; *p; p++) {
        ch = *p;

        यदि (ch != '/') {
            अनुवर्त;
        }

        *p = '\0';

        यदि (ngx_create_dir(dir, access) == NGX_FILE_ERROR) {
            err = ngx_errno;

            विकल्प (err) {
            पक्ष NGX_EEXIST:
                err = 0;
            पक्ष NGX_EACCES:
                भङ्ग;

            स्वभाव:
                प्रत्यागम err;
            }
        }

        *p = '/';
    }

    प्रत्यागम err;
}


ngx_atomic_uint_t
ngx_अग्र_अस्थायी_सङ्ख्या(ngx_uint_t collision)
{
    ngx_atomic_uint_t  n, सङ्कलय;

    सङ्कलय = collision ? ngx_random_सङ्ख्या : 1;

    n = ngx_atomic_fetch_सङ्कलय(ngx_अस्थायी_सङ्ख्या, सङ्कलय);

    प्रत्यागम n + सङ्कलय;
}


वर्ण *
ngx_conf_समुच्चयः_मार्गः_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *p = conf;

    ssize_t      level;
    ngx_str_t   *मानम्;
    ngx_uint_t   i, n;
    ngx_मार्गः_t  *मार्गः, **slot;

    slot = (ngx_मार्गः_t **) (p + cmd->विस्थापनम्);

    यदि (*slot) {
        प्रत्यागम "is duplicate";
    }

    मार्गः = ngx_pcalloc(cf->pool, परिमाण(ngx_मार्गः_t));
    यदि (मार्गः == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    मानम् = cf->तर्काः->elts;

    मार्गः->name = मानम्[1];

    यदि (मार्गः->name.दत्त[मार्गः->name.len - 1] == '/') {
        मार्गः->name.len--;
    }

    यदि (ngx_conf_पूर्णम्_name(cf->आवृत्तिः, &मार्गः->name, 0) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    मार्गः->conf_file = cf->conf_file->file.name.दत्त;
    मार्गः->पङ्क्तिः = cf->conf_file->पङ्क्तिः;

    प्रति (i = 0, n = 2; n < cf->तर्काः->nelts; i++, n++) {
        level = ngx_atoi(मानम्[n].दत्त, मानम्[n].len);
        यदि (level == NGX_ERROR || level == 0) {
            प्रत्यागम "invalid value";
        }

        मार्गः->level[i] = level;
        मार्गः->len += level + 1;
    }

    यदि (मार्गः->len > 10 + i) {
        प्रत्यागम "invalid value";
    }

    *slot = मार्गः;

    यदि (ngx_सङ्कलय_मार्गः(cf, slot) == NGX_ERROR) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_merge_मार्गः_मानम्(ngx_conf_t *cf, ngx_मार्गः_t **मार्गः, ngx_मार्गः_t *पूर्व,
    ngx_मार्गः_init_t *init)
{
    ngx_uint_t  i;

    यदि (*मार्गः) {
        प्रत्यागम NGX_CONF_OK;
    }

    यदि (पूर्व) {
        *मार्गः = पूर्व;
        प्रत्यागम NGX_CONF_OK;
    }

    *मार्गः = ngx_pcalloc(cf->pool, परिमाण(ngx_मार्गः_t));
    यदि (*मार्गः == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    (*मार्गः)->name = init->name;

    यदि (ngx_conf_पूर्णम्_name(cf->आवृत्तिः, &(*मार्गः)->name, 0) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    प्रति (i = 0; i < NGX_MAX_मार्गः_LEVEL; i++) {
        (*मार्गः)->level[i] = init->level[i];
        (*मार्गः)->len += init->level[i] + (init->level[i] ? 1 : 0);
    }

    यदि (ngx_सङ्कलय_मार्गः(cf, मार्गः) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    प्रत्यागम NGX_CONF_OK;
}


वर्ण *
ngx_conf_समुच्चयः_access_slot(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    वर्ण  *confp = conf;

    u_char      *p;
    ngx_str_t   *मानम्;
    ngx_uint_t   i, दक्षिणः, सरणम्, *access, user;

    access = (ngx_uint_t *) (confp + cmd->विस्थापनम्);

    यदि (*access != NGX_CONF_UNSET_UINT) {
        प्रत्यागम "is duplicate";
    }

    मानम् = cf->तर्काः->elts;

    *access = 0;
    user = 0600;

    प्रति (i = 1; i < cf->तर्काः->nelts; i++) {

        p = मानम्[i].दत्त;

        यदि (ngx_strncmp(p, "user:", परिमाण("user:") - 1) == 0) {
            सरणम् = 6;
            p += परिमाण("user:") - 1;
            user = 0;

        } अन्यथा यदि (ngx_strncmp(p, "group:", परिमाण("group:") - 1) == 0) {
            सरणम् = 3;
            p += परिमाण("group:") - 1;

        } अन्यथा यदि (ngx_strncmp(p, "all:", परिमाण("all:") - 1) == 0) {
            सरणम् = 0;
            p += परिमाण("all:") - 1;

        } अन्यथा {
            गच्छ invalid;
        }

        यदि (ngx_strcmp(p, "rw") == 0) {
            दक्षिणः = 6;

        } अन्यथा यदि (ngx_strcmp(p, "r") == 0) {
            दक्षिणः = 4;

        } अन्यथा {
            गच्छ invalid;
        }

        *access |= दक्षिणः << सरणम्;
    }

    *access |= user;

    प्रत्यागम NGX_CONF_OK;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid value \"%V\"", &मानम्[i]);

    प्रत्यागम NGX_CONF_ERROR;
}


ngx_int_t
ngx_सङ्कलय_मार्गः(ngx_conf_t *cf, ngx_मार्गः_t **slot)
{
    ngx_uint_t   i, n;
    ngx_मार्गः_t  *मार्गः, **p;

    मार्गः = *slot;

    p = cf->आवृत्तिः->paths.elts;
    प्रति (i = 0; i < cf->आवृत्तिः->paths.nelts; i++) {
        यदि (p[i]->name.len == मार्गः->name.len
            && ngx_strcmp(p[i]->name.दत्त, मार्गः->name.दत्त) == 0)
        {
            यदि (p[i]->दत्त != मार्गः->दत्त) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "the same path name \"%V\" "
                                   "used in %s:%ui and",
                                   &p[i]->name, p[i]->conf_file, p[i]->पङ्क्तिः);
                प्रत्यागम NGX_ERROR;
            }

            प्रति (n = 0; n < NGX_MAX_मार्गः_LEVEL; n++) {
                यदि (p[i]->level[n] != मार्गः->level[n]) {
                    यदि (मार्गः->conf_file == रिक्त) {
                        यदि (p[i]->conf_file == रिक्त) {
                            ngx_log_error(NGX_LOG_EMERG, cf->लघुगणकः, 0,
                                      "the default path name \"%V\" has "
                                      "the same name as another default path, "
                                      "but the different levels, you need to "
                                      "redefine one of them in http section",
                                      &p[i]->name);
                            प्रत्यागम NGX_ERROR;
                        }

                        ngx_log_error(NGX_LOG_EMERG, cf->लघुगणकः, 0,
                                      "the path name \"%V\" in %s:%ui has "
                                      "the same name as default path, but "
                                      "the different levels, you need to "
                                      "define default path in http section",
                                      &p[i]->name, p[i]->conf_file, p[i]->पङ्क्तिः);
                        प्रत्यागम NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                      "the same path name \"%V\" in %s:%ui "
                                      "has the different levels than",
                                      &p[i]->name, p[i]->conf_file, p[i]->पङ्क्तिः);
                    प्रत्यागम NGX_ERROR;
                }

                यदि (p[i]->level[n] == 0) {
                    भङ्ग;
                }
            }

            *slot = p[i];

            प्रत्यागम NGX_OK;
        }
    }

    p = ngx_array_प्रक्षिप(&cf->आवृत्तिः->paths);
    यदि (p == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    *p = मार्गः;

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_create_paths(ngx_आवृत्तिः_t *आवृत्तिः, ngx_uid_t user)
{
    ngx_err_t         err;
    ngx_uint_t        i;
    ngx_मार्गः_t      **मार्गः;

    मार्गः = आवृत्तिः->paths.elts;
    प्रति (i = 0; i < आवृत्तिः->paths.nelts; i++) {

        यदि (ngx_create_dir(मार्गः[i]->name.दत्त, 0700) == NGX_FILE_ERROR) {
            err = ngx_errno;
            यदि (err != NGX_EEXIST) {
                ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, err,
                              ngx_create_dir_n " \"%s\" failed",
                              मार्गः[i]->name.दत्त);
                प्रत्यागम NGX_ERROR;
            }
        }

        यदि (user == (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            अनुवर्त;
        }

#यदि !(NGX_WIN32)
        {
        ngx_file_info_t   fi;

        यदि (ngx_file_info(मार्गः[i]->name.दत्त, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                          ngx_file_info_n " \"%s\" failed", मार्गः[i]->name.दत्त);
            प्रत्यागम NGX_ERROR;
        }

        यदि (fi.st_uid != user) {
            यदि (chown((ध्रुव वर्ण *) मार्गः[i]->name.दत्त, user, -1) == -1) {
                ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                              "chown(\"%s\", %d) failed",
                              मार्गः[i]->name.दत्त, user);
                प्रत्यागम NGX_ERROR;
            }
        }

        यदि ((fi.st_बहुलकम् & (S_IRUSR|S_IWUSR|S_IXUSR))
                                                  != (S_IRUSR|S_IWUSR|S_IXUSR))
        {
            fi.st_बहुलकम् |= (S_IRUSR|S_IWUSR|S_IXUSR);

            यदि (chmod((ध्रुव वर्ण *) मार्गः[i]->name.दत्त, fi.st_बहुलकम्) == -1) {
                ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                              "chmod() \"%s\" failed", मार्गः[i]->name.दत्त);
                प्रत्यागम NGX_ERROR;
            }
        }
        }
#यदि_अन्तः
    }

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_ext_rename_file(ngx_str_t *src, ngx_str_t *गन्तव्यम्, ngx_ext_rename_file_t *ext)
{
    u_char           *name;
    ngx_err_t         err;
    ngx_copy_file_t   cf;

#यदि !(NGX_WIN32)

    यदि (ext->access) {
        यदि (ngx_change_file_access(src->दत्त, ext->access) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->लघुगणकः, ngx_errno,
                          ngx_change_file_access_n " \"%s\" failed", src->दत्त);
            err = 0;
            गच्छ failed;
        }
    }

#यदि_अन्तः

    यदि (ext->time != -1) {
        यदि (ngx_समुच्चयः_file_time(src->दत्त, ext->fd, ext->time) != NGX_OK) {
            ngx_log_error(NGX_LOG_CRIT, ext->लघुगणकः, ngx_errno,
                          ngx_समुच्चयः_file_time_n " \"%s\" failed", src->दत्त);
            err = 0;
            गच्छ failed;
        }
    }

    यदि (ngx_rename_file(src->दत्त, गन्तव्यम्->दत्त) != NGX_FILE_ERROR) {
        प्रत्यागम NGX_OK;
    }

    err = ngx_errno;

    यदि (err == NGX_ENOPATH) {

        यदि (!ext->create_मार्गः) {
            गच्छ failed;
        }

        err = ngx_create_पूर्णम्_मार्गः(गन्तव्यम्->दत्त, ngx_dir_access(ext->मार्गः_access));

        यदि (err) {
            ngx_log_error(NGX_LOG_CRIT, ext->लघुगणकः, err,
                          ngx_create_dir_n " \"%s\" failed", गन्तव्यम्->दत्त);
            err = 0;
            गच्छ failed;
        }

        यदि (ngx_rename_file(src->दत्त, गन्तव्यम्->दत्त) != NGX_FILE_ERROR) {
            प्रत्यागम NGX_OK;
        }

        err = ngx_errno;
    }

#यदि (NGX_WIN32)

    यदि (err == NGX_EEXIST || err == NGX_EEXIST_FILE) {
        err = ngx_win32_rename_file(src, गन्तव्यम्, ext->लघुगणकः);

        यदि (err == 0) {
            प्रत्यागम NGX_OK;
        }
    }

#यदि_अन्तः

    यदि (err == NGX_EXDEV) {

        cf.आकार = -1;
        cf.buf_आकार = 0;
        cf.access = ext->access;
        cf.time = ext->time;
        cf.लघुगणकः = ext->लघुगणकः;

        name = ngx_alloc(गन्तव्यम्->len + 1 + 10 + 1, ext->लघुगणकः);
        यदि (name == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        (शून्य) ngx_sprintf(name, "%*s.%010uD%Z", गन्तव्यम्->len, गन्तव्यम्->दत्त,
                           (uint32_t) ngx_अग्र_अस्थायी_सङ्ख्या(0));

        यदि (ngx_copy_file(src->दत्त, name, &cf) == NGX_OK) {

            यदि (ngx_rename_file(name, गन्तव्यम्->दत्त) != NGX_FILE_ERROR) {
                ngx_free(name);

                यदि (ngx_delete_file(src->दत्त) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_CRIT, ext->लघुगणकः, ngx_errno,
                                  ngx_delete_file_n " \"%s\" failed",
                                  src->दत्त);
                    प्रत्यागम NGX_ERROR;
                }

                प्रत्यागम NGX_OK;
            }

            ngx_log_error(NGX_LOG_CRIT, ext->लघुगणकः, ngx_errno,
                          ngx_rename_file_n " \"%s\" to \"%s\" failed",
                          name, गन्तव्यम्->दत्त);

            यदि (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ext->लघुगणकः, ngx_errno,
                              ngx_delete_file_n " \"%s\" failed", name);

            }
        }

        ngx_free(name);

        err = 0;
    }

failed:

    यदि (ext->delete_file) {
        यदि (ngx_delete_file(src->दत्त) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_CRIT, ext->लघुगणकः, ngx_errno,
                          ngx_delete_file_n " \"%s\" failed", src->दत्त);
        }
    }

    यदि (err) {
        ngx_log_error(NGX_LOG_CRIT, ext->लघुगणकः, err,
                      ngx_rename_file_n " \"%s\" to \"%s\" failed",
                      src->दत्त, गन्तव्यम्->दत्त);
    }

    प्रत्यागम NGX_ERROR;
}


ngx_int_t
ngx_copy_file(u_char *from, u_char *गन्तव्यम्, ngx_copy_file_t *cf)
{
    वर्ण             *buf;
    off_t             आकार;
    time_t            time;
    आकार_t            len;
    ssize_t           n;
    ngx_fd_t          fd, nfd;
    ngx_int_t         rc;
    ngx_uint_t        access;
    ngx_file_info_t   fi;

    rc = NGX_ERROR;
    buf = रिक्त;
    nfd = NGX_INVALID_FILE;

    fd = ngx_open_file(from, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);

    यदि (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->लघुगणकः, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", from);
        गच्छ failed;
    }

    यदि (cf->आकार != -1 && cf->access != 0 && cf->time != -1) {
        आकार = cf->आकार;
        access = cf->access;
        time = cf->time;

    } अन्यथा {
        यदि (ngx_fd_info(fd, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->लघुगणकः, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", from);

            गच्छ failed;
        }

        आकार = (cf->आकार != -1) ? cf->आकार : ngx_file_आकार(&fi);
        access = cf->access ? cf->access : ngx_file_access(&fi);
        time = (cf->time != -1) ? cf->time : ngx_file_mtime(&fi);
    }

    len = cf->buf_आकार ? cf->buf_आकार : 65536;

    यदि ((off_t) len > आकार) {
        len = (आकार_t) आकार;
    }

    buf = ngx_alloc(len, cf->लघुगणकः);
    यदि (buf == रिक्त) {
        गच्छ failed;
    }

    nfd = ngx_open_file(गन्तव्यम्, NGX_FILE_WRONLY, NGX_FILE_TRUNCATE, access);

    यदि (nfd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_CRIT, cf->लघुगणकः, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", गन्तव्यम्);
        गच्छ failed;
    }

    यावत् (आकार > 0) {

        यदि ((off_t) len > आकार) {
            len = (आकार_t) आकार;
        }

        n = ngx_read_fd(fd, buf, len);

        यदि (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->लघुगणकः, ngx_errno,
                          ngx_read_fd_n " \"%s\" failed", from);
            गच्छ failed;
        }

        यदि ((आकार_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->लघुगणकः, 0,
                          ngx_read_fd_n " has read only %z of %O from %s",
                          n, आकार, from);
            गच्छ failed;
        }

        n = ngx_write_fd(nfd, buf, len);

        यदि (n == -1) {
            ngx_log_error(NGX_LOG_ALERT, cf->लघुगणकः, ngx_errno,
                          ngx_write_fd_n " \"%s\" failed", गन्तव्यम्);
            गच्छ failed;
        }

        यदि ((आकार_t) n != len) {
            ngx_log_error(NGX_LOG_ALERT, cf->लघुगणकः, 0,
                          ngx_write_fd_n " has written only %z of %O to %s",
                          n, आकार, गन्तव्यम्);
            गच्छ failed;
        }

        आकार -= n;
    }

    यदि (ngx_समुच्चयः_file_time(गन्तव्यम्, nfd, time) != NGX_OK) {
        ngx_log_error(NGX_LOG_ALERT, cf->लघुगणकः, ngx_errno,
                      ngx_समुच्चयः_file_time_n " \"%s\" failed", गन्तव्यम्);
        गच्छ failed;
    }

    rc = NGX_OK;

failed:

    यदि (nfd != NGX_INVALID_FILE) {
        यदि (ngx_close_file(nfd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->लघुगणकः, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", गन्तव्यम्);
        }
    }

    यदि (fd != NGX_INVALID_FILE) {
        यदि (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->लघुगणकः, ngx_errno,
                          ngx_close_file_n " \"%s\" failed", from);
        }
    }

    यदि (buf) {
        ngx_free(buf);
    }

    प्रत्यागम rc;
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
ngx_walk_वृक्षः(ngx_वृक्षः_ctx_t *ctx, ngx_str_t *वृक्षः)
{
    शून्य       *दत्त, *पूर्व;
    u_char     *p, *name;
    आकार_t      len;
    ngx_int_t   rc;
    ngx_err_t   err;
    ngx_str_t   file, buf;
    ngx_dir_t   dir;

    ngx_str_null(&buf);

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->लघुगणकः, 0,
                   "walk tree \"%V\"", वृक्षः);

    यदि (ngx_open_dir(वृक्षः, &dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->लघुगणकः, ngx_errno,
                      ngx_open_dir_n " \"%s\" failed", वृक्षः->दत्त);
        प्रत्यागम NGX_ERROR;
    }

    पूर्व = ctx->दत्त;

    यदि (ctx->alloc) {
        दत्त = ngx_alloc(ctx->alloc, ctx->लघुगणकः);
        यदि (दत्त == रिक्त) {
            गच्छ failed;
        }

        यदि (ctx->init_handler(दत्त, पूर्व) == NGX_ABORT) {
            गच्छ failed;
        }

        ctx->दत्त = दत्त;

    } अन्यथा {
        दत्त = रिक्त;
    }

    प्रति ( ;; ) {

        ngx_समुच्चयः_errno(0);

        यदि (ngx_read_dir(&dir) == NGX_ERROR) {
            err = ngx_errno;

            यदि (err == NGX_ENOMOREFILES) {
                rc = NGX_OK;

            } अन्यथा {
                ngx_log_error(NGX_LOG_CRIT, ctx->लघुगणकः, err,
                              ngx_read_dir_n " \"%s\" failed", वृक्षः->दत्त);
                rc = NGX_ERROR;
            }

            गच्छ समाप्तम्;
        }

        len = ngx_de_namelen(&dir);
        name = ngx_de_name(&dir);

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, ctx->लघुगणकः, 0,
                      "tree name %uz:\"%s\"", len, name);

        यदि (len == 1 && name[0] == '.') {
            अनुवर्त;
        }

        यदि (len == 2 && name[0] == '.' && name[1] == '.') {
            अनुवर्त;
        }

        file.len = वृक्षः->len + 1 + len;

        यदि (file.len > buf.len) {

            यदि (buf.len) {
                ngx_free(buf.दत्त);
            }

            buf.len = वृक्षः->len + 1 + len;

            buf.दत्त = ngx_alloc(buf.len + 1, ctx->लघुगणकः);
            यदि (buf.दत्त == रिक्त) {
                गच्छ failed;
            }
        }

        p = ngx_cpymem(buf.दत्त, वृक्षः->दत्त, वृक्षः->len);
        *p++ = '/';
        ngx_memcpy(p, name, len + 1);

        file.दत्त = buf.दत्त;

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->लघुगणकः, 0,
                       "tree path \"%s\"", file.दत्त);

        यदि (!dir.valid_info) {
            यदि (ngx_de_info(file.दत्त, &dir) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_CRIT, ctx->लघुगणकः, ngx_errno,
                              ngx_de_info_n " \"%s\" failed", file.दत्त);
                अनुवर्त;
            }
        }

        यदि (ngx_de_is_file(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->लघुगणकः, 0,
                           "tree file \"%s\"", file.दत्त);

            ctx->आकार = ngx_de_आकार(&dir);
            ctx->fs_आकार = ngx_de_fs_आकार(&dir);
            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            यदि (ctx->file_handler(ctx, &file) == NGX_ABORT) {
                गच्छ failed;
            }

        } अन्यथा यदि (ngx_de_is_dir(&dir)) {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->लघुगणकः, 0,
                           "tree enter dir \"%s\"", file.दत्त);

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            rc = ctx->pre_वृक्षः_handler(ctx, &file);

            यदि (rc == NGX_ABORT) {
                गच्छ failed;
            }

            यदि (rc == NGX_DECLINED) {
                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->लघुगणकः, 0,
                               "tree skip dir \"%s\"", file.दत्त);
                अनुवर्त;
            }

            यदि (ngx_walk_वृक्षः(ctx, &file) == NGX_ABORT) {
                गच्छ failed;
            }

            ctx->access = ngx_de_access(&dir);
            ctx->mtime = ngx_de_mtime(&dir);

            यदि (ctx->post_वृक्षः_handler(ctx, &file) == NGX_ABORT) {
                गच्छ failed;
            }

        } अन्यथा {

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, ctx->लघुगणकः, 0,
                           "tree special \"%s\"", file.दत्त);

            यदि (ctx->spec_handler(ctx, &file) == NGX_ABORT) {
                गच्छ failed;
            }
        }
    }

failed:

    rc = NGX_ABORT;

समाप्तम्:

    यदि (buf.len) {
        ngx_free(buf.दत्त);
    }

    यदि (दत्त) {
        ngx_free(दत्त);
        ctx->दत्त = पूर्व;
    }

    यदि (ngx_close_dir(&dir) == NGX_ERROR) {
        ngx_log_error(NGX_LOG_CRIT, ctx->लघुगणकः, ngx_errno,
                      ngx_close_dir_n " \"%s\" failed", वृक्षः->दत्त);
    }

    प्रत्यागम rc;
}
