
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>
#অন্তর্ভাব <ngx_event.h>


স্থির শূন্য ngx_destroy_cycle_pools(ngx_conf_t *conf);
স্থির ngx_int_t ngx_init_zone_pool(ngx_cycle_t *cycle,
    ngx_shm_zone_t *shm_zone);
স্থির ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *log);
স্থির শূন্য ngx_clean_old_cycles(ngx_event_t *ev);
স্থির শূন্য ngx_shutdown_timer_handler(ngx_event_t *ev);


অস্থির ngx_cycle_t  *ngx_cycle;
ngx_array_t            ngx_old_cycles;

স্থির ngx_pool_t     *ngx_অস্থায়ী_pool;
স্থির ngx_event_t     ngx_cleaner_event;
স্থির ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_mode;


/* STUB NAME */
স্থির ngx_connection_t  dumb;
/* STUB */


ngx_cycle_t *
ngx_init_cycle(ngx_cycle_t *old_cycle)
{
    শূন্য                *rv, *তথ্য;
    অক্ষর               **senv;
    ngx_uint_t           i, n;
    ngx_log_t           *log;
    ngx_time_t          *tp;
    ngx_conf_t           conf;
    ngx_pool_t          *pool;
    ngx_cycle_t         *cycle, **old;
    ngx_shm_zone_t      *shm_zone, *oshm_zone;
    ngx_list_part_t     *part, *opart;
    ngx_open_file_t     *file;
    ngx_listening_t     *ls, *nls;
    ngx_core_conf_t     *ccf, *old_ccf;
    ngx_core_module_t   *module;
    অক্ষর                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_time_update();


    log = old_cycle->log;

    pool = ngx_create_pool(NGX_CYCLE_POOL_আকার, log);
    যদি (pool == ফাঁকা) {
        ফেরত ফাঁকা;
    }
    pool->log = log;

    cycle = ngx_pcalloc(pool, আকার(ngx_cycle_t));
    যদি (cycle == ফাঁকা) {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }

    cycle->pool = pool;
    cycle->log = log;
    cycle->old_cycle = old_cycle;

    cycle->conf_prefix.len = old_cycle->conf_prefix.len;
    cycle->conf_prefix.তথ্য = ngx_pstrdup(pool, &old_cycle->conf_prefix);
    যদি (cycle->conf_prefix.তথ্য == ফাঁকা) {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }

    cycle->prefix.len = old_cycle->prefix.len;
    cycle->prefix.তথ্য = ngx_pstrdup(pool, &old_cycle->prefix);
    যদি (cycle->prefix.তথ্য == ফাঁকা) {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }

    cycle->error_log.len = old_cycle->error_log.len;
    cycle->error_log.তথ্য = ngx_pnalloc(pool, old_cycle->error_log.len + 1);
    যদি (cycle->error_log.তথ্য == ফাঁকা) {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }
    ngx_cpystrn(cycle->error_log.তথ্য, old_cycle->error_log.তথ্য,
                old_cycle->error_log.len + 1);

    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.তথ্য = ngx_pnalloc(pool, old_cycle->conf_file.len + 1);
    যদি (cycle->conf_file.তথ্য == ফাঁকা) {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }
    ngx_cpystrn(cycle->conf_file.তথ্য, old_cycle->conf_file.তথ্য,
                old_cycle->conf_file.len + 1);

    যদি (old_cycle->conf_param.len) {
        cycle->conf_param.len = old_cycle->conf_param.len;
        cycle->conf_param.তথ্য = ngx_pstrdup(pool, &old_cycle->conf_param);
        যদি (cycle->conf_param.তথ্য == ফাঁকা) {
            ngx_destroy_pool(pool);
            ফেরত ফাঁকা;
        }
    }


    n = old_cycle->paths.nelts ? old_cycle->paths.nelts : 10;

    যদি (ngx_array_init(&cycle->paths, pool, n, আকার(ngx_path_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }

    ngx_memzero(cycle->paths.elts, n * আকার(ngx_path_t *));


    যদি (ngx_array_init(&cycle->config_dump, pool, 1, আকার(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }

    ngx_rbtree_init(&cycle->config_dump_rbtree, &cycle->config_dump_sentinel,
                    ngx_str_rbtree_ঢোকাও_মান);

    যদি (old_cycle->open_files.part.nelts) {
        n = old_cycle->open_files.part.nelts;
        জন্য (part = old_cycle->open_files.part.পরবর্তী; part; part = part->পরবর্তী) {
            n += part->nelts;
        }

    } অন্যথা {
        n = 20;
    }

    যদি (ngx_list_init(&cycle->open_files, pool, n, আকার(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }


    যদি (old_cycle->shared_memory.part.nelts) {
        n = old_cycle->shared_memory.part.nelts;
        জন্য (part = old_cycle->shared_memory.part.পরবর্তী; part; part = part->পরবর্তী)
        {
            n += part->nelts;
        }

    } অন্যথা {
        n = 1;
    }

    যদি (ngx_list_init(&cycle->shared_memory, pool, n, আকার(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }

    n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;

    যদি (ngx_array_init(&cycle->listening, pool, n, আকার(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }

    ngx_memzero(cycle->listening.elts, n * আকার(ngx_listening_t));


    ngx_queue_init(&cycle->reusable_connections_queue);


    cycle->conf_ctx = ngx_pcalloc(pool, ngx_max_module * আকার(শূন্য *));
    যদি (cycle->conf_ctx == ফাঁকা) {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }


    যদি (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    cycle->hostname.len = ngx_strlen(hostname);

    cycle->hostname.তথ্য = ngx_pnalloc(pool, cycle->hostname.len);
    যদি (cycle->hostname.তথ্য == ফাঁকা) {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }

    ngx_strlow(cycle->hostname.তথ্য, (u_char *) hostname, cycle->hostname.len);


    যদি (ngx_cycle_modules(cycle) != NGX_OK) {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }


    জন্য (i = 0; cycle->modules[i]; i++) {
        যদি (cycle->modules[i]->type != NGX_CORE_MODULE) {
            চালু;
        }

        module = cycle->modules[i]->ctx;

        যদি (module->create_conf) {
            rv = module->create_conf(cycle);
            যদি (rv == ফাঁকা) {
                ngx_destroy_pool(pool);
                ফেরত ফাঁকা;
            }
            cycle->conf_ctx[cycle->modules[i]->সূচক] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, আকার(ngx_conf_t));
    /* STUB: init array ? */
    conf.args = ngx_array_create(pool, 10, আকার(ngx_str_t));
    যদি (conf.args == ফাঁকা) {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }

    conf.অস্থায়ী_pool = ngx_create_pool(NGX_CYCLE_POOL_আকার, log);
    যদি (conf.অস্থায়ী_pool == ফাঁকা) {
        ngx_destroy_pool(pool);
        ফেরত ফাঁকা;
    }


    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#যদি 0
    log->log_level = NGX_LOG_DEBUG_ALL;
#endif

    যদি (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        ফেরত ফাঁকা;
    }

    যদি (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        ফেরত ফাঁকা;
    }

    যদি (ngx_test_config && !ngx_quiet_mode) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       cycle->conf_file.তথ্য);
    }

    জন্য (i = 0; cycle->modules[i]; i++) {
        যদি (cycle->modules[i]->type != NGX_CORE_MODULE) {
            চালু;
        }

        module = cycle->modules[i]->ctx;

        যদি (module->init_conf) {
            যদি (module->init_conf(cycle,
                                  cycle->conf_ctx[cycle->modules[i]->সূচক])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_cycle_pools(&conf);
                ফেরত ফাঁকা;
            }
        }
    }

    যদি (ngx_process == NGX_PROCESS_SIGNALLER) {
        ফেরত cycle;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    যদি (ngx_test_config) {

        যদি (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
            যাও failed;
        }

    } অন্যথা যদি (!ngx_is_init_cycle(old_cycle)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        যদি (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.তথ্য, old_ccf->pid.তথ্য) != 0)
        {
            /* new pid file name */

            যদি (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
                যাও failed;
            }

            ngx_delete_pidfile(old_cycle);
        }
    }


    যদি (ngx_test_lockfile(cycle->lock_file.তথ্য, log) != NGX_OK) {
        যাও failed;
    }


    যদি (ngx_create_paths(cycle, ccf->user) != NGX_OK) {
        যাও failed;
    }


    যদি (ngx_log_open_default(cycle) != NGX_OK) {
        যাও failed;
    }

    /* open the new files */

    part = &cycle->open_files.part;
    file = part->elts;

    জন্য (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->পরবর্তী == ফাঁকা) {
                বিরতি;
            }
            part = part->পরবর্তী;
            file = part->elts;
            i = 0;
        }

        যদি (file[i].name.len == 0) {
            চালু;
        }

        file[i].fd = ngx_open_file(file[i].name.তথ্য,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, log, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.তথ্য);

        যদি (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.তথ্য);
            যাও failed;
        }

#যদি !(NGX_WIN32)
        যদি (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.তথ্য);
            যাও failed;
        }
#endif
    }

    cycle->log = &cycle->new_log;
    pool->log = &cycle->new_log;


    /* create shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    জন্য (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->পরবর্তী == ফাঁকা) {
                বিরতি;
            }
            part = part->পরবর্তী;
            shm_zone = part->elts;
            i = 0;
        }

        যদি (shm_zone[i].shm.আকার_2 == 0) {
            ngx_log_error(NGX_LOG_EMERG, log, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            যাও failed;
        }

        shm_zone[i].shm.log = cycle->log;

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        তথ্য = ফাঁকা;

        জন্য (n = 0; /* void */ ; n++) {

            যদি (n >= opart->nelts) {
                যদি (opart->পরবর্তী == ফাঁকা) {
                    বিরতি;
                }
                opart = opart->পরবর্তী;
                oshm_zone = opart->elts;
                n = 0;
            }

            যদি (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                চালু;
            }

            যদি (ngx_strncmp(shm_zone[i].shm.name.তথ্য,
                            oshm_zone[n].shm.name.তথ্য,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                চালু;
            }

            যদি (shm_zone[i].tag == oshm_zone[n].tag && shm_zone[i].noreuse) {
                তথ্য = oshm_zone[n].তথ্য;
                বিরতি;
            }

            যদি (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.আকার_2 == oshm_zone[n].shm.আকার_2)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#যদি (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#endif

                যদি (shm_zone[i].init(&shm_zone[i], oshm_zone[n].তথ্য)
                    != NGX_OK)
                {
                    যাও failed;
                }

                যাও shm_zone_found;
            }

            বিরতি;
        }

        যদি (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            যাও failed;
        }

        যদি (ngx_init_zone_pool(cycle, &shm_zone[i]) != NGX_OK) {
            যাও failed;
        }

        যদি (shm_zone[i].init(&shm_zone[i], তথ্য) != NGX_OK) {
            যাও failed;
        }

    shm_zone_found:

        চালু;
    }


    /* handle the listening sockets */

    যদি (old_cycle->listening.nelts) {
        ls = old_cycle->listening.elts;
        জন্য (i = 0; i < old_cycle->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = cycle->listening.elts;
        জন্য (n = 0; n < cycle->listening.nelts; n++) {

            জন্য (i = 0; i < old_cycle->listening.nelts; i++) {
                যদি (ls[i].ignore) {
                    চালু;
                }

                যদি (ls[i].remain) {
                    চালু;
                }

                যদি (ls[i].type != nls[n].type) {
                    চালু;
                }

                যদি (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].previous = &ls[i];

                    যদি (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } অন্যথা {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    যদি (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#যদি (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    যদি (ls[i].accept_filter && nls[n].accept_filter) {
                        যদি (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].add_deferred = 1;
                        }

                    } অন্যথা যদি (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } অন্যথা যদি (nls[n].accept_filter) {
                        nls[n].add_deferred = 1;
                    }
#endif

#যদি (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    যদি (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } অন্যথা যদি (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].add_deferred = 1;
                    }
#endif

#যদি (NGX_HAVE_REUSEPORT)
                    যদি (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].add_reuseport = 1;
                    }
#endif

                    বিরতি;
                }
            }

            যদি (nls[n].fd == (ngx_socket_t) -1) {
#যদি (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                যদি (nls[n].accept_filter) {
                    nls[n].add_deferred = 1;
                }
#endif
#যদি (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                যদি (nls[n].deferred_accept) {
                    nls[n].add_deferred = 1;
                }
#endif
            }
        }

    } অন্যথা {
#যদি (NGX_HAVE_DEFERRED_ACCEPT)
        ls = cycle->listening.elts;
        জন্য (i = 0; i < cycle->listening.nelts; i++) {
#ifdef SO_ACCEPTFILTER
            যদি (ls[i].accept_filter) {
                ls[i].add_deferred = 1;
            }
#endif
#ifdef TCP_DEFER_ACCEPT
            যদি (ls[i].deferred_accept) {
                ls[i].add_deferred = 1;
            }
#endif
        }
#endif
    }

    যদি (ngx_open_listening_sockets(cycle) != NGX_OK) {
        যাও failed;
    }

    যদি (!ngx_test_config) {
        ngx_configure_listening_sockets(cycle);
    }


    /* commit the new cycle configuration */

    যদি (!ngx_use_stderr) {
        (শূন্য) ngx_log_redirect_stderr(cycle);
    }

    pool->log = cycle->log;

    যদি (ngx_init_modules(cycle) != NGX_OK) {
        /* fatal */
        exit(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_cycle->shared_memory.part;
    oshm_zone = opart->elts;

    জন্য (i = 0; /* void */ ; i++) {

        যদি (i >= opart->nelts) {
            যদি (opart->পরবর্তী == ফাঁকা) {
                যাও old_shm_zone_done;
            }
            opart = opart->পরবর্তী;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &cycle->shared_memory.part;
        shm_zone = part->elts;

        জন্য (n = 0; /* void */ ; n++) {

            যদি (n >= part->nelts) {
                যদি (part->পরবর্তী == ফাঁকা) {
                    বিরতি;
                }
                part = part->পরবর্তী;
                shm_zone = part->elts;
                n = 0;
            }

            যদি (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                চালু;
            }

            যদি (ngx_strncmp(oshm_zone[i].shm.name.তথ্য,
                            shm_zone[n].shm.name.তথ্য,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                চালু;
            }

            যদি (oshm_zone[i].tag == shm_zone[n].tag
                && oshm_zone[i].shm.আকার_2 == shm_zone[n].shm.আকার_2
                && !oshm_zone[i].noreuse)
            {
                যাও live_shm_zone;
            }

            বিরতি;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    live_shm_zone:

        চালু;
    }

old_shm_zone_done:


    /* close the unnecessary listening sockets */

    ls = old_cycle->listening.elts;
    জন্য (i = 0; i < old_cycle->listening.nelts; i++) {

        যদি (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            চালু;
        }

        যদি (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_text);
        }

#যদি (NGX_HAVE_UNIX_DOMAIN)

        যদি (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_text.তথ্য + আকার("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                          "deleting socket %s", name);

            যদি (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#endif
    }


    /* close the unnecessary open files */

    part = &old_cycle->open_files.part;
    file = part->elts;

    জন্য (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->পরবর্তী == ফাঁকা) {
                বিরতি;
            }
            part = part->পরবর্তী;
            file = part->elts;
            i = 0;
        }

        যদি (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            চালু;
        }

        যদি (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.তথ্য);
        }
    }

    ngx_destroy_pool(conf.অস্থায়ী_pool);

    যদি (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_cycle(old_cycle)) {

        ngx_destroy_pool(old_cycle->pool);
        cycle->old_cycle = ফাঁকা;

        ফেরত cycle;
    }


    যদি (ngx_অস্থায়ী_pool == ফাঁকা) {
        ngx_অস্থায়ী_pool = ngx_create_pool(128, cycle->log);
        যদি (ngx_অস্থায়ী_pool == ফাঁকা) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "could not create ngx_temp_pool");
            exit(1);
        }

        n = 10;

        যদি (ngx_array_init(&ngx_old_cycles, ngx_অস্থায়ী_pool, n,
                           আকার(ngx_cycle_t *))
            != NGX_OK)
        {
            exit(1);
        }

        ngx_memzero(ngx_old_cycles.elts, n * আকার(ngx_cycle_t *));

        ngx_cleaner_event.handler = ngx_clean_old_cycles;
        ngx_cleaner_event.log = cycle->log;
        ngx_cleaner_event.তথ্য = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_অস্থায়ী_pool->log = cycle->log;

    old = ngx_array_ঠেলো(&ngx_old_cycles);
    যদি (old == ফাঁকা) {
        exit(1);
    }
    *old = old_cycle;

    যদি (!ngx_cleaner_event.timer_set) {
        ngx_add_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_set = 1;
    }

    ফেরত cycle;


failed:

    যদি (!ngx_is_init_cycle(old_cycle)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        যদি (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &cycle->open_files.part;
    file = part->elts;

    জন্য (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->পরবর্তী == ফাঁকা) {
                বিরতি;
            }
            part = part->পরবর্তী;
            file = part->elts;
            i = 0;
        }

        যদি (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            চালু;
        }

        যদি (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.তথ্য);
        }
    }

    /* free the newly created shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    জন্য (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->পরবর্তী == ফাঁকা) {
                বিরতি;
            }
            part = part->পরবর্তী;
            shm_zone = part->elts;
            i = 0;
        }

        যদি (shm_zone[i].shm.addr == ফাঁকা) {
            চালু;
        }

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        জন্য (n = 0; /* void */ ; n++) {

            যদি (n >= opart->nelts) {
                যদি (opart->পরবর্তী == ফাঁকা) {
                    বিরতি;
                }
                opart = opart->পরবর্তী;
                oshm_zone = opart->elts;
                n = 0;
            }

            যদি (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                চালু;
            }

            যদি (ngx_strncmp(shm_zone[i].shm.name.তথ্য,
                            oshm_zone[n].shm.name.তথ্য,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                চালু;
            }

            যদি (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.আকার_2 == oshm_zone[n].shm.আকার_2
                && !shm_zone[i].noreuse)
            {
                যাও old_shm_zone_found;
            }

            বিরতি;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_found:

        চালু;
    }

    যদি (ngx_test_config) {
        ngx_destroy_cycle_pools(&conf);
        ফেরত ফাঁকা;
    }

    ls = cycle->listening.elts;
    জন্য (i = 0; i < cycle->listening.nelts; i++) {
        যদি (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            চালু;
        }

        যদি (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_text);
        }
    }

    ngx_destroy_cycle_pools(&conf);

    ফেরত ফাঁকা;
}


স্থির শূন্য
ngx_destroy_cycle_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->অস্থায়ী_pool);
    ngx_destroy_pool(conf->pool);
}


