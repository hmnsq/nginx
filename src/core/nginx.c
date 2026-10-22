
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>
#अन्तर्भाव <nginx.h>


स्थिर शून्य ngx_show_version_info(शून्य);
स्थिर ngx_int_t ngx_add_inherited_sockets(ngx_cycle_t *cycle);
स्थिर शून्य ngx_cleanup_environment(शून्य *data);
स्थिर शून्य ngx_cleanup_environment_variable(शून्य *data);
स्थिर ngx_int_t ngx_get_options(पूर्णाङ्क argc, वर्ण *ध्रुव *argv);
स्थिर ngx_int_t ngx_process_options(ngx_cycle_t *cycle);
स्थिर ngx_int_t ngx_save_argv(ngx_cycle_t *cycle, पूर्णाङ्क argc, वर्ण *ध्रुव *argv);
स्थिर शून्य *ngx_core_module_create_conf(ngx_cycle_t *cycle);
स्थिर वर्ण *ngx_core_module_init_conf(ngx_cycle_t *cycle, शून्य *conf);
स्थिर वर्ण *ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
स्थिर वर्ण *ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
स्थिर वर्ण *ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
स्थिर वर्ण *ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    शून्य *conf);
स्थिर वर्ण *ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    शून्य *conf);
स्थिर वर्ण *ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf);
#यदि (NGX_HAVE_DLOPEN)
स्थिर शून्य ngx_unload_module(शून्य *data);
#endif


स्थिर ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


स्थिर ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      दुरत्व(ngx_core_conf_t, daemon),
      रिक्त },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      दुरत्व(ngx_core_conf_t, master),
      रिक्त },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      दुरत्व(ngx_core_conf_t, timer_resolution),
      रिक्त },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      दुरत्व(ngx_core_conf_t, pid),
      रिक्त },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      दुरत्व(ngx_core_conf_t, lock_file),
      रिक्त },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_worker_processes,
      0,
      0,
      रिक्त },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      0,
      दुरत्व(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE12,
      ngx_set_user,
      0,
      0,
      रिक्त },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_priority,
      0,
      0,
      रिक्त },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_1MORE,
      ngx_set_cpu_affinity,
      0,
      0,
      रिक्त },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_अंक_slot,
      0,
      दुरत्व(ngx_core_conf_t, rlimit_nofile),
      रिक्त },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_off_slot,
      0,
      दुरत्व(ngx_core_conf_t, rlimit_core),
      रिक्त },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      दुरत्व(ngx_core_conf_t, shutdown_timeout),
      रिक्त },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      दुरत्व(ngx_core_conf_t, working_directory),
      रिक्त },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_env,
      0,
      0,
      रिक्त },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_load_module,
      0,
      0,
      रिक्त },

      ngx_null_command
};


स्थिर ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    रिक्त,                                  /* init master */
    रिक्त,                                  /* init module */
    रिक्त,                                  /* init process */
    रिक्त,                                  /* init thread */
    रिक्त,                                  /* exit thread */
    रिक्त,                                  /* exit process */
    रिक्त,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


स्थिर ngx_uint_t   ngx_show_help;
स्थिर ngx_uint_t   ngx_show_version;
स्थिर ngx_uint_t   ngx_show_configure;
स्थिर u_char      *ngx_prefix;
स्थिर u_char      *ngx_error_log;
स्थिर u_char      *ngx_conf_file;
स्थिर u_char      *ngx_conf_params;
#यदि (NGX_CONTROL_API)
स्थिर u_char      *ngx_control_addr;
#endif
स्थिर वर्ण        *ngx_signal;


स्थिर वर्ण **ngx_os_environ;


