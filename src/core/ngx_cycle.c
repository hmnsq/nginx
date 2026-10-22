
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>
#ଅନ୍ତର୍ଭାଵ <ngx_event.h>


ସ୍ଥିର ଶୂନ୍ୟ ngx_destroy_cycle_pools(ngx_conf_t *conf);
ସ୍ଥିର ngx_int_t ngx_init_zone_pool(ngx_cycle_t *cycle,
    ngx_shm_zone_t *shm_zone);
ସ୍ଥିର ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *log);
ସ୍ଥିର ଶୂନ୍ୟ ngx_clean_old_cycles(ngx_event_t *ev);
ସ୍ଥିର ଶୂନ୍ୟ ngx_shutdown_timer_handler(ngx_event_t *ev);


ଅସ୍ଥିର ngx_cycle_t  *ngx_cycle;
ngx_array_t            ngx_old_cycles;

ସ୍ଥିର ngx_pool_t     *ngx_temp_pool;
ସ୍ଥିର ngx_event_t     ngx_cleaner_event;
ସ୍ଥିର ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_mode;


/* STUB NAME */
ସ୍ଥିର ngx_connection_t  dumb;
/* STUB */


ngx_cycle_t *
ngx_init_cycle(ngx_cycle_t *old_cycle)
{
    ଶୂନ୍ୟ                *rv, *data;
    ଅକ୍ଷର               **senv;
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
    ଅକ୍ଷର                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_time_update();


    log = old_cycle->log;

    pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    ଯଦି (pool == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }
    pool->log = log;

    cycle = ngx_pcalloc(pool, ଆକାର(ngx_cycle_t));
    ଯଦି (cycle == ଖାଲି) {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    cycle->pool = pool;
    cycle->log = log;
    cycle->old_cycle = old_cycle;

    cycle->conf_prefix.len = old_cycle->conf_prefix.len;
    cycle->conf_prefix.data = ngx_pstrdup(pool, &old_cycle->conf_prefix);
    ଯଦି (cycle->conf_prefix.data == ଖାଲି) {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    cycle->prefix.len = old_cycle->prefix.len;
    cycle->prefix.data = ngx_pstrdup(pool, &old_cycle->prefix);
    ଯଦି (cycle->prefix.data == ଖାଲି) {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    cycle->error_log.len = old_cycle->error_log.len;
    cycle->error_log.data = ngx_pnalloc(pool, old_cycle->error_log.len + 1);
    ଯଦି (cycle->error_log.data == ଖାଲି) {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }
    ngx_cpystrn(cycle->error_log.data, old_cycle->error_log.data,
                old_cycle->error_log.len + 1);

    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.data = ngx_pnalloc(pool, old_cycle->conf_file.len + 1);
    ଯଦି (cycle->conf_file.data == ଖାଲି) {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }
    ngx_cpystrn(cycle->conf_file.data, old_cycle->conf_file.data,
                old_cycle->conf_file.len + 1);

    ଯଦି (old_cycle->conf_param.len) {
        cycle->conf_param.len = old_cycle->conf_param.len;
        cycle->conf_param.data = ngx_pstrdup(pool, &old_cycle->conf_param);
        ଯଦି (cycle->conf_param.data == ଖାଲି) {
            ngx_destroy_pool(pool);
            ଫେରନ୍ତୁ ଖାଲି;
        }
    }


    n = old_cycle->paths.nelts ? old_cycle->paths.nelts : 10;

    ଯଦି (ngx_array_init(&cycle->paths, pool, n, ଆକାର(ngx_path_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ngx_memzero(cycle->paths.elts, n * ଆକାର(ngx_path_t *));


    ଯଦି (ngx_array_init(&cycle->config_dump, pool, 1, ଆକାର(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ngx_rbtree_init(&cycle->config_dump_rbtree, &cycle->config_dump_sentinel,
                    ngx_str_rbtree_ଭର୍ତ୍ତି_ମୂଲ୍ୟ);

    ଯଦି (old_cycle->open_files.part.nelts) {
        n = old_cycle->open_files.part.nelts;
        ପାଇଁ (part = old_cycle->open_files.part.next; part; part = part->next) {
            n += part->nelts;
        }

    } ଅନ୍ୟଥା {
        n = 20;
    }

    ଯଦି (ngx_list_init(&cycle->open_files, pool, n, ଆକାର(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }


    ଯଦି (old_cycle->shared_memory.part.nelts) {
        n = old_cycle->shared_memory.part.nelts;
        ପାଇଁ (part = old_cycle->shared_memory.part.next; part; part = part->next)
        {
            n += part->nelts;
        }

    } ଅନ୍ୟଥା {
        n = 1;
    }

    ଯଦି (ngx_list_init(&cycle->shared_memory, pool, n, ଆକାର(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;

    ଯଦି (ngx_array_init(&cycle->listening, pool, n, ଆକାର(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ngx_memzero(cycle->listening.elts, n * ଆକାର(ngx_listening_t));


    ngx_queue_init(&cycle->reusable_connections_queue);


    cycle->conf_ctx = ngx_pcalloc(pool, ngx_max_module * ଆକାର(ଶୂନ୍ୟ *));
    ଯଦି (cycle->conf_ctx == ଖାଲି) {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }


    ଯଦି (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    cycle->hostname.len = ngx_strlen(hostname);

    cycle->hostname.data = ngx_pnalloc(pool, cycle->hostname.len);
    ଯଦି (cycle->hostname.data == ଖାଲି) {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ngx_strlow(cycle->hostname.data, (u_char *) hostname, cycle->hostname.len);


    ଯଦି (ngx_cycle_modules(cycle) != NGX_OK) {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }


    ପାଇଁ (i = 0; cycle->modules[i]; i++) {
        ଯଦି (cycle->modules[i]->type != NGX_CORE_MODULE) {
            ଜାରି;
        }

        module = cycle->modules[i]->ctx;

        ଯଦି (module->create_conf) {
            rv = module->create_conf(cycle);
            ଯଦି (rv == ଖାଲି) {
                ngx_destroy_pool(pool);
                ଫେରନ୍ତୁ ଖାଲି;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, ଆକାର(ngx_conf_t));
    /* STUB: init array ? */
    conf.args = ngx_array_create(pool, 10, ଆକାର(ngx_str_t));
    ଯଦି (conf.args == ଖାଲି) {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    conf.temp_pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    ଯଦି (conf.temp_pool == ଖାଲି) {
        ngx_destroy_pool(pool);
        ଫେରନ୍ତୁ ଖାଲି;
    }


    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#ଯଦି 0
    log->log_level = NGX_LOG_DEBUG_ALL;
#endif

    ଯଦି (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (ngx_test_config && !ngx_quiet_mode) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       cycle->conf_file.data);
    }

    ପାଇଁ (i = 0; cycle->modules[i]; i++) {
        ଯଦି (cycle->modules[i]->type != NGX_CORE_MODULE) {
            ଜାରି;
        }

        module = cycle->modules[i]->ctx;

        ଯଦି (module->init_conf) {
            ଯଦି (module->init_conf(cycle,
                                  cycle->conf_ctx[cycle->modules[i]->index])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_cycle_pools(&conf);
                ଫେରନ୍ତୁ ଖାଲି;
            }
        }
    }

    ଯଦି (ngx_process == NGX_PROCESS_SIGNALLER) {
        ଫେରନ୍ତୁ cycle;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ଯଦି (ngx_test_config) {

        ଯଦି (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
            ଯାଅ failed;
        }

    } ଅନ୍ୟଥା ଯଦି (!ngx_is_init_cycle(old_cycle)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        ଯଦି (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.data, old_ccf->pid.data) != 0)
        {
            /* new pid file name */

            ଯଦି (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
                ଯାଅ failed;
            }

            ngx_delete_pidfile(old_cycle);
        }
    }


    ଯଦି (ngx_test_lockfile(cycle->lock_file.data, log) != NGX_OK) {
        ଯାଅ failed;
    }


    ଯଦି (ngx_create_paths(cycle, ccf->user) != NGX_OK) {
        ଯାଅ failed;
    }


    ଯଦି (ngx_log_open_default(cycle) != NGX_OK) {
        ଯାଅ failed;
    }

    /* open the new files */

    part = &cycle->open_files.part;
    file = part->elts;

    ପାଇଁ (i = 0; /* void */ ; i++) {

        ଯଦି (i >= part->nelts) {
            ଯଦି (part->next == ଖାଲି) {
                ଭାଙ୍ଗ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ଯଦି (file[i].name.len == 0) {
            ଜାରି;
        }

        file[i].fd = ngx_open_file(file[i].name.data,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, log, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.data);

        ଯଦି (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.data);
            ଯାଅ failed;
        }

#ଯଦି !(NGX_WIN32)
        ଯଦି (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);
            ଯାଅ failed;
        }
#endif
    }

    cycle->log = &cycle->new_log;
    pool->log = &cycle->new_log;


    /* create shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    ପାଇଁ (i = 0; /* void */ ; i++) {

        ଯଦି (i >= part->nelts) {
            ଯଦି (part->next == ଖାଲି) {
                ଭାଙ୍ଗ;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        ଯଦି (shm_zone[i].shm.size == 0) {
            ngx_log_error(NGX_LOG_EMERG, log, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            ଯାଅ failed;
        }

        shm_zone[i].shm.log = cycle->log;

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        data = ଖାଲି;

        ପାଇଁ (n = 0; /* void */ ; n++) {

            ଯଦି (n >= opart->nelts) {
                ଯଦି (opart->next == ଖାଲି) {
                    ଭାଙ୍ଗ;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            ଯଦି (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                ଜାରି;
            }

            ଯଦି (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                ଜାରି;
            }

            ଯଦି (shm_zone[i].tag == oshm_zone[n].tag && shm_zone[i].noreuse) {
                data = oshm_zone[n].data;
                ଭାଙ୍ଗ;
            }

            ଯଦି (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#ଯଦି (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#endif

                ଯଦି (shm_zone[i].init(&shm_zone[i], oshm_zone[n].data)
                    != NGX_OK)
                {
                    ଯାଅ failed;
                }

                ଯାଅ shm_zone_found;
            }

            ଭାଙ୍ଗ;
        }

        ଯଦି (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            ଯାଅ failed;
        }

        ଯଦି (ngx_init_zone_pool(cycle, &shm_zone[i]) != NGX_OK) {
            ଯାଅ failed;
        }

        ଯଦି (shm_zone[i].init(&shm_zone[i], data) != NGX_OK) {
            ଯାଅ failed;
        }

    shm_zone_found:

        ଜାରି;
    }


    /* handle the listening sockets */

    ଯଦି (old_cycle->listening.nelts) {
        ls = old_cycle->listening.elts;
        ପାଇଁ (i = 0; i < old_cycle->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = cycle->listening.elts;
        ପାଇଁ (n = 0; n < cycle->listening.nelts; n++) {

            ପାଇଁ (i = 0; i < old_cycle->listening.nelts; i++) {
                ଯଦି (ls[i].ignore) {
                    ଜାରି;
                }

                ଯଦି (ls[i].remain) {
                    ଜାରି;
                }

                ଯଦି (ls[i].type != nls[n].type) {
                    ଜାରି;
                }

                ଯଦି (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].previous = &ls[i];

                    ଯଦି (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } ଅନ୍ୟଥା {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    ଯଦି (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#ଯଦି (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    ଯଦି (ls[i].accept_filter && nls[n].accept_filter) {
                        ଯଦି (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].add_deferred = 1;
                        }

                    } ଅନ୍ୟଥା ଯଦି (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } ଅନ୍ୟଥା ଯଦି (nls[n].accept_filter) {
                        nls[n].add_deferred = 1;
                    }
#endif

#ଯଦି (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    ଯଦି (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } ଅନ୍ୟଥା ଯଦି (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].add_deferred = 1;
                    }
#endif

#ଯଦି (NGX_HAVE_REUSEPORT)
                    ଯଦି (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].add_reuseport = 1;
                    }
#endif

                    ଭାଙ୍ଗ;
                }
            }

            ଯଦି (nls[n].fd == (ngx_socket_t) -1) {
#ଯଦି (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                ଯଦି (nls[n].accept_filter) {
                    nls[n].add_deferred = 1;
                }
#endif
#ଯଦି (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                ଯଦି (nls[n].deferred_accept) {
                    nls[n].add_deferred = 1;
                }
#endif
            }
        }

    } ଅନ୍ୟଥା {
#ଯଦି (NGX_HAVE_DEFERRED_ACCEPT)
        ls = cycle->listening.elts;
        ପାଇଁ (i = 0; i < cycle->listening.nelts; i++) {
#ifdef SO_ACCEPTFILTER
            ଯଦି (ls[i].accept_filter) {
                ls[i].add_deferred = 1;
            }
#endif
#ifdef TCP_DEFER_ACCEPT
            ଯଦି (ls[i].deferred_accept) {
                ls[i].add_deferred = 1;
            }
#endif
        }
#endif
    }

    ଯଦି (ngx_open_listening_sockets(cycle) != NGX_OK) {
        ଯାଅ failed;
    }

    ଯଦି (!ngx_test_config) {
        ngx_configure_listening_sockets(cycle);
    }


    /* commit the new cycle configuration */

    ଯଦି (!ngx_use_stderr) {
        (ଶୂନ୍ୟ) ngx_log_redirect_stderr(cycle);
    }

    pool->log = cycle->log;

    ଯଦି (ngx_init_modules(cycle) != NGX_OK) {
        /* fatal */
        exit(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_cycle->shared_memory.part;
    oshm_zone = opart->elts;

    ପାଇଁ (i = 0; /* void */ ; i++) {

        ଯଦି (i >= opart->nelts) {
            ଯଦି (opart->next == ଖାଲି) {
                ଯାଅ old_shm_zone_done;
            }
            opart = opart->next;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &cycle->shared_memory.part;
        shm_zone = part->elts;

        ପାଇଁ (n = 0; /* void */ ; n++) {

            ଯଦି (n >= part->nelts) {
                ଯଦି (part->next == ଖାଲି) {
                    ଭାଙ୍ଗ;
                }
                part = part->next;
                shm_zone = part->elts;
                n = 0;
            }

            ଯଦି (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                ଜାରି;
            }

            ଯଦି (ngx_strncmp(oshm_zone[i].shm.name.data,
                            shm_zone[n].shm.name.data,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                ଜାରି;
            }

            ଯଦି (oshm_zone[i].tag == shm_zone[n].tag
                && oshm_zone[i].shm.size == shm_zone[n].shm.size
                && !oshm_zone[i].noreuse)
            {
                ଯାଅ live_shm_zone;
            }

            ଭାଙ୍ଗ;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    live_shm_zone:

        ଜାରି;
    }

old_shm_zone_done:


    /* close the unnecessary listening sockets */

    ls = old_cycle->listening.elts;
    ପାଇଁ (i = 0; i < old_cycle->listening.nelts; i++) {

        ଯଦି (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            ଜାରି;
        }

        ଯଦି (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_text);
        }

#ଯଦି (NGX_HAVE_UNIX_DOMAIN)

        ଯଦି (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_text.data + ଆକାର("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                          "deleting socket %s", name);

            ଯଦି (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#endif
    }


    /* close the unnecessary open files */

    part = &old_cycle->open_files.part;
    file = part->elts;

    ପାଇଁ (i = 0; /* void */ ; i++) {

        ଯଦି (i >= part->nelts) {
            ଯଦି (part->next == ଖାଲି) {
                ଭାଙ୍ଗ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ଯଦି (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            ଜାରି;
        }

        ଯଦି (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    ngx_destroy_pool(conf.temp_pool);

    ଯଦି (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_cycle(old_cycle)) {

        ngx_destroy_pool(old_cycle->pool);
        cycle->old_cycle = ଖାଲି;

        ଫେରନ୍ତୁ cycle;
    }


    ଯଦି (ngx_temp_pool == ଖାଲି) {
        ngx_temp_pool = ngx_create_pool(128, cycle->log);
        ଯଦି (ngx_temp_pool == ଖାଲି) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "could not create ngx_temp_pool");
            exit(1);
        }

        n = 10;

        ଯଦି (ngx_array_init(&ngx_old_cycles, ngx_temp_pool, n,
                           ଆକାର(ngx_cycle_t *))
            != NGX_OK)
        {
            exit(1);
        }

        ngx_memzero(ngx_old_cycles.elts, n * ଆକାର(ngx_cycle_t *));

        ngx_cleaner_event.handler = ngx_clean_old_cycles;
        ngx_cleaner_event.log = cycle->log;
        ngx_cleaner_event.data = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_temp_pool->log = cycle->log;

    old = ngx_array_ଠେଲ(&ngx_old_cycles);
    ଯଦି (old == ଖାଲି) {
        exit(1);
    }
    *old = old_cycle;

    ଯଦି (!ngx_cleaner_event.timer_set) {
        ngx_add_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_set = 1;
    }

    ଫେରନ୍ତୁ cycle;


failed:

    ଯଦି (!ngx_is_init_cycle(old_cycle)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        ଯଦି (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &cycle->open_files.part;
    file = part->elts;

    ପାଇଁ (i = 0; /* void */ ; i++) {

        ଯଦି (i >= part->nelts) {
            ଯଦି (part->next == ଖାଲି) {
                ଭାଙ୍ଗ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ଯଦି (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            ଜାରି;
        }

        ଯଦି (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    /* free the newly created shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    ପାଇଁ (i = 0; /* void */ ; i++) {

        ଯଦି (i >= part->nelts) {
            ଯଦି (part->next == ଖାଲି) {
                ଭାଙ୍ଗ;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        ଯଦି (shm_zone[i].shm.addr == ଖାଲି) {
            ଜାରି;
        }

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        ପାଇଁ (n = 0; /* void */ ; n++) {

            ଯଦି (n >= opart->nelts) {
                ଯଦି (opart->next == ଖାଲି) {
                    ଭାଙ୍ଗ;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            ଯଦି (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                ଜାରି;
            }

            ଯଦି (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                ଜାରି;
            }

            ଯଦି (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size
                && !shm_zone[i].noreuse)
            {
                ଯାଅ old_shm_zone_found;
            }

            ଭାଙ୍ଗ;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_found:

        ଜାରି;
    }

    ଯଦି (ngx_test_config) {
        ngx_destroy_cycle_pools(&conf);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ls = cycle->listening.elts;
    ପାଇଁ (i = 0; i < cycle->listening.nelts; i++) {
        ଯଦି (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            ଜାରି;
        }

        ଯଦି (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_text);
        }
    }

    ngx_destroy_cycle_pools(&conf);

    ଫେରନ୍ତୁ ଖାଲି;
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_destroy_cycle_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->temp_pool);
    ngx_destroy_pool(conf->pool);
}


ସ୍ଥିର ngx_int_t
ngx_init_zone_pool(ngx_cycle_t *cycle, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    ଯଦି (zn->shm.exists) {

        ଯଦି (sp == sp->addr) {
            ଫେରନ୍ତୁ NGX_OK;
        }

#ଯଦି (NGX_WIN32)

        /* remap at the required address */

        ଯଦି (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            ଫେରନ୍ତୁ NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        ଯଦି (sp == sp->addr) {
            ଫେରନ୍ତୁ NGX_OK;
        }

#endif

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    sp->end = zn->shm.addr + zn->shm.size;
    sp->min_shift = 3;
    sp->addr = zn->shm.addr;

#ଯଦି (NGX_HAVE_ATOMIC_OPS)

    file = ଖାଲି;

#ଅନ୍ୟଥା

    file = ngx_pnalloc(cycle->pool,
                       cycle->lock_file.len + zn->shm.name.len + 1);
    ଯଦି (file == ଖାଲି) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    (ଶୂନ୍ୟ) ngx_sprintf(file, "%V%V%Z", &cycle->lock_file, &zn->shm.name);

#endif

    ଯଦି (ngx_shmtx_create(&sp->mutex, &sp->lock, file) != NGX_OK) {
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ngx_slab_init(sp);

    ଫେରନ୍ତୁ NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log)
{
    size_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    ଯଦି (ngx_process > NGX_PROCESS_MASTER) {
        ଫେରନ୍ତୁ NGX_OK;
    }

    ngx_memzero(&file, ଆକାର(ngx_file_t));

    file.name = *name;
    file.log = log;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    ଯଦି (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    rc = NGX_OK;

    ଯଦି (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        ଯଦି (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    ଯଦି (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    ଫେରନ୍ତୁ rc;
}


ଶୂନ୍ୟ
ngx_delete_pidfile(ngx_cycle_t *cycle)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    name = ngx_new_ଦ୍ୱିମିକ ? ccf->oldpid.data : ccf->pid.data;

    ଯଦି (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_cycle_t *cycle, ଅକ୍ଷର *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ngx_memzero(&file, ଆକାର(ngx_file_t));

    file.name = ccf->pid;
    file.log = cycle->log;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    ଯଦି (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        ଫେରନ୍ତୁ 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    ଯଦି (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    ଯଦି (n == NGX_ERROR) {
        ଫେରନ୍ତୁ 1;
    }

    ଯେତେବେଳେ (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    ଯଦି (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.data);
        ଫେରନ୍ତୁ 1;
    }

    ଫେରନ୍ତୁ ngx_os_signal_process(cycle, sig, pid);

}


ସ୍ଥିର ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *log)
{
#ଯଦି !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    ଯଦି (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        ଫେରନ୍ତୁ NGX_ERROR;
    }

    ଯଦି (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    ଯଦି (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#endif

    ଫେରନ୍ତୁ NGX_OK;
}


ଶୂନ୍ୟ
ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &cycle->open_files.part;
    file = part->elts;

    ପାଇଁ (i = 0; /* void */ ; i++) {

        ଯଦି (i >= part->nelts) {
            ଯଦି (part->next == ଖାଲି) {
                ଭାଙ୍ଗ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ଯଦି (file[i].name.len == 0) {
            ଜାରି;
        }

        ଯଦି (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }

        fd = ngx_open_file(file[i].name.data, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.data, file[i].fd, fd);

        ଯଦି (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.data);
            ଜାରି;
        }

#ଯଦି !(NGX_WIN32)
        ଯଦି (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            ଯଦି (ngx_file_info(file[i].name.data, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.data);

                ଯଦି (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.data);
                }

                ଜାରି;
            }

            ଯଦି (fi.st_uid != user) {
                ଯଦି (chown((ଅଚଳ ଅକ୍ଷର *) file[i].name.data, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.data, user);

                    ଯଦି (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    ଜାରି;
                }
            }

            ଯଦି ((fi.st_mode & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_mode |= (S_IRUSR|S_IWUSR);

                ଯଦି (chmod((ଅଚଳ ଅକ୍ଷର *) file[i].name.data, fi.st_mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.data);

                    ଯଦି (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    ଜାରି;
                }
            }
        }

        ଯଦି (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);

            ଯଦି (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.data);
            }

            ଜାରି;
        }
#endif

        ଯଦି (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }

        file[i].fd = fd;
    }

    (ଶୂନ୍ୟ) ngx_log_redirect_stderr(cycle);
}


ngx_shm_zone_t *
ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name, size_t size, ଶୂନ୍ୟ *tag)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->cycle->shared_memory.part;
    shm_zone = part->elts;

    ପାଇଁ (i = 0; /* void */ ; i++) {

        ଯଦି (i >= part->nelts) {
            ଯଦି (part->next == ଖାଲି) {
                ଭାଙ୍ଗ;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        ଯଦି (name->len != shm_zone[i].shm.name.len) {
            ଜାରି;
        }

        ଯଦି (ngx_strncmp(name->data, shm_zone[i].shm.name.data, name->len)
            != 0)
        {
            ଜାରି;
        }

        ଯଦି (tag != shm_zone[i].tag) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            ଫେରନ୍ତୁ ଖାଲି;
        }

        ଯଦି (shm_zone[i].shm.size == 0) {
            shm_zone[i].shm.size = size;
        }

        ଯଦି (size && size != shm_zone[i].shm.size) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            size, &shm_zone[i].shm.name, shm_zone[i].shm.size);
            ଫେରନ୍ତୁ ଖାଲି;
        }

        ଫେରନ୍ତୁ &shm_zone[i];
    }

    shm_zone = ngx_list_ଠେଲ(&cf->cycle->shared_memory);

    ଯଦି (shm_zone == ଖାଲି) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    shm_zone->data = ଖାଲି;
    shm_zone->shm.log = cf->cycle->log;
    shm_zone->shm.addr = ଖାଲି;
    shm_zone->shm.size = size;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = ଖାଲି;
    shm_zone->tag = tag;
    shm_zone->noreuse = 0;

    ଫେରନ୍ତୁ shm_zone;
}


ସ୍ଥିର ଶୂନ୍ୟ
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
    ପାଇଁ (i = 0; i < ngx_old_cycles.nelts; i++) {

        ଯଦି (cycle[i] == ଖାଲି) {
            ଜାରି;
        }

        found = 0;

        ପାଇଁ (n = 0; n < cycle[i]->connection_n; n++) {
            ଯଦି (cycle[i]->connections[n].fd != (ngx_socket_t) -1) {
                found = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "live fd:%ui", n);

                ଭାଙ୍ଗ;
            }
        }

        ଯଦି (found) {
            live = 1;
            ଜାରି;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(cycle[i]->pool);
        cycle[i] = ଖାଲି;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "old cycles status: %ui", live);

    ଯଦି (live) {
        ngx_add_timer(ev, 30000);

    } ଅନ୍ୟଥା {
        ngx_destroy_pool(ngx_temp_pool);
        ngx_temp_pool = ଖାଲି;
        ngx_old_cycles.nelts = 0;
    }
}


ଶୂନ୍ୟ
ngx_set_shutdown_timer(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ଯଦି (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.data = cycle;
        ngx_shutdown_event.log = cycle->log;
        ngx_shutdown_event.cancelable = 1;

        ngx_add_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


ସ୍ଥିର ଶୂନ୍ୟ
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_cycle_t       *cycle;
    ngx_connection_t  *c;

    cycle = ev->data;

    c = cycle->connections;

    ପାଇଁ (i = 0; i < cycle->connection_n; i++) {

        ଯଦି (c[i].fd == (ngx_socket_t) -1
            || c[i].ପଠ == ଖାଲି
            || c[i].ପଠ->accept
            || c[i].ପଠ->channel
            || c[i].ପଠ->resolver)
        {
            ଜାରି;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "*%uA shutdown timeout", c[i].ସଂଖ୍ୟା);

        c[i].close = 1;
        c[i].error = 1;

        c[i].ପଠ->handler(c[i].ପଠ);
    }
}
