
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


ساكن فراغ ngx_destroy_چرخه_pools(ngx_conf_t *conf);
ساكن ngx_int_t ngx_init_zone_pool(ngx_چرخه_t *چرخه,
    ngx_shm_zone_t *shm_zone);
ساكن ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *لگاریتم);
ساكن فراغ ngx_clean_old_چرخهها(ngx_event_t *ev);
ساكن فراغ ngx_shutdown_timer_handler(ngx_event_t *ev);


متطاير ngx_چرخه_t  *ngx_چرخه;
ngx_array_t            ngx_old_چرخهها;

ساكن ngx_pool_t     *ngx_موقت_pool;
ساكن ngx_event_t     ngx_cleaner_event;
ساكن ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_نما;


/* STUB NAME */
ساكن ngx_connection_t  dumb;
/* STUB */


ngx_چرخه_t *
ngx_init_چرخه(ngx_چرخه_t *old_چرخه)
{
    فراغ                *rv, *داده;
    حرف               **senv;
    ngx_uint_t           i, n;
    ngx_log_t           *لگاریتم;
    ngx_زمان_t          *tp;
    ngx_conf_t           conf;
    ngx_pool_t          *pool;
    ngx_چرخه_t         *چرخه, **old;
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

    ngx_زمان_update();


    لگاریتم = old_چرخه->لگاریتم;

    pool = ngx_create_pool(NGX_چرخه_POOL_اندازه, لگاریتم);
    إذا (pool == عدم) {
        أرجع عدم;
    }
    pool->لگاریتم = لگاریتم;

    چرخه = ngx_pcalloc(pool, حجم(ngx_چرخه_t));
    إذا (چرخه == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    چرخه->pool = pool;
    چرخه->لگاریتم = لگاریتم;
    چرخه->old_چرخه = old_چرخه;

    چرخه->conf_پیشوند.len = old_چرخه->conf_پیشوند.len;
    چرخه->conf_پیشوند.داده = ngx_pstrdup(pool, &old_چرخه->conf_پیشوند);
    إذا (چرخه->conf_پیشوند.داده == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    چرخه->پیشوند.len = old_چرخه->پیشوند.len;
    چرخه->پیشوند.داده = ngx_pstrdup(pool, &old_چرخه->پیشوند);
    إذا (چرخه->پیشوند.داده == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    چرخه->error_log.len = old_چرخه->error_log.len;
    چرخه->error_log.داده = ngx_pnalloc(pool, old_چرخه->error_log.len + 1);
    إذا (چرخه->error_log.داده == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }
    ngx_cpystrn(چرخه->error_log.داده, old_چرخه->error_log.داده,
                old_چرخه->error_log.len + 1);

    چرخه->conf_file.len = old_چرخه->conf_file.len;
    چرخه->conf_file.داده = ngx_pnalloc(pool, old_چرخه->conf_file.len + 1);
    إذا (چرخه->conf_file.داده == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }
    ngx_cpystrn(چرخه->conf_file.داده, old_چرخه->conf_file.داده,
                old_چرخه->conf_file.len + 1);

    إذا (old_چرخه->conf_param.len) {
        چرخه->conf_param.len = old_چرخه->conf_param.len;
        چرخه->conf_param.داده = ngx_pstrdup(pool, &old_چرخه->conf_param);
        إذا (چرخه->conf_param.داده == عدم) {
            ngx_destroy_pool(pool);
            أرجع عدم;
        }
    }


    n = old_چرخه->paths.nelts ? old_چرخه->paths.nelts : 10;

    إذا (ngx_array_init(&چرخه->paths, pool, n, حجم(ngx_مسیر_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_memzero(چرخه->paths.elts, n * حجم(ngx_مسیر_t *));


    إذا (ngx_array_init(&چرخه->config_dump, pool, 1, حجم(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_rbtree_init(&چرخه->config_dump_rbtree, &چرخه->config_dump_sentinel,
                    ngx_str_rbtree_درج_مقدار);

    إذا (old_چرخه->open_files.part.nelts) {
        n = old_چرخه->open_files.part.nelts;
        لكل (part = old_چرخه->open_files.part.بعدی; part; part = part->بعدی) {
            n += part->nelts;
        }

    } وإلا {
        n = 20;
    }

    إذا (ngx_list_init(&چرخه->open_files, pool, n, حجم(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    إذا (old_چرخه->shared_حافظه.part.nelts) {
        n = old_چرخه->shared_حافظه.part.nelts;
        لكل (part = old_چرخه->shared_حافظه.part.بعدی; part; part = part->بعدی)
        {
            n += part->nelts;
        }

    } وإلا {
        n = 1;
    }

    إذا (ngx_list_init(&چرخه->shared_حافظه, pool, n, حجم(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    n = old_چرخه->listening.nelts ? old_چرخه->listening.nelts : 10;

    إذا (ngx_array_init(&چرخه->listening, pool, n, حجم(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_memzero(چرخه->listening.elts, n * حجم(ngx_listening_t));


    ngx_صف_init(&چرخه->reusable_connections_صف);


    چرخه->conf_ctx = ngx_pcalloc(pool, ngx_max_module * حجم(فراغ *));
    إذا (چرخه->conf_ctx == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    إذا (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, لگاریتم, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    چرخه->hostname.len = ngx_strlen(hostname);

    چرخه->hostname.داده = ngx_pnalloc(pool, چرخه->hostname.len);
    إذا (چرخه->hostname.داده == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_strlow(چرخه->hostname.داده, (u_char *) hostname, چرخه->hostname.len);


    إذا (ngx_چرخه_modules(چرخه) != NGX_OK) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    لكل (i = 0; چرخه->modules[i]; i++) {
        إذا (چرخه->modules[i]->type != NGX_CORE_MODULE) {
            استمر;
        }

        module = چرخه->modules[i]->ctx;

        إذا (module->create_conf) {
            rv = module->create_conf(چرخه);
            إذا (rv == عدم) {
                ngx_destroy_pool(pool);
                أرجع عدم;
            }
            چرخه->conf_ctx[چرخه->modules[i]->شاخص] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, حجم(ngx_conf_t));
    /* STUB: init array ? */
    conf.آرگومانها = ngx_array_create(pool, 10, حجم(ngx_str_t));
    إذا (conf.آرگومانها == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    conf.موقت_pool = ngx_create_pool(NGX_چرخه_POOL_اندازه, لگاریتم);
    إذا (conf.موقت_pool == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    conf.ctx = چرخه->conf_ctx;
    conf.چرخه = چرخه;
    conf.pool = pool;
    conf.لگاریتم = لگاریتم;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#إذا 0
    لگاریتم->log_level = NGX_LOG_DEBUG_ALL;
#پایان_اگر

    إذا (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_چرخه_pools(&conf);
        أرجع عدم;
    }

    إذا (ngx_conf_parse(&conf, &چرخه->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_چرخه_pools(&conf);
        أرجع عدم;
    }

    إذا (ngx_test_config && !ngx_quiet_نما) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       چرخه->conf_file.داده);
    }

    لكل (i = 0; چرخه->modules[i]; i++) {
        إذا (چرخه->modules[i]->type != NGX_CORE_MODULE) {
            استمر;
        }

        module = چرخه->modules[i]->ctx;

        إذا (module->init_conf) {
            إذا (module->init_conf(چرخه,
                                  چرخه->conf_ctx[چرخه->modules[i]->شاخص])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_چرخه_pools(&conf);
                أرجع عدم;
            }
        }
    }

    إذا (ngx_process == NGX_PROCESS_SIGNALLER) {
        أرجع چرخه;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(چرخه->conf_ctx, ngx_core_module);

    إذا (ngx_test_config) {

        إذا (ngx_create_pidfile(&ccf->pid, لگاریتم) != NGX_OK) {
            اذهب failed;
        }

    } وإلا إذا (!ngx_is_init_چرخه(old_چرخه)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_چرخه->conf_ctx,
                                                   ngx_core_module);
        إذا (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.داده, old_ccf->pid.داده) != 0)
        {
            /* new pid file name */

            إذا (ngx_create_pidfile(&ccf->pid, لگاریتم) != NGX_OK) {
                اذهب failed;
            }

            ngx_delete_pidfile(old_چرخه);
        }
    }


    إذا (ngx_test_lockfile(چرخه->قفل_file.داده, لگاریتم) != NGX_OK) {
        اذهب failed;
    }


    إذا (ngx_create_paths(چرخه, ccf->user) != NGX_OK) {
        اذهب failed;
    }


    إذا (ngx_log_open_default(چرخه) != NGX_OK) {
        اذهب failed;
    }

    /* open the new files */

    part = &چرخه->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بعدی == عدم) {
                اكسر;
            }
            part = part->بعدی;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].name.len == 0) {
            استمر;
        }

        file[i].fd = ngx_open_file(file[i].name.داده,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, لگاریتم, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.داده);

        إذا (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, لگاریتم, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.داده);
            اذهب failed;
        }

#إذا !(NGX_WIN32)
        إذا (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لگاریتم, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.داده);
            اذهب failed;
        }
#پایان_اگر
    }

    چرخه->لگاریتم = &چرخه->new_log;
    pool->لگاریتم = &چرخه->new_log;


    /* create shared memory */

    part = &چرخه->shared_حافظه.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بعدی == عدم) {
                اكسر;
            }
            part = part->بعدی;
            shm_zone = part->elts;
            i = 0;
        }

        إذا (shm_zone[i].shm.اندازه == 0) {
            ngx_log_error(NGX_LOG_EMERG, لگاریتم, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            اذهب failed;
        }

        shm_zone[i].shm.لگاریتم = چرخه->لگاریتم;

        opart = &old_چرخه->shared_حافظه.part;
        oshm_zone = opart->elts;

        داده = عدم;

        لكل (n = 0; /* void */ ; n++) {

            إذا (n >= opart->nelts) {
                إذا (opart->بعدی == عدم) {
                    اكسر;
                }
                opart = opart->بعدی;
                oshm_zone = opart->elts;
                n = 0;
            }

            إذا (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                استمر;
            }

            إذا (ngx_strncmp(shm_zone[i].shm.name.داده,
                            oshm_zone[n].shm.name.داده,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                استمر;
            }

            إذا (shm_zone[i].برچسب == oshm_zone[n].برچسب && shm_zone[i].noreuse) {
                داده = oshm_zone[n].داده;
                اكسر;
            }

            إذا (shm_zone[i].برچسب == oshm_zone[n].برچسب
                && shm_zone[i].shm.اندازه == oshm_zone[n].shm.اندازه)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#إذا (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#پایان_اگر

                إذا (shm_zone[i].init(&shm_zone[i], oshm_zone[n].داده)
                    != NGX_OK)
                {
                    اذهب failed;
                }

                اذهب shm_zone_یافت;
            }

            اكسر;
        }

        إذا (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            اذهب failed;
        }

        إذا (ngx_init_zone_pool(چرخه, &shm_zone[i]) != NGX_OK) {
            اذهب failed;
        }

        إذا (shm_zone[i].init(&shm_zone[i], داده) != NGX_OK) {
            اذهب failed;
        }

    shm_zone_یافت:

        استمر;
    }


    /* handle the listening sockets */

    إذا (old_چرخه->listening.nelts) {
        ls = old_چرخه->listening.elts;
        لكل (i = 0; i < old_چرخه->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = چرخه->listening.elts;
        لكل (n = 0; n < چرخه->listening.nelts; n++) {

            لكل (i = 0; i < old_چرخه->listening.nelts; i++) {
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
                    nls[n].قبلی = &ls[i];

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
                            nls[n].جمع_کن_deferred = 1;
                        }

                    } وإلا إذا (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } وإلا إذا (nls[n].accept_filter) {
                        nls[n].جمع_کن_deferred = 1;
                    }
#پایان_اگر

#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    إذا (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } وإلا إذا (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].جمع_کن_deferred = 1;
                    }
#پایان_اگر

#إذا (NGX_HAVE_REUSEPORT)
                    إذا (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].جمع_کن_reuseport = 1;
                    }
#پایان_اگر

                    اكسر;
                }
            }

            إذا (nls[n].fd == (ngx_socket_t) -1) {
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                إذا (nls[n].accept_filter) {
                    nls[n].جمع_کن_deferred = 1;
                }
#پایان_اگر
#إذا (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                إذا (nls[n].deferred_accept) {
                    nls[n].جمع_کن_deferred = 1;
                }
#پایان_اگر
            }
        }

    } وإلا {
#إذا (NGX_HAVE_DEFERRED_ACCEPT)
        ls = چرخه->listening.elts;
        لكل (i = 0; i < چرخه->listening.nelts; i++) {
#اگر_تعریف_شده SO_ACCEPTFILTER
            إذا (ls[i].accept_filter) {
                ls[i].جمع_کن_deferred = 1;
            }
#پایان_اگر
#اگر_تعریف_شده TCP_DEFER_ACCEPT
            إذا (ls[i].deferred_accept) {
                ls[i].جمع_کن_deferred = 1;
            }
#پایان_اگر
        }
#پایان_اگر
    }

    إذا (ngx_open_listening_sockets(چرخه) != NGX_OK) {
        اذهب failed;
    }

    إذا (!ngx_test_config) {
        ngx_configure_listening_sockets(چرخه);
    }


    /* commit the new cycle configuration */

    إذا (!ngx_use_stderr) {
        (فراغ) ngx_log_redirect_stderr(چرخه);
    }

    pool->لگاریتم = چرخه->لگاریتم;

    إذا (ngx_init_modules(چرخه) != NGX_OK) {
        /* fatal */
        خروج(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_چرخه->shared_حافظه.part;
    oshm_zone = opart->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= opart->nelts) {
            إذا (opart->بعدی == عدم) {
                اذهب old_shm_zone_انجامشده;
            }
            opart = opart->بعدی;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &چرخه->shared_حافظه.part;
        shm_zone = part->elts;

        لكل (n = 0; /* void */ ; n++) {

            إذا (n >= part->nelts) {
                إذا (part->بعدی == عدم) {
                    اكسر;
                }
                part = part->بعدی;
                shm_zone = part->elts;
                n = 0;
            }

            إذا (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                استمر;
            }

            إذا (ngx_strncmp(oshm_zone[i].shm.name.داده,
                            shm_zone[n].shm.name.داده,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                استمر;
            }

            إذا (oshm_zone[i].برچسب == shm_zone[n].برچسب
                && oshm_zone[i].shm.اندازه == shm_zone[n].shm.اندازه
                && !oshm_zone[i].noreuse)
            {
                اذهب زنده_shm_zone;
            }

            اكسر;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    زنده_shm_zone:

        استمر;
    }

old_shm_zone_انجامشده:


    /* close the unnecessary listening sockets */

    ls = old_چرخه->listening.elts;
    لكل (i = 0; i < old_چرخه->listening.nelts; i++) {

        إذا (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            استمر;
        }

        إذا (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لگاریتم, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_متن);
        }

#إذا (NGX_HAVE_UNIX_DOMAIN)

        إذا (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_متن.داده + حجم("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, چرخه->لگاریتم, 0,
                          "deleting socket %s", name);

            إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#پایان_اگر
    }


    /* close the unnecessary open files */

    part = &old_چرخه->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بعدی == عدم) {
                اكسر;
            }
            part = part->بعدی;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            استمر;
        }

        إذا (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, لگاریتم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.داده);
        }
    }

    ngx_destroy_pool(conf.موقت_pool);

    إذا (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_چرخه(old_چرخه)) {

        ngx_destroy_pool(old_چرخه->pool);
        چرخه->old_چرخه = عدم;

        أرجع چرخه;
    }


    إذا (ngx_موقت_pool == عدم) {
        ngx_موقت_pool = ngx_create_pool(128, چرخه->لگاریتم);
        إذا (ngx_موقت_pool == عدم) {
            ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, 0,
                          "could not create ngx_temp_pool");
            خروج(1);
        }

        n = 10;

        إذا (ngx_array_init(&ngx_old_چرخهها, ngx_موقت_pool, n,
                           حجم(ngx_چرخه_t *))
            != NGX_OK)
        {
            خروج(1);
        }

        ngx_memzero(ngx_old_چرخهها.elts, n * حجم(ngx_چرخه_t *));

        ngx_cleaner_event.handler = ngx_clean_old_چرخهها;
        ngx_cleaner_event.لگاریتم = چرخه->لگاریتم;
        ngx_cleaner_event.داده = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_موقت_pool->لگاریتم = چرخه->لگاریتم;

    old = ngx_array_افزودن(&ngx_old_چرخهها);
    إذا (old == عدم) {
        خروج(1);
    }
    *old = old_چرخه;

    إذا (!ngx_cleaner_event.timer_مجموعه) {
        ngx_جمع_کن_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_مجموعه = 1;
    }

    أرجع چرخه;


failed:

    إذا (!ngx_is_init_چرخه(old_چرخه)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_چرخه->conf_ctx,
                                                   ngx_core_module);
        إذا (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &چرخه->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بعدی == عدم) {
                اكسر;
            }
            part = part->بعدی;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            استمر;
        }

        إذا (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, لگاریتم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.داده);
        }
    }

    /* free the newly created shared memory */

    part = &چرخه->shared_حافظه.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بعدی == عدم) {
                اكسر;
            }
            part = part->بعدی;
            shm_zone = part->elts;
            i = 0;
        }

        إذا (shm_zone[i].shm.addr == عدم) {
            استمر;
        }

        opart = &old_چرخه->shared_حافظه.part;
        oshm_zone = opart->elts;

        لكل (n = 0; /* void */ ; n++) {

            إذا (n >= opart->nelts) {
                إذا (opart->بعدی == عدم) {
                    اكسر;
                }
                opart = opart->بعدی;
                oshm_zone = opart->elts;
                n = 0;
            }

            إذا (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                استمر;
            }

            إذا (ngx_strncmp(shm_zone[i].shm.name.داده,
                            oshm_zone[n].shm.name.داده,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                استمر;
            }

            إذا (shm_zone[i].برچسب == oshm_zone[n].برچسب
                && shm_zone[i].shm.اندازه == oshm_zone[n].shm.اندازه
                && !shm_zone[i].noreuse)
            {
                اذهب old_shm_zone_یافت;
            }

            اكسر;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_یافت:

        استمر;
    }

    إذا (ngx_test_config) {
        ngx_destroy_چرخه_pools(&conf);
        أرجع عدم;
    }

    ls = چرخه->listening.elts;
    لكل (i = 0; i < چرخه->listening.nelts; i++) {
        إذا (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            استمر;
        }

        إذا (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لگاریتم, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_متن);
        }
    }

    ngx_destroy_چرخه_pools(&conf);

    أرجع عدم;
}


ساكن فراغ
ngx_destroy_چرخه_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->موقت_pool);
    ngx_destroy_pool(conf->pool);
}


ساكن ngx_int_t
ngx_init_zone_pool(ngx_چرخه_t *چرخه, ngx_shm_zone_t *zn)
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

#پایان_اگر

        ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        أرجع NGX_ERROR;
    }

    sp->انتها = zn->shm.addr + zn->shm.اندازه;
    sp->min_لغزش = 3;
    sp->addr = zn->shm.addr;

