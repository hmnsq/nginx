
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>
#അന്തര്ഭാവ <nginx.h>


സ്ഥിതം ശൂന്യം ngx_show_version_info(ശൂന്യം);
സ്ഥിതം ngx_int_t ngx_add_inherited_sockets(ngx_cycle_t *cycle);
സ്ഥിതം ശൂന്യം ngx_cleanup_environment(ശൂന്യം *data);
സ്ഥിതം ശൂന്യം ngx_cleanup_environment_variable(ശൂന്യം *data);
സ്ഥിതം ngx_int_t ngx_get_options(പൂർണ്ണസംഖ്യ argc, അക്ഷരം *സ്ഥിരം *argv);
സ്ഥിതം ngx_int_t ngx_process_options(ngx_cycle_t *cycle);
സ്ഥിതം ngx_int_t ngx_save_argv(ngx_cycle_t *cycle, പൂർണ്ണസംഖ്യ argc, അക്ഷരം *സ്ഥിരം *argv);
സ്ഥിതം ശൂന്യം *ngx_core_module_create_conf(ngx_cycle_t *cycle);
സ്ഥിതം അക്ഷരം *ngx_core_module_init_conf(ngx_cycle_t *cycle, ശൂന്യം *conf);
സ്ഥിതം അക്ഷരം *ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
സ്ഥിതം അക്ഷരം *ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
സ്ഥിതം അക്ഷരം *ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
സ്ഥിതം അക്ഷരം *ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    ശൂന്യം *conf);
സ്ഥിതം അക്ഷരം *ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    ശൂന്യം *conf);
സ്ഥിതം അക്ഷരം *ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf);
#എങ്കിൽ (NGX_HAVE_DLOPEN)
സ്ഥിതം ശൂന്യം ngx_unload_module(ശൂന്യം *data);
#endif


സ്ഥിതം ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


സ്ഥിതം ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, daemon),
      ശൂന്യ },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, master),
      ശൂന്യ },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      ശൂന്യ },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, pid),
      ശൂന്യ },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, lock_file),
      ശൂന്യ },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_worker_processes,
      0,
      0,
      ശൂന്യ },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      0,
      offsetof(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE12,
      ngx_set_user,
      0,
      0,
      ശൂന്യ },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_priority,
      0,
      0,
      ശൂന്യ },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_1MORE,
      ngx_set_cpu_affinity,
      0,
      0,
      ശൂന്യ },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_അക്കം_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      ശൂന്യ },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_off_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      ശൂന്യ },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      ശൂന്യ },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      ശൂന്യ },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_env,
      0,
      0,
      ശൂന്യ },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_load_module,
      0,
      0,
      ശൂന്യ },

      ngx_null_command
};


സ്ഥിതം ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    ശൂന്യ,                                  /* init master */
    ശൂന്യ,                                  /* init module */
    ശൂന്യ,                                  /* init process */
    ശൂന്യ,                                  /* init thread */
    ശൂന്യ,                                  /* exit thread */
    ശൂന്യ,                                  /* exit process */
    ശൂന്യ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


സ്ഥിതം ngx_uint_t   ngx_show_help;
സ്ഥിതം ngx_uint_t   ngx_show_version;
സ്ഥിതം ngx_uint_t   ngx_show_configure;
സ്ഥിതം u_char      *ngx_prefix;
സ്ഥിതം u_char      *ngx_error_log;
സ്ഥിതം u_char      *ngx_conf_file;
സ്ഥിതം u_char      *ngx_conf_params;
#എങ്കിൽ (NGX_CONTROL_API)
സ്ഥിതം u_char      *ngx_control_addr;
#endif
സ്ഥിതം അക്ഷരം        *ngx_signal;


സ്ഥിതം അക്ഷരം **ngx_os_environ;


