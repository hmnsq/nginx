
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>
#அந்தர்பாவ <nginx.h>


நிலையான வெறுமை ngx_show_version_info(வெறுமை);
நிலையான ngx_int_t ngx_add_inherited_sockets(ngx_cycle_t *cycle);
நிலையான வெறுமை ngx_cleanup_environment(வெறுமை *data);
நிலையான வெறுமை ngx_cleanup_environment_variable(வெறுமை *data);
நிலையான ngx_int_t ngx_get_options(முழு argc, எழுத்து *மாறிலி *argv);
நிலையான ngx_int_t ngx_process_options(ngx_cycle_t *cycle);
நிலையான ngx_int_t ngx_save_argv(ngx_cycle_t *cycle, முழு argc, எழுத்து *மாறிலி *argv);
நிலையான வெறுமை *ngx_core_module_create_conf(ngx_cycle_t *cycle);
நிலையான எழுத்து *ngx_core_module_init_conf(ngx_cycle_t *cycle, வெறுமை *conf);
நிலையான எழுத்து *ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
நிலையான எழுத்து *ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
நிலையான எழுத்து *ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
நிலையான எழுத்து *ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    வெறுமை *conf);
நிலையான எழுத்து *ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    வெறுமை *conf);
நிலையான எழுத்து *ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf);
#எனில் (NGX_HAVE_DLOPEN)
நிலையான வெறுமை ngx_unload_module(வெறுமை *data);
#endif


நிலையான ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


நிலையான ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, daemon),
      வெற்று },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, master),
      வெற்று },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      வெற்று },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, pid),
      வெற்று },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, lock_file),
      வெற்று },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_worker_processes,
      0,
      0,
      வெற்று },

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
      வெற்று },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_priority,
      0,
      0,
      வெற்று },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_1MORE,
      ngx_set_cpu_affinity,
      0,
      0,
      வெற்று },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_இலக்கம்_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      வெற்று },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_off_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      வெற்று },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      வெற்று },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      வெற்று },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_env,
      0,
      0,
      வெற்று },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_load_module,
      0,
      0,
      வெற்று },

      ngx_null_command
};


நிலையான ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    வெற்று,                                  /* init master */
    வெற்று,                                  /* init module */
    வெற்று,                                  /* init process */
    வெற்று,                                  /* init thread */
    வெற்று,                                  /* exit thread */
    வெற்று,                                  /* exit process */
    வெற்று,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


நிலையான ngx_uint_t   ngx_show_help;
நிலையான ngx_uint_t   ngx_show_version;
நிலையான ngx_uint_t   ngx_show_configure;
நிலையான u_char      *ngx_prefix;
நிலையான u_char      *ngx_error_log;
நிலையான u_char      *ngx_conf_file;
நிலையான u_char      *ngx_conf_params;
#எனில் (NGX_CONTROL_API)
நிலையான u_char      *ngx_control_addr;
#endif
நிலையான எழுத்து        *ngx_signal;


நிலையான எழுத்து **ngx_os_environ;


