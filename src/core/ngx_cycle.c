
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


ساكن فراغ ngx_destroy_چڪر_pools(ngx_conf_t *conf);
ساكن ngx_int_t ngx_init_zone_pool(ngx_چڪر_t *چڪر,
    ngx_shm_zone_t *shm_zone);
ساكن ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *لاگرٿم);
ساكن فراغ ngx_clean_old_چڪرن(ngx_event_t *ev);
ساكن فراغ ngx_shutdown_timer_handler(ngx_event_t *ev);


متطاير ngx_چڪر_t  *ngx_چڪر;
ngx_array_t            ngx_old_چڪرن;

ساكن ngx_pool_t     *ngx_عارضي_pool;
ساكن ngx_event_t     ngx_cleaner_event;
ساكن ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_گهڻائي;


/* STUB NAME */
ساكن ngx_connection_t  dumb;
/* STUB */


ngx_چڪر_t *
ngx_init_چڪر(ngx_چڪر_t *old_چڪر)
{
    فراغ                *rv, *معلومات;
    حرف               **senv;
    ngx_uint_t           i, n;
    ngx_log_t           *لاگرٿم;
    ngx_وقت_t          *tp;
    ngx_conf_t           conf;
    ngx_pool_t          *pool;
    ngx_چڪر_t         *چڪر, **old;
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

    ngx_وقت_update();


    لاگرٿم = old_چڪر->لاگرٿم;

    pool = ngx_create_pool(NGX_چڪر_POOL_سائيز, لاگرٿم);
    إذا (pool == عدم) {
        أرجع عدم;
    }
    pool->لاگرٿم = لاگرٿم;

    چڪر = ngx_pcalloc(pool, حجم(ngx_چڪر_t));
    إذا (چڪر == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    چڪر->pool = pool;
    چڪر->لاگرٿم = لاگرٿم;
    چڪر->old_چڪر = old_چڪر;

    چڪر->conf_اڳياڙي.len = old_چڪر->conf_اڳياڙي.len;
    چڪر->conf_اڳياڙي.معلومات = ngx_pstrdup(pool, &old_چڪر->conf_اڳياڙي);
    إذا (چڪر->conf_اڳياڙي.معلومات == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    چڪر->اڳياڙي.len = old_چڪر->اڳياڙي.len;
    چڪر->اڳياڙي.معلومات = ngx_pstrdup(pool, &old_چڪر->اڳياڙي);
    إذا (چڪر->اڳياڙي.معلومات == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    چڪر->error_log.len = old_چڪر->error_log.len;
    چڪر->error_log.معلومات = ngx_pnalloc(pool, old_چڪر->error_log.len + 1);
    إذا (چڪر->error_log.معلومات == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }
    ngx_cpystrn(چڪر->error_log.معلومات, old_چڪر->error_log.معلومات,
                old_چڪر->error_log.len + 1);

    چڪر->conf_file.len = old_چڪر->conf_file.len;
    چڪر->conf_file.معلومات = ngx_pnalloc(pool, old_چڪر->conf_file.len + 1);
    إذا (چڪر->conf_file.معلومات == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }
    ngx_cpystrn(چڪر->conf_file.معلومات, old_چڪر->conf_file.معلومات,
                old_چڪر->conf_file.len + 1);

    إذا (old_چڪر->conf_param.len) {
        چڪر->conf_param.len = old_چڪر->conf_param.len;
        چڪر->conf_param.معلومات = ngx_pstrdup(pool, &old_چڪر->conf_param);
        إذا (چڪر->conf_param.معلومات == عدم) {
            ngx_destroy_pool(pool);
            أرجع عدم;
        }
    }


    n = old_چڪر->paths.nelts ? old_چڪر->paths.nelts : 10;

    إذا (ngx_array_init(&چڪر->paths, pool, n, حجم(ngx_رستو_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_memzero(چڪر->paths.elts, n * حجم(ngx_رستو_t *));


    إذا (ngx_array_init(&چڪر->config_dump, pool, 1, حجم(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_rbtree_init(&چڪر->config_dump_rbtree, &چڪر->config_dump_sentinel,
                    ngx_str_rbtree_وجهو_قدر);

    إذا (old_چڪر->open_files.part.nelts) {
        n = old_چڪر->open_files.part.nelts;
        لكل (part = old_چڪر->open_files.part.ايندڙ; part; part = part->ايندڙ) {
            n += part->nelts;
        }

    } وإلا {
        n = 20;
    }

    إذا (ngx_list_init(&چڪر->open_files, pool, n, حجم(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    إذا (old_چڪر->shared_يادگيري.part.nelts) {
        n = old_چڪر->shared_يادگيري.part.nelts;
        لكل (part = old_چڪر->shared_يادگيري.part.ايندڙ; part; part = part->ايندڙ)
        {
            n += part->nelts;
        }

    } وإلا {
        n = 1;
    }

    إذا (ngx_list_init(&چڪر->shared_يادگيري, pool, n, حجم(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    n = old_چڪر->listening.nelts ? old_چڪر->listening.nelts : 10;

    إذا (ngx_array_init(&چڪر->listening, pool, n, حجم(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_memzero(چڪر->listening.elts, n * حجم(ngx_listening_t));


    ngx_قطار_init(&چڪر->reusable_connections_قطار);


    چڪر->conf_ctx = ngx_pcalloc(pool, ngx_max_module * حجم(فراغ *));
    إذا (چڪر->conf_ctx == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    إذا (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, لاگرٿم, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    چڪر->hostname.len = ngx_strlen(hostname);

    چڪر->hostname.معلومات = ngx_pnalloc(pool, چڪر->hostname.len);
    إذا (چڪر->hostname.معلومات == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_strlow(چڪر->hostname.معلومات, (u_char *) hostname, چڪر->hostname.len);


    إذا (ngx_چڪر_modules(چڪر) != NGX_OK) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    لكل (i = 0; چڪر->modules[i]; i++) {
        إذا (چڪر->modules[i]->type != NGX_CORE_MODULE) {
            استمر;
        }

        module = چڪر->modules[i]->ctx;

        إذا (module->create_conf) {
            rv = module->create_conf(چڪر);
            إذا (rv == عدم) {
                ngx_destroy_pool(pool);
                أرجع عدم;
            }
            چڪر->conf_ctx[چڪر->modules[i]->اشاريو] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, حجم(ngx_conf_t));
    /* STUB: init array ? */
    conf.دليلون = ngx_array_create(pool, 10, حجم(ngx_str_t));
    إذا (conf.دليلون == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    conf.عارضي_pool = ngx_create_pool(NGX_چڪر_POOL_سائيز, لاگرٿم);
    إذا (conf.عارضي_pool == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    conf.ctx = چڪر->conf_ctx;
    conf.چڪر = چڪر;
    conf.pool = pool;
    conf.لاگرٿم = لاگرٿم;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#إذا 0
    لاگرٿم->log_level = NGX_LOG_DEBUG_ALL;
#جيڪڏهن_پورو

    إذا (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_چڪر_pools(&conf);
        أرجع عدم;
    }

    إذا (ngx_conf_parse(&conf, &چڪر->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_چڪر_pools(&conf);
        أرجع عدم;
    }

    إذا (ngx_test_config && !ngx_quiet_گهڻائي) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       چڪر->conf_file.معلومات);
    }

    لكل (i = 0; چڪر->modules[i]; i++) {
        إذا (چڪر->modules[i]->type != NGX_CORE_MODULE) {
            استمر;
        }

        module = چڪر->modules[i]->ctx;

        إذا (module->init_conf) {
            إذا (module->init_conf(چڪر,
                                  چڪر->conf_ctx[چڪر->modules[i]->اشاريو])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_چڪر_pools(&conf);
                أرجع عدم;
            }
        }
    }

    إذا (ngx_process == NGX_PROCESS_SIGNALLER) {
        أرجع چڪر;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(چڪر->conf_ctx, ngx_core_module);

    إذا (ngx_test_config) {

        إذا (ngx_create_pidfile(&ccf->pid, لاگرٿم) != NGX_OK) {
            اذهب failed;
        }

    } وإلا إذا (!ngx_is_init_چڪر(old_چڪر)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_چڪر->conf_ctx,
                                                   ngx_core_module);
        إذا (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.معلومات, old_ccf->pid.معلومات) != 0)
        {
            /* new pid file name */

            إذا (ngx_create_pidfile(&ccf->pid, لاگرٿم) != NGX_OK) {
                اذهب failed;
            }

            ngx_delete_pidfile(old_چڪر);
        }
    }


    إذا (ngx_test_lockfile(چڪر->تالو_file.معلومات, لاگرٿم) != NGX_OK) {
        اذهب failed;
    }


    إذا (ngx_create_paths(چڪر, ccf->user) != NGX_OK) {
        اذهب failed;
    }


    إذا (ngx_log_open_default(چڪر) != NGX_OK) {
        اذهب failed;
    }

    /* open the new files */

    part = &چڪر->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->ايندڙ == عدم) {
                اكسر;
            }
            part = part->ايندڙ;
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

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, لاگرٿم, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.معلومات);

        إذا (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, لاگرٿم, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.معلومات);
            اذهب failed;
        }

#إذا !(NGX_WIN32)
        إذا (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لاگرٿم, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.معلومات);
            اذهب failed;
        }
#جيڪڏهن_پورو
    }

    چڪر->لاگرٿم = &چڪر->new_log;
    pool->لاگرٿم = &چڪر->new_log;


    /* create shared memory */

    part = &چڪر->shared_يادگيري.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->ايندڙ == عدم) {
                اكسر;
            }
            part = part->ايندڙ;
            shm_zone = part->elts;
            i = 0;
        }

        إذا (shm_zone[i].shm.سائيز == 0) {
            ngx_log_error(NGX_LOG_EMERG, لاگرٿم, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            اذهب failed;
        }

        shm_zone[i].shm.لاگرٿم = چڪر->لاگرٿم;

        opart = &old_چڪر->shared_يادگيري.part;
        oshm_zone = opart->elts;

        معلومات = عدم;

        لكل (n = 0; /* void */ ; n++) {

            إذا (n >= opart->nelts) {
                إذا (opart->ايندڙ == عدم) {
                    اكسر;
                }
                opart = opart->ايندڙ;
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

            إذا (shm_zone[i].ٽيگ == oshm_zone[n].ٽيگ && shm_zone[i].noreuse) {
                معلومات = oshm_zone[n].معلومات;
                اكسر;
            }

            إذا (shm_zone[i].ٽيگ == oshm_zone[n].ٽيگ
                && shm_zone[i].shm.سائيز == oshm_zone[n].shm.سائيز)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#إذا (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#جيڪڏهن_پورو

                إذا (shm_zone[i].init(&shm_zone[i], oshm_zone[n].معلومات)
                    != NGX_OK)
                {
                    اذهب failed;
                }

                اذهب shm_zone_مليو;
            }

            اكسر;
        }

        إذا (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            اذهب failed;
        }

        إذا (ngx_init_zone_pool(چڪر, &shm_zone[i]) != NGX_OK) {
            اذهب failed;
        }

        إذا (shm_zone[i].init(&shm_zone[i], معلومات) != NGX_OK) {
            اذهب failed;
        }

    shm_zone_مليو:

        استمر;
    }


    /* handle the listening sockets */

    إذا (old_چڪر->listening.nelts) {
        ls = old_چڪر->listening.elts;
        لكل (i = 0; i < old_چڪر->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = چڪر->listening.elts;
        لكل (n = 0; n < چڪر->listening.nelts; n++) {

            لكل (i = 0; i < old_چڪر->listening.nelts; i++) {
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
                    nls[n].اڳيون = &ls[i];

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
                            nls[n].جوڙيو_deferred = 1;
                        }

                    } وإلا إذا (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } وإلا إذا (nls[n].accept_filter) {
                        nls[n].جوڙيو_deferred = 1;
                    }
#جيڪڏهن_پورو

#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    إذا (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } وإلا إذا (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].جوڙيو_deferred = 1;
                    }
#جيڪڏهن_پورو

#إذا (NGX_HAVE_REUSEPORT)
                    إذا (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].جوڙيو_reuseport = 1;
                    }
#جيڪڏهن_پورو

                    اكسر;
                }
            }

            إذا (nls[n].fd == (ngx_socket_t) -1) {
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                إذا (nls[n].accept_filter) {
                    nls[n].جوڙيو_deferred = 1;
                }
#جيڪڏهن_پورو
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                إذا (nls[n].deferred_accept) {
                    nls[n].جوڙيو_deferred = 1;
                }
#جيڪڏهن_پورو
            }
        }

    } وإلا {
#إذا (NGX_HAVE_DEFERRED_ACCEPT)
        ls = چڪر->listening.elts;
        لكل (i = 0; i < چڪر->listening.nelts; i++) {
#جيڪڏهن_وصف_ٿيل SO_ACCEPTFILTER
            إذا (ls[i].accept_filter) {
                ls[i].جوڙيو_deferred = 1;
            }
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل TCP_DEFER_ACCEPT
            إذا (ls[i].deferred_accept) {
                ls[i].جوڙيو_deferred = 1;
            }
#جيڪڏهن_پورو
        }
#جيڪڏهن_پورو
    }

    إذا (ngx_open_listening_sockets(چڪر) != NGX_OK) {
        اذهب failed;
    }

    إذا (!ngx_test_config) {
        ngx_configure_listening_sockets(چڪر);
    }


    /* commit the new cycle configuration */

    إذا (!ngx_use_stderr) {
        (فراغ) ngx_log_redirect_stderr(چڪر);
    }

    pool->لاگرٿم = چڪر->لاگرٿم;

    إذا (ngx_init_modules(چڪر) != NGX_OK) {
        /* fatal */
        ٻاهر_نڪرو(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_چڪر->shared_يادگيري.part;
    oshm_zone = opart->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= opart->nelts) {
            إذا (opart->ايندڙ == عدم) {
                اذهب old_shm_zone_پورو;
            }
            opart = opart->ايندڙ;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &چڪر->shared_يادگيري.part;
        shm_zone = part->elts;

        لكل (n = 0; /* void */ ; n++) {

            إذا (n >= part->nelts) {
                إذا (part->ايندڙ == عدم) {
                    اكسر;
                }
                part = part->ايندڙ;
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

            إذا (oshm_zone[i].ٽيگ == shm_zone[n].ٽيگ
                && oshm_zone[i].shm.سائيز == shm_zone[n].shm.سائيز
                && !oshm_zone[i].noreuse)
            {
                اذهب جيئرو_shm_zone;
            }

            اكسر;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    جيئرو_shm_zone:

        استمر;
    }

old_shm_zone_پورو:


    /* close the unnecessary listening sockets */

    ls = old_چڪر->listening.elts;
    لكل (i = 0; i < old_چڪر->listening.nelts; i++) {

        إذا (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            استمر;
        }

        إذا (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لاگرٿم, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_متن);
        }

#إذا (NGX_HAVE_UNIX_DOMAIN)

        إذا (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_متن.معلومات + حجم("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, چڪر->لاگرٿم, 0,
                          "deleting socket %s", name);

            إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#جيڪڏهن_پورو
    }


    /* close the unnecessary open files */

    part = &old_چڪر->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->ايندڙ == عدم) {
                اكسر;
            }
            part = part->ايندڙ;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            استمر;
        }

        إذا (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, لاگرٿم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.معلومات);
        }
    }

    ngx_destroy_pool(conf.عارضي_pool);

    إذا (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_چڪر(old_چڪر)) {

        ngx_destroy_pool(old_چڪر->pool);
        چڪر->old_چڪر = عدم;

        أرجع چڪر;
    }


    إذا (ngx_عارضي_pool == عدم) {
        ngx_عارضي_pool = ngx_create_pool(128, چڪر->لاگرٿم);
        إذا (ngx_عارضي_pool == عدم) {
            ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, 0,
                          "could not create ngx_temp_pool");
            ٻاهر_نڪرو(1);
        }

        n = 10;

        إذا (ngx_array_init(&ngx_old_چڪرن, ngx_عارضي_pool, n,
                           حجم(ngx_چڪر_t *))
            != NGX_OK)
        {
            ٻاهر_نڪرو(1);
        }

        ngx_memzero(ngx_old_چڪرن.elts, n * حجم(ngx_چڪر_t *));

        ngx_cleaner_event.handler = ngx_clean_old_چڪرن;
        ngx_cleaner_event.لاگرٿم = چڪر->لاگرٿم;
        ngx_cleaner_event.معلومات = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_عارضي_pool->لاگرٿم = چڪر->لاگرٿم;

    old = ngx_array_ڌڪو(&ngx_old_چڪرن);
    إذا (old == عدم) {
        ٻاهر_نڪرو(1);
    }
    *old = old_چڪر;

    إذا (!ngx_cleaner_event.timer_سيٽ) {
        ngx_جوڙيو_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_سيٽ = 1;
    }

    أرجع چڪر;


failed:

    إذا (!ngx_is_init_چڪر(old_چڪر)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_چڪر->conf_ctx,
                                                   ngx_core_module);
        إذا (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &چڪر->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->ايندڙ == عدم) {
                اكسر;
            }
            part = part->ايندڙ;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            استمر;
        }

        إذا (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, لاگرٿم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.معلومات);
        }
    }

    /* free the newly created shared memory */

    part = &چڪر->shared_يادگيري.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->ايندڙ == عدم) {
                اكسر;
            }
            part = part->ايندڙ;
            shm_zone = part->elts;
            i = 0;
        }

        إذا (shm_zone[i].shm.addr == عدم) {
            استمر;
        }

        opart = &old_چڪر->shared_يادگيري.part;
        oshm_zone = opart->elts;

        لكل (n = 0; /* void */ ; n++) {

            إذا (n >= opart->nelts) {
                إذا (opart->ايندڙ == عدم) {
                    اكسر;
                }
                opart = opart->ايندڙ;
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

            إذا (shm_zone[i].ٽيگ == oshm_zone[n].ٽيگ
                && shm_zone[i].shm.سائيز == oshm_zone[n].shm.سائيز
                && !shm_zone[i].noreuse)
            {
                اذهب old_shm_zone_مليو;
            }

            اكسر;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_مليو:

        استمر;
    }

    إذا (ngx_test_config) {
        ngx_destroy_چڪر_pools(&conf);
        أرجع عدم;
    }

    ls = چڪر->listening.elts;
    لكل (i = 0; i < چڪر->listening.nelts; i++) {
        إذا (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            استمر;
        }

        إذا (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لاگرٿم, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_متن);
        }
    }

    ngx_destroy_چڪر_pools(&conf);

    أرجع عدم;
}


ساكن فراغ
ngx_destroy_چڪر_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->عارضي_pool);
    ngx_destroy_pool(conf->pool);
}


ساكن ngx_int_t
ngx_init_zone_pool(ngx_چڪر_t *چڪر, ngx_shm_zone_t *zn)
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

#جيڪڏهن_پورو

        ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        أرجع NGX_ERROR;
    }

    sp->پڇاڙي = zn->shm.addr + zn->shm.سائيز;
    sp->min_سرڪايو = 3;
    sp->addr = zn->shm.addr;

#إذا (NGX_HAVE_ATOMIC_OPS)

    file = عدم;

#وإلا

    file = ngx_pnalloc(چڪر->pool,
                       چڪر->تالو_file.len + zn->shm.name.len + 1);
    إذا (file == عدم) {
        أرجع NGX_ERROR;
    }

    (فراغ) ngx_sprintf(file, "%V%V%Z", &چڪر->تالو_file, &zn->shm.name);

#جيڪڏهن_پورو

    إذا (ngx_shmtx_create(&sp->mutex, &sp->تالو, file) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    ngx_slab_init(sp);

    أرجع NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *لاگرٿم)
{
    سائيز_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    إذا (ngx_process > NGX_PROCESS_MASTER) {
        أرجع NGX_OK;
    }

    ngx_memzero(&file, حجم(ngx_file_t));

    file.name = *name;
    file.لاگرٿم = لاگرٿم;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.معلومات, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    إذا (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, لاگرٿم, ngx_errno,
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
        ngx_log_error(NGX_LOG_ALERT, لاگرٿم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.معلومات);
    }

    أرجع rc;
}


فراغ
ngx_delete_pidfile(ngx_چڪر_t *چڪر)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چڪر->conf_ctx, ngx_core_module);

    name = ngx_new_ٻٽو ? ccf->oldpid.معلومات : ccf->pid.معلومات;

    إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, چڪر->لاگرٿم, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_چڪر_t *چڪر, حرف *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, چڪر->لاگرٿم, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(چڪر->conf_ctx, ngx_core_module);

    ngx_memzero(&file, حجم(ngx_file_t));

    file.name = ccf->pid;
    file.لاگرٿم = چڪر->لاگرٿم;

    file.fd = ngx_open_file(file.name.معلومات, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    إذا (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, چڪر->لاگرٿم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.معلومات);
        أرجع 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    إذا (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, چڪر->لاگرٿم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.معلومات);
    }

    إذا (n == NGX_ERROR) {
        أرجع 1;
    }

    طالما (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    إذا (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, چڪر->لاگرٿم, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.معلومات);
        أرجع 1;
    }

    أرجع ngx_os_signal_process(چڪر, sig, pid);

}