স্থির ngx_int_t
ngx_init_zone_pool(ngx_cycle_t *cycle, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    যদি (zn->shm.exists) {

        যদি (sp == sp->addr) {
            ফেরত NGX_OK;
        }

#যদি (NGX_WIN32)

        /* remap at the required address */

        যদি (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            ফেরত NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        যদি (sp == sp->addr) {
            ফেরত NGX_OK;
        }

#endif

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        ফেরত NGX_ERROR;
    }

    sp->end = zn->shm.addr + zn->shm.আকার_2;
    sp->min_shift = 3;
    sp->addr = zn->shm.addr;

#যদি (NGX_HAVE_ATOMIC_OPS)

    file = ফাঁকা;

#অন্যথা

    file = ngx_pnalloc(cycle->pool,
                       cycle->lock_file.len + zn->shm.name.len + 1);
    যদি (file == ফাঁকা) {
        ফেরত NGX_ERROR;
    }

    (শূন্য) ngx_sprintf(file, "%V%V%Z", &cycle->lock_file, &zn->shm.name);

#endif

    যদি (ngx_shmtx_create(&sp->mutex, &sp->lock, file) != NGX_OK) {
        ফেরত NGX_ERROR;
    }

    ngx_slab_init(sp);

    ফেরত NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log)
{
    আকার_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    যদি (ngx_process > NGX_PROCESS_MASTER) {
        ফেরত NGX_OK;
    }

    ngx_memzero(&file, আকার(ngx_file_t));

    file.name = *name;
    file.log = log;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.তথ্য, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    যদি (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.তথ্য);
        ফেরত NGX_ERROR;
    }

    rc = NGX_OK;

    যদি (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        যদি (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    যদি (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.তথ্য);
    }

    ফেরত rc;
}


শূন্য
ngx_delete_pidfile(ngx_cycle_t *cycle)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    name = ngx_new_দ্বিমিক ? ccf->oldpid.তথ্য : ccf->pid.তথ্য;

    যদি (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_cycle_t *cycle, অক্ষর *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ngx_memzero(&file, আকার(ngx_file_t));

    file.name = ccf->pid;
    file.log = cycle->log;

    file.fd = ngx_open_file(file.name.তথ্য, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    যদি (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.তথ্য);
        ফেরত 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    যদি (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.তথ্য);
    }

    যদি (n == NGX_ERROR) {
        ফেরত 1;
    }

    যখন (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    যদি (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.তথ্য);
        ফেরত 1;
    }

    ফেরত ngx_os_signal_process(cycle, sig, pid);

}


স্থির ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *log)
{
#যদি !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    যদি (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        ফেরত NGX_ERROR;
    }

    যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    যদি (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#endif

    ফেরত NGX_OK;
}


