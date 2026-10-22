
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <ngx_event.h>


स्थिर शून्य ngx_destroy_आवृत्तिः_pools(ngx_conf_t *conf);
स्थिर ngx_int_t ngx_init_zone_pool(ngx_आवृत्तिः_t *आवृत्तिः,
    ngx_shm_zone_t *shm_zone);
स्थिर ngx_int_t ngx_test_lockfile(u_char *file, ngx_log_t *लघुगणकः);
स्थिर शून्य ngx_clean_old_आवृत्तयः(ngx_event_t *ev);
स्थिर शून्य ngx_shutdown_timer_handler(ngx_event_t *ev);


चञ्चल ngx_आवृत्तिः_t  *ngx_आवृत्तिः;
ngx_array_t            ngx_old_आवृत्तयः;

स्थिर ngx_pool_t     *ngx_अस्थायी_pool;
स्थिर ngx_event_t     ngx_cleaner_event;
स्थिर ngx_event_t     ngx_shutdown_event;

ngx_uint_t             ngx_test_config;
ngx_uint_t             ngx_dump_config;
ngx_uint_t             ngx_quiet_बहुलकम्;


/* STUB NAME */
स्थिर ngx_connection_t  dumb;
/* STUB */


ngx_आवृत्तिः_t *
ngx_init_आवृत्तिः(ngx_आवृत्तिः_t *old_आवृत्तिः)
{
    शून्य                *rv, *दत्त;
    वर्ण               **senv;
    ngx_uint_t           i, n;
    ngx_log_t           *लघुगणकः;
    ngx_time_t          *tp;
    ngx_conf_t           conf;
    ngx_pool_t          *pool;
    ngx_आवृत्तिः_t         *आवृत्तिः, **old;
    ngx_shm_zone_t      *shm_zone, *oshm_zone;
    ngx_list_part_t     *part, *opart;
    ngx_open_file_t     *file;
    ngx_listening_t     *ls, *nls;
    ngx_core_conf_t     *ccf, *old_ccf;
    ngx_core_module_t   *module;
    वर्ण                 hostname[NGX_MAXHOSTNAMELEN];

    ngx_timezone_update();

    /* force localtime update with a new timezone */

    tp = ngx_timeofday();
    tp->sec = 0;

    ngx_time_update();


    लघुगणकः = old_आवृत्तिः->लघुगणकः;

    pool = ngx_create_pool(NGX_आवृत्तिः_POOL_आकार, लघुगणकः);
    यदि (pool == रिक्त) {
        प्रत्यागम रिक्त;
    }
    pool->लघुगणकः = लघुगणकः;

    आवृत्तिः = ngx_pcalloc(pool, परिमाण(ngx_आवृत्तिः_t));
    यदि (आवृत्तिः == रिक्त) {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }

    आवृत्तिः->pool = pool;
    आवृत्तिः->लघुगणकः = लघुगणकः;
    आवृत्तिः->old_आवृत्तिः = old_आवृत्तिः;

    आवृत्तिः->conf_उपसर्गः.len = old_आवृत्तिः->conf_उपसर्गः.len;
    आवृत्तिः->conf_उपसर्गः.दत्त = ngx_pstrdup(pool, &old_आवृत्तिः->conf_उपसर्गः);
    यदि (आवृत्तिः->conf_उपसर्गः.दत्त == रिक्त) {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }

    आवृत्तिः->उपसर्गः.len = old_आवृत्तिः->उपसर्गः.len;
    आवृत्तिः->उपसर्गः.दत्त = ngx_pstrdup(pool, &old_आवृत्तिः->उपसर्गः);
    यदि (आवृत्तिः->उपसर्गः.दत्त == रिक्त) {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }

    आवृत्तिः->error_log.len = old_आवृत्तिः->error_log.len;
    आवृत्तिः->error_log.दत्त = ngx_pnalloc(pool, old_आवृत्तिः->error_log.len + 1);
    यदि (आवृत्तिः->error_log.दत्त == रिक्त) {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }
    ngx_cpystrn(आवृत्तिः->error_log.दत्त, old_आवृत्तिः->error_log.दत्त,
                old_आवृत्तिः->error_log.len + 1);

    आवृत्तिः->conf_file.len = old_आवृत्तिः->conf_file.len;
    आवृत्तिः->conf_file.दत्त = ngx_pnalloc(pool, old_आवृत्तिः->conf_file.len + 1);
    यदि (आवृत्तिः->conf_file.दत्त == रिक्त) {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }
    ngx_cpystrn(आवृत्तिः->conf_file.दत्त, old_आवृत्तिः->conf_file.दत्त,
                old_आवृत्तिः->conf_file.len + 1);

    यदि (old_आवृत्तिः->conf_param.len) {
        आवृत्तिः->conf_param.len = old_आवृत्तिः->conf_param.len;
        आवृत्तिः->conf_param.दत्त = ngx_pstrdup(pool, &old_आवृत्तिः->conf_param);
        यदि (आवृत्तिः->conf_param.दत्त == रिक्त) {
            ngx_destroy_pool(pool);
            प्रत्यागम रिक्त;
        }
    }


    n = old_आवृत्तिः->paths.nelts ? old_आवृत्तिः->paths.nelts : 10;

    यदि (ngx_array_init(&आवृत्तिः->paths, pool, n, परिमाण(ngx_मार्गः_t *))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }

    ngx_memzero(आवृत्तिः->paths.elts, n * परिमाण(ngx_मार्गः_t *));


    यदि (ngx_array_init(&आवृत्तिः->config_dump, pool, 1, परिमाण(ngx_conf_dump_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }

    ngx_rbtree_init(&आवृत्तिः->config_dump_rbtree, &आवृत्तिः->config_dump_sentinel,
                    ngx_str_rbtree_निवेशय_मानम्);

    यदि (old_आवृत्तिः->open_files.part.nelts) {
        n = old_आवृत्तिः->open_files.part.nelts;
        प्रति (part = old_आवृत्तिः->open_files.part.अग्र; part; part = part->अग्र) {
            n += part->nelts;
        }

    } अन्यथा {
        n = 20;
    }

    यदि (ngx_list_init(&आवृत्तिः->open_files, pool, n, परिमाण(ngx_open_file_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }


    यदि (old_आवृत्तिः->shared_स्मृतिः.part.nelts) {
        n = old_आवृत्तिः->shared_स्मृतिः.part.nelts;
        प्रति (part = old_आवृत्तिः->shared_स्मृतिः.part.अग्र; part; part = part->अग्र)
        {
            n += part->nelts;
        }

    } अन्यथा {
        n = 1;
    }

    यदि (ngx_list_init(&आवृत्तिः->shared_स्मृतिः, pool, n, परिमाण(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }

    n = old_आवृत्तिः->listening.nelts ? old_आवृत्तिः->listening.nelts : 10;

    यदि (ngx_array_init(&आवृत्तिः->listening, pool, n, परिमाण(ngx_listening_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }

    ngx_memzero(आवृत्तिः->listening.elts, n * परिमाण(ngx_listening_t));


    ngx_queue_init(&आवृत्तिः->reusable_connections_queue);


    आवृत्तिः->conf_ctx = ngx_pcalloc(pool, ngx_max_module * परिमाण(शून्य *));
    यदि (आवृत्तिः->conf_ctx == रिक्त) {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }


    यदि (gethostname(hostname, NGX_MAXHOSTNAMELEN) == -1) {
        ngx_log_error(NGX_LOG_EMERG, लघुगणकः, ngx_errno, "gethostname() failed");
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }

    /* on Linux gethostname() silently truncates name that does not fit */

    hostname[NGX_MAXHOSTNAMELEN - 1] = '\0';
    आवृत्तिः->hostname.len = ngx_strlen(hostname);

    आवृत्तिः->hostname.दत्त = ngx_pnalloc(pool, आवृत्तिः->hostname.len);
    यदि (आवृत्तिः->hostname.दत्त == रिक्त) {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }

    ngx_strlow(आवृत्तिः->hostname.दत्त, (u_char *) hostname, आवृत्तिः->hostname.len);


    यदि (ngx_आवृत्तिः_modules(आवृत्तिः) != NGX_OK) {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }


    प्रति (i = 0; आवृत्तिः->modules[i]; i++) {
        यदि (आवृत्तिः->modules[i]->type != NGX_CORE_MODULE) {
            अनुवर्त;
        }

        module = आवृत्तिः->modules[i]->ctx;

        यदि (module->create_conf) {
            rv = module->create_conf(आवृत्तिः);
            यदि (rv == रिक्त) {
                ngx_destroy_pool(pool);
                प्रत्यागम रिक्त;
            }
            आवृत्तिः->conf_ctx[आवृत्तिः->modules[i]->सूचकांक] = rv;
        }
    }


    senv = environ;


    ngx_memzero(&conf, परिमाण(ngx_conf_t));
    /* STUB: init array ? */
    conf.तर्काः = ngx_array_create(pool, 10, परिमाण(ngx_str_t));
    यदि (conf.तर्काः == रिक्त) {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }

    conf.अस्थायी_pool = ngx_create_pool(NGX_आवृत्तिः_POOL_आकार, लघुगणकः);
    यदि (conf.अस्थायी_pool == रिक्त) {
        ngx_destroy_pool(pool);
        प्रत्यागम रिक्त;
    }


    conf.ctx = आवृत्तिः->conf_ctx;
    conf.आवृत्तिः = आवृत्तिः;
    conf.pool = pool;
    conf.लघुगणकः = लघुगणकः;
    conf.module_type = NGX_CORE_MODULE;
    conf.cmd_type = NGX_MAIN_CONF;

#यदि 0
    लघुगणकः->log_level = NGX_LOG_DEBUG_ALL;
#यदि_अन्तः

    यदि (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_आवृत्तिः_pools(&conf);
        प्रत्यागम रिक्त;
    }

    यदि (ngx_conf_parse(&conf, &आवृत्तिः->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_आवृत्तिः_pools(&conf);
        प्रत्यागम रिक्त;
    }

    यदि (ngx_test_config && !ngx_quiet_बहुलकम्) {
        ngx_log_stderr(0, "the configuration file %s syntax is ok",
                       आवृत्तिः->conf_file.दत्त);
    }

    प्रति (i = 0; आवृत्तिः->modules[i]; i++) {
        यदि (आवृत्तिः->modules[i]->type != NGX_CORE_MODULE) {
            अनुवर्त;
        }

        module = आवृत्तिः->modules[i]->ctx;

        यदि (module->init_conf) {
            यदि (module->init_conf(आवृत्तिः,
                                  आवृत्तिः->conf_ctx[आवृत्तिः->modules[i]->सूचकांक])
                == NGX_CONF_ERROR)
            {
                environ = senv;
                ngx_destroy_आवृत्तिः_pools(&conf);
                प्रत्यागम रिक्त;
            }
        }
    }

    यदि (ngx_process == NGX_PROCESS_SIGNALLER) {
        प्रत्यागम आवृत्तिः;
    }

    ccf = (ngx_core_conf_t *) ngx_get_conf(आवृत्तिः->conf_ctx, ngx_core_module);

    यदि (ngx_test_config) {

        यदि (ngx_create_pidfile(&ccf->pid, लघुगणकः) != NGX_OK) {
            गच्छ failed;
        }

    } अन्यथा यदि (!ngx_is_init_आवृत्तिः(old_आवृत्तिः)) {

        /*
         * we do not create the pid file in the first ngx_init_cycle() call
         * because we need to write the demonized process pid
         */

        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_आवृत्तिः->conf_ctx,
                                                   ngx_core_module);
        यदि (ccf->pid.len != old_ccf->pid.len
            || ngx_strcmp(ccf->pid.दत्त, old_ccf->pid.दत्त) != 0)
        {
            /* new pid file name */

            यदि (ngx_create_pidfile(&ccf->pid, लघुगणकः) != NGX_OK) {
                गच्छ failed;
            }

            ngx_delete_pidfile(old_आवृत्तिः);
        }
    }


    यदि (ngx_test_lockfile(आवृत्तिः->तालकम्_file.दत्त, लघुगणकः) != NGX_OK) {
        गच्छ failed;
    }


    यदि (ngx_create_paths(आवृत्तिः, ccf->user) != NGX_OK) {
        गच्छ failed;
    }


    यदि (ngx_log_open_default(आवृत्तिः) != NGX_OK) {
        गच्छ failed;
    }

    /* open the new files */

    part = &आवृत्तिः->open_files.part;
    file = part->elts;

    प्रति (i = 0; /* void */ ; i++) {

        यदि (i >= part->nelts) {
            यदि (part->अग्र == रिक्त) {
                भङ्ग;
            }
            part = part->अग्र;
            file = part->elts;
            i = 0;
        }

        यदि (file[i].name.len == 0) {
            अनुवर्त;
        }

        file[i].fd = ngx_open_file(file[i].name.दत्त,
                                   NGX_FILE_APPEND,
                                   NGX_FILE_CREATE_OR_OPEN,
                                   NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_CORE, लघुगणकः, 0,
                       "log: %p %d \"%s\"",
                       &file[i], file[i].fd, file[i].name.दत्त);

        यदि (file[i].fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, लघुगणकः, ngx_errno,
                          ngx_open_file_n " \"%s\" failed",
                          file[i].name.दत्त);
            गच्छ failed;
        }

#यदि !(NGX_WIN32)
        यदि (fcntl(file[i].fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, लघुगणकः, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.दत्त);
            गच्छ failed;
        }
#यदि_अन्तः
    }

    आवृत्तिः->लघुगणकः = &आवृत्तिः->new_log;
    pool->लघुगणकः = &आवृत्तिः->new_log;


    /* create shared memory */

    part = &आवृत्तिः->shared_स्मृतिः.part;
    shm_zone = part->elts;

    प्रति (i = 0; /* void */ ; i++) {

        यदि (i >= part->nelts) {
            यदि (part->अग्र == रिक्त) {
                भङ्ग;
            }
            part = part->अग्र;
            shm_zone = part->elts;
            i = 0;
        }

        यदि (shm_zone[i].shm.आकार == 0) {
            ngx_log_error(NGX_LOG_EMERG, लघुगणकः, 0,
                          "zero size shared memory zone \"%V\"",
                          &shm_zone[i].shm.name);
            गच्छ failed;
        }

        shm_zone[i].shm.लघुगणकः = आवृत्तिः->लघुगणकः;

        opart = &old_आवृत्तिः->shared_स्मृतिः.part;
        oshm_zone = opart->elts;

        दत्त = रिक्त;

        प्रति (n = 0; /* void */ ; n++) {

            यदि (n >= opart->nelts) {
                यदि (opart->अग्र == रिक्त) {
                    भङ्ग;
                }
                opart = opart->अग्र;
                oshm_zone = opart->elts;
                n = 0;
            }

            यदि (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                अनुवर्त;
            }

            यदि (ngx_strncmp(shm_zone[i].shm.name.दत्त,
                            oshm_zone[n].shm.name.दत्त,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                अनुवर्त;
            }

            यदि (shm_zone[i].चिह्नकम् == oshm_zone[n].चिह्नकम् && shm_zone[i].noreuse) {
                दत्त = oshm_zone[n].दत्त;
                भङ्ग;
            }

            यदि (shm_zone[i].चिह्नकम् == oshm_zone[n].चिह्नकम्
                && shm_zone[i].shm.आकार == oshm_zone[n].shm.आकार)
            {
                shm_zone[i].shm.addr = oshm_zone[n].shm.addr;
#यदि (NGX_WIN32)
                shm_zone[i].shm.handle = oshm_zone[n].shm.handle;
#यदि_अन्तः

                यदि (shm_zone[i].init(&shm_zone[i], oshm_zone[n].दत्त)
                    != NGX_OK)
                {
                    गच्छ failed;
                }

                गच्छ shm_zone_प्राप्तम्;
            }

            भङ्ग;
        }

        यदि (ngx_shm_alloc(&shm_zone[i].shm) != NGX_OK) {
            गच्छ failed;
        }

        यदि (ngx_init_zone_pool(आवृत्तिः, &shm_zone[i]) != NGX_OK) {
            गच्छ failed;
        }

        यदि (shm_zone[i].init(&shm_zone[i], दत्त) != NGX_OK) {
            गच्छ failed;
        }

    shm_zone_प्राप्तम्:

        अनुवर्त;
    }


    /* handle the listening sockets */

    यदि (old_आवृत्तिः->listening.nelts) {
        ls = old_आवृत्तिः->listening.elts;
        प्रति (i = 0; i < old_आवृत्तिः->listening.nelts; i++) {
            ls[i].remain = 0;
        }

        nls = आवृत्तिः->listening.elts;
        प्रति (n = 0; n < आवृत्तिः->listening.nelts; n++) {

            प्रति (i = 0; i < old_आवृत्तिः->listening.nelts; i++) {
                यदि (ls[i].ignore) {
                    अनुवर्त;
                }

                यदि (ls[i].remain) {
                    अनुवर्त;
                }

                यदि (ls[i].type != nls[n].type) {
                    अनुवर्त;
                }

                यदि (ngx_cmp_sockaddr(nls[n].sockaddr, nls[n].socklen,
                                     ls[i].sockaddr, ls[i].socklen, 1)
                    == NGX_OK)
                {
                    nls[n].fd = ls[i].fd;
                    nls[n].previous = &ls[i];

                    यदि (ls[i].protocol != nls[n].protocol) {
                        nls[n].change_protocol = 1;

                    } अन्यथा {
                        nls[n].inherited = ls[i].inherited;
                        ls[i].remain = 1;
                    }

                    यदि (ls[i].backlog != nls[n].backlog) {
                        nls[n].listen = 1;
                    }

#यदि (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)

                    /*
                     * FreeBSD, except the most recent versions,
                     * could not remove accept filter
                     */
                    nls[n].deferred_accept = ls[i].deferred_accept;

                    यदि (ls[i].accept_filter && nls[n].accept_filter) {
                        यदि (ngx_strcmp(ls[i].accept_filter,
                                       nls[n].accept_filter)
                            != 0)
                        {
                            nls[n].delete_deferred = 1;
                            nls[n].सङ्कलय_deferred = 1;
                        }

                    } अन्यथा यदि (ls[i].accept_filter) {
                        nls[n].delete_deferred = 1;

                    } अन्यथा यदि (nls[n].accept_filter) {
                        nls[n].सङ्कलय_deferred = 1;
                    }
#यदि_अन्तः

#यदि (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)

                    यदि (ls[i].deferred_accept && !nls[n].deferred_accept) {
                        nls[n].delete_deferred = 1;

                    } अन्यथा यदि (ls[i].deferred_accept != nls[n].deferred_accept)
                    {
                        nls[n].सङ्कलय_deferred = 1;
                    }
#यदि_अन्तः

#यदि (NGX_HAVE_REUSEPORT)
                    यदि (nls[n].reuseport && !ls[i].reuseport) {
                        nls[n].सङ्कलय_reuseport = 1;
                    }
#यदि_अन्तः

                    भङ्ग;
                }
            }

            यदि (nls[n].fd == (ngx_socket_t) -1) {
#यदि (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
                यदि (nls[n].accept_filter) {
                    nls[n].सङ्कलय_deferred = 1;
                }
#यदि_अन्तः
#यदि (NGX_HAVE_DEFERRED_ACCEPT && defined TCP_DEFER_ACCEPT)
                यदि (nls[n].deferred_accept) {
                    nls[n].सङ्कलय_deferred = 1;
                }
#यदि_अन्तः
            }
        }

    } अन्यथा {
#यदि (NGX_HAVE_DEFERRED_ACCEPT)
        ls = आवृत्तिः->listening.elts;
        प्रति (i = 0; i < आवृत्तिः->listening.nelts; i++) {
#यदि_परिभाषितम् SO_ACCEPTFILTER
            यदि (ls[i].accept_filter) {
                ls[i].सङ्कलय_deferred = 1;
            }
#यदि_अन्तः
#यदि_परिभाषितम् TCP_DEFER_ACCEPT
            यदि (ls[i].deferred_accept) {
                ls[i].सङ्कलय_deferred = 1;
            }
#यदि_अन्तः
        }
#यदि_अन्तः
    }

    यदि (ngx_open_listening_sockets(आवृत्तिः) != NGX_OK) {
        गच्छ failed;
    }

    यदि (!ngx_test_config) {
        ngx_configure_listening_sockets(आवृत्तिः);
    }


    /* commit the new cycle configuration */

    यदि (!ngx_use_stderr) {
        (शून्य) ngx_log_redirect_stderr(आवृत्तिः);
    }

    pool->लघुगणकः = आवृत्तिः->लघुगणकः;

    यदि (ngx_init_modules(आवृत्तिः) != NGX_OK) {
        /* fatal */
        निर्गच्छ(1);
    }


    /* close and delete stuff that lefts from an old cycle */

    /* free the unnecessary shared memory */

    opart = &old_आवृत्तिः->shared_स्मृतिः.part;
    oshm_zone = opart->elts;

    प्रति (i = 0; /* void */ ; i++) {

        यदि (i >= opart->nelts) {
            यदि (opart->अग्र == रिक्त) {
                गच्छ old_shm_zone_समाप्तम्;
            }
            opart = opart->अग्र;
            oshm_zone = opart->elts;
            i = 0;
        }

        part = &आवृत्तिः->shared_स्मृतिः.part;
        shm_zone = part->elts;

        प्रति (n = 0; /* void */ ; n++) {

            यदि (n >= part->nelts) {
                यदि (part->अग्र == रिक्त) {
                    भङ्ग;
                }
                part = part->अग्र;
                shm_zone = part->elts;
                n = 0;
            }

            यदि (oshm_zone[i].shm.name.len != shm_zone[n].shm.name.len) {
                अनुवर्त;
            }

            यदि (ngx_strncmp(oshm_zone[i].shm.name.दत्त,
                            shm_zone[n].shm.name.दत्त,
                            oshm_zone[i].shm.name.len)
                != 0)
            {
                अनुवर्त;
            }

            यदि (oshm_zone[i].चिह्नकम् == shm_zone[n].चिह्नकम्
                && oshm_zone[i].shm.आकार == shm_zone[n].shm.आकार
                && !oshm_zone[i].noreuse)
            {
                गच्छ जीवितम्_shm_zone;
            }

            भङ्ग;
        }

        ngx_shm_free(&oshm_zone[i].shm);

    जीवितम्_shm_zone:

        अनुवर्त;
    }

old_shm_zone_समाप्तम्:


    /* close the unnecessary listening sockets */

    ls = old_आवृत्तिः->listening.elts;
    प्रति (i = 0; i < old_आवृत्तिः->listening.nelts; i++) {

        यदि (ls[i].remain || ls[i].fd == (ngx_socket_t) -1) {
            अनुवर्त;
        }

        यदि (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, लघुगणकः, ngx_socket_errno,
                          ngx_close_socket_n " listening socket on %V failed",
                          &ls[i].addr_text);
        }

#यदि (NGX_HAVE_UNIX_DOMAIN)

        यदि (ls[i].sockaddr->sa_family == AF_UNIX) {
            u_char  *name;

            name = ls[i].addr_text.दत्त + परिमाण("unix:") - 1;

            ngx_log_error(NGX_LOG_WARN, आवृत्तिः->लघुगणकः, 0,
                          "deleting socket %s", name);

            यदि (ngx_delete_file(name) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_socket_errno,
                              ngx_delete_file_n " %s failed", name);
            }
        }

#यदि_अन्तः
    }


    /* close the unnecessary open files */

    part = &old_आवृत्तिः->open_files.part;
    file = part->elts;

    प्रति (i = 0; /* void */ ; i++) {

        यदि (i >= part->nelts) {
            यदि (part->अग्र == रिक्त) {
                भङ्ग;
            }
            part = part->अग्र;
            file = part->elts;
            i = 0;
        }

        यदि (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            अनुवर्त;
        }

        यदि (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, लघुगणकः, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.दत्त);
        }
    }

    ngx_destroy_pool(conf.अस्थायी_pool);

    यदि (ngx_process == NGX_PROCESS_MASTER || ngx_is_init_आवृत्तिः(old_आवृत्तिः)) {

        ngx_destroy_pool(old_आवृत्तिः->pool);
        आवृत्तिः->old_आवृत्तिः = रिक्त;

        प्रत्यागम आवृत्तिः;
    }


    यदि (ngx_अस्थायी_pool == रिक्त) {
        ngx_अस्थायी_pool = ngx_create_pool(128, आवृत्तिः->लघुगणकः);
        यदि (ngx_अस्थायी_pool == रिक्त) {
            ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, 0,
                          "could not create ngx_temp_pool");
            निर्गच्छ(1);
        }

        n = 10;

        यदि (ngx_array_init(&ngx_old_आवृत्तयः, ngx_अस्थायी_pool, n,
                           परिमाण(ngx_आवृत्तिः_t *))
            != NGX_OK)
        {
            निर्गच्छ(1);
        }

        ngx_memzero(ngx_old_आवृत्तयः.elts, n * परिमाण(ngx_आवृत्तिः_t *));

        ngx_cleaner_event.handler = ngx_clean_old_आवृत्तयः;
        ngx_cleaner_event.लघुगणकः = आवृत्तिः->लघुगणकः;
        ngx_cleaner_event.दत्त = &dumb;
        dumb.fd = (ngx_socket_t) -1;
    }

    ngx_अस्थायी_pool->लघुगणकः = आवृत्तिः->लघुगणकः;

    old = ngx_array_प्रक्षिप(&ngx_old_आवृत्तयः);
    यदि (old == रिक्त) {
        निर्गच्छ(1);
    }
    *old = old_आवृत्तिः;

    यदि (!ngx_cleaner_event.timer_समुच्चयः) {
        ngx_सङ्कलय_timer(&ngx_cleaner_event, 30000);
        ngx_cleaner_event.timer_समुच्चयः = 1;
    }

    प्रत्यागम आवृत्तिः;


failed:

    यदि (!ngx_is_init_आवृत्तिः(old_आवृत्तिः)) {
        old_ccf = (ngx_core_conf_t *) ngx_get_conf(old_आवृत्तिः->conf_ctx,
                                                   ngx_core_module);
        यदि (old_ccf->environment) {
            environ = old_ccf->environment;
        }
    }

    /* rollback the new cycle configuration */

    part = &आवृत्तिः->open_files.part;
    file = part->elts;

    प्रति (i = 0; /* void */ ; i++) {

        यदि (i >= part->nelts) {
            यदि (part->अग्र == रिक्त) {
                भङ्ग;
            }
            part = part->अग्र;
            file = part->elts;
            i = 0;
        }

        यदि (file[i].fd == NGX_INVALID_FILE || file[i].fd == ngx_stderr) {
            अनुवर्त;
        }

        यदि (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, लघुगणकः, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.दत्त);
        }
    }

    /* free the newly created shared memory */

    part = &आवृत्तिः->shared_स्मृतिः.part;
    shm_zone = part->elts;

    प्रति (i = 0; /* void */ ; i++) {

        यदि (i >= part->nelts) {
            यदि (part->अग्र == रिक्त) {
                भङ्ग;
            }
            part = part->अग्र;
            shm_zone = part->elts;
            i = 0;
        }

        यदि (shm_zone[i].shm.addr == रिक्त) {
            अनुवर्त;
        }

        opart = &old_आवृत्तिः->shared_स्मृतिः.part;
        oshm_zone = opart->elts;

        प्रति (n = 0; /* void */ ; n++) {

            यदि (n >= opart->nelts) {
                यदि (opart->अग्र == रिक्त) {
                    भङ्ग;
                }
                opart = opart->अग्र;
                oshm_zone = opart->elts;
                n = 0;
            }

            यदि (shm_zone[i].shm.name.len != oshm_zone[n].shm.name.len) {
                अनुवर्त;
            }

            यदि (ngx_strncmp(shm_zone[i].shm.name.दत्त,
                            oshm_zone[n].shm.name.दत्त,
                            shm_zone[i].shm.name.len)
                != 0)
            {
                अनुवर्त;
            }

            यदि (shm_zone[i].चिह्नकम् == oshm_zone[n].चिह्नकम्
                && shm_zone[i].shm.आकार == oshm_zone[n].shm.आकार
                && !shm_zone[i].noreuse)
            {
                गच्छ old_shm_zone_प्राप्तम्;
            }

            भङ्ग;
        }

        ngx_shm_free(&shm_zone[i].shm);

    old_shm_zone_प्राप्तम्:

        अनुवर्त;
    }

    यदि (ngx_test_config) {
        ngx_destroy_आवृत्तिः_pools(&conf);
        प्रत्यागम रिक्त;
    }

    ls = आवृत्तिः->listening.elts;
    प्रति (i = 0; i < आवृत्तिः->listening.nelts; i++) {
        यदि (ls[i].fd == (ngx_socket_t) -1 || !ls[i].उद्घाटय) {
            अनुवर्त;
        }

        यदि (ngx_close_socket(ls[i].fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, लघुगणकः, ngx_socket_errno,
                          ngx_close_socket_n " %V failed",
                          &ls[i].addr_text);
        }
    }

    ngx_destroy_आवृत्तिः_pools(&conf);

    प्रत्यागम रिक्त;
}


स्थिर शून्य
ngx_destroy_आवृत्तिः_pools(ngx_conf_t *conf)
{
    ngx_destroy_pool(conf->अस्थायी_pool);
    ngx_destroy_pool(conf->pool);
}


स्थिर ngx_int_t
ngx_init_zone_pool(ngx_आवृत्तिः_t *आवृत्तिः, ngx_shm_zone_t *zn)
{
    u_char           *file;
    ngx_slab_pool_t  *sp;

    sp = (ngx_slab_pool_t *) zn->shm.addr;

    यदि (zn->shm.exists) {

        यदि (sp == sp->addr) {
            प्रत्यागम NGX_OK;
        }

#यदि (NGX_WIN32)

        /* remap at the required address */

        यदि (ngx_shm_remap(&zn->shm, sp->addr) != NGX_OK) {
            प्रत्यागम NGX_ERROR;
        }

        sp = (ngx_slab_pool_t *) zn->shm.addr;

        यदि (sp == sp->addr) {
            प्रत्यागम NGX_OK;
        }

#यदि_अन्तः

        ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, 0,
                      "shared zone \"%V\" has no equal addresses: %p vs %p",
                      &zn->shm.name, sp->addr, sp);
        प्रत्यागम NGX_ERROR;
    }

    sp->अन्तः = zn->shm.addr + zn->shm.आकार;
    sp->min_सरणम् = 3;
    sp->addr = zn->shm.addr;

#यदि (NGX_HAVE_ATOMIC_OPS)

    file = रिक्त;

#अन्यथा

    file = ngx_pnalloc(आवृत्तिः->pool,
                       आवृत्तिः->तालकम्_file.len + zn->shm.name.len + 1);
    यदि (file == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    (शून्य) ngx_sprintf(file, "%V%V%Z", &आवृत्तिः->तालकम्_file, &zn->shm.name);

#यदि_अन्तः

    यदि (ngx_shmtx_create(&sp->mutex, &sp->तालकम्, file) != NGX_OK) {
        प्रत्यागम NGX_ERROR;
    }

    ngx_slab_init(sp);

    प्रत्यागम NGX_OK;
}


ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *लघुगणकः)
{
    आकार_t      len;
    ngx_int_t   rc;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    यदि (ngx_process > NGX_PROCESS_MASTER) {
        प्रत्यागम NGX_OK;
    }

    ngx_memzero(&file, परिमाण(ngx_file_t));

    file.name = *name;
    file.लघुगणकः = लघुगणकः;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.दत्त, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    यदि (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, लघुगणकः, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.दत्त);
        प्रत्यागम NGX_ERROR;
    }

    rc = NGX_OK;

    यदि (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        यदि (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            rc = NGX_ERROR;
        }
    }

    यदि (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.दत्त);
    }

    प्रत्यागम rc;
}


