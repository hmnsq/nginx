
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
#אם (NGX_HAVE_DLOPEN)
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
      ngx_conf_set_num_slot,
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
#אם (NGX_CONTROL_API)
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

    אם (ngx_strerror_init() != NGX_OK) {
        החזר 1;
    }

    אם (ngx_get_options(argc, argv) != NGX_OK) {
        החזר 1;
    }

    אם (ngx_show_version) {
        ngx_show_version_info();

        אם (!ngx_test_config) {
            החזר 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init();

#אם (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid();
    ngx_parent = ngx_getppid();

    log = ngx_log_init(ngx_prefix, ngx_error_log);
    אם (log == عدم) {
        החזר 1;
    }

    /* STUB */
#אם (NGX_OPENSSL)
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
    אם (init_cycle.pool == عدم) {
        החזר 1;
    }

    אם (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        החזר 1;
    }

    אם (ngx_process_options(&init_cycle) != NGX_OK) {
        החזר 1;
    }

    אם (ngx_os_init(log) != NGX_OK) {
        החזר 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    אם (ngx_crc32_table_init() != NGX_OK) {
        החזר 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init();

    אם (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        החזר 1;
    }

#אם (NGX_CONTROL_API)
    ngx_control_preinit();
#endif

    אם (ngx_preinit_modules() != NGX_OK) {
        החזר 1;
    }

    cycle = ngx_init_cycle(&init_cycle);
    אם (cycle == عدم) {
        אם (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        החזר 1;
    }

    אם (ngx_test_config) {
        אם (!ngx_quiet_mode) {
            ngx_log_stderr(0, "configuration file %s test is successful",
                           cycle->conf_file.data);
        }

        אם (ngx_dump_config) {
            cd = cycle->config_dump.elts;

            עבור (i = 0; i < cycle->config_dump.nelts; i++) {

                ngx_write_stdout("# configuration file ");
                (فراغ) ngx_write_fd(ngx_stdout, cd[i].name.data,
                                    cd[i].name.len);
                ngx_write_stdout(":" NGX_LINEFEED);

                b = cd[i].buffer;

                (فراغ) ngx_write_fd(ngx_stdout, b->pos, b->last - b->pos);
                ngx_write_stdout(NGX_LINEFEED);
            }
        }

        החזר 0;
    }

    אם (ngx_signal) {
        החזר ngx_signal_process(cycle, ngx_signal);
    }

    ngx_os_status(cycle->log);

    ngx_cycle = cycle;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    אם (ccf->master && ngx_process == NGX_PROCESS_SINGLE) {
        ngx_process = NGX_PROCESS_MASTER;
    }

#אם !(NGX_WIN32)

    אם (ngx_init_signals(cycle->log) != NGX_OK) {
        החזר 1;
    }

    אם (!ngx_inherited && ccf->daemon) {
        אם (ngx_daemon(cycle->log) != NGX_OK) {
            החזר 1;
        }

        ngx_daemonized = 1;
    }

    אם (ngx_inherited) {
        ngx_daemonized = 1;
    }

#endif

#אם (NGX_CONTROL_API)
    אם (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                      "control API unavailable in single process mode");

    } אחרת {
        אם (ngx_control_init(ngx_control_addr) != NGX_OK) {
            החזר 1;
        }
    }
#endif

    אם (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        החזר 1;
    }

    אם (ngx_log_redirect_stderr(cycle) != NGX_OK) {
        החזר 1;
    }

    אם (log->file->fd != ngx_stderr) {
        אם (ngx_close_file(log->file->fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_close_file_n " built-in log failed");
        }
    }

    ngx_use_stderr = 0;

    אם (ngx_process == NGX_PROCESS_SINGLE) {
        ngx_single_process_cycle(cycle);

    } אחרת {
        ngx_master_process_cycle(cycle);
    }

    החזר 0;
}


ساكن فراغ
ngx_show_version_info(فراغ)
{
    ngx_write_stderr("nginx version: " NGINX_VER_BUILD NGX_LINEFEED);

    אם (ngx_show_help) {
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
#אם (NGX_CONTROL_API)
            "  -l addr       : open control socket on address" NGX_LINEFEED
#endif
#ifdef NGX_PREFIX
            "  -p prefix     : set prefix path (default: " NGX_PREFIX ")"
                               NGX_LINEFEED
#אחרת
            "  -p prefix     : set prefix path (default: NONE)" NGX_LINEFEED
#endif
            "  -e filename   : set error log file (default: "
#ifdef NGX_ERROR_LOG_STDERR
                               "stderr)" NGX_LINEFEED
#אחרת
                               NGX_ERROR_LOG_PATH ")" NGX_LINEFEED
#endif
            "  -c filename   : set configuration file (default: " NGX_CONF_PATH
                               ")" NGX_LINEFEED
            "  -g directives : set global directives out of configuration "
                               "file" NGX_LINEFEED NGX_LINEFEED
        );
    }

    אם (ngx_show_configure) {

#ifdef NGX_COMPILER
        ngx_write_stderr("built by " NGX_COMPILER NGX_LINEFEED);
#endif

#אם (NGX_SSL)
        אם (ngx_strcmp(ngx_ssl_version(), OPENSSL_VERSION_TEXT) == 0) {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT NGX_LINEFEED);
        } אחרת {
            ngx_write_stderr("built with " OPENSSL_VERSION_TEXT
                             " (running with ");
            ngx_write_stderr((حرف *) (uintptr_t) ngx_ssl_version());
            ngx_write_stderr(")" NGX_LINEFEED);
        }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
        ngx_write_stderr("TLS SNI support enabled" NGX_LINEFEED);
#אחרת
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

    אם (inherited == عدم) {
        החזר NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    אם (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       حجم(ngx_listening_t))
        != NGX_OK)
    {
        החזר NGX_ERROR;
    }

    עבור (p = inherited, v = p; *p; p++) {
        אם (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            אם (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                اكسر;
            }

            v = p + 1;

            ls = ngx_array_push(&cycle->listening);
            אם (ls == عدم) {
                החזר NGX_ERROR;
            }

            ngx_memzero(ls, حجم(ngx_listening_t));

            ls->fd = (ngx_socket_t) s;
            ls->inherited = 1;
        }
    }

    אם (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    ngx_inherited = 1;

    החזר ngx_set_inherited_sockets(cycle);
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

    אם (last == عدم && ccf->environment) {
        החזר ccf->environment;
    }

    var = ccf->env.elts;

    עבור (i = 0; i < ccf->env.nelts; i++) {
        אם (ngx_strcmp(var[i].data, "TZ") == 0
            || ngx_strncmp(var[i].data, "TZ=", 3) == 0)
        {
            اذهب tz_found;
        }
    }

    var = ngx_array_push(&ccf->env);
    אם (var == عدم) {
        החזר عدم;
    }

    var->len = 2;
    var->data = (u_char *) "TZ";

    var = ccf->env.elts;

tz_found:

    n = 0;

    עבור (i = 0; i < ccf->env.nelts; i++) {

        אם (var[i].data[var[i].len] == '=') {
            n++;
            استمر;
        }

        עבור (p = ngx_os_environ; *p; p++) {

            אם (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                n++;
                اكسر;
            }
        }
    }

    אם (last) {
        env = ngx_alloc((*last + n + 1) * حجم(حرف *), cycle->log);
        אם (env == عدم) {
            החזר عدم;
        }

        *last = n;

    } אחרת {
        cln = ngx_pool_cleanup_add(cycle->pool, 0);
        אם (cln == عدم) {
            החזר عدم;
        }

        env = ngx_alloc((n + 1) * حجم(حرف *), cycle->log);
        אם (env == عدم) {
            החזר عدم;
        }

        cln->handler = ngx_cleanup_environment;
        cln->data = env;
    }

    n = 0;

    עבור (i = 0; i < ccf->env.nelts; i++) {

        אם (var[i].data[var[i].len] == '=') {

            אם (last) {
                env[n++] = (حرف *) var[i].data;
                استمر;
            }

            cln = ngx_pool_cleanup_add(cycle->pool, 0);
            אם (cln == عدم) {
                החזר عدم;
            }

            len = ngx_strlen(var[i].data) + 1;

            str = ngx_alloc(len, cycle->log);
            אם (str == عدم) {
                החזר عدم;
            }

            ngx_memcpy(str, var[i].data, len);

            cln->handler = ngx_cleanup_environment_variable;
            cln->data = str;

            env[n++] = str;

            استمر;
        }

        עבור (p = ngx_os_environ; *p; p++) {

            אם (ngx_strncmp(*p, var[i].data, var[i].len) == 0
                && (*p)[var[i].len] == '=')
            {
                env[n++] = *p;
                اكسر;
            }
        }
    }

    env[n] = عدم;

    אם (last == عدم) {
        ccf->environment = env;
        environ = env;
    }

    החזר env;
}


ساكن فراغ
ngx_cleanup_environment(فراغ *data)
{
    حرف  **env = data;

    אם (environ == env) {

        /*
         * if the environment is still used, as it happens on exit,
         * the only option is to leak it
         */

        החזר;
    }

    ngx_free(env);
}


ساكن فراغ
ngx_cleanup_environment_variable(فراغ *data)
{
    حرف  *var = data;

    حرف  **p;

    עבור (p = environ; *p; p++) {

        /*
         * if an environment variable is still used, as it happens on exit,
         * the only option is to leak it
         */

        אם (*p == var) {
            החזר;
        }
    }

    ngx_free(var);
}


ngx_pid_t
ngx_exec_new_binary(ngx_cycle_t *cycle, حرف *ثابت *argv)
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
    אם (env == عدم) {
        החזר NGX_INVALID_PID;
    }

    var = ngx_alloc(حجم(NGINX_VAR)
                    + cycle->listening.nelts * (NGX_INT32_LEN + 1) + 2,
                    cycle->log);
    אם (var == عدم) {
        ngx_free(env);
        החזר NGX_INVALID_PID;
    }

    p = ngx_cpymem(var, NGINX_VAR "=", حجم(NGINX_VAR));

    ls = cycle->listening.elts;
    עבור (i = 0; i < cycle->listening.nelts; i++) {
        אם (ls[i].ignore) {
            استمر;
        }
        p = ngx_sprintf(p, "%ud;", ls[i].fd);
    }

    *p = '\0';

    env[n++] = var;

#אם (NGX_SETPROCTITLE_USES_ENV)

    /* allocate the spare 300 bytes for the new binary process title */

    env[n++] = "SPARE=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#endif

#אם (NGX_CONTROL_API)
    env[n] = ngx_control_handoff();

    אם (env[n] != عدم) {
        n++;
    }
#endif

    env[n] = عدم;

#אם (NGX_DEBUG)
    {
    حرف  **e;
    עבור (e = env; *e; e++) {
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0, "env: %s", *e);
    }
    }
#endif

    ctx.envp = (حرف *ثابت *) env;

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    אם (ngx_rename_file(ccf->pid.data, ccf->oldpid.data) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_rename_file_n " %s to %s failed "
                      "before executing new binary process \"%s\"",
                      ccf->pid.data, ccf->oldpid.data, argv[0]);

        ngx_free(env);
        ngx_free(var);

        החזר NGX_INVALID_PID;
    }

    pid = ngx_execute(cycle, &ctx);

    אם (pid == NGX_INVALID_PID) {
        אם (ngx_rename_file(ccf->oldpid.data, ccf->pid.data)
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

    החזר pid;
}


ساكن ngx_int_t
ngx_get_options(صحيح argc, حرف *ثابت *argv)
{
    u_char     *p;
    ngx_int_t   i;

    עבור (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        אם (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            החזר NGX_ERROR;
        }

        כלעוד (*p) {

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
                אם (*p) {
                    ngx_prefix = p;
                    اذهب next;
                }

                אם (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    اذهب next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                החזר NGX_ERROR;

            حالة 'e':
                אם (*p) {
                    ngx_error_log = p;

                } אחרת אם (argv[++i]) {
                    ngx_error_log = (u_char *) argv[i];

                } אחרת {
                    ngx_log_stderr(0, "option \"-e\" requires file name");
                    החזר NGX_ERROR;
                }

                אם (ngx_strcmp(ngx_error_log, "stderr") == 0) {
                    ngx_error_log = (u_char *) "";
                }

                اذهب next;

            حالة 'c':
                אם (*p) {
                    ngx_conf_file = p;
                    اذهب next;
                }

                אם (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    اذهب next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                החזר NGX_ERROR;

            حالة 'g':
                אם (*p) {
                    ngx_conf_params = p;
                    اذهب next;
                }

                אם (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    اذهب next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                החזר NGX_ERROR;

            حالة 's':
                אם (*p) {
                    ngx_signal = (حرف *) p;

                } אחרת אם (argv[++i]) {
                    ngx_signal = argv[i];

                } אחרת {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    החזר NGX_ERROR;
                }

                אם (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    اذهب next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                החזר NGX_ERROR;

#אם (NGX_CONTROL_API)
            حالة 'l':
                ngx_control_api_enabled = 1;

                אם (*p) {
                    ngx_control_addr = p;
                    اذهب next;
                }

                אם (argv[++i]) {
                    ngx_control_addr = (u_char *) argv[i];
                    اذهب next;
                }

                ngx_log_stderr(0, "option \"-l\" requires listen address");
                החזר NGX_ERROR;
#endif

            افتراضي:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                החזר NGX_ERROR;
            }
        }

    next:

        استمر;
    }

    החזר NGX_OK;
}


ساكن ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, صحيح argc, حرف *ثابت *argv)
{
#אם (NGX_FREEBSD)

    ngx_os_argv = (حرف **) argv;
    ngx_argc = argc;
    ngx_argv = (حرف **) argv;

#אחרת
    size_t     len;
    ngx_int_t  i;

    ngx_os_argv = (حرف **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * حجم(حرف *), cycle->log);
    אם (ngx_argv == عدم) {
        החזר NGX_ERROR;
    }

    עבור (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        אם (ngx_argv[i] == عدم) {
            החזר NGX_ERROR;
        }

        (فراغ) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = عدم;

#endif

    ngx_os_environ = environ;

    החזר NGX_OK;
}


ساكن ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    size_t   len;

    אם (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        אם (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            אם (p == عدم) {
                החזר NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

    } אחרת {

#ifndef NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        אם (p == عدم) {
            החזר NGX_ERROR;
        }

        אם (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            החזר NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#אחרת

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#אחרת
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#endif
    }

    אם (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } אחרת {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    אם (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        החזר NGX_ERROR;
    }

    עבור (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        אם (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - cycle->conf_file.data + 1;
            cycle->conf_prefix.data = cycle->conf_file.data;
            اكسر;
        }
    }

    אם (ngx_error_log) {
        cycle->error_log.len = ngx_strlen(ngx_error_log);
        cycle->error_log.data = ngx_error_log;

    } אחרת {
        ngx_str_set(&cycle->error_log, NGX_ERROR_LOG_PATH);
    }

    אם (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    אם (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    החזר NGX_OK;
}


ساكن فراغ *
ngx_core_module_create_conf(ngx_cycle_t *cycle)
{
    ngx_core_conf_t  *ccf;

    ccf = ngx_pcalloc(cycle->pool, حجم(ngx_core_conf_t));
    אם (ccf == عدم) {
        החזר عدم;
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

    אם (ngx_array_init(&ccf->env, cycle->pool, 1, حجم(ngx_str_t))
        != NGX_OK)
    {
        החזר عدم;
    }

    החזר ccf;
}


ساكن حرف *
ngx_core_module_init_conf(ngx_cycle_t *cycle, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_conf_init_value(ccf->daemon, 1);
    ngx_conf_init_value(ccf->master, 1);
    ngx_conf_init_msec_value(ccf->timer_resolution, 0);
    ngx_conf_init_msec_value(ccf->shutdown_timeout, 0);

    ngx_conf_init_value(ccf->worker_processes, 1);
    ngx_conf_init_value(ccf->debug_points, 0);

#אם (NGX_HAVE_CPU_AFFINITY)

    אם (!ccf->cpu_affinity_auto
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


    אם (ccf->pid.len == 0) {
        ngx_str_set(&ccf->pid, NGX_PID_PATH);
    }

    אם (ngx_conf_full_name(cycle, &ccf->pid, 0) != NGX_OK) {
        החזר NGX_CONF_ERROR;
    }

    ccf->oldpid.len = ccf->pid.len + حجم(NGX_OLDPID_EXT);

    ccf->oldpid.data = ngx_pnalloc(cycle->pool, ccf->oldpid.len);
    אם (ccf->oldpid.data == عدم) {
        החזר NGX_CONF_ERROR;
    }

    ngx_memcpy(ngx_cpymem(ccf->oldpid.data, ccf->pid.data, ccf->pid.len),
               NGX_OLDPID_EXT, حجم(NGX_OLDPID_EXT));


    אם (ccf->working_directory.len) {
        ngx_file_info_t  fi;

        אם (ngx_conf_full_name(cycle, &ccf->working_directory, 0) != NGX_OK) {
            החזר NGX_CONF_ERROR;
        }

        אם (ngx_file_info(ccf->working_directory.data, &fi) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          ngx_file_info_n " \"%s\" failed",
                          ccf->working_directory.data);
            החזר NGX_CONF_ERROR;
        }

        אם (!ngx_is_dir(&fi)) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"%s\" is not a directory",
                          ccf->working_directory.data);
            החזר NGX_CONF_ERROR;
        }
    }


#אם !(NGX_WIN32)

    אם (ccf->user == (uid_t) NGX_CONF_UNSET_UINT && geteuid() == 0) {
        بنية group   *grp;
        بنية passwd  *pwd;

        ngx_set_errno(0);
        pwd = getpwnam(NGX_USER);
        אם (pwd == عدم) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getpwnam(\"" NGX_USER "\") failed");
            החזר NGX_CONF_ERROR;
        }

        ccf->username = NGX_USER;
        ccf->user = pwd->pw_uid;

        ngx_set_errno(0);
        grp = getgrnam(NGX_GROUP);
        אם (grp == عدم) {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_errno,
                          "getgrnam(\"" NGX_GROUP "\") failed");
            החזר NGX_CONF_ERROR;
        }

        ccf->group = grp->gr_gid;
    }


    אם (ccf->lock_file.len == 0) {
        ngx_str_set(&ccf->lock_file, NGX_LOCK_PATH);
    }

    אם (ngx_conf_full_name(cycle, &ccf->lock_file, 0) != NGX_OK) {
        החזר NGX_CONF_ERROR;
    }

    {
    ngx_str_t  lock_file;

    lock_file = cycle->old_cycle->lock_file;

    אם (lock_file.len) {
        lock_file.len--;

        אם (ccf->lock_file.len != lock_file.len
            || ngx_strncmp(ccf->lock_file.data, lock_file.data, lock_file.len)
               != 0)
        {
            ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                          "\"lock_file\" could not be changed, ignored");
        }

        cycle->lock_file.len = lock_file.len + 1;
        lock_file.len += حجم(".accept");

        cycle->lock_file.data = ngx_pstrdup(cycle->pool, &lock_file);
        אם (cycle->lock_file.data == عدم) {
            החזר NGX_CONF_ERROR;
        }

    } אחרת {
        cycle->lock_file.len = ccf->lock_file.len + 1;
        cycle->lock_file.data = ngx_pnalloc(cycle->pool,
                                      ccf->lock_file.len + حجم(".accept"));
        אם (cycle->lock_file.data == عدم) {
            החזר NGX_CONF_ERROR;
        }

        ngx_memcpy(ngx_cpymem(cycle->lock_file.data, ccf->lock_file.data,
                              ccf->lock_file.len),
                   ".accept", حجم(".accept"));
    }
    }

#endif

    החזר NGX_CONF_OK;
}


ساكن حرف *
ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#אם (NGX_WIN32)

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"user\" is not supported, ignored");

    החזר NGX_CONF_OK;

#אחרת

    ngx_core_conf_t  *ccf = conf;

    حرف             *group;
    بنية passwd    *pwd;
    بنية group     *grp;
    ngx_str_t        *value;

    אם (ccf->user != (uid_t) NGX_CONF_UNSET_UINT) {
        החזר "is duplicate";
    }

    אם (geteuid() != 0) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "the \"user\" directive makes sense only "
                           "if the master process runs "
                           "with super-user privileges, ignored");
        החזר NGX_CONF_OK;
    }

    value = cf->args->elts;

    ccf->username = (حرف *) value[1].data;

    ngx_set_errno(0);
    pwd = getpwnam((ثابت حرف *) value[1].data);
    אם (pwd == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getpwnam(\"%s\") failed", value[1].data);
        החזר NGX_CONF_ERROR;
    }

    ccf->user = pwd->pw_uid;

    group = (حرف *) ((cf->args->nelts == 2) ? value[1].data : value[2].data);

    ngx_set_errno(0);
    grp = getgrnam(group);
    אם (grp == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                           "getgrnam(\"%s\") failed", group);
        החזר NGX_CONF_ERROR;
    }

    ccf->group = grp->gr_gid;

    החזר NGX_CONF_OK;

#endif
}


