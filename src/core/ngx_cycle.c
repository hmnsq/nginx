
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>
#समावेश <ngx_event.h>


अटल व्योम ngx_destroy_cycle_pools(ngx_conf_t *conf);
अटल ngx_int_t ngx_init_zone_pool(ngx_cycle_t *cycle,
    ngx_shm_zone_t *shm_zone);
अटल ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *log);
अटल व्योम ngx_clean_old_cycles(ngx_event_t *ev);
अटल व्योम ngx_shutdown_timer_handler(ngx_event_t *ev);


अस्थिर ngx_cycle_t  *ngx_cycle;
ngx_array_t            ngx_old_cycles;

अटल ngx_pool_t     *ngx_अस्थायी_pool;
अटल ngx_event_t     ngx_cleaner_event;
अटल ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_mode;


/* STUB NAME */
अटल ngx_connection_t  dumb;
/* STUB */


ngx_cycle_t *
ngx_init_cycle(ngx_cycle_t *old_cycle)
{
    व्योम                *rv, *तथ्य;
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

    pool = ngx_create_pool(NGX_CYCLE_POOL_आकार, log);
    अगर (pool == शून्य) {
        वापस शून्य;
    }
    pool->log = log;

    cycle = ngx_pcalloc(pool, माप(ngx_cycle_t));
    अगर (cycle == शून्य) {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }

    cycle->pool = pool;
    cycle->log = log;
    cycle->old_cycle = old_cycle;

    cycle->conf_prefix.len = old_cycle->conf_prefix.len;
    cycle->conf_prefix.तथ्य = ngx_pstrdup(pool, &old_cycle->conf_prefix);
    अगर (cycle->conf_prefix.तथ्य == शून्य) {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }

    cycle->prefix.len = old_cycle->prefix.len;
    cycle->prefix.तथ्य = ngx_pstrdup(pool, &old_cycle->prefix);
    अगर (cycle->prefix.तथ्य == शून्य) {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }

    cycle->error_log.len = old_cycle->error_log.len;
    cycle->error_log.तथ्य = ngx_pnalloc(pool, old_cycle->error_log.len + 1);
    अगर (cycle->error_log.तथ्य == शून्य) {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }
    ngx_cpystrn(cycle->error_log.तथ्य, old_cycle->error_log.तथ्य,
                old_cycle->error_log.len + 1);

    cycle->conf_file.len = old_cycle->conf_file.len;
    cycle->conf_file.तथ्य = ngx_pnalloc(pool, old_cycle->conf_file.len + 1);
    अगर (cycle->conf_file.तथ्य == शून्य) {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }
    ngx_cpystrn(cycle->conf_file.तथ्य, old_cycle->conf_file.तथ्य,
                old_cycle->conf_file.len + 1);

    अगर (old_cycle->conf_param.len) {
        cycle->conf_param.len = old_cycle->conf_param.len;
        cycle->conf_param.तथ्य = ngx_pstrdup(pool, &old_cycle->conf_param);
        अगर (cycle->conf_param.तथ्य == शून्य) {
            ngx_destroy_pool(pool);
            वापस शून्य;
        }
    }


    n = old_cycle->paths.nelts ? old_cycle->paths.nelts : 10;

    अगर (ngx_array_init(&cycle->paths, pool, n, माप(ngx_path_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }

    ngx_memzero(cycle->paths.elts, n * माप(ngx_path_t *));


    अगर (ngx_array_init(&cycle->config_dump, pool, 1, माप(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }

    ngx_rbtree_init(&cycle->config_dump_rbtree, &cycle->config_dump_sentinel,
                    ngx_str_rbtree_जोड़ो_मूल्य);

    अगर (old_cycle->open_files.part.nelts) {
        n = old_cycle->open_files.part.nelts;
        क्रम (part = old_cycle->open_files.part.अग्र; part; part = part->अग्र) {
            n += part->nelts;
        }

    } अन्यथा {
        n = 20;
    }

    अगर (ngx_list_init(&cycle->open_files, pool, n, माप(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }


    अगर (old_cycle->shared_memory.part.nelts) {
        n = old_cycle->shared_memory.part.nelts;
        क्रम (part = old_cycle->shared_memory.part.अग्र; part; part = part->अग्र)
        {
            n += part->nelts;
        }

    } अन्यथा {
        n = 1;
    }

    अगर (ngx_list_init(&cycle->shared_memory, pool, n, माप(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }

    n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;

    अगर (ngx_array_init(&cycle->listening, pool, n, माप(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }

    ngx_memzero(cycle->listening.elts, n * माप(ngx_listening_t));


    ngx_queue_init(&cycle->reusable_connections_queue);


    cycle->conf_ctx = ngx_pcalloc(pool, ngx_max_module * माप(व्योम *));
    अगर (cycle->conf_ctx == शून्य) {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }


    अगर (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        वापस शून्य;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    cycle->hostname.len = ngx_strlen(hostname);

    cycle->hostname.तथ्य = ngx_pnalloc(pool, cycle->hostname.len);
    अगर (cycle->hostname.तथ्य == शून्य) {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }

    ngx_strlow(cycle->hostname.तथ्य, (u_char *) hostname, cycle->hostname.len);


    अगर (ngx_cycle_modules(cycle) != NGX_OK) {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }


    क्रम (i = 0; cycle->modules[i]; i++) {
        अगर (cycle->modules[i]->type != NGX_CORE_MODULE) {
            जारी;
        }

        module = cycle->modules[i]->ctx;

        अगर (module->create_conf) {
            rv = module->create_conf(cycle);
            अगर (rv == शून्य) {
                ngx_destroy_pool(pool);
                वापस शून्य;
            }
            cycle->conf_ctx[cycle->modules[i]->सूचकांक] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, माप(ngx_conf_t));
    /* STUB: init array ? */
    conf.args = ngx_array_create(pool, 10, माप(ngx_str_t));
    अगर (conf.args == शून्य) {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }

    conf.अस्थायी_pool = ngx_create_pool(NGX_CYCLE_POOL_आकार, log);
    अगर (conf.अस्थायी_pool == शून्य) {
        ngx_destroy_pool(pool);
        वापस शून्य;
    }


    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#अगर 0
    log->log_level = NGX_LOG_DEBUG_ALL;
#पूर्ण_अगर

    अगर (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        वापस शून्य;
    }

    अगर (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        वापस शून्य;
    }

    अगर (ngx_test_config && !ngx_quiet_mode) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       cycle->conf_file.तथ्य);
    }

    क्रम (i = 0; cycle->modules[i]; i++) {
        अगर (cycle->modules[i]->type != NGX_CORE_MODULE) {
            जारी;
        }

        module = cycle->modules[i]->ctx;

        अगर (module->init_conf) {
            अगर (module->init_conf(cycle,
                                  cycle->conf_ctx[cycle->modules[i]->सूचकांक])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_cycle_pools(&conf);
                वापस शून्य;
            }
        }
    }

    अगर (ngx_process == NGX_PROCESS_SIGNALLER) {
        वापस cycle;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    अगर (ngx_test_config) {

        अगर (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
            जाओ failed;
        }

    } अन्यथा अगर (!ngx_is_init_cycle(old_cycle)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        अगर (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.तथ्य, old_ccf->pid.तथ्य) != 0)
        {
            /* new pid file name */

            अगर (ngx_create_pidfile(&ccf->pid, log) != NGX_OK) {
                जाओ failed;
            }

            ngx_delete_pidfile(old_cycle);
        }
    }


    अगर (ngx_test_lockfile(cycle->lock_file.तथ्य, log) != NGX_OK) {
        जाओ failed;
    }


    अगर (ngx_create_paths(cycle, ccf->user) != NGX_OK) {
        जाओ failed;
    }


    अगर (ngx_log_open_default(cycle) != NGX_OK) {
        जाओ failed;
    }

    /* open the new files */

    part = &cycle->open_files.part;
    file = part->elts;

    क्रम (i = 0; /* void */ ; i++) {

        अगर (i >= part->nelts) {
            अगर (part->अग्र == शून्य) {
                अवरोध;
            }
            part = part->अग्र;
            file = part->elts;
            i = 0;
        }

        अगर (file[i].name.len == 0) {
            जारी;
        }

        file[i].fd = ngx_open_file(file[i].name.तथ्य,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, log, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.तथ्य);

        अगर (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.तथ्य);
            जाओ failed;
        }

#अगर !(NGX_WIN32)
        अगर (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.तथ्य);
            जाओ failed;
        }
#पूर्ण_अगर
    }

    cycle->log = &cycle->new_log;
    pool->log = &cycle->new_log;


    /* create shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    क्रम (i = 0; /* void */ ; i++) {

        अगर (i >= part->nelts) {
            अगर (part->अग्र == शून्य) {
                अवरोध;
            }
            part = part->अग्र;
            shm_zone = part->elts;
            i = 0;
        }

        अगर (shm_zone[i].shm.आकार == 0) {
            ngx_log_error(NGX_LOG_EMERG, log, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            जाओ failed;
        }

        shm_zone[i].shm.log = cycle->log;

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        तथ्य = शून्य;

        क्रम (n = 0; /* void */ ; n++) {

            अगर (n >= opart->nelts) {
                अगर (opart->अग्र == शून्य) {
                    अवरोध;
                }
                opart = opart->अग्र;
                oshm_zone = opart->elts;
                n = 0;
            }

            अगर (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                जारी;
            }

            अगर (ngx_strncmp(shm_zone[i].shm.name.तथ्य,
                            oshm_zone[n].shm.name.तथ्य,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                जारी;
            }

            अगर (shm_zone[i].tag == oshm_zone[n].tag && shm_zone[i].noreuse) {
                तथ्य = oshm_zone[n].तथ्य;
                अवरोध;
            }

            अगर (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.आकार == oshm_zone[n].shm.आकार)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#अगर (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#पूर्ण_अगर

                अगर (shm_zone[i].init(&shm_zone[i], oshm_zone[n].तथ्य)
                    != NGX_OK)
                {
                    जाओ failed;
                }

                जाओ shm_zone_found;
            }

            अवरोध;
        }

        अगर (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            जाओ failed;
        }

        अगर (ngx_init_zone_pool(cycle, &shm_zone[i]) != NGX_OK) {
            जाओ failed;
        }

        अगर (shm_zone[i].init(&shm_zone[i], तथ्य) != NGX_OK) {
            जाओ failed;
        }

    shm_zone_found:

        जारी;
    }


    /* handle the listening sockets */

    अगर (old_cycle->listening.nelts) {
        ls = old_cycle->listening.elts;
        क्रम (i = 0; i < old_cycle->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = cycle->listening.elts;
        क्रम (n = 0; n < cycle->listening.nelts; n++) {

            क्रम (i = 0; i < old_cycle->listening.nelts; i++) {
                अगर (ls[i].ignore) {
                    जारी;
                }

                अगर (ls[i].remain) {
                    जारी;
                }

                अगर (ls[i].type != nls[n].type) {
                    जारी;
                }

                अगर (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].previous = &ls[i];

                    अगर (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } अन्यथा {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    अगर (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#अगर (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    अगर (ls[i].accept_filter && nls[n].accept_filter) {
                        अगर (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].add_deferred = 1;
                        }

                    } अन्यथा अगर (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } अन्यथा अगर (nls[n].accept_filter) {
                        nls[n].add_deferred = 1;
                    }
#पूर्ण_अगर

#अगर (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    अगर (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } अन्यथा अगर (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].add_deferred = 1;
                    }
#पूर्ण_अगर

#अगर (NGX_HAVE_REUSEPORT)
                    अगर (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].add_reuseport = 1;
                    }
#पूर्ण_अगर

                    अवरोध;
                }
            }

            अगर (nls[n].fd == (ngx_socket_t) -1) {
#अगर (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                अगर (nls[n].accept_filter) {
                    nls[n].add_deferred = 1;
                }
#पूर्ण_अगर
#अगर (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                अगर (nls[n].deferred_accept) {
                    nls[n].add_deferred = 1;
                }
#पूर्ण_अगर
            }
        }

    } अन्यथा {
#अगर (NGX_HAVE_DEFERRED_ACCEPT)
        ls = cycle->listening.elts;
        क्रम (i = 0; i < cycle->listening.nelts; i++) {
#अगर_घोषित SO_ACCEPTFILTER
            अगर (ls[i].accept_filter) {
                ls[i].add_deferred = 1;
            }
#पूर्ण_अगर
#अगर_घोषित TCP_DEFER_ACCEPT
            अगर (ls[i].deferred_accept) {
                ls[i].add_deferred = 1;
            }
#पूर्ण_अगर
        }
#पूर्ण_अगर
    }

    अगर (ngx_open_listening_sockets(cycle) != NGX_OK) {
        जाओ failed;
    }

    अगर (!ngx_test_config) {
        ngx_configure_listening_sockets(cycle);
    }


    /* commit the new cycle configuration */

    अगर (!ngx_use_stderr) {
        (व्योम) ngx_log_redirect_stderr(cycle);
    }

    pool->log = cycle->log;

    अगर (ngx_init_modules(cycle) != NGX_OK) {
        /* fatal */
        निकास(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_cycle->shared_memory.part;
    oshm_zone = opart->elts;

    क्रम (i = 0; /* void */ ; i++) {

        अगर (i >= opart->nelts) {
            अगर (opart->अग्र == शून्य) {
                जाओ old_shm_zone_done;
            }
            opart = opart->अग्र;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &cycle->shared_memory.part;
        shm_zone = part->elts;

        क्रम (n = 0; /* void */ ; n++) {

            अगर (n >= part->nelts) {
                अगर (part->अग्र == शून्य) {
                    अवरोध;
                }
                part = part->अग्र;
                shm_zone = part->elts;
                n = 0;
            }

            अगर (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                जारी;
            }

            अगर (ngx_strncmp(oshm_zone[i].shm.name.तथ्य,
                            shm_zone[n].shm.name.तथ्य,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                जारी;
            }

            अगर (oshm_zone[i].tag == shm_zone[n].tag
                && oshm_zone[i].shm.आकार == shm_zone[n].shm.आकार
                && !oshm_zone[i].noreuse)
            {
                जाओ live_shm_zone;
            }

            अवरोध;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    live_shm_zone:

        जारी;
    }

old_shm_zone_done:


    /* close the unnecessary listening sockets */

    ls = old_cycle->listening.elts;
    क्रम (i = 0; i < old_cycle->listening.nelts; i++) {

        अगर (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            जारी;
        }

        अगर (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_text);
        }

#अगर (NGX_HAVE_UNIX_DOMAIN)

        अगर (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_text.तथ्य + माप("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                          "deleting socket %s", name);

            अगर (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#पूर्ण_अगर
    }


    /* close the unnecessary open files */

    part = &old_cycle->open_files.part;
    file = part->elts;

    क्रम (i = 0; /* void */ ; i++) {

        अगर (i >= part->nelts) {
            अगर (part->अग्र == शून्य) {
                अवरोध;
            }
            part = part->अग्र;
            file = part->elts;
            i = 0;
        }

        अगर (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            जारी;
        }

        अगर (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.तथ्य);
        }
    }

    ngx_destroy_pool(conf.अस्थायी_pool);

    अगर (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_cycle(old_cycle)) {

        ngx_destroy_pool(old_cycle->pool);
        cycle->old_cycle = शून्य;

        वापस cycle;
    }


    अगर (ngx_अस्थायी_pool == शून्य) {
        ngx_अस्थायी_pool = ngx_create_pool(128, cycle->log);
        अगर (ngx_अस्थायी_pool == शून्य) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "could not create ngx_temp_pool");
            निकास(1);
        }

        n = 10;

        अगर (ngx_array_init(&ngx_old_cycles, ngx_अस्थायी_pool, n,
                           माप(ngx_cycle_t *))
            != NGX_OK)
        {
            निकास(1);
        }

        ngx_memzero(ngx_old_cycles.elts, n * माप(ngx_cycle_t *));

        ngx_cleaner_event.handler = ngx_clean_old_cycles;
        ngx_cleaner_event.log = cycle->log;
        ngx_cleaner_event.तथ्य = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_अस्थायी_pool->log = cycle->log;

    old = ngx_array_धकेलो(&ngx_old_cycles);
    अगर (old == शून्य) {
        निकास(1);
    }
    *old = old_cycle;

    अगर (!ngx_cleaner_event.timer_set) {
        ngx_add_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_set = 1;
    }

    वापस cycle;


failed:

    अगर (!ngx_is_init_cycle(old_cycle)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_cycle->conf_ctx,
                                                   ngx_core_module);
        अगर (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &cycle->open_files.part;
    file = part->elts;

    क्रम (i = 0; /* void */ ; i++) {

        अगर (i >= part->nelts) {
            अगर (part->अग्र == शून्य) {
                अवरोध;
            }
            part = part->अग्र;
            file = part->elts;
            i = 0;
        }

        अगर (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            जारी;
        }

        अगर (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.तथ्य);
        }
    }

    /* free the newly created shared memory */

    part = &cycle->shared_memory.part;
    shm_zone = part->elts;

    क्रम (i = 0; /* void */ ; i++) {

        अगर (i >= part->nelts) {
            अगर (part->अग्र == शून्य) {
                अवरोध;
            }
            part = part->अग्र;
            shm_zone = part->elts;
            i = 0;
        }

        अगर (shm_zone[i].shm.addr == शून्य) {
            जारी;
        }

        opart = &old_cycle->shared_memory.part;
        oshm_zone = opart->elts;

        क्रम (n = 0; /* void */ ; n++) {

            अगर (n >= opart->nelts) {
                अगर (opart->अग्र == शून्य) {
                    अवरोध;
                }
                opart = opart->अग्र;
                oshm_zone = opart->elts;
                n = 0;
            }

            अगर (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                जारी;
            }

            अगर (ngx_strncmp(shm_zone[i].shm.name.तथ्य,
                            oshm_zone[n].shm.name.तथ्य,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                जारी;
            }

            अगर (shm_zone[i].tag == oshm_zone[n].tag
                && shm_zone[i].shm.आकार == oshm_zone[n].shm.आकार
                && !shm_zone[i].noreuse)
            {
                जाओ old_shm_zone_found;
            }

            अवरोध;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_found:

        जारी;
    }

    अगर (ngx_test_config) {
        ngx_destroy_cycle_pools(&conf);
        वापस शून्य;
    }

    ls = cycle->listening.elts;
    क्रम (i = 0; i < cycle->listening.nelts; i++) {
        अगर (ls[i].fd == (ngx_socket_t) -1 || !ls[i].खोलो) {
            जारी;
        }

        अगर (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_text);
        }
    }

    ngx_destroy_cycle_pools(&conf);

    वापस शून्य;
}


अटल व्योम
ngx_destroy_cycle_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->अस्थायी_pool);
    ngx_destroy_pool(conf->pool);
}


अटल ngx_int_t
ngx_init_zone_pool(ngx_cycle_t *cycle, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    अगर (zn->shm.exists) {

        अगर (sp == sp->addr) {
            वापस NGX_OK;
        }

#अगर (NGX_WIN32)

        /* remap at the required address */

        अगर (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            वापस NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        अगर (sp == sp->addr) {
            वापस NGX_OK;
        }

#पूर्ण_अगर

        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        वापस NGX_ERROR;
    }

    sp->end = zn->shm.addr + zn->shm.आकार;
    sp->min_shift = 3;
    sp->addr = zn->shm.addr;

#अगर (NGX_HAVE_ATOMIC_OPS)

    file = शून्य;

#अन्यथा

    file = ngx_pnalloc(cycle->pool,
                       cycle->lock_file.len + zn->shm.name.len + 1);
    अगर (file == शून्य) {
        वापस NGX_ERROR;
    }

    (व्योम) ngx_sprintf(file, "%V%V%Z", &cycle->lock_file, &zn->shm.name);

#पूर्ण_अगर

    अगर (ngx_shmtx_create(&sp->mutex, &sp->lock, file) != NGX_OK) {
        वापस NGX_ERROR;
    }

    ngx_slab_init(sp);

    वापस NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log)
{
    माप_प्रकार      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    अगर (ngx_process > NGX_PROCESS_MASTER) {
        वापस NGX_OK;
    }

    ngx_memzero(&file, माप(ngx_file_t));

    file.name = *name;
    file.log = log;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.तथ्य, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    अगर (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.तथ्य);
        वापस NGX_ERROR;
    }

    rc = NGX_OK;

    अगर (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        अगर (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    अगर (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.तथ्य);
    }

    वापस rc;
}


व्योम
ngx_delete_pidfile(ngx_cycle_t *cycle)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    name = ngx_new_द्विआधारी ? ccf->oldpid.तथ्य : ccf->pid.तथ्य;

    अगर (ngx_delete_file(name) == NGX_FILE_ERROR) {
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

    ngx_memzero(&file, माप(ngx_file_t));

    file.name = ccf->pid;
    file.log = cycle->log;

    file.fd = ngx_open_file(file.name.तथ्य, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    अगर (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.तथ्य);
        वापस 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    अगर (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.तथ्य);
    }

    अगर (n == NGX_ERROR) {
        वापस 1;
    }

    जबतक (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    अगर (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.तथ्य);
        वापस 1;
    }

    वापस ngx_os_signal_process(cycle, sig, pid);

}


अटल ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *log)
{
#अगर !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    अगर (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        वापस NGX_ERROR;
    }

    अगर (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    अगर (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#पूर्ण_अगर

    वापस NGX_OK;
}


व्योम
ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &cycle->open_files.part;
    file = part->elts;

    क्रम (i = 0; /* void */ ; i++) {

        अगर (i >= part->nelts) {
            अगर (part->अग्र == शून्य) {
                अवरोध;
            }
            part = part->अग्र;
            file = part->elts;
            i = 0;
        }

        अगर (file[i].name.len == 0) {
            जारी;
        }

        अगर (file[i].flush) {
            file[i].flush(&file[i], cycle->log);
        }

        fd = ngx_open_file(file[i].name.तथ्य, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.तथ्य, file[i].fd, fd);

        अगर (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.तथ्य);
            जारी;
        }

#अगर !(NGX_WIN32)
        अगर (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            अगर (ngx_file_info(file[i].name.तथ्य, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.तथ्य);

                अगर (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.तथ्य);
                }

                जारी;
            }

            अगर (fi.st_uid != user) {
                अगर (chown((स्थिर अक्षर *) file[i].name.तथ्य, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.तथ्य, user);

                    अगर (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.तथ्य);
                    }

                    जारी;
                }
            }

            अगर ((fi.st_mode & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_mode |= (S_IRUSR|S_IWUSR);

                अगर (chmod((स्थिर अक्षर *) file[i].name.तथ्य, fi.st_mode) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.तथ्य);

                    अगर (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.तथ्य);
                    }

                    जारी;
                }
            }
        }

        अगर (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.तथ्य);

            अगर (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.तथ्य);
            }

            जारी;
        }
#पूर्ण_अगर

        अगर (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.तथ्य);
        }

        file[i].fd = fd;
    }

    (व्योम) ngx_log_redirect_stderr(cycle);
}


ngx_shm_zone_t *
ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name, माप_प्रकार आकार, व्योम *tag)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->cycle->shared_memory.part;
    shm_zone = part->elts;

    क्रम (i = 0; /* void */ ; i++) {

        अगर (i >= part->nelts) {
            अगर (part->अग्र == शून्य) {
                अवरोध;
            }
            part = part->अग्र;
            shm_zone = part->elts;
            i = 0;
        }

        अगर (name->len != shm_zone[i].shm.name.len) {
            जारी;
        }

        अगर (ngx_strncmp(name->तथ्य, shm_zone[i].shm.name.तथ्य, name->len)
            != 0)
        {
            जारी;
        }

        अगर (tag != shm_zone[i].tag) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            वापस शून्य;
        }

        अगर (shm_zone[i].shm.आकार == 0) {
            shm_zone[i].shm.आकार = आकार;
        }

        अगर (आकार && आकार != shm_zone[i].shm.आकार) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            आकार, &shm_zone[i].shm.name, shm_zone[i].shm.आकार);
            वापस शून्य;
        }

        वापस &shm_zone[i];
    }

    shm_zone = ngx_list_धकेलो(&cf->cycle->shared_memory);

    अगर (shm_zone == शून्य) {
        वापस शून्य;
    }

    shm_zone->तथ्य = शून्य;
    shm_zone->shm.log = cf->cycle->log;
    shm_zone->shm.addr = शून्य;
    shm_zone->shm.आकार = आकार;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = शून्य;
    shm_zone->tag = tag;
    shm_zone->noreuse = 0;

    वापस shm_zone;
}


