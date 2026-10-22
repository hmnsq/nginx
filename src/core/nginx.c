
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <nginx.h>


ساكن فراغ ngx_show_version_info(فراغ);
ساكن ngx_int_t ngx_add_inherited_sockets(ngx_cycle_t *cycle);
ساكن فراغ ngx_cleanup_environment(فراغ *data);
ساكن فراغ ngx_cleanup_environment_variable(فراغ *data);
ساكن ngx_int_t ngx_get_options(صحيح argc, حرف *ثابت *argv);
ساكن ngx_int_t ngx_process_options(ngx_cycle_t *cycle);
ساكن ngx_int_t ngx_save_argv(ngx_cycle_t *cycle, صحيح argc, حرف *ثابت *argv);
ساكن فراغ *ngx_core_module_create_conf(ngx_cycle_t *cycle);
ساكن حرف *ngx_core_module_init_conf(ngx_cycle_t *cycle, فراغ *conf);
ساكن حرف *ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
ساكن حرف *ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
ساكن حرف *ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd,
    فراغ *conf);
ساكن حرف *ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf);
#إذا (NGX_HAVE_DLOPEN)
ساكن فراغ ngx_unload_module(فراغ *data);
#endif


ساكن ngx_conf_enum_t  ngx_debug_points[] = {
    { ngx_string("stop"), NGX_DEBUG_POINTS_STOP },
    { ngx_string("abort"), NGX_DEBUG_POINTS_ABORT },
    { ngx_null_string, 0 }
};


ساكن ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, daemon),
      عدم },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, master),
      عدم },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      عدم },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, pid),
      عدم },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, lock_file),
      عدم },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_worker_processes,
      0,
      0,
      عدم },

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
      عدم },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_priority,
      0,
      0,
      عدم },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_1MORE,
      ngx_set_cpu_affinity,
      0,
      0,
      عدم },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_عدد_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      عدم },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_off_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      عدم },

    { ngx_string("worker_shutdown_timeout"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, shutdown_timeout),
      عدم },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      عدم },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_env,
      0,
      0,
      عدم },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_load_module,
      0,
      0,
      عدم },

      ngx_null_command
};


ساكن ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};


ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    عدم,                                  /* init master */
    عدم,                                  /* init module */
    عدم,                                  /* init process */
    عدم,                                  /* init thread */
    عدم,                                  /* exit thread */
    عدم,                                  /* exit process */
    عدم,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


ساكن ngx_uint_t   ngx_show_help;
ساكن ngx_uint_t   ngx_show_version;
ساكن ngx_uint_t   ngx_show_configure;
ساكن u_char      *ngx_prefix;
ساكن u_char      *ngx_error_log;
ساكن u_char      *ngx_conf_file;
ساكن u_char      *ngx_conf_params;
#إذا (NGX_CONTROL_API)
ساكن u_char      *ngx_control_addr;
#endif
ساكن حرف        *ngx_signal;


ساكن حرف **ngx_os_environ;


