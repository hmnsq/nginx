
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_event.h>


ساکن خالی ngx_destroy_چکر_pools(ngx_conf_t *conf);
ساکن ngx_int_t ngx_init_zone_pool(ngx_چکر_t *چکر,
    ngx_shm_zone_t *shm_zone);
ساکن ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *لوگارتھم);
ساکن خالی ngx_clean_old_ادوار(ngx_event_t *ev);
ساکن خالی ngx_shutdown_timer_handler(ngx_event_t *ev);


متلون ngx_چکر_t  *ngx_چکر;
ngx_array_t            ngx_old_ادوار;

ساکن ngx_pool_t     *ngx_عارضی_pool;
ساکن ngx_event_t     ngx_cleaner_event;
ساکن ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_منوال;


/* STUB NAME */
ساکن ngx_connection_t  dumb;
/* STUB */


ngx_چکر_t *
ngx_init_چکر(ngx_چکر_t *old_چکر)
{
    خالی                *rv, *ڈیٹا;
    حرف               **senv;
    ngx_uint_t           i, n;
    ngx_log_t           *لوگارتھم;
    ngx_وقت_t          *tp;
    ngx_conf_t           conf;
    ngx_pool_t          *pool;
    ngx_چکر_t         *چکر, **old;
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

    ngx_وقت_update();


    لوگارتھم = old_چکر->لوگارتھم;

    pool = ngx_create_pool(NGX_چکر_POOL_سائز, لوگارتھم);
    اگر (pool == تہی) {
        واپس تہی;
    }
    pool->لوگارتھم = لوگارتھم;

    چکر = ngx_pcalloc(pool, حجم(ngx_چکر_t));
    اگر (چکر == تہی) {
        ngx_destroy_pool(pool);
        واپس تہی;
    }

    چکر->pool = pool;
    چکر->لوگارتھم = لوگارتھم;
    چکر->old_چکر = old_چکر;

    چکر->conf_سابقہ.len = old_چکر->conf_سابقہ.len;
    چکر->conf_سابقہ.ڈیٹا = ngx_pstrdup(pool, &old_چکر->conf_سابقہ);
    اگر (چکر->conf_سابقہ.ڈیٹا == تہی) {
        ngx_destroy_pool(pool);
        واپس تہی;
    }

    چکر->سابقہ.len = old_چکر->سابقہ.len;
    چکر->سابقہ.ڈیٹا = ngx_pstrdup(pool, &old_چکر->سابقہ);
    اگر (چکر->سابقہ.ڈیٹا == تہی) {
        ngx_destroy_pool(pool);
        واپس تہی;
    }

    چکر->error_log.len = old_چکر->error_log.len;
    چکر->error_log.ڈیٹا = ngx_pnalloc(pool, old_چکر->error_log.len + 1);
    اگر (چکر->error_log.ڈیٹا == تہی) {
        ngx_destroy_pool(pool);
        واپس تہی;
    }
    ngx_cpystrn(چکر->error_log.ڈیٹا, old_چکر->error_log.ڈیٹا,
                old_چکر->error_log.len + 1);

    چکر->conf_file.len = old_چکر->conf_file.len;
    چکر->conf_file.ڈیٹا = ngx_pnalloc(pool, old_چکر->conf_file.len + 1);
    اگر (چکر->conf_file.ڈیٹا == تہی) {
        ngx_destroy_pool(pool);
        واپس تہی;
    }
    ngx_cpystrn(چکر->conf_file.ڈیٹا, old_چکر->conf_file.ڈیٹا,
                old_چکر->conf_file.len + 1);

    اگر (old_چکر->conf_param.len) {
        چکر->conf_param.len = old_چکر->conf_param.len;
        چکر->conf_param.ڈیٹا = ngx_pstrdup(pool, &old_چکر->conf_param);
        اگر (چکر->conf_param.ڈیٹا == تہی) {
            ngx_destroy_pool(pool);
            واپس تہی;
        }
    }


    n = old_چکر->paths.nelts ? old_چکر->paths.nelts : 10;

    اگر (ngx_array_init(&چکر->paths, pool, n, حجم(ngx_راستہ_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        واپس تہی;
    }

    ngx_memzero(چکر->paths.elts, n * حجم(ngx_راستہ_t *));


    اگر (ngx_array_init(&چکر->config_dump, pool, 1, حجم(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        واپس تہی;
    }

    ngx_rbtree_init(&چکر->config_dump_rbtree, &چکر->config_dump_sentinel,
                    ngx_str_rbtree_داخل_کرو_قدر);

    اگر (old_چکر->open_files.part.nelts) {
        n = old_چکر->open_files.part.nelts;
        برائے (part = old_چکر->open_files.part.اگلا; part; part = part->اگلا) {
            n += part->nelts;
        }

    } ورنہ {
        n = 20;
    }

    اگر (ngx_list_init(&چکر->open_files, pool, n, حجم(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        واپس تہی;
    }


    اگر (old_چکر->shared_حافظہ.part.nelts) {
        n = old_چکر->shared_حافظہ.part.nelts;
        برائے (part = old_چکر->shared_حافظہ.part.اگلا; part; part = part->اگلا)
        {
            n += part->nelts;
        }

    } ورنہ {
        n = 1;
    }

    اگر (ngx_list_init(&چکر->shared_حافظہ, pool, n, حجم(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        واپس تہی;
    }

    n = old_چکر->listening.nelts ? old_چکر->listening.nelts : 10;

    اگر (ngx_array_init(&چکر->listening, pool, n, حجم(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        واپس تہی;
    }

    ngx_memzero(چکر->listening.elts, n * حجم(ngx_listening_t));


    ngx_قطار_init(&چکر->reusable_connections_قطار);


    چکر->conf_ctx = ngx_pcalloc(pool, ngx_max_module * حجم(خالی *));
    اگر (چکر->conf_ctx == تہی) {
        ngx_destroy_pool(pool);
        واپس تہی;
    }


    اگر (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        واپس تہی;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    چکر->hostname.len = ngx_strlen(hostname);

    چکر->hostname.ڈیٹا = ngx_pnalloc(pool, چکر->hostname.len);
    اگر (چکر->hostname.ڈیٹا == تہی) {
        ngx_destroy_pool(pool);
        واپس تہی;
    }

    ngx_strlow(چکر->hostname.ڈیٹا, (u_char *) hostname, چکر->hostname.len);


    اگر (ngx_چکر_modules(چکر) != NGX_OK) {
        ngx_destroy_pool(pool);
        واپس تہی;
    }


    برائے (i = 0; چکر->modules[i]; i++) {
        اگر (چکر->modules[i]->type != NGX_CORE_MODULE) {
            جاری;
        }

        module = چکر->modules[i]->ctx;

        اگر (module->create_conf) {
            rv = module->create_conf(چکر);
            اگر (rv == تہی) {
                ngx_destroy_pool(pool);
                واپس تہی;
            }
            چکر->conf_ctx[چکر->modules[i]->اشاریہ] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, حجم(ngx_conf_t));
    /* STUB: init array ? */
    conf.دلائل = ngx_array_create(pool, 10, حجم(ngx_str_t));
    اگر (conf.دلائل == تہی) {
        ngx_destroy_pool(pool);
        واپس تہی;
    }

    conf.عارضی_pool = ngx_create_pool(NGX_چکر_POOL_سائز, لوگارتھم);
    اگر (conf.عارضی_pool == تہی) {
        ngx_destroy_pool(pool);
        واپس تہی;
    }


    conf.ctx = چکر->conf_ctx;
    conf.چکر = چکر;
    conf.pool = pool;
    conf.لوگارتھم = لوگارتھم;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#اگر 0
    لوگارتھم->log_level = NGX_LOG_DEBUG_ALL;
#اگر_ختم

    اگر (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_چکر_pools(&conf);
        واپس تہی;
    }

    اگر (ngx_conf_parse(&conf, &چکر->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_چکر_pools(&conf);
        واپس تہی;
    }

    اگر (ngx_test_config && !ngx_quiet_منوال) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       چکر->conf_file.ڈیٹا);
    }

    برائے (i = 0; چکر->modules[i]; i++) {
        اگر (چکر->modules[i]->type != NGX_CORE_MODULE) {
            جاری;
        }

        module = چکر->modules[i]->ctx;

        اگر (module->init_conf) {
            اگر (module->init_conf(چکر,
                                  چکر->conf_ctx[چکر->modules[i]->اشاریہ])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_چکر_pools(&conf);
                واپس تہی;
            }
        }
    }

    اگر (ngx_process == NGX_PROCESS_SIGNALLER) {
        واپس چکر;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    اگر (ngx_test_config) {

        اگر (ngx_create_pidfile(&ccf->pid, لوگارتھم) != NGX_OK) {
            جاؤ failed;
        }

    } ورنہ اگر (!ngx_is_init_چکر(old_چکر)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_چکر->conf_ctx,
                                                   ngx_core_module);
        اگر (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.ڈیٹا, old_ccf->pid.ڈیٹا) != 0)
        {
            /* new pid file name */

            اگر (ngx_create_pidfile(&ccf->pid, لوگارتھم) != NGX_OK) {
                جاؤ failed;
            }

            ngx_delete_pidfile(old_چکر);
        }
    }


    اگر (ngx_test_lockfile(چکر->تالا_file.ڈیٹا, لوگارتھم) != NGX_OK) {
        جاؤ failed;
    }


    اگر (ngx_create_paths(چکر, ccf->user) != NGX_OK) {
        جاؤ failed;
    }


    اگر (ngx_log_open_default(چکر) != NGX_OK) {
        جاؤ failed;
    }

    /* open the new files */

    part = &چکر->open_files.part;
    file = part->elts;

    برائے (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->اگلا == تہی) {
                توڑو;
            }
            part = part->اگلا;
            file = part->elts;
            i = 0;
        }

        اگر (file[i].name.len == 0) {
            جاری;
        }

        file[i].fd = ngx_open_file(file[i].name.ڈیٹا,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, لوگارتھم, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.ڈیٹا);

        اگر (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.ڈیٹا);
            جاؤ failed;
        }

#اگر !(NGX_WIN32)
        اگر (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.ڈیٹا);
            جاؤ failed;
        }
#اگر_ختم
    }

    چکر->لوگارتھم = &چکر->new_log;
    pool->لوگارتھم = &چکر->new_log;


    /* create shared memory */

    part = &چکر->shared_حافظہ.part;
    shm_zone = part->elts;

    برائے (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->اگلا == تہی) {
                توڑو;
            }
            part = part->اگلا;
            shm_zone = part->elts;
            i = 0;
        }

        اگر (shm_zone[i].shm.سائز == 0) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            جاؤ failed;
        }

        shm_zone[i].shm.لوگارتھم = چکر->لوگارتھم;

        opart = &old_چکر->shared_حافظہ.part;
        oshm_zone = opart->elts;

        ڈیٹا = تہی;

        برائے (n = 0; /* void */ ; n++) {

            اگر (n >= opart->nelts) {
                اگر (opart->اگلا == تہی) {
                    توڑو;
                }
                opart = opart->اگلا;
                oshm_zone = opart->elts;
                n = 0;
            }

            اگر (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                جاری;
            }

            اگر (ngx_strncmp(shm_zone[i].shm.name.ڈیٹا,
                            oshm_zone[n].shm.name.ڈیٹا,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                جاری;
            }

            اگر (shm_zone[i].لیبل == oshm_zone[n].لیبل && shm_zone[i].noreuse) {
                ڈیٹا = oshm_zone[n].ڈیٹا;
                توڑو;
            }

            اگر (shm_zone[i].لیبل == oshm_zone[n].لیبل
                && shm_zone[i].shm.سائز == oshm_zone[n].shm.سائز)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#اگر (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#اگر_ختم

                اگر (shm_zone[i].init(&shm_zone[i], oshm_zone[n].ڈیٹا)
                    != NGX_OK)
                {
                    جاؤ failed;
                }

                جاؤ shm_zone_ملا;
            }

            توڑو;
        }

        اگر (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            جاؤ failed;
        }

        اگر (ngx_init_zone_pool(چکر, &shm_zone[i]) != NGX_OK) {
            جاؤ failed;
        }

        اگر (shm_zone[i].init(&shm_zone[i], ڈیٹا) != NGX_OK) {
            جاؤ failed;
        }

    shm_zone_ملا:

        جاری;
    }


    /* handle the listening sockets */

    اگر (old_چکر->listening.nelts) {
        ls = old_چکر->listening.elts;
        برائے (i = 0; i < old_چکر->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = چکر->listening.elts;
        برائے (n = 0; n < چکر->listening.nelts; n++) {

            برائے (i = 0; i < old_چکر->listening.nelts; i++) {
                اگر (ls[i].ignore) {
                    جاری;
                }

                اگر (ls[i].remain) {
                    جاری;
                }

                اگر (ls[i].type != nls[n].type) {
                    جاری;
                }

                اگر (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].پچھلا_2 = &ls[i];

                    اگر (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } ورنہ {
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
                            nls[n].جوڑو_deferred = 1;
                        }

                    } ورنہ اگر (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } ورنہ اگر (nls[n].accept_filter) {
                        nls[n].جوڑو_deferred = 1;
                    }
#اگر_ختم

#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    اگر (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } ورنہ اگر (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].جوڑو_deferred = 1;
                    }
#اگر_ختم

#اگر (NGX_HAVE_REUSEPORT)
                    اگر (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].جوڑو_reuseport = 1;
                    }
#اگر_ختم

                    توڑو;
                }
            }

            اگر (nls[n].fd == (ngx_socket_t) -1) {
#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                اگر (nls[n].accept_filter) {
                    nls[n].جوڑو_deferred = 1;
                }
#اگر_ختم
#اگر (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                اگر (nls[n].deferred_accept) {
                    nls[n].جوڑو_deferred = 1;
                }
#اگر_ختم
            }
        }

    } ورنہ {
#اگر (NGX_HAVE_DEFERRED_ACCEPT)
        ls = چکر->listening.elts;
        برائے (i = 0; i < چکر->listening.nelts; i++) {
#اگر_تعریف_شدہ SO_ACCEPTFILTER
            اگر (ls[i].accept_filter) {
                ls[i].جوڑو_deferred = 1;
            }
#اگر_ختم
#اگر_تعریف_شدہ TCP_DEFER_ACCEPT
            اگر (ls[i].deferred_accept) {
                ls[i].جوڑو_deferred = 1;
            }
#اگر_ختم
        }
#اگر_ختم
    }

    اگر (ngx_open_listening_sockets(چکر) != NGX_OK) {
        جاؤ failed;
    }

    اگر (!ngx_test_config) {
        ngx_configure_listening_sockets(چکر);
    }


    /* commit the new cycle configuration */

    اگر (!ngx_use_stderr) {
        (خالی) ngx_log_redirect_stderr(چکر);
    }

    pool->لوگارتھم = چکر->لوگارتھم;

    اگر (ngx_init_modules(چکر) != NGX_OK) {
        /* fatal */
        باہر_نکلو(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_چکر->shared_حافظہ.part;
    oshm_zone = opart->elts;

    برائے (i = 0; /* void */ ; i++) {

        اگر (i >= opart->nelts) {
            اگر (opart->اگلا == تہی) {
                جاؤ old_shm_zone_مکمل;
            }
            opart = opart->اگلا;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &چکر->shared_حافظہ.part;
        shm_zone = part->elts;

        برائے (n = 0; /* void */ ; n++) {

            اگر (n >= part->nelts) {
                اگر (part->اگلا == تہی) {
                    توڑو;
                }
                part = part->اگلا;
                shm_zone = part->elts;
                n = 0;
            }

            اگر (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                جاری;
            }

            اگر (ngx_strncmp(oshm_zone[i].shm.name.ڈیٹا,
                            shm_zone[n].shm.name.ڈیٹا,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                جاری;
            }

            اگر (oshm_zone[i].لیبل == shm_zone[n].لیبل
                && oshm_zone[i].shm.سائز == shm_zone[n].shm.سائز
                && !oshm_zone[i].noreuse)
            {
                جاؤ زندہ_shm_zone;
            }

            توڑو;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    زندہ_shm_zone:

        جاری;
    }

old_shm_zone_مکمل:


    /* close the unnecessary listening sockets */

    ls = old_چکر->listening.elts;
    برائے (i = 0; i < old_چکر->listening.nelts; i++) {

        اگر (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            جاری;
        }

        اگر (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_متن);
        }

#اگر (NGX_HAVE_UNIX_DOMAIN)

        اگر (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_متن.ڈیٹا + حجم("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, چکر->لوگارتھم, 0,
                          "deleting socket %s", name);

            اگر (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#اگر_ختم
    }


    /* close the unnecessary open files */

    part = &old_چکر->open_files.part;
    file = part->elts;

    برائے (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->اگلا == تہی) {
                توڑو;
            }
            part = part->اگلا;
            file = part->elts;
            i = 0;
        }

        اگر (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            جاری;
        }

        اگر (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.ڈیٹا);
        }
    }

    ngx_destroy_pool(conf.عارضی_pool);

    اگر (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_چکر(old_چکر)) {

        ngx_destroy_pool(old_چکر->pool);
        چکر->old_چکر = تہی;

        واپس چکر;
    }


    اگر (ngx_عارضی_pool == تہی) {
        ngx_عارضی_pool = ngx_create_pool(128, چکر->لوگارتھم);
        اگر (ngx_عارضی_pool == تہی) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                          "could not create ngx_temp_pool");
            باہر_نکلو(1);
        }

        n = 10;

        اگر (ngx_array_init(&ngx_old_ادوار, ngx_عارضی_pool, n,
                           حجم(ngx_چکر_t *))
            != NGX_OK)
        {
            باہر_نکلو(1);
        }

        ngx_memzero(ngx_old_ادوار.elts, n * حجم(ngx_چکر_t *));

        ngx_cleaner_event.handler = ngx_clean_old_ادوار;
        ngx_cleaner_event.لوگارتھم = چکر->لوگارتھم;
        ngx_cleaner_event.ڈیٹا = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_عارضی_pool->لوگارتھم = چکر->لوگارتھم;

    old = ngx_array_ڈالو(&ngx_old_ادوار);
    اگر (old == تہی) {
        باہر_نکلو(1);
    }
    *old = old_چکر;

    اگر (!ngx_cleaner_event.timer_سیٹ) {
        ngx_جوڑو_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_سیٹ = 1;
    }

    واپس چکر;


failed:

    اگر (!ngx_is_init_چکر(old_چکر)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_چکر->conf_ctx,
                                                   ngx_core_module);
        اگر (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &چکر->open_files.part;
    file = part->elts;

    برائے (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->اگلا == تہی) {
                توڑو;
            }
            part = part->اگلا;
            file = part->elts;
            i = 0;
        }

        اگر (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            جاری;
        }

        اگر (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.ڈیٹا);
        }
    }

    /* free the newly created shared memory */

    part = &چکر->shared_حافظہ.part;
    shm_zone = part->elts;

    برائے (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->اگلا == تہی) {
                توڑو;
            }
            part = part->اگلا;
            shm_zone = part->elts;
            i = 0;
        }

        اگر (shm_zone[i].shm.addr == تہی) {
            جاری;
        }

        opart = &old_چکر->shared_حافظہ.part;
        oshm_zone = opart->elts;

        برائے (n = 0; /* void */ ; n++) {

            اگر (n >= opart->nelts) {
                اگر (opart->اگلا == تہی) {
                    توڑو;
                }
                opart = opart->اگلا;
                oshm_zone = opart->elts;
                n = 0;
            }

            اگر (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                جاری;
            }

            اگر (ngx_strncmp(shm_zone[i].shm.name.ڈیٹا,
                            oshm_zone[n].shm.name.ڈیٹا,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                جاری;
            }

            اگر (shm_zone[i].لیبل == oshm_zone[n].لیبل
                && shm_zone[i].shm.سائز == oshm_zone[n].shm.سائز
                && !shm_zone[i].noreuse)
            {
                جاؤ old_shm_zone_ملا;
            }

            توڑو;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_ملا:

        جاری;
    }

    اگر (ngx_test_config) {
        ngx_destroy_چکر_pools(&conf);
        واپس تہی;
    }

    ls = چکر->listening.elts;
    برائے (i = 0; i < چکر->listening.nelts; i++) {
        اگر (ls[i].fd == (ngx_socket_t) -1 || !ls[i].open) {
            جاری;
        }

        اگر (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_متن);
        }
    }

    ngx_destroy_چکر_pools(&conf);

    واپس تہی;
}