शून्य
ngx_delete_pidfile(ngx_आवृत्तिः_t *आवृत्तिः)
{
    u_char           *name;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(आवृत्तिः->conf_ctx, ngx_core_module);

    name = ngx_new_द्विमान ? ccf->oldpid.दत्त : ccf->pid.दत्त;

    यदि (ngx_delete_file(name) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, आवृत्तिः->लघुगणकः, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", name);
    }
}


ngx_int_t
ngx_signal_process(ngx_आवृत्तिः_t *आवृत्तिः, वर्ण *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, आवृत्तिः->लघुगणकः, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(आवृत्तिः->conf_ctx, ngx_core_module);

    ngx_memzero(&file, परिमाण(ngx_file_t));

    file.name = ccf->pid;
    file.लघुगणकः = आवृत्तिः->लघुगणकः;

    file.fd = ngx_open_file(file.name.दत्त, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    यदि (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, आवृत्तिः->लघुगणकः, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.दत्त);
        प्रत्यागम 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    यदि (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, आवृत्तिः->लघुगणकः, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.दत्त);
    }

    यदि (n == NGX_ERROR) {
        प्रत्यागम 1;
    }

    यावत् (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    pid = ngx_atoi(buf, ++n);

    यदि (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, आवृत्तिः->लघुगणकः, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.दत्त);
        प्रत्यागम 1;
    }

    प्रत्यागम ngx_os_signal_process(आवृत्तिः, sig, pid);

}


