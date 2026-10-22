
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>
#அந்தர்பாவ <ngx_event.h>


நிலையான வெறுமை ngx_destroy_cycle_pools(ngx_conf_t *conf);
நிலையான ngx_int_t ngx_init_zone_pool(ngx_cycle_t *cycle,
    ngx_shm_zone_t *shm_zone);
நிலையான ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *log);
நிலையான வெறுமை ngx_clean_old_cycles(ngx_event_t *ev);
நிலையான வெறுமை ngx_shutdown_timer_handler(ngx_event_t *ev);


நிலையற்ற ngx_cycle_t  *ngx_cycle;
ngx_array_t            ngx_old_cycles;

நிலையான ngx_pool_t     *ngx_temp_pool;
நிலையான ngx_event_t     ngx_cleaner_event;
நிலையான ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_mode;


/* STUB NAME */
நிலையான ngx_connection_t  dumb;
/* STUB */


ngx_cycle_t *
ngx_init_cycle(ngx_cycle_t *old_cycle)
{
    வெறுமை                *rv, *data;
    எழுத்து               **senv;
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
    எழுத்து                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_time_update();


    log = old_cycle->log;

    pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    எனில் (pool == வெற்று) {
        திரும்பு வெற்று;
    }
    pool->log = log;

    cycle = ngx_pcalloc(pool, அளவு(ngx_cycle_t));
    எனில் (cycle == வெற்று) {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }

    cycle->pool = pool;
    cycle->log = log;
    cycle->old_cycle = old_cycle;

    cycle->conf_prefix.len = old_cycle->conf_prefix.len;
    cycle->conf_prefix.data = ngx_pstrdup(pool, &old_cycle->conf_prefix);
    எனில் (cycle->conf_prefix.data == வெற்று) {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }

    cycle->prefix.len = old_cycle->prefix.len;
    cycle->prefix.data = ngx_pstrdup(pool, &old_cycle->prefix);
    எனில் (cycle->prefix.data == வெற்று) {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }

    cycle->error_log.len = old_cycle->error_log.len;
    cycle->error_log.data = ngx_pnalloc(pool, old_cycle->error_log.len + 1);
    எனில் (cycle->error_log.data == வெற்று) {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }
    ngx_cpystrn(cycle->error_log.data, old_cycle->error_log.data,
                old_cycle->error_log.len + 1);

    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.data = ngx_pnalloc(pool, old_cycle->conf_file.len + 1);
    எனில் (cycle->conf_file.data == வெற்று) {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }
    ngx_cpystrn(cycle->conf_file.data, old_cycle->conf_file.data,
                old_cycle->conf_file.len + 1);

    எனில் (old_cycle->conf_param.len) {
        cycle->conf_param.len = old_cycle->conf_param.len;
        cycle->conf_param.data = ngx_pstrdup(pool, &old_cycle->conf_param);
        எனில் (cycle->conf_param.data == வெற்று) {
            ngx_destroy_pool(pool);
            திரும்பு வெற்று;
        }
    }


    n = old_cycle->paths.nelts ? old_cycle->paths.nelts : 10;

    எனில் (ngx_array_init(&cycle->paths, pool, n, அளவு(ngx_path_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }

    ngx_memzero(cycle->paths.elts, n * அளவு(ngx_path_t *));


    எனில் (ngx_array_init(&cycle->config_dump, pool, 1, அளவு(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }

    ngx_rbtree_init(&cycle->config_dump_rbtree, &cycle->config_dump_sentinel,
                    ngx_str_rbtree_செருகு_மதிப்பு);

    எனில் (old_cycle->open_files.part.nelts) {
        n = old_cycle->open_files.part.nelts;
        க்கு (part = old_cycle->open_files.part.next; part; part = part->next) {
            n += part->nelts;
        }

    } இல்லையெனில் {
        n = 20;
    }

    எனில் (ngx_list_init(&cycle->open_files, pool, n, அளவு(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }


    எனில் (old_cycle->shared_memory.part.nelts) {
        n = old_cycle->shared_memory.part.nelts;
        க்கு (part = old_cycle->shared_memory.part.next; part; part = part->next)
        {
            n += part->nelts;
        }

    } இல்லையெனில் {
        n = 1;
    }

    எனில் (ngx_list_init(&cycle->shared_memory, pool, n, அளவு(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }

    n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;

    எனில் (ngx_array_init(&cycle->listening, pool, n, அளவு(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }

    ngx_memzero(cycle->listening.elts, n * அளவு(ngx_listening_t));


    ngx_queue_init(&cycle->reusable_connections_queue);


    cycle->conf_ctx = ngx_pcalloc(pool, ngx_max_module * அளவு(வெறுமை *));
    எனில் (cycle->conf_ctx == வெற்று) {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }


    எனில் (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    cycle->hostname.len = ngx_strlen(hostname);

    cycle->hostname.data = ngx_pnalloc(pool, cycle->hostname.len);
    எனில் (cycle->hostname.data == வெற்று) {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }

    ngx_strlow(cycle->hostname.data, (u_char *) hostname, cycle->hostname.len);


    எனில் (ngx_cycle_modules(cycle) != NGX_OK) {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }


    க்கு (i = 0; cycle->modules[i]; i++) {
        எனில் (cycle->modules[i]->type != NGX_CORE_MODULE) {
            தொடர்;
        }

        module = cycle->modules[i]->ctx;

        எனில் (module->create_conf) {
            rv = module->create_conf(cycle);
            எனில் (rv == வெற்று) {
                ngx_destroy_pool(pool);
                திரும்பு வெற்று;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, அளவு(ngx_conf_t));
    /* STUB: init array ? */
    conf.args = ngx_array_create(pool, 10, அளவு(ngx_str_t));
    எனில் (conf.args == வெற்று) {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }

    conf.temp_pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    எனில் (conf.temp_pool == வெற்று) {
        ngx_destroy_pool(pool);
        திரும்பு வெற்று;
    }


    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#எனில் 0
    log->log_level = NGX_LOG_DEBUG_ALL;
#endif

    எனில் (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        திரும்பு வெற்று;
    }

    எனில் (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        திரும்பு வெற்று;
    }

    எனில் (ngx_test_config && !ngx_quiet_mode) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       cycle->conf_file.data);
    }

    க்கு (i = 0; cycle->modules[i]; i++) {
        எனில் (cycle->modules[i]->type != NGX_CORE_MODULE) {
            தொடர்;
        }

        module = cycle->modules[i]->ctx;

        எனில் (module->init_conf) {
            எனில் (module->init_conf(cycle,
                                  cycle->conf_ctx[cycle->modules[i]->index])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_cycle_pools(&conf);
                திரும்பு வெற்று;
            }
        }
    }

    எனில் (ngx_process == NGX_PROCESS_SIGNALLER) {
        திரும்பு cycle;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    எனில் (ngx_test_config) {

        எனில் (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
            செல் failed;
        }

    } இல்லையெனில் எனில் (!ngx_is_init_cycle(old_cycle)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        எனில் (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.data, old_ccf->pid.data) != 0)
        {
            /* new pid file name */

            எனில் (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
                செல் failed;
            }

            ngx_delete_pidfile(old_cycle);
        }
    }


    எனில் (ngx_test_lockfile(cycle->lock_file.data, log) != NGX_OK) {
        செல் failed;
    }


    எனில் (ngx_create_paths(cycle, ccf->user) != NGX_OK) {
        செல் failed;
    }


    எனில் (ngx_log_open_default(cycle) != NGX_OK) {
        செல் failed;
    }

    /* open the new files */

    part = &cycle->open_files.part;
    file = part->elts;

    க்கு (i = 0; /* void */ ; i++) {

        எனில் (i >= part->nelts) {
            எனில் (part->next == வெற்று) {
                நிறுத்து;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        எனில் (file[i].name.len == 0) {
            தொடர்;
        }

        file[i].fd = ngx_open_file(file[i].name.data,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, log, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.data);

        எனில் (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.data);
            செல் failed;
        }

#எனில் !(NGX_WIN32)
        எனில் (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);
            செல் failed;
        }
#endif
    }

    cycle->log = &cycle->new_log;
    pool->log = &cycle->new_log;


    /* create shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    க்கு (i = 0; /* void */ ; i++) {

        எனில் (i >= part->nelts) {
            எனில் (part->next == வெற்று) {
                நிறுத்து;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        எனில் (shm_zone[i].shm.size == 0) {
            ngx_log_error(NGX_LOG_EMERG, log, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            செல் failed;
        }

        shm_zone[i].shm.log = cycle->log;

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        data = வெற்று;

        க்கு (n = 0; /* void */ ; n++) {

            எனில் (n >= opart->nelts) {
                எனில் (opart->next == வெற்று) {
                    நிறுத்து;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            எனில் (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                தொடர்;
            }

            எனில் (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                தொடர்;
            }

            எனில் (shm_zone[i].tag == oshm_zone[n].tag && shm_zone[i].noreuse) {
                data = oshm_zone[n].data;
                நிறுத்து;
            }

            எனில் (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#எனில் (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#endif

                எனில் (shm_zone[i].init(&shm_zone[i], oshm_zone[n].data)
                    != NGX_OK)
                {
                    செல் failed;
                }

                செல் shm_zone_found;
            }

            நிறுத்து;
        }

        எனில் (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            செல் failed;
        }

        எனில் (ngx_init_zone_pool(cycle, &shm_zone[i]) != NGX_OK) {
            செல் failed;
        }

        எனில் (shm_zone[i].init(&shm_zone[i], data) != NGX_OK) {
            செல் failed;
        }

    shm_zone_found:

        தொடர்;
    }


    /* handle the listening sockets */

    எனில் (old_cycle->listening.nelts) {
        ls = old_cycle->listening.elts;
        க்கு (i = 0; i < old_cycle->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = cycle->listening.elts;
        க்கு (n = 0; n < cycle->listening.nelts; n++) {

            க்கு (i = 0; i < old_cycle->listening.nelts; i++) {
                எனில் (ls[i].ignore) {
                    தொடர்;
                }

                எனில் (ls[i].remain) {
                    தொடர்;
                }

                எனில் (ls[i].type != nls[n].type) {
                    தொடர்;
                }

                எனில் (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].previous = &ls[i];

                    எனில் (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } இல்லையெனில் {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    எனில் (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#எனில் (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    எனில் (ls[i].accept_filter && nls[n].accept_filter) {
                        எனில் (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].add_deferred = 1;
                        }

                    } இல்லையெனில் எனில் (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } இல்லையெனில் எனில் (nls[n].accept_filter) {
                        nls[n].add_deferred = 1;
                    }
#endif

#எனில் (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    எனில் (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } இல்லையெனில் எனில் (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].add_deferred = 1;
                    }
#endif

#எனில் (NGX_HAVE_REUSEPORT)
                    எனில் (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].add_reuseport = 1;
                    }
#endif

                    நிறுத்து;
                }
            }

            எனில் (nls[n].fd == (ngx_socket_t) -1) {
#எனில் (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                எனில் (nls[n].accept_filter) {
                    nls[n].add_deferred = 1;
                }
#endif
#எனில் (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                எனில் (nls[n].deferred_accept) {
                    nls[n].add_deferred = 1;
                }
#endif
            }
        }

    } இல்லையெனில் {
#எனில் (NGX_HAVE_DEFERRED_ACCEPT)
        ls = cycle->listening.elts;
        க்கு (i = 0; i < cycle->listening.nelts; i++) {
#ifdef SO_ACCEPTFILTER
            எனில் (ls[i].accept_filter) {
                ls[i].add_deferred = 1;
            }
#endif
#ifdef TCP_DEFER_ACCEPT
            எனில் (ls[i].deferred_accept) {
                ls[i].add_deferred = 1;
            }
#endif
        }
#endif
    }

    எனில் (ngx_open_listening_sockets(cycle) != NGX_OK) {
        செல் failed;
    }

    எனில் (!ngx_test_config) {
        ngx_configure_listening_sockets(cycle);
    }


    /* commit the new cycle configuration */

    எனில் (!ngx_use_stderr) {
        (வெறுமை) ngx_log_redirect_stderr(cycle);
    }

    pool->log = cycle->log;

    எனில் (ngx_init_modules(cycle) != NGX_OK) {
        /* fatal */
        exit(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_cycle->shared_memory.part;
    oshm_zone = opart->elts;

    க்கு (i = 0; /* void */ ; i++) {

        எனில் (i >= opart->nelts) {
            எனில் (opart->next == வெற்று) {
                செல் old_shm_zone_done;
            }
            opart = opart->next;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &cycle->shared_memory.part;
        shm_zone = part->elts;

        க்கு (n = 0; /* void */ ; n++) {

            எனில் (n >= part->nelts) {
                எனில் (part->next == வெற்று) {
                    நிறுத்து;
                }
                part = part->next;
                shm_zone = part->elts;
                n = 0;
            }

            எனில் (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                தொடர்;
            }

            எனில் (ngx_strncmp(oshm_zone[i].shm.name.data,
                            shm_zone[n].shm.name.data,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                தொடர்;
            }

            எனில் (oshm_zone[i].tag == shm_zone[n].tag
                && oshm_zone[i].shm.size == shm_zone[n].shm.size
                && !oshm_zone[i].noreuse)
            {
                செல் live_shm_zone;
            }

            நிறுத்து;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    live_shm_zone:

        தொடர்;
    }

old_shm_zone_done:


    /* close the unnecessary listening sockets */

    ls = old_cycle->listening.elts;
    க்கு (i = 0; i < old_cycle->listening.nelts; i++) {

        எனில் (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            தொடர்;
        }

        எனில் (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_text);
        }

#எனில் (NGX_HAVE_UNIX_DOMAIN)

        எனில் (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_text.data + அளவு("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                          "deleting socket %s", name);

            எனில் (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#endif
    }


    /* close the unnecessary open files */

    part = &old_cycle->open_files.part;
    file = part->elts;

    க்கு (i = 0; /* void */ ; i++) {

        எனில் (i >= part->nelts) {
            எனில் (part->next == வெற்று) {
                நிறுத்து;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        எனில் (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            தொடர்;
        }

        எனில் (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    ngx_destroy_pool(conf.temp_pool);

    எனில் (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_cycle(old_cycle)) {

        ngx_destroy_pool(old_cycle->pool);
        cycle->old_cycle = வெற்று;

        திரும்பு cycle;
    }


    எனில் (ngx_temp_pool == வெற்று) {
        ngx_temp_pool = ngx_create_pool(128, cycle->log);
        எனில் (ngx_temp_pool == வெற்று) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "could not create ngx_temp_pool");
            exit(1);
        }

        n = 10;

        எனில் (ngx_array_init(&ngx_old_cycles, ngx_temp_pool, n,
                           அளவு(ngx_cycle_t *))
            != NGX_OK)
        {
            exit(1);
        }

        ngx_memzero(ngx_old_cycles.elts, n * அளவு(ngx_cycle_t *));

        ngx_cleaner_event.handler = ngx_clean_old_cycles;
        ngx_cleaner_event.log = cycle->log;
        ngx_cleaner_event.data = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_temp_pool->log = cycle->log;

    old = ngx_array_தள்ளு(&ngx_old_cycles);
    எனில் (old == வெற்று) {
        exit(1);
    }
    *old = old_cycle;

    எனில் (!ngx_cleaner_event.timer_set) {
        ngx_add_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_set = 1;
    }

    திரும்பு cycle;


failed:

    எனில் (!ngx_is_init_cycle(old_cycle)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        எனில் (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &cycle->open_files.part;
    file = part->elts;

    க்கு (i = 0; /* void */ ; i++) {

        எனில் (i >= part->nelts) {
            எனில் (part->next == வெற்று) {
                நிறுத்து;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        எனில் (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            தொடர்;
        }

        எனில் (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    /* free the newly created shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    க்கு (i = 0; /* void */ ; i++) {

        எனில் (i >= part->nelts) {
            எனில் (part->next == வெற்று) {
                நிறுத்து;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        எனில் (shm_zone[i].shm.addr == வெற்று) {
            தொடர்;
        }

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        க்கு (n = 0; /* void */ ; n++) {

            எனில் (n >= opart->nelts) {
                எனில் (opart->next == வெற்று) {
                    நிறுத்து;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            எனில் (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                தொடர்;
            }

            எனில் (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                தொடர்;
            }

            எனில் (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size
                && !shm_zone[i].noreuse)
            {
                செல் old_shm_zone_found;
            }

            நிறுத்து;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_found:

        தொடர்;
    }

    எனில் (ngx_test_config) {
        ngx_destroy_cycle_pools(&conf);
        திரும்பு வெற்று;
    }

    ls = cycle->listening.elts;
    க்கு (i = 0; i < cycle->listening.nelts; i++) {
        எனில் (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            தொடர்;
        }

        எனில் (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_text);
        }
    }

    ngx_destroy_cycle_pools(&conf);

    திரும்பு வெற்று;
}


நிலையான வெறுமை
ngx_destroy_cycle_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->temp_pool);
    ngx_destroy_pool(conf->pool);
}


நிலையான ngx_int_t
ngx_init_zone_pool(ngx_cycle_t *cycle, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    எனில் (zn->shm.exists) {

        எனில் (sp == sp->addr) {
            திரும்பு NGX_OK;
        }

#எனில் (NGX_WIN32)

        /* remap at the required address */

        எனில் (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            திரும்பு NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        எனில் (sp == sp->addr) {
            திரும்பு NGX_OK;
        }

#endif

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        திரும்பு NGX_ERROR;
    }

    sp->end = zn->shm.addr + zn->shm.size;
    sp->min_shift = 3;
    sp->addr = zn->shm.addr;

#எனில் (NGX_HAVE_ATOMIC_OPS)

    file = வெற்று;

#இல்லையெனில்

    file = ngx_pnalloc(cycle->pool,
                       cycle->lock_file.len + zn->shm.name.len + 1);
    எனில் (file == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    (வெறுமை) ngx_sprintf(file, "%V%V%Z", &cycle->lock_file, &zn->shm.name);

#endif

    எனில் (ngx_shmtx_create(&sp->mutex, &sp->lock, file) != NGX_OK) {
        திரும்பு NGX_ERROR;
    }

    ngx_slab_init(sp);

    திரும்பு NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log)
{
    size_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    எனில் (ngx_process > NGX_PROCESS_MASTER) {
        திரும்பு NGX_OK;
    }

    ngx_memzero(&file, அளவு(ngx_file_t));

    file.name = *name;
    file.log = log;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    எனில் (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        திரும்பு NGX_ERROR;
    }

    rc = NGX_OK;

    எனில் (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        எனில் (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    எனில் (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    திரும்பு rc;
}


வெறுமை
ngx_delete_pidfile(ngx_cycle_t *cycle)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    name = ngx_new_இருமம் ? ccf->oldpid.data : ccf->pid.data;

    எனில் (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_cycle_t *cycle, எழுத்து *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ngx_memzero(&file, அளவு(ngx_file_t));

    file.name = ccf->pid;
    file.log = cycle->log;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    எனில் (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        திரும்பு 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    எனில் (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    எனில் (n == NGX_ERROR) {
        திரும்பு 1;
    }

    போது (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    எனில் (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.data);
        திரும்பு 1;
    }

    திரும்பு ngx_os_signal_process(cycle, sig, pid);

}


நிலையான ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *log)
{
#எனில் !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    எனில் (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        திரும்பு NGX_ERROR;
    }

    எனில் (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    எனில் (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#endif

    திரும்பு NGX_OK;
}


வெறுமை
ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &cycle->open_files.part;
    file = part->elts;

    க்கு (i = 0; /* void */ ; i++) {

        எனில் (i >= part->nelts) {
            எனில் (part->next == வெற்று) {
                நிறுத்து;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        எனில் (file[i].name.len == 0) {
            தொடர்;
        }

        எனில் (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }

        fd = ngx_open_file(file[i].name.data, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.data, file[i].fd, fd);

        எனில் (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.data);
            தொடர்;
        }

#எனில் !(NGX_WIN32)
        எனில் (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            எனில் (ngx_file_info(file[i].name.data, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.data);

                எனில் (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.data);
                }

                தொடர்;
            }

            எனில் (fi.st_uid != user) {
                எனில் (chown((மாறிலி எழுத்து *) file[i].name.data, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.data, user);

                    எனில் (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    தொடர்;
                }
            }

            எனில் ((fi.st_mode & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_mode |= (S_IRUSR|S_IWUSR);

                எனில் (chmod((மாறிலி எழுத்து *) file[i].name.data, fi.st_mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.data);

                    எனில் (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    தொடர்;
                }
            }
        }

        எனில் (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);

            எனில் (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.data);
            }

            தொடர்;
        }
#endif

        எனில் (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }

        file[i].fd = fd;
    }

    (வெறுமை) ngx_log_redirect_stderr(cycle);
}


ngx_shm_zone_t *
ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name, size_t size, வெறுமை *tag)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->cycle->shared_memory.part;
    shm_zone = part->elts;

    க்கு (i = 0; /* void */ ; i++) {

        எனில் (i >= part->nelts) {
            எனில் (part->next == வெற்று) {
                நிறுத்து;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        எனில் (name->len != shm_zone[i].shm.name.len) {
            தொடர்;
        }

        எனில் (ngx_strncmp(name->data, shm_zone[i].shm.name.data, name->len)
            != 0)
        {
            தொடர்;
        }

        எனில் (tag != shm_zone[i].tag) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            திரும்பு வெற்று;
        }

        எனில் (shm_zone[i].shm.size == 0) {
            shm_zone[i].shm.size = size;
        }

        எனில் (size && size != shm_zone[i].shm.size) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            size, &shm_zone[i].shm.name, shm_zone[i].shm.size);
            திரும்பு வெற்று;
        }

        திரும்பு &shm_zone[i];
    }

    shm_zone = ngx_list_தள்ளு(&cf->cycle->shared_memory);

    எனில் (shm_zone == வெற்று) {
        திரும்பு வெற்று;
    }

    shm_zone->data = வெற்று;
    shm_zone->shm.log = cf->cycle->log;
    shm_zone->shm.addr = வெற்று;
    shm_zone->shm.size = size;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = வெற்று;
    shm_zone->tag = tag;
    shm_zone->noreuse = 0;

    திரும்பு shm_zone;
}


நிலையான வெறுமை
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
    க்கு (i = 0; i < ngx_old_cycles.nelts; i++) {

        எனில் (cycle[i] == வெற்று) {
            தொடர்;
        }

        found = 0;

        க்கு (n = 0; n < cycle[i]->connection_n; n++) {
            எனில் (cycle[i]->connections[n].fd != (ngx_socket_t) -1) {
                found = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "live fd:%ui", n);

                நிறுத்து;
            }
        }

        எனில் (found) {
            live = 1;
            தொடர்;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(cycle[i]->pool);
        cycle[i] = வெற்று;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "old cycles status: %ui", live);

    எனில் (live) {
        ngx_add_timer(ev, 30000);

    } இல்லையெனில் {
        ngx_destroy_pool(ngx_temp_pool);
        ngx_temp_pool = வெற்று;
        ngx_old_cycles.nelts = 0;
    }
}


வெறுமை
ngx_set_shutdown_timer(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    எனில் (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.data = cycle;
        ngx_shutdown_event.log = cycle->log;
        ngx_shutdown_event.cancelable = 1;

        ngx_add_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


நிலையான வெறுமை
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_cycle_t       *cycle;
    ngx_connection_t  *c;

    cycle = ev->data;

    c = cycle->connections;

    க்கு (i = 0; i < cycle->connection_n; i++) {

        எனில் (c[i].fd == (ngx_socket_t) -1
            || c[i].பட == வெற்று
            || c[i].பட->accept
            || c[i].பட->channel
            || c[i].பட->resolver)
        {
            தொடர்;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "*%uA shutdown timeout", c[i].எண்);

        c[i].close = 1;
        c[i].error = 1;

        c[i].பட->handler(c[i].பட);
    }
}