ساكن حرف *
ngx_set_env(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t   *value, *var;
    ngx_uint_t   i;

    var = ngx_array_push(&ccf->env);
    אם (var == عدم) {
        החזר NGX_CONF_ERROR;
    }

    value = cf->args->elts;
    *var = value[1];

    עבור (i = 0; i < value[1].len; i++) {

        אם (value[1].data[i] == '=') {

            var->len = i;

            החזר NGX_CONF_OK;
        }
    }

    החזר NGX_CONF_OK;
}


ساكن حرف *
ngx_set_priority(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_core_conf_t  *ccf = conf;

    ngx_str_t        *value;
    ngx_uint_t        n, minus;

    אם (ccf->priority != 0) {
        החזר "is duplicate";
    }

    value = cf->args->elts;

    אם (value[1].data[0] == '-') {
        n = 1;
        minus = 1;

    } אחרת אם (value[1].data[0] == '+') {
        n = 1;
        minus = 0;

    } אחרת {
        n = 0;
        minus = 0;
    }

    ccf->priority = ngx_atoi(&value[1].data[n], value[1].len - n);
    אם (ccf->priority == NGX_ERROR) {
        החזר "invalid number";
    }

    אם (minus) {
        ccf->priority = -ccf->priority;
    }

    החזר NGX_CONF_OK;
}


