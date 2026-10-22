
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_event.h>


స్థిర శూన్య ngx_destroy_చక్రం_pools(ngx_conf_t *conf);
స్థిర ngx_int_t ngx_init_zone_pool(ngx_చక్రం_t *చక్రం,
    ngx_shm_zone_t *shm_zone);
స్థిర ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *సంవర్గమానం);
స్థిర శూన్య ngx_clean_old_చక్రాలు(ngx_event_t *ev);
స్థిర శూన్య ngx_shutdown_timer_handler(ngx_event_t *ev);


అస్థిర ngx_చక్రం_t  *ngx_చక్రం;
ngx_array_t            ngx_old_చక్రాలు;

స్థిర ngx_pool_t     *ngx_తాత్కాలిక_pool;
స్థిర ngx_event_t     ngx_cleaner_event;
స్థిర ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_బహుళకం;


/* STUB NAME */
స్థిర ngx_connection_t  dumb;
/* STUB */


ngx_చక్రం_t *
ngx_init_చక్రం(ngx_చక్రం_t *old_చక్రం)
{
    శూన్య                *rv, *దత్తాంశం;
    అక్షరం               **senv;
    ngx_uint_t           i, n;
    ngx_log_t           *సంవర్గమానం;
    ngx_సమయం_t          *tp;
    ngx_conf_t           conf;
    ngx_pool_t          *pool;
    ngx_చక్రం_t         *చక్రం, **old;
    ngx_shm_zone_t      *shm_zone, *oshm_zone;
    ngx_list_part_t     *part, *opart;
    ngx_open_file_t     *file;
    ngx_listening_t     *ls, *nls;
    ngx_core_conf_t     *ccf, *old_ccf;
    ngx_core_module_t   *module;
    అక్షరం                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_సమయం_update();


    సంవర్గమానం = old_చక్రం->సంవర్గమానం;

    pool = ngx_create_pool(NGX_చక్రం_POOL_కొలత, సంవర్గమానం);
    ఒకవేళ (pool == ఖాళీ) {
        తిరిగి ఖాళీ;
    }
    pool->సంవర్గమానం = సంవర్గమానం;

    చక్రం = ngx_pcalloc(pool, పరిమాణం(ngx_చక్రం_t));
    ఒకవేళ (చక్రం == ఖాళీ) {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }

    చక్రం->pool = pool;
    చక్రం->సంవర్గమానం = సంవర్గమానం;
    చక్రం->old_చక్రం = old_చక్రం;

    చక్రం->conf_ఉపసర్గ.len = old_చక్రం->conf_ఉపసర్గ.len;
    చక్రం->conf_ఉపసర్గ.దత్తాంశం = ngx_pstrdup(pool, &old_చక్రం->conf_ఉపసర్గ);
    ఒకవేళ (చక్రం->conf_ఉపసర్గ.దత్తాంశం == ఖాళీ) {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }

    చక్రం->ఉపసర్గ.len = old_చక్రం->ఉపసర్గ.len;
    చక్రం->ఉపసర్గ.దత్తాంశం = ngx_pstrdup(pool, &old_చక్రం->ఉపసర్గ);
    ఒకవేళ (చక్రం->ఉపసర్గ.దత్తాంశం == ఖాళీ) {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }

    చక్రం->error_log.len = old_చక్రం->error_log.len;
    చక్రం->error_log.దత్తాంశం = ngx_pnalloc(pool, old_చక్రం->error_log.len + 1);
    ఒకవేళ (చక్రం->error_log.దత్తాంశం == ఖాళీ) {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }
    ngx_cpystrn(చక్రం->error_log.దత్తాంశం, old_చక్రం->error_log.దత్తాంశం,
                old_చక్రం->error_log.len + 1);

    చక్రం->conf_file.len = old_చక్రం->conf_file.len;
    చక్రం->conf_file.దత్తాంశం = ngx_pnalloc(pool, old_చక్రం->conf_file.len + 1);
    ఒకవేళ (చక్రం->conf_file.దత్తాంశం == ఖాళీ) {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }
    ngx_cpystrn(చక్రం->conf_file.దత్తాంశం, old_చక్రం->conf_file.దత్తాంశం,
                old_చక్రం->conf_file.len + 1);

    ఒకవేళ (old_చక్రం->conf_param.len) {
        చక్రం->conf_param.len = old_చక్రం->conf_param.len;
        చక్రం->conf_param.దత్తాంశం = ngx_pstrdup(pool, &old_చక్రం->conf_param);
        ఒకవేళ (చక్రం->conf_param.దత్తాంశం == ఖాళీ) {
            ngx_destroy_pool(pool);
            తిరిగి ఖాళీ;
        }
    }


    n = old_చక్రం->paths.nelts ? old_చక్రం->paths.nelts : 10;

    ఒకవేళ (ngx_array_init(&చక్రం->paths, pool, n, పరిమాణం(ngx_మార్గం_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }

    ngx_memzero(చక్రం->paths.elts, n * పరిమాణం(ngx_మార్గం_t *));


    ఒకవేళ (ngx_array_init(&చక్రం->config_dump, pool, 1, పరిమాణం(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }

    ngx_rbtree_init(&చక్రం->config_dump_rbtree, &చక్రం->config_dump_sentinel,
                    ngx_str_rbtree_చొప్పించు_విలువ);

    ఒకవేళ (old_చక్రం->open_files.part.nelts) {
        n = old_చక్రం->open_files.part.nelts;
        కొరకు (part = old_చక్రం->open_files.part.తదుపరి; part; part = part->తదుపరి) {
            n += part->nelts;
        }

    } లేకపోతే {
        n = 20;
    }

    ఒకవేళ (ngx_list_init(&చక్రం->open_files, pool, n, పరిమాణం(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }


    ఒకవేళ (old_చక్రం->shared_జ్ఞాపకం.part.nelts) {
        n = old_చక్రం->shared_జ్ఞాపకం.part.nelts;
        కొరకు (part = old_చక్రం->shared_జ్ఞాపకం.part.తదుపరి; part; part = part->తదుపరి)
        {
            n += part->nelts;
        }

    } లేకపోతే {
        n = 1;
    }

    ఒకవేళ (ngx_list_init(&చక్రం->shared_జ్ఞాపకం, pool, n, పరిమాణం(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }

    n = old_చక్రం->listening.nelts ? old_చక్రం->listening.nelts : 10;

    ఒకవేళ (ngx_array_init(&చక్రం->listening, pool, n, పరిమాణం(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }

    ngx_memzero(చక్రం->listening.elts, n * పరిమాణం(ngx_listening_t));


    ngx_వరుస_init(&చక్రం->reusable_connections_వరుస);


    చక్రం->conf_ctx = ngx_pcalloc(pool, ngx_max_module * పరిమాణం(శూన్య *));
    ఒకవేళ (చక్రం->conf_ctx == ఖాళీ) {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }


    ఒకవేళ (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    చక్రం->hostname.len = ngx_strlen(hostname);

    చక్రం->hostname.దత్తాంశం = ngx_pnalloc(pool, చక్రం->hostname.len);
    ఒకవేళ (చక్రం->hostname.దత్తాంశం == ఖాళీ) {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }

    ngx_strlow(చక్రం->hostname.దత్తాంశం, (u_char *) hostname, చక్రం->hostname.len);


    ఒకవేళ (ngx_చక్రం_modules(చక్రం) != NGX_OK) {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }


    కొరకు (i = 0; చక్రం->modules[i]; i++) {
        ఒకవేళ (చక్రం->modules[i]->type != NGX_CORE_MODULE) {
            కొనసాగు;
        }

        module = చక్రం->modules[i]->ctx;

        ఒకవేళ (module->create_conf) {
            rv = module->create_conf(చక్రం);
            ఒకవేళ (rv == ఖాళీ) {
                ngx_destroy_pool(pool);
                తిరిగి ఖాళీ;
            }
            చక్రం->conf_ctx[చక్రం->modules[i]->సూచీ] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, పరిమాణం(ngx_conf_t));
    /* STUB: init array ? */
    conf.వాదనలు = ngx_array_create(pool, 10, పరిమాణం(ngx_str_t));
    ఒకవేళ (conf.వాదనలు == ఖాళీ) {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }

    conf.తాత్కాలిక_pool = ngx_create_pool(NGX_చక్రం_POOL_కొలత, సంవర్గమానం);
    ఒకవేళ (conf.తాత్కాలిక_pool == ఖాళీ) {
        ngx_destroy_pool(pool);
        తిరిగి ఖాళీ;
    }


    conf.ctx = చక్రం->conf_ctx;
    conf.చక్రం = చక్రం;
    conf.pool = pool;
    conf.సంవర్గమానం = సంవర్గమానం;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#ఒకవేళ 0
    సంవర్గమానం->log_level = NGX_LOG_DEBUG_ALL;
#అయితే_ముగింపు

    ఒకవేళ (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_చక్రం_pools(&conf);
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (ngx_conf_parse(&conf, &చక్రం->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_చక్రం_pools(&conf);
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (ngx_test_config && !ngx_quiet_బహుళకం) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       చక్రం->conf_file.దత్తాంశం);
    }

    కొరకు (i = 0; చక్రం->modules[i]; i++) {
        ఒకవేళ (చక్రం->modules[i]->type != NGX_CORE_MODULE) {
            కొనసాగు;
        }

        module = చక్రం->modules[i]->ctx;

        ఒకవేళ (module->init_conf) {
            ఒకవేళ (module->init_conf(చక్రం,
                                  చక్రం->conf_ctx[చక్రం->modules[i]->సూచీ])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_చక్రం_pools(&conf);
                తిరిగి ఖాళీ;
            }
        }
    }

    ఒకవేళ (ngx_process == NGX_PROCESS_SIGNALLER) {
        తిరిగి చక్రం;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(చక్రం->conf_ctx, ngx_core_module);

    ఒకవేళ (ngx_test_config) {

        ఒకవేళ (ngx_create_pidfile(&ccf->pid, సంవర్గమానం) != NGX_OK) {
            వెళ్ళు failed;
        }

    } లేకపోతే ఒకవేళ (!ngx_is_init_చక్రం(old_చక్రం)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_చక్రం->conf_ctx,
                                                   ngx_core_module);
        ఒకవేళ (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.దత్తాంశం, old_ccf->pid.దత్తాంశం) != 0)
        {
            /* new pid file name */

            ఒకవేళ (ngx_create_pidfile(&ccf->pid, సంవర్గమానం) != NGX_OK) {
                వెళ్ళు failed;
            }

            ngx_delete_pidfile(old_చక్రం);
        }
    }


    ఒకవేళ (ngx_test_lockfile(చక్రం->తాళం_file.దత్తాంశం, సంవర్గమానం) != NGX_OK) {
        వెళ్ళు failed;
    }


    ఒకవేళ (ngx_create_paths(చక్రం, ccf->user) != NGX_OK) {
        వెళ్ళు failed;
    }


    ఒకవేళ (ngx_log_open_default(చక్రం) != NGX_OK) {
        వెళ్ళు failed;
    }

    /* open the new files */

    part = &చక్రం->open_files.part;
    file = part->elts;

    కొరకు (i = 0; /* void */ ; i++) {

        ఒకవేళ (i >= part->nelts) {
            ఒకవేళ (part->తదుపరి == ఖాళీ) {
                ఆపు;
            }
            part = part->తదుపరి;
            file = part->elts;
            i = 0;
        }

        ఒకవేళ (file[i].name.len == 0) {
            కొనసాగు;
        }

        file[i].fd = ngx_open_file(file[i].name.దత్తాంశం,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, సంవర్గమానం, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.దత్తాంశం);

        ఒకవేళ (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.దత్తాంశం);
            వెళ్ళు failed;
        }

#ఒకవేళ !(NGX_WIN32)
        ఒకవేళ (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.దత్తాంశం);
            వెళ్ళు failed;
        }
#అయితే_ముగింపు
    }

    చక్రం->సంవర్గమానం = &చక్రం->new_log;
    pool->సంవర్గమానం = &చక్రం->new_log;


    /* create shared memory */

    part = &చక్రం->shared_జ్ఞాపకం.part;
    shm_zone = part->elts;

    కొరకు (i = 0; /* void */ ; i++) {

        ఒకవేళ (i >= part->nelts) {
            ఒకవేళ (part->తదుపరి == ఖాళీ) {
                ఆపు;
            }
            part = part->తదుపరి;
            shm_zone = part->elts;
            i = 0;
        }

        ఒకవేళ (shm_zone[i].shm.కొలత == 0) {
            ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            వెళ్ళు failed;
        }

        shm_zone[i].shm.సంవర్గమానం = చక్రం->సంవర్గమానం;

        opart = &old_చక్రం->shared_జ్ఞాపకం.part;
        oshm_zone = opart->elts;

        దత్తాంశం = ఖాళీ;

        కొరకు (n = 0; /* void */ ; n++) {

            ఒకవేళ (n >= opart->nelts) {
                ఒకవేళ (opart->తదుపరి == ఖాళీ) {
                    ఆపు;
                }
                opart = opart->తదుపరి;
                oshm_zone = opart->elts;
                n = 0;
            }

            ఒకవేళ (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                కొనసాగు;
            }

            ఒకవేళ (ngx_strncmp(shm_zone[i].shm.name.దత్తాంశం,
                            oshm_zone[n].shm.name.దత్తాంశం,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                కొనసాగు;
            }

            ఒకవేళ (shm_zone[i].ట్యాగ్ == oshm_zone[n].ట్యాగ్ && shm_zone[i].noreuse) {
                దత్తాంశం = oshm_zone[n].దత్తాంశం;
                ఆపు;
            }

            ఒకవేళ (shm_zone[i].ట్యాగ్ == oshm_zone[n].ట్యాగ్
                && shm_zone[i].shm.కొలత == oshm_zone[n].shm.కొలత)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#ఒకవేళ (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#అయితే_ముగింపు

                ఒకవేళ (shm_zone[i].init(&shm_zone[i], oshm_zone[n].దత్తాంశం)
                    != NGX_OK)
                {
                    వెళ్ళు failed;
                }

                వెళ్ళు shm_zone_దొరికింది;
            }

            ఆపు;
        }

        ఒకవేళ (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            వెళ్ళు failed;
        }

        ఒకవేళ (ngx_init_zone_pool(చక్రం, &shm_zone[i]) != NGX_OK) {
            వెళ్ళు failed;
        }

        ఒకవేళ (shm_zone[i].init(&shm_zone[i], దత్తాంశం) != NGX_OK) {
            వెళ్ళు failed;
        }

    shm_zone_దొరికింది:

        కొనసాగు;
    }


    /* handle the listening sockets */

    ఒకవేళ (old_చక్రం->listening.nelts) {
        ls = old_చక్రం->listening.elts;
        కొరకు (i = 0; i < old_చక్రం->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = చక్రం->listening.elts;
        కొరకు (n = 0; n < చక్రం->listening.nelts; n++) {

            కొరకు (i = 0; i < old_చక్రం->listening.nelts; i++) {
                ఒకవేళ (ls[i].ignore) {
                    కొనసాగు;
                }

                ఒకవేళ (ls[i].remain) {
                    కొనసాగు;
                }

                ఒకవేళ (ls[i].type != nls[n].type) {
                    కొనసాగు;
                }

                ఒకవేళ (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].మునుపటి = &ls[i];

                    ఒకవేళ (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } లేకపోతే {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    ఒకవేళ (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    ఒకవేళ (ls[i].accept_filter && nls[n].accept_filter) {
                        ఒకవేళ (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].కూడు_deferred = 1;
                        }

                    } లేకపోతే ఒకవేళ (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } లేకపోతే ఒకవేళ (nls[n].accept_filter) {
                        nls[n].కూడు_deferred = 1;
                    }
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    ఒకవేళ (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } లేకపోతే ఒకవేళ (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].కూడు_deferred = 1;
                    }
#అయితే_ముగింపు

#ఒకవేళ (NGX_HAVE_REUSEPORT)
                    ఒకవేళ (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].కూడు_reuseport = 1;
                    }
#అయితే_ముగింపు

                    ఆపు;
                }
            }

            ఒకవేళ (nls[n].fd == (ngx_socket_t) -1) {
#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                ఒకవేళ (nls[n].accept_filter) {
                    nls[n].కూడు_deferred = 1;
                }
#అయితే_ముగింపు
#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                ఒకవేళ (nls[n].deferred_accept) {
                    nls[n].కూడు_deferred = 1;
                }
#అయితే_ముగింపు
            }
        }

    } లేకపోతే {
#ఒకవేళ (NGX_HAVE_DEFERRED_ACCEPT)
        ls = చక్రం->listening.elts;
        కొరకు (i = 0; i < చక్రం->listening.nelts; i++) {
#నిర్వచితమైతే SO_ACCEPTFILTER
            ఒకవేళ (ls[i].accept_filter) {
                ls[i].కూడు_deferred = 1;
            }
#అయితే_ముగింపు
#నిర్వచితమైతే TCP_DEFER_ACCEPT
            ఒకవేళ (ls[i].deferred_accept) {
                ls[i].కూడు_deferred = 1;
            }
#అయితే_ముగింపు
        }
#అయితే_ముగింపు
    }

    ఒకవేళ (ngx_open_listening_sockets(చక్రం) != NGX_OK) {
        వెళ్ళు failed;
    }

    ఒకవేళ (!ngx_test_config) {
        ngx_configure_listening_sockets(చక్రం);
    }


    /* commit the new cycle configuration */

    ఒకవేళ (!ngx_use_stderr) {
        (శూన్య) ngx_log_redirect_stderr(చక్రం);
    }

    pool->సంవర్గమానం = చక్రం->సంవర్గమానం;

    ఒకవేళ (ngx_init_modules(చక్రం) != NGX_OK) {
        /* fatal */
        నిష్క్రమించు(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_చక్రం->shared_జ్ఞాపకం.part;
    oshm_zone = opart->elts;

    కొరకు (i = 0; /* void */ ; i++) {

        ఒకవేళ (i >= opart->nelts) {
            ఒకవేళ (opart->తదుపరి == ఖాళీ) {
                వెళ్ళు old_shm_zone_పూర్తి;
            }
            opart = opart->తదుపరి;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &చక్రం->shared_జ్ఞాపకం.part;
        shm_zone = part->elts;

        కొరకు (n = 0; /* void */ ; n++) {

            ఒకవేళ (n >= part->nelts) {
                ఒకవేళ (part->తదుపరి == ఖాళీ) {
                    ఆపు;
                }
                part = part->తదుపరి;
                shm_zone = part->elts;
                n = 0;
            }

            ఒకవేళ (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                కొనసాగు;
            }

            ఒకవేళ (ngx_strncmp(oshm_zone[i].shm.name.దత్తాంశం,
                            shm_zone[n].shm.name.దత్తాంశం,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                కొనసాగు;
            }

            ఒకవేళ (oshm_zone[i].ట్యాగ్ == shm_zone[n].ట్యాగ్
                && oshm_zone[i].shm.కొలత == shm_zone[n].shm.కొలత
                && !oshm_zone[i].noreuse)
            {
                వెళ్ళు సజీవ_shm_zone;
            }

            ఆపు;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    సజీవ_shm_zone:

        కొనసాగు;
    }

old_shm_zone_పూర్తి:


    /* close the unnecessary listening sockets */

    ls = old_చక్రం->listening.elts;
    కొరకు (i = 0; i < old_చక్రం->listening.nelts; i++) {

        ఒకవేళ (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            కొనసాగు;
        }

        ఒకవేళ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_పాఠ్యం);
        }

#ఒకవేళ (NGX_HAVE_UNIX_DOMAIN)

        ఒకవేళ (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_పాఠ్యం.దత్తాంశం + పరిమాణం("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, చక్రం->సంవర్గమానం, 0,
                          "deleting socket %s", name);

            ఒకవేళ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#అయితే_ముగింపు
    }


    /* close the unnecessary open files */

    part = &old_చక్రం->open_files.part;
    file = part->elts;

    కొరకు (i = 0; /* void */ ; i++) {

        ఒకవేళ (i >= part->nelts) {
            ఒకవేళ (part->తదుపరి == ఖాళీ) {
                ఆపు;
            }
            part = part->తదుపరి;
            file = part->elts;
            i = 0;
        }

        ఒకవేళ (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            కొనసాగు;
        }

        ఒకవేళ (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.దత్తాంశం);
        }
    }

    ngx_destroy_pool(conf.తాత్కాలిక_pool);

    ఒకవేళ (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_చక్రం(old_చక్రం)) {

        ngx_destroy_pool(old_చక్రం->pool);
        చక్రం->old_చక్రం = ఖాళీ;

        తిరిగి చక్రం;
    }


    ఒకవేళ (ngx_తాత్కాలిక_pool == ఖాళీ) {
        ngx_తాత్కాలిక_pool = ngx_create_pool(128, చక్రం->సంవర్గమానం);
        ఒకవేళ (ngx_తాత్కాలిక_pool == ఖాళీ) {
            ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, 0,
                          "could not create ngx_temp_pool");
            నిష్క్రమించు(1);
        }

        n = 10;

        ఒకవేళ (ngx_array_init(&ngx_old_చక్రాలు, ngx_తాత్కాలిక_pool, n,
                           పరిమాణం(ngx_చక్రం_t *))
            != NGX_OK)
        {
            నిష్క్రమించు(1);
        }

        ngx_memzero(ngx_old_చక్రాలు.elts, n * పరిమాణం(ngx_చక్రం_t *));

        ngx_cleaner_event.handler = ngx_clean_old_చక్రాలు;
        ngx_cleaner_event.సంవర్గమానం = చక్రం->సంవర్గమానం;
        ngx_cleaner_event.దత్తాంశం = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_తాత్కాలిక_pool->సంవర్గమానం = చక్రం->సంవర్గమానం;

    old = ngx_array_నెట్టు(&ngx_old_చక్రాలు);
    ఒకవేళ (old == ఖాళీ) {
        నిష్క్రమించు(1);
    }
    *old = old_చక్రం;

    ఒకవేళ (!ngx_cleaner_event.timer_సమితి) {
        ngx_కూడు_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_సమితి = 1;
    }

    తిరిగి చక్రం;


failed:

    ఒకవేళ (!ngx_is_init_చక్రం(old_చక్రం)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_చక్రం->conf_ctx,
                                                   ngx_core_module);
        ఒకవేళ (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &చక్రం->open_files.part;
    file = part->elts;

    కొరకు (i = 0; /* void */ ; i++) {

        ఒకవేళ (i >= part->nelts) {
            ఒకవేళ (part->తదుపరి == ఖాళీ) {
                ఆపు;
            }
            part = part->తదుపరి;
            file = part->elts;
            i = 0;
        }

        ఒకవేళ (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            కొనసాగు;
        }

        ఒకవేళ (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.దత్తాంశం);
        }
    }

    /* free the newly created shared memory */

    part = &చక్రం->shared_జ్ఞాపకం.part;
    shm_zone = part->elts;

    కొరకు (i = 0; /* void */ ; i++) {

        ఒకవేళ (i >= part->nelts) {
            ఒకవేళ (part->తదుపరి == ఖాళీ) {
                ఆపు;
            }
            part = part->తదుపరి;
            shm_zone = part->elts;
            i = 0;
        }

        ఒకవేళ (shm_zone[i].shm.addr == ఖాళీ) {
            కొనసాగు;
        }

        opart = &old_చక్రం->shared_జ్ఞాపకం.part;
        oshm_zone = opart->elts;

        కొరకు (n = 0; /* void */ ; n++) {

            ఒకవేళ (n >= opart->nelts) {
                ఒకవేళ (opart->తదుపరి == ఖాళీ) {
                    ఆపు;
                }
                opart = opart->తదుపరి;
                oshm_zone = opart->elts;
                n = 0;
            }

            ఒకవేళ (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                కొనసాగు;
            }

            ఒకవేళ (ngx_strncmp(shm_zone[i].shm.name.దత్తాంశం,
                            oshm_zone[n].shm.name.దత్తాంశం,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                కొనసాగు;
            }

            ఒకవేళ (shm_zone[i].ట్యాగ్ == oshm_zone[n].ట్యాగ్
                && shm_zone[i].shm.కొలత == oshm_zone[n].shm.కొలత
                && !shm_zone[i].noreuse)
            {
                వెళ్ళు old_shm_zone_దొరికింది;
            }

            ఆపు;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_దొరికింది:

        కొనసాగు;
    }

    ఒకవేళ (ngx_test_config) {
        ngx_destroy_చక్రం_pools(&conf);
        తిరిగి ఖాళీ;
    }

    ls = చక్రం->listening.elts;
    కొరకు (i = 0; i < చక్రం->listening.nelts; i++) {
        ఒకవేళ (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            కొనసాగు;
        }

        ఒకవేళ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_పాఠ్యం);
        }
    }

    ngx_destroy_చక్రం_pools(&conf);

    తిరిగి ఖాళీ;
}


స్థిర శూన్య
ngx_destroy_చక్రం_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->తాత్కాలిక_pool);
    ngx_destroy_pool(conf->pool);
}


స్థిర ngx_int_t
ngx_init_zone_pool(ngx_చక్రం_t *చక్రం, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    ఒకవేళ (zn->shm.exists) {

        ఒకవేళ (sp == sp->addr) {
            తిరిగి NGX_OK;
        }

#ఒకవేళ (NGX_WIN32)

        /* remap at the required address */

        ఒకవేళ (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            తిరిగి NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        ఒకవేళ (sp == sp->addr) {
            తిరిగి NGX_OK;
        }

#అయితే_ముగింపు

        ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        తిరిగి NGX_ERROR;
    }

    sp->చివర = zn->shm.addr + zn->shm.కొలత;
    sp->min_జరుపు = 3;
    sp->addr = zn->shm.addr;

#ఒకవేళ (NGX_HAVE_ATOMIC_OPS)

    file = ఖాళీ;

#లేకపోతే

    file = ngx_pnalloc(చక్రం->pool,
                       చక్రం->తాళం_file.len + zn->shm.name.len + 1);
    ఒకవేళ (file == ఖాళీ) {
        తిరిగి NGX_ERROR;
    }

    (శూన్య) ngx_sprintf(file, "%V%V%Z", &చక్రం->తాళం_file, &zn->shm.name);

#అయితే_ముగింపు

    ఒకవేళ (ngx_shmtx_create(&sp->mutex, &sp->తాళం, file) != NGX_OK) {
        తిరిగి NGX_ERROR;
    }

    ngx_slab_init(sp);

    తిరిగి NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *సంవర్గమానం)
{
    కొలత_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    ఒకవేళ (ngx_process > NGX_PROCESS_MASTER) {
        తిరిగి NGX_OK;
    }

    ngx_memzero(&file, పరిమాణం(ngx_file_t));

    file.name = *name;
    file.సంవర్గమానం = సంవర్గమానం;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.దత్తాంశం, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    ఒకవేళ (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.దత్తాంశం);
        తిరిగి NGX_ERROR;
    }

    rc = NGX_OK;

    ఒకవేళ (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        ఒకవేళ (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    ఒకవేళ (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.దత్తాంశం);
    }

    తిరిగి rc;
}


శూన్య
ngx_delete_pidfile(ngx_చక్రం_t *చక్రం)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(చక్రం->conf_ctx, ngx_core_module);

    name = ngx_new_ద్విమాన ? ccf->oldpid.దత్తాంశం : ccf->pid.దత్తాంశం;

    ఒకవేళ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_చక్రం_t *చక్రం, అక్షరం *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, చక్రం->సంవర్గమానం, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(చక్రం->conf_ctx, ngx_core_module);

    ngx_memzero(&file, పరిమాణం(ngx_file_t));

    file.name = ccf->pid;
    file.సంవర్గమానం = చక్రం->సంవర్గమానం;

    file.fd = ngx_open_file(file.name.దత్తాంశం, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    ఒకవేళ (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, చక్రం->సంవర్గమానం, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.దత్తాంశం);
        తిరిగి 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    ఒకవేళ (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, చక్రం->సంవర్గమానం, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.దత్తాంశం);
    }

    ఒకవేళ (n == NGX_ERROR) {
        తిరిగి 1;
    }

    అయితే (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    ఒకవేళ (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, చక్రం->సంవర్గమానం, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.దత్తాంశం);
        తిరిగి 1;
    }

    తిరిగి ngx_os_signal_process(చక్రం, sig, pid);

}


స్థిర ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *సంవర్గమానం)
{
#ఒకవేళ !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    ఒకవేళ (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, సంవర్గమానం, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    ఒకవేళ (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, సంవర్గమానం, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#అయితే_ముగింపు

    తిరిగి NGX_OK;
}


శూన్య
ngx_reopen_files(ngx_చక్రం_t *చక్రం, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &చక్రం->open_files.part;
    file = part->elts;

    కొరకు (i = 0; /* void */ ; i++) {

        ఒకవేళ (i >= part->nelts) {
            ఒకవేళ (part->తదుపరి == ఖాళీ) {
                ఆపు;
            }
            part = part->తదుపరి;
            file = part->elts;
            i = 0;
        }

        ఒకవేళ (file[i].name.len == 0) {
            కొనసాగు;
        }

        ఒకవేళ (file[i].flush) {
            file[i].flush(&file[i], చక్రం->సంవర్గమానం);
        }

        fd = ngx_open_file(file[i].name.దత్తాంశం, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, చక్రం->సంవర్గమానం, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.దత్తాంశం, file[i].fd, fd);

        ఒకవేళ (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.దత్తాంశం);
            కొనసాగు;
        }

#ఒకవేళ !(NGX_WIN32)
        ఒకవేళ (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            ఒకవేళ (ngx_file_info(file[i].name.దత్తాంశం, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.దత్తాంశం);

                ఒకవేళ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.దత్తాంశం);
                }

                కొనసాగు;
            }

            ఒకవేళ (fi.st_uid != user) {
                ఒకవేళ (chown((స్థిరరాశి అక్షరం *) file[i].name.దత్తాంశం, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.దత్తాంశం, user);

                    ఒకవేళ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.దత్తాంశం);
                    }

                    కొనసాగు;
                }
            }

            ఒకవేళ ((fi.st_బహుళకం & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_బహుళకం |= (S_IRUSR|S_IWUSR);

                ఒకవేళ (chmod((స్థిరరాశి అక్షరం *) file[i].name.దత్తాంశం, fi.st_బహుళకం) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.దత్తాంశం);

                    ఒకవేళ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.దత్తాంశం);
                    }

                    కొనసాగు;
                }
            }
        }

        ఒకవేళ (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.దత్తాంశం);

            ఒకవేళ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.దత్తాంశం);
            }

            కొనసాగు;
        }
