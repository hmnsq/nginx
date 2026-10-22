
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


চঞ্চল ngx_cycle_t  *ngx_cycle;
ngx_array_t            ngx_old_cycles;

স্থির ngx_pool_t     *ngx_temp_pool;
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
    শূন্য                *rv, *data;
    বর্ণ               **senv;
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
    বর্ণ                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_time_update();


    log = old_cycle->log;

    pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    যদি (pool == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }
    pool->log = log;

    cycle = ngx_pcalloc(pool, পরিমাণ(ngx_cycle_t));
    যদি (cycle == রিক্ত) {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }

    cycle->pool = pool;
    cycle->log = log;
    cycle->old_cycle = old_cycle;

    cycle->conf_prefix.len = old_cycle->conf_prefix.len;
    cycle->conf_prefix.data = ngx_pstrdup(pool, &old_cycle->conf_prefix);
    যদি (cycle->conf_prefix.data == রিক্ত) {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }

    cycle->prefix.len = old_cycle->prefix.len;
    cycle->prefix.data = ngx_pstrdup(pool, &old_cycle->prefix);
    যদি (cycle->prefix.data == রিক্ত) {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }

    cycle->error_log.len = old_cycle->error_log.len;
    cycle->error_log.data = ngx_pnalloc(pool, old_cycle->error_log.len + 1);
    যদি (cycle->error_log.data == রিক্ত) {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }
    ngx_cpystrn(cycle->error_log.data, old_cycle->error_log.data,
                old_cycle->error_log.len + 1);

    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.data = ngx_pnalloc(pool, old_cycle->conf_file.len + 1);
    যদি (cycle->conf_file.data == রিক্ত) {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }
    ngx_cpystrn(cycle->conf_file.data, old_cycle->conf_file.data,
                old_cycle->conf_file.len + 1);

    যদি (old_cycle->conf_param.len) {
        cycle->conf_param.len = old_cycle->conf_param.len;
        cycle->conf_param.data = ngx_pstrdup(pool, &old_cycle->conf_param);
        যদি (cycle->conf_param.data == রিক্ত) {
            ngx_destroy_pool(pool);
            প্রত্যাগম রিক্ত;
        }
    }


    n = old_cycle->paths.nelts ? old_cycle->paths.nelts : 10;

    যদি (ngx_array_init(&cycle->paths, pool, n, পরিমাণ(ngx_path_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }

    ngx_memzero(cycle->paths.elts, n * পরিমাণ(ngx_path_t *));


    যদি (ngx_array_init(&cycle->config_dump, pool, 1, পরিমাণ(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }

    ngx_rbtree_init(&cycle->config_dump_rbtree, &cycle->config_dump_sentinel,
                    ngx_str_rbtree_ঢোকাও_মান);

    যদি (old_cycle->open_files.part.nelts) {
        n = old_cycle->open_files.part.nelts;
        প্রতি (part = old_cycle->open_files.part.next; part; part = part->next) {
            n += part->nelts;
        }

    } অন্যথা {
        n = 20;
    }

    যদি (ngx_list_init(&cycle->open_files, pool, n, পরিমাণ(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }


    যদি (old_cycle->shared_memory.part.nelts) {
        n = old_cycle->shared_memory.part.nelts;
        প্রতি (part = old_cycle->shared_memory.part.next; part; part = part->next)
        {
            n += part->nelts;
        }

    } অন্যথা {
        n = 1;
    }

    যদি (ngx_list_init(&cycle->shared_memory, pool, n, পরিমাণ(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }

    n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;

    যদি (ngx_array_init(&cycle->listening, pool, n, পরিমাণ(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }

    ngx_memzero(cycle->listening.elts, n * পরিমাণ(ngx_listening_t));


    ngx_queue_init(&cycle->reusable_connections_queue);


    cycle->conf_ctx = ngx_pcalloc(pool, ngx_max_module * পরিমাণ(শূন্য *));
    যদি (cycle->conf_ctx == রিক্ত) {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }


    যদি (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    cycle->hostname.len = ngx_strlen(hostname);

    cycle->hostname.data = ngx_pnalloc(pool, cycle->hostname.len);
    যদি (cycle->hostname.data == রিক্ত) {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }

    ngx_strlow(cycle->hostname.data, (u_char *) hostname, cycle->hostname.len);


    যদি (ngx_cycle_modules(cycle) != NGX_OK) {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }


    প্রতি (i = 0; cycle->modules[i]; i++) {
        যদি (cycle->modules[i]->type != NGX_CORE_MODULE) {
            অনুবর্ত;
        }

        module = cycle->modules[i]->ctx;

        যদি (module->create_conf) {
            rv = module->create_conf(cycle);
            যদি (rv == রিক্ত) {
                ngx_destroy_pool(pool);
                প্রত্যাগম রিক্ত;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, পরিমাণ(ngx_conf_t));
    /* STUB: init array ? */
    conf.args = ngx_array_create(pool, 10, পরিমাণ(ngx_str_t));
    যদি (conf.args == রিক্ত) {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
    }

    conf.temp_pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    যদি (conf.temp_pool == রিক্ত) {
        ngx_destroy_pool(pool);
        প্রত্যাগম রিক্ত;
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
        প্রত্যাগম রিক্ত;
    }

    যদি (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        প্রত্যাগম রিক্ত;
    }

    যদি (ngx_test_config && !ngx_quiet_mode) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       cycle->conf_file.data);
    }

    প্রতি (i = 0; cycle->modules[i]; i++) {
        যদি (cycle->modules[i]->type != NGX_CORE_MODULE) {
            অনুবর্ত;
        }

        module = cycle->modules[i]->ctx;

        যদি (module->init_conf) {
            যদি (module->init_conf(cycle,
                                  cycle->conf_ctx[cycle->modules[i]->index])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_cycle_pools(&conf);
                প্রত্যাগম রিক্ত;
            }
        }
    }

    যদি (ngx_process == NGX_PROCESS_SIGNALLER) {
        প্রত্যাগম cycle;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    যদি (ngx_test_config) {

        যদি (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
            গচ্ছ failed;
        }

    } অন্যথা যদি (!ngx_is_init_cycle(old_cycle)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        যদি (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.data, old_ccf->pid.data) != 0)
        {
            /* new pid file name */

            যদি (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
                গচ্ছ failed;
            }

            ngx_delete_pidfile(old_cycle);
        }
    }


    যদি (ngx_test_lockfile(cycle->lock_file.data, log) != NGX_OK) {
        গচ্ছ failed;
    }


    যদি (ngx_create_paths(cycle, ccf->user) != NGX_OK) {
        গচ্ছ failed;
    }


    যদি (ngx_log_open_default(cycle) != NGX_OK) {
        গচ্ছ failed;
    }

    /* open the new files */

    part = &cycle->open_files.part;
    file = part->elts;

    প্রতি (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->next == রিক্ত) {
                ভঙ্গ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        যদি (file[i].name.len == 0) {
            অনুবর্ত;
        }

        file[i].fd = ngx_open_file(file[i].name.data,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, log, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.data);

        যদি (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.data);
            গচ্ছ failed;
        }

#যদি !(NGX_WIN32)
        যদি (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);
            গচ্ছ failed;
        }
#endif
    }

    cycle->log = &cycle->new_log;
    pool->log = &cycle->new_log;


    /* create shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    প্রতি (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->next == রিক্ত) {
                ভঙ্গ;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        যদি (shm_zone[i].shm.size == 0) {
            ngx_log_error(NGX_LOG_EMERG, log, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            গচ্ছ failed;
        }

        shm_zone[i].shm.log = cycle->log;

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        data = রিক্ত;

        প্রতি (n = 0; /* void */ ; n++) {

            যদি (n >= opart->nelts) {
                যদি (opart->next == রিক্ত) {
                    ভঙ্গ;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            যদি (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                অনুবর্ত;
            }

            যদি (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                অনুবর্ত;
            }

            যদি (shm_zone[i].tag == oshm_zone[n].tag && shm_zone[i].noreuse) {
                data = oshm_zone[n].data;
                ভঙ্গ;
            }

            যদি (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#যদি (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#endif

                যদি (shm_zone[i].init(&shm_zone[i], oshm_zone[n].data)
                    != NGX_OK)
                {
                    গচ্ছ failed;
                }

                গচ্ছ shm_zone_found;
            }

            ভঙ্গ;
        }

        যদি (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            গচ্ছ failed;
        }

        যদি (ngx_init_zone_pool(cycle, &shm_zone[i]) != NGX_OK) {
            গচ্ছ failed;
        }

        যদি (shm_zone[i].init(&shm_zone[i], data) != NGX_OK) {
            গচ্ছ failed;
        }

    shm_zone_found:

        অনুবর্ত;
    }


    /* handle the listening sockets */

    যদি (old_cycle->listening.nelts) {
        ls = old_cycle->listening.elts;
        প্রতি (i = 0; i < old_cycle->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = cycle->listening.elts;
        প্রতি (n = 0; n < cycle->listening.nelts; n++) {

            প্রতি (i = 0; i < old_cycle->listening.nelts; i++) {
                যদি (ls[i].ignore) {
                    অনুবর্ত;
                }

                যদি (ls[i].remain) {
                    অনুবর্ত;
                }

                যদি (ls[i].type != nls[n].type) {
                    অনুবর্ত;
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

                    ভঙ্গ;
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
        প্রতি (i = 0; i < cycle->listening.nelts; i++) {
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
        গচ্ছ failed;
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

    প্রতি (i = 0; /* void */ ; i++) {

        যদি (i >= opart->nelts) {
            যদি (opart->next == রিক্ত) {
                গচ্ছ old_shm_zone_done;
            }
            opart = opart->next;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &cycle->shared_memory.part;
        shm_zone = part->elts;

        প্রতি (n = 0; /* void */ ; n++) {

            যদি (n >= part->nelts) {
                যদি (part->next == রিক্ত) {
                    ভঙ্গ;
                }
                part = part->next;
                shm_zone = part->elts;
                n = 0;
            }

            যদি (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                অনুবর্ত;
            }

            যদি (ngx_strncmp(oshm_zone[i].shm.name.data,
                            shm_zone[n].shm.name.data,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                অনুবর্ত;
            }

            যদি (oshm_zone[i].tag == shm_zone[n].tag
                && oshm_zone[i].shm.size == shm_zone[n].shm.size
                && !oshm_zone[i].noreuse)
            {
                গচ্ছ live_shm_zone;
            }

            ভঙ্গ;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    live_shm_zone:

        অনুবর্ত;
    }

old_shm_zone_done:


    /* close the unnecessary listening sockets */

    ls = old_cycle->listening.elts;
    প্রতি (i = 0; i < old_cycle->listening.nelts; i++) {

        যদি (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            অনুবর্ত;
        }

        যদি (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_text);
        }

#যদি (NGX_HAVE_UNIX_DOMAIN)

        যদি (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_text.data + পরিমাণ("unix:") - 1;

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

    প্রতি (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->next == রিক্ত) {
                ভঙ্গ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        যদি (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            অনুবর্ত;
        }

        যদি (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    ngx_destroy_pool(conf.temp_pool);

    যদি (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_cycle(old_cycle)) {

        ngx_destroy_pool(old_cycle->pool);
        cycle->old_cycle = রিক্ত;

        প্রত্যাগম cycle;
    }


    যদি (ngx_temp_pool == রিক্ত) {
        ngx_temp_pool = ngx_create_pool(128, cycle->log);
        যদি (ngx_temp_pool == রিক্ত) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "could not create ngx_temp_pool");
            exit(1);
        }

        n = 10;

        যদি (ngx_array_init(&ngx_old_cycles, ngx_temp_pool, n,
                           পরিমাণ(ngx_cycle_t *))
            != NGX_OK)
        {
            exit(1);
        }

        ngx_memzero(ngx_old_cycles.elts, n * পরিমাণ(ngx_cycle_t *));

        ngx_cleaner_event.handler = ngx_clean_old_cycles;
        ngx_cleaner_event.log = cycle->log;
        ngx_cleaner_event.data = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_temp_pool->log = cycle->log;

    old = ngx_array_ঠেলো(&ngx_old_cycles);
    যদি (old == রিক্ত) {
        exit(1);
    }
    *old = old_cycle;

    যদি (!ngx_cleaner_event.timer_set) {
        ngx_add_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_set = 1;
    }

    প্রত্যাগম cycle;


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

    প্রতি (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->next == রিক্ত) {
                ভঙ্গ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        যদি (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            অনুবর্ত;
        }

        যদি (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    /* free the newly created shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    প্রতি (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->next == রিক্ত) {
                ভঙ্গ;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        যদি (shm_zone[i].shm.addr == রিক্ত) {
            অনুবর্ত;
        }

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        প্রতি (n = 0; /* void */ ; n++) {

            যদি (n >= opart->nelts) {
                যদি (opart->next == রিক্ত) {
                    ভঙ্গ;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            যদি (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                অনুবর্ত;
            }

            যদি (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                অনুবর্ত;
            }

            যদি (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size
                && !shm_zone[i].noreuse)
            {
                গচ্ছ old_shm_zone_found;
            }

            ভঙ্গ;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_found:

        অনুবর্ত;
    }

    যদি (ngx_test_config) {
        ngx_destroy_cycle_pools(&conf);
        প্রত্যাগম রিক্ত;
    }

    ls = cycle->listening.elts;
    প্রতি (i = 0; i < cycle->listening.nelts; i++) {
        যদি (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            অনুবর্ত;
        }

        যদি (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_text);
        }
    }

    ngx_destroy_cycle_pools(&conf);

    প্রত্যাগম রিক্ত;
}


স্থির শূন্য
ngx_destroy_cycle_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->temp_pool);
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
            প্রত্যাগম NGX_OK;
        }

#যদি (NGX_WIN32)

        /* remap at the required address */

        যদি (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            প্রত্যাগম NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        যদি (sp == sp->addr) {
            প্রত্যাগম NGX_OK;
        }

#endif

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        প্রত্যাগম NGX_ERROR;
    }

    sp->end = zn->shm.addr + zn->shm.size;
    sp->min_shift = 3;
    sp->addr = zn->shm.addr;

#যদি (NGX_HAVE_ATOMIC_OPS)

    file = রিক্ত;

#অন্যথা

    file = ngx_pnalloc(cycle->pool,
                       cycle->lock_file.len + zn->shm.name.len + 1);
    যদি (file == রিক্ত) {
        প্রত্যাগম NGX_ERROR;
    }

    (শূন্য) ngx_sprintf(file, "%V%V%Z", &cycle->lock_file, &zn->shm.name);

#endif

    যদি (ngx_shmtx_create(&sp->mutex, &sp->lock, file) != NGX_OK) {
        প্রত্যাগম NGX_ERROR;
    }

    ngx_slab_init(sp);

    প্রত্যাগম NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log)
{
    size_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    যদি (ngx_process > NGX_PROCESS_MASTER) {
        প্রত্যাগম NGX_OK;
    }

    ngx_memzero(&file, পরিমাণ(ngx_file_t));

    file.name = *name;
    file.log = log;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    যদি (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        প্রত্যাগম NGX_ERROR;
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
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    প্রত্যাগম rc;
}


শূন্য
ngx_delete_pidfile(ngx_cycle_t *cycle)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    name = ngx_new_দ্বিমিক ? ccf->oldpid.data : ccf->pid.data;

    যদি (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_cycle_t *cycle, বর্ণ *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ngx_memzero(&file, পরিমাণ(ngx_file_t));

    file.name = ccf->pid;
    file.log = cycle->log;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    যদি (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        প্রত্যাগম 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    যদি (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    যদি (n == NGX_ERROR) {
        প্রত্যাগম 1;
    }

    যাবত্ (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    যদি (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.data);
        প্রত্যাগম 1;
    }

    প্রত্যাগম ngx_os_signal_process(cycle, sig, pid);

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
        প্রত্যাগম NGX_ERROR;
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

    প্রত্যাগম NGX_OK;
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

    প্রতি (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->next == রিক্ত) {
                ভঙ্গ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        যদি (file[i].name.len == 0) {
            অনুবর্ত;
        }

        যদি (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }

        fd = ngx_open_file(file[i].name.data, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.data, file[i].fd, fd);

        যদি (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.data);
            অনুবর্ত;
        }

#যদি !(NGX_WIN32)
        যদি (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            যদি (ngx_file_info(file[i].name.data, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.data);

                যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.data);
                }

                অনুবর্ত;
            }

            যদি (fi.st_uid != user) {
                যদি (chown((ধ্রুব বর্ণ *) file[i].name.data, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.data, user);

                    যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    অনুবর্ত;
                }
            }

            যদি ((fi.st_mode & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_mode |= (S_IRUSR|S_IWUSR);

                যদি (chmod((ধ্রুব বর্ণ *) file[i].name.data, fi.st_mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.data);

                    যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    অনুবর্ত;
                }
            }
        }

        যদি (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);

            যদি (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.data);
            }

            অনুবর্ত;
        }
#endif

        যদি (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }

        file[i].fd = fd;
    }

    (শূন্য) ngx_log_redirect_stderr(cycle);
}


ngx_shm_zone_t *
ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name, size_t size, শূন্য *tag)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->cycle->shared_memory.part;
    shm_zone = part->elts;

    প্রতি (i = 0; /* void */ ; i++) {

        যদি (i >= part->nelts) {
            যদি (part->next == রিক্ত) {
                ভঙ্গ;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        যদি (name->len != shm_zone[i].shm.name.len) {
            অনুবর্ত;
        }

        যদি (ngx_strncmp(name->data, shm_zone[i].shm.name.data, name->len)
            != 0)
        {
            অনুবর্ত;
        }

        যদি (tag != shm_zone[i].tag) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            প্রত্যাগম রিক্ত;
        }

        যদি (shm_zone[i].shm.size == 0) {
            shm_zone[i].shm.size = size;
        }

        যদি (size && size != shm_zone[i].shm.size) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            size, &shm_zone[i].shm.name, shm_zone[i].shm.size);
            প্রত্যাগম রিক্ত;
        }

        প্রত্যাগম &shm_zone[i];
    }

    shm_zone = ngx_list_ঠেলো(&cf->cycle->shared_memory);

    যদি (shm_zone == রিক্ত) {
        প্রত্যাগম রিক্ত;
    }

    shm_zone->data = রিক্ত;
    shm_zone->shm.log = cf->cycle->log;
    shm_zone->shm.addr = রিক্ত;
    shm_zone->shm.size = size;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = রিক্ত;
    shm_zone->tag = tag;
    shm_zone->noreuse = 0;

    প্রত্যাগম shm_zone;
}


স্থির শূন্য
ngx_clean_old_cycles(ngx_event_t *ev)
{
    ngx_uint_t     i, n, found, live;
    ngx_log_t     *log;
    ngx_cycle_t  **cycle;

    log = ngx_cycle->log;
    ngx_temp_pool->log = log;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, log, 0, "clean old cycles");

    live = 0;

    cycle = ngx_old_cycles.elts;
    প্রতি (i = 0; i < ngx_old_cycles.nelts; i++) {

        যদি (cycle[i] == রিক্ত) {
            অনুবর্ত;
        }

        found = 0;

        প্রতি (n = 0; n < cycle[i]->connection_n; n++) {
            যদি (cycle[i]->connections[n].fd != (ngx_socket_t) -1) {
                found = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "live fd:%ui", n);

                ভঙ্গ;
            }
        }

        যদি (found) {
            live = 1;
            অনুবর্ত;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(cycle[i]->pool);
        cycle[i] = রিক্ত;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "old cycles status: %ui", live);

    যদি (live) {
        ngx_add_timer(ev, 30000);

    } অন্যথা {
        ngx_destroy_pool(ngx_temp_pool);
        ngx_temp_pool = রিক্ত;
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
        ngx_shutdown_event.data = cycle;
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

    cycle = ev->data;

    c = cycle->connections;

    প্রতি (i = 0; i < cycle->connection_n; i++) {

        যদি (c[i].fd == (ngx_socket_t) -1
            || c[i].পঠ == রিক্ত
            || c[i].পঠ->accept
            || c[i].পঠ->channel
            || c[i].পঠ->resolver)
        {
            অনুবর্ত;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "*%uA shutdown timeout", c[i].সংখ্যা);

        c[i].close = 1;
        c[i].error = 1;

        c[i].পঠ->handler(c[i].পঠ);
    }
}