#إذا (NGX_HAVE_ATOMIC_OPS)

    file = عدم;

#وإلا

    file = ngx_pnalloc(چرخه->pool,
                       چرخه->قفل_file.len + zn->shm.name.len + 1);
    إذا (file == عدم) {
        أرجع NGX_ERROR;
    }

    (فراغ) ngx_sprintf(file, "%V%V%Z", &چرخه->قفل_file, &zn->shm.name);

#پایان_اگر

    إذا (ngx_shmtx_create(&sp->mutex, &sp->قفل, file) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    ngx_slab_init(sp);

    أرجع NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *لگاریتم)
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
    file.لگاریتم = لگاریتم;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.داده, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    إذا (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, لگاریتم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.داده);
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
        ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.داده);
    }

    أرجع rc;
}


فراغ
ngx_delete_pidfile(ngx_چرخه_t *چرخه)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چرخه->conf_ctx, ngx_core_module);

    name = ngx_new_دودویی ? ccf->oldpid.داده : ccf->pid.داده;

    إذا (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, چرخه->لگاریتم, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_چرخه_t *چرخه, حرف *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, چرخه->لگاریتم, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(چرخه->conf_ctx, ngx_core_module);

    ngx_memzero(&file, حجم(ngx_file_t));

    file.name = ccf->pid;
    file.لگاریتم = چرخه->لگاریتم;

    file.fd = ngx_open_file(file.name.داده, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    إذا (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, چرخه->لگاریتم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.داده);
        أرجع 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    إذا (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, چرخه->لگاریتم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.داده);
    }

    إذا (n == NGX_ERROR) {
        أرجع 1;
    }

    طالما (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    إذا (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, چرخه->لگاریتم, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.داده);
        أرجع 1;
    }

    أرجع ngx_os_signal_process(چرخه, sig, pid);

}