শূন্য
ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &cycle->open_files.part;
    file = part->elts;

    জন্য (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->পরবর্তী == ফাঁকা) {
                বিরতি;
            }
            part = part->পরবর্তী;
            file = part->elts;
            i = 0;
        }

        যদি (file[i].name.len == 0) {
            চালু;
        }

        যদি (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }

        fd = ngx_open_file(file[i].name.তথ্য, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.তথ্য, file[i].fd, fd);

        যদি (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.তথ্য);
            চালু;
        }

#যদি !(NGX_WIN32)
        যদি (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            যদি (ngx_file_info(file[i].name.তথ্য, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.তথ্য);

                যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.তথ্য);
                }

                চালু;
            }

            যদি (fi.st_uid != user) {
                যদি (chown((ধ্রুবক অক্ষর *) file[i].name.তথ্য, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.তথ্য, user);

                    যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.তথ্য);
                    }

                    চালু;
                }
            }

            যদি ((fi.st_mode & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_mode |= (S_IRUSR|S_IWUSR);

                যদি (chmod((ধ্রুবক অক্ষর *) file[i].name.তথ্য, fi.st_mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.তথ্য);

                    যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.তথ্য);
                    }

                    চালু;
                }
            }
        }

        যদি (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.তথ্য);

            যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.তথ্য);
            }

            চালু;
        }
