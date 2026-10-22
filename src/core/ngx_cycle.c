
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


ساكن فراغ ngx_destroy_دوره_pools(ngx_conf_t *conf);
ساكن ngx_int_t ngx_init_zone_pool(ngx_دوره_t *دوره,
    ngx_shm_zone_t *shm_zone);
ساكن ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *لوګاریتم);
ساكن فراغ ngx_clean_old_دورې(ngx_event_t *ev);
ساكن فراغ ngx_shutdown_timer_handler(ngx_event_t *ev);


متطاير ngx_دوره_t  *ngx_دوره;
ngx_array_t            ngx_old_دورې;

ساكن ngx_pool_t     *ngx_لنډمهاله_pool;
ساكن ngx_event_t     ngx_cleaner_event;
ساكن ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_ډېرپېښ;


/* STUB NAME */
ساكن ngx_connection_t  dumb;
/* STUB */


ngx_دوره_t *
ngx_init_دوره(ngx_دوره_t *old_دوره)
{
    فراغ                *rv, *معلومات;
    حرف               **senv;
    ngx_uint_t           i, n;
    ngx_log_t           *لوګاریتم;
    ngx_وخت_t          *tp;
    ngx_conf_t           conf;
    ngx_pool_t          *pool;
    ngx_دوره_t         *دوره, **old;
    ngx_shm_zone_t      *shm_zone, *oshm_zone;
    ngx_list_part_t     *part, *opart;
    ngx_open_file_t     *file;
    ngx_listening_t     *ls, *nls;
    ngx_core_conf_t     *ccf, *old_ccf;
    ngx_core_module_t   *module;
    حرف                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_وخت_update();


    لوګاریتم = old_دوره->لوګاریتم;

    pool = ngx_create_pool(NGX_دوره_POOL_اندازه, لوګاریتم);
    إذا (pool == عدم) {
        أرجع عدم;
    }
    pool->لوګاریتم = لوګاریتم;

    دوره = ngx_pcalloc(pool, حجم(ngx_دوره_t));
    إذا (دوره == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    دوره->pool = pool;
    دوره->لوګاریتم = لوګاریتم;
    دوره->old_دوره = old_دوره;

    دوره->conf_مختاړی.len = old_دوره->conf_مختاړی.len;
    دوره->conf_مختاړی.معلومات = ngx_pstrdup(pool, &old_دوره->conf_مختاړی);
    إذا (دوره->conf_مختاړی.معلومات == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    دوره->مختاړی.len = old_دوره->مختاړی.len;
    دوره->مختاړی.معلومات = ngx_pstrdup(pool, &old_دوره->مختاړی);
    إذا (دوره->مختاړی.معلومات == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    دوره->error_log.len = old_دوره->error_log.len;
    دوره->error_log.معلومات = ngx_pnalloc(pool, old_دوره->error_log.len + 1);
    إذا (دوره->error_log.معلومات == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }
    ngx_cpystrn(دوره->error_log.معلومات, old_دوره->error_log.معلومات,
                old_دوره->error_log.len + 1);

    دوره->conf_file.len = old_دوره->conf_file.len;
    دوره->conf_file.معلومات = ngx_pnalloc(pool, old_دوره->conf_file.len + 1);
    إذا (دوره->conf_file.معلومات == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }
    ngx_cpystrn(دوره->conf_file.معلومات, old_دوره->conf_file.معلومات,
                old_دوره->conf_file.len + 1);

    إذا (old_دوره->conf_param.len) {
        دوره->conf_param.len = old_دوره->conf_param.len;
        دوره->conf_param.معلومات = ngx_pstrdup(pool, &old_دوره->conf_param);
        إذا (دوره->conf_param.معلومات == عدم) {
            ngx_destroy_pool(pool);
            أرجع عدم;
        }
    }


    n = old_دوره->paths.nelts ? old_دوره->paths.nelts : 10;

    إذا (ngx_array_init(&دوره->paths, pool, n, حجم(ngx_لار_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_memzero(دوره->paths.elts, n * حجم(ngx_لار_t *));


    إذا (ngx_array_init(&دوره->config_dump, pool, 1, حجم(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_rbtree_init(&دوره->config_dump_rbtree, &دوره->config_dump_sentinel,
                    ngx_str_rbtree_ورننباسه_ارزښت);

    إذا (old_دوره->open_files.part.nelts) {
        n = old_دوره->open_files.part.nelts;
        لكل (part = old_دوره->open_files.part.بل; part; part = part->بل) {
            n += part->nelts;
        }

    } وإلا {
        n = 20;
    }

    إذا (ngx_list_init(&دوره->open_files, pool, n, حجم(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    إذا (old_دوره->shared_حافظه.part.nelts) {
        n = old_دوره->shared_حافظه.part.nelts;
        لكل (part = old_دوره->shared_حافظه.part.بل; part; part = part->بل)
        {
            n += part->nelts;
        }

    } وإلا {
        n = 1;
    }

    إذا (ngx_list_init(&دوره->shared_حافظه, pool, n, حجم(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    n = old_دوره->listening.nelts ? old_دوره->listening.nelts : 10;

    إذا (ngx_array_init(&دوره->listening, pool, n, حجم(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_memzero(دوره->listening.elts, n * حجم(ngx_listening_t));


    ngx_کتار_init(&دوره->reusable_connections_کتار);


    دوره->conf_ctx = ngx_pcalloc(pool, ngx_max_module * حجم(فراغ *));
    إذا (دوره->conf_ctx == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    إذا (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, لوګاریتم, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    دوره->hostname.len = ngx_strlen(hostname);

    دوره->hostname.معلومات = ngx_pnalloc(pool, دوره->hostname.len);
    إذا (دوره->hostname.معلومات == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_strlow(دوره->hostname.معلومات, (u_char *) hostname, دوره->hostname.len);


    إذا (ngx_دوره_modules(دوره) != NGX_OK) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    لكل (i = 0; دوره->modules[i]; i++) {
        إذا (دوره->modules[i]->type != NGX_CORE_MODULE) {
            استمر;
        }

        module = دوره->modules[i]->ctx;

        إذا (module->create_conf) {
            rv = module->create_conf(دوره);
            إذا (rv == عدم) {
                ngx_destroy_pool(pool);
                أرجع عدم;
            }
            دوره->conf_ctx[دوره->modules[i]->شاخص] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, حجم(ngx_conf_t));
    /* STUB: init array ? */
    conf.دلیلونه = ngx_array_create(pool, 10, حجم(ngx_str_t));
    إذا (conf.دلیلونه == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    conf.لنډمهاله_pool = ngx_create_pool(NGX_دوره_POOL_اندازه, لوګاریتم);
    إذا (conf.لنډمهاله_pool == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    conf.ctx = دوره->conf_ctx;
    conf.دوره = دوره;
    conf.pool = pool;
    conf.لوګاریتم = لوګاریتم;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#إذا 0
    لوګاریتم->log_level = NGX_LOG_DEBUG_ALL;
#د_که_پای

    إذا (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_دوره_pools(&conf);
        أرجع عدم;
    }

    إذا (ngx_conf_parse(&conf, &دوره->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_دوره_pools(&conf);
        أرجع عدم;
    }

    إذا (ngx_test_config && !ngx_quiet_ډېرپېښ) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       دوره->conf_file.معلومات);
    }

    لكل (i = 0; دوره->modules[i]; i++) {
        إذا (دوره->modules[i]->type != NGX_CORE_MODULE) {
            استمر;
        }

        module = دوره->modules[i]->ctx;

        إذا (module->init_conf) {
            إذا (module->init_conf(دوره,
                                  دوره->conf_ctx[دوره->modules[i]->شاخص])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_دوره_pools(&conf);
                أرجع عدم;
            }
        }
    }

    إذا (ngx_process == NGX_PROCESS_SIGNALLER) {
        أرجع دوره;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(دوره->conf_ctx, ngx_core_module);

    إذا (ngx_test_config) {

        إذا (ngx_create_pidfile(&ccf->pid, لوګاریتم) != NGX_OK) {
            اذهب failed;
        }

    } وإلا إذا (!ngx_is_init_دوره(old_دوره)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_دوره->conf_ctx,
                                                   ngx_core_module);
        إذا (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.معلومات, old_ccf->pid.معلومات) != 0)
        {
            /* new pid file name */

            إذا (ngx_create_pidfile(&ccf->pid, لوګاریتم) != NGX_OK) {
                اذهب failed;
            }

            ngx_delete_pidfile(old_دوره);
        }
    }


    إذا (ngx_test_lockfile(دوره->قلف_file.معلومات, لوګاریتم) != NGX_OK) {
        اذهب failed;
    }


    إذا (ngx_create_paths(دوره, ccf->user) != NGX_OK) {
        اذهب failed;
    }


    إذا (ngx_log_open_default(دوره) != NGX_OK) {
        اذهب failed;
    }

    /* open the new files */

    part = &دوره->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بل == عدم) {
                اكسر;
            }
            part = part->بل;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].name.len == 0) {
            استمر;
        }

        file[i].fd = ngx_open_file(file[i].name.معلومات,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, لوګاریتم, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.معلومات);

        إذا (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, لوګاریتم, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.معلومات);
            اذهب failed;
        }

#إذا !(NGX_WIN32)
        إذا (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لوګاریتم, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.معلومات);
            اذهب failed;
        }
#د_که_پای
    }

    دوره->لوګاریتم = &دوره->new_log;
    pool->لوګاریتم = &دوره->new_log;


    /* create shared memory */

    part = &دوره->shared_حافظه.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بل == عدم) {
                اكسر;
            }
            part = part->بل;
            shm_zone = part->elts;
            i = 0;
        }

        إذا (shm_zone[i].shm.اندازه == 0) {
            ngx_log_error(NGX_LOG_EMERG, لوګاریتم, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            اذهب failed;
        }

        shm_zone[i].shm.لوګاریتم = دوره->لوګاریتم;

        opart = &old_دوره->shared_حافظه.part;
        oshm_zone = opart->elts;

        معلومات = عدم;

        لكل (n = 0; /* void */ ; n++) {

            إذا (n >= opart->nelts) {
                إذا (opart->بل == عدم) {
                    اكسر;
                }
                opart = opart->بل;
                oshm_zone = opart->elts;
                n = 0;
            }

            إذا (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                استمر;
            }

            إذا (ngx_strncmp(shm_zone[i].shm.name.معلومات,
                            oshm_zone[n].shm.name.معلومات,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                استمر;
            }

            إذا (shm_zone[i].نښان == oshm_zone[n].نښان && shm_zone[i].noreuse) {
                معلومات = oshm_zone[n].معلومات;
                اكسر;
            }

            إذا (shm_zone[i].نښان == oshm_zone[n].نښان
                && shm_zone[i].shm.اندازه == oshm_zone[n].shm.اندازه)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#إذا (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#د_که_پای

                إذا (shm_zone[i].init(&shm_zone[i], oshm_zone[n].معلومات)
                    != NGX_OK)
                {
                    اذهب failed;
                }

                اذهب shm_zone_وموندل;
            }

            اكسر;
        }

        إذا (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            اذهب failed;
        }

        إذا (ngx_init_zone_pool(دوره, &shm_zone[i]) != NGX_OK) {
            اذهب failed;
        }

        إذا (shm_zone[i].init(&shm_zone[i], معلومات) != NGX_OK) {
            اذهب failed;
        }

    shm_zone_وموندل:

        استمر;
    }


    /* handle the listening sockets */

    إذا (old_دوره->listening.nelts) {
        ls = old_دوره->listening.elts;
        لكل (i = 0; i < old_دوره->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = دوره->listening.elts;
        لكل (n = 0; n < دوره->listening.nelts; n++) {

            لكل (i = 0; i < old_دوره->listening.nelts; i++) {
                إذا (ls[i].ignore) {
                    استمر;
                }

                إذا (ls[i].remain) {
                    استمر;
                }

                إذا (ls[i].type != nls[n].type) {
                    استمر;
                }

                إذا (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].پخوانی = &ls[i];

                    إذا (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } وإلا {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    إذا (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    إذا (ls[i].accept_filter && nls[n].accept_filter) {
                        إذا (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].جمع_کړه_deferred = 1;
                        }

                    } وإلا إذا (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } وإلا إذا (nls[n].accept_filter) {
                        nls[n].جمع_کړه_deferred = 1;
                    }
#د_که_پای

#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    إذا (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } وإلا إذا (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].جمع_کړه_deferred = 1;
                    }
#د_که_پای

#إذا (NGX_HAVE_REUSEPORT)
                    إذا (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].جمع_کړه_reuseport = 1;
                    }
#د_که_پای

                    اكسر;
                }
            }

            إذا (nls[n].fd == (ngx_socket_t) -1) {
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                إذا (nls[n].accept_filter) {
                    nls[n].جمع_کړه_deferred = 1;
                }
#د_که_پای
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                إذا (nls[n].deferred_accept) {
                    nls[n].جمع_کړه_deferred = 1;
                }
#د_که_پای
            }
        }

    } وإلا {
#إذا (NGX_HAVE_DEFERRED_ACCEPT)
        ls = دوره->listening.elts;
        لكل (i = 0; i < دوره->listening.nelts; i++) {
#که_تعریف_شوی SO_ACCEPTFILTER
            إذا (ls[i].accept_filter) {
                ls[i].جمع_کړه_deferred = 1;
            }
#د_که_پای
#که_تعریف_شوی TCP_DEFER_ACCEPT
            إذا (ls[i].deferred_accept) {
                ls[i].جمع_کړه_deferred = 1;
            }
#د_که_پای
        }
#د_که_پای
    }

    إذا (ngx_open_listening_sockets(دوره) != NGX_OK) {
        اذهب failed;
    }

    إذا (!ngx_test_config) {
        ngx_configure_listening_sockets(دوره);
    }


    /* commit the new cycle configuration */

    إذا (!ngx_use_stderr) {
        (فراغ) ngx_log_redirect_stderr(دوره);
    }

    pool->لوګاریتم = دوره->لوګاریتم;

    إذا (ngx_init_modules(دوره) != NGX_OK) {
        /* fatal */
        وتل(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_دوره->shared_حافظه.part;
    oshm_zone = opart->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= opart->nelts) {
            إذا (opart->بل == عدم) {
                اذهب old_shm_zone_بشپړ;
            }
            opart = opart->بل;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &دوره->shared_حافظه.part;
        shm_zone = part->elts;

        لكل (n = 0; /* void */ ; n++) {

            إذا (n >= part->nelts) {
                إذا (part->بل == عدم) {
                    اكسر;
                }
                part = part->بل;
                shm_zone = part->elts;
                n = 0;
            }

            إذا (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                استمر;
            }

            إذا (ngx_strncmp(oshm_zone[i].shm.name.معلومات,
                            shm_zone[n].shm.name.معلومات,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                استمر;
            }

            إذا (oshm_zone[i].نښان == shm_zone[n].نښان
                && oshm_zone[i].shm.اندازه == shm_zone[n].shm.اندازه
                && !oshm_zone[i].noreuse)
            {
                اذهب ژوندی_shm_zone;
            }

            اكسر;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    ژوندی_shm_zone:

        استمر;
    }

old_shm_zone_بشپړ:


    /* close the unnecessary listening sockets */

    ls = old_دوره->listening.elts;
    لكل (i = 0; i < old_دوره->listening.nelts; i++) {

        إذا (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            استمر;
        }

        إذا (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لوګاریتم, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_متن);
        }

#إذا (NGX_HAVE_UNIX_DOMAIN)

        إذا (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_متن.معلومات + حجم("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, دوره->لوګاریتم, 0,
                          "deleting socket %s", name);

            إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#د_که_پای
    }


    /* close the unnecessary open files */

    part = &old_دوره->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بل == عدم) {
                اكسر;
            }
            part = part->بل;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            استمر;
        }

        إذا (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, لوګاریتم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.معلومات);
        }
    }

    ngx_destroy_pool(conf.لنډمهاله_pool);

    إذا (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_دوره(old_دوره)) {

        ngx_destroy_pool(old_دوره->pool);
        دوره->old_دوره = عدم;

        أرجع دوره;
    }


    إذا (ngx_لنډمهاله_pool == عدم) {
        ngx_لنډمهاله_pool = ngx_create_pool(128, دوره->لوګاریتم);
        إذا (ngx_لنډمهاله_pool == عدم) {
            ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, 0,
                          "could not create ngx_temp_pool");
            وتل(1);
        }

        n = 10;

        إذا (ngx_array_init(&ngx_old_دورې, ngx_لنډمهاله_pool, n,
                           حجم(ngx_دوره_t *))
            != NGX_OK)
        {
            وتل(1);
        }

        ngx_memzero(ngx_old_دورې.elts, n * حجم(ngx_دوره_t *));

        ngx_cleaner_event.handler = ngx_clean_old_دورې;
        ngx_cleaner_event.لوګاریتم = دوره->لوګاریتم;
        ngx_cleaner_event.معلومات = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_لنډمهاله_pool->لوګاریتم = دوره->لوګاریتم;

    old = ngx_array_ټېل(&ngx_old_دورې);
    إذا (old == عدم) {
        وتل(1);
    }
    *old = old_دوره;

    إذا (!ngx_cleaner_event.timer_سېټ) {
        ngx_جمع_کړه_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_سېټ = 1;
    }

    أرجع دوره;


failed:

    إذا (!ngx_is_init_دوره(old_دوره)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_دوره->conf_ctx,
                                                   ngx_core_module);
        إذا (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &دوره->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بل == عدم) {
                اكسر;
            }
            part = part->بل;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            استمر;
        }

        إذا (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, لوګاریتم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.معلومات);
        }
    }

    /* free the newly created shared memory */

    part = &دوره->shared_حافظه.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بل == عدم) {
                اكسر;
            }
            part = part->بل;
            shm_zone = part->elts;
            i = 0;
        }

        إذا (shm_zone[i].shm.addr == عدم) {
            استمر;
        }

        opart = &old_دوره->shared_حافظه.part;
        oshm_zone = opart->elts;

        لكل (n = 0; /* void */ ; n++) {

            إذا (n >= opart->nelts) {
                إذا (opart->بل == عدم) {
                    اكسر;
                }
                opart = opart->بل;
                oshm_zone = opart->elts;
                n = 0;
            }

            إذا (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                استمر;
            }

            إذا (ngx_strncmp(shm_zone[i].shm.name.معلومات,
                            oshm_zone[n].shm.name.معلومات,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                استمر;
            }

            إذا (shm_zone[i].نښان == oshm_zone[n].نښان
                && shm_zone[i].shm.اندازه == oshm_zone[n].shm.اندازه
                && !shm_zone[i].noreuse)
            {
                اذهب old_shm_zone_وموندل;
            }

            اكسر;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_وموندل:

        استمر;
    }

    إذا (ngx_test_config) {
        ngx_destroy_دوره_pools(&conf);
        أرجع عدم;
    }

    ls = دوره->listening.elts;
    لكل (i = 0; i < دوره->listening.nelts; i++) {
        إذا (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            استمر;
        }

        إذا (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لوګاریتم, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_متن);
        }
    }

    ngx_destroy_دوره_pools(&conf);

    أرجع عدم;
}


ساكن فراغ
ngx_destroy_دوره_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->لنډمهاله_pool);
    ngx_destroy_pool(conf->pool);
}


ساكن ngx_int_t
ngx_init_zone_pool(ngx_دوره_t *دوره, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    إذا (zn->shm.exists) {

        إذا (sp == sp->addr) {
            أرجع NGX_OK;
        }

#إذا (NGX_WIN32)

        /* remap at the required address */

        إذا (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            أرجع NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        إذا (sp == sp->addr) {
            أرجع NGX_OK;
        }

#د_که_پای

        ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        أرجع NGX_ERROR;
    }

    sp->پای_ټکی = zn->shm.addr + zn->shm.اندازه;
    sp->min_ښويدنه = 3;
    sp->addr = zn->shm.addr;

#إذا (NGX_HAVE_ATOMIC_OPS)

    file = عدم;

#وإلا

    file = ngx_pnalloc(دوره->pool,
                       دوره->قلف_file.len + zn->shm.name.len + 1);
    إذا (file == عدم) {
        أرجع NGX_ERROR;
    }

    (فراغ) ngx_sprintf(file, "%V%V%Z", &دوره->قلف_file, &zn->shm.name);

#د_که_پای

    إذا (ngx_shmtx_create(&sp->mutex, &sp->قلف, file) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    ngx_slab_init(sp);

    أرجع NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *لوګاریتم)
{
    اندازه_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    إذا (ngx_process > NGX_PROCESS_MASTER) {
        أرجع NGX_OK;
    }

    ngx_memzero(&file, حجم(ngx_file_t));

    file.name = *name;
    file.لوګاریتم = لوګاریتم;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.معلومات, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    إذا (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, لوګاریتم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.معلومات);
        أرجع NGX_ERROR;
    }

    rc = NGX_OK;

    إذا (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        إذا (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    إذا (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.معلومات);
    }

    أرجع rc;
}


فراغ
ngx_delete_pidfile(ngx_دوره_t *دوره)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(دوره->conf_ctx, ngx_core_module);

    name = ngx_new_دوګونی ? ccf->oldpid.معلومات : ccf->pid.معلومات;

    إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, دوره->لوګاریتم, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_دوره_t *دوره, حرف *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, دوره->لوګاریتم, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(دوره->conf_ctx, ngx_core_module);

    ngx_memzero(&file, حجم(ngx_file_t));

    file.name = ccf->pid;
    file.لوګاریتم = دوره->لوګاریتم;

    file.fd = ngx_open_file(file.name.معلومات, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    إذا (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, دوره->لوګاریتم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.معلومات);
        أرجع 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    إذا (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, دوره->لوګاریتم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.معلومات);
    }

    إذا (n == NGX_ERROR) {
        أرجع 1;
    }

    طالما (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    إذا (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, دوره->لوګاریتم, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.معلومات);
        أرجع 1;
    }

    أرجع ngx_os_signal_process(دوره, sig, pid);

}


ساكن ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *لوګاریتم)
{
#إذا !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    إذا (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, لوګاریتم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        أرجع NGX_ERROR;
    }

    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    إذا (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوګاریتم, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#د_که_پای

    أرجع NGX_OK;
}


فراغ
ngx_reopen_files(ngx_دوره_t *دوره, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &دوره->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بل == عدم) {
                اكسر;
            }
            part = part->بل;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].name.len == 0) {
            استمر;
        }

        إذا (file[i].flush) {
            file[i].flush(&file[i], دوره->لوګاریتم);
        }

        fd = ngx_open_file(file[i].name.معلومات, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, دوره->لوګاریتم, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.معلومات, file[i].fd, fd);

        إذا (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.معلومات);
            استمر;
        }

#إذا !(NGX_WIN32)
        إذا (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            إذا (ngx_file_info(file[i].name.معلومات, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.معلومات);

                إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.معلومات);
                }

                استمر;
            }

            إذا (fi.st_uid != user) {
                إذا (chown((ثابت حرف *) file[i].name.معلومات, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.معلومات, user);

                    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.معلومات);
                    }

                    استمر;
                }
            }

            إذا ((fi.st_ډېرپېښ & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_ډېرپېښ |= (S_IRUSR|S_IWUSR);

                إذا (chmod((ثابت حرف *) file[i].name.معلومات, fi.st_ډېرپېښ) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.معلومات);

                    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.معلومات);
                    }

                    استمر;
                }
            }
        }

        إذا (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.معلومات);

            إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.معلومات);
            }

            استمر;
        }