ساكن حرف *
ngx_set_cpu_affinity(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#אם (NGX_HAVE_CPU_AFFINITY)
    ngx_core_conf_t  *ccf = conf;

    u_char            ch, *p;
    ngx_str_t        *value;
    ngx_uint_t        i, n;
    ngx_cpuset_t     *mask;

    אם (ccf->cpu_affinity) {
        החזר "is duplicate";
    }

    mask = ngx_palloc(cf->pool, (cf->args->nelts - 1) * حجم(ngx_cpuset_t));
    אם (mask == عدم) {
        החזר NGX_CONF_ERROR;
    }

    ccf->cpu_affinity_n = cf->args->nelts - 1;
    ccf->cpu_affinity = mask;

    value = cf->args->elts;

    אם (ngx_strcmp(value[1].data, "auto") == 0) {

        אם (cf->args->nelts > 3) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "invalid number of arguments in "
                               "\"worker_cpu_affinity\" directive");
            החזר NGX_CONF_ERROR;
        }

        ccf->cpu_affinity_auto = 1;

        CPU_ZERO(&mask[0]);
        עבור (i = 0; i < (ngx_uint_t) ngx_min(ngx_ncpu, CPU_SETSIZE); i++) {
            CPU_SET(i, &mask[0]);
        }

        n = 2;

    } אחרת {
        n = 1;
    }

    עבור ( /* void */ ; n < cf->args->nelts; n++) {

        אם (value[n].len > CPU_SETSIZE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                         "\"worker_cpu_affinity\" supports up to %d CPUs only",
                         CPU_SETSIZE);
            החזר NGX_CONF_ERROR;
        }

        i = 0;
        CPU_ZERO(&mask[n - 1]);

        עבור (p = value[n].data + value[n].len - 1;
             p >= value[n].data;
             p--)
        {
            ch = *p;

            אם (ch == ' ') {
                استمر;
            }

            i++;

            אם (ch == '0') {
                استمر;
            }

            אם (ch == '1') {
                CPU_SET(i - 1, &mask[n - 1]);
                استمر;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                          "invalid character \"%c\" in \"worker_cpu_affinity\"",
                          ch);
            החזר NGX_CONF_ERROR;
        }
    }