#endif

        যদি (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.তথ্য);
        }

        file[i].fd = fd;
    }

    (শূন্য) ngx_log_redirect_stderr(cycle);
}


ngx_shm_zone_t *
ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name, আকার_t আকার_2, শূন্য *tag)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->cycle->shared_memory.part;
    shm_zone = part->elts;

    জন্য (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->পরবর্তী == ফাঁকা) {
                বিরতি;
            }
            part = part->পরবর্তী;
            shm_zone = part->elts;
            i = 0;
        }

        যদি (name->len != shm_zone[i].shm.name.len) {
            চালু;
        }

        যদি (ngx_strncmp(name->তথ্য, shm_zone[i].shm.name.তথ্য, name->len)
            != 0)
        {
            চালু;
        }

        যদি (tag != shm_zone[i].tag) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            ফেরত ফাঁকা;
        }

        যদি (shm_zone[i].shm.আকার_2 == 0) {
            shm_zone[i].shm.আকার_2 = আকার_2;
        }

        যদি (আকার_2 && আকার_2 != shm_zone[i].shm.আকার_2) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            আকার_2, &shm_zone[i].shm.name, shm_zone[i].shm.আকার_2);
            ফেরত ফাঁকা;
        }

        ফেরত &shm_zone[i];
    }

    shm_zone = ngx_list_ঠেলো(&cf->cycle->shared_memory);

    যদি (shm_zone == ফাঁকা) {
        ফেরত ফাঁকা;
    }

    shm_zone->তথ্য = ফাঁকা;
    shm_zone->shm.log = cf->cycle->log;
    shm_zone->shm.addr = ফাঁকা;
    shm_zone->shm.আকার_2 = আকার_2;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = ফাঁকা;
    shm_zone->tag = tag;
    shm_zone->noreuse = 0;

    ফেরত shm_zone;
}