ساکن خالی
ngx_destroy_چکر_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->عارضی_pool);
    ngx_destroy_pool(conf->pool);
}


ساکن ngx_int_t
ngx_init_zone_pool(ngx_چکر_t *چکر, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    اگر (zn->shm.exists) {

        اگر (sp == sp->addr) {
            واپس NGX_OK;
        }

#اگر (NGX_WIN32)

        /* remap at the required address */

        اگر (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            واپس NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        اگر (sp == sp->addr) {
            واپس NGX_OK;
        }

#اگر_ختم

        ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        واپس NGX_ERROR;
    }

    sp->انتہا = zn->shm.addr + zn->shm.سائز;
    sp->min_سرکاؤ = 3;
    sp->addr = zn->shm.addr;

#اگر (NGX_HAVE_ATOMIC_OPS)

    file = تہی;

#ورنہ

    file = ngx_pnalloc(چکر->pool,
                       چکر->تالا_file.len + zn->shm.name.len + 1);
    اگر (file == تہی) {
        واپس NGX_ERROR;
    }

    (خالی) ngx_sprintf(file, "%V%V%Z", &چکر->تالا_file, &zn->shm.name);

#اگر_ختم

    اگر (ngx_shmtx_create(&sp->mutex, &sp->تالا, file) != NGX_OK) {
        واپس NGX_ERROR;
    }

    ngx_slab_init(sp);

    واپس NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *لوگارتھم)
{
    سائز_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    اگر (ngx_process > NGX_PROCESS_MASTER) {
        واپس NGX_OK;
    }

    ngx_memzero(&file, حجم(ngx_file_t));

    file.name = *name;
    file.لوگارتھم = لوگارتھم;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.ڈیٹا, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    اگر (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.ڈیٹا);
        واپس NGX_ERROR;
    }

    rc = NGX_OK;

    اگر (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        اگر (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    اگر (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.ڈیٹا);
    }

    واپس rc;
}


خالی
ngx_delete_pidfile(ngx_چکر_t *چکر)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    name = ngx_new_ثنائی ? ccf->oldpid.ڈیٹا : ccf->pid.ڈیٹا;

    اگر (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_چکر_t *چکر, حرف *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, چکر->لوگارتھم, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    ngx_memzero(&file, حجم(ngx_file_t));

    file.name = ccf->pid;
    file.لوگارتھم = چکر->لوگارتھم;

    file.fd = ngx_open_file(file.name.ڈیٹا, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    اگر (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, چکر->لوگارتھم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.ڈیٹا);
        واپس 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    اگر (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, چکر->لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.ڈیٹا);
    }

    اگر (n == NGX_ERROR) {
        واپس 1;
    }

    جب (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    اگر (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, چکر->لوگارتھم, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.ڈیٹا);
        واپس 1;
    }

    واپس ngx_os_signal_process(چکر, sig, pid);

}


ساکن ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *لوگارتھم)
{
#اگر !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    اگر (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, لوگارتھم, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        واپس NGX_ERROR;
    }

    اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    اگر (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, لوگارتھم, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#اگر_ختم

    واپس NGX_OK;
}