स्थिर ngx_int_t
ngx_test_lockfile(u_char *file, ngx_log_t *लघुगणकः)
{
#यदि !(NGX_HAVE_ATOMIC_OPS)
    ngx_fd_t  fd;

    fd = ngx_open_file(file, NGX_FILE_RDWR, NGX_FILE_CREATE_OR_OPEN,
                       NGX_FILE_DEFAULT_ACCESS);

    यदि (fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, लघुगणकः, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file);
        प्रत्यागम NGX_ERROR;
    }

    यदि (ngx_close_file(fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file);
    }

    यदि (ngx_delete_file(file) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, लघुगणकः, ngx_errno,
                      ngx_delete_file_n " \"%s\" failed", file);
    }

#यदि_अन्तः

    प्रत्यागम NGX_OK;
}


शून्य
ngx_reopen_files(ngx_आवृत्तिः_t *आवृत्तिः, ngx_uid_t user)
{
    ngx_fd_t          fd;
    ngx_uint_t        i;
    ngx_list_part_t  *part;
    ngx_open_file_t  *file;

    part = &आवृत्तिः->open_files.part;
    file = part->elts;

    प्रति (i = 0; /* void */ ; i++) {

        यदि (i >= part->nelts) {
            यदि (part->अग्र == रिक्त) {
                भङ्ग;
            }
            part = part->अग्र;
            file = part->elts;
            i = 0;
        }

        यदि (file[i].name.len == 0) {
            अनुवर्त;
        }

        यदि (file[i].flush) {
            file[i].flush(&file[i], आवृत्तिः->लघुगणकः);
        }

        fd = ngx_open_file(file[i].name.दत्त, NGX_FILE_APPEND,
                           NGX_FILE_CREATE_OR_OPEN, NGX_FILE_DEFAULT_ACCESS);

        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, आवृत्तिः->लघुगणकः, 0,
                       "reopen file \"%s\", old:%d new:%d",
                       file[i].name.दत्त, file[i].fd, fd);

        यदि (fd == NGX_INVALID_FILE) {
            ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                          ngx_open_file_n " \"%s\" failed", file[i].name.दत्त);
            अनुवर्त;
        }