ساكن ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *لاگرٿم)
{
#إذا !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    إذا (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, لاگرٿم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        أرجع NGX_ERROR;
    }

    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لاگرٿم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    إذا (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لاگرٿم, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#جيڪڏهن_پورو

    أرجع NGX_OK;
}


فراغ
ngx_reopen_files(ngx_چڪر_t *چڪر, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &چڪر->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->ايندڙ == عدم) {
                اكسر;
            }
            part = part->ايندڙ;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].name.len == 0) {
            استمر;
        }

        إذا (file[i].flush) {
            file[i].flush(&file[i], چڪر->لاگرٿم);
        }

        fd = ngx_open_file(file[i].name.معلومات, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, چڪر->لاگرٿم, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.معلومات, file[i].fd, fd);

        إذا (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.معلومات);
            استمر;
        }

#إذا !(NGX_WIN32)
        إذا (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            إذا (ngx_file_info(file[i].name.معلومات, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.معلومات);

                إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.معلومات);
                }

                استمر;
            }

            إذا (fi.st_uid != user) {
                إذا (chown((ثابت حرف *) file[i].name.معلومات, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.معلومات, user);

                    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.معلومات);
                    }

                    استمر;
                }
            }

            إذا ((fi.st_گهڻائي & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_گهڻائي |= (S_IRUSR|S_IWUSR);

                إذا (chmod((ثابت حرف *) file[i].name.معلومات, fi.st_گهڻائي) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.معلومات);

                    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.معلومات);
                    }

                    استمر;
                }
            }
        }

        إذا (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.معلومات);

            إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.معلومات);
            }

            استمر;
        }