ساكن ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *لگاریتم)
{
#إذا !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    إذا (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, لگاریتم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        أرجع NGX_ERROR;
    }

    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    إذا (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لگاریتم, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#پایان_اگر

    أرجع NGX_OK;
}


فراغ
ngx_reopen_files(ngx_چرخه_t *چرخه, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &چرخه->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بعدی == عدم) {
                اكسر;
            }
            part = part->بعدی;
            file = part->elts;
            i = 0;
        }

        إذا (file[i].name.len == 0) {
            استمر;
        }

        إذا (file[i].flush) {
            file[i].flush(&file[i], چرخه->لگاریتم);
        }

        fd = ngx_open_file(file[i].name.داده, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, چرخه->لگاریتم, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.داده, file[i].fd, fd);

        إذا (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.داده);
            استمر;
        }

#إذا !(NGX_WIN32)
        إذا (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            إذا (ngx_file_info(file[i].name.داده, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.داده);

                إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.داده);
                }

                استمر;
            }

            إذا (fi.st_uid != user) {
                إذا (chown((ثابت حرف *) file[i].name.داده, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.داده, user);

                    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.داده);
                    }

                    استمر;
                }
            }

            إذا ((fi.st_نما & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_نما |= (S_IRUSR|S_IWUSR);

                إذا (chmod((ثابت حرف *) file[i].name.داده, fi.st_نما) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.داده);

                    إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.داده);
                    }

                    استمر;
                }
            }
        }

        إذا (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.داده);

            إذا (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.داده);
            }

            استمر;
        }