अटल व्योम
ngx_clean_old_cycles(ngx_event_t *ev)
{
    ngx_uint_t     i, n, found, live;
    ngx_log_t     *log;
    ngx_cycle_t  **cycle;

    log = ngx_cycle->log;
    ngx_अस्थायी_pool->log = log;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, log, 0, "clean old cycles");

    live = 0;

    cycle = ngx_old_cycles.elts;
    क्रम (i = 0; i < ngx_old_cycles.nelts; i++) {

        अगर (cycle[i] == शून्य) {
            जारी;
        }

        found = 0;

        क्रम (n = 0; n < cycle[i]->connection_n; n++) {
            अगर (cycle[i]->connections[n].fd != (ngx_socket_t) -1) {
                found = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "live fd:%ui", n);

                अवरोध;
            }
        }

        अगर (found) {
            live = 1;
            जारी;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(cycle[i]->pool);
        cycle[i] = शून्य;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, log, 0, "old cycles status: %ui", live);

    अगर (live) {
        ngx_add_timer(ev, 30000);

    } अन्यथा {
        ngx_destroy_pool(ngx_अस्थायी_pool);
        ngx_अस्थायी_pool = शून्य;
        ngx_old_cycles.nelts = 0;
    }
}


व्योम
ngx_set_shutdown_timer(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    अगर (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.तथ्य = cycle;
        ngx_shutdown_event.log = cycle->log;
        ngx_shutdown_event.cancelable = 1;

        ngx_add_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


अटल व्योम
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_cycle_t       *cycle;
    ngx_connection_t  *c;

    cycle = ev->तथ्य;

    c = cycle->connections;

    क्रम (i = 0; i < cycle->connection_n; i++) {

        अगर (c[i].fd == (ngx_socket_t) -1
            || c[i].पढ़ो == शून्य
            || c[i].पढ़ो->accept
            || c[i].पढ़ो->channel
            || c[i].पढ़ो->resolver)
        {
            जारी;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,
                       "*%uA shutdown timeout", c[i].संख्या);

        c[i].बंद = 1;
        c[i].error = 1;

        c[i].पढ़ो->handler(c[i].पढ़ो);
    }
}