স্থির শূন্য
ngx_clean_old_cycles(ngx_event_t *ev)
{
    ngx_uint_t     i, n, found, live;
    ngx_log_t     *log;
    ngx_cycle_t  **cycle;

    log = ngx_cycle->log;
    ngx_অস্থায়ী_pool->log = log;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, log, 0, "clean old cycles");

    live = 0;

    cycle = ngx_old_cycles.elts;
    জন্য (i = 0; i < ngx_old_cycles.nelts; i++) {

        যদি (cycle[i] == ফাঁকা) {
            চালু;
        }

        found = 0;

        জন্য (n = 0; n < cycle[i]->connection_n; n++) {
            যদি (cycle[i]->connections[n].fd != (ngx_socket_t) -1) {
                found = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "live fd:%ui", n);

                বিরতি;
            }
        }

        যদি (found) {
            live = 1;
            চালু;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(cycle[i]->pool);
        cycle[i] = ফাঁকা;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "old cycles status: %ui", live);

    যদি (live) {
        ngx_add_timer(ev, 30000);

    } অন্যথা {
        ngx_destroy_pool(ngx_অস্থায়ী_pool);
        ngx_অস্থায়ী_pool = ফাঁকা;
        ngx_old_cycles.nelts = 0;
    }
}


শূন্য
ngx_set_shutdown_timer(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    যদি (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.তথ্য = cycle;
        ngx_shutdown_event.log = cycle->log;
        ngx_shutdown_event.cancelable = 1;

        ngx_add_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


স্থির শূন্য
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_cycle_t       *cycle;
    ngx_connection_t  *c;

    cycle = ev->তথ্য;

    c = cycle->connections;

    জন্য (i = 0; i < cycle->connection_n; i++) {

        যদি (c[i].fd == (ngx_socket_t) -1
            || c[i].পঠ == ফাঁকা
            || c[i].পঠ->accept
            || c[i].পঠ->channel
            || c[i].পঠ->resolver)
        {
            চালু;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "*%uA shutdown timeout", c[i].সংখ্যা);

        c[i].close = 1;
        c[i].error = 1;

        c[i].পঠ->handler(c[i].পঠ);
    }
}