#د_که_پای

        إذا (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, دوره->لوګاریتم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.معلومات);
        }

        file[i].fd = fd;
    }

    (فراغ) ngx_log_redirect_stderr(دوره);
}


ngx_shm_zone_t *
ngx_shared_حافظه_جمع_کړه(ngx_conf_t *cf, ngx_str_t *name, اندازه_t اندازه, فراغ *نښان)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->دوره->shared_حافظه.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بل == عدم) {
                اكسر;
            }
            part = part->بل;
            shm_zone = part->elts;
            i = 0;
        }

        إذا (name->len != shm_zone[i].shm.name.len) {
            استمر;
        }

        إذا (ngx_strncmp(name->معلومات, shm_zone[i].shm.name.معلومات, name->len)
            != 0)
        {
            استمر;
        }

        إذا (نښان != shm_zone[i].نښان) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            أرجع عدم;
        }

        إذا (shm_zone[i].shm.اندازه == 0) {
            shm_zone[i].shm.اندازه = اندازه;
        }

        إذا (اندازه && اندازه != shm_zone[i].shm.اندازه) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            اندازه, &shm_zone[i].shm.name, shm_zone[i].shm.اندازه);
            أرجع عدم;
        }

        أرجع &shm_zone[i];
    }

    shm_zone = ngx_list_ټېل(&cf->دوره->shared_حافظه);

    إذا (shm_zone == عدم) {
        أرجع عدم;
    }

    shm_zone->معلومات = عدم;
    shm_zone->shm.لوګاریتم = cf->دوره->لوګاریتم;
    shm_zone->shm.addr = عدم;
    shm_zone->shm.اندازه = اندازه;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = عدم;
    shm_zone->نښان = نښان;
    shm_zone->noreuse = 0;

    أرجع shm_zone;
}


