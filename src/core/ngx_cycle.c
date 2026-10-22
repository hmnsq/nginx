
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


ساكن فراغ ngx_destroy_چکر_pools(ngx_conf_t *conf);
ساكن ngx_int_t ngx_init_zone_pool(ngx_چکر_t *چکر,
    ngx_shm_zone_t *shm_zone);
ساكن ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *لوگارتھم);
ساكن فراغ ngx_clean_old_چکراں(ngx_event_t *ev);
ساكن فراغ ngx_shutdown_timer_handler(ngx_event_t *ev);


متطاير ngx_چکر_t  *ngx_چکر;
ngx_array_t            ngx_old_چکراں;

ساكن ngx_pool_t     *ngx_عارضی_pool;
ساكن ngx_event_t     ngx_cleaner_event;
ساكن ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_منوال;


/* STUB NAME */
ساكن ngx_connection_t  dumb;
/* STUB */


ngx_چکر_t *
ngx_init_چکر(ngx_چکر_t *old_چکر)
{
    فراغ                *rv, *انکڑے;
    حرف               **senv;
    ngx_uint_t           i, n;
    ngx_log_t           *لوگارتھم;
    ngx_ویلا_t          *tp;
    ngx_conf_t           conf;
    ngx_pool_t          *pool;
    ngx_چکر_t         *چکر, **old;
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

    ngx_ویلا_update();


    لوگارتھم = old_چکر->لوگارتھم;

    pool = ngx_create_pool(NGX_چکر_POOL_ناپ, لوگارتھم);
    إذا (pool == عدم) {
        أرجع عدم;
    }
    pool->لوگارتھم = لوگارتھم;

    چکر = ngx_pcalloc(pool, حجم(ngx_چکر_t));
    إذا (چکر == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    چکر->pool = pool;
    چکر->لوگارتھم = لوگارتھم;
    چکر->old_چکر = old_چکر;

    چکر->conf_اگیتر.len = old_چکر->conf_اگیتر.len;
    چکر->conf_اگیتر.انکڑے = ngx_pstrdup(pool, &old_چکر->conf_اگیتر);
    إذا (چکر->conf_اگیتر.انکڑے == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    چکر->اگیتر.len = old_چکر->اگیتر.len;
    چکر->اگیتر.انکڑے = ngx_pstrdup(pool, &old_چکر->اگیتر);
    إذا (چکر->اگیتر.انکڑے == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    چکر->error_log.len = old_چکر->error_log.len;
    چکر->error_log.انکڑے = ngx_pnalloc(pool, old_چکر->error_log.len + 1);
    إذا (چکر->error_log.انکڑے == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }
    ngx_cpystrn(چکر->error_log.انکڑے, old_چکر->error_log.انکڑے,
                old_چکر->error_log.len + 1);

    چکر->conf_file.len = old_چکر->conf_file.len;
    چکر->conf_file.انکڑے = ngx_pnalloc(pool, old_چکر->conf_file.len + 1);
    إذا (چکر->conf_file.انکڑے == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }
    ngx_cpystrn(چکر->conf_file.انکڑے, old_چکر->conf_file.انکڑے,
                old_چکر->conf_file.len + 1);

    إذا (old_چکر->conf_param.len) {
        چکر->conf_param.len = old_چکر->conf_param.len;
        چکر->conf_param.انکڑے = ngx_pstrdup(pool, &old_چکر->conf_param);
        إذا (چکر->conf_param.انکڑے == عدم) {
            ngx_destroy_pool(pool);
            أرجع عدم;
        }
    }


    n = old_چکر->paths.nelts ? old_چکر->paths.nelts : 10;

    إذا (ngx_array_init(&چکر->paths, pool, n, حجم(ngx_رستہ_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_memzero(چکر->paths.elts, n * حجم(ngx_رستہ_t *));


    إذا (ngx_array_init(&چکر->config_dump, pool, 1, حجم(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_rbtree_init(&چکر->config_dump_rbtree, &چکر->config_dump_sentinel,
                    ngx_str_rbtree_داخل_کرو_قدر);

    إذا (old_چکر->open_files.part.nelts) {
        n = old_چکر->open_files.part.nelts;
        لكل (part = old_چکر->open_files.part.اگلا; part; part = part->اگلا) {
            n += part->nelts;
        }

    } وإلا {
        n = 20;
    }

    إذا (ngx_list_init(&چکر->open_files, pool, n, حجم(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    إذا (old_چکر->shared_حافظہ.part.nelts) {
        n = old_چکر->shared_حافظہ.part.nelts;
        لكل (part = old_چکر->shared_حافظہ.part.اگلا; part; part = part->اگلا)
        {
            n += part->nelts;
        }

    } وإلا {
        n = 1;
    }

    إذا (ngx_list_init(&چکر->shared_حافظہ, pool, n, حجم(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    n = old_چکر->listening.nelts ? old_چکر->listening.nelts : 10;

    إذا (ngx_array_init(&چکر->listening, pool, n, حجم(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_memzero(چکر->listening.elts, n * حجم(ngx_listening_t));


    ngx_قطار_init(&چکر->reusable_connections_قطار);


    چکر->conf_ctx = ngx_pcalloc(pool, ngx_max_module * حجم(فراغ *));
    إذا (چکر->conf_ctx == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    إذا (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    چکر->hostname.len = ngx_strlen(hostname);

    چکر->hostname.انکڑے = ngx_pnalloc(pool, چکر->hostname.len);
    إذا (چکر->hostname.انکڑے == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_strlow(چکر->hostname.انکڑے, (u_char *) hostname, چکر->hostname.len);


    إذا (ngx_چکر_modules(چکر) != NGX_OK) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    لكل (i = 0; چکر->modules[i]; i++) {
        إذا (چکر->modules[i]->type != NGX_CORE_MODULE) {
            استمر;
        }

        module = چکر->modules[i]->ctx;

        إذا (module->create_conf) {
            rv = module->create_conf(چکر);
            إذا (rv == عدم) {
                ngx_destroy_pool(pool);
                أرجع عدم;
            }
            چکر->conf_ctx[چکر->modules[i]->اشاریہ] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, حجم(ngx_conf_t));
    /* STUB: init array ? */
    conf.دلیلاں = ngx_array_create(pool, 10, حجم(ngx_str_t));
    إذا (conf.دلیلاں == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    conf.عارضی_pool = ngx_create_pool(NGX_چکر_POOL_ناپ, لوگارتھم);
    إذا (conf.عارضی_pool == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    conf.ctx = چکر->conf_ctx;
    conf.چکر = چکر;
    conf.pool = pool;
    conf.لوگارتھم = لوگارتھم;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#إذا 0
    لوگارتھم->log_level = NGX_LOG_DEBUG_ALL;
#جے_انت

    إذا (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_چکر_pools(&conf);
        أرجع عدم;
    }

    إذا (ngx_conf_parse(&conf, &چکر->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_چکر_pools(&conf);
        أرجع عدم;
    }

    إذا (ngx_test_config && !ngx_quiet_منوال) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       چکر->conf_file.انکڑے);
    }

    لكل (i = 0; چکر->modules[i]; i++) {
        إذا (چکر->modules[i]->type != NGX_CORE_MODULE) {
            استمر;
        }

        module = چکر->modules[i]->ctx;

        إذا (module->init_conf) {
            إذا (module->init_conf(چکر,
                                  چکر->conf_ctx[چکر->modules[i]->اشاریہ])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_چکر_pools(&conf);
                أرجع عدم;
            }
        }
    }

    إذا (ngx_process == NGX_PROCESS_SIGNALLER) {
        أرجع چکر;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    إذا (ngx_test_config) {

        إذا (ngx_create_pidfile(&ccf->pid, لوگارتھم) != NGX_OK) {
            اذهب failed;
        }

    } وإلا إذا (!ngx_is_init_چکر(old_چکر)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_چکر->conf_ctx,
                                                   ngx_core_module);
        إذا (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.انکڑے, old_ccf->pid.انکڑے) != 0)
        {
            /* new pid file name */

            إذا (ngx_create_pidfile(&ccf->pid, لوگارتھم) != NGX_OK) {
                اذهب failed;
            }

            ngx_delete_pidfile(old_چکر);
        }
    }


    إذا (ngx_test_lockfile(چکر->جندرا_file.انکڑے, لوگارتھم) != NGX_OK) {
        اذهب failed;
    }


    إذا (ngx_create_paths(چکر, ccf->user) != NGX_OK) {
        اذهب failed;
    }


    إذا (ngx_log_open_default(چکر) != NGX_OK) {
        اذهب failed;
    }

    /* open the new files */

    part = &چکر->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->اگلا == عدم) {
                اكسر;
            }
            part = part->اگلا;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].name.len == 0) {
            استمر;
        }

        file[i].fd = ngx_open_file(file[i].name.انکڑے,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, لوگارتھم, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.انکڑے);

        إذا (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.انکڑے);
            اذهب failed;
        }

#إذا !(NGX_WIN32)
        إذا (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.انکڑے);
            اذهب failed;
        }
#جے_انت
    }

    چکر->لوگارتھم = &چکر->new_log;
    pool->لوگارتھم = &چکر->new_log;


    /* create shared memory */

    part = &چکر->shared_حافظہ.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->اگلا == عدم) {
                اكسر;
            }
            part = part->اگلا;
            shm_zone = part->elts;
            i = 0;
        }

        إذا (shm_zone[i].shm.ناپ == 0) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            اذهب failed;
        }

        shm_zone[i].shm.لوگارتھم = چکر->لوگارتھم;

        opart = &old_چکر->shared_حافظہ.part;
        oshm_zone = opart->elts;

        انکڑے = عدم;

        لكل (n = 0; /* void */ ; n++) {

            إذا (n >= opart->nelts) {
                إذا (opart->اگلا == عدم) {
                    اكسر;
                }
                opart = opart->اگلا;
                oshm_zone = opart->elts;
                n = 0;
            }

            إذا (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                استمر;
            }

            إذا (ngx_strncmp(shm_zone[i].shm.name.انکڑے,
                            oshm_zone[n].shm.name.انکڑے,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                استمر;
            }

            إذا (shm_zone[i].ٹیگ == oshm_zone[n].ٹیگ && shm_zone[i].noreuse) {
                انکڑے = oshm_zone[n].انکڑے;
                اكسر;
            }

            إذا (shm_zone[i].ٹیگ == oshm_zone[n].ٹیگ
                && shm_zone[i].shm.ناپ == oshm_zone[n].shm.ناپ)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#إذا (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#جے_انت

                إذا (shm_zone[i].init(&shm_zone[i], oshm_zone[n].انکڑے)
                    != NGX_OK)
                {
                    اذهب failed;
                }

                اذهب shm_zone_لبھیا;
            }

            اكسر;
        }

        إذا (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            اذهب failed;
        }

        إذا (ngx_init_zone_pool(چکر, &shm_zone[i]) != NGX_OK) {
            اذهب failed;
        }

        إذا (shm_zone[i].init(&shm_zone[i], انکڑے) != NGX_OK) {
            اذهب failed;
        }

    shm_zone_لبھیا:

        استمر;
    }


    /* handle the listening sockets */

    إذا (old_چکر->listening.nelts) {
        ls = old_چکر->listening.elts;
        لكل (i = 0; i < old_چکر->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = چکر->listening.elts;
        لكل (n = 0; n < چکر->listening.nelts; n++) {

            لكل (i = 0; i < old_چکر->listening.nelts; i++) {
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
                    nls[n].پچھلا = &ls[i];

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
                            nls[n].جوڑو_deferred = 1;
                        }

                    } وإلا إذا (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } وإلا إذا (nls[n].accept_filter) {
                        nls[n].جوڑو_deferred = 1;
                    }
#جے_انت

#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    إذا (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } وإلا إذا (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].جوڑو_deferred = 1;
                    }
#جے_انت

#إذا (NGX_HAVE_REUSEPORT)
                    إذا (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].جوڑو_reuseport = 1;
                    }
#جے_انت

                    اكسر;
                }
            }

            إذا (nls[n].fd == (ngx_socket_t) -1) {
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                إذا (nls[n].accept_filter) {
                    nls[n].جوڑو_deferred = 1;
                }
#جے_انت
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                إذا (nls[n].deferred_accept) {
                    nls[n].جوڑو_deferred = 1;
                }
#جے_انت
            }
        }

    } وإلا {
#إذا (NGX_HAVE_DEFERRED_ACCEPT)
        ls = چکر->listening.elts;
        لكل (i = 0; i < چکر->listening.nelts; i++) {
#جے_تعریف_شدہ SO_ACCEPTFILTER
            إذا (ls[i].accept_filter) {
                ls[i].جوڑو_deferred = 1;
            }
#جے_انت
#جے_تعریف_شدہ TCP_DEFER_ACCEPT
            إذا (ls[i].deferred_accept) {
                ls[i].جوڑو_deferred = 1;
            }
#جے_انت
        }
#جے_انت
    }

    إذا (ngx_open_listening_sockets(چکر) != NGX_OK) {
        اذهب failed;
    }

    إذا (!ngx_test_config) {
        ngx_configure_listening_sockets(چکر);
    }


    /* commit the new cycle configuration */

    إذا (!ngx_use_stderr) {
        (فراغ) ngx_log_redirect_stderr(چکر);
    }

    pool->لوگارتھم = چکر->لوگارتھم;

    إذا (ngx_init_modules(چکر) != NGX_OK) {
        /* fatal */
        باہر_نکلو(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_چکر->shared_حافظہ.part;
    oshm_zone = opart->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= opart->nelts) {
            إذا (opart->اگلا == عدم) {
                اذهب old_shm_zone_پورا;
            }
            opart = opart->اگلا;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &چکر->shared_حافظہ.part;
        shm_zone = part->elts;

        لكل (n = 0; /* void */ ; n++) {

            إذا (n >= part->nelts) {
                إذا (part->اگلا == عدم) {
                    اكسر;
                }
                part = part->اگلا;
                shm_zone = part->elts;
                n = 0;
            }

            إذا (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                استمر;
            }

            إذا (ngx_strncmp(oshm_zone[i].shm.name.انکڑے,
                            shm_zone[n].shm.name.انکڑے,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                استمر;
            }

            إذا (oshm_zone[i].ٹیگ == shm_zone[n].ٹیگ
                && oshm_zone[i].shm.ناپ == shm_zone[n].shm.ناپ
                && !oshm_zone[i].noreuse)
            {
                اذهب جیوندا_shm_zone;
            }

            اكسر;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    جیوندا_shm_zone:

        استمر;
    }

old_shm_zone_پورا:


    /* close the unnecessary listening sockets */

    ls = old_چکر->listening.elts;
    لكل (i = 0; i < old_چکر->listening.nelts; i++) {

        إذا (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            استمر;
        }

        إذا (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_متن);
        }

#إذا (NGX_HAVE_UNIX_DOMAIN)

        إذا (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_متن.انکڑے + حجم("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, چکر->لوگارتھم, 0,
                          "deleting socket %s", name);

            إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#جے_انت
    }


    /* close the unnecessary open files */

    part = &old_چکر->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->اگلا == عدم) {
                اكسر;
            }
            part = part->اگلا;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            استمر;
        }

        إذا (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.انکڑے);
        }
    }

    ngx_destroy_pool(conf.عارضی_pool);

    إذا (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_چکر(old_چکر)) {

        ngx_destroy_pool(old_چکر->pool);
        چکر->old_چکر = عدم;

        أرجع چکر;
    }


    إذا (ngx_عارضی_pool == عدم) {
        ngx_عارضی_pool = ngx_create_pool(128, چکر->لوگارتھم);
        إذا (ngx_عارضی_pool == عدم) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                          "could not create ngx_temp_pool");
            باہر_نکلو(1);
        }

        n = 10;

        إذا (ngx_array_init(&ngx_old_چکراں, ngx_عارضی_pool, n,
                           حجم(ngx_چکر_t *))
            != NGX_OK)
        {
            باہر_نکلو(1);
        }

        ngx_memzero(ngx_old_چکراں.elts, n * حجم(ngx_چکر_t *));

        ngx_cleaner_event.handler = ngx_clean_old_چکراں;
        ngx_cleaner_event.لوگارتھم = چکر->لوگارتھم;
        ngx_cleaner_event.انکڑے = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_عارضی_pool->لوگارتھم = چکر->لوگارتھم;

    old = ngx_array_ڈالو(&ngx_old_چکراں);
    إذا (old == عدم) {
        باہر_نکلو(1);
    }
    *old = old_چکر;

    إذا (!ngx_cleaner_event.timer_سیٹ) {
        ngx_جوڑو_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_سیٹ = 1;
    }

    أرجع چکر;


failed:

    إذا (!ngx_is_init_چکر(old_چکر)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_چکر->conf_ctx,
                                                   ngx_core_module);
        إذا (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &چکر->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->اگلا == عدم) {
                اكسر;
            }
            part = part->اگلا;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            استمر;
        }

        إذا (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.انکڑے);
        }
    }

    /* free the newly created shared memory */

    part = &چکر->shared_حافظہ.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->اگلا == عدم) {
                اكسر;
            }
            part = part->اگلا;
            shm_zone = part->elts;
            i = 0;
        }

        إذا (shm_zone[i].shm.addr == عدم) {
            استمر;
        }

        opart = &old_چکر->shared_حافظہ.part;
        oshm_zone = opart->elts;

        لكل (n = 0; /* void */ ; n++) {

            إذا (n >= opart->nelts) {
                إذا (opart->اگلا == عدم) {
                    اكسر;
                }
                opart = opart->اگلا;
                oshm_zone = opart->elts;
                n = 0;
            }

            إذا (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                استمر;
            }

            إذا (ngx_strncmp(shm_zone[i].shm.name.انکڑے,
                            oshm_zone[n].shm.name.انکڑے,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                استمر;
            }

            إذا (shm_zone[i].ٹیگ == oshm_zone[n].ٹیگ
                && shm_zone[i].shm.ناپ == oshm_zone[n].shm.ناپ
                && !shm_zone[i].noreuse)
            {
                اذهب old_shm_zone_لبھیا;
            }

            اكسر;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_لبھیا:

        استمر;
    }

    إذا (ngx_test_config) {
        ngx_destroy_چکر_pools(&conf);
        أرجع عدم;
    }

    ls = چکر->listening.elts;
    لكل (i = 0; i < چکر->listening.nelts; i++) {
        إذا (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            استمر;
        }

        إذا (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_متن);
        }
    }

    ngx_destroy_چکر_pools(&conf);

    أرجع عدم;
}


ساكن فراغ
ngx_destroy_چکر_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->عارضی_pool);
    ngx_destroy_pool(conf->pool);
}