#جيڪڏهن_پورو

        إذا (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, چڪر->لاگرٿم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.معلومات);
        }

        file[i].fd = fd;
    }

    (فراغ) ngx_log_redirect_stderr(چڪر);
}


ngx_shm_zone_t *
ngx_shared_يادگيري_جوڙيو(ngx_conf_t *cf, ngx_str_t *name, سائيز_t سائيز, فراغ *ٽيگ)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->چڪر->shared_يادگيري.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->ايندڙ == عدم) {
                اكسر;
            }
            part = part->ايندڙ;
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

        إذا (ٽيگ != shm_zone[i].ٽيگ) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            أرجع عدم;
        }

        إذا (shm_zone[i].shm.سائيز == 0) {
            shm_zone[i].shm.سائيز = سائيز;
        }

        إذا (سائيز && سائيز != shm_zone[i].shm.سائيز) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            سائيز, &shm_zone[i].shm.name, shm_zone[i].shm.سائيز);
            أرجع عدم;
        }

        أرجع &shm_zone[i];
    }

    shm_zone = ngx_list_ڌڪو(&cf->چڪر->shared_يادگيري);

    إذا (shm_zone == عدم) {
        أرجع عدم;
    }

    shm_zone->معلومات = عدم;
    shm_zone->shm.لاگرٿم = cf->چڪر->لاگرٿم;
    shm_zone->shm.addr = عدم;
    shm_zone->shm.سائيز = سائيز;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = عدم;
    shm_zone->ٽيگ = ٽيگ;
    shm_zone->noreuse = 0;

    أرجع shm_zone;
}


