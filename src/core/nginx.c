
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <nginx.h>


ایستا تهی ngx_show_version_info(تهی);
ایستا ngx_int_t ngx_add_inherited_sockets(ngx_cycle_t *cycle);
ایستا تهی ngx_cleanup_environment(تهی *data);
ایستا تهی ngx_cleanup_environment_variable(تهی *data);
ایستا ngx_int_t ngx_get_options(صحیح argc, نویسه *ثابت *argv);
ایستا ngx_int_t ngx_process_options(ngx_cycle_t *cycle);
ایستا ngx_int_t ngx_save_argv(ngx_cycle_t *cycle, صحیح argc, نویسه *ثابت *argv);
ایستا تهی *ngx_core_module_create_conf(ngx_cycle_t *cycle);
ایستا نویسه *ngx_core_module_init_conf(ngx_cycle_t *cycle, تهی *conf);
ایستا نویسه *ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
ایستا نویسه *ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
ایستا نویسه *ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
ایستا نویسه *ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    تهی *conf);
ایستا نویسه *ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    تهی *conf);
ایستا نویسه *ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf);
#اگر (NGX_HAVE_DLOPEN)
ایستا تهی ngx_unload_module(تهی *data);
#endif


ایستا ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


ایستا ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, daemon),
      پوچ },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, master),
      پوچ },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      پوچ },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, pid),
      پوچ },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, lock_file),
      پوچ },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_worker_processes,
      0,
      0,
      پوچ },

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
      پوچ },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_priority,
      0,
      0,
      پوچ },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_1MORE,
      ngx_set_cpu_affinity,
      0,
      0,
      پوچ },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_رقم_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      پوچ },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_off_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      پوچ },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      پوچ },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      پوچ },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_env,
      0,
      0,
      پوچ },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_load_module,
      0,
      0,
      پوچ },

      ngx_null_command
};


ایستا ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    پوچ,                                  /* init master */
    پوچ,                                  /* init module */
    پوچ,                                  /* init process */
    پوچ,                                  /* init thread */
    پوچ,                                  /* exit thread */
    پوچ,                                  /* exit process */
    پوچ,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ایستا ngx_uint_t   ngx_show_help;
ایستا ngx_uint_t   ngx_show_version;
ایستا ngx_uint_t   ngx_show_configure;
ایستا u_char      *ngx_prefix;
ایستا u_char      *ngx_error_log;
ایستا u_char      *ngx_conf_file;
ایستا u_char      *ngx_conf_params;
#اگر (NGX_CONTROL_API)
ایستا u_char      *ngx_control_addr;
#endif
ایستا نویسه        *ngx_signal;


ایستا نویسه **ngx_os_environ;