ساكن ngx_int_t
ngx_init_zone_pool(ngx_چکر_t *چکر, ngx_shm_zone_t *zn)
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

#جے_انت

        ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        أرجع NGX_ERROR;
    }

    sp->سرا = zn->shm.addr + zn->shm.ناپ;
    sp->min_کھسکاؤ = 3;
    sp->addr = zn->shm.addr;

#إذا (NGX_HAVE_ATOMIC_OPS)

    file = عدم;

#وإلا

    file = ngx_pnalloc(چکر->pool,
                       چکر->جندرا_file.len + zn->shm.name.len + 1);
    إذا (file == عدم) {
        أرجع NGX_ERROR;
    }

    (فراغ) ngx_sprintf(file, "%V%V%Z", &چکر->جندرا_file, &zn->shm.name);

#جے_انت

    إذا (ngx_shmtx_create(&sp->mutex, &sp->جندرا, file) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    ngx_slab_init(sp);

    أرجع NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *لوگارتھم)
{
    ناپ_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    إذا (ngx_process > NGX_PROCESS_MASTER) {
        أرجع NGX_OK;
    }

    ngx_memzero(&file, حجم(ngx_file_t));

    file.name = *name;
    file.لوگارتھم = لوگارتھم;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.انکڑے, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    إذا (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.انکڑے);
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
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.انکڑے);
    }

    أرجع rc;
}


