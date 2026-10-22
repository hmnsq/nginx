
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_event.h>


ਸਥਿਰ ਖਾਲੀ ngx_destroy_ਚੱਕਰ_pools(ngx_conf_t *conf);
ਸਥਿਰ ngx_int_t ngx_init_zone_pool(ngx_ਚੱਕਰ_t *ਚੱਕਰ,
    ngx_shm_zone_t *shm_zone);
ਸਥਿਰ ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *ਲਘੂਗਣਕ);
ਸਥਿਰ ਖਾਲੀ ngx_clean_old_ਚੱਕਰਾਂ(ngx_event_t *ev);
ਸਥਿਰ ਖਾਲੀ ngx_shutdown_timer_handler(ngx_event_t *ev);


ਅਸਥਿਰ ngx_ਚੱਕਰ_t  *ngx_ਚੱਕਰ;
ngx_array_t            ngx_old_ਚੱਕਰਾਂ;

ਸਥਿਰ ngx_pool_t     *ngx_ਆਰਜ਼ੀ_pool;
ਸਥਿਰ ngx_event_t     ngx_cleaner_event;
ਸਥਿਰ ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_ਬਹੁਲਕ;


/* STUB NAME */
ਸਥਿਰ ngx_connection_t  dumb;
/* STUB */