صحیح ngx_cdecl
اصلی(صحیح argc, نویسه *ثابت *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *log;
    ngx_uint_t        i;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    اگر (ngx_strerror_init() != NGX_OK) {
        بازگشت 1;
    }

    اگر (ngx_get_options(argc, argv) != NGX_OK) {
        بازگشت 1;
    }

    اگر (ngx_show_version) {
        ngx_show_version_info();

        اگر (!ngx_test_config) {
            بازگشت 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#اگر (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    اگر (log == پوچ) {
        بازگشت 1;
    }

    /* STUB */
#اگر (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_cycle, اندازه(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    init_cycle.pool = ngx_create_pool(1024, log);
    اگر (init_cycle.pool == پوچ) {
        بازگشت 1;
    }

    اگر (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        بازگشت 1;
    }

    اگر (ngx_process_options(&init_cycle) != NGX_OK) {
        بازگشت 1;
    }

    اگر (ngx_os_init(log) != NGX_OK) {
        بازگشت 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    اگر (ngx_crc32_table_init() != NGX_OK) {
        بازگشت 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    اگر (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        بازگشت 1;
    }

#اگر (NGX_CONTROL_API)
    ngx_control_preinit();
#endif

    اگر (ngx_preinit_modules() != NGX_OK) {
        بازگشت 1;
    }

    cycle = ngx_init_cycle(&init_cycle);
    اگر (cycle == پوچ) {
        اگر (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        بازگشت 1;
    }

    اگر (ngx_test_config) {
        اگر (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.data);
        }

        اگر (ngx_dump_config) {
            cd = cycle->config_dump.elts;

            برای (i = 0; i < cycle->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (تهی) ngx_write_fd(ngx_stdout, cd[i].name.data,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].buffer;

                (تهی) ngx_write_fd(ngx_stdout, b->pos, b->last - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        بازگشت 0;
    }

    اگر (ngx_signal) {
        بازگشت ngx_signal_process(cycle, ngx_signal);
    }

    ngx_os_status(cycle->log);

    ngx_cycle = cycle;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    اگر (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#اگر !(NGX_WIN32)

    اگر (ngx_init_signals(cycle->log) != NGX_OK) {
        بازگشت 1;
    }

    اگر (!ngx_inherited && ccf->daemon) {
        اگر (ngx_daemon(cycle->log) != NGX_OK) {
            بازگشت 1;
        }

        ngx_daemonized = 1;
    }

    اگر (ngx_inherited) {
        ngx_daemonized = 1;
    }

#endif

#اگر (NGX_CONTROL_API)
    اگر (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "control API unavailable in single process mode");

    } وگرنه {
        اگر (ngx_control_init(ngx_control_addr) != NGX_OK) {
            بازگشت 1;
        }
    }
#endif

    اگر (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        بازگشت 1;
    }

    اگر (ngx_log_redirect_stderr(cycle) != NGX_OK) {
        بازگشت 1;
    }

    اگر (log->file->fd != ngx_stderr) {
        اگر (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    اگر (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } وگرنه {
        ngx_master_process_cycle(cycle);
    }

    بازگشت 0;
}


ایستا تهی
ngx_show_version_info(تهی)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    اگر (ngx_show_help) {
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
#اگر (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#endif
#ifdef NGX_PREFIX
            "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#وگرنه
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
            "  -e filename   : set error log file (default: "
#ifdef NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#وگرنه
                               NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#endif
            "  -c filename   : set configuration file (default: " NGX_CONF_PATH
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    اگر (ngx_show_configure) {

#ifdef NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#endif

#اگر (NGX_SSL)
        اگر (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } وگرنه {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((نویسه *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#وگرنه
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#endif
#endif

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


ایستا ngx_int_t
ngx_add_inherited_sockets(ngx_cycle_t *cycle)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    اگر (inherited == پوچ) {
        بازگشت NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    اگر (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       اندازه(ngx_listening_t))
        != NGX_OK)
    {
        بازگشت NGX_ERROR;
    }

    برای (p = inherited, v = p; *p; p++) {
        اگر (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            اگر (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                بشکن;
            }

            v = p + 1;

            ls = ngx_array_افزودن(&cycle->listening);
            اگر (ls == پوچ) {
                بازگشت NGX_ERROR;
            }

            ngx_memzero(ls, اندازه(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    اگر (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    بازگشت ngx_set_inherited_sockets(cycle);
}


نویسه **
ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last)
{
    نویسه                **p, **env, *str;
    size_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    اگر (last == پوچ && ccf->environment) {
        بازگشت ccf->environment;
    }

    var = ccf->env.elts;

    برای (i = 0; i < ccf->env.nelts; i++) {
        اگر (ngx_strcmp(var[i].data, "TZ") == 0
            || ngx_strncmp(var[i].data, "TZ=", 3) == 0)
        {
            برو tz_found;
        }
    }

    var = ngx_array_افزودن(&ccf->env);
    اگر (var == پوچ) {
        بازگشت پوچ;
    }

    var->len = 2;
    var->data = (u_char *) "TZ";

    var = ccf->env.elts;

tz_found:

    n = 0;

    برای (i = 0; i < ccf->env.nelts; i++) {

        اگر (var[i].data[var[i].len] == '=') {
            n++;
            ادامه;
        }

        برای (p = ngx_os_environ; *p; p++) {

            اگر (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                بشکن;
            }
        }
    }

    اگر (last) {
        env = ngx_alloc((*last + n + 1) * اندازه(نویسه *), cycle->log);
        اگر (env == پوچ) {
            بازگشت پوچ;
        }

        *last = n;

    } وگرنه {
        cln = ngx_pool_cleanup_add(cycle->pool, 0);
        اگر (cln == پوچ) {
            بازگشت پوچ;
        }

        env = ngx_alloc((n + 1) * اندازه(نویسه *), cycle->log);
        اگر (env == پوچ) {
            بازگشت پوچ;
        }

        cln->handler = ngx_cleanup_environment;
        cln->data = env;
    }

    n = 0;

    برای (i = 0; i < ccf->env.nelts; i++) {

        اگر (var[i].data[var[i].len] == '=') {

            اگر (last) {
                env[n++] = (نویسه *) var[i].data;
                ادامه;
            }

            cln = ngx_pool_cleanup_add(cycle->pool, 0);
            اگر (cln == پوچ) {
                بازگشت پوچ;
            }

            len = ngx_strlen(var[i].data) + 1;

            str = ngx_alloc(len, cycle->log);
            اگر (str == پوچ) {
                بازگشت پوچ;
            }

            ngx_memcpy(str, var[i].data, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->data = str;

            env[n++] = str;

            ادامه;
        }

        برای (p = ngx_os_environ; *p; p++) {

            اگر (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                بشکن;
            }
        }
    }

    env[n] = پوچ;

    اگر (last == پوچ) {
        ccf->environment = env;
        environ = env;
    }

    بازگشت env;
}


ایستا تهی
ngx_cleanup_environment(تهی *data)
{
    نویسه  **env = data;

    اگر (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        بازگشت;
    }

    ngx_free(env);
}


ایستا تهی
ngx_cleanup_environment_variable(تهی *data)
{
    نویسه  *var = data;

    نویسه  **p;

    برای (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        اگر (*p == var) {
            بازگشت;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_دودویی(ngx_cycle_t *cycle, نویسه *ثابت *argv)
{
    نویسه             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, اندازه(ngx_exec_ctx_t));

    ctx.path = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_set_environment(cycle, &n);
    اگر (env == پوچ) {
        بازگشت NGX_INVALID_PID;
    }

    var = ngx_alloc(اندازه(NGINX_VAR)
                    + cycle->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    cycle->log);
    اگر (var == پوچ) {
        ngx_free(env);
        بازگشت NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", اندازه(NGINX_VAR));

    ls = cycle->listening.elts;
    برای (i = 0; i < cycle->listening.nelts; i++) {
        اگر (ls[i].ignore) {
            ادامه;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#اگر (NGX_SETPROCTITLE_USES_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#endif

#اگر (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    اگر (env[n] != پوچ) {
        n++;
    }
#endif

    env[n] = پوچ;

#اگر (NGX_DEBUG)
    {
    نویسه  **e;
    برای (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0, "env: %s", *e);
    }
    }
#endif

    ctx.envp = (نویسه *ثابت *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    اگر (ngx_rename_file(ccf->pid.data, ccf->oldpid.data) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.data, ccf->oldpid.data, argv[0]);

        ngx_free(env);
        ngx_free(var);

        بازگشت NGX_INVALID_PID;
    }

    pid = ngx_execute(cycle, &ctx);

    اگر (pid == NGX_INVALID_PID) {
        اگر (ngx_rename_file(ccf->oldpid.data, ccf->pid.data)
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

    بازگشت pid;
}


ایستا ngx_int_t
ngx_get_options(صحیح argc, نویسه *ثابت *argv)
{
    u_char     *p;
    ngx_int_t   i;

    برای (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        اگر (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            بازگشت NGX_ERROR;
        }

        وقتی (*p) {

            انتخاب (*p++) {

            حالت '?':
            حالت 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                بشکن;

            حالت 'v':
                ngx_show_version = 1;
                بشکن;

            حالت 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                بشکن;

            حالت 't':
                ngx_test_config = 1;
                بشکن;

            حالت 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                بشکن;

            حالت 'q':
                ngx_quiet_mode = 1;
                بشکن;

            حالت 'p':
                اگر (*p) {
                    ngx_prefix = p;
                    برو next;
                }

                اگر (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    برو next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                بازگشت NGX_ERROR;

            حالت 'e':
                اگر (*p) {
                    ngx_error_log = p;

                } وگرنه اگر (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } وگرنه {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    بازگشت NGX_ERROR;
                }

                اگر (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                برو next;

            حالت 'c':
                اگر (*p) {
                    ngx_conf_file = p;
                    برو next;
                }

                اگر (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    برو next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                بازگشت NGX_ERROR;

            حالت 'g':
                اگر (*p) {
                    ngx_conf_params = p;
                    برو next;
                }

                اگر (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    برو next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                بازگشت NGX_ERROR;

            حالت 's':
                اگر (*p) {
                    ngx_signal = (نویسه *) p;

                } وگرنه اگر (argv[++i]) {
                    ngx_signal = argv[i];

                } وگرنه {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    بازگشت NGX_ERROR;
                }

                اگر (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    برو next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                بازگشت NGX_ERROR;

#اگر (NGX_CONTROL_API)
            حالت 'l':
                ngx_control_api_enabled = 1;

                اگر (*p) {
                    ngx_control_addr = p;
                    برو next;
                }

                اگر (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    برو next;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                بازگشت NGX_ERROR;
#endif

            پیشفرض:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                بازگشت NGX_ERROR;
            }
        }

    next:

        ادامه;
    }

    بازگشت NGX_OK;
}


ایستا ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, صحیح argc, نویسه *ثابت *argv)
{
#اگر (NGX_FREEBSD)

    ngx_os_argv = (نویسه **) argv;
    ngx_argc = argc;
    ngx_argv = (نویسه **) argv;

#وگرنه
    size_t     len;
    ngx_int_t  i;

    ngx_os_argv = (نویسه **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * اندازه(نویسه *), cycle->log);
    اگر (ngx_argv == پوچ) {
        بازگشت NGX_ERROR;
    }

    برای (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        اگر (ngx_argv[i] == پوچ) {
            بازگشت NGX_ERROR;
        }

        (تهی) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = پوچ;

#endif

    ngx_os_environ = environ;

    بازگشت NGX_OK;
}


ایستا ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    size_t   len;

    اگر (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        اگر (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            اگر (p == پوچ) {
                بازگشت NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

    } وگرنه {

#ifndef NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        اگر (p == پوچ) {
            بازگشت NGX_ERROR;
        }

        اگر (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            بازگشت NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#وگرنه

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#وگرنه
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#endif
    }

    اگر (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } وگرنه {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    اگر (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        بازگشت NGX_ERROR;
    }

    برای (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        اگر (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.data + 1;
            cycle->conf_prefix.data = cycle->conf_file.data;
            بشکن;
        }
    }

    اگر (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.data = ngx_error_log;

    } وگرنه {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    اگر (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    اگر (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    بازگشت NGX_OK;
}


ایستا تهی *
ngx_core_module_create_conf(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(cycle->pool, اندازه(ngx_core_conf_t));
    اگر (ccf == پوچ) {
        بازگشت پوچ;
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

    اگر (ngx_array_init(&ccf->env, cycle->pool, 1, اندازه(ngx_str_t))
        != NGX_OK)
    {
        بازگشت پوچ;
    }

    بازگشت ccf;
}


ایستا نویسه *
ngx_core_module_init_conf(ngx_cycle_t *cycle, تهی *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_مقدار(ccf->daemon, 1);
    ngx_conf_init_مقدار(ccf->master, 1);
    ngx_conf_init_msec_مقدار(ccf->timer_resolution, 0);
    ngx_conf_init_msec_مقدار(ccf->shutdown_timeout, 0);

    ngx_conf_init_مقدار(ccf->worker_processes, 1);
    ngx_conf_init_مقدار(ccf->debug_points, 0);

#اگر (NGX_HAVE_CPU_AFFINITY)

    اگر (!ccf->cpu_affinity_auto
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


    اگر (ccf->pid.len == 0) {
        ngx_str_set(&ccf->pid, NGX_PID_PATH);
    }

    اگر (ngx_conf_full_name(cycle, &ccf->pid, 0) != NGX_OK) {
        بازگشت NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + اندازه(NGX_OLDPID_EXT);

    ccf->oldpid.data = ngx_pnalloc(cycle->pool, ccf->oldpid.len);
    اگر (ccf->oldpid.data == پوچ) {
        بازگشت NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.data, ccf->pid.data, ccf->pid.len),
               NGX_OLDPID_EXT, اندازه(NGX_OLDPID_EXT));


    اگر (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        اگر (ngx_conf_full_name(cycle, &ccf->working_directory, 0) != NGX_OK) {
            بازگشت NGX_CONF_ERROR;
        }

        اگر (ngx_file_info(ccf->working_directory.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.data);
            بازگشت NGX_CONF_ERROR;
        }

        اگر (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.data);
            بازگشت NGX_CONF_ERROR;
        }
    }


#اگر !(NGX_WIN32)

    اگر (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        ساختار group   *grp;
        ساختار passwd  *pwd;

        ngx_set_errno(0);
        pwd = getpwnam(NGX_USER);
        اگر (pwd == پوچ) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            بازگشت NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_set_errno(0);
        grp = getgrnam(NGX_GROUP);
        اگر (grp == پوچ) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getgrnam(\"" NGX_GROUP "\") failed");
            بازگشت NGX_CONF_ERROR;
        }

        ccf->group = grp->gr_gid;
    }


    اگر (ccf->lock_file.len == 0) {
        ngx_str_set(&ccf->lock_file, NGX_LOCK_PATH);
    }

    اگر (ngx_conf_full_name(cycle, &ccf->lock_file, 0) != NGX_OK) {
        بازگشت NGX_CONF_ERROR;
    }

    {
    ngx_str_t  lock_file;

    lock_file = cycle->old_cycle->lock_file;

    اگر (lock_file.len) {
        lock_file.len--;

        اگر (ccf->lock_file.len != lock_file.len
            || ngx_strncmp(ccf->lock_file.data, lock_file.data, lock_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        cycle->lock_file.len = lock_file.len + 1;
        lock_file.len += اندازه(".accept");

        cycle->lock_file.data = ngx_pstrdup(cycle->pool, &lock_file);
        اگر (cycle->lock_file.data == پوچ) {
            بازگشت NGX_CONF_ERROR;
        }

    } وگرنه {
        cycle->lock_file.len = ccf->lock_file.len + 1;
        cycle->lock_file.data = ngx_pnalloc(cycle->pool,
                                      ccf->lock_file.len + اندازه(".accept"));
        اگر (cycle->lock_file.data == پوچ) {
            بازگشت NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(cycle->lock_file.data, ccf->lock_file.data,
                              ccf->lock_file.len),
                   ".accept", اندازه(".accept"));
    }
    }

#endif

    بازگشت NGX_CONF_OK;
}


ایستا نویسه *
ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
#اگر (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    بازگشت NGX_CONF_OK;

#وگرنه

    ngx_core_conf_t  *ccf = conf;

    نویسه             *group;
    ساختار passwd    *pwd;
    ساختار group     *grp;
    ngx_str_t        *مقدار;

    اگر (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        بازگشت "is duplicate";
    }

    اگر (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        بازگشت NGX_CONF_OK;
    }

    مقدار = cf->args->elts;

    ccf->username = (نویسه *) مقدار[1].data;

    ngx_set_errno(0);
    pwd = getpwnam((ثابت نویسه *) مقدار[1].data);
    اگر (pwd == پوچ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", مقدار[1].data);
        بازگشت NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    group = (نویسه *) ((cf->args->nelts == 2) ? مقدار[1].data : مقدار[2].data);

    ngx_set_errno(0);
    grp = getgrnam(group);
    اگر (grp == پوچ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", group);
        بازگشت NGX_CONF_ERROR;
    }

    ccf->group = grp->gr_gid;

    بازگشت NGX_CONF_OK;

#endif
}


ایستا نویسه *
ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *مقدار, *var;
    ngx_uint_t   i;

    var = ngx_array_افزودن(&ccf->env);
    اگر (var == پوچ) {
        بازگشت NGX_CONF_ERROR;
    }

    مقدار = cf->args->elts;
    *var = مقدار[1];

    برای (i = 0; i < مقدار[1].len; i++) {

        اگر (مقدار[1].data[i] == '=') {

            var->len = i;

            بازگشت NGX_CONF_OK;
        }
    }

    بازگشت NGX_CONF_OK;
}


ایستا نویسه *
ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *مقدار;
    ngx_uint_t        n, minus;

    اگر (ccf->priority != 0) {
        بازگشت "is duplicate";
    }

    مقدار = cf->args->elts;

    اگر (مقدار[1].data[0] == '-') {
        n = 1;
        minus = 1;

    } وگرنه اگر (مقدار[1].data[0] == '+') {
        n = 1;
        minus = 0;

    } وگرنه {
        n = 0;
        minus = 0;
    }

    ccf->priority = ngx_atoi(&مقدار[1].data[n], مقدار[1].len - n);
    اگر (ccf->priority == NGX_ERROR) {
        بازگشت "invalid number";
    }

    اگر (minus) {
        ccf->priority = -ccf->priority;
    }

    بازگشت NGX_CONF_OK;
}


ایستا نویسه *
ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
#اگر (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *مقدار;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *mask;

    اگر (ccf->cpu_affinity) {
        بازگشت "is duplicate";
    }

    mask = ngx_palloc(cf->pool, (cf->args->nelts - 1) * اندازه(ngx_cpuset_t));
    اگر (mask == پوچ) {
        بازگشت NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->args->nelts - 1;
    ccf->cpu_affinity = mask;

    مقدار = cf->args->elts;

    اگر (ngx_strcmp(مقدار[1].data, "auto") == 0) {

        اگر (cf->args->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            بازگشت NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&mask[0]);
        برای (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_SET(i, &mask[0]);
        }

        n = 2;

    } وگرنه {
        n = 1;
    }

    برای ( /* void */ ; n < cf->args->nelts; n++) {

        اگر (مقدار[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            بازگشت NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&mask[n - 1]);

        برای (p = مقدار[n].data + مقدار[n].len - 1;
             p >= مقدار[n].data;
             p--)
        {
            ch = *p;

            اگر (ch == ' ') {
                ادامه;
            }

            i++;

            اگر (ch == '0') {
                ادامه;
            }

            اگر (ch == '1') {
                CPU_SET(i - 1, &mask[n - 1]);
                ادامه;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            بازگشت NGX_CONF_ERROR;
        }
    }

#وگرنه

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#endif

    بازگشت NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#اگر (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *mask;
    ngx_core_conf_t  *ccf;

    ایستا ngx_cpuset_t  result;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_cycle->conf_ctx,
                                           ngx_core_module);

    اگر (ccf->cpu_affinity == پوچ) {
        بازگشت پوچ;
    }

    اگر (ccf->cpu_affinity_auto) {
        mask = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        برای (i = 0, j = n; /* void */ ; i++) {

            اگر (CPU_ISSET(i % CPU_SETSIZE, mask) && j-- == 0) {
                بشکن;
            }

            اگر (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                بازگشت پوچ;
            }

            /* void */
        }

        CPU_ZERO(&result);
        CPU_SET(i % CPU_SETSIZE, &result);

        بازگشت &result;
    }

    اگر (ccf->cpu_affinity_n > n) {
        بازگشت &ccf->cpu_affinity[n];
    }

    بازگشت &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#وگرنه

    بازگشت پوچ;

#endif
}


ایستا نویسه *
ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
    ngx_str_t        *مقدار;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    اگر (ccf->worker_processes != NGX_CONF_UNSET) {
        بازگشت "is duplicate";
    }

    مقدار = cf->args->elts;

    اگر (ngx_strcmp(مقدار[1].data, "auto") == 0) {
        ccf->worker_processes = ngx_ncpu;
        بازگشت NGX_CONF_OK;
    }

    ccf->worker_processes = ngx_atoi(مقدار[1].data, مقدار[1].len);

    اگر (ccf->worker_processes == NGX_ERROR) {
        بازگشت "invalid value";
    }

    بازگشت NGX_CONF_OK;
}


ایستا نویسه *
ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, تهی *conf)
{
#اگر (NGX_HAVE_DLOPEN)
    تهی                *handle;
    نویسه               **names, **order;
    ngx_str_t           *مقدار, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    اگر (cf->cycle->modules_used) {
        بازگشت "is specified too late";
    }

    مقدار = cf->args->elts;

    file = مقدار[1];

    اگر (ngx_conf_full_name(cf->cycle, &file, 0) != NGX_OK) {
        بازگشت NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_add(cf->cycle->pool, 0);
    اگر (cln == پوچ) {
        بازگشت NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.data);
    اگر (handle == پوچ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.data, ngx_dlerror());
        بازگشت NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->data = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    اگر (modules == پوچ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &مقدار[1], "ngx_modules", ngx_dlerror());
        بازگشت NGX_CONF_ERROR;
    }

    names = ngx_dlsym(handle, "ngx_module_names");
    اگر (names == پوچ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &مقدار[1], "ngx_module_names", ngx_dlerror());
        بازگشت NGX_CONF_ERROR;
    }

    order = ngx_dlsym(handle, "ngx_module_order");

    برای (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = names[i];

        اگر (ngx_add_module(cf, &file, module, order) != NGX_OK) {
            بازگشت NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0, "module: %s i:%ui",
                       module->name, module->index);
    }

    بازگشت NGX_CONF_OK;

#وگرنه

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    بازگشت NGX_CONF_ERROR;

#endif
}


#اگر (NGX_HAVE_DLOPEN)

ایستا تهی
ngx_unload_module(تهی *data)
{
    تهی  *handle = data;

    اگر (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#endif
