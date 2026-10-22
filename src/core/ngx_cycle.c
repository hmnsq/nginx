
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>
#ಅನ್ತರ್ಭಾವ <ngx_event.h>


ಸ್ಥಿರ ಶೂನ್ಯ ngx_destroy_cycle_pools(ngx_conf_t *conf);
ಸ್ಥಿರ ngx_int_t ngx_init_zone_pool(ngx_cycle_t *cycle,
    ngx_shm_zone_t *shm_zone);
ಸ್ಥಿರ ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *log);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_clean_old_cycles(ngx_event_t *ev);
ಸ್ಥಿರ ಶೂನ್ಯ ngx_shutdown_timer_handler(ngx_event_t *ev);


ಚಞ್ಚಲ ngx_cycle_t  *ngx_cycle;
ngx_array_t            ngx_old_cycles;

ಸ್ಥಿರ ngx_pool_t     *ngx_temp_pool;
ಸ್ಥಿರ ngx_event_t     ngx_cleaner_event;
ಸ್ಥಿರ ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_mode;


/* STUB NAME */
ಸ್ಥಿರ ngx_connection_t  dumb;
/* STUB */


ngx_cycle_t *
ngx_init_cycle(ngx_cycle_t *old_cycle)
{
    ಶೂನ್ಯ                *rv, *data;
    ವರ್ಣ               **senv;
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
    ವರ್ಣ                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_time_update();


    log = old_cycle->log;

    pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    ಯದಿ (pool == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
    pool->log = log;

    cycle = ngx_pcalloc(pool, ಪರಿಮಾಣ(ngx_cycle_t));
    ಯದಿ (cycle == ರಿಕ್ತ) {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    cycle->pool = pool;
    cycle->log = log;
    cycle->old_cycle = old_cycle;

    cycle->conf_prefix.len = old_cycle->conf_prefix.len;
    cycle->conf_prefix.data = ngx_pstrdup(pool, &old_cycle->conf_prefix);
    ಯದಿ (cycle->conf_prefix.data == ರಿಕ್ತ) {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    cycle->prefix.len = old_cycle->prefix.len;
    cycle->prefix.data = ngx_pstrdup(pool, &old_cycle->prefix);
    ಯದಿ (cycle->prefix.data == ರಿಕ್ತ) {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    cycle->error_log.len = old_cycle->error_log.len;
    cycle->error_log.data = ngx_pnalloc(pool, old_cycle->error_log.len + 1);
    ಯದಿ (cycle->error_log.data == ರಿಕ್ತ) {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
    ngx_cpystrn(cycle->error_log.data, old_cycle->error_log.data,
                old_cycle->error_log.len + 1);

    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.data = ngx_pnalloc(pool, old_cycle->conf_file.len + 1);
    ಯದಿ (cycle->conf_file.data == ರಿಕ್ತ) {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
    ngx_cpystrn(cycle->conf_file.data, old_cycle->conf_file.data,
                old_cycle->conf_file.len + 1);

    ಯದಿ (old_cycle->conf_param.len) {
        cycle->conf_param.len = old_cycle->conf_param.len;
        cycle->conf_param.data = ngx_pstrdup(pool, &old_cycle->conf_param);
        ಯದಿ (cycle->conf_param.data == ರಿಕ್ತ) {
            ngx_destroy_pool(pool);
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }
    }


    n = old_cycle->paths.nelts ? old_cycle->paths.nelts : 10;

    ಯದಿ (ngx_array_init(&cycle->paths, pool, n, ಪರಿಮಾಣ(ngx_path_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ngx_memzero(cycle->paths.elts, n * ಪರಿಮಾಣ(ngx_path_t *));


    ಯದಿ (ngx_array_init(&cycle->config_dump, pool, 1, ಪರಿಮಾಣ(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ngx_rbtree_init(&cycle->config_dump_rbtree, &cycle->config_dump_sentinel,
                    ngx_str_rbtree_ಸೇರಿಸು_ಮೌಲ್ಯ);

    ಯದಿ (old_cycle->open_files.part.nelts) {
        n = old_cycle->open_files.part.nelts;
        ಪ್ರತಿ (part = old_cycle->open_files.part.next; part; part = part->next) {
            n += part->nelts;
        }

    } ಅನ್ಯಥಾ {
        n = 20;
    }

    ಯದಿ (ngx_list_init(&cycle->open_files, pool, n, ಪರಿಮಾಣ(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }


    ಯದಿ (old_cycle->shared_memory.part.nelts) {
        n = old_cycle->shared_memory.part.nelts;
        ಪ್ರತಿ (part = old_cycle->shared_memory.part.next; part; part = part->next)
        {
            n += part->nelts;
        }

    } ಅನ್ಯಥಾ {
        n = 1;
    }

    ಯದಿ (ngx_list_init(&cycle->shared_memory, pool, n, ಪರಿಮಾಣ(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;

    ಯದಿ (ngx_array_init(&cycle->listening, pool, n, ಪರಿಮಾಣ(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ngx_memzero(cycle->listening.elts, n * ಪರಿಮಾಣ(ngx_listening_t));


    ngx_queue_init(&cycle->reusable_connections_queue);


    cycle->conf_ctx = ngx_pcalloc(pool, ngx_max_module * ಪರಿಮಾಣ(ಶೂನ್ಯ *));
    ಯದಿ (cycle->conf_ctx == ರಿಕ್ತ) {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }


    ಯದಿ (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    cycle->hostname.len = ngx_strlen(hostname);

    cycle->hostname.data = ngx_pnalloc(pool, cycle->hostname.len);
    ಯದಿ (cycle->hostname.data == ರಿಕ್ತ) {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ngx_strlow(cycle->hostname.data, (u_char *) hostname, cycle->hostname.len);


    ಯದಿ (ngx_cycle_modules(cycle) != NGX_OK) {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }


    ಪ್ರತಿ (i = 0; cycle->modules[i]; i++) {
        ಯದಿ (cycle->modules[i]->type != NGX_CORE_MODULE) {
            ಅನುವರ್ತ;
        }

        module = cycle->modules[i]->ctx;

        ಯದಿ (module->create_conf) {
            rv = module->create_conf(cycle);
            ಯದಿ (rv == ರಿಕ್ತ) {
                ngx_destroy_pool(pool);
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, ಪರಿಮಾಣ(ngx_conf_t));
    /* STUB: init array ? */
    conf.args = ngx_array_create(pool, 10, ಪರಿಮಾಣ(ngx_str_t));
    ಯದಿ (conf.args == ರಿಕ್ತ) {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    conf.temp_pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    ಯದಿ (conf.temp_pool == ರಿಕ್ತ) {
        ngx_destroy_pool(pool);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }


    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#ಯದಿ 0
    log->log_level = NGX_LOG_DEBUG_ALL;
#endif

    ಯದಿ (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯದಿ (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯದಿ (ngx_test_config && !ngx_quiet_mode) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       cycle->conf_file.data);
    }

    ಪ್ರತಿ (i = 0; cycle->modules[i]; i++) {
        ಯದಿ (cycle->modules[i]->type != NGX_CORE_MODULE) {
            ಅನುವರ್ತ;
        }

        module = cycle->modules[i]->ctx;

        ಯದಿ (module->init_conf) {
            ಯದಿ (module->init_conf(cycle,
                                  cycle->conf_ctx[cycle->modules[i]->index])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_cycle_pools(&conf);
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            }
        }
    }

    ಯದಿ (ngx_process == NGX_PROCESS_SIGNALLER) {
        ಪ್ರತ್ಯಾಗಮ cycle;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ಯದಿ (ngx_test_config) {

        ಯದಿ (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
            ಗಚ್ಛ failed;
        }

    } ಅನ್ಯಥಾ ಯದಿ (!ngx_is_init_cycle(old_cycle)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        ಯದಿ (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.data, old_ccf->pid.data) != 0)
        {
            /* new pid file name */

            ಯದಿ (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
                ಗಚ್ಛ failed;
            }

            ngx_delete_pidfile(old_cycle);
        }
    }


    ಯದಿ (ngx_test_lockfile(cycle->lock_file.data, log) != NGX_OK) {
        ಗಚ್ಛ failed;
    }


    ಯದಿ (ngx_create_paths(cycle, ccf->user) != NGX_OK) {
        ಗಚ್ಛ failed;
    }


    ಯದಿ (ngx_log_open_default(cycle) != NGX_OK) {
        ಗಚ್ಛ failed;
    }

    /* open the new files */

    part = &cycle->open_files.part;
    file = part->elts;

    ಪ್ರತಿ (i = 0; /* void */ ; i++) {

        ಯದಿ (i >= part->nelts) {
            ಯದಿ (part->next == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ಯದಿ (file[i].name.len == 0) {
            ಅನುವರ್ತ;
        }

        file[i].fd = ngx_open_file(file[i].name.data,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, log, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.data);

        ಯದಿ (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.data);
            ಗಚ್ಛ failed;
        }

#ಯದಿ !(NGX_WIN32)
        ಯದಿ (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);
            ಗಚ್ಛ failed;
        }
#endif
    }

    cycle->log = &cycle->new_log;
    pool->log = &cycle->new_log;


    /* create shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    ಪ್ರತಿ (i = 0; /* void */ ; i++) {

        ಯದಿ (i >= part->nelts) {
            ಯದಿ (part->next == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        ಯದಿ (shm_zone[i].shm.size == 0) {
            ngx_log_error(NGX_LOG_EMERG, log, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            ಗಚ್ಛ failed;
        }

        shm_zone[i].shm.log = cycle->log;

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        data = ರಿಕ್ತ;

        ಪ್ರತಿ (n = 0; /* void */ ; n++) {

            ಯದಿ (n >= opart->nelts) {
                ಯದಿ (opart->next == ರಿಕ್ತ) {
                    ಭಙ್ಗ;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            ಯದಿ (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                ಅನುವರ್ತ;
            }

            ಯದಿ (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                ಅನುವರ್ತ;
            }

            ಯದಿ (shm_zone[i].tag == oshm_zone[n].tag && shm_zone[i].noreuse) {
                data = oshm_zone[n].data;
                ಭಙ್ಗ;
            }

            ಯದಿ (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#ಯದಿ (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#endif

                ಯದಿ (shm_zone[i].init(&shm_zone[i], oshm_zone[n].data)
                    != NGX_OK)
                {
                    ಗಚ್ಛ failed;
                }

                ಗಚ್ಛ shm_zone_found;
            }

            ಭಙ್ಗ;
        }

        ಯದಿ (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            ಗಚ್ಛ failed;
        }

        ಯದಿ (ngx_init_zone_pool(cycle, &shm_zone[i]) != NGX_OK) {
            ಗಚ್ಛ failed;
        }

        ಯದಿ (shm_zone[i].init(&shm_zone[i], data) != NGX_OK) {
            ಗಚ್ಛ failed;
        }

    shm_zone_found:

        ಅನುವರ್ತ;
    }


    /* handle the listening sockets */

    ಯದಿ (old_cycle->listening.nelts) {
        ls = old_cycle->listening.elts;
        ಪ್ರತಿ (i = 0; i < old_cycle->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = cycle->listening.elts;
        ಪ್ರತಿ (n = 0; n < cycle->listening.nelts; n++) {

            ಪ್ರತಿ (i = 0; i < old_cycle->listening.nelts; i++) {
                ಯದಿ (ls[i].ignore) {
                    ಅನುವರ್ತ;
                }

                ಯದಿ (ls[i].remain) {
                    ಅನುವರ್ತ;
                }

                ಯದಿ (ls[i].type != nls[n].type) {
                    ಅನುವರ್ತ;
                }

                ಯದಿ (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].previous = &ls[i];

                    ಯದಿ (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } ಅನ್ಯಥಾ {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    ಯದಿ (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#ಯದಿ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    ಯದಿ (ls[i].accept_filter && nls[n].accept_filter) {
                        ಯದಿ (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].add_deferred = 1;
                        }

                    } ಅನ್ಯಥಾ ಯದಿ (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } ಅನ್ಯಥಾ ಯದಿ (nls[n].accept_filter) {
                        nls[n].add_deferred = 1;
                    }
#endif

#ಯದಿ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    ಯದಿ (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } ಅನ್ಯಥಾ ಯದಿ (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].add_deferred = 1;
                    }
#endif

#ಯದಿ (NGX_HAVE_REUSEPORT)
                    ಯದಿ (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].add_reuseport = 1;
                    }
#endif

                    ಭಙ್ಗ;
                }
            }

            ಯದಿ (nls[n].fd == (ngx_socket_t) -1) {
#ಯದಿ (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                ಯದಿ (nls[n].accept_filter) {
                    nls[n].add_deferred = 1;
                }
#endif
#ಯದಿ (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                ಯದಿ (nls[n].deferred_accept) {
                    nls[n].add_deferred = 1;
                }
#endif
            }
        }

    } ಅನ್ಯಥಾ {
#ಯದಿ (NGX_HAVE_DEFERRED_ACCEPT)
        ls = cycle->listening.elts;
        ಪ್ರತಿ (i = 0; i < cycle->listening.nelts; i++) {
#ifdef SO_ACCEPTFILTER
            ಯದಿ (ls[i].accept_filter) {
                ls[i].add_deferred = 1;
            }
#endif
#ifdef TCP_DEFER_ACCEPT
            ಯದಿ (ls[i].deferred_accept) {
                ls[i].add_deferred = 1;
            }
#endif
        }
#endif
    }

    ಯದಿ (ngx_open_listening_sockets(cycle) != NGX_OK) {
        ಗಚ್ಛ failed;
    }

    ಯದಿ (!ngx_test_config) {
        ngx_configure_listening_sockets(cycle);
    }


    /* commit the new cycle configuration */

    ಯದಿ (!ngx_use_stderr) {
        (ಶೂನ್ಯ) ngx_log_redirect_stderr(cycle);
    }

    pool->log = cycle->log;

    ಯದಿ (ngx_init_modules(cycle) != NGX_OK) {
        /* fatal */
        exit(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_cycle->shared_memory.part;
    oshm_zone = opart->elts;

    ಪ್ರತಿ (i = 0; /* void */ ; i++) {

        ಯದಿ (i >= opart->nelts) {
            ಯದಿ (opart->next == ರಿಕ್ತ) {
                ಗಚ್ಛ old_shm_zone_done;
            }
            opart = opart->next;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &cycle->shared_memory.part;
        shm_zone = part->elts;

        ಪ್ರತಿ (n = 0; /* void */ ; n++) {

            ಯದಿ (n >= part->nelts) {
                ಯದಿ (part->next == ರಿಕ್ತ) {
                    ಭಙ್ಗ;
                }
                part = part->next;
                shm_zone = part->elts;
                n = 0;
            }

            ಯದಿ (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                ಅನುವರ್ತ;
            }

            ಯದಿ (ngx_strncmp(oshm_zone[i].shm.name.data,
                            shm_zone[n].shm.name.data,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                ಅನುವರ್ತ;
            }

            ಯದಿ (oshm_zone[i].tag == shm_zone[n].tag
                && oshm_zone[i].shm.size == shm_zone[n].shm.size
                && !oshm_zone[i].noreuse)
            {
                ಗಚ್ಛ live_shm_zone;
            }

            ಭಙ್ಗ;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    live_shm_zone:

        ಅನುವರ್ತ;
    }

old_shm_zone_done:


    /* close the unnecessary listening sockets */

    ls = old_cycle->listening.elts;
    ಪ್ರತಿ (i = 0; i < old_cycle->listening.nelts; i++) {

        ಯದಿ (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            ಅನುವರ್ತ;
        }

        ಯದಿ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_text);
        }

#ಯದಿ (NGX_HAVE_UNIX_DOMAIN)

        ಯದಿ (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_text.data + ಪರಿಮಾಣ("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                          "deleting socket %s", name);

            ಯದಿ (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#endif
    }


    /* close the unnecessary open files */

    part = &old_cycle->open_files.part;
    file = part->elts;

    ಪ್ರತಿ (i = 0; /* void */ ; i++) {

        ಯದಿ (i >= part->nelts) {
            ಯದಿ (part->next == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ಯದಿ (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            ಅನುವರ್ತ;
        }

        ಯದಿ (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    ngx_destroy_pool(conf.temp_pool);

    ಯದಿ (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_cycle(old_cycle)) {

        ngx_destroy_pool(old_cycle->pool);
        cycle->old_cycle = ರಿಕ್ತ;

        ಪ್ರತ್ಯಾಗಮ cycle;
    }


    ಯದಿ (ngx_temp_pool == ರಿಕ್ತ) {
        ngx_temp_pool = ngx_create_pool(128, cycle->log);
        ಯದಿ (ngx_temp_pool == ರಿಕ್ತ) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "could not create ngx_temp_pool");
            exit(1);
        }

        n = 10;

        ಯದಿ (ngx_array_init(&ngx_old_cycles, ngx_temp_pool, n,
                           ಪರಿಮಾಣ(ngx_cycle_t *))
            != NGX_OK)
        {
            exit(1);
        }

        ngx_memzero(ngx_old_cycles.elts, n * ಪರಿಮಾಣ(ngx_cycle_t *));

        ngx_cleaner_event.handler = ngx_clean_old_cycles;
        ngx_cleaner_event.log = cycle->log;
        ngx_cleaner_event.data = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_temp_pool->log = cycle->log;

    old = ngx_array_ತಳ್ಳು(&ngx_old_cycles);
    ಯದಿ (old == ರಿಕ್ತ) {
        exit(1);
    }
    *old = old_cycle;

    ಯದಿ (!ngx_cleaner_event.timer_set) {
        ngx_add_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_set = 1;
    }

    ಪ್ರತ್ಯಾಗಮ cycle;


failed:

    ಯದಿ (!ngx_is_init_cycle(old_cycle)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        ಯದಿ (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &cycle->open_files.part;
    file = part->elts;

    ಪ್ರತಿ (i = 0; /* void */ ; i++) {

        ಯದಿ (i >= part->nelts) {
            ಯದಿ (part->next == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ಯದಿ (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            ಅನುವರ್ತ;
        }

        ಯದಿ (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    /* free the newly created shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    ಪ್ರತಿ (i = 0; /* void */ ; i++) {

        ಯದಿ (i >= part->nelts) {
            ಯದಿ (part->next == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        ಯದಿ (shm_zone[i].shm.addr == ರಿಕ್ತ) {
            ಅನುವರ್ತ;
        }

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        ಪ್ರತಿ (n = 0; /* void */ ; n++) {

            ಯದಿ (n >= opart->nelts) {
                ಯದಿ (opart->next == ರಿಕ್ತ) {
                    ಭಙ್ಗ;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            ಯದಿ (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                ಅನುವರ್ತ;
            }

            ಯದಿ (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                ಅನುವರ್ತ;
            }

            ಯದಿ (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size
                && !shm_zone[i].noreuse)
            {
                ಗಚ್ಛ old_shm_zone_found;
            }

            ಭಙ್ಗ;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_found:

        ಅನುವರ್ತ;
    }

    ಯದಿ (ngx_test_config) {
        ngx_destroy_cycle_pools(&conf);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ls = cycle->listening.elts;
    ಪ್ರತಿ (i = 0; i < cycle->listening.nelts; i++) {
        ಯದಿ (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            ಅನುವರ್ತ;
        }

        ಯದಿ (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_text);
        }
    }

    ngx_destroy_cycle_pools(&conf);

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_destroy_cycle_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->temp_pool);
    ngx_destroy_pool(conf->pool);
}


ಸ್ಥಿರ ngx_int_t
ngx_init_zone_pool(ngx_cycle_t *cycle, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    ಯದಿ (zn->shm.exists) {

        ಯದಿ (sp == sp->addr) {
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

#ಯದಿ (NGX_WIN32)

        /* remap at the required address */

        ಯದಿ (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        ಯದಿ (sp == sp->addr) {
            ಪ್ರತ್ಯಾಗಮ NGX_OK;
        }

#endif

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    sp->end = zn->shm.addr + zn->shm.size;
    sp->min_shift = 3;
    sp->addr = zn->shm.addr;

#ಯದಿ (NGX_HAVE_ATOMIC_OPS)

    file = ರಿಕ್ತ;

#ಅನ್ಯಥಾ

    file = ngx_pnalloc(cycle->pool,
                       cycle->lock_file.len + zn->shm.name.len + 1);
    ಯದಿ (file == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    (ಶೂನ್ಯ) ngx_sprintf(file, "%V%V%Z", &cycle->lock_file, &zn->shm.name);

#endif

    ಯದಿ (ngx_shmtx_create(&sp->mutex, &sp->lock, file) != NGX_OK) {
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ngx_slab_init(sp);

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log)
{
    size_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    ಯದಿ (ngx_process > NGX_PROCESS_MASTER) {
        ಪ್ರತ್ಯಾಗಮ NGX_OK;
    }

    ngx_memzero(&file, ಪರಿಮಾಣ(ngx_file_t));

    file.name = *name;
    file.log = log;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    ಯದಿ (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    rc = NGX_OK;

    ಯದಿ (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        ಯದಿ (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    ಯದಿ (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    ಪ್ರತ್ಯಾಗಮ rc;
}


ಶೂನ್ಯ
ngx_delete_pidfile(ngx_cycle_t *cycle)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    name = ngx_new_ದ್ವಿಮಾನ ? ccf->oldpid.data : ccf->pid.data;

    ಯದಿ (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_cycle_t *cycle, ವರ್ಣ *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ngx_memzero(&file, ಪರಿಮಾಣ(ngx_file_t));

    file.name = ccf->pid;
    file.log = cycle->log;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    ಯದಿ (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        ಪ್ರತ್ಯಾಗಮ 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    ಯದಿ (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    ಯದಿ (n == NGX_ERROR) {
        ಪ್ರತ್ಯಾಗಮ 1;
    }

    ಯಾವತ್ (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    ಯದಿ (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.data);
        ಪ್ರತ್ಯಾಗಮ 1;
    }

    ಪ್ರತ್ಯಾಗಮ ngx_os_signal_process(cycle, sig, pid);

}


ಸ್ಥಿರ ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *log)
{
#ಯದಿ !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    ಯದಿ (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        ಪ್ರತ್ಯಾಗಮ NGX_ERROR;
    }

    ಯದಿ (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    ಯದಿ (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#endif

    ಪ್ರತ್ಯಾಗಮ NGX_OK;
}


ಶೂನ್ಯ
ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &cycle->open_files.part;
    file = part->elts;

    ಪ್ರತಿ (i = 0; /* void */ ; i++) {

        ಯದಿ (i >= part->nelts) {
            ಯದಿ (part->next == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        ಯದಿ (file[i].name.len == 0) {
            ಅನುವರ್ತ;
        }

        ಯದಿ (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }

        fd = ngx_open_file(file[i].name.data, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.data, file[i].fd, fd);

        ಯದಿ (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.data);
            ಅನುವರ್ತ;
        }

#ಯದಿ !(NGX_WIN32)
        ಯದಿ (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            ಯದಿ (ngx_file_info(file[i].name.data, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.data);

                ಯದಿ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.data);
                }

                ಅನುವರ್ತ;
            }

            ಯದಿ (fi.st_uid != user) {
                ಯದಿ (chown((ಧ್ರುವ ವರ್ಣ *) file[i].name.data, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.data, user);

                    ಯದಿ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    ಅನುವರ್ತ;
                }
            }

            ಯದಿ ((fi.st_mode & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_mode |= (S_IRUSR|S_IWUSR);

                ಯದಿ (chmod((ಧ್ರುವ ವರ್ಣ *) file[i].name.data, fi.st_mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.data);

                    ಯದಿ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    ಅನುವರ್ತ;
                }
            }
        }

        ಯದಿ (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);

            ಯದಿ (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.data);
            }

            ಅನುವರ್ತ;
        }
#endif

        ಯದಿ (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }

        file[i].fd = fd;
    }

    (ಶೂನ್ಯ) ngx_log_redirect_stderr(cycle);
}


ngx_shm_zone_t *
ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name, size_t size, ಶೂನ್ಯ *tag)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->cycle->shared_memory.part;
    shm_zone = part->elts;

    ಪ್ರತಿ (i = 0; /* void */ ; i++) {

        ಯದಿ (i >= part->nelts) {
            ಯದಿ (part->next == ರಿಕ್ತ) {
                ಭಙ್ಗ;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        ಯದಿ (name->len != shm_zone[i].shm.name.len) {
            ಅನುವರ್ತ;
        }

        ಯದಿ (ngx_strncmp(name->data, shm_zone[i].shm.name.data, name->len)
            != 0)
        {
            ಅನುವರ್ತ;
        }

        ಯದಿ (tag != shm_zone[i].tag) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        ಯದಿ (shm_zone[i].shm.size == 0) {
            shm_zone[i].shm.size = size;
        }

        ಯದಿ (size && size != shm_zone[i].shm.size) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            size, &shm_zone[i].shm.name, shm_zone[i].shm.size);
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        ಪ್ರತ್ಯಾಗಮ &shm_zone[i];
    }

    shm_zone = ngx_list_ತಳ್ಳು(&cf->cycle->shared_memory);

    ಯದಿ (shm_zone == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    shm_zone->data = ರಿಕ್ತ;
    shm_zone->shm.log = cf->cycle->log;
    shm_zone->shm.addr = ರಿಕ್ತ;
    shm_zone->shm.size = size;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = ರಿಕ್ತ;
    shm_zone->tag = tag;
    shm_zone->noreuse = 0;

    ಪ್ರತ್ಯಾಗಮ shm_zone;
}


ಸ್ಥಿರ ಶೂನ್ಯ
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
    ಪ್ರತಿ (i = 0; i < ngx_old_cycles.nelts; i++) {

        ಯದಿ (cycle[i] == ರಿಕ್ತ) {
            ಅನುವರ್ತ;
        }

        found = 0;

        ಪ್ರತಿ (n = 0; n < cycle[i]->connection_n; n++) {
            ಯದಿ (cycle[i]->connections[n].fd != (ngx_socket_t) -1) {
                found = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "live fd:%ui", n);

                ಭಙ್ಗ;
            }
        }

        ಯದಿ (found) {
            live = 1;
            ಅನುವರ್ತ;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(cycle[i]->pool);
        cycle[i] = ರಿಕ್ತ;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "old cycles status: %ui", live);

    ಯದಿ (live) {
        ngx_add_timer(ev, 30000);

    } ಅನ್ಯಥಾ {
        ngx_destroy_pool(ngx_temp_pool);
        ngx_temp_pool = ರಿಕ್ತ;
        ngx_old_cycles.nelts = 0;
    }
}


ಶೂನ್ಯ
ngx_set_shutdown_timer(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ಯದಿ (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.data = cycle;
        ngx_shutdown_event.log = cycle->log;
        ngx_shutdown_event.cancelable = 1;

        ngx_add_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_cycle_t       *cycle;
    ngx_connection_t  *c;

    cycle = ev->data;

    c = cycle->connections;

    ಪ್ರತಿ (i = 0; i < cycle->connection_n; i++) {

        ಯದಿ (c[i].fd == (ngx_socket_t) -1
            || c[i].ಪಠ == ರಿಕ್ತ
            || c[i].ಪಠ->accept
            || c[i].ಪಠ->channel
            || c[i].ಪಠ->resolver)
        {
            ಅನುವರ್ತ;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "*%uA shutdown timeout", c[i].ಸಂಖ್ಯೆ);

        c[i].close = 1;
        c[i].error = 1;

        c[i].ಪಠ->handler(c[i].ಪಠ);
    }
}