ngx_ਚੱਕਰ_t *
ngx_init_ਚੱਕਰ(ngx_ਚੱਕਰ_t *old_ਚੱਕਰ)
{
    ਖਾਲੀ                *rv, *ਅੰਕੜੇ;
    ਅੱਖਰ               **senv;
    ngx_uint_t           i, n;
    ngx_log_t           *ਲਘੂਗਣਕ;
    ngx_ਸਮਾਂ_t          *tp;
    ngx_conf_t           conf;
    ngx_pool_t          *pool;
    ngx_ਚੱਕਰ_t         *ਚੱਕਰ, **old;
    ngx_shm_zone_t      *shm_zone, *oshm_zone;
    ngx_list_part_t     *part, *opart;
    ngx_open_file_t     *file;
    ngx_listening_t     *ls, *nls;
    ngx_core_conf_t     *ccf, *old_ccf;
    ngx_core_module_t   *module;
    ਅੱਖਰ                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_ਸਮਾਂ_update();


    ਲਘੂਗਣਕ = old_ਚੱਕਰ->ਲਘੂਗਣਕ;

    pool = ngx_create_pool(NGX_ਚੱਕਰ_POOL_ਮਾਪ, ਲਘੂਗਣਕ);
    ਜੇ (pool == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }
    pool->ਲਘੂਗਣਕ = ਲਘੂਗਣਕ;

    ਚੱਕਰ = ngx_pcalloc(pool, ਆਕਾਰ(ngx_ਚੱਕਰ_t));
    ਜੇ (ਚੱਕਰ == ਨਿਰਾ) {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਚੱਕਰ->pool = pool;
    ਚੱਕਰ->ਲਘੂਗਣਕ = ਲਘੂਗਣਕ;
    ਚੱਕਰ->old_ਚੱਕਰ = old_ਚੱਕਰ;

    ਚੱਕਰ->conf_ਅਗੇਤਰ.len = old_ਚੱਕਰ->conf_ਅਗੇਤਰ.len;
    ਚੱਕਰ->conf_ਅਗੇਤਰ.ਅੰਕੜੇ = ngx_pstrdup(pool, &old_ਚੱਕਰ->conf_ਅਗੇਤਰ);
    ਜੇ (ਚੱਕਰ->conf_ਅਗੇਤਰ.ਅੰਕੜੇ == ਨਿਰਾ) {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਚੱਕਰ->ਅਗੇਤਰ.len = old_ਚੱਕਰ->ਅਗੇਤਰ.len;
    ਚੱਕਰ->ਅਗੇਤਰ.ਅੰਕੜੇ = ngx_pstrdup(pool, &old_ਚੱਕਰ->ਅਗੇਤਰ);
    ਜੇ (ਚੱਕਰ->ਅਗੇਤਰ.ਅੰਕੜੇ == ਨਿਰਾ) {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਚੱਕਰ->error_log.len = old_ਚੱਕਰ->error_log.len;
    ਚੱਕਰ->error_log.ਅੰਕੜੇ = ngx_pnalloc(pool, old_ਚੱਕਰ->error_log.len + 1);
    ਜੇ (ਚੱਕਰ->error_log.ਅੰਕੜੇ == ਨਿਰਾ) {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }
    ngx_cpystrn(ਚੱਕਰ->error_log.ਅੰਕੜੇ, old_ਚੱਕਰ->error_log.ਅੰਕੜੇ,
                old_ਚੱਕਰ->error_log.len + 1);

    ਚੱਕਰ->conf_file.len = old_ਚੱਕਰ->conf_file.len;
    ਚੱਕਰ->conf_file.ਅੰਕੜੇ = ngx_pnalloc(pool, old_ਚੱਕਰ->conf_file.len + 1);
    ਜੇ (ਚੱਕਰ->conf_file.ਅੰਕੜੇ == ਨਿਰਾ) {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }
    ngx_cpystrn(ਚੱਕਰ->conf_file.ਅੰਕੜੇ, old_ਚੱਕਰ->conf_file.ਅੰਕੜੇ,
                old_ਚੱਕਰ->conf_file.len + 1);

    ਜੇ (old_ਚੱਕਰ->conf_param.len) {
        ਚੱਕਰ->conf_param.len = old_ਚੱਕਰ->conf_param.len;
        ਚੱਕਰ->conf_param.ਅੰਕੜੇ = ngx_pstrdup(pool, &old_ਚੱਕਰ->conf_param);
        ਜੇ (ਚੱਕਰ->conf_param.ਅੰਕੜੇ == ਨਿਰਾ) {
            ngx_destroy_pool(pool);
            ਵਾਪਸ ਨਿਰਾ;
        }
    }


    n = old_ਚੱਕਰ->paths.nelts ? old_ਚੱਕਰ->paths.nelts : 10;

    ਜੇ (ngx_array_init(&ਚੱਕਰ->paths, pool, n, ਆਕਾਰ(ngx_ਰਸਤਾ_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ngx_memzero(ਚੱਕਰ->paths.elts, n * ਆਕਾਰ(ngx_ਰਸਤਾ_t *));


    ਜੇ (ngx_array_init(&ਚੱਕਰ->config_dump, pool, 1, ਆਕਾਰ(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ngx_rbtree_init(&ਚੱਕਰ->config_dump_rbtree, &ਚੱਕਰ->config_dump_sentinel,
                    ngx_str_rbtree_ਪਾਓ_ਮੁੱਲ);

    ਜੇ (old_ਚੱਕਰ->open_files.part.nelts) {
        n = old_ਚੱਕਰ->open_files.part.nelts;
        ਲਈ (part = old_ਚੱਕਰ->open_files.part.ਅਗਲਾ; part; part = part->ਅਗਲਾ) {
            n += part->nelts;
        }

    } ਨਹੀਂਤਾਂ {
        n = 20;
    }

    ਜੇ (ngx_list_init(&ਚੱਕਰ->open_files, pool, n, ਆਕਾਰ(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }


    ਜੇ (old_ਚੱਕਰ->shared_ਸਮ੍ਰਿਤੀ.part.nelts) {
        n = old_ਚੱਕਰ->shared_ਸਮ੍ਰਿਤੀ.part.nelts;
        ਲਈ (part = old_ਚੱਕਰ->shared_ਸਮ੍ਰਿਤੀ.part.ਅਗਲਾ; part; part = part->ਅਗਲਾ)
        {
            n += part->nelts;
        }

    } ਨਹੀਂਤਾਂ {
        n = 1;
    }

    ਜੇ (ngx_list_init(&ਚੱਕਰ->shared_ਸਮ੍ਰਿਤੀ, pool, n, ਆਕਾਰ(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }

    n = old_ਚੱਕਰ->listening.nelts ? old_ਚੱਕਰ->listening.nelts : 10;

    ਜੇ (ngx_array_init(&ਚੱਕਰ->listening, pool, n, ਆਕਾਰ(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ngx_memzero(ਚੱਕਰ->listening.elts, n * ਆਕਾਰ(ngx_listening_t));


    ngx_ਕਤਾਰ_init(&ਚੱਕਰ->reusable_connections_ਕਤਾਰ);


    ਚੱਕਰ->conf_ctx = ngx_pcalloc(pool, ngx_max_module * ਆਕਾਰ(ਖਾਲੀ *));
    ਜੇ (ਚੱਕਰ->conf_ctx == ਨਿਰਾ) {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }


    ਜੇ (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    ਚੱਕਰ->hostname.len = ngx_strlen(hostname);

    ਚੱਕਰ->hostname.ਅੰਕੜੇ = ngx_pnalloc(pool, ਚੱਕਰ->hostname.len);
    ਜੇ (ਚੱਕਰ->hostname.ਅੰਕੜੇ == ਨਿਰਾ) {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ngx_strlow(ਚੱਕਰ->hostname.ਅੰਕੜੇ, (u_char *) hostname, ਚੱਕਰ->hostname.len);


    ਜੇ (ngx_ਚੱਕਰ_modules(ਚੱਕਰ) != NGX_OK) {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }


    ਲਈ (i = 0; ਚੱਕਰ->modules[i]; i++) {
        ਜੇ (ਚੱਕਰ->modules[i]->type != NGX_CORE_MODULE) {
            ਜਾਰੀ;
        }

        module = ਚੱਕਰ->modules[i]->ctx;

        ਜੇ (module->create_conf) {
            rv = module->create_conf(ਚੱਕਰ);
            ਜੇ (rv == ਨਿਰਾ) {
                ngx_destroy_pool(pool);
                ਵਾਪਸ ਨਿਰਾ;
            }
            ਚੱਕਰ->conf_ctx[ਚੱਕਰ->modules[i]->ਸੂਚਕਾਂਕ] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, ਆਕਾਰ(ngx_conf_t));
    /* STUB: init array ? */
    conf.ਦਲੀਲਾਂ = ngx_array_create(pool, 10, ਆਕਾਰ(ngx_str_t));
    ਜੇ (conf.ਦਲੀਲਾਂ == ਨਿਰਾ) {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }

    conf.ਆਰਜ਼ੀ_pool = ngx_create_pool(NGX_ਚੱਕਰ_POOL_ਮਾਪ, ਲਘੂਗਣਕ);
    ਜੇ (conf.ਆਰਜ਼ੀ_pool == ਨਿਰਾ) {
        ngx_destroy_pool(pool);
        ਵਾਪਸ ਨਿਰਾ;
    }


    conf.ctx = ਚੱਕਰ->conf_ctx;
    conf.ਚੱਕਰ = ਚੱਕਰ;
    conf.pool = pool;
    conf.ਲਘੂਗਣਕ = ਲਘੂਗਣਕ;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#ਜੇ 0
    ਲਘੂਗਣਕ->log_level = NGX_LOG_DEBUG_ALL;
#ਜੇ_ਅੰਤ

    ਜੇ (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_ਚੱਕਰ_pools(&conf);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (ngx_conf_parse(&conf, &ਚੱਕਰ->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_ਚੱਕਰ_pools(&conf);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (ngx_test_config && !ngx_quiet_ਬਹੁਲਕ) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       ਚੱਕਰ->conf_file.ਅੰਕੜੇ);
    }

    ਲਈ (i = 0; ਚੱਕਰ->modules[i]; i++) {
        ਜੇ (ਚੱਕਰ->modules[i]->type != NGX_CORE_MODULE) {
            ਜਾਰੀ;
        }

        module = ਚੱਕਰ->modules[i]->ctx;

        ਜੇ (module->init_conf) {
            ਜੇ (module->init_conf(ਚੱਕਰ,
                                  ਚੱਕਰ->conf_ctx[ਚੱਕਰ->modules[i]->ਸੂਚਕਾਂਕ])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_ਚੱਕਰ_pools(&conf);
                ਵਾਪਸ ਨਿਰਾ;
            }
        }
    }

    ਜੇ (ngx_process == NGX_PROCESS_SIGNALLER) {
        ਵਾਪਸ ਚੱਕਰ;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(ਚੱਕਰ->conf_ctx, ngx_core_module);

    ਜੇ (ngx_test_config) {

        ਜੇ (ngx_create_pidfile(&ccf->pid, ਲਘੂਗਣਕ) != NGX_OK) {
            ਜਾਓ failed;
        }

    } ਨਹੀਂਤਾਂ ਜੇ (!ngx_is_init_ਚੱਕਰ(old_ਚੱਕਰ)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_ਚੱਕਰ->conf_ctx,
                                                   ngx_core_module);
        ਜੇ (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.ਅੰਕੜੇ, old_ccf->pid.ਅੰਕੜੇ) != 0)
        {
            /* new pid file name */

            ਜੇ (ngx_create_pidfile(&ccf->pid, ਲਘੂਗਣਕ) != NGX_OK) {
                ਜਾਓ failed;
            }

            ngx_delete_pidfile(old_ਚੱਕਰ);
        }
    }


    ਜੇ (ngx_test_lockfile(ਚੱਕਰ->ਤਾਲਾ_file.ਅੰਕੜੇ, ਲਘੂਗਣਕ) != NGX_OK) {
        ਜਾਓ failed;
    }


    ਜੇ (ngx_create_paths(ਚੱਕਰ, ccf->user) != NGX_OK) {
        ਜਾਓ failed;
    }


    ਜੇ (ngx_log_open_default(ਚੱਕਰ) != NGX_OK) {
        ਜਾਓ failed;
    }

    /* open the new files */

    part = &ਚੱਕਰ->open_files.part;
    file = part->elts;

    ਲਈ (i = 0; /* void */ ; i++) {

        ਜੇ (i >= part->nelts) {
            ਜੇ (part->ਅਗਲਾ == ਨਿਰਾ) {
                ਰੋਕੋ;
            }
            part = part->ਅਗਲਾ;
            file = part->elts;
            i = 0;
        }

        ਜੇ (file[i].name.len == 0) {
            ਜਾਰੀ;
        }

        file[i].fd = ngx_open_file(file[i].name.ਅੰਕੜੇ,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, ਲਘੂਗਣਕ, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.ਅੰਕੜੇ);

        ਜੇ (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.ਅੰਕੜੇ);
            ਜਾਓ failed;
        }

#ਜੇ !(NGX_WIN32)
        ਜੇ (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.ਅੰਕੜੇ);
            ਜਾਓ failed;
        }
#ਜੇ_ਅੰਤ
    }

    ਚੱਕਰ->ਲਘੂਗਣਕ = &ਚੱਕਰ->new_log;
    pool->ਲਘੂਗਣਕ = &ਚੱਕਰ->new_log;


    /* create shared memory */

    part = &ਚੱਕਰ->shared_ਸਮ੍ਰਿਤੀ.part;
    shm_zone = part->elts;

    ਲਈ (i = 0; /* void */ ; i++) {

        ਜੇ (i >= part->nelts) {
            ਜੇ (part->ਅਗਲਾ == ਨਿਰਾ) {
                ਰੋਕੋ;
            }
            part = part->ਅਗਲਾ;
            shm_zone = part->elts;
            i = 0;
        }

        ਜੇ (shm_zone[i].shm.ਮਾਪ == 0) {
            ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            ਜਾਓ failed;
        }

        shm_zone[i].shm.ਲਘੂਗਣਕ = ਚੱਕਰ->ਲਘੂਗਣਕ;

        opart = &old_ਚੱਕਰ->shared_ਸਮ੍ਰਿਤੀ.part;
        oshm_zone = opart->elts;

        ਅੰਕੜੇ = ਨਿਰਾ;

        ਲਈ (n = 0; /* void */ ; n++) {

            ਜੇ (n >= opart->nelts) {
                ਜੇ (opart->ਅਗਲਾ == ਨਿਰਾ) {
                    ਰੋਕੋ;
                }
                opart = opart->ਅਗਲਾ;
                oshm_zone = opart->elts;
                n = 0;
            }

            ਜੇ (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                ਜਾਰੀ;
            }

            ਜੇ (ngx_strncmp(shm_zone[i].shm.name.ਅੰਕੜੇ,
                            oshm_zone[n].shm.name.ਅੰਕੜੇ,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                ਜਾਰੀ;
            }

            ਜੇ (shm_zone[i].ਟੈਗ == oshm_zone[n].ਟੈਗ && shm_zone[i].noreuse) {
                ਅੰਕੜੇ = oshm_zone[n].ਅੰਕੜੇ;
                ਰੋਕੋ;
            }

            ਜੇ (shm_zone[i].ਟੈਗ == oshm_zone[n].ਟੈਗ
                && shm_zone[i].shm.ਮਾਪ == oshm_zone[n].shm.ਮਾਪ)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#ਜੇ (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#ਜੇ_ਅੰਤ

                ਜੇ (shm_zone[i].init(&shm_zone[i], oshm_zone[n].ਅੰਕੜੇ)
                    != NGX_OK)
                {
                    ਜਾਓ failed;
                }

                ਜਾਓ shm_zone_ਮਿਲਿਆ;
            }

            ਰੋਕੋ;
        }

        ਜੇ (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            ਜਾਓ failed;
        }

        ਜੇ (ngx_init_zone_pool(ਚੱਕਰ, &shm_zone[i]) != NGX_OK) {
            ਜਾਓ failed;
        }

        ਜੇ (shm_zone[i].init(&shm_zone[i], ਅੰਕੜੇ) != NGX_OK) {
            ਜਾਓ failed;
        }

    shm_zone_ਮਿਲਿਆ:

        ਜਾਰੀ;
    }


    /* handle the listening sockets */

    ਜੇ (old_ਚੱਕਰ->listening.nelts) {
        ls = old_ਚੱਕਰ->listening.elts;
        ਲਈ (i = 0; i < old_ਚੱਕਰ->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = ਚੱਕਰ->listening.elts;
        ਲਈ (n = 0; n < ਚੱਕਰ->listening.nelts; n++) {

            ਲਈ (i = 0; i < old_ਚੱਕਰ->listening.nelts; i++) {
                ਜੇ (ls[i].ignore) {
                    ਜਾਰੀ;
                }

                ਜੇ (ls[i].remain) {
                    ਜਾਰੀ;
                }

                ਜੇ (ls[i].type != nls[n].type) {
                    ਜਾਰੀ;
                }

                ਜੇ (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].ਪਿਛਲਾ = &ls[i];

                    ਜੇ (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } ਨਹੀਂਤਾਂ {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    ਜੇ (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    ਜੇ (ls[i].accept_filter && nls[n].accept_filter) {
                        ਜੇ (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].ਜੋੜੋ_deferred = 1;
                        }

                    } ਨਹੀਂਤਾਂ ਜੇ (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } ਨਹੀਂਤਾਂ ਜੇ (nls[n].accept_filter) {
                        nls[n].ਜੋੜੋ_deferred = 1;
                    }
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    ਜੇ (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } ਨਹੀਂਤਾਂ ਜੇ (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].ਜੋੜੋ_deferred = 1;
                    }
#ਜੇ_ਅੰਤ

#ਜੇ (NGX_HAVE_REUSEPORT)
                    ਜੇ (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].ਜੋੜੋ_reuseport = 1;
                    }
#ਜੇ_ਅੰਤ

                    ਰੋਕੋ;
                }
            }

            ਜੇ (nls[n].fd == (ngx_socket_t) -1) {
#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                ਜੇ (nls[n].accept_filter) {
                    nls[n].ਜੋੜੋ_deferred = 1;
                }
#ਜੇ_ਅੰਤ
#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                ਜੇ (nls[n].deferred_accept) {
                    nls[n].ਜੋੜੋ_deferred = 1;
                }
#ਜੇ_ਅੰਤ
            }
        }

    } ਨਹੀਂਤਾਂ {
#ਜੇ (NGX_HAVE_DEFERRED_ACCEPT)
        ls = ਚੱਕਰ->listening.elts;
        ਲਈ (i = 0; i < ਚੱਕਰ->listening.nelts; i++) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SO_ACCEPTFILTER
            ਜੇ (ls[i].accept_filter) {
                ls[i].ਜੋੜੋ_deferred = 1;
            }
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TCP_DEFER_ACCEPT
            ਜੇ (ls[i].deferred_accept) {
                ls[i].ਜੋੜੋ_deferred = 1;
            }
#ਜੇ_ਅੰਤ
        }
#ਜੇ_ਅੰਤ
    }

    ਜੇ (ngx_open_listening_sockets(ਚੱਕਰ) != NGX_OK) {
        ਜਾਓ failed;
    }

    ਜੇ (!ngx_test_config) {
        ngx_configure_listening_sockets(ਚੱਕਰ);
    }


    /* commit the new cycle configuration */

    ਜੇ (!ngx_use_stderr) {
        (ਖਾਲੀ) ngx_log_redirect_stderr(ਚੱਕਰ);
    }

    pool->ਲਘੂਗਣਕ = ਚੱਕਰ->ਲਘੂਗਣਕ;

    ਜੇ (ngx_init_modules(ਚੱਕਰ) != NGX_OK) {
        /* fatal */
        ਬਾਹਰ_ਨਿਕਲੋ(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_ਚੱਕਰ->shared_ਸਮ੍ਰਿਤੀ.part;
    oshm_zone = opart->elts;

    ਲਈ (i = 0; /* void */ ; i++) {

        ਜੇ (i >= opart->nelts) {
            ਜੇ (opart->ਅਗਲਾ == ਨਿਰਾ) {
                ਜਾਓ old_shm_zone_ਪੂਰਾ;
            }
            opart = opart->ਅਗਲਾ;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &ਚੱਕਰ->shared_ਸਮ੍ਰਿਤੀ.part;
        shm_zone = part->elts;

        ਲਈ (n = 0; /* void */ ; n++) {

            ਜੇ (n >= part->nelts) {
                ਜੇ (part->ਅਗਲਾ == ਨਿਰਾ) {
                    ਰੋਕੋ;
                }
                part = part->ਅਗਲਾ;
                shm_zone = part->elts;
                n = 0;
            }

            ਜੇ (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                ਜਾਰੀ;
            }

            ਜੇ (ngx_strncmp(oshm_zone[i].shm.name.ਅੰਕੜੇ,
                            shm_zone[n].shm.name.ਅੰਕੜੇ,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                ਜਾਰੀ;
            }

            ਜੇ (oshm_zone[i].ਟੈਗ == shm_zone[n].ਟੈਗ
                && oshm_zone[i].shm.ਮਾਪ == shm_zone[n].shm.ਮਾਪ
                && !oshm_zone[i].noreuse)
            {
                ਜਾਓ ਜੀਵਤ_shm_zone;
            }

            ਰੋਕੋ;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    ਜੀਵਤ_shm_zone:

        ਜਾਰੀ;
    }

old_shm_zone_ਪੂਰਾ:


    /* close the unnecessary listening sockets */

    ls = old_ਚੱਕਰ->listening.elts;
    ਲਈ (i = 0; i < old_ਚੱਕਰ->listening.nelts; i++) {

        ਜੇ (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            ਜਾਰੀ;
        }

        ਜੇ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_ਪਾਠ);
        }

#ਜੇ (NGX_HAVE_UNIX_DOMAIN)

        ਜੇ (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_ਪਾਠ.ਅੰਕੜੇ + ਆਕਾਰ("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                          "deleting socket %s", name);

            ਜੇ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#ਜੇ_ਅੰਤ
    }


    /* close the unnecessary open files */

    part = &old_ਚੱਕਰ->open_files.part;
    file = part->elts;

    ਲਈ (i = 0; /* void */ ; i++) {

        ਜੇ (i >= part->nelts) {
            ਜੇ (part->ਅਗਲਾ == ਨਿਰਾ) {
                ਰੋਕੋ;
            }
            part = part->ਅਗਲਾ;
            file = part->elts;
            i = 0;
        }

        ਜੇ (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            ਜਾਰੀ;
        }

        ਜੇ (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.ਅੰਕੜੇ);
        }
    }

    ngx_destroy_pool(conf.ਆਰਜ਼ੀ_pool);

    ਜੇ (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_ਚੱਕਰ(old_ਚੱਕਰ)) {

        ngx_destroy_pool(old_ਚੱਕਰ->pool);
        ਚੱਕਰ->old_ਚੱਕਰ = ਨਿਰਾ;

        ਵਾਪਸ ਚੱਕਰ;
    }


    ਜੇ (ngx_ਆਰਜ਼ੀ_pool == ਨਿਰਾ) {
        ngx_ਆਰਜ਼ੀ_pool = ngx_create_pool(128, ਚੱਕਰ->ਲਘੂਗਣਕ);
        ਜੇ (ngx_ਆਰਜ਼ੀ_pool == ਨਿਰਾ) {
            ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                          "could not create ngx_temp_pool");
            ਬਾਹਰ_ਨਿਕਲੋ(1);
        }

        n = 10;

        ਜੇ (ngx_array_init(&ngx_old_ਚੱਕਰਾਂ, ngx_ਆਰਜ਼ੀ_pool, n,
                           ਆਕਾਰ(ngx_ਚੱਕਰ_t *))
            != NGX_OK)
        {
            ਬਾਹਰ_ਨਿਕਲੋ(1);
        }

        ngx_memzero(ngx_old_ਚੱਕਰਾਂ.elts, n * ਆਕਾਰ(ngx_ਚੱਕਰ_t *));

        ngx_cleaner_event.handler = ngx_clean_old_ਚੱਕਰਾਂ;
        ngx_cleaner_event.ਲਘੂਗਣਕ = ਚੱਕਰ->ਲਘੂਗਣਕ;
        ngx_cleaner_event.ਅੰਕੜੇ = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_ਆਰਜ਼ੀ_pool->ਲਘੂਗਣਕ = ਚੱਕਰ->ਲਘੂਗਣਕ;

    old = ngx_array_ਧੱਕੋ(&ngx_old_ਚੱਕਰਾਂ);
    ਜੇ (old == ਨਿਰਾ) {
        ਬਾਹਰ_ਨਿਕਲੋ(1);
    }
    *old = old_ਚੱਕਰ;

    ਜੇ (!ngx_cleaner_event.timer_ਸੈੱਟ) {
        ngx_ਜੋੜੋ_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_ਸੈੱਟ = 1;
    }

    ਵਾਪਸ ਚੱਕਰ;


failed:

    ਜੇ (!ngx_is_init_ਚੱਕਰ(old_ਚੱਕਰ)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_ਚੱਕਰ->conf_ctx,
                                                   ngx_core_module);
        ਜੇ (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &ਚੱਕਰ->open_files.part;
    file = part->elts;

    ਲਈ (i = 0; /* void */ ; i++) {

        ਜੇ (i >= part->nelts) {
            ਜੇ (part->ਅਗਲਾ == ਨਿਰਾ) {
                ਰੋਕੋ;
            }
            part = part->ਅਗਲਾ;
            file = part->elts;
            i = 0;
        }

        ਜੇ (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            ਜਾਰੀ;
        }

        ਜੇ (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.ਅੰਕੜੇ);
        }
    }

    /* free the newly created shared memory */

    part = &ਚੱਕਰ->shared_ਸਮ੍ਰਿਤੀ.part;
    shm_zone = part->elts;

    ਲਈ (i = 0; /* void */ ; i++) {

        ਜੇ (i >= part->nelts) {
            ਜੇ (part->ਅਗਲਾ == ਨਿਰਾ) {
                ਰੋਕੋ;
            }
            part = part->ਅਗਲਾ;
            shm_zone = part->elts;
            i = 0;
        }

        ਜੇ (shm_zone[i].shm.addr == ਨਿਰਾ) {
            ਜਾਰੀ;
        }

        opart = &old_ਚੱਕਰ->shared_ਸਮ੍ਰਿਤੀ.part;
        oshm_zone = opart->elts;

        ਲਈ (n = 0; /* void */ ; n++) {

            ਜੇ (n >= opart->nelts) {
                ਜੇ (opart->ਅਗਲਾ == ਨਿਰਾ) {
                    ਰੋਕੋ;
                }
                opart = opart->ਅਗਲਾ;
                oshm_zone = opart->elts;
                n = 0;
            }

            ਜੇ (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                ਜਾਰੀ;
            }

            ਜੇ (ngx_strncmp(shm_zone[i].shm.name.ਅੰਕੜੇ,
                            oshm_zone[n].shm.name.ਅੰਕੜੇ,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                ਜਾਰੀ;
            }

            ਜੇ (shm_zone[i].ਟੈਗ == oshm_zone[n].ਟੈਗ
                && shm_zone[i].shm.ਮਾਪ == oshm_zone[n].shm.ਮਾਪ
                && !shm_zone[i].noreuse)
            {
                ਜਾਓ old_shm_zone_ਮਿਲਿਆ;
            }

            ਰੋਕੋ;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_ਮਿਲਿਆ:

        ਜਾਰੀ;
    }

    ਜੇ (ngx_test_config) {
        ngx_destroy_ਚੱਕਰ_pools(&conf);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ls = ਚੱਕਰ->listening.elts;
    ਲਈ (i = 0; i < ਚੱਕਰ->listening.nelts; i++) {
        ਜੇ (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            ਜਾਰੀ;
        }

        ਜੇ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_ਪਾਠ);
        }
    }

    ngx_destroy_ਚੱਕਰ_pools(&conf);

    ਵਾਪਸ ਨਿਰਾ;
}


ਸਥਿਰ ਖਾਲੀ
ngx_destroy_ਚੱਕਰ_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->ਆਰਜ਼ੀ_pool);
    ngx_destroy_pool(conf->pool);
}


ਸਥਿਰ ngx_int_t
ngx_init_zone_pool(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    ਜੇ (zn->shm.exists) {

        ਜੇ (sp == sp->addr) {
            ਵਾਪਸ NGX_OK;
        }

#ਜੇ (NGX_WIN32)

        /* remap at the required address */

        ਜੇ (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            ਵਾਪਸ NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        ਜੇ (sp == sp->addr) {
            ਵਾਪਸ NGX_OK;
        }

#ਜੇ_ਅੰਤ

        ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        ਵਾਪਸ NGX_ERROR;
    }

    sp->ਸਿਰਾ = zn->shm.addr + zn->shm.ਮਾਪ;
    sp->min_ਖਿਸਕਾਓ = 3;
    sp->addr = zn->shm.addr;

#ਜੇ (NGX_HAVE_ATOMIC_OPS)

    file = ਨਿਰਾ;

#ਨਹੀਂਤਾਂ

    file = ngx_pnalloc(ਚੱਕਰ->pool,
                       ਚੱਕਰ->ਤਾਲਾ_file.len + zn->shm.name.len + 1);
    ਜੇ (file == ਨਿਰਾ) {
        ਵਾਪਸ NGX_ERROR;
    }

    (ਖਾਲੀ) ngx_sprintf(file, "%V%V%Z", &ਚੱਕਰ->ਤਾਲਾ_file, &zn->shm.name);

#ਜੇ_ਅੰਤ

    ਜੇ (ngx_shmtx_create(&sp->mutex, &sp->ਤਾਲਾ, file) != NGX_OK) {
        ਵਾਪਸ NGX_ERROR;
    }

    ngx_slab_init(sp);

    ਵਾਪਸ NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *ਲਘੂਗਣਕ)
{
    ਮਾਪ_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    ਜੇ (ngx_process > NGX_PROCESS_MASTER) {
        ਵਾਪਸ NGX_OK;
    }

    ngx_memzero(&file, ਆਕਾਰ(ngx_file_t));

    file.name = *name;
    file.ਲਘੂਗਣਕ = ਲਘੂਗਣਕ;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.ਅੰਕੜੇ, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    ਜੇ (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.ਅੰਕੜੇ);
        ਵਾਪਸ NGX_ERROR;
    }

    rc = NGX_OK;

    ਜੇ (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        ਜੇ (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    ਜੇ (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.ਅੰਕੜੇ);
    }

    ਵਾਪਸ rc;
}


ਖਾਲੀ
ngx_delete_pidfile(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ਚੱਕਰ->conf_ctx, ngx_core_module);

    name = ngx_new_ਦੁਆਧਾਰੀ ? ccf->oldpid.ਅੰਕੜੇ : ccf->pid.ਅੰਕੜੇ;

    ਜੇ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ਅੱਖਰ *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, ਚੱਕਰ->ਲਘੂਗਣਕ, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(ਚੱਕਰ->conf_ctx, ngx_core_module);

    ngx_memzero(&file, ਆਕਾਰ(ngx_file_t));

    file.name = ccf->pid;
    file.ਲਘੂਗਣਕ = ਚੱਕਰ->ਲਘੂਗਣਕ;

    file.fd = ngx_open_file(file.name.ਅੰਕੜੇ, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    ਜੇ (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.ਅੰਕੜੇ);
        ਵਾਪਸ 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    ਜੇ (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.ਅੰਕੜੇ);
    }

    ਜੇ (n == NGX_ERROR) {
        ਵਾਪਸ 1;
    }

    ਜਦੋਂ (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    ਜੇ (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.ਅੰਕੜੇ);
        ਵਾਪਸ 1;
    }

    ਵਾਪਸ ngx_os_signal_process(ਚੱਕਰ, sig, pid);

}


ਸਥਿਰ ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *ਲਘੂਗਣਕ)
{
#ਜੇ !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    ਜੇ (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ਲਘੂਗਣਕ, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        ਵਾਪਸ NGX_ERROR;
    }

    ਜੇ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    ਜੇ (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ਲਘੂਗਣਕ, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#ਜੇ_ਅੰਤ

    ਵਾਪਸ NGX_OK;
}


ਖਾਲੀ
ngx_reopen_files(ngx_ਚੱਕਰ_t *ਚੱਕਰ, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &ਚੱਕਰ->open_files.part;
    file = part->elts;

    ਲਈ (i = 0; /* void */ ; i++) {

        ਜੇ (i >= part->nelts) {
            ਜੇ (part->ਅਗਲਾ == ਨਿਰਾ) {
                ਰੋਕੋ;
            }
            part = part->ਅਗਲਾ;
            file = part->elts;
            i = 0;
        }

        ਜੇ (file[i].name.len == 0) {
            ਜਾਰੀ;
        }

        ਜੇ (file[i].flush) {
            file[i].flush(&file[i], ਚੱਕਰ->ਲਘੂਗਣਕ);
        }

        fd = ngx_open_file(file[i].name.ਅੰਕੜੇ, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, ਚੱਕਰ->ਲਘੂਗਣਕ, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.ਅੰਕੜੇ, file[i].fd, fd);

        ਜੇ (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.ਅੰਕੜੇ);
            ਜਾਰੀ;
        }

#ਜੇ !(NGX_WIN32)
        ਜੇ (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            ਜੇ (ngx_file_info(file[i].name.ਅੰਕੜੇ, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.ਅੰਕੜੇ);

                ਜੇ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.ਅੰਕੜੇ);
                }

                ਜਾਰੀ;
            }

            ਜੇ (fi.st_uid != user) {
                ਜੇ (chown((ਅਚਲ ਅੱਖਰ *) file[i].name.ਅੰਕੜੇ, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.ਅੰਕੜੇ, user);

                    ਜੇ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.ਅੰਕੜੇ);
                    }

                    ਜਾਰੀ;
                }
            }

            ਜੇ ((fi.st_ਬਹੁਲਕ & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_ਬਹੁਲਕ |= (S_IRUSR|S_IWUSR);

                ਜੇ (chmod((ਅਚਲ ਅੱਖਰ *) file[i].name.ਅੰਕੜੇ, fi.st_ਬਹੁਲਕ) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.ਅੰਕੜੇ);

                    ਜੇ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.ਅੰਕੜੇ);
                    }

                    ਜਾਰੀ;
                }
            }
        }

        ਜੇ (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.ਅੰਕੜੇ);

            ਜੇ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.ਅੰਕੜੇ);
            }

            ਜਾਰੀ;
        }
#ਜੇ_ਅੰਤ

        ਜੇ (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, ਚੱਕਰ->ਲਘੂਗਣਕ, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.ਅੰਕੜੇ);
        }

        file[i].fd = fd;
    }

    (ਖਾਲੀ) ngx_log_redirect_stderr(ਚੱਕਰ);
}


ngx_shm_zone_t *
ngx_shared_ਸਮ੍ਰਿਤੀ_ਜੋੜੋ(ngx_conf_t *cf, ngx_str_t *name, ਮਾਪ_t ਮਾਪ, ਖਾਲੀ *ਟੈਗ)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->ਚੱਕਰ->shared_ਸਮ੍ਰਿਤੀ.part;
    shm_zone = part->elts;

    ਲਈ (i = 0; /* void */ ; i++) {

        ਜੇ (i >= part->nelts) {
            ਜੇ (part->ਅਗਲਾ == ਨਿਰਾ) {
                ਰੋਕੋ;
            }
            part = part->ਅਗਲਾ;
            shm_zone = part->elts;
            i = 0;
        }

        ਜੇ (name->len != shm_zone[i].shm.name.len) {
            ਜਾਰੀ;
        }

        ਜੇ (ngx_strncmp(name->ਅੰਕੜੇ, shm_zone[i].shm.name.ਅੰਕੜੇ, name->len)
            != 0)
        {
            ਜਾਰੀ;
        }

        ਜੇ (ਟੈਗ != shm_zone[i].ਟੈਗ) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            ਵਾਪਸ ਨਿਰਾ;
        }

        ਜੇ (shm_zone[i].shm.ਮਾਪ == 0) {
            shm_zone[i].shm.ਮਾਪ = ਮਾਪ;
        }

        ਜੇ (ਮਾਪ && ਮਾਪ != shm_zone[i].shm.ਮਾਪ) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            ਮਾਪ, &shm_zone[i].shm.name, shm_zone[i].shm.ਮਾਪ);
            ਵਾਪਸ ਨਿਰਾ;
        }

        ਵਾਪਸ &shm_zone[i];
    }

    shm_zone = ngx_list_ਧੱਕੋ(&cf->ਚੱਕਰ->shared_ਸਮ੍ਰਿਤੀ);

    ਜੇ (shm_zone == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    shm_zone->ਅੰਕੜੇ = ਨਿਰਾ;
    shm_zone->shm.ਲਘੂਗਣਕ = cf->ਚੱਕਰ->ਲਘੂਗਣਕ;
    shm_zone->shm.addr = ਨਿਰਾ;
    shm_zone->shm.ਮਾਪ = ਮਾਪ;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = ਨਿਰਾ;
    shm_zone->ਟੈਗ = ਟੈਗ;
    shm_zone->noreuse = 0;

    ਵਾਪਸ shm_zone;
}


ਸਥਿਰ ਖਾਲੀ
ngx_clean_old_ਚੱਕਰਾਂ(ngx_event_t *ev)
{
    ngx_uint_t     i, n, ਮਿਲਿਆ, ਜੀਵਤ;
    ngx_log_t     *ਲਘੂਗਣਕ;
    ngx_ਚੱਕਰ_t  **ਚੱਕਰ;

    ਲਘੂਗਣਕ = ngx_ਚੱਕਰ->ਲਘੂਗਣਕ;
    ngx_ਆਰਜ਼ੀ_pool->ਲਘੂਗਣਕ = ਲਘੂਗਣਕ;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ਲਘੂਗਣਕ, 0, "clean old cycles");

    ਜੀਵਤ = 0;

    ਚੱਕਰ = ngx_old_ਚੱਕਰਾਂ.elts;
    ਲਈ (i = 0; i < ngx_old_ਚੱਕਰਾਂ.nelts; i++) {

        ਜੇ (ਚੱਕਰ[i] == ਨਿਰਾ) {
            ਜਾਰੀ;
        }

        ਮਿਲਿਆ = 0;

        ਲਈ (n = 0; n < ਚੱਕਰ[i]->connection_n; n++) {
            ਜੇ (ਚੱਕਰ[i]->connections[n].fd != (ngx_socket_t) -1) {
                ਮਿਲਿਆ = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ਲਘੂਗਣਕ, 0, "live fd:%ui", n);

                ਰੋਕੋ;
            }
        }

        ਜੇ (ਮਿਲਿਆ) {
            ਜੀਵਤ = 1;
            ਜਾਰੀ;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ਲਘੂਗਣਕ, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(ਚੱਕਰ[i]->pool);
        ਚੱਕਰ[i] = ਨਿਰਾ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ਲਘੂਗਣਕ, 0, "old cycles status: %ui", ਜੀਵਤ);

    ਜੇ (ਜੀਵਤ) {
        ngx_ਜੋੜੋ_timer(ev, 30000);

    } ਨਹੀਂਤਾਂ {
        ngx_destroy_pool(ngx_ਆਰਜ਼ੀ_pool);
        ngx_ਆਰਜ਼ੀ_pool = ਨਿਰਾ;
        ngx_old_ਚੱਕਰਾਂ.nelts = 0;
    }
}