முழு ngx_cdecl
முதன்மை(முழு argc, எழுத்து *மாறிலி *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *log;
    ngx_uint_t        i;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    எனில் (ngx_strerror_init() != NGX_OK) {
        திரும்பு 1;
    }

    எனில் (ngx_get_options(argc, argv) != NGX_OK) {
        திரும்பு 1;
    }

    எனில் (ngx_show_version) {
        ngx_show_version_info();

        எனில் (!ngx_test_config) {
            திரும்பு 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#எனில் (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    எனில் (log == வெற்று) {
        திரும்பு 1;
    }

    /* STUB */
#எனில் (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_cycle, அளவு(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    init_cycle.pool = ngx_create_pool(1024, log);
    எனில் (init_cycle.pool == வெற்று) {
        திரும்பு 1;
    }

    எனில் (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        திரும்பு 1;
    }

    எனில் (ngx_process_options(&init_cycle) != NGX_OK) {
        திரும்பு 1;
    }

    எனில் (ngx_os_init(log) != NGX_OK) {
        திரும்பு 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    எனில் (ngx_crc32_table_init() != NGX_OK) {
        திரும்பு 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    எனில் (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        திரும்பு 1;
    }

#எனில் (NGX_CONTROL_API)
    ngx_control_preinit();
#endif

    எனில் (ngx_preinit_modules() != NGX_OK) {
        திரும்பு 1;
    }

    cycle = ngx_init_cycle(&init_cycle);
    எனில் (cycle == வெற்று) {
        எனில் (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        திரும்பு 1;
    }

    எனில் (ngx_test_config) {
        எனில் (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.data);
        }

        எனில் (ngx_dump_config) {
            cd = cycle->config_dump.elts;

            க்கு (i = 0; i < cycle->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (வெறுமை) ngx_write_fd(ngx_stdout, cd[i].name.data,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].buffer;

                (வெறுமை) ngx_write_fd(ngx_stdout, b->pos, b->last - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        திரும்பு 0;
    }

    எனில் (ngx_signal) {
        திரும்பு ngx_signal_process(cycle, ngx_signal);
    }

    ngx_os_status(cycle->log);

    ngx_cycle = cycle;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    எனில் (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#எனில் !(NGX_WIN32)

    எனில் (ngx_init_signals(cycle->log) != NGX_OK) {
        திரும்பு 1;
    }

    எனில் (!ngx_inherited && ccf->daemon) {
        எனில் (ngx_daemon(cycle->log) != NGX_OK) {
            திரும்பு 1;
        }

        ngx_daemonized = 1;
    }

    எனில் (ngx_inherited) {
        ngx_daemonized = 1;
    }

#endif

#எனில் (NGX_CONTROL_API)
    எனில் (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "control API unavailable in single process mode");

    } இல்லையெனில் {
        எனில் (ngx_control_init(ngx_control_addr) != NGX_OK) {
            திரும்பு 1;
        }
    }
#endif

    எனில் (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        திரும்பு 1;
    }

    எனில் (ngx_log_redirect_stderr(cycle) != NGX_OK) {
        திரும்பு 1;
    }

    எனில் (log->file->fd != ngx_stderr) {
        எனில் (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    எனில் (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } இல்லையெனில் {
        ngx_master_process_cycle(cycle);
    }

    திரும்பு 0;
}


நிலையான வெறுமை
ngx_show_version_info(வெறுமை)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    எனில் (ngx_show_help) {
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
#எனில் (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#endif
#ifdef NGX_PREFIX
            "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#இல்லையெனில்
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
            "  -e filename   : set error log file (default: "
#ifdef NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#இல்லையெனில்
                               NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#endif
            "  -c filename   : set configuration file (default: " NGX_CONF_PATH
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    எனில் (ngx_show_configure) {

#ifdef NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#endif

#எனில் (NGX_SSL)
        எனில் (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } இல்லையெனில் {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((எழுத்து *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#இல்லையெனில்
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#endif
#endif

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


நிலையான ngx_int_t
ngx_add_inherited_sockets(ngx_cycle_t *cycle)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    எனில் (inherited == வெற்று) {
        திரும்பு NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    எனில் (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       அளவு(ngx_listening_t))
        != NGX_OK)
    {
        திரும்பு NGX_ERROR;
    }

    க்கு (p = inherited, v = p; *p; p++) {
        எனில் (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            எனில் (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                நிறுத்து;
            }

            v = p + 1;

            ls = ngx_array_தள்ளு(&cycle->listening);
            எனில் (ls == வெற்று) {
                திரும்பு NGX_ERROR;
            }

            ngx_memzero(ls, அளவு(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    எனில் (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    திரும்பு ngx_set_inherited_sockets(cycle);
}


எழுத்து **
ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last)
{
    எழுத்து                **p, **env, *str;
    size_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    எனில் (last == வெற்று && ccf->environment) {
        திரும்பு ccf->environment;
    }

    var = ccf->env.elts;

    க்கு (i = 0; i < ccf->env.nelts; i++) {
        எனில் (ngx_strcmp(var[i].data, "TZ") == 0
            || ngx_strncmp(var[i].data, "TZ=", 3) == 0)
        {
            செல் tz_found;
        }
    }

    var = ngx_array_தள்ளு(&ccf->env);
    எனில் (var == வெற்று) {
        திரும்பு வெற்று;
    }

    var->len = 2;
    var->data = (u_char *) "TZ";

    var = ccf->env.elts;

tz_found:

    n = 0;

    க்கு (i = 0; i < ccf->env.nelts; i++) {

        எனில் (var[i].data[var[i].len] == '=') {
            n++;
            தொடர்;
        }

        க்கு (p = ngx_os_environ; *p; p++) {

            எனில் (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                நிறுத்து;
            }
        }
    }

    எனில் (last) {
        env = ngx_alloc((*last + n + 1) * அளவு(எழுத்து *), cycle->log);
        எனில் (env == வெற்று) {
            திரும்பு வெற்று;
        }

        *last = n;

    } இல்லையெனில் {
        cln = ngx_pool_cleanup_add(cycle->pool, 0);
        எனில் (cln == வெற்று) {
            திரும்பு வெற்று;
        }

        env = ngx_alloc((n + 1) * அளவு(எழுத்து *), cycle->log);
        எனில் (env == வெற்று) {
            திரும்பு வெற்று;
        }

        cln->handler = ngx_cleanup_environment;
        cln->data = env;
    }

    n = 0;

    க்கு (i = 0; i < ccf->env.nelts; i++) {

        எனில் (var[i].data[var[i].len] == '=') {

            எனில் (last) {
                env[n++] = (எழுத்து *) var[i].data;
                தொடர்;
            }

            cln = ngx_pool_cleanup_add(cycle->pool, 0);
            எனில் (cln == வெற்று) {
                திரும்பு வெற்று;
            }

            len = ngx_strlen(var[i].data) + 1;

            str = ngx_alloc(len, cycle->log);
            எனில் (str == வெற்று) {
                திரும்பு வெற்று;
            }

            ngx_memcpy(str, var[i].data, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->data = str;

            env[n++] = str;

            தொடர்;
        }

        க்கு (p = ngx_os_environ; *p; p++) {

            எனில் (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                நிறுத்து;
            }
        }
    }

    env[n] = வெற்று;

    எனில் (last == வெற்று) {
        ccf->environment = env;
        environ = env;
    }

    திரும்பு env;
}


நிலையான வெறுமை
ngx_cleanup_environment(வெறுமை *data)
{
    எழுத்து  **env = data;

    எனில் (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        திரும்பு;
    }

    ngx_free(env);
}


நிலையான வெறுமை
ngx_cleanup_environment_variable(வெறுமை *data)
{
    எழுத்து  *var = data;

    எழுத்து  **p;

    க்கு (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        எனில் (*p == var) {
            திரும்பு;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_இருமம்(ngx_cycle_t *cycle, எழுத்து *மாறிலி *argv)
{
    எழுத்து             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, அளவு(ngx_exec_ctx_t));

    ctx.path = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_set_environment(cycle, &n);
    எனில் (env == வெற்று) {
        திரும்பு NGX_INVALID_PID;
    }

    var = ngx_alloc(அளவு(NGINX_VAR)
                    + cycle->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    cycle->log);
    எனில் (var == வெற்று) {
        ngx_free(env);
        திரும்பு NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", அளவு(NGINX_VAR));

    ls = cycle->listening.elts;
    க்கு (i = 0; i < cycle->listening.nelts; i++) {
        எனில் (ls[i].ignore) {
            தொடர்;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#எனில் (NGX_SETPROCTITLE_USES_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#endif

#எனில் (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    எனில் (env[n] != வெற்று) {
        n++;
    }
#endif

    env[n] = வெற்று;

#எனில் (NGX_DEBUG)
    {
    எழுத்து  **e;
    க்கு (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0, "env: %s", *e);
    }
    }
#endif

    ctx.envp = (எழுத்து *மாறிலி *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    எனில் (ngx_rename_file(ccf->pid.data, ccf->oldpid.data) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.data, ccf->oldpid.data, argv[0]);

        ngx_free(env);
        ngx_free(var);

        திரும்பு NGX_INVALID_PID;
    }

    pid = ngx_execute(cycle, &ctx);

    எனில் (pid == NGX_INVALID_PID) {
        எனில் (ngx_rename_file(ccf->oldpid.data, ccf->pid.data)
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

    திரும்பு pid;
}


நிலையான ngx_int_t
ngx_get_options(முழு argc, எழுத்து *மாறிலி *argv)
{
    u_char     *p;
    ngx_int_t   i;

    க்கு (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        எனில் (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            திரும்பு NGX_ERROR;
        }

        போது (*p) {

            தேர்வு (*p++) {

            நிலை '?':
            நிலை 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                நிறுத்து;

            நிலை 'v':
                ngx_show_version = 1;
                நிறுத்து;

            நிலை 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                நிறுத்து;

            நிலை 't':
                ngx_test_config = 1;
                நிறுத்து;

            நிலை 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                நிறுத்து;

            நிலை 'q':
                ngx_quiet_mode = 1;
                நிறுத்து;

            நிலை 'p':
                எனில் (*p) {
                    ngx_prefix = p;
                    செல் next;
                }

                எனில் (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    செல் next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                திரும்பு NGX_ERROR;

            நிலை 'e':
                எனில் (*p) {
                    ngx_error_log = p;

                } இல்லையெனில் எனில் (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } இல்லையெனில் {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    திரும்பு NGX_ERROR;
                }

                எனில் (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                செல் next;

            நிலை 'c':
                எனில் (*p) {
                    ngx_conf_file = p;
                    செல் next;
                }

                எனில் (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    செல் next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                திரும்பு NGX_ERROR;

            நிலை 'g':
                எனில் (*p) {
                    ngx_conf_params = p;
                    செல் next;
                }

                எனில் (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    செல் next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                திரும்பு NGX_ERROR;

            நிலை 's':
                எனில் (*p) {
                    ngx_signal = (எழுத்து *) p;

                } இல்லையெனில் எனில் (argv[++i]) {
                    ngx_signal = argv[i];

                } இல்லையெனில் {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    திரும்பு NGX_ERROR;
                }

                எனில் (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    செல் next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                திரும்பு NGX_ERROR;

#எனில் (NGX_CONTROL_API)
            நிலை 'l':
                ngx_control_api_enabled = 1;

                எனில் (*p) {
                    ngx_control_addr = p;
                    செல் next;
                }

                எனில் (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    செல் next;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                திரும்பு NGX_ERROR;
#endif

            இயல்பு:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                திரும்பு NGX_ERROR;
            }
        }

    next:

        தொடர்;
    }

    திரும்பு NGX_OK;
}


நிலையான ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, முழு argc, எழுத்து *மாறிலி *argv)
{
#எனில் (NGX_FREEBSD)

    ngx_os_argv = (எழுத்து **) argv;
    ngx_argc = argc;
    ngx_argv = (எழுத்து **) argv;

#இல்லையெனில்
    size_t     len;
    ngx_int_t  i;

    ngx_os_argv = (எழுத்து **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * அளவு(எழுத்து *), cycle->log);
    எனில் (ngx_argv == வெற்று) {
        திரும்பு NGX_ERROR;
    }

    க்கு (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        எனில் (ngx_argv[i] == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        (வெறுமை) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = வெற்று;

#endif

    ngx_os_environ = environ;

    திரும்பு NGX_OK;
}


நிலையான ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    size_t   len;

    எனில் (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        எனில் (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            எனில் (p == வெற்று) {
                திரும்பு NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

    } இல்லையெனில் {

#ifndef NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        எனில் (p == வெற்று) {
            திரும்பு NGX_ERROR;
        }

        எனில் (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            திரும்பு NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#இல்லையெனில்

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#இல்லையெனில்
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#endif
    }

    எனில் (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } இல்லையெனில் {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    எனில் (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        திரும்பு NGX_ERROR;
    }

    க்கு (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        எனில் (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.data + 1;
            cycle->conf_prefix.data = cycle->conf_file.data;
            நிறுத்து;
        }
    }

    எனில் (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.data = ngx_error_log;

    } இல்லையெனில் {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    எனில் (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    எனில் (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    திரும்பு NGX_OK;
}


நிலையான வெறுமை *
ngx_core_module_create_conf(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(cycle->pool, அளவு(ngx_core_conf_t));
    எனில் (ccf == வெற்று) {
        திரும்பு வெற்று;
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

    எனில் (ngx_array_init(&ccf->env, cycle->pool, 1, அளவு(ngx_str_t))
        != NGX_OK)
    {
        திரும்பு வெற்று;
    }

    திரும்பு ccf;
}


நிலையான எழுத்து *
ngx_core_module_init_conf(ngx_cycle_t *cycle, வெறுமை *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_மதிப்பு(ccf->daemon, 1);
    ngx_conf_init_மதிப்பு(ccf->master, 1);
    ngx_conf_init_msec_மதிப்பு(ccf->timer_resolution, 0);
    ngx_conf_init_msec_மதிப்பு(ccf->shutdown_timeout, 0);

    ngx_conf_init_மதிப்பு(ccf->worker_processes, 1);
    ngx_conf_init_மதிப்பு(ccf->debug_points, 0);

#எனில் (NGX_HAVE_CPU_AFFINITY)

    எனில் (!ccf->cpu_affinity_auto
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


    எனில் (ccf->pid.len == 0) {
        ngx_str_set(&ccf->pid, NGX_PID_PATH);
    }

    எனில் (ngx_conf_full_name(cycle, &ccf->pid, 0) != NGX_OK) {
        திரும்பு NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + அளவு(NGX_OLDPID_EXT);

    ccf->oldpid.data = ngx_pnalloc(cycle->pool, ccf->oldpid.len);
    எனில் (ccf->oldpid.data == வெற்று) {
        திரும்பு NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.data, ccf->pid.data, ccf->pid.len),
               NGX_OLDPID_EXT, அளவு(NGX_OLDPID_EXT));


    எனில் (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        எனில் (ngx_conf_full_name(cycle, &ccf->working_directory, 0) != NGX_OK) {
            திரும்பு NGX_CONF_ERROR;
        }

        எனில் (ngx_file_info(ccf->working_directory.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.data);
            திரும்பு NGX_CONF_ERROR;
        }

        எனில் (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.data);
            திரும்பு NGX_CONF_ERROR;
        }
    }


#எனில் !(NGX_WIN32)

    எனில் (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        கட்டமைப்பு group   *grp;
        கட்டமைப்பு passwd  *pwd;

        ngx_set_errno(0);
        pwd = getpwnam(NGX_USER);
        எனில் (pwd == வெற்று) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            திரும்பு NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_set_errno(0);
        grp = getgrnam(NGX_GROUP);
        எனில் (grp == வெற்று) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getgrnam(\"" NGX_GROUP "\") failed");
            திரும்பு NGX_CONF_ERROR;
        }

        ccf->group = grp->gr_gid;
    }


    எனில் (ccf->lock_file.len == 0) {
        ngx_str_set(&ccf->lock_file, NGX_LOCK_PATH);
    }

    எனில் (ngx_conf_full_name(cycle, &ccf->lock_file, 0) != NGX_OK) {
        திரும்பு NGX_CONF_ERROR;
    }

    {
    ngx_str_t  lock_file;

    lock_file = cycle->old_cycle->lock_file;

    எனில் (lock_file.len) {
        lock_file.len--;

        எனில் (ccf->lock_file.len != lock_file.len
            || ngx_strncmp(ccf->lock_file.data, lock_file.data, lock_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        cycle->lock_file.len = lock_file.len + 1;
        lock_file.len += அளவு(".accept");

        cycle->lock_file.data = ngx_pstrdup(cycle->pool, &lock_file);
        எனில் (cycle->lock_file.data == வெற்று) {
            திரும்பு NGX_CONF_ERROR;
        }

    } இல்லையெனில் {
        cycle->lock_file.len = ccf->lock_file.len + 1;
        cycle->lock_file.data = ngx_pnalloc(cycle->pool,
                                      ccf->lock_file.len + அளவு(".accept"));
        எனில் (cycle->lock_file.data == வெற்று) {
            திரும்பு NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(cycle->lock_file.data, ccf->lock_file.data,
                              ccf->lock_file.len),
                   ".accept", அளவு(".accept"));
    }
    }

#endif

    திரும்பு NGX_CONF_OK;
}


நிலையான எழுத்து *
ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
#எனில் (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    திரும்பு NGX_CONF_OK;

#இல்லையெனில்

    ngx_core_conf_t  *ccf = conf;

    எழுத்து             *group;
    கட்டமைப்பு passwd    *pwd;
    கட்டமைப்பு group     *grp;
    ngx_str_t        *மதிப்பு;

    எனில் (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        திரும்பு "is duplicate";
    }

    எனில் (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        திரும்பு NGX_CONF_OK;
    }

    மதிப்பு = cf->args->elts;

    ccf->username = (எழுத்து *) மதிப்பு[1].data;

    ngx_set_errno(0);
    pwd = getpwnam((மாறிலி எழுத்து *) மதிப்பு[1].data);
    எனில் (pwd == வெற்று) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", மதிப்பு[1].data);
        திரும்பு NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    group = (எழுத்து *) ((cf->args->nelts == 2) ? மதிப்பு[1].data : மதிப்பு[2].data);

    ngx_set_errno(0);
    grp = getgrnam(group);
    எனில் (grp == வெற்று) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", group);
        திரும்பு NGX_CONF_ERROR;
    }

    ccf->group = grp->gr_gid;

    திரும்பு NGX_CONF_OK;

#endif
}


நிலையான எழுத்து *
ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *மதிப்பு, *var;
    ngx_uint_t   i;

    var = ngx_array_தள்ளு(&ccf->env);
    எனில் (var == வெற்று) {
        திரும்பு NGX_CONF_ERROR;
    }

    மதிப்பு = cf->args->elts;
    *var = மதிப்பு[1];

    க்கு (i = 0; i < மதிப்பு[1].len; i++) {

        எனில் (மதிப்பு[1].data[i] == '=') {

            var->len = i;

            திரும்பு NGX_CONF_OK;
        }
    }

    திரும்பு NGX_CONF_OK;
}


நிலையான எழுத்து *
ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *மதிப்பு;
    ngx_uint_t        n, minus;

    எனில் (ccf->priority != 0) {
        திரும்பு "is duplicate";
    }

    மதிப்பு = cf->args->elts;

    எனில் (மதிப்பு[1].data[0] == '-') {
        n = 1;
        minus = 1;

    } இல்லையெனில் எனில் (மதிப்பு[1].data[0] == '+') {
        n = 1;
        minus = 0;

    } இல்லையெனில் {
        n = 0;
        minus = 0;
    }

    ccf->priority = ngx_atoi(&மதிப்பு[1].data[n], மதிப்பு[1].len - n);
    எனில் (ccf->priority == NGX_ERROR) {
        திரும்பு "invalid number";
    }

    எனில் (minus) {
        ccf->priority = -ccf->priority;
    }

    திரும்பு NGX_CONF_OK;
}


நிலையான எழுத்து *
ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
#எனில் (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *மதிப்பு;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *mask;

    எனில் (ccf->cpu_affinity) {
        திரும்பு "is duplicate";
    }

    mask = ngx_palloc(cf->pool, (cf->args->nelts - 1) * அளவு(ngx_cpuset_t));
    எனில் (mask == வெற்று) {
        திரும்பு NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->args->nelts - 1;
    ccf->cpu_affinity = mask;

    மதிப்பு = cf->args->elts;

    எனில் (ngx_strcmp(மதிப்பு[1].data, "auto") == 0) {

        எனில் (cf->args->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            திரும்பு NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&mask[0]);
        க்கு (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_SET(i, &mask[0]);
        }

        n = 2;

    } இல்லையெனில் {
        n = 1;
    }

    க்கு ( /* void */ ; n < cf->args->nelts; n++) {

        எனில் (மதிப்பு[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            திரும்பு NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&mask[n - 1]);

        க்கு (p = மதிப்பு[n].data + மதிப்பு[n].len - 1;
             p >= மதிப்பு[n].data;
             p--)
        {
            ch = *p;

            எனில் (ch == ' ') {
                தொடர்;
            }

            i++;

            எனில் (ch == '0') {
                தொடர்;
            }

            எனில் (ch == '1') {
                CPU_SET(i - 1, &mask[n - 1]);
                தொடர்;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            திரும்பு NGX_CONF_ERROR;
        }
    }

#இல்லையெனில்

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#endif

    திரும்பு NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#எனில் (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *mask;
    ngx_core_conf_t  *ccf;

    நிலையான ngx_cpuset_t  result;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_cycle->conf_ctx,
                                           ngx_core_module);

    எனில் (ccf->cpu_affinity == வெற்று) {
        திரும்பு வெற்று;
    }

    எனில் (ccf->cpu_affinity_auto) {
        mask = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        க்கு (i = 0, j = n; /* void */ ; i++) {

            எனில் (CPU_ISSET(i % CPU_SETSIZE, mask) && j-- == 0) {
                நிறுத்து;
            }

            எனில் (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                திரும்பு வெற்று;
            }

            /* void */
        }

        CPU_ZERO(&result);
        CPU_SET(i % CPU_SETSIZE, &result);

        திரும்பு &result;
    }

    எனில் (ccf->cpu_affinity_n > n) {
        திரும்பு &ccf->cpu_affinity[n];
    }

    திரும்பு &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#இல்லையெனில்

    திரும்பு வெற்று;

#endif
}


நிலையான எழுத்து *
ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
    ngx_str_t        *மதிப்பு;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    எனில் (ccf->worker_processes != NGX_CONF_UNSET) {
        திரும்பு "is duplicate";
    }

    மதிப்பு = cf->args->elts;

    எனில் (ngx_strcmp(மதிப்பு[1].data, "auto") == 0) {
        ccf->worker_processes = ngx_ncpu;
        திரும்பு NGX_CONF_OK;
    }

    ccf->worker_processes = ngx_atoi(மதிப்பு[1].data, மதிப்பு[1].len);

    எனில் (ccf->worker_processes == NGX_ERROR) {
        திரும்பு "invalid value";
    }

    திரும்பு NGX_CONF_OK;
}


நிலையான எழுத்து *
ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, வெறுமை *conf)
{
#எனில் (NGX_HAVE_DLOPEN)
    வெறுமை                *handle;
    எழுத்து               **names, **order;
    ngx_str_t           *மதிப்பு, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    எனில் (cf->cycle->modules_used) {
        திரும்பு "is specified too late";
    }

    மதிப்பு = cf->args->elts;

    file = மதிப்பு[1];

    எனில் (ngx_conf_full_name(cf->cycle, &file, 0) != NGX_OK) {
        திரும்பு NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_add(cf->cycle->pool, 0);
    எனில் (cln == வெற்று) {
        திரும்பு NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.data);
    எனில் (handle == வெற்று) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.data, ngx_dlerror());
        திரும்பு NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->data = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    எனில் (modules == வெற்று) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &மதிப்பு[1], "ngx_modules", ngx_dlerror());
        திரும்பு NGX_CONF_ERROR;
    }

    names = ngx_dlsym(handle, "ngx_module_names");
    எனில் (names == வெற்று) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &மதிப்பு[1], "ngx_module_names", ngx_dlerror());
        திரும்பு NGX_CONF_ERROR;
    }

    order = ngx_dlsym(handle, "ngx_module_order");

    க்கு (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = names[i];

        எனில் (ngx_add_module(cf, &file, module, order) != NGX_OK) {
            திரும்பு NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0, "module: %s i:%ui",
                       module->name, module->index);
    }

    திரும்பு NGX_CONF_OK;

#இல்லையெனில்

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    திரும்பு NGX_CONF_ERROR;

#endif
}


#எனில் (NGX_HAVE_DLOPEN)

நிலையான வெறுமை
ngx_unload_module(வெறுமை *data)
{
    வெறுமை  *handle = data;

    எனில் (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#endif