ساكن فراغ
ngx_clean_old_دورې(ngx_event_t *ev)
{
    ngx_uint_t     i, n, وموندل, ژوندی;
    ngx_log_t     *لوګاریتم;
    ngx_دوره_t  **دوره;

    لوګاریتم = ngx_دوره->لوګاریتم;
    ngx_لنډمهاله_pool->لوګاریتم = لوګاریتم;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, لوګاریتم, 0, "clean old cycles");

    ژوندی = 0;

    دوره = ngx_old_دورې.elts;
    لكل (i = 0; i < ngx_old_دورې.nelts; i++) {

        إذا (دوره[i] == عدم) {
            استمر;
        }

        وموندل = 0;

        لكل (n = 0; n < دوره[i]->connection_n; n++) {
            إذا (دوره[i]->connections[n].fd != (ngx_socket_t) -1) {
                وموندل = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوګاریتم, 0, "live fd:%ui", n);

                اكسر;
            }
        }

        إذا (وموندل) {
            ژوندی = 1;
            استمر;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوګاریتم, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(دوره[i]->pool);
        دوره[i] = عدم;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوګاریتم, 0, "old cycles status: %ui", ژوندی);

    إذا (ژوندی) {
        ngx_جمع_کړه_timer(ev, 30000);

    } وإلا {
        ngx_destroy_pool(ngx_لنډمهاله_pool);
        ngx_لنډمهاله_pool = عدم;
        ngx_old_دورې.nelts = 0;
    }
}


فراغ
ngx_سېټ_shutdown_timer(ngx_دوره_t *دوره)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(دوره->conf_ctx, ngx_core_module);

    إذا (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.معلومات = دوره;
        ngx_shutdown_event.لوګاریتم = دوره->لوګاریتم;
        ngx_shutdown_event.cancelable = 1;

        ngx_جمع_کړه_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


ساكن فراغ
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_دوره_t       *دوره;
    ngx_connection_t  *c;

    دوره = ev->معلومات;

    c = دوره->connections;

    لكل (i = 0; i < دوره->connection_n; i++) {

        إذا (c[i].fd == (ngx_socket_t) -1
            || c[i].اقرأ == عدم
            || c[i].اقرأ->accept
            || c[i].اقرأ->channel
            || c[i].اقرأ->resolver)
        {
            استمر;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->لوګاریتم, 0,
                       "*%uA shutdown timeout", c[i].شمېره);

        c[i].close = 1;
        c[i].error = 1;

        c[i].اقرأ->handler(c[i].اقرأ);
    }
}
