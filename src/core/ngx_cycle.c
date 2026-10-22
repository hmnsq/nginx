
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


ساكن فراغ ngx_destroy_cycle_pools(ngx_conf_t *conf);
ساكن ngx_int_t ngx_init_zone_pool(ngx_cycle_t *cycle,
    ngx_shm_zone_t *shm_zone);
ساكن ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *log);
ساكن فراغ ngx_clean_old_cycles(ngx_event_t *ev);
ساكن فراغ ngx_shutdown_timer_handler(ngx_event_t *ev);


متطاير ngx_cycle_t  *ngx_cycle;
ngx_array_t            ngx_old_cycles;

ساكن ngx_pool_t     *ngx_temp_pool;
ساكن ngx_event_t     ngx_cleaner_event;
ساكن ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_mode;


/* STUB NAME */
ساكن ngx_connection_t  dumb;
/* STUB */


ngx_cycle_t *
ngx_init_cycle(ngx_cycle_t *old_cycle)
{
    فراغ                *rv, *data;
    حرف               **senv;
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
    حرف                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_time_update();


    log = old_cycle->log;

    pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    𐡄𐡍 (pool == عدم) {
        أرجع عدم;
    }
    pool->log = log;

    cycle = ngx_pcalloc(pool, حجم(ngx_cycle_t));
    𐡄𐡍 (cycle == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    cycle->pool = pool;
    cycle->log = log;
    cycle->old_cycle = old_cycle;

    cycle->conf_prefix.len = old_cycle->conf_prefix.len;
    cycle->conf_prefix.data = ngx_pstrdup(pool, &old_cycle->conf_prefix);
    𐡄𐡍 (cycle->conf_prefix.data == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    cycle->prefix.len = old_cycle->prefix.len;
    cycle->prefix.data = ngx_pstrdup(pool, &old_cycle->prefix);
    𐡄𐡍 (cycle->prefix.data == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    cycle->error_log.len = old_cycle->error_log.len;
    cycle->error_log.data = ngx_pnalloc(pool, old_cycle->error_log.len + 1);
    𐡄𐡍 (cycle->error_log.data == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }
    ngx_cpystrn(cycle->error_log.data, old_cycle->error_log.data,
                old_cycle->error_log.len + 1);

    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.data = ngx_pnalloc(pool, old_cycle->conf_file.len + 1);
    𐡄𐡍 (cycle->conf_file.data == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }
    ngx_cpystrn(cycle->conf_file.data, old_cycle->conf_file.data,
                old_cycle->conf_file.len + 1);

    𐡄𐡍 (old_cycle->conf_param.len) {
        cycle->conf_param.len = old_cycle->conf_param.len;
        cycle->conf_param.data = ngx_pstrdup(pool, &old_cycle->conf_param);
        𐡄𐡍 (cycle->conf_param.data == عدم) {
            ngx_destroy_pool(pool);
            أرجع عدم;
        }
    }


    n = old_cycle->paths.nelts ? old_cycle->paths.nelts : 10;

    𐡄𐡍 (ngx_array_init(&cycle->paths, pool, n, حجم(ngx_path_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_memzero(cycle->paths.elts, n * حجم(ngx_path_t *));


    𐡄𐡍 (ngx_array_init(&cycle->config_dump, pool, 1, حجم(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_rbtree_init(&cycle->config_dump_rbtree, &cycle->config_dump_sentinel,
                    ngx_str_rbtree_insert_value);

    𐡄𐡍 (old_cycle->open_files.part.nelts) {
        n = old_cycle->open_files.part.nelts;
        لكل (part = old_cycle->open_files.part.next; part; part = part->next) {
            n += part->nelts;
        }

    } وإلا {
        n = 20;
    }

    𐡄𐡍 (ngx_list_init(&cycle->open_files, pool, n, حجم(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    𐡄𐡍 (old_cycle->shared_memory.part.nelts) {
        n = old_cycle->shared_memory.part.nelts;
        لكل (part = old_cycle->shared_memory.part.next; part; part = part->next)
        {
            n += part->nelts;
        }

    } وإلا {
        n = 1;
    }

    𐡄𐡍 (ngx_list_init(&cycle->shared_memory, pool, n, حجم(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;

    𐡄𐡍 (ngx_array_init(&cycle->listening, pool, n, حجم(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_memzero(cycle->listening.elts, n * حجم(ngx_listening_t));


    ngx_queue_init(&cycle->reusable_connections_queue);


    cycle->conf_ctx = ngx_pcalloc(pool, ngx_max_module * حجم(فراغ *));
    𐡄𐡍 (cycle->conf_ctx == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    𐡄𐡍 (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    cycle->hostname.len = ngx_strlen(hostname);

    cycle->hostname.data = ngx_pnalloc(pool, cycle->hostname.len);
    𐡄𐡍 (cycle->hostname.data == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    ngx_strlow(cycle->hostname.data, (u_char *) hostname, cycle->hostname.len);


    𐡄𐡍 (ngx_cycle_modules(cycle) != NGX_OK) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    لكل (i = 0; cycle->modules[i]; i++) {
        𐡄𐡍 (cycle->modules[i]->type != NGX_CORE_MODULE) {
            استمر;
        }

        module = cycle->modules[i]->ctx;

        𐡄𐡍 (module->create_conf) {
            rv = module->create_conf(cycle);
            𐡄𐡍 (rv == عدم) {
                ngx_destroy_pool(pool);
                أرجع عدم;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, حجم(ngx_conf_t));
    /* STUB: init array ? */
    conf.args = ngx_array_create(pool, 10, حجم(ngx_str_t));
    𐡄𐡍 (conf.args == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }

    conf.temp_pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    𐡄𐡍 (conf.temp_pool == عدم) {
        ngx_destroy_pool(pool);
        أرجع عدم;
    }


    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#𐡄𐡍 0
    log->log_level = NGX_LOG_DEBUG_ALL;
#endif

    𐡄𐡍 (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        أرجع عدم;
    }

    𐡄𐡍 (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        أرجع عدم;
    }

    𐡄𐡍 (ngx_test_config && !ngx_quiet_mode) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       cycle->conf_file.data);
    }

    لكل (i = 0; cycle->modules[i]; i++) {
        𐡄𐡍 (cycle->modules[i]->type != NGX_CORE_MODULE) {
            استمر;
        }

        module = cycle->modules[i]->ctx;

        𐡄𐡍 (module->init_conf) {
            𐡄𐡍 (module->init_conf(cycle,
                                  cycle->conf_ctx[cycle->modules[i]->index])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_cycle_pools(&conf);
                أرجع عدم;
            }
        }
    }

    𐡄𐡍 (ngx_process == NGX_PROCESS_SIGNALLER) {
        أرجع cycle;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    𐡄𐡍 (ngx_test_config) {

        𐡄𐡍 (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
            اذهب failed;
        }

    } وإلا 𐡄𐡍 (!ngx_is_init_cycle(old_cycle)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        𐡄𐡍 (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.data, old_ccf->pid.data) != 0)
        {
            /* new pid file name */

            𐡄𐡍 (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
                اذهب failed;
            }

            ngx_delete_pidfile(old_cycle);
        }
    }


    𐡄𐡍 (ngx_test_lockfile(cycle->lock_file.data, log) != NGX_OK) {
        اذهب failed;
    }


    𐡄𐡍 (ngx_create_paths(cycle, ccf->user) != NGX_OK) {
        اذهب failed;
    }


    𐡄𐡍 (ngx_log_open_default(cycle) != NGX_OK) {
        اذهب failed;
    }

    /* open the new files */

    part = &cycle->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        𐡄𐡍 (i >= part->nelts) {
            𐡄𐡍 (part->next == عدم) {
                اكسر;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        𐡄𐡍 (file[i].name.len == 0) {
            استمر;
        }

        file[i].fd = ngx_open_file(file[i].name.data,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, log, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.data);

        𐡄𐡍 (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.data);
            اذهب failed;
        }

#𐡄𐡍 !(NGX_WIN32)
        𐡄𐡍 (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);
            اذهب failed;
        }
#endif
    }

    cycle->log = &cycle->new_log;
    pool->log = &cycle->new_log;


    /* create shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        𐡄𐡍 (i >= part->nelts) {
            𐡄𐡍 (part->next == عدم) {
                اكسر;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        𐡄𐡍 (shm_zone[i].shm.size == 0) {
            ngx_log_error(NGX_LOG_EMERG, log, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            اذهب failed;
        }

        shm_zone[i].shm.log = cycle->log;

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        data = عدم;

        لكل (n = 0; /* void */ ; n++) {

            𐡄𐡍 (n >= opart->nelts) {
                𐡄𐡍 (opart->next == عدم) {
                    اكسر;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            𐡄𐡍 (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                استمر;
            }

            𐡄𐡍 (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                استمر;
            }

            𐡄𐡍 (shm_zone[i].tag == oshm_zone[n].tag && shm_zone[i].noreuse) {
                data = oshm_zone[n].data;
                اكسر;
            }

            𐡄𐡍 (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#𐡄𐡍 (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#endif

                𐡄𐡍 (shm_zone[i].init(&shm_zone[i], oshm_zone[n].data)
                    != NGX_OK)
                {
                    اذهب failed;
                }

                اذهب shm_zone_found;
            }

            اكسر;
        }

        𐡄𐡍 (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            اذهب failed;
        }

        𐡄𐡍 (ngx_init_zone_pool(cycle, &shm_zone[i]) != NGX_OK) {
            اذهب failed;
        }

        𐡄𐡍 (shm_zone[i].init(&shm_zone[i], data) != NGX_OK) {
            اذهب failed;
        }

    shm_zone_found:

        استمر;
    }


    /* handle the listening sockets */

    𐡄𐡍 (old_cycle->listening.nelts) {
        ls = old_cycle->listening.elts;
        لكل (i = 0; i < old_cycle->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = cycle->listening.elts;
        لكل (n = 0; n < cycle->listening.nelts; n++) {

            لكل (i = 0; i < old_cycle->listening.nelts; i++) {
                𐡄𐡍 (ls[i].ignore) {
                    استمر;
                }

                𐡄𐡍 (ls[i].remain) {
                    استمر;
                }

                𐡄𐡍 (ls[i].type != nls[n].type) {
                    استمر;
                }

                𐡄𐡍 (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].previous = &ls[i];

                    𐡄𐡍 (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } وإلا {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    𐡄𐡍 (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#𐡄𐡍 (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    𐡄𐡍 (ls[i].accept_filter && nls[n].accept_filter) {
                        𐡄𐡍 (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].add_deferred = 1;
                        }

                    } وإلا 𐡄𐡍 (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } وإلا 𐡄𐡍 (nls[n].accept_filter) {
                        nls[n].add_deferred = 1;
                    }
#endif

#𐡄𐡍 (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    𐡄𐡍 (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } وإلا 𐡄𐡍 (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].add_deferred = 1;
                    }
#endif

#𐡄𐡍 (NGX_HAVE_REUSEPORT)
                    𐡄𐡍 (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].add_reuseport = 1;
                    }
#endif

                    اكسر;
                }
            }

            𐡄𐡍 (nls[n].fd == (ngx_socket_t) -1) {
#𐡄𐡍 (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                𐡄𐡍 (nls[n].accept_filter) {
                    nls[n].add_deferred = 1;
                }
#endif
#𐡄𐡍 (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                𐡄𐡍 (nls[n].deferred_accept) {
                    nls[n].add_deferred = 1;
                }
#endif
            }
        }

    } وإلا {
#𐡄𐡍 (NGX_HAVE_DEFERRED_ACCEPT)
        ls = cycle->listening.elts;
        لكل (i = 0; i < cycle->listening.nelts; i++) {
#ifdef SO_ACCEPTFILTER
            𐡄𐡍 (ls[i].accept_filter) {
                ls[i].add_deferred = 1;
            }
#endif
#ifdef TCP_DEFER_ACCEPT
            𐡄𐡍 (ls[i].deferred_accept) {
                ls[i].add_deferred = 1;
            }
#endif
        }
#endif
    }

    𐡄𐡍 (ngx_open_listening_sockets(cycle) != NGX_OK) {
        اذهب failed;
    }

    𐡄𐡍 (!ngx_test_config) {
        ngx_configure_listening_sockets(cycle);
    }


    /* commit the new cycle configuration */

    𐡄𐡍 (!ngx_use_stderr) {
        (فراغ) ngx_log_redirect_stderr(cycle);
    }

    pool->log = cycle->log;

    𐡄𐡍 (ngx_init_modules(cycle) != NGX_OK) {
        /* fatal */
        exit(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_cycle->shared_memory.part;
    oshm_zone = opart->elts;

    لكل (i = 0; /* void */ ; i++) {

        𐡄𐡍 (i >= opart->nelts) {
            𐡄𐡍 (opart->next == عدم) {
                اذهب old_shm_zone_done;
            }
            opart = opart->next;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &cycle->shared_memory.part;
        shm_zone = part->elts;

        لكل (n = 0; /* void */ ; n++) {

            𐡄𐡍 (n >= part->nelts) {
                𐡄𐡍 (part->next == عدم) {
                    اكسر;
                }
                part = part->next;
                shm_zone = part->elts;
                n = 0;
            }

            𐡄𐡍 (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                استمر;
            }

            𐡄𐡍 (ngx_strncmp(oshm_zone[i].shm.name.data,
                            shm_zone[n].shm.name.data,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                استمر;
            }

            𐡄𐡍 (oshm_zone[i].tag == shm_zone[n].tag
                && oshm_zone[i].shm.size == shm_zone[n].shm.size
                && !oshm_zone[i].noreuse)
            {
                اذهب live_shm_zone;
            }

            اكسر;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    live_shm_zone:

        استمر;
    }

old_shm_zone_done:


    /* close the unnecessary listening sockets */

    ls = old_cycle->listening.elts;
    لكل (i = 0; i < old_cycle->listening.nelts; i++) {

        𐡄𐡍 (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            استمر;
        }

        𐡄𐡍 (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_text);
        }

#𐡄𐡍 (NGX_HAVE_UNIX_DOMAIN)

        𐡄𐡍 (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_text.data + حجم("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                          "deleting socket %s", name);

            𐡄𐡍 (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#endif
    }


    /* close the unnecessary open files */

    part = &old_cycle->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        𐡄𐡍 (i >= part->nelts) {
            𐡄𐡍 (part->next == عدم) {
                اكسر;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        𐡄𐡍 (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            استمر;
        }

        𐡄𐡍 (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    ngx_destroy_pool(conf.temp_pool);

    𐡄𐡍 (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_cycle(old_cycle)) {

        ngx_destroy_pool(old_cycle->pool);
        cycle->old_cycle = عدم;

        أرجع cycle;
    }


    𐡄𐡍 (ngx_temp_pool == عدم) {
        ngx_temp_pool = ngx_create_pool(128, cycle->log);
        𐡄𐡍 (ngx_temp_pool == عدم) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "could not create ngx_temp_pool");
            exit(1);
        }

        n = 10;

        𐡄𐡍 (ngx_array_init(&ngx_old_cycles, ngx_temp_pool, n,
                           حجم(ngx_cycle_t *))
            != NGX_OK)
        {
            exit(1);
        }

        ngx_memzero(ngx_old_cycles.elts, n * حجم(ngx_cycle_t *));

        ngx_cleaner_event.handler = ngx_clean_old_cycles;
        ngx_cleaner_event.log = cycle->log;
        ngx_cleaner_event.data = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_temp_pool->log = cycle->log;

    old = ngx_array_push(&ngx_old_cycles);
    𐡄𐡍 (old == عدم) {
        exit(1);
    }
    *old = old_cycle;

    𐡄𐡍 (!ngx_cleaner_event.timer_set) {
        ngx_add_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_set = 1;
    }

    أرجع cycle;


failed:

    𐡄𐡍 (!ngx_is_init_cycle(old_cycle)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        𐡄𐡍 (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &cycle->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        𐡄𐡍 (i >= part->nelts) {
            𐡄𐡍 (part->next == عدم) {
                اكسر;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        𐡄𐡍 (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            استمر;
        }

        𐡄𐡍 (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    /* free the newly created shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        𐡄𐡍 (i >= part->nelts) {
            𐡄𐡍 (part->next == عدم) {
                اكسر;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        𐡄𐡍 (shm_zone[i].shm.addr == عدم) {
            استمر;
        }

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        لكل (n = 0; /* void */ ; n++) {

            𐡄𐡍 (n >= opart->nelts) {
                𐡄𐡍 (opart->next == عدم) {
                    اكسر;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            𐡄𐡍 (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                استمر;
            }

            𐡄𐡍 (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                استمر;
            }

            𐡄𐡍 (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size
                && !shm_zone[i].noreuse)
            {
                اذهب old_shm_zone_found;
            }

            اكسر;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_found:

        استمر;
    }

    𐡄𐡍 (ngx_test_config) {
        ngx_destroy_cycle_pools(&conf);
        أرجع عدم;
    }

    ls = cycle->listening.elts;
    لكل (i = 0; i < cycle->listening.nelts; i++) {
        𐡄𐡍 (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            استمر;
        }

        𐡄𐡍 (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_text);
        }
    }

    ngx_destroy_cycle_pools(&conf);

    أرجع عدم;
}


ساكن فراغ
ngx_destroy_cycle_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->temp_pool);
    ngx_destroy_pool(conf->pool);
}


ساكن ngx_int_t
ngx_init_zone_pool(ngx_cycle_t *cycle, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    𐡄𐡍 (zn->shm.exists) {

        𐡄𐡍 (sp == sp->addr) {
            أرجع NGX_OK;
        }

#𐡄𐡍 (NGX_WIN32)

        /* remap at the required address */

        𐡄𐡍 (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            أرجع NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        𐡄𐡍 (sp == sp->addr) {
            أرجع NGX_OK;
        }

#endif

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        أرجع NGX_ERROR;
    }

    sp->end = zn->shm.addr + zn->shm.size;
    sp->min_shift = 3;
    sp->addr = zn->shm.addr;

#𐡄𐡍 (NGX_HAVE_ATOMIC_OPS)

    file = عدم;

#وإلا

    file = ngx_pnalloc(cycle->pool,
                       cycle->lock_file.len + zn->shm.name.len + 1);
    𐡄𐡍 (file == عدم) {
        أرجع NGX_ERROR;
    }

    (فراغ) ngx_sprintf(file, "%V%V%Z", &cycle->lock_file, &zn->shm.name);

#endif

    𐡄𐡍 (ngx_shmtx_create(&sp->mutex, &sp->lock, file) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    ngx_slab_init(sp);

    أرجع NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log)
{
    size_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    𐡄𐡍 (ngx_process > NGX_PROCESS_MASTER) {
        أرجع NGX_OK;
    }

    ngx_memzero(&file, حجم(ngx_file_t));

    file.name = *name;
    file.log = log;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    𐡄𐡍 (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        أرجع NGX_ERROR;
    }

    rc = NGX_OK;

    𐡄𐡍 (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        𐡄𐡍 (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    𐡄𐡍 (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    أرجع rc;
}


فراغ
ngx_delete_pidfile(ngx_cycle_t *cycle)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    name = ngx_new_binary ? ccf->oldpid.data : ccf->pid.data;

    𐡄𐡍 (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_cycle_t *cycle, حرف *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ngx_memzero(&file, حجم(ngx_file_t));

    file.name = ccf->pid;
    file.log = cycle->log;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    𐡄𐡍 (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        أرجع 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    𐡄𐡍 (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    𐡄𐡍 (n == NGX_ERROR) {
        أرجع 1;
    }

    طالما (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    𐡄𐡍 (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.data);
        أرجع 1;
    }

    أرجع ngx_os_signal_process(cycle, sig, pid);

}


ساكن ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *log)
{
#𐡄𐡍 !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    𐡄𐡍 (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        أرجع NGX_ERROR;
    }

    𐡄𐡍 (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    𐡄𐡍 (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#endif

    أرجع NGX_OK;
}


فراغ
ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &cycle->open_files.part;
    file = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        𐡄𐡍 (i >= part->nelts) {
            𐡄𐡍 (part->next == عدم) {
                اكسر;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        𐡄𐡍 (file[i].name.len == 0) {
            استمر;
        }

        𐡄𐡍 (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }

        fd = ngx_open_file(file[i].name.data, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.data, file[i].fd, fd);

        𐡄𐡍 (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.data);
            استمر;
        }

#𐡄𐡍 !(NGX_WIN32)
        𐡄𐡍 (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            𐡄𐡍 (ngx_file_info(file[i].name.data, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.data);

                𐡄𐡍 (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.data);
                }

                استمر;
            }

            𐡄𐡍 (fi.st_uid != user) {
                𐡄𐡍 (chown((ثابت حرف *) file[i].name.data, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.data, user);

                    𐡄𐡍 (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    استمر;
                }
            }

            𐡄𐡍 ((fi.st_mode & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_mode |= (S_IRUSR|S_IWUSR);

                𐡄𐡍 (chmod((ثابت حرف *) file[i].name.data, fi.st_mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.data);

                    𐡄𐡍 (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    استمر;
                }
            }
        }

        𐡄𐡍 (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);

            𐡄𐡍 (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.data);
            }

            استمر;
        }
#endif

        𐡄𐡍 (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }

        file[i].fd = fd;
    }

    (فراغ) ngx_log_redirect_stderr(cycle);
}


ngx_shm_zone_t *
ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name, size_t size, فراغ *tag)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->cycle->shared_memory.part;
    shm_zone = part->elts;

    لكل (i = 0; /* void */ ; i++) {

        𐡄𐡍 (i >= part->nelts) {
            𐡄𐡍 (part->next == عدم) {
                اكسر;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        𐡄𐡍 (name->len != shm_zone[i].shm.name.len) {
            استمر;
        }

        𐡄𐡍 (ngx_strncmp(name->data, shm_zone[i].shm.name.data, name->len)
            != 0)
        {
            استمر;
        }

        𐡄𐡍 (tag != shm_zone[i].tag) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            أرجع عدم;
        }

        𐡄𐡍 (shm_zone[i].shm.size == 0) {
            shm_zone[i].shm.size = size;
        }

        𐡄𐡍 (size && size != shm_zone[i].shm.size) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            size, &shm_zone[i].shm.name, shm_zone[i].shm.size);
            أرجع عدم;
        }

        أرجع &shm_zone[i];
    }

    shm_zone = ngx_list_push(&cf->cycle->shared_memory);

    𐡄𐡍 (shm_zone == عدم) {
        أرجع عدم;
    }

    shm_zone->data = عدم;
    shm_zone->shm.log = cf->cycle->log;
    shm_zone->shm.addr = عدم;
    shm_zone->shm.size = size;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = عدم;
    shm_zone->tag = tag;
    shm_zone->noreuse = 0;

    أرجع shm_zone;
}


ساكن فراغ
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
    لكل (i = 0; i < ngx_old_cycles.nelts; i++) {

        𐡄𐡍 (cycle[i] == عدم) {
            استمر;
        }

        found = 0;

        لكل (n = 0; n < cycle[i]->connection_n; n++) {
            𐡄𐡍 (cycle[i]->connections[n].fd != (ngx_socket_t) -1) {
                found = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "live fd:%ui", n);

                اكسر;
            }
        }

        𐡄𐡍 (found) {
            live = 1;
            استمر;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(cycle[i]->pool);
        cycle[i] = عدم;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "old cycles status: %ui", live);

    𐡄𐡍 (live) {
        ngx_add_timer(ev, 30000);

    } وإلا {
        ngx_destroy_pool(ngx_temp_pool);
        ngx_temp_pool = عدم;
        ngx_old_cycles.nelts = 0;
    }
}


فراغ
ngx_set_shutdown_timer(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    𐡄𐡍 (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.data = cycle;
        ngx_shutdown_event.log = cycle->log;
        ngx_shutdown_event.cancelable = 1;

        ngx_add_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


ساكن فراغ
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_cycle_t       *cycle;
    ngx_connection_t  *c;

    cycle = ev->data;

    c = cycle->connections;

    لكل (i = 0; i < cycle->connection_n; i++) {

        𐡄𐡍 (c[i].fd == (ngx_socket_t) -1
            || c[i].اقرأ == عدم
            || c[i].اقرأ->accept
            || c[i].اقرأ->channel
            || c[i].اقرأ->resolver)
        {
            استمر;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "*%uA shutdown timeout", c[i].number);

        c[i].close = 1;
        c[i].error = 1;

        c[i].اقرأ->handler(c[i].اقرأ);
    }
}