فراغ
ngx_delete_pidfile(ngx_چکر_t *چکر)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    name = ngx_new_ثنائی ? ccf->oldpid.انکڑے : ccf->pid.انکڑے;

    إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_چکر_t *چکر, حرف *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, چکر->لوگارتھم, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    ngx_memzero(&file, حجم(ngx_file_t));

    file.name = ccf->pid;
    file.لوگارتھم = چکر->لوگارتھم;

    file.fd = ngx_open_file(file.name.انکڑے, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    إذا (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, چکر->لوگارتھم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.انکڑے);
        أرجع 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    إذا (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.انکڑے);
    }

    إذا (n == NGX_ERROR) {
        أرجع 1;
    }

    طالما (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    إذا (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, چکر->لوگارتھم, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.انکڑے);
        أرجع 1;
    }

    أرجع ngx_os_signal_process(چکر, sig, pid);

}


ساكن ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *لوگارتھم)
{
#إذا !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    إذا (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        أرجع NGX_ERROR;
    }

    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    إذا (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#جے_انت

    أرجع NGX_OK;
}


فراغ
ngx_reopen_files(ngx_چکر_t *چکر, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &چکر->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->اگلا == عدم) {
                اكسر;
            }
            part = part->اگلا;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].name.len == 0) {
            استمر;
        }

        إذا (file[i].flush) {
            file[i].flush(&file[i], چکر->لوگارتھم);
        }

        fd = ngx_open_file(file[i].name.انکڑے, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, چکر->لوگارتھم, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.انکڑے, file[i].fd, fd);

        إذا (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.انکڑے);
            استمر;
        }