خالی
ngx_reopen_files(ngx_چکر_t *چکر, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &چکر->open_files.part;
    file = part->elts;

    برائے (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->اگلا == تہی) {
                توڑو;
            }
            part = part->اگلا;
            file = part->elts;
            i = 0;
        }

        اگر (file[i].name.len == 0) {
            جاری;
        }

        اگر (file[i].flush) {
            file[i].flush(&file[i], چکر->لوگارتھم);
        }

        fd = ngx_open_file(file[i].name.ڈیٹا, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, چکر->لوگارتھم, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.ڈیٹا, file[i].fd, fd);

        اگر (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.ڈیٹا);
            جاری;
        }

#اگر !(NGX_WIN32)
        اگر (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            اگر (ngx_file_info(file[i].name.ڈیٹا, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.ڈیٹا);

                اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.ڈیٹا);
                }

                جاری;
            }

            اگر (fi.st_uid != user) {
                اگر (chown((ثابت حرف *) file[i].name.ڈیٹا, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.ڈیٹا, user);

                    اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.ڈیٹا);
                    }

                    جاری;
                }
            }

            اگر ((fi.st_منوال & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_منوال |= (S_IRUSR|S_IWUSR);

                اگر (chmod((ثابت حرف *) file[i].name.ڈیٹا, fi.st_منوال) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.ڈیٹا);

                    اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.ڈیٹا);
                    }

                    جاری;
                }
            }
        }

        اگر (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.ڈیٹا);

            اگر (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.ڈیٹا);
            }

            جاری;
        }