ਖਾਲੀ
ngx_ਸੈੱਟ_shutdown_timer(ngx_ਚੱਕਰ_t *ਚੱਕਰ)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ਚੱਕਰ->conf_ctx, ngx_core_module);

    ਜੇ (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.ਅੰਕੜੇ = ਚੱਕਰ;
        ngx_shutdown_event.ਲਘੂਗਣਕ = ਚੱਕਰ->ਲਘੂਗਣਕ;
        ngx_shutdown_event.cancelable = 1;

        ngx_ਜੋੜੋ_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


ਸਥਿਰ ਖਾਲੀ
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_ਚੱਕਰ_t       *ਚੱਕਰ;
    ngx_connection_t  *c;

    ਚੱਕਰ = ev->ਅੰਕੜੇ;

    c = ਚੱਕਰ->connections;

    ਲਈ (i = 0; i < ਚੱਕਰ->connection_n; i++) {

        ਜੇ (c[i].fd == (ngx_socket_t) -1
            || c[i].ਪਠ == ਨਿਰਾ
            || c[i].ਪਠ->accept
            || c[i].ਪਠ->channel
            || c[i].ਪਠ->resolver)
        {
            ਜਾਰੀ;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->ਲਘੂਗਣਕ, 0,
                       "*%uA shutdown timeout", c[i].ਸੰਖਿਆ);

        c[i].close = 1;
        c[i].error = 1;

        c[i].ਪਠ->handler(c[i].ਪਠ);
    }
}