പൂർണ്ണസംഖ്യ ngx_cdecl
പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം *സ്ഥിരം *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *log;
    ngx_uint_t        i;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    എങ്കിൽ (ngx_strerror_init() != NGX_OK) {
        തിരികെ 1;
    }

    എങ്കിൽ (ngx_get_options(argc, argv) != NGX_OK) {
        തിരികെ 1;
    }

    എങ്കിൽ (ngx_show_version) {
        ngx_show_version_info();

        എങ്കിൽ (!ngx_test_config) {
            തിരികെ 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#എങ്കിൽ (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    എങ്കിൽ (log == ശൂന്യ) {
        തിരികെ 1;
    }

    /* STUB */
#എങ്കിൽ (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_cycle, വലുപ്പം(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    init_cycle.pool = ngx_create_pool(1024, log);
    എങ്കിൽ (init_cycle.pool == ശൂന്യ) {
        തിരികെ 1;
    }

    എങ്കിൽ (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        തിരികെ 1;
    }

    എങ്കിൽ (ngx_process_options(&init_cycle) != NGX_OK) {
        തിരികെ 1;
    }

    എങ്കിൽ (ngx_os_init(log) != NGX_OK) {
        തിരികെ 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    എങ്കിൽ (ngx_crc32_table_init() != NGX_OK) {
        തിരികെ 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    എങ്കിൽ (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        തിരികെ 1;
    }

#എങ്കിൽ (NGX_CONTROL_API)
    ngx_control_preinit();
#endif

    എങ്കിൽ (ngx_preinit_modules() != NGX_OK) {
        തിരികെ 1;
    }

    cycle = ngx_init_cycle(&init_cycle);
    എങ്കിൽ (cycle == ശൂന്യ) {
        എങ്കിൽ (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        തിരികെ 1;
    }

    എങ്കിൽ (ngx_test_config) {
        എങ്കിൽ (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.data);
        }

        എങ്കിൽ (ngx_dump_config) {
            cd = cycle->config_dump.elts;

            വേണ്ടി (i = 0; i < cycle->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (ശൂന്യം) ngx_write_fd(ngx_stdout, cd[i].name.data,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].buffer;

                (ശൂന്യം) ngx_write_fd(ngx_stdout, b->pos, b->last - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        തിരികെ 0;
    }

    എങ്കിൽ (ngx_signal) {
        തിരികെ ngx_signal_process(cycle, ngx_signal);
    }

    ngx_os_status(cycle->log);

    ngx_cycle = cycle;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    എങ്കിൽ (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#എങ്കിൽ !(NGX_WIN32)

    എങ്കിൽ (ngx_init_signals(cycle->log) != NGX_OK) {
        തിരികെ 1;
    }

    എങ്കിൽ (!ngx_inherited && ccf->daemon) {
        എങ്കിൽ (ngx_daemon(cycle->log) != NGX_OK) {
            തിരികെ 1;
        }

        ngx_daemonized = 1;
    }

    എങ്കിൽ (ngx_inherited) {
        ngx_daemonized = 1;
    }

#endif

#എങ്കിൽ (NGX_CONTROL_API)
    എങ്കിൽ (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "control API unavailable in single process mode");

    } അല്ലെങ്കിൽ {
        എങ്കിൽ (ngx_control_init(ngx_control_addr) != NGX_OK) {
            തിരികെ 1;
        }
    }
#endif

    എങ്കിൽ (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        തിരികെ 1;
    }

    എങ്കിൽ (ngx_log_redirect_stderr(cycle) != NGX_OK) {
        തിരികെ 1;
    }

    എങ്കിൽ (log->file->fd != ngx_stderr) {
        എങ്കിൽ (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    എങ്കിൽ (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } അല്ലെങ്കിൽ {
        ngx_master_process_cycle(cycle);
    }

    തിരികെ 0;
}


സ്ഥിതം ശൂന്യം
ngx_show_version_info(ശൂന്യം)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    എങ്കിൽ (ngx_show_help) {
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
#എങ്കിൽ (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#endif
#ifdef NGX_PREFIX
            "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#അല്ലെങ്കിൽ
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
            "  -e filename   : set error log file (default: "
#ifdef NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#അല്ലെങ്കിൽ
                               NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#endif
            "  -c filename   : set configuration file (default: " NGX_CONF_PATH
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    എങ്കിൽ (ngx_show_configure) {

#ifdef NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#endif

#എങ്കിൽ (NGX_SSL)
        എങ്കിൽ (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } അല്ലെങ്കിൽ {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((അക്ഷരം *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#അല്ലെങ്കിൽ
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#endif
#endif

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


സ്ഥിതം ngx_int_t
ngx_add_inherited_sockets(ngx_cycle_t *cycle)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    എങ്കിൽ (inherited == ശൂന്യ) {
        തിരികെ NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    എങ്കിൽ (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       വലുപ്പം(ngx_listening_t))
        != NGX_OK)
    {
        തിരികെ NGX_ERROR;
    }

    വേണ്ടി (p = inherited, v = p; *p; p++) {
        എങ്കിൽ (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            എങ്കിൽ (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                നിർത്തുക;
            }

            v = p + 1;

            ls = ngx_array_തള്ളുക(&cycle->listening);
            എങ്കിൽ (ls == ശൂന്യ) {
                തിരികെ NGX_ERROR;
            }

            ngx_memzero(ls, വലുപ്പം(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    എങ്കിൽ (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    തിരികെ ngx_set_inherited_sockets(cycle);
}


അക്ഷരം **
ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last)
{
    അക്ഷരം                **p, **env, *str;
    size_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    എങ്കിൽ (last == ശൂന്യ && ccf->environment) {
        തിരികെ ccf->environment;
    }

    var = ccf->env.elts;

    വേണ്ടി (i = 0; i < ccf->env.nelts; i++) {
        എങ്കിൽ (ngx_strcmp(var[i].data, "TZ") == 0
            || ngx_strncmp(var[i].data, "TZ=", 3) == 0)
        {
            പോകുക tz_found;
        }
    }

    var = ngx_array_തള്ളുക(&ccf->env);
    എങ്കിൽ (var == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    var->len = 2;
    var->data = (u_char *) "TZ";

    var = ccf->env.elts;

tz_found:

    n = 0;

    വേണ്ടി (i = 0; i < ccf->env.nelts; i++) {

        എങ്കിൽ (var[i].data[var[i].len] == '=') {
            n++;
            തുടരുക;
        }

        വേണ്ടി (p = ngx_os_environ; *p; p++) {

            എങ്കിൽ (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                നിർത്തുക;
            }
        }
    }

    എങ്കിൽ (last) {
        env = ngx_alloc((*last + n + 1) * വലുപ്പം(അക്ഷരം *), cycle->log);
        എങ്കിൽ (env == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }

        *last = n;

    } അല്ലെങ്കിൽ {
        cln = ngx_pool_cleanup_add(cycle->pool, 0);
        എങ്കിൽ (cln == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }

        env = ngx_alloc((n + 1) * വലുപ്പം(അക്ഷരം *), cycle->log);
        എങ്കിൽ (env == ശൂന്യ) {
            തിരികെ ശൂന്യ;
        }

        cln->handler = ngx_cleanup_environment;
        cln->data = env;
    }

    n = 0;

    വേണ്ടി (i = 0; i < ccf->env.nelts; i++) {

        എങ്കിൽ (var[i].data[var[i].len] == '=') {

            എങ്കിൽ (last) {
                env[n++] = (അക്ഷരം *) var[i].data;
                തുടരുക;
            }

            cln = ngx_pool_cleanup_add(cycle->pool, 0);
            എങ്കിൽ (cln == ശൂന്യ) {
                തിരികെ ശൂന്യ;
            }

            len = ngx_strlen(var[i].data) + 1;

            str = ngx_alloc(len, cycle->log);
            എങ്കിൽ (str == ശൂന്യ) {
                തിരികെ ശൂന്യ;
            }

            ngx_memcpy(str, var[i].data, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->data = str;

            env[n++] = str;

            തുടരുക;
        }

        വേണ്ടി (p = ngx_os_environ; *p; p++) {

            എങ്കിൽ (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                നിർത്തുക;
            }
        }
    }

    env[n] = ശൂന്യ;

    എങ്കിൽ (last == ശൂന്യ) {
        ccf->environment = env;
        environ = env;
    }

    തിരികെ env;
}


സ്ഥിതം ശൂന്യം
ngx_cleanup_environment(ശൂന്യം *data)
{
    അക്ഷരം  **env = data;

    എങ്കിൽ (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        തിരികെ;
    }

    ngx_free(env);
}


സ്ഥിതം ശൂന്യം
ngx_cleanup_environment_variable(ശൂന്യം *data)
{
    അക്ഷരം  *var = data;

    അക്ഷരം  **p;

    വേണ്ടി (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        എങ്കിൽ (*p == var) {
            തിരികെ;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_ദ്വിമാന(ngx_cycle_t *cycle, അക്ഷരം *സ്ഥിരം *argv)
{
    അക്ഷരം             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, വലുപ്പം(ngx_exec_ctx_t));

    ctx.path = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_set_environment(cycle, &n);
    എങ്കിൽ (env == ശൂന്യ) {
        തിരികെ NGX_INVALID_PID;
    }

    var = ngx_alloc(വലുപ്പം(NGINX_VAR)
                    + cycle->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    cycle->log);
    എങ്കിൽ (var == ശൂന്യ) {
        ngx_free(env);
        തിരികെ NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", വലുപ്പം(NGINX_VAR));

    ls = cycle->listening.elts;
    വേണ്ടി (i = 0; i < cycle->listening.nelts; i++) {
        എങ്കിൽ (ls[i].ignore) {
            തുടരുക;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#എങ്കിൽ (NGX_SETPROCTITLE_USES_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#endif

#എങ്കിൽ (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    എങ്കിൽ (env[n] != ശൂന്യ) {
        n++;
    }
#endif

    env[n] = ശൂന്യ;

#എങ്കിൽ (NGX_DEBUG)
    {
    അക്ഷരം  **e;
    വേണ്ടി (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0, "env: %s", *e);
    }
    }
#endif

    ctx.envp = (അക്ഷരം *സ്ഥിരം *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    എങ്കിൽ (ngx_rename_file(ccf->pid.data, ccf->oldpid.data) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.data, ccf->oldpid.data, argv[0]);

        ngx_free(env);
        ngx_free(var);

        തിരികെ NGX_INVALID_PID;
    }

    pid = ngx_execute(cycle, &ctx);

    എങ്കിൽ (pid == NGX_INVALID_PID) {
        എങ്കിൽ (ngx_rename_file(ccf->oldpid.data, ccf->pid.data)
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

    തിരികെ pid;
}


സ്ഥിതം ngx_int_t
ngx_get_options(പൂർണ്ണസംഖ്യ argc, അക്ഷരം *സ്ഥിരം *argv)
{
    u_char     *p;
    ngx_int_t   i;

    വേണ്ടി (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        എങ്കിൽ (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            തിരികെ NGX_ERROR;
        }

        ആകുമ്പോൾ (*p) {

            തിരഞ്ഞെടുപ്പ് (*p++) {

            അവസ്ഥ '?':
            അവസ്ഥ 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                നിർത്തുക;

            അവസ്ഥ 'v':
                ngx_show_version = 1;
                നിർത്തുക;

            അവസ്ഥ 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                നിർത്തുക;

            അവസ്ഥ 't':
                ngx_test_config = 1;
                നിർത്തുക;

            അവസ്ഥ 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                നിർത്തുക;

            അവസ്ഥ 'q':
                ngx_quiet_mode = 1;
                നിർത്തുക;

            അവസ്ഥ 'p':
                എങ്കിൽ (*p) {
                    ngx_prefix = p;
                    പോകുക next;
                }

                എങ്കിൽ (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    പോകുക next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                തിരികെ NGX_ERROR;

            അവസ്ഥ 'e':
                എങ്കിൽ (*p) {
                    ngx_error_log = p;

                } അല്ലെങ്കിൽ എങ്കിൽ (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } അല്ലെങ്കിൽ {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    തിരികെ NGX_ERROR;
                }

                എങ്കിൽ (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                പോകുക next;

            അവസ്ഥ 'c':
                എങ്കിൽ (*p) {
                    ngx_conf_file = p;
                    പോകുക next;
                }

                എങ്കിൽ (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    പോകുക next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                തിരികെ NGX_ERROR;

            അവസ്ഥ 'g':
                എങ്കിൽ (*p) {
                    ngx_conf_params = p;
                    പോകുക next;
                }

                എങ്കിൽ (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    പോകുക next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                തിരികെ NGX_ERROR;

            അവസ്ഥ 's':
                എങ്കിൽ (*p) {
                    ngx_signal = (അക്ഷരം *) p;

                } അല്ലെങ്കിൽ എങ്കിൽ (argv[++i]) {
                    ngx_signal = argv[i];

                } അല്ലെങ്കിൽ {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    തിരികെ NGX_ERROR;
                }

                എങ്കിൽ (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    പോകുക next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                തിരികെ NGX_ERROR;

#എങ്കിൽ (NGX_CONTROL_API)
            അവസ്ഥ 'l':
                ngx_control_api_enabled = 1;

                എങ്കിൽ (*p) {
                    ngx_control_addr = p;
                    പോകുക next;
                }

                എങ്കിൽ (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    പോകുക next;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                തിരികെ NGX_ERROR;
#endif

            സ്ഥിരസ്ഥിതി:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                തിരികെ NGX_ERROR;
            }
        }

    next:

        തുടരുക;
    }

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, പൂർണ്ണസംഖ്യ argc, അക്ഷരം *സ്ഥിരം *argv)
{
#എങ്കിൽ (NGX_FREEBSD)

    ngx_os_argv = (അക്ഷരം **) argv;
    ngx_argc = argc;
    ngx_argv = (അക്ഷരം **) argv;

#അല്ലെങ്കിൽ
    size_t     len;
    ngx_int_t  i;

    ngx_os_argv = (അക്ഷരം **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * വലുപ്പം(അക്ഷരം *), cycle->log);
    എങ്കിൽ (ngx_argv == ശൂന്യ) {
        തിരികെ NGX_ERROR;
    }

    വേണ്ടി (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        എങ്കിൽ (ngx_argv[i] == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        (ശൂന്യം) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = ശൂന്യ;

#endif

    ngx_os_environ = environ;

    തിരികെ NGX_OK;
}


സ്ഥിതം ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    size_t   len;

    എങ്കിൽ (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        എങ്കിൽ (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            എങ്കിൽ (p == ശൂന്യ) {
                തിരികെ NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

    } അല്ലെങ്കിൽ {

#ifndef NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        എങ്കിൽ (p == ശൂന്യ) {
            തിരികെ NGX_ERROR;
        }

        എങ്കിൽ (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            തിരികെ NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#അല്ലെങ്കിൽ

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#അല്ലെങ്കിൽ
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#endif
    }

    എങ്കിൽ (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } അല്ലെങ്കിൽ {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    എങ്കിൽ (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        തിരികെ NGX_ERROR;
    }

    വേണ്ടി (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        എങ്കിൽ (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.data + 1;
            cycle->conf_prefix.data = cycle->conf_file.data;
            നിർത്തുക;
        }
    }

    എങ്കിൽ (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.data = ngx_error_log;

    } അല്ലെങ്കിൽ {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    എങ്കിൽ (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    എങ്കിൽ (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    തിരികെ NGX_OK;
}


സ്ഥിതം ശൂന്യം *
ngx_core_module_create_conf(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(cycle->pool, വലുപ്പം(ngx_core_conf_t));
    എങ്കിൽ (ccf == ശൂന്യ) {
        തിരികെ ശൂന്യ;
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

    എങ്കിൽ (ngx_array_init(&ccf->env, cycle->pool, 1, വലുപ്പം(ngx_str_t))
        != NGX_OK)
    {
        തിരികെ ശൂന്യ;
    }

    തിരികെ ccf;
}


സ്ഥിതം അക്ഷരം *
ngx_core_module_init_conf(ngx_cycle_t *cycle, ശൂന്യം *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_മൂല്യം(ccf->daemon, 1);
    ngx_conf_init_മൂല്യം(ccf->master, 1);
    ngx_conf_init_msec_മൂല്യം(ccf->timer_resolution, 0);
    ngx_conf_init_msec_മൂല്യം(ccf->shutdown_timeout, 0);

    ngx_conf_init_മൂല്യം(ccf->worker_processes, 1);
    ngx_conf_init_മൂല്യം(ccf->debug_points, 0);

#എങ്കിൽ (NGX_HAVE_CPU_AFFINITY)

    എങ്കിൽ (!ccf->cpu_affinity_auto
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


    എങ്കിൽ (ccf->pid.len == 0) {
        ngx_str_set(&ccf->pid, NGX_PID_PATH);
    }

    എങ്കിൽ (ngx_conf_full_name(cycle, &ccf->pid, 0) != NGX_OK) {
        തിരികെ NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + വലുപ്പം(NGX_OLDPID_EXT);

    ccf->oldpid.data = ngx_pnalloc(cycle->pool, ccf->oldpid.len);
    എങ്കിൽ (ccf->oldpid.data == ശൂന്യ) {
        തിരികെ NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.data, ccf->pid.data, ccf->pid.len),
               NGX_OLDPID_EXT, വലുപ്പം(NGX_OLDPID_EXT));


    എങ്കിൽ (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        എങ്കിൽ (ngx_conf_full_name(cycle, &ccf->working_directory, 0) != NGX_OK) {
            തിരികെ NGX_CONF_ERROR;
        }

        എങ്കിൽ (ngx_file_info(ccf->working_directory.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.data);
            തിരികെ NGX_CONF_ERROR;
        }

        എങ്കിൽ (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.data);
            തിരികെ NGX_CONF_ERROR;
        }
    }


#എങ്കിൽ !(NGX_WIN32)

    എങ്കിൽ (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        ഘടന group   *grp;
        ഘടന passwd  *pwd;

        ngx_set_errno(0);
        pwd = getpwnam(NGX_USER);
        എങ്കിൽ (pwd == ശൂന്യ) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            തിരികെ NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_set_errno(0);
        grp = getgrnam(NGX_GROUP);
        എങ്കിൽ (grp == ശൂന്യ) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getgrnam(\"" NGX_GROUP "\") failed");
            തിരികെ NGX_CONF_ERROR;
        }

        ccf->group = grp->gr_gid;
    }


    എങ്കിൽ (ccf->lock_file.len == 0) {
        ngx_str_set(&ccf->lock_file, NGX_LOCK_PATH);
    }

    എങ്കിൽ (ngx_conf_full_name(cycle, &ccf->lock_file, 0) != NGX_OK) {
        തിരികെ NGX_CONF_ERROR;
    }

    {
    ngx_str_t  lock_file;

    lock_file = cycle->old_cycle->lock_file;

    എങ്കിൽ (lock_file.len) {
        lock_file.len--;

        എങ്കിൽ (ccf->lock_file.len != lock_file.len
            || ngx_strncmp(ccf->lock_file.data, lock_file.data, lock_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        cycle->lock_file.len = lock_file.len + 1;
        lock_file.len += വലുപ്പം(".accept");

        cycle->lock_file.data = ngx_pstrdup(cycle->pool, &lock_file);
        എങ്കിൽ (cycle->lock_file.data == ശൂന്യ) {
            തിരികെ NGX_CONF_ERROR;
        }

    } അല്ലെങ്കിൽ {
        cycle->lock_file.len = ccf->lock_file.len + 1;
        cycle->lock_file.data = ngx_pnalloc(cycle->pool,
                                      ccf->lock_file.len + വലുപ്പം(".accept"));
        എങ്കിൽ (cycle->lock_file.data == ശൂന്യ) {
            തിരികെ NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(cycle->lock_file.data, ccf->lock_file.data,
                              ccf->lock_file.len),
                   ".accept", വലുപ്പം(".accept"));
    }
    }

#endif

    തിരികെ NGX_CONF_OK;
}


സ്ഥിതം അക്ഷരം *
ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
#എങ്കിൽ (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    തിരികെ NGX_CONF_OK;

#അല്ലെങ്കിൽ

    ngx_core_conf_t  *ccf = conf;

    അക്ഷരം             *group;
    ഘടന passwd    *pwd;
    ഘടന group     *grp;
    ngx_str_t        *മൂല്യം;

    എങ്കിൽ (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        തിരികെ "is duplicate";
    }

    എങ്കിൽ (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        തിരികെ NGX_CONF_OK;
    }

    മൂല്യം = cf->args->elts;

    ccf->username = (അക്ഷരം *) മൂല്യം[1].data;

    ngx_set_errno(0);
    pwd = getpwnam((സ്ഥിരം അക്ഷരം *) മൂല്യം[1].data);
    എങ്കിൽ (pwd == ശൂന്യ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", മൂല്യം[1].data);
        തിരികെ NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    group = (അക്ഷരം *) ((cf->args->nelts == 2) ? മൂല്യം[1].data : മൂല്യം[2].data);

    ngx_set_errno(0);
    grp = getgrnam(group);
    എങ്കിൽ (grp == ശൂന്യ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", group);
        തിരികെ NGX_CONF_ERROR;
    }

    ccf->group = grp->gr_gid;

    തിരികെ NGX_CONF_OK;

#endif
}


സ്ഥിതം അക്ഷരം *
ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *മൂല്യം, *var;
    ngx_uint_t   i;

    var = ngx_array_തള്ളുക(&ccf->env);
    എങ്കിൽ (var == ശൂന്യ) {
        തിരികെ NGX_CONF_ERROR;
    }

    മൂല്യം = cf->args->elts;
    *var = മൂല്യം[1];

    വേണ്ടി (i = 0; i < മൂല്യം[1].len; i++) {

        എങ്കിൽ (മൂല്യം[1].data[i] == '=') {

            var->len = i;

            തിരികെ NGX_CONF_OK;
        }
    }

    തിരികെ NGX_CONF_OK;
}


സ്ഥിതം അക്ഷരം *
ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *മൂല്യം;
    ngx_uint_t        n, minus;

    എങ്കിൽ (ccf->priority != 0) {
        തിരികെ "is duplicate";
    }

    മൂല്യം = cf->args->elts;

    എങ്കിൽ (മൂല്യം[1].data[0] == '-') {
        n = 1;
        minus = 1;

    } അല്ലെങ്കിൽ എങ്കിൽ (മൂല്യം[1].data[0] == '+') {
        n = 1;
        minus = 0;

    } അല്ലെങ്കിൽ {
        n = 0;
        minus = 0;
    }

    ccf->priority = ngx_atoi(&മൂല്യം[1].data[n], മൂല്യം[1].len - n);
    എങ്കിൽ (ccf->priority == NGX_ERROR) {
        തിരികെ "invalid number";
    }

    എങ്കിൽ (minus) {
        ccf->priority = -ccf->priority;
    }

    തിരികെ NGX_CONF_OK;
}


സ്ഥിതം അക്ഷരം *
ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
#എങ്കിൽ (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *മൂല്യം;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *mask;

    എങ്കിൽ (ccf->cpu_affinity) {
        തിരികെ "is duplicate";
    }

    mask = ngx_palloc(cf->pool, (cf->args->nelts - 1) * വലുപ്പം(ngx_cpuset_t));
    എങ്കിൽ (mask == ശൂന്യ) {
        തിരികെ NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->args->nelts - 1;
    ccf->cpu_affinity = mask;

    മൂല്യം = cf->args->elts;

    എങ്കിൽ (ngx_strcmp(മൂല്യം[1].data, "auto") == 0) {

        എങ്കിൽ (cf->args->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            തിരികെ NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&mask[0]);
        വേണ്ടി (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_SET(i, &mask[0]);
        }

        n = 2;

    } അല്ലെങ്കിൽ {
        n = 1;
    }

    വേണ്ടി ( /* void */ ; n < cf->args->nelts; n++) {

        എങ്കിൽ (മൂല്യം[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            തിരികെ NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&mask[n - 1]);

        വേണ്ടി (p = മൂല്യം[n].data + മൂല്യം[n].len - 1;
             p >= മൂല്യം[n].data;
             p--)
        {
            ch = *p;

            എങ്കിൽ (ch == ' ') {
                തുടരുക;
            }

            i++;

            എങ്കിൽ (ch == '0') {
                തുടരുക;
            }

            എങ്കിൽ (ch == '1') {
                CPU_SET(i - 1, &mask[n - 1]);
                തുടരുക;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            തിരികെ NGX_CONF_ERROR;
        }
    }

#അല്ലെങ്കിൽ

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#endif

    തിരികെ NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#എങ്കിൽ (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *mask;
    ngx_core_conf_t  *ccf;

    സ്ഥിതം ngx_cpuset_t  result;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_cycle->conf_ctx,
                                           ngx_core_module);

    എങ്കിൽ (ccf->cpu_affinity == ശൂന്യ) {
        തിരികെ ശൂന്യ;
    }

    എങ്കിൽ (ccf->cpu_affinity_auto) {
        mask = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        വേണ്ടി (i = 0, j = n; /* void */ ; i++) {

            എങ്കിൽ (CPU_ISSET(i % CPU_SETSIZE, mask) && j-- == 0) {
                നിർത്തുക;
            }

            എങ്കിൽ (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                തിരികെ ശൂന്യ;
            }

            /* void */
        }

        CPU_ZERO(&result);
        CPU_SET(i % CPU_SETSIZE, &result);

        തിരികെ &result;
    }

    എങ്കിൽ (ccf->cpu_affinity_n > n) {
        തിരികെ &ccf->cpu_affinity[n];
    }

    തിരികെ &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#അല്ലെങ്കിൽ

    തിരികെ ശൂന്യ;

#endif
}


സ്ഥിതം അക്ഷരം *
ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
    ngx_str_t        *മൂല്യം;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    എങ്കിൽ (ccf->worker_processes != NGX_CONF_UNSET) {
        തിരികെ "is duplicate";
    }

    മൂല്യം = cf->args->elts;

    എങ്കിൽ (ngx_strcmp(മൂല്യം[1].data, "auto") == 0) {
        ccf->worker_processes = ngx_ncpu;
        തിരികെ NGX_CONF_OK;
    }

    ccf->worker_processes = ngx_atoi(മൂല്യം[1].data, മൂല്യം[1].len);

    എങ്കിൽ (ccf->worker_processes == NGX_ERROR) {
        തിരികെ "invalid value";
    }

    തിരികെ NGX_CONF_OK;
}


സ്ഥിതം അക്ഷരം *
ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, ശൂന്യം *conf)
{
#എങ്കിൽ (NGX_HAVE_DLOPEN)
    ശൂന്യം                *handle;
    അക്ഷരം               **names, **order;
    ngx_str_t           *മൂല്യം, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    എങ്കിൽ (cf->cycle->modules_used) {
        തിരികെ "is specified too late";
    }

    മൂല്യം = cf->args->elts;

    file = മൂല്യം[1];

    എങ്കിൽ (ngx_conf_full_name(cf->cycle, &file, 0) != NGX_OK) {
        തിരികെ NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_add(cf->cycle->pool, 0);
    എങ്കിൽ (cln == ശൂന്യ) {
        തിരികെ NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.data);
    എങ്കിൽ (handle == ശൂന്യ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.data, ngx_dlerror());
        തിരികെ NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->data = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    എങ്കിൽ (modules == ശൂന്യ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &മൂല്യം[1], "ngx_modules", ngx_dlerror());
        തിരികെ NGX_CONF_ERROR;
    }

    names = ngx_dlsym(handle, "ngx_module_names");
    എങ്കിൽ (names == ശൂന്യ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &മൂല്യം[1], "ngx_module_names", ngx_dlerror());
        തിരികെ NGX_CONF_ERROR;
    }

    order = ngx_dlsym(handle, "ngx_module_order");

    വേണ്ടി (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = names[i];

        എങ്കിൽ (ngx_add_module(cf, &file, module, order) != NGX_OK) {
            തിരികെ NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0, "module: %s i:%ui",
                       module->name, module->index);
    }

    തിരികെ NGX_CONF_OK;

#അല്ലെങ്കിൽ

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    തിരികെ NGX_CONF_ERROR;

#endif
}


#എങ്കിൽ (NGX_HAVE_DLOPEN)

സ്ഥിതം ശൂന്യം
ngx_unload_module(ശൂന്യം *data)
{
    ശൂന്യം  *handle = data;

    എങ്കിൽ (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#endif