#అయితే_ముగింపు

        ఒకవేళ (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, చక్రం->సంవర్గమానం, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.దత్తాంశం);
        }

        file[i].fd = fd;
    }

    (శూన్య) ngx_log_redirect_stderr(చక్రం);
}


ngx_shm_zone_t *
ngx_shared_జ్ఞాపకం_కూడు(ngx_conf_t *cf, ngx_str_t *name, కొలత_t కొలత, శూన్య *ట్యాగ్)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->చక్రం->shared_జ్ఞాపకం.part;
    shm_zone = part->elts;

    కొరకు (i = 0; /* void */ ; i++) {

        ఒకవేళ (i >= part->nelts) {
            ఒకవేళ (part->తదుపరి == ఖాళీ) {
                ఆపు;
            }
            part = part->తదుపరి;
            shm_zone = part->elts;
            i = 0;
        }

        ఒకవేళ (name->len != shm_zone[i].shm.name.len) {
            కొనసాగు;
        }

        ఒకవేళ (ngx_strncmp(name->దత్తాంశం, shm_zone[i].shm.name.దత్తాంశం, name->len)
            != 0)
        {
            కొనసాగు;
        }

        ఒకవేళ (ట్యాగ్ != shm_zone[i].ట్యాగ్) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            తిరిగి ఖాళీ;
        }

        ఒకవేళ (shm_zone[i].shm.కొలత == 0) {
            shm_zone[i].shm.కొలత = కొలత;
        }

        ఒకవేళ (కొలత && కొలత != shm_zone[i].shm.కొలత) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            కొలత, &shm_zone[i].shm.name, shm_zone[i].shm.కొలత);
            తిరిగి ఖాళీ;
        }

        తిరిగి &shm_zone[i];
    }

    shm_zone = ngx_list_నెట్టు(&cf->చక్రం->shared_జ్ఞాపకం);

    ఒకవేళ (shm_zone == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    shm_zone->దత్తాంశం = ఖాళీ;
    shm_zone->shm.సంవర్గమానం = cf->చక్రం->సంవర్గమానం;
    shm_zone->shm.addr = ఖాళీ;
    shm_zone->shm.కొలత = కొలత;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = ఖాళీ;
    shm_zone->ట్యాగ్ = ట్యాగ్;
    shm_zone->noreuse = 0;

    తిరిగి shm_zone;
}