#यदि !(NGX_WIN32)
        यदि (user != (ngx_uid_t) NGX_CONF_UNSET_UINT) {
            ngx_file_info_t  fi;

            यदि (ngx_file_info(file[i].name.दत्त, &fi) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                              ngx_file_info_n " \"%s\" failed",
                              file[i].name.दत्त);

                यदि (ngx_close_file(fd) == NGX_FILE_ERROR) {
                    ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                                  ngx_close_file_n " \"%s\" failed",
                                  file[i].name.दत्त);
                }

                अनुवर्त;
            }

            यदि (fi.st_uid != user) {
                यदि (chown((ध्रुव वर्ण *) file[i].name.दत्त, user, -1) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                                  "chown(\"%s\", %d) failed",
                                  file[i].name.दत्त, user);

                    यदि (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.दत्त);
                    }

                    अनुवर्त;
                }
            }

            यदि ((fi.st_बहुलकम् & (S_IRUSR|S_IWUSR)) != (S_IRUSR|S_IWUSR)) {

                fi.st_बहुलकम् |= (S_IRUSR|S_IWUSR);

                यदि (chmod((ध्रुव वर्ण *) file[i].name.दत्त, fi.st_बहुलकम्) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                                  "chmod() \"%s\" failed", file[i].name.दत्त);

                    यदि (ngx_close_file(fd) == NGX_FILE_ERROR) {
                        ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                                      ngx_close_file_n " \"%s\" failed",
                                      file[i].name.दत्त);
                    }

                    अनुवर्त;
                }
            }
        }

        यदि (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                          "fcntl(FD_CLOEXEC) \"%s\" failed",
                          file[i].name.दत्त);

            यदि (ngx_close_file(fd) == NGX_FILE_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                              ngx_close_file_n " \"%s\" failed",
                              file[i].name.दत्त);
            }

            अनुवर्त;
        }
