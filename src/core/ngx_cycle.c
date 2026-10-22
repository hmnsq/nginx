
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_event.h>


स्थिर शून्य ngx_destroy_cycle_pools(ngx_conf_t *conf);
स्थिर ngx_int_t ngx_init_zone_pool(ngx_cycle_t *cycle,
    ngx_shm_zone_t *shm_zone);
स्थिर ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *log);
स्थिर शून्य ngx_clean_old_cycles(ngx_event_t *ev);
स्थिर शून्य ngx_shutdown_timer_handler(ngx_event_t *ev);


अस्थिर ngx_cycle_t  *ngx_cycle;
ngx_array_t            ngx_old_cycles;

स्थिर ngx_pool_t     *ngx_temp_pool;
स्थिर ngx_event_t     ngx_cleaner_event;
स्थिर ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_mode;


/* STUB NAME */
स्थिर ngx_connection_t  dumb;
/* STUB */


ngx_cycle_t *
ngx_init_cycle(ngx_cycle_t *old_cycle)
{
    शून्य                *rv, *data;
    अक्षर               **senv;
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
    अक्षर                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_time_update();


    log = old_cycle->log;

    pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    जर (pool == रिक्त) {
        परत रिक्त;
    }
    pool->log = log;

    cycle = ngx_pcalloc(pool, आकार(ngx_cycle_t));
    जर (cycle == रिक्त) {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }

    cycle->pool = pool;
    cycle->log = log;
    cycle->old_cycle = old_cycle;

    cycle->conf_prefix.len = old_cycle->conf_prefix.len;
    cycle->conf_prefix.data = ngx_pstrdup(pool, &old_cycle->conf_prefix);
    जर (cycle->conf_prefix.data == रिक्त) {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }

    cycle->prefix.len = old_cycle->prefix.len;
    cycle->prefix.data = ngx_pstrdup(pool, &old_cycle->prefix);
    जर (cycle->prefix.data == रिक्त) {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }

    cycle->error_log.len = old_cycle->error_log.len;
    cycle->error_log.data = ngx_pnalloc(pool, old_cycle->error_log.len + 1);
    जर (cycle->error_log.data == रिक्त) {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }
    ngx_cpystrn(cycle->error_log.data, old_cycle->error_log.data,
                old_cycle->error_log.len + 1);

    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.data = ngx_pnalloc(pool, old_cycle->conf_file.len + 1);
    जर (cycle->conf_file.data == रिक्त) {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }
    ngx_cpystrn(cycle->conf_file.data, old_cycle->conf_file.data,
                old_cycle->conf_file.len + 1);

    जर (old_cycle->conf_param.len) {
        cycle->conf_param.len = old_cycle->conf_param.len;
        cycle->conf_param.data = ngx_pstrdup(pool, &old_cycle->conf_param);
        जर (cycle->conf_param.data == रिक्त) {
            ngx_destroy_pool(pool);
            परत रिक्त;
        }
    }


    n = old_cycle->paths.nelts ? old_cycle->paths.nelts : 10;

    जर (ngx_array_init(&cycle->paths, pool, n, आकार(ngx_path_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }

    ngx_memzero(cycle->paths.elts, n * आकार(ngx_path_t *));


    जर (ngx_array_init(&cycle->config_dump, pool, 1, आकार(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }

    ngx_rbtree_init(&cycle->config_dump_rbtree, &cycle->config_dump_sentinel,
                    ngx_str_rbtree_घाला_मूल्य);

    जर (old_cycle->open_files.part.nelts) {
        n = old_cycle->open_files.part.nelts;
        साठी (part = old_cycle->open_files.part.next; part; part = part->next) {
            n += part->nelts;
        }

    } अन्यथा {
        n = 20;
    }

    जर (ngx_list_init(&cycle->open_files, pool, n, आकार(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }


    जर (old_cycle->shared_memory.part.nelts) {
        n = old_cycle->shared_memory.part.nelts;
        साठी (part = old_cycle->shared_memory.part.next; part; part = part->next)
        {
            n += part->nelts;
        }

    } अन्यथा {
        n = 1;
    }

    जर (ngx_list_init(&cycle->shared_memory, pool, n, आकार(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }

    n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;

    जर (ngx_array_init(&cycle->listening, pool, n, आकार(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }

    ngx_memzero(cycle->listening.elts, n * आकार(ngx_listening_t));


    ngx_queue_init(&cycle->reusable_connections_queue);


    cycle->conf_ctx = ngx_pcalloc(pool, ngx_max_module * आकार(शून्य *));
    जर (cycle->conf_ctx == रिक्त) {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }


    जर (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        परत रिक्त;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    cycle->hostname.len = ngx_strlen(hostname);

    cycle->hostname.data = ngx_pnalloc(pool, cycle->hostname.len);
    जर (cycle->hostname.data == रिक्त) {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }

    ngx_strlow(cycle->hostname.data, (u_char *) hostname, cycle->hostname.len);


    जर (ngx_cycle_modules(cycle) != NGX_OK) {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }


    साठी (i = 0; cycle->modules[i]; i++) {
        जर (cycle->modules[i]->type != NGX_CORE_MODULE) {
            सुरूठेवा;
        }

        module = cycle->modules[i]->ctx;

        जर (module->create_conf) {
            rv = module->create_conf(cycle);
            जर (rv == रिक्त) {
                ngx_destroy_pool(pool);
                परत रिक्त;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, आकार(ngx_conf_t));
    /* STUB: init array ? */
    conf.args = ngx_array_create(pool, 10, आकार(ngx_str_t));
    जर (conf.args == रिक्त) {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }

    conf.temp_pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    जर (conf.temp_pool == रिक्त) {
        ngx_destroy_pool(pool);
        परत रिक्त;
    }


    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#जर 0
    log->log_level = NGX_LOG_DEBUG_ALL;
#endif

    जर (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        परत रिक्त;
    }

    जर (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        परत रिक्त;
    }

    जर (ngx_test_config && !ngx_quiet_mode) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       cycle->conf_file.data);
    }

    साठी (i = 0; cycle->modules[i]; i++) {
        जर (cycle->modules[i]->type != NGX_CORE_MODULE) {
            सुरूठेवा;
        }

        module = cycle->modules[i]->ctx;

        जर (module->init_conf) {
            जर (module->init_conf(cycle,
                                  cycle->conf_ctx[cycle->modules[i]->index])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_cycle_pools(&conf);
                परत रिक्त;
            }
        }
    }

    जर (ngx_process == NGX_PROCESS_SIGNALLER) {
        परत cycle;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    जर (ngx_test_config) {

        जर (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
            जा failed;
        }

    } अन्यथा जर (!ngx_is_init_cycle(old_cycle)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        जर (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.data, old_ccf->pid.data) != 0)
        {
            /* new pid file name */

            जर (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
                जा failed;
            }

            ngx_delete_pidfile(old_cycle);
        }
    }


    जर (ngx_test_lockfile(cycle->lock_file.data, log) != NGX_OK) {
        जा failed;
    }


    जर (ngx_create_paths(cycle, ccf->user) != NGX_OK) {
        जा failed;
    }


    जर (ngx_log_open_default(cycle) != NGX_OK) {
        जा failed;
    }

    /* open the new files */

    part = &cycle->open_files.part;
    file = part->elts;

    साठी (i = 0; /* void */ ; i++) {

        जर (i >= part->nelts) {
            जर (part->next == रिक्त) {
                थांबा;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        जर (file[i].name.len == 0) {
            सुरूठेवा;
        }

        file[i].fd = ngx_open_file(file[i].name.data,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, log, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.data);

        जर (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.data);
            जा failed;
        }

#जर !(NGX_WIN32)
        जर (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);
            जा failed;
        }
#endif
    }

    cycle->log = &cycle->new_log;
    pool->log = &cycle->new_log;


    /* create shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    साठी (i = 0; /* void */ ; i++) {

        जर (i >= part->nelts) {
            जर (part->next == रिक्त) {
                थांबा;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        जर (shm_zone[i].shm.size == 0) {
            ngx_log_error(NGX_LOG_EMERG, log, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            जा failed;
        }

        shm_zone[i].shm.log = cycle->log;

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        data = रिक्त;

        साठी (n = 0; /* void */ ; n++) {

            जर (n >= opart->nelts) {
                जर (opart->next == रिक्त) {
                    थांबा;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            जर (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                सुरूठेवा;
            }

            जर (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                सुरूठेवा;
            }

            जर (shm_zone[i].tag == oshm_zone[n].tag && shm_zone[i].noreuse) {
                data = oshm_zone[n].data;
                थांबा;
            }

            जर (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#जर (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#endif

                जर (shm_zone[i].init(&shm_zone[i], oshm_zone[n].data)
                    != NGX_OK)
                {
                    जा failed;
                }

                जा shm_zone_found;
            }

            थांबा;
        }

        जर (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            जा failed;
        }

        जर (ngx_init_zone_pool(cycle, &shm_zone[i]) != NGX_OK) {
            जा failed;
        }

        जर (shm_zone[i].init(&shm_zone[i], data) != NGX_OK) {
            जा failed;
        }

    shm_zone_found:

        सुरूठेवा;
    }


    /* handle the listening sockets */

    जर (old_cycle->listening.nelts) {
        ls = old_cycle->listening.elts;
        साठी (i = 0; i < old_cycle->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = cycle->listening.elts;
        साठी (n = 0; n < cycle->listening.nelts; n++) {

            साठी (i = 0; i < old_cycle->listening.nelts; i++) {
                जर (ls[i].ignore) {
                    सुरूठेवा;
                }

                जर (ls[i].remain) {
                    सुरूठेवा;
                }

                जर (ls[i].type != nls[n].type) {
                    सुरूठेवा;
                }

                जर (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].previous = &ls[i];

                    जर (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } अन्यथा {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    जर (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#जर (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    जर (ls[i].accept_filter && nls[n].accept_filter) {
                        जर (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].add_deferred = 1;
                        }

                    } अन्यथा जर (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } अन्यथा जर (nls[n].accept_filter) {
                        nls[n].add_deferred = 1;
                    }
#endif

#जर (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    जर (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } अन्यथा जर (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].add_deferred = 1;
                    }
#endif

#जर (NGX_HAVE_REUSEPORT)
                    जर (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].add_reuseport = 1;
                    }
#endif

                    थांबा;
                }
            }

            जर (nls[n].fd == (ngx_socket_t) -1) {
#जर (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                जर (nls[n].accept_filter) {
                    nls[n].add_deferred = 1;
                }
#endif
#जर (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                जर (nls[n].deferred_accept) {
                    nls[n].add_deferred = 1;
                }
#endif
            }
        }

    } अन्यथा {
#जर (NGX_HAVE_DEFERRED_ACCEPT)
        ls = cycle->listening.elts;
        साठी (i = 0; i < cycle->listening.nelts; i++) {
#ifdef SO_ACCEPTFILTER
            जर (ls[i].accept_filter) {
                ls[i].add_deferred = 1;
            }
#endif
#ifdef TCP_DEFER_ACCEPT
            जर (ls[i].deferred_accept) {
                ls[i].add_deferred = 1;
            }
#endif
        }
#endif
    }

    जर (ngx_open_listening_sockets(cycle) != NGX_OK) {
        जा failed;
    }

    जर (!ngx_test_config) {
        ngx_configure_listening_sockets(cycle);
    }


    /* commit the new cycle configuration */

    जर (!ngx_use_stderr) {
        (शून्य) ngx_log_redirect_stderr(cycle);
    }

    pool->log = cycle->log;

    जर (ngx_init_modules(cycle) != NGX_OK) {
        /* fatal */
        निकास(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_cycle->shared_memory.part;
    oshm_zone = opart->elts;

    साठी (i = 0; /* void */ ; i++) {

        जर (i >= opart->nelts) {
            जर (opart->next == रिक्त) {
                जा old_shm_zone_done;
            }
            opart = opart->next;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &cycle->shared_memory.part;
        shm_zone = part->elts;

        साठी (n = 0; /* void */ ; n++) {

            जर (n >= part->nelts) {
                जर (part->next == रिक्त) {
                    थांबा;
                }
                part = part->next;
                shm_zone = part->elts;
                n = 0;
            }

            जर (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                सुरूठेवा;
            }

            जर (ngx_strncmp(oshm_zone[i].shm.name.data,
                            shm_zone[n].shm.name.data,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                सुरूठेवा;
            }

            जर (oshm_zone[i].tag == shm_zone[n].tag
                && oshm_zone[i].shm.size == shm_zone[n].shm.size
                && !oshm_zone[i].noreuse)
            {
                जा live_shm_zone;
            }

            थांबा;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    live_shm_zone:

        सुरूठेवा;
    }

old_shm_zone_done:


    /* close the unnecessary listening sockets */

    ls = old_cycle->listening.elts;
    साठी (i = 0; i < old_cycle->listening.nelts; i++) {

        जर (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            सुरूठेवा;
        }

        जर (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_text);
        }

#जर (NGX_HAVE_UNIX_DOMAIN)

        जर (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_text.data + आकार("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                          "deleting socket %s", name);

            जर (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#endif
    }


    /* close the unnecessary open files */

    part = &old_cycle->open_files.part;
    file = part->elts;

    साठी (i = 0; /* void */ ; i++) {

        जर (i >= part->nelts) {
            जर (part->next == रिक्त) {
                थांबा;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        जर (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            सुरूठेवा;
        }

        जर (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    ngx_destroy_pool(conf.temp_pool);

    जर (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_cycle(old_cycle)) {

        ngx_destroy_pool(old_cycle->pool);
        cycle->old_cycle = रिक्त;

        परत cycle;
    }


    जर (ngx_temp_pool == रिक्त) {
        ngx_temp_pool = ngx_create_pool(128, cycle->log);
        जर (ngx_temp_pool == रिक्त) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "could not create ngx_temp_pool");
            निकास(1);
        }

        n = 10;

        जर (ngx_array_init(&ngx_old_cycles, ngx_temp_pool, n,
                           आकार(ngx_cycle_t *))
            != NGX_OK)
        {
            निकास(1);
        }

        ngx_memzero(ngx_old_cycles.elts, n * आकार(ngx_cycle_t *));

        ngx_cleaner_event.handler = ngx_clean_old_cycles;
        ngx_cleaner_event.log = cycle->log;
        ngx_cleaner_event.data = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_temp_pool->log = cycle->log;

    old = ngx_array_ढकला(&ngx_old_cycles);
    जर (old == रिक्त) {
        निकास(1);
    }
    *old = old_cycle;

    जर (!ngx_cleaner_event.timer_set) {
        ngx_add_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_set = 1;
    }

    परत cycle;


failed:

    जर (!ngx_is_init_cycle(old_cycle)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        जर (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &cycle->open_files.part;
    file = part->elts;

    साठी (i = 0; /* void */ ; i++) {

        जर (i >= part->nelts) {
            जर (part->next == रिक्त) {
                थांबा;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        जर (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            सुरूठेवा;
        }

        जर (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    /* free the newly created shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    साठी (i = 0; /* void */ ; i++) {

        जर (i >= part->nelts) {
            जर (part->next == रिक्त) {
                थांबा;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        जर (shm_zone[i].shm.addr == रिक्त) {
            सुरूठेवा;
        }

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        साठी (n = 0; /* void */ ; n++) {

            जर (n >= opart->nelts) {
                जर (opart->next == रिक्त) {
                    थांबा;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            जर (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                सुरूठेवा;
            }

            जर (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                सुरूठेवा;
            }

            जर (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size
                && !shm_zone[i].noreuse)
            {
                जा old_shm_zone_found;
            }

            थांबा;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_found:

        सुरूठेवा;
    }

    जर (ngx_test_config) {
        ngx_destroy_cycle_pools(&conf);
        परत रिक्त;
    }

    ls = cycle->listening.elts;
    साठी (i = 0; i < cycle->listening.nelts; i++) {
        जर (ls[i].fd == (ngx_socket_t) -1 || !ls[i].खोलो) {
            सुरूठेवा;
        }

        जर (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_text);
        }
    }

    ngx_destroy_cycle_pools(&conf);

    परत रिक्त;
}


स्थिर शून्य
ngx_destroy_cycle_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->temp_pool);
    ngx_destroy_pool(conf->pool);
}


स्थिर ngx_int_t
ngx_init_zone_pool(ngx_cycle_t *cycle, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    जर (zn->shm.exists) {

        जर (sp == sp->addr) {
            परत NGX_OK;
        }

#जर (NGX_WIN32)

        /* remap at the required address */

        जर (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            परत NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        जर (sp == sp->addr) {
            परत NGX_OK;
        }

#endif

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        परत NGX_ERROR;
    }

    sp->end = zn->shm.addr + zn->shm.size;
    sp->min_shift = 3;
    sp->addr = zn->shm.addr;

#जर (NGX_HAVE_ATOMIC_OPS)

    file = रिक्त;

#अन्यथा

    file = ngx_pnalloc(cycle->pool,
                       cycle->lock_file.len + zn->shm.name.len + 1);
    जर (file == रिक्त) {
        परत NGX_ERROR;
    }

    (शून्य) ngx_sprintf(file, "%V%V%Z", &cycle->lock_file, &zn->shm.name);

#endif

    जर (ngx_shmtx_create(&sp->mutex, &sp->lock, file) != NGX_OK) {
        परत NGX_ERROR;
    }

    ngx_slab_init(sp);

    परत NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log)
{
    माप_प्रकार      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    जर (ngx_process > NGX_PROCESS_MASTER) {
        परत NGX_OK;
    }

    ngx_memzero(&file, आकार(ngx_file_t));

    file.name = *name;
    file.log = log;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    जर (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        परत NGX_ERROR;
    }

    rc = NGX_OK;

    जर (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        जर (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    जर (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    परत rc;
}


शून्य
ngx_delete_pidfile(ngx_cycle_t *cycle)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    name = ngx_new_द्विमान ? ccf->oldpid.data : ccf->pid.data;

    जर (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_cycle_t *cycle, अक्षर *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ngx_memzero(&file, आकार(ngx_file_t));

    file.name = ccf->pid;
    file.log = cycle->log;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    जर (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        परत 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    जर (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    जर (n == NGX_ERROR) {
        परत 1;
    }

    पर्यंत (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    जर (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.data);
        परत 1;
    }

    परत ngx_os_signal_process(cycle, sig, pid);

}


स्थिर ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *log)
{
#जर !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    जर (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        परत NGX_ERROR;
    }

    जर (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    जर (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#endif

    परत NGX_OK;
}


शून्य
ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &cycle->open_files.part;
    file = part->elts;

    साठी (i = 0; /* void */ ; i++) {

        जर (i >= part->nelts) {
            जर (part->next == रिक्त) {
                थांबा;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        जर (file[i].name.len == 0) {
            सुरूठेवा;
        }

        जर (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }

        fd = ngx_open_file(file[i].name.data, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.data, file[i].fd, fd);

        जर (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.data);
            सुरूठेवा;
        }

#जर !(NGX_WIN32)
        जर (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            जर (ngx_file_info(file[i].name.data, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.data);

                जर (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.data);
                }

                सुरूठेवा;
            }

            जर (fi.st_uid != user) {
                जर (chown((स्थिरांक अक्षर *) file[i].name.data, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.data, user);

                    जर (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    सुरूठेवा;
                }
            }

            जर ((fi.st_mode & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_mode |= (S_IRUSR|S_IWUSR);

                जर (chmod((स्थिरांक अक्षर *) file[i].name.data, fi.st_mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.data);

                    जर (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    सुरूठेवा;
                }
            }
        }

        जर (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);

            जर (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.data);
            }

            सुरूठेवा;
        }
#endif

        जर (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }

        file[i].fd = fd;
    }

    (शून्य) ngx_log_redirect_stderr(cycle);
}


ngx_shm_zone_t *
ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name, माप_प्रकार size, शून्य *tag)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->cycle->shared_memory.part;
    shm_zone = part->elts;

    साठी (i = 0; /* void */ ; i++) {

        जर (i >= part->nelts) {
            जर (part->next == रिक्त) {
                थांबा;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        जर (name->len != shm_zone[i].shm.name.len) {
            सुरूठेवा;
        }

        जर (ngx_strncmp(name->data, shm_zone[i].shm.name.data, name->len)
            != 0)
        {
            सुरूठेवा;
        }

        जर (tag != shm_zone[i].tag) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            परत रिक्त;
        }

        जर (shm_zone[i].shm.size == 0) {
            shm_zone[i].shm.size = size;
        }

        जर (size && size != shm_zone[i].shm.size) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            size, &shm_zone[i].shm.name, shm_zone[i].shm.size);
            परत रिक्त;
        }

        परत &shm_zone[i];
    }

    shm_zone = ngx_list_ढकला(&cf->cycle->shared_memory);

    जर (shm_zone == रिक्त) {
        परत रिक्त;
    }

    shm_zone->data = रिक्त;
    shm_zone->shm.log = cf->cycle->log;
    shm_zone->shm.addr = रिक्त;
    shm_zone->shm.size = size;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = रिक्त;
    shm_zone->tag = tag;
    shm_zone->noreuse = 0;

    परत shm_zone;
}


स्थिर शून्य
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
    साठी (i = 0; i < ngx_old_cycles.nelts; i++) {

        जर (cycle[i] == रिक्त) {
            सुरूठेवा;
        }

        found = 0;

        साठी (n = 0; n < cycle[i]->connection_n; n++) {
            जर (cycle[i]->connections[n].fd != (ngx_socket_t) -1) {
                found = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "live fd:%ui", n);

                थांबा;
            }
        }

        जर (found) {
            live = 1;
            सुरूठेवा;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(cycle[i]->pool);
        cycle[i] = रिक्त;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "old cycles status: %ui", live);

    जर (live) {
        ngx_add_timer(ev, 30000);

    } अन्यथा {
        ngx_destroy_pool(ngx_temp_pool);
        ngx_temp_pool = रिक्त;
        ngx_old_cycles.nelts = 0;
    }
}


शून्य
ngx_set_shutdown_timer(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    जर (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.data = cycle;
        ngx_shutdown_event.log = cycle->log;
        ngx_shutdown_event.cancelable = 1;

        ngx_add_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


स्थिर शून्य
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_cycle_t       *cycle;
    ngx_connection_t  *c;

    cycle = ev->data;

    c = cycle->connections;

    साठी (i = 0; i < cycle->connection_n; i++) {

        जर (c[i].fd == (ngx_socket_t) -1
            || c[i].पठ == रिक्त
            || c[i].पठ->accept
            || c[i].पठ->channel
            || c[i].पठ->resolver)
        {
            सुरूठेवा;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "*%uA shutdown timeout", c[i].संख्या);

        c[i].बंद = 1;
        c[i].error = 1;

        c[i].पठ->handler(c[i].पठ);
    }
}
