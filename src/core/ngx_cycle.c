
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>
#અન્તર્ભાવ <ngx_event.h>


સ્થિર શૂન્ય ngx_destroy_cycle_pools(ngx_conf_t *conf);
સ્થિર ngx_int_t ngx_init_zone_pool(ngx_cycle_t *cycle,
    ngx_shm_zone_t *shm_zone);
સ્થિર ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *log);
સ્થિર શૂન્ય ngx_clean_old_cycles(ngx_event_t *ev);
સ્થિર શૂન્ય ngx_shutdown_timer_handler(ngx_event_t *ev);


અસ્થિર ngx_cycle_t  *ngx_cycle;
ngx_array_t            ngx_old_cycles;

સ્થિર ngx_pool_t     *ngx_temp_pool;
સ્થિર ngx_event_t     ngx_cleaner_event;
સ્થિર ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_mode;


/* STUB NAME */
સ્થિર ngx_connection_t  dumb;
/* STUB */


ngx_cycle_t *
ngx_init_cycle(ngx_cycle_t *old_cycle)
{
    શૂન્ય                *rv, *data;
    અક્ષર               **senv;
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
    અક્ષર                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_time_update();


    log = old_cycle->log;

    pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    જો (pool == ખાલી) {
        પાછા ખાલી;
    }
    pool->log = log;

    cycle = ngx_pcalloc(pool, કદ(ngx_cycle_t));
    જો (cycle == ખાલી) {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }

    cycle->pool = pool;
    cycle->log = log;
    cycle->old_cycle = old_cycle;

    cycle->conf_prefix.len = old_cycle->conf_prefix.len;
    cycle->conf_prefix.data = ngx_pstrdup(pool, &old_cycle->conf_prefix);
    જો (cycle->conf_prefix.data == ખાલી) {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }

    cycle->prefix.len = old_cycle->prefix.len;
    cycle->prefix.data = ngx_pstrdup(pool, &old_cycle->prefix);
    જો (cycle->prefix.data == ખાલી) {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }

    cycle->error_log.len = old_cycle->error_log.len;
    cycle->error_log.data = ngx_pnalloc(pool, old_cycle->error_log.len + 1);
    જો (cycle->error_log.data == ખાલી) {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }
    ngx_cpystrn(cycle->error_log.data, old_cycle->error_log.data,
                old_cycle->error_log.len + 1);

    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.data = ngx_pnalloc(pool, old_cycle->conf_file.len + 1);
    જો (cycle->conf_file.data == ખાલી) {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }
    ngx_cpystrn(cycle->conf_file.data, old_cycle->conf_file.data,
                old_cycle->conf_file.len + 1);

    જો (old_cycle->conf_param.len) {
        cycle->conf_param.len = old_cycle->conf_param.len;
        cycle->conf_param.data = ngx_pstrdup(pool, &old_cycle->conf_param);
        જો (cycle->conf_param.data == ખાલી) {
            ngx_destroy_pool(pool);
            પાછા ખાલી;
        }
    }


    n = old_cycle->paths.nelts ? old_cycle->paths.nelts : 10;

    જો (ngx_array_init(&cycle->paths, pool, n, કદ(ngx_path_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }

    ngx_memzero(cycle->paths.elts, n * કદ(ngx_path_t *));


    જો (ngx_array_init(&cycle->config_dump, pool, 1, કદ(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }

    ngx_rbtree_init(&cycle->config_dump_rbtree, &cycle->config_dump_sentinel,
                    ngx_str_rbtree_ઉમેરો_કિંમત);

    જો (old_cycle->open_files.part.nelts) {
        n = old_cycle->open_files.part.nelts;
        માટે (part = old_cycle->open_files.part.next; part; part = part->next) {
            n += part->nelts;
        }

    } નહીંતર {
        n = 20;
    }

    જો (ngx_list_init(&cycle->open_files, pool, n, કદ(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }


    જો (old_cycle->shared_memory.part.nelts) {
        n = old_cycle->shared_memory.part.nelts;
        માટે (part = old_cycle->shared_memory.part.next; part; part = part->next)
        {
            n += part->nelts;
        }

    } નહીંતર {
        n = 1;
    }

    જો (ngx_list_init(&cycle->shared_memory, pool, n, કદ(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }

    n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;

    જો (ngx_array_init(&cycle->listening, pool, n, કદ(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }

    ngx_memzero(cycle->listening.elts, n * કદ(ngx_listening_t));


    ngx_queue_init(&cycle->reusable_connections_queue);


    cycle->conf_ctx = ngx_pcalloc(pool, ngx_max_module * કદ(શૂન્ય *));
    જો (cycle->conf_ctx == ખાલી) {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }


    જો (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    cycle->hostname.len = ngx_strlen(hostname);

    cycle->hostname.data = ngx_pnalloc(pool, cycle->hostname.len);
    જો (cycle->hostname.data == ખાલી) {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }

    ngx_strlow(cycle->hostname.data, (u_char *) hostname, cycle->hostname.len);


    જો (ngx_cycle_modules(cycle) != NGX_OK) {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }


    માટે (i = 0; cycle->modules[i]; i++) {
        જો (cycle->modules[i]->type != NGX_CORE_MODULE) {
            ચાલુ;
        }

        module = cycle->modules[i]->ctx;

        જો (module->create_conf) {
            rv = module->create_conf(cycle);
            જો (rv == ખાલી) {
                ngx_destroy_pool(pool);
                પાછા ખાલી;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, કદ(ngx_conf_t));
    /* STUB: init array ? */
    conf.args = ngx_array_create(pool, 10, કદ(ngx_str_t));
    જો (conf.args == ખાલી) {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }

    conf.temp_pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    જો (conf.temp_pool == ખાલી) {
        ngx_destroy_pool(pool);
        પાછા ખાલી;
    }


    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#જો 0
    log->log_level = NGX_LOG_DEBUG_ALL;
#endif

    જો (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        પાછા ખાલી;
    }

    જો (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        પાછા ખાલી;
    }

    જો (ngx_test_config && !ngx_quiet_mode) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       cycle->conf_file.data);
    }

    માટે (i = 0; cycle->modules[i]; i++) {
        જો (cycle->modules[i]->type != NGX_CORE_MODULE) {
            ચાલુ;
        }

        module = cycle->modules[i]->ctx;

        જો (module->init_conf) {
            જો (module->init_conf(cycle,
                                  cycle->conf_ctx[cycle->modules[i]->index])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_cycle_pools(&conf);
                પાછા ખાલી;
            }
        }
    }

    જો (ngx_process == NGX_PROCESS_SIGNALLER) {
        પાછા cycle;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    જો (ngx_test_config) {

        જો (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
            જાઓ failed;
        }

    } નહીંતર જો (!ngx_is_init_cycle(old_cycle)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        જો (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.data, old_ccf->pid.data) != 0)
        {
            /* new pid file name */

            જો (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
                જાઓ failed;
            }

            ngx_delete_pidfile(old_cycle);
        }
    }


    જો (ngx_test_lockfile(cycle->lock_file.data, log) != NGX_OK) {
        જાઓ failed;
    }


    જો (ngx_create_paths(cycle, ccf->user) != NGX_OK) {
        જાઓ failed;
    }


    જો (ngx_log_open_default(cycle) != NGX_OK) {
        જાઓ failed;
    }

    /* open the new files */

    part = &cycle->open_files.part;
    file = part->elts;

    માટે (i = 0; /* void */ ; i++) {

        જો (i >= part->nelts) {
            જો (part->next == ખાલી) {
                તોડો;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        જો (file[i].name.len == 0) {
            ચાલુ;
        }

        file[i].fd = ngx_open_file(file[i].name.data,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, log, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.data);

        જો (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.data);
            જાઓ failed;
        }

#જો !(NGX_WIN32)
        જો (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);
            જાઓ failed;
        }
#endif
    }

    cycle->log = &cycle->new_log;
    pool->log = &cycle->new_log;


    /* create shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    માટે (i = 0; /* void */ ; i++) {

        જો (i >= part->nelts) {
            જો (part->next == ખાલી) {
                તોડો;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        જો (shm_zone[i].shm.size == 0) {
            ngx_log_error(NGX_LOG_EMERG, log, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            જાઓ failed;
        }

        shm_zone[i].shm.log = cycle->log;

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        data = ખાલી;

        માટે (n = 0; /* void */ ; n++) {

            જો (n >= opart->nelts) {
                જો (opart->next == ખાલી) {
                    તોડો;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            જો (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                ચાલુ;
            }

            જો (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                ચાલુ;
            }

            જો (shm_zone[i].tag == oshm_zone[n].tag && shm_zone[i].noreuse) {
                data = oshm_zone[n].data;
                તોડો;
            }

            જો (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#જો (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#endif

                જો (shm_zone[i].init(&shm_zone[i], oshm_zone[n].data)
                    != NGX_OK)
                {
                    જાઓ failed;
                }

                જાઓ shm_zone_found;
            }

            તોડો;
        }

        જો (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            જાઓ failed;
        }

        જો (ngx_init_zone_pool(cycle, &shm_zone[i]) != NGX_OK) {
            જાઓ failed;
        }

        જો (shm_zone[i].init(&shm_zone[i], data) != NGX_OK) {
            જાઓ failed;
        }

    shm_zone_found:

        ચાલુ;
    }


    /* handle the listening sockets */

    જો (old_cycle->listening.nelts) {
        ls = old_cycle->listening.elts;
        માટે (i = 0; i < old_cycle->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = cycle->listening.elts;
        માટે (n = 0; n < cycle->listening.nelts; n++) {

            માટે (i = 0; i < old_cycle->listening.nelts; i++) {
                જો (ls[i].ignore) {
                    ચાલુ;
                }

                જો (ls[i].remain) {
                    ચાલુ;
                }

                જો (ls[i].type != nls[n].type) {
                    ચાલુ;
                }

                જો (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].previous = &ls[i];

                    જો (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } નહીંતર {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    જો (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#જો (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    જો (ls[i].accept_filter && nls[n].accept_filter) {
                        જો (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].add_deferred = 1;
                        }

                    } નહીંતર જો (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } નહીંતર જો (nls[n].accept_filter) {
                        nls[n].add_deferred = 1;
                    }
#endif

#જો (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    જો (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } નહીંતર જો (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].add_deferred = 1;
                    }
#endif

#જો (NGX_HAVE_REUSEPORT)
                    જો (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].add_reuseport = 1;
                    }
#endif

                    તોડો;
                }
            }

            જો (nls[n].fd == (ngx_socket_t) -1) {
#જો (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                જો (nls[n].accept_filter) {
                    nls[n].add_deferred = 1;
                }
#endif
#જો (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                જો (nls[n].deferred_accept) {
                    nls[n].add_deferred = 1;
                }
#endif
            }
        }

    } નહીંતર {
#જો (NGX_HAVE_DEFERRED_ACCEPT)
        ls = cycle->listening.elts;
        માટે (i = 0; i < cycle->listening.nelts; i++) {
#ifdef SO_ACCEPTFILTER
            જો (ls[i].accept_filter) {
                ls[i].add_deferred = 1;
            }
#endif
#ifdef TCP_DEFER_ACCEPT
            જો (ls[i].deferred_accept) {
                ls[i].add_deferred = 1;
            }
#endif
        }
#endif
    }

    જો (ngx_open_listening_sockets(cycle) != NGX_OK) {
        જાઓ failed;
    }

    જો (!ngx_test_config) {
        ngx_configure_listening_sockets(cycle);
    }


    /* commit the new cycle configuration */

    જો (!ngx_use_stderr) {
        (શૂન્ય) ngx_log_redirect_stderr(cycle);
    }

    pool->log = cycle->log;

    જો (ngx_init_modules(cycle) != NGX_OK) {
        /* fatal */
        exit(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_cycle->shared_memory.part;
    oshm_zone = opart->elts;

    માટે (i = 0; /* void */ ; i++) {

        જો (i >= opart->nelts) {
            જો (opart->next == ખાલી) {
                જાઓ old_shm_zone_done;
            }
            opart = opart->next;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &cycle->shared_memory.part;
        shm_zone = part->elts;

        માટે (n = 0; /* void */ ; n++) {

            જો (n >= part->nelts) {
                જો (part->next == ખાલી) {
                    તોડો;
                }
                part = part->next;
                shm_zone = part->elts;
                n = 0;
            }

            જો (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                ચાલુ;
            }

            જો (ngx_strncmp(oshm_zone[i].shm.name.data,
                            shm_zone[n].shm.name.data,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                ચાલુ;
            }

            જો (oshm_zone[i].tag == shm_zone[n].tag
                && oshm_zone[i].shm.size == shm_zone[n].shm.size
                && !oshm_zone[i].noreuse)
            {
                જાઓ live_shm_zone;
            }

            તોડો;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    live_shm_zone:

        ચાલુ;
    }

old_shm_zone_done:


    /* close the unnecessary listening sockets */

    ls = old_cycle->listening.elts;
    માટે (i = 0; i < old_cycle->listening.nelts; i++) {

        જો (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            ચાલુ;
        }

        જો (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_text);
        }

#જો (NGX_HAVE_UNIX_DOMAIN)

        જો (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_text.data + કદ("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                          "deleting socket %s", name);

            જો (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#endif
    }


    /* close the unnecessary open files */

    part = &old_cycle->open_files.part;
    file = part->elts;

    માટે (i = 0; /* void */ ; i++) {

        જો (i >= part->nelts) {
            જો (part->next == ખાલી) {
                તોડો;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        જો (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            ચાલુ;
        }

        જો (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    ngx_destroy_pool(conf.temp_pool);

    જો (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_cycle(old_cycle)) {

        ngx_destroy_pool(old_cycle->pool);
        cycle->old_cycle = ખાલી;

        પાછા cycle;
    }


    જો (ngx_temp_pool == ખાલી) {
        ngx_temp_pool = ngx_create_pool(128, cycle->log);
        જો (ngx_temp_pool == ખાલી) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "could not create ngx_temp_pool");
            exit(1);
        }

        n = 10;

        જો (ngx_array_init(&ngx_old_cycles, ngx_temp_pool, n,
                           કદ(ngx_cycle_t *))
            != NGX_OK)
        {
            exit(1);
        }

        ngx_memzero(ngx_old_cycles.elts, n * કદ(ngx_cycle_t *));

        ngx_cleaner_event.handler = ngx_clean_old_cycles;
        ngx_cleaner_event.log = cycle->log;
        ngx_cleaner_event.data = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_temp_pool->log = cycle->log;

    old = ngx_array_ધકેલો(&ngx_old_cycles);
    જો (old == ખાલી) {
        exit(1);
    }
    *old = old_cycle;

    જો (!ngx_cleaner_event.timer_set) {
        ngx_add_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_set = 1;
    }

    પાછા cycle;


failed:

    જો (!ngx_is_init_cycle(old_cycle)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        જો (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &cycle->open_files.part;
    file = part->elts;

    માટે (i = 0; /* void */ ; i++) {

        જો (i >= part->nelts) {
            જો (part->next == ખાલી) {
                તોડો;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        જો (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            ચાલુ;
        }

        જો (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }
    }

    /* free the newly created shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    માટે (i = 0; /* void */ ; i++) {

        જો (i >= part->nelts) {
            જો (part->next == ખાલી) {
                તોડો;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        જો (shm_zone[i].shm.addr == ખાલી) {
            ચાલુ;
        }

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        માટે (n = 0; /* void */ ; n++) {

            જો (n >= opart->nelts) {
                જો (opart->next == ખાલી) {
                    તોડો;
                }
                opart = opart->next;
                oshm_zone = opart->elts;
                n = 0;
            }

            જો (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                ચાલુ;
            }

            જો (ngx_strncmp(shm_zone[i].shm.name.data,
                            oshm_zone[n].shm.name.data,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                ચાલુ;
            }

            જો (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.size == oshm_zone[n].shm.size
                && !shm_zone[i].noreuse)
            {
                જાઓ old_shm_zone_found;
            }

            તોડો;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_found:

        ચાલુ;
    }

    જો (ngx_test_config) {
        ngx_destroy_cycle_pools(&conf);
        પાછા ખાલી;
    }

    ls = cycle->listening.elts;
    માટે (i = 0; i < cycle->listening.nelts; i++) {
        જો (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            ચાલુ;
        }

        જો (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_text);
        }
    }

    ngx_destroy_cycle_pools(&conf);

    પાછા ખાલી;
}


સ્થિર શૂન્ય
ngx_destroy_cycle_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->temp_pool);
    ngx_destroy_pool(conf->pool);
}


સ્થિર ngx_int_t
ngx_init_zone_pool(ngx_cycle_t *cycle, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    જો (zn->shm.exists) {

        જો (sp == sp->addr) {
            પાછા NGX_OK;
        }

#જો (NGX_WIN32)

        /* remap at the required address */

        જો (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            પાછા NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        જો (sp == sp->addr) {
            પાછા NGX_OK;
        }

#endif

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        પાછા NGX_ERROR;
    }

    sp->end = zn->shm.addr + zn->shm.size;
    sp->min_shift = 3;
    sp->addr = zn->shm.addr;

#જો (NGX_HAVE_ATOMIC_OPS)

    file = ખાલી;

#નહીંતર

    file = ngx_pnalloc(cycle->pool,
                       cycle->lock_file.len + zn->shm.name.len + 1);
    જો (file == ખાલી) {
        પાછા NGX_ERROR;
    }

    (શૂન્ય) ngx_sprintf(file, "%V%V%Z", &cycle->lock_file, &zn->shm.name);

#endif

    જો (ngx_shmtx_create(&sp->mutex, &sp->lock, file) != NGX_OK) {
        પાછા NGX_ERROR;
    }

    ngx_slab_init(sp);

    પાછા NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log)
{
    size_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    જો (ngx_process > NGX_PROCESS_MASTER) {
        પાછા NGX_OK;
    }

    ngx_memzero(&file, કદ(ngx_file_t));

    file.name = *name;
    file.log = log;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    જો (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        પાછા NGX_ERROR;
    }

    rc = NGX_OK;

    જો (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        જો (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    જો (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    પાછા rc;
}


શૂન્ય
ngx_delete_pidfile(ngx_cycle_t *cycle)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    name = ngx_new_દ્વિઅંકી ? ccf->oldpid.data : ccf->pid.data;

    જો (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_cycle_t *cycle, અક્ષર *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ngx_memzero(&file, કદ(ngx_file_t));

    file.name = ccf->pid;
    file.log = cycle->log;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    જો (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        પાછા 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    જો (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    જો (n == NGX_ERROR) {
        પાછા 1;
    }

    જ્યારે (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    જો (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.data);
        પાછા 1;
    }

    પાછા ngx_os_signal_process(cycle, sig, pid);

}


સ્થિર ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *log)
{
#જો !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    જો (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        પાછા NGX_ERROR;
    }

    જો (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    જો (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#endif

    પાછા NGX_OK;
}


શૂન્ય
ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &cycle->open_files.part;
    file = part->elts;

    માટે (i = 0; /* void */ ; i++) {

        જો (i >= part->nelts) {
            જો (part->next == ખાલી) {
                તોડો;
            }
            part = part->next;
            file = part->elts;
            i = 0;
        }

        જો (file[i].name.len == 0) {
            ચાલુ;
        }

        જો (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }

        fd = ngx_open_file(file[i].name.data, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.data, file[i].fd, fd);

        જો (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.data);
            ચાલુ;
        }

#જો !(NGX_WIN32)
        જો (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            જો (ngx_file_info(file[i].name.data, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.data);

                જો (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.data);
                }

                ચાલુ;
            }

            જો (fi.st_uid != user) {
                જો (chown((અચલ અક્ષર *) file[i].name.data, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.data, user);

                    જો (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    ચાલુ;
                }
            }

            જો ((fi.st_mode & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_mode |= (S_IRUSR|S_IWUSR);

                જો (chmod((અચલ અક્ષર *) file[i].name.data, fi.st_mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.data);

                    જો (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.data);
                    }

                    ચાલુ;
                }
            }
        }

        જો (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.data);

            જો (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.data);
            }

            ચાલુ;
        }
#endif

        જો (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.data);
        }

        file[i].fd = fd;
    }

    (શૂન્ય) ngx_log_redirect_stderr(cycle);
}


ngx_shm_zone_t *
ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name, size_t size, શૂન્ય *tag)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->cycle->shared_memory.part;
    shm_zone = part->elts;

    માટે (i = 0; /* void */ ; i++) {

        જો (i >= part->nelts) {
            જો (part->next == ખાલી) {
                તોડો;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        જો (name->len != shm_zone[i].shm.name.len) {
            ચાલુ;
        }

        જો (ngx_strncmp(name->data, shm_zone[i].shm.name.data, name->len)
            != 0)
        {
            ચાલુ;
        }

        જો (tag != shm_zone[i].tag) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            પાછા ખાલી;
        }

        જો (shm_zone[i].shm.size == 0) {
            shm_zone[i].shm.size = size;
        }

        જો (size && size != shm_zone[i].shm.size) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            size, &shm_zone[i].shm.name, shm_zone[i].shm.size);
            પાછા ખાલી;
        }

        પાછા &shm_zone[i];
    }

    shm_zone = ngx_list_ધકેલો(&cf->cycle->shared_memory);

    જો (shm_zone == ખાલી) {
        પાછા ખાલી;
    }

    shm_zone->data = ખાલી;
    shm_zone->shm.log = cf->cycle->log;
    shm_zone->shm.addr = ખાલી;
    shm_zone->shm.size = size;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = ખાલી;
    shm_zone->tag = tag;
    shm_zone->noreuse = 0;

    પાછા shm_zone;
}


સ્થિર શૂન્ય
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
    માટે (i = 0; i < ngx_old_cycles.nelts; i++) {

        જો (cycle[i] == ખાલી) {
            ચાલુ;
        }

        found = 0;

        માટે (n = 0; n < cycle[i]->connection_n; n++) {
            જો (cycle[i]->connections[n].fd != (ngx_socket_t) -1) {
                found = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "live fd:%ui", n);

                તોડો;
            }
        }

        જો (found) {
            live = 1;
            ચાલુ;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(cycle[i]->pool);
        cycle[i] = ખાલી;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "old cycles status: %ui", live);

    જો (live) {
        ngx_add_timer(ev, 30000);

    } નહીંતર {
        ngx_destroy_pool(ngx_temp_pool);
        ngx_temp_pool = ખાલી;
        ngx_old_cycles.nelts = 0;
    }
}


શૂન્ય
ngx_set_shutdown_timer(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    જો (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.data = cycle;
        ngx_shutdown_event.log = cycle->log;
        ngx_shutdown_event.cancelable = 1;

        ngx_add_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


સ્થિર શૂન્ય
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_cycle_t       *cycle;
    ngx_connection_t  *c;

    cycle = ev->data;

    c = cycle->connections;

    માટે (i = 0; i < cycle->connection_n; i++) {

        જો (c[i].fd == (ngx_socket_t) -1
            || c[i].પઠ == ખાલી
            || c[i].પઠ->accept
            || c[i].પઠ->channel
            || c[i].પઠ->resolver)
        {
            ચાલુ;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "*%uA shutdown timeout", c[i].સંખ્યા);

        c[i].close = 1;
        c[i].error = 1;

        c[i].પઠ->handler(c[i].પઠ);
    }
}