#यदि_अन्तः

        यदि (ngx_close_file(file[i].fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, आवृत्तिः->लघुगणकः, ngx_errno,
                          ngx_close_file_n " \"%s\" failed",
                          file[i].name.दत्त);
        }

        file[i].fd = fd;
    }

    (शून्य) ngx_log_redirect_stderr(आवृत्तिः);
}


ngx_shm_zone_t *
ngx_shared_स्मृतिः_सङ्कलय(ngx_conf_t *cf, ngx_str_t *name, आकार_t आकार, शून्य *चिह्नकम्)
{
    ngx_uint_t        i;
    ngx_shm_zone_t   *shm_zone;
    ngx_list_part_t  *part;

    part = &cf->आवृत्तिः->shared_स्मृतिः.part;
    shm_zone = part->elts;

    प्रति (i = 0; /* void */ ; i++) {

        यदि (i >= part->nelts) {
            यदि (part->अग्र == रिक्त) {
                भङ्ग;
            }
            part = part->अग्र;
            shm_zone = part->elts;
            i = 0;
        }

        यदि (name->len != shm_zone[i].shm.name.len) {
            अनुवर्त;
        }

        यदि (ngx_strncmp(name->दत्त, shm_zone[i].shm.name.दत्त, name->len)
            != 0)
        {
            अनुवर्त;
        }

        यदि (चिह्नकम् != shm_zone[i].चिह्नकम्) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the shared memory zone \"%V\" is "
                            "already declared for a different use",
                            &shm_zone[i].shm.name);
            प्रत्यागम रिक्त;
        }

        यदि (shm_zone[i].shm.आकार == 0) {
            shm_zone[i].shm.आकार = आकार;
        }

        यदि (आकार && आकार != shm_zone[i].shm.आकार) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                            "the size %uz of shared memory zone \"%V\" "
                            "conflicts with already declared size %uz",
                            आकार, &shm_zone[i].shm.name, shm_zone[i].shm.आकार);
            प्रत्यागम रिक्त;
        }

        प्रत्यागम &shm_zone[i];
    }

    shm_zone = ngx_list_प्रक्षिप(&cf->आवृत्तिः->shared_स्मृतिः);

    यदि (shm_zone == रिक्त) {
        प्रत्यागम रिक्त;
    }

    shm_zone->दत्त = रिक्त;
    shm_zone->shm.लघुगणकः = cf->आवृत्तिः->लघुगणकः;
    shm_zone->shm.addr = रिक्त;
    shm_zone->shm.आकार = आकार;
    shm_zone->shm.name = *name;
    shm_zone->shm.exists = 0;
    shm_zone->init = रिक्त;
    shm_zone->चिह्नकम् = चिह्नकम्;
    shm_zone->noreuse = 0;

    प्रत्यागम shm_zone;
}