#اگر_ختم

        اگر (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, چکر->لوگارتھم, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.ڈیٹا);
        }

        file[i].fd = fd;
    }

    (خالی) ngx_log_redirect_stderr(چکر);
}


ngx_shm_zone_t *
ngx_shared_حافظہ_جوڑو(ngx_conf_t *cf, ngx_str_t *name, سائز_t سائز, خالی *لیبل)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->چکر->shared_حافظہ.part;
    shm_zone = part->elts;

    برائے (i = 0; /* void */ ; i++) {

        اگر (i >= part->nelts) {
            اگر (part->اگلا == تہی) {
                توڑو;
            }
            part = part->اگلا;
            shm_zone = part->elts;
            i = 0;
        }

        اگر (name->len != shm_zone[i].shm.name.len) {
            جاری;
        }

        اگر (ngx_strncmp(name->ڈیٹا, shm_zone[i].shm.name.ڈیٹا, name->len)
            != 0)
        {
            جاری;
        }

        اگر (لیبل != shm_zone[i].لیبل) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            واپس تہی;
        }

        اگر (shm_zone[i].shm.سائز == 0) {
            shm_zone[i].shm.سائز = سائز;
        }

        اگر (سائز && سائز != shm_zone[i].shm.سائز) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            سائز, &shm_zone[i].shm.name, shm_zone[i].shm.سائز);
            واپس تہی;
        }

        واپس &shm_zone[i];
    }

    shm_zone = ngx_list_ڈالو(&cf->چکر->shared_حافظہ);

    اگر (shm_zone == تہی) {
        واپس تہی;
    }

    shm_zone->ڈیٹا = تہی;
    shm_zone->shm.لوگارتھم = cf->چکر->لوگارتھم;
    shm_zone->shm.addr = تہی;
    shm_zone->shm.سائز = سائز;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = تہی;
    shm_zone->لیبل = لیبل;
    shm_zone->noreuse = 0;

    واپس shm_zone;
}


