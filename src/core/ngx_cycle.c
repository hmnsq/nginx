
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_event.h>


ایستا تهی ngx_destroy_cycle_pools(ngx_conf_t *conf);
ایستا ngx_int_t ngx_init_zone_pool(ngx_cycle_t *cycle,
    ngx_shm_zone_t *shm_zone);
ایستا ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *log);
ایستا تهی ngx_clean_old_cycles(ngx_event_t *ev);
ایستا تهی ngx_shutdown_timer_handler(ngx_event_t *ev);


فرار ngx_cycle_t  *ngx_cycle;
ngx_array_t            ngx_old_cycles;

ایستا ngx_pool_t     *ngx_temp_pool;
ایستا ngx_event_t     ngx_cleaner_event;
ایستا ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_mode;


/* STUB NAME */
ایستا ngx_connection_t  dumb;
/* STUB */


ngx_cycle_t *
ngx_init_cycle(ngx_cycle_t *old_cycle)
{
    تهی                *rv, *data;
    نویسه               **senv;
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
    نویسه                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_time_update();


    log = old_cycle->log;

    pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    اگر (pool == پوچ) {
        بازگشت پوچ;
    }
    pool->log = log;

    cycle = ngx_pcalloc(pool, اندازه(ngx_cycle_t));
    اگر (cycle == پوچ) {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }

    cycle->pool = pool;
    cycle->log = log;
    cycle->old_cycle = old_cycle;

    cycle->conf_prefix.len = old_cycle->conf_prefix.len;
    cycle->conf_prefix.data = ngx_pstrdup(pool, &old_cycle->conf_prefix);
    اگر (cycle->conf_prefix.data == پوچ) {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }

    cycle->prefix.len = old_cycle->prefix.len;
    cycle->prefix.data = ngx_pstrdup(pool, &old_cycle->prefix);
    اگر (cycle->prefix.data == پوچ) {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }

    cycle->error_log.len = old_cycle->error_log.len;
    cycle->error_log.data = ngx_pnalloc(pool, old_cycle->error_log.len + 1);
    اگر (cycle->error_log.data == پوچ) {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }
    ngx_cpystrn(cycle->error_log.data, old_cycle->error_log.data,
                old_cycle->error_log.len + 1);

    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.data = ngx_pnalloc(pool, old_cycle->conf_file.len + 1);
    اگر (cycle->conf_file.data == پوچ) {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }
    ngx_cpystrn(cycle->conf_file.data, old_cycle->conf_file.data,
                old_cycle->conf_file.len + 1);

    اگر (old_cycle->conf_param.len) {
        cycle->conf_param.len = old_cycle->conf_param.len;
        cycle->conf_param.data = ngx_pstrdup(pool, &old_cycle->conf_param);
        اگر (cycle->conf_param.data == پوچ) {
            ngx_destroy_pool(pool);
            بازگشت پوچ;
        }
    }


    n = old_cycle->paths.nelts ? old_cycle->paths.nelts : 10;

    اگر (ngx_array_init(&cycle->paths, pool, n, اندازه(ngx_path_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }

    ngx_memzero(cycle->paths.elts, n * اندازه(ngx_path_t *));


    اگر (ngx_array_init(&cycle->config_dump, pool, 1, اندازه(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }

    ngx_rbtree_init(&cycle->config_dump_rbtree, &cycle->config_dump_sentinel,
                    ngx_str_rbtree_درج_مقدار);

    اگر (old_cycle->open_files.part.nelts) {
        n = old_cycle->open_files.part.nelts;
        برای (part = old_cycle->open_files.part.next; part; part = part->next) {
            n += part->nelts;
        }

    } وگرنه {
        n = 20;
    }

    اگر (ngx_list_init(&cycle->open_files, pool, n, اندازه(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }


    اگر (old_cycle->shared_memory.part.nelts) {
        n = old_cycle->shared_memory.part.nelts;
        برای (part = old_cycle->shared_memory.part.next; part; part = part->next)
        {
            n += part->nelts;
        }

    } وگرنه {
        n = 1;
    }

    اگر (ngx_list_init(&cycle->shared_memory, pool, n, اندازه(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }

    n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;

    اگر (ngx_array_init(&cycle->listening, pool, n, اندازه(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }

    ngx_memzero(cycle->listening.elts, n * اندازه(ngx_listening_t));


    ngx_queue_init(&cycle->reusable_connections_queue);


    cycle->conf_ctx = ngx_pcalloc(pool, ngx_max_module * اندازه(تهی *));
    اگر (cycle->conf_ctx == پوچ) {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }


    اگر (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    cycle->hostname.len = ngx_strlen(hostname);

    cycle->hostname.data = ngx_pnalloc(pool, cycle->hostname.len);
    اگر (cycle->hostname.data == پوچ) {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }

    ngx_strlow(cycle->hostname.data, (u_char *) hostname, cycle->hostname.len);


    اگر (ngx_cycle_modules(cycle) != NGX_OK) {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }


    برای (i = 0; cycle->modules[i]; i++) {
        اگر (cycle->modules[i]->type != NGX_CORE_MODULE) {
            ادامه;
        }

        module = cycle->modules[i]->ctx;

        اگر (module->create_conf) {
            rv = module->create_conf(cycle);
            اگر (rv == پوچ) {
                ngx_destroy_pool(pool);
                بازگشت پوچ;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, اندازه(ngx_conf_t));
    /* STUB: init array ? */
    conf.args = ngx_array_create(pool, 10, اندازه(ngx_str_t));
    اگر (conf.args == پوچ) {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }

    conf.temp_pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    اگر (conf.temp_pool == پوچ) {
        ngx_destroy_pool(pool);
        بازگشت پوچ;
    }


    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#اگر 0
    log->log_level = NGX_LOG_DEBUG_ALL;
#endif

    اگر (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        بازگشت پوچ;
    }

    اگر (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        بازگشت پوچ;
    }

    اگر (ngx_test_config && !ngx_quiet_mode) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       cycle->conf_file.data);
    }

    برای (i = 0; cycle->modules[i]; i++) {
        اگر (cycle->modules[i]->type != NGX_CORE_MODULE) {
            ادامه;
        }

        module = cycle->modules[i]->ctx;

        اگر (module->init_conf) {
            اگر (module->init_conf(cycle,
                                  cycle->conf_ctx[cycle->modules[i]->index])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_cycle_pools(&conf);
                بازگشت پوچ;
            }
        }
    }

    اگر (ngx_process == NGX_PROCESS_SIGNALLER) {
        بازگشت cycle;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    اگر (ngx_test_config) {

        اگر (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
            برو failed;
        }

    } وگرنه اگر (!ngx_is_init_cycle(old_cycle)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        اگر (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.data, old_ccf->pid.data) != 0)
        {
            /* new pid file name */

            اگر (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
                برو failed;
            }

            ngx_delete_pidfile(old_cycle);
        }
    }


    اگر (ngx_test_lockfile(cycle->lock_file.data, log) != NGX_OK) {
        برو failed;
    }


    اگر (ngx_create_paths(cycle, ccf->user) != NGX_OK) {
        برو failed;
    }


    اگر (ngx_log_open_default(cycle) != NGX_OK) {
        برو failed;
    }

    /* open the new files */

    part = &cycle->open_files.part;
    file = part->elts;

    برای (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->next == پوچ) {
                بشکن;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        اگر (file[i].name.len == 0) {
            ادامه;
        }

        file[i].fd = ngx_open_file(file[i].name.data,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, log, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.data);

        اگر (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.data);
            برو failed;
        }

#اگر !(NGX_WIN32)
        اگر (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);
            برو failed;
        }
#endif
    }

    cycle->log = &cycle->new_log;
    pool->log = &cycle->new_log;


    /* create shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    برای (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->next == پوچ) {
                بشکن;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        اگر (shm_zone[i].shm.size == 0) {
            ngx_log_error(NGX_LOG_EMERG, log, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            برو failed;
        }

        shm_zone[i].shm.log = cycle->log;

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        data = پوچ;

        برای (n = 0; /* void */ ; n++) {

            اگر (n >= opart->nelts) {
                اگر (opart->next == پوچ) {
                    بشکن;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            اگر (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                ادامه;
            }

            اگر (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                ادامه;
            }

            اگر (shm_zone[i].tag == oshm_zone[n].tag && shm_zone[i].noreuse) {
                data = oshm_zone[n].data;
                بشکن;
            }

            اگر (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#اگر (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#endif

                اگر (shm_zone[i].init(&shm_zone[i], oshm_zone[n].data)
                    != NGX_OK)
                {
                    برو failed;
                }

                برو shm_zone_found;
            }

            بشکن;
        }

        اگر (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            برو failed;
        }

        اگر (ngx_init_zone_pool(cycle, &shm_zone[i]) != NGX_OK) {
            برو failed;
        }

        اگر (shm_zone[i].init(&shm_zone[i], data) != NGX_OK) {
            برو failed;
        }

    shm_zone_found:

        ادامه;
    }


    /* handle the listening sockets */

    اگر (old_cycle->listening.nelts) {
        ls = old_cycle->listening.elts;
        برای (i = 0; i < old_cycle->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = cycle->listening.elts;
        برای (n = 0; n < cycle->listening.nelts; n++) {

            برای (i = 0; i < old_cycle->listening.nelts; i++) {
                اگر (ls[i].ignore) {
                    ادامه;
                }

                اگر (ls[i].remain) {
                    ادامه;
                }

                اگر (ls[i].type != nls[n].type) {
                    ادامه;
                }

                اگر (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].previous = &ls[i];

                    اگر (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } وگرنه {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    اگر (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    اگر (ls[i].accept_filter && nls[n].accept_filter) {
                        اگر (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].add_deferred = 1;
                        }

                    } وگرنه اگر (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } وگرنه اگر (nls[n].accept_filter) {
                        nls[n].add_deferred = 1;
                    }
#endif

#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    اگر (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } وگرنه اگر (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].add_deferred = 1;
                    }
#endif

#اگر (NGX_HAVE_REUSEPORT)
                    اگر (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].add_reuseport = 1;
                    }
#endif

                    بشکن;
                }
            }

            اگر (nls[n].fd == (ngx_socket_t) -1) {
#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                اگر (nls[n].accept_filter) {
                    nls[n].add_deferred = 1;
                }
#endif
#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                اگر (nls[n].deferred_accept) {
                    nls[n].add_deferred = 1;
                }
#endif
            }
        }

    } وگرنه {
#اگر (NGX_HAVE_DEFERRED_ACCEPT)
        ls = cycle->listening.elts;
        برای (i = 0; i < cycle->listening.nelts; i++) {
#ifdef SO_ACCEPTFILTER
            اگر (ls[i].accept_filter) {
                ls[i].add_deferred = 1;
            }
#endif
#ifdef TCP_DEFER_ACCEPT
            اگر (ls[i].deferred_accept) {
                ls[i].add_deferred = 1;
            }
#endif
        }
#endif
    }

    اگر (ngx_open_listening_sockets(cycle) != NGX_OK) {
        برو failed;
    }

    اگر (!ngx_test_config) {
        ngx_configure_listening_sockets(cycle);
    }


    /* commit the new cycle configuration */

    اگر (!ngx_use_stderr) {
        (تهی) ngx_log_redirect_stderr(cycle);
    }

    pool->log = cycle->log;

    اگر (ngx_init_modules(cycle) != NGX_OK) {
        /* fatal */
        exit(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_cycle->shared_memory.part;
    oshm_zone = opart->elts;

    برای (i = 0; /* void */ ; i++) {

        اگر (i >= opart->nelts) {
            اگر (opart->next == پوچ) {
                برو old_shm_zone_done;
            }
            opart = opart->next;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &cycle->shared_memory.part;
        shm_zone = part->elts;

        برای (n = 0; /* void */ ; n++) {

            اگر (n >= part->nelts) {
                اگر (part->next == پوچ) {
                    بشکن;
                }
                part = part->next;
                shm_zone = part->elts;
                n = 0;
            }

            اگر (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                ادامه;
            }

            اگر (ngx_strncmp(oshm_zone[i].shm.name.data,
                            shm_zone[n].shm.name.data,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                ادامه;
            }

            اگر (oshm_zone[i].tag == shm_zone[n].tag
                && oshm_zone[i].shm.size == shm_zone[n].shm.size
                && !oshm_zone[i].noreuse)
            {
                برو live_shm_zone;
            }

            بشکن;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    live_shm_zone:

        ادامه;
    }

old_shm_zone_done:


    /* close the unnecessary listening sockets */

    ls = old_cycle->listening.elts;
    برای (i = 0; i < old_cycle->listening.nelts; i++) {

        اگر (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            ادامه;
        }

        اگر (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_text);
        }

#اگر (NGX_HAVE_UNIX_DOMAIN)

        اگر (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_text.data + اندازه("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                          "deleting socket %s", name);

            اگر (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#endif
    }


    /* close the unnecessary open files */

    part = &old_cycle->open_files.part;
    file = part->elts;

    برای (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->next == پوچ) {
                بشکن;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        اگر (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            ادامه;
        }

        اگر (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    ngx_destroy_pool(conf.temp_pool);

    اگر (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_cycle(old_cycle)) {

        ngx_destroy_pool(old_cycle->pool);
        cycle->old_cycle = پوچ;

        بازگشت cycle;
    }


    اگر (ngx_temp_pool == پوچ) {
        ngx_temp_pool = ngx_create_pool(128, cycle->log);
        اگر (ngx_temp_pool == پوچ) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "could not create ngx_temp_pool");
            exit(1);
        }

        n = 10;

        اگر (ngx_array_init(&ngx_old_cycles, ngx_temp_pool, n,
                           اندازه(ngx_cycle_t *))
            != NGX_OK)
        {
            exit(1);
        }

        ngx_memzero(ngx_old_cycles.elts, n * اندازه(ngx_cycle_t *));

        ngx_cleaner_event.handler = ngx_clean_old_cycles;
        ngx_cleaner_event.log = cycle->log;
        ngx_cleaner_event.data = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_temp_pool->log = cycle->log;

    old = ngx_array_افزودن(&ngx_old_cycles);
    اگر (old == پوچ) {
        exit(1);
    }
    *old = old_cycle;

    اگر (!ngx_cleaner_event.timer_set) {
        ngx_add_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_set = 1;
    }

    بازگشت cycle;


failed:

    اگر (!ngx_is_init_cycle(old_cycle)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        اگر (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &cycle->open_files.part;
    file = part->elts;

    برای (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->next == پوچ) {
                بشکن;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        اگر (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            ادامه;
        }

        اگر (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    /* free the newly created shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    برای (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->next == پوچ) {
                بشکن;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        اگر (shm_zone[i].shm.addr == پوچ) {
            ادامه;
        }

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        برای (n = 0; /* void */ ; n++) {

            اگر (n >= opart->nelts) {
                اگر (opart->next == پوچ) {
                    بشکن;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            اگر (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                ادامه;
            }

            اگر (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                ادامه;
            }

            اگر (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size
                && !shm_zone[i].noreuse)
            {
                برو old_shm_zone_found;
            }

            بشکن;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_found:

        ادامه;
    }

    اگر (ngx_test_config) {
        ngx_destroy_cycle_pools(&conf);
        بازگشت پوچ;
    }

    ls = cycle->listening.elts;
    برای (i = 0; i < cycle->listening.nelts; i++) {
        اگر (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            ادامه;
        }

        اگر (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_text);
        }
    }

    ngx_destroy_cycle_pools(&conf);

    بازگشت پوچ;
}


ایستا تهی
ngx_destroy_cycle_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->temp_pool);
    ngx_destroy_pool(conf->pool);
}


ایستا ngx_int_t
ngx_init_zone_pool(ngx_cycle_t *cycle, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    اگر (zn->shm.exists) {

        اگر (sp == sp->addr) {
            بازگشت NGX_OK;
        }

#اگر (NGX_WIN32)

        /* remap at the required address */

        اگر (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            بازگشت NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        اگر (sp == sp->addr) {
            بازگشت NGX_OK;
        }

#endif

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        بازگشت NGX_ERROR;
    }

    sp->end = zn->shm.addr + zn->shm.size;
    sp->min_shift = 3;
    sp->addr = zn->shm.addr;

#اگر (NGX_HAVE_ATOMIC_OPS)

    file = پوچ;

#وگرنه

    file = ngx_pnalloc(cycle->pool,
                       cycle->lock_file.len + zn->shm.name.len + 1);
    اگر (file == پوچ) {
        بازگشت NGX_ERROR;
    }

    (تهی) ngx_sprintf(file, "%V%V%Z", &cycle->lock_file, &zn->shm.name);

#endif

    اگر (ngx_shmtx_create(&sp->mutex, &sp->lock, file) != NGX_OK) {
        بازگشت NGX_ERROR;
    }

    ngx_slab_init(sp);

    بازگشت NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log)
{
    size_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    اگر (ngx_process > NGX_PROCESS_MASTER) {
        بازگشت NGX_OK;
    }

    ngx_memzero(&file, اندازه(ngx_file_t));

    file.name = *name;
    file.log = log;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    اگر (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        بازگشت NGX_ERROR;
    }

    rc = NGX_OK;

    اگر (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        اگر (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    اگر (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    بازگشت rc;
}


تهی
ngx_delete_pidfile(ngx_cycle_t *cycle)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    name = ngx_new_دودویی ? ccf->oldpid.data : ccf->pid.data;

    اگر (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_cycle_t *cycle, نویسه *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ngx_memzero(&file, اندازه(ngx_file_t));

    file.name = ccf->pid;
    file.log = cycle->log;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    اگر (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        بازگشت 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    اگر (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    اگر (n == NGX_ERROR) {
        بازگشت 1;
    }

    وقتی (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    اگر (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.data);
        بازگشت 1;
    }

    بازگشت ngx_os_signal_process(cycle, sig, pid);

}


ایستا ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *log)
{
#اگر !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    اگر (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        بازگشت NGX_ERROR;
    }

    اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    اگر (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#endif

    بازگشت NGX_OK;
}


تهی
ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &cycle->open_files.part;
    file = part->elts;

    برای (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->next == پوچ) {
                بشکن;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        اگر (file[i].name.len == 0) {
            ادامه;
        }

        اگر (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }

        fd = ngx_open_file(file[i].name.data, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.data, file[i].fd, fd);

        اگر (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.data);
            ادامه;
        }

#اگر !(NGX_WIN32)
        اگر (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            اگر (ngx_file_info(file[i].name.data, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.data);

                اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.data);
                }

                ادامه;
            }

            اگر (fi.st_uid != user) {
                اگر (chown((ثابت نویسه *) file[i].name.data, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.data, user);

                    اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    ادامه;
                }
            }

            اگر ((fi.st_mode & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_mode |= (S_IRUSR|S_IWUSR);

                اگر (chmod((ثابت نویسه *) file[i].name.data, fi.st_mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.data);

                    اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    ادامه;
                }
            }
        }

        اگر (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);

            اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.data);
            }

            ادامه;
        }
#endif

        اگر (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }

        file[i].fd = fd;
    }

    (تهی) ngx_log_redirect_stderr(cycle);
}


ngx_shm_zone_t *
ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name, size_t size, تهی *tag)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->cycle->shared_memory.part;
    shm_zone = part->elts;

    برای (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->next == پوچ) {
                بشکن;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        اگر (name->len != shm_zone[i].shm.name.len) {
            ادامه;
        }

        اگر (ngx_strncmp(name->data, shm_zone[i].shm.name.data, name->len)
            != 0)
        {
            ادامه;
        }

        اگر (tag != shm_zone[i].tag) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            بازگشت پوچ;
        }

        اگر (shm_zone[i].shm.size == 0) {
            shm_zone[i].shm.size = size;
        }

        اگر (size && size != shm_zone[i].shm.size) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            size, &shm_zone[i].shm.name, shm_zone[i].shm.size);
            بازگشت پوچ;
        }

        بازگشت &shm_zone[i];
    }

    shm_zone = ngx_list_افزودن(&cf->cycle->shared_memory);

    اگر (shm_zone == پوچ) {
        بازگشت پوچ;
    }

    shm_zone->data = پوچ;
    shm_zone->shm.log = cf->cycle->log;
    shm_zone->shm.addr = پوچ;
    shm_zone->shm.size = size;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = پوچ;
    shm_zone->tag = tag;
    shm_zone->noreuse = 0;

    بازگشت shm_zone;
}


ایستا تهی
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
    برای (i = 0; i < ngx_old_cycles.nelts; i++) {

        اگر (cycle[i] == پوچ) {
            ادامه;
        }

        found = 0;

        برای (n = 0; n < cycle[i]->connection_n; n++) {
            اگر (cycle[i]->connections[n].fd != (ngx_socket_t) -1) {
                found = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "live fd:%ui", n);

                بشکن;
            }
        }

        اگر (found) {
            live = 1;
            ادامه;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(cycle[i]->pool);
        cycle[i] = پوچ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "old cycles status: %ui", live);

    اگر (live) {
        ngx_add_timer(ev, 30000);

    } وگرنه {
        ngx_destroy_pool(ngx_temp_pool);
        ngx_temp_pool = پوچ;
        ngx_old_cycles.nelts = 0;
    }
}


تهی
ngx_set_shutdown_timer(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    اگر (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.data = cycle;
        ngx_shutdown_event.log = cycle->log;
        ngx_shutdown_event.cancelable = 1;

        ngx_add_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


ایستا تهی
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_cycle_t       *cycle;
    ngx_connection_t  *c;

    cycle = ev->data;

    c = cycle->connections;

    برای (i = 0; i < cycle->connection_n; i++) {

        اگر (c[i].fd == (ngx_socket_t) -1
            || c[i].اقرأ == پوچ
            || c[i].اقرأ->accept
            || c[i].اقرأ->channel
            || c[i].اقرأ->resolver)
        {
            ادامه;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "*%uA shutdown timeout", c[i].عدد);

        c[i].close = 1;
        c[i].error = 1;

        c[i].اقرأ->handler(c[i].اقرأ);
    }
}