स्थिर शून्य
ngx_clean_old_आवृत्तयः(ngx_event_t *ev)
{
    ngx_uint_t     i, n, प्राप्तम्, जीवितम्;
    ngx_log_t     *लघुगणकः;
    ngx_आवृत्तिः_t  **आवृत्तिः;

    लघुगणकः = ngx_आवृत्तिः->लघुगणकः;
    ngx_अस्थायी_pool->लघुगणकः = लघुगणकः;

    ngx_log_debug0(NGX_LOG_DEBUG_CORE, लघुगणकः, 0, "clean old cycles");

    जीवितम् = 0;

    आवृत्तिः = ngx_old_आवृत्तयः.elts;
    प्रति (i = 0; i < ngx_old_आवृत्तयः.nelts; i++) {

        यदि (आवृत्तिः[i] == रिक्त) {
            अनुवर्त;
        }

        प्राप्तम् = 0;

        प्रति (n = 0; n < आवृत्तिः[i]->connection_n; n++) {
            यदि (आवृत्तिः[i]->connections[n].fd != (ngx_socket_t) -1) {
                प्राप्तम् = 1;

                ngx_log_debug1(NGX_LOG_DEBUG_CORE, लघुगणकः, 0, "live fd:%ui", n);

                भङ्ग;
            }
        }

        यदि (प्राप्तम्) {
            जीवितम् = 1;
            अनुवर्त;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, लघुगणकः, 0, "clean old cycle: %ui", i);

        ngx_destroy_pool(आवृत्तिः[i]->pool);
        आवृत्तिः[i] = रिक्त;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_CORE, लघुगणकः, 0, "old cycles status: %ui", जीवितम्);

    यदि (जीवितम्) {
        ngx_सङ्कलय_timer(ev, 30000);

    } अन्यथा {
        ngx_destroy_pool(ngx_अस्थायी_pool);
        ngx_अस्थायी_pool = रिक्त;
        ngx_old_आवृत्तयः.nelts = 0;
    }
}