#אחרת

    ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                       "\"worker_cpu_affinity\" is not supported "
                       "on this platform, ignored");
#endif

    החזר NGX_CONF_OK;
}


ngx_cpuset_t *
ngx_get_cpu_affinity(ngx_uint_t n)
{
#אם (NGX_HAVE_CPU_AFFINITY)
    ngx_uint_t        i, j;
    ngx_cpuset_t     *mask;
    ngx_core_conf_t  *ccf;

    ساكن ngx_cpuset_t  result;

    ccf = (ngx_core_conf_t *) ngx_get_conf(ngx_cycle->conf_ctx,
                                           ngx_core_module);

    אם (ccf->cpu_affinity == عدم) {
        החזר عدم;
    }

    אם (ccf->cpu_affinity_auto) {
        mask = &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

        עבור (i = 0, j = n; /* void */ ; i++) {

            אם (CPU_ISSET(i % CPU_SETSIZE, mask) && j-- == 0) {
                اكسر;
            }

            אם (i == CPU_SETSIZE && j == n) {
                /* empty mask */
                החזר عدم;
            }

            /* void */
        }

        CPU_ZERO(&result);
        CPU_SET(i % CPU_SETSIZE, &result);

        החזר &result;
    }

    אם (ccf->cpu_affinity_n > n) {
        החזר &ccf->cpu_affinity[n];
    }

    החזר &ccf->cpu_affinity[ccf->cpu_affinity_n - 1];

#אחרת

    החזר عدم;

#endif
}