पूर्णाङ्क ngx_cdecl
main(पूर्णाङ्क argc, वर्ण *ध्रुव *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *log;
    ngx_uint_t        i;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    यदि (ngx_strerror_init() != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (ngx_get_options(argc, argv) != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (ngx_show_version) {
        ngx_show_version_info();

        यदि (!ngx_test_config) {
            प्रत्यागम 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#यदि (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    यदि (log == रिक्त) {
        प्रत्यागम 1;
    }

    /* STUB */
#यदि (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_cycle, परिमाण(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    init_cycle.pool = ngx_create_pool(1024, log);
    यदि (init_cycle.pool == रिक्त) {
        प्रत्यागम 1;
    }

    यदि (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (ngx_process_options(&init_cycle) != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (ngx_os_init(log) != NGX_OK) {
        प्रत्यागम 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    यदि (ngx_crc32_table_init() != NGX_OK) {
        प्रत्यागम 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    यदि (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        प्रत्यागम 1;
    }

#यदि (NGX_CONTROL_API)
    ngx_control_preinit();
#endif

    यदि (ngx_preinit_modules() != NGX_OK) {
        प्रत्यागम 1;
    }

    cycle = ngx_init_cycle(&init_cycle);
    यदि (cycle == रिक्त) {
        यदि (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        प्रत्यागम 1;
    }

    यदि (ngx_test_config) {
        यदि (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.data);
        }

        यदि (ngx_dump_config) {
            cd = cycle->config_dump.elts;

            प्रति (i = 0; i < cycle->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (शून्य) ngx_write_fd(ngx_stdout, cd[i].name.data,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].buffer;

                (शून्य) ngx_write_fd(ngx_stdout, b->pos, b->last - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        प्रत्यागम 0;
    }

    यदि (ngx_signal) {
        प्रत्यागम ngx_signal_process(cycle, ngx_signal);
    }

    ngx_os_status(cycle->log);

    ngx_cycle = cycle;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    यदि (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#यदि !(NGX_WIN32)

    यदि (ngx_init_signals(cycle->log) != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (!ngx_inherited && ccf->daemon) {
        यदि (ngx_daemon(cycle->log) != NGX_OK) {
            प्रत्यागम 1;
        }

        ngx_daemonized = 1;
    }

    यदि (ngx_inherited) {
        ngx_daemonized = 1;
    }

#endif

#यदि (NGX_CONTROL_API)
    यदि (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "control API unavailable in single process mode");

    } अन्यथा {
        यदि (ngx_control_init(ngx_control_addr) != NGX_OK) {
            प्रत्यागम 1;
        }
    }
#endif

    यदि (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (ngx_log_redirect_stderr(cycle) != NGX_OK) {
        प्रत्यागम 1;
    }

    यदि (log->file->fd != ngx_stderr) {
        यदि (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    यदि (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } अन्यथा {
        ngx_master_process_cycle(cycle);
    }

    प्रत्यागम 0;
}


स्थिर शून्य
ngx_show_version_info(शून्य)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    यदि (ngx_show_help) {
        ngx_write_stderr(
            "Usage: nginx [-?hvVtTq] [-s signal] [-p prefix]" NGX_LINEFEED
            "             [-e filename] [-c filename] [-g directives]"
                          NGX_LINEFEED NGX_LINEFEED
            "Options:" NGX_LINEFEED
            "  -?,-h         : this help" NGX_LINEFEED
            "  -v            : show version and exit" NGX_LINEFEED
            "  -V            : show version and configure options then exit"
                               NGX_LINEFEED
            "  -t            : test configuration and exit" NGX_LINEFEED
            "  -T            : test configuration, dump it and exit"
                               NGX_LINEFEED
            "  -q            : suppress non-error messages "
                               "during configuration testing" NGX_LINEFEED
            "  -s signal     : send signal to a master process: "
                               "stop, quit, reopen, reload" NGX_LINEFEED
#यदि (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#endif
#ifdef NGX_PREFIX
            "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#अन्यथा
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
            "  -e filename   : set error log file (default: "
#ifdef NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#अन्यथा
                               NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#endif
            "  -c filename   : set configuration file (default: " NGX_CONF_PATH
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    यदि (ngx_show_configure) {

#ifdef NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#endif

#यदि (NGX_SSL)
        यदि (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } अन्यथा {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((वर्ण *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#अन्यथा
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#endif
#endif

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


स्थिर ngx_int_t
ngx_add_inherited_sockets(ngx_cycle_t *cycle)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) दो_पर्या(NGINX_VAR);

    यदि (inherited == रिक्त) {
        प्रत्यागम NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    यदि (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       परिमाण(ngx_listening_t))
        != NGX_OK)
    {
        प्रत्यागम NGX_ERROR;
    }

    प्रति (p = inherited, v = p; *p; p++) {
        यदि (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            यदि (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                भङ्ग;
            }

            v = p + 1;

            ls = ngx_array_धकेलो(&cycle->listening);
            यदि (ls == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            ngx_memzero(ls, परिमाण(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    यदि (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    प्रत्यागम ngx_set_inherited_sockets(cycle);
}


वर्ण **
ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last)
{
    वर्ण                **p, **env, *str;
    माप_प्रकार                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    यदि (last == रिक्त && ccf->environment) {
        प्रत्यागम ccf->environment;
    }

    var = ccf->env.elts;

    प्रति (i = 0; i < ccf->env.nelts; i++) {
        यदि (ngx_strcmp(var[i].data, "TZ") == 0
            || ngx_strncmp(var[i].data, "TZ=", 3) == 0)
        {
            गच्छ tz_found;
        }
    }

    var = ngx_array_धकेलो(&ccf->env);
    यदि (var == रिक्त) {
        प्रत्यागम रिक्त;
    }

    var->len = 2;
    var->data = (u_char *) "TZ";

    var = ccf->env.elts;

tz_found:

    n = 0;

    प्रति (i = 0; i < ccf->env.nelts; i++) {

        यदि (var[i].data[var[i].len] == '=') {
            n++;
            अनुवर्त;
        }

        प्रति (p = ngx_os_environ; *p; p++) {

            यदि (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                भङ्ग;
            }
        }
    }

    यदि (last) {
        env = ngx_alloc((*last + n + 1) * परिमाण(वर्ण *), cycle->log);
        यदि (env == रिक्त) {
            प्रत्यागम रिक्त;
        }

        *last = n;

    } अन्यथा {
        cln = ngx_pool_cleanup_add(cycle->pool, 0);
        यदि (cln == रिक्त) {
            प्रत्यागम रिक्त;
        }

        env = ngx_alloc((n + 1) * परिमाण(वर्ण *), cycle->log);
        यदि (env == रिक्त) {
            प्रत्यागम रिक्त;
        }

        cln->handler = ngx_cleanup_environment;
        cln->data = env;
    }

    n = 0;

    प्रति (i = 0; i < ccf->env.nelts; i++) {

        यदि (var[i].data[var[i].len] == '=') {

            यदि (last) {
                env[n++] = (वर्ण *) var[i].data;
                अनुवर्त;
            }

            cln = ngx_pool_cleanup_add(cycle->pool, 0);
            यदि (cln == रिक्त) {
                प्रत्यागम रिक्त;
            }

            len = ngx_strlen(var[i].data) + 1;

            str = ngx_alloc(len, cycle->log);
            यदि (str == रिक्त) {
                प्रत्यागम रिक्त;
            }

            ngx_memcpy(str, var[i].data, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->data = str;

            env[n++] = str;

            अनुवर्त;
        }

        प्रति (p = ngx_os_environ; *p; p++) {

            यदि (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                भङ्ग;
            }
        }
    }

    env[n] = रिक्त;

    यदि (last == रिक्त) {
        ccf->environment = env;
        environ = env;
    }

    प्रत्यागम env;
}


स्थिर शून्य
ngx_cleanup_environment(शून्य *data)
{
    वर्ण  **env = data;

    यदि (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        प्रत्यागम;
    }

    ngx_free(env);
}


स्थिर शून्य
ngx_cleanup_environment_variable(शून्य *data)
{
    वर्ण  *var = data;

    वर्ण  **p;

    प्रति (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        यदि (*p == var) {
            प्रत्यागम;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_द्विआधारी(ngx_cycle_t *cycle, वर्ण *ध्रुव *argv)
{
    वर्ण             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, परिमाण(ngx_exec_ctx_t));

    ctx.path = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_set_environment(cycle, &n);
    यदि (env == रिक्त) {
        प्रत्यागम NGX_INVALID_PID;
    }

    var = ngx_alloc(परिमाण(NGINX_VAR)
                    + cycle->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    cycle->log);
    यदि (var == रिक्त) {
        ngx_free(env);
        प्रत्यागम NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", परिमाण(NGINX_VAR));

    ls = cycle->listening.elts;
    प्रति (i = 0; i < cycle->listening.nelts; i++) {
        यदि (ls[i].ignore) {
            अनुवर्त;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#यदि (NGX_SETPROCTITLE_USES_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#endif

#यदि (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    यदि (env[n] != रिक्त) {
        n++;
    }
#endif

    env[n] = रिक्त;

#यदि (NGX_DEBUG)
    {
    वर्ण  **e;
    प्रति (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0, "env: %s", *e);
    }
    }
#endif

    ctx.envp = (वर्ण *ध्रुव *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    यदि (ngx_rename_file(ccf->pid.data, ccf->oldpid.data) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.data, ccf->oldpid.data, argv[0]);

        ngx_free(env);
        ngx_free(var);

        प्रत्यागम NGX_INVALID_PID;
    }

    pid = ngx_execute(cycle, &ctx);

    यदि (pid == NGX_INVALID_PID) {
        यदि (ngx_rename_file(ccf->oldpid.data, ccf->pid.data)
            == NGX_FILE_ERROR)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_rename_file_n " %s back to %s failed after "
                          "an attempt to execute new binary process \"%s\"",
                          ccf->oldpid.data, ccf->pid.data, argv[0]);
        }
    }

    ngx_free(env);
    ngx_free(var);

    प्रत्यागम pid;
}


स्थिर ngx_int_t
ngx_get_options(पूर्णाङ्क argc, वर्ण *ध्रुव *argv)
{
    u_char     *p;
    ngx_int_t   i;

    प्रति (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        यदि (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            प्रत्यागम NGX_ERROR;
        }

        यावत् (*p) {

            विकल्प (*p++) {

            पक्ष '?':
            पक्ष 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                भङ्ग;

            पक्ष 'v':
                ngx_show_version = 1;
                भङ्ग;

            पक्ष 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                भङ्ग;

            पक्ष 't':
                ngx_test_config = 1;
                भङ्ग;

            पक्ष 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                भङ्ग;

            पक्ष 'q':
                ngx_quiet_mode = 1;
                भङ्ग;

            पक्ष 'p':
                यदि (*p) {
                    ngx_prefix = p;
                    गच्छ next;
                }

                यदि (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    गच्छ next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                प्रत्यागम NGX_ERROR;

            पक्ष 'e':
                यदि (*p) {
                    ngx_error_log = p;

                } अन्यथा यदि (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } अन्यथा {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    प्रत्यागम NGX_ERROR;
                }

                यदि (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                गच्छ next;

            पक्ष 'c':
                यदि (*p) {
                    ngx_conf_file = p;
                    गच्छ next;
                }

                यदि (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    गच्छ next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                प्रत्यागम NGX_ERROR;

            पक्ष 'g':
                यदि (*p) {
                    ngx_conf_params = p;
                    गच्छ next;
                }

                यदि (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    गच्छ next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                प्रत्यागम NGX_ERROR;

            पक्ष 's':
                यदि (*p) {
                    ngx_signal = (वर्ण *) p;

                } अन्यथा यदि (argv[++i]) {
                    ngx_signal = argv[i];

                } अन्यथा {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    प्रत्यागम NGX_ERROR;
                }

                यदि (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    गच्छ next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                प्रत्यागम NGX_ERROR;

#यदि (NGX_CONTROL_API)
            पक्ष 'l':
                ngx_control_api_enabled = 1;

                यदि (*p) {
                    ngx_control_addr = p;
                    गच्छ next;
                }

                यदि (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    गच्छ next;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                प्रत्यागम NGX_ERROR;
#endif

            स्वभाव:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                प्रत्यागम NGX_ERROR;
            }
        }

    next:

        अनुवर्त;
    }

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, पूर्णाङ्क argc, वर्ण *ध्रुव *argv)
{
#यदि (NGX_FREEBSD)

    ngx_os_argv = (वर्ण **) argv;
    ngx_argc = argc;
    ngx_argv = (वर्ण **) argv;

#अन्यथा
    माप_प्रकार     len;
    ngx_int_t  i;

    ngx_os_argv = (वर्ण **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * परिमाण(वर्ण *), cycle->log);
    यदि (ngx_argv == रिक्त) {
        प्रत्यागम NGX_ERROR;
    }

    प्रति (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        यदि (ngx_argv[i] == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        (शून्य) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = रिक्त;

#endif

    ngx_os_environ = environ;

    प्रत्यागम NGX_OK;
}


स्थिर ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    माप_प्रकार   len;

    यदि (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        यदि (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            यदि (p == रिक्त) {
                प्रत्यागम NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

    } अन्यथा {

#ifndef NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        यदि (p == रिक्त) {
            प्रत्यागम NGX_ERROR;
        }

        यदि (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            प्रत्यागम NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#अन्यथा

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#अन्यथा
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#endif
    }

    यदि (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } अन्यथा {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    यदि (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        प्रत्यागम NGX_ERROR;
    }

    प्रति (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        यदि (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.data + 1;
            cycle->conf_prefix.data = cycle->conf_file.data;
            भङ्ग;
        }
    }

    यदि (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.data = ngx_error_log;

    } अन्यथा {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    यदि (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    यदि (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    प्रत्यागम NGX_OK;
}


स्थिर शून्य *
ngx_core_module_create_conf(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(cycle->pool, परिमाण(ngx_core_conf_t));
    यदि (ccf == रिक्त) {
        प्रत्यागम रिक्त;
    }

    /*
     * set by ngx_pcalloc()
     *
     *     ccf->pid = NULL;
     *     ccf->oldpid = NULL;
     *     ccf->priority = 0;
     *     ccf->cpu_affinity_auto = 0;
     *     ccf->cpu_affinity_n = 0;
     *     ccf->cpu_affinity = NULL;
     */

    ccf->daemon = NGX_CONF_UNSET;
    ccf->master = NGX_CONF_UNSET;
    ccf->timer_resolution = NGX_CONF_UNSET_MSEC;
    ccf->shutdown_timeout = NGX_CONF_UNSET_MSEC;

    ccf->worker_processes = NGX_CONF_UNSET;
    ccf->debug_points = NGX_CONF_UNSET;

    ccf->rlimit_nofile = NGX_CONF_UNSET;
    ccf->rlimit_core = NGX_CONF_UNSET;

    ccf->user = (ngx_uid_t) NGX_CONF_UNSET_UINT;
    ccf->group = (ngx_gid_t) NGX_CONF_UNSET_UINT;

    यदि (ngx_array_init(&ccf->env, cycle->pool, 1, परिमाण(ngx_str_t))
        != NGX_OK)
    {
        प्रत्यागम रिक्त;
    }

    प्रत्यागम ccf;
}


स्थिर वर्ण *
ngx_core_module_init_conf(ngx_cycle_t *cycle, शून्य *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_मूल्य(ccf->daemon, 1);
    ngx_conf_init_मूल्य(ccf->master, 1);
    ngx_conf_init_msec_मूल्य(ccf->timer_resolution, 0);
    ngx_conf_init_msec_मूल्य(ccf->shutdown_timeout, 0);

    ngx_conf_init_मूल्य(ccf->worker_processes, 1);
    ngx_conf_init_मूल्य(ccf->debug_points, 0);

#यदि (NGX_HAVE_CPU_AFFINITY)

    यदि (!ccf->cpu_affinity_auto
        && ccf->cpu_affinity_n
        && ccf->cpu_affinity_n != 1
        && ccf->cpu_affinity_n != (ngx_uint_t) ccf->worker_processes)
    {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "the number of \"worker_processes\" is not equal to "
                      "the number of \"worker_cpu_affinity\" masks, "
                      "using last mask for remaining worker processes");
    }

#endif


    यदि (ccf->pid.len == 0) {
        ngx_str_set(&ccf->pid, NGX_PID_PATH);
    }

    यदि (ngx_conf_full_name(cycle, &ccf->pid, 0) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + परिमाण(NGX_OLDPID_EXT);

    ccf->oldpid.data = ngx_pnalloc(cycle->pool, ccf->oldpid.len);
    यदि (ccf->oldpid.data == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.data, ccf->pid.data, ccf->pid.len),
               NGX_OLDPID_EXT, परिमाण(NGX_OLDPID_EXT));


    यदि (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        यदि (ngx_conf_full_name(cycle, &ccf->working_directory, 0) != NGX_OK) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        यदि (ngx_file_info(ccf->working_directory.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.data);
            प्रत्यागम NGX_CONF_ERROR;
        }

        यदि (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.data);
            प्रत्यागम NGX_CONF_ERROR;
        }
    }


#यदि !(NGX_WIN32)

    यदि (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        संरचना group   *grp;
        संरचना passwd  *pwd;

        ngx_set_errno(0);
        pwd = getpwnam(NGX_USER);
        यदि (pwd == रिक्त) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            प्रत्यागम NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_set_errno(0);
        grp = getgrnam(NGX_GROUP);
        यदि (grp == रिक्त) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getgrnam(\"" NGX_GROUP "\") failed");
            प्रत्यागम NGX_CONF_ERROR;
        }

        ccf->group = grp->gr_gid;
    }


    यदि (ccf->lock_file.len == 0) {
        ngx_str_set(&ccf->lock_file, NGX_LOCK_PATH);
    }

    यदि (ngx_conf_full_name(cycle, &ccf->lock_file, 0) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    {
    ngx_str_t  lock_file;

    lock_file = cycle->old_cycle->lock_file;

    यदि (lock_file.len) {
        lock_file.len--;

        यदि (ccf->lock_file.len != lock_file.len
            || ngx_strncmp(ccf->lock_file.data, lock_file.data, lock_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        cycle->lock_file.len = lock_file.len + 1;
        lock_file.len += परिमाण(".accept");

        cycle->lock_file.data = ngx_pstrdup(cycle->pool, &lock_file);
        यदि (cycle->lock_file.data == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

    } अन्यथा {
        cycle->lock_file.len = ccf->lock_file.len + 1;
        cycle->lock_file.data = ngx_pnalloc(cycle->pool,
                                      ccf->lock_file.len + परिमाण(".accept"));
        यदि (cycle->lock_file.data == रिक्त) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(cycle->lock_file.data, ccf->lock_file.data,
                              ccf->lock_file.len),
                   ".accept", परिमाण(".accept"));
    }
    }

#endif

    प्रत्यागम NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
#यदि (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    प्रत्यागम NGX_CONF_OK;

#अन्यथा

    ngx_core_conf_t  *ccf = conf;

    वर्ण             *group;
    संरचना passwd    *pwd;
    संरचना group     *grp;
    ngx_str_t        *मूल्य;

    यदि (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        प्रत्यागम "is duplicate";
    }

    यदि (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        प्रत्यागम NGX_CONF_OK;
    }

    मूल्य = cf->args->elts;

    ccf->username = (वर्ण *) मूल्य[1].data;

    ngx_set_errno(0);
    pwd = getpwnam((ध्रुव वर्ण *) मूल्य[1].data);
    यदि (pwd == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", मूल्य[1].data);
        प्रत्यागम NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    group = (वर्ण *) ((cf->args->nelts == 2) ? मूल्य[1].data : मूल्य[2].data);

    ngx_set_errno(0);
    grp = getgrnam(group);
    यदि (grp == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", group);
        प्रत्यागम NGX_CONF_ERROR;
    }

    ccf->group = grp->gr_gid;

    प्रत्यागम NGX_CONF_OK;

#endif
}


स्थिर वर्ण *
ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *मूल्य, *var;
    ngx_uint_t   i;

    var = ngx_array_धकेलो(&ccf->env);
    यदि (var == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    मूल्य = cf->args->elts;
    *var = मूल्य[1];

    प्रति (i = 0; i < मूल्य[1].len; i++) {

        यदि (मूल्य[1].data[i] == '=') {

            var->len = i;

            प्रत्यागम NGX_CONF_OK;
        }
    }

    प्रत्यागम NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *मूल्य;
    ngx_uint_t        n, minus;

    यदि (ccf->priority != 0) {
        प्रत्यागम "is duplicate";
    }

    मूल्य = cf->args->elts;

    यदि (मूल्य[1].data[0] == '-') {
        n = 1;
        minus = 1;

    } अन्यथा यदि (मूल्य[1].data[0] == '+') {
        n = 1;
        minus = 0;

    } अन्यथा {
        n = 0;
        minus = 0;
    }

    ccf->priority = ngx_atoi(&मूल्य[1].data[n], मूल्य[1].len - n);
    यदि (ccf->priority == NGX_ERROR) {
        प्रत्यागम "invalid number";
    }

    यदि (minus) {
        ccf->priority = -ccf->priority;
    }

    प्रत्यागम NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
#यदि (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *मूल्य;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *mask;

    यदि (ccf->cpu_affinity) {
        प्रत्यागम "is duplicate";
    }

    mask = ngx_palloc(cf->pool, (cf->args->nelts - 1) * परिमाण(ngx_cpuset_t));
    यदि (mask == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->args->nelts - 1;
    ccf->cpu_affinity = mask;

    मूल्य = cf->args->elts;

    यदि (ngx_strcmp(मूल्य[1].data, "auto") == 0) {

        यदि (cf->args->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            प्रत्यागम NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&mask[0]);
        प्रति (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_SET(i, &mask[0]);
        }

        n = 2;

    } अन्यथा {
        n = 1;
    }

    प्रति ( /* void */ ; n < cf->args->nelts; n++) {

        यदि (मूल्य[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            प्रत्यागम NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&mask[n - 1]);

        प्रति (p = मूल्य[n].data + मूल्य[n].len - 1;
             p >= मूल्य[n].data;
             p--)
        {
            ch = *p;

            यदि (ch == ' ') {
                अनुवर्त;
            }

            i++;

            यदि (ch == '0') {
                अनुवर्त;
            }

            यदि (ch == '1') {
                CPU_SET(i - 1, &mask[n - 1]);
                अनुवर्त;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            प्रत्यागम NGX_CONF_ERROR;
        }
    }

#अन्यथा

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#endif

    प्रत्यागम NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#यदि (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *mask;
    ngx_core_conf_t  *ccf;

    स्थिर ngx_cpuset_t  result;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_cycle->conf_ctx,
                                           ngx_core_module);

    यदि (ccf->cpu_affinity == रिक्त) {
        प्रत्यागम रिक्त;
    }

    यदि (ccf->cpu_affinity_auto) {
        mask = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        प्रति (i = 0, j = n; /* void */ ; i++) {

            यदि (CPU_ISSET(i % CPU_SETSIZE, mask) && j-- == 0) {
                भङ्ग;
            }

            यदि (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                प्रत्यागम रिक्त;
            }

            /* void */
        }

        CPU_ZERO(&result);
        CPU_SET(i % CPU_SETSIZE, &result);

        प्रत्यागम &result;
    }

    यदि (ccf->cpu_affinity_n > n) {
        प्रत्यागम &ccf->cpu_affinity[n];
    }

    प्रत्यागम &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#अन्यथा

    प्रत्यागम रिक्त;

#endif
}


स्थिर वर्ण *
ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
    ngx_str_t        *मूल्य;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    यदि (ccf->worker_processes != NGX_CONF_UNSET) {
        प्रत्यागम "is duplicate";
    }

    मूल्य = cf->args->elts;

    यदि (ngx_strcmp(मूल्य[1].data, "auto") == 0) {
        ccf->worker_processes = ngx_ncpu;
        प्रत्यागम NGX_CONF_OK;
    }

    ccf->worker_processes = ngx_atoi(मूल्य[1].data, मूल्य[1].len);

    यदि (ccf->worker_processes == NGX_ERROR) {
        प्रत्यागम "invalid value";
    }

    प्रत्यागम NGX_CONF_OK;
}


स्थिर वर्ण *
ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, शून्य *conf)
{
#यदि (NGX_HAVE_DLOPEN)
    शून्य                *handle;
    वर्ण               **names, **order;
    ngx_str_t           *मूल्य, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    यदि (cf->cycle->modules_used) {
        प्रत्यागम "is specified too late";
    }

    मूल्य = cf->args->elts;

    file = मूल्य[1];

    यदि (ngx_conf_full_name(cf->cycle, &file, 0) != NGX_OK) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_add(cf->cycle->pool, 0);
    यदि (cln == रिक्त) {
        प्रत्यागम NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.data);
    यदि (handle == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.data, ngx_dlerror());
        प्रत्यागम NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->data = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    यदि (modules == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &मूल्य[1], "ngx_modules", ngx_dlerror());
        प्रत्यागम NGX_CONF_ERROR;
    }

    names = ngx_dlsym(handle, "ngx_module_names");
    यदि (names == रिक्त) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &मूल्य[1], "ngx_module_names", ngx_dlerror());
        प्रत्यागम NGX_CONF_ERROR;
    }

    order = ngx_dlsym(handle, "ngx_module_order");

    प्रति (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = names[i];

        यदि (ngx_add_module(cf, &file, module, order) != NGX_OK) {
            प्रत्यागम NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0, "module: %s i:%ui",
                       module->name, module->index);
    }

    प्रत्यागम NGX_CONF_OK;

#अन्यथा

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    प्रत्यागम NGX_CONF_ERROR;

#endif
}


#यदि (NGX_HAVE_DLOPEN)

स्थिर शून्य
ngx_unload_module(शून्य *data)
{
    शून्य  *handle = data;

    यदि (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#endif