शून्य
ngx_समुच्चयः_shutdown_timer(ngx_आवृत्तिः_t *आवृत्तिः)
{
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) ngx_get_conf(आवृत्तिः->conf_ctx, ngx_core_module);

    यदि (ccf->shutdown_timeout) {
        ngx_shutdown_event.handler = ngx_shutdown_timer_handler;
        ngx_shutdown_event.दत्त = आवृत्तिः;
        ngx_shutdown_event.लघुगणकः = आवृत्तिः->लघुगणकः;
        ngx_shutdown_event.cancelable = 1;

        ngx_सङ्कलय_timer(&ngx_shutdown_event, ccf->shutdown_timeout);
    }
}


स्थिर शून्य
ngx_shutdown_timer_handler(ngx_event_t *ev)
{
    ngx_uint_t         i;
    ngx_आवृत्तिः_t       *आवृत्तिः;
    ngx_connection_t  *c;

    आवृत्तिः = ev->दत्त;

    c = आवृत्तिः->connections;

    प्रति (i = 0; i < आवृत्तिः->connection_n; i++) {

        यदि (c[i].fd == (ngx_socket_t) -1
            || c[i].पठ == रिक्त
            || c[i].पठ->accept
            || c[i].पठ->channel
            || c[i].पठ->resolver)
        {
            अनुवर्त;
        }

        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->लघुगणकः, 0,
                       "*%uA shutdown timeout", c[i].सङ्ख्या);

        c[i].close = 1;
        c[i].error = 1;

        c[i].पठ->handler(c[i].पठ);
    }
}