ساكن فراغ
ngx_clean_old_چڪرن(ngx_event_t *ev)
{
    ngx_uint_t     i, n, مليو, جيئرو;
    ngx_log_t     *لاگرٿم;
    ngx_چڪر_t  **چڪر;

    لاگرٿم = ngx_چڪر->لاگرٿم;
    ngx_عارضي_pool->لاگرٿم = لاگرٿم;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, لاگرٿم, 0, "clean old cycles");

    جيئرو = 0;

    چڪر = ngx_old_چڪرن.elts;
    لكل (i = 0; i < ngx_old_چڪرن.nelts; i++) {

        إذا (چڪر[i] == عدم) {
            استمر;
        }

        مليو = 0;

        لكل (n = 0; n < چڪر[i]->connection_n; n++) {
            إذا (چڪر[i]->connections[n].fd != (ngx_socket_t) -1) {
                مليو = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, لاگرٿم, 0, "live fd:%ui", n);

                اكسر;
            }
        }

        إذا (مليو) {
            جيئرو = 1;
            استمر;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لاگرٿم, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(چڪر[i]->pool);
        چڪر[i] = عدم;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, لاگرٿم, 0, "old cycles status: %ui", جيئرو);

    إذا (جيئرو) {
        ngx_جوڙيو_timer(ev, 30000);

    } وإلا {
        ngx_destroy_pool(ngx_عارضي_pool);
        ngx_عارضي_pool = عدم;
        ngx_old_چڪرن.nelts = 0;
    }
}


فراغ
ngx_سيٽ_shutdown_timer(ngx_چڪر_t *چڪر)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چڪر->conf_ctx, ngx_core_module);

    إذا (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.معلومات = چڪر;
        ngx_shutdown_event.لاگرٿم = چڪر->لاگرٿم;
        ngx_shutdown_event.cancelable = 1;

        ngx_جوڙيو_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


ساكن فراغ
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_چڪر_t       *چڪر;
    ngx_connection_t  *c;

    چڪر = ev->معلومات;

    c = چڪر->connections;

    لكل (i = 0; i < چڪر->connection_n; i++) {

        إذا (c[i].fd == (ngx_socket_t) -1
            || c[i].اقرأ == عدم
            || c[i].اقرأ->accept
            || c[i].اقرأ->channel
            || c[i].اقرأ->resolver)
        {
            استمر;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->لاگرٿم, 0,
                       "*%uA shutdown timeout", c[i].عدد);

        c[i].close = 1;
        c[i].error = 1;

        c[i].اقرأ->handler(c[i].اقرأ);
    }
}