స్థిర శూన్య
ngx_clean_old_చక్రాలు(ngx_event_t *ev)
{
    ngx_uint_t     i, n, దొరికింది, సజీవ;
    ngx_log_t     *సంవర్గమానం;
    ngx_చక్రం_t  **చక్రం;

    సంవర్గమానం = ngx_చక్రం->సంవర్గమానం;
    ngx_తాత్కాలిక_pool->సంవర్గమానం = సంవర్గమానం;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, సంవర్గమానం, 0, "clean old cycles");

    సజీవ = 0;

    చక్రం = ngx_old_చక్రాలు.elts;
    కొరకు (i = 0; i < ngx_old_చక్రాలు.nelts; i++) {

        ఒకవేళ (చక్రం[i] == ఖాళీ) {
            కొనసాగు;
        }

        దొరికింది = 0;

        కొరకు (n = 0; n < చక్రం[i]->connection_n; n++) {
            ఒకవేళ (చక్రం[i]->connections[n].fd != (ngx_socket_t) -1) {
                దొరికింది = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, సంవర్గమానం, 0, "live fd:%ui", n);

                ఆపు;
            }
        }

        ఒకవేళ (దొరికింది) {
            సజీవ = 1;
            కొనసాగు;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, సంవర్గమానం, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(చక్రం[i]->pool);
        చక్రం[i] = ఖాళీ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, సంవర్గమానం, 0, "old cycles status: %ui", సజీవ);

    ఒకవేళ (సజీవ) {
        ngx_కూడు_timer(ev, 30000);

    } లేకపోతే {
        ngx_destroy_pool(ngx_తాత్కాలిక_pool);
        ngx_తాత్కాలిక_pool = ఖాళీ;
        ngx_old_చక్రాలు.nelts = 0;
    }
}


శూన్య
ngx_సమితి_shutdown_timer(ngx_చక్రం_t *చక్రం)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(చక్రం->conf_ctx, ngx_core_module);

    ఒకవేళ (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.దత్తాంశం = చక్రం;
        ngx_shutdown_event.సంవర్గమానం = చక్రం->సంవర్గమానం;
        ngx_shutdown_event.cancelable = 1;

        ngx_కూడు_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


స్థిర శూన్య
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_చక్రం_t       *చక్రం;
    ngx_connection_t  *c;

    చక్రం = ev->దత్తాంశం;

    c = చక్రం->connections;

    కొరకు (i = 0; i < చక్రం->connection_n; i++) {

        ఒకవేళ (c[i].fd == (ngx_socket_t) -1
            || c[i].పఠ == ఖాళీ
            || c[i].పఠ->accept
            || c[i].పఠ->channel
            || c[i].పఠ->resolver)
        {
            కొనసాగు;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->సంవర్గమానం, 0,
                       "*%uA shutdown timeout", c[i].సంఖ్య);

        c[i].close = 1;
        c[i].error = 1;

        c[i].పఠ->handler(c[i].పఠ);
    }
}