#إذا !(NGX_WIN32)
        إذا (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            إذا (ngx_file_info(file[i].name.انکڑے, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.انکڑے);

                إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.انکڑے);
                }

                استمر;
            }

            إذا (fi.st_uid != user) {
                إذا (chown((ثابت حرف *) file[i].name.انکڑے, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.انکڑے, user);

                    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.انکڑے);
                    }

                    استمر;
                }
            }

            إذا ((fi.st_منوال & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_منوال |= (S_IRUSR|S_IWUSR);

                إذا (chmod((ثابت حرف *) file[i].name.انکڑے, fi.st_منوال) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.انکڑے);

                    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.انکڑے);
                    }

                    استمر;
                }
            }
        }

        إذا (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.انکڑے);

            إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.انکڑے);
            }

            استمر;
        }
#جے_انت

        إذا (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.انکڑے);
        }

        file[i].fd = fd;
    }

    (فراغ) ngx_log_redirect_stderr(چکر);
}


ngx_shm_zone_t *
ngx_shared_حافظہ_جوڑو(ngx_conf_t *cf, ngx_str_t *name, ناپ_t ناپ, فراغ *ٹیگ)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->چکر->shared_حافظہ.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->اگلا == عدم) {
                اكسر;
            }
            part = part->اگلا;
            shm_zone = part->elts;
            i = 0;
        }

        إذا (name->len != shm_zone[i].shm.name.len) {
            استمر;
        }

        إذا (ngx_strncmp(name->انکڑے, shm_zone[i].shm.name.انکڑے, name->len)
            != 0)
        {
            استمر;
        }

        إذا (ٹیگ != shm_zone[i].ٹیگ) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            أرجع عدم;
        }

        إذا (shm_zone[i].shm.ناپ == 0) {
            shm_zone[i].shm.ناپ = ناپ;
        }

        إذا (ناپ && ناپ != shm_zone[i].shm.ناپ) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            ناپ, &shm_zone[i].shm.name, shm_zone[i].shm.ناپ);
            أرجع عدم;
        }

        أرجع &shm_zone[i];
    }

    shm_zone = ngx_list_ڈالو(&cf->چکر->shared_حافظہ);

    إذا (shm_zone == عدم) {
        أرجع عدم;
    }

    shm_zone->انکڑے = عدم;
    shm_zone->shm.لوگارتھم = cf->چکر->لوگارتھم;
    shm_zone->shm.addr = عدم;
    shm_zone->shm.ناپ = ناپ;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = عدم;
    shm_zone->ٹیگ = ٹیگ;
    shm_zone->noreuse = 0;

    أرجع shm_zone;
}