ساکن خالی
ngx_clean_old_ادوار(ngx_event_t *ev)
{
    ngx_uint_t     i, n, ملا, زندہ;
    ngx_log_t     *لوگارتھم;
    ngx_چکر_t  **چکر;

    لوگارتھم = ngx_چکر->لوگارتھم;
    ngx_عارضی_pool->لوگارتھم = لوگارتھم;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, لوگارتھم, 0, "clean old cycles");

    زندہ = 0;

    چکر = ngx_old_ادوار.elts;
    برائے (i = 0; i < ngx_old_ادوار.nelts; i++) {

        اگر (چکر[i] == تہی) {
            جاری;
        }

        ملا = 0;

        برائے (n = 0; n < چکر[i]->connection_n; n++) {
            اگر (چکر[i]->connections[n].fd != (ngx_socket_t) -1) {
                ملا = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوگارتھم, 0, "live fd:%ui", n);

                توڑو;
            }
        }

        اگر (ملا) {
            زندہ = 1;
            جاری;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوگارتھم, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(چکر[i]->pool);
        چکر[i] = تہی;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, لوگارتھم, 0, "old cycles status: %ui", زندہ);

    اگر (زندہ) {
        ngx_جوڑو_timer(ev, 30000);

    } ورنہ {
        ngx_destroy_pool(ngx_عارضی_pool);
        ngx_عارضی_pool = تہی;
        ngx_old_ادوار.nelts = 0;
    }
}


خالی
ngx_سیٹ_shutdown_timer(ngx_چکر_t *چکر)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(چکر->conf_ctx, ngx_core_module);

    اگر (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.ڈیٹا = چکر;
        ngx_shutdown_event.لوگارتھم = چکر->لوگارتھم;
        ngx_shutdown_event.cancelable = 1;

        ngx_جوڑو_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


ساکن خالی
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_چکر_t       *چکر;
    ngx_connection_t  *c;

    چکر = ev->ڈیٹا;

    c = چکر->connections;

    برائے (i = 0; i < چکر->connection_n; i++) {

        اگر (c[i].fd == (ngx_socket_t) -1
            || c[i].اقرأ == تہی
            || c[i].اقرأ->accept
            || c[i].اقرأ->channel
            || c[i].اقرأ->resolver)
        {
            جاری;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->لوگارتھم, 0,
                       "*%uA shutdown timeout", c[i].نمبر);

        c[i].close = 1;
        c[i].error = 1;

        c[i].اقرأ->handler(c[i].اقرأ);
    }
}