#پایان_اگر

        إذا (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, چرخه->لگاریتم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.داده);
        }

        file[i].fd = fd;
    }

    (فراغ) ngx_log_redirect_stderr(چرخه);
}


ngx_shm_zone_t *
ngx_shared_حافظه_جمع_کن(ngx_conf_t *cf, ngx_str_t *name, اندازه_t اندازه, فراغ *برچسب)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->چرخه->shared_حافظه.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        إذا (i >= part->nelts) {
            إذا (part->بعدی == عدم) {
                اكسر;
            }
            part = part->بعدی;
            shm_zone = part->elts;
            i = 0;
        }

        إذا (name->len != shm_zone[i].shm.name.len) {
            استمر;
        }

        إذا (ngx_strncmp(name->داده, shm_zone[i].shm.name.داده, name->len)
            != 0)
        {
            استمر;
        }

        إذا (برچسب != shm_zone[i].برچسب) {
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

    shm_zone = ngx_list_افزودن(&cf->چرخه->shared_حافظه);

    إذا (shm_zone == عدم) {
        أرجع عدم;
    }

    shm_zone->داده = عدم;
    shm_zone->shm.لگاریتم = cf->چرخه->لگاریتم;
    shm_zone->shm.addr = عدم;
    shm_zone->shm.اندازه = اندازه;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = عدم;
    shm_zone->برچسب = برچسب;
    shm_zone->noreuse = 0;

    أرجع shm_zone;
}