ساكن فراغ
ngx_clean_old_چکراں(ngx_event_t *ev)
{
    ngx_uint_t     i, n, لبھیا, جیوندا;
    ngx_log_t     *لوگارتھم;
    ngx_چکر_t  **چکر;

    لوگارتھم = ngx_چکر->لوگارتھم;
    ngx_عارضی_pool->لوگارتھم = لوگارتھم;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, لوگارتھم, 0, "clean old cycles");

    جیوندا = 0;

    چکر = ngx_old_چکراں.elts;
    لكل (i = 0; i < ngx_old_چکراں.nelts; i++) {

        إذا (چکر[i] == عدم) {
            استمر;
        }

        لبھیا = 0;

        لكل (n = 0; n < چکر[i]->connection_n; n++) {
            إذا (چکر[i]->connections[n].fd != (ngx_socket_t) -1) {
                لبھیا = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوگارتھم, 0, "live fd:%ui", n);

                اكسر;
            }
        }

        إذا (لبھیا) {
            جیوندا = 1;
            استمر;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوگارتھم, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(چکر[i]->pool);
        چکر[i] = عدم;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوگارتھم, 0, "old cycles status: %ui", جیوندا);

    إذا (جیوندا) {
        ngx_جوڑو_timer(ev, 30000);

    } وإلا {
        ngx_destroy_pool(ngx_عارضی_pool);
        ngx_عارضی_pool = عدم;
        ngx_old_چکراں.nelts = 0;
    }
}


فراغ
ngx_سیٹ_shutdown_timer(ngx_چکر_t *چکر)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    إذا (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.انکڑے = چکر;
        ngx_shutdown_event.لوگارتھم = چکر->لوگارتھم;
        ngx_shutdown_event.cancelable = 1;

        ngx_جوڑو_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


ساكن فراغ
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_چکر_t       *چکر;
    ngx_connection_t  *c;

    چکر = ev->انکڑے;

    c = چکر->connections;

    لكل (i = 0; i < چکر->connection_n; i++) {

        إذا (c[i].fd == (ngx_socket_t) -1
            || c[i].اقرأ == عدم
            || c[i].اقرأ->accept
            || c[i].اقرأ->channel
            || c[i].اقرأ->resolver)
        {
            استمر;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->لوگارتھم, 0,
                       "*%uA shutdown timeout", c[i].نمبر);

        c[i].close = 1;
        c[i].error = 1;

        c[i].اقرأ->handler(c[i].اقرأ);
    }
}