صحيح ngx_cdecl
main(صحيح argc, حرف *ثابت *argv)
{
    ngx_buf_t        *b;
    ngx_log_t        *log;
    ngx_uint_t        i;
    ngx_cycle_t      *cycle, init_cycle;
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init();

    إذا (ngx_strerror_init() != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_get_options(argc, argv) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_show_version) {
        ngx_show_version_info();

        إذا (!ngx_test_config) {
            أرجع 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#إذا (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    إذا (log == عدم) {
        أرجع 1;
    }

    /* STUB */
#إذا (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif

    /*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */

    ngx_memzero(&init_cycle, حجم(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    init_cycle.pool = ngx_create_pool(1024, log);
    إذا (init_cycle.pool == عدم) {
        أرجع 1;
    }

    إذا (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_process_options(&init_cycle) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_os_init(log) != NGX_OK) {
        أرجع 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    إذا (ngx_crc32_table_init() != NGX_OK) {
        أرجع 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    إذا (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        أرجع 1;
    }

#إذا (NGX_CONTROL_API)
    ngx_control_preinit();
#endif

    إذا (ngx_preinit_modules() != NGX_OK) {
        أرجع 1;
    }

    cycle = ngx_init_cycle(&init_cycle);
    إذا (cycle == عدم) {
        إذا (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        أرجع 1;
    }

    إذا (ngx_test_config) {
        إذا (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.data);
        }

        إذا (ngx_dump_config) {
            cd = cycle->config_dump.elts;

            لكل (i = 0; i < cycle->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (فراغ) ngx_write_fd(ngx_stdout, cd[i].name.data,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].buffer;

                (فراغ) ngx_write_fd(ngx_stdout, b->pos, b->last - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        أرجع 0;
    }

    إذا (ngx_signal) {
        أرجع ngx_signal_process(cycle, ngx_signal);
    }

    ngx_os_status(cycle->log);

    ngx_cycle = cycle;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    إذا (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#إذا !(NGX_WIN32)

    إذا (ngx_init_signals(cycle->log) != NGX_OK) {
        أرجع 1;
    }

    إذا (!ngx_inherited && ccf->daemon) {
        إذا (ngx_daemon(cycle->log) != NGX_OK) {
            أرجع 1;
        }

        ngx_daemonized = 1;
    }

    إذا (ngx_inherited) {
        ngx_daemonized = 1;
    }

#endif

#إذا (NGX_CONTROL_API)
    إذا (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "control API unavailable in single process mode");

    } وإلا {
        إذا (ngx_control_init(ngx_control_addr) != NGX_OK) {
            أرجع 1;
        }
    }
#endif

    إذا (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        أرجع 1;
    }

    إذا (ngx_log_redirect_stderr(cycle) != NGX_OK) {
        أرجع 1;
    }

    إذا (log->file->fd != ngx_stderr) {
        إذا (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    إذا (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } وإلا {
        ngx_master_process_cycle(cycle);
    }

    أرجع 0;
}


ساكن فراغ
ngx_show_version_info(فراغ)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    إذا (ngx_show_help) {
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
#إذا (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#endif
#ifdef NGX_PREFIX
            "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#وإلا
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
            "  -e filename   : set error log file (default: "
#ifdef NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#وإلا
                               NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#endif
            "  -c filename   : set configuration file (default: " NGX_CONF_PATH
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    إذا (ngx_show_configure) {

#ifdef NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#endif

#إذا (NGX_SSL)
        إذا (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } وإلا {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((حرف *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#وإلا
        ngx_write_stderr("TLS SNI support disabled" NGX_LINEFEED);
#endif
#endif

        ngx_write_stderr("configure arguments:" NGX_CONFIGURE NGX_LINEFEED);
    }
}


ساكن ngx_int_t
ngx_add_inherited_sockets(ngx_cycle_t *cycle)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    inherited = (u_char *) getenv(NGINX_VAR);

    إذا (inherited == عدم) {
        أرجع NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    إذا (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       حجم(ngx_listening_t))
        != NGX_OK)
    {
        أرجع NGX_ERROR;
    }

    لكل (p = inherited, v = p; *p; p++) {
        إذا (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            إذا (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                اكسر;
            }

            v = p + 1;

            ls = ngx_array_ڈالو(&cycle->listening);
            إذا (ls == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memzero(ls, حجم(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    إذا (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    أرجع ngx_set_inherited_sockets(cycle);
}


حرف **
ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last)
{
    حرف                **p, **env, *str;
    size_t                len;
    ngx_str_t            *var;
    ngx_uint_t            i, n;
    ngx_core_conf_t      *ccf;
    ngx_pool_cleanup_t   *cln;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    إذا (last == عدم && ccf->environment) {
        أرجع ccf->environment;
    }

    var = ccf->env.elts;

    لكل (i = 0; i < ccf->env.nelts; i++) {
        إذا (ngx_strcmp(var[i].data, "TZ") == 0
            || ngx_strncmp(var[i].data, "TZ=", 3) == 0)
        {
            اذهب tz_found;
        }
    }

    var = ngx_array_ڈالو(&ccf->env);
    إذا (var == عدم) {
        أرجع عدم;
    }

    var->len = 2;
    var->data = (u_char *) "TZ";

    var = ccf->env.elts;

tz_found:

    n = 0;

    لكل (i = 0; i < ccf->env.nelts; i++) {

        إذا (var[i].data[var[i].len] == '=') {
            n++;
            استمر;
        }

        لكل (p = ngx_os_environ; *p; p++) {

            إذا (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                اكسر;
            }
        }
    }

    إذا (last) {
        env = ngx_alloc((*last + n + 1) * حجم(حرف *), cycle->log);
        إذا (env == عدم) {
            أرجع عدم;
        }

        *last = n;

    } وإلا {
        cln = ngx_pool_cleanup_add(cycle->pool, 0);
        إذا (cln == عدم) {
            أرجع عدم;
        }

        env = ngx_alloc((n + 1) * حجم(حرف *), cycle->log);
        إذا (env == عدم) {
            أرجع عدم;
        }

        cln->handler = ngx_cleanup_environment;
        cln->data = env;
    }

    n = 0;

    لكل (i = 0; i < ccf->env.nelts; i++) {

        إذا (var[i].data[var[i].len] == '=') {

            إذا (last) {
                env[n++] = (حرف *) var[i].data;
                استمر;
            }

            cln = ngx_pool_cleanup_add(cycle->pool, 0);
            إذا (cln == عدم) {
                أرجع عدم;
            }

            len = ngx_strlen(var[i].data) + 1;

            str = ngx_alloc(len, cycle->log);
            إذا (str == عدم) {
                أرجع عدم;
            }

            ngx_memcpy(str, var[i].data, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->data = str;

            env[n++] = str;

            استمر;
        }

        لكل (p = ngx_os_environ; *p; p++) {

            إذا (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                اكسر;
            }
        }
    }

    env[n] = عدم;

    إذا (last == عدم) {
        ccf->environment = env;
        environ = env;
    }

    أرجع env;
}


ساكن فراغ
ngx_cleanup_environment(فراغ *data)
{
    حرف  **env = data;

    إذا (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        أرجع;
    }

    ngx_free(env);
}


ساكن فراغ
ngx_cleanup_environment_variable(فراغ *data)
{
    حرف  *var = data;

    حرف  **p;

    لكل (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        إذا (*p == var) {
            أرجع;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_ثنائی(ngx_cycle_t *cycle, حرف *ثابت *argv)
{
    حرف             **env, *var;
    u_char            *p;
    ngx_uint_t         i, n;
    ngx_pid_t          pid;
    ngx_exec_ctx_t     ctx;
    ngx_core_conf_t   *ccf;
    ngx_listening_t   *ls;

    ngx_memzero(&ctx, حجم(ngx_exec_ctx_t));

    ctx.path = argv[0];
    ctx.name = "new binary process";
    ctx.argv = argv;

    n = 3;
    env = ngx_set_environment(cycle, &n);
    إذا (env == عدم) {
        أرجع NGX_INVALID_PID;
    }

    var = ngx_alloc(حجم(NGINX_VAR)
                    + cycle->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    cycle->log);
    إذا (var == عدم) {
        ngx_free(env);
        أرجع NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", حجم(NGINX_VAR));

    ls = cycle->listening.elts;
    لكل (i = 0; i < cycle->listening.nelts; i++) {
        إذا (ls[i].ignore) {
            استمر;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#إذا (NGX_SETPROCTITLE_USES_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#endif

#إذا (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    إذا (env[n] != عدم) {
        n++;
    }
#endif

    env[n] = عدم;

#إذا (NGX_DEBUG)
    {
    حرف  **e;
    لكل (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0, "env: %s", *e);
    }
    }
#endif

    ctx.envp = (حرف *ثابت *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    إذا (ngx_rename_file(ccf->pid.data, ccf->oldpid.data) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.data, ccf->oldpid.data, argv[0]);

        ngx_free(env);
        ngx_free(var);

        أرجع NGX_INVALID_PID;
    }

    pid = ngx_execute(cycle, &ctx);

    إذا (pid == NGX_INVALID_PID) {
        إذا (ngx_rename_file(ccf->oldpid.data, ccf->pid.data)
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

    أرجع pid;
}


ساكن ngx_int_t
ngx_get_options(صحيح argc, حرف *ثابت *argv)
{
    u_char     *p;
    ngx_int_t   i;

    لكل (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        إذا (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            أرجع NGX_ERROR;
        }

        طالما (*p) {

            بدل (*p++) {

            حالة '?':
            حالة 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                اكسر;

            حالة 'v':
                ngx_show_version = 1;
                اكسر;

            حالة 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                اكسر;

            حالة 't':
                ngx_test_config = 1;
                اكسر;

            حالة 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                اكسر;

            حالة 'q':
                ngx_quiet_mode = 1;
                اكسر;

            حالة 'p':
                إذا (*p) {
                    ngx_prefix = p;
                    اذهب next;
                }

                إذا (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    اذهب next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                أرجع NGX_ERROR;

            حالة 'e':
                إذا (*p) {
                    ngx_error_log = p;

                } وإلا إذا (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } وإلا {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    أرجع NGX_ERROR;
                }

                إذا (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                اذهب next;

            حالة 'c':
                إذا (*p) {
                    ngx_conf_file = p;
                    اذهب next;
                }

                إذا (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    اذهب next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                أرجع NGX_ERROR;

            حالة 'g':
                إذا (*p) {
                    ngx_conf_params = p;
                    اذهب next;
                }

                إذا (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    اذهب next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                أرجع NGX_ERROR;

            حالة 's':
                إذا (*p) {
                    ngx_signal = (حرف *) p;

                } وإلا إذا (argv[++i]) {
                    ngx_signal = argv[i];

                } وإلا {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    أرجع NGX_ERROR;
                }

                إذا (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    اذهب next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                أرجع NGX_ERROR;

#إذا (NGX_CONTROL_API)
            حالة 'l':
                ngx_control_api_enabled = 1;

                إذا (*p) {
                    ngx_control_addr = p;
                    اذهب next;
                }

                إذا (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    اذهب next;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                أرجع NGX_ERROR;
#endif

            افتراضي:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                أرجع NGX_ERROR;
            }
        }

    next:

        استمر;
    }

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, صحيح argc, حرف *ثابت *argv)
{
#إذا (NGX_FREEBSD)

    ngx_os_argv = (حرف **) argv;
    ngx_argc = argc;
    ngx_argv = (حرف **) argv;

#وإلا
    size_t     len;
    ngx_int_t  i;

    ngx_os_argv = (حرف **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * حجم(حرف *), cycle->log);
    إذا (ngx_argv == عدم) {
        أرجع NGX_ERROR;
    }

    لكل (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        إذا (ngx_argv[i] == عدم) {
            أرجع NGX_ERROR;
        }

        (فراغ) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = عدم;

#endif

    ngx_os_environ = environ;

    أرجع NGX_OK;
}


ساكن ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    size_t   len;

    إذا (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        إذا (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            إذا (p == عدم) {
                أرجع NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

    } وإلا {

#ifndef NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        إذا (p == عدم) {
            أرجع NGX_ERROR;
        }

        إذا (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            أرجع NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#وإلا

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#وإلا
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#endif
    }

    إذا (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } وإلا {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    إذا (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        أرجع NGX_ERROR;
    }

    لكل (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        إذا (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.data + 1;
            cycle->conf_prefix.data = cycle->conf_file.data;
            اكسر;
        }
    }

    إذا (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.data = ngx_error_log;

    } وإلا {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    إذا (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    إذا (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    أرجع NGX_OK;
}


ساكن فراغ *
ngx_core_module_create_conf(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(cycle->pool, حجم(ngx_core_conf_t));
    إذا (ccf == عدم) {
        أرجع عدم;
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

    إذا (ngx_array_init(&ccf->env, cycle->pool, 1, حجم(ngx_str_t))
        != NGX_OK)
    {
        أرجع عدم;
    }

    أرجع ccf;
}


ساكن حرف *
ngx_core_module_init_conf(ngx_cycle_t *cycle, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_قدر(ccf->daemon, 1);
    ngx_conf_init_قدر(ccf->master, 1);
    ngx_conf_init_msec_قدر(ccf->timer_resolution, 0);
    ngx_conf_init_msec_قدر(ccf->shutdown_timeout, 0);

    ngx_conf_init_قدر(ccf->worker_processes, 1);
    ngx_conf_init_قدر(ccf->debug_points, 0);

#إذا (NGX_HAVE_CPU_AFFINITY)

    إذا (!ccf->cpu_affinity_auto
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


    إذا (ccf->pid.len == 0) {
        ngx_str_set(&ccf->pid, NGX_PID_PATH);
    }

    إذا (ngx_conf_full_name(cycle, &ccf->pid, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + حجم(NGX_OLDPID_EXT);

    ccf->oldpid.data = ngx_pnalloc(cycle->pool, ccf->oldpid.len);
    إذا (ccf->oldpid.data == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.data, ccf->pid.data, ccf->pid.len),
               NGX_OLDPID_EXT, حجم(NGX_OLDPID_EXT));


    إذا (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        إذا (ngx_conf_full_name(cycle, &ccf->working_directory, 0) != NGX_OK) {
            أرجع NGX_CONF_ERROR;
        }

        إذا (ngx_file_info(ccf->working_directory.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.data);
            أرجع NGX_CONF_ERROR;
        }

        إذا (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.data);
            أرجع NGX_CONF_ERROR;
        }
    }


#إذا !(NGX_WIN32)

    إذا (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        بنية group   *grp;
        بنية passwd  *pwd;

        ngx_set_errno(0);
        pwd = getpwnam(NGX_USER);
        إذا (pwd == عدم) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            أرجع NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_set_errno(0);
        grp = getgrnam(NGX_GROUP);
        إذا (grp == عدم) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getgrnam(\"" NGX_GROUP "\") failed");
            أرجع NGX_CONF_ERROR;
        }

        ccf->group = grp->gr_gid;
    }


    إذا (ccf->lock_file.len == 0) {
        ngx_str_set(&ccf->lock_file, NGX_LOCK_PATH);
    }

    إذا (ngx_conf_full_name(cycle, &ccf->lock_file, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    {
    ngx_str_t  lock_file;

    lock_file = cycle->old_cycle->lock_file;

    إذا (lock_file.len) {
        lock_file.len--;

        إذا (ccf->lock_file.len != lock_file.len
            || ngx_strncmp(ccf->lock_file.data, lock_file.data, lock_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        cycle->lock_file.len = lock_file.len + 1;
        lock_file.len += حجم(".accept");

        cycle->lock_file.data = ngx_pstrdup(cycle->pool, &lock_file);
        إذا (cycle->lock_file.data == عدم) {
            أرجع NGX_CONF_ERROR;
        }

    } وإلا {
        cycle->lock_file.len = ccf->lock_file.len + 1;
        cycle->lock_file.data = ngx_pnalloc(cycle->pool,
                                      ccf->lock_file.len + حجم(".accept"));
        إذا (cycle->lock_file.data == عدم) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(cycle->lock_file.data, ccf->lock_file.data,
                              ccf->lock_file.len),
                   ".accept", حجم(".accept"));
    }
    }

#endif

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    أرجع NGX_CONF_OK;

#وإلا

    ngx_core_conf_t  *ccf = conf;

    حرف             *group;
    بنية passwd    *pwd;
    بنية group     *grp;
    ngx_str_t        *قدر;

    إذا (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        أرجع "is duplicate";
    }

    إذا (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        أرجع NGX_CONF_OK;
    }

    قدر = cf->args->elts;

    ccf->username = (حرف *) قدر[1].data;

    ngx_set_errno(0);
    pwd = getpwnam((ثابت حرف *) قدر[1].data);
    إذا (pwd == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", قدر[1].data);
        أرجع NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    group = (حرف *) ((cf->args->nelts == 2) ? قدر[1].data : قدر[2].data);

    ngx_set_errno(0);
    grp = getgrnam(group);
    إذا (grp == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", group);
        أرجع NGX_CONF_ERROR;
    }

    ccf->group = grp->gr_gid;

    أرجع NGX_CONF_OK;

#endif
}


ساكن حرف *
ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *قدر, *var;
    ngx_uint_t   i;

    var = ngx_array_ڈالو(&ccf->env);
    إذا (var == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    قدر = cf->args->elts;
    *var = قدر[1];

    لكل (i = 0; i < قدر[1].len; i++) {

        إذا (قدر[1].data[i] == '=') {

            var->len = i;

            أرجع NGX_CONF_OK;
        }
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *قدر;
    ngx_uint_t        n, minus;

    إذا (ccf->priority != 0) {
        أرجع "is duplicate";
    }

    قدر = cf->args->elts;

    إذا (قدر[1].data[0] == '-') {
        n = 1;
        minus = 1;

    } وإلا إذا (قدر[1].data[0] == '+') {
        n = 1;
        minus = 0;

    } وإلا {
        n = 0;
        minus = 0;
    }

    ccf->priority = ngx_atoi(&قدر[1].data[n], قدر[1].len - n);
    إذا (ccf->priority == NGX_ERROR) {
        أرجع "invalid number";
    }

    إذا (minus) {
        ccf->priority = -ccf->priority;
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *قدر;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *mask;

    إذا (ccf->cpu_affinity) {
        أرجع "is duplicate";
    }

    mask = ngx_palloc(cf->pool, (cf->args->nelts - 1) * حجم(ngx_cpuset_t));
    إذا (mask == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->args->nelts - 1;
    ccf->cpu_affinity = mask;

    قدر = cf->args->elts;

    إذا (ngx_strcmp(قدر[1].data, "auto") == 0) {

        إذا (cf->args->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            أرجع NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&mask[0]);
        لكل (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_SET(i, &mask[0]);
        }

        n = 2;

    } وإلا {
        n = 1;
    }

    لكل ( /* void */ ; n < cf->args->nelts; n++) {

        إذا (قدر[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            أرجع NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&mask[n - 1]);

        لكل (p = قدر[n].data + قدر[n].len - 1;
             p >= قدر[n].data;
             p--)
        {
            ch = *p;

            إذا (ch == ' ') {
                استمر;
            }

            i++;

            إذا (ch == '0') {
                استمر;
            }

            إذا (ch == '1') {
                CPU_SET(i - 1, &mask[n - 1]);
                استمر;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            أرجع NGX_CONF_ERROR;
        }
    }

#وإلا

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#endif

    أرجع NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#إذا (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *mask;
    ngx_core_conf_t  *ccf;

    ساكن ngx_cpuset_t  result;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_cycle->conf_ctx,
                                           ngx_core_module);

    إذا (ccf->cpu_affinity == عدم) {
        أرجع عدم;
    }

    إذا (ccf->cpu_affinity_auto) {
        mask = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        لكل (i = 0, j = n; /* void */ ; i++) {

            إذا (CPU_ISSET(i % CPU_SETSIZE, mask) && j-- == 0) {
                اكسر;
            }

            إذا (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                أرجع عدم;
            }

            /* void */
        }

        CPU_ZERO(&result);
        CPU_SET(i % CPU_SETSIZE, &result);

        أرجع &result;
    }

    إذا (ccf->cpu_affinity_n > n) {
        أرجع &ccf->cpu_affinity[n];
    }

    أرجع &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#وإلا

    أرجع عدم;

#endif
}


ساكن حرف *
ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_str_t        *قدر;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    إذا (ccf->worker_processes != NGX_CONF_UNSET) {
        أرجع "is duplicate";
    }

    قدر = cf->args->elts;

    إذا (ngx_strcmp(قدر[1].data, "auto") == 0) {
        ccf->worker_processes = ngx_ncpu;
        أرجع NGX_CONF_OK;
    }

    ccf->worker_processes = ngx_atoi(قدر[1].data, قدر[1].len);

    إذا (ccf->worker_processes == NGX_ERROR) {
        أرجع "invalid value";
    }

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#إذا (NGX_HAVE_DLOPEN)
    فراغ                *handle;
    حرف               **names, **order;
    ngx_str_t           *قدر, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    إذا (cf->cycle->modules_used) {
        أرجع "is specified too late";
    }

    قدر = cf->args->elts;

    file = قدر[1];

    إذا (ngx_conf_full_name(cf->cycle, &file, 0) != NGX_OK) {
        أرجع NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_add(cf->cycle->pool, 0);
    إذا (cln == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.data);
    إذا (handle == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.data, ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->data = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    إذا (modules == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &قدر[1], "ngx_modules", ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    names = ngx_dlsym(handle, "ngx_module_names");
    إذا (names == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &قدر[1], "ngx_module_names", ngx_dlerror());
        أرجع NGX_CONF_ERROR;
    }

    order = ngx_dlsym(handle, "ngx_module_order");

    لكل (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = names[i];

        إذا (ngx_add_module(cf, &file, module, order) != NGX_OK) {
            أرجع NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0, "module: %s i:%ui",
                       module->name, module->index);
    }

    أرجع NGX_CONF_OK;

#وإلا

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    أرجع NGX_CONF_ERROR;

#endif
}


#إذا (NGX_HAVE_DLOPEN)

ساكن فراغ
ngx_unload_module(فراغ *data)
{
    فراغ  *handle = data;

    إذا (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#endif
