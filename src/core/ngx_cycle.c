
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_event.h>


ಸ್ಥಿರ ಶೂನ್ಯ ngx_destroy_ಚಕ್ರ_pools(ngx_conf_t *conf);
ಸ್ಥಿರ ngx_int_t ngx_init_zone_pool(ngx_ಚಕ್ರ_t *ಚಕ್ರ,
    ngx_shm_zone_t *shm_zone);
ಸ್ಥಿರ ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *ಲಘುಗಣಕ);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_clean_old_ಚಕ್ರಗಳು(ngx_event_t *ev);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_shutdown_timer_handler(ngx_event_t *ev);


ಅಸ್ಥಿರ ngx_ಚಕ್ರ_t  *ngx_ಚಕ್ರ;
ngx_array_t            ngx_old_ಚಕ್ರಗಳು;

ಸ್ಥಿರ ngx_pool_t     *ngx_ತಾತ್ಕಾಲಿಕ_pool;
ಸ್ಥಿರ ngx_event_t     ngx_cleaner_event;
ಸ್ಥಿರ ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_ಬಹುಲಕ;


/* STUB NAME */
ಸ್ಥಿರ ngx_connection_t  dumb;
/* STUB */


ngx_ಚಕ್ರ_t *
ngx_init_ಚಕ್ರ(ngx_ಚಕ್ರ_t *old_ಚಕ್ರ)
{
    ಶೂನ್ಯ                *rv, *ದತ್ತಾಂಶ;
    ಅಕ್ಷರ               **senv;
    ngx_uint_t           i, n;
    ngx_log_t           *ಲಘುಗಣಕ;
    ngx_ಸಮಯ_t          *tp;
    ngx_conf_t           conf;
    ngx_pool_t          *pool;
    ngx_ಚಕ್ರ_t         *ಚಕ್ರ, **old;
    ngx_shm_zone_t      *shm_zone, *oshm_zone;
    ngx_list_part_t     *part, *opart;
    ngx_open_file_t     *file;
    ngx_listening_t     *ls, *nls;
    ngx_core_conf_t     *ccf, *old_ccf;
    ngx_core_module_t   *module;
    ಅಕ್ಷರ                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_ಸಮಯ_update();


    ಲಘುಗಣಕ = old_ಚಕ್ರ->ಲಘುಗಣಕ;

    pool = ngx_create_pool(NGX_ಚಕ್ರ_POOL_ಗಾತ್ರ, ಲಘುಗಣಕ);
    ಆದರೆ (pool == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
    pool->ಲಘುಗಣಕ = ಲಘುಗಣಕ;

    ಚಕ್ರ = ngx_pcalloc(pool, ಗಾತ್ರ(ngx_ಚಕ್ರ_t));
    ಆದರೆ (ಚಕ್ರ == ಖಾಲಿ) {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಚಕ್ರ->pool = pool;
    ಚಕ್ರ->ಲಘುಗಣಕ = ಲಘುಗಣಕ;
    ಚಕ್ರ->old_ಚಕ್ರ = old_ಚಕ್ರ;

    ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ.len = old_ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ.len;
    ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ.ದತ್ತಾಂಶ = ngx_pstrdup(pool, &old_ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ);
    ಆದರೆ (ಚಕ್ರ->conf_ಪೂರ್ವಪ್ರತ್ಯಯ.ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಚಕ್ರ->ಪೂರ್ವಪ್ರತ್ಯಯ.len = old_ಚಕ್ರ->ಪೂರ್ವಪ್ರತ್ಯಯ.len;
    ಚಕ್ರ->ಪೂರ್ವಪ್ರತ್ಯಯ.ದತ್ತಾಂಶ = ngx_pstrdup(pool, &old_ಚಕ್ರ->ಪೂರ್ವಪ್ರತ್ಯಯ);
    ಆದರೆ (ಚಕ್ರ->ಪೂರ್ವಪ್ರತ್ಯಯ.ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಚಕ್ರ->error_log.len = old_ಚಕ್ರ->error_log.len;
    ಚಕ್ರ->error_log.ದತ್ತಾಂಶ = ngx_pnalloc(pool, old_ಚಕ್ರ->error_log.len + 1);
    ಆದರೆ (ಚಕ್ರ->error_log.ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
    ngx_cpystrn(ಚಕ್ರ->error_log.ದತ್ತಾಂಶ, old_ಚಕ್ರ->error_log.ದತ್ತಾಂಶ,
                old_ಚಕ್ರ->error_log.len + 1);

    ಚಕ್ರ->conf_file.len = old_ಚಕ್ರ->conf_file.len;
    ಚಕ್ರ->conf_file.ದತ್ತಾಂಶ = ngx_pnalloc(pool, old_ಚಕ್ರ->conf_file.len + 1);
    ಆದರೆ (ಚಕ್ರ->conf_file.ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
    ngx_cpystrn(ಚಕ್ರ->conf_file.ದತ್ತಾಂಶ, old_ಚಕ್ರ->conf_file.ದತ್ತಾಂಶ,
                old_ಚಕ್ರ->conf_file.len + 1);

    ಆದರೆ (old_ಚಕ್ರ->conf_param.len) {
        ಚಕ್ರ->conf_param.len = old_ಚಕ್ರ->conf_param.len;
        ಚಕ್ರ->conf_param.ದತ್ತಾಂಶ = ngx_pstrdup(pool, &old_ಚಕ್ರ->conf_param);
        ಆದರೆ (ಚಕ್ರ->conf_param.ದತ್ತಾಂಶ == ಖಾಲಿ) {
            ngx_destroy_pool(pool);
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
    }


    n = old_ಚಕ್ರ->paths.nelts ? old_ಚಕ್ರ->paths.nelts : 10;

    ಆದರೆ (ngx_array_init(&ಚಕ್ರ->paths, pool, n, ಗಾತ್ರ(ngx_ಮಾರ್ಗ_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ngx_memzero(ಚಕ್ರ->paths.elts, n * ಗಾತ್ರ(ngx_ಮಾರ್ಗ_t *));


    ಆದರೆ (ngx_array_init(&ಚಕ್ರ->config_dump, pool, 1, ಗಾತ್ರ(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ngx_rbtree_init(&ಚಕ್ರ->config_dump_rbtree, &ಚಕ್ರ->config_dump_sentinel,
                    ngx_str_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ);

    ಆದರೆ (old_ಚಕ್ರ->open_files.part.nelts) {
        n = old_ಚಕ್ರ->open_files.part.nelts;
        ಗಾಗಿ (part = old_ಚಕ್ರ->open_files.part.ಮುಂದಿನ; part; part = part->ಮುಂದಿನ) {
            n += part->nelts;
        }

    } ಇಲ್ಲದಿದ್ದರೆ {
        n = 20;
    }

    ಆದರೆ (ngx_list_init(&ಚಕ್ರ->open_files, pool, n, ಗಾತ್ರ(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }


    ಆದರೆ (old_ಚಕ್ರ->shared_ಸ್ಮರಣೆ.part.nelts) {
        n = old_ಚಕ್ರ->shared_ಸ್ಮರಣೆ.part.nelts;
        ಗಾಗಿ (part = old_ಚಕ್ರ->shared_ಸ್ಮರಣೆ.part.ಮುಂದಿನ; part; part = part->ಮುಂದಿನ)
        {
            n += part->nelts;
        }

    } ಇಲ್ಲದಿದ್ದರೆ {
        n = 1;
    }

    ಆದರೆ (ngx_list_init(&ಚಕ್ರ->shared_ಸ್ಮರಣೆ, pool, n, ಗಾತ್ರ(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    n = old_ಚಕ್ರ->listening.nelts ? old_ಚಕ್ರ->listening.nelts : 10;

    ಆದರೆ (ngx_array_init(&ಚಕ್ರ->listening, pool, n, ಗಾತ್ರ(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ngx_memzero(ಚಕ್ರ->listening.elts, n * ಗಾತ್ರ(ngx_listening_t));


    ngx_ಸರತಿ_init(&ಚಕ್ರ->reusable_connections_ಸರತಿ);


    ಚಕ್ರ->conf_ctx = ngx_pcalloc(pool, ngx_max_module * ಗಾತ್ರ(ಶೂನ್ಯ *));
    ಆದರೆ (ಚಕ್ರ->conf_ctx == ಖಾಲಿ) {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }


    ಆದರೆ (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    ಚಕ್ರ->hostname.len = ngx_strlen(hostname);

    ಚಕ್ರ->hostname.ದತ್ತಾಂಶ = ngx_pnalloc(pool, ಚಕ್ರ->hostname.len);
    ಆದರೆ (ಚಕ್ರ->hostname.ದತ್ತಾಂಶ == ಖಾಲಿ) {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ngx_strlow(ಚಕ್ರ->hostname.ದತ್ತಾಂಶ, (u_char *) hostname, ಚಕ್ರ->hostname.len);


    ಆದರೆ (ngx_ಚಕ್ರ_modules(ಚಕ್ರ) != NGX_OK) {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }


    ಗಾಗಿ (i = 0; ಚಕ್ರ->modules[i]; i++) {
        ಆದರೆ (ಚಕ್ರ->modules[i]->type != NGX_CORE_MODULE) {
            ಮುಂದುವರಿ;
        }

        module = ಚಕ್ರ->modules[i]->ctx;

        ಆದರೆ (module->create_conf) {
            rv = module->create_conf(ಚಕ್ರ);
            ಆದರೆ (rv == ಖಾಲಿ) {
                ngx_destroy_pool(pool);
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }
            ಚಕ್ರ->conf_ctx[ಚಕ್ರ->modules[i]->ಸೂಚ್ಯಂಕ] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, ಗಾತ್ರ(ngx_conf_t));
    /* STUB: init array ? */
    conf.ವಾದಗಳು = ngx_array_create(pool, 10, ಗಾತ್ರ(ngx_str_t));
    ಆದರೆ (conf.ವಾದಗಳು == ಖಾಲಿ) {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    conf.ತಾತ್ಕಾಲಿಕ_pool = ngx_create_pool(NGX_ಚಕ್ರ_POOL_ಗಾತ್ರ, ಲಘುಗಣಕ);
    ಆದರೆ (conf.ತಾತ್ಕಾಲಿಕ_pool == ಖಾಲಿ) {
        ngx_destroy_pool(pool);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }


    conf.ctx = ಚಕ್ರ->conf_ctx;
    conf.ಚಕ್ರ = ಚಕ್ರ;
    conf.pool = pool;
    conf.ಲಘುಗಣಕ = ಲಘುಗಣಕ;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#ಆದರೆ 0
    ಲಘುಗಣಕ->log_level = NGX_LOG_DEBUG_ALL;
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_ಚಕ್ರ_pools(&conf);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (ngx_conf_parse(&conf, &ಚಕ್ರ->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_ಚಕ್ರ_pools(&conf);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (ngx_test_config && !ngx_quiet_ಬಹುಲಕ) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       ಚಕ್ರ->conf_file.ದತ್ತಾಂಶ);
    }

    ಗಾಗಿ (i = 0; ಚಕ್ರ->modules[i]; i++) {
        ಆದರೆ (ಚಕ್ರ->modules[i]->type != NGX_CORE_MODULE) {
            ಮುಂದುವರಿ;
        }

        module = ಚಕ್ರ->modules[i]->ctx;

        ಆದರೆ (module->init_conf) {
            ಆದರೆ (module->init_conf(ಚಕ್ರ,
                                  ಚಕ್ರ->conf_ctx[ಚಕ್ರ->modules[i]->ಸೂಚ್ಯಂಕ])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_ಚಕ್ರ_pools(&conf);
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }
        }
    }

    ಆದರೆ (ngx_process == NGX_PROCESS_SIGNALLER) {
        ಹಿಂತಿರುಗಿ ಚಕ್ರ;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(ಚಕ್ರ->conf_ctx, ngx_core_module);

    ಆದರೆ (ngx_test_config) {

        ಆದರೆ (ngx_create_pidfile(&ccf->pid, ಲಘುಗಣಕ) != NGX_OK) {
            ಹೋಗು failed;
        }

    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ngx_is_init_ಚಕ್ರ(old_ಚಕ್ರ)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_ಚಕ್ರ->conf_ctx,
                                                   ngx_core_module);
        ಆದರೆ (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.ದತ್ತಾಂಶ, old_ccf->pid.ದತ್ತಾಂಶ) != 0)
        {
            /* new pid file name */

            ಆದರೆ (ngx_create_pidfile(&ccf->pid, ಲಘುಗಣಕ) != NGX_OK) {
                ಹೋಗು failed;
            }

            ngx_delete_pidfile(old_ಚಕ್ರ);
        }
    }


    ಆದರೆ (ngx_test_lockfile(ಚಕ್ರ->ಬೀಗ_file.ದತ್ತಾಂಶ, ಲಘುಗಣಕ) != NGX_OK) {
        ಹೋಗು failed;
    }


    ಆದರೆ (ngx_create_paths(ಚಕ್ರ, ccf->user) != NGX_OK) {
        ಹೋಗು failed;
    }


    ಆದರೆ (ngx_log_open_default(ಚಕ್ರ) != NGX_OK) {
        ಹೋಗು failed;
    }

    /* open the new files */

    part = &ಚಕ್ರ->open_files.part;
    file = part->elts;

    ಗಾಗಿ (i = 0; /* void */ ; i++) {

        ಆದರೆ (i >= part->nelts) {
            ಆದರೆ (part->ಮುಂದಿನ == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }
            part = part->ಮುಂದಿನ;
            file = part->elts;
            i = 0;
        }

        ಆದರೆ (file[i].name.len == 0) {
            ಮುಂದುವರಿ;
        }

        file[i].fd = ngx_open_file(file[i].name.ದತ್ತಾಂಶ,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, ಲಘುಗಣಕ, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.ದತ್ತಾಂಶ);

        ಆದರೆ (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.ದತ್ತಾಂಶ);
            ಹೋಗು failed;
        }

#ಆದರೆ !(NGX_WIN32)
        ಆದರೆ (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.ದತ್ತಾಂಶ);
            ಹೋಗು failed;
        }
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಚಕ್ರ->ಲಘುಗಣಕ = &ಚಕ್ರ->new_log;
    pool->ಲಘುಗಣಕ = &ಚಕ್ರ->new_log;


    /* create shared memory */

    part = &ಚಕ್ರ->shared_ಸ್ಮರಣೆ.part;
    shm_zone = part->elts;

    ಗಾಗಿ (i = 0; /* void */ ; i++) {

        ಆದರೆ (i >= part->nelts) {
            ಆದರೆ (part->ಮುಂದಿನ == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }
            part = part->ಮುಂದಿನ;
            shm_zone = part->elts;
            i = 0;
        }

        ಆದರೆ (shm_zone[i].shm.ಗಾತ್ರ_2 == 0) {
            ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            ಹೋಗು failed;
        }

        shm_zone[i].shm.ಲಘುಗಣಕ = ಚಕ್ರ->ಲಘುಗಣಕ;

        opart = &old_ಚಕ್ರ->shared_ಸ್ಮರಣೆ.part;
        oshm_zone = opart->elts;

        ದತ್ತಾಂಶ = ಖಾಲಿ;

        ಗಾಗಿ (n = 0; /* void */ ; n++) {

            ಆದರೆ (n >= opart->nelts) {
                ಆದರೆ (opart->ಮುಂದಿನ == ಖಾಲಿ) {
                    ನಿಲ್ಲಿಸು;
                }
                opart = opart->ಮುಂದಿನ;
                oshm_zone = opart->elts;
                n = 0;
            }

            ಆದರೆ (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (ngx_strncmp(shm_zone[i].shm.name.ದತ್ತಾಂಶ,
                            oshm_zone[n].shm.name.ದತ್ತಾಂಶ,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (shm_zone[i].ಟ್ಯಾಗ್ == oshm_zone[n].ಟ್ಯಾಗ್ && shm_zone[i].noreuse) {
                ದತ್ತಾಂಶ = oshm_zone[n].ದತ್ತಾಂಶ;
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ (shm_zone[i].ಟ್ಯಾಗ್ == oshm_zone[n].ಟ್ಯಾಗ್
                && shm_zone[i].shm.ಗಾತ್ರ_2 == oshm_zone[n].shm.ಗಾತ್ರ_2)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#ಆದರೆ (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#ಆದರೆ_ಅಂತ್ಯ

                ಆದರೆ (shm_zone[i].init(&shm_zone[i], oshm_zone[n].ದತ್ತಾಂಶ)
                    != NGX_OK)
                {
                    ಹೋಗು failed;
                }

                ಹೋಗು shm_zone_ಸಿಕ್ಕಿತು;
            }

            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            ಹೋಗು failed;
        }

        ಆದರೆ (ngx_init_zone_pool(ಚಕ್ರ, &shm_zone[i]) != NGX_OK) {
            ಹೋಗು failed;
        }

        ಆದರೆ (shm_zone[i].init(&shm_zone[i], ದತ್ತಾಂಶ) != NGX_OK) {
            ಹೋಗು failed;
        }

    shm_zone_ಸಿಕ್ಕಿತು:

        ಮುಂದುವರಿ;
    }


    /* handle the listening sockets */

    ಆದರೆ (old_ಚಕ್ರ->listening.nelts) {
        ls = old_ಚಕ್ರ->listening.elts;
        ಗಾಗಿ (i = 0; i < old_ಚಕ್ರ->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = ಚಕ್ರ->listening.elts;
        ಗಾಗಿ (n = 0; n < ಚಕ್ರ->listening.nelts; n++) {

            ಗಾಗಿ (i = 0; i < old_ಚಕ್ರ->listening.nelts; i++) {
                ಆದರೆ (ls[i].ignore) {
                    ಮುಂದುವರಿ;
                }

                ಆದರೆ (ls[i].remain) {
                    ಮುಂದುವರಿ;
                }

                ಆದರೆ (ls[i].type != nls[n].type) {
                    ಮುಂದುವರಿ;
                }

                ಆದರೆ (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].ಹಿಂದಿನ = &ls[i];

                    ಆದರೆ (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } ಇಲ್ಲದಿದ್ದರೆ {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    ಆದರೆ (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    ಆದರೆ (ls[i].accept_filter && nls[n].accept_filter) {
                        ಆದರೆ (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].ಕೂಡಿಸು_deferred = 1;
                        }

                    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nls[n].accept_filter) {
                        nls[n].ಕೂಡಿಸು_deferred = 1;
                    }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    ಆದರೆ (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].ಕೂಡಿಸು_deferred = 1;
                    }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (NGX_HAVE_REUSEPORT)
                    ಆದರೆ (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].ಕೂಡಿಸು_reuseport = 1;
                    }
#ಆದರೆ_ಅಂತ್ಯ

                    ನಿಲ್ಲಿಸು;
                }
            }

            ಆದರೆ (nls[n].fd == (ngx_socket_t) -1) {
#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                ಆದರೆ (nls[n].accept_filter) {
                    nls[n].ಕೂಡಿಸು_deferred = 1;
                }
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                ಆದರೆ (nls[n].deferred_accept) {
                    nls[n].ಕೂಡಿಸು_deferred = 1;
                }
#ಆದರೆ_ಅಂತ್ಯ
            }
        }

    } ಇಲ್ಲದಿದ್ದರೆ {
#ಆದರೆ (NGX_HAVE_DEFERRED_ACCEPT)
        ls = ಚಕ್ರ->listening.elts;
        ಗಾಗಿ (i = 0; i < ಚಕ್ರ->listening.nelts; i++) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SO_ACCEPTFILTER
            ಆದರೆ (ls[i].accept_filter) {
                ls[i].ಕೂಡಿಸು_deferred = 1;
            }
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TCP_DEFER_ACCEPT
            ಆದರೆ (ls[i].deferred_accept) {
                ls[i].ಕೂಡಿಸು_deferred = 1;
            }
#ಆದರೆ_ಅಂತ್ಯ
        }
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಆದರೆ (ngx_open_listening_sockets(ಚಕ್ರ) != NGX_OK) {
        ಹೋಗು failed;
    }

    ಆದರೆ (!ngx_test_config) {
        ngx_configure_listening_sockets(ಚಕ್ರ);
    }


    /* commit the new cycle configuration */

    ಆದರೆ (!ngx_use_stderr) {
        (ಶೂನ್ಯ) ngx_log_redirect_stderr(ಚಕ್ರ);
    }

    pool->ಲಘುಗಣಕ = ಚಕ್ರ->ಲಘುಗಣಕ;

    ಆದರೆ (ngx_init_modules(ಚಕ್ರ) != NGX_OK) {
        /* fatal */
        ನಿರ್ಗಮಿಸು(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_ಚಕ್ರ->shared_ಸ್ಮರಣೆ.part;
    oshm_zone = opart->elts;

    ಗಾಗಿ (i = 0; /* void */ ; i++) {

        ಆದರೆ (i >= opart->nelts) {
            ಆದರೆ (opart->ಮುಂದಿನ == ಖಾಲಿ) {
                ಹೋಗು old_shm_zone_ಮುಗಿದ;
            }
            opart = opart->ಮುಂದಿನ;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &ಚಕ್ರ->shared_ಸ್ಮರಣೆ.part;
        shm_zone = part->elts;

        ಗಾಗಿ (n = 0; /* void */ ; n++) {

            ಆದರೆ (n >= part->nelts) {
                ಆದರೆ (part->ಮುಂದಿನ == ಖಾಲಿ) {
                    ನಿಲ್ಲಿಸು;
                }
                part = part->ಮುಂದಿನ;
                shm_zone = part->elts;
                n = 0;
            }

            ಆದರೆ (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (ngx_strncmp(oshm_zone[i].shm.name.ದತ್ತಾಂಶ,
                            shm_zone[n].shm.name.ದತ್ತಾಂಶ,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (oshm_zone[i].ಟ್ಯಾಗ್ == shm_zone[n].ಟ್ಯಾಗ್
                && oshm_zone[i].shm.ಗಾತ್ರ_2 == shm_zone[n].shm.ಗಾತ್ರ_2
                && !oshm_zone[i].noreuse)
            {
                ಹೋಗು ಜೀವಂತ_shm_zone;
            }

            ನಿಲ್ಲಿಸು;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    ಜೀವಂತ_shm_zone:

        ಮುಂದುವರಿ;
    }

old_shm_zone_ಮುಗಿದ:


    /* close the unnecessary listening sockets */

    ls = old_ಚಕ್ರ->listening.elts;
    ಗಾಗಿ (i = 0; i < old_ಚಕ್ರ->listening.nelts; i++) {

        ಆದರೆ (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_ಪಠ್ಯ);
        }

#ಆದರೆ (NGX_HAVE_UNIX_DOMAIN)

        ಆದರೆ (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_ಪಠ್ಯ.ದತ್ತಾಂಶ + ಗಾತ್ರ("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                          "deleting socket %s", name);

            ಆದರೆ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#ಆದರೆ_ಅಂತ್ಯ
    }


    /* close the unnecessary open files */

    part = &old_ಚಕ್ರ->open_files.part;
    file = part->elts;

    ಗಾಗಿ (i = 0; /* void */ ; i++) {

        ಆದರೆ (i >= part->nelts) {
            ಆದರೆ (part->ಮುಂದಿನ == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }
            part = part->ಮುಂದಿನ;
            file = part->elts;
            i = 0;
        }

        ಆದರೆ (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.ದತ್ತಾಂಶ);
        }
    }

    ngx_destroy_pool(conf.ತಾತ್ಕಾಲಿಕ_pool);

    ಆದರೆ (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_ಚಕ್ರ(old_ಚಕ್ರ)) {

        ngx_destroy_pool(old_ಚಕ್ರ->pool);
        ಚಕ್ರ->old_ಚಕ್ರ = ಖಾಲಿ;

        ಹಿಂತಿರುಗಿ ಚಕ್ರ;
    }


    ಆದರೆ (ngx_ತಾತ್ಕಾಲಿಕ_pool == ಖಾಲಿ) {
        ngx_ತಾತ್ಕಾಲಿಕ_pool = ngx_create_pool(128, ಚಕ್ರ->ಲಘುಗಣಕ);
        ಆದರೆ (ngx_ತಾತ್ಕಾಲಿಕ_pool == ಖಾಲಿ) {
            ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                          "could not create ngx_temp_pool");
            ನಿರ್ಗಮಿಸು(1);
        }

        n = 10;

        ಆದರೆ (ngx_array_init(&ngx_old_ಚಕ್ರಗಳು, ngx_ತಾತ್ಕಾಲಿಕ_pool, n,
                           ಗಾತ್ರ(ngx_ಚಕ್ರ_t *))
            != NGX_OK)
        {
            ನಿರ್ಗಮಿಸು(1);
        }

        ngx_memzero(ngx_old_ಚಕ್ರಗಳು.elts, n * ಗಾತ್ರ(ngx_ಚಕ್ರ_t *));

        ngx_cleaner_event.handler = ngx_clean_old_ಚಕ್ರಗಳು;
        ngx_cleaner_event.ಲಘುಗಣಕ = ಚಕ್ರ->ಲಘುಗಣಕ;
        ngx_cleaner_event.ದತ್ತಾಂಶ = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_ತಾತ್ಕಾಲಿಕ_pool->ಲಘುಗಣಕ = ಚಕ್ರ->ಲಘುಗಣಕ;

    old = ngx_array_ತಳ್ಳು(&ngx_old_ಚಕ್ರಗಳು);
    ಆದರೆ (old == ಖಾಲಿ) {
        ನಿರ್ಗಮಿಸು(1);
    }
    *old = old_ಚಕ್ರ;

    ಆದರೆ (!ngx_cleaner_event.timer_ಗಣ) {
        ngx_ಕೂಡಿಸು_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_ಗಣ = 1;
    }

    ಹಿಂತಿರುಗಿ ಚಕ್ರ;


failed:

    ಆದರೆ (!ngx_is_init_ಚಕ್ರ(old_ಚಕ್ರ)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_ಚಕ್ರ->conf_ctx,
                                                   ngx_core_module);
        ಆದರೆ (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &ಚಕ್ರ->open_files.part;
    file = part->elts;

    ಗಾಗಿ (i = 0; /* void */ ; i++) {

        ಆದರೆ (i >= part->nelts) {
            ಆದರೆ (part->ಮುಂದಿನ == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }
            part = part->ಮುಂದಿನ;
            file = part->elts;
            i = 0;
        }

        ಆದರೆ (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.ದತ್ತಾಂಶ);
        }
    }

    /* free the newly created shared memory */

    part = &ಚಕ್ರ->shared_ಸ್ಮರಣೆ.part;
    shm_zone = part->elts;

    ಗಾಗಿ (i = 0; /* void */ ; i++) {

        ಆದರೆ (i >= part->nelts) {
            ಆದರೆ (part->ಮುಂದಿನ == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }
            part = part->ಮುಂದಿನ;
            shm_zone = part->elts;
            i = 0;
        }

        ಆದರೆ (shm_zone[i].shm.addr == ಖಾಲಿ) {
            ಮುಂದುವರಿ;
        }

        opart = &old_ಚಕ್ರ->shared_ಸ್ಮರಣೆ.part;
        oshm_zone = opart->elts;

        ಗಾಗಿ (n = 0; /* void */ ; n++) {

            ಆದರೆ (n >= opart->nelts) {
                ಆದರೆ (opart->ಮುಂದಿನ == ಖಾಲಿ) {
                    ನಿಲ್ಲಿಸು;
                }
                opart = opart->ಮುಂದಿನ;
                oshm_zone = opart->elts;
                n = 0;
            }

            ಆದರೆ (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (ngx_strncmp(shm_zone[i].shm.name.ದತ್ತಾಂಶ,
                            oshm_zone[n].shm.name.ದತ್ತಾಂಶ,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                ಮುಂದುವರಿ;
            }

            ಆದರೆ (shm_zone[i].ಟ್ಯಾಗ್ == oshm_zone[n].ಟ್ಯಾಗ್
                && shm_zone[i].shm.ಗಾತ್ರ_2 == oshm_zone[n].shm.ಗಾತ್ರ_2
                && !shm_zone[i].noreuse)
            {
                ಹೋಗು old_shm_zone_ಸಿಕ್ಕಿತು;
            }

            ನಿಲ್ಲಿಸು;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_ಸಿಕ್ಕಿತು:

        ಮುಂದುವರಿ;
    }

    ಆದರೆ (ngx_test_config) {
        ngx_destroy_ಚಕ್ರ_pools(&conf);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ls = ಚಕ್ರ->listening.elts;
    ಗಾಗಿ (i = 0; i < ಚಕ್ರ->listening.nelts; i++) {
        ಆದರೆ (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_ಪಠ್ಯ);
        }
    }

    ngx_destroy_ಚಕ್ರ_pools(&conf);

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_destroy_ಚಕ್ರ_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->ತಾತ್ಕಾಲಿಕ_pool);
    ngx_destroy_pool(conf->pool);
}


ಸ್ಥಿರ ngx_int_t
ngx_init_zone_pool(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    ಆದರೆ (zn->shm.exists) {

        ಆದರೆ (sp == sp->addr) {
            ಹಿಂತಿರುಗಿ NGX_OK;
        }

#ಆದರೆ (NGX_WIN32)

        /* remap at the required address */

        ಆದರೆ (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            ಹಿಂತಿರುಗಿ NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        ಆದರೆ (sp == sp->addr) {
            ಹಿಂತಿರುಗಿ NGX_OK;
        }

#ಆದರೆ_ಅಂತ್ಯ

        ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    sp->ಕೊನೆ = zn->shm.addr + zn->shm.ಗಾತ್ರ_2;
    sp->min_ಸರಿಸು = 3;
    sp->addr = zn->shm.addr;

#ಆದರೆ (NGX_HAVE_ATOMIC_OPS)

    file = ಖಾಲಿ;

#ಇಲ್ಲದಿದ್ದರೆ

    file = ngx_pnalloc(ಚಕ್ರ->pool,
                       ಚಕ್ರ->ಬೀಗ_file.len + zn->shm.name.len + 1);
    ಆದರೆ (file == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    (ಶೂನ್ಯ) ngx_sprintf(file, "%V%V%Z", &ಚಕ್ರ->ಬೀಗ_file, &zn->shm.name);

#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (ngx_shmtx_create(&sp->mutex, &sp->ಬೀಗ, file) != NGX_OK) {
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ngx_slab_init(sp);

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *ಲಘುಗಣಕ)
{
    ಗಾತ್ರ_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    ಆದರೆ (ngx_process > NGX_PROCESS_MASTER) {
        ಹಿಂತಿರುಗಿ NGX_OK;
    }

    ngx_memzero(&file, ಗಾತ್ರ(ngx_file_t));

    file.name = *name;
    file.ಲಘುಗಣಕ = ಲಘುಗಣಕ;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.ದತ್ತಾಂಶ, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    ಆದರೆ (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.ದತ್ತಾಂಶ);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    rc = NGX_OK;

    ಆದರೆ (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        ಆದರೆ (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    ಆದರೆ (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.ದತ್ತಾಂಶ);
    }

    ಹಿಂತಿರುಗಿ rc;
}


ಶೂನ್ಯ
ngx_delete_pidfile(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ಚಕ್ರ->conf_ctx, ngx_core_module);

    name = ngx_new_ದ್ವಿಮಾನ ? ccf->oldpid.ದತ್ತಾಂಶ : ccf->pid.ದತ್ತಾಂಶ;

    ಆದರೆ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ಅಕ್ಷರ *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, ಚಕ್ರ->ಲಘುಗಣಕ, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(ಚಕ್ರ->conf_ctx, ngx_core_module);

    ngx_memzero(&file, ಗಾತ್ರ(ngx_file_t));

    file.name = ccf->pid;
    file.ಲಘುಗಣಕ = ಚಕ್ರ->ಲಘುಗಣಕ;

    file.fd = ngx_open_file(file.name.ದತ್ತಾಂಶ, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    ಆದರೆ (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.ದತ್ತಾಂಶ);
        ಹಿಂತಿರುಗಿ 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    ಆದರೆ (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.ದತ್ತಾಂಶ);
    }

    ಆದರೆ (n == NGX_ERROR) {
        ಹಿಂತಿರುಗಿ 1;
    }

    ಆಗ (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    ಆದರೆ (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.ದತ್ತಾಂಶ);
        ಹಿಂತಿರುಗಿ 1;
    }

    ಹಿಂತಿರುಗಿ ngx_os_signal_process(ಚಕ್ರ, sig, pid);

}


ಸ್ಥಿರ ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *ಲಘುಗಣಕ)
{
#ಆದರೆ !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    ಆದರೆ (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, ಲಘುಗಣಕ, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        ಹಿಂತಿರುಗಿ NGX_ERROR;
    }

    ಆದರೆ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    ಆದರೆ (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, ಲಘುಗಣಕ, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ NGX_OK;
}


ಶೂನ್ಯ
ngx_reopen_files(ngx_ಚಕ್ರ_t *ಚಕ್ರ, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &ಚಕ್ರ->open_files.part;
    file = part->elts;

    ಗಾಗಿ (i = 0; /* void */ ; i++) {

        ಆದರೆ (i >= part->nelts) {
            ಆದರೆ (part->ಮುಂದಿನ == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }
            part = part->ಮುಂದಿನ;
            file = part->elts;
            i = 0;
        }

        ಆದರೆ (file[i].name.len == 0) {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (file[i].flush) {
            file[i].flush(&file[i], ಚಕ್ರ->ಲಘುಗಣಕ);
        }

        fd = ngx_open_file(file[i].name.ದತ್ತಾಂಶ, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, ಚಕ್ರ->ಲಘುಗಣಕ, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.ದತ್ತಾಂಶ, file[i].fd, fd);

        ಆದರೆ (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.ದತ್ತಾಂಶ);
            ಮುಂದುವರಿ;
        }

#ಆದರೆ !(NGX_WIN32)
        ಆದರೆ (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            ಆದರೆ (ngx_file_info(file[i].name.ದತ್ತಾಂಶ, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.ದತ್ತಾಂಶ);

                ಆದರೆ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.ದತ್ತಾಂಶ);
                }

                ಮುಂದುವರಿ;
            }

            ಆದರೆ (fi.st_uid != user) {
                ಆದರೆ (chown((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) file[i].name.ದತ್ತಾಂಶ, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.ದತ್ತಾಂಶ, user);

                    ಆದರೆ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.ದತ್ತಾಂಶ);
                    }

                    ಮುಂದುವರಿ;
                }
            }

            ಆದರೆ ((fi.st_ಬಹುಲಕ & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_ಬಹುಲಕ |= (S_IRUSR|S_IWUSR);

                ಆದರೆ (chmod((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) file[i].name.ದತ್ತಾಂಶ, fi.st_ಬಹುಲಕ) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.ದತ್ತಾಂಶ);

                    ಆದರೆ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.ದತ್ತಾಂಶ);
                    }

                    ಮುಂದುವರಿ;
                }
            }
        }

        ಆದರೆ (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.ದತ್ತಾಂಶ);

            ಆದರೆ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.ದತ್ತಾಂಶ);
            }

            ಮುಂದುವರಿ;
        }
#ಆದರೆ_ಅಂತ್ಯ

        ಆದರೆ (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, ಚಕ್ರ->ಲಘುಗಣಕ, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.ದತ್ತಾಂಶ);
        }

        file[i].fd = fd;
    }

    (ಶೂನ್ಯ) ngx_log_redirect_stderr(ಚಕ್ರ);
}


ngx_shm_zone_t *
ngx_shared_ಸ್ಮರಣೆ_ಕೂಡಿಸು(ngx_conf_t *cf, ngx_str_t *name, ಗಾತ್ರ_t ಗಾತ್ರ_2, ಶೂನ್ಯ *ಟ್ಯಾಗ್)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->ಚಕ್ರ->shared_ಸ್ಮರಣೆ.part;
    shm_zone = part->elts;

    ಗಾಗಿ (i = 0; /* void */ ; i++) {

        ಆದರೆ (i >= part->nelts) {
            ಆದರೆ (part->ಮುಂದಿನ == ಖಾಲಿ) {
                ನಿಲ್ಲಿಸು;
            }
            part = part->ಮುಂದಿನ;
            shm_zone = part->elts;
            i = 0;
        }

        ಆದರೆ (name->len != shm_zone[i].shm.name.len) {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ngx_strncmp(name->ದತ್ತಾಂಶ, shm_zone[i].shm.name.ದತ್ತಾಂಶ, name->len)
            != 0)
        {
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ಟ್ಯಾಗ್ != shm_zone[i].ಟ್ಯಾಗ್) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ಆದರೆ (shm_zone[i].shm.ಗಾತ್ರ_2 == 0) {
            shm_zone[i].shm.ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
        }

        ಆದರೆ (ಗಾತ್ರ_2 && ಗಾತ್ರ_2 != shm_zone[i].shm.ಗಾತ್ರ_2) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            ಗಾತ್ರ_2, &shm_zone[i].shm.name, shm_zone[i].shm.ಗಾತ್ರ_2);
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ಹಿಂತಿರುಗಿ &shm_zone[i];
    }

    shm_zone = ngx_list_ತಳ್ಳು(&cf->ಚಕ್ರ->shared_ಸ್ಮರಣೆ);

    ಆದರೆ (shm_zone == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    shm_zone->ದತ್ತಾಂಶ = ಖಾಲಿ;
    shm_zone->shm.ಲಘುಗಣಕ = cf->ಚಕ್ರ->ಲಘುಗಣಕ;
    shm_zone->shm.addr = ಖಾಲಿ;
    shm_zone->shm.ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = ಖಾಲಿ;
    shm_zone->ಟ್ಯಾಗ್ = ಟ್ಯಾಗ್;
    shm_zone->noreuse = 0;

    ಹಿಂತಿರುಗಿ shm_zone;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_clean_old_ಚಕ್ರಗಳು(ngx_event_t *ev)
{
    ngx_uint_t     i, n, ಸಿಕ್ಕಿತು, ಜೀವಂತ;
    ngx_log_t     *ಲಘುಗಣಕ;
    ngx_ಚಕ್ರ_t  **ಚಕ್ರ;

    ಲಘುಗಣಕ = ngx_ಚಕ್ರ->ಲಘುಗಣಕ;
    ngx_ತಾತ್ಕಾಲಿಕ_pool->ಲಘುಗಣಕ = ಲಘುಗಣಕ;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, ಲಘುಗಣಕ, 0, "clean old cycles");

    ಜೀವಂತ = 0;

    ಚಕ್ರ = ngx_old_ಚಕ್ರಗಳು.elts;
    ಗಾಗಿ (i = 0; i < ngx_old_ಚಕ್ರಗಳು.nelts; i++) {

        ಆದರೆ (ಚಕ್ರ[i] == ಖಾಲಿ) {
            ಮುಂದುವರಿ;
        }

        ಸಿಕ್ಕಿತು = 0;

        ಗಾಗಿ (n = 0; n < ಚಕ್ರ[i]->connection_n; n++) {
            ಆದರೆ (ಚಕ್ರ[i]->connections[n].fd != (ngx_socket_t) -1) {
                ಸಿಕ್ಕಿತು = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, ಲಘುಗಣಕ, 0, "live fd:%ui", n);

                ನಿಲ್ಲಿಸು;
            }
        }

        ಆದರೆ (ಸಿಕ್ಕಿತು) {
            ಜೀವಂತ = 1;
            ಮುಂದುವರಿ;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ಲಘುಗಣಕ, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(ಚಕ್ರ[i]->pool);
        ಚಕ್ರ[i] = ಖಾಲಿ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ಲಘುಗಣಕ, 0, "old cycles status: %ui", ಜೀವಂತ);

    ಆದರೆ (ಜೀವಂತ) {
        ngx_ಕೂಡಿಸು_timer(ev, 30000);

    } ಇಲ್ಲದಿದ್ದರೆ {
        ngx_destroy_pool(ngx_ತಾತ್ಕಾಲಿಕ_pool);
        ngx_ತಾತ್ಕಾಲಿಕ_pool = ಖಾಲಿ;
        ngx_old_ಚಕ್ರಗಳು.nelts = 0;
    }
}


ಶೂನ್ಯ
ngx_ಗಣ_shutdown_timer(ngx_ಚಕ್ರ_t *ಚಕ್ರ)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ಚಕ್ರ->conf_ctx, ngx_core_module);

    ಆದರೆ (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.ದತ್ತಾಂಶ = ಚಕ್ರ;
        ngx_shutdown_event.ಲಘುಗಣಕ = ಚಕ್ರ->ಲಘುಗಣಕ;
        ngx_shutdown_event.cancelable = 1;

        ngx_ಕೂಡಿಸು_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_ಚಕ್ರ_t       *ಚಕ್ರ;
    ngx_connection_t  *c;

    ಚಕ್ರ = ev->ದತ್ತಾಂಶ;

    c = ಚಕ್ರ->connections;

    ಗಾಗಿ (i = 0; i < ಚಕ್ರ->connection_n; i++) {

        ಆದರೆ (c[i].fd == (ngx_socket_t) -1
            || c[i].ಪಠ == ಖಾಲಿ
            || c[i].ಪಠ->accept
            || c[i].ಪಠ->channel
            || c[i].ಪಠ->resolver)
        {
            ಮುಂದುವರಿ;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->ಲಘುಗಣಕ, 0,
                       "*%uA shutdown timeout", c[i].ಸಂಖ್ಯೆ);

        c[i].close = 1;
        c[i].error = 1;

        c[i].ಪಠ->handler(c[i].ಪಠ);
    }
}