ساكن حرف *
ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
    ngx_str_t        *value;
    ngx_core_conf_t  *ccf;

    ccf = (ngx_core_conf_t *) conf;

    אם (ccf->worker_processes != NGX_CONF_UNSET) {
        החזר "is duplicate";
    }

    value = cf->args->elts;

    אם (ngx_strcmp(value[1].data, "auto") == 0) {
        ccf->worker_processes = ngx_ncpu;
        החזר NGX_CONF_OK;
    }

    ccf->worker_processes = ngx_atoi(value[1].data, value[1].len);

    אם (ccf->worker_processes == NGX_ERROR) {
        החזר "invalid value";
    }

    החזר NGX_CONF_OK;
}


ساكن حرف *
ngx_load_module(ngx_conf_t *cf, ngx_command_t *cmd, فراغ *conf)
{
#אם (NGX_HAVE_DLOPEN)
    فراغ                *handle;
    حرف               **names, **order;
    ngx_str_t           *value, file;
    ngx_uint_t           i;
    ngx_module_t        *module, **modules;
    ngx_pool_cleanup_t  *cln;

    אם (cf->cycle->modules_used) {
        החזר "is specified too late";
    }

    value = cf->args->elts;

    file = value[1];

    אם (ngx_conf_full_name(cf->cycle, &file, 0) != NGX_OK) {
        החזר NGX_CONF_ERROR;
    }

    cln = ngx_pool_cleanup_add(cf->cycle->pool, 0);
    אם (cln == عدم) {
        החזר NGX_CONF_ERROR;
    }

    handle = ngx_dlopen(file.data);
    אם (handle == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlopen_n " \"%s\" failed (%s)",
                           file.data, ngx_dlerror());
        החזר NGX_CONF_ERROR;
    }

    cln->handler = ngx_unload_module;
    cln->data = handle;

    modules = ngx_dlsym(handle, "ngx_modules");
    אם (modules == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &value[1], "ngx_modules", ngx_dlerror());
        החזר NGX_CONF_ERROR;
    }

    names = ngx_dlsym(handle, "ngx_module_names");
    אם (names == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           ngx_dlsym_n " \"%V\", \"%s\" failed (%s)",
                           &value[1], "ngx_module_names", ngx_dlerror());
        החזר NGX_CONF_ERROR;
    }

    order = ngx_dlsym(handle, "ngx_module_order");

    עבור (i = 0; modules[i]; i++) {
        module = modules[i];
        module->name = names[i];

        אם (ngx_add_module(cf, &file, module, order) != NGX_OK) {
            החזר NGX_CONF_ERROR;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cf->log, 0, "module: %s i:%ui",
                       module->name, module->index);
    }

    החזר NGX_CONF_OK;

#אחרת

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "\"load_module\" is not supported "
                       "on this platform");
    החזר NGX_CONF_ERROR;

#endif
}


#אם (NGX_HAVE_DLOPEN)

ساكن فراغ
ngx_unload_module(فراغ *data)
{
    فراغ  *handle = data;

    אם (ngx_dlclose(handle) != 0) {
        ngx_log_error(NGX_LOG_ALERT, ngx_cycle->log, 0,
                      ngx_dlclose_n " failed (%s)", ngx_dlerror());
    }
}

#endif