ساكن فراغ
ngx_clean_old_چرخهها(ngx_event_t *ev)
{
    ngx_uint_t     i, n, یافت, زنده;
    ngx_log_t     *لگاریتم;
    ngx_چرخه_t  **چرخه;

    لگاریتم = ngx_چرخه->لگاریتم;
    ngx_موقت_pool->لگاریتم = لگاریتم;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, لگاریتم, 0, "clean old cycles");

    زنده = 0;

    چرخه = ngx_old_چرخهها.elts;
    لكل (i = 0; i < ngx_old_چرخهها.nelts; i++) {

        إذا (چرخه[i] == عدم) {
            استمر;
        }

        یافت = 0;

        لكل (n = 0; n < چرخه[i]->connection_n; n++) {
            إذا (چرخه[i]->connections[n].fd != (ngx_socket_t) -1) {
                یافت = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, لگاریتم, 0, "live fd:%ui", n);

                اكسر;
            }
        }

        إذا (یافت) {
            زنده = 1;
            استمر;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لگاریتم, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(چرخه[i]->pool);
        چرخه[i] = عدم;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, لگاریتم, 0, "old cycles status: %ui", زنده);

    إذا (زنده) {
        ngx_جمع_کن_timer(ev, 30000);

    } وإلا {
        ngx_destroy_pool(ngx_موقت_pool);
        ngx_موقت_pool = عدم;
        ngx_old_چرخهها.nelts = 0;
    }
}


فراغ
ngx_مجموعه_shutdown_timer(ngx_چرخه_t *چرخه)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چرخه->conf_ctx, ngx_core_module);

    إذا (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.داده = چرخه;
        ngx_shutdown_event.لگاریتم = چرخه->لگاریتم;
        ngx_shutdown_event.cancelable = 1;

        ngx_جمع_کن_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


ساكن فراغ
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_چرخه_t       *چرخه;
    ngx_connection_t  *c;

    چرخه = ev->داده;

    c = چرخه->connections;

    لكل (i = 0; i < چرخه->connection_n; i++) {

        إذا (c[i].fd == (ngx_socket_t) -1
            || c[i].اقرأ == عدم
            || c[i].اقرأ->accept
            || c[i].اقرأ->channel
            || c[i].اقرأ->resolver)
        {
            استمر;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->لگاریتم, 0,
                       "*%uA shutdown timeout", c[i].عدد);

        c[i].close = 1;
        c[i].error = 1;

        c[i].اقرأ->handler(c[i].اقرأ);
    }
}
